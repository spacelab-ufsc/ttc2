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

obdh_response_t generate_random_response(void);

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

  
  /*printf("\nRequest info:\n");
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

    switch(cmdpr_param_size(obdh_request.obdh_parameter))
    {
      case 1:
	expect_value(__wrap_spi_slave_read, port, spi_port);
	expect_value(__wrap_spi_slave_read, len, 1);
	will_return(__wrap_spi_slave_read, obdh_request.data.param_8);
	will_return(__wrap_spi_slave_read, 0);
      
        break;

      case 2:
	expect_value(__wrap_spi_slave_read, port, spi_port);
	expect_value(__wrap_spi_slave_read, len, 2);
	for(int i=0;i<2;i++)
	{
	  will_return(__wrap_spi_slave_read, read_buffer[i]);
	}
	obdh_request.data.param_16 = ((uint16_t)read_buffer[0]) | ((uint16_t)(read_buffer[1])<<8);
	will_return(__wrap_spi_slave_read, 0);
	
	break;

    case 4:
      expect_value(__wrap_spi_slave_read, port, spi_port);
      expect_value(__wrap_spi_slave_read, len, 4);
      for(int i=0;i<4;i++)
      {
	will_return(__wrap_spi_slave_read, read_buffer[i]);
      }
      obdh_request.data.param_32 = ((uint32_t)read_buffer[0]) | ((uint32_t)(read_buffer[1])<<8) | ((uint32_t)(read_buffer[2])<<16) | ((uint32_t)(read_buffer[3])<<24);
      will_return(__wrap_spi_slave_read, 0);

      break;
    }

    break;
    
  case CMDPR_CMD_TRANSMIT_PACKET:
    for (int i=0; i<obdh_request.data.data_packet.len; i++)
    {
      obdh_request.data.data_packet.packet[i] = (uint8_t)(i+1);
    }
    
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
    break;
  default:
    break;
  }

  assert_return_code(obdh_read_request(&(obdh_request)), 0);
}

static void obdh_send_response_test(void **state)
{
  uint8_t write_buffer[4] = {0};

  obdh_response_t obdh_response = generate_random_response();
  obdh_response.obdh_answer_command = CMDPR_CMD_READ_FIRST_PACKET;
  obdh_response.data.data_packet.len = 30;

  printf("\nRequest info:\n");
  printf("packet_size: %i", obdh_response.data.data_packet.len);
  printf("\ncommand: %i", obdh_response.obdh_answer_command);
  printf("\nparameter: %i\n", obdh_response.obdh_parameter);
  
  expect_value(__wrap_spi_slave_write, port, spi_port);
  expect_memory(__wrap_spi_slave_write, data, (void*)&(obdh_response.obdh_answer_command), 1);
  expect_value(__wrap_spi_slave_write, len, 1);
  will_return(__wrap_spi_slave_write, 0);

  if (obdh_response.obdh_answer_command == CMDPR_CMD_READ_PARAM)
  {
    expect_value(__wrap_spi_slave_write, port, spi_port);
    expect_value(__wrap_spi_slave_write, data, obdh_response.obdh_parameter);
    expect_value(__wrap_spi_slave_write, len, 1);
    will_return(__wrap_spi_slave_write, 0);
    
    switch(cmdpr_param_size(obdh_response.obdh_parameter))
    {
      case 1:
	obdh_response.data.param_8 = (uint8_t)(rand()%50);
	
	expect_value(__wrap_spi_slave_write, port, spi_port);
	expect_value(__wrap_spi_slave_write, data, obdh_response.data.param_8);
	expect_value(__wrap_spi_slave_write, len, 1);
	will_return(__wrap_spi_slave_write, 0);
	
	break;
	
      case 2:
	obdh_response.data.param_16 = (uint16_t)(rand()%50);
	
	write_buffer[0] = (uint8_t)(obdh_response.data.param_16 & 0xFFU);
	write_buffer[1] = (uint8_t)((obdh_response.data.param_16 >> 8) & 0xFFU);
      
	expect_value(__wrap_spi_slave_write, port, spi_port);
	expect_value(__wrap_spi_slave_write, data, write_buffer);
	expect_value(__wrap_spi_slave_write, len, 2);
	will_return(__wrap_spi_slave_write, 0);
	
	break;
	
      case 4:
	obdh_response.data.param_32 = (uint32_t)(rand()%50);
	
	write_buffer[0] = (uint8_t)(obdh_response.data.param_32 & 0xFFU);
	write_buffer[1] = (uint8_t)((obdh_response.data.param_32 >> 8) & 0xFFU);
	write_buffer[2] = (uint8_t)((obdh_response.data.param_32 >> 16) & 0xFFU);
	write_buffer[3] = (uint8_t)((obdh_response.data.param_32 >> 24) & 0xFFU);

	expect_value(__wrap_spi_slave_write, port, spi_port);
	expect_value(__wrap_spi_slave_write, data, write_buffer);
	expect_value(__wrap_spi_slave_write, len, 4);
	will_return(__wrap_spi_slave_write, 0);

	break;
    }
  }
  else if (obdh_response.obdh_answer_command == CMDPR_CMD_READ_FIRST_PACKET)
  {
    expect_value(__wrap_spi_slave_write, port, spi_port);
    expect_value(__wrap_spi_slave_write, data, obdh_response.data.data_packet.packet);
    expect_value(__wrap_spi_slave_write, len, obdh_response.data.data_packet.len);
    will_return(__wrap_spi_slave_write, 0); 
  }
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

obdh_response_t generate_random_response(void)
{
  uint8_t answer_commands[] = {CMDPR_CMD_READ_PARAM, CMDPR_CMD_READ_FIRST_PACKET};
  obdh_response_t rand_response;
  rand_response.obdh_answer_command = answer_commands[rand()%2];

  if (rand_response.obdh_answer_command == CMDPR_CMD_READ_PARAM)
  {
    rand_response.obdh_parameter = (uint8_t)(rand()%24);
  }

  return rand_response; 
}

/** \} End of obdh_test group */