/*
 * si446x_test.c
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
 * \brief Unit test of the Si446x driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.23
 * 
 * \date 2021/09/04
 * 
 * \defgroup si446x_unit_test Si446x
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

#include <config/radio_config_Si4463.h>
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <drivers/si446x/si446x.h>

#define SI446X_GPIO_SDN         GPIO_PIN_16
#define SI446X_GPIO_NIRQ        GPIO_PIN_17
#define SI446X_SPI_PORT         SPI_PORT_1
#define SI446X_SPI_CS           GPIO_PIN_8

unsigned int generate_random(unsigned int l, unsigned int r);

static void si446x_init_test(void **state)
{
}

static void si446x_reg_config_test(void **state)
{
}

static void si446x_power_on_reset_test(void **state)
{
}

static void si446x_tx_packet_test(void **state)
{
}

static void si446x_tx_long_packet_test(void **state)
{
}

static void si446x_rx_packet_test(void **state)
{
}

static void si446x_rx_init_test(void **state)
{
}

static void si446x_check_device_test(void **state)
{
}

static void si446x_check_cts_test(void **state)
{
}

static void si446x_set_tx_power_test(void **state)
{
}

static void si446x_set_properties_test(void **state)
{
}

static void si446x_get_properties_test(void **state)
{
}

static void si446x_set_config_test(void **state)
{
}

static void si446x_set_preamble_len_test(void **state)
{
}

static void si446x_set_sync_word_test(void **state)
{
}

static void si446x_set_gpio_mode_test(void **state)
{
}

static void si446x_set_cmd_test(void **state)
{
}

static void si446x_get_cmd_test(void **state)
{
}

static void si446x_set_tx_interrupt_test(void **state)
{
}

static void si446x_set_rx_interrupt_test(void **state)
{
}

static void si446x_clear_interrupts_test(void **state)
{
}

static void si446x_write_tx_fifo_test(void **state)
{
}

static void si446x_read_rx_fifo_test(void **state)
{
}

static void si446x_fifo_reset_test(void **state)
{
}

static void si446x_enter_tx_mode_test(void **state)
{
}

static void si446x_enter_rx_mode_test(void **state)
{
}

static void si446x_enter_standby_mode_test(void **state)
{
}

static void si446x_wait_nirq_test(void **state)
{
}

static void si446x_wait_packet_sent_test(void **state)
{
}

static void si446x_wait_gpio1_test(void **state)
{
}

static void si446x_spi_init_test(void **state)
{
}

static void si446x_spi_enable_test(void **state)
{
}

static void si446x_spi_disable_test(void **state)
{
}

static void si446x_spi_write_test(void **state)
{
}

static void si446x_spi_read_test(void **state)
{
}

static void si446x_spi_transfer_test(void **state)
{
}

static void si446x_gpio_init_test(void **state)
{
}

static void si446x_gpio_set_pin_test(void **state)
{
}

static void si446x_gpio_clear_pin_test(void **state)
{
}

static void si446x_gpio_get_pin_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest si446x_tests[] = {
        cmocka_unit_test(si446x_init_test),
        cmocka_unit_test(si446x_reg_config_test),
        cmocka_unit_test(si446x_power_on_reset_test),
        cmocka_unit_test(si446x_tx_packet_test),
        cmocka_unit_test(si446x_tx_long_packet_test),
        cmocka_unit_test(si446x_rx_packet_test),
        cmocka_unit_test(si446x_rx_init_test),
        cmocka_unit_test(si446x_check_device_test),
        cmocka_unit_test(si446x_check_cts_test),
        cmocka_unit_test(si446x_set_tx_power_test),
        cmocka_unit_test(si446x_set_properties_test),
        cmocka_unit_test(si446x_get_properties_test),
        cmocka_unit_test(si446x_set_config_test),
        cmocka_unit_test(si446x_set_preamble_len_test),
        cmocka_unit_test(si446x_set_sync_word_test),
        cmocka_unit_test(si446x_set_gpio_mode_test),
        cmocka_unit_test(si446x_set_cmd_test),
        cmocka_unit_test(si446x_get_cmd_test),
        cmocka_unit_test(si446x_set_tx_interrupt_test),
        cmocka_unit_test(si446x_set_rx_interrupt_test),
        cmocka_unit_test(si446x_clear_interrupts_test),
        cmocka_unit_test(si446x_write_tx_fifo_test),
        cmocka_unit_test(si446x_read_rx_fifo_test),
        cmocka_unit_test(si446x_fifo_reset_test),
        cmocka_unit_test(si446x_enter_tx_mode_test),
        cmocka_unit_test(si446x_enter_rx_mode_test),
        cmocka_unit_test(si446x_enter_standby_mode_test),
        cmocka_unit_test(si446x_wait_nirq_test),
        cmocka_unit_test(si446x_wait_packet_sent_test),
        cmocka_unit_test(si446x_wait_gpio1_test),
        cmocka_unit_test(si446x_spi_init_test),
        cmocka_unit_test(si446x_spi_enable_test),
        cmocka_unit_test(si446x_spi_disable_test),
        cmocka_unit_test(si446x_spi_write_test),
        cmocka_unit_test(si446x_spi_read_test),
        cmocka_unit_test(si446x_spi_transfer_test),
        cmocka_unit_test(si446x_gpio_init_test),
        cmocka_unit_test(si446x_gpio_set_pin_test),
        cmocka_unit_test(si446x_gpio_clear_pin_test),
        cmocka_unit_test(si446x_gpio_get_pin_test),
    };

    return cmocka_run_group_tests(si446x_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of si446x_test group */
