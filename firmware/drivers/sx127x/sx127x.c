/*
 * sx127x.c
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
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SX127x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 2021/12/14
 * 
 * \addtogroup sx127x
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sx127x.h"

int sx127x_init(void)
{
    return -1;
}

int sx127x_rx_init(void)
{
    return -1;
}

int sx127x_tx_packet(uint8_t *data, uint16_t len)
{
    return -1;
}

int sx127x_rx_packet(uint8_t *data, uint16_t *len)
{
    return -1;
}

int sx127x_wait_irq(uint8_t irq_mask)
{
    return -1;
}

int sx127x_enter_standby_mode(void)
{
    return -1;
}

int sx127x_enter_sleep_mode(void)
{
    return -1;
}

int sx127x_clear_interrupt(void)
{
    return -1;
}

int sx127x_set_frequency(uint32_t freq)
{
    return -1;
}

int sx127x_set_rf_param(uint8_t bw, uint8_t cr, uint8_t sf, uint8_t crc)
{
    return -1;
}

int sx127x_set_preamble_len(uint16_t len)
{
    return -1;
}

int sx127x_set_header_mode(uint8_t mode)
{
    return -1;
}

int sx127x_set_payload_len(uint8_t len)
{
    return -1;
}

int sx127x_tx_power(uint8_t pwr)
{
    return -1;
}

int sx127x_set_rx_timeout(uint16_t symb_timeout)
{
    return -1;
}

int sx127x_read_rssi(uint8_t mode)
{
    return -1;
}

/** \} End of sx127x group */
