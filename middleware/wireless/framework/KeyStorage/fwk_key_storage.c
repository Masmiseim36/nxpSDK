/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "fsl_common.h"
#include "fwk_file_cache.h"
#include "fwk_key_storage.h"
#include "fsl_os_abstraction.h"
#include "FreeRTOS.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define KS_SIZE_TMP_KEY    15
#define KS_MAX_CHAR_NUMBER 15

/* Indication of the type of the key */
#define FILE_INT 0
#define FILE_STR 1
/* Collects the possible third characters of the string keys starting with 'f' */
static char key_third_char[KS_MAX_CHAR_NUMBER];
/*******************************************************************************
 * Type definition
 ******************************************************************************/

typedef struct ks_handle_s
{
    void *fc_context_p;
    void *ks_config_p;
} ks_handle_t;

/* Keeps the max between the number of the fabrics
 * It will be used in the KS_Reset() function to identify all the files related to the string keys that start with 'f'
 * and contain the number of the fabric */
static uint16_t num_fabrics = 0;

/************************************************************************************
 * Private functions
 ************************************************************************************/

static void KS_AddKeyToBuffer(uint8_t *buf, uint16_t *buf_len, uint16_t req_size, int key, void *value, int val_sz)
{
    int *    key_place;
    uint8_t *value_place;
    uint8_t *value_l;
    uint16_t prev_buf_len;

    value_l      = (uint8_t *)value;
    prev_buf_len = *buf_len;

    /* Leave space for the offset value to the next key */
    key_place   = (int *)(&buf[prev_buf_len] + 4);
    *key_place  = key;
    value_place = (uint8_t *)(key_place + 1);

    memcpy(value_place, value_l, val_sz);
    /* Set the value of the offset */
    /* req_size is on 2 bytes */
    buf[prev_buf_len]     = req_size & 0xff;
    buf[prev_buf_len + 1] = (req_size >> 8);
    buf[prev_buf_len + 2] = 0;
    buf[prev_buf_len + 3] = 0;

    /* Update the length of useful data in the buffer
       new length = old length + required size for the new value */
    *buf_len = prev_buf_len + req_size;
}

static void *KS_SearchKeyIntoBuffer(uint8_t *buf, uint16_t data_len, int key, int *val_sz, int *buf_off)
{
    void *ret;
    int * key_place;
    int   offset;
    int   off_value;

    ret    = NULL;
    offset = 0;

    while (offset < data_len)
    {
        /* Value of the current offset on 4 bytes */
        off_value = (buf[offset] | (buf[offset + 1] << 8) | (buf[offset + 2] << 16) | (buf[offset + 3] << 24));
        /* Position of the key in the buffer:
           Offset from the beginning of the buffer + size of the offset for the current key */
        key_place = (int *)(buf + offset + sizeof(int));
        /* Assuming no equal keys present in the buffer for the moment */
        if (*key_place == key)
        {
            /* Value location in the buffer is just after the key */
            *val_sz = off_value - 2 * sizeof(int);
            /* Offset from the beginning of the buffer to easily localizate the found value in the buffer:
               current offset + size of the offset for the current key (int) + size of the key (int) */
            *buf_off = offset + 2 * sizeof(int);
            ret      = key_place + 1;
            break;
        }
        assert(off_value != 0);
        offset += off_value;
    }
    /* If the key is not found, the return will be a NULL pointer */
    return ret;
}

