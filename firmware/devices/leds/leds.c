/*
 * leds.c
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
 * \brief System LEDs implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.12
 * 
 * \date 2020/01/20
 * 
 * \addtogroup leds
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <drivers/gpio/gpio.h>

#include "leds.h"

/* GPIO configuration */
#define LED_SYSTEM_PIN          GPIO_PIN_36
#define LED_FAULT_PIN           GPIO_PIN_28
#define LED_DOWNLINK_PIN        GPIO_PIN_30
#define LED_UPLINK_PIN          GPIO_PIN_29

int leds_init(void)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, LEDS_MODULE_NAME, "Initializing system LEDs...");
    sys_log_new_line();

    gpio_config_t config_sys        = {.mode = GPIO_MODE_OUTPUT};
    gpio_config_t config_fault      = {.mode = GPIO_MODE_OUTPUT};
    gpio_config_t config_downlink   = {.mode = GPIO_MODE_OUTPUT};
    gpio_config_t config_uplink     = {.mode = GPIO_MODE_OUTPUT};

    if ((gpio_init(LED_SYSTEM_PIN, config_sys) != 0) ||
        (gpio_init(LED_FAULT_PIN, config_fault) != 0) ||
        (gpio_init(LED_DOWNLINK_PIN, config_downlink) != 0) ||
        (gpio_init(LED_UPLINK_PIN, config_uplink) != 0))
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, LEDS_MODULE_NAME, "Error initializing the system LEDs!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int led_set(led_t l)
{
    switch(l)
    {
        case LED_SYSTEM:    return gpio_set_state(LED_SYSTEM_PIN, true);
        case LED_FAULT:     return gpio_set_state(LED_FAULT_PIN, true);
        case LED_DOWNLINK:  return gpio_set_state(LED_DOWNLINK_PIN, true);
        case LED_UPLINK:    return gpio_set_state(LED_UPLINK_PIN, true);
        default:            return -1;      /* Invalid LED */
    }
}

int led_clear(led_t l)
{
    switch(l)
    {
        case LED_SYSTEM:    return gpio_set_state(LED_SYSTEM_PIN, false);
        case LED_FAULT:     return gpio_set_state(LED_FAULT_PIN, false);
        case LED_DOWNLINK:  return gpio_set_state(LED_DOWNLINK_PIN, false);
        case LED_UPLINK:    return gpio_set_state(LED_UPLINK_PIN, false);
        default:            return -1;      /* Invalid LED */
    }
}

int led_toggle(led_t l)
{
    switch(l)
    {
        case LED_SYSTEM:    return gpio_toggle(LED_SYSTEM_PIN);
        case LED_FAULT:     return gpio_toggle(LED_FAULT_PIN);
        case LED_DOWNLINK:  return gpio_toggle(LED_DOWNLINK_PIN);
        case LED_UPLINK:    return gpio_toggle(LED_UPLINK_PIN);
        default:            return -1;      /* Invalid LED */
    }
}

/** \} End of leds group */
