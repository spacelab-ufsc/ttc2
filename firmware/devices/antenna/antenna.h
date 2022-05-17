/*
 * antenna.h
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
 * \brief Antenna device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.5
 * 
 * \date 2019/11/01
 * 
 * \defgroup antenna Antenna
 * \ingroup devices
 * \{
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <stdint.h>

#include <drivers/isis_antenna/isis_antenna.h>

#include "antenna_data.h"

#define ANTENNA_MODULE_NAME         "Antenna"

/**
 * \brief Antenna initialization.
 *
 * \return The status/error code.
 */
int antenna_init(void);

/**
 * \brief Gets the antenna data (status and telemetry).
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \return The status/error code.
 */
int antenna_get_data(antenna_data_t *data);

/**
 * \brief Execute the antenna deployment.
 *
 * \param[in] timeout_ms is the deployment sequence timeout in milliseconds.
 *
 * \return The status/error code.
 */
int antenna_deploy(uint32_t timeout_ms);

#endif /* ANTENNA_H_ */

/** \} End of antenna group */
