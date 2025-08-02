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
 * \version 1.0.0
 *
 * \date 2023/07/18
 *
 * \addtogroup antenna_deployment
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <devices/antenna/antenna.h>
#include <devices/media/media.h>
#include <drivers/flash/flash.h>

#include <structs/ttc_data.h>

#include <libs/crc/crc.h>

#include "antenna_deployment.h"

/**
 * \brief Saves deployment state to internal flash memory.
 *
 * \param[in] ttc is a pointer to ttc data buffer.
 *
 * \return The status/error code.
 */
static int save_deployment_state_to_flash(ttc_data_t *ttc);

/**
 * \brief Loads deployment state from internal flash memory.
 *
 * \param[in] ttc is a pointer to ttc data buffer.
 *
 * \return The status/error code.
 */
static int load_deployment_state_from_flash(ttc_data_t *ttc);

xTaskHandle xTaskAntennaDeploymentHandle;

void vTaskAntennaDeployment(void)
{
    vTaskDelay(pdMS_TO_TICKS(TASK_ANTENNA_DEPLOYMENT_TIMEOUT_MS));

    if (load_deployment_state_from_flash(&ttc_data_buf) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_ANTENNA_DEPLOYMENT_NAME, "Failed to load deployment state from internal flash!");
        sys_log_new_line();
    }

    /* Initial hibernation */
    ttc_data_buf.ant_deploy_hib_count = 0;

    if (!ttc_data_buf.ant_deploy_hib_exec)
    {
        uint8_t initial_hib_time_counter = ttc_data_buf.ant_deploy_hib_count;

        uint8_t i = 0;

        for(i = initial_hib_time_counter; i < CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN; i++)
        {
            uint32_t hib_dur = CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN - (uint32_t)i;
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_ANTENNA_DEPLOYMENT_NAME, "Antenna deployment will happen in ");
            sys_log_print_uint(hib_dur);
            sys_log_print_msg(" minutes!");
            sys_log_new_line();

            vTaskDelay(pdMS_TO_TICKS(60000U));

            ttc_data_buf.ant_deploy_hib_count++;

            (void)save_deployment_state_to_flash(&ttc_data_buf);
        }

        ttc_data_buf.ant_deploy_hib_exec = true;

        (void)save_deployment_state_to_flash(&ttc_data_buf);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "Initial deployment already executed!");
        sys_log_new_line();
    }

    /* Antenna deployment */
    if (ttc_data_buf.ant_deploy_count < CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "Antenna deployment attempt number ");
        sys_log_print_uint(ttc_data_buf.ant_deploy_count + 1U);
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

        (void)save_deployment_state_to_flash(&ttc_data_buf);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "All antenna deployments attempts executed! (");
        sys_log_print_uint(ttc_data_buf.ant_deploy_count + 1U);
        sys_log_print_msg(")");
        sys_log_new_line();

    }

    vTaskSuspend(xTaskAntennaDeploymentHandle);
}

static int save_deployment_state_to_flash(ttc_data_t *ttc)
{
    int err = -1;
    uint8_t buf[5] = {0};

    if (media_erase(MEDIA_INT_FLASH, FLASH_SEG_C_ADR) == 0)
    {
        buf[0] = ttc->ant_deploy_hib_count;
        buf[1] = ttc->ant_deploy_exec;
        buf[2] = ttc->ant_deploy_count;
        buf[3] = ttc->ant_deploy_hib_exec;
        buf[4] = crc8_get_val(buf, 4U);

        err = media_write(MEDIA_INT_FLASH, 0U, FLASH_SEG_C_ADR, buf, 5U);
    }

    return err;
}

static int load_deployment_state_from_flash(ttc_data_t *ttc)
{
    int err = -1;

    uint8_t buf[5] = {0};

    /* Getting the previous reset count parameter */
    if (media_read(MEDIA_INT_FLASH, 0U, FLASH_SEG_C_ADR, buf, 5U) == 0)
    {
        if (buf[4] == crc8_get_val(buf, 4U))
        {
            ttc->ant_deploy_hib_count = buf[0];
            ttc->ant_deploy_exec = buf[1];
            ttc->ant_deploy_count = buf[2];
            ttc->ant_deploy_hib_exec = buf[3];

            err = 0;
        }
    }

    return err;
}

/** \} End of antenna_deployment group */
