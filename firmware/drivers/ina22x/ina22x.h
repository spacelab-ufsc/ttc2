/*
 * ina22x.h
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
 * \brief INA22x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.0
 * 
 * \date 2022/06/01
 * 
 * \defgroup ina22x INA22x
 * \ingroup drivers
 * \{
 */

#ifndef INA22X_H_
#define INA22X_H_

#include <stdint.h>

#include <drivers/i2c/i2c.h>

/* I2C configuration */
#define INA22X_I2C_PORT                 I2C_PORT_2
#define INA22X_I2C_CLOCK_HZ             100000
#define INA22X_I2C_SLAVE_ADDRESS        4

/* Registers */
#define INA22X_REG_CONFIGURATION        0x00    /**< Configuration register. */
#define INA22X_REG_SHUNT_VOLTAGE        0x01    /**< Shunt voltage register. */
#define INA22X_REG_BUS_VOLTAGE          0x02    /**< Bus voltage register. */
#define INA22X_REG_POWER                0x03    /**< Power register. */
#define INA22X_REG_CURRENT              0x04    /**< Current register. */
#define INA22X_REG_CALIBRATION          0x05    /**< Calibration register. */
#define INA22X_REG_MASK_ENABLE          0x06    /**< Mask/Enable register. */
#define INA22X_REG_ALERT_LIMIT          0x07    /**< Alert limit register. */
#define INA22X_REG_MANUFACTURER_ID      0xFE    /**< Manufacturer ID register. */
#define INA22X_REG_DIE_ID               0xFF    /**< Die ID register. */

/* Calibration values */
typedef enum
{
    INA22X_CAL_UC=0,                            /**< Calibration values for uC */
    INA22X_CAL_RADIO,                           /**< Calibration values for radio */
} ina22x_cal_device_t;


/* Configuration */

/**
 * \brief INA22x average mode samples.
 */
typedef enum
{
    INA22X_AVERAGING_MODE_1=0X0000,             /**< 1 average sample. */
    INA22X_AVERAGING_MODE_4=0X0200,             /**< 4 average samples. */
    INA22X_AVERAGING_MODE_16,                   /**< 16 average samples. */
    INA22X_AVERAGING_MODE_64,                   /**< 64 average samples. */
    INA22X_AVERAGING_MODE_128,                  /**< 128 average samples. */
    INA22X_AVERAGING_MODE_256,                  /**< 256 average samples. */
    INA22X_AVERAGING_MODE_512,                  /**< 512 average samples. */
    INA22X_AVERAGING_MODE_1024,                 /**< 1024 average samples. */
} ina22x_averaging_mode_t;

/**
 * \brief INA22x bus voltage conversion time.
 */
typedef enum
{
    INA22X_BUS_VOLTAGE_CONV_TIME_140u=0X0000,   /**< Bus Voltage Conversion Time 140u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_204u=0X0040,   /**< Bus Voltage Conversion Time 204u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_332u,          /**< Bus Voltage Conversion Time 332u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_588u,          /**< Bus Voltage Conversion Time 588u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_1100u,         /**< Bus Voltage Conversion Time 1100u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_2116u,         /**< Bus Voltage Conversion Time 2116u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_4156u,         /**< Bus Voltage Conversion Time 4156u. */
    INA22X_BUS_VOLTAGE_CONV_TIME_8244u,         /**< Bus Voltage Conversion Time 8244u. */
} ina22x_bus_voltage_conv_time_t;

/**
 * \brief INA22x shunt voltage conversion time.
 */
typedef enum
{
    INA22X_SHUNT_VOLTAGE_CONV_TIME_140u=0X0000, /**< Shunt Voltage Conversion Time 140u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_204u=0X0008, /**< Shunt Voltage Conversion Time 204u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_332u,        /**< Shunt Voltage Conversion Time 332u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_588u,        /**< Shunt Voltage Conversion Time 588u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_1100u,       /**< Shunt Voltage Conversion Time 1100u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_2116u,       /**< Shunt Voltage Conversion Time 2116u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_4156u,       /**< Shunt Voltage Conversion Time 4156u. */
    INA22X_SHUNT_VOLTAGE_CONV_TIME_8244u,       /**< Shunt Voltage Conversion Time 8244u. */
} ina22x_shunt_voltage_conv_time_t;

/**
 * \brief INA22x modes.
 */
typedef enum
{
    INA22X_MODE_SHUTDOWN=0,                     /**< Power-Down (or Shutdown). */
    INA22X_MODE_SHUNT_TRIG=0X001,               /**< Shunt Voltage, Triggered. */
    INA22X_MODE_BUS_TRIG,                       /**< Bus Voltage, Triggered. */
    INA22X_MODE_SHUTDOWN_2,                     /**< Power-Down (or Shutdown). */
    INA22X_MODE_SHUNT_BUS_TRIG,                 /**< Shunt and Bus, Triggered. */
    INA22X_MODE_SHUNT_CONT,                     /**< Shunt Voltage, Continuous. */
    INA22X_MODE_BUS_CONT,                       /**< Bus Voltage, Continuous. */
    INA22X_MODE_SHUNT_BUS_CONT,                 /**< Shunt and Bus, Continuous. */
} ina22x_mode_t;

/**
 * \brief INA22x voltage possibilities.
 */
typedef enum
{
    INA22X_BUS_VOLTAGE=0,                       /**< Access to BUS voltage register */
    INA22X_SHUNT_VOLTAGE,                       /**< Access to shunt voltage register */
} ina22x_voltage_device_t;

/**
 * \brief INA22x register type.
 */
typedef uint8_t ina22x_reg_t;

/**
 * \brief INA22x current, voltage and power types.
 */
typedef int16_t ina22x_current_t;
typedef int16_t ina22x_voltage_t;
typedef int16_t ina22x_power_t;

/**
 * \brief INA22x ID type.
 */
typedef uint16_t ina22x_id_t;

/**
 * \brief INA22x LSB current type.
 */
typedef float ina22x_lsb_current_t;

/**
 * \brief INA22x calibration value.
 */
typedef uint16_t ina22x_cal_value_t;

typedef struct
{
    i2c_port_t i2c_port;
    i2c_config_t i2c_conf;
    ina22x_averaging_mode_t avg_mode;
    ina22x_bus_voltage_conv_time_t bus_voltage_conv_time;
    ina22x_shunt_voltage_conv_time_t shunt_voltage_conv_time;
    ina22x_mode_t op_mode;
    ina22x_cal_device_t device;
    ina22x_lsb_current_t lsb_current;
    ina22x_cal_value_t cal;

} ina22x_config_t;

/**
 * \brief INA22x initialization routine.
 *
 * This function configures the WDI pin as an GPIO output pin.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return The status/error code.
 */
int ina22x_init(ina22x_config_t config);

/**
 * \brief Configure the INA22X sensor.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \return The status/error code.
 */
int ina22x_configuration(ina22x_config_t config);

/**
 * \brief Calibrate the INA22X to scale better scale the measurements.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in] cal is the value to calibrate the sensor.
 *
 * \return The status/error code.
 */
int ina22x_calibration(ina22x_config_t config);

/**
 * \brief Writes a value to a register of the device.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in] reg is the register to write. It can be:
 * \parblock
 *      -\b INA22X_REG_CONFIGURATION
 *      -\b INA22X_REG_CALIBRATION
 *      -\b INA22X_REG_MASK_ENABLE
 *      -\b INA22X_REG_ALERT_LIMIT
 *      .
 * \endparblock
 *
 * \param[in] val is the value to write into the given register.
 *
 * \return The status/error code.
 */
int ina22x_write_reg(ina22x_config_t config, ina22x_reg_t reg, uint16_t val);

/**
 * \brief Reads the value of a given register.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in] reg is the register to read. It can be:
 * \parblock
 *      -\b INA22X_REG_CONFIGURATION
 *      -\b INA22X_REG_SHUNT_VOLTAGE
 *      -\b INA22X_REG_BUS_VOLTAGE
 *      -\b INA22X_REG_POWER
 *      -\b INA22X_REG_CURRENT
 *      -\b INA22X_REG_CALIBRATION
 *      -\b INA22X_REG_MASK_ENABLE
 *      -\b INA22X_REG_ALERT_LIMIT
 *      -\b INA22X_REG_MANUFACTURER_ID
 *      -\b INA22X_REG_DIE_ID
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read value from the given address.
 *
 * \return The status/error code.
 */
int ina22x_read_reg(ina22x_config_t config, ina22x_reg_t reg, uint16_t *val);

/**
 * \brief Reads the current from the device without conversion.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read current.
 *
 * \return The status/error code.
 */
int ina22x_get_current_raw(ina22x_config_t config, ina22x_current_t *cur);

/**
 * \brief Reads the voltage from the device without conversion.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read current.
 *
 * \return The status/error code.
 */
int ina22x_get_voltage_raw(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *vol);

/**
 * \brief Reads the power from the device without conversion.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read voltage.
 *
 * \return The status/error code.
 */
int ina22x_get_power_raw(ina22x_config_t config, ina22x_power_t *pow);

/**
 * \brief Reads the current from the device converting to Amperes.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read power.
 *
 * \return The status/error code.
 */
int ina22x_get_current_A(ina22x_config_t config, ina22x_current_t *cur);

/**
 * \brief Reads the voltage from the device converting to Volts.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read current.
 *
 * \return The status/error code.
 */
int ina22x_get_voltage_V(ina22x_config_t config, ina22x_voltage_device_t device, ina22x_voltage_t *vol);

/**
 * \brief Reads the power from the device converting to watts.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read power.
 *
 * \return The status/error code.
 */
int ina22x_get_power_W(ina22x_config_t config, ina22x_power_t *pow);

/**
 * \brief Reads the manufacturer ID.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] id is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int ina22x_get_manufacturer_id(ina22x_config_t config, ina22x_id_t *id);

/**
 * \brief Reads the die ID.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] id is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int ina22x_get_die_id(ina22x_config_t config, ina22x_id_t *id);

#endif /* INA22X_H_ */

/** \} End of ina22x group */
