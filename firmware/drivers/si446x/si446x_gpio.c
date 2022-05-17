/*
 * si446x_gpio.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Si446x GPIO driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.23
 * 
 * \date 2017/07/29
 * 
 * \addtogroup rf4463
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "si446x.h"

void si446x_gpio_init(void)
{
    int err = -1;

    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    /* SDN pin */
    if (gpio_init(GPIO_PIN_16, conf) == 0)
    {
        conf.mode = GPIO_MODE_INPUT;

        /* nIRQ pin */
        err = gpio_init(GPIO_PIN_17, conf);
    }
}

void si446x_gpio_set_pin(si446x_gpio_pin_t pin)
{
    switch(pin)
    {
        case SI446X_GPIO_SDN:       gpio_set_state(GPIO_PIN_16, true);      break;
        case SI446X_GPIO_GPIO_0:    gpio_set_state(GPIO_PIN_18, true);      break;
        case SI446X_GPIO_GPIO_1:    gpio_set_state(GPIO_PIN_19, true);      break;
        default:                                                            break;
    }
}

void si446x_gpio_clear_pin(si446x_gpio_pin_t pin)
{
    switch(pin)
    {
        case SI446X_GPIO_SDN:       gpio_set_state(GPIO_PIN_16, false);     break;
        case SI446X_GPIO_GPIO_0:    gpio_set_state(GPIO_PIN_18, false);     break;
        case SI446X_GPIO_GPIO_1:    gpio_set_state(GPIO_PIN_19, false);     break;
        default:                                                            break;
    }
}

int si446x_gpio_get_pin(si446x_gpio_pin_t pin)
{
    switch(pin)
    {
        case SI446X_GPIO_NIRQ:      return gpio_get_state(GPIO_PIN_17);
        case SI446X_GPIO_GPIO_0:    return gpio_get_state(GPIO_PIN_18);
        case SI446X_GPIO_GPIO_1:    return gpio_get_state(GPIO_PIN_19);
        default:                    return -1;
    }
}

/**< \} End of rf4463 group */
