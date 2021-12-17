/*
 * sx127x_gpio.c
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
 * \brief SX127x GPIO interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 2021/12/15
 * 
 * \addtogroup sx127x
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sx127x.h"

/* GPIO configuration */
#define SX127X_GPIO_RESET_PIN           GPIO_PIN_16
#define SX127X_GPIO_NIRQ_PIN            GPIO_PIN_17

int sx127x_gpio_init(void)
{
    int err = 0;

    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    /* RESET pin */
    if (gpio_init(SX127X_GPIO_RESET_PIN, conf) != 0)
    {
        err = -1;
    }

    conf.mode = GPIO_MODE_INPUT;

    /* nIRQ pin */
    if (gpio_init(SX127X_GPIO_NIRQ_PIN, conf) != 0)
    {
        err = -1;
    }

    return err;
}

int sx127X_gpio_write_reset(bool state)
{
    return gpio_set_state(SX127X_GPIO_RESET_PIN, state);
}

int sx127x_gpio_read_nirq(void)
{
    return gpio_get_state(SX127X_GPIO_NIRQ_PIN);
}

/** \} End of sx127x group */
