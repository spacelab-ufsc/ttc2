/*
 * spi_slave.h
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
 * \brief SPI Slave driver definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.4.5
 *
 * \date 2022/05/21
 *
 * \defgroup spi_slave SPI_SLAVE
 * \ingroup drivers
 * \{
 */


#ifndef DRIVERS_SPI_SLAVE_SPI_SLAVE_H_
#define DRIVERS_SPI_SLAVE_SPI_SLAVE_H_

#define SPI_SLAVE_MODULE_NAME         "SPI_SLAVE"

#include <drivers/spi/spi.h>

/**
 * \brief SPI Slave port initialization.
 *
 * \param[in] port is the SPI port to initialize. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 * \param[in] config is the configuration of the SPI port.
 *
 * \return The status/error code.
 */
int spi_slave_init(spi_port_t port, spi_config_t config);

/**
 * \brief Writes on the SPI Slave port DMA buffer.
 *
 * \param[in] data is the array to be transfered to the DMA buffer.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return None.
 */
void spi_slave_dma_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads on the SPI Slave port DMA buffer.
 *
 * \param[out] data is the array to be transfered from the DMA buffer.
 *
 * \param[in] len is the number of bytes to be read.
 *
 * \return None.
 */
void spi_slave_dma_read(uint8_t *data, uint16_t len);

void spi_slave_dma_change_transfer_size(uint16_t transfer_size);

/**
 * \brief Enables SPI Slave port interruption.
 *
 * \param[in] port is the SPI port to be enabled. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int spi_slave_enable_isr(spi_port_t port);

/**
 * \brief Disables SPI Slave port interruption.
 *
 * \param[in] port is the SPI port to be disabled. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int spi_slave_disable_isr(spi_port_t port);

/**
 * \brief Reads the number of data bytes available to be read from the buffer.
 *
 * \param[in] port is the SPI port to be read. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 *
 * \return The number of bytes received in the buffer.
 */
uint16_t spi_slave_read_available(spi_port_t port);

/**
 * \brief Reads data from a given SPI port.
 *
 * \param[in] port is the SPI port to be read. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 * \param[in] data is an array to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int spi_slave_read(spi_port_t port, uint8_t *data, uint16_t len);

/**
 * \brief Writes data from a given SPI port.
 *
 * \param[in] port is the SPI port to be written. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 * \param[in] data is an array with stored data.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return The status/error code.
 */
int spi_slave_write(spi_port_t port, uint8_t *data, uint16_t len);

/**
 * \brief Flushes/Clears the RX buffer of a given port.
 *
 * \param[in] port is the SPI port to flush. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int spi_slave_flush(spi_port_t port);

/**
 * \brief Reads the number of bytes of data not yet sent.
 *
 * \param[in] port is the SPI port to be read. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      -\b SPI_PORT_3
 *      -\b SPI_PORT_4
 *      -\b SPI_PORT_5
 *      .
 * \endparblock
 *
 *
 * \return The number of unsent bytes.
 */
uint16_t spi_slave_bytes_not_sent(spi_port_t port);

#endif /* DRIVERS_SPI_SLAVE_SPI_SLAVE_H_ */

/** \} End of spi_slave group */
