/*
 * spi_slave.h
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
 * \brief SPI Slave driver definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2022/05/21
 *
 * \defgroup spi_slave SPI_SLAVE
 * \ingroup drivers
 * \{
 */


#ifndef DRIVERS_SPI_SLAVE_SPI_SLAVE_H_
#define DRIVERS_SPI_SLAVE_SPI_SLAVE_H_

#define SPI_MODULE_NAME         "SPI_SLAVE"

int spi_slave_init(spi_port_t port, spi_config_t config);

int spi_slave_enable_isr(spi_port_t port);

int spi_slave_disable_isr(spi_port_t port);

uint16_t spi_slave_read_available(spi_port_t port);

int spi_slave_read(spi_port_t port, uint8_t *data, uint16_t len);

int spi_slave_write(spi_port_t port, uint8_t *data, uint16_t len);

int spi_flush(spi_port_t port);

#endif /* DRIVERS_SPI_SLAVE_SPI_SLAVE_H_ */