static void KS_AddStringKeyToBuffer(
    uint8_t *buf, uint16_t *buf_len, uint16_t req_size, char *key, int key_len, void *value, int val_sz)
{
    char *   key_place;
    uint8_t *value_place;
    uint8_t *value_l;
    uint16_t prev_buf_len;

    value_l      = (uint8_t *)value;
    prev_buf_len = *buf_len;

    /* Leave space for the offset value to the next key */
    key_place = (char *)(&buf[prev_buf_len] + 4);
    memcpy(key_place, key, key_len);

    value_place = (uint8_t *)(key_place + key_len);
    memcpy(value_place, value_l, val_sz);
    /* Set the value of the offset */
    /* req_size is on 2 bytes */
    buf[prev_buf_len]     = req_size & 0xff;
    buf[prev_buf_len + 1] = (req_size >> 8);
    buf[prev_buf_len + 2] = 0;
    buf[prev_buf_len + 3] = 0;

    /* Update the length of useful data in the buffer
        new length = old length + required size for the new value */
    *buf_len = prev_buf_len + req_size;
}

static void *KS_SearchStringKeyIntoBuffer(
    uint8_t *buf, uint16_t data_len, char *key_buf, int key_len, int *val_sz, int *buf_off)
{
    void *ret;
    char *key_place;
    int   offset;
    int   off_value;

    ret     = NULL;
    offset  = 0;
    *val_sz = 0;

    while (offset < data_len)
    {
        /* Value of the current offset on 4 bytes */
        off_value = (buf[offset] | (buf[offset + 1] << 8) | (buf[offset + 2] << 16) | (buf[offset + 3] << 24));
        /* Position of the key in the buffer:
           Offset from the beginning of the buffer + size of the offset for the current key */
        key_place = (char *)(buf + offset + sizeof(int));
        /* Assuming no equal keys present in the buffer for the moment */
        if (memcmp(key_place, key_buf, key_len) == 0)
        {
            /* Value location in the buffer is just after the key */
            *val_sz = off_value - sizeof(int) - key_len;
            /* Offset from the beginning of the buffer to easily localizate the found value in the buffer:
               current offset + size of the offset for the current key (int) + length of the key  */
            *buf_off = offset + sizeof(int) + key_len;
            ret      = key_place + key_len;
            break;
        }
        offset += off_value;
    }
    /* If the key is not found, the return will be a NULL pointer */
    return ret;
}

/**
 * \brief Shift left or right the content of the buffer
 *
 * If the size of the new value is greater than the old one, a right shift is needed to leave the space for the new
 * value. If the size of the new value is less than the old one, a left shift is needed to avoid to leave unused space
 * in the buffer after the insertion of the new value.
 *
 * \param buf [in/out] buffer to be shifted
 * \param buf_len [in] length of the buffer
 * \param old_sz [in] size of the old value that will be replaced
 * \param new_sz [in] size of the new value that will be set
 * \param buf_off [in] offset inside the buffer which indicates the position of the old value
 * \return void
 */
static void KS_BufferShift(uint8_t *buf, uint16_t *buf_len, int key_sz, int old_sz, int new_sz, int buf_off)
{
    int      gap;
    int      cur_off;
    uint8_t *source;
    uint8_t *dest;
    uint16_t n_bytes;

    gap = new_sz - old_sz;
    /* Index in the buffer of the value offset to update */
    cur_off  = buf_off - sizeof(int) - key_sz;
    *buf_len = *buf_len + gap;
    /* Update the value offset */
    buf[cur_off] = buf[cur_off] + gap;
    /* If the gap is positive, a right shift is needed before insert the new value in the buffer */
    /* If the gap is negative, a left shift needed to compact the buffer (gap will be negative) */
    n_bytes = *buf_len - buf_off - new_sz;
    source  = buf + buf_off + old_sz;
    /* If the new size is 0, a cancellation has been requested */
    if (new_sz == 0)
    {
        /* Delete offset, key and value */
        dest = buf + buf_off - sizeof(int) - key_sz;
        /* Update size of data in the buffer */
        *buf_len = *buf_len - sizeof(int) - key_sz;
    }
    else
    {
        dest = buf + buf_off + new_sz;
    }
    memmove(dest, source, n_bytes);
}

