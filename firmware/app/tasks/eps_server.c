/*
 * eps_server.c
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
 * \brief EPS Server task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.5
 *
 * \date 2023/03/03
 *
 * \addtogroup eps_server
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/eps/eps.h>
#include <devices/radio/radio.h>
#include <devices/eps/eps.h>
#include <system/cmdpr.h>
#include <drivers/uart/uart.h>

#include "eps_server.h"
#include "startup.h"

xTaskHandle xTaskEpsServerHandle;

void vTaskEpsServer(void)
{
    /* Wait startup task to finish */
     xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_EPS_SERVER_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_EPS_SERVER_INITIAL_DELAY_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_EPS_SERVER_NAME, "Initializing the EPS server");
    sys_log_new_line();

    eps_request_t eps_request = {0};
    eps_request.command = 0x00; /* No command */

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Receiving data from eps */
        eps_read_request(&eps_request);

        if (eps_request.command == CMDPR_CMD_TRANSMIT_PACKET)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_EPS_SERVER_NAME, "Received command to transmit ");
            sys_log_print_uint(eps_request.data.data_packet.len);
            sys_log_print_msg(" bytes!");
            sys_log_new_line();

            /* TODO: Implement data processing and radio link */
            sys_log_print_str("Packet: ");
            for (int i = 0; i < eps_request.data.data_packet.len; i++)
            {
                sys_log_print_hex(eps_request.data.data_packet.packet[i]);
                sys_log_print_str("|");
            }
            sys_log_new_line();
        }
        else if (eps_request.command == 0x00)
        {
            /* No request */
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_EPS_SERVER_NAME, "Received invalid command (");
            sys_log_print_hex(eps_request.command);
            sys_log_print_str(").");
            sys_log_new_line();

            eps_flush_request(&eps_request);
        }

        /* Resetting command */
        eps_request.command = 0x00;

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_EPS_SERVER_PERIOD_MS));
    }
}

/** \} End of eps_server group */
