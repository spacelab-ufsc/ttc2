/*
 * wdt.c
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
 * \brief Internal watchdog timer driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.4
 * 
 * \date 2020/01/17
 * 
 * \addtogroup wdt
 * \{
 */

#include <hal/wdt_a.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "wdt.h"

int wdt_init(wdt_config_t config)
{
    int err = 0;

    uint8_t clk_src = UINT8_MAX;

    /* Checking clock source value */
    switch(config.clk_src)
    {
        case WDT_CLK_SRC_SMCLK:       clk_src = WDT_A_CLOCKSOURCE_SMCLK;    break;
        case WDT_CLK_SRC_ACLK:        clk_src = WDT_A_CLOCKSOURCE_ACLK;     break;
        case WDT_CLK_SRC_VLOCLK:      clk_src = WDT_A_CLOCKSOURCE_VLOCLK;   break;
        case WDT_CLK_SRC_XCLK:        clk_src = WDT_A_CLOCKSOURCE_XCLK;     break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, WDT_MODULE_NAME, "Error during initialization: Invalid clock source!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;       /* Invalid clock source */

            break;
    }

    uint8_t clk_div = UINT8_MAX;

    /* Checking clock divider value */
    switch(config.clk_div)
    {
        case WDT_CLK_DIV_2G:         clk_div = WDT_A_CLOCKDIVIDER_2G;       break;
        case WDT_CLK_DIV_128M:       clk_div = WDT_A_CLOCKDIVIDER_128M;     break;
        case WDT_CLK_DIV_8192K:      clk_div = WDT_A_CLOCKDIVIDER_8192K;    break;
        case WDT_CLK_DIV_512K:       clk_div = WDT_A_CLOCKDIVIDER_512K;     break;
        case WDT_CLK_DIV_32K:        clk_div = WDT_A_CLOCKDIVIDER_32K;      break;
        case WDT_CLK_DIV_8192:       clk_div = WDT_A_CLOCKDIVIDER_8192;     break;
        case WDT_CLK_DIV_512:        clk_div = WDT_A_CLOCKDIVIDER_512;      break;
        case WDT_CLK_DIV_64:         clk_div = WDT_A_CLOCKDIVIDER_64;       break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, WDT_MODULE_NAME, "Error during initialization: Invalid clock divider!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;       /* Invalid clock divider value */

            break;
    }

    /* Watchdog initialization */
    WDT_A_initWatchdogTimer(WDT_A_BASE, clk_src, clk_div);

    /* Start counter */
    WDT_A_start(WDT_A_BASE);

    return err;
}

void wdt_reset(void)
{
    WDT_A_resetTimer(WDT_A_BASE);
}

/** \} End of wdt group */
