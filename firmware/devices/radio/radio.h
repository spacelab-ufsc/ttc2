/*
 * radio.h
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
 * \brief Radio device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 * 
 * \defgroup radio Radio
 * \ingroup devices
 * \{
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>

#define RADIO_MODULE_NAME           "Radio"

/**
 * \brief Temperature type.
 */
typedef uint16_t radio_temp_t;

/**
 * \brief RSSI type.
 */
typedef uint16_t radio_rssi_t;

/**
 * \brief Radio device initialization routine.
 *
 * \return The status/error code.
 */
int radio_init(void);

/**
 * \brief Writes data to the radio device.
 *
 * \param[in] data is an array of bytes.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int radio_send(uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the radio device (if available) with timeout.
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read from the radio.
 *
 * \param[in] timeout_ms is the timeout to received a packet in milliseconds.
 *
 * \return The status/error code.
 */
int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms);

/**
 * \brief Verifies if there are unread received bytes.
 *
 * \return The number of available bytes to receive (-1 on error).
 */
int radio_available(void);

/**
 * \brief Puts the radio in sleep mode.
 *
 * \return The status/error code.
 */
int radio_sleep(void);

/**
 * \brief Reads the current temperature of the radio device.
 *
 * \param[in,out] temp is a pointer to store the read temperature.
 *
 * \return The status/error code.
 */
int radio_get_temperature(radio_temp_t *temp);

/**
 * \brief Reads the last available RSSI value of the radio device.
 *
 * \param[in,out] rssi is a pointer to store the read RSSI value.
 *
 * \return The status/error code.
 */
int radio_get_rssi(radio_rssi_t *rssi);

#endif /* RADIO_H_ */

/** \} End of radio group */
