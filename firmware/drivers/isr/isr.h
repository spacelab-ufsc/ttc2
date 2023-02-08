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

typedef enum
{
    ISR_NO_CONFIG=0,           /**< Port not configured. */
    ISR_UART_CONFIG,            /**< Port configured as UART. */
    ISR_SPI_CONFIG,            /**< Port configured as SPI. */
    ISR_I2C_CONFIG             /**< Port configured as I2C. */
} isr_ports_e;

/* Ports config */
isr_ports_e isr_a0_bus;
isr_ports_e isr_a1_bus;
isr_ports_e isr_a2_bus;

isr_ports_e isr_b0_bus;
isr_ports_e isr_b1_bus;
isr_ports_e isr_b2_bus;

/* Queue UART Buffers */
queue_t uart_port_0_rx_buffer;
queue_t uart_port_1_rx_buffer;
queue_t uart_port_2_rx_buffer;

/* Queue SPI Slave Buffers */
queue_t spi_port_0_rx_buffer;
queue_t spi_port_1_rx_buffer;
queue_t spi_port_2_rx_buffer;
queue_t spi_port_3_rx_buffer;
queue_t spi_port_4_rx_buffer;
queue_t spi_port_5_rx_buffer;

queue_t spi_port_0_tx_buffer;
queue_t spi_port_1_tx_buffer;
queue_t spi_port_2_tx_buffer;
queue_t spi_port_3_tx_buffer;
queue_t spi_port_4_tx_buffer;
queue_t spi_port_5_tx_buffer;

/**
 * \brief Enables the interruption service routines.
 *
 * \return None.
 */
void isr_enable(void);

/**
 * \brief Disables the interruption service routines.
 *
 * \return None.
 */
void isr_disable(void);

#endif /* ISR_H_ */
