/*
 * ttc_data.c
 * 
 * Copyright (C) 2021, SpaceLab.
 * 
 * This file is part of TTC 2.0.
 * 
 * TTC 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TTC 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief TTC data structure implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.23
 * 
 * \date 2021/04/14
 * 
 * \addtogroup ttc_data
 * \{
 */

#include "ttc_data.h"

ttc_data_t ttc_data_buf = {0};

void downlink_add_packet(uint8_t *packet, uint16_t packet_size)
{
    ttc_data_buf.down_buf.packet_sizes[ttc_data_buf.down_buf.position_to_write] = packet_size;

    for (uint16_t i = 0; i <ttc_data_buf.down_buf.packet_sizes[ttc_data_buf.down_buf.position_to_write]; i++)
    {
        ttc_data_buf.down_buf.packet_array[ttc_data_buf.down_buf.position_to_write][i] = packet[i];
    }

    ttc_data_buf.radio.tx_fifo_counter++;

    if (++ttc_data_buf.down_buf.position_to_write >= 5) ttc_data_buf.down_buf.position_to_write = 0;
}

void downlink_pop_packet(uint8_t *packet, uint16_t *packet_size)
{
    *packet_size = ttc_data_buf.down_buf.packet_sizes[ttc_data_buf.down_buf.position_to_read];

    for (uint16_t i = 0; i < ttc_data_buf.down_buf.packet_sizes[ttc_data_buf.down_buf.position_to_read]; i++)
    {
        packet[i] = ttc_data_buf.down_buf.packet_array[ttc_data_buf.down_buf.position_to_read][i];
    }

    ttc_data_buf.radio.tx_fifo_counter--;

    if (++ttc_data_buf.down_buf.position_to_read >= 5) ttc_data_buf.down_buf.position_to_read = 0;
}
/** \} End of ttc_data group */
