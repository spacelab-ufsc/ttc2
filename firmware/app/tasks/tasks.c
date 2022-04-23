/*
 * tasks.c
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
 * \brief Tasks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.20
 * 
 * \date 2019/11/02
 * 
 * \addtogroup tasks
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <config/config.h>

#include "tasks.h"
#include "startup.h"
#include "watchdog_reset.h"
#include "heartbeat.h"
#include "system_reset.h"
#include "radio_reset.h"
#include "read_sensors.h"
#include "beacon.h"
#include "uplink.h"
#include "time_control.h"

void create_tasks(void)
{
    /* Startup task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskStartup, TASK_STARTUP_NAME, TASK_STARTUP_STACK_SIZE, NULL, TASK_STARTUP_PRIORITY, &xTaskStartupHandle);

    if (xTaskStartupHandle == NULL)
    {
        /* Error creating the startup task */
    }
#endif /* CONFIG_TASK_STARTUP_ENABLED */

    /* Watchdog reset task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskWatchdogReset, TASK_WATCHDOG_RESET_NAME, TASK_WATCHDOG_RESET_STACK_SIZE, NULL, TASK_WATCHDOG_RESET_PRIORITY, &xTaskWatchdogResetHandle);

    if (xTaskWatchdogResetHandle == NULL)
    {
        /* Error creating the watchdog reset task */
    }
#endif /* CONFIG_TASK_WATCHDOG_RESET_ENABLED */

    /* Heartbeat task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskHeartbeat, TASK_HEARTBEAT_NAME, TASK_HEARTBEAT_STACK_SIZE, NULL, TASK_HEARTBEAT_PRIORITY, &xTaskHeartbeatHandle);

    if (xTaskHeartbeatHandle == NULL)
    {
        /* Error creating the heartbeat task */
    }
#endif /* CONFIG_TASK_HEARTBEAT_ENABLED */

#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskSystemReset, TASK_SYSTEM_RESET_NAME, TASK_SYSTEM_RESET_STACK_SIZE, NULL, TASK_SYSTEM_RESET_PRIORITY, &xTaskSystemResetHandle);

    if (xTaskSystemResetHandle == NULL)
    {
        /* Error creating the system reset task */
    }
#endif /* CONFIG_TASK_SYSTEM_RESET_ENABLED */

#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskRadioReset, TASK_RADIO_RESET_NAME, TASK_RADIO_RESET_STACK_SIZE, NULL, TASK_RADIO_RESET_PRIORITY, &xTaskRadioResetHandle);

    if (xTaskRadioResetHandle == NULL)
    {
        /* Error creating the radio reset task */
    }
#endif /* CONFIG_TASK_RADIO_RESET_ENABLED */

#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskReadSensors, TASK_READ_SENSORS_NAME, TASK_READ_SENSORS_STACK_SIZE, NULL, TASK_READ_SENSORS_PRIORITY, &xTaskReadSensorsHandle);

    if (xTaskReadSensorsHandle == NULL)
    {
        /* Error creating the read sensors task */
    }
#endif /* CONFIG_TASK_READ_TEMP_ENABLED */

#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskBeacon, TASK_BEACON_NAME, TASK_BEACON_STACK_SIZE, NULL, TASK_BEACON_PRIORITY, &xTaskBeaconHandle);

    if (xTaskBeaconHandle == NULL)
    {
        /* Error creating the beacon task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskUplink, TASK_UPLINK_NAME, TASK_UPLINK_STACK_SIZE, NULL, TASK_UPLINK_PRIORITY, &xTaskUplinkHandle);

    if (xTaskUplinkHandle == NULL)
    {
        /* Error creating the uplink task */
    }
#endif /* CONFIG_TASK_UPLINK_ENABLED */

#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskTimeControl, TASK_TIME_CONTROL_NAME, TASK_TIME_CONTROL_STACK_SIZE, NULL, TASK_TIME_CONTROL_PRIORITY, &xTaskTimeControlHandle);

    if (xTaskTimeControlHandle == NULL)
    {
        /* Error creating the time control task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

    create_event_groups();
}

void create_event_groups(void)
{
    task_startup_status = xEventGroupCreate();

    if (task_startup_status == NULL)
    {
        /* Error creating the startup status event group */
    }
}

/** \} End of tasks group */
