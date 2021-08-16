/*
 * Copyright 2018-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* Common Key store implementation between keystore_a7x and keystore_pc */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */
#include <fsl_sss_sscp.h>

#if SSS_HAVE_A71XX
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

sss_status_t ks_common_extId_to_int_index(keyStoreTable_t *keystore_shadow, uint32_t extId, uint16_t *intIndex)
{
    uint32_t i;
    bool found_entry    = FALSE;
    sss_status_t retval = isValidKeyStoreShadow(keystore_shadow);

    if (retval != kStatus_SSS_Success) {
        return kStatus_SSS_Fail;
    }

    for (i = 0; i < keystore_shadow->maxEntries; i++) {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == extId) {
            *intIndex   = keyEntry->keyIntIndex;
            found_entry = TRUE;
            break;
        }
    }
    retval = (found_entry == TRUE) ? kStatus_SSS_Success : kStatus_SSS_Fail;
    return retval;
}

sss_status_t ks_common_get_keyType_from_keyid(
    keyStoreTable_t *keystore_shadow, uint32_t keyId, uint32_t *keyType, uint32_t *cipherType)
{
    sss_status_t retval = kStatus_SSS_Fail;
    uint8_t i;
    bool found_entry = FALSE;
    retval           = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;

    for (i = 0; i < keystore_shadow->maxEntries; i++) {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        if (keyEntry->extKeyId == keyId) {
            *keyType    = keyEntry->keyPart;
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

sss_status_t ks_common_check_available_int_index(keyStoreTable_t *keystore_shadow,
    uint8_t object_type,
    uint8_t cipher_type,
    uint16_t *next_free_index,
    uint16_t keyLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    uint32_t i;
    *next_free_index           = 0;
    uint32_t max_index         = 0;
    uint8_t slots_req          = 1;
    uint16_t keyLen_roundoff   = 0;
    uint16_t slots_used        = 0;
    uint8_t slots_found        = 0;
    uint8_t slots_id           = 0;
    uint8_t slots_len          = 0;
    uint32_t j                 = 1;
    uint32_t consider_keyentry = 0;
    uint8_t key_shadow[19]     = {
        0,
    }; /* Change logic using bits later */

    retval = isValidKeyStoreShadow(keystore_shadow);
    if (retval != kStatus_SSS_Success)
        goto cleanup;
    if (object_type == kSSS_KeyPart_Default && cipher_type == kSSS_CipherType_Binary)
        max_index = KS_N_CERTIFCATES;
    else if (object_type == kSSS_KeyPart_Pair && cipher_type == kSSS_CipherType_EC_NIST_P)
        max_index = KS_N_ECC_KEY_PAIRS;
    else if (object_type == kSSS_KeyPart_Public && cipher_type == kSSS_CipherType_EC_NIST_P)
        max_index = KS_N_ECC_PUB_KEYS;
    else if (object_type == kSSS_KeyPart_Pair && cipher_type == kSSS_CipherType_RSA)
        max_index = KS_N_RSA_KEY_PAIRS;
    else if (object_type == kSSS_KeyPart_Default &&
             (cipher_type == kSSS_CipherType_AES || cipher_type == kSSS_CipherType_HMAC)) {
#if SSS_HAVE_A71CH || SSS_HAVE_A71CH_SIM
        max_index = KS_N_AES_KEYS;
        if (keyLen > 64) {
            retval = kStatus_SSS_Fail;
            goto cleanup;
        }
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
        max_index = KS_N_SYM_KEYS;
#endif
        keyLen_roundoff = ((keyLen / 16) * 16) + ((keyLen % 16) == 0 ? 0 : 16);
        slots_req       = (keyLen_roundoff / 16);
    }
    else {
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }

    for (i = 0; i < keystore_shadow->maxEntries; i++) {
        keyIdAndTypeIndexLookup_t *keyEntry = &keystore_shadow->entries[i];
        consider_keyentry                   = 0;
        if ((keyEntry->keyPart & 0x0F) == object_type) {
            if (object_type == kSSS_KeyPart_Default &&
                (cipher_type == kSSS_CipherType_AES || cipher_type == kSSS_CipherType_HMAC)) {
                if (keyEntry->cipherType == kSSS_CipherType_AES || keyEntry->cipherType == kSSS_CipherType_HMAC) {
                    slots_used++;
                    consider_keyentry = 1;
                }
            }
            else {
                if (keyEntry->cipherType == cipher_type) {
                    slots_used++;
                    consider_keyentry = 1;
                }
            }

            if (slots_used == max_index - (slots_req - 1)) {
                retval = kStatus_SSS_Fail;
                goto cleanup;
            }

            if (consider_keyentry) {
                if (keyEntry->extKeyId != 0) {
                    j                    = 1;
                    slots_id             = keyEntry->keyIntIndex & 0x0F;
                    slots_len            = (keyEntry->keyIntIndex & 0xF0) >> 4;
                    key_shadow[slots_id] = 1;
                    while (j < slots_len) {
                        key_shadow[slots_id + j] = 1;
                        j++;
                    }
                }
            }
        }
    }

    /* Make sure required slots are in sequence */
    for (i = 0; i < max_index; i++) {
        if (key_shadow[i] == 0) {
            uint8_t valid_slot = 1;
            j                  = 1;
            while (j < slots_req) {
                if ((key_shadow[i + j] != 0) || ((i + j) >= max_index)) {
                    valid_slot = 0;
                }
                j++;
            }

            if (valid_slot) {
                slots_found      = 1;
                *next_free_index = i;
                break;
            }
        }
    }

    if (slots_found == 0) {
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }

    if (object_type == kSSS_KeyPart_Default &&
        (cipher_type == kSSS_CipherType_AES || cipher_type == kSSS_CipherType_HMAC)) {
        (*next_free_index) = (*next_free_index) | (slots_req << 4);
    }

cleanup:
    return retval;
}
#endif
