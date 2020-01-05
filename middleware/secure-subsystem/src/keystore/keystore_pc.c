/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Key store in PC : For testing */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <assert.h>
#include <fsl_sss_mbedtls_types.h>
#include <fsl_sss_keyid_map.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <fsl_sss_mbedtls_apis.h>

#ifdef MBEDTLS_FS_IO

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

/* File allocation table file name */
#define FAT_FILENAME "sss_fat.bin"
#define MAX_FILE_NAME_SIZE 255

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

keyStoreTable_t gKeyStoreShadow;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static void get_file_name(char *const file_name, const size_t size, const sss_mbedtls_object_t *sss_key);

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

void ks_mbedtls_fat_allocate(keyStoreTable_t **keystore_shadow)
{
    *keystore_shadow = &gKeyStoreShadow;
    ks_common_init_fat(&gKeyStoreShadow);
}

void ks_mbedtls_fat_remove(const char *szRootPath)
{
    char file_name[MAX_FILE_NAME_SIZE];
    FILE *fp = NULL;
    snprintf(file_name, sizeof(file_name), "%s/" FAT_FILENAME, szRootPath);
    fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        /* OK. File does not exist. */
    }
    else
    {
        fclose(fp);
#ifdef _WIN32
        _unlink(file_name);
#else
        unlink(file_name);
#endif
    }
}

sss_status_t ks_mbedtls_fat_update(sss_mbedtls_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Success;
    char file_name[MAX_FILE_NAME_SIZE];
    FILE *fp = NULL;
    keyStoreTable_t *keystore_shadow;
    assert(keyStore);
    keystore_shadow = keyStore->keystore_shadow;
    assert(keystore_shadow);
    snprintf(file_name, sizeof(file_name), "%s/" FAT_FILENAME, keyStore->session->szRootPath);
    fp = fopen(file_name, "ab+");
    if (fp == NULL)
    {
        printf("ERROR: Can not open '%s'\n", file_name);
        retval = kStatus_SSS_Fail;
    }
    else
    {
        fseek(fp, 0, SEEK_SET);
        fwrite(keystore_shadow, sizeof(*keystore_shadow), 1, fp);
        fclose(fp);
    }
    return retval;
}

/* Create new table */
sss_status_t ks_mbedtls_fat_new(const char *file_name, keyStoreTable_t *keystore_shadow)
{
    sss_status_t retval;
    FILE *fp = NULL;
    fp = fopen(file_name, "wb+");
    if (fp == NULL)
    {
        printf("ERROR: Can not create '%s'\n", file_name);
        retval = kStatus_SSS_Fail;
    }
    else
    {
        fwrite(keystore_shadow, sizeof(*keystore_shadow), 1, fp);
        fclose(fp);
        retval = kStatus_SSS_Success;
    }
    return kStatus_SSS_Success;
}
sss_status_t ks_mbedtls_fat_load(const char *szRootPath, keyStoreTable_t *keystore_shadow)
{
    sss_status_t retval;
    char file_name[MAX_FILE_NAME_SIZE];
    FILE *fp = NULL;
    assert(keystore_shadow);
    snprintf(file_name, sizeof(file_name), "%s/" FAT_FILENAME, szRootPath);
    fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        retval = kStatus_SSS_Fail;
    }
    else
    {
        fread(keystore_shadow, sizeof(*keystore_shadow), 1, fp);
        fclose(fp);
        retval = kStatus_SSS_Success;
    }
    return retval;
}

