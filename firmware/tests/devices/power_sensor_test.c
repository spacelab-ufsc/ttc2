/*
 * power_sensor_test.c
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
 * \brief Unit test of the power sensor device.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.2.5
 *
 * \date 2021/08/12
 *
 * \defgroup power_sensor_unit_test Power Sensor
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
#include <stdio.h>
#include <math.h>

#include <devices/power_sensor/power_sensor.h>
#include <drivers/ina22x/ina22x.h>

typedef enum
{
    INITIALIZATION=0,
    CONFIGURATION,
    CALIBRATION,
    VOLTAGE,
    CURRENT,
    POWER,
}config_type_t;

power_sensor_data_t test_data;

voltage_t f_volt_i_b, f_volt_i_s;
current_t f_curr;
power_t f_pwr;

void write_config_test(ina22x_config_t config_test, config_type_t config_type);

float generate_random_u_value(float l, float r);

float generate_random_m_value(float l, float r);

ina22x_config_t uc_config = {
    .i2c_port                   = I2C_PORT_1,
    .i2c_conf.speed_hz          = 100000,
    .i2c_adr                    = 0x44,
    .avg_mode                   = INA22X_AVERAGING_MODE_128,
    .bus_voltage_conv_time      = INA22X_BUS_VOLTAGE_CONV_TIME_588u,
    .shunt_voltage_conv_time    = INA22X_SHUNT_VOLTAGE_CONV_TIME_588u,
    .op_mode                    = INA22X_MODE_SHUNT_BUS_CONT,
    .lsb_current                = 5e-6,
    .cal                        = 10240,
};

ina22x_config_t radio_config = {
    .i2c_port                   = I2C_PORT_1,
    .i2c_conf.speed_hz          = 100000,
    .i2c_adr                    = 0x45,
    .avg_mode                   = INA22X_AVERAGING_MODE_128,
    .bus_voltage_conv_time      = INA22X_BUS_VOLTAGE_CONV_TIME_588u,
    .shunt_voltage_conv_time    = INA22X_SHUNT_VOLTAGE_CONV_TIME_588u,
    .op_mode                    = INA22X_MODE_SHUNT_BUS_CONT,
    .lsb_current                = 5e-5,
    .cal                        = 1024,
};

static void power_sensor_init_test(void **state)
{
    /* Initialization */
    write_config_test(radio_config, INITIALIZATION);

    write_config_test(uc_config, INITIALIZATION);

    /* Configuration */
    write_config_test(radio_config, CONFIGURATION);

    write_config_test(uc_config, CONFIGURATION);

    /* Calibration */
    write_config_test(radio_config, CALIBRATION);

    write_config_test(uc_config, CALIBRATION);


    assert_return_code(power_sensor_init(), 0);
}

