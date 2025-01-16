/*
 * Copyright 2022-2024 NXP
 * All rights reserved.
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

/*****************************************************************************************************************/
/* WARNING: These recipes are here only for compilation success, EL2Go support is not yet done for MIMXRT family */
/*****************************************************************************************************************/


#define NXP_DIE_MK_SK_SLOT          0x00U
#define NXP_DIE_EL2GOOEM_MK_SK_SLOT 0x04U

#define RECIPE_STEP_CREATE_NXP_DIE_KEK_SK                                                                          \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_FINAL_KEY,                                                        \
        .ckdf = {                                                                                                  \
            .source_key_slot           = NXP_DIE_MK_SK_SLOT,                                                       \
            .target_key_id             = MBEDTLS_NXP_DIE_KEK_SK_ID,                                                \
            .key_properties.word.value = 0x0,                                                                      \
            .derivation_data           = {0},                                                                      \
        },                                                                                                         \
    }

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORT_KEK_SK                                                              \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_FINAL_KEY,                                                        \
        .ckdf = {                                                                                                  \
            .source_key_slot           = NXP_DIE_EL2GOOEM_MK_SK_SLOT,                                              \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOIMPORT_KEK_SK_ID,                                    \
            .key_properties.word.value = 0x0,                                                                      \
            .derivation_data           = {},                                                                       \
        },                                                                                                         \
    }

const key_recipe_t recipe_el2goimport_kek_sk = {
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
            .source_key_slot           = NXP_DIE_EL2GOOEM_MK_SK_SLOT,                                              \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOIMPORTTFM_KEK_SK_ID,                                 \
            .key_properties.word.value = 0x0,                                                                      \
            .derivation_data           = {0},                                                                      \
        },                                                                                                         \
    }

const key_recipe_t recipe_el2goimporttfm_kek_sk = {
    .number_of_steps = 1,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORTTFM_KEK_SK,
        },
};

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORT_AUTH_SK                                                             \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_FINAL_KEY,                                                        \
        .ckdf = {                                                                                                  \
            .source_key_slot           = NXP_DIE_EL2GOOEM_MK_SK_SLOT,                                              \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOIMPORT_AUTH_SK_ID,                                   \
            .key_properties.word.value = 0x0,                                                                      \
            .derivation_data           = {0},                                                                      \
        },                                                                                                         \
    }

const key_recipe_t recipe_el2goimport_auth_sk = {
    .number_of_steps = 1,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_EL2GOIMPORT_AUTH_SK,
        },
};

#define RECIPE_STEP_CREATE_NXP_DIE_EL2GOCONN_AUTH_PRK_SEED                                                         \
    {                                                                                                              \
        .operation = OP_CKDF, .storage = STORAGE_TEMP_KEY,                                                         \
        .ckdf = {                                                                                                  \
            .source_key_slot           = NXP_DIE_EL2GOPUBLIC_MK_SK_SLOT,                                           \
            .target_key_id             = MBEDTLS_NXP_DIE_EL2GOCONN_AUTH_PRK_ID,                                    \
            .key_properties.word.value = 0x0,                                                                      \
            .derivation_data           = {0},                                                                      \
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
            .key_properties.word.value = 0x0,                                                                      \
            .derivation_data           = {0},                                                                      \
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

const key_recipe_t recipe_die_kek_sk = {
    .number_of_steps = 1,
    .steps =
        {
            RECIPE_STEP_CREATE_NXP_DIE_KEK_SK,
        },
};

#endif //_MCUXCLPSADRIVER_ORACLE_KEYRECIPES_
