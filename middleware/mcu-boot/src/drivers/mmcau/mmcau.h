/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _mmcau_h
#define _mmcau_h

#include "bootloader_common.h"

//! @addtogroup mmcau
//! @{

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name mmcau_aes_security
//@{

//! @brief An initialization function for the decryption peripheral
void mmcau_aes_init(uint32_t *key, uint32_t *keySchedule, uint32_t *rcon);

//! @brief Encrypts a 16 byte block of data
//! in and out may use the same address so encrypting in place is supported
void mmcau_aes_encrypt(uint32_t *in, uint32_t *key, uint32_t *keySchedule, uint32_t *out);

//! @brief Decrypts a 16 byte block of data
//! in and out may use the same address so decrypting in place is supported
void mmcau_aes_decrypt(uint32_t *in, uint32_t *key, uint32_t *keySchedule, uint32_t *out);
//@}

#if defined(__cplusplus)
}
#endif

//! @}

#endif // _mmcau_h
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
