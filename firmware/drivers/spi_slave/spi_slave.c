/*
 * spi_slave.c
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
 * \brief SPI Slave driver implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.4
 *
 * \date 2022/05/21
 *
 * \addtogroup spi_slave
 * \{
 */

#include <hal/usci_a_spi.h>
#include <hal/usci_b_spi.h>
#include <hal/gpio.h>
#include <hal/ucs.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <libs/containers/queue.h>
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <drivers/isr/isr.h>
#include <hal/dma.h>

#include "spi_slave.h"

/**
 * \brief Reads the MTU value of a given SPI RX buffer.
 *
 * \param[in,out] spi_rx_buffer is the SPI RX buffer to get the MTU size.
 *
 * \return The MTU of the given RX buffer.
 */
static uint16_t spi_read_mtu(queue_t *spi_rx_buffer);

/**
 * \brief Reads the RX ISR buffer.
 *
 * \param[in] port is the SPI port to read. It can be:
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
 * \param[in] len is the number of bytes to be read from the buffer.
 *
 * \return The status/error code.
 */

static int spi_read_isr_rx_buffer(spi_port_t port, uint8_t *data, uint16_t len);

static int spi_slave_setup_gpio(spi_port_t port);

static uint8_t spi_slave_dma_tx_data[228] = {0};
static uint8_t spi_slave_dma_rx_data[228] = {0};

static uint16_t spi_slave_dma_tx_position;
static uint16_t spi_slave_dma_rx_position;

int spi_slave_init(spi_port_t port, spi_config_t config)
{
    int err = 0;
    uint8_t i = 0U;

    uint16_t base_address;
    uint8_t msb_first = USCI_A_SPI_MSB_FIRST;
    uint8_t clock_phase;
    uint8_t clock_polarity;

    static DMA_initParam spi_slave_dma_param_tx = {
    .channelSelect = DMA_CHANNEL_0,
    .transferModeSelect = DMA_TRANSFER_REPEATED_SINGLE,
    .transferSize = 228,
    .triggerSourceSelect = DMA_TRIGGERSOURCE_13,
    .transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE,
    .triggerTypeSelect = DMA_TRIGGER_HIGH,
    };

static DMA_initParam spi_slave_dma_param_rx = {
    .channelSelect = DMA_CHANNEL_1,
     .transferModeSelect = DMA_TRANSFER_REPEATED_SINGLE,
     .transferSize = 228,
     .triggerSourceSelect = DMA_TRIGGERSOURCE_12,
     .transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE,
     .triggerTypeSelect = DMA_TRIGGER_HIGH,

    };

    switch(port)
    {
        case SPI_PORT_0:
            base_address = USCI_A0_BASE;
            queue_init(&spi_port_0_rx_buffer);
            queue_init(&spi_port_0_tx_buffer);
            break;
        case SPI_PORT_1:
            base_address = USCI_A1_BASE;
            queue_init(&spi_port_1_rx_buffer);
            queue_init(&spi_port_1_tx_buffer);
            break;
        case SPI_PORT_2:
            base_address = USCI_A2_BASE;
            queue_init(&spi_port_2_rx_buffer);
            queue_init(&spi_port_2_tx_buffer);
            break;
        case SPI_PORT_3:
            base_address = USCI_B0_BASE;
            queue_init(&spi_port_3_rx_buffer);
            queue_init(&spi_port_3_tx_buffer);
            break;
        case SPI_PORT_4:
            base_address = USCI_B1_BASE;
            queue_init(&spi_port_4_rx_buffer);
            queue_init(&spi_port_4_tx_buffer);
            break;
        case SPI_PORT_5:
            base_address = USCI_B2_BASE;
            queue_init(&spi_port_5_rx_buffer);
            queue_init(&spi_port_5_tx_buffer);
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */
            break;
    }

    if (err == 0)
    {
        err = spi_slave_setup_gpio(port);

        if ((err == 0) && (((base_address == USCI_A0_BASE) || (base_address == USCI_A1_BASE) || (base_address == USCI_A2_BASE))))
        {
            /* SPI mode */
            switch(config.mode)
            {
                case SPI_MODE_0:
                    clock_phase       = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                    clock_polarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;

                    break;
                case SPI_MODE_1:
                    clock_phase       = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                    clock_polarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                    break;
                case SPI_MODE_2:
                    clock_phase       = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                    clock_polarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                    break;
                case SPI_MODE_3:
                    clock_phase       = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                    clock_polarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                    break;
                default:
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid mode!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    err = -1;   /* Invalid SPI mode */
                    break;
            }

            if (USCI_A_SPI_initSlave(base_address, msb_first, clock_phase, clock_polarity) == STATUS_SUCCESS)
            {
                HWREG8(base_address + OFS_UCAxCTL0) |= UCMODE_2;

                USCI_A_SPI_enable(base_address);
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error configuring as slave!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;   /* Error initializing the SPI port */
            }
        }
        else if ((err == 0) && (((base_address == USCI_B0_BASE) || (base_address == USCI_B1_BASE) || (base_address == USCI_B2_BASE))))
        {
            /* SPI mode */
            switch(config.mode)
            {
                case SPI_MODE_0:
                    clock_phase       = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                    clock_polarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                    break;
                case SPI_MODE_1:
                    clock_phase       = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                    clock_polarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                    break;
                case SPI_MODE_2:
                    clock_phase       = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                    clock_polarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                    break;
                case SPI_MODE_3:
                    clock_phase       = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                    clock_polarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                    break;
                default:
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid mode!");
                    sys_log_new_line();
               #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    err = -1;   /* Invalid SPI mode */
                    break;
            }

            if (USCI_B_SPI_initSlave(base_address, msb_first, clock_phase, clock_polarity) == STATUS_SUCCESS)
            {
                switch(config.mode)
                {
                    case SPI_MODE_1: HWREG8(base_address + OFS_UCBxCTL0) |= UCMODE_1;    break;
                    case SPI_MODE_2: HWREG8(base_address + OFS_UCBxCTL0) |= UCMODE_2;    break;
                    case SPI_MODE_3: HWREG8(base_address + OFS_UCBxCTL0) |= UCMODE_3;    break;
                    default:                                                             break;
                }

                USCI_B_SPI_enable(base_address);
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error configuring as slave!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;    /* Error initializing the SPI port */
            }
        }
        else
        {
        }

        switch(base_address)
        {
        case USCI_A0_BASE:

            spi_slave_dma_param_tx.triggerSourceSelect = DMA_TRIGGERSOURCE_17;
            spi_slave_dma_param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_16;

            break;

        case USCI_A1_BASE:
            spi_slave_dma_param_tx.triggerSourceSelect = DMA_TRIGGERSOURCE_21;
            spi_slave_dma_param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_20;

            break;

        case USCI_A2_BASE:
            spi_slave_dma_param_tx.triggerSourceSelect = DMA_TRIGGERSOURCE_13;
            spi_slave_dma_param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_12;

            break;

        case USCI_B0_BASE:
            spi_slave_dma_param_tx.triggerSourceSelect = DMA_TRIGGERSOURCE_19;
            spi_slave_dma_param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_18;

            break;

        case USCI_B1_BASE:
            spi_slave_dma_param_tx.triggerSourceSelect = DMA_TRIGGERSOURCE_23;
            spi_slave_dma_param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_22;

            break;

        case USCI_B2_BASE:
            spi_slave_dma_param_tx.triggerSourceSelect = DMA_TRIGGERSOURCE_15;
            spi_slave_dma_param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_14;
            break;

        default:
            err = -1;
            break;
        }

        DMA_init(&spi_slave_dma_param_tx);

        DMA_setSrcAddress(DMA_CHANNEL_0, (uint32_t)(uintptr_t)spi_slave_dma_tx_data, DMA_DIRECTION_INCREMENT); // cppcheck-suppress misra-c2012-11.4

        DMA_setDstAddress(DMA_CHANNEL_0, USCI_A_SPI_getTransmitBufferAddressForDMA(base_address), DMA_DIRECTION_UNCHANGED);

        DMA_clearInterrupt(DMA_CHANNEL_0);

        DMA_enableInterrupt(DMA_CHANNEL_0);

        DMA_enableTransfers(DMA_CHANNEL_0);

        spi_slave_dma_rx_position = 0U;

        for (i = 0U; i<228U; i++)
        {
            spi_slave_dma_rx_data[i] = 0xFFU;
            spi_slave_dma_tx_data[i] = 0xFFU;
        }

        spi_slave_dma_tx_data[0] = 0x7EU;
        spi_slave_dma_tx_data[1] = 0x00U;
        spi_slave_dma_tx_data[2] = 0x00U;
        spi_slave_dma_tx_data[3] = 0x00U;
        spi_slave_dma_tx_data[4] = 0x00U;
        spi_slave_dma_tx_data[5] = 0x00U;
        spi_slave_dma_tx_data[6] = 0x00U;

        /* Next command preamble */
        spi_slave_dma_tx_data[7] = 0x7EU;

        DMA_init(&spi_slave_dma_param_rx);

        DMA_setSrcAddress(DMA_CHANNEL_1, USCI_A_SPI_getReceiveBufferAddressForDMA(base_address), DMA_DIRECTION_UNCHANGED);

        DMA_setDstAddress(DMA_CHANNEL_1, (uint32_t)(uintptr_t)spi_slave_dma_rx_data, DMA_DIRECTION_INCREMENT); // cppcheck-suppress misra-c2012-11.4

        DMA_clearInterrupt(DMA_CHANNEL_1);

        DMA_enableInterrupt(DMA_CHANNEL_1);

        DMA_enableTransfers(DMA_CHANNEL_1);

        spi_slave_dma_tx_position = 8U;

        __bis_SR_register(LPM4_bits + GIE);

        if (err == 0)
        {
            switch(base_address)
            {
                case USCI_A0_BASE:  isr_a0_bus = ISR_SPI_CONFIG;    break;
                case USCI_A1_BASE:  isr_a1_bus = ISR_SPI_CONFIG;    break;
                case USCI_A2_BASE:  isr_a2_bus = ISR_SPI_CONFIG;    break;
                case USCI_B0_BASE:  isr_b0_bus = ISR_SPI_CONFIG;    break;
                case USCI_B1_BASE:  isr_b1_bus = ISR_SPI_CONFIG;    break;
                case USCI_B2_BASE:  isr_b2_bus = ISR_SPI_CONFIG;    break;
                default:                                            break;
            }
        }
        else
        {
        }
    }

    return err;
}

