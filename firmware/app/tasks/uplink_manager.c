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
 * \version 0.4.3
 *
 * \date 2023/04/03
 *
 * \addtogroup uplink_manager
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/radio/radio.h>
#include <structs/ttc_data.h>

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

    ttc_data_buf.radio.rx_fifo_counter = 0;
    ttc_data_buf.radio.rx_packet_counter = 0;

    ttc_data_buf.up_buf.position_to_read = 0;
    ttc_data_buf.up_buf.position_to_write = 0;

    uint16_t rx_size = 230;
    uint8_t rx_packet[230];
    uint8_t pop_rx_packet[230];
    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();


        if (radio_available())
        {
            radio_recv(rx_packet, rx_size, 10);

            /*TODO: Implement decodification */

            uplink_add_packet(rx_packet, rx_size);
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Received a new package: ");
            for (int i=0; i<230; i++)
            {
                sys_log_print_hex(rx_packet[i]);
                sys_log_print_msg("|");
            }
            sys_log_new_line();

        }
        /*Print the package for debug*/
        if (ttc_data_buf.radio.rx_fifo_counter > 0U)
        {
            uplink_pop_packet(&pop_rx_packet, 230);
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "Popping out a package: ");
            for (int i=0; i<230; i++)
            {
                sys_log_print_hex(pop_rx_packet[i]);
                sys_log_print_msg("|");
            }
            sys_log_new_line();
        }


    vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_UPLINK_MANAGER_PERIOD_MS));
    }
}

/** \} End of uplink_manager group */
