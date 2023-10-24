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
 * \version 0.4.3
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

#include <system/sys_log/sys_log.h>

#include <system/cmdpr.h>
#include <devices/obdh/obdh.h>
#include <app/structs/ttc_data.h>
#include <drivers/spi_slave/spi_slave.h>

void generate_random_request(uint8_t *request);

void generate_random_packet(uint8_t *packet, uint16_t len);

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

    assert_return_code(obdh_init(), 0);
}

static void obdh_read_request_test(void **state)
{
    int err = 0;
    uint16_t dummy;


    obdh_request_t obdh_request;

    uint8_t request[7] = {0};
    uint16_t i = 0;

    generate_random_request(request);

    obdh_request.command = request[1];

    expect_value(__wrap_spi_slave_dma_read, len, 7);

    for (i = 0; i<7; i++)
    {
        will_return(__wrap_spi_slave_dma_read, request[i]);
    }

    if ((obdh_request.command != 0xFF) && (obdh_request.command != 0x00)) /* Received a request */
    {
        switch(obdh_request.command)
        {
            case CMDPR_CMD_READ_PARAM:
                obdh_request.parameter = request[2];

                break;

            case CMDPR_CMD_WRITE_PARAM:
                obdh_request.parameter = request[2];

                if (obdh_request.parameter == CMDPR_PARAM_TX_ENABLE)
                {
                    obdh_request.data.param_8 = request[3];
                }
                else
                {
                    err = -1;
                }

                break;

            case CMDPR_CMD_TRANSMIT_PACKET:

                obdh_request.data.data_packet.len = request[2];

                /* obdh_write_read_bytes */
                uint8_t buffer[230];

                for (i=0U; i<230; i++)
                {
                    buffer[i] = 0x00U;
                }

                dummy = request[2] + 2U;

                expect_memory(__wrap_spi_slave_dma_write, data, buffer, (int)dummy);
                expect_value(__wrap_spi_slave_dma_write, len, dummy);

                /* Create a random packet */
                generate_random_packet(obdh_request.data.data_packet.packet, obdh_request.data.data_packet.len);

                dummy = obdh_request.data.data_packet.len + 3;
                expect_value(__wrap_spi_slave_dma_read, len, dummy);
                for (i = 0; i<dummy; i++)
                {
                    will_return(__wrap_spi_slave_dma_read, obdh_request.data.data_packet.packet[i]); 
                }

                break;

        case CMDPR_CMD_READ_FIRST_PACKET:
            obdh_request.data.data_packet.len = request[2];

            break;

        case 0x00:
            /* Read Mode */
            break;

        default:
            err = -1;

            break;
        }
   }

    assert_int_equal(obdh_read_request(&(obdh_request)), err);
}

static void obdh_send_response_test(void **state)
{
    uint8_t write_buffer[4] = {0};

    obdh_response_t obdh_response = generate_random_response();

    expect_value(__wrap_spi_slave_write, port, spi_port);
    expect_memory(__wrap_spi_slave_write, data, &obdh_response.command, 1);
    expect_value(__wrap_spi_slave_write, len, 1);
    will_return(__wrap_spi_slave_write, 0);

    if (obdh_response.command == CMDPR_CMD_READ_PARAM)
    {
        expect_value(__wrap_spi_slave_write, port, spi_port);
        expect_memory(__wrap_spi_slave_write, data, &obdh_response.parameter, 1);
        expect_value(__wrap_spi_slave_write, len, 1);
        will_return(__wrap_spi_slave_write, 0);
    
        switch(cmdpr_param_size(obdh_response.parameter))
        {
            case 1:
                obdh_response.data.param_8 = (uint8_t)(rand() % 50);

                expect_value(__wrap_spi_slave_write, port, spi_port);
                expect_memory(__wrap_spi_slave_write, data, &obdh_response.data.param_8, 1);
                expect_value(__wrap_spi_slave_write, len, 1);
                will_return(__wrap_spi_slave_write, 0);

                break;
            case 2:
                obdh_response.data.param_16 = (uint16_t)(rand() % 50);

                write_buffer[0] = (uint8_t)(obdh_response.data.param_16 & 0xFFU);
                write_buffer[1] = (uint8_t)((obdh_response.data.param_16 >> 8) & 0xFFU);

                expect_value(__wrap_spi_slave_write, port, spi_port);
                expect_memory(__wrap_spi_slave_write, data, write_buffer, 2);
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
                expect_memory(__wrap_spi_slave_write, data, write_buffer, 4);
                expect_value(__wrap_spi_slave_write, len, 4);
                will_return(__wrap_spi_slave_write, 0);

                break;
        }
    }
    else if (obdh_response.command == CMDPR_CMD_READ_FIRST_PACKET)
    {
        write_buffer[0]=obdh_response.data.data_packet.len & 0xFFU;
        write_buffer[1]=(obdh_response.data.data_packet.len >> 8);

        expect_value(__wrap_spi_slave_write, port, spi_port);
        expect_memory(__wrap_spi_slave_write, data, write_buffer, (int)2);
        expect_value(__wrap_spi_slave_write, len, 2);
        will_return(__wrap_spi_slave_write, 0);

        expect_value(__wrap_spi_slave_write, port, spi_port);
        expect_memory(__wrap_spi_slave_write, data, obdh_response.data.data_packet.packet, (int)obdh_response.data.data_packet.len);
        expect_value(__wrap_spi_slave_write, len, obdh_response.data.data_packet.len);
        will_return(__wrap_spi_slave_write, 0);
    }

    assert_return_code(obdh_send_response(&obdh_response), 0);
}

int main(void)
{
    srand(time(NULL));
  
    const struct CMUnitTest obdh_tests[] = {
        cmocka_unit_test(obdh_init_test),
        cmocka_unit_test(obdh_read_request_test),
        //cmocka_unit_test(obdh_send_response_test),
    };

    return cmocka_run_group_tests(obdh_tests, NULL, NULL);
}

void generate_random_request(uint8_t *request)
{
    /* Preamble */
    request[0]= 0xE7;

    /* Command */
    request[1]= (uint8_t)(rand() % 4 + 1);

    /* Paramater */
    if ((request[1] == CMDPR_CMD_READ_PARAM) || (request[1]== CMDPR_CMD_WRITE_PARAM))
    {
        request[2] = (uint8_t)(rand() % 24);
    }
    /* Length */
    else if ((request[1] == CMDPR_CMD_TRANSMIT_PACKET))
    {
        request[2] = (uint8_t)(rand() % max_packet_size + 1);
    }
}

void generate_random_packet(uint8_t *packet, uint16_t len)
{
    uint16_t i;
    for (i = 0; i<len; i++)
    {
        packet[i] = (uint8_t)(rand());
    }
}

obdh_response_t generate_random_response(void)
{
    uint8_t answer_commands[] = {CMDPR_CMD_READ_PARAM, CMDPR_CMD_READ_FIRST_PACKET};
    obdh_response_t rand_response;

    rand_response.command = answer_commands[rand()%2];

    if (rand_response.command == CMDPR_CMD_READ_PARAM)
    {
        rand_response.parameter = (uint8_t)(rand()%24);
    }
    else if(rand_response.command == CMDPR_CMD_READ_FIRST_PACKET)
    {
        rand_response.data.data_packet.len = ((uint16_t)(rand()%max_packet_size));

        uint16_t i = 0;

        for(i = 0; i < rand_response.data.data_packet.len; i++)
        {
            rand_response.data.data_packet.packet[i] = i;
        }
    }

    return rand_response;
}

/** \} End of obdh_test group */