void spi_slave_dma_write(uint8_t *data, uint16_t len)
{
    uint16_t i = 0U;

    for (i = 0U; i <len; i++)
    {
        spi_slave_dma_tx_data[spi_slave_dma_tx_position] = data[i];

        spi_slave_dma_tx_position++;

        if (spi_slave_dma_tx_position > 227U) /* Reset position after the end of the buffer */
        {
            spi_slave_dma_tx_position = 0U;
        }
    }

    spi_slave_dma_tx_data[spi_slave_dma_tx_position] = 0x7EU;

    spi_slave_dma_tx_position++;

    if (spi_slave_dma_tx_position > 227U) /* Reset position after the end of the buffer */
    {
        spi_slave_dma_tx_position = 0U;
    }
}

void spi_slave_dma_read(uint8_t *data, uint16_t len)
{
    uint16_t i = 0U;

    if ((spi_slave_dma_rx_data[spi_slave_dma_rx_position] != 0xFFU))
    {
        for (i = 0U; i < len; i++)
        {
            data[i] = spi_slave_dma_rx_data[spi_slave_dma_rx_position];

            spi_slave_dma_rx_data[spi_slave_dma_rx_position] = 0xFFU; /* Clear after read */

            spi_slave_dma_rx_position++;

            if (spi_slave_dma_rx_position > 227U)
            {
                spi_slave_dma_rx_position = 0U; /* Reset position after the end of the buffer */
            }
        }
    }

    else
    {
        for (i=0U; i < len; i++)
        {
            data[i] = 0xFFU;
        }
    }
}

static int spi_slave_setup_gpio(spi_port_t port)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN0);
            break;
        case SPI_PORT_1:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN4 + GPIO_PIN2 + GPIO_PIN3 + GPIO_PIN1);
            break;
        case SPI_PORT_2:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN4);
            GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN3);
            break;
        case SPI_PORT_3:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3);
            break;
        case SPI_PORT_4:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN1 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN4);
            break;
        case SPI_PORT_5:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN9 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN4);
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during GPIO configuration: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    return err;
}