static void KS_GetFileName(void *ks_handle_p, char *suffix, char *file_name, uint8_t file_type, char *key_string)
{
    ks_handle_t *ks_hdl_p;
    ks_config_t *ks_cfg_p;
    char *       prefix;

    ks_hdl_p = (ks_handle_t *)ks_handle_p;
    ks_cfg_p = (ks_config_t *)ks_hdl_p->ks_config_p;
    prefix   = (char *)ks_cfg_p->KS_name;
    strcpy(file_name, prefix);
    strcat(file_name, suffix);

    /* DIVISION OF FILES:
     * - One file for the integer keys;
     * - One file for the string keys NOT starting with 'f' ('g', 'w');
     * - As many files, for the string keys starting with 'f', as the combination of the number of fabrics the device
     * is commissioned to and the possible third characters the string keys starting with 'f' have.
     * The key starting with 'f' is in the format 'f/{fabric_number}/{third_char}/{other}'.
     * We want to separate them based on the fabric number and also the third character, since they are the ones that
     * take more memory space. In this way, for every ks configuration, we will have 2 files + 1 for each combination
     * of fabric number and third char of the string key starting with 'f'.
     * To make the division, the key is needed as parameter of the function.
     */
    if (file_type == FILE_STR && key_string[0] == 'f')
    {
        const char div_char = '/';
        char *     lim1;
        char *     lim2;
        char *     fabric_extracted;
        uint16_t   fabric_number;
        uint8_t    i;
        uint8_t    len;

        assert(key_string != NULL);
        /* Extract the fabric number from the key string */
        lim1             = strchr(key_string, div_char);
        lim2             = strchr(lim1 + 1, div_char);
        fabric_extracted = strndup(lim1 + 1, (lim2 - lim1 - 1));
        strcat(file_name, "_");
        strcat(file_name, fabric_extracted);
        fabric_number = (uint16_t)atoi(fabric_extracted);
        if (fabric_number > num_fabrics)
        {
            num_fabrics = fabric_number;
        }
        /* Add the third character of the string in the name of the file */
        strncat(file_name, &key_string[4], 1);
        /* Every time a string key starting with 'f' is added, the third character is saved in a global array (if not
         * already present). In this way we can trace dynamycally the third character that string keys starting with 'f'
         * possibly have. Whereby, when the KS_Reset() is called, it is possible to reconstruct the name of the
         * different files belonging to a given KS configuration using the configuration name in combination with the
         * number of the fabrics and the characters stored in the array */
        len = strlen(key_third_char);
        assert(len != KS_MAX_CHAR_NUMBER);
        if (len == 0)
        {
            key_third_char[len] = key_string[4];
        }
        else
        {
            for (i = 0; i < len; i++)
            {
                if (key_third_char[i] == key_string[4])
                {
                    break;
                }
                if (i == (len - 1))
                {
                    key_third_char[len] = key_string[4];
                }
            }
        }
    }
    assert(strlen(file_name) <= FC_FILE_NAME_SIZE_MAX);
}

static void *KS_OpenWaitLoop(void *fc_ctx_p, char *file_name, uint8_t **buf, uint16_t buf_len, uint16_t new_value_space)
{
    int      i;
    uint16_t req_buf_size;
    void *   file_hdl = NULL;

    assert(fc_ctx_p != NULL);
    assert(file_name != NULL);

    i = 0;

    /* Add to the current size, the size of the new value */
    /* Try to allocate more space than needed to have the chance to allocate an other key later */
    req_buf_size = buf_len + (new_value_space * 2);
    /* Check if there is enough space to open the buffer with the updated size */
    file_hdl = FC_Open(fc_ctx_p, file_name, buf, &req_buf_size, 0);
#if (KS_OPEN_WAIT_LOOP_MAX_ITERATIONS_N != 0)
    if ((file_hdl == NULL) && (KS_OPEN_WAIT_LOOP_MAX_ITERATIONS_N != 0) && (KS_OPEN_WAIT_LOOP_MS != 0))
    {
        /* No memory left or buffer in write list too small */
        do
        {
            /* Wait to give time to write in flash and leave space in scratch RAM to open a new buffer */
            /* Every time in the FC_Open req_buf_size is updated with the available size */
            req_buf_size = buf_len + new_value_space;
            /* Delay the task to have the time to write in flash and leave space
            Try to open the buffer until it succeed or reach the max number of iterations */
            OSA_TimeDelay(KS_OPEN_WAIT_LOOP_MS);
            file_hdl = FC_Open(fc_ctx_p, file_name, buf, &req_buf_size, 0);
            i++;
        } while ((file_hdl == NULL) && (i < KS_OPEN_WAIT_LOOP_MAX_ITERATIONS_N));
    }
#endif
    return file_hdl;
}

