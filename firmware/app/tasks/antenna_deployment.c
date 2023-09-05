/*
 * antenna_deployment.h
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
 * \brief Antenna deployment task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.3
 *
 * \date 2023/07/18
 *
 * \addtogroup antenna_deployment
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <devices/antenna/antenna.h>

#include <structs/ttc_data.h>

#include "antenna_deployment.h"

xTaskHandle xTaskAntennaDeploymentHandle;

void vTaskAntennaDeployment(void)
{
    /* Initial hibernation */
    ttc_data_buf.ant_deploy_hib_count = 0;

    if (!ttc_data_buf.ant_deploy_hib_exec)
    {
        uint8_t initial_hib_time_counter = ttc_data_buf.ant_deploy_hib_count;

        uint8_t i = 0;

        for(i = initial_hib_time_counter; i < CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN; i++)
        {
            vTaskDelay(pdMS_TO_TICKS(60000U));

            ttc_data_buf.ant_deploy_hib_count++;
        }

        ttc_data_buf.ant_deploy_hib_exec = true;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "Initial deployment already executed!");
        sys_log_new_line();
    }

    /* Antenna deployment */
    if (ttc_data_buf.ant_deploy_count< CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "Antenna deployment attempt number ");
        sys_log_print_uint(ttc_data_buf.ant_deploy_count + 1);
        sys_log_print_msg(" of ");
        sys_log_print_uint(CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS);
        sys_log_print_msg("...");
        sys_log_new_line();

        if (antenna_deploy(10U*1000U) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_ANTENNA_DEPLOYMENT_NAME, "Error deploying the antenna!");
            sys_log_new_line();
        }

        ttc_data_buf.ant_deploy_count++;

        ttc_data_buf.ant_deploy_exec = true;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "All antenna deployments attempts executed! (");
        sys_log_print_uint(ttc_data_buf.ant_deploy_count + 1);
        sys_log_print_msg(")");
        sys_log_new_line();

    }

    vTaskSuspend(xTaskAntennaDeploymentHandle);
}

/** \} End of antenna_deployment group */





