/*
 * radio_data.h
 *
 * Copyright The TTC 2.0 Contributors.
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
 * \brief Radio data structure definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.1
 *
 * \date 2020/07/24
 *
 * \defgroup radio_data Data
 * \ingroup radio
 * \{
 */

#ifndef RADIO_DATA_H_
#define RADIO_DATA_H_

#include <stdint.h>

/**
 * \brief Radio data type.
 */
typedef struct
{
    uint16_t temperature;           /**< Temperature in Kelvin. */
    uint16_t voltage;               /**< Input voltage in mV. */
    uint16_t current;               /**< Input current in mA. */
    uint16_t power;                 /**< Input power in mW. */
    uint8_t last_valid_tm;          /**< Last valid command (uplink packet ID). */
    uint16_t rssi;                  /**< RSSI of the last valid command. */
    uint8_t tx_enable;              /**< TX Enable 0=off, 1=on TODO */
    uint32_t rx_packet_counter;     /**< Number of total packets received */
    uint32_t tx_packet_counter;     /**< Number of total packets sent */
    uint8_t rx_fifo_counter;        /**< Packets received in queue */
    uint8_t tx_fifo_counter;        /**< Packets in queue to be sent */
    uint16_t last_rx_packet_bytes;  /**< Number of available bytes in the last packet received */
} radio_data_t;

#endif /* RADIO_DATA_H_ */

/** \} End of radio_data group */
