/*
 * uart.c
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
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief UART driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.1.4
 * 
 * \date 2019/12/07
 * 
 * \addtogroup uart
 * \{
 */

#include <hal/usci_a_uart.h>
#include <hal/gpio.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "uart.h"

/* Uart Interruption Buffers */
extern uint16_t buffer_size;

static uint8_t uart_usci_a0_rx_buffer[buffer_size]; /* USCI_A0 receiver buffer */
static uint8_t uart_usci_a1_rx_buffer[buffer_size]; /* USCI_A1 receiver buffer */
static uint8_t uart_usci_a2_rx_buffer[buffer_size]; /* USCI_A2 receiver buffer */

/* Uart buffer positions */
static uint16_t uart_usci_a0_rx_buffer_pos;
static uint16_t uart_usci_a1_rx_buffer_pos;
static uint16_t uart_usci_a2_rx_buffer_pos;

int uart_init(uart_port_t port, uart_config_t config)
{
    int err = 0;

    USCI_A_UART_initParam uart_params = {0};

    switch(config.baudrate)
    {
        case 1200:
            uart_params.clockPrescalar      = 1665; /* 1200 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 11;   /* 1200 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 1200 bps @ 31.981568 MHz */
            break;
        case 2400:
            uart_params.clockPrescalar      = 832;  /* 2400 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 14;   /* 2400 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 2400 bps @ 31.981568 MHz */
            break;
        case 4800:
            uart_params.clockPrescalar      = 416;  /* 4800 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 7;    /* 4800 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 4800 bps @ 31.981568 MHz */
            break;
        case 9600:
            uart_params.clockPrescalar      = 208;  /* 9600 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 3;    /* 9600 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 9600 bps @ 31.981568 MHz */
            break;
        case 19200:
            uart_params.clockPrescalar      = 104;  /* 19200 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 2;    /* 19200 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 19200 bps @ 31.981568 MHz */
            break;
        case 38400:
            uart_params.clockPrescalar      = 52;   /* 38400 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 1;    /* 38400 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 38400 bps @ 31.981568 MHz */
            break;
        case 57600:
            uart_params.clockPrescalar      = 34;   /* 57600 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 11;   /* 57600 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 57600 bps @ 31.981568 MHz */
            break;
        case 115200:
            uart_params.clockPrescalar      = 17;   /* 115200 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 6;    /* 115200 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 0;    /* 115200 bps @ 31.981568 MHz */
            break;
        case 230400:
            uart_params.clockPrescalar      = 8;    /* 230400 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 11;   /* 230400 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 5;    /* 230400 bps @ 31.981568 MHz */
            break;
        case 460800:
            uart_params.clockPrescalar      = 4;    /* 460800 bps @ 31.981568 MHz */
            uart_params.firstModReg         = 3;    /* 460800 bps @ 31.981568 MHz */
            uart_params.secondModReg        = 5;    /* 460800 bps @ 31.981568 MHz */
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during the initialization: Invalid baudrate!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;       /* Invalid baudrate value */
            break;
    }

    uart_params.selectClockSource   = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.parity              = config.parity;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = config.stop_bits;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:
            base_address = USCI_A0_BASE;

            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN4 + GPIO_PIN5);

            break;
        case UART_PORT_1:
            base_address = USCI_A1_BASE;

            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN2 + GPIO_PIN3);

            break;
        case UART_PORT_2:
            base_address = USCI_A2_BASE;

            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN2 + GPIO_PIN3);

            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during the initialization: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;       /* Invalid port */

            break;
    }

    if (USCI_A_UART_init(base_address, &uart_params) != STATUS_SUCCESS)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during the initialization!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    USCI_A_UART_enable(base_address);
    USCI_A_UART_enableInterrupt(base_address, USCI_A_UART_RECEIVE_INTERRUPT); //Enables RX interruption

    return err;
}

