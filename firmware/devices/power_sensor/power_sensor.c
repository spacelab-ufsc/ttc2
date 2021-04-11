/*
 * power_sensor.c
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
 * \brief Power sensor device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.17
 * 
 * \date 2021/04/11
 * 
 * \addtogroup power_sensor
 * \{
 */

#include <system/sys_log/sys_log.h>

#include "power_sensor.h"

int power_sensor_init(void)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, POWER_SENSOR_MODULE_NAME, "Initializing the power sensor...");
    sys_log_new_line();

    return -1;
}

int power_sensor_read(power_sensor_data_t *data)
{
    return -1;
}

int power_sensor_read_voltage_mv(voltage_t *volt)
{
    return -1;
}

int power_sensor_read_current_ma(current_t *curr)
{
    return -1;
}

int power_sensor_read_power_mw(power_t *pwr)
{
    return -1;
}

/** \} End of power_sensor group */
