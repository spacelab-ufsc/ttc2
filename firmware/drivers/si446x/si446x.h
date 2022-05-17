/*
 * si446x.h
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
 * \brief Si446x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.23
 * 
 * \date 2017/06/01
 * 
 * \defgroup si446x Si446x
 * \{
 */

#ifndef SI446X_H_
#define SI446X_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * \brief Si446x modes.
 */
typedef enum
{
    SI446X_MODE_STANDBY=0,      /**< Standby mode. */
    SI446X_MODE_TX,             /**< TX mode. */
    SI446X_MODE_RX              /**< RX mode. */
} si446x_modes_t;

/**
 * \brief GPIO pins.
 */
typedef enum
{
    SI446X_GPIO_SDN=0,          /**< SDN pin. */
    SI446X_GPIO_NIRQ,           /**< nIRQ pin. */
    SI446X_GPIO_GPIO_0,         /**< GPIO 0 pin. */
    SI446X_GPIO_GPIO_1,         /**< GPIO 1 pin. */
    SI446X_GPIO_GPIO_2,         /**< GPIO 2 pin. */
    SI446X_GPIO_GPIO_3          /**< GPIO 3 pin. */
} si446x_gpio_pin_t;

/**
 * \brief SI446X initialization.
 * 
 * Initializes the SI446X module with the configuration parameters from "si446x_reg_config.h".
 * 
 * \return The status/error code.
 */
int si446x_init(void);

/**
 * \brief Configures the registers of the SI446X device.
 * 
 * All the configuration parameters are defined in the "si446x_reg_config.h" file.
 * 
 * \return None.
 */
void si446x_reg_config(void);

/**
 * \brief Power on reset procedure of the SI446X module.
 * 
 * \return None.
 */
void si446x_power_on_reset(void);

/**
 * \brief Transmit a packet through RF.
 * 
 * \param[in] data is the data with the packet to send.
 *
 * \param[in] len is the length of the data to send.
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_tx_packet(uint8_t *data, uint8_t len);

/**
 * \brief Transmit a long packet (> FIFO buffer, or 64 bytes) through RF.
 * 
 * \note AN633, Figure 62.
 * 
 * \param[in] packet is the packet to send.
 *
 * \param[in] len is the length of the packet.
 * 
 * \return None.
 */
bool si446x_tx_long_packet(uint8_t *packet, uint16_t len);

/**
 * \brief .
 * 
 * \param[in] rx_buf .
 * 
 * \param[in] read_len .
 *
 * \return .
 */
uint8_t si446x_rx_packet(uint8_t *rx_buf, uint8_t read_len);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_rx_init(void);

/**
 * \brief Checks if the device is working.
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_check_device(void);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_check_cts(void);

/**
 * \brief .
 * 
 * \param[in] pwr .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_tx_power(uint8_t pwr);

/**
 * \brief .
 * 
 * \param[in] start_property .
 *
 * \param[in] para_buf .
 *
 * \param[in] length .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_properties(uint16_t start_property, uint8_t *para_buf, uint8_t length);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf);

/**
 * \brief .
 * 
 * \param[in] parameters .
 *
 * \param[in] para_len .
 * 
 * \return None.
 */
void si446x_set_config(const uint8_t *parameters, uint16_t para_len);

/**
 * \brief .
 * 
 * \param[in] len .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_preamble_len(uint8_t len);

/**
 * \brief .
 * 
 * \param[in] sync_word .
 *
 * \param[in] len .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_sync_word(uint8_t *sync_word, uint8_t len);

/**
 * \brief .
 * 
 * \param[in] gpio0_mode .
 *
 * \param[in] gpio1_mode .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode);

/**
 * \brief .
 * 
 * \param[in] cmd .
 *
 * \param[in] para_buf .
 *
 * \param[in] len .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t len);

/**
 * \brief Reads a command.
 * 
 * \param[in] cmd is the command to read.
 *
 * \param[in] para_buf is buffer to store the parameters.
 *
 * \param[in] length is the length of the parameter to read.
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_get_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t length);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_tx_interrupt(void);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_rx_interrupt(void);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_clear_interrupts(void);

/**
 * \brief .
 * 
 * \param[in] data .
 *
 * \param[in] len .
 * 
 * \return None.
 */
void si446x_write_tx_fifo(uint8_t *data, uint8_t len);

/**
 * \brief .
 * 
 * \param[in] data .
 * 
 * \param[in] read_len .
 *
 * \return .
 */
uint8_t si446x_read_rx_fifo(uint8_t *data, uint8_t read_len);

