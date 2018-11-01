/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ltc_h
#define _ltc_h

#include "bootloader_common.h"

//! @addtogroup ltc
//! @{

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name ltc_aes_security
//@{

//! @brief An initialization function for the decryption peripheral
void ltc_aes_init(uint32_t *key);

//! @brief Encrypts a 16 byte block of data
//! in and out may use the same address so encrypting in place is supported
void ltc_aes_encrypt(uint32_t *in, uint32_t *key, uint32_t *out);

//! @brief Decrypts a 16 byte block of data
//! in and out may use the same address so decrypting in place is supported
void ltc_aes_decrypt(uint32_t *in, uint32_t *key, uint32_t *out);
//@}

#if defined(__cplusplus)
}
#endif

//! @}

#endif // _ltc_h
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
