/*
 * sx127x_spi.c
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
 * \brief SX127x SPI interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.21
 * 
 * \date 2021/12/15
 * 
 * \addtogroup sx127x
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/spi/spi.h>

#include "sx127x.h"

/* SPI configuration */
#define SX127X_SPI_PORT         SPI_PORT_3
#define SX127X_SPI_CS_PIN       SPI_CS_0
#define SX127X_SPI_MODE         SPI_MODE_1
#define SX127X_SPI_SPEED        CONFIG_SPI_PORT_0_SPEED_BPS

int sx127x_spi_init(void)
{
    spi_config_t conf = {0};

    conf.speed_hz   = SX127X_SPI_SPEED;
    conf.mode       = SX127X_SPI_MODE;

    return spi_init(SX127X_SPI_PORT, conf);
}

int sx127x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len)
{
    return spi_transfer(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, wd, rd, len);
}

int sx127x_spi_write_byte(uint8_t byte)
{
    return spi_write(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, &byte, 1);
}

int sx127x_spi_write(uint8_t *data, uint16_t len)
{
    return spi_write(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, data, len);
}

int sx127x_spi_read(uint8_t *data, uint16_t len)
{
    return spi_read(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, data, len);
}

/** \} End of sx127x group */
