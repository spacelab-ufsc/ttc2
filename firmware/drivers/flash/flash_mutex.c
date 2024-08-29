/*
 * flash_mutex.c
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
 * \brief Flash mutex functions implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.5.1
 *
 * \date 2024/04/22
 *
 * \addtogroup flash
 * \{
 */

#include <stddef.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "flash.h"
#include <system/sys_log/sys_log.h>

static SemaphoreHandle_t flash_mutex = NULL;

int flash_mutex_create(void)
{
    int err = 0;

    flash_mutex = xSemaphoreCreateMutex();

    if (flash_mutex == NULL)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, FLASH_MODULE_NAME, "Error creating a mutex!");
        sys_log_new_line();

        err = -1;
    }

    return err;
}

int flash_mutex_take(void)
{
    int err = -1;

    if (flash_mutex != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait FLASH_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(flash_mutex, pdMS_TO_TICKS(FLASH_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int flash_mutex_give(void)
{
    int err = -1;

    if (flash_mutex != NULL)
    {
        xSemaphoreGive(flash_mutex);

        err = 0;
    }

    return err;
}

/**< \} End of flash group */
