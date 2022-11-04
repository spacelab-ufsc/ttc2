/*
 * uart.h
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
 * \brief ISR driver definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2022/05/26
 *
 * \defgroup isr ISR
 * \ingroup drivers
 * \{
 */

#ifndef ISR_H_
#define ISR_H_
#include <libs/containers/queue.h>

/* SPI Slave buffers */
extern queue_t spi_port_0_rx_buffer;
extern queue_t spi_port_1_rx_buffer;
extern queue_t spi_port_2_rx_buffer;
extern queue_t spi_port_3_rx_buffer;
extern queue_t spi_port_4_rx_buffer;
extern queue_t spi_port_5_rx_buffer;

extern queue_t spi_port_0_tx_buffer;
extern queue_t spi_port_1_tx_buffer;
extern queue_t spi_port_2_tx_buffer;
extern queue_t spi_port_3_tx_buffer;
extern queue_t spi_port_4_tx_buffer;
extern queue_t spi_port_5_tx_buffer;

/* UART buffers */
extern queue_t uart_port_0_rx_buffer;
extern queue_t uart_port_1_rx_buffer;
extern queue_t uart_port_2_rx_buffer;

#endif /* ISR_H_ */