static void power_sensor_read_test(void **state)
{
    ina22x_voltage_t voltage_value_b = (ina22x_voltage_t) generate_random_m_value(1e-4, 1e4);
    ina22x_voltage_t voltage_value_s = (ina22x_voltage_t) generate_random_u_value(1e-4, 1e4);
    f_volt_i_b = (voltage_t) (voltage_value_b * 1000);
    f_volt_i_s = (voltage_t) (voltage_value_s * 1000000);
    float comp_b = (float) f_volt_i_b;
    float comp_s = (float) f_volt_i_s;

    ina22x_current_t current_value = (ina22x_current_t) generate_random_u_value(1e-4, 1e4);
    f_curr = (current_t) (current_value * 1000);
    float comp_curr = (float) f_curr;

    ina22x_power_t power_value = (ina22x_power_t) generate_random_m_value(1e-4, 1e4);
    f_pwr = (power_t) (floor(power_value * 1000));
    float comp_pwr = (float) f_pwr;

    test_data.shunt_voltage = f_volt_i_s;
    test_data.bus_voltage = f_volt_i_b;
    test_data.current = f_curr;
    test_data.power = f_pwr;

    /* Bus Voltage */
    write_config_test(uc_config, VOLTAGE);

    expect_value(__wrap_ina22x_get_voltage_V, device, INA22X_BUS_VOLTAGE);

    will_return(__wrap_ina22x_get_voltage_V, voltage_value_b);

    will_return(__wrap_ina22x_get_voltage_V, 0);

    /* Shunt Voltage */
    write_config_test(uc_config, VOLTAGE);

    expect_value(__wrap_ina22x_get_voltage_V, device, INA22X_SHUNT_VOLTAGE);

    will_return(__wrap_ina22x_get_voltage_V, voltage_value_s);

    will_return(__wrap_ina22x_get_voltage_V, 0);


    /* Current */
    write_config_test(uc_config, CURRENT);

    will_return(__wrap_ina22x_get_current_A, current_value);

    will_return(__wrap_ina22x_get_current_A, 0);


    /* Power */
    write_config_test(uc_config, POWER);

    will_return(__wrap_ina22x_get_power_W, power_value);

    will_return(__wrap_ina22x_get_power_W, 0);

    assert_return_code(power_sensor_read(POWER_SENSOR_UC, &test_data), 0);
    assert_float_equal(comp_b, floor(voltage_value_b * 1000), 0.0);
    assert_float_equal(comp_s, floor(voltage_value_s * 1000000), 0.0);
    assert_float_equal(comp_curr, floor(current_value * 1000), 0.0);
    assert_float_equal(comp_pwr, floor(power_value * 1000), 0.0);
}

static void power_sensor_read_voltage_test(void **state)
{
    ina22x_voltage_t voltage_value_b = (ina22x_voltage_t) generate_random_m_value(1e-4, 1e4);
    ina22x_voltage_t voltage_value_s = (ina22x_voltage_t) generate_random_u_value(1e-4, 1e4);
    f_volt_i_b = (voltage_t) (voltage_value_b * 1000);
    f_volt_i_s = (voltage_t) (voltage_value_s * 1000000);
    float comp_b = (float) f_volt_i_b;
    float comp_s = (float) f_volt_i_s;

    write_config_test(uc_config, VOLTAGE);
    expect_value(__wrap_ina22x_get_voltage_V, device, INA22X_BUS_VOLTAGE);

    will_return(__wrap_ina22x_get_voltage_V, voltage_value_b);
    will_return(__wrap_ina22x_get_voltage_V, 0);


    write_config_test(uc_config, VOLTAGE);
    expect_value(__wrap_ina22x_get_voltage_V, device, INA22X_SHUNT_VOLTAGE);

    will_return(__wrap_ina22x_get_voltage_V, voltage_value_s);
    will_return(__wrap_ina22x_get_voltage_V, 0);

    assert_return_code(power_sensor_read_voltage(POWER_SENSOR_UC, POWER_SENSOR_MICRO_SCALE, POWER_SENSOR_MILI_SCALE, &f_volt_i_s, &f_volt_i_b), 0);
    assert_float_equal(comp_b, floor(voltage_value_b * 1000), 0.0);
    assert_float_equal(comp_s, floor(voltage_value_s *1000000), 0.0);
}

static void power_sensor_read_current_test(void **state)
{
    ina22x_current_t current_value = (ina22x_current_t) generate_random_m_value(1e-4, 1e4);
    f_curr = (current_t) (current_value * 1000);
    float comp = (float) f_curr;

    write_config_test(uc_config, CURRENT);

    will_return(__wrap_ina22x_get_current_A, current_value);
    will_return(__wrap_ina22x_get_current_A, 0);

    assert_return_code(power_sensor_read_current(POWER_SENSOR_UC, POWER_SENSOR_MILI_SCALE, &f_curr), 0);
    assert_float_equal(comp, floor(current_value * 1000), 0.0);
}

