/*
 * sx127x.h
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
 * \brief SX127x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 2021/12/14
 * 
 * \defgroup sx127x SX127x
 * \ingroup drivers
 * \{
 */

#ifndef SX127X_H_
#define SX127X_H_

#include <stdint.h>

#define SX127X_MODULE_NAME                  "SX127x"

/* Registers addresses (common) */
#define SX127X_REG_FIFO                     0x00U       /**< FIFO read/write access. */
#define SX127X_REG_OP_MODE                  0x01U       /**< Operating mode & LoRa/FSK selection. */
#define SX127X_REG_FRF_MSB                  0x06U       /**< RF Carrier Frequency, Most Significant Bits. */
#define SX127X_REG_FRF_MID                  0x07U       /**< RF Carrier Frequency, Intermediate Bits. */
#define SX127X_REG_FRF_LSB                  0x08U       /**< RF Carrier Frequency, Least Significant Bits. */
#define SX127X_REG_PA_CONFIG                0x09U       /**< PA selection and Output Power control. */
#define SX127X_REG_PA_RAMP                  0x0AU       /**< Control of PA ramp time, low phase noise PLL. */
#define SX127X_REG_OCP                      0x0BU       /**< Over Current Protection control. */
#define SX127X_REG_LNA                      0x0CU       /**< LNA settings. */
#define SX127X_REG_DIO_MAPPING_1            0x40U       /**< Mapping of pins DIO0 to DIO3. */
#define SX127X_REG_DIO_MAPPING_2            0x41U       /**< Mapping of pins DIO4 and DIO5, ClkOut frequency. */
#define SX127X_REG_VERSION                  0x42U       /**< Semtech ID relating the silicon revision. */
#define SX127X_REG_TCXO                     0x4BU       /**< TCXO or XTAL input setting. */
#define SX127X_REG_PA_DAC                   0x4DU       /**< Higher power settings of the PA. */
#define SX127X_REG_FORMER_TEMP              0x5BU       /**< Stored temperature during the former IQ Calibration. */
#define SX127X_REG_AGC_REF                  0x61U       /**< Adjustment of the AGC thresholds. */
#define SX127X_REG_AGC_THRESH_1             0x62U       /**< Adjustment of the AGC thresholds. */
#define SX127X_REG_AGC_THRESH_2             0x63U       /**< Adjustment of the AGC thresholds. */
#define SX127X_REG_AGC_THRESH_3             0x64U       /**< Adjustment of the AGC thresholds. */
#define SX127X_REG_PLL                      0x70U       /**< Control of the PLL bandwidth. */

