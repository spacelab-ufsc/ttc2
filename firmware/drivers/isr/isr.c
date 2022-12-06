/*
 * uart.c
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
 * \brief ISR driver implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2022/05/26
 *
 * \addtogroup isr
 * \{
 */


#include <hal/usci_a_uart.h>
#include <hal/usci_a_spi.h>
#include <hal/usci_b_spi.h>

#include <hal/gpio.h>

#include "drivers/spi/spi.h"
#include "drivers/spi_slave/spi_slave.h"
#include "drivers/uart/uart.h"
#include "libs/containers/queue.h"


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

void isr_enable(void)
{
    __enable_interrupt();
}

void isr_disable(void)
{
    __disable_interrupt();
}

/* Interrupt Service Routines */


#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) // cppcheck-suppress misra-c2012-8.4
{
    if (USCI_A_UART_getInterruptStatus(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG) == USCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        queue_push_back(&uart_port_0_rx_buffer, USCI_A_UART_receiveData(USCI_A0_BASE));
        USCI_A_UART_clearInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
    else if (USCI_A_SPI_getInterruptStatus(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT) == USCI_A_SPI_RECEIVE_INTERRUPT)
    {
        queue_push_back(&spi_port_0_rx_buffer, USCI_A_SPI_receiveData(USCI_A0_BASE));
        USCI_A_SPI_clearInterrupt(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);
    }
    else if (USCI_A_SPI_getInterruptStatus(USCI_A0_BASE, USCI_A_SPI_TRANSMIT_INTERRUPT) == USCI_A_SPI_TRANSMIT_INTERRUPT)
    {
        USCI_A_SPI_transmitData(USCI_A0_BASE, queue_pop_front(&spi_port_0_tx_buffer));
        USCI_A_SPI_clearInterrupt(USCI_A0_BASE, USCI_A_SPI_TRANSMIT_INTERRUPT);
    }
    else
    {
    }
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) // cppcheck-suppress misra-c2012-8.4
{
    if (USCI_A_UART_getInterruptStatus(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG) == USCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        queue_push_back(&uart_port_1_rx_buffer, USCI_A_UART_receiveData(USCI_A1_BASE));
        USCI_A_UART_clearInterrupt(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
    else if (USCI_A_SPI_getInterruptStatus(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT) == USCI_A_SPI_RECEIVE_INTERRUPT)
    {
        queue_push_back(&spi_port_1_rx_buffer, USCI_A_SPI_receiveData(USCI_A1_BASE));
        USCI_A_SPI_clearInterrupt(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);
    }
    else if (USCI_A_SPI_getInterruptStatus(USCI_A1_BASE, USCI_A_SPI_TRANSMIT_INTERRUPT) == USCI_A_SPI_TRANSMIT_INTERRUPT)
    {
        USCI_A_SPI_transmitData(USCI_A1_BASE, queue_pop_front(&spi_port_1_tx_buffer));
        USCI_A_SPI_clearInterrupt(USCI_A1_BASE, USCI_A_SPI_TRANSMIT_INTERRUPT);
    }
    else
    {
    }
}

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void) // cppcheck-suppress misra-c2012-8.4
{
    if (USCI_A_UART_getInterruptStatus(USCI_A2_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG) == USCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        queue_push_back(&uart_port_2_rx_buffer, USCI_A_UART_receiveData(USCI_A2_BASE));
        USCI_A_UART_clearInterrupt(USCI_A2_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
    else if (USCI_A_SPI_getInterruptStatus(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT) == USCI_A_SPI_RECEIVE_INTERRUPT)
    {
        queue_push_back(&spi_port_2_rx_buffer, USCI_A_SPI_receiveData(USCI_A2_BASE));
        USCI_A_SPI_clearInterrupt(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);
    }
    else if (USCI_A_SPI_getInterruptStatus(USCI_A2_BASE, USCI_A_SPI_TRANSMIT_INTERRUPT) == USCI_A_SPI_TRANSMIT_INTERRUPT)
    {
        USCI_A_SPI_transmitData(USCI_A2_BASE, queue_pop_front(&spi_port_2_tx_buffer));
        USCI_A_SPI_clearInterrupt(USCI_A2_BASE, USCI_A_SPI_TRANSMIT_INTERRUPT);
    }
    else
    {
    }
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void) // cppcheck-suppress misra-c2012-8.4
{
    if (USCI_B_SPI_getInterruptStatus(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT) == USCI_B_SPI_RECEIVE_INTERRUPT)
    {
        queue_push_back(&spi_port_3_rx_buffer, USCI_B_SPI_receiveData(USCI_B0_BASE));
        USCI_B_SPI_clearInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    }
    else if (USCI_B_SPI_getInterruptStatus(USCI_B0_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT) == USCI_B_SPI_TRANSMIT_INTERRUPT)
    {
        USCI_B_SPI_transmitData(USCI_B0_BASE, queue_pop_front(&spi_port_3_tx_buffer));
        USCI_B_SPI_clearInterrupt(USCI_B0_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT);
    }
    else
    {
    }
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) // cppcheck-suppress misra-c2012-8.4
{
    if (USCI_B_SPI_getInterruptStatus(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT) == USCI_B_SPI_RECEIVE_INTERRUPT)
    {
        queue_push_back(&spi_port_4_rx_buffer, USCI_B_SPI_receiveData(USCI_B1_BASE));
        USCI_B_SPI_clearInterrupt(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    }
    else if (USCI_B_SPI_getInterruptStatus(USCI_B1_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT) == USCI_B_SPI_TRANSMIT_INTERRUPT)
    {
        USCI_B_SPI_transmitData(USCI_B1_BASE, queue_pop_front(&spi_port_4_tx_buffer));
        USCI_B_SPI_clearInterrupt(USCI_B1_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT);
    }
    else
    {
    }
}

__interrupt void USCI_B2_ISR(void) // cppcheck-suppress misra-c2012-8.4
{
    if (USCI_B_SPI_getInterruptStatus(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT) == USCI_B_SPI_RECEIVE_INTERRUPT)
    {
        queue_push_back(&spi_port_5_rx_buffer, USCI_B_SPI_receiveData(USCI_B1_BASE));
        USCI_B_SPI_clearInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    }
    else if (USCI_B_SPI_getInterruptStatus(USCI_B2_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT) == USCI_B_SPI_TRANSMIT_INTERRUPT)
    {
        USCI_B_SPI_transmitData(USCI_B2_BASE, queue_pop_front(&spi_port_5_tx_buffer));
        USCI_B_SPI_clearInterrupt(USCI_B2_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT);
    }
    else
    {
    }
}

/** \} End of isr group */
