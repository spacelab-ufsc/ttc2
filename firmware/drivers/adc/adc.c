/*
 * adc.c
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
 * \brief ADC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.0.5
 * 
 * \date 2020/03/03
 * 
 * \addtogroup adc
 * \{
 */

#include <stdbool.h>

#include <hal/adc10_a.h>
#include <hal/adc12_a.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "adc.h"

bool adc_is_ready = false;

int adc_init(adc_port_t port, adc_config_t config)
{
    if (adc_is_ready)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_WARNING, ADC_MODULE_NAME, "ADC driver already initialized!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return 0;
    }

    /* Reset REFMSTR to hand over control to ADC12_A ref control registers */
    REFCTL0 &= ~REFMSTR;

    /* Vref+ = 3.0 V, Vref- = 0 V */
    ADC12CTL0 = ADC12MSC | ADC12SHT0_15 | ADC12REFON | ADC12ON;

    /* Enable sample timer */
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;

    P6SEL |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                 /* Daughterboard ADC0. */
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;                 /* Daughterboard ADC1. */
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;                 /* Daughterboard ADC2. */
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;                 /* Current sensor. */
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;                 /* Voltage sensor. */
    ADC12MCTL5 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;     /* Temperature sensor. */

    /* Allow ~100us (at default UCS settings) for REF to settle */
    adc_delay_ms(1);

    ADC12CTL0 |= ADC12ENC;

    adc_is_ready = true;

    return 0;
}

int adc_read(adc_port_t port, uint16_t *val)
{
    while(ADC12CTL1 & ADC12BUSY);

    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

    switch(port)
    {
        case ADC_PORT_0:
            while(!(ADC12IFG & (1 << 0)));

            *val = ADC12MEM0;

            break;
        case ADC_PORT_1:
            while(!(ADC12IFG & (1 << 1)));

            *val = ADC12MEM1;

            break;
        case ADC_PORT_2:
            while(!(ADC12IFG & (1 << 2)));

            *val = ADC12MEM2;

            break;
        case ADC_PORT_3:
            while(!(ADC12IFG & (1 << 3)));

            *val = ADC12MEM3;

            break;
        case ADC_PORT_4:
            while(!(ADC12IFG & (1 << 4)));

            *val = ADC12MEM4;

            break;
        case ADC_PORT_5:
            while(!(ADC12IFG & (1 << 5)));

            *val = ADC12MEM5;

            break;
        case ADC_PORT_6:
            while(!(ADC12IFG & (1 << 6)));

            *val = ADC12MEM6;

            break;
        case ADC_PORT_7:
            while(!(ADC12IFG & (1 << 7)));

            *val = ADC12MEM7;

            break;
        case ADC_PORT_8:
            while(!(ADC12IFG & (1 << 8)));

            *val = ADC12MEM8;

            break;
        case ADC_PORT_9:
            while(!(ADC12IFG & (1 << 9)));

            *val = ADC12MEM9;

            break;
        case ADC_PORT_10:
            while(!(ADC12IFG & (1 << 10)));

            *val = ADC12MEM10;

            break;
        case ADC_PORT_11:
            while(!(ADC12IFG & (1 << 11)));

            *val = ADC12MEM11;

            break;
        case ADC_PORT_12:
            while(!(ADC12IFG & (1 << 12)));

            *val = ADC12MEM12;

            break;
        case ADC_PORT_13:
            while(!(ADC12IFG & (1 << 13)));

            *val = ADC12MEM13;

            break;
        case ADC_PORT_14:
            while(!(ADC12IFG & (1 << 14)));

            *val = ADC12MEM14;

            break;
        case ADC_PORT_15:
            while(!(ADC12IFG & (1 << 15)));

            *val = ADC12MEM15;

            break;
        default:
            *val = UINT16_MAX;

            return -1;
    }

    return 0;
}

/** \} End of adc group */
