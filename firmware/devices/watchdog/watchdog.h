/*
 * watchdog.h
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
 * \brief Watchdog device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.7
 * 
 * \date 2019/11/01
 * 
 * \defgroup watchdog Watchdog
 * \ingroup devices
 * \{
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/**
 * \brief Watchdog initialization.
 *
 * \return The status/error code.
 */
int watchdog_init(void);

/**
 * \brief Watchdog timer reset.
 *
 * \return None.
 */
void watchdog_reset(void);

#endif /* WATCHDOG_H_ */

/** \} End of watchdog group */
