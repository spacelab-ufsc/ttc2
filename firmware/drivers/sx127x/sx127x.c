/*
 * sx127x.c
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
 * \brief SX127x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.21
 * 
 * \date 2021/12/14
 * 
 * \addtogroup sx127x
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sx127x.h"

static uint8_t header_mode;
static uint8_t payload_length = 0;

int sx127x_init(void)
{
    int err = 0;

    if (sx127x_gpio_init() != 0)
    {
        err = -1;
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error initializing the GPIO pins!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    if (sx127x_spi_init() != 0)
    {
        err = -1;
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error initializing the SPI port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    if (sx127x_power_on_reset() != 0)
    {
        err = -1;
    }

    if (sx127x_config() != 0)
    {
        err = -1;
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error loading the configuration parameters!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sx127x_rx_init(void)
{
    return -1;
}

int sx127x_tx_packet(uint8_t *data, uint16_t len)
{
    return -1;
}

int sx127x_rx_packet(uint8_t *data, uint16_t *len)
{
    return -1;
}

int sx127x_wait_irq(uint8_t irq_mask)
{
    int err = -1;

    uint8_t flag = UINT8_MAX;

    if (sx127x_read_reg(SX127X_REG_IRQ_FLAGS, &flag) == 0)
    {
        /* if irq flag was set */
        if (flag & irq_mask)
        {
            err = 0;
        }
    }

    return err;
}

