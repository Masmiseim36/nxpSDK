/*--------------------------------------------------------------------------*/
/* Copyright 2020, 2022-2023 NXP                                            */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example.c
 * @brief Example AES-128 CBC encryption using the ELS (CLNS component mcuxClEls
 *
 * @example mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example.c
 * @brief   Example AES-128 CBC encryption using the ELS (CLNS component mcuxClEls)
 */

#include <mcuxClToolchain.h>
#include <mcuxClEls.h> // Interface to the entire mcuxClEls component
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClCore_Examples.h>
#include <mcuxClExample_ELS_Helper.h>

/** Plaintext input for the AES encryption. */
static ALIGNED uint8_t const aes128_input[MCUXCLELS_CIPHER_BLOCK_SIZE_AES] = {0x6BU, 0xC1U, 0xBEU, 0xE2U,
                                                                             0x2EU, 0x40U, 0x9FU, 0x96U,
                                                                             0xE9U, 0x3DU, 0x7EU, 0x11U,
                                                                             0x73U, 0x93U, 0x17U, 0x2AU};

/** IV of the AES encryption. */
static ALIGNED uint8_t aes128_iv[MCUXCLELS_CIPHER_BLOCK_SIZE_AES] = {0xF8U, 0xD2U, 0x68U, 0x76U,
                                                                    0x81U, 0x6FU, 0x0FU, 0xBAU,
                                                                    0x86U, 0x2BU, 0xD8U, 0xA3U,
                                                                    0x2DU, 0x04U, 0x67U, 0xC3U};

/** Expected ciphertext output of the AES encryption. */
static ALIGNED uint8_t const aes128_expected_output[MCUXCLELS_CIPHER_BLOCK_SIZE_AES] = {0xCAU, 0xEAU, 0x07U, 0x26U,
                                                                                       0x62U, 0xE2U, 0x20U, 0x06U,
                                                                                       0x2DU, 0x45U, 0x46U, 0x41U,
                                                                                       0x5EU, 0xFFU, 0xFAU, 0xD2U};

/** Key for the AES encryption. */
static uint32_t const aes128_key[MCUXCLELS_CIPHER_KEY_SIZE_AES_128 / sizeof(uint32_t)] = {0x16157E2BU, 0xA6D2AE28U, 0x8815F7ABU, 0x3C4FCF09U};

/** Destination buffer to receive the ciphertext output of the AES encryption. */
static ALIGNED uint8_t aes128_output[MCUXCLELS_CIPHER_BLOCK_SIZE_AES];


/** Performs AES-128 CBC encryption using mcuxClEls functions.
 * @retval MCUXCLEXAMPLE_STATUS_OK     The example code completed successfully
 * @retval MCUXCLEXAMPLE_STATUS_ERROR  The example code failed */
MCUXCLEXAMPLE_FUNCTION(mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example)
{
    /** Initialize ELS, Enable the ELS **/
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
        
    mcuxClEls_CipherOption_t cipher_options = {0U};                        // Initialize a new configuration for the planned mcuxClEls_Cipher_Async operation.
    cipher_options.bits.cphmde = MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_CBC;  // Configure the AES block cipher mode of operation to be the CBC (Cipher Block Chaining) mode.
    cipher_options.bits.dcrpt = MCUXCLELS_CIPHER_ENCRYPT;                  // Configure that the mcuxClEls_Cipher_Async operation shall perform  encryption.
    cipher_options.bits.extkey = MCUXCLELS_CIPHER_EXTERNAL_KEY;            // Configure that an external key should be used.
    
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Cipher_Async(   // Perform the encryption.
            cipher_options,                                                 // Set the prepared configuration.
            (mcuxClEls_KeyIndex_t) 0U,                                       // This parameter (keyIdx) is ignored, since an external key is used.
            (const uint8_t *) aes128_key, MCUXCLELS_CIPHER_KEY_SIZE_AES_128,   // The AES key for the encryption (external key).
            aes128_input, sizeof(aes128_input),                             // The plaintext to encrypt. Note that this plaintext's length is a multiple of the block length, so no padding is required.
            aes128_iv,                                                      // The IV (Initialization Vector) used for the encryption.
            aes128_output                                                   // Output buffer, which the operation will write the ciphertext to.
            ));
    // mcuxClEls_Cipher_Async is a flow-protected function: Check the protection token and the return value
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Cipher_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;// Expect that no error occurred, meaning that the mcuxClEls_Cipher_Async operation was started.
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR)); // Wait for the mcuxClEls_Cipher_Async operation to complete.
    // mcuxClEls_WaitForOperation is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
        
    for (size_t i = 0U; i < sizeof(aes128_output); i++)
    {
        if (aes128_output[i] != aes128_expected_output[i]) 
        {
            return MCUXCLEXAMPLE_STATUS_ERROR;// Expect that the resulting ciphertext matches our expected output
        }
    }
    
    /** Disable the ELS **/
    if(!mcuxClExample_Els_Disable())
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    
    return MCUXCLEXAMPLE_STATUS_OK;
}
