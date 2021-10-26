/*
 * csp_server.h
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
 * \brief CSP server task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.10
 * 
 * \date 2020/04/04
 * 
 * \defgroup csp_server CSP Server
 * \ingroup tasks
 * \{
 */

#ifndef CSP_SERVER_H_
#define CSP_SERVER_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_CSP_SERVER_NAME                "CSP_Server"    /**< Task name. */
#define TASK_CSP_SERVER_STACK_SIZE          1024            /**< Memory stack size in bytes. */
#define TASK_CSP_SERVER_PRIORITY            1               /**< Priority. */
#define TASK_CSP_SERVER_PERIOD_MS           500             /**< Period in milliseconds. */
#define TASK_CSP_SERVER_INIT_TIMEOUT_MS     2000            /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief CSP server task handle.
 */
extern xTaskHandle xTaskCSPServerHandle;

/**
 * \brief CSP server task.
 *
 * \return None.
 */
void vTaskCSPServer(void);

#endif /* CSP_SERVER_H_ */

/** \} End of csp_server group */
