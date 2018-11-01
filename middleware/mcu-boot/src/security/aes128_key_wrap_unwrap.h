/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _aes128_key_wrap_unwrap_h
#define _aes128_key_wrap_unwrap_h

#include <stdint.h>

//! @addtogroup aes_key_wrap
//! @{

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name aes_key_wrap
//@{

//! @brief Encrypt and wrap plaintext key.
//!
//! @param plaintext 40 byte plaintext input array
//! @param wrapped_ciphertext 48 byte wrapped output array
//! @param expanded_kek Expanded kek (64 bytes)
void do_aes128_key_wrap(const uint8_t plaintext[], uint8_t wrapped_ciphertext[], uint32_t expanded_kek[]);

//! @brief Decrypt and unwrap wrapped key.
//!
//! @param wrapped_ciphertext 48 byte wrapped input array
//! @param unwrapped_plaintext 48 byte plaintext output array, Note: this includes an 8 byte IV header
//!         that is added to start of the original 40 input bytes by the wrapping
//! @param expanded_kek Expanded kek (64 ints on host, 4 ints on device)
//! @retval -1 on error
//! @retval 0 on success
uint32_t do_aes128_key_unwrap(const uint8_t wrapped_ciphertext[],
                              uint8_t unwrapped_plaintext[],
                              uint32_t expanded_kek[]);
//@}

#if defined(__cplusplus)
}
#endif

//! @}

#endif // _aes128_key_wrap_unwrap_h

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
