/*
 * tps382x.c
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
 * \brief TPS382x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.4
 * 
 * \date 2020/01/15
 * 
 * \addtogroup tps382x
 * \{
 */

#include "tps382x.h"

int tps382x_init(tps382x_config_t config)
{
    int err = -1;

    gpio_config_t gpio_conf = {0};

    gpio_conf.mode = GPIO_MODE_OUTPUT;

    if (gpio_init(config.wdi_pin, gpio_conf) == 0)
    {
        if (gpio_init(config.mr_pin, gpio_conf) == 0)
        {
            err = 0;
        }
    }

    return err;
}

void tps382x_trigger(tps382x_config_t config)
{
    gpio_toggle(config.wdi_pin);
}

int tps382x_manual_reset(tps382x_config_t config)
{
    return gpio_set_state(config.mr_pin, false);
}

/** \} End of tps382x group */
