/*
 * ina22x_wrap.c
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
 * \brief INA22x driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.1.5
 * 
 * \date 2021/08/12
 * 
 * \addtogroup ina22x_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "ina22x_wrap.h"

int __wrap_ina22x_init(ina22x_config_t config)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    return mock_type(int);
}

int __wrap_ina22x_configuration(ina22x_config_t config)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    return mock_type(int);
}

int __wrap_ina22x_calibration(ina22x_config_t config)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    return mock_type(int);
}

int __wrap_ina22x_write_reg(ina22x_config_t config, ina22x_register_t reg, uint16_t val)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    check_expected(reg);
    check_expected(val);

    return mock_type(int);
}

int __wrap_ina22x_read_reg(ina22x_config_t config, ina22x_register_t reg, uint16_t *val)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);
    
    check_expected(reg);

    *val = mock_type(uint16_t);

    return mock_type(int);
}

int __wrap_ina22x_get_current_A(ina22x_config_t config, ina22x_current_t *cur)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    *cur = mock_type(float);

    return mock_type(int);
}

int __wrap_ina22x_get_voltage_V(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *volt)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    check_expected(device);

    *volt = mock_type(float);

    return mock_type(int);
}

int __wrap_ina22x_get_power_W(ina22x_config_t config,ina22x_power_t *pwr)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    *pwr = mock_type(float);

    return mock_type(int);
}

int __wrap_ina22x_get_manufacturer_id(ina22x_config_t config, ina22x_id_t *id)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    *id = mock_type(ina22x_id_t);

    return mock_type(int);

}

int __wrap_ina22x_get_die_id(ina22x_config_t config, ina22x_id_t *id)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_conf.speed_hz);
    check_expected(config.i2c_adr);
    check_expected(config.avg_mode);
    check_expected(config.bus_voltage_conv_time);
    check_expected(config.shunt_voltage_conv_time);
    check_expected(config.op_mode);
    check_expected(config.lsb_current);
    check_expected(config.cal);

    *id = mock_type(ina22x_id_t);

    return mock_type(int);
}
/** \} End of ina22x_wrap group */
