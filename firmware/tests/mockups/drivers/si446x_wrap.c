/*
 * si446x_wrap.c
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
 * \brief Si446x driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.23
 * 
 * \date 2021/04/27
 * 
 * \addtogroup si446x_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "si446x_wrap.h"

int __wrap_si446x_init(void)
{
    return mock_type(int);
}

void __wrap_si446x_reg_config(void)
{
    function_called();
}

void __wrap_si446x_power_on_reset(void)
{
    function_called();
}

bool __wrap_si446x_tx_packet(uint8_t *data, uint8_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(bool);
}

bool __wrap_si446x_tx_long_packet(uint8_t *packet, uint16_t len)
{
    check_expected_ptr(packet);
    check_expected(len);

    return mock_type(bool);
}

uint8_t __wrap_si446x_rx_packet(uint8_t *rx_buf, uint8_t read_len)
{
    check_expected(read_len);
    check_expected_ptr(rx_buf);


    if (rx_buf != NULL)
    {
        uint16_t i = 0;
        for(i = 0; i < read_len; i++)
        {
            rx_buf[i] = mock_type(uint8_t);
        }
    }

    return mock_type(uint8_t);
}

bool __wrap_si446x_rx_init(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_check_device(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_check_cts(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_set_tx_power(uint8_t pwr)
{
    check_expected(pwr);

    return mock_type(bool);
}

bool __wrap_si446x_set_properties(uint16_t start_property, uint8_t *para_buf, uint8_t length)
{
    check_expected(start_property);
    check_expected_ptr(para_buf);
    check_expected(length);

    return mock_type(bool);
}

bool __wrap_si446x_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf)
{
    check_expected(start_property);
    check_expected(length);

    if (para_buf != NULL)
    {
        uint16_t i = 0;
        for(i = 0; i < length; i++)
        {
            para_buf[i] = mock_type(uint8_t);
        }
    }

    return mock_type(bool);
}

void __wrap_si446x_set_config(const uint8_t *parameters, uint16_t para_len)
{
    check_expected_ptr(parameters);
    check_expected(para_len);
}

bool __wrap_si446x_set_preamble_len(uint8_t len)
{
    check_expected(len);

    return mock_type(bool);
}

bool __wrap_si446x_set_sync_word(uint8_t *sync_word, uint8_t len)
{
    check_expected_ptr(sync_word);
    check_expected(len);

    return mock_type(bool);
}

bool __wrap_si446x_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode)
{
    check_expected(gpio0_mode);
    check_expected(gpio1_mode);

    return mock_type(bool);
}

bool __wrap_si446x_set_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t len)
{
    check_expected(cmd);
    check_expected_ptr(para_buf);
    check_expected(len);

    return mock_type(bool);
}

bool __wrap_si446x_get_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t length)
{
    check_expected(cmd);
    check_expected(length);

    if (para_buf != NULL)
    {
        uint16_t i = 0;
        for(i = 0; i < length; i++)
        {
            para_buf[i] = mock_type(uint8_t);
        }
    }

    return mock_type(bool);
}

bool __wrap_si446x_set_tx_interrupt(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_set_rx_interrupt(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_clear_interrupts(void)
{
    return mock_type(bool);
}

void __wrap_si446x_write_tx_fifo(uint8_t *data, uint8_t len)
{
    check_expected_ptr(data);
    check_expected(len);
}

uint8_t __wrap_si446x_read_rx_fifo(uint8_t *data, uint8_t read_len)
{
    check_expected(read_len);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i = 0; i < read_len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(uint8_t);
}

void __wrap_si446x_fifo_reset(void)
{
    function_called();
}

void __wrap_si446x_enter_tx_mode(void)
{
    function_called();
}

void __wrap_si446x_enter_rx_mode(void)
{
    function_called();
}

bool __wrap_si446x_enter_standby_mode(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_wait_nirq(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_wait_packet_sent(void)
{
    return mock_type(bool);
}

bool __wrap_si446x_wait_gpio1(void)
{
    return mock_type(bool);
}

int __wrap_si446x_spi_init(void)
{
    return mock_type(int);
}

void __wrap_si446x_spi_enable(void)
{
    function_called();
}

void __wrap_si446x_spi_disable(void)
{
    function_called();
}

int __wrap_si446x_spi_write(uint8_t *data, uint16_t size)
{
    //check_expected_prt(data);
    check_expected(size);

    return mock_type(int);
}

int __wrap_si446x_spi_read(uint8_t *data, uint16_t size)
{
    check_expected(size);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i = 0; i < size; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }
    return mock_type(int);
}

uint8_t __wrap_si446x_spi_transfer(uint8_t byte)
{
    check_expected(byte);

    return mock_type(uint8_t);
}

void __wrap_si446x_gpio_init(void)
{
    function_called();
}

void __wrap_si446x_gpio_set_pin(si446x_gpio_pin_t pin)
{
    check_expected(pin);
}

void __wrap_si446x_gpio_clear_pin(si446x_gpio_pin_t pin)
{
    check_expected(pin);
}

int __wrap_si446x_gpio_get_pin(si446x_gpio_pin_t pin)
{
    check_expected(pin);

    return mock_type(int);
}

void __wrap_si446x_delay_s(uint8_t s)
{
    check_expected(s);
}

void __wrap_si446x_delay_ms(uint16_t ms)
{
    check_expected(ms);
}

void __wrap_si446x_delay_us(uint32_t us)
{
    check_expected(us);
}

int __wrap_si446x_mutex_take(void)
{
    return mock_type(int);
}

int __wrap_si446x_mutex_give(void)
{
    return mock_type(int);
}

/** \} End of si446x_wrap group */
