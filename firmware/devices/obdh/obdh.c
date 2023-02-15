/*
 * obdh.c
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
 * \brief OBDH device implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.3
 *
 * \date 2023/02/12
 *
 * \addtogroup obdh
 * \{
 */

#include <drivers/spi_slave/spi_slave.h>

#include "obdh.h"

static int obdh_read_parameter(uint8_t param, uint32_t *param_value);

int obdh_init(void)
{
    int err = -1;

    spi_config_t spi_slave_config;
    spi_slave_config.mode = SPI_MODE_0;
    spi_slave_config.speed_hz = 0U; /* Parameter not used in slave mode */

    if (spi_slave_init(SPI_PORT_2, spi_slave_config) == 0)
    {
        err = spi_slave_enable_isr(SPI_PORT_2);
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error during OBDH initialization !");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int obdh_read_request(obdh_request_t *obdh_request)
{
    int err = 0;

    if (spi_slave_read_available(SPI_PORT_2) > 0)
    {
        if (spi_slave_read(SPI_PORT_2, &(obdh_request->obdh_command), 1) == 0)
        {
            switch(obdh_request->obdh_command)
            {
            case OBDH_CMD_READ_PARAM:
                err = spi_slave_read(SPI_PORT_2, &(obdh_request->obdh_parameter), 1);
                break;
            case OBDH_CMD_WRITE_PARAM:
                if (spi_slave_read(SPI_PORT_2, &(obdh_request->obdh_parameter), 1) == 0)
                {
                    err = obdh_read_parameter(obdh_request->obdh_parameter, &(obdh_request->obdh_data));
                }
                else
                {
                    err = -1;
                }

                break;
            case OBDH_CMD_TRANSMIT_PACKET:
                //TODO: Implement code to receive the packet to be transmitted.
                break;
            case OBDH_CMD_READ_FIRST_PACKET:
                break;
            default:
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error reading OBDH command: unknown command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;
                break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error reading OBDH command: unable to read command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    }

    return err;
}

int obdh_send_packet(uint8_t *packet, uint16_t len)
{
    int err = -1;

    err = spi_slave_write(SPI_PORT_2, packet, len);

    return err;
}

static int obdh_read_parameter(uint8_t param, uint32_t *param_value)
{
    int err = 0;
    uint8_t read_buffer[4] = {0};
    /* Read uint8_t param */
    if ((param == OBDH_PARAM_HW_VER) || (param == OBDH_PARAM_LAST_RST_CAUSE) || (param == OBDH_PARAM_LAST_UP_COMMAND) ||
       (param == OBDH_PARAM_ANT_DEP_STATUS) || (param == OBDH_PARAM_ANT_DEP_HIB) || (param == OBDH_PARAM_TX_ENABLE) ||
       (param == OBDH_PARAM_PACKETS_AV_FIFO_RX) || (param == OBDH_PARAM_PACKETS_AV_FIFO_TX))
    {
        err = spi_slave_read(SPI_PORT_2, read_buffer, 1);
    }
    /* Read uint16_t param */
    else if ((param == OBDH_PARAM_DEVICE_ID) || (param == OBDH_PARAM_RST_COUNTER) || (param == OBDH_PARAM_UC_VOLTAGE) ||
            (param == OBDH_PARAM_UC_CURRENT) || (param == OBDH_PARAM_UC_TEMP) || (param == OBDH_PARAM_RADIO_VOLTAGE) ||
            (param == OBDH_PARAM_RADIO_CURRENT) || (param == OBDH_PARAM_RADIO_TEMP) || (param == OBDH_PARAM_LAST_COMMAND_RSSI) ||
            (param == OBDH_PARAM_ANT_TEMP) || (param == OBDH_PARAM_ANT_MOD_STATUS_BITS) || (param == OBDH_PARAM_N_BYTES_FIRST_AV_RX))
    {
        err = spi_slave_read(SPI_PORT_2, read_buffer, 2);
    }
    /* Read uint32_t param */
    else if ((param == OBDH_PARAM_FW_VER) || (param == OBDH_PARAM_COUNTER) ||
            (param == OBDH_PARAM_TX_PACKET_COUNTER) || (param == OBDH_PARAM_RX_VAL_PACKET_COUNTER))
    {
        err = spi_slave_read(SPI_PORT_2, read_buffer, 4);
    }
    /* Unknown param */
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error reading OBDH parameter: unknown parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    err = -1;
    }
    if (err == -1)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error reading OBDH parameter: unable to read parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }
    else
    {
        *param_value = ((uint32_t)read_buffer[0]) | ((uint32_t)read_buffer[1]<<8) | ((uint32_t)read_buffer[2]<<16) | ((uint32_t)read_buffer[1]<<24);
    }

    return err;
}