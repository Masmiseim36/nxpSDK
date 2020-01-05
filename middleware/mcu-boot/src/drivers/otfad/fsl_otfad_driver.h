/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_OTFAD_DRIVER_H_
#define _FSL_OTFAD_DRIVER_H_

#include "bootloader_common.h"
#include "fsl_otfad_hal.h"

//! @addtogroup otfad
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief OTFAD Driver status codes.
enum _otfad_driver_errors
{
    kStatus_OtfadSecurityViolation = MAKE_STATUS(kStatusGroup_OTFADDriver, 0),
    kStatus_OtfadLogicallyDisabled = MAKE_STATUS(kStatusGroup_OTFADDriver, 1),
    kStatus_OtfadInvalidKey = MAKE_STATUS(kStatusGroup_OTFADDriver, 2),
    kStatus_OtfadInvalidKeyBlob = MAKE_STATUS(kStatusGroup_OTFADDriver, 3)
};

//! @brief Format of OTFAD key wrap key (KEK).
//!
//! The four consecutive little-endian memory-mapped
//! registers provide 128 bits of key storage.
typedef struct OtfadKek
{
    uint32_t keyWord0; //!< Word0: KEY[31:0][A03, A02, A01, A00]
    uint32_t keyWord1; //!< Word1: KEY[31:0][A07, A06, A05, A04]
    uint32_t keyWord2; //!< Word2: KEY[31:0][A11, A10, A09, A08]
    uint32_t keyWord3; //!< Word3: KEY[31:0][A15, A14, A13, A12]
} otfad_kek_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//! @brief Initialize.
//!
//! If OTFAD KEY in the SIM module is invalid, or the key blobs are invalid,
//! then the OTFAD will be initialized to bypass mode
//! (no decryption). Otherwise, keyBlobs will be unwrapped and programmed into
//! the OTFAD context registers.
//!
//! @param instance Module instance
//! @param keyBlobs Pointer to array of 4 key blobs
//! @param kek Pointer to OTFAD Key Encryption Key (KEK)
//! @retval kStatus_OtfadLogicallyDisabled OTFAD is disabled
//! @retval kStatus_OtfadSecurityViolation Security violation detectd
//! @retval kStatus_Success OTFAD has been initialized
status_t otfad_init(uint32_t instance, const uint8_t *keyBlobs, const otfad_kek_t *kek);

//! @brief Bypass.
//!
//! @param instance Module instance
//! @retval kStatus_Success OTFAD has been bypassed.
status_t otfad_bypass(uint32_t instance);

//! @brief Resume.
//!
//! @param instance Module instance
//! @retval kStatus_Success OTFAD has been resumed.
status_t otfad_resume(uint32_t instance);

#if defined(__cplusplus)
}
#endif // __cplusplus

//! @}

#endif // _FSL_OTFAD_DRIVER_H_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
