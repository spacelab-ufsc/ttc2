/*
 * spi_slave_wrap.h
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
 * \brief SPI Slave driver wrap definition.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.0.1
 * 
 * \date 2021/08/25
 * 
 * \defgroup spi_slave_wrap SPI Slave Wrap
 * \ingroup tests
 * \{
 */

#ifndef SPI_WRAP_H_
#define SPI_WRAP_H_

#include <stdint.h>
#include <drivers/spi/spi.h>


#include <drivers/spi/spi_slave.h>

int __wrap_spi_slave_init(spi_port_t port, spi_config_t config);

int __wrap_spi_slave_enable(spi_port_t port);

int __wrap_spi_slave_disable(spi_port_t port);

uint16_t __wrap_spi_slave_read_available(spi_port_t port);

int __wrap_spi_slave_read(spi_port_t port, uint8_t *data, uint16_t len);

int __wrap_spi_slave_write(spi_port_t port, uint8_t *data, uint16_t len);

int __wrap_spi_flush(spi_port_t port);

uint16_t spi_slave_bytes_not_sent(spi_port_t port);

#endif /* SPI_WRAP_H_ */

/** \} End of spi_slave_wrap group */