/* Registers addresses (FSK/OOK mode) */
#define SX127X_REG_BITRATE_MSB              0x02U       /**< Bit Rate setting, Most Significant Bits. */
#define SX127X_REG_BITRATE_LSB              0x03U       /**< Bit Rate setting, Least Significant Bits. */
#define SX127X_REG_FDEV_MSB                 0x04U       /**< Frequency Deviation setting, Most Significant Bits. */
#define SX127X_REG_FDEV_LSB                 0x05U       /**< Frequency Deviation setting, Least Significant Bits. */
#define SX127X_REG_RX_CONFIG                0x0DU       /**< AFC, AGC, ctrl. */
#define SX127X_REG_RSSI_CONFIG              0x0EU       /**< RSSI. */
#define SX127X_REG_RSSI_COLLISION           0x0FU       /**< RSSI Collision detector. */
#define SX127X_REG_RSSI_THRESH              0x10U       /**< RSSI Threshold control. */
#define SX127X_REG_RSSI_VALUE               0x11U       /**< RSSI value in dBm. */
#define SX127X_REG_RX_BW                    0x12U       /**< Channel Filter BW Control. */
#define SX127X_REG_AFC_BW                   0x13U       /**< AFC Channel Filter BW. */
#define SX127X_REG_OOK_PEAK                 0x14U       /**< OOK demodulator. */
#define SX127X_REG_OOK_FIX                  0x15U       /**< Threshold of the OOK demod. */
#define SX127X_REG_OOK_AVG                  0x16U       /**< Average of the OOK demod. */
#define SX127X_REG_AFC_FEI                  0x1AU       /**< AFC and FEI control. */
#define SX127X_REG_AFC_MSB                  0x1BU       /**< Frequency correction value of the AFC. */
#define SX127X_REG_AFC_LSB                  0x1CU       /**< Frequency correction value of the AFC. */
#define SX127X_REG_FEI_MSB                  0x1DU       /**< Value of the calculated frequency error. */
#define SX127X_REG_FEI_LSB                  0x1EU       /**< Value of the calculated frequency error. */
#define SX127X_REG_PREAMBLE_DETECT          0x1FU       /**< Settings of the Preamble Detector. */
#define SX127X_REG_RX_TIMEOUT_1             0x20U       /**< Timeout Rx request and RSSI. */
#define SX127X_REG_RX_TIMEOUT_2             0x21U       /**< imeout RSSI and Payload ready. */
#define SX127X_REG_RX_TIMEOUT_3             0x22U       /**< Timeout RSSI and SyncAddress. */
#define SX127X_REG_RX_DELAY                 0x23U       /**< Delay between Rx cycles. */
#define SX127X_REG_OSC                      0x24U       /**< RC Oscillators Settings, CLKOUT frequency. */
#define SX127X_REG_PREAMBLE_MSB             0x25U       /**< Preamble length, MSB. */
#define SX127X_REG_PREAMBLE_LSB             0x26U       /**< Preamble length, LSB. */
#define SX127X_REG_SYNC_CONFIG              0x27U       /**< Sync Word Recognition control. */
#define SX127X_REG_SYNC_VALUE_1             0x28U       /**< Sync Word bytes 1. */
#define SX127X_REG_SYNC_VALUE_2             0x29U       /**< Sync Word bytes 2. */
#define SX127X_REG_SYNC_VALUE_3             0x2AU       /**< Sync Word bytes 3. */
#define SX127X_REG_SYNC_VALUE_4             0x2BU       /**< Sync Word bytes 4. */
#define SX127X_REG_SYNC_VALUE_5             0x2CU       /**< Sync Word bytes 5. */
#define SX127X_REG_SYNC_VALUE_6             0x2DU       /**< Sync Word bytes 6. */
#define SX127X_REG_SYNC_VALUE_7             0x2EU       /**< Sync Word bytes 7. */
#define SX127X_REG_SYNC_VALUE_8             0x2FU       /**< Sync Word bytes 8. */
#define SX127X_REG_PACKET_CONFIG_1          0x30U       /**< Packet mode settings. */
#define SX127X_REG_PACKET_CONFIG_2          0x31U       /**< Packet mode settings. */
#define SX127X_REG_PAYLOAD_LENGTH           0x32U       /**< Payload length setting. */
#define SX127X_REG_NODE_ADRS                0x33U       /**< Node address. */
#define SX127X_REG_BROADCAST_ADRS           0x34U       /**< Broadcast address. */
#define SX127X_REG_FIFO_THRESH              0x35U       /**< Fifo threshold, Tx start condition. */
#define SX127X_REG_SEQ_CONFIG_1             0x36U       /**< Top level Sequencer settings. */
#define SX127X_REG_SEQ_CONFIG_2             0x37U       /**< Top level Sequencer settings. */
#define SX127X_REG_TIMER_RESOL              0x38U       /**< Timer 1 and 2 resolution control. */
#define SX127X_REG_TIMER_1_COEF             0x39U       /**< Timer 1 setting. */
#define SX127X_REG_TIMER_2_COEF             0x3AU       /**< Timer 2 setting. */
#define SX127X_REG_IMAGE_CAL                0x3BU       /**< Image calibration engine control. */
#define SX127X_REG_TEMP                     0x3CU       /**< Temperature Sensor value. */
#define SX127X_REG_LOW_BAT                  0x3DU       /**< Low Battery Indicator Settings. */
#define SX127X_REG_IRQ_FLAGS_1              0x3EU       /**< Status register: PLL Lock state, Timeout, RSSI. */
#define SX127X_REG_IRQ_FLAGS_2              0x3FU       /**< Status register: FIFO handling flags, Low Battery. */
#define SX127X_REG_PLL_HOP                  0x44U       /**< Control the fast frequency hopping mode. */
#define SX127X_REG_BIT_RATE_FRAC            0x5DU       /**< Fractional part in the Bit Rate division ratio. */

