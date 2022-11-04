/*
 * spi_slave_wrap.c
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
 * \brief SPI Slave driver wrap implementation.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.0.1
 * 
 * \date 2022/05/22
 * 
 * \addtogroup spi_slave_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "spi_wrap.h"

int __wrap_spi_slave_init(spi_port_t port, spi_config_t config)
{
    check_expected(port);
    check_expected(config.speed_hz);
    check_expected(config.mode);

    return mock_type(int);
}

int __wrap_spi_slave_enable(spi_port_t port)
{
    check_expected(port);

    return mock_type(int);
}

int __wrap_spi_slave_disable(spi_port_t port)
{
    check_expected(port);

    return mock_type(int);
}

uint16_t __wrap_spi_slave_read_available(spi_port_t port)
{
    check_expected(port);

    return mock_type(uint16_t);
}

int __wrap_spi_slave_read(spi_port_t port, uint8_t *data, uint16_t len)
{
    check_expected(port);
    check_expected(len);

    if (data != NULL)
    {
        uint16_t i = 0;
        for (i=0;i<len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_spi_slave_write(spi_port_t port, uint8_t *data, uint16_t len)
{
    check_expected(port);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_spi_flush(spi_port_t port)
{
    check_expected(port);

    return mock_type(int);
}

uint16_t spi_slave_bytes_not_sent(spi_port_t port)
{
    check_expected(port);

    return mock_type(uint16_t)
}

/** \} End of spi_slave_wrap group */
