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
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.5.1
 *
 * \date 2024/04/22
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

void generate_random_response(obdh_response_t *response);

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

    /* obdh_write_read_bytes */
    uint8_t buffer[230];

    generate_random_request(request);

    obdh_request.command = request[1];

    expect_value(__wrap_spi_slave_dma_read, len, 7);

    for(i = 0; i < 7; i++)
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

                buffer[0] = 0x7EU;

                for(i = 1U; i < 230; i++)
                {
                    buffer[i] = 0x00U;
                }

                expect_value(__wrap_spi_slave_dma_change_transfer_size, transfer_size, request[2] + 3);

                dummy = request[2] + 3U;

                expect_memory(__wrap_spi_slave_dma_write, data, buffer, (int)dummy);
                expect_value(__wrap_spi_slave_dma_write, len, dummy);

                /* Create a random packet */
                generate_random_packet(obdh_request.data.data_packet.packet, obdh_request.data.data_packet.len);

                dummy = obdh_request.data.data_packet.len + 3;

                expect_value(__wrap_spi_slave_dma_read, len, dummy);
                for(i = 0; i < dummy; i++)
                {
                    will_return(__wrap_spi_slave_dma_read, obdh_request.data.data_packet.packet[i]); 
                }

                expect_value(__wrap_spi_slave_dma_change_transfer_size, transfer_size, 7U);                

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
    int err = 0;
    uint8_t i = 0;
    obdh_response_t obdh_response;
    uint8_t response[7];

    uint8_t transmission_buffer[70U];
    uint8_t transmission_buffer_p;

    generate_random_response(&(obdh_response));

    printf("obdh_command: %d\n", obdh_response.command);
    printf("obdh_param:%d\n", obdh_response.parameter);

    response[0] = 0x7EU;
    response[1] = obdh_response.command;

    switch(obdh_response.command)
    {
        case CMDPR_CMD_READ_PARAM:
            response[2] = obdh_response.parameter;

            switch(cmdpr_param_size(obdh_response.parameter))
            {
                case 1:
                    response[3] = obdh_response.data.param_8;

                    for(i = 4; i < 7; i++)
                    {
                        response[i] = 0x00;
                    }

                    break;
                case 2:
                    obdh_response.data.param_16 =  ((uint16_t)(response[3]) << 8) | (uint16_t)(response[4] & 0xFFU);

                    for(i = 5; i < 7; i++)
                    {
                        response[i] = 0x00;
                    }

                    break;
                case 4:
                    obdh_response.data.param_32 = (uint32_t)(response[3] << 24) | (uint32_t)(response[4] << 16) | (uint32_t)(response[5] << 8) | (uint32_t)(response[6]);

                    break;

                default:
                    err = -1;

                    break;
                }

            if (err == 0)
            {
                expect_memory(__wrap_spi_slave_dma_write, data, response, 7U);
                expect_value(__wrap_spi_slave_dma_write, len, (uint16_t) 7);
            }

            break;
        case CMDPR_CMD_READ_FIRST_PACKET:
            err = -1;

            if ((obdh_response.data.data_packet.len + 2) < 70U)
            {

                expect_value(__wrap_spi_slave_dma_change_transfer_size, transfer_size, (obdh_response.data.data_packet.len + 2));

                transmission_buffer[0] = 0x7EU;
                transmission_buffer[1] = 0x04U;

                for (transmission_buffer_p = 0U; transmission_buffer_p < obdh_response.data.data_packet.len; transmission_buffer_p++)
                {
                    transmission_buffer[transmission_buffer_p + 2U] = obdh_response.data.data_packet.packet[transmission_buffer_p];
                }

                expect_memory(__wrap_spi_slave_dma_write, data, transmission_buffer, (obdh_response.data.data_packet.len + 2));
                expect_value(__wrap_spi_slave_dma_write, len, obdh_response.data.data_packet.len + 2);

                expect_value(__wrap_spi_slave_dma_read, len, (obdh_response.data.data_packet.len + 2));

                expect_value(__wrap_spi_slave_dma_change_transfer_size, transfer_size, 7U);

                err = 0;
            }

            break;
        default:
            err = -1;

            break;
    }

    assert_int_equal((int) obdh_send_response(&(obdh_response)), err);
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

void generate_random_request(uint8_t *request)
{
    /* Preamble */
    request[0] = 0xE7;

    /* Command */
    request[1] = (uint8_t)(rand() % 4 + 1);

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
    for(i = 0; i < len; i++)
    {
        packet[i] = (uint8_t)(rand() % 0x100);
    }
}

void generate_random_response(obdh_response_t *response)
{
    uint8_t answer_commands[] = {CMDPR_CMD_READ_PARAM, CMDPR_CMD_READ_FIRST_PACKET};

    response->command = answer_commands[rand()%2];

    if (response->command == CMDPR_CMD_READ_PARAM)
    {
        response->parameter = (uint8_t)(rand()%24);

        switch(cmdpr_param_size(response->parameter))
        {
        case 1:
            response->data.param_8 = (uint8_t)(rand() % 0x100);
            
            break;
        case 2:
            response->data.param_16 = (uint16_t)(rand() % 0x10000);
            
            break;
        case 4:
            response->data.param_32 = (uint32_t)(rand() % 0x100000000);
            
            break;
        }
    }
    else if(response->command == CMDPR_CMD_READ_FIRST_PACKET)
    {
        response->data.data_packet.len = (uint8_t)(rand() % max_packet_size + 1);
    }
}

/** \} End of obdh_test group */
