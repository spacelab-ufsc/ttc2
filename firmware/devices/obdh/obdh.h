/*
 * obdh.h
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
 * \brief OBDH device definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.3
 *
 * \date 2023/02/12
 *
 * \defgroup obdh OBDH
 * \ingroup devices
 * \{
 */

#ifndef DEVICES_OBDH_H_
#define DEVICES_OBDH_H_

typedef cmdpr_package_t obdh_request_t;
typedef cmdpr_package_t obdh_response_t;

/**
 * \brief OBDH initialization.
 *
 * \return The status/error code.
 */
int obdh_init(void);

/**
 * \brief OBDH read request from SPI RX buffer.
 *
 * \param[in] *obdh_request is the structure to store the received information.
 *
 * \return The status/error code.
 */
int obdh_read_request(obdh_request_t *obdh_request);

/**
 * \brief Sends response to OBDH.
 *
 * \param[in] *obdh_response is the structure for a response to OBDH.
 *
 * \return The status/error code.
 */
int obdh_send_response(obdh_response_t obdh_response);

#endif /* DEVICES_OBDH_H_ */