static void power_sensor_read_power_test(void **state)
{
    ina22x_power_t power_value = (ina22x_power_t) generate_random_u_value(1e-4, 1e4);
    f_pwr = (power_t) (floor(power_value * 1000000));
    float comp = (float) f_pwr;

    write_config_test(uc_config, POWER);

    will_return(__wrap_ina22x_get_power_W, power_value);
    will_return(__wrap_ina22x_get_power_W, 0);

    assert_return_code(power_sensor_read_power(POWER_SENSOR_UC, POWER_SENSOR_MICRO_SCALE, &f_pwr), 0);
    assert_float_equal(comp, floor(power_value * 1000000), 0.0);
}

int main(void)
{
    const struct CMUnitTest power_sensor_tests[] = {
        cmocka_unit_test(power_sensor_init_test),
        cmocka_unit_test(power_sensor_read_test),
        cmocka_unit_test(power_sensor_read_voltage_test),
        cmocka_unit_test(power_sensor_read_current_test),
        cmocka_unit_test(power_sensor_read_power_test),
    };

    return cmocka_run_group_tests(power_sensor_tests, NULL, NULL);
}

void write_config_test(ina22x_config_t config_test, config_type_t config_type)
{
    switch(config_type)
    {
        case INITIALIZATION:
            expect_value(__wrap_ina22x_init, config.i2c_port, config_test.i2c_port);
            expect_value(__wrap_ina22x_init, config.i2c_conf.speed_hz, config_test.i2c_conf.speed_hz);
            expect_value(__wrap_ina22x_init, config.i2c_adr, config_test.i2c_adr);
            expect_value(__wrap_ina22x_init, config.avg_mode, config_test.avg_mode);
            expect_value(__wrap_ina22x_init, config.bus_voltage_conv_time, config_test.bus_voltage_conv_time);
            expect_value(__wrap_ina22x_init, config.shunt_voltage_conv_time, config_test.shunt_voltage_conv_time);
            expect_value(__wrap_ina22x_init, config.op_mode, config_test.op_mode);
            expect_value(__wrap_ina22x_init, config.lsb_current, config_test.lsb_current);
            expect_value(__wrap_ina22x_init, config.cal, config_test.cal);

            will_return(__wrap_ina22x_init, 0);

            break;

        case CONFIGURATION:
            expect_value(__wrap_ina22x_configuration, config.i2c_port, config_test.i2c_port);
            expect_value(__wrap_ina22x_configuration, config.i2c_conf.speed_hz, config_test.i2c_conf.speed_hz);
            expect_value(__wrap_ina22x_configuration, config.i2c_adr, config_test.i2c_adr);
            expect_value(__wrap_ina22x_configuration, config.avg_mode, config_test.avg_mode);
            expect_value(__wrap_ina22x_configuration, config.bus_voltage_conv_time, config_test.bus_voltage_conv_time);
            expect_value(__wrap_ina22x_configuration, config.shunt_voltage_conv_time, config_test.shunt_voltage_conv_time);
            expect_value(__wrap_ina22x_configuration, config.op_mode, config_test.op_mode);
            expect_value(__wrap_ina22x_configuration, config.lsb_current, config_test.lsb_current);
            expect_value(__wrap_ina22x_configuration, config.cal, config_test.cal);

            will_return(__wrap_ina22x_configuration, 0);

            break;

        case CALIBRATION:
            expect_value(__wrap_ina22x_calibration, config.i2c_port, config_test.i2c_port);
            expect_value(__wrap_ina22x_calibration, config.i2c_conf.speed_hz, config_test.i2c_conf.speed_hz);
            expect_value(__wrap_ina22x_calibration, config.i2c_adr, config_test.i2c_adr);
            expect_value(__wrap_ina22x_calibration, config.avg_mode, config_test.avg_mode);
            expect_value(__wrap_ina22x_calibration, config.bus_voltage_conv_time, config_test.bus_voltage_conv_time);
            expect_value(__wrap_ina22x_calibration, config.shunt_voltage_conv_time, config_test.shunt_voltage_conv_time);
            expect_value(__wrap_ina22x_calibration, config.op_mode, config_test.op_mode);
            expect_value(__wrap_ina22x_calibration, config.lsb_current, config_test.lsb_current);
            expect_value(__wrap_ina22x_calibration, config.cal, config_test.cal);

            will_return(__wrap_ina22x_calibration, 0);

            break;

        case VOLTAGE:
            expect_value(__wrap_ina22x_get_voltage_V, config.i2c_port, config_test.i2c_port);
            expect_value(__wrap_ina22x_get_voltage_V, config.i2c_conf.speed_hz, config_test.i2c_conf.speed_hz);
            expect_value(__wrap_ina22x_get_voltage_V, config.i2c_adr, config_test.i2c_adr);
            expect_value(__wrap_ina22x_get_voltage_V, config.avg_mode, config_test.avg_mode);
            expect_value(__wrap_ina22x_get_voltage_V, config.bus_voltage_conv_time, config_test.bus_voltage_conv_time);
            expect_value(__wrap_ina22x_get_voltage_V, config.shunt_voltage_conv_time, config_test.shunt_voltage_conv_time);
            expect_value(__wrap_ina22x_get_voltage_V, config.op_mode, config_test.op_mode);
            expect_value(__wrap_ina22x_get_voltage_V, config.lsb_current, config_test.lsb_current);
            expect_value(__wrap_ina22x_get_voltage_V, config.cal, config_test.cal);

            break;

        case CURRENT:
            expect_value(__wrap_ina22x_get_current_A, config.i2c_port, config_test.i2c_port);
            expect_value(__wrap_ina22x_get_current_A, config.i2c_conf.speed_hz, config_test.i2c_conf.speed_hz);
            expect_value(__wrap_ina22x_get_current_A, config.i2c_adr, config_test.i2c_adr);
            expect_value(__wrap_ina22x_get_current_A, config.avg_mode, config_test.avg_mode);
            expect_value(__wrap_ina22x_get_current_A, config.bus_voltage_conv_time, config_test.bus_voltage_conv_time);
            expect_value(__wrap_ina22x_get_current_A, config.shunt_voltage_conv_time, config_test.shunt_voltage_conv_time);
            expect_value(__wrap_ina22x_get_current_A, config.op_mode, config_test.op_mode);
            expect_value(__wrap_ina22x_get_current_A, config.lsb_current, config_test.lsb_current);
            expect_value(__wrap_ina22x_get_current_A, config.cal, config_test.cal);

            break;

        case POWER:
            expect_value(__wrap_ina22x_get_power_W, config.i2c_port, config_test.i2c_port);
            expect_value(__wrap_ina22x_get_power_W, config.i2c_conf.speed_hz, config_test.i2c_conf.speed_hz);
            expect_value(__wrap_ina22x_get_power_W, config.i2c_adr, config_test.i2c_adr);
            expect_value(__wrap_ina22x_get_power_W, config.avg_mode, config_test.avg_mode);
            expect_value(__wrap_ina22x_get_power_W, config.bus_voltage_conv_time, config_test.bus_voltage_conv_time);
            expect_value(__wrap_ina22x_get_power_W, config.shunt_voltage_conv_time, config_test.shunt_voltage_conv_time);
            expect_value(__wrap_ina22x_get_power_W, config.op_mode, config_test.op_mode);
            expect_value(__wrap_ina22x_get_power_W, config.lsb_current, config_test.lsb_current);
            expect_value(__wrap_ina22x_get_power_W, config.cal, config_test.cal);

            break;
    }
}

float generate_random_u_value(float l, float r)
{
    float x = rand() / (float) RAND_MAX;
    return (l + x * (r - l))/1000000;
}

float generate_random_m_value(float l, float r)
{
  float x = rand() / (float) RAND_MAX;
  return (l + x * (r - l))/1000;
}

/** \} End of power_sensor_test group */
