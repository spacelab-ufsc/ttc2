/*
 * si446x_spi.c
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
 * \brief Si446x SPI implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.18
 * 
 * \date 2017/07/29
 * 
 * \defgroup si446x_spi SPI
 * \ingroup si446x
 * \{
 */

#include <drivers/spi/spi.h>
#include <drivers/gpio/gpio.h>

#include "si446x.h"

#define SI446X_SPI_PORT         SPI_PORT_0
#define SI446X_SPI_CS_PIN       GPIO_PIN_8
#define SI446X_SPI_CLK_HZ       100000
#define SI446X_SPI_MODE         SPI_MODE_1

int si446x_spi_init(void)
{
    spi_config_t spi_conf = {0};

    spi_conf.speed_hz   = SI446X_SPI_CLK_HZ;
    spi_conf.mode       = SI446X_SPI_MODE;

    return spi_init(SI446X_SPI_PORT, spi_conf);
}

int si446x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len)
{
    return spi_transfer(SI446X_SPI_PORT, SI446X_SPI_CS_PIN, wd, rd, len);
}

int si446x_spi_write_byte(uint8_t byte)
{
    return spi_write(SI446X_SPI_PORT, SI446X_SPI_CS_PIN, &byte, 1);
}

int si446x_spi_write(uint8_t *data, uint16_t len)
{
    return spi_write(SI446X_SPI_PORT, SI446X_SPI_CS_PIN, data, len);
}

int si446x_spi_read(uint8_t *data, uint16_t len)
{
    return spi_read(SI446X_SPI_PORT, SI446X_SPI_CS_PIN, data, len);
}

/** \} End of si446x group */
