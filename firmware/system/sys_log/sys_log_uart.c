/*
 * sys_log_uart.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief System log UART implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.9
 * 
 * \date 2019/11/03
 * 
 * \defgroup sys_log_uart UART
 * \ingroup sys_log
 * \{
 */

#include <drivers/uart/uart.h>

#include "sys_log.h"

int sys_log_uart_init(void)
{
    uart_config_t config;

    config.baudrate     = 115200;
    config.data_bits    = 8;
    config.parity       = UART_NO_PARITY;
    config.stop_bits    = UART_ONE_STOP_BIT;

    return (uart_init(UART_PORT_1, config) == 0) ? 0 : -1;
}

void sys_log_uart_write_byte(uint8_t byte)
{
    uart_write(UART_PORT_1, &byte, 1);
}

/** \} End of sys_log_uart group */
