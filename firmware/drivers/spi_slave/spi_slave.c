/*
 * spi_slave.c
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
 * \brief SPI Slave driver implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
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

int spi_slave_init(spi_port_t port, spi_config_t config)
{
    int err = 0;

    uint16_t base_address;
    uint8_t msb_first = USCI_A_SPI_MSB_FIRST;
    uint8_t clock_phase;
    uint8_t clock_polarity;

    switch(port)
    {
        case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
        case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
        case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
        case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
        case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
        case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */
            break;
    }
       if ((base_address == USCI_A0_BASE) || (base_address == USCI_A1_BASE) || (base_address == USCI_A2_BASE))
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

       else
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
               USCI_B_SPI_enable(base_address);
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

    return err;
}

int spi_slave_enable_isr(spi_port_t port)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:    USCI_A_SPI_enableInterrupt(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT | USCI_A_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_1:    USCI_A_SPI_enableInterrupt(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT | USCI_A_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_2:    USCI_A_SPI_enableInterrupt(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT | USCI_A_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_3:    USCI_B_SPI_enableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT | USCI_B_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_4:    USCI_B_SPI_enableInterrupt(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT | USCI_B_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_5:    USCI_B_SPI_enableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT | USCI_B_SPI_TRANSMIT_INTERRUPT);    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during enabling interruption: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;   /* Invalid SPI port */

        break;
    }

    return err;
}

int spi_slave_disable_isr(spi_port_t port)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:    USCI_A_SPI_disableInterrupt(USCI_A0_BASE, USCI_A_SPI_RECEIVE_INTERRUPT | USCI_A_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_1:    USCI_A_SPI_disableInterrupt(USCI_A1_BASE, USCI_A_SPI_RECEIVE_INTERRUPT | USCI_A_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_2:    USCI_A_SPI_disableInterrupt(USCI_A2_BASE, USCI_A_SPI_RECEIVE_INTERRUPT | USCI_A_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_3:    USCI_B_SPI_disableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT | USCI_B_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_4:    USCI_B_SPI_disableInterrupt(USCI_B1_BASE, USCI_B_SPI_RECEIVE_INTERRUPT | USCI_B_SPI_TRANSMIT_INTERRUPT);    break;
        case SPI_PORT_5:    USCI_B_SPI_disableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT | USCI_B_SPI_TRANSMIT_INTERRUPT);    break;
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
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading isr rx buffer: Invalid port!");
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

    uint16_t base_address;

    switch(port)
    {
        case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
        case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
        case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
        case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
        case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
        case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
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
        err = spi_read_isr_rx_buffer(port, data, len);
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

    for (i=0;i<len;i++)
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
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading tx buffer unsent bytes: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return bytes_not_sent;
}

/** \} End of spi_slave group */
