/*
 * ngham.h
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
 * \brief NGHam library definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2023/03/12
 * 
 * \defgroup ngham NGHam
 * \{
 */

#ifndef NGHAM_H_
#define NGHAM_H_

#include <stdint.h>

#include "ngham_packets.h"

#define NGHAM_VERSION                   "v0.1.0"

#define NGH_PREAMBLE_SIZE               4U
#define NGH_SYNC_SIZE                   4U
#define NGH_PREAMBLE_SIZE_FOUR_LEVEL    8U
#define NGH_SYNC_SIZE_FOUR_LEVEL        8U
#define NGH_SIZE_TAG_SIZE               3U
#define NGH_MAX_CODEWORD_SIZE           255
#define NGH_MAX_TOT_SIZE                (NGH_PREAMBLE_SIZE_FOUR_LEVEL+NGH_SYNC_SIZE_FOUR_LEVEL+NGH_SIZE_TAG_SIZE+NGH_MAX_CODEWORD_SIZE)

#define NGH_PADDING_bm                  0x1FU
#define NGH_FLAGS_bm                    0xE0U
#define NGH_FLAGS_bp                    5U

/**
 * \brief NGHam protocol initialization.
 *
 * \return The status/error code.
 */
int ngham_init(void);

/**
 * \brief Encodes a given data into a NGHam packet.
 *
 * Packets to be transmitted are passed to this function - max. length 220B.
 *
 * \param[in] data is the array of bytes to be encoded.
 *
 * \param[in] len is number of bytes of the given data.
 *
 * \param[in] flags is bytes with the NGHam packets flags.
 *
 * \param[in,out] pkt is a pointer to store the encoded packet.
 *
 * \param[in,out] pkt_len is the number of bytes of the encoded packet.
 *
 * \return The status/error code.
 */
int ngham_encode(uint8_t *data, uint16_t len, uint8_t flags, uint8_t *pkt, uint16_t *pkt_len);

/**
 * \brief Decodes a given NGHam packet to data.
 *
 * \param[in] d .
 *
 * \return None.
 */
void ngham_decode(uint8_t d);

#endif /* NGHAM_H_ */

/**< \} End of ngham group */
