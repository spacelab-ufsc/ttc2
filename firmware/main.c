/*
 * main.c
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
 * \brief Main file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.9
 * 
 * \date 2021/04/04
 * 
 * \defgroup main Main file
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include "devices/watchdog/watchdog.h"
#include "system/clocks.h"
#include "app/tasks/tasks.h"

void main(void)
{
    /* Watchdog device initialization */
    int err = watchdog_init();

    /* System clocks configuration */
    clocks_config_t clk_conf = {0};

    clk_conf.mclk_hz    = 32000000UL;
    clk_conf.smclk_hz   = 32000000UL;
    clk_conf.aclk_hz    = 32768;

    err = clocks_setup(clk_conf);

    /* Create all the tasks */
    create_tasks();

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Will only get here if there was insufficient memory to create the idle and/or timer task */
    while(1)
    {
    }
}

/** \} End of main group */
