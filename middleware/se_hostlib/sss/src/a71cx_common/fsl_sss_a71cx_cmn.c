/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Common Key store implementation between keystore_a7x and keystore_pc */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */
#include <fsl_sss_sscp.h>

#if SSS_HAVE_A71XX || SSS_HAVE_SE050_EAR
#include <fsl_sss_keyid_map.h>
#include <string.h>

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

sss_status_t ks_common_extId_to_int_index(
    keyStoreTable_t *keystore_shadow, uint32_t extId, uint16_t *intIndex)
{
    sss_status_t retval = isValidKeyStoreShadow(keystore_shadow);
    uint32_t i;
    for (i = 0; i < keystore_shadow->maxEntries; i++) {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == extId) {
            *intIndex = keyEntry->keyIntIndex;
            retval = kStatus_SSS_Success;
            break;
        }
    }
    return retval;
}

sss_status_t ks_common_get_keyType_from_keyid(keyStoreTable_t *keystore_shadow,
    uint32_t keyId,
    uint32_t *keyType,
    uint32_t *cipherType)
{
    sss_status_t retval = kStatus_SSS_Fail;
    uint8_t i;
    bool found_entry = FALSE;
    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;

    for (i = 0; i < keystore_shadow->maxEntries; i++) {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == keyId) {
            *keyType = keyEntry->keyPart;
            *cipherType = keyEntry->cipherType;
            found_entry = TRUE;
            break;
        }
    }
    if (!found_entry) {
        retval = kStatus_SSS_Fail;
    }
cleanup:
    return retval;
}

sss_status_t ks_common_check_available_int_index(
    keyStoreTable_t *keystore_shadow,
    uint8_t object_type,
    uint8_t cipher_type,
    uint16_t *next_free_index)
{
    sss_status_t retval = kStatus_SSS_Success;
    uint8_t i;
    *next_free_index = 0;
    uint32_t max_index = 0;
    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;
    if (object_type == kSSS_KeyPart_Default &&
        cipher_type == kSSS_CipherType_Binary)
        max_index = KS_N_CERTIFCATES;
    else if (object_type == kSSS_KeyPart_Pair &&
             cipher_type == kSSS_CipherType_EC_NIST_P)
        max_index = KS_N_ECC_KEY_PAIRS;
    else if (object_type == kSSS_KeyPart_Public &&
             cipher_type == kSSS_CipherType_EC_NIST_P)
        max_index = KS_N_ECC_PUB_KEYS;
    else if (object_type == kSSS_KeyPart_Pair &&
             cipher_type == kSSS_CipherType_RSA)
        max_index = KS_N_RSA_KEY_PAIRS;
    else if (object_type == kSSS_KeyPart_Default &&
             cipher_type == kSSS_CipherType_AES)
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
        max_index = KS_N_AES_KEYS;
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
        max_index = KS_N_SYM_KEYS;
#endif
    for (i = 0; i < keystore_shadow->maxEntries; i++) {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->keyPart == object_type &&
            keyEntry->cipherType == cipher_type) {
            (*next_free_index)++;
            if ((*next_free_index) == max_index) {
                retval = kStatus_SSS_Fail;
                break;
            }
        }
    }

cleanup:
    return retval;
}
#endif
