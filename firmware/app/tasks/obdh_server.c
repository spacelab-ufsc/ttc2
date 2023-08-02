/*
 * obdh_server.c
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
 * \brief OBDH Server task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.3
 *
 * \date 2023/03/03
 *
 * \addtogroup obdh_server
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/obdh/obdh.h>
#include <devices/radio/radio.h>
#include <system/cmdpr.h>
#include <drivers/uart/uart.h>
#include <app/structs/ttc_data.h>
#include <drivers/spi_slave/spi_slave.h>
#include <hal/dma.h>

#include "obdh_server.h"
#include "startup.h"

xTaskHandle xTaskObdhServerHandle;

void vTaskObdhServer(void)
{
    /* Wait startup task to finish */
     xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_OBDH_SERVER_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_OBDH_SERVER_INITIAL_DELAY_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "Initializing the OBDH server...");
    sys_log_new_line();

    obdh_request_t obdh_request = {0};
    obdh_response_t obdh_response = {0};
    obdh_request.command = 0x00U;   /* No command */

    ttc_data_buf.hw_version = 0x42;
    ttc_data_buf.device_id = 0xCC2A;
    ttc_data_buf.fw_version = 0xABCDEFCC;


    uint8_t buffer1[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer2[6] = {0x22, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Receiving data from obdh */

        obdh_read_request(&obdh_request);

        //sys_log_print_hex(obdh_request.command);
        //sys_log_new_line();

        if (obdh_request.command != 0xFF)
        {
            if (obdh_request.command == 0x01)
            {

                obdh_response.command = obdh_request.command;
                obdh_response.parameter = obdh_request.parameter;
                obdh_write_response_param(&ttc_data_buf, &obdh_response);

                obdh_send_response(&obdh_response);
            }
            else if (obdh_request.command == 0x02)
            {
                obdh_write_read_bytes(6);

                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "TX is now ");
                switch (obdh_request.data.param_8)
                {
                case 0x00:
                    sys_log_print_msg("Turned on.");
                    ttc_data_buf.radio.tx_enable = obdh_request.data.param_8;

                    break;

                case 0x01:
                    sys_log_print_msg("Turned off.");
                    ttc_data_buf.radio.tx_enable = obdh_request.data.param_8;

                    break;

                default:
                    sys_log_print_msg("Invalid mode: ");
                    sys_log_print_uint(obdh_request.data.param_8);

                    break;
                }
                sys_log_new_line();


            }
            else if(obdh_request.command == 0x03)
            {
                obdh_write_read_bytes(6);

                downlink_add_packet(obdh_request.data.data_packet.packet, (obdh_request.data.data_packet.len)+3);
            }
            else if(obdh_request.command == 0x00)
            {
                //spi_slave_dma_write(SPI_PORT_2, buffer1, 6);
                obdh_write_read_bytes(6);
            //    sys_log_print_msg("L");
             //   sys_log_new_line();
            }
        }
        /*for (int i=0;i<6;i++)
        {
            sys_log_print_hex(buffer[i]);
            sys_log_print_msg("|");

        }
        sys_log_new_line();

       /* switch(obdh_request.command)
        {
            case CMDPR_CMD_READ_PARAM:
                obdh_response.command = CMDPR_CMD_READ_PARAM;
                obdh_response.parameter = obdh_request.parameter;

           //     obdh_write_response_param(&ttc_data_buf, &obdh_response);
             //   obdh_send_response(&obdh_response);

               // spi_slave_dma_write(SPI_PORT_2, buffer, 6);

                break;
/*
            case CMDPR_CMD_WRITE_PARAM:
                if (obdh_request.parameter == CMDPR_PARAM_TX_ENABLE)
                {

                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "TX mode is now: ");
                    switch (obdh_request.data.param_8)
                    {
                    case 0x00:
                        sys_log_print_msg("Turned on.");
                        ttc_data_buf.radio.tx_enable = obdh_request.data.param_8;

                        break;
                    case 0x01:
                        sys_log_print_msg("Turned off.");
                        ttc_data_buf.radio.tx_enable = obdh_request.data.param_8;

                        break;
                    default:
                        sys_log_print_msg("Invalid mode!");

                        break;
                    }
                    sys_log_new_line();

                }

                break;

            case CMDPR_CMD_TRANSMIT_PACKET:
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "Received command to transmit:");
                sys_log_print_uint(obdh_request.data.data_packet.len);
                sys_log_print_msg(" bytes!");
                sys_log_new_line();

                sys_log_print_str("Packet: ");

                uint16_t i = 0;

                for(i = 0; i < obdh_request.data.data_packet.len; i++)
                {
                    sys_log_print_hex(obdh_request.data.data_packet.packet[i]);
                    sys_log_print_str("|");
                }

                sys_log_new_line();

                downlink_add_packet(obdh_request.data.data_packet.packet, obdh_request.data.data_packet.len);

                break;

            case CMDPR_CMD_READ_FIRST_PACKET:
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "Received command to read received packet. Reading: ");
                uplink_pop_packet(obdh_response.data.data_packet.packet, &obdh_response.data.data_packet.len);
                sys_log_print_uint(obdh_response.data.data_packet.len);
                sys_log_print_msg(" bytes...");
                sys_log_new_line();

                sys_log_print_str("Packet: ");

                for(i = 0; i < obdh_response.data.data_packet.len; i++)
                {
                    sys_log_print_hex(obdh_response.data.data_packet.packet[i]);
                    sys_log_print_str("|");
                }

                sys_log_new_line();

                obdh_send_response(obdh_response);

                break;

            case 0x00:

                break;

            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OBDH_SERVER_NAME, "Received invalid command (");
                sys_log_print_hex(obdh_request.command);
                sys_log_print_str(").");
                sys_log_new_line();

                obdh_flush_request(&obdh_request);

                break;
        }


        obdh_request.command = 0x00U;*/
     //       case 0x00:
                /* No command received, TX MODE */
      //  }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_OBDH_SERVER_PERIOD_MS));
    }
}

/** \} End of obdh_server group */
