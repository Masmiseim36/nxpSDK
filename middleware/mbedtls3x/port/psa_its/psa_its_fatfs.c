/*
 *  PSA ITS simulator over fatfs files.
 *  This is adopted from psa_its_file.c in lib folder, hence keeping the copyright.
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "common.h"

#include "mbedtls/platform.h"

#include "psa_crypto_its.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ff.h"

#if !defined(PSA_ITS_STORAGE_PREFIX)
#define PSA_ITS_STORAGE_PREFIX "/mbedtls_psa/"
#endif

#define PSA_ITS_STORAGE_FILENAME_PATTERN "%08x%08x"
#define PSA_ITS_STORAGE_SUFFIX ".psa_its"
#define PSA_ITS_STORAGE_FILENAME_LENGTH         \
    (sizeof(PSA_ITS_STORAGE_PREFIX) - 1 +    /*prefix without terminating 0*/ \
     16 +  /*UID (64-bit number in hex)*/                               \
     sizeof(PSA_ITS_STORAGE_SUFFIX) - 1 +    /*suffix without terminating 0*/ \
     1 /*terminating null byte*/)
#define PSA_ITS_STORAGE_TEMP \
    PSA_ITS_STORAGE_PREFIX "tempfile" PSA_ITS_STORAGE_SUFFIX

/* The maximum value of psa_storage_info_t.size */
#define PSA_ITS_MAX_SIZE 0xffffffff

#define PSA_ITS_MAGIC_STRING "PSA\0ITS\0"
#define PSA_ITS_MAGIC_LENGTH 8

/* As rename fails on Windows if the new filepath already exists,
 * use MoveFileExA with the MOVEFILE_REPLACE_EXISTING flag instead.
 * Returns 0 on success, nonzero on failure. */
#if defined(_WIN32)
#define rename_replace_existing(oldpath, newpath) \
    (!MoveFileExA(oldpath, newpath, MOVEFILE_REPLACE_EXISTING))
#else
#define rename_replace_existing(oldpath, newpath) rename(oldpath, newpath)
#endif

typedef struct {
    uint8_t magic[PSA_ITS_MAGIC_LENGTH];
    uint8_t size[sizeof(uint32_t)];
    uint8_t flags[sizeof(psa_storage_create_flags_t)];
} psa_its_file_header_t;

static void psa_its_fill_filename(psa_storage_uid_t uid, char *filename)
{
    /* Break up the UID into two 32-bit pieces so as not to rely on
     * long long support in snprintf. */
    mbedtls_snprintf(filename, PSA_ITS_STORAGE_FILENAME_LENGTH,
                     "%s" PSA_ITS_STORAGE_FILENAME_PATTERN "%s",
                     PSA_ITS_STORAGE_PREFIX,
                     (unsigned) (uid >> 32),
                     (unsigned) (uid & 0xffffffff),
                     PSA_ITS_STORAGE_SUFFIX);
}

static psa_status_t psa_its_read_file(psa_storage_uid_t uid,
                                      struct psa_storage_info_t *p_info,
                                      FIL **p_stream)
{
    char filename[PSA_ITS_STORAGE_FILENAME_LENGTH];
    FRESULT error;
    psa_its_file_header_t header;
    size_t n;

    *p_stream = malloc(sizeof(FIL));
    psa_its_fill_filename(uid, filename);
    error = f_open(*p_stream, filename, (FA_READ));
    if (error) {
        if (error != FR_EXIST) {
            free(*p_stream);
            *p_stream = NULL;
            return PSA_ERROR_DOES_NOT_EXIST;
        }
    }

    error = f_read(*p_stream, &header, sizeof(header), &n);
    if ((error) || (n != sizeof(header))) {
        return PSA_ERROR_DATA_CORRUPT;
    }

    if (memcmp(header.magic, PSA_ITS_MAGIC_STRING,
               PSA_ITS_MAGIC_LENGTH) != 0) {
        return PSA_ERROR_DATA_CORRUPT;
    }

    p_info->size = (header.size[0] |
                    header.size[1] << 8 |
                    header.size[2] << 16 |
                    header.size[3] << 24);
    p_info->flags = (header.flags[0] |
                     header.flags[1] << 8 |
                     header.flags[2] << 16 |
                     header.flags[3] << 24);
    return PSA_SUCCESS;
}