sss_status_t ks_mbedtls_load_key(sss_mbedtls_object_t *sss_key, keyStoreTable_t *keystore_shadow, uint32_t extKeyId)
{
    sss_status_t retval = kStatus_SSS_Fail;
    char file_name[MAX_FILE_NAME_SIZE];
    FILE *fp = NULL;
    const char *root_folder = sss_key->keyStore->session->szRootPath;
    uint32_t size = 0;
    uint32_t i;
    keyIdAndTypeIndexLookup_t *shadowEntry;

    for (i = 0; i < sss_key->keyStore->max_object_count; i++)
    {
        if (keystore_shadow->entries[i].extKeyId == extKeyId)
        {
            shadowEntry = &keystore_shadow->entries[i];
            snprintf(file_name, MAX_FILE_NAME_SIZE, "%s/sss_%08X_%08X.bin", root_folder, shadowEntry->keyType,
                     shadowEntry->extKeyId);
            retval = kStatus_SSS_Success;
            break;
        }
    }
    if (retval == kStatus_SSS_Success)
    {
        fp = fopen(file_name, "rb");
        if (fp == NULL)
        {
            printf("ERROR: Can not open '%s'\n", file_name);
            retval = kStatus_SSS_Fail;
        }
        else
        {
            uint8_t keyBuf[256];
            fseek(fp, 0, SEEK_END);
            size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            fread(keyBuf, size, 1, fp);
            fclose(fp);
            retval = sss_mbedtls_key_object_allocate(sss_key, shadowEntry->extKeyId, shadowEntry->keyType, size,
                                                     kKeyObject_Mode_Persistent);
            if (retval == kStatus_SSS_Success)
                retval = sss_mbedtls_key_store_set_key(sss_key->keyStore, sss_key, keyBuf, size * 8, NULL, 0);
        }
    }
    return retval;
}
sss_status_t ks_mbedtls_store_key(const sss_mbedtls_object_t *sss_key)
{
    sss_status_t retval;
    char file_name[MAX_FILE_NAME_SIZE];
    FILE *fp = NULL;
    get_file_name(file_name, sizeof(file_name), sss_key);
    fp = fopen(file_name, "wb+");
    if (fp == NULL)
    {
        printf("ERROR: Can not open '%s'\n", file_name);
        retval = kStatus_SSS_Fail;
    }
    else
    {
        uint8_t key_buf[256];
        int ret = 0;
        unsigned char *c = key_buf;
        memset(key_buf, 0, sizeof(key_buf));
        size_t len = 0;
        mbedtls_pk_context *pk;
        pk = (mbedtls_pk_context *)sss_key->contents;
        switch (sss_key->objectType)
        {
            case kSSS_KeyType_Certificate:
            case kSSS_KeyType_AES:
                fwrite(sss_key->contents, sss_key->contents_max_size, 1, fp);
                retval = kStatus_SSS_Success;
                break;
            case kSSS_KeyType_ECC_Pair:
            case kSSS_KeyType_ECC_Private:
                ret = mbedtls_pk_write_key_der(pk, key_buf, sizeof(key_buf));
                break;
            case kSSS_KeyType_ECC_Public:
                ret = mbedtls_pk_write_pubkey_der(pk, key_buf, sizeof(key_buf));
                break;
        }
        if (ret > 0 && retval != kStatus_SSS_Success)
        {
            c = key_buf + sizeof(key_buf) - ret;
            fwrite(c, ret, 1, fp);
            retval = kStatus_SSS_Success;
        }
        fclose(fp);
    }
    return retval;
}

sss_status_t ks_mbedtls_remove_key(const sss_mbedtls_object_t *sss_key)
{
    sss_status_t retval = kStatus_SSS_Fail;
    char file_name[MAX_FILE_NAME_SIZE];
    get_file_name(file_name, sizeof(file_name), sss_key);
    if (!remove(file_name))
    {
        retval = kStatus_SSS_Success;
    }
    return retval;
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

/* For the key sss_key, what will the file name look like */
static void get_file_name(char *const file_name, const size_t size, const sss_mbedtls_object_t *sss_key)
{
    uint32_t keyId = sss_key->keyId;
    uint32_t keyType = sss_key->objectType;
    const char *root_folder = sss_key->keyStore->session->szRootPath;
    snprintf(file_name, size, "%s/sss_%08X_%08X.bin", root_folder, keyType, keyId);
}

#endif /* MBEDTLS_FS_IO */
