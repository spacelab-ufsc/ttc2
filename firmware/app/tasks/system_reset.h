/*
 * system_reset.h
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
 * \brief Periodic system reset task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.9
 * 
 * \date 2020/01/12
 * 
 * \defgroup system_reset System Reset
 * \ingroup tasks
 * \{
 */

#ifndef SYSTEM_RESET_H_
#define SYSTEM_RESET_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_SYSTEM_RESET_NAME                      "SystemReset"       /**< Task name. */
#define TASK_SYSTEM_RESET_STACK_SIZE                128                 /**< Stack size in bytes. */
#define TASK_SYSTEM_RESET_PRIORITY                  2                   /**< Task priority. */
#define TASK_SYSTEM_RESET_PERIOD_MS                 (1000UL*60*60*10)   /**< Task period in milliseconds. */

/**
 * \brief Periodic system reset handle.
 */
extern xTaskHandle xTaskSystemResetHandle;

/**
 * \brief Periodic system reset task.
 *
 * This task resets the microcontroller.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskSystemReset(void *pvParameters);

#endif /* SYSTEM_RESET_H_ */

/** \} End of system_reset group */
