/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "el2go_blob_test_parser.h"
#include "el2go_blob_test_executor_psa.h"

#define PSA_CMD_TAG_KEY_ID              0x41
#define PSA_CMD_TAG_PERMITTED_ALGORITHM 0x42
#define PSA_CMD_TAG_KEY_USAGE_FLAGS     0x43
#define PSA_CMD_TAG_KEY_TYPE            0x44
#define PSA_CMD_TAG_KEY_BITS            0x45
#define PSA_CMD_TAG_KEY_LIFETIME        0x46

static uint32_t get_uint32_val(const uint8_t *input)
{
    uint32_t output = 0U;
    output          = *(input);
    output <<= 8;
    output |= *(input + 1);
    output <<= 8;
    output |= *(input + 2);
    output <<= 8;
    output |= *(input + 3);
    return output;
}

static uint16_t get_uint16_val(const uint8_t *input)
{
    uint16_t output = 0U;
    output          = *input;
    output <<= 8;
    output |= *(input + 1);
    return output;
}

static int get_len(const unsigned char **p, const unsigned char *end, size_t *len)
{
    if ((end - *p) < 1)
        return (PSA_ERROR_INVALID_ARGUMENT);

    if ((**p & 0x80) == 0)
        *len = *(*p)++;
    else
    {
        switch (**p & 0x7F)
        {
            case 1:
                if ((end - *p) < 2)
                    return (PSA_ERROR_INVALID_ARGUMENT);

                *len = (*p)[1];
                (*p) += 2;
                break;

            case 2:
                if ((end - *p) < 3)
                    return (PSA_ERROR_INVALID_ARGUMENT);

                *len = ((size_t)(*p)[1] << 8) | (*p)[2];
                (*p) += 3;
                break;

            case 3:
                if ((end - *p) < 4)
                    return (PSA_ERROR_INVALID_ARGUMENT);

                *len = ((size_t)(*p)[1] << 16) | ((size_t)(*p)[2] << 8) | (*p)[3];
                (*p) += 4;
                break;

            case 4:
                if ((end - *p) < 5)
                    return (PSA_ERROR_INVALID_ARGUMENT);

                *len = ((size_t)(*p)[1] << 24) | ((size_t)(*p)[2] << 16) | ((size_t)(*p)[3] << 8) | (*p)[4];
                (*p) += 5;
                break;

            default:
                return (PSA_ERROR_INVALID_ARGUMENT);
        }
    }
    if (*len > (size_t)(end - *p))
        return (PSA_ERROR_INVALID_ARGUMENT);

    return (0);
}

static int get_tag(const unsigned char **p, const unsigned char *end, size_t *len, int tag)
{
    if ((end - *p) < 1)
        return (PSA_ERROR_INVALID_ARGUMENT);

    if (**p != tag)
        return (PSA_ERROR_INVALID_ARGUMENT);

    (*p)++;

    return (get_len(p, end, len));
}

