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
 * \version 0.4.5
 *
 * \date 2023/04/03
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

    uint8_t rx_packet[230] = {0U};

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        if (radio_available() == 0U)
        {
            radio_recv(rx_packet, 80U, 100U);
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Received a new package:");
            sys_log_dump_hex(rx_packet, 230U);
            sys_log_new_line();

            //ngham_decode(rx_packet); /* TODO */

            uplink_add_packet(rx_packet, 220U);
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_UPLINK_MANAGER_PERIOD_MS));
    }
}

/** \} End of uplink_manager group */
