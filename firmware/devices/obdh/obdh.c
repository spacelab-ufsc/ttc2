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
 * \version 0.4.3
 *
 * \date 2023/02/12
 *
 * \addtogroup obdh
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <system/sys_log/sys_log.h>

#include <system/cmdpr.h>
#include <drivers/spi_slave/spi_slave.h>
#include <app/structs/ttc_data.h>

#include "obdh.h"

static int obdh_read_parameter(uint8_t param, cmdpr_data_t *data);

static int obdh_write_parameter(obdh_request_t *obdh_request);

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
        //err = spi_slave_enable_isr(obdh_spi_port);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, "Error during OBDH initialization!");
        sys_log_new_line();
    }

    return err;
}

int obdh_read_request(obdh_request_t *obdh_request)
{
    int err = 0;
    uint8_t request[6] = {0};

    spi_slave_dma_read(obdh_spi_port, request, 6);

    obdh_request->command = request[0];


    if ((obdh_request->command != 0xFF) && (obdh_request->command != 0x00)) /* Received a request */
    {
        switch(obdh_request->command)
        {

        case CMDPR_CMD_READ_PARAM:
                    obdh_request->parameter = request[1];

                    sys_log_print_event_from_module(SYS_LOG_INFO, OBDH_MODULE_NAME, "Read command received, parameter:");
                    sys_log_print_hex(obdh_request->parameter);
                    sys_log_new_line();

                    break;

                /*case CMDPR_CMD_WRITE_PARAM:
                    obdh_request->parameter = request[1];

                    sys_log_print_event_from_module(SYS_LOG_INFO, OBDH_MODULE_NAME, "Write command received, parameter:");
                    sys_log_print_hex(obdh_request->parameter);
                    sys_log_new_line();

                    if (obdh_request->parameter == CMDPR_PARAM_TX_ENABLE)
                    {
                        obdh_request->data.param_8 = request[2];
                    }
                    else
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, "Unknown parameter!");
                        sys_log_new_line();
                        err = -1;
                    }
                    break;

                case CMDPR_CMD_TRANSMIT_PACKET:
                    uint8_t packet[230] = {0};
                    obdh_request->data.data_packet.len = request[1];
                    spi_slave_change_dma_transfer_size(obdh_spi_port, request[1], SPI_SLAVE_DMA_RX);

                    vTaskDelay(pdMS_TO_TICKS(100)); /* Wait for 105ms */
                   /* spi_slave_dma_read(obdh_spi_port, packet, 230);

                    sys_log_print_str("Packet: ");

                    for(uint16_t i = 0; i < 230; i++)
                    {
                        sys_log_print_hex(packet[i]);
                        sys_log_print_str("|");
                    }

                    sys_log_new_line();

                    spi_slave_change_dma_transfer_size(obdh_spi_port, 6, SPI_SLAVE_DMA_RX);

                    break;
                    /*
                case CMDPR_CMD_READ_FIRST_PACKET:
                    /* Nothing more to do */
                    //break;


        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, " Unknown command: ");
            sys_log_print_hex(obdh_request->command);
            sys_log_new_line();
            err = -1;
            break;
        }
    }
    else
    {
        /* TX mode*/
    }

    return err;
}

int obdh_send_response(obdh_response_t *obdh_response)
{
    int err = 0;

    switch(obdh_response->command)
    {
          case CMDPR_CMD_READ_PARAM:
              obdh_write_parameter(obdh_response);
              break;

          case CMDPR_CMD_READ_FIRST_PACKET:
              //obdh_write_packet(packet, len);
              //TODO
          default:
              err = -1;

              break;
    }

    return err;
}

