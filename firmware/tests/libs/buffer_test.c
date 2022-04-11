/*
 * buffer_test.c
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
 * \brief Unit test of the Buffer container.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.14
 * 
 * \date 2022/04/11
 * 
 * \defgroup buffer_unit_test Buffer
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <stdlib.h>

#include <libs/containers/buffer.h>

unsigned int generate_random(unsigned int l, unsigned int r);

static void buffer_init_test(void **state)
{
    buffer_t buf = {0};

    buffer_init(&buf);

    assert_int_equal(buf.mtu, BUFFER_LENGTH);
    assert_int_equal(buf.size, 0U);

    uint16_t i = 0;
    for(i = 0; i < buf.mtu; i++)
    {
        assert_int_equal(buf.data[i], BUFFER_DEFAULT_BYTE);
    }
}

static void buffer_length_test(void **state)
{
    buffer_t buf = {0};

    buffer_init(&buf);

    assert_int_equal(buffer_length(&buf), BUFFER_LENGTH);
}

static void buffer_fill_test(void **state)
{
    uint16_t i = 0;
    for(i = 0; i < (2 * BUFFER_LENGTH); i++)
    {
        buffer_t buf = {0};

        buffer_init(&buf);

        uint8_t data[2 * BUFFER_LENGTH] = {0};

        uint16_t j = 0;
        for(j = 0; j < i; j++)
        {
            data[j] = generate_random(0, UINT8_MAX);
        }

        if (i <= buffer_length(&buf))
        {
            assert_true(buffer_fill(&buf, data, i));

            assert_memory_equal(buf.data, data, i);
        }
        else
        {
            assert_false(buffer_fill(&buf, data, i));

            assert_memory_not_equal(buf.data, data, i);
        }
    }
}

static void buffer_append_test(void **state)
{
    buffer_t buf = {0};

    buffer_init(&buf);

    uint8_t data[BUFFER_LENGTH] = {0};
    uint16_t len = generate_random(1, BUFFER_LENGTH);

    uint16_t i = 0;
    for(i = 0; i < len; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
    }

    assert_true(buffer_fill(&buf, data, len));
    assert_true(buffer_append(&buf, data, BUFFER_LENGTH - len));

    assert_memory_equal(buf.data, data, len);
    assert_memory_equal(buf.data + len, data, BUFFER_LENGTH - len);
}

static void buffer_clear_test(void **state)
{
    buffer_t buf = {0};

    buffer_init(&buf);

    uint8_t data[BUFFER_LENGTH] = {0};
    uint16_t len = generate_random(0, BUFFER_LENGTH);

    uint16_t i = 0;
    for(i = 0; i < len; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
    }

    assert_true(buffer_fill(&buf, data, len));

    buffer_clear(&buf);

    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        assert_int_equal(buf.data[i], BUFFER_DEFAULT_BYTE);
    }

    assert_int_equal(buf.size, 0U);
}

static void buffer_empty_test(void **state)
{
    uint16_t i = 0;
    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        buffer_t buf = {0};

        buffer_init(&buf);

        uint8_t data[BUFFER_LENGTH] = {0};

        assert_true(buffer_fill(&buf, data, i));

        if (i == 0)
        {
            assert_true(buffer_empty(&buf));
        }
        else
        {
            assert_false(buffer_empty(&buf));
        }
    }
}

static void buffer_full_test(void **state)
{
    uint16_t i = 0;
    for(i = 0; i < (2 * BUFFER_LENGTH); i++)
    {
        buffer_t buf = {0};

        buffer_init(&buf);

        uint8_t data[BUFFER_LENGTH] = {0};

        if (i <= BUFFER_LENGTH)
        {
            assert_true(buffer_fill(&buf, data, i));
        }
        else
        {
            assert_false(buffer_fill(&buf, data, i));
        }

        if (i == BUFFER_LENGTH)
        {
            assert_true(buffer_full(&buf));
        }
        else
        {
            assert_false(buffer_full(&buf));
        }
    }
}

static void buffer_size_test(void **state)
{
    uint16_t i = 0;
    for(i = 0; i < BUFFER_LENGTH; i++)
    {
        buffer_t buf = {0};

        buffer_init(&buf);

        uint8_t data[BUFFER_LENGTH] = {0};

        assert_true(buffer_fill(&buf, data, i));

        assert_int_equal(buffer_size(&buf), i);
    }
}

int main(void)
{
    const struct CMUnitTest buffer_tests[] = {
        cmocka_unit_test(buffer_init_test),
        cmocka_unit_test(buffer_length_test),
        cmocka_unit_test(buffer_fill_test),
        cmocka_unit_test(buffer_append_test),
        cmocka_unit_test(buffer_clear_test),
        cmocka_unit_test(buffer_empty_test),
        cmocka_unit_test(buffer_full_test),
        cmocka_unit_test(buffer_size_test),
    };

    return cmocka_run_group_tests(buffer_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of buffer_test group */
