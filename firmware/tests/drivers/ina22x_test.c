/*
 * ina22x_test.c
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
 * \brief Unit test of the Ina22x driver.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 2022/06/07
 * 
 * \defgroup ina22x_unit_test INA22x
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <stdlib.h>
#include <string.h>

#include <drivers/i2c/i2c.h>
#include <drivers/ina22x/ina22x.h>

#define INA22X_IIC_PORT         I2C_PORT_2
#define INA22X_IIC_CLOCK_HZ     100000UL
#define INA22X_IIC_ADR          0x04

unsigned int generate_random(unsigned int l, unsigned int r);

void write_test(uint8_t *wd, uint16_t wd_len);

void read_test(uint8_t *rd, uint16_t rd_len);

ina22x_config_t conf = {0};

static void ina22x_init_test(void **state)
{
    expect_value(__wrap_i2c_init, port, INA22X_IIC_PORT);
    expect_value(__wrap_i2c_init, config.speed_hz, INA22X_IIC_CLOCK_HZ);

    will_return(__wrap_i2c_init, 0);

    uint8_t data[3] = {INA22X_REG_CONFIGURATION, 0x80, 0x00};

    write_test(data, 3);

    assert_return_code(ina22x_init(conf), 0);
}

static void ina22x_configuration_test(void **state)
{
}

static void ina22x_calibration_test(void **state)
{
}

static void ina22x_write_reg_test(void **state)
{
}

static void ina22x_read_reg_test(void **state)
{
}

static void ina22x_get_current_raw_test(void **state)
{
}

static void ina22x_get_voltage_raw_test(void **state)
{
}

static void ina22x_power_raw_test(void **state)
{
}

static void ina22x_get_current_A_test(void **state)
{
}

static void ina22x_get_voltage_V_test(void **state)
{
}

static void ina22x_get_power_W_test(void **state)
{
}

static void ina22x_get_manufacturer_id_test(void **state)
{
}

static void ina22x_get_die_id_test(void **state)
{
}

int main(void)
{
    conf.i2c_port                   = INA22X_IIC_PORT;
    conf.i2c_conf.speed_hz          = INA22X_IIC_CLOCK_HZ;
//    conf.avg_mode                   = ;
//    conf.bus_voltage_conv_time      = ;
//    conf.shunt_voltage_conv_time    = ;
//    conf.op_mode                    = ;
//    conf.device                     = ;
//    conf.lsb_current                = ;
//    conf.cal                        = ;

    const struct CMUnitTest ina22x_tests[] = {
        cmocka_unit_test(ina22x_init_test),
        cmocka_unit_test(ina22x_configuration_test),
        cmocka_unit_test(ina22x_calibration_test),
        cmocka_unit_test(ina22x_write_reg_test),
        cmocka_unit_test(ina22x_read_reg_test),
        cmocka_unit_test(ina22x_get_current_raw_test),
        cmocka_unit_test(ina22x_get_voltage_raw_test),
        cmocka_unit_test(ina22x_power_raw_test),
        cmocka_unit_test(ina22x_get_current_A_test),
        cmocka_unit_test(ina22x_get_voltage_V_test),
        cmocka_unit_test(ina22x_get_power_W_test),
        cmocka_unit_test(ina22x_get_manufacturer_id_test),
        cmocka_unit_test(ina22x_get_die_id_test),
    };

    return cmocka_run_group_tests(ina22x_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

void write_test(uint8_t *wd, uint16_t wd_len)
{
    expect_value(__wrap_i2c_write, port, INA22X_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, INA22X_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)wd, wd_len);
    expect_value(__wrap_i2c_write, len, wd_len);

    will_return(__wrap_i2c_write, 0);
}

void read_test(uint8_t *rd, uint16_t rd_len)
{
    expect_value(__wrap_i2c_read, port, INA22X_IIC_PORT);
    expect_value(__wrap_i2c_read, adr, INA22X_IIC_ADR);

    expect_value(__wrap_i2c_read, len, rd_len);

    uint16_t i = 0;
    for(i = 0; i < rd_len; i++)
    {
        will_return(__wrap_i2c_read, rd[i]);
    }

    will_return(__wrap_i2c_read, 0);
}

/** \} End of ina22x_test group */
