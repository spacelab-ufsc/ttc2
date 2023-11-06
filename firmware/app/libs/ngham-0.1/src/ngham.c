/*
 * ngham.c
 * 
 * Copyright The NGHam Contributors.
 * 
 * This file is part of NGHam.
 * 
 * NGHam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * NGHam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with NGHam. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief NGHam library implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2023/03/12
 * 
 * \addtogroup ngham
 * \{
 */

#include <stddef.h>                 /* For NULL etc. */
#include <string.h>                 /* For memcpy */
#include <stdlib.h>                 /* For free */

#include <rsc/rsc.h>

#include <ngham/ngham.h>
#include <ngham/config.h>

#include <ngham/ccsds_scrambler.h>  /* Pre-generated array from scrambling polynomial */
#include <ngham/crc_ccitt.h>

/* There are seven different sizes. */
/* Each size has a correlation tag for size, a total size, a maximum payload size and a parity data size. */
#define NGH_SIZES                   7

/* Decoder states */
#define NGH_STATE_SIZE_TAG          0
#define NGH_STATE_SIZE_TAG_2        1
#define NGH_STATE_SIZE_TAG_3        2
#define NGH_STATE_SIZE_KNOWN        3
#define NGH_STATE_STATUS            4
#define NGH_STATE_STATUS_2          5

/* Maximum number of errors in the size tag */
#define NGH_SIZE_TAG_MAX_ERROR      6

uint8_t NGH_PL_SIZE[7]      = {28U, 60U, 92U,  124U, 156U, 188U, 220U}; /* Actual payload */
uint8_t NGH_PL_SIZE_FULL[7] = {31U, 63U, 95U,  127U, 159U, 191U, 223U}; /* Size with LEN, payload and CRC */
uint8_t NGH_PL_PAR_SIZE[7]  = {47U, 79U, 111U, 159U, 191U, 223U, 255U}; /* Size with RS parity added */
uint8_t NGH_PAR_SIZE[7]     = {16U, 16U, 16U,  32U,  32U,  32U,  32U};

/* The seven different size tag vectors */
uint32_t NGH_SIZE_TAG[7] = {
    0b001110110100100111001101,
    0b010011011101101001010111,
    0b011101101001001110011010,
    0b100110111011010010101110,
    0b101000001111110101100011,
    0b110101100110111011111001,
    0b111011010010011100110100
};

/* Preamble and synchronization vector */
uint8_t NGH_PREAMBLE            = 0xAAU;
uint8_t NGH_PREAMBLE_FOUR_LEVEL = 0xDDU;
uint8_t NGH_SYNC[4]             = {0x5DU, 0xE6U, 0x2AU, 0x7EU};
uint8_t NGH_SYNC_FOUR_LEVEL[8]  = {0x77U, 0xF7U, 0xFDU, 0x7DU, 0x5DU, 0xDDU, 0x7FU, 0xFDU};

/* Reed Solomon control blocks for the different NGHAM sizes */
reed_solomon_t rs[NGH_SIZES] = {0};

int decoder_state = NGH_STATE_SIZE_TAG;

/**
 * \brief Generates the Reed-Solomon tables for all packets sizes.
 *
 * Run only once - generates Reed Solomon tables for all 7 packet sizes.
 *
 * MM=8, genpoly=0x187, fcs=112, prim=11, nroots=32 or 16
 *
 * \return The status/error code.
 */
int ngham_init_arrays(void);

/**
 * \brief Used to check if hamming distance in size tag is smaller than treshold.
 *
 * \param[in] x .
 *
 * \param[in] y .
 *
 * \return .
 */
uint8_t ngham_tag_check(uint32_t x, uint32_t y);

int ngham_init(void)
{
    decoder_state = NGH_STATE_SIZE_TAG;

    return ngham_init_arrays();
}

