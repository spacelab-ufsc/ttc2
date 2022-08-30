/*
 * watchdog.c
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
 * \brief Watchdog device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.2.6
 * 
 * \date 2019/11/01
 * 
 * \addtogroup watchdog
 * \{
 */

#include <drivers/wdt/wdt.h>
#include <drivers/tps382x/tps382x.h>

#include "watchdog.h"

#define WATCHDOG_MODULE_NAME        "Watchdog"

/* Internal watchdog parameters */
#define WATCHDOG_INT_CLK_SRC        WDT_CLK_SRC_ACLK
#define WATCHDOG_INT_CLK_DIV        WDT_CLK_DIV_32K

/* External watchdog parameters */
#define WATCHDOG_EXT_WDI_PIN        GPIO_PIN_41
#define WATCHDOG_EXT_MR_PIN         GPIO_PIN_40

static tps382x_config_t ext_wdt = {0};

int watchdog_init(void)
{
    /* Internal watchdog configuration */
    wdt_config_t int_wdt;

    int_wdt.clk_src = WATCHDOG_INT_CLK_SRC;
    int_wdt.clk_div = WATCHDOG_INT_CLK_DIV;

    ext_wdt.wdi_pin = WATCHDOG_EXT_WDI_PIN;
    ext_wdt.mr_pin = WATCHDOG_EXT_MR_PIN;

    /* Watchdogs initialization */
    return wdt_init(int_wdt) | tps382x_init(ext_wdt);
}

int watchdog_reset(void)
{
    /* Internal watchdog reset */
    wdt_reset();

    return tps382x_trigger(ext_wdt);
}

/** \} End of watchdog group */
