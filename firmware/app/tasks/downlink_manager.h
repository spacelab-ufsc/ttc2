/*
 * downlink_manager.h
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
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Downlink manager task definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.3
 *
 * \date 2023/03/03
 *
 * \defgroup downlink_manager DOWNLINK_MANAGER
 * \ingroup tasks
 * \{
 */

#ifndef TASK_DOWNLINK_MANAGER_H_
#define TASK_DOWNLINK_MANAGER_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_DOWNLINK_MANAGER_NAME                "Downlink Manager"  /**< Task name. */
#define TASK_DOWNLINK_MANAGER_STACK_SIZE          2000                /**< Stack size in bytes. */
#define TASK_DOWNLINK_MANAGER_PRIORITY            3                   /**< Task priority. */
#define TASK_DOWNLINK_MANAGER_PERIOD_MS           150                 /**< Task period in milliseconds. */
#define TASK_DOWNLINK_MANAGER_INITIAL_DELAY_MS    550                 /**< Delay, in milliseconds, before the first execution. */
#define TASK_DOWNLINK_MANAGER_INIT_TIMEOUT_MS     10000               /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Downlink Manager handle.
 */
extern xTaskHandle xTaskDownlinkManagerHandle;

/**
 * \brief Downlink Manager task.
 *
 * \return None.
 */
void vTaskDownlinkManager(void);

#endif /* TASK_DOWNLINK_MANAGER_H_ */

/** \} End of downlink_manager group */