static ks_error_t KS_DeleteKey(void *ks_handle_p, void *key_p, int key_len, char *key_namespace, uint8_t file_type)
{
    ks_error_t   ret;
    int          buf_off;
    int          rm_val_sz;
    int          close_file;
    void *       fc_ctx_p;
    void *       file_hdl;
    void *       value_l;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     buf_len;
    ks_handle_t *ks_hdl_p;

    ret        = KS_ERROR_KEY_NOT_FOUND;
    buf        = NULL;
    close_file = FC_CLOSE_ONLY;
    ks_hdl_p   = (ks_handle_t *)ks_handle_p;
    fc_ctx_p   = ks_hdl_p->fc_context_p;

    /* Open file, search for the key in the file, close file */
    KS_GetFileName(ks_handle_p, key_namespace, file_name, file_type, (char *)key_p);
    FC_GetFileSize(file_name, &buf_len);

    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, 0);
    if (file_hdl == NULL)
    {
        /* There is not enough space to open the buffer */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        /* TO DO: identify files of the configuration through the context and go through the buffers in the ram section
         */
        /* Recognize the type of key based on file_type (file for integer or string keys) */
        if (file_type == FILE_INT)
        {
            /* The key is an integer */
            int *key;
            key     = (int *)key_p;
            value_l = KS_SearchKeyIntoBuffer(buf, buf_len, *key, &rm_val_sz, &buf_off);
            while (value_l != NULL)
            {
                int next_position;
                int data_len;
                KS_BufferShift(buf, &buf_len, key_len, rm_val_sz, 0, buf_off);
                ret        = KS_ERROR_NONE;
                close_file = FC_CLOSE_AND_SAVE_TO_QUEUE;
                if (buf_len > 0)
                {
                    /* Next position from which start to search for the key in the buffer */
                    next_position = buf_off - key_len - sizeof(int);
                    data_len      = buf_len - next_position;
                    value_l       = KS_SearchKeyIntoBuffer(buf + next_position, data_len, *key, &rm_val_sz, &buf_off);
                    buf_off += next_position;
                }
                else
                {
                    /* The buffer is empty */
                    break;
                }
            }
        }
        else
        {
            /* The key is a string */
            char *key_buf;
            key_buf = (char *)key_p;
            value_l = KS_SearchStringKeyIntoBuffer(buf, buf_len, key_buf, key_len, &rm_val_sz, &buf_off);
            while (value_l != NULL)
            {
                int next_position;
                int data_len;
                KS_BufferShift(buf, &buf_len, key_len, rm_val_sz, 0, buf_off);
                ret        = KS_ERROR_NONE;
                close_file = FC_CLOSE_AND_SAVE_TO_QUEUE;
                if (buf_len > 0)
                {
                    /* Next position from which start to search for the key in the buffer */
                    next_position = buf_off - key_len - sizeof(int);
                    data_len      = buf_len - next_position;
                    value_l = KS_SearchStringKeyIntoBuffer(buf + next_position, data_len, key_buf, key_len, &rm_val_sz,
                                                           &buf_off);
                    buf_off += next_position;
                }
                else
                {
                    /* The buffer is empty */
                    break;
                }
            }
        }
        FC_Close(file_hdl, buf_len, close_file);
    }

    return ret;
}

