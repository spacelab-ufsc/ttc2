/*
 * setup.c
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
 * \brief System setup implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.9
 * 
 * \date 2020/01/22
 * 
 * \defgroup setup System setup
 * \ingroup system
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

/* The MSP430X port uses this callback function to configure its tick interrupt. */
/* This allows the application to choose the tick interrupt source. */
/* configTICK_VECTOR must also be set in FreeRTOSConfig.h to the correct */
/* interrupt vector for the chosen tick interrupt source.  This implementation of */
/* vApplicationSetupTimerInterrupt() generates the tick from timer A0, so in this */
/* case configTICK_VECTOR is set to TIMER0_A0_VECTOR */
void vApplicationSetupTimerInterrupt(void)  // cppcheck-suppress misra-c2012-8.4
{
    const unsigned short aclk_freq_hz = 32768U;

    /* Ensure the timer is stopped */
    TA0CTL = 0;

    /* Run the timer from the ACLK */
    TA0CTL = TASSEL_1;

    /* Clear everything to start with */
    TA0CTL |= TACLR;

    /* Set the compare match value according to the tick rate we want */
    TA0CCR0 = aclk_freq_hz / configTICK_RATE_HZ;

    /* Enable the interrupts */
    TA0CCTL0 = CCIE;

    /* Start up clean */
    TA0CTL |= TACLR;

    /* Up mode */
    TA0CTL |= MC_1;
}

/** \} End of setup group */