int spi_slave_enable_isr(spi_port_t port)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:
            USCI_A_SPI_clearInterrupt(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);
            USCI_A_SPI_enableInterrupt(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);

            break;
        case SPI_PORT_1:
            USCI_A_SPI_clearInterrupt(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);
            USCI_A_SPI_enableInterrupt(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);

            break;
        case SPI_PORT_2:
            //USCI_A_SPI_clearInterrupt(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);
            //USCI_A_SPI_enableInterrupt(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);

            break;
        case SPI_PORT_3:
            USCI_B_SPI_clearInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
            USCI_B_SPI_enableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

            break;
        case SPI_PORT_4:
            USCI_B_SPI_clearInterrupt(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
            USCI_B_SPI_enableInterrupt(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

            break;
        case SPI_PORT_5:
            USCI_B_SPI_clearInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
            USCI_B_SPI_enableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during enabling interruption: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    if (err == 0)
    {
        isr_enable();
    }

    return err;
}

int spi_slave_disable_isr(spi_port_t port)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:    USCI_A_SPI_disableInterrupt(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);    break;
        case SPI_PORT_1:    USCI_A_SPI_disableInterrupt(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);    break;
        case SPI_PORT_2:    USCI_A_SPI_disableInterrupt(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT);    break;
        case SPI_PORT_3:    USCI_B_SPI_disableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);    break;
        case SPI_PORT_4:    USCI_B_SPI_disableInterrupt(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);    break;
        case SPI_PORT_5:    USCI_B_SPI_disableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during disabling interruption: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    return err;
}

uint16_t spi_slave_read_available(spi_port_t port)
{
    uint16_t available_bytes = 0U;

    switch(port)
    {
        case SPI_PORT_0:   available_bytes = queue_size(&spi_port_0_rx_buffer);  break;
        case SPI_PORT_1:   available_bytes = queue_size(&spi_port_1_rx_buffer);  break;
        case SPI_PORT_2:   available_bytes = queue_size(&spi_port_2_rx_buffer);  break;
        case SPI_PORT_3:   available_bytes = queue_size(&spi_port_3_rx_buffer);  break;
        case SPI_PORT_4:   available_bytes = queue_size(&spi_port_4_rx_buffer);  break;
        case SPI_PORT_5:   available_bytes = queue_size(&spi_port_5_rx_buffer);  break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading buffer available bytes: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return available_bytes;
}

static uint16_t spi_read_mtu(queue_t *spi_rx_buffer)
{
    return queue_length(spi_rx_buffer);
}

int spi_read_isr_rx_buffer(spi_port_t port, uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t num_bytes = len;

    uint16_t i = 0U;

    switch(port)
    {
        case SPI_PORT_0:
            if (num_bytes > spi_read_mtu(&spi_port_0_rx_buffer))
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_WARNING, SPI_MODULE_NAME, "Port 0: Read size is bigger than RX buffer size!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                num_bytes = spi_read_mtu(&spi_port_0_rx_buffer);
            }

            for(i = 0U; i < num_bytes; i++)
            {
                data[i] = queue_pop_front(&spi_port_0_rx_buffer);
            }

            break;

        case SPI_PORT_1:
            if (num_bytes > spi_read_mtu(&spi_port_1_rx_buffer))
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_WARNING, SPI_MODULE_NAME, "Port 1: Read size is bigger than RX buffer size!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                num_bytes = spi_read_mtu(&spi_port_1_rx_buffer);
            }

            for(i = 0U; i < num_bytes; i++)
            {
                data[i] = queue_pop_front(&spi_port_1_rx_buffer);
            }

            break;

        case SPI_PORT_2:
            if (num_bytes > spi_read_mtu(&spi_port_2_rx_buffer))
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_WARNING, SPI_MODULE_NAME, "Port 2: Read size is bigger than RX buffer size!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                num_bytes = spi_read_mtu(&spi_port_2_rx_buffer);
            }

            for(i = 0U; i < num_bytes; i++)
            {
                data[i] = queue_pop_front(&spi_port_2_rx_buffer);
            }

            break;

        case SPI_PORT_3:
            if (num_bytes > spi_read_mtu(&spi_port_3_rx_buffer))
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_WARNING, SPI_MODULE_NAME, "Port 3: Read size is bigger than RX buffer size!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                num_bytes = spi_read_mtu(&spi_port_3_rx_buffer);
            }

            for(i = 0U; i < num_bytes; i++)
            {
                data[i] = queue_pop_front(&spi_port_3_rx_buffer);
            }

            break;

        case SPI_PORT_4:
            if (num_bytes > spi_read_mtu(&spi_port_4_rx_buffer))
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_WARNING, SPI_MODULE_NAME, "Port 4: Read size is bigger than RX buffer size!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                num_bytes = spi_read_mtu(&spi_port_4_rx_buffer);
            }

            for(i = 0U; i < num_bytes; i++)
            {
                data[i] = queue_pop_front(&spi_port_4_rx_buffer);
            }

            break;

        case SPI_PORT_5:
            if (num_bytes > spi_read_mtu(&spi_port_5_rx_buffer))
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_WARNING, SPI_MODULE_NAME, "Port 5: Read size is bigger than RX buffer size!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                num_bytes = spi_read_mtu(&spi_port_5_rx_buffer);
            }

            for(i = 0U; i < num_bytes; i++)
            {
                data[i] = queue_pop_front(&spi_port_5_rx_buffer);
            }

            break;

        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading ISR RX buffer: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;

            break;
    }

    return err;
}

