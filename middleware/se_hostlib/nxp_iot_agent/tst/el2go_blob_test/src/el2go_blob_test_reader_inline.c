/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "el2go_blob_test_reader.h"

#include <stdlib.h>
#include <string.h>

void read_blob_data(
    const uint8_t *data, size_t data_length, uint8_t **blob, size_t *blob_length, struct test_result_t *result)
{
    if (data == NULL || data_length == 0 || blob == NULL || blob_length == NULL)
    {
        TEST_FAIL_READER("Invalid input data");
        return;
    }

    *blob = malloc(data_length);
    if (*blob == NULL)
    {
        TEST_FAIL_READER("Out of memory");
        return;
    }

    memcpy(*blob, data, data_length);
    *blob_length = data_length;
}
