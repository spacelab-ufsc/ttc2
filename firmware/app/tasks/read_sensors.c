/*
 * read_sensors.h
 * 
 * Copyright (C) 2021, SpaceLab.
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
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Read onboard sensors task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.25
 * 
 * \date 2020/07/12
 * 
 * \addtogroup read_sensors
 * \{
 */

#include <devices/temp_sensor/temp_sensor.h>
#include <devices/power_sensor/power_sensor.h>

#include <structs/ttc_data.h>

#include "read_sensors.h"
#include "startup.h"

xTaskHandle xTaskReadSensorsHandle;

void vTaskReadSensors(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_SENSORS_INIT_TIMEOUT_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        uint16_t buf = 0;

        /* uC temperature */
        if (temp_sensor_read_raw(&buf) == 0)
        {
            ttc_data_buf.temperature = buf;
        }

        /* TTC voltage */
        if (power_sensor_read_voltage_mv(&buf) == 0)
        {
            ttc_data_buf.voltage = buf;
        }

        /* TTC current */
        if (power_sensor_read_current_ma(&buf) == 0)
        {
            ttc_data_buf.current = buf;
        }

        /* TTC power */
        if (power_sensor_read_power_mw(&buf) == 0)
        {
            ttc_data_buf.current = buf;
        }

        /* Radio temperature */
        if (radio_get_temperature(&buf) == 0)
        {
            ttc_data_buf.radio.temperature = buf;
        }

        /* Radio RSSI */
        if (radio_get_rssi(&buf) == 0)
        {
            ttc_data_buf.radio.rssi = buf;
        }

        /* Data timestamp */
        ttc_data_buf.timestamp = (uint32_t)xTaskGetTickCount();

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_SENSORS_PERIOD_MS));
    }
}

/** \} End of read_sensors group */
