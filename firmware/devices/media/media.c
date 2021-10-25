/*
 * media.c
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
 * \brief Media device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.7
 * 
 * \date 2020/07/21
 * 
 * \addtogroup media
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/flash/flash.h>

#include "media.h"

int media_init(media_t med)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
            err = flash_init();

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to initialize!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_write(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            /* Address index */
            uint32_t adr_idx = adr + FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4U)
            {
                uint32_t adr_counter = adr_idx + i;

                flash_write_long((uint32_t)data[i], &adr_counter);
            }

            err = 0;

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to write!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_read(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            /* Address index */
            uint32_t adr_idx = adr + FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4U)
            {
                uint32_t adr_counter = adr_idx + i;

                data[i] = (uint8_t)flash_read_long(&adr_counter);
            }

            err = 0;

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to read!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_erase(media_t med, media_erase_t type, uint32_t sector)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            uint8_t sector_conv = UINT8_MAX;
            if (sector > UINT8_MAX)
            {
                sector_conv = UINT8_MAX;
            }
            else
            {
                sector_conv = (uint8_t)sector;
            }

            flash_write_single(0xFF, &sector_conv);

            err = 0;

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to erase!");
            sys_log_new_line();

            break;
    }

    return err;
}

media_info_t media_get_info(media_t med)
{
    media_info_t info = {0};

    switch(med)
    {
        case MEDIA_INT_FLASH:                                           break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to get the information!");
            sys_log_new_line();

            break;
    }

    return info;
}

/** \} End of media group */