/* Registers addresses (LoRa mode) */
#define SX127X_REG_FIFO_ADDR_PTR            0x0DU       /**< FIFO SPI pointer. */
#define SX127X_REG_FIFO_TX_BASE_ADDR        0x0EU       /**< Start Tx data. */
#define SX127X_REG_FIFO_RX_BASE_ADDR        0x0FU       /**< Start Rx data. */
#define SX127X_REG_FIFO_RX_CURRENT_ADDR     0x10U       /**< Start address of last packet received. */
#define SX127X_REG_IRQ_FLAGS_MASK           0x11U       /**< Optional IRQ flag mask. */
#define SX127X_REG_IRQ_FLAGS                0x12U       /**< IRQ flags. */
#define SX127X_REG_RX_NB_BYTES              0x13U       /**< Number of received bytes. */
#define SX127X_REG_RX_HEADER_CNT_VALUE_MSB  0x14U       /**< Number of valid headers received. */
#define SX127X_REG_RX_HEADER_CNT_VALUE_LSB  0x15U       /**< Number of valid headers received. */
#define SX127X_REG_RX_PACKET_CNT_VALUE_MSB  0x16U       /**< Number of valid packets received. */
#define SX127X_REG_RX_PACKET_CNT_VALUE_LSB  0x17U       /**< Number of valid packets received. */
#define SX127X_REG_MODEM_STAT               0x18U       /**< Live LoRa modem status. */
#define SX127X_REG_PKT_SNR_VALUE            0x19U       /**< Espimation of last packet SNR. */
#define SX127X_REG_PKT_RSSI_VALUE           0x1AU       /**< RSSI of last packet. */
#define SX127X_REG_RSSI_VALUE               0x1BU       /**< Current RSSI. */
#define SX127X_REG_HOP_CHANNEL              0x1CU       /**< FHSS start channel. */
#define SX127X_REG_MODEM_CONFIG_1           0x1DU       /**< Modem PHY config 1. */
#define SX127X_REG_MODEM_CONFIG_2           0x1EU       /**< Modem PHY config 2. */
#define SX127X_REG_SYMB_TIMEOUT_LSB         0x1FU       /**< Receiver timeout value. */
#define SX127X_REG_PREAMBLE_MSB             0x20U       /**< Size of preamble. */
#define SX127X_REG_PREAMBLE_LSB             0x21U       /**< Size of preamble. */
#define SX127X_REG_PAYLOAD_LENGTH           0x22U       /**< LoRa payload length. */
#define SX127X_REG_MAX_PAYLOAD_LENGTH       0x23U       /**< LoRa maximum payload length. */
#define SX127X_REG_HOP_PERIOD               0x24U       /**< FHSS Hop period. */
#define SX127X_REG_FIFO_RX_BYTE_ADDR        0x25U       /**< Address of last byte written in FIFO. */
#define SX127X_REG_MODEM_CONFIG_3           0x26U       /**< Modem PHY config 3. */
#define SX127X_REG_FEI_MSB                  0x28U       /**< Estimated frequency error. */
#define SX127X_REG_FEI_MID                  0x29U       /**< Estimated frequency error. */
#define SX127X_REG_FEI_LSB                  0x2AU       /**< Estimated frequency error. */
#define SX127X_REG_RSSI_WIDEBAND            0x2CU       /**< Wideband RSSI measurement. */
#define SX127X_REG_IF_FREQ_1                0x2FU       /**< Optimize receiver. */
#define SX127X_REG_IF_FREQ_2                0x30U       /**< Optimize receiver. */
#define SX127X_REG_DETECT_OPTIMIZE          0x31U       /**< LoRa detection Optimize for SF6. */
#define SX127X_REG_INVERT_IQ                0x33U       /**< Invert LoRa I and Q signals. */
#define SX127X_REG_HIGH_BW_OPTIMIZE_1       0x36U       /**< Sensitivity optimisation for 500 kHz bandwidth. */
#define SX127X_REG_DETECTION_THRESHOLD      0x37U       /**< LoRa detection threshold for SF6. */
#define SX127X_REG_SYNC_WORD                0x39U       /**< LoRa Sync Word. */
#define SX127X_REG_HIGH_BW_OPTIMIZE_2       0x3AU       /**< Sensitivity optimisation for 500 kHz bandwidth. */
#define SX127X_REG_INVERT_IQ_2              0x3BU       /**< Optimize for inverted IQ. */

