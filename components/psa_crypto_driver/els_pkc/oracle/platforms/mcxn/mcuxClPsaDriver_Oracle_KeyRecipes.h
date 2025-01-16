/*
 * Copyright 2022-2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** @file  mcuxClPsaDriver_Oracle_KeyRecipes.h
 *  @brief Key Recipes used in the Oracle code*/

#ifndef _MCUXCLPSADRIVER_ORACLE_KEYRECIPES_
#define _MCUXCLPSADRIVER_ORACLE_KEYRECIPES_

#include <common.h>
#include <psa/crypto.h>

#include <stdint.h>
#include <stddef.h>
#include "mcuxClPsaDriver_Oracle_Utils.h"

#define NXP_DIE_MK_SK_SLOT 0x00U

#define NXP_CUST_DIE_EL2GO_MK_SK_ID    0x7FFF816DU
#define NXP_CUST_DIE_EL2GOOEM_MK_SK_ID 0x7FFF816AU 

#if !defined(MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER)
#define MBEDTLS_NXP_CUST_DIE_EL2GO_MK_SK_ID    NXP_CUST_DIE_EL2GO_MK_SK_ID
#define MBEDTLS_NXP_CUST_DIE_EL2GOOEM_MK_SK_ID NXP_CUST_DIE_EL2GOOEM_MK_SK_ID 
#else
#define MBEDTLS_NXP_CUST_DIE_EL2GO_MK_SK_ID                                                 \
    {                                                                                       \
        .MBEDTLS_PRIVATE(owner) = 0, .MBEDTLS_PRIVATE(key_id) = NXP_CUST_DIE_EL2GO_MK_SK_ID \
    }
#define MBEDTLS_NXP_CUST_DIE_EL2GOOEM_MK_SK_ID                                                 \
    {                                                                                          \
        .MBEDTLS_PRIVATE(owner) = 0, .MBEDTLS_PRIVATE(key_id) = NXP_CUST_DIE_EL2GOOEM_MK_SK_ID \
    }
#endif

psa_status_t get_el2goem_mk_dd(uint8_t* dd);

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORT_KEK_SK                                                              \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_FINAL_KEY,                                                        \
        .ckdf = {                                                                                                  \
            .source                    = SOURCE_KEY_SLOT,                                                          \
            .dd_src                    = DERIVATION_DATA_SOURCE_STATIC,                                            \
            .kdf_mask                  = 0x07001FCFu,                                                              \
            .source_key_slot           = NXP_DIE_MK_SK_SLOT,                                                       \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOIMPORT_KEK_SK_ID,                                    \
            .key_properties.word.value = 0x404000E1u,                                                              \
            .derivation_data           = {0x94, 0xf2, 0x84, 0x46, 0x62, 0x09, 0x4e, 0x64, 0x59, 0xd0, 0x4d, 0x9f}, \
        },                                                                                                         \
    }

const key_recipe_t recipe_el2goimport_kek_sk = {
    .number_of_steps = 1,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORT_KEK_SK,
        },
};

/* For N10 DIE KEK SK and EL2GOIMPORT_KEK_SK are same */
const key_recipe_t recipe_die_kek_sk = {
    .number_of_steps = 1,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORT_KEK_SK,
        },
};

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORTTFM_KEK_SK                                                           \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_FINAL_KEY,                                                        \
        .ckdf = {                                                                                                  \
            .source                    = SOURCE_KEY_ID,                                                            \
            .dd_src                    = DERIVATION_DATA_SOURCE_STATIC,                                            \
            .kdf_mask                  = 0x07001FCFu,                                                              \
            .source_key_id             = MBEDTLS_NXP_CUST_DIE_EL2GOOEM_MK_SK_ID,                                   \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOIMPORTTFM_KEK_SK_ID,                                 \
            .key_properties.word.value = 0x40100021U,                                                              \
            .derivation_data           = {0x00, 0x65, 0x32, 0x67, 0x69, 0x74, 0x66, 0x6d, 0x5f, 0x73, 0x6b, 0x00}, \
        },                                                                                                         \
    }

#define RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GO_MK_SK                                                                \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_TEMP_KEY,                                                         \
        .ckdf = {                                                                                                  \
            .source                    = SOURCE_KEY_SLOT,                                                          \
            .dd_src                    = DERIVATION_DATA_SOURCE_STATIC,                                            \
            .kdf_mask                  = 0x07001FCFu,                                                              \
            .source_key_slot           = NXP_DIE_MK_SK_SLOT,                                                       \
            .target_key_id             = MBEDTLS_NXP_CUST_DIE_EL2GO_MK_SK_ID,                                      \
            .key_properties.word.value = 0x500100E1U,                                                              \
            .derivation_data           = {0x16, 0x5b, 0x2e, 0xec, 0x00, 0x81, 0x94, 0x34, 0x7b, 0x3f, 0x37, 0x98}, \
        },                                                                                                         \
    }

#define RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GOOEM_MK_SK                           \
    {                                                                            \
        .operation = OP_CKDF, .storage = STORAGE_TEMP_KEY,                       \
        .ckdf = {                                                                \
            .source                    = SOURCE_KEY_ID,                          \
            .dd_src                    = DERIVATION_DATA_SOURCE_DYNAMIC,         \
            .kdf_mask                  = 0x07001FCFu,                            \
            .source_key_id             = MBEDTLS_NXP_CUST_DIE_EL2GO_MK_SK_ID,    \
            .target_key_id             = MBEDTLS_NXP_CUST_DIE_EL2GOOEM_MK_SK_ID, \
            .key_properties.word.value = 0x40010021U,                            \
            .derivation_fn             = &get_el2goem_mk_dd,                     \
        },                                                                       \
    }

