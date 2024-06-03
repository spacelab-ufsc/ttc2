/*
 * test.c
 * 
 * Copyright The RSCLib Contributors.
 * 
 * This file is part of RSCLib.
 * 
 * RSCLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RSCLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with RSCLib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Reed-Solomon C library unit test.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2022/05/30
 * 
 * \defgroup test Test
 * \ingroup rsclib
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <string.h>
#include <cmocka.h>

#include <rsc/rsc.h>

static void rsc_init_test(void **state)
{
    reed_solomon_t rs16 = {0};

    assert_return_code(rsc_init(8, 0x187, 112, 11, 16, 208, &rs16), 0);

    /* Expected results */
    uint8_t alpha_to[50] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x87, 0x89, 0x95, 0xAD, 0xDD, 0x3D, 0x7A, 0xF4};
    uint8_t index_of[50] = {0xFF, 0x00, 0x01, 0x63, 0x02, 0xC6, 0x64, 0x6A, 0x03, 0xCD, 0xC7, 0xBC, 0x65, 0x7E, 0x6B, 0x2A};
    uint8_t genpoly[50] = {0x7A, 0xF0, 0x12, 0xB4, 0xC7, 0xB5, 0xDD, 0x31, 0xEA, 0xE1, 0x3F, 0xC7, 0x8A, 0x28, 0x36, 0xC5};

    assert_int_equal(rs16.mm,       8);
    assert_int_equal(rs16.nn,       0xFF);
    assert_memory_equal(rs16.alpha_to, alpha_to, 16);
    assert_memory_equal(rs16.index_of, index_of, 16);
    assert_memory_equal(rs16.genpoly, genpoly, 16);
    assert_int_equal(rs16.nroots,   0x10);
    assert_int_equal(rs16.fcr,      112);
    assert_int_equal(rs16.prim,     0x0B);
    assert_int_equal(rs16.iprim,    116);
    assert_int_equal(rs16.pad,      208);
}

static void rsc_encode_test(void **state)
{
    /* Expected result */
    uint8_t par_ref[32] = {219, 47, 89, 171, 224, 14, 4, 30, 113, 224, 22, 100, 152, 45, 160, 32};

    reed_solomon_t rs16 = {0};

    rsc_init(8, 0x187, 112, 11, 16, 208, &rs16);

    uint8_t data[32] = {0};
    uint8_t par[32] = {0};
    uint8_t par_len = 0;

    uint8_t i = 0;
    for(i = 0; i < 32; i++)
    {
        data[i] = i;
    }

    rsc_encode(&rs16, data, par, &par_len);

    assert_int_equal(par_len, 16);
    assert_memory_equal(par, par_ref, par_len);
}

static void rsc_decode_test(void **state)
{
//    reed_solomon_t rs16 = {0};
//
//    rsc_init(8, 0x187, 112, 11, 16, 208, &rs16);
//
//    uint8_t data[32] = {0U};
//    uint8_t par[32] = {0U};
//    uint8_t par_len = 0U;
//    uint8_t pkt[300] = {0U};
//
//    uint8_t i = 0;
//
//    for(i = 0; i < 32; i++)
//    {
//        data[i] = i;
//    }
//
//    rsc_encode(&rs16, data, par, &par_len);
//
//    /* Merge data and parity */
//    memcpy(pkt, data, 32);
//    memcpy(&pkt[32], par, par_len);
//
//    uint8_t dec_data[32] = {0U};
//    uint8_t err_pos[32] = {0U};
//    uint8_t num_err = 0U;
//
//    assert_return_code(rsc_decode(&rs16, pkt, dec_data, err_pos, &num_err), 0);
//
//    assert_memory_equal(dec_data, data, 32);
//    assert_int_equal(num_err, 0);
}

int main()
{
    const struct CMUnitTest rsc_tests[] = {
        cmocka_unit_test(rsc_init_test),
        cmocka_unit_test(rsc_encode_test),
        cmocka_unit_test(rsc_decode_test),
    };

    return cmocka_run_group_tests(rsc_tests, NULL, NULL);
}

/**< \} End of test group */