/************************************************************************************
 * Public functions
 ************************************************************************************/

void *KS_Init(ks_config_t *ks_config_p)
{
    ks_handle_t *ks_ctx_p;
    fc_config_t *fc_config_p;
    void *       ret;
    uint8_t *    memory_p;

    ret = NULL;
    assert(ks_config_p != NULL);
    /* Pointer to a pre-allocated memory buffer */
    memory_p = (uint8_t *)(ks_config_p->mem_p);

    if (memory_p == NULL)
    {
        ks_ctx_p = (ks_handle_t *)pvPortMalloc(sizeof(ks_handle_t) + FC_CONTEXT_SIZE + ks_config_p->size);
    }
    else
    {
        ks_ctx_p          = (ks_handle_t *)memory_p;
        ks_config_p->size = ks_config_p->size - sizeof(ks_handle_t) - FC_CONTEXT_SIZE;
    }

    if (ks_ctx_p != NULL)
    {
        fc_config_p                 = (fc_config_t *)(ks_ctx_p + 1);
        fc_config_p->folder_name    = ks_config_p->KS_name;
        fc_config_p->scracth_ram_p  = (void *)(fc_config_p + 1);
        fc_config_p->scracth_ram_sz = ks_config_p->size;

        ret = FC_InitConfig(fc_config_p);
        if (ret != NULL)
        {
            ks_ctx_p->fc_context_p = ret;
            ks_ctx_p->ks_config_p  = ks_config_p;
            ret                    = ks_ctx_p;
        }
    }

    return ret;
}

int KS_Reset(void *ks_handle_p)
{
    int          ret;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    char         key[KS_SIZE_TMP_KEY];
    ks_handle_t *ks_hdl_p;
    uint8_t      i;
    uint8_t      j;

    assert(ks_handle_p != NULL);
    ks_hdl_p = (ks_handle_t *)ks_handle_p;
    i        = 1;
    j        = 0;

    /* I can check the buffers in the ram section with the context, but I can't check the buffers already written in
     * flash if I don't know the complete name of the file */

    /* No division for integer keys: Delete one file */
    KS_GetFileName(ks_hdl_p, "_fki", file_name, FILE_INT, NULL);
    ret = FC_Delete(file_name);
    /* No division for string keys NOT starting with 'f': Delete one file */
    KS_GetFileName(ks_hdl_p, "_fks", file_name, FILE_STR, NULL);
    ret += FC_Delete(file_name);
    /* String keys starting with 'f' divided based on the number of fabirc: Delete as many files as the number of
     * fabrics */
    while (i <= num_fabrics)
    {
        while (j < strlen(key_third_char))
        {
            sprintf(key, "f/%d/%c", i, key_third_char[j]);
            KS_GetFileName(ks_hdl_p, "_fks", file_name, FILE_STR, key);
            j++;
            ret += FC_Delete(file_name);
        }
        j = 0;
        i++;
    }
    if (ret != 0)
    {
        ret = -1;
    }

    return ret;
}

int KS_DeInit(void *ks_handle_p)
{
    int          ret;
    ks_handle_t *ks_hdl_p;

    assert(ks_handle_p != NULL);

    ks_hdl_p = (ks_handle_t *)ks_handle_p;
    ret      = FC_DeInit(ks_hdl_p->fc_context_p);
    vPortFree(ks_hdl_p);
    return ret;
}