int sx127x_enter_standby_mode(void)
{
    int err = 0;

    if ((sx127x_set_ant_switch(SX127X_MODE_STANDBY) != 0) ||
        (sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_OP_MODE_STBY | SX127X_FREQUENCY_BAND) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error entering in standby mode!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int sx127x_enter_sleep_mode(void)
{
    int err = 0;

    if ((sx127x_set_ant_switch(SX127X_MODE_STANDBY) != 0) ||
        (sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_OP_MODE_SLEEP | SX127X_FREQUENCY_BAND) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error entering in sleep mode!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int sx127x_clear_interrupt(void)
{
    return sx127x_write_reg(SX127X_REG_IRQ_FLAGS, 0xFFU);
}

int sx127x_set_frequency(uint32_t freq)
{
    int err  = 0;

    uint32_t frf = UINT32_MAX;
    uint32_t temp1 = UINT32_MAX;
    uint32_t temp2 = UINT32_MAX;
    uint8_t reg[3] = {0};

    temp1 = freq / 1000000UL;
    temp2 = SX127X_XOSC_HZ / 1000000UL;
    frf = temp1 * 524288UL / temp2;

    temp1 = freq % 1000000UL/1000UL;
    temp2 = SX127X_XOSC_HZ / 1000UL;
    frf = frf + temp1 * 524288UL / temp2;

    temp1 = freq % 1000UL;
    temp2 = SX127X_XOSC_HZ;
    frf = frf + temp1 * 524288UL / temp2;

    reg[0] = frf >> 16U & 0xFFU;
    reg[1] = frf >> 8U & 0xFFU;
    reg[2] = frf & 0xFFU;

    /* Writing the value to FRF register */
    if ((sx127x_write_reg(SX127X_REG_FRF_MSB, reg[0]) != 0) ||
        (sx127x_write_reg(SX127X_REG_FRF_MID, reg[1]) != 0) ||
        (sx127x_write_reg(SX127X_REG_FRF_LSB, reg[2]) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to register FRF!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    /* Checking the register's value */
    uint8_t reg_buf[3] = {0};

    if ((sx127x_read_reg(SX127X_REG_FRF_MSB, &reg_buf[0]) != 0) ||
        (sx127x_read_reg(SX127X_REG_FRF_MID, &reg_buf[1]) != 0) ||
        (sx127x_read_reg(SX127X_REG_FRF_LSB, &reg_buf[2]) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register FRF!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    if ((reg[0] != reg_buf[0]) || (reg[1] != reg_buf[1]) || (reg[2] != reg_buf[2]))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to FRF registers!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int sx127x_set_rf_param(uint8_t bw, uint8_t cr, uint8_t sf, uint8_t crc)
{
    int err = -1;

    /* Check if the data is correct */
    if (((bw & 0x0FU) == 0) && ((bw >> 8) <= 0x09U))
    {
        if (((cr & 0xF1U) == 0) && ((cr >> 1) <= 0x04U) && ((cr >> 1) >= 0x00U))
        {
            if (((sf & 0x0FU) == 0) && ((sf >> 4) <= 12U) && ((sf >> 4) >= 6U))
            {
                if ((crc & 0xFBU) == 0)
                {
                    uint8_t temp = UINT8_MAX;

                    /* SF=6 must be use in implicit header mode, and have some special setting */
                    if (sf == SX127X_SPREADING_FACTOR_6)
                    {
                        header_mode = SX127X_IMPLICIT_HEADER_MODE;

                        if (sx127x_write_reg(SX127X_REG_MODEM_CONFIG_1, bw | cr | SX127X_IMPLICIT_HEADER_MODE) != 0)
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to register MODEM_CONFIG_1!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }

                        if (sx127x_read_reg(SX127X_REG_MODEM_CONFIG_2, &temp) != 0)
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register MODEM_CONFIG_2!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }

                        temp = temp & 0x03U;

                        if (sx127x_write_reg(SX127X_REG_MODEM_CONFIG_2, sf | crc | temp) != 0)
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register MODEM_CONFIG_2!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }

                        /* According to datasheet */
                        if (sx127x_read_reg(0x31U, &temp) != 0)
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register 0x31!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }

                        temp &= 0xF8U;
                        temp |= 0x05U;

                        if ((sx127x_write_reg(0x31U, temp) == 0) && (sx127x_write_reg(0x37U, 0x0CU) == 0))
                        {
                            err = 0;
                        }
                        else
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to registers 0x31 and 0x37!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }
                    }
                    else
                    {
                        if (sx127x_read_reg(SX127X_REG_MODEM_CONFIG_2, &temp) != 0)
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the MODEM_CONFIG_2 register!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }

                        temp = temp & 0x03U;

                        if ((sx127x_write_reg(SX127X_REG_MODEM_CONFIG_1, bw | cr | header_mode) == 0) &&
                            (sx127x_write_reg(SX127X_REG_MODEM_CONFIG_2, sf | crc | temp) == 0))
                        {
                            err = 0;
                        }
                        else
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to MODEM_CONFIG registers!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        }
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Invalid CRC option!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Invalid SF value!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Invalid CR value!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Invalid BW value!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sx127x_set_preamble_len(uint16_t len)
{
    int err = 0;

    /* Preamble length is 6 to 65535 */
    if (len < 6)
    {
        err = -1;
    }
    else
    {
        if ((sx127x_write_reg(SX127X_REG_PREAMBLE_MSB, len >> 8) != 0) ||
            (sx127x_write_reg(SX127X_REG_PREAMBLE_LSB, len & 0xFFU) != 0))
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to Preamble register!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int sx127x_set_header_mode(uint8_t mode)
{
    int err = -1;

    if(header_mode <= 0x01U)
    {
        header_mode = mode;

        uint8_t temp = UINT8_MAX;

        /* Avoid overload the other setting */
        if (sx127x_read_reg(SX127X_REG_MODEM_CONFIG_1, &temp) == 0)
        {
            temp = temp & 0xFEU;

            if (sx127x_write_reg(SX127X_REG_MODEM_CONFIG_1, temp | mode) == 0)
            {
                err = 0;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to MODEM_CONFIG_1 register!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the MODEM_CONFIG_1 register!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

int sx127x_set_payload_len(uint8_t len)
{
    payload_length = len;

    return sx127x_write_reg(SX127X_REG_PAYLOAD_LENGTH, len);
}

int sx127x_set_tx_power(uint8_t pwr)
{
    int err = -1;

    if (pwr <= 0x0FU)
    {
        if (sx127x_write_reg(SX127X_REG_PA_CONFIG, SX127X_PA_SELECT_PA_BOOST | 0x70U | pwr) == 0)
        {
            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to register PA_CONFIG!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Invalid power value!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sx127x_set_rx_timeout(uint16_t symb_timeout)
{
    int err = -1;

    /* rxtimeout = symb_timeout*(2^SF*BW) */
	if ((symb_timeout != 0) && (symb_timeout <= 0x3FFU))
    {
        uint8_t temp = UINT8_MAX;

        if (sx127x_read_reg(SX127X_REG_MODEM_CONFIG_2, &temp) != 0)
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register MODEM_CONFIG_2!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }

        temp = temp & 0xFCU;

        if ((sx127x_write_reg(SX127X_REG_MODEM_CONFIG_2, temp | (symb_timeout >> 8 & 0x03U)) != 0) ||
            (sx127x_write_reg(SX127X_REG_SYMB_TIMEOUT_LSB, symb_timeout & 0xFFU) != 0))
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error writing to registers MODEM_CONFIG_2 and SYMB_TIMEOUT_LSB!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }

        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Invalid symb_timeout value!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sx127x_read_rssi(uint8_t mode, uint8_t rssi_val)
{
    int err = -1;

    if (!mode)
    {
        if (sx127x_read_reg(SX127X_REG_RSSI_VALUE, &rssi_val) == 0)
        {
            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register RSSI_VALUE!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
        if (sx127x_read_reg(SX127X_REG_PKT_RSSI_VALUE, &rssi_val) == 0)
        {
            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error reading the register PKT_RSSI_VALUE!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

int sx127x_power_on_reset(void)
{
    int err = sx127X_gpio_write_reset(false);

    sx127x_delay_ms(10);

    err = sx127X_gpio_write_reset(true);

    sx127x_delay_ms(20);

    return err;
}

int sx127x_config(void)
{
    /* Sleep mode */
    sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_OP_MODE_SLEEP | SX127X_FREQUENCY_BAND);
    sx127x_delay_ms(5);

    /* External Crystal */
    sx127x_write_reg(SX127X_REG_TCXO, SX127X_TCXO_EXT_CRYSTAL | SX127X_TCXO_REGTCXO_RESERVED);

    /* LoRa Mode */
    sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_LONG_RANGE_MODE_LORA | SX127X_FREQUENCY_BAND);

    /* Setting Frequency */
    sx127x_set_frequency(433500000);

    /* Maximum Power, 20 dB */
    sx127x_set_tx_power(0x0f);

    /* Close OCP */
    sx127x_write_reg(SX127X_REG_OCP, SX127X_OCPON_OFF| 0x0B);
    //sx127x_write_reg(adr, val)

    /* Enable LNA */
    sx127x_write_reg(SX127X_REG_LNA, SX127X_LNA_GAIN_G1 | SX127X_LNA_BOOSTHF_1);

    /* RF Params */
    /*BW = 62.5 Hz, spreading factor = 9, coding rate = 4/5, explicit header mode */
    header_mode = SX127X_EXPLICIT_HEADER_MODE;
    sx127x_set_header_mode(header_mode);
    sx127x_set_rf_param(SX127X_BW_62P5K, SX127X_CODING_RATE_1P25, SX127X_SPREADING_FACTOR_9, SX127X_PAYLOAD_CRC_ON);

    /* LNA */
    sx127x_write_reg(SX127X_REG_MODEM_CONFIG_3, SX127X_LOWDATARATEOPTIMIZE_DSBLD);

    /* Maximum RX time out */
    sx127x_set_rx_timeout(0x3ff);

    // Preamble 12+ 4.25 bytes
    sx127x_set_preamble_len(12);

    /* 20 dBm on PA_BOOST pin */
    sx127x_write_reg(SX127X_REG_PA_DAC, SX127X_REGPADAC_RESERVED | SX127X_20DB_OUTPUT_ON);

    /* No hopping */
    sx127x_write_reg(SX127X_REG_HOP_PERIOD, 0x00);

    /*DIO5 = ModeReady, DIO4=CadDetected */
    sx127x_write_reg(SX127X_REG_DIO_MAPPING_2, SX127X_DIO4_CADDETECTED | SX127X_DIO5_MODEREADY);

    /* Standby mode */
    sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_OP_MODE_STBY | SX127X_FREQUENCY_BAND);

    /* Set Payload Length 10 bytes in explicit mode */
    return sx127x_set_payload_len(10);
}

int sx127x_set_ant_switch(sx127x_mode_t mode)
{
    return -1;
}

int sx127x_set_fifo_addr_ptr(uint8_t adr)
{
    int err = 0;

    uint8_t adr_val = UINT8_MAX;

    if ((sx127x_read_reg(adr, &adr_val) != 0) ||
        (sx127x_write_reg(SX127X_REG_FIFO_ADDR_PTR, adr_val) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error configuring the FIFO address pointer!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int sx127x_enter_rx_mode(void)
{
    int err = 0;

    if ((sx127x_set_ant_switch(SX127X_MODE_RX) != 0) ||
        (sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_OP_MODE_RX_CONTINUOUS | SX127X_FREQUENCY_BAND) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error entering in RX mode!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int sx127x_enter_tx_mode(void)
{
    int err = 0;

    if ((sx127x_set_ant_switch(SX127X_MODE_TX) != 0) ||
        (sx127x_write_reg(SX127X_REG_OP_MODE, SX127X_OP_MODE_TX | SX127X_FREQUENCY_BAND) != 0))
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SX127X_MODULE_NAME, "Error entering in TX mode!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int sx127x_write_fifo(uint8_t *data, uint8_t len)
{
    return -1;
}

int sx127x_read_fifo(uint8_t *data, uint8_t *len)
{
    return -1;
}

int sx127x_set_tx_interrupt(void)
{
    return -1;
}

int sx127x_set_rx_interrupt(void)
{
    return -1;
}

int sx127x_read_reg(uint8_t adr, uint8_t *val)
{
    int err = -1;

    uint8_t wbuf[2] = {0};
    uint8_t rbuf[2] = {0};

    if (sx127x_spi_transfer(wbuf, rbuf, 2) == 0)
    {
        *val = rbuf[1];

        err = 0;
    }

    return err;
}

int sx127x_write_reg(uint8_t adr, uint8_t val)
{
    uint8_t wbuf[2] = {0};

    wbuf[0] = adr | SX127X_SPI_WNR;
    wbuf[1] = val;

    return sx127x_spi_write(wbuf, 2);
}

int sx127x_burst_read(uint8_t adr, uint8_t *ptr, uint8_t len)
{
    int err = -1;

    if (len <= 1)
    {
        err = sx127x_read_reg(adr, ptr);
    }
    else
    {
        uint8_t wbuf[32] = {0};

        err = sx127x_spi_write(adr, 1);

        uint8_t i = 0;
        for(i = 0; i < len; i++)
        {
            err = sx127x_spi_transfer(wbuf, ptr, len);
        }
    }

    return err;
}

int sx127x_burst_write(uint8_t adr, uint8_t *ptr, uint8_t len)
{
    int err = -1;

    if (len <= 1)
    {
        err = sx127x_write_reg(adr, ptr[0]);
    }
    else
    {
        uint8_t wbuf[32] = {0};

        wbuf[0] = adr | SX127X_SPI_WNR;

        memcpy(&wbuf[1], ptr, len);

        err = sx127x_spi_write(wbuf, 1U + len);
    }

    return err;
}

/** \} End of sx127x group */
