/*
 * radio.c
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
 * \brief Radio device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 * 
 * \addtogroup radio
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <system/sys_log/sys_log.h>

#include <drivers/si446x/si446x.h>

#include <devices/leds/leds.h>

#include "radio.h"

int radio_init(void)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Initializing radio device...");
    sys_log_new_line();

    int err = -1;
    if (si446x_init() == 0)
    {
        if (si446x_rx_init())
        {
            err = 0;
        }
    }

    return err;
}

int radio_send(uint8_t *data, uint16_t len)
{
    int err = -1;

    if (si446x_mutex_take() == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Transmitting ");
        sys_log_print_uint(len);
        sys_log_print_msg(" byte(s)...");
        sys_log_new_line();

        led_set(LED_DOWNLINK);

        if(si446x_tx_long_packet(data, len))
        {
            led_clear(LED_DOWNLINK);

            if(si446x_rx_init())
            {
                err = 0;
            }
        }

        led_clear(LED_DOWNLINK);

        si446x_mutex_give();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Couldn't get mutex control.");
        sys_log_new_line();
    }

    return err;
}

int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    int res = 0;

    uint16_t i = 0;

    if (si446x_mutex_take() == 0)
    {
        for(i = 0; i < (timeout_ms/100); i++)
        {
            if (si446x_wait_nirq())
            {
                res = (int)si446x_rx_packet(data, len);

                sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Received ");
                sys_log_print_uint(res);
                sys_log_print_msg(" byte(s)...");
                sys_log_new_line();

                si446x_clear_interrupts();

                si446x_rx_init();

                break;
            }

            vTaskDelay(pdMS_TO_TICKS(500));
        }

        si446x_mutex_give();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Couldn't get mutex control.");
        sys_log_new_line();
    }

    return res;
}

int radio_available(void)
{
    return si446x_gpio_get_pin(SI446X_GPIO_NIRQ);
}

int radio_sleep(void)
{
    int err = -1;

    if (si446x_mutex_take() == 0)
    {
    err = si446x_enter_standby_mode();

    si446x_mutex_give();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Couldn't get mutex control.");
        sys_log_new_line();
    }

    return err;
}

int radio_get_temperature(radio_temp_t *temp)
{
    /* TODO */
    return -1;
}

int radio_get_rssi(radio_rssi_t *rssi)
{
    /* TODO */
    return -1;
}

/** \} End of radio group */
