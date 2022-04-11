/* 
 * buffer.c
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
 * \brief Basic buffer manipulation functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/11/2017
 * 
 * \addtogroup buffer
 * \{
 */

#include "buffer.h"

void buffer_init(Buffer *buffer)
{
    buffer_clear(buffer);
}

uint8_t buffer_length(Buffer *buffer)
{
    return BUFFER_LENGTH;
}

bool buffer_fill(Buffer *buffer, uint8_t *data, uint8_t len)
{
    if (len <= buffer_length(buffer))
    {
        buffer_clear(buffer);
        
        uint8_t i = 0;
        for(i=0; i<len; i++)
        {
            buffer->data[i] = data[i];
        }
        
        buffer->size = len;
        
        return true;
    }
    else
    {
        return false;
    }
}

bool buffer_append(Buffer *buffer, uint8_t *data, uint8_t len)
{
    if ((buffer->size + len) <= BUFFER_LENGTH)
    {
        uint8_t i = 0;
        for(i=0; i<len; i++)
        {
            buffer->data[buffer->size + i] = data[i];
        }
        
        buffer->size += len;
        
        return true;
    }
    else
    {
        return false;
    }
}

void buffer_clear(Buffer *buffer)
{
    uint8_t i = 0;
    for(i=0; i<BUFFER_LENGTH; i++)
    {
        buffer->data[i] = BUFFER_DEFAULT_BYTE;
    }
    
    buffer->size = 0;
}

bool buffer_empty(Buffer *buffer)
{
    if (buffer_size(buffer) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool buffer_full(Buffer *buffer)
{
    if (buffer_size(buffer) == BUFFER_LENGTH)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t buffer_size(Buffer *buffer)
{
    return buffer->size;
}

//! \} End of buffer group
