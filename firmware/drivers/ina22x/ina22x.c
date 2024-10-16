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
 * \version 0.3.1
 * 
 * \date 2022/07/19
 * 
 * \addtogroup ina22x
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <drivers/i2c/i2c.h>

#include "ina22x.h"

/**
 * \brief Reads the current from the device without conversion.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read current.
 *
 * \return The status/error code.
 */
static int ina22x_get_current_raw(ina22x_config_t config, uint16_t *cur);

/**
 * \brief Reads the voltage from the device without conversion.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] volt is a pointer to store the read raw voltage.
 *
 * \return The status/error code.
 */
static int ina22x_get_voltage_raw(ina22x_config_t config, ina22x_voltage_device_t device, uint16_t *volt);

/**
 * \brief Reads the power from the device without conversion.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] pwr is a pointer to store the read raw power.
 *
 * \return The status/error code.
 */
static int ina22x_get_power_raw(ina22x_config_t config, uint16_t *pwr);

/**
 * \brief Converts the read raw value to Amperes.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in] cur is the raw current to convert.
 *
 * \return The converted current in Amperes.
 */
static ina22x_current_t ina22x_convert_raw_to_A(ina22x_config_t config, uint16_t cur);

/**
 * \brief Converts the read raw value to Volts.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in] volt is the raw voltage to convert.
 *
 * \return The converted voltage in Volts.
 */
static ina22x_voltage_t ina22x_convert_raw_to_V(ina22x_voltage_device_t device, uint16_t volt);

/**
 * \brief Converts the read raw value to Watts.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in] device is the parameter to define if the voltage is from bus or shunt.
 *
 * \param[in] pwr is the raw power to convert.
 *
 * \return The converted power in Watts.
 */
static ina22x_power_t ina22x_convert_raw_to_W(ina22x_config_t config, uint16_t pwr);

int ina22x_init(ina22x_config_t config)
{
    int err = -1;

    if (i2c_init(config.i2c_port, config.i2c_conf) == 0)
    {
        /* Sets ina22x to default */
        if (ina22x_write_reg(config, INA22X_REG_CONFIGURATION, 0x8000U) == 0)
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

    config_mask |= ((uint16_t)config.avg_mode << 9);                /* Bits [11:9] */
    config_mask |= ((uint16_t)config.bus_voltage_conv_time << 6);   /* Bits [8:6] */
    config_mask |= ((uint16_t)config.shunt_voltage_conv_time << 3); /* Bits [5:3] */
    config_mask |= (uint16_t)config.op_mode;                        /* Bits [2:0] */

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
    uint16_t calib_reg = 0;
    calib_reg = (uint16_t)config.cal;

    if (ina22x_write_reg(config, INA22X_REG_CALIBRATION, calib_reg) == -1)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x calibration: Could not write register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;       /* Invalid calibration value */
    }

    return err;
}

int ina22x_write_reg(ina22x_config_t config, ina22x_register_t reg, uint16_t val)
{
    int err = 0;
    uint8_t buf[3] = {0};
    buf[0] = reg;
    buf[1] = val >> 8;
    buf[2] = val & (uint16_t) 0xFF;

    if (i2c_write(config.i2c_port, config.i2c_adr, buf, 3) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x write reg function: Could not write register!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;       /* Invalid calibration value */
    }

    return err;
}

