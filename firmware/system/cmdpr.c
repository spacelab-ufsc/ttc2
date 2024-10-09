/*
 * cmdpr.c
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
 * \brief List of commands and parameters for external communication.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 *
 * \defgroup cmdpr CMDPR
 * \ingroup system
 * \{
 */

#include <stdint.h>

#include "cmdpr.h"

uint16_t cmdpr_param_size(uint8_t param)
{
    int err = 0;
    uint16_t param_size = 0;

    /*uint8_t param */
    if ((param == CMDPR_PARAM_HW_VER) || (param == CMDPR_PARAM_LAST_RST_CAUSE) || (param == CMDPR_PARAM_LAST_UP_COMMAND) ||
       (param == CMDPR_PARAM_ANT_DEP_STATUS) || (param == CMDPR_PARAM_ANT_DEP_HIB) || (param == CMDPR_PARAM_TX_ENABLE) ||
       (param == CMDPR_PARAM_PACKETS_AV_FIFO_RX) || (param == CMDPR_PARAM_PACKETS_AV_FIFO_TX) || (param == CMDPR_PARAM_RESET_DEVICE) ||
       (param == CMDPR_PARAM_CONSEQ_FAILED_PACKETS))
    {
        param_size = 1;

    }
    /*uint16_t param */
    else if ((param == CMDPR_PARAM_DEVICE_ID) || (param == CMDPR_PARAM_RST_COUNTER) || (param == CMDPR_PARAM_UC_VOLTAGE) ||
            (param == CMDPR_PARAM_UC_CURRENT) || (param == CMDPR_PARAM_UC_TEMP) || (param == CMDPR_PARAM_RADIO_VOLTAGE) ||
            (param == CMDPR_PARAM_RADIO_CURRENT) || (param == CMDPR_PARAM_RADIO_TEMP) || (param == CMDPR_PARAM_LAST_COMMAND_RSSI) ||
            (param == CMDPR_PARAM_ANT_TEMP) || (param == CMDPR_PARAM_ANT_MOD_STATUS_BITS) || (param == CMDPR_PARAM_N_BYTES_FIRST_AV_RX))
    {
        param_size = 2;
    }
    /*uint32_t param */
    else if ((param == CMDPR_PARAM_FW_VER) || (param == CMDPR_PARAM_TIMESTAMP) ||
            (param == CMDPR_PARAM_TX_PACKET_COUNTER) || (param == CMDPR_PARAM_RX_VAL_PACKET_COUNTER) || (param == CMDPR_PARAM_TIMESTAMP))
    {
        param_size = 4;
    }
    /* Unknown param */
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error writing OBDH parameter: unknown parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    if (err == -1)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_MODULE_ERROR, "Error writing OBDH parameter: unable to write parameter!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return param_size;
}

/** \} End of cmdpr group */
