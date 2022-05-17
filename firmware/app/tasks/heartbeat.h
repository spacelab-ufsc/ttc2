/*
 * heartbeat.h
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
 * \brief Heartbeat task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.10
 * 
 * \date 2020/01/20
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
#define TASK_HEARTBEAT_STACK_SIZE           160             /**< Memory stack size in bytes. */
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
 * \return None.
 */
void vTaskHeartbeat(void);

#endif /* HEARTBEAT_H_ */

/** \} End of heartbeat group */
