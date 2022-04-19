/*
 * watchdog.c
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
 * \brief Watchdog device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.16
 * 
 * \date 2019/11/01
 * 
 * \addtogroup watchdog
 * \{
 */

#include <drivers/gpio/gpio.h>
#include <drivers/wdt/wdt.h>

#include "watchdog.h"

#define WATCHDOG_MODULE_NAME        "Watchdog"

/* Internal watchdog parameters */
#define WATCHDOG_INT_CLK_SRC        WDT_CLK_SRC_ACLK
#define WATCHDOG_INT_CLK_DIV        WDT_CLK_DIV_32K

int watchdog_init(void)
{
    /* Internal watchdog configuration */
    wdt_config_t int_wdt;

    int_wdt.clk_src = WATCHDOG_INT_CLK_SRC;
    int_wdt.clk_div = WATCHDOG_INT_CLK_DIV;

    /* Watchdogs initialization */
    return wdt_init(int_wdt);
}

void watchdog_reset(void)
{
    /* Internal watchdog reset */
    wdt_reset();
}

/** \} End of watchdog group */