int ina22x_read_reg(ina22x_config_t config, ina22x_register_t reg, uint16_t *val)
{
    int err = -1;
    uint8_t buf[2] = {0};

    if (i2c_write(config.i2c_port, config.i2c_adr, (uint8_t*)&reg, 1) == 0)
    {
        if (i2c_read(config.i2c_port, config.i2c_adr, buf, 2) == 0)
        {
            err = 0;
            *val = ((uint16_t)buf[0] << 8) | buf[1];
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

static int ina22x_get_current_raw(ina22x_config_t config, uint16_t *cur)
{
    int err = -1;
    uint16_t current_reg = 0U;
    int16_t current_signed = 0x0000;

    if (ina22x_read_reg(config, INA22X_REG_CURRENT, &current_reg) == 0)
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

static int ina22x_get_voltage_raw(ina22x_config_t config, ina22x_voltage_device_t device, uint16_t *volt)
{
    int err = -1;
    uint16_t target_reg = 0U;

    switch(device)
    {
        case INA22X_BUS_VOLTAGE:    target_reg = INA22X_REG_BUS_VOLTAGE;    err = 0;    break;
        case INA22X_SHUNT_VOLTAGE:  target_reg = INA22X_REG_SHUNT_VOLTAGE;  err = 0;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current raw: Invalid target!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    if (err == 0)
    {
        uint16_t voltage_reg = 0U;

        if (ina22x_read_reg(config, target_reg, &voltage_reg) == 0)
        {
            *volt = voltage_reg;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current: Could not read register!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

static int ina22x_get_power_raw(ina22x_config_t config, uint16_t *pwr)
{
    int err = -1;
    uint16_t power_reg = 0U;
    int16_t power_signed = 0x00;

    if (ina22x_read_reg(config, INA22X_REG_POWER, &power_reg) == 0)
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

    *pwr = power_signed;

    return err;
}

static ina22x_current_t ina22x_convert_raw_to_A(ina22x_config_t config, uint16_t cur)
{
    return (cur * config.lsb_current);
}

ina22x_voltage_t ina22x_convert_raw_to_V(ina22x_voltage_device_t device, uint16_t volt)
{
    ina22x_voltage_t voltage = 0.0;

    switch(device)
    {
        case INA22X_BUS_VOLTAGE:      voltage = (float) volt * 1.25e-3;    break;
        case INA22X_SHUNT_VOLTAGE:    voltage = (float) volt * 2.5e-6;     break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get voltage in volts, invalid device!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return voltage;
}


static ina22x_power_t ina22x_convert_raw_to_W(ina22x_config_t config, uint16_t pwr)
{
    return ((float)pwr * 25.0 * config.lsb_current);
}

int ina22x_get_current_A(ina22x_config_t config, ina22x_current_t *cur)
{
    int err = -1;
    uint16_t cur_reg = UINT16_MAX;

    if ((config.op_mode == INA22X_MODE_SHUNT_TRIG) || (config.op_mode == INA22X_MODE_BUS_TRIG) || (config.op_mode == INA22X_MODE_SHUNT_BUS_TRIG))
    {
        err = ina22x_configuration(config);
    }
    if (ina22x_get_current_raw(config, &cur_reg) == 0)
    {
        err = 0;
        *cur = ina22x_convert_raw_to_A(config, cur_reg);
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get current in amps!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}


int ina22x_get_voltage_V(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *volt)
{
    int err = -1;
    uint16_t voltage_reg = 0U;

    if ((config.op_mode == INA22X_MODE_SHUNT_TRIG) || (config.op_mode == INA22X_MODE_BUS_TRIG) || (config.op_mode == INA22X_MODE_SHUNT_BUS_TRIG))
    {
        err = ina22x_configuration(config);
    }

    if (ina22x_get_voltage_raw(config, device, &voltage_reg) == 0)
    {
        err = 0;
        *volt = ina22x_convert_raw_to_V(device, voltage_reg);
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

int ina22x_get_power_W(ina22x_config_t config, ina22x_power_t *pwr)
{
    int err = -1;
    uint16_t power_reg = UINT16_MAX;

    if ((config.op_mode == INA22X_MODE_SHUNT_TRIG) || (config.op_mode == INA22X_MODE_BUS_TRIG) || (config.op_mode == INA22X_MODE_SHUNT_BUS_TRIG))
    {
        err = ina22x_configuration(config);
    }

    if (ina22x_get_power_raw(config, &power_reg) == 0)
    {
        err = 0;
        *pwr = ina22x_convert_raw_to_W(config, power_reg);
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, INA22X_MODULE_NAME, "Error during ina22x get power in watts!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

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

    if (ina22x_read_reg(config, INA22X_REG_DIE_ID, id) == 0)
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
