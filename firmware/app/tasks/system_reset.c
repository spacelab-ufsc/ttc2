/*
 * system_reset.c
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
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Periodic system reset task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 * 
 * \addtogroup system_reset
 * \{
 */

#include <system/system.h>
#include <system/sys_log/sys_log.h>

#include "system_reset.h"

#define pdMS_TO_TICKS_LONG( xTimeInMs ) ( ( TickType_t ) ( ( ( uint64_t ) ( xTimeInMs ) * ( uint64_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000 ) )

xTaskHandle xTaskSystemResetHandle;

void vTaskSystemReset(void)
{
    TickType_t reset_period_ticks = pdMS_TO_TICKS_LONG((TickType_t) TASK_SYSTEM_RESET_PERIOD_MS);

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS_LONG(reset_period_ticks));

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_SYSTEM_RESET_NAME, "Restarting the system...");
        sys_log_new_line();

        system_reset();
    }
}

/** \} End of system_reset group */