ks_error_t KS_AddKeyInt(void *ks_handle_p, int key, char *key_namespace, void *value, int val_sz)
{
    ks_error_t   ret;
    void *       fc_ctx_p;
    void *       file_hdl;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     buf_len;
    uint16_t     new_value_space;
    ks_handle_t *ks_hdl_p;

    ret      = KS_ERROR_NONE;
    buf      = NULL;
    ks_hdl_p = (ks_handle_t *)ks_handle_p;

    assert(ks_handle_p != NULL);
    fc_ctx_p = ks_hdl_p->fc_context_p;

    KS_GetFileName(ks_handle_p, key_namespace, file_name, FILE_INT, NULL);
    FC_GetFileSize(file_name, &buf_len);
    /* Space required to store the new value, offset and key */
    new_value_space = 2 * sizeof(int) + val_sz;

    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, new_value_space);
    if (file_hdl == NULL)
    {
        /* If even after waiting the buffer hasn't been opened, there is no space to store the new key */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        KS_AddKeyToBuffer(buf, &buf_len, new_value_space, key, value, val_sz);
        FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
    }

    return ret;
}

ks_error_t KS_GetKeyInt(void *ks_handle_p, int key, char *key_namespace, void *value_p, int buf_sz, int *val_sz)
{
    ks_error_t   ret;
    int          buf_off;
    void *       fc_ctx_p;
    void *       file_hdl;
    void *       value_l;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     buf_len;
    ks_handle_t *ks_hdl_p;

    /* If the key is not found return an error status */
    ret      = KS_ERROR_KEY_NOT_FOUND;
    buf      = NULL;
    ks_hdl_p = (ks_handle_t *)ks_handle_p;

    assert(ks_handle_p != NULL);
    fc_ctx_p = ks_hdl_p->fc_context_p;
    KS_GetFileName(ks_handle_p, key_namespace, file_name, FILE_INT, NULL);
    FC_GetFileSize(file_name, &buf_len);

    /* Open file, search for the key in the file, close file */
    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, 0);
    if (file_hdl == NULL)
    {
        /* There is not enough space to open the buffer */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        value_l = KS_SearchKeyIntoBuffer(buf, buf_len, key, val_sz, &buf_off);
        /* The length of the read value should not be greater than the size of the buffer */
        if (*val_sz > buf_sz)
        {
            *val_sz = buf_sz;
        }
        if (value_l != NULL)
        {
            ret = KS_ERROR_NONE;
            memcpy(value_p, value_l, *val_sz);
        }
        FC_Close(file_hdl, 0, FC_CLOSE_ONLY);
    }

    return ret;
}

ks_error_t KS_AddKeyString(void *ks_handle_p, char *key_buf, int key_len, char *key_namespace, void *value, int val_sz)
{
    ks_error_t   ret;
    void *       fc_ctx_p;
    void *       file_hdl;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     buf_len;
    uint16_t     new_value_space;
    ks_handle_t *ks_hdl_p;

    ret      = KS_ERROR_NONE;
    buf      = NULL;
    ks_hdl_p = (ks_handle_t *)ks_handle_p;

    assert(ks_handle_p != NULL);
    fc_ctx_p = ks_hdl_p->fc_context_p;

    KS_GetFileName(ks_handle_p, key_namespace, file_name, FILE_STR, key_buf);
    FC_GetFileSize(file_name, &buf_len);
    /* Space required to store the new value, offset and key */
    new_value_space = sizeof(int) + key_len + val_sz;

    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, new_value_space);
    if (file_hdl == NULL)
    {
        /* If even after waiting the buffer hasn't been opened, there is no space to store the new key */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        KS_AddStringKeyToBuffer(buf, &buf_len, new_value_space, key_buf, key_len, value, val_sz);
        FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
    }

    return ret;
}

