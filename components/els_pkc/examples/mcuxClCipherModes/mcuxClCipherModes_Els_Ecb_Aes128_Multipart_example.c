/*--------------------------------------------------------------------------*/
/* Copyright 2022-2024 NXP                                                  */
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

#include <mcuxClToolchain.h>
#include <mcuxClCore_Examples.h> // Defines and assertions for examples
#include <mcuxClExample_ELS_Helper.h>
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClExample_Key_Helper.h>
#include <mcuxClEls.h> // Interface to the entire mcuxClEls component
#include <mcuxClSession.h> // Interface to the entire mcuxClSession component
#include <mcuxClKey.h> // Interface to the entire mcuxClKey component
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClToolchain.h> // memory segment definitions
#include <mcuxClAes.h> // Interface to AES-related definitions and types
#include <mcuxClCipher.h> // Interface to the entire mcuxClCipher component
#include <mcuxClCipherModes.h> // Interface to the entire mcuxClCipherModes component
#include <mcuxClExample_RNG_Helper.h>

/** Key for the AES encryption. */
static ALIGNED uint8_t aes128_key[MCUXCLAES_BLOCK_SIZE] = {
    0x2Bu, 0x7Eu, 0x15u, 0x16u,
    0x28u, 0xAEu, 0xD2u, 0xA6u,
    0xABu, 0xF7u, 0x15u, 0x88u,
    0x09u, 0xCFu, 0x4Fu, 0x3Cu
};

/** Plaintext input for the AES encryption. */
static ALIGNED uint8_t const msg_plain[MCUXCLAES_BLOCK_SIZE] = {
    0x6Bu, 0xC1u, 0xBEu, 0xE2u,
    0x2Eu, 0x40u, 0x9Fu, 0x96u,
    0xE9u, 0x3Du, 0x7Eu, 0x11u,
    0x73u, 0x93u, 0x17u, 0x2Au
};

/** Expected ciphertext output of the AES encryption. */
static ALIGNED uint8_t const msg_enc_expected[MCUXCLAES_BLOCK_SIZE] = {
    0x3Au, 0xD7u, 0x7Bu, 0xB4u,
    0x0Du, 0x7Au, 0x36u, 0x60u,
    0xA8u, 0x9Eu, 0xCAu, 0xF3u,
    0x24u, 0x66u, 0xEFu, 0x97u
};

MCUXCLEXAMPLE_FUNCTION(mcuxClCipherModes_Els_Ecb_Aes128_Multipart_example)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/

    /** Initialize ELS, MCUXCLELS_RESET_DO_NOT_CANCEL **/
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }


    /* Initialize session */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLEXAMPLE_MAX_WA(MCUXCLCIPHER_MAX_AES_CPU_WA_BUFFER_SIZE, MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE), 0u);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    /* Initialize the key */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t) keyDesc;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    /* Set key properties. */
    mcuxClEls_KeyProp_t key_properties;

    key_properties.word.value = 0u;
    key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128;
    MCUX_CSSL_ANALYSIS_START_PATTERN_0U_1U_ARE_UNSIGNED()
    key_properties.bits.kactv = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_0U_1U_ARE_UNSIGNED()

    /* Load key. */
    uint32_t dstData[8];
    //Initializes a key handle, Set key properties and Load key.
    if(!mcuxClExample_Key_Init_And_Load(session,
      MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer key points to an object of the right type, the cast was valid.")
                                       key,
      MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
                                       mcuxClKey_Type_Aes128,
                                       aes128_key,
                                       sizeof(aes128_key),
                                       &key_properties,
                                       dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Create a buffer for the context */
    uint32_t ctxBuf[MCUXCLCIPHER_AES_CONTEXT_SIZE_IN_WORDS];
    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    mcuxClCipher_Context_t * const ctx = (mcuxClCipher_Context_t *) ctxBuf;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    /* Declare message buffer and size. */
    ALIGNED uint8_t msg_enc[MCUXCLAES_BLOCK_SIZE];
    uint32_t msg_enc_size = 0u;

    /**************************************************************************/
    /* Init                                                                   */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_init, token_init, mcuxClCipher_init(
      /* mcuxClSession_Handle_t session:          */ session,
      MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer ctx points to an object of the right type, the cast was valid.")
      /* mcuxClCipher_Context_t * const pContext: */ ctx,
      MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
      /* mcuxClKey_Handle_t key                   */ key,
      /* mcuxClCipher_Mode_t mode:                */ mcuxClCipher_Mode_AES_ECB_Enc_NoPadding,
      MCUX_CSSL_ANALYSIS_START_PATTERN_NULL_POINTER_CONSTANT()
      /* mcuxCl_InputBuffer_t pIv:                */ NULL,
      MCUX_CSSL_ANALYSIS_STOP_PATTERN_NULL_POINTER_CONSTANT()
      /* uint32_t ivLength:                      */ 0u
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_init) != token_init) || (MCUXCLCIPHER_STATUS_OK != result_init))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();


    /**************************************************************************/
    /* Process                                                                */
    /**************************************************************************/

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_ESCAPING_LOCAL_ADDRESS("Address of msg_enc is for internal use only and does not escape")
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_proc, token_proc, mcuxClCipher_process(
      /* mcuxClSession_Handle_t session:         */ session,
      /* mcuxClCipher_Context_t * const pContext, */ ctx,
      /* mcuxCl_InputBuffer_t pIn:               */ msg_plain,
      /* uint32_t inLength:                     */ sizeof(msg_plain),
      /* mcuxCl_Buffer_t pOut:                   */ msg_enc,
      /* uint32_t * const pOutLength:           */ &msg_enc_size
    ));
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_ESCAPING_LOCAL_ADDRESS()

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_process) != token_proc) || (MCUXCLCIPHER_STATUS_OK != result_proc))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();


    /**************************************************************************/
    /* Finish                                                                 */
    /**************************************************************************/

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_ESCAPING_LOCAL_ADDRESS("Address of msg_enc is for internal use only and does not escape")
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_fin, token_fin, mcuxClCipher_finish(
      /* mcuxClSession_Handle_t session:         */ session,
      /* mcuxClCipher_Context_t * const pContext:*/ ctx,
      /* mcuxCl_Buffer_t pOut:                   */ msg_enc,
      /* uint32_t * const pOutLength:           */ &msg_enc_size
    ));
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_ESCAPING_LOCAL_ADDRESS()

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_finish) != token_fin) || (MCUXCLCIPHER_STATUS_OK != result_fin))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if(msg_enc_size != sizeof(msg_enc_expected))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the result of the encryption, compare it against the reference */
    if(!mcuxClCore_assertEqual(msg_enc, msg_enc_expected, msg_enc_size))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }


    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/

    /* Flush the key. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        return MCUXCLEXAMPLE_STATUS_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /** Destroy Session and cleanup Session **/
    if(!mcuxClExample_Session_Clean(session))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /** Disable the ELS **/
    if(!mcuxClExample_Els_Disable())
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}
