/*
 * leds_test.c
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
 * \brief Unit test of the LEDs device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.25
 * 
 * \date 2021/02/16
 * 
 * \defgroup leds_unit_test LEDs
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/leds/leds.h>
#include <drivers/gpio/gpio.h>

#define LED_SYSTEM_NUM              0
#define LED_FAULT_NUM               1
#define LED_DOWNLINK_NUM            2
#define LED_UPLINK_NUM              3

/* GPIO configuration */
#define LED_SYSTEM_GPIO_PIN         GPIO_PIN_27
#define LED_FAULT_GPIO_PIN          GPIO_PIN_28
#define LED_DOWNLINK_GPIO_PIN       GPIO_PIN_30
#define LED_UPLINK_GPIO_PIN         GPIO_PIN_29

static void leds_init_test(void **state)
{
    expect_value(__wrap_gpio_init, pin, LED_SYSTEM_GPIO_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);
    expect_value(__wrap_gpio_init, pin, LED_FAULT_GPIO_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);
    expect_value(__wrap_gpio_init, pin, LED_DOWNLINK_GPIO_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);
    expect_value(__wrap_gpio_init, pin, LED_UPLINK_GPIO_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);
    will_return(__wrap_gpio_init, 0);
    will_return(__wrap_gpio_init, 0);
    will_return(__wrap_gpio_init, 0);

    assert_return_code(leds_init(), 0);
}

static void led_set_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        if (i == LED_SYSTEM_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_SYSTEM_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_HIGH);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_set(i), 0);
        }
        else if (i == LED_FAULT_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_FAULT_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_HIGH);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_set(i), 0);
        }
        else if (i == LED_DOWNLINK_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_DOWNLINK_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_HIGH);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_set(i), 0);
        }
        else if (i == LED_UPLINK_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_UPLINK_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_HIGH);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_set(i), 0);
        }
        else
        {
            assert_true(led_set(i) != 0);
        }
    }
}

static void led_clear_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        if (i == LED_SYSTEM_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_SYSTEM_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_LOW);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_clear(i), 0);
        }
        else if (i == LED_FAULT_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_FAULT_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_LOW);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_clear(i), 0);
        }
        else if (i == LED_DOWNLINK_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_DOWNLINK_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_LOW);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_clear(i), 0);
        }
        else if (i == LED_UPLINK_NUM)
        {
            expect_value(__wrap_gpio_set_state, pin, LED_UPLINK_GPIO_PIN);
            expect_value(__wrap_gpio_set_state, level, GPIO_STATE_LOW);

            will_return(__wrap_gpio_set_state, 0);

            assert_return_code(led_clear(i), 0);
        }
        else
        {
            assert_true(led_clear(i) != 0);
        }
    }
}

static void led_toggle_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        if (i == LED_SYSTEM_NUM)
        {
            expect_value(__wrap_gpio_toggle, pin, LED_SYSTEM_GPIO_PIN);

            will_return(__wrap_gpio_toggle, 0);

            assert_return_code(led_toggle(i), 0);
        }
        else if (i == LED_FAULT_NUM)
        {
            expect_value(__wrap_gpio_toggle, pin, LED_FAULT_GPIO_PIN);

            will_return(__wrap_gpio_toggle, 0);

            assert_return_code(led_toggle(i), 0);
        }
        else if (i == LED_DOWNLINK_NUM)
        {
            expect_value(__wrap_gpio_toggle, pin, LED_DOWNLINK_GPIO_PIN);

            will_return(__wrap_gpio_toggle, 0);

            assert_return_code(led_toggle(i), 0);
        }
        else if (i == LED_UPLINK_NUM)
        {
            expect_value(__wrap_gpio_toggle, pin, LED_UPLINK_GPIO_PIN);

            will_return(__wrap_gpio_toggle, 0);

            assert_return_code(led_toggle(i), 0);
        }
        else
        {
            assert_true(led_toggle(i) != 0);
        }
    }
}

int main(void)
{
    const struct CMUnitTest leds_tests[] = {
        cmocka_unit_test(leds_init_test),
        cmocka_unit_test(led_set_test),
        cmocka_unit_test(led_clear_test),
        cmocka_unit_test(led_toggle_test),
    };

    return cmocka_run_group_tests(leds_tests, NULL, NULL);
}

/** \} End of leds_test group */