psa_status_t psa_its_get_info(psa_storage_uid_t uid,
                              struct psa_storage_info_t *p_info)
{
    psa_status_t status;
    FIL *stream = NULL;
    status = psa_its_read_file(uid, p_info, &stream);
    if (stream != NULL) {
        f_close(stream);
        free(stream);
    }
    return status;
}

psa_status_t psa_its_get(psa_storage_uid_t uid,
                         uint32_t data_offset,
                         uint32_t data_length,
                         void *p_data,
                         size_t *p_data_length)
{
    psa_status_t status;
    FIL *stream = NULL;
    size_t n;
    struct psa_storage_info_t info;
    FRESULT error;

    status = psa_its_read_file(uid, &info, &stream);
    if (status != PSA_SUCCESS) {
        goto exit;
    }
    status = PSA_ERROR_INVALID_ARGUMENT;
    if (data_offset + data_length < data_offset) {
        goto exit;
    }
#if SIZE_MAX < 0xffffffff
    if (data_offset + data_length > SIZE_MAX) {
        goto exit;
    }
#endif
    if (data_offset + data_length > info.size) {
        goto exit;
    }

    status = PSA_ERROR_STORAGE_FAILURE;
#if LONG_MAX < 0xffffffff
    while (data_offset > LONG_MAX) {
        if (f_lseek(stream, LONG_MAX) != 0) {
            goto exit;
        }
        data_offset -= LONG_MAX;
    }
#endif
    if (f_lseek(stream, f_tell(stream) + data_offset) != 0) {
        goto exit;
    }


    error = f_read(stream, p_data, data_length, &n);
    if ((error) || (n != data_length)) {
        goto exit;
    }

    status = PSA_SUCCESS;
    if (p_data_length != NULL) {
        *p_data_length = n;
    }

exit:
    if (stream != NULL) {
        f_close(stream);
        free(stream);
    }
    return status;
}

psa_status_t psa_its_set(psa_storage_uid_t uid,
                         uint32_t data_length,
                         const void *p_data,
                         psa_storage_create_flags_t create_flags)
{
    if (uid == 0) {
        return PSA_ERROR_INVALID_HANDLE;
    }

    psa_status_t status = PSA_ERROR_STORAGE_FAILURE;
    char filename[PSA_ITS_STORAGE_FILENAME_LENGTH];
    FIL stream;
    FRESULT error;
    psa_its_file_header_t header;
    size_t n;

    memcpy(header.magic, PSA_ITS_MAGIC_STRING, PSA_ITS_MAGIC_LENGTH);
    MBEDTLS_PUT_UINT32_LE(data_length, header.size, 0);
    MBEDTLS_PUT_UINT32_LE(create_flags, header.flags, 0);

    psa_its_fill_filename(uid, filename);
    error = f_open(&stream, filename, (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));

    if (error) {
        if (error != FR_EXIST) {
            return PSA_ERROR_DOES_NOT_EXIST;
        }
    }

    status = PSA_ERROR_INSUFFICIENT_STORAGE;
    error = f_write(&stream, &header, sizeof(header), &n);
    if (error || (n != sizeof(header))) {
        goto exit;
    }
    if (data_length != 0) {
        error = f_write(&stream, p_data, data_length, &n);
        if (error || n != data_length) {
            goto exit;
        }
    }
    status = PSA_SUCCESS;

exit:

    error = f_close(&stream);
    if (status == PSA_SUCCESS && error != 0) {
        status = PSA_ERROR_INSUFFICIENT_STORAGE;
    }

    return status;
}

psa_status_t psa_its_remove(psa_storage_uid_t uid)
{
    char filename[PSA_ITS_STORAGE_FILENAME_LENGTH];
    FIL stream;
    psa_its_fill_filename(uid, filename);
    FRESULT error;

    error = f_open(&stream, filename, (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error) {
        if (error != FR_EXIST) {
            return PSA_ERROR_DOES_NOT_EXIST;
        }
    }

    f_close(&stream);

    if (f_unlink(filename) != FR_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

/* Create the top level directory for NVM storage */
int sd_its_fs_initialize(void)
{
    FRESULT error;

    error = f_mkdir(_T(PSA_ITS_STORAGE_PREFIX));
    if (error) {
        if (error != FR_EXIST) {
            return -1;
        }
    }

    return 0;
}
