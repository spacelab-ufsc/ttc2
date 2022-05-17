/*
 * fsat_pkt.c
 * 
 * Copyright The TTC 2.0 Contributors.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief FloripaSat packet implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.22
 * 
 * \date 2020/03/14
 * 
 * \addtogroup fsat_pkt
 * \{
 */

#include <string.h>

#include "fsat_pkt.h"

void fsat_pkt_add_id(fsat_pkt_pl_t *pkt, uint8_t id)
{
    pkt->id = id;
}

void fsat_pkt_add_callsign(fsat_pkt_pl_t *pkt, const char *callsign)
{
    unsigned int cs_len = strlen(callsign);

    if (cs_len <= 7U)
    {
        int dummy = strncpy(&pkt->callsign[0], callsign, cs_len);
    }
}

void fsat_pkt_add_payload(fsat_pkt_pl_t *pkt, uint8_t *pl, uint16_t len)
{
    int dummy = memcpy(&pkt->payload[0], pl, len);

    pkt->length = len;
}

void fsat_pkt_encode(fsat_pkt_pl_t pkt, uint8_t *pl, uint16_t *len)
{
    /* Packet ID */
    pl[0] = pkt.id;

    /* Callsign */
    uint8_t cs_len = 0U;
    for(cs_len = 0U; cs_len < 7U; cs_len++)
    {
        if (pkt.callsign[cs_len] == 0)  /* 0 = '\0' */
        {
            break;
        }
    }

    uint8_t i = 0U;
    for(i = 0U; i < (7U - cs_len); i++)
    {
        pl[1U + i] = FSAT_PKT_CALLSIGN_PADDING_CHAR;
    }

    int dummy = memcpy(&pl[1U + i], pkt.callsign, 7U - i);

    /* Packet data */
    dummy = memcpy(&pl[1U + 7U], pkt.payload, pkt.length);

    *len = 1U + 7U + pkt.length;
}

void fsat_pkt_decode(uint8_t *raw_pkt, uint16_t len, fsat_pkt_pl_t *pkt)
{
    /* Copy packet ID */
    pkt->id = raw_pkt[0];

    /* Copy callsign */
    uint8_t i = 0U;
    for(i = 0U; i < 7U; i++)
    {
        if ((raw_pkt[i + 1U] >= 0x41U) && (raw_pkt[i + 1U] <= 0x5AU))   /* 0x41 = 'A', 0x5A = 'Z' */
        {
            break;
        }
    }

    int dummy = memcpy(pkt->callsign, &raw_pkt[1U + i], 7U - i);

    pkt->callsign[7U - i] = '\0';

    /* Copy payload data */
    dummy = memcpy(&pkt->payload[0], &raw_pkt[1U + 7U], len - 7U - 1U);

    /* Payload data length */
    pkt->length = len - 7U - 1U;
}

/** \} End of fsat_pkt group */
