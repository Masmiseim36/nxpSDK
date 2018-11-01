/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "ltc.h"
#include "mmcau.h"
#include "aes_security.h"
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
// This is enough space to allocate the debug version of the MMCAU functions as of commit 4b621d8
#if BL_FEATURE_LOAD_MMCAU_LIB
#define MMCAU_FUNCTION_MEMORY_MAX_SIZE 0x500

typedef void (*user_mmcau_aes_init)(uint32_t *key, uint32_t *keySchedule, uint32_t *rcon);
typedef void (*user_mmcau_aes_encrypt)(uint32_t *in, uint32_t *key, uint32_t *keySchedule, uint32_t *out);
typedef void (*user_mmcau_aes_decrypt)(uint32_t *in, uint32_t *key, uint32_t *keySchedule, uint32_t *out);

typedef struct mmcau_functions
{
    user_mmcau_aes_init init;
    user_mmcau_aes_encrypt encrypt;
    user_mmcau_aes_decrypt decrypt;
} mmcau_functions_t;

static void loadMmcauFunctions(void);
#endif // if BL_FEATURE_LOAD_MMCAU_LIB

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU)
static uint32_t s_aesKeySchedule[AES_128_KEY_SCHEDULE_SIZE_WORDS];
#endif

#if BL_FEATURE_LOAD_MMCAU_LIB
static mmcau_functions_t s_mmcauFunctions = { 0 };

//! @brief A static buffer used to hold the MMCAU functions
static uint8_t s_mmcauFunctionMemory[MMCAU_FUNCTION_MEMORY_MAX_SIZE];
#endif // if BL_FEATURE_LOAD_MMCAU_LIB

/*******************************************************************************
 * Code
 ******************************************************************************/

// See aes_security.h for a description of this function.
void aes_init(uint32_t *key)
{
    if (is_ltc_present())
    {
#if (FSL_FEATURE_SOC_LTC_COUNT && BL_FEATURE_ENCRYPTION_LTC)
        ltc_aes_init(key);
#endif
    }
    else
    {
#if (FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU)
        // rcon is only needed for MMCAU initialization and setting up the keyschedule, this code
        // needed to be moved to here in order to make the code relocatable
        uint32_t rcon[10] = { 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                              0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000 };

#ifndef BL_FEATURE_LOAD_MMCAU_LIB
        mmcau_aes_init(key, s_aesKeySchedule, rcon);
#else
        if (s_mmcauFunctions.init == 0)
        {
            loadMmcauFunctions();
        }

        if (s_mmcauFunctions.init)
        {
            s_mmcauFunctions.init(key, s_aesKeySchedule, rcon);
        }
#endif // BL_FEATURE_LOAD_MMCAU_LIB
#endif // (FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU)
    }
}

// See aes_security.h for a description of this function.
void aes_encrypt(uint32_t *in, uint32_t *key, uint32_t *out)
{
    if (is_ltc_present())
    {
#if (FSL_FEATURE_SOC_LTC_COUNT && BL_FEATURE_ENCRYPTION_LTC)
        ltc_aes_encrypt(in, key, out);
#endif
    }
    else
    {
#if (FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU)

#ifndef BL_FEATURE_LOAD_MMCAU_LIB
        mmcau_aes_encrypt(in, key, s_aesKeySchedule, out);
#else
        if (s_mmcauFunctions.encrypt)
        {
            s_mmcauFunctions.encrypt(in, key, s_aesKeySchedule, out);
        }
#endif // BL_FEATURE_LOAD_MMCAU_LIB
#endif // FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU
    }
}

// See aes_security.h for a description of this function.
void aes_decrypt(uint32_t *in, uint32_t *key, uint32_t *out)
{
    if (is_ltc_present())
    {
#if (FSL_FEATURE_SOC_LTC_COUNT && BL_FEATURE_ENCRYPTION_LTC)
        ltc_aes_decrypt(in, key, out);
#endif
    }
    else
    {
#if (FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU)
#ifndef BL_FEATURE_LOAD_MMCAU_LIB
        mmcau_aes_decrypt(in, key, s_aesKeySchedule, out);
#else
        if (s_mmcauFunctions.decrypt)
        {
            s_mmcauFunctions.decrypt(in, key, s_aesKeySchedule, out);
        }
#endif // BL_FEATURE_LOAD_MMCAU_LIB
#endif // (FSL_FEATURE_SOC_MMCAU_COUNT && BL_FEATURE_ENCRYPTION_MMCAU)
    }
}

//!< @brief This function is to load the MMCAU functions from a location that is provided in the BCA
//!<  In the case of a debug build that has MMCAU_TEST defined it will load the info from the global
//!<  variable g_mmcauFunctions to help facilitate testing.
//!<  The location of the different functions in the binary blob are located through the info struct members
//!<  The functions must be contiguous since the memory copy only copies one continuous block
#if BL_FEATURE_LOAD_MMCAU_LIB
void loadMmcauFunctions(void)
{
    uint32_t mmcauInfoAddress;

#if defined(DEBUG) && defined(MMCAU_TEST)
    extern uint32_t g_mmcauFunctions;
    mmcauInfoAddress = g_mmcauFunctions;
#else
    mmcauInfoAddress = g_bootloaderContext.propertyInterface->store->configurationData.mmcauConfigPointer;
#endif // DEBUG

    mmcau_function_info_t *info;
    uint32_t copyLength;

    if (mmcauInfoAddress != ~0)
    {
        info = (mmcau_function_info_t *)mmcauInfoAddress;

// Make sure that the aes library info isn't in exeucte-only region, because bootloader isn't allowed to
// access such region.
#if FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
        if (is_in_execute_only_region(mmcauInfoAddress, sizeof(mmcau_function_info_t)))
        {
            return;
        }
#endif //  FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

        if (kMmcauInfoTag == info->tag)
        {
            copyLength = MIN(sizeof(s_mmcauFunctionMemory), info->length);
// Make sure that the aes library isn't in exeucte-only region, because bootloader isn't allowed to
// access such region.
#if FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
            if (is_in_execute_only_region(info->aes_init_start, sizeof(copyLength)))
            {
                return;
            }
#endif //  FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

            memcpy(s_mmcauFunctionMemory, (void *)info->aes_init_start, copyLength);

            // Add 1 to keep in thumb mode
            s_mmcauFunctions.init = (user_mmcau_aes_init)(s_mmcauFunctionMemory + 1);
            s_mmcauFunctions.encrypt =
                (user_mmcau_aes_encrypt)(s_mmcauFunctionMemory + (info->aes_encrypt_start - info->aes_init_start) + 1);
            s_mmcauFunctions.decrypt =
                (user_mmcau_aes_decrypt)(s_mmcauFunctionMemory + (info->aes_decrypt_start - info->aes_init_start) + 1);
        }
    }
}

#endif // BL_FEATURE_LOAD_MMCAU_LIB

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
