/*
 * tps382x_wrap.c
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
 * \brief TPS382x driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.6
 * 
 * \date 2021/02/17
 * 
 * \addtogroup tps382x_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "tps382x_wrap.h"

int __wrap_tps382x_init(tps382x_config_t config)
{
    check_expected(config.wdi_pin);
    check_expected(config.mr_pin);

    return mock_type(int);
}

int __wrap_tps382x_trigger(tps382x_config_t config)
{
    check_expected(config.wdi_pin);
    check_expected(config.mr_pin);

    return mock_type(int);
}

int __wrap_tps382x_manual_reset(tps382x_config_t config)
{
    check_expected(config.wdi_pin);
    check_expected(config.mr_pin);

    return mock_type(int);
}

/** \} End of tps382x_wrap group */
