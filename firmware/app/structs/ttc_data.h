/*
 * ttc_data.h
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
 * \brief TTC data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.1
 * 
 * \date 2021/04/14
 * 
 * \defgroup ttc_data TTC Data
 * \ingroup structs
 * \{
 */

#ifndef TTC_DATA_H_
#define TTC_DATA_H_

#include <stdint.h>

#include <devices/antenna/antenna_data.h>
#include <devices/radio/radio_data.h>

typedef struct
{
    uint8_t packet_array[5][300];
    uint16_t packet_sizes[5];
    uint8_t position_to_write;
    uint8_t position_to_read;
} transmission_buf_t;

/**
 * \brief TTC data.
 */
typedef struct
{
    uint32_t timestamp;             /**< Data timestamp (system ticks). */
    uint16_t temperature;           /**< uC temperature in Kelvin. */
    uint16_t voltage;               /**< Input voltage in mV. */
    uint16_t current;               /**< Input current in mA. */
    uint16_t power;                 /**< Input power in mW. */
    uint8_t last_reset_cause;       /**< Last uC reset cause code. */
    uint16_t reset_counter;         /**< uC reset counter. */
    uint8_t hw_version;             /**< Hardware version. */
    uint32_t fw_version;            /**< Firmware version ("v1.2.3" = 0x00010203). */
    uint16_t device_id;             /**< Device ID (can be 0xCC2A or 0xCC2B) TODO: */
    uint8_t ant_deploy;             /**< Number of antenna deployment tries TODO: */
    uint8_t ant_deploy_hib;         /**< Hibernation time completed */
    radio_data_t radio;             /**< Radio data. */
    antenna_data_t antenna;         /**< Antenna data. */
    transmission_buf_t down_buf;    /**< Downlink Buffer */
    transmission_buf_t up_buf;      /**< Uplink Buffer */
} ttc_data_t;

/**
 * \brief TTC data buffer.
 */
extern ttc_data_t ttc_data_buf;

/**
 * \brief Add a packet to the TX queue.
 *
 * \param[in] packet to be sent.
 *
 * \param[in] packet_size is the size of the packet
 *
 * \return None.
 */
void downlink_add_packet(uint8_t *packet, uint16_t packet_size);

/**
 * \brief Returns the next packet in queue to be sent.
 *
 * \param[in] packet to be sent.
 *
 * \param[in] packet_size is the size of the packet.
 *
 * \return None.
 */
void downlink_pop_packet(uint8_t *packet, uint16_t *packet_size);

#endif /* TTC_DATA_H_ */

/** \} End of ttc_data group */
