/*
 * temp_sensor_test.c
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
 * \brief Unit test of the temperature sensor device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.5
 * 
 * \date 2021/02/13
 * 
 * \defgroup temp_sensor_unit_test Temperature Sensor
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/temp_sensor/temp_sensor.h>
#include <drivers/adc/adc.h>

#define TEMP_SENSOR_ADC_PORT            ADC_PORT_5
#define TEMP_SENSOR_ADC_MIN_VAL         0
#define TEMP_SENSOR_ADC_MAX_VAL         0xFFF       /* 12-bit precision = 0xFFF */
#define TEMP_SENSOR_ADC_MREF_VAL        6.5999999
#define TEMP_SENSOR_ADC_NREF_VAL        1949.0
#define TEMP_SENSOR_MIN_VAL_C           (-273)
#define TEMP_SENSOR_MAX_VAL_C           358
#define TEMP_SENSOR_MIN_VAL_K           0
#define TEMP_SENSOR_MAX_VAL_K           (TEMP_SENSOR_MAX_VAL_C + 273)

static void temp_sensor_init_test(void **state)
{
    expect_value(__wrap_adc_read, port, TEMP_SENSOR_ADC_PORT);

    will_return(__wrap_adc_init, 0);
    will_return(__wrap_adc_read, 1024);
    will_return(__wrap_adc_read, 0);
    will_return(__wrap_adc_temp_get_mref, TEMP_SENSOR_ADC_MREF_VAL);
    will_return(__wrap_adc_temp_get_nref, TEMP_SENSOR_ADC_NREF_VAL);

    assert_return_code(temp_sensor_init(), 0);
}

static void temp_sensor_read_raw_test(void **state)
{
    uint16_t i = 0;

    for(i=TEMP_SENSOR_ADC_MIN_VAL; i<=TEMP_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, TEMP_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);
        will_return(__wrap_adc_read, 0);

        uint16_t raw_temp = UINT16_MAX;

        int result = temp_sensor_read_raw(&raw_temp);

        assert_return_code(result, 0);
        assert_int_equal(raw_temp, i);
    }
}

static void temp_sensor_raw_to_c_test(void **state)
{
    uint16_t i = 0;

    for(i=TEMP_SENSOR_ADC_MIN_VAL; i<=TEMP_SENSOR_ADC_MAX_VAL; i++)
    {
        will_return(__wrap_adc_temp_get_mref, TEMP_SENSOR_ADC_MREF_VAL);
        will_return(__wrap_adc_temp_get_nref, TEMP_SENSOR_ADC_NREF_VAL);

        int16_t temp_c = temp_sensor_raw_to_c(i);

        assert_in_range(temp_c+300, TEMP_SENSOR_MIN_VAL_C+300, TEMP_SENSOR_MAX_VAL_C+300);
    }
}

static void temp_sensor_raw_to_k_test(void **state)
{
    uint16_t i = 0;

    for(i=TEMP_SENSOR_ADC_MIN_VAL; i<=TEMP_SENSOR_ADC_MAX_VAL; i++)
    {
        will_return(__wrap_adc_temp_get_mref, TEMP_SENSOR_ADC_MREF_VAL);
        will_return(__wrap_adc_temp_get_nref, TEMP_SENSOR_ADC_NREF_VAL);

        assert_in_range(temp_sensor_raw_to_k(i), TEMP_SENSOR_MIN_VAL_K, TEMP_SENSOR_MAX_VAL_K);
    }
}

static void temp_sensor_read_c_test(void **state)
{
    uint16_t i = 0;

    for(i=TEMP_SENSOR_ADC_MIN_VAL; i<=TEMP_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, TEMP_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);
        will_return(__wrap_adc_read, 0);
        will_return(__wrap_adc_temp_get_mref, TEMP_SENSOR_ADC_MREF_VAL);
        will_return(__wrap_adc_temp_get_nref, TEMP_SENSOR_ADC_NREF_VAL);

        int16_t temp_c = INT16_MAX;

        int result = temp_sensor_read_c(&temp_c);

        assert_return_code(result, 0);
        assert_in_range(temp_c+300, TEMP_SENSOR_MIN_VAL_C+300, TEMP_SENSOR_MAX_VAL_C+300);
    }
}

static void temp_sensor_read_k_test(void **state)
{
    uint16_t i = 0;

    for(i=TEMP_SENSOR_ADC_MIN_VAL; i<=TEMP_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, TEMP_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);
        will_return(__wrap_adc_read, 0);
        will_return(__wrap_adc_temp_get_mref, TEMP_SENSOR_ADC_MREF_VAL);
        will_return(__wrap_adc_temp_get_nref, TEMP_SENSOR_ADC_NREF_VAL);

        uint16_t temp_k = UINT16_MAX;

        int result = temp_sensor_read_k(&temp_k);

        assert_return_code(result, 0);
        assert_in_range(temp_k+200, TEMP_SENSOR_MIN_VAL_K+200, TEMP_SENSOR_MAX_VAL_K+200);
    }
}

int main(void)
{
    const struct CMUnitTest temp_sensor_tests[] = {
        cmocka_unit_test(temp_sensor_init_test),
        cmocka_unit_test(temp_sensor_read_raw_test),
        cmocka_unit_test(temp_sensor_raw_to_c_test),
        cmocka_unit_test(temp_sensor_raw_to_k_test),
        cmocka_unit_test(temp_sensor_read_c_test),
        cmocka_unit_test(temp_sensor_read_k_test),
    };

    return cmocka_run_group_tests(temp_sensor_tests, NULL, NULL);
}

/** \} End of temp_sensor_unit_test group */