int uart_available(uart_port_t port)
{
    int err = 0;

    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:   base_address = USCI_A0_BASE;    break;
        case UART_PORT_1:   base_address = USCI_A1_BASE;    break;
        case UART_PORT_2:   base_address = USCI_A2_BASE;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during RX buffer verification: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid UART port */

            break;
    }

    /* Check RX interrupt flag */
    uint8_t status = USCI_A_UART_getInterruptStatus(base_address, USCI_A_UART_RECEIVE_INTERRUPT_FLAG);

    if ((status | USCI_A_UART_RECEIVE_INTERRUPT_FLAG) > 0)
    {
        err = UART_AVAILABLE;
    }
    else
    {
        err = UART_NOT_AVAILABLE;
    }

    return err;
}

int uart_flush(uart_port_t port)
{
    int err = 0;

    switch(port)
    {
        case UART_PORT_0:   break;
        case UART_PORT_1:   break;
        case UART_PORT_2:   break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error flushing the RX buffer: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid UART port */

            break;
    }

    while(uart_available(port) == (int)UART_AVAILABLE)
    {
        uint8_t dummy;
        err = uart_read(port, &dummy, 1);
    }

    return err;
}

int uart_write(uart_port_t port, uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:   base_address = USCI_A0_BASE;    break;
        case UART_PORT_1:   base_address = USCI_A1_BASE;    break;
        case UART_PORT_2:   base_address = USCI_A2_BASE;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during writing: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;

            break;
    }

    uint16_t i = 0;
    for(i=0; i<len; i++)
    {
        USCI_A_UART_transmitData(base_address, data[i]);
    }

    return err;
}

int uart_read(uart_port_t port, uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:   base_address = USCI_A0_BASE;    break;
        case UART_PORT_1:   base_address = USCI_A1_BASE;    break;
        case UART_PORT_2:   base_address = USCI_A2_BASE;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during reading: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;

            break;
    }

    uint16_t i = 0;
    for(i=0; i<len; i++)
    {
        data[i] = USCI_A_UART_receiveData(base_address);
    }

    return err;
}


int uart_read_isr_rx_buffer(uart_port_t port, uint8_t *data)
{
    int err = 0;
    uint16_t i = 0;
    switch(port)
    {
    case UART_PORT_0:
        for(i=0; i<buffer_size; i++)
            {
                data[i] = uart_usci_a0_rx_buffer[i];
            }
        break;
    case UART_PORT_1:
        for(i=0; i<buffer_size; i++)
            {
                data[i] = uart_usci_a1_rx_buffer[i];
            }
        break;
    case UART_PORT_2:
        for(i=0; i<buffer_size; i++)
            {
                data[i] = uart_usci_a2_rx_buffer[i];
            }
        break;
    default:
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, UART_MODULE_NAME, "Error during reading isr rx buffer: Invalid port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;

}

int uart_return_isr_rx_buffer_size(void)
{
    return buffer_size;
}

/* Interrupt Service Routines */

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    if (USCI_A_UART_getInterruptStatus(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG))
    {
        uart_usci_a0_rx_buffer[uart_usci_a0_rx_buffer_pos++] = UART_A_UART_receiveData(USCI_A0_BASE);
        if (uart_usci_a0_rx_buffer_pos == buffer_size) uart_usci_a0_rx_buffer_pos = 0; //Returns to the first position
    }
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    if (USCI_A_UART_getInterruptStatus(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG))
    {
        uart_usci_a1_rx_buffer[uart_usci_a1_rx_buffer_pos++] = UART_A_UART_receiveData(USCI_A1_BASE);
        if (uart_usci_a1_rx_buffer_pos == buffer_size) uart_usci_a1_rx_buffer_pos = 0; //Returns to the first position
    }
}

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
    if (USCI_A_UART_getInterruptStatus(USCI_A2_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG))
    {
        uart_usci_a2_rx_buffer[uart_usci_a2_rx_buffer_pos++] = UART_A_UART_receiveData(USCI_A2_BASE);
        if (uart_usci_a2_rx_buffer_pos == buffer_size) uart_usci_a2_rx_buffer_pos = 0; //Returns to the first position
    }
}
/** \} End of uart group */
