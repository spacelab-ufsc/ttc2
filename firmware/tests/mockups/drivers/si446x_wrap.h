/*
 * si446x_wrap.h
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
 * \brief Si446x driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.23
 * 
 * \date 2021/04/27
 * 
 * \defgroup si446x_wrap Si446x Wrap
 * \ingroup tests
 * \{
 */

#ifndef SI446X_WRAP_H_
#define SI446X_WRAP_H_

#include <stdint.h>
#include <stdbool.h>

#include <drivers/si446x/si446x.h>

int __wrap_si446x_init(void);

void __wrap_si446x_reg_config(void);

void __wrap_si446x_power_on_reset(void);

bool __wrap_si446x_tx_packet(uint8_t *data, uint8_t len);

bool __wrap_si446x_tx_long_packet(uint8_t *packet, uint16_t len);

uint8_t __wrap_si446x_rx_packet(uint8_t *rx_buf, uint8_t read_len);

bool __wrap_si446x_rx_init(void);

bool __wrap_si446x_check_device(void);

bool __wrap_si446x_check_cts(void);

bool __wrap_si446x_set_tx_power(uint8_t pwr);

bool __wrap_si446x_set_properties(uint16_t start_property, uint8_t *para_buf, uint8_t length);

bool __wrap_si446x_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf);

void __wrap_si446x_set_config(const uint8_t *parameters, uint16_t para_len);

bool __wrap_si446x_set_preamble_len(uint8_t len);

bool __wrap_si446x_set_sync_word(uint8_t *sync_word, uint8_t len);

bool __wrap_si446x_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode);

bool __wrap_si446x_set_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t len);

bool __wrap_si446x_get_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t length);

bool __wrap_si446x_set_tx_interrupt(void);

bool __wrap_si446x_set_rx_interrupt(void);

bool __wrap_si446x_clear_interrupts(void);

void __wrap_si446x_write_tx_fifo(uint8_t *data, uint8_t len);

uint8_t __wrap_si446x_read_rx_fifo(uint8_t *data, uint8_t read_len);

void __wrap_si446x_fifo_reset(void);

void __wrap_si446x_enter_tx_mode(void);

void __wrap_si446x_enter_rx_mode(void);

bool __wrap_si446x_enter_standby_mode(void);

bool __wrap_si446x_wait_nirq(void);

bool __wrap_si446x_wait_packet_sent(void);

bool __wrap_si446x_wait_gpio1(void);

int __wrap_si446x_spi_init(void);

void __wrap_si446x_spi_enable(void);

void __wrap_si446x_spi_disable(void);

int __wrap_si446x_spi_write(uint8_t *data, uint16_t size);

int __wrap_si446x_spi_read(uint8_t *data, uint16_t size);

uint8_t __wrap_si446x_spi_transfer(uint8_t byte);

void __wrap_si446x_gpio_init(void);

void __wrap_si446x_gpio_set_pin(si446x_gpio_pin_t pin);

void __wrap_si446x_gpio_clear_pin(si446x_gpio_pin_t pin);

int __wrap_si446x_gpio_get_pin(si446x_gpio_pin_t pin);

void __wrap_si446x_delay_s(uint8_t s);

void __wrap_si446x_delay_ms(uint16_t ms);

void __wrap_si446x_delay_us(uint32_t us);

#endif /* SI446X_WRAP_H_ */

/** \} End of si446x_wrap group */
