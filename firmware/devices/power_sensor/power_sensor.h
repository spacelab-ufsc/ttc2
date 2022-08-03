/*
 * power_sensor.h
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
 * \brief Power sensor device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.17
 * 
 * \date 2021/04/11
 * 
 * \defgroup power_sensor Power Sensor
 * \ingroup devices
 * \{
 */

#ifndef POWER_SENSOR_H_
#define POWER_SENSOR_H_

#include <stdint.h>

#define POWER_SENSOR_MODULE_NAME    "Power Sensor"

/**
 * \brief Power Sensor measurement devices
 */
typedef enum
{
    POWER_SENSOR_UC = 0,                        /**< Measurements from uC */
    POWER_SENSOR_RADIO,                         /**< Measurements from Radio */
}power_sensor_measured_device_t;

/**
 * \brief Power Sensor measurement scale
 */
typedef enum
{
    POWER_SENSOR_MICRO_SCALE = 0,                        /**< Measurements in micro scale */
    POWER_SENSOR_MILI_SCALE,                             /**< Measurements in mili scale */
}power_sensor_scale_t;

/**
 * \brief Current type.
 */
typedef uint16_t current_t;

/**
 * \brief Voltage type.
 */
typedef uint16_t voltage_t;

/**
 * \brief Power type.
 */
typedef uint16_t power_t;

/**
 * \brief Power sensor data.
 */
typedef struct
{
    voltage_t shunt_voltage;      /**< Power sensor shunt voltage. */
    voltage_t bus_voltage;        /**< Power sensor bus voltage. */
    current_t current;            /**< Power sensor current. */
    power_t power;                /**< Power sensor power. */
} power_sensor_data_t;

/**
 * \brief Power sensor device initialization routine.
 *
 * \return The status/error code.
 */
int power_sensor_init(void);

/**
 * \brief Reads the power sensor data.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \return The status/error code.
 */
int power_sensor_read(power_sensor_measured_device_t device, power_sensor_data_t *data);

/**
 * \brief Reads the voltage, in mV, from the power sensor.
 *
 * \param[in,out] volt is a pointer to store the read voltage in mV.
 *
 * \return The status/error code.
 */
int power_sensor_read_voltage_scaled(power_sensor_measured_device_t device, voltage_t *volt_shunt, voltage_t *volt_bus, power_sensor_scale_t shunt_scale, power_sensor_scale_t bus_scale);

/**
 * \brief Reads the current, in mA, from the power sensor.
 *
 * \param[in,out] curr is a pointer to store the read current in mW.
 *
 * \return The status/error code.
 */
int power_sensor_read_current_scaled(power_sensor_measured_device_t device, current_t *curr, power_sensor_scale_t scale);

/**
 * \brief Reads the power, in mW, from the power sensor.
 *
 * \param[in,out] pwr is a pointer to store the read power in mW.
 *
 * \return The status/error code.
 */
int power_sensor_read_power_scaled(power_sensor_measured_device_t device, power_t *pwr, power_sensor_scale_t scale);

#endif /* POWER_SENSOR_H_ */

/** \} End of power_sensor group */
