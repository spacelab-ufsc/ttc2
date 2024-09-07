/*
 * media.c
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
 * \brief Media device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.2
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

int media_write(media_t med, uint32_t adr, uint32_t sector, uint8_t *data, uint16_t len)
{
    int err = -1;
    uint16_t i = 0U;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            if (flash_mutex_take() == 0)
            {
                /* Address index */
                uintptr_t adr_idx = adr + sector;

                for(i=0; i<len; ++i)
                {
                    uintptr_t adr_counter = adr_idx + i;

                    flash_write_single(data[i], adr_counter);
                }

                err = flash_mutex_give();
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Couldn't get mutex control.");
                sys_log_new_line();
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to write!");
            sys_log_new_line();

            break;
        }
    }

    return err;
}

int media_read(media_t med, uint32_t adr, uint32_t sector, uint8_t *data, uint16_t len)
{
    int err = -1;
    uint16_t i = 0;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            if (flash_mutex_take() == 0)
            {
                /* Address index */
                uintptr_t adr_idx = adr + sector;

                for(i=0; i<len; ++i)
                {
                    uintptr_t adr_counter = adr_idx + i;

                    data[i] = flash_read_single(adr_counter);
                }

                err = flash_mutex_give();
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Couldn't get mutex control.");
                sys_log_new_line();
            }
            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to read!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_erase(media_t med, uint32_t sector)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
            if (flash_mutex_take() == 0)
            {
                if ((sector == FLASH_SEG_A_ADR) || (sector == FLASH_SEG_B_ADR))
                {
                    flash_erase((uintptr_t)sector);
                    err = flash_mutex_give();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Erasing invalid sector!");
                    sys_log_new_line();
                }

            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Couldn't get mutex control.");
                sys_log_new_line();
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to erase!");
            sys_log_new_line();

            break;
    }

    return err;
}

/** \} End of media group */
