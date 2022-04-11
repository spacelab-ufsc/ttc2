/* 
 * buffer.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Basic buffer.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/11/2017
 * 
 * \defgroup buffer Buffer
 * \ingroup system
 * \{
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_LENGTH           128

#define BUFFER_DEFAULT_BYTE     0xFF

/**
 * \brief Buffer implementation as a struct.
 */
typedef struct
{
    uint8_t data[BUFFER_LENGTH];    /**< Data of the buffer. */
    uint8_t size;                   /**< Number of elements into the buffer. */
} Buffer;

/**
 * \brief Buffer initialization.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return None.
 */
void buffer_init(Buffer *buffer);

/**
 * \brief Returns the length (capacity) of a buffer.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return The length of the buffer (or capacity).
 */
uint8_t buffer_length(Buffer *buffer);

/**
 * \brief Fills the buffer with data.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * \param data is a pointer to an array with the data to store in the buffer.
 * \param len is the length of data.
 * 
 * \return True/False if the length of the data fits into the buffer.
 */
bool buffer_fill(Buffer *buffer, uint8_t *data, uint8_t len);

/**
 * \brief Append data to a buffer.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * \param data is a pointer to an array with the data to append in the buffer.
 * \param len is the length of data.
 * 
 * \return True/False if the length of the data fits into the buffer.
 */
bool buffer_append(Buffer *buffer, uint8_t *data, uint8_t len);

/**
 * \brief Clear a buffer filling it with the dafult byte (\b BUFFER_DEFAULT_BYTE).
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return None
 */
void buffer_clear(Buffer *buffer);

/**
 * \brief Verifies if the a buffer is empty or not.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return True/False if the buffer is empty or not.
 */
bool buffer_empty(Buffer *buffer);

/**
 * \brief Verifies if the a buffer is full or not.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return True/False if the buffer is full or not.
 */
bool buffer_full(Buffer *buffer);

/**
 * \brief Returns the size of a buffer.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return The size of the buffer.
 */
uint8_t buffer_size(Buffer *buffer);

#endif // BUFFER_H_

//! \} End of buffer group
