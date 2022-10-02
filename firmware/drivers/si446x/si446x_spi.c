/*
 * si446x_spi.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Si446x SPI driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.10
 * 
 * \date 2017/07/29
 * 
 * \addtogroup si446x
 * \{
 */

#include <drivers/spi/spi.h>

#include "si446x.h"

int si446x_spi_init(void)
{
    spi_config_t conf = {0};

    conf.speed_hz   = 100000UL;
    conf.mode       = SPI_MODE_0;

    return spi_init(SPI_PORT_3, conf);
}

void si446x_spi_enable(void)
{
    spi_select_slave(SPI_PORT_3, SPI_CS_0, true);
}

void si446x_spi_disable(void)
{
    spi_select_slave(SPI_PORT_3, SPI_CS_0, false);
}

int si446x_spi_write(uint8_t *data, uint16_t size)
{
    return spi_write(SPI_PORT_3, SPI_CS_NONE, data, size);
}

int si446x_spi_read(uint8_t *data, uint16_t size)
{
    return spi_read(SPI_PORT_3, SPI_CS_NONE, data, size);
}

uint8_t si446x_spi_transfer(uint8_t byte)
{
    uint8_t rd = UINT8_MAX;

    spi_transfer(SPI_PORT_3, SPI_CS_NONE, &byte, &rd, 1U);

    return rd;
}

/**< \} End of si446x group */
