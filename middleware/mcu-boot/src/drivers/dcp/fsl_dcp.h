/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __FSL_DCP_H__
#define __FSL_DCP_H__

#include <stdint.h>

//!@brief DCP context structure
typedef struct
{
    uint32_t _private[4];
} dcp_alg_ctx_t;

//!@brief DCP AES Mode
typedef enum __aes_mode
{
    kAesMode_Decrypt,
    kAesMode_Encrypt,
} aes_mode_t;

//!@brief Status code for DCP AES operation
enum
{
    kAesOperation_Success = 0,
    kAesOperation_InvalidArgument = 1,
    kAesOperation_InvalidMAC = 2,
    kAesOperation_Timeout = 3,
    kAesOperation_Error = 4,
};

//!@brief Select OTPMK[255:128] / SNVS[255:128] as DCP AES key using little endian
#define OTPMK_SNVS_HIGH_FLAG_LE 0xfedc89ab
//!@brief Select OTPMK[255:128] / SNVS[255:128] as DCP AES key using big endian
#define OTPMK_SNVS_HIGH_FLAG_BE 0xfedcab89

typedef union _aes_key_sel
{
    uint32_t option;
    uint8_t *key;
} aes_key_sel_t;

//!@brief Select OTPMK[127:0] / SNVS[127:0] as DCP AES key using little endian
#define OTPMK_SNVS_LOW_FLAG_LE 0xfedccdef
//!@brief Select OTPMK[127:0] / SNVS[127:0] as DCP AES key using big endian
#define OTPMK_SNVS_LOW_FLAG_BE 0xfedcefdc

//!@brief Select SW_GP2 as DCP AES key using little endian
#define SW_GP2_FLAG_LE 0xfedc1234
//!@brief Select SW_GP2 as DCP AES key using big endian
#define SW_GP2_FLAG_BE 0xfedc3412

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * @brief DCP AES Engine Intialization
 * @param ctx  DCP AES Context to be initialized
 *
 * @return 0 - Success 1, 4 - Invalid Argument
 */
int dcp_aes_init(dcp_alg_ctx_t *ctx);

/*
 * @brief Set KEY to DCP AES Engine
 * @param ctx  DCP Context
 * @param key_sel  encryption/decryption key
 * @param keybits 128bit key
 *
 * @return 0 - Success 1 - Invalid Argument
 */
int dcp_aes_set_key(dcp_alg_ctx_t *ctx, const aes_key_sel_t key_sel, uint32_t keybits);

/*
 * @brief Do AES CBC Encryption/Decryption
 * @param ctx  DCP context
 * @param mode kAesMode_Decrypt or kAesMode_Encypt
 * @param iv  Initialization vector
 * @param input buffer saved the input data
 * @param output buffer for output data
 * @param length length of the input data, in terms of bytes
 *
 * @return 0 - Success, 1 - Invalid Argument
 */
int dcp_aes_cbc_crypt(
    dcp_alg_ctx_t *ctx, aes_mode_t mode, uint8_t *iv, uint8_t *input, uint8_t *output, uint32_t length);

/*
 * @brief Do AES ECB Encryption/Decryption
 * @param ctx  DCP context
 * @param mode kAesMode_Decrypt or kAesMode_Encypt
 * @param input buffer saved the input data
 * @param output buffer for output data
 * @param length length of the input data, in terms of bytes
 *
 * @return 0 - Success, 1 - Invalid Argument
 */
int dcp_aes_ecb_crypt(dcp_alg_ctx_t *ctx, aes_mode_t mode, uint8_t *input, uint8_t *output, uint32_t length);

/*
 * @brief Do AES CTR Encryption/Decryption
 * @param ctx  DCP context
 * @param nonce_counter Intialization vector
 * @param input buffer saved the input data
 * @param output buffer for output data
 * @param length length of the input data, in terms of bytes
 *
 * @return 0 - Success, 1 - Invalid Argument
 */
int dcp_aes_ctr_crypt(dcp_alg_ctx_t *ctx, uint8_t *nonce_counter, uint8_t *input, uint8_t *output, uint32_t length);

/*
 * @brief Do AES CCM Encryption
 * @param ctx  DCP context
 * @param input_len Message length in bytes
 * @param iv Initialization vector
 * @param iv_len length of Intialization vector
 * @param aad Associated data
 * @param aad_len length of Associated data
 * @param input Input data
 * @param output buffer to hold output data
 * @param mac buffer to hold MAC
 * @param mac_len length of mac
 *
 * @return 0 - Success, 1 - Invalid Argument
 */
int dcp_aes_ccm_encrypt_and_mac(dcp_alg_ctx_t *ctx,
                                uint32_t input_len,
                                const uint8_t *iv,
                                uint32_t iv_len,
                                const uint8_t *aad,
                                uint32_t aad_len,
                                const uint8_t *input,
                                uint8_t *output,
                                uint8_t *mac,
                                uint32_t mac_len);

/*
 * @brief Do AES CCM Decryption
 * @param ctx  DCP context
 * @param input_len Message length in bytes
 * @param iv Initialization vector
 * @param iv_len length of Intialization vector
 * @param aad Associated data
 * @param aad_len length of Associated data
 * @param input Input data
 * @param output buffer to hold output data
 * @param mac buffer to hold MAC
 * @param mac_len length of mac
 *
 * @return 0 - Success, 1 - Invalid Argument, 2 - Invalid MAC
 */
int dcp_aes_ccm_auth_decrypt(dcp_alg_ctx_t *ctx,
                             uint32_t input_len,
                             const uint8_t *iv,
                             uint32_t iv_len,
                             const uint8_t *aad,
                             uint32_t aad_len,
                             const uint8_t *input,
                             uint8_t *output,
                             const uint8_t *mac,
                             uint32_t mac_len);

/*
 * @brief Clear DCP aes context
 * @param ctx  DCP context
 *
 * @return 0 - Success, 1 - Invalid Argument
 */
int dcp_aes_exit(dcp_alg_ctx_t *ctx);

/*
 * @brief DCP entry, enable DCP and reset DCP controller
 */
void dcp_entry(void);

/*
 * @brief DCP exit, reset DCP controller
 */
void dcp_exit(void);

#if defined(__cplusplus)
}
#endif

#endif // __FSL_DCP_H__
