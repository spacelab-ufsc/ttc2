/*
 * power_sensor_test.c
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
 * \brief Unit test of the power sensor device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
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

#include <devices/power_sensor/power_sensor.h>
#include <drivers/ina22x/ina22x.h>

static void power_sensor_init_test(void **state)
{
}

static void power_sensor_read_test(void **state)
{
}

static void power_sensor_read_voltage_mv_test(void **state)
{
}

static void power_sensor_read_current_ma_test(void **state)
{
}

static void power_sensor_read_power_mw_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest power_sensor_tests[] = {
        cmocka_unit_test(power_sensor_init_test),
        cmocka_unit_test(power_sensor_read_test),
        cmocka_unit_test(power_sensor_read_voltage_mv_test),
        cmocka_unit_test(power_sensor_read_current_ma_test),
        cmocka_unit_test(power_sensor_read_power_mw_test),
    };

    return cmocka_run_group_tests(power_sensor_tests, NULL, NULL);
}

/** \} End of power_sensor_test group */
