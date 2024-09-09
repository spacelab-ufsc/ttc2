/*
 * si446x_mutex.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Si446x mutex functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2024/09/09
 * 
 * \addtogroup si446x
 * \{
 */

#include <stddef.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include <system/sys_log/sys_log.h>

#include "si446x.h"
#include "si446x_config.h"

static SemaphoreHandle_t si446x_mutex = NULL;

int si446x_mutex_create(void)
{
    int err = 0;

    si446x_mutex = xSemaphoreCreateMutex();

    if (si446x_mutex == NULL)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, SI446X_MODULE_NAME, "Error creating a mutex!");
        sys_log_new_line();

        err = -1;
    }

    return err;
}

int si446x_mutex_take(void)
{
    int err = -1;

    if (si446x_mutex != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait SI446X_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(si446x_mutex, pdMS_TO_TICKS(SI446X_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int si446x_mutex_give(void)
{
    int err = -1;

    if (si446x_mutex != NULL)
    {
        xSemaphoreGive(si446x_mutex);

        err = 0;
    }

    return err;
}

/**< \} End of si446x group */
