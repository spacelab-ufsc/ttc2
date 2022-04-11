/*
 * queue_test.c
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
 * \brief Unit test of the Queue container.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.14
 * 
 * \date 2022/04/11
 * 
 * \defgroup queue_unit_test Queue
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

#include <libs/containers/queue.h>

unsigned int generate_random(unsigned int l, unsigned int r);

static void queue_init_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    assert_int_equal(buf.head, 0U);
    assert_int_equal(buf.tail, 0U);
    assert_int_equal(buf.size, 0U);
    assert_int_equal(buf.mtu, QUEUE_LENGTH);

    uint16_t i = 0;
    for(i = 0; i < buf.mtu; i++)
    {
        assert_int_equal(buf.data[i], QUEUE_DEFAULT_BYTE);
    }
}

static void queue_length_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    assert_int_equal(queue_length(&buf), QUEUE_LENGTH);
}

static void queue_push_back_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    uint8_t data[QUEUE_LENGTH] = {0};

    uint16_t i = 0;
    for(i = 0; i < QUEUE_LENGTH; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
    }

    for(i = 1; i < QUEUE_LENGTH; i++)
    {
        queue_push_back(&buf, data[i - 1]);

        assert_memory_equal(buf.data, data, i);

        assert_int_equal(queue_size(&buf), i);
    }
}

static void queue_pop_front_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    uint8_t data[QUEUE_LENGTH] = {0};

    uint16_t i = 0;
    for(i = 0; i < QUEUE_LENGTH; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
        queue_push_back(&buf, data[i]);
    }

    for(i = 1; i < QUEUE_LENGTH; i++)
    {
        assert_int_equal(queue_pop_front(&buf), data[i - 1]);

        assert_int_equal(queue_size(&buf), QUEUE_LENGTH - i - 1);
    }
}

static void queue_empty_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    uint16_t i = 0;
    for(i = 0; i < (2 * QUEUE_LENGTH); i++)
    {
        if (i == 0)
        {
            assert_true(queue_empty(&buf));
        }
        else
        {
            assert_false(queue_empty(&buf));
        }

        queue_push_back(&buf, generate_random(0, UINT8_MAX));
    }
}

static void queue_full_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    uint16_t i = 0;
    for(i = 0; i < (2 * QUEUE_LENGTH); i++)
    {
        queue_push_back(&buf, generate_random(0, UINT8_MAX));

        if ((i + 1) < (QUEUE_LENGTH - 1))
        {
            assert_false(queue_full(&buf));
        }
        else
        {
            assert_true(queue_full(&buf));
        }
    }
}

static void queue_size_test(void **state)
{
    queue_t buf = {0};

    queue_init(&buf);

    uint16_t i = 0;
    for(i = 0; i < queue_length(&buf) - 1; i++)
    {
        queue_push_back(&buf, generate_random(0, UINT8_MAX));

        assert_int_equal(queue_size(&buf), i + 1);
    }
}

int main(void)
{
    const struct CMUnitTest queue_tests[] = {
        cmocka_unit_test(queue_init_test),
        cmocka_unit_test(queue_length_test),
        cmocka_unit_test(queue_push_back_test),
        cmocka_unit_test(queue_pop_front_test),
        cmocka_unit_test(queue_empty_test),
        cmocka_unit_test(queue_full_test),
        cmocka_unit_test(queue_size_test),
    };

    return cmocka_run_group_tests(queue_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of queue_test group */
