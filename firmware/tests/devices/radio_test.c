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
 * \version 0.1.23
 * 
 * \date 2021/02/21
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
#include <tests/mockups/drivers/si446x_wrap.h>

#define RADIO_ID    0x4463

static void radio_init_test(void **state)
{
    expect_function_call(__wrap_si446x_init);
    expect_function_call(__wrap_si446x_rx_init);
    assert_return_code(radio_init(), 0);
}

static void radio_send_test(void **state)
{
    uint8_t data[50] = {0};
    uint16_t len = 50;

    will_return(__wrap_si446x_tx_long_packet,       data);
    will_return(__wrap_si446x_tx_long_packet,       len);

    expect_function_call(__wrap_si446x_tx_long_packet);
    expect_function_call(__wrap_si446x_rx_init);

    assert_return_code(radio_sent(data, len), 0);
}

static void radio_recv_test(void **state)
{
    uint8_t data[50] = {0};
    uint16_t len = 50;
    uint32_t timeout_ms = 100;
    uint32_t i;
    for (i=0; i < (timeout_ms /= 100); i++)
    {
        expect_function_call(__wrap_si446x_wait_nirq);

        will_return(__wrap_si446x_rx_packet, data);
        will_return(__wrap_si446x_rx_packet, len);

        will_return(__wrap_si446x_rx_packet, 0);

        expect_function_call(__wrap_si446x_clear_interrupts);
        expect_function_call(__wrap_si446x_rx_init);

    }

    assert_return_code(radio_recv(data, len, timeout_ms), 0);
}

static void radio_available_test(void **state)
{
    /*TODO*/
    assert_return_code(radio_available(), 0);
}

static void radio_sleep_test(void **state)
{
    will_return(__wrap_si446x_enter_standby_mode, 0);

    assert_return_code(radio_sleep(), 0);
}

static void radio_get_temperature_test(void **state)
{
    /*TODO*/
    radio_temp_t temp = 30;
    assert_return_code(radio_get_temperature(&temp), 0);
}

static void radio_get_rssi_test(void **state)
{
    /*TODO*/
    radio_rssi_t temp = 30;
    assert_return_code(radio_get_temperature(&temp), 0);
}

int main(void)
{
    const struct CMUnitTest radio_tests[] = {
        cmocka_unit_test(radio_init_test),
        cmocka_unit_test(radio_send_test),
        cmocka_unit_test(radio_recv_test),
        cmocka_unit_test(radio_available_test),
        cmocka_unit_test(radio_sleep_test),
        cmocka_unit_test(radio_get_temperature_test),
        cmocka_unit_test(radio_get_rssi_test)
    };

    return cmocka_run_group_tests(radio_tests, NULL, NULL);
}

/** \} End of radio_unit_test group */
