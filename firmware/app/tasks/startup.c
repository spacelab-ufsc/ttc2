/*
 * startup.c
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
 * \brief Startup task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 * 
 * \addtogroup startup
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <system/clocks.h>
#include <devices/watchdog/watchdog.h>
#include <devices/leds/leds.h>
#include <devices/radio/radio.h>
#include <devices/power_sensor/power_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <devices/antenna/antenna.h>
#include <devices/media/media.h>
#include <devices/eps/eps.h>
#include <app/structs/ttc_data.h>
#include <devices/obdh/obdh.h>

#include <ngham/ngham.h>

#include "startup.h"

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void)
{
    unsigned int error_counter = 0;

    /* Logger device initialization */
    sys_log_init();

    /* Print the TTC radio module */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Booting TTC Module ");
    sys_log_print_uint((uint32_t)RADIO_MODULE);
    sys_log_new_line();

    /* Print the FreeRTOS version */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "FreeRTOS ");
    sys_log_print_msg(tskKERNEL_VERSION_NUMBER);
    sys_log_new_line();

    /* Print the hardware version */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Hardware revision is ");
    sys_log_print_uint(system_get_hw_version());
    sys_log_new_line();

    /* Print the system clocks */
    clocks_config_t clks = clocks_read();
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "System clocks: MCLK=");
    sys_log_print_uint(clks.mclk_hz);
    sys_log_print_msg(" Hz, SMCLK=");
    sys_log_print_uint(clks.smclk_hz);
    sys_log_print_msg(" Hz, ACLK=");
    sys_log_print_uint(clks.aclk_hz);
    sys_log_print_msg(" Hz");
    sys_log_new_line();

    /* Print last reset cause (code) */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Last reset cause: ");
    sys_log_print_hex(system_get_reset_cause());
    sys_log_new_line();

    /* TTC parameters */
    ttc_data_buf.hw_version = 0x04;
    ttc_data_buf.fw_version = 0x00000405;
    ttc_data_buf.device_id = 0xCC2A + RADIO_MODULE;

#if defined(CONFIG_DEV_MEDIA_INT_ENABLED) && (CONFIG_DEV_MEDIA_INT_ENABLED == 1)
    /* Internal non-volatile memory initialization */
    if (media_init(MEDIA_INT_FLASH) != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_MEDIA_INT_ENABLED */

    /* LEDs device initialization */
#if defined(CONFIG_DEV_LEDS_ENABLED) && (CONFIG_DEV_LEDS_ENABLED == 1)
    if (leds_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_LEDS_ENABLED */

    /* Power sensor device initialization */
#if defined(CONFIG_DEV_POWER_SENSOR_ENABLED) && (CONFIG_DEV_POWER_SENSOR_ENABLED == 1)
    if (power_sensor_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_POWER_SENSOR_ENABLED */

    /* Temperature sensor device initialization */
#if defined(CONFIG_DEV_TEMP_SENSOR_ENABLED) && (CONFIG_DEV_TEMP_SENSOR_ENABLED == 1)
    if (temp_sensor_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_TEMP_SENSOR_ENABLED */

    /* Radio device initialization */
#if defined(CONFIG_DEV_RADIO_ENABLED) && (CONFIG_DEV_RADIO_ENABLED == 1)
    if (radio_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_RADIO_ENABLED */

    /* NGHam initialization */
    if (ngham_init() != 0)
    {
        error_counter++;
    }

    /* Antenna device initialization */
#if defined(CONFIG_DEV_ANTENNA_ENABLED) && (CONFIG_DEV_ANTENNA_ENABLED == 1)
    if (antenna_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_ANTENNA_ENABLED */

#if defined(CONFIG_DEV_EPS_ENABLED) && (CONFIG_DEV_EPS_ENABLED == 1)
    if (eps_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_EPS_ENABLED */

#if defined(CONFIG_DEV_OBDH_ENABLED) && (CONFIG_DEV_OBDH_ENABLED == 1)
    if (obdh_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_OBDH_ENABLED */

    /* Checking and updating the reset counter parameter */
    if (system_reset_count() == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Reset counter: ");
        sys_log_print_uint((uint32_t)(ttc_data_buf.reset_counter));
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME, "Failed to access last reset counter");
        sys_log_new_line();
    }


    if (error_counter > 0U)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME, "TTC Radio Module ");
        sys_log_print_uint((uint32_t)RADIO_MODULE);
        sys_log_print_msg(" booted with ");
        sys_log_print_uint((uint32_t)error_counter);
        sys_log_print_msg(" ERROR(S)!");
        sys_log_new_line();

        led_set(LED_FAULT);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Successfully booted TTC Radio Module ");
        sys_log_print_uint((uint32_t)RADIO_MODULE);
        sys_log_print_msg("!");
        sys_log_new_line();

        led_clear(LED_FAULT);
    }

    /* Startup task status = Done */
    xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

    vTaskSuspend(xTaskStartupHandle);
}

/** \} End of startup group */
