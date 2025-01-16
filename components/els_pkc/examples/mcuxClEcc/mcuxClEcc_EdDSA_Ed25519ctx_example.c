/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
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
 * @file  mcuxClEcc_EdDSA_Ed25519ctx_example.c
 * @brief Example for the mcuxClEcc component
 *
 * @example mcuxClEcc_EdDSA_Ed25519ctx_example.c
 * @brief   Example for the mcuxClEcc component EdDsa related functions
 */


#include <mcuxClToolchain.h>
#include <mcuxClBuffer.h>
#include <mcuxClRandomModes.h>
#include <mcuxClEcc.h>
#include <mcuxClKey.h>
#include <mcuxClPkc_Types.h>
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClCore_Examples.h>
#include <mcuxClCore_Macros.h>
#include <mcuxClExample_RNG_Helper.h>

#include <mcuxClExample_ELS_Helper.h>

#define MAX_CPUWA_SIZE MCUXCLCORE_MAX(MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE,                                  \
                                        MCUXCLCORE_MAX(MCUXCLRANDOMMODES_INIT_WACPU_SIZE,                   \
                                        MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WACPU_SIZE,  \
                                        MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WACPU_SIZE,\
                                        MCUXCLECC_EDDSA_VERIFYSIGNATURE_ED25519_WACPU_SIZE))))

#define MAX_PKCWA_SIZE MCUXCLCORE_MAX(MCUXCLCORE_MAX(\
                                        MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WAPKC_SIZE,\
                                        MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WAPKC_SIZE),\
                                        MCUXCLECC_EDDSA_VERIFYSIGNATURE_ED25519_WAPKC_SIZE)

/* Input taken from "foo" from Section 7.2 of IRTF rfc 8032 */
static const ALIGNED uint8_t pMessage[] =
{
    0xf7u, 0x26u, 0x93u, 0x6du, 0x19u, 0xc8u, 0x00u, 0x49u,
    0x4eu, 0x3fu, 0xdau, 0xffu, 0x20u, 0xb2u, 0x76u, 0xa8u,
};

/* Context taken from "foo" from Section 7.2 of IRTF rfc 8032 */
static const ALIGNED uint8_t pContext[] =
{
    0x66u, 0x6fu, 0x6fu
};

/* Signature taken from "foo" from Section 7.2 of IRTF rfc 8032 */
static const ALIGNED uint8_t pRefSignature[MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE] =
{
    0x55u, 0xa4u, 0xccu, 0x2fu, 0x70u, 0xa5u, 0x4eu, 0x04u,
    0x28u, 0x8cu, 0x5fu, 0x4cu, 0xd1u, 0xe4u, 0x5au, 0x7bu,
    0xb5u, 0x20u, 0xb3u, 0x62u, 0x92u, 0x91u, 0x18u, 0x76u,
    0xcau, 0xdau, 0x73u, 0x23u, 0x19u, 0x8du, 0xd8u, 0x7au,
    0x8bu, 0x36u, 0x95u, 0x0bu, 0x95u, 0x13u, 0x00u, 0x22u,
    0x90u, 0x7au, 0x7fu, 0xb7u, 0xc4u, 0xe9u, 0xb2u, 0xd5u,
    0xf6u, 0xccu, 0xa6u, 0x85u, 0xa5u, 0x87u, 0xb4u, 0xb2u,
    0x1fu, 0x4bu, 0x88u, 0x8eu, 0x4eu, 0x7eu, 0xdbu, 0x0du
};

/* Private key taken from "foo" from Section 7.2 of IRTF rfc 8032 */
static const ALIGNED uint8_t pPrivateKey[MCUXCLECC_EDDSA_ED25519_SIZE_PRIVATEKEY] =
{
    0x03u, 0x05u, 0x33u, 0x4eu, 0x38u, 0x1au, 0xf7u, 0x8fu,
    0x14u, 0x1cu, 0xb6u, 0x66u, 0xf6u, 0x19u, 0x9fu, 0x57u,
    0xbcu, 0x34u, 0x95u, 0x33u, 0x5au, 0x25u, 0x6au, 0x95u,
    0xbdu, 0x2au, 0x55u, 0xbfu, 0x54u, 0x66u, 0x63u, 0xf6u
};


