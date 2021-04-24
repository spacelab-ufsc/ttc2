/*
 * clocks.h
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
 * \brief System clocks functions definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.3
 * 
 * \date 2020/01/25
 * 
 * \defgroup clocks Clocks
 * \ingroup system
 * \{
 */

#ifndef CLOCKS_H_
#define CLOCKS_H_

#include <stdint.h>

/**
 * \brief Clocks configuration.
 */
typedef struct
{
    uint32_t mclk_hz;       /**< MCLK frequency in Hz. */
    uint32_t smclk_hz;      /**< SMCLK frequency in Hz. */
    uint32_t aclk_hz;       /**< ACLK frequency in Hz. */
} clocks_config_t;

/**
 * \brief System clocks setup.
 *
 * \see SLAU388F - Power Management Module and Supply Voltage Supervisor
 *
 * \return The status/error code.
 */
int clocks_setup(clocks_config_t clks);

/**
 * \brief Reads the current system clocks.
 *
 * \return A clocks_config_t struct with the system clocks.
 */
clocks_config_t clocks_read(void);

#endif /* CLOCKS_H_ */

/** \} End of clocks group */
