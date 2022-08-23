/*
 * ina22x_wrap.h
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
 * \brief INA22x driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.1.5
 * 
 * \date 2021/08/12
 * 
 * \defgroup ina22x_wrap INA22x Wrap
 * \ingroup tests
 * \{
 */

#ifndef INA22X_WRAP_H_
#define INA22X_WRAP_H_

#include <drivers/ina22x/ina22x.h>

int __wrap_ina22x_init(ina22x_config_t config);

int __wrap_ina22x_configuration(ina22x_config_t config);

int __wrap_ina22x_calibration(ina22x_config_t config);

int __wrap_ina22x_write_reg(ina22x_config_t config, ina22x_register_t reg, uint16_t val);

int __wrap_ina22x_read_reg(ina22x_config_t config, ina22x_register_t reg, uint16_t *val);

int __wrap_ina22x_get_current_A(ina22x_config_t config, ina22x_current_t *cur);

int __wrap_ina22x_get_voltage_V(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *volt);

int __wrap_ina22x_get_power_W(ina22x_config_t config,ina22x_power_t *pwr);

int __wrap_ina22x_get_manufacturer_id(ina22x_config_t config, ina22x_id_t *id);

int __wrap_ina22x_get_die_id(ina22x_config_t config, ina22x_id_t *id);

#endif /* INA22X_WRAP_H_ */

/** \} End of ina22x_wrap group */
