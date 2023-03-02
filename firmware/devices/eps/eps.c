/*
 * eps.c
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
 * \brief EPS device implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.3
 *
 * \date 2023/02/12
 *
 * \addtogroup eps
 * \{
 */

#include <system/cmdpr.h>
#include <drivers/uart/uart.h>

#include "eps.h"

static int eps_read_parameter(uint8_t param, cmdpr_data_t *data);

static const uart_ports_e eps_uart_port = UART_PORT_0;

int eps_init(void)
{
    int err = -1;

    uart_config_t config;
    config.clock = 1000000;
    config.baudrate = 115200;
    config.data_bits = 8;
    config.parity = UART_NO_PARITY;
    config.stop_bits = UART_ONE_STOP_BIT;

    if (uart_init(eps_uart_port, config) == 0)
    {
        err = uart_rx_enable(eps_uart_port);
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_ERROR, "Error during EPS initialization !");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}


int eps_read_request(eps_request_t *eps_request)
{
    int err = 0;

    if (uart_read_available(eps_uart_port) > 0)
    {
        if (uart_read(eps_uart_port, &(eps_request->command), 1) == 0)
        {
            switch(eps_request->command)
            {
                case CMDPR_CMD_READ_PARAM:
                    err = uart_read(eps_uart_port, &(eps_request->parameter), 1);
                    break;
                case CMDPR_CMD_WRITE_PARAM:
                    if (uart_read(eps_uart_port, &(eps_request->parameter), 1) == 0)
                    {
                        err = eps_read_parameter(eps_request->parameter, &(eps_request->data));
                    }
                    else
                    {
                        err = -1;
                    }
                    break;
                case CMDPR_CMD_TRANSMIT_PACKET:
                    eps_request->data.data_packet.len = uart_read_available(eps_uart_port);
                    err = uart_read(eps_uart_port, eps_request->data.data_packet.packet, eps_request->data.data_packet.len);
                    break;
                case CMDPR_CMD_READ_FIRST_PACKET:
                    /* Nothing more to do */
                    break;
                default:
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_ERROR, "Error reading EPS command: unknown command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    err = -1;
                    break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_ERROR, "Error reading EPS command: unable to read command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

static int eps_read_parameter(uint8_t param, cmdpr_data_t *data)
{
    int err = 0;
    uint8_t read_buffer[4] = {0};
    switch(cmdpr_param_size(param))
    {
        case 1:
            err = uart_read(eps_uart_port, &(data->param_8), 1);
            break;
        case 2:
            err = uart_read(eps_uart_port, read_buffer, 2);
            data->param_16 = ((uint16_t)read_buffer[0]) | ((uint16_t)(read_buffer[1])<<8);
            break;
        case 4:
            err = uart_read(eps_uart_port, read_buffer, 4);
            data->param_32 = ((uint32_t)read_buffer[0]) | ((uint32_t)(read_buffer[1])<<8) | ((uint32_t)(read_buffer[2])<<16) | ((uint32_t)(read_buffer[3])<<24);
            break;
        default:
            err = -1;
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_ERROR, "Error reading EPS parameter: unknown parameter!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }
    if (err == -1)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_ERROR, "Error reading EPS parameter: unable to read parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

/** \} End of eps group */

