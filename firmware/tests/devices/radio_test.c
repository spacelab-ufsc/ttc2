/*
 * radio_test.c
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
 * \brief Unit test of the radio device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.1
 * 
 * \date 2024/04/25
 * 
 * \defgroup radio_unit_test Radio
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

#include <devices/radio/radio.h>
#include <devices/leds/leds.h>
#include <tests/mockups/drivers/si446x_wrap.h>

#define RADIO_ID    0x4463

static void radio_init_test(void **state)
{
    will_return(__wrap_si446x_init, 0);

    will_return(__wrap_si446x_rx_init, true);

    assert_return_code(radio_init(), 0);
}

static void radio_send_test(void **state)
{
    uint8_t data[50] = {0};
    uint16_t len = 50;

    will_return(__wrap_si446x_mutex_take, 0);

    expect_value(__wrap_led_set, l, LED_DOWNLINK);
    will_return(__wrap_led_set, 0);

    expect_value(__wrap_si446x_tx_long_packet, packet, data);
    expect_value(__wrap_si446x_tx_long_packet, len, len);

    will_return(__wrap_si446x_tx_long_packet, true);

    expect_value(__wrap_led_clear, l, LED_DOWNLINK);
    will_return(__wrap_led_clear, 0);

    will_return(__wrap_si446x_rx_init, true);

    expect_value(__wrap_led_clear, l, LED_DOWNLINK);
    will_return(__wrap_led_clear, 0);

    will_return(__wrap_si446x_mutex_give, 0);

    assert_return_code(radio_send(data, len), 0);
}

static void radio_recv_test(void **state)
{
    uint8_t data[50] = {0};
    uint8_t len = 50;
    uint32_t timeout_ms = 100;
    uint16_t i;

    will_return(__wrap_si446x_mutex_take, 0);

    will_return(__wrap_si446x_wait_nirq, true);

    expect_value(__wrap_si446x_rx_packet, rx_buf, data);
    expect_value(__wrap_si446x_rx_packet, read_len, len);

    for(i = 0; i < len; i++)
    {
        will_return(__wrap_si446x_rx_packet, data[i]);
    }
    will_return(__wrap_si446x_rx_packet, 50);

    will_return(__wrap_si446x_clear_interrupts, true);

    will_return(__wrap_si446x_rx_init, true);

    will_return(__wrap_si446x_mutex_give, 0);

    assert_return_code(radio_recv(data, len, timeout_ms), len);
}

static void radio_sleep_test(void **state)
{
    will_return(__wrap_si446x_mutex_take, 0);

    will_return(__wrap_si446x_enter_standby_mode, 0);

    will_return(__wrap_si446x_mutex_give, 0);

    assert_return_code(radio_sleep(), 0);
}

int main(void)
{
    const struct CMUnitTest radio_tests[] = {
        cmocka_unit_test(radio_init_test),
        cmocka_unit_test(radio_send_test),
        cmocka_unit_test(radio_recv_test),
        cmocka_unit_test(radio_sleep_test)
    };

    return cmocka_run_group_tests(radio_tests, NULL, NULL);
}

/** \} End of radio_unit_test group */
