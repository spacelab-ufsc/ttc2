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
 * \version 0.3.4
 *
 * \date 2023/02/12
 *
 * \addtogroup obdh
 * \{
 */

#include <system/cmdpr.h>
#include <drivers/spi_slave/spi_slave.h>
#include <app/structs/ttc_data.h>

#include "obdh.h"

static int obdh_read_parameter(uint8_t param, cmdpr_data_t *data);

static int obdh_write_parameter(uint8_t param, cmdpr_data_t data);

static int obdh_write_packet(uint8_t *packet, uint16_t len);

static const spi_port_t obdh_spi_port = SPI_PORT_2;

int obdh_init(void)
{
    int err = -1;

    spi_config_t spi_slave_config;
    spi_slave_config.mode = SPI_MODE_0;
    spi_slave_config.speed_hz = 0U; /* Parameter not used in slave mode */

    if (spi_slave_init(obdh_spi_port, spi_slave_config) == 0)
    {
        err = spi_slave_enable_isr(obdh_spi_port);
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

    if (spi_slave_read_available(obdh_spi_port) > 0)
    {
        if (spi_slave_read(obdh_spi_port, &(obdh_request->command), 1) == 0)
        {
            switch(obdh_request->command)
            {
                case CMDPR_CMD_READ_PARAM:
                    err = spi_slave_read(obdh_spi_port, &(obdh_request->parameter), 1);
                    break;
                case CMDPR_CMD_WRITE_PARAM:
                    if (spi_slave_read(obdh_spi_port, &(obdh_request->parameter), 1) == 0)
                    {
                        err = obdh_read_parameter(obdh_request->parameter, &(obdh_request->data));
                    }
                    else
                    {
                        err = -1;
                    }
                    break;
                case CMDPR_CMD_TRANSMIT_PACKET:
                    obdh_request->data.data_packet.len = spi_slave_read_available(obdh_spi_port);
                    err = spi_slave_read(obdh_spi_port, obdh_request->data.data_packet.packet, obdh_request->data.data_packet.len);
                    break;
                case CMDPR_CMD_READ_FIRST_PACKET:
                    /* Nothing more to do */
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

    return err;
}

int obdh_send_response(obdh_response_t obdh_response)
{
    int err = 0;

    if (spi_slave_write(obdh_spi_port, &(obdh_response.command), 1) == 0)
    {
       switch(obdh_response.command)
       {
          case CMDPR_CMD_READ_PARAM:
              /* Send the response parameter */
              if ((spi_slave_write(obdh_spi_port, &(obdh_response.parameter), 1) == 0) &&
              (obdh_write_parameter(obdh_response.parameter, obdh_response.data) == 0))
              {
                  err = 0;
              }
              else
              {
                  err = -1;
              }

              break;

          case CMDPR_CMD_READ_FIRST_PACKET:
              /* Send the response packet */
              err = obdh_write_packet(obdh_response.data.data_packet.packet, obdh_response.data.data_packet.len);

              break;

          default:
              err = -1;

              break;
       }
    }
    else
    {
        err = -1;
    }

    return err;
}

int obdh_write_response_param(ttc_data_t ttc_data_buf, obdh_response_t *obdh_response)
{
    int err = 0;

    if (obdh_response->command == CMDPR_CMD_READ_PARAM)
    {
        switch(obdh_response->parameter)
        {
            case CMDPR_PARAM_HW_VER:
                obdh_response->data.param_8 = ttc_data_buf.hw_version;
                break;

            case CMDPR_PARAM_FW_VER:
                obdh_response->data.param_32 = ttc_data_buf.fw_version;
                break;

            case CMDPR_PARAM_COUNTER:
                obdh_response->data.param_32 = ttc_data_buf.timestamp;
                break;

            case CMDPR_PARAM_RST_COUNTER:
                obdh_response->data.param_16 = ttc_data_buf.reset_counter;
                break;

            case CMDPR_PARAM_DEVICE_ID:
                //TODO Add to struct device ID
                break;

            case CMDPR_PARAM_LAST_RST_CAUSE:
                obdh_response->data.param_8 = ttc_data_buf.last_reset_cause;
                break;

            case CMDPR_PARAM_UC_VOLTAGE:
                obdh_response->data.param_16 = ttc_data_buf.voltage;
                break;

            case CMDPR_PARAM_UC_CURRENT:
                obdh_response->data.param_16 = ttc_data_buf.current;
                break;

            case CMDPR_PARAM_UC_TEMP:
                obdh_response->data.param_16 = ttc_data_buf.temperature;
                break;

            case CMDPR_PARAM_RADIO_VOLTAGE:
                obdh_response->data.param_16 = ttc_data_buf.radio.voltage;
                break;

            case CMDPR_PARAM_RADIO_CURRENT:
                obdh_response->data.param_16 = ttc_data_buf.radio.current;
                break;

            case CMDPR_PARAM_RADIO_TEMP:
                obdh_response->data.param_16 = ttc_data_buf.radio.temperature;
                break;

            case CMDPR_PARAM_LAST_UP_COMMAND:
                obdh_response->data.param_8 = ttc_data_buf.radio.last_valid_tm;
                break;

            case CMDPR_PARAM_LAST_COMMAND_RSSI:
                obdh_response->data.param_16 = ttc_data_buf.radio.rssi;
                break;

            case CMDPR_PARAM_ANT_TEMP:
                //TODO
                break;

            case CMDPR_PARAM_ANT_MOD_STATUS_BITS:
                //TODO
                break;

            case CMDPR_PARAM_ANT_DEP_STATUS:
                //TODO
                break;

            case CMDPR_PARAM_ANT_DEP_HIB:
                //TODO
                break;

            case CMDPR_PARAM_TX_ENABLE:
                //TODO
                break;

            case CMDPR_PARAM_TX_PACKET_COUNTER:
                //TODO
                break;

            case CMDPR_PARAM_RX_VAL_PACKET_COUNTER:
                //TODO
                break;

            case CMDPR_PARAM_PACKETS_AV_FIFO_TX:
                //TODO
                break;

            case CMDPR_PARAM_PACKETS_AV_FIFO_RX:
                //TODO
                break;

            case CMDPR_PARAM_N_BYTES_FIRST_AV_RX:
                //TODO
                break;
        }
    }
    else
    {
        err = -1;
    }

    return err;
}

int obdh_flush_request(obdh_request_t *obdh_request)
{
    obdh_request->data.data_packet.len = 0;

    return spi_slave_flush(obdh_spi_port);
}

static int obdh_read_parameter(uint8_t param, cmdpr_data_t *data)
{
    int err = 0;
    uint8_t read_buffer[4] = {0};
    switch(cmdpr_param_size(param))
    {
        case 1:
            err = spi_slave_read(obdh_spi_port, &(data->param_8), 1);
            break;
        case 2:
            err = spi_slave_read(obdh_spi_port, read_buffer, 2);
            data->param_16 = ((uint16_t)read_buffer[0]) | ((uint16_t)(read_buffer[1])<<8);
            break;
        case 4:
            err = spi_slave_read(obdh_spi_port, read_buffer, 4);
            data->param_32 = ((uint32_t)read_buffer[0]) | ((uint32_t)(read_buffer[1])<<8) | ((uint32_t)(read_buffer[2])<<16) | ((uint32_t)(read_buffer[3])<<24);
            break;
        default:
            err = -1;
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error reading OBDH parameter: unknown parameter!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }
    if (err == -1)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error reading OBDH parameter: unable to read parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

static int obdh_write_parameter(uint8_t param, cmdpr_data_t data)
{
    int err = 0;
    uint8_t write_buffer[4] = {0};
    switch(cmdpr_param_size(param))
    {
        case 1:
            err = spi_slave_write(obdh_spi_port, &(data.param_8), 1);
            break;
        case 2:
            write_buffer[0] = (uint8_t)(data.param_16 & 0xFFU);
            write_buffer[1] = (uint8_t)((data.param_16 >> 8) & 0xFFU);
            err = spi_slave_write(obdh_spi_port, write_buffer, 2);
            break;
        case 4:
            write_buffer[0] = (uint8_t)(data.param_32 & 0xFFU);
            write_buffer[1] = (uint8_t)((data.param_32 >> 8) & 0xFFU);
            write_buffer[2] = (uint8_t)((data.param_32 >> 16) & 0xFFU);
            write_buffer[3] = (uint8_t)((data.param_32 >> 24) & 0xFFU);
            err = spi_slave_write(obdh_spi_port, write_buffer, 4);
            break;
        default:
            err = -1;
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error writing OBDH parameter: unknown parameter!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    if (err == -1)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error writing OBDH parameter: unable to write parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

static int obdh_write_packet(uint8_t *packet, uint16_t len)
{
    int err = -1;

    err = spi_slave_write(obdh_spi_port, packet, len);

    return err;
}

/** \} End of obdh group */
