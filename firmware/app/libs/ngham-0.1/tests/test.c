/*
 * test.c
 * 
 * Copyright The NGHam Contributors.
 * 
 * This file is part of NGHam.
 * 
 * NGHam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * NGHam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with NGHam. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief NGHam library unit test.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2023/03/12
 * 
 * \defgroup test Test
 * \ingroup ngham
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <ngham/ngham.h>

static void ngham_init_test(void **state)
{
    assert_return_code(ngham_init(), 0);
}

static void ngham_encode_test(void **state)
{
    uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t data_len = 10U;
    uint8_t flags = 0U;

    uint8_t pkt[300] = {0U};
    uint16_t pkt_len = UINT16_MAX;

    assert_return_code(ngham_encode(data, data_len, flags, pkt, &pkt_len), 0);

    uint8_t expected_pkt[300] = {170, 170, 170, 170, 93, 230, 42, 126, 59, 73, 205, 237, 72, 15, 194, 153, 9, 117, 186, 137, 36, 154, 52, 142, 183, 70, 206, 90, 151, 125, 204, 50, 162, 191, 62, 10, 16, 241, 136, 148, 205, 234, 224, 253, 216, 143, 5, 252, 58, 82, 207, 117, 235, 194, 57, 208, 120, 236};
    uint16_t expected_pkt_len = 58U;

    assert_memory_equal((void*)pkt, (void*)expected_pkt, expected_pkt_len);
    assert_int_equal(pkt_len, expected_pkt_len);
}

static void ngham_decode_test(void **state)
{
}

static void crc_ccitt_byte_test(void **state)
{
//    assert_return_code(crc_ccitt_byte(), );
}

static void crc_ccitt_test(void **state)
{
//    assert_return_code(crc_ccitt(), );
}

int main()
{
    const struct CMUnitTest ngham_tests[] = {
        cmocka_unit_test(ngham_init_test),
        cmocka_unit_test(ngham_encode_test),
        cmocka_unit_test(ngham_decode_test),
        cmocka_unit_test(crc_ccitt_byte_test),
        cmocka_unit_test(crc_ccitt_test),
    };

    return cmocka_run_group_tests(ngham_tests, NULL, NULL);
}

/**< \} End of test group */
