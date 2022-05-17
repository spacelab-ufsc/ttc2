/*
 * watchdog_reset.c
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
 * \brief Watchdog reset task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.10
 * 
 * \date 2020/01/11
 * 
 * \defgroup watchdog_reset Watchdog Reset
 * \ingroup tasks
 * \{
 */

#ifndef WATCHDOG_RESET_H_
#define WATCHDOG_RESET_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_WATCHDOG_RESET_NAME                    "WatchdogReset"     /**< Task name. */
#define TASK_WATCHDOG_RESET_STACK_SIZE              150                 /**< Stack size in bytes. */
#define TASK_WATCHDOG_RESET_PRIORITY                1                   /**< Task priority. */
#define TASK_WATCHDOG_RESET_PERIOD_MS               100                 /**< Task period in milliseconds. */
#define TASK_WATCHDOG_RESET_INITIAL_DELAY_MS        0                   /**< Delay, in milliseconds, before the first execution. */

/**
 * \brief Watchdog reset handle.
 */
extern xTaskHandle xTaskWatchdogResetHandle;

/**
 * \brief Watchdog reset task.
 *
 * \return None.
 */
void vTaskWatchdogReset(void);

#endif /* WATCHDOG_RESET_H_ */

/** \} End of watchdog_reset group */
