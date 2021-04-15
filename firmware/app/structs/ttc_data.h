/*
 * ttc_data.h
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
 * \brief TTC data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.23
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

/**
 * \brief Radio data.
 */
typedef struct
{
    uint16_t temperature;           /**< Temperature in Kelvin. */
    uint16_t voltage;               /**< Input voltage in mV. */
    uint16_t current;               /**< Input current in mA. */
    uint16_t power;                 /**< Input power in mW. */
    uint8_t last_valid_tm;          /**< Last valid command (uplink packet ID). */
    uint16_t rssi;                  /**< RSSI of the last valid command. */
} radio_data_t;

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
    radio_data_t radio;             /**< Radio data. */
    uint8_t hw_version;             /**< Hardware version. */
    uint32_t fw_version;            /**< Firmware version ("v1.2.3" = 0x00010203). */
} ttc_data_t;

/**
 * \brief TTC data buffer.
 */
extern ttc_data_t ttc_data_buf;

#endif /* TTC_DATA_H_ */

/** \} End of ttc_data group */
