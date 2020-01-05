/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Common Key store implementation between keystore_a7x and keystore_pc */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <fsl_sss_keyid_map.h>
#include <stdio.h>
#include <string.h>

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

#define KEYSTORE_MAGIC (0xA71C400)
#define KEYSTORE_VERSION (0x0001)

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */
static sss_status_t isValidKeyStoreShadow(keyStoreTable_t *keystore_shadow);
/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

void ks_common_init_fat(keyStoreTable_t *keystore_shadow)
{
    memset(keystore_shadow, 0, sizeof(*keystore_shadow));
    keystore_shadow->magic = KEYSTORE_MAGIC;
    keystore_shadow->version = KEYSTORE_VERSION;
    keystore_shadow->maxEntries = KS_N_ENTIRES;
}

sss_status_t ks_common_update_fat(keyStoreTable_t *keystore_shadow,
                                  uint32_t extId,
                                  uint8_t object_type,
                                  uint16_t intIndex)
{
    sss_status_t retval = kStatus_SSS_Fail;
    uint32_t i;
    bool found_entry = FALSE;
    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;
    for (i = 0; i < keystore_shadow->maxEntries; i++)
    {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == extId)
        {
            printf("ERRROR: %s 0x%08X ENTRY already exits.\r\n", __FUNCTION__, extId);
            retval = kStatus_SSS_Fail;
            found_entry = TRUE;
            break;
        }
    }
    if (!found_entry)
    {
        retval = kStatus_SSS_Fail;
        for (i = 0; i < keystore_shadow->maxEntries; i++)
        {
            keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
            if (keyEntry->extKeyId == 0)
            {
                keyEntry->extKeyId = extId;
                keyEntry->keyIntIndex = intIndex;
                keyEntry->keyType = object_type;
                retval = kStatus_SSS_Success;
                break;
            }
        }
    }
cleanup:
    return retval;
}

sss_status_t ks_common_remove_fat(keyStoreTable_t *keystore_shadow, uint32_t extId)
{
    sss_status_t retval = kStatus_SSS_Fail;
    uint32_t i;
    bool found_entry = FALSE;
    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;

    for (i = 0; i < keystore_shadow->maxEntries; i++)
    {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == extId)
        {
            retval = kStatus_SSS_Success;
            memset(keyEntry, 0, sizeof(keyIdAndTypeIndexLookup_t));
            found_entry = TRUE;
            break;
        }
    }
    if (!found_entry)
    {
        retval = kStatus_SSS_Fail;
    }
cleanup:
    return retval;
}

sss_status_t ks_common_extId_to_int_index(keyStoreTable_t *keystore_shadow, uint32_t extId, uint16_t *intIndex)
{
    sss_status_t retval = isValidKeyStoreShadow(keystore_shadow);
    uint32_t i;
    for (i = 0; i < keystore_shadow->maxEntries; i++)
    {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == extId)
        {
            *intIndex = keyEntry->keyIntIndex;
            retval = kStatus_SSS_Success;
            break;
        }
    }
    return retval;
}

sss_status_t ks_common_get_keyType_from_keyid(keyStoreTable_t *keystore_shadow, uint32_t keyId, uint32_t *keyType)
{
    sss_status_t retval = kStatus_SSS_Fail;
    uint8_t i;
    bool found_entry = FALSE;
    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;

    for (i = 0; i < keystore_shadow->maxEntries; i++)
    {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == keyId)
        {
            *keyType = keyEntry->keyType;
            found_entry = TRUE;
            break;
        }
    }
    if (!found_entry)
        retval = kStatus_SSS_Fail;
cleanup:
    return retval;
}
sss_status_t ks_common_check_available_int_index(keyStoreTable_t *keystore_shadow,
                                                 uint8_t object_type,
                                                 uint16_t *next_free_index)
{
    sss_status_t retval = kStatus_SSS_Success;
    uint8_t i;
    *next_free_index = 0;
    bool found_entry = FALSE;
    uint32_t max_index = 0;
    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;
    if (object_type == kSSS_KeyType_Certificate)
        max_index = KS_N_CERTIFCATES;
    else if (object_type == kSSS_KeyType_ECC_Pair)
        max_index = KS_N_ECC_KEY_PAIRS;
    else if (object_type == kSSS_KeyType_ECC_Public)
        max_index = KS_N_ECC_PUB_KEYS;
    else if (object_type == kSSS_KeyType_AES)
        max_index = KS_N_AES_KEYS;
    for (i = 0; i < keystore_shadow->maxEntries; i++)
    {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->keyType == object_type)
        {
            (*next_free_index)++;
            if ((*next_free_index) == max_index)
            {
                retval = kStatus_SSS_Fail;
                break;
            }
        }
    }

cleanup:
    return retval;
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

static sss_status_t isValidKeyStoreShadow(keyStoreTable_t *keystore_shadow)
{
    sss_status_t retval = kStatus_SSS_Success;
    if (keystore_shadow->magic != KEYSTORE_MAGIC)
    {
        printf("ERROR %s: Magic mismatch. Expected=0x%08X got 0x%08X.\n", __FUNCTION__, KEYSTORE_MAGIC,
               keystore_shadow->magic);
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }
    if (keystore_shadow->version != KEYSTORE_VERSION)
    {
        printf("ERROR %s: version mismatch. Expected=0x%04X got 0x%04X.\n", __FUNCTION__, KEYSTORE_VERSION,
               keystore_shadow->version);
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }
    if (keystore_shadow->maxEntries == 0)
    {
        printf("ERROR: Keystore not yet allocated");
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }
cleanup:
    return retval;
}
