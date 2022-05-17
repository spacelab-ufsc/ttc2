/*
 * clocks.c
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
 * \brief System clocks functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.9
 * 
 * \date 2020/01/25
 * 
 * \addtogroup clocks
 * \{
 */

#include <hal/pmm.h>
#include <hal/ucs.h>
#include <hal/sfr.h>

#include "clocks.h"

int clocks_setup(clocks_config_t clks)
{
    /* Set VCore */
    if (clks.mclk_hz >= 25000000UL)
    {
        PMM_setVCore(PMM_CORE_LEVEL_3);
    }
    else if (clks.mclk_hz >= 20000000UL)
    {
        PMM_setVCore(PMM_CORE_LEVEL_2);
    }
    else if (clks.mclk_hz >= 12000000UL)
    {
        PMM_setVCore(PMM_CORE_LEVEL_1);
    }
    else
    {
        PMM_setVCore(PMM_CORE_LEVEL_0);
    }

    /* Set DCO FLL reference = REFO */
    UCS_initClockSignal(UCS_FLLREF, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    /* Set ACLK = REFO */
    UCS_initClockSignal(UCS_ACLK, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    /* Set SMCLK = DCO/1 */
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    /* Set Ratio and Desired MCLK Frequency and initialize DCO */
    UCS_initFLLSettle(clks.aclk_hz, clks.mclk_hz/clks.aclk_hz);

    /* Enable global oscillator fault flag */
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);

    return 0;
}

clocks_config_t clocks_read(void)
{
    clocks_config_t clks;

    clks.mclk_hz = UCS_getMCLK();
    clks.smclk_hz = UCS_getSMCLK();
    clks.aclk_hz = UCS_getACLK();

    return clks;
}

/**
 * \brief CPU clock failure ISR.
 *
 * \return None.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(UNMI_VECTOR)))
#endif
void NMI_ISR(void)  // cppcheck-suppress misra-c2012-8.4
{
    static uint16_t status = 0;

    do
    {
        /* If it still can't clear the oscillator fault flags after the timeout, trap and wait here */
        status = UCS_clearAllOscFlagsWithTimeout(1000);
    }
    while(status != 0U);
}

/** \} End of clocks group */
