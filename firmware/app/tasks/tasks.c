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
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
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
#include "time_control.h"
#include "eps_server.h"
#include "obdh_server.h"
#include "downlink_manager.h"
#include "uplink_manager.h"
#include "antenna_deployment.h"
#include "read_antenna.h"

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
#if defined(CONFIG_TASK_WATCHDOG_RESET_ENABLED) && (CONFIG_TASK_WATCHDOG_RESET_ENABLED == 1)
    xTaskCreate(vTaskWatchdogReset, TASK_WATCHDOG_RESET_NAME, TASK_WATCHDOG_RESET_STACK_SIZE, NULL, TASK_WATCHDOG_RESET_PRIORITY, &xTaskWatchdogResetHandle);

    if (xTaskWatchdogResetHandle == NULL)
    {
        /* Error creating the watchdog reset task */
    }
#endif /* CONFIG_TASK_WATCHDOG_RESET_ENABLED */

    /* Heartbeat task */
#if defined(CONFIG_TASK_HEARTBEAT_ENABLED) && (CONFIG_TASK_HEARTBEAT_ENABLED == 1)
    xTaskCreate(vTaskHeartbeat, TASK_HEARTBEAT_NAME, TASK_HEARTBEAT_STACK_SIZE, NULL, TASK_HEARTBEAT_PRIORITY, &xTaskHeartbeatHandle);

    if (xTaskHeartbeatHandle == NULL)
    {
        /* Error creating the heartbeat task */
    }
#endif /* CONFIG_TASK_HEARTBEAT_ENABLED */

#if defined(CONFIG_TASK_SYSTEM_RESET_ENABLED) && (CONFIG_TASK_SYSTEM_RESET_ENABLED == 1)
    xTaskCreate(vTaskSystemReset, TASK_SYSTEM_RESET_NAME, TASK_SYSTEM_RESET_STACK_SIZE, NULL, TASK_SYSTEM_RESET_PRIORITY, &xTaskSystemResetHandle);

    if (xTaskSystemResetHandle == NULL)
    {
        /* Error creating the system reset task */
    }
#endif /* CONFIG_TASK_SYSTEM_RESET_ENABLED */

#if defined(CONFIG_TASK_RADIO_RESET_ENABLED) && (CONFIG_TASK_RADIO_RESET_ENABLED == 1)
    xTaskCreate(vTaskRadioReset, TASK_RADIO_RESET_NAME, TASK_RADIO_RESET_STACK_SIZE, NULL, TASK_RADIO_RESET_PRIORITY, &xTaskRadioResetHandle);

    if (xTaskRadioResetHandle == NULL)
    {
        /* Error creating the radio reset task */
    }
#endif /* CONFIG_TASK_RADIO_RESET_ENABLED */

#if defined(CONFIG_TASK_READ_SENSORS_ENABLED) && (CONFIG_TASK_READ_SENSORS_ENABLED == 1)
    xTaskCreate(vTaskReadSensors, TASK_READ_SENSORS_NAME, TASK_READ_SENSORS_STACK_SIZE, NULL, TASK_READ_SENSORS_PRIORITY, &xTaskReadSensorsHandle);

    if (xTaskReadSensorsHandle == NULL)
    {
        /* Error creating the read sensors task */
    }
#endif /* CONFIG_TASK_READ_SENSORS_ENABLED */


#if defined(CONFIG_TASK_TIME_CONTROL_ENABLED) && (CONFIG_TASK_TIME_CONTROL_ENABLED == 1)
    xTaskCreate(vTaskTimeControl, TASK_TIME_CONTROL_NAME, TASK_TIME_CONTROL_STACK_SIZE, NULL, TASK_TIME_CONTROL_PRIORITY, &xTaskTimeControlHandle);

    if (xTaskTimeControlHandle == NULL)
    {
        /* Error creating the time control task */
    }
#endif /* CONFIG_TASK_TIME_CONTROL_ENABLED */

#if defined(CONFIG_TASK_EPS_SERVER_ENABLED) && (CONFIG_TASK_EPS_SERVER_ENABLED == 1)
    xTaskCreate(vTaskEpsServer, TASK_EPS_SERVER_NAME, TASK_EPS_SERVER_STACK_SIZE, NULL, TASK_EPS_SERVER_PRIORITY, &xTaskEpsServerHandle);

    if (xTaskEpsServerHandle == NULL)
    {
        /* Error creating the eps server task */
    }
#endif /* CONFIG_TASK_EPS_SERVER_ENABLED */

#if defined(CONFIG_TASK_OBDH_SERVER_ENABLED) && (CONFIG_TASK_OBDH_SERVER_ENABLED == 1)
    xTaskCreate(vTaskObdhServer, TASK_OBDH_SERVER_NAME, TASK_OBDH_SERVER_STACK_SIZE, NULL, TASK_OBDH_SERVER_PRIORITY, &xTaskObdhServerHandle);

    if (xTaskObdhServerHandle == NULL)
    {
        /* Error creating the eps server task */
    }
#endif /* CONFIG_TASK_OBDH_SERVER_ENABLED */

#if defined(CONFIG_TASK_DOWNLINK_MANAGER_ENABLED) && (CONFIG_TASK_DOWNLINK_MANAGER_ENABLED == 1)
    xTaskCreate(vTaskDownlinkManager, TASK_DOWNLINK_MANAGER_NAME, TASK_DOWNLINK_MANAGER_STACK_SIZE, NULL, TASK_DOWNLINK_MANAGER_PRIORITY, &xTaskDownlinkManagerHandle);

    if (xTaskDownlinkManagerHandle == NULL)
    {
        /* Error creating the eps server task */
    }
#endif /* CONFIG_TASK_DOWNLINK_MANAGER_ENABLED */

#if defined(CONFIG_TASK_UPLINK_MANAGER_ENABLED) && (CONFIG_TASK_UPLINK_MANAGER_ENABLED == 1)
    xTaskCreate(vTaskUplinkManager, TASK_UPLINK_MANAGER_NAME, TASK_UPLINK_MANAGER_STACK_SIZE, NULL, TASK_UPLINK_MANAGER_PRIORITY, &xTaskUplinkManagerHandle);

    if (xTaskUplinkManagerHandle == NULL)
    {
        /* Error creating the eps server task */
    }
#endif /* CONFIG_TASK_UPLINK_MANAGER_ENABLED */


#if defined(CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED) && (CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED == 1)
    xTaskCreate(vTaskAntennaDeployment, TASK_ANTENNA_DEPLOYMENT_NAME, TASK_ANTENNA_DEPLOYMENT_STACK_SIZE, NULL, TASK_ANTENNA_DEPLOYMENT_PRIORITY, &xTaskAntennaDeploymentHandle);

    if (xTaskAntennaDeploymentHandle == NULL)
    {
        /* Error creating the antenna deployment task */
    }
#endif /* CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED */


#if defined(CONFIG_TASK_READ_ANTENNA_ENABLED) && (CONFIG_TASK_READ_ANTENNA_ENABLED == 1)
    xTaskCreate(vTaskReadAntenna, TASK_READ_ANTENNA_NAME, TASK_READ_ANTENNA_STACK_SIZE, NULL, TASK_READ_ANTENNA_PRIORITY, &xTaskReadAntennaHandle);

    if (xTaskReadAntennaHandle == NULL)
    {
        /* Error creating the read Antenna task */
    }
#endif /* CONFIG_TASK_READ_ANTENNA_ENABLED */

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