ks_error_t KS_GetKeyString(
    void *ks_handle_p, char *key_buf, int key_len, char *key_namespace, void *value_p, int buf_sz, int *val_sz)
{
    ks_error_t   ret;
    int          buf_off;
    void *       fc_ctx_p;
    void *       file_hdl;
    void *       value_l;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     buf_len;
    ks_handle_t *ks_hdl_p;

    /* If the key is not found return an error status */
    ret      = KS_ERROR_KEY_NOT_FOUND;
    buf      = NULL;
    ks_hdl_p = (ks_handle_t *)ks_handle_p;

    assert(ks_handle_p != NULL);
    fc_ctx_p = ks_hdl_p->fc_context_p;
    KS_GetFileName(ks_handle_p, key_namespace, file_name, FILE_STR, key_buf);
    FC_GetFileSize(file_name, &buf_len);

    /* Open file, search for the key in the file, close file */
    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, 0);

    if (file_hdl == NULL)
    {
        /* There is not enough space to open the buffer */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        value_l = KS_SearchStringKeyIntoBuffer(buf, buf_len, key_buf, key_len, val_sz, &buf_off);
        /* The length of the read value should not be greater than the size of the buffer */
        if (*val_sz > buf_sz)
        {
            *val_sz = buf_sz;
        }
        if (value_l != NULL)
        {
            ret = KS_ERROR_NONE;
            memcpy(value_p, value_l, *val_sz);
        }
        FC_Close(file_hdl, 0, FC_CLOSE_ONLY);
    }

    return ret;
}

ks_error_t KS_SetKeyInt(void *ks_handle_p, int key, char *key_namespace, void *value_p, int val_sz)
{
    ks_error_t   ret;
    int          buf_off;
    int          key_sz;
    int          old_val_sz;
    void *       fc_ctx_p;
    void *       file_hdl;
    void *       value_l;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     new_buf_size;
    uint16_t     old_buf_size;
    uint16_t     buf_len;
    uint16_t     new_value_space;
    uint8_t *    new_value_p;
    ks_handle_t *ks_hdl_p;

    ret         = KS_ERROR_BUF_TOO_SMALL;
    buf         = NULL;
    key_sz      = sizeof(int);
    new_value_p = (uint8_t *)value_p;
    ks_hdl_p    = (ks_handle_t *)ks_handle_p;

    assert(ks_handle_p != NULL);
    fc_ctx_p = ks_hdl_p->fc_context_p;

    KS_GetFileName(ks_handle_p, key_namespace, file_name, FILE_INT, NULL);
    FC_GetFileSize(file_name, &buf_len);
    old_buf_size = buf_len;

    /* Open a first time the file with the effective size */
    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, 0);
    if (file_hdl == NULL)
    {
        /* There is not enough space to open the buffer */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        /* buf_off is an offset from the beginning of the buffer to localizate the value to replace */
        value_l = KS_SearchKeyIntoBuffer(buf, buf_len, key, &old_val_sz, &buf_off);
        /* Delete all the entries associated to the key to set */
        while (value_l != NULL)
        {
            KS_BufferShift(buf, &buf_len, key_sz, old_val_sz, 0, buf_off);
            value_l = KS_SearchKeyIntoBuffer(buf, buf_len, key, &old_val_sz, &buf_off);
        }
        /* Space required to store the new value, offset and key */
        new_value_space = 2 * sizeof(int) + val_sz;
        new_buf_size    = buf_len + new_value_space;
        /* If the updated size of the buffer is less greater or equal than the old one, we're sure that there is enough
         * space */
        if (new_buf_size <= old_buf_size)
        {
            /* Add new key at the end of the buffer */
            KS_AddKeyToBuffer(buf, &buf_len, new_value_space, key, new_value_p, val_sz);
            FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
            ret = KS_ERROR_NONE;
        }
        else
        {
            /* We need to open a larger buffer */
            FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
            /* Try to open the file with a bigger size */
            file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, new_value_space);
            if (file_hdl == NULL)
            {
                /* If even after waiting the buffer hasn't been opened, there is no space to store the new key */
                ret = KS_ERROR_BUF_TOO_SMALL;
            }
            else
            {
                KS_AddKeyToBuffer(buf, &buf_len, new_value_space, key, new_value_p, val_sz);
                FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
                ret = KS_ERROR_NONE;
            }
        }
    }
    return ret;
}

