/*
 * media_test.c
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
 * \brief Unit test of the media device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.2
 * 
 * \date 2021/08/07
 * 
 * \defgroup media_unit_test Media
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/media/media.h>
#include <drivers/flash/flash.h>

static void media_init_test(void **state)
{
    media_t med = MEDIA_INT_FLASH;
    int err = -1;

    switch(med)
    {
    case MEDIA_INT_FLASH:
        will_return(__wrap_flash_init, 0);
        err = 0;

        break;
    default:
        break;

    }
    assert_return_code(media_init(med), err);
}

static void media_write_test(void **state)
{
    int err = -1;
    uint16_t i = 0U;

    media_t med = MEDIA_INT_FLASH;
    uint8_t data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t adr = 0U;
    uint32_t sector = FLASH_SEG_A_ADR;
    uint16_t len = 10U;

    switch(med)
    {
    case MEDIA_INT_FLASH:
        will_return(__wrap_flash_mutex_take, 0);

        /* Address index */
        uintptr_t adr_idx = adr + sector;

        for(i=0; i<len; ++i)
        {
            uintptr_t adr_counter = adr_idx + i;

            expect_value(__wrap_flash_write_single, data, data[i]);
            expect_value(__wrap_flash_write_single, addr, adr_counter);
        }
        will_return(__wrap_flash_mutex_give, 0);
        err = 0;
        break;
    default:
        break;
    }

    assert_return_code(media_write(med, adr, sector, data, len), err);
}

static void media_read_test(void **state)
{
    int err = -1;
    uint16_t i = 0U;

    media_t med = MEDIA_INT_FLASH;
    uint8_t data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t adr = 0U;
    uint32_t sector = FLASH_SEG_A_ADR;
    uint16_t len = 10U;

    switch(med)
    {
    case MEDIA_INT_FLASH:
        will_return(__wrap_flash_mutex_take, 0);

        /* Address index */
        uintptr_t adr_idx = adr + sector;

        for(i=0; i<len; ++i)
        {
            uintptr_t adr_counter = adr_idx + i;

            expect_value(__wrap_flash_read_single, addr, adr_counter);
            will_return(__wrap_flash_read_single, data[i]);
        }
        will_return(__wrap_flash_mutex_give, 0);
        err = 0;
        break;
    default:
        break;
    }

    assert_return_code(media_read(med, adr, sector, data, len), err);
}

static void media_erase_test(void **state)
{
    int err = -1;

    media_t med = MEDIA_INT_FLASH;
    uint32_t sector = FLASH_SEG_A_ADR;

    switch(med)
    {
    case MEDIA_INT_FLASH:
        will_return(__wrap_flash_mutex_take, 0);
        expect_value(__wrap_flash_erase, region,(uintptr_t)sector);
        will_return(__wrap_flash_mutex_give, 0);
        err = 0;
        break;

    default:
        break;
    }

    assert_return_code(media_erase(med, sector), err);
}

int main(void)
{
    const struct CMUnitTest media_tests[] = {
        cmocka_unit_test(media_init_test),
        cmocka_unit_test(media_write_test),
        cmocka_unit_test(media_read_test),
        cmocka_unit_test(media_erase_test),
    };

    return cmocka_run_group_tests(media_tests, NULL, NULL);
}

/** \} End of media_test group */
