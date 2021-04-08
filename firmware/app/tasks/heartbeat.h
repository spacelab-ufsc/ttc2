/*
 * heartbeat.h
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Heartbeat task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 20/01/2020
 * 
 * \defgroup heartbeat Heartbeat
 * \ingroup tasks
 * \{
 */

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_HEARTBEAT_NAME                 "Heartbeat"     /**< Task name. */
#define TASK_HEARTBEAT_STACK_SIZE           128             /**< Memory stack size in bytes. */
#define TASK_HEARTBEAT_PRIORITY             1               /**< Priority. */
#define TASK_HEARTBEAT_PERIOD_MS            500             /**< Period in milliseconds. */
#define TASK_HEARTBEAT_INIT_TIMEOUT_MS      2000            /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Heartbeat task handle.
 */
extern xTaskHandle xTaskHeartbeatHandle;

/**
 * \brief System heartbeat task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskHeartbeat(void *pvParameters);

#endif /* HEARTBEAT_H_ */

/** \} End of heartbeat group */