int obdh_write_response_param(ttc_data_t *ttc_data_buf, obdh_response_t *obdh_response)
{
    int err = 0;

    if (obdh_response->command == CMDPR_CMD_READ_PARAM)
    {
        switch(obdh_response->parameter)
        {
            case CMDPR_PARAM_HW_VER:
                obdh_response->data.param_8 = ttc_data_buf->hw_version;
                break;

            case CMDPR_PARAM_FW_VER:
                obdh_response->data.param_32 = ttc_data_buf->fw_version;
                break;

            case CMDPR_PARAM_COUNTER:
                obdh_response->data.param_32 = ttc_data_buf->timestamp;
                break;

            case CMDPR_PARAM_RST_COUNTER:
                obdh_response->data.param_16 = ttc_data_buf->reset_counter;
                break;

            case CMDPR_PARAM_DEVICE_ID:
                obdh_response->data.param_16 = ttc_data_buf->device_id;
                break;

            case CMDPR_PARAM_LAST_RST_CAUSE:
                obdh_response->data.param_8 = ttc_data_buf->last_reset_cause;
                break;

            case CMDPR_PARAM_UC_VOLTAGE:
                obdh_response->data.param_16 = ttc_data_buf->voltage;
                break;

            case CMDPR_PARAM_UC_CURRENT:
                obdh_response->data.param_16 = ttc_data_buf->current;
                break;

            case CMDPR_PARAM_UC_TEMP:
                obdh_response->data.param_16 = ttc_data_buf->temperature;
                break;

            case CMDPR_PARAM_RADIO_VOLTAGE:
                obdh_response->data.param_16 = ttc_data_buf->radio.voltage;
                break;

            case CMDPR_PARAM_RADIO_CURRENT:
                obdh_response->data.param_16 = ttc_data_buf->radio.current;
                break;

            case CMDPR_PARAM_RADIO_TEMP:
                obdh_response->data.param_16 = ttc_data_buf->radio.temperature;
                break;

            case CMDPR_PARAM_LAST_UP_COMMAND:
                obdh_response->data.param_8 = ttc_data_buf->radio.last_valid_tm;
                break;

            case CMDPR_PARAM_LAST_COMMAND_RSSI:
                obdh_response->data.param_16 = ttc_data_buf->radio.rssi;
                break;

            case CMDPR_PARAM_ANT_TEMP:
                obdh_response->data.param_16 = ttc_data_buf->antenna.temperature;
                break;

            case CMDPR_PARAM_ANT_MOD_STATUS_BITS:
                obdh_response->data.param_16 = ttc_data_buf->antenna.status.code;
                break;

            case CMDPR_PARAM_ANT_DEP_STATUS:
                obdh_response->data.param_8 = ttc_data_buf->ant_deploy;
                break;

            case CMDPR_PARAM_ANT_DEP_HIB:
                obdh_response->data.param_8 = ttc_data_buf->ant_deploy_hib;
                break;

            case CMDPR_PARAM_TX_ENABLE:
                obdh_response->data.param_8 = ttc_data_buf->radio.tx_enable;
                break;

            case CMDPR_PARAM_TX_PACKET_COUNTER:
                obdh_response->data.param_32 = ttc_data_buf->radio.tx_packet_counter;
                break;

            case CMDPR_PARAM_RX_VAL_PACKET_COUNTER:
                obdh_response->data.param_32 = ttc_data_buf->radio.rx_packet_counter;
                break;

            case CMDPR_PARAM_PACKETS_AV_FIFO_TX:
                obdh_response->data.param_8 = ttc_data_buf->radio.tx_fifo_counter;
                break;

            case CMDPR_PARAM_PACKETS_AV_FIFO_RX:
                obdh_response->data.param_8 = ttc_data_buf->radio.rx_fifo_counter;
                break;

            case CMDPR_PARAM_N_BYTES_FIRST_AV_RX:
                obdh_response->data.param_16 = ttc_data_buf->radio.last_rx_packet_bytes;
                break;

            default:

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
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, "Error reading OBDH parameter: unknown parameter!");
            sys_log_new_line();
            break;
    }

    if (err == -1)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, "Error reading OBDH parameter: unable to read parameter!");
        sys_log_new_line();
    }

    return err;
}

static int obdh_write_parameter(obdh_response_t *obdh_response)
{
    int err = 0;
    uint8_t response[6] = {0};

    response[0] = obdh_response->command;
    response[1] = obdh_response->parameter;

    switch(cmdpr_param_size(obdh_response->parameter))
    {
        case 1:
            response[2] = obdh_response->data.param_8;

            break;
        case 2:
            response[2] = (uint8_t)((obdh_response->data.param_16 >> 8) & 0xFFU);
            response[3] = (uint8_t)(obdh_response->data.param_16 & 0xFFU);

            break;
        case 4:
            response[2] = (uint8_t)((obdh_response->data.param_32 >> 24) & 0xFFU);
            response[3] = (uint8_t)((obdh_response->data.param_32 >> 16) & 0xFFU);
            response[4] = (uint8_t)((obdh_response->data.param_32 >> 8) & 0xFFU);
            response[5] = (uint8_t)(obdh_response->data.param_32 & 0xFFU);

            break;

        default:
            err = -1;
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, "Error writing OBDH parameter: unknown parameter!");
            sys_log_new_line();
            break;
    }

    if (err == 0)
    {
        spi_slave_dma_write(obdh_spi_port, response, 6);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_NAME, "Error writing OBDH parameter: unable to write parameter!");
        sys_log_new_line();
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
