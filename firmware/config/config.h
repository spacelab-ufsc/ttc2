/*
 * config.h
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
 * \brief Configuration parameters definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.5
 * 
 * \date 2019/10/26
 * 
 * \defgroup config Configuration
 * \{
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* Tasks */
#define CONFIG_TASK_STARTUP_ENABLED                     1
#define CONFIG_TASK_WATCHDOG_RESET_ENABLED              1
#define CONFIG_TASK_HEARTBEAT_ENABLED                   1
#define CONFIG_TASK_SYSTEM_RESET_ENABLED                1
#define CONFIG_TASK_RADIO_RESET_ENABLED                 1
#define CONFIG_TASK_READ_SENSORS_ENABLED                1
#define CONFIG_TASK_BEACON_ENABLED                      0
#define CONFIG_TASK_TIME_CONTROL_ENABLED                1
#define CONFIG_TASK_EPS_SERVER_ENABLED                  1
#define CONFIG_TASK_OBDH_SERVER_ENABLED                 1
#define CONFIG_TASK_DOWNLINK_MANAGER_ENABLED            1
#define CONFIG_TASK_UPLINK_MANAGER_ENABLED              1

/* Devices */
#define CONFIG_DEV_MEDIA_INT_ENABLED                    1
#define CONFIG_DEV_LEDS_ENABLED                         1
#define CONFIG_DEV_POWER_SENSOR_ENABLED                 1
#define CONFIG_DEV_TEMP_SENSOR_ENABLED                  1
#define CONFIG_DEV_RADIO_ENABLED                        1
#define CONFIG_DEV_ANTENNA_ENABLED                      1
#define CONFIG_DEV_OBDH_ENABLED                         1
#define CONFIG_DEV_EPS_ENABLED                          1

/* Debug and log messages */
#define CONFIG_DRIVERS_DEBUG_ENABLED                    0

#define CONFIG_SATELLITE_CALLSIGN                       "PY0EFS"

/* Packets IDs */
#define CONFIG_PKT_ID_BEACON                            0x10

/* Ports */
#define CONFIG_SPI_PORT_0_SPEED_BPS                     1000000UL

/* Radio */
#define SI446X_XO_TUNE_REG_VALUE                        97

/* Drivers */
#define ISIS_ANTENNA_ENABLED                            1
#define CONFIG_DRV_ISIS_ANTENNA_ENABLED                 1

/* Antenna */
#define CONFIG_ANTENNA_INDEP_DEPLOY_BURN_TIME_SEC       10U
#define CONFIG_ANTENNA_SEQ_DEPLOY_BURN_TIME_SEC         20U
#define CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS              10U
#define CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN       45

/* Memory addresses */
#define CONFIG_MEM_ADR_SYS_TIME                         0

#endif /* CONFIG_H_ */

/** \} End of config group */
