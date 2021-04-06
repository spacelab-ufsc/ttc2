/*
 * media.h
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Media device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.11
 * 
 * \date 21/04/2020
 * 
 * \defgroup media Media
 * \ingroup devices
 * \{
 */

#ifndef MEDIA_H_
#define MEDIA_H_

#include <stdint.h>

#define MEDIA_MODULE_NAME           "Media"

/**
 * \brief Media types.
 */
typedef enum
{
    MEDIA_INT_FLASH=0,      /**< Internal flash memory. */
    MEDIA_NOR               /**< NOR flash memory. */
} media_types_e;

/**
 * \brief Media type.
 */
typedef uint8_t media_t;

/**
 * \brief Media initialization.
 *
 * \param[in] med is the storage media to initiailize. It can be:
 * \parblock
 *      -\b MEDIA_INT_FLASH
 *      -\b MEDIA_NOR
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int media_init(media_t med);

/**
 * \brief Writes data into a given address of a media device.
 *
 * \param[in] med is the storage media to write. It can be:
 * \parblock
 *      -\b MEDIA_INT_FLASH
 *      -\b MEDIA_NOR
 *      .
 * \endparblock
 *
 * \param[in] adr is the address to write data.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int media_write(media_t med, uint32_t adr, uint32_t *data, uint16_t len);

/**
 * \brief Reads data from a given address of a media device.
 *
 * \param[in] med is the storage media to read. It can be:
 * \parblock
 *      -\b MEDIA_INT_FLASH
 *      -\b MEDIA_NOR
 *      .
 * \endparblock
 *
 * \param[in] adr is the address to read.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read starting at addr.
 *
 * \return The status/error code.
 */
int media_read(media_t med, uint32_t adr, uint32_t *data, uint16_t len);

/**
 * \brief Erases a memory region from a media device.
 *
 * \param[in] med is the storage media to erase. It can be:
 * \parblock
 *      -\b MEDIA_INT_FLASH
 *      -\b MEDIA_NOR
 *      .
 * \endparblock
 *
 * \param[in] adr is the address to erase.
 *
 * \return The status/error code.
 */
int media_erase(media_t med, uint32_t adr);

#endif /* MEDIA_H_ */

/** \} End of media group */
