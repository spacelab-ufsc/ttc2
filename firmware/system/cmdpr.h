/*
 * cmds_parameters.h
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
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief List of commands and parameters for external communication.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.1
 *
 * \date 2023/02/15
 *
 * \defgroup cmdpr CMDPR
 * \ingroup system
 * \{
 */

#ifndef SYSTEM_CMDPR_H_
#define SYSTEM_CMDPR_H_

/* CMDPR Commands */
#define CMDPR_CMD_READ_PARAM                 0x01U       /**< Read parameter */
#define CMDPR_CMD_WRITE_PARAM                0x02U       /**< Write parameter */
#define CMDPR_CMD_TRANSMIT_PACKET            0x03U       /**< Transmit a packet */
#define CMDPR_CMD_READ_FIRST_PACKET          0x04U       /**< Read first available packet */

/* CMDPR Parameters */
#define CMDPR_PARAM_HW_VER                   0x00U       /**< Device ID (0xCC2A or 0xCC2B) */
#define CMDPR_PARAM_FW_VER                   0x01U       /**< Hardware version */
#define CMDPR_PARAM_COUNTER                  0x02U       /**< Firmware version */
#define CMDPR_PARAM_RST_COUNTER              0x03U       /**< Time counter in millseconds */
#define CMDPR_PARAM_DEVICE_ID                0x04U       /**< Reset counter */
#define CMDPR_PARAM_LAST_RST_CAUSE           0x05U       /**< Last reset cause */
#define CMDPR_PARAM_UC_VOLTAGE               0x06U       /**< Input voltage of the uC in mV */
#define CMDPR_PARAM_UC_CURRENT               0x07U       /**< Input current of the uC in mA */
#define CMDPR_PARAM_UC_TEMP                  0x08U       /**< Temperature of the uC in K */
#define CMDPR_PARAM_RADIO_VOLTAGE            0x09U       /**< Input voltage of the radio in mV  */
#define CMDPR_PARAM_RADIO_CURRENT            0x0AU       /**< Input current of the radio in mA */
#define CMDPR_PARAM_RADIO_TEMP               0x0BU       /**< Temperature of the radio in K */
#define CMDPR_PARAM_LAST_UP_COMMAND          0x0CU       /**< Last valid command (uplink packet ID) */
#define CMDPR_PARAM_LAST_COMMAND_RSSI        0x0DU       /**< RSSI of the last valid telecommand */
#define CMDPR_PARAM_ANT_TEMP                 0x0EU       /**< Temperature of the antenna module in K */
#define CMDPR_PARAM_ANT_MOD_STATUS_BITS      0x0FU       /**< Antenna module status bits */
#define CMDPR_PARAM_ANT_DEP_STATUS           0x10U       /**< Antenna deployment status */
#define CMDPR_PARAM_ANT_DEP_HIB              0x11U       /**< Antenna deployment hibernation */
#define CMDPR_PARAM_TX_ENABLE                0x12U       /**< TX enable */
#define CMDPR_PARAM_TX_PACKET_COUNTER        0x13U       /**< TX packet counter */
#define CMDPR_PARAM_RX_VAL_PACKET_COUNTER    0x14U       /**< RX packet counter (valid packets) */
#define CMDPR_PARAM_PACKETS_AV_FIFO_TX       0x15U       /**< TX packets available in the FIFO buffer */
#define CMDPR_PARAM_PACKETS_AV_FIFO_RX       0x16U       /**< RX packets available in the FIFO buffer */
#define CMDPR_PARAM_N_BYTES_FIRST_AV_RX      0x17U       /**< Number of bytes of the first available packet in the RX buffer */

#endif /* SYSTEM_CMDPR_H_ */