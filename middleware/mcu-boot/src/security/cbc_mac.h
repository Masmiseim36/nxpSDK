/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _cbc_mac_h
#define _cbc_mac_h

#include "bootloader_common.h"

//! @addtogroup cbc_mac
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define CBC_BLOCK_SIZE 16

#define CBC_MAC_SUPPORTED (defined(CAU_BASE) || defined(LTC0_BASE))

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name cbc_mac
//@{

//! @brief Initializes the CBC MAC algorithm
void cbc_mac_init(uint8_t *key, uint8_t *iv);

//! @brief Encrypt the current CBCMAC with more data
void cbc_mac_encrypt(uint8_t *data, uint32_t length, uint8_t *dest);

//! @brief Decrypt the current CBCMAC with more data
void cbc_mac_decrypt(uint8_t *data, uint32_t length, uint8_t *dest);

//! @brief Finalizes the CBC MAC encryption process
void cbc_mac_finalize(void);

//! @brief Writes the current calculated
void cbc_mac_get(uint32_t *macDest);

//@}

#if defined(__cplusplus)
}
#endif

//! @}

#endif // _cbc_mac_h
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
