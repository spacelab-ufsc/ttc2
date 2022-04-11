/* 
 * queue.h
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
 * \brief Basic queue.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/11/2017
 * 
 * \defgroup queue Queue
 * \ingroup system
 * \{
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

#define QUEUE_LENGTH        128     /**< Queue length in bytes. */

#define QUEUE_DEFAULT_BYTE  0xFF    /**< Queue default byte (empty position). */

/**
 * \brief Queue representation as a struct.
 */
typedef struct
{
    uint8_t data[QUEUE_LENGTH];     /**< Data buffer. */
    uint8_t head;                   /**< Head position of the data array. */
    uint8_t tail;                   /**< Tail position of the data array. */
    uint8_t size;                   /**< Length, in bytes, of the data. */
} Queue;

/**
 * \brief Queue initialization.
 * 
 * \param queue is a pointer to a Queue struct.
 * 
 * \return None.
 */
void queue_init(Queue *queue);

/**
 * \brief Returns the length (capacity) of a queue.
 * 
 * \param queue is a pointer to a Queue struct.
 * 
 * \return The length of the queue (or capacity).
 */
uint8_t queue_length(Queue *queue);

/**
 * \brief Puts an element into the back position of an queue.
 * 
 * \param queue is a pointer to a Queue struct.
 * \param byte is the byte to be pushed to the queue.
 * 
 * \return True/False if the element was pushed or not.
 */
bool queue_push_back(Queue *queue, uint8_t byte);

/**
 * \brief Grabs an element from the front position of an queue.
 * 
 * \param queue is a pointer to a Queue struct.
 * 
 * \return The byte grabbed from the queue.
 */
uint8_t queue_pop_front(Queue *queue);

/**
 * \brief Verifies if the a queue is empty or not.
 * 
 * \param queue is a pointer to a Queue struct.
 * 
 * \return True/False if the queue is empty or not.
 */
bool queue_empty(Queue *queue);

/**
 * \brief Verifies if the a queue is full or not.
 * 
 * \param queue is a pointer to a Queue struct.
 * 
 * \return True/False if the queue is full or not.
 */
bool queue_full(Queue *queue);

/**
 * \brief Returns the size of a queue.
 * 
 * \param queue is a pointer to a Queue struct.
 * 
 * \return The size of the queue.
 */
uint8_t queue_size(Queue *queue);

#endif // QUEUE_H_

//! \} End of queue group
