/*
 * downlink_manager.c
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
 * \brief Downlink Manager task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.3
 *
 * \date 2023/03/03
 *
 * \addtogroup downlink_manager
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/radio/radio.h>
#include <structs/ttc_data.h>

#include "downlink_manager.h"
#include "startup.h"

xTaskHandle xTaskDownlinkManagerHandle;

void vTaskDownlinkManager(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_DOWNLINK_MANAGER_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_DOWNLINK_MANAGER_INITIAL_DELAY_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_DOWNLINK_MANAGER_NAME, "Initializing the Downlink Manager...");
    sys_log_new_line();

    ttc_data_buf.down_buf.position_to_read = 0;
    ttc_data_buf.down_buf.position_to_write = 0;

    uint8_t tx_packet[230];
    uint16_t tx_size = 0;

    while(1)
     {
        while((ttc_data_buf.radio.tx_fifo_counter > 0) && (ttc_data_buf.radio.tx_enable == 1))
        {
            downlink_pop_packet(tx_packet, &tx_size);

            /*TODO: Implement codification */
            radio_send(tx_packet, tx_size);
        }
     }
}

/** \} End of downlink_manager group */