/**
 * \brief Module initialization.
 *
 * \return The status/error code.
 */
int sx127x_init(void);

/**
 * \brief RX mode initialization.
 *
 * \return The status/error code.
 */
int sx127x_rx_init(void);

/**
 * \brief Transmits a packet.
 *
 * \param[in] data is an array with the bytes to transmit.
 *
 * \param[in] len is the number of bytes to transmit.
 *
 * \return The status/error code.
 */
int sx127x_tx_packet(uint8_t *data, uint16_t len);

/**
 * \brief Receives a packet.
 *
 * \param[in,out] data is a pointer to store the received data.
 *
 * \param[in,out] len is a pointer to store the number of received bytes.
 *
 * \return The status/error code.
 */
int sx127x_rx_packet(uint8_t *data, uint16_t *len);

/**
 * \brief Inquire interrupt.
 *
 * \param[in] irq_mask is the interrupt flag.
 *
 * \return The status/error code.
 */
int sx127x_wait_irq(uint8_t irq_mask);

/**
 * \brief Enters in standby mode.
 *
 * \return The status/error code.
 */
int sx127x_enter_standby_mode(void);

/**
 * \brief Enters in sleep mode.
 *
 * \return The status/error code.
 */
int sx127x_enter_sleep_mode(void);

/**
 * \brief Clears the interrupt flags.
 *
 * \return The status/error code.
 */
int sx127x_clear_interrupt(void);

/**
 * \brief Sets the RF frequency.
 *
 * \param[in] freq is the new value of the frequency in Hertz.
 *
 * \note The range is 137-1020 MHz for SX1276, and 137-525 MHz for SX1278.
 *
 * \return The status/error code.
 */
int sx127x_set_frequency(uint32_t freq);

/**
 * \brief Sets the RF parameters.
 *
 * \param[in] bw is the bandwidth (from 7800 to 500000).
 *
 * \param[in] cr is the coding rate (from 4/5 to 4/8).
 *
 * \param[in] sf is the spread factor (from 6 to 12).
 *
 * \param[in] crc is the CRC flag. It can be:
 * \parblock
 *      -\b 
 *      -\b 
 *      .
 * \endparblock
 *
 * \note The RF data rate depends on bandwidth and spreading factor
 *
 * \note Coding rate affects time on air.
 *
 * \note If SF=6, it will turn to implict mode in this function.
 *
 * \return The status/error code.
 */
int sx127x_set_rf_param(uint8_t bw, uint8_t cr, uint8_t sf, uint8_t crc);

/**
 * \brief Sets the preamble length.
 *
 * \param[in] len is the new preamble lenght.
 *
 * \return The status/error code.
 */
int sx127x_set_preamble_len(uint16_t len);

/**
 * \brief Sets the header mode.
 *
 * \param[in] mode is the new header mode. It can be:
 * \parblock
 *      -\b 
 *      -\b 
 *      .
 * \endparblock
 *
 * \note If SF=6, it must be implicit header mode.
 *
 * \return The status/error code.
 */
int sx127x_set_header_mode(uint8_t mode);

/**
 * \brief Sets the payload length.
 *
 * \param[in] len is the new payload length in bytes.
 *
 * \note In implicit header mode, payload length must be set first.
 *
 * \note Length is fixed in implicit header mode.
 *
 * \return The status/error code.
 */
int sx127x_set_payload_len(uint8_t len);

/**
 * \brief Sets the TX power.
 *
 * \param[in] pwr is the new power level, from 0 to 15.
 *
 * \return The status/error code.
 */
int sx127x_tx_power(uint8_t pwr);

/**
 * \brief Sets RX timeout.
 *
 * \param[in] symb_timeout actual timeout = symb_timeout * (2**SF/BW).
 *
 * \return The status/error code.
 */
int sx127x_set_rx_timeout(uint16_t symb_timeout);

/**
 * \brief Reads the RSSI value.
 *
 * \param[in] mode it can be:
 * \parblock
 *      -\b 0 reads the current RSSI.
 *      -\b 1 reads the RSSI of the last received packet.
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int sx127x_read_rssi(uint8_t mode);

/**
 * \brief Power on the module.
 *
 * \return The status/error code.
 */
static int sx127x_power_on_reset(void);