#define RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GOIMPORT_AUTH_SK                                                        \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_FINAL_KEY,                                                        \
        .ckdf = {                                                                                                  \
            .source                    = SOURCE_KEY_ID,                                                            \
            .dd_src                    = DERIVATION_DATA_SOURCE_STATIC,                                            \
            .kdf_mask                  = 0x07001FCFu,                                                              \
            .source_key_id             = MBEDTLS_NXP_CUST_DIE_EL2GOOEM_MK_SK_ID,                                   \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOIMPORT_AUTH_SK_ID,                                   \
            .key_properties.word.value = 0x40002021u,                                                              \
            .derivation_data           = {0x00, 0x65, 0x32, 0x67, 0x69, 0x61, 0x75, 0x74, 0x5f, 0x73, 0x6b, 0x00}, \
        },                                                                                                         \
    }

#define RECIPE_STEP_DELETE_NXP_CUST_DIE_EL2GO_MK_SK               \
    {                                                             \
        .operation = OP_KDELETE, .storage = STORAGE_TEMP_KEY,     \
        .kdelete = {                                              \
            .target_key_id = MBEDTLS_NXP_CUST_DIE_EL2GO_MK_SK_ID, \
        },                                                        \
    }

#define RECIPE_STEP_DELETE_NXP_CUST_DIE_EL2GOOEM_MK_SK               \
    {                                                                \
        .operation = OP_KDELETE, .storage = STORAGE_TEMP_KEY,        \
        .kdelete = {                                                 \
            .target_key_id = MBEDTLS_NXP_CUST_DIE_EL2GOOEM_MK_SK_ID, \
        },                                                           \
    }

const key_recipe_t recipe_el2goimport_auth_sk = {
    .number_of_steps = 5,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GO_MK_SK,
            RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GOOEM_MK_SK,
            RECIPE_STEP_DELETE_NXP_CUST_DIE_EL2GO_MK_SK,
            RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GOIMPORT_AUTH_SK,
            RECIPE_STEP_DELETE_NXP_CUST_DIE_EL2GOOEM_MK_SK,
        },
};

const key_recipe_t recipe_el2goimporttfm_kek_sk = {
    .number_of_steps = 5,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GO_MK_SK,
            RECIPE_STEP_CREATE_NXP_CUST_DIE_EL2GOOEM_MK_SK,
            RECIPE_STEP_DELETE_NXP_CUST_DIE_EL2GO_MK_SK,
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORTTFM_KEK_SK,
            RECIPE_STEP_DELETE_NXP_CUST_DIE_EL2GOOEM_MK_SK,
        },
};

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOCONN_AUTH_PRK_SEED                                                         \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_TEMP_KEY,                                                         \
        .ckdf = {                                                                                                  \
            .source_key_slot           = NXP_DIE_EL2GOPUBLIC_MK_SK_SLOT,                                           \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOCONN_AUTH_PRK_ID,                                    \
            .key_properties.word.value = 0x84000021U,                                                              \
            .derivation_data           = {0x00, 0x65, 0x32, 0x67, 0x63, 0x6f, 0x6e, 0x5f, 0x73, 0x65, 0x00, 0x00}, \
        },                                                                                                         \
    }

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOCONN_AUTH_PRK                           \
    {                                                                           \
        .operation = OP_KEYGEN, .storage = STORAGE_FINAL_KEY,                   \
        .keygen = {                                                             \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOCONN_AUTH_PRK_ID, \
            .key_properties.word.value = 0x80040001U,                           \
        },                                                                      \
    }

const key_recipe_t recipe_el2goconn_auth_prk = {
    .number_of_steps = 2,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOCONN_AUTH_PRK_SEED,
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOCONN_AUTH_PRK,
        },
};

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOATTEST_AUTH_PRK_SEED                                                       \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_TEMP_KEY,                                                         \
        .ckdf = {                                                                                                  \
            .source_key_slot           = NXP_DIE_EL2GOPUBLIC_MK_SK_SLOT,                                           \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOATTEST_AUTH_PRK_ID,                                  \
            .key_properties.word.value = 0x84000021U,                                                              \
            .derivation_data           = {0x00, 0x65, 0x32, 0x67, 0x61, 0x74, 0x74, 0x5f, 0x73, 0x65, 0x00, 0x00}, \
        },                                                                                                         \
    }

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOATTEST_AUTH_PRK                           \
    {                                                                             \
        .operation = OP_KEYGEN, .storage = STORAGE_FINAL_KEY,                     \
        .keygen = {                                                               \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOATTEST_AUTH_PRK_ID, \
            .key_properties.word.value = 0x80040001U,                             \
        },                                                                        \
    }

const key_recipe_t recipe_el2goattest_auth_prk = {
    .number_of_steps = 2,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOATTEST_AUTH_PRK_SEED,
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOATTEST_AUTH_PRK,
        },
};

#endif //_MCUXCLPSADRIVER_ORACLE_KEYRECIPES_
