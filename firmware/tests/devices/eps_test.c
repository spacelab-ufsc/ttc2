/*
 * eps_test.c
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
 * \brief Unit test of the eps device.
 *
 *  * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.3.4
 *
 * \date 2023/02/17
 *
 * \defgroup eps_unit_test EPS
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

#include <devices/eps/eps.h>
#include <drivers/uart/uart.h>

uint16_t generate_random_len(int max_value);
eps_request_t generate_random_request(void);

/* EPS Configuration */
uart_config_t uart_config = {1000000, 115200, 8, UART_NO_PARITY, UART_ONE_STOP_BIT};
uart_port_t uart_port = UART_PORT_0;
int max_packet_size = 220;


static void eps_init_test(void **state)
{
  expect_value(__wrap_uart_init, port, uart_port);
  expect_value(__wrap_uart_init, config.clock, uart_config.clock);
  expect_value(__wrap_uart_init, config.baudrate, uart_config.baudrate);
  expect_value(__wrap_uart_init, config.data_bits, uart_config.data_bits);
  expect_value(__wrap_uart_init, config.parity, uart_config.parity);
  expect_value(__wrap_uart_init, config.stop_bits, uart_config.stop_bits);
  will_return(__wrap_uart_init, 0);

  expect_value(__wrap_uart_rx_enable, port, uart_port);
  will_return(__wrap_uart_rx_enable, 0);

  assert_return_code(eps_init(), 0);
}

static void eps_read_request_test(void **state)
{
  eps_request_t eps_request = generate_random_request();

  expect_value(__wrap_uart_read_available, port, uart_port);
  will_return(__wrap_uart_read_available, eps_request.data.data_packet.len + 1); /* Packet len + 1 byte for command) */

  expect_value(__wrap_uart_read, port, uart_port);
  expect_value(__wrap_uart_read, len, 1);
  will_return(__wrap_uart_read, eps_request.command);
  will_return(__wrap_uart_read, 0);

  expect_value(__wrap_uart_read_available, port, uart_port);
  will_return(__wrap_uart_read_available, eps_request.data.data_packet.len);
  
  expect_value(__wrap_uart_read, port, uart_port);
  expect_value(__wrap_uart_read, len, eps_request.data.data_packet.len);
  for (int i = 0; i < eps_request.data.data_packet.len; i++)
  {
    will_return(__wrap_uart_read, eps_request.data.data_packet.packet[i]);
  }
  will_return(__wrap_uart_read, 0);

  assert_return_code(eps_read_request(&eps_request), 0);
}

int main(void)
{
  srand(time(NULL));
  
  const struct CMUnitTest eps_tests[] = {
    cmocka_unit_test(eps_init_test),
    cmocka_unit_test(eps_read_request_test),
};

  return cmocka_run_group_tests(eps_tests, NULL, NULL);
}

uint16_t generate_random_len(int max_value)
{
  return (uint16_t)(rand()%max_value+1);
}

eps_request_t generate_random_request(void)
{
  eps_request_t rand_request;
  rand_request.command = CMDPR_CMD_TRANSMIT_PACKET;
  rand_request.data.data_packet.len = generate_random_len(max_packet_size);
  
  for (int i = 0; i < rand_request.data.data_packet.len; i++)
  {
    rand_request.data.data_packet.packet[i] = (uint8_t)i;
  }

  return rand_request;
}

/** \} End of eps_test group */