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
 * \version 0.1.11
 * 
 * \date 2021/04/10
 * 
 * \defgroup ina22x INA22x
 * \ingroup drivers
 * \{
 */

#ifndef INA22X_H_
#define INA22X_H_

#include <stdint.h>

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

/**
 * \brief INA22x register type.
 */
typedef uint8_t ina22x_reg_t;

/**
 * \brief INA22x current type.
 */
typedef int16_t ina22x_current_t;

/**
 * \brief INA22x ID type.
 */
typedef uint16_t ina22x_id_t;

/**
 * \brief INA22x configuration.
 */
typedef struct
{
    i2c_port_t i2c_port;
    i2c_config_t i2c_conf;
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
 * \brief Reads the current from the device.
 *
 * \param[in] config is configuration parameters of the driver.
 *
 * \param[in,out] cur is a pointer to store the read current.
 *
 * \note The current value is in Amperes.
 *
 * \return The status/error code.
 */
int ina22x_get_current(ina22x_config_t config, ina22x_current_t *cur);

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
