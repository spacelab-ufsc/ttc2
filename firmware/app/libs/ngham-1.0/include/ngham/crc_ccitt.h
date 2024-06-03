/*
 * crc_ccitt.h
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
 * along with NGHam. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief CRC-CCITT definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.2
 * 
 * \date 2023/03/12
 * 
 * \defgroup crc-ccitt CRC-CCITT
 * \ingroup ngham
 * \{
 */

#ifndef CRC_CCITT_H_
#define CRC_CCITT_H_

#include <stdint.h>

/**
 * \brief Computes the CRC16-CCITT value of a single byte.
 *
 * \param[in] buf is the byte to compute the CRC.
 *
 * \param[in] crc is the value.
 *
 * \return The computed CRC16-CCITT value.
 */
uint16_t crc_ccitt_byte(uint8_t buf, uint16_t crc);

/**
 * \brief Computes the CRC16-CCITT value of a sequence of bytes.
 *
 * \param[in] buf is the array of bytes to compute the CRC.
 *
 * \param[in] buf_len is the number of bytes of the given array.
 *
 * \return The computed CRC16-CCITT value.
 */
uint16_t crc_ccitt(uint8_t *buf, uint16_t buf_len);

#endif /* CRC_CCITT_H_ */

/**< \} End of crc-ccitt group */
