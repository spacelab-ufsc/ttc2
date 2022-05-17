/*
 * uplink.h
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
 * \brief Uplink task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.10
 * 
 * \date 2020/04/18
 * 
 * \defgroup uplink Uplink
 * \ingroup tasks
 * \{
 */

#ifndef UPLINK_H_
#define UPLINK_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_UPLINK_NAME                    "Uplink"            /**< Task name. */
#define TASK_UPLINK_STACK_SIZE              500                 /**< Stack size in bytes. */
#define TASK_UPLINK_PRIORITY                2                   /**< Task priority. */
#define TASK_UPLINK_PERIOD_MS               (10*1000)           /**< Task period in milliseconds. */
#define TASK_UPLINK_INITIAL_DELAY_MS        1000                /**< Delay, in milliseconds, before the first execution. */
#define TASK_UPLINK_INIT_TIMEOUT_MS         (10*1000)           /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Uplink handle.
 */
extern xTaskHandle xTaskUplinkHandle;

/**
 * \brief Uplink task.
 *
 * \return None.
 */
void vTaskUplink(void);

#endif /* UPLINK_H_ */

/** \} End of uplink group */