/**
 * \brief Sets the initial parameters.
 *
 * \return The status/error code.
 */
static int sx127x_config(void);

/**
 * \brief Sets the antenna switch.
 *
 * \param[in] mode .
 *
 * \return The status/error code.
 */
static int sx127x_set_ant_switch(uint8_t mode);

/**
 * \brief .
 *
 * \param[in] adr .
 *
 * \return The status/error code.
 */
static int sx127x_set_fifo_addr_ptr(uint8_t adr);

/**
 * \brief Enters in RX mode.
 *
 * \return The status/error code.
 */
static int sx127x_enter_rx_mode(void);

/**
 * \brief Enters in TX mode.
 *
 * \return The status/error code.
 */
static int sx127x_enter_tx_mode(void);

/**
 * \brief Writes data to the FIFO.
 *
 * \param[in] data is an array with the bytes to be written in the FIFO.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return The status/error code.
 */
static int sx127x_write_fifo(uint8_t *data, uint8_t len);

/**
 * \brief Reads data from the FIFO.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in,out] len is the number of read bytes.
 *
 * \return The status/error code.
 */
static int sx127x_read_fifo(uint8_t *data, uint8_t *len);

/**
 * \brief Enables TX done interrupt.
 *
 * \return The status/error code.
 */
static int sx127x_set_tx_interrupt(void);

/**
 * \brief Enables RX done interrupt.
 *
 * \return The status/error code.
 */
static int sx127x_set_rx_interrupt(void);

/**
 * \brief Reads the value of a given register.
 *
 * \param[in] adr is the address of the register.
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
static int sx127x_read_reg(uint8_t adr, uint8_t *val);

/**
 * \brief Writes a value to a register.
 *
 * \param[in] adr is the address of the register to write.
 *
 * \param[in] val is the value to write.
 *
 * \return The status/error code.
 */
static int sx127x_write_reg(uint8_t adr, uint8_t val);

/**
 * \brief Reads the buffer trough SPI.
 *
 * \param[in] adr is the address to start reading.
 *
 * \param[in] ptr is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
static int sx127x_burst_read(uint8_t adr, uint8_t *ptr, uint8_t len);

/**
 * \brief Writes buffer trough SPI.
 *
 * \param[in] adr is the address to start writing.
 *
 * \param[in] ptr is a pointer with the data to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
static int sx127x_burst_write(uint8_t adr, uint8_t *ptr, uint8_t len);

/**
 * \brief SPI interface initialization.
 *
 * \return The status/error code.
 */
static int sx127x_spi_init(void);

/**
 * \brief SPI transfer routine (write and read at the same time).
 *
 * \param[in] wd is an array ot bytes to write during the transfer.
 *
 * \param[in,out] rd is an array to store the read bytes during the transfer.
 *
 * \param[in] len is the number of bytes to transfer.
 *
 * \return The status/error code.
 */
static int sx127x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len);

/**
 * \brief Writes a byte over the SPI interface.
 *
 * \param[in] byte is the byte to be written to the SPI interface.
 *
 * \return The status/error code.
 */
static int sx127x_spi_write_byte(uint8_t byte);

/**
 * \brief Write an array of bytes over the SPI interface.
 *
 * \param[in] data is the array of bytes to write to the SPI interface.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return The status/error code.
 */
static int sx127x_spi_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads N bytes from the SPI interface.
 *
 * \param[in] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
static int sx127x_spi_read(uint8_t *data, uint16_t len);

/**
 * \brief GPIO pins initialization.
 *
 * \return The status/error code.
 */
static int sx127x_gpio_init(void);

/**
 * \brief Write the state of the RESET pin.
 *
 * \param[in] state is new state of the RESET pin.
 *
 * \return The status/error code.
 */
static int sx127X_gpio_write_reset(bool state);

/**
 * \brief Reads the state of the nIRQ pin.
 *
 * \return The state of the nIRQ pin. It can be:
 * \parblock
 *      -\b GPIO_STATE_HIGH
 *      -\b GPIO_STATE_LOW
 *      .
 * \endparblock
 */
static int sx127x_gpio_read_nirq(void);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the time to delay in milliseconds.
 *
 * \return None.
 */
void sx127x_delay_ms(uint32_t ms);

#endif /* SX127X_H_ */

/** \} End of sx127x group */
