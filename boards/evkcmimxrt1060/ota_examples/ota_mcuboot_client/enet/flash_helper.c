/*
 * Copyright (c) 2022, Freescale Semiconductor, Inc.
 * Copyright 2022 NXP. Not a Contribution
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>

#include "flash_helper.h"
#include "mflash_drv.h"
#include "fsl_debug_console.h"
#include "mbedtls/sha256.h"

void hexdump(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    const int CNT             = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08x  ", i);
        PRINTF("%02X ", src8[i]);
        if ((i && n == CNT - 1) || i + 1 == size)
        {
            int rem = CNT - 1 - n;
            for (int j = 0; j < rem; j++)
                PRINTF("   ");
            PRINTF("|");
            for (int j = n; j >= 0; j--)
                PUTCHAR(isprint(src8[i - j]) ? src8[i - j] : '.');
            PRINTF("|\n");
        }
    }
    PUTCHAR('\n');
}

int flash_verify_erase(uint32_t offset, size_t size)
{
    uint32_t buf[128 / sizeof(uint32_t)];
    status_t status;

    if (size % sizeof(uint32_t))
    {
        return kStatus_InvalidArgument;
    }

    /* All sectors are of the same size */

    while (size > 0)
    {
        size_t chunk = (size > sizeof(buf)) ? sizeof(buf) : size;

        /* mflash demands size to be in multiples of 4 */
        size_t chunkAlign4 = (chunk + 3) & (~3);

        status = mflash_drv_read(offset, buf, chunkAlign4);
        if (status != kStatus_Success)
        {
            return status;
        }

        for (int i = 0; i < ARRAY_SIZE(buf); i++)
        {
            if (buf[i] != 0xffffffff)
            {
                return kStatus_Fail;
            }
        }

        size -= chunk;
        offset += chunk;
    }

    return kStatus_Success;
}

int flash_erase(uint32_t offset, size_t size)
{
    uint32_t endaddr = offset + size;
    status_t status = kStatus_Success;

    PRINTF("Erasing %u sectors of flash from offset 0x%x ", (size + MFLASH_SECTOR_SIZE - 1) / MFLASH_SECTOR_SIZE,
           offset);

    /* All sectors are of the same size */

    for (; offset < endaddr; offset += MFLASH_SECTOR_SIZE)
    {
        /* Test erasing is needed to save time */
        status = flash_verify_erase(offset, MFLASH_SECTOR_SIZE);
        if (status == kStatus_Success)
        {
            PUTCHAR('.');
            continue;
        }

        status = mflash_drv_sector_erase(offset);
        if (status != kStatus_Success)
            break;

        PUTCHAR('x');
    }

    PUTCHAR('\n');

    return status;
}

int flash_program(uint32_t offset, void *data, size_t size)
{
    /* mflash expects 4B aligned buffer */
    uint32_t pagebuf[MFLASH_PAGE_SIZE / sizeof(uint32_t)];
    uint8_t *pagebuf8 = (uint8_t *)pagebuf;
    uint8_t *data8    = data;
    status_t status;

    while (size > 0)
    {
        size_t chunk = (size > sizeof(pagebuf)) ? sizeof(pagebuf) : size;

        memcpy(pagebuf, data8, chunk);

        /* In case of non-page-aligned size pad with 0xff */
        if (chunk < sizeof(pagebuf))
        {
            memset(pagebuf8 + chunk, 0xff, sizeof(pagebuf) - chunk);
        }

        status = mflash_drv_page_program(offset, pagebuf);
        if (status != kStatus_Success)
        {
            return status;
        }

        offset += chunk;
        data8 += chunk;
        size -= chunk;
    }

    return kStatus_Success;
}

int flash_sha256(uint32_t offset, size_t size, uint8_t sha256[32])
{
    uint32_t buf[128 / sizeof(uint32_t)];
    status_t status;
    mbedtls_sha256_context sha256ctx;

    mbedtls_sha256_init(&sha256ctx);
    mbedtls_sha256_starts_ret(&sha256ctx, 0);

    while (size > 0)
    {
        size_t chunk = (size > sizeof(buf)) ? sizeof(buf) : size;
        /* mflash demands size to be in multiples of 4 */
        size_t chunkAlign4 = (chunk + 3) & (~3);

        status = mflash_drv_read(offset, buf, chunkAlign4);
        if (status != kStatus_Success)
        {
            return status;
        }

        mbedtls_sha256_update(&sha256ctx, (unsigned char *)buf, chunk);

        size -= chunk;
        offset += chunk;
    }

    mbedtls_sha256_finish(&sha256ctx, sha256);

    return kStatus_Success;
}
