/*
 * ina22x.c
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
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief INA22x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.1.11
 * 
 * \date 2022/06/01
 * 
 * \addtogroup ina22x
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <drivers/i2c/i2c.h>

#include "ina22x.h"

int ina22x_init(ina22x_config_t config)
{
    int err = -1;
    if (i2c_init(INA22X_I2C_PORT, config.i2c_conf) == 0)
    {
        /* Sets ina22x to default */
        if (ina22x_write_reg(config, INA22X_REG_CONFIGURATION, 0X8000) == 0)
        {
            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x initialization: Could not reset!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x initialization: Failed to initialize I2C!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }
    return err;


}

int ina22x_configuration(ina22x_config_t config)
{
    int err = -1;
    uint16_t config_mask = 0x0000;

    config_mask |= config.avg_mode;
    config_mask |= config.bus_voltage_conv_time;
    config_mask |= config.shunt_voltage_conv_time;
    config_mask |= config.op_mode;

    if (ina22x_write_reg(config, INA22X_REG_CONFIGURATION, config_mask) == 0)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x configuration!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int ina22x_calibration(ina22x_config_t config)
{
    int err = 0;

    switch(config.device)
    {
        case INA22X_CAL_UC:
            config.cal= 10240;
            config.lsb_current = 5e-6;
            break;
        case INA22X_CAL_RADIO:
            config.cal = 1024;
            config.lsb_current = 50e-6;
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x calibration: Invalid calibration value!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;       /* Invalid calibration value */
        break;
    }
    if (err == 0)
    {
        if (ina22x_write_reg(config, INA22X_REG_CALIBRATION, config.cal) == -1)
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x calibration: Could not write register!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;       /* Invalid calibration value */
        }
    }
    return err;
}

int ina22x_write_reg(ina22x_config_t config, ina22x_reg_t reg, uint16_t val)
{
    int err = 0;
    uint8_t buf[3];
    buf[0] = reg;
    buf[1] = val >> 8;
    buf[2] = val & (uint16_t) 0xFF;
    if (i2c_write(config.i2c_port, INA22X_I2C_SLAVE_ADDRESS, buf, 3) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x write reg function: Could not write register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    err = -1;       /* Invalid calibration value */
    }

    return err;

}

int ina22x_read_reg(ina22x_config_t config, ina22x_reg_t reg, uint16_t *val)
{
    uint8_t buf[2];

    int err = -1;
    if (i2c_write(config.i2c_port, INA22X_I2C_SLAVE_ADDRESS, &reg, 1) == 0)
    {
        if (i2c_read(config.i2c_port, INA22X_I2C_SLAVE_ADDRESS, buf, 2) == 0)
        {
            err = 0;
            *val = ((uint16_t) buf[1] << 8 ) | buf[0];
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x read register function: Could not read register content!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x read register function: Could not access register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int ina22x_get_current_raw(ina22x_config_t config, ina22x_current_t *cur)
{
    int err = -1;
    uint16_t current_reg;
    int16_t current_signed = 0x0000;

    if(ina22x_read_reg(config, INA22X_REG_CURRENT, &current_reg) == 0)
    {
        err = 0;
        current_signed |= current_reg;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current: Could not read register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    *cur = current_signed;

    return err;
}

int ina22x_get_voltage_raw(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *vol)
{
    int err = -1;
    uint16_t voltage_reg;
    int16_t voltage_signed = 0x0000;
    uint16_t target_reg;

    switch(device)
    {
    case INA22X_BUS_VOLTAGE:    target_reg = INA22X_REG_BUS_VOLTAGE;    break;
    case INA22X_SHUNT_VOLTAGE:  target_reg = INA22X_REG_SHUNT_VOLTAGE;  break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current raw: Invalid target!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        break;
    }
    if(ina22x_read_reg(config, target_reg, &voltage_reg) == 0)
    {
        err = 0;
        voltage_signed |= voltage_reg;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current: Could not read register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    *vol = voltage_signed;

    return err;
}

int ina22x_get_power_raw(ina22x_config_t config, ina22x_current_t *pow)
{
    int err = -1;
    uint16_t power_reg;
    int16_t power_signed = 0x00;

    if(ina22x_read_reg(config, INA22X_REG_POWER, &power_reg) == 0)
    {
        err = 0;
        power_signed |= power_reg;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get power: Could not read register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }
    *pow = power_signed;

    return err;
}

int ina22x_get_current_A(ina22x_config_t config, ina22x_current_t *cur)
{
    int err = -1;
    ina22x_current_t current_reg;

    if (ina22x_get_current_raw(config, &current_reg) == 0)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current in amps!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    *cur = current_reg * config.lsb_current;

    return err;
}

int ina22x_get_voltage_V(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *vol)
{
    int err = -1;
    ina22x_voltage_t voltage_reg;

    if (ina22x_get_voltage_raw(config, device, &voltage_reg) == 0)
    {
        err = 0;
        switch(device)
        {
            case INA22X_BUS_VOLTAGE:      *vol = voltage_reg * 1.25e-3;    break;
            case INA22X_SHUNT_VOLTAGE:    *vol = voltage_reg * 2.5e-6;     break;
            default:
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get voltage in volts, invalid device!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                break;
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get voltage in volts!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int ina22x_get_power_W(ina22x_config_t config, ina22x_current_t *pow)
{
    int err = -1;
    ina22x_power_t power_reg;

    if (ina22x_get_power_raw(config, &power_reg) == 0)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get power in watts!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    *pow = power_reg * 25 * config.lsb_current;

    return err;
}

int ina22x_get_manufacturer_id(ina22x_config_t config, ina22x_id_t *id)
{
    int err = -1;

    if(ina22x_read_reg(config, INA22X_REG_MANUFACTURER_ID, id) == 0)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get manufacturer id: Could not read register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int ina22x_get_die_id(ina22x_config_t config, ina22x_id_t *id)
{
    int err = -1;

    if(ina22x_read_reg(config, INA22X_REG_DIE_ID, id) == 0)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get manufacturer id: Could not read register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

/** \} End of ina22x group */