static psa_status_t parse_psa_key_attributes(const uint8_t *data, size_t data_size, psa_key_attributes_t *attributes)
{
    psa_status_t psa_status = PSA_SUCCESS;

    uint8_t tag    = 0U; // the tag of the current TLV
    size_t cmd_len = 0U; // the length of the current TLV

    const uint8_t *cmd_ptr = NULL;
    const uint8_t *end     = NULL;

    if (data == NULL || data_size == 0 || attributes == NULL)
    {
        psa_status = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    *attributes = psa_key_attributes_init();

    cmd_ptr = data;
    end     = cmd_ptr + data_size;

    while ((cmd_ptr + 1) < end)
    {
        tag        = *cmd_ptr;
        psa_status = get_tag(&cmd_ptr, end, &cmd_len, tag);
        if (psa_status != PSA_SUCCESS)
        {
            goto exit;
        }

        switch (tag)
        {
            case PSA_CMD_TAG_KEY_ID:
                psa_set_key_id(attributes, (psa_key_id_t)get_uint32_val(cmd_ptr));
                break;
            case PSA_CMD_TAG_PERMITTED_ALGORITHM:
                psa_set_key_algorithm(attributes, (psa_algorithm_t)get_uint32_val(cmd_ptr));
                break;
            case PSA_CMD_TAG_KEY_USAGE_FLAGS:
                psa_set_key_usage_flags(attributes, (psa_key_usage_t)get_uint32_val(cmd_ptr));
                break;
            case PSA_CMD_TAG_KEY_TYPE:
                psa_set_key_type(attributes, (psa_key_type_t)get_uint16_val(cmd_ptr));
                break;
            case PSA_CMD_TAG_KEY_BITS:
                psa_set_key_bits(attributes, (size_t)get_uint32_val(cmd_ptr));
                break;
            case PSA_CMD_TAG_KEY_LIFETIME:
                psa_set_key_lifetime(attributes, (psa_key_lifetime_t)get_uint32_val(cmd_ptr));
                break;
            default:
                break;
        }
        cmd_ptr += cmd_len;
    }

exit:
    return psa_status;
}

void parse_and_run_test(const uint8_t *blob, size_t blob_length, struct test_result_t *result)
{
    if (blob_length == 1)
    {
        TEST_SKIP("Placeholder blob");
        return;
    }

    psa_status_t psa_status;
    psa_key_attributes_t attributes;
    psa_status = parse_psa_key_attributes(blob, blob_length, &attributes);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("parse_psa_key_attributes");
        return;
    }

    psa_key_type_t key_type         = psa_get_key_type(&attributes);
    size_t key_bits                 = psa_get_key_bits(&attributes);
    psa_key_usage_t key_usage       = psa_get_key_usage_flags(&attributes);
    psa_algorithm_t key_algorithm   = psa_get_key_algorithm(&attributes);
    psa_key_location_t key_location = PSA_KEY_LIFETIME_GET_LOCATION(psa_get_key_lifetime(&attributes));
    psa_key_id_t key_id             = psa_get_key_id(&attributes);

    switch (key_usage)
    {
        case PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT:
            if (PSA_ALG_IS_AEAD(key_algorithm))
            {
                psa_blob_aead_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            }
            else if (PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(key_algorithm))
            {
                psa_blob_crypt_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            }
            else
            {
                psa_blob_cipher_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                     result);
            }
            break;
        case PSA_KEY_USAGE_ENCRYPT:
            if (PSA_ALG_IS_AEAD(key_algorithm))
            {
                psa_blob_aead_encrypt_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                           result);
            }
            else
            {
                psa_blob_encrypt_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                      result);
            }
            break;
        case PSA_KEY_USAGE_DECRYPT:
            if (PSA_ALG_IS_AEAD(key_algorithm))
            {
                psa_blob_aead_decrypt_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                           result);
            }
            else
            {
                psa_blob_decrypt_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                      result);
            }
            break;
        case PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE:
            if (PSA_ALG_IS_MAC(key_algorithm))
            {
                psa_blob_mac_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            }
            else
            {
                psa_blob_sigvermsg_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                        result);
            }
            break;
        case PSA_KEY_USAGE_SIGN_MESSAGE:
            if (PSA_ALG_IS_MAC(key_algorithm))
            {
                psa_blob_mac_compute_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                          result);
            }
            else
            {
                psa_blob_sigmsg_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                     result);
            }
            break;
        case PSA_KEY_USAGE_VERIFY_MESSAGE:
            if (PSA_ALG_IS_MAC(key_algorithm))
            {
                psa_blob_mac_verify_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                         result);
            }
            else
            {
                psa_blob_vermsg_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                     result);
            }
            break;
        case PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH:
        // Message is always added automatically when allowing Hash
        case PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_HASH |
            PSA_KEY_USAGE_VERIFY_MESSAGE:
            psa_blob_sigverhash_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                     result);
            break;
        case PSA_KEY_USAGE_SIGN_HASH:
        // Message is always added automatically when allowing Hash
        case PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_SIGN_MESSAGE:
            psa_blob_sighash_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            break;
        case PSA_KEY_USAGE_VERIFY_HASH:
        // Message is always added automatically when allowing Hash
        case PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_VERIFY_MESSAGE:
            psa_blob_verhash_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            break;
        case PSA_KEY_USAGE_DERIVE:
            if (PSA_ALG_IS_KEY_AGREEMENT(key_algorithm))
            {
                psa_blob_keyexch_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length,
                                      result);
            }
            else
            {
                psa_blob_kdf_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            }
            break;
        case PSA_KEY_USAGE_EXPORT:
        case 0: // PSA_KEY_USAGE_NONE
            psa_blob_export_test(key_type, key_bits, key_algorithm, key_location, key_id, blob, blob_length, result);
            break;
        default:
            TEST_SKIP("Unsupported key usage");
            return;
    }
}
