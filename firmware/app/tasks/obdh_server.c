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
 * \version 1.0.0
 *
 * \date 2024/09/09
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

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Receiving data from obdh */
        if (obdh_read_request(&obdh_request) != -1)
        {
            if (obdh_request.command != 0xFF)
            {
                taskENTER_CRITICAL();

                switch(obdh_request.command)
                {
                    case CMDPR_CMD_READ_PARAM:
                        obdh_response.command = obdh_request.command;
                        obdh_response.parameter = obdh_request.parameter;
                        obdh_write_response_param(&ttc_data_buf, &obdh_response);

                        obdh_send_response(&obdh_response);

                        break;
                    case CMDPR_CMD_WRITE_PARAM:
                        obdh_write_read_bytes(7);

                        switch(obdh_request.parameter)
                        {
                        case CMDPR_PARAM_TX_ENABLE:
                            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "TX is now ");

                            switch(obdh_request.data.param_8)
                            {
                            case 0x00:
                                sys_log_print_msg("Turned off.");
                                ttc_data_buf.radio.tx_enable = obdh_request.data.param_8;

                                break;
                            case 0x01:
                                sys_log_print_msg("Turned on.");
                                ttc_data_buf.radio.tx_enable = obdh_request.data.param_8;

                                break;
                            default:

                                sys_log_print_msg("Invalid mode: ");
                                sys_log_print_uint(obdh_request.data.param_8);

                                break;
                            }

                            sys_log_new_line();

                            break;
                        case CMDPR_PARAM_RESET_DEVICE:
                            if (obdh_request.data.param_8 == 0x01)
                            {
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "Received command to reset system...");
                                system_reset();
                            }
                            break;

                        case CMDPR_PARAM_TIMESTAMP:
                            ttc_data_buf.timestamp = obdh_request.data.param_32;

                            system_set_time((sys_time_t)obdh_request.data.param_32);

                            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_OBDH_SERVER_NAME, "Updating system time to ");
                            sys_log_print_uint(ttc_data_buf.timestamp);
                            sys_log_print_msg(".");
                            sys_log_new_line();

                            break;

                        default:
                            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OBDH_SERVER_NAME, "Invalid write parameter.");

                            break;
                        }
                        break;
                    case CMDPR_CMD_TRANSMIT_PACKET:
                        obdh_write_read_bytes(7);

                        downlink_add_packet(obdh_request.data.data_packet.packet, obdh_request.data.data_packet.len);

                        break;
                    case CMDPR_CMD_READ_FIRST_PACKET:
                        obdh_response.command = obdh_request.command;

                        uplink_pop_packet(obdh_response.data.data_packet.packet, &(obdh_response.data.data_packet.len));

                        obdh_send_response(&obdh_response);

                        obdh_write_read_bytes(7U);

                        break;
                    case 0x00:
                        /* Read mode */
                        obdh_write_read_bytes(7U);

                        break;
                    default:
                        break;
                }

                taskEXIT_CRITICAL();
            }
        }
        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_OBDH_SERVER_PERIOD_MS));
    }
}

/** \} End of obdh_server group */
