/*
 * obdh_test.c
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
 * \brief Unit test of the obdh device.
 *
 *  * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.4
 *
 * \date 2023/02/17
 *
 * \defgroup obdh_unit_test OBDH
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <system/cmdpr.h>

#include <devices/obdh/obdh.h>
#include <drivers/spi_slave/spi_slave.h>

uint16_t generate_random_len(int max_value);

obdh_request_t generate_random_request(void);

/* OBDH Configuration */
spi_config_t spi_config = {0U, SPI_MODE_0};
spi_port_t spi_port = SPI_PORT_2;
int max_packet_size = 220;

static void obdh_init_test(void **state)
{
  expect_value(__wrap_spi_slave_init, port, spi_port);
  expect_value(__wrap_spi_slave_init, config.speed_hz, spi_config.speed_hz);
  expect_value(__wrap_spi_slave_init, config.mode, spi_config.mode);
  will_return(__wrap_spi_slave_init, 0);
  
  expect_value(__wrap_spi_slave_enable_isr, port, spi_port);
  will_return(__wrap_spi_slave_enable_isr, 0);

  assert_return_code(obdh_init(), 0); 
}

static void obdh_read_request_test(void **state)
{
  uint8_t read_buffer[4] = {0};
  for (int i=0;i<4;i++)
  {
    read_buffer[i] = i;
  }
  uint16_t packet_size = generate_random_len(max_packet_size);
  obdh_request_t obdh_request = generate_random_request();
  obdh_request.data.data_packet.len = packet_size;
  for (int i=0; i<obdh_request.data.data_packet.len; i++)
  {
    obdh_request.data.data_packet.packet[i] = (uint8_t)(i+1);
  }
  /*(
  printf("\nRequest info:\n");
  printf("packet_size: %i", packet_size);
  printf("\ncommand: %i", obdh_request.obdh_command);
  printf("\nparameter: %i\n", obdh_request.obdh_parameter);
  */
  
  expect_value(__wrap_spi_slave_read_available, port, spi_port);
  will_return(__wrap_spi_slave_read_available, packet_size);

  expect_value(__wrap_spi_slave_read, port, spi_port);
  expect_value(__wrap_spi_slave_read, len, 1);
  will_return(__wrap_spi_slave_read, obdh_request.obdh_command);
  will_return(__wrap_spi_slave_read, 0);

  switch(obdh_request.obdh_command)
  {
  case CMDPR_CMD_READ_PARAM:
    expect_value(__wrap_spi_slave_read, port, spi_port);
    expect_value(__wrap_spi_slave_read, len, 1);
    will_return(__wrap_spi_slave_read, obdh_request.obdh_parameter);
    will_return(__wrap_spi_slave_read, 0);
    break;

  case CMDPR_CMD_WRITE_PARAM:
    expect_value(__wrap_spi_slave_read, port, spi_port);
    expect_value(__wrap_spi_slave_read, len, 1);
    will_return(__wrap_spi_slave_read, obdh_request.obdh_parameter);
    will_return(__wrap_spi_slave_read, 0);
    
    /* Read uint8_t param */
    if ((obdh_request.obdh_parameter == CMDPR_PARAM_HW_VER) || (obdh_request.obdh_parameter == CMDPR_PARAM_LAST_RST_CAUSE) || (obdh_request.obdh_parameter == CMDPR_PARAM_LAST_UP_COMMAND) ||
       (obdh_request.obdh_parameter == CMDPR_PARAM_ANT_DEP_STATUS) || (obdh_request.obdh_parameter == CMDPR_PARAM_ANT_DEP_HIB) || (obdh_request.obdh_parameter == CMDPR_PARAM_TX_ENABLE) ||
       (obdh_request.obdh_parameter == CMDPR_PARAM_PACKETS_AV_FIFO_RX) || (obdh_request.obdh_parameter == CMDPR_PARAM_PACKETS_AV_FIFO_TX))
    {
      expect_value(__wrap_spi_slave_read, port, spi_port);
      expect_value(__wrap_spi_slave_read, len, 1);
      will_return(__wrap_spi_slave_read, obdh_request.data.param_8);
      will_return(__wrap_spi_slave_read, 0);
    }
    /* Read uint16_t param */
    else if ((obdh_request.obdh_parameter == CMDPR_PARAM_DEVICE_ID) || (obdh_request.obdh_parameter == CMDPR_PARAM_RST_COUNTER) || (obdh_request.obdh_parameter == CMDPR_PARAM_UC_VOLTAGE) ||
            (obdh_request.obdh_parameter == CMDPR_PARAM_UC_CURRENT) || (obdh_request.obdh_parameter == CMDPR_PARAM_UC_TEMP) || (obdh_request.obdh_parameter == CMDPR_PARAM_RADIO_VOLTAGE) ||
            (obdh_request.obdh_parameter == CMDPR_PARAM_RADIO_CURRENT) || (obdh_request.obdh_parameter == CMDPR_PARAM_RADIO_TEMP) || (obdh_request.obdh_parameter == CMDPR_PARAM_LAST_COMMAND_RSSI) ||
            (obdh_request.obdh_parameter == CMDPR_PARAM_ANT_TEMP) || (obdh_request.obdh_parameter == CMDPR_PARAM_ANT_MOD_STATUS_BITS) || (obdh_request.obdh_parameter == CMDPR_PARAM_N_BYTES_FIRST_AV_RX))
    {
      expect_value(__wrap_spi_slave_read, port, spi_port);
      expect_value(__wrap_spi_slave_read, len, 2);
      for(int i=0;i<2;i++)
      {
	will_return(__wrap_spi_slave_read, read_buffer[i]);
      }
      obdh_request.data.param_16 = ((uint16_t)read_buffer[0]) | ((uint16_t)(read_buffer[1])<<8);
      will_return(__wrap_spi_slave_read, 0);
    }
    /* Read uint32_t param */
    else if ((obdh_request.obdh_parameter == CMDPR_PARAM_FW_VER) || (obdh_request.obdh_parameter == CMDPR_PARAM_COUNTER) ||
								      (obdh_request.obdh_parameter == CMDPR_PARAM_TX_PACKET_COUNTER) || (obdh_request.obdh_parameter == CMDPR_PARAM_RX_VAL_PACKET_COUNTER))
    {
      expect_value(__wrap_spi_slave_read, port, spi_port);
      expect_value(__wrap_spi_slave_read, len, 4);
      for(int i=0;i<4;i++)
      {
	will_return(__wrap_spi_slave_read, read_buffer[i]);
      }
      obdh_request.data.param_32 = ((uint32_t)read_buffer[0]) | ((uint32_t)(read_buffer[1])<<8) | ((uint32_t)(read_buffer[2])<<16) | ((uint32_t)(read_buffer[3])<<24);
      will_return(__wrap_spi_slave_read, 0);
    }
    break;
    
  case CMDPR_CMD_TRANSMIT_PACKET:
    expect_value(__wrap_spi_slave_read_available, port, spi_port);
    will_return(__wrap_spi_slave_read_available, obdh_request.data.data_packet.len);

    expect_value(__wrap_spi_slave_read, port, spi_port);
    expect_value(__wrap_spi_slave_read, len, obdh_request.data.data_packet.len);
    for (int i=0;i<obdh_request.data.data_packet.len;i++)
    {
      will_return(__wrap_spi_slave_read, obdh_request.data.data_packet.packet[i]);
    }
    will_return(__wrap_spi_slave_read, 0);
    
  case CMDPR_CMD_READ_FIRST_PACKET:
    /* Nothing more to do */

  default:
    break;
  }

  assert_return_code(obdh_read_request(&(obdh_request)), 0);
}

static void obdh_send_response_test(void **state)
{
}

int main(void)
{
  srand(time(NULL));
  
  const struct CMUnitTest obdh_tests[] = {
    cmocka_unit_test(obdh_init_test),
    cmocka_unit_test(obdh_read_request_test),
    cmocka_unit_test(obdh_send_response_test),
};

  return cmocka_run_group_tests(obdh_tests, NULL, NULL);
}

uint16_t generate_random_len(int max_value)
{
  return (uint16_t)(rand()%max_value+1);
}

obdh_request_t generate_random_request(void)
{
  obdh_request_t rand_request;
  rand_request.obdh_command = (uint8_t)(rand()%4+1);
  if ((rand_request.obdh_command == CMDPR_CMD_READ_PARAM) || (rand_request.obdh_command == CMDPR_CMD_WRITE_PARAM))
  {
    rand_request.obdh_parameter = (uint8_t)(rand()%24); 
  }
  
  return rand_request;
}

/** \} End of obdh_test group */
