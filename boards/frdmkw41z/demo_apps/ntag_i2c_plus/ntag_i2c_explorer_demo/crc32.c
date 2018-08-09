#include <stdio.h>
#include <stdint.h>

#include "crc32.h"

static uint32_t crc32_table[256];    /* CRCs of all 8-bit messages.    */
static int crc32_table_computed = 0; /* Flag: Has table been computed? */

/* Make the table: */

static void make_crc32_table(void)
{
    uint32_t c;
    int i, k;

    for (i = 0; i < 256; ++i)
    {
        c = (uint32_t)i;

        for (k = 0; k < 8; ++k)
        {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c >>= 1;
        }

        crc32_table[i] = c;
    }

    crc32_table_computed = 1;
}

uint32_t crc32(const uint32_t seed, const unsigned char *bytes, size_t n)
{
    return crc32_update(seed, bytes, n);
}

uint32_t crc32_update(uint32_t crc, const unsigned char *bytes, uint16_t n)
{
    uint32_t c = crc;
    int i;

    if (!crc32_table_computed)
        make_crc32_table();

    for (i = 0; i < n; i++)
    {
        c = crc32_table[(c ^ bytes[i]) & 0xff] ^ (c >> 8);
    }

    return c;
}
