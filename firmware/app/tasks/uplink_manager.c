/*
 * uplink_manager.c
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
 * \brief Uplink Manager task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 *
 * \addtogroup uplink_manager
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/radio/radio.h>
#include <structs/ttc_data.h>
#include <ngham/ngham.h>

#include "uplink_manager.h"
#include "startup.h"

xTaskHandle xTaskUplinkManagerHandle;

void vTaskUplinkManager(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_UPLINK_MANAGER_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_UPLINK_MANAGER_INITIAL_DELAY_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Initializing the Uplink Manager...");
    sys_log_new_line();

    ttc_data_buf.radio.rx_fifo_counter = 0U;
    ttc_data_buf.radio.rx_packet_counter = 0U;
    ttc_data_buf.radio.last_rx_packet_bytes = 0U;

    ttc_data_buf.up_buf.position_to_read = 0U;
    ttc_data_buf.up_buf.position_to_write = 0U;

    uint8_t rx_packet[230] = {0};
    uint8_t ngham_decoded_packet[220] = {0};
    uint16_t ngham_decoded_packet_len = 0;

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        if (radio_available() == 0U)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Receiving a new package:");
            sys_log_new_line();

            if(radio_recv(rx_packet, 80U, 100U) > 0)
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Decoding packet...");
                sys_log_new_line();

                if(ngham_decode(rx_packet, 220, ngham_decoded_packet, &ngham_decoded_packet_len) == 0)
                {
                    uplink_add_packet(ngham_decoded_packet, ngham_decoded_packet_len);

                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Packet successfully received");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_UPLINK_MANAGER_NAME, "Failed to receive a new packet");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_UPLINK_MANAGER_NAME, "Failed to receive a new packet");
                sys_log_new_line();
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_UPLINK_MANAGER_PERIOD_MS));
    }
}

/** \} End of uplink_manager group */