ks_error_t KS_DeleteKeyInt(void *ks_handle_p, int key, char *key_namespace)
{
    ks_error_t ret;

    assert(ks_handle_p != NULL);
    ret = KS_DeleteKey(ks_handle_p, (void *)&key, sizeof(int), key_namespace, FILE_INT);

    return ret;
}

ks_error_t KS_SetKeyString(
    void *ks_handle_p, char *key_buf, int key_len, char *key_namespace, void *value_p, int val_sz)
{
    ks_error_t   ret;
    int          buf_off;
    int          old_val_sz;
    void *       fc_ctx_p;
    void *       file_hdl;
    void *       value_l;
    char         file_name[FC_FILE_NAME_SIZE_MAX];
    uint8_t *    buf;
    uint16_t     new_buf_size;
    uint16_t     old_buf_size;
    uint16_t     buf_len;
    uint8_t *    new_value_p;
    uint16_t     new_value_space;
    ks_handle_t *ks_hdl_p;

    ret         = KS_ERROR_BUF_TOO_SMALL;
    buf         = NULL;
    new_value_p = (uint8_t *)value_p;
    ks_hdl_p    = (ks_handle_t *)ks_handle_p;

    assert(ks_handle_p != NULL);
    fc_ctx_p = ks_hdl_p->fc_context_p;

    KS_GetFileName(ks_handle_p, key_namespace, file_name, FILE_STR, key_buf);
    FC_GetFileSize(file_name, &buf_len);
    old_buf_size = buf_len;

    /* Open a first time the file with the effective size */
    /* Try to open the buffer and wait if the opening fails */
    file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, 0);
    if (file_hdl == NULL)
    {
        /* There is not enough space to open the buffer */
        ret = KS_ERROR_BUF_TOO_SMALL;
    }
    else
    {
        /* buf_off is an offset from the beginning of the buffer to localizate the value to replace */
        value_l = KS_SearchStringKeyIntoBuffer(buf, buf_len, key_buf, key_len, &old_val_sz, &buf_off);
        /* Delete all the entries associated to the key to set */
        while (value_l != NULL)
        {
            KS_BufferShift(buf, &buf_len, key_len, old_val_sz, 0, buf_off);
            value_l = KS_SearchStringKeyIntoBuffer(buf, buf_len, key_buf, key_len, &old_val_sz, &buf_off);
        }
        /* Space required to store the new value, offset and key */
        new_value_space = val_sz + sizeof(int) + key_len;
        new_buf_size    = buf_len + new_value_space;
        if (new_buf_size <= old_buf_size)
        {
            /* Add new key at the end of the buffer */
            KS_AddStringKeyToBuffer(buf, &buf_len, new_value_space, key_buf, key_len, new_value_p, val_sz);
            FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
            ret = KS_ERROR_NONE;
        }
        else
        {
            FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
            /* Try to open the file with a bigger size */
            file_hdl = KS_OpenWaitLoop(fc_ctx_p, file_name, &buf, buf_len, new_value_space);
            if (file_hdl == NULL)
            {
                /* If even after waiting the buffer hasn't been opened, there is no space to store the new key */
                ret = KS_ERROR_BUF_TOO_SMALL;
            }
            else
            {
                KS_AddStringKeyToBuffer(buf, &buf_len, new_value_space, key_buf, key_len, new_value_p, val_sz);
                FC_Close(file_hdl, buf_len, FC_CLOSE_AND_SAVE_TO_QUEUE);
                ret = KS_ERROR_NONE;
            }
        }
    }
    return ret;
}

ks_error_t KS_DeleteKeyString(void *ks_handle_p, char *key_buf, int key_len, char *key_namespace)
{
    ks_error_t ret;

    assert(ks_handle_p != NULL);
    ret = KS_DeleteKey(ks_handle_p, (void *)key_buf, key_len, key_namespace, FILE_STR);

    return ret;
}