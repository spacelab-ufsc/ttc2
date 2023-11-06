/*
 * antenna_deployment.h
 *
 * Copyright (C) 2023, SpaceLab.
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
 * \brief Antenna Deployment task definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2023/07/18
 *
 * \defgroup antenna_deployment Antenna Deployment
 * \ingroup tasks
 * \{
 */

#ifndef ANTENNA_DEPLOYMENT_H_
#define ANTENNA_DEPLOYMENT_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_ANTENNA_DEPLOYMENT_NAME                    "Antenna Deployment" /**< Task name. */
#define TASK_ANTENNA_DEPLOYMENT_STACK_SIZE              150                  /**< Stack size in bytes. */
#define TASK_ANTENNA_DEPLOYMENT_PRIORITY                6                    /**< Task priority. */

/**
 * \brief Antenna deployment handle.
 */
extern xTaskHandle xTaskAntennaDeploymentHandle;

/**
 * \brief Antenna deployment task.
 *
 * \return None.
 */
void vTaskAntennaDeployment(void);

#endif /* ANTENNA_DEPLOYMENT_H_ */

/** \} End of antenna_deployment group */