int spi_slave_read(spi_port_t port, uint8_t *data,uint16_t len)
{
    int err = 0;

    if ((port == SPI_PORT_0) || (port == SPI_PORT_1) || (port == SPI_PORT_2) || (port == SPI_PORT_3) || (port == SPI_PORT_4) || (port == SPI_PORT_5))
    {
        err = spi_read_isr_rx_buffer(port, data, len);
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during spi_slave_read: Invalid port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int spi_slave_write(spi_port_t port, uint8_t *data, uint16_t len)
{
    int err = 0;
    uint16_t i;
    queue_t* queue;

    switch(port)
    {
        case SPI_PORT_0:    queue = &spi_port_0_tx_buffer;   break;
        case SPI_PORT_1:    queue = &spi_port_1_tx_buffer;   break;
        case SPI_PORT_2:    queue = &spi_port_2_tx_buffer;   break;
        case SPI_PORT_3:    queue = &spi_port_3_tx_buffer;   break;
        case SPI_PORT_4:    queue = &spi_port_4_tx_buffer;   break;
        case SPI_PORT_5:    queue = &spi_port_5_tx_buffer;   break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    for(i = 0U; i < len; i++)
    {
        queue_push_back(queue, data[i]);
    }

    return err;
}

int spi_slave_flush(spi_port_t port)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:   queue_clear(&spi_port_0_rx_buffer);   break;
        case SPI_PORT_1:   queue_clear(&spi_port_1_rx_buffer);   break;
        case SPI_PORT_2:   queue_clear(&spi_port_2_rx_buffer);   break;
        case SPI_PORT_3:   queue_clear(&spi_port_3_rx_buffer);   break;
        case SPI_PORT_4:   queue_clear(&spi_port_4_rx_buffer);   break;
        case SPI_PORT_5:   queue_clear(&spi_port_5_rx_buffer);   break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_SLAVE_MODULE_NAME, "Error flushing the RX buffer: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;

            break;
    }

    return err;
}

uint16_t spi_slave_bytes_not_sent(spi_port_t port)
{
    uint16_t bytes_not_sent = 0U;

    switch(port)
    {
        case SPI_PORT_0:   bytes_not_sent = queue_size(&spi_port_0_tx_buffer);  break;
        case SPI_PORT_1:   bytes_not_sent = queue_size(&spi_port_1_tx_buffer);  break;
        case SPI_PORT_2:   bytes_not_sent = queue_size(&spi_port_2_tx_buffer);  break;
        case SPI_PORT_3:   bytes_not_sent = queue_size(&spi_port_3_tx_buffer);  break;
        case SPI_PORT_4:   bytes_not_sent = queue_size(&spi_port_4_tx_buffer);  break;
        case SPI_PORT_5:   bytes_not_sent = queue_size(&spi_port_5_tx_buffer);  break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading TX buffer unsent bytes: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return bytes_not_sent;
}

/** \} End of spi_slave group */
