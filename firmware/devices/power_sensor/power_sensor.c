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
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.0.17
 * 
 * \date 2021/04/11
 * 
 * \addtogroup power_sensor
 * \{
 */

#include <system/sys_log/sys_log.h>

#include <drivers/ina22x/ina22x.h>


#include "power_sensor.h"

static ina22x_config_t uc_config = {
                                    .i2c_port = I2C_PORT_1,
                                    .i2c_conf = 100000,
                                    .i2c_adr  = 0x44,
                                    .avg_mode = INA22X_AVERAGING_MODE_128,
                                    .bus_voltage_conv_time = INA22X_BUS_VOLTAGE_CONV_TIME_588u,
                                    .shunt_voltage_conv_time = INA22X_SHUNT_VOLTAGE_CONV_TIME_588u,
                                    .op_mode = INA22X_MODE_SHUNT_BUS_CONT,
                                    .lsb_current = 5e-6,
                                    .cal = 10240,
} ;
static ina22x_config_t radio_config = {
                                       .i2c_port = I2C_PORT_1,
                                       .i2c_conf = 100000,
                                       .i2c_adr  = 0x45,
                                       .avg_mode = INA22X_AVERAGING_MODE_128,
                                       .bus_voltage_conv_time = INA22X_BUS_VOLTAGE_CONV_TIME_588u,
                                       .shunt_voltage_conv_time = INA22X_SHUNT_VOLTAGE_CONV_TIME_588u,
                                       .op_mode = INA22X_MODE_SHUNT_BUS_CONT,
                                       .lsb_current = 5e-5,
                                       .cal = 1024,

} ;

int power_sensor_init(void)
{
    int err = -1;

    sys_log_print_event_from_module(SYS_LOG_INFO, POWER_SENSOR_MODULE_NAME, "Initializing the power sensor...");
    sys_log_new_line();

    /* Power sensor initialization */
    if ((ina22x_init(radio_config) == 0) && (ina22x_init(uc_config) == 0))
    {
        /* Power sensor configuration */
        if ((ina22x_configuration(radio_config) == 0) && (ina22x_configuration(uc_config) == 0))
        {
            /* Power sensor calibration */
            if ((ina22x_calibration(radio_config) == 0) && (ina22x_calibration(uc_config) == 0))
            {
                err = 0;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during the initialization: Driver calibration has failed!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during the initialization: Driver configuration has failed!");
                sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during the initialization: Driver initialization has failed!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int power_sensor_read(power_sensor_measured_device_t device, power_sensor_data_t *data)
{
    int err = -1;
    power_sensor_data_t sensor_data = {0};
    power_sensor_scale_t shunt_v_scale;
    power_sensor_scale_t bus_v_scale;
    power_sensor_scale_t curr_scale;
    power_sensor_scale_t pwr_scale;

    switch(device)
    {
    case POWER_SENSOR_UC:
        shunt_v_scale = POWER_SENSOR_MICRO_SCALE;
        bus_v_scale = POWER_SENSOR_MILI_SCALE;
        curr_scale = POWER_SENSOR_MICRO_SCALE;
        pwr_scale = POWER_SENSOR_MILI_SCALE;

        break;

    case POWER_SENSOR_RADIO:
        shunt_v_scale = POWER_SENSOR_MICRO_SCALE;
        bus_v_scale = POWER_SENSOR_MILI_SCALE;
        curr_scale = POWER_SENSOR_MILI_SCALE;
        pwr_scale = POWER_SENSOR_MILI_SCALE;

        break;

    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during sensor reading: Invalid device for scale");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;

        break;
    }
    /* Voltage reading */
    if (power_sensor_read_voltage_scaled(device, &sensor_data.shunt_voltage, &sensor_data.bus_voltage, shunt_v_scale, bus_v_scale) == 0)
    {
        /* Current reading */
        if (power_sensor_read_current_scaled(device, &sensor_data.current, curr_scale) == 0)
        {
            /* Power reading */
            if (power_sensor_read_power_scaled(device, &sensor_data.power, pwr_scale) == 0)
            {
                err = 0;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error sensor power reading (power): Driver level error!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error sensor power reading (current): Driver level error!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error sensor power reading (voltage): Driver level error!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    *data = sensor_data;

    return err;
}

int power_sensor_read_voltage_scaled(power_sensor_measured_device_t device, voltage_t *volt_shunt, voltage_t *volt_bus, power_sensor_scale_t shunt_scale, power_sensor_scale_t bus_scale)
{
    int err = 0;
    ina22x_config_t config;
    float f_volt_shunt, f_volt_bus;
    uint16_t scale_coef_bus, scale_coef_shunt;

    switch(device)
    {
    case POWER_SENSOR_RADIO:
        config = radio_config;

        break;
    case POWER_SENSOR_UC:
        config = uc_config;

        break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during voltage reading (mV): Invalid device!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;

        break;
    }

    switch(shunt_scale)
    {
    case POWER_SENSOR_MICRO_SCALE:   scale_coef_shunt = 1000000;   break;
    case POWER_SENSOR_MILI_SCALE:    scale_coef_shunt = 1000;      break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during voltage power reading: Invalid scale!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    err = -1;

    break;
    }

    switch(bus_scale)
    {
    case POWER_SENSOR_MICRO_SCALE:   scale_coef_bus = 1000000;   break;
    case POWER_SENSOR_MILI_SCALE:    scale_coef_bus = 1000;      break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during voltage power reading: Invalid scale!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    err = -1;

    break;
    }

    if (err == 0)
    {
        if ((ina22x_get_voltage_V(config, INA22X_BUS_VOLTAGE, &f_volt_bus) == -1) || (ina22x_get_voltage_V(config, INA22X_SHUNT_VOLTAGE, &f_volt_shunt) == -1))
        {

        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during voltage reading (mV): Driver level error!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    *volt_shunt = (voltage_t) (f_volt_shunt) * scale_coef_shunt;
    *volt_bus   = (voltage_t) (f_volt_bus) * scale_coef_bus;

    return err;
}

int power_sensor_read_current_scaled(power_sensor_measured_device_t device, current_t *curr, power_sensor_scale_t scale)
{
    int err = 0;
    ina22x_config_t config;
    float f_curr;
    uint16_t scale_coef = 0;

    switch(device)
    {
    case POWER_SENSOR_RADIO:
        config = radio_config;

        break;
    case POWER_SENSOR_UC:
        config = uc_config;

        break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during current reading (mA): Invalid device!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;

        break;
    }

    switch(scale)
    {
    case POWER_SENSOR_MICRO_SCALE:   scale_coef = 1000000;   break;
    case POWER_SENSOR_MILI_SCALE:    scale_coef = 1000;      break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during current reading: Invalid scale!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    err = -1;

    break;
    }

    if (err == 0)
    {
        if ((ina22x_get_current_A(config, &f_curr) == -1))
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during voltage reading (mA): Driver level error!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    switch(scale)
    {
    case POWER_SENSOR_MICRO_SCALE: scale_coef = 1000000;  break;
    case POWER_SENSOR_MILI_SCALE:  scale_coef = 1000;     break;
    }

    *curr = (current_t) (f_curr) * scale_coef;

    return err;
}

int power_sensor_read_power_scaled(power_sensor_measured_device_t device, power_t *pwr, power_sensor_scale_t scale)
{
    int err = 0;
    ina22x_config_t config;
    float f_pwr;
    uint16_t scale_coef = 0;

    switch(device)
    {
    case POWER_SENSOR_RADIO:
        config = radio_config;

        break;
    case POWER_SENSOR_UC:
        config = uc_config;

        break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during current reading: Invalid device!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;

        break;
    }

    switch(scale)
    {
    case POWER_SENSOR_MICRO_SCALE:   scale_coef = 1000000;   break;
    case POWER_SENSOR_MILI_SCALE:    scale_coef = 1000;      break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during power reading: Invalid scale!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    err = -1;

    break;
    }

    if (err == 0)
    {
        if ((ina22x_get_power_W(config, &f_pwr) == -1))
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, POWER_SENSOR_MODULE_NAME, "Error during voltage reading: Driver level error!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    *pwr = (power_t) (f_pwr) * scale_coef;

    return err;
}

/*TODO: Update function descriptions */
/*TODO: Check for misra rules */
/*TODO: Update ina22x mockup */
/*TODO: Power sensor unitary test */


/** \} End of power_sensor group */
