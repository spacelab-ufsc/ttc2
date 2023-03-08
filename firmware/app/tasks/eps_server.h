/*
 * eps_server.h
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
 * \brief EPS Server task definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.5
 *
 * \date 2023/03/03
 *
 * \defgroup eps EPS
 * \ingroup tasks
 * \{
 */

#ifndef TASK_EPS_SERVER_H_
#define TASK_EPS_SERVER_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_EPS_SERVER_NAME                "EPS Server"        /**< Task name. */
#define TASK_EPS_SERVER_STACK_SIZE          2000                /**< Stack size in bytes. */
#define TASK_EPS_SERVER_PRIORITY            4                   /**< Task priority. */
#define TASK_EPS_SERVER_PERIOD_MS           100                 /**< Task period in milliseconds. */
#define TASK_EPS_SERVER_INITIAL_DELAY_MS    1000                /**< Delay, in milliseconds, before the first execution. */
#define TASK_EPS_SERVER_INIT_TIMEOUT_MS     10000               /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Eps Server handle.
 */
extern xTaskHandle xTaskEpsServerHandle;

/**
 * \brief Eps Server task.
 *
 * \return None.
 */
void vTaskEpsServer(void);

#endif /* TASK_EPS_SERVER_H_ */

/** \} End of eps_server group */

