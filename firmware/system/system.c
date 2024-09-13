/*
 * system.c
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
 * \brief System management routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2024/09/09
 * 
 * \addtogroup system
 * \{
 */

#include <msp430.h>
#include <drivers/gpio/gpio.h>
#include <devices/media/media.h>
#include <app/structs/ttc_data.h>

#include "system.h"

static sys_time_t sys_time = 0;

#define SYSTEM_CRC8_INITIAL_VAL   0x00U       /* CRC8-CCITT initial value. */
#define SYSTEM_CRC8_POLYNOMIAL    0x07U       /* CRC8-CCITT polynomial. */

static uint8_t system_crc8(uint8_t *data, uint8_t len);

int system_reset_count(void)
{
    int err = -1;

    uint8_t buf[3] = {0};

    /* Getting the previous reset count parameter */
    if (media_read(MEDIA_INT_FLASH, 0U, FLASH_SEG_B_ADR, buf, 3U) == 0)
    {
        if (buf[2] == system_crc8(buf, 2U))
        {
            ttc_data_buf.reset_counter = (uint16_t)(buf[0]) | ((uint16_t)(buf[1]) << 8U);

            err = 0;

            if (ttc_data_buf.reset_counter == UINT16_MAX)
            {
                ttc_data_buf.reset_counter = 0U;
            }
        }
    }

    if (err != 0)
    {
        /* Failed to get last reset counter, reseting the parameter... */
        ttc_data_buf.reset_counter = 0U;
    }

    ttc_data_buf.reset_counter++;

    if (media_erase(MEDIA_INT_FLASH, FLASH_SEG_B_ADR) == 0)
    {
        buf[0] = (uint8_t)(ttc_data_buf.reset_counter & 0xFF);
        buf[1] = (uint8_t)(ttc_data_buf.reset_counter >> 8U);
        buf[2] = system_crc8(buf, 2U);

        err = media_write(MEDIA_INT_FLASH, 0U, FLASH_SEG_B_ADR, buf, 3U);
    }

    return err;
}

void system_reset(void)
{
    PMMCTL0 = PMMPW | PMMSWBOR;     /* Triggers a software BOR */

    WDTCTL = 0xDEAD;                /* Reset system by writing to the WDT register without using the proper password */
}

uint8_t system_get_reset_cause(void)
{
    return (uint8_t)(SYSRSTIV & 0xFF);
}

void system_set_time(sys_time_t tm)
{
    sys_time = tm;
}

void system_increment_time(void)
{
    sys_time++;
}

sys_time_t system_get_time(void)
{
    return sys_time;
}

sys_hw_version_t system_get_hw_version(void)
{
    gpio_pin_t bit_0_pin = GPIO_PIN_34;
    gpio_pin_t bit_1_pin = GPIO_PIN_35;

    sys_hw_version_t res = HW_VERSION_UNKNOWN;

    gpio_config_t pin_conf = {0};

    pin_conf.mode = GPIO_MODE_INPUT;

    /* Initializing bit 0 GPIO pin */
    if (gpio_init(bit_0_pin, pin_conf) == 0)
    {
        /* Initializing bit 1 GPIO pin */
        if (gpio_init(bit_1_pin, pin_conf) == 0)
        {
            int bit_0_state = gpio_get_state(bit_0_pin);
            int bit_1_state = gpio_get_state(bit_1_pin);

            if ((bit_0_state != -1) && (bit_1_state != -1))
            {
                res = ((uint8_t)bit_1_state << 1) | (uint8_t)bit_0_state;
            }
        }
    }

    return res;
}

static uint8_t system_crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = SYSTEM_CRC8_INITIAL_VAL;

    uint8_t i = 0U;
    for(i = 0; i < len; i++)
    {
        crc ^= data[i];

        uint8_t j = 0U;
        for (j = 0U; j < 8U; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80U) ? SYSTEM_CRC8_POLYNOMIAL : 0U);
        }

        crc &= 0xFFU;
    }

    return crc;
}
/** \} End of system group */
