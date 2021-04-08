/*
 * time_control.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Time control task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.14
 * 
 * \date 2020/08/09
 * 
 * \defgroup time_control Time Control
 * \ingroup tasks
 * \{
 */

#ifndef TIME_CONTROL_H_
#define TIME_CONTROL_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_TIME_CONTROL_NAME                  "Time Control"      /**< Task name. */
#define TASK_TIME_CONTROL_STACK_SIZE            128                 /**< Stack size in bytes. */
#define TASK_TIME_CONTROL_PRIORITY              3                   /**< Task priority. */
#define TASK_TIME_CONTROL_PERIOD_MS             1000                /**< Task period in milliseconds. */
#define TASK_TIME_CONTROL_INIT_TIMEOUT_MS       0                   /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Time control task handle.
 */
extern xTaskHandle xTaskTimeControlHandle;

/**
 * \brief Time control task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskTimeControl(void *pvParameters);

#endif /* TIME_CONTROL_H_ */

/** \} End of time_control group */
