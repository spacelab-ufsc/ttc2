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
 * 
 * \version 0.4.3
 * 
 * \date 2023/04/13
 * 
 * \addtogroup si446x
 * \{
 */

#include <FreeRTOS.h>
#include <semphr.h>

#include "si446x.h"
#include "si446x_config.h"

static SemaphoreHandle_t si446x_semaphore = NULL;

int si446x_mutex_create(void)
{
    /* Create a mutex type semaphore */
    si446x_semaphore = xSemaphoreCreateMutex();

    int err = 0;

    if (si446x_semaphore == NULL)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SI446X_DEVICE_NAME, "Error creating a mutex!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */

        err = -1;
    }

    return err;
}

int si446x_mutex_take(void)
{
    int err = -1;

    if (si446x_semaphore != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait SYS_LOG_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(si446x_semaphore, pdMS_TO_TICKS(SI446X_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int si446x_mutex_give(void)
{
    int err = -1;

    if (si446x_semaphore != NULL)
    {
        xSemaphoreGive(si446x_semaphore);

        err = 0;
    }

    return err;
}

/**< \} End of si446x group */