int ngham_encode(uint8_t *data, uint16_t len, uint8_t flags, uint8_t *pkt, uint16_t *pkt_len)
{
    uint16_t j = 0U;
    uint16_t crc = 0U;
    uint8_t size_nr = 0U;
    uint16_t d_len = 0U;
    uint8_t codeword_start = 0U;

	/* Check size and find control block for smallest possible RS codeword */
	if ((len == 0) || (len > NGH_PL_SIZE[NGH_SIZES - 1]))
    {
        return -1;
    }

	while(len > NGH_PL_SIZE[size_nr])
    {
        size_nr++;
    }

	/* Insert preamble, sync and size-tag */
    if (NGHAM_FOUR_LEVEL_MODULATION)
    {
        codeword_start = NGH_PREAMBLE_SIZE_FOUR_LEVEL + NGH_SYNC_SIZE_FOUR_LEVEL + NGH_SIZE_TAG_SIZE;

        for(j = 0; j < NGH_PREAMBLE_SIZE_FOUR_LEVEL; j++)
        {
            pkt[d_len++] = NGH_PREAMBLE_FOUR_LEVEL;
        }

        for(j = 0; j < NGH_SYNC_SIZE_FOUR_LEVEL; j++)
        {
            pkt[d_len++] = NGH_SYNC_FOUR_LEVEL[j];
        }
    }
    else
    {
        codeword_start = NGH_PREAMBLE_SIZE + NGH_SYNC_SIZE + NGH_SIZE_TAG_SIZE;

        for(j = 0U; j < NGH_PREAMBLE_SIZE; j++)
        {
            pkt[d_len++] = NGH_PREAMBLE;
        }

        for(j = 0U; j < NGH_SYNC_SIZE; j++)
        {
            pkt[d_len++] = NGH_SYNC[j];
        }
    }

    pkt[d_len++] = (NGH_SIZE_TAG[size_nr] >> 16) & 0xFFU;
    pkt[d_len++] = (NGH_SIZE_TAG[size_nr] >> 8) & 0xFFU;
    pkt[d_len++] = NGH_SIZE_TAG[size_nr] & 0xFFU;

    /* Prepare content of codeword */
    pkt[d_len] = (NGH_PL_SIZE[size_nr] - len) & 0x1FU;      /* Insert padding size */
    pkt[d_len] |= (flags << 5) & 0xE0U;                     /* Insert flags */
    d_len++;
    for(j = 0; j < len; j++)
    {
        pkt[d_len++] = data[j];                             /* Insert data */
    }
    crc = crc_ccitt(&pkt[codeword_start], len + 1);         /* Insert CRC */
    pkt[d_len++] = (crc >> 8) & 0xFFU;
    pkt[d_len++] = crc & 0xFFU;
    for(j = len + 3; j < NGH_PL_SIZE_FULL[size_nr]; j++)
    {
        pkt[d_len++] = 0;                                   /* Insert padding */
    }

    /* Generate parity data */
    uint8_t par_len = UINT8_MAX;
    rsc_encode(&rs[size_nr], &pkt[codeword_start], &pkt[d_len], &par_len);
    d_len += NGH_PAR_SIZE[size_nr];

    /* Scramble */
    for(j = 0; j < NGH_PL_PAR_SIZE[size_nr]; j++)
    {
        pkt[codeword_start + j] ^= ccsds_poly[j];
    }

    *pkt_len = d_len;

    return 0;
}

