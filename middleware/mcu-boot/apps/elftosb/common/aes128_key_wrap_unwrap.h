/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _aes128_key_wrap_unwrap_h
#define _aes128_key_wrap_unwrap_h

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
void do_aes128_key_wrap(const unsigned char plaintext[],
                        unsigned char wrapped_ciphertext[],
                        unsigned int expanded_kek[]);

//! @brief Decrypt and unwrap wrapped key.
//!
//! @param wrapped_ciphertext 48 byte wrapped input array
//! @param unwrapped_plaintext 48 byte plaintext output array, Note: this includes an 8 byte IV header
//!         that is added to start of the original 40 input bytes by the wrapping
//! @param expanded_kek Expanded kek (64 ints on host, 4 ints on device)
//! @retval -1 on error
//! @retval 0 on success
unsigned int do_aes128_key_unwrap(const unsigned char wrapped_ciphertext[],
                                  unsigned char unwrapped_plaintext[],
                                  unsigned int expanded_kek[]);

//! @brief Encrypt and wrap plaintext key.
//!
//! @param plaintext 64 byte plaintext input array
//! @param wrapped_ciphertext 64 byte wrapped output array
//! @param expanded_kek Expanded kek (64 bytes)
void do_aes_key_wrap_N8(const unsigned char plaintext[],
    unsigned char wrapped_ciphertext[],
    unsigned char kek[]);

//! @brief Decrypt and unwrap wrapped key.
//!
//! @param wrapped_ciphertext 64 byte wrapped input array
//! @param unwrapped_plaintext 64 byte plaintext output array, Note: this includes an 8 byte IV header
//!         that is added to start of the original 64 input bytes by the wrapping
//! @param expanded_kek Expanded kek (64 ints on host, 4 ints on device)
//! @retval -1 on error
//! @retval 0 on success
unsigned int do_aes_key_unwrap_N8(const unsigned char wrapped_ciphertext[],
    unsigned char unwrapped_plaintext[],
    unsigned char kek[]);
//@}

#if defined(__cplusplus)
}
#endif

//! @}

#endif // _aes128_key_wrap_unwrap_h

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
