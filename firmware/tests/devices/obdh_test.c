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

#include <devices/obdh/obdh.h>
#include <drivers/spi_slave/spi_slave.h>

uint16_t generate_random_len(int max_value);

/* OBDH Configuration */
spi_config_t spi_config = {0U, SPI_MODE_0};
spi_port_t spi_port = SPI_PORT_2;

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
}

static void obdh_send_response_test(void **state)
{
}

int main(void)
{ 
  const struct CMUnitTest obdh_tests[] = {
    cmocka_unit_test(obdh_init_test),
    cmocka_unit_test(obdh_read_request_test),
    cmocka_unit_test(obdh_send_response_test),
};

  return cmocka_run_group_tests(obdh_tests, NULL, NULL);
}

uint16_t generate_random_len(int max_value)
{
  return (uint16_t)(rand()%max_value);
}

/** \} End of obdh_test group */
