/*
 * si446x_mutex.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Si446x mutex functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.3
 * 
 * \date 2023/04/13
 * 
 * \addtogroup si446x
 * \{
 */

#include <FreeRTOS.h>
#include <semphr.h>

#include "si446x.h"

int si446x_mutex_create(void)
{
    return -1;
}

int si446x_mutex_take(void)
{
    return -1;
}

int si446x_mutex_give(void)
{
    return -1;
}

/**< \} End of si446x group */
