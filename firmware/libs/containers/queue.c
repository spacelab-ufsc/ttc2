/* 
 * queue.c
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
 * \brief Basic queue manipulation functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/11/2017
 * 
 * \addtogroup queue
 * \{
 */

#include "queue.h"

void queue_init(Queue *queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    
    uint8_t i = 0;
    for(i=0; i<QUEUE_LENGTH; i++)
    {
        queue->data[i] = QUEUE_DEFAULT_BYTE;
    }
}

uint8_t queue_length(Queue *queue)
{
    return QUEUE_LENGTH;
}

bool queue_push_back(Queue *queue, uint8_t byte)
{
    if (queue_full(queue))
    {
        return false;
    }
    else
    {
        queue->data[queue->tail++] = byte;
        
        if (queue->tail == queue_length(queue))
        {
            queue->tail = 0;
        }
        
        return true;
    }
}

uint8_t queue_pop_front(Queue *queue)
{
    if (queue_empty(queue))
    {
        return QUEUE_DEFAULT_BYTE;
    }
    else
    {
        uint8_t byte = queue->data[queue->head++];
        
        if (queue->head == queue_length(queue))
        {
            queue->head = 0;
        }
        
        return byte;
    }
}

bool queue_empty(Queue *queue)
{
    if (queue->head == queue->tail)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool queue_full(Queue *queue)
{
    if (((queue->tail + 1) == queue->head) || (((queue->tail + 1) == queue_length(queue)) && (queue->head == 0)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t queue_size(Queue *queue)
{
    return queue->tail - queue->head;
}

//! \} End of queue group
