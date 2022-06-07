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
 * 
 * \version 0.0.1
 * 
 * \date 2022/06/07
 * 
 * \defgroup ina22x_unit_test Ina22x
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

unsigned int generate_random(unsigned int l, unsigned int r);

static void ina22x_init_test(void **state)
{
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

/** \} End of ina22x_test group */
