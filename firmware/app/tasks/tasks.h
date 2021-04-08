/*
 * tasks.h
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
 * \brief Tasks definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.9
 * 
 * \date 2019/11/02
 * 
 * \defgroup tasks Tasks
 * \{
 */

#ifndef TASKS_H_
#define TASKS_H_

/**
 * \brief Creates the system tasks.
 *
 * \return None.
 */
void create_tasks(void);

/**
 * \brief Creates the system event groups.
 *
 * \return None.
 */
void create_event_groups(void);

#endif /* TASKS_H_ */

/** \} End of tasks group */
