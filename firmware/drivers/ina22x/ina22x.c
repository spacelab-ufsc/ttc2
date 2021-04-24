/*
 * ina22x.c
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
 * \brief INA22x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.18=9
 * 
 * \date 2021/04/11
 * 
 * \addtogroup ina22x
 * \{
 */

#include "ina22x.h"

int ina22x_init(ina22x_config_t config)
{
    return -1;
}

int ina22x_write_reg(ina22x_config_t config, ina22x_reg_t reg, uint16_t val)
{
    return -1;
}

int ina22x_read_reg(ina22x_config_t config, ina22x_reg_t reg, uint16_t *val)
{
    return -1;
}

int ina22x_get_current(ina22x_config_t config, ina22x_current_t *cur)
{
    return -1;
}

int ina22x_get_manufacturer_id(ina22x_config_t config, ina22x_id_t *id)
{
    return -1;
}

int ina22x_get_die_id(ina22x_config_t config, ina22x_id_t *id)
{
    return -1;
}

/** \} End of ina22x group */