MCUXCLEXAMPLE_FUNCTION(mcuxClEcc_EdDSA_Ed25519ctx_example)
{
  /**************************************************************************/
  /* Preparation                                                            */
  /**************************************************************************/

  /* Initialize ELS, Enable the ELS */
  if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /* Setup one session to be used by all functions called */
  mcuxClSession_Descriptor_t session;
  /* Allocate and initialize session with pkcWA on the beginning of PKC RAM */
  MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(&session, MAX_CPUWA_SIZE, MAX_PKCWA_SIZE);

  /* Initialize the RNG context and Initialize the PRNG */
  MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_RNG(&session, 0u, mcuxClRandomModes_Mode_ELS_Drbg);

  /* Allocate space for and initialize private key handle for an Ed25519 private key */
  uint32_t privKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  mcuxClKey_Handle_t privKey = (mcuxClKey_Handle_t) privKeyDesc;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  ALIGNED uint8_t pPrivKeyData[MCUXCLECC_EDDSA_ED25519_SIZE_PRIVATEKEYDATA];

  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(privkeyinit_result, privkeyinit_token, mcuxClKey_init(
  /* mcuxClSession_Handle_t session         */ &session,
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer privKey is of the right type (mcuxClKey_Handle_t)")
  /* mcuxClKey_Handle_t key                 */ privKey,
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
  /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Priv,
  /* uint8_t * pKeyData                    */ pPrivKeyData,
  /* uint32_t keyDataLength                */ sizeof(pPrivKeyData)));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != privkeyinit_token) || (MCUXCLKEY_STATUS_OK != privkeyinit_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /* Allocate space for and initialize public key handle for an Ed25519 public key */
  uint32_t pubKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  mcuxClKey_Handle_t pubKey = (mcuxClKey_Handle_t) pubKeyDesc;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  ALIGNED uint8_t pPubKeyData[MCUXCLECC_EDDSA_ED25519_SIZE_PUBLICKEY];

  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(pubkeyinit_result, pubkeyinit_token, mcuxClKey_init(
  /* mcuxClSession_Handle_t session         */ &session,
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer pubKey is of the right type (mcuxClKey_Handle_t)")
  /* mcuxClKey_Handle_t key                 */ pubKey,
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
  /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Pub,
  /* uint8_t * pKeyData                    */ pPubKeyData,
  /* uint32_t keyDataLength                */ (uint32_t)sizeof(pPubKeyData)));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != pubkeyinit_token) || (MCUXCLKEY_STATUS_OK != pubkeyinit_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /* Allocate space for and initialize EdDSA key pair generation descriptor for private key input */
  uint32_t privKeyInputDescriptor[MCUXCLECC_EDDSA_GENERATEKEYPAIR_DESCRIPTOR_SIZE_IN_WORDS];
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(initmode_result, initmode_token, mcuxClEcc_EdDSA_InitPrivKeyInputMode(
  /* mcuxClSession_Handle_t pSession                   */ &session,
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  /* mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *mode */ (mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *) &privKeyInputDescriptor,
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  /* const uint8_t *pPrivKey                          */ pPrivateKey));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_InitPrivKeyInputMode) != initmode_token) || (MCUXCLECC_STATUS_OK != initmode_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /**************************************************************************/
  /* Key pair generation for EdDSA on Ed25519                               */
  /**************************************************************************/

  /* Call mcuxClEcc_EdDSA_GenerateKeyPair to derive the public key from the private one. */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keygen_result, keygen_token, mcuxClEcc_EdDSA_GenerateKeyPair(
  /* mcuxClSession_Handle_t pSession                          */ &session,
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  /* const mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *mode  */ (mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *) &privKeyInputDescriptor,
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  /* mcuxClKey_Handle_t privKey                               */ privKey,
  /* mcuxClKey_Handle_t pubKey                                */ pubKey));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateKeyPair) != keygen_token) || (MCUXCLECC_STATUS_OK != keygen_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /**************************************************************************/
  /* Generate the mode and protocol descriptor                              */
  /**************************************************************************/

  MCUXCLBUFFER_INIT_RO(buffContext, NULL, pContext, sizeof(pContext));

  /* Allocate space for the hash prefix and a protocol descriptor for Ed25519ctx. */
  uint32_t protocolDescBytes[MCUXCLECC_EDDSA_ED25519_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE_IN_WORD(sizeof(pContext))];
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *pProtocolDesc = (mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *) protocolDescBytes;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

  /* Generate Ed25519ctx protocol descriptor */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(genProtocolDescr_result, protocolDescr_token, mcuxClEcc_EdDSA_GenerateProtocolDescriptor(
  /*  mcuxClSession_Handle_t pSession                      */ &session,
  /* const mcuxClEcc_EdDSA_DomainParams_t *pDomainParams   */ &mcuxClEcc_EdDSA_DomainParams_Ed25519,
  /* mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *pProtocolDescriptor */ pProtocolDesc,
  /* uint32_t phflag                                      */ MCUXCLECC_EDDSA_PHFLAG_ZERO,
  /* mcuxCl_InputBuffer_t pContext                         */ buffContext,
  /* uint32_t contextLen                                  */ sizeof(pContext)));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateProtocolDescriptor) != protocolDescr_token)
      || (MCUXCLECC_STATUS_OK != genProtocolDescr_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /**************************************************************************/
  /* Ed25519ctx signature generation                                        */
  /**************************************************************************/
  ALIGNED uint8_t signature[MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE] = {0u};
  uint32_t signatureSize = 0u;
  MCUXCLBUFFER_INIT_RO(buffIn, NULL, pMessage, sizeof(pMessage));
  MCUXCLBUFFER_INIT(buffSignature, NULL, signature, MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE);

  /* Call mcuxClEcc_EdDSA_GenerateSignature to generate the signature. */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sign_result, sign_token, mcuxClEcc_EdDSA_GenerateSignature(
  /* mcuxClSession_Handle_t pSession                           */ &session,
  /* mcuxClKey_Handle_t key                                    */ privKey,
  /* const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *mode */ pProtocolDesc,
  /* mcuxCl_InputBuffer_t pIn                                  */ buffIn,
  /* uint32_t inSize                                          */ sizeof(pMessage),
  /* mcuxCl_Buffer_t pSignature                                */ buffSignature,
  /* uint32_t *const pSignatureSize                           */ &signatureSize));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateSignature) != sign_token)
    || (MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE != signatureSize)
    || (MCUXCLECC_STATUS_OK != sign_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();


  /******************************************/
  /* Signature verification                 */
  /******************************************/

  /* Call mcuxClEcc_EdDSA_VerifySignature to verify the signature. */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(verify_result, verify_token, mcuxClEcc_EdDSA_VerifySignature(
  /* mcuxClSession_Handle_t pSession */ &session,
  /* mcuxClKey_Handle_t key          */ pubKey,
  /* const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t * */ pProtocolDesc,
  /* mcuxCl_InputBuffer_t pIn        */ buffIn,
  /* uint32_t inSize                */ sizeof(pMessage),
  /* mcuxCl_InputBuffer_t pSignature */ buffSignature,
  /* uint32_t signatureSize         */ signatureSize
  ));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_VerifySignature) != verify_token) || (MCUXCLECC_STATUS_OK != verify_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /* Verify the signature with the reference signature. */
  if(!mcuxClCore_assertEqual((const uint8_t*)signature, pRefSignature, sizeof(pRefSignature)))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /******************************************/
  /* Clean up                               */
  /******************************************/

  /* Destroy Session and cleanup Session */
  if(!mcuxClExample_Session_Clean(&session))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /* Disable the ELS */
  if(!mcuxClExample_Els_Disable())
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  return MCUXCLEXAMPLE_STATUS_OK;
}