/**
 * \brief Resets the FIFO.
 * 
 * \return None.
 */
void si446x_fifo_reset(void);

/**
 * \brief Changes the radio state to TX.
 * 
 * \return None.
 */
void si446x_enter_tx_mode(void);

/**
 * \brief Changes the radio state to RX.
 * 
 * \return None.
 */
void si446x_enter_rx_mode(void);

/**
 * \brief Changes the radio state to standby.
 * 
 * return TRUE/FALSE if successful or not.
 */
bool si446x_enter_standby_mode(void);

/**
 * \brief Inquire interrupt.
 * 
 * \return TRUE/FALSE if an interrupt occurs.
 */
bool si446x_wait_nirq(void);

/**
 * \brief .
 *
 * \return TRUE/FALSE if a packet was sent or not.
 */
bool si446x_wait_packet_sent(void);

/**
 * \brief Checks GPIO1 interruption.
 * 
 * \return TRUE/FALSE if an interrupt occurs.
 */
bool si446x_wait_gpio1(void);

/**
 * \brief SPI port initialization.
 *
 * \return The status/error code.
 */
int si446x_spi_init(void);

/**
 * \brief Enables the SPI bus.
 *
 * \return None.
 */
void si446x_spi_enable(void);

/**
 * \brief Disables the SPI bus.
 *
 * \return None.
 */
void si446x_spi_disable(void);

/**
 * \brief Transfers an array through the SPI interface.
 * 
 * \param[in] data is an array to be  transfered.
 *
 * \param[in] size is the size of the data to be transfered.
 * 
 * \return The status/error code.
 */
int si446x_spi_write(uint8_t *data, uint16_t size);

/**
 * \brief Reads data from the SPI interface.
 * 
 * \param[in] data is a pointer to where the incoming data will be stored.
 *
 * \param[in] size is how many bytes will be read from the SPI interface.
 * 
 * \return The status/error code.
 */
int si446x_spi_read(uint8_t *data, uint16_t size);

/**
 * \brief Makes a byte transfer in the SPI port (Writing and reading).
 * 
 * \param[in] byte is the byte to write in the SPI port.
 * 
 * \return The read byte during the transfer operation.
 */
uint8_t si446x_spi_transfer(uint8_t byte);

/**
 * \brief Si446x GPIO initialization.
 * 
 * \return None.
 */
void si446x_gpio_init(void);

/**
 * \brief Sets a given GPIO pin.
 *
 * \param[in] pin is the GPIO pin to set. It can be:
 * \parblock
 *      -\b SI446X_GPIO_SDN
 *      -\b SI446X_GPIO_GPIO_0
 *      -\b SI446X_GPIO_GPIO_1
 *      -\b SI446X_GPIO_GPIO_2
 *      -\b SI446X_GPIO_GPIO_3
 *      .
 * \endparblock
 *
 * \return None.
 */
void si446x_gpio_set_pin(si446x_gpio_pin_t pin);

/**
 * \brief Clears a given GPIO pin.
 *
 * \param[in] pin is the GPIO pin to clear. It can be:
 * \parblock
 *      -\b SI446X_GPIO_SDN
 *      -\b SI446X_GPIO_GPIO_0
 *      -\b SI446X_GPIO_GPIO_1
 *      -\b SI446X_GPIO_GPIO_2
 *      -\b SI446X_GPIO_GPIO_3
 *      .
 * \endparblock
 *
 * \return None.
 */
void si446x_gpio_clear_pin(si446x_gpio_pin_t pin);

/**
 * \brief Reads the state of a given GPIO pin.
 *
 * \param[in] pin is the GPIO pin to read. It can be:
 * \parblock
 *      -\b SI446X_GPIO_NIRQ
 *      -\b SI446X_GPIO_GPIO_1
 *      .
 * \endparblock
 *
 * \return None.
 */
int si446x_gpio_get_pin(si446x_gpio_pin_t pin);

/**
 * \brief Seconds delay.
 * 
 * \param[in] s is the delay in seconds.
 * 
 * \return None.
 */
void si446x_delay_s(uint8_t s);

/**
 * \brief Milliseconds delay.
 * 
 * \param[in] ms is the delay in milliseconds.
 * 
 * \return None.
 */
void si446x_delay_ms(uint16_t ms);

/**
 * \brief Microseconds delay.
 * 
 * \param[in] us is the delay in microseconds.
 * 
 * \return None.
 */
void si446x_delay_us(uint32_t us);

#endif /* SI446X_H_ */

/**< \} End of si446x group */