void ngham_decode(uint8_t d)
{
//    static uint8_t size_nr;
//    static uint32_t size_tag;
//    static unsigned int length;
//    /* This points to the address one lower than the payload! */
//    static uint8_t* buf = (uint8_t*)&rx_pkt.ngham_flags;
//
//    switch(decoder_state)
//    {
//        case NGH_STATE_SIZE_TAG:
//            size_tag = 0;
//            ngham_action_reception_started();
//        case NGH_STATE_SIZE_TAG_2:
//            size_tag <<= 8;
//            size_tag |= d;
//            decoder_state++;
//            break;
//        case NGH_STATE_SIZE_TAG_3:
//            size_tag <<= 8;
//            size_tag |= d;
//            {
//                for (size_nr = 0; size_nr < NGH_SIZES; size_nr++)
//                {
//                    /* If tag is intact, set known size */
//                    if (ngham_tag_check(size_tag, NGH_SIZE_TAG[size_nr]))
//                    {
//                        decoder_state = NGH_STATE_SIZE_KNOWN;
//                        length = 0;
//
//                        /* Set new packet size as soon as possible */
////                        ngham_action_set_packet_size(NGH_PL_PAR_SIZE[size_nr] + NGH_SIZE_TAG_SIZE);
//                        break;
//                    }
//                }
//                /* If size tag is not found, every size can theoretically be attempted */
//                if (decoder_state != NGH_STATE_SIZE_KNOWN)
//                {
////                    ngham_action_handle_packet(PKT_CONDITION_PREFAIL, NULL);
//                    decoder_state = NGH_STATE_SIZE_TAG;
//                }
//            }
//            break;
//        case NGH_STATE_SIZE_KNOWN:
//            /* De-scramble byte and append to buffer */
//            buf[length] = d^ccsds_poly[length];
//            length++;
//
//			/* Do whatever is necessary in this action */
//            if (length == NGHAM_BYTES_TILL_ACTION_HALFWAY)
//            {
//                ngham_action_reception_halfway();
//            }
//
//            if (length == NGH_PL_PAR_SIZE[size_nr])
//            {
//                int8_t errors;
//
//                /* Set packet size back to a large value */
////                ngham_action_set_packet_size(255);
//                decoder_state = NGH_STATE_SIZE_TAG;
//
//                /* Run Reed Solomon decoding, calculate packet length */
//                errors = decode_rs_char(&rs_cb[size_nr], buf, 0, 0);
//                rx_pkt.pl_len = NGH_PL_SIZE[size_nr] - (buf[0] & NGH_PADDING_bm);
//
//                /* Check if the packet is decodeable and then if CRC is OK */
//                if ((errors != -1) &&
//                    (crc_ccitt(buf, rx_pkt.pl_len + 1) == ((buf[rx_pkt.pl_len + 1] << 8) | buf[rx_pkt.pl_len + 2])) )
//                {
//                    /* Copy remaining fields and pass on */
//                    rx_pkt.errors = errors;
//                    rx_pkt.ngham_flags = (buf[0] & NGH_FLAGS_bm) >> NGH_FLAGS_bp;
//                    rx_pkt.noise = ngham_action_get_noise_floor();
//                    rx_pkt.rssi = ngham_action_get_rssi();
////                    ngham_action_handle_packet(PKT_CONDITION_OK, &rx_pkt);
//                }
//                /* If packet decoding not was successful, count this as an error */
//                else
//                {
////                    ngham_action_handle_packet(PKT_CONDITION_FAIL, NULL);
//                }
//            }
//            break;
//    }
}

int ngham_init_arrays(void)
{
    int err = 0;
    int j = 0;

    for(j = 0; j < 3; j++)
    {
        if (rsc_init(8, 0x187, 112, 11, 16, (NGH_PL_PAR_SIZE[6] - NGH_PL_PAR_SIZE[j]), &rs[j]) != 0)
        {
            err = -1;
        }
    }

    for(; j < NGH_SIZES; j++)
    {
        if (rsc_init(8, 0x187, 112, 11, 32, (NGH_PL_PAR_SIZE[6] - NGH_PL_PAR_SIZE[j]), &rs[j]) != 0)
        {
            err = -1;
        }
    }

    return err;
}

uint8_t ngham_tag_check(uint32_t x, uint32_t y)
{
    uint8_t j = 0U;
    uint8_t distance = 0U;
    uint32_t diff = x ^ y;
    uint8_t res = 0;

	if (diff != 0U)
    {
        for(j = 0U; j < 24U; j++)
        {
            if (diff & 0x01U)
            {
                distance++;
                if (distance > NGH_SIZE_TAG_MAX_ERROR)
                {
                    res = 0;
                    break;
                }
            }

            diff >>= 1;
        }
    }
    else
    {
        res = 1;
    }

    return res;
}

/**< \} End of ngham group */
