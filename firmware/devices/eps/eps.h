/*
 * eps.h
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
 * \brief EPS device definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.5
 *
 * \date 2023/02/25
 *
 * \defgroup eps EPS
 * \ingroup devices
 * \{
 */

#ifndef DEVICES_EPS_H_
#define DEVICES_EPS_H_

#include <system/cmdpr.h>

typedef cmdpr_package_t eps_request_t;

#define EPS_MODULE_NAME         "EPS"

/**
 * \brief EPS initialization.
 *
 * \return The status/error code.
 */
int eps_init(void);

/**
 * \brief EPS read request from UART RX buffer.
 *
 * \param[in] *eps_request is the structure to store the received information.
 *
 * \return The status/error code.
 */
int eps_read_request(eps_request_t *eps_request);

/**
 * \brief EPS flush UART RX buffer.
 *
 * \param[in] eps_request is the structure to store the received information.
 *
 * \return The status/error code.
 */
int eps_flush_request(eps_request_t *eps_request);

#endif /* DEVICES_EPS_H_ */

/** \} End of eps group */
