/*
 * Copyright (c) 2019-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "common.h"

#include "mbedtls/platform.h"

#include "psa_crypto_its.h"

#include <string.h>

#ifndef MBEDTLS_PSA_ITS_RAM_MAX_NUM_OBJECT
#define MBEDTLS_PSA_ITS_RAM_MAX_NUM_OBJECT 100
#endif

// the structure includes the data used to identify a single object stored
// in the mocked internal trusted storage
typedef struct psa_its_storage_s
{
    // unique identifier of the object
    psa_storage_uid_t uid;
    // includes the size of the data and the flags
    struct psa_storage_info_t info;
    // the pointer to the memory allocated for the data
    uint8_t *p_data;
} psa_its_storage_t;

// array of the structures including the objects stored in the internal trusted storage
static psa_its_storage_t internal_storage_array[MBEDTLS_PSA_ITS_RAM_MAX_NUM_OBJECT] = {{0}};

static int find_uid_in_memory(psa_storage_uid_t uid, size_t *index)
{
    for (size_t i = 0; i < MBEDTLS_PSA_ITS_RAM_MAX_NUM_OBJECT; i++)
    {
        if (internal_storage_array[i].uid == uid)
        {
            *index = i;
            return 1;
        }
    }

    return 0;
}

static void free_cell(size_t index)
{
    mbedtls_free(internal_storage_array[index].p_data);

    memset(&internal_storage_array[index], 0, sizeof(psa_its_storage_t));
}

psa_status_t psa_its_set(psa_storage_uid_t uid,
                         uint32_t data_length,
                         const void *p_data,
                         psa_storage_create_flags_t create_flags)
{
    size_t index_id       = 0;
    uint8_t *p_data_local = NULL;

    // check if the UID is already stored in the memory
    // in case it is in memory, update the slot id, otherwise choose the first free slot
    if (!find_uid_in_memory(uid, &index_id))
    {
        // uid not in memory, find first free position
        // in case the no slot free, return error
        if (!find_uid_in_memory(0, &index_id))
        {
            return PSA_ERROR_INSUFFICIENT_STORAGE;
        }
    }
    else
    {
        // uid non in memory, free the location
        free_cell(index_id);
    }

    // at this point index_id is including the value of the selected slot
    // allocate the memory needed for the data and copy the input value
    p_data_local = (uint8_t *)mbedtls_calloc(1, data_length);

    if (p_data_local == NULL)
    {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    memcpy(p_data_local, p_data, data_length);

    // fill the cell
    internal_storage_array[index_id].uid        = uid;
    internal_storage_array[index_id].info.size  = data_length;
    internal_storage_array[index_id].info.flags = create_flags;
    internal_storage_array[index_id].p_data     = p_data_local;

    return PSA_SUCCESS;
}

psa_status_t psa_its_get(psa_storage_uid_t uid,
                         uint32_t data_offset,
                         uint32_t data_length,
                         void *p_data,
                         size_t *p_data_length)
{
    size_t index_id      = 0;
    uint32_t temp_length = 0;

    // check if the UID is in memory, otherwise return an error
    if (!find_uid_in_memory(uid, &index_id))
    {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    // define the length
    temp_length    = internal_storage_array[index_id].info.size - data_offset;
    *p_data_length = (temp_length < data_length) ? temp_length : data_length;

    memcpy(p_data, internal_storage_array[index_id].p_data + data_offset, *p_data_length);

    return PSA_SUCCESS;
}

psa_status_t psa_its_get_info(psa_storage_uid_t uid,
                              struct psa_storage_info_t *p_info)
{
    size_t index_id = 0;

    // check if the UID is in memory, otherwise return an error
    if (!find_uid_in_memory(uid, &index_id))
    {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    memcpy(p_info, &internal_storage_array[index_id].info, sizeof(struct psa_storage_info_t));

    return PSA_SUCCESS;
}

psa_status_t psa_its_remove(psa_storage_uid_t uid)
{
    size_t index_id = 0;

    // check if the UID is in memory, otherwise return an error
    if (!find_uid_in_memory(uid, &index_id))
    {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    // remove UID from memory
    free_cell(index_id);

    return PSA_SUCCESS;
}
