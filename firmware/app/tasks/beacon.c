/*
 * beacon.c
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
 * \brief Beacon task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.4
 * 
 * \date 2019/10/27
 * 
 * \addtogroup beacon
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <fsat_pkt/fsat_pkt.h>
#include <ngham/ngham.h>
#include <config/config.h>

#include "beacon.h"
#include "startup.h"

xTaskHandle xTaskBeaconHandle;

void vTaskBeacon(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_BEACON_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_BEACON_INITIAL_DELAY_MS));

    uint8_t tx_pkt[30] = {0U};
    uint16_t tx_pkt_len = UINT8_MAX;

    uint8_t ngham_pkt[60] = {0U};
    uint16_t ngham_pkt_len = UINT16_MAX;

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Beacon data */
        uint8_t data[30] = {0};
        uint16_t data_len = 1;

        fsat_pkt_pl_t beacon_pl;

        /* Packet ID */
        fsat_pkt_add_id(&beacon_pl, CONFIG_PKT_ID_BEACON);

        /* Source callsign */
        fsat_pkt_add_callsign(&beacon_pl, CONFIG_SATELLITE_CALLSIGN);

        /* Payload data */
        fsat_pkt_add_payload(&beacon_pl, data, data_len);

        fsat_pkt_encode(beacon_pl, tx_pkt, &tx_pkt_len);

        if (ngham_encode(tx_pkt, tx_pkt_len, 0U, ngham_pkt, &ngham_pkt_len) == 0)
        {
            radio_send(&ngham_pkt[8], ngham_pkt_len);   /* 8 = Removing preamble and sync word */
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_BEACON_NAME, "Error encoding a NGHam packet!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_BEACON_PERIOD_MS));
    }
}

/** \} End of beacon group */
