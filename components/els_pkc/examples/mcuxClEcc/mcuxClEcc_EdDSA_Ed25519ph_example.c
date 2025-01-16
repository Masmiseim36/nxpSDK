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
 * @file  mcuxClEcc_EdDSA_Ed25519ph_example.c
 * @brief Example for the mcuxClEcc component
 *
 * @example mcuxClEcc_EdDSA_Ed25519ph_example.c
 * @brief   Example for the mcuxClEcc component EdDsa related functions
 */

#include <mcuxClToolchain.h>
#include <mcuxClBuffer.h>
#include <mcuxClEcc.h>
#include <mcuxClKey.h>
#include <mcuxClSession.h>
#include <mcuxClPkc_Types.h>
#include <mcuxClCore_Examples.h>
#include <mcuxClCore_Macros.h>
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClExample_RNG_Helper.h>
#include <mcuxClRandomModes.h>

#include <mcuxClExample_ELS_Helper.h>

#define MAX_CPUWA_SIZE MCUXCLCORE_MAX(MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE,                                                                     \
                                        MCUXCLCORE_MAX(MCUXCLRANDOMMODES_INIT_WACPU_SIZE,                                                      \
                                        MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WACPU_SIZE,                    \
                                        MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WACPU_SIZE, \
                                                          MCUXCLECC_EDDSA_VERIFYSIGNATURE_ED25519_WACPU_SIZE))))
#define MAX_PKCWA_SIZE MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WAPKC_SIZE,                    \
                                        MCUXCLCORE_MAX(MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WAPKC_SIZE, \
                                                         MCUXCLECC_EDDSA_VERIFYSIGNATURE_ED25519_WAPKC_SIZE))

/* Input taken from "TEST abc" from Section 7.3 of IRTF rfc 8032 */
static const ALIGNED uint8_t pMessage[] __attribute__((aligned(4))) =
{
  0x61u, 0x62u, 0x63u
};

/* Signature taken from "TEST abc" from Section 7.3 of IRTF rfc 8032 */
static const ALIGNED uint8_t pRefSignature[MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE] __attribute__((aligned(4))) =
{
  0x98u, 0xa7u, 0x02u, 0x22u, 0xf0u, 0xb8u, 0x12u, 0x1au,
  0xa9u, 0xd3u, 0x0fu, 0x81u, 0x3du, 0x68u, 0x3fu, 0x80u,
  0x9eu, 0x46u, 0x2bu, 0x46u, 0x9cu, 0x7fu, 0xf8u, 0x76u,
  0x39u, 0x49u, 0x9bu, 0xb9u, 0x4eu, 0x6du, 0xaeu, 0x41u,
  0x31u, 0xf8u, 0x50u, 0x42u, 0x46u, 0x3cu, 0x2au, 0x35u,
  0x5au, 0x20u, 0x03u, 0xd0u, 0x62u, 0xadu, 0xf5u, 0xaau,
  0xa1u, 0x0bu, 0x8cu, 0x61u, 0xe6u, 0x36u, 0x06u, 0x2au,
  0xaau, 0xd1u, 0x1cu, 0x2au, 0x26u, 0x08u, 0x34u, 0x06u
};

/* Private key taken from "TEST abc" from Section 7.3 of IRTF rfc 8032 */
static const ALIGNED uint8_t pPrivateKey[MCUXCLECC_EDDSA_ED25519_SIZE_PRIVATEKEY] __attribute__((aligned(4))) =
{
  0x83u, 0x3fu, 0xe6u, 0x24u, 0x09u, 0x23u, 0x7bu, 0x9du,
  0x62u, 0xecu, 0x77u, 0x58u, 0x75u, 0x20u, 0x91u, 0x1eu,
  0x9au, 0x75u, 0x9cu, 0xecu, 0x1du, 0x19u, 0x75u, 0x5bu,
  0x7du, 0xa9u, 0x01u, 0xb9u, 0x6du, 0xcau, 0x3du, 0x42u
};


MCUXCLEXAMPLE_FUNCTION(mcuxClEcc_EdDSA_Ed25519ph_example)
{
  /******************************************/
  /* Set up the environment                 */
  /******************************************/

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

  /******************************************/
  /* Initialize the private and public keys */
  /******************************************/

  /* Allocate space for and initialize private key handle for an Ed25519 private key */
  uint32_t privKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  mcuxClKey_Handle_t privKey = (mcuxClKey_Handle_t) privKeyDesc;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  ALIGNED uint8_t pPrivateKeyData[MCUXCLECC_EDDSA_ED25519_SIZE_PRIVATEKEYDATA];

  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(privkeyinit_result, privkeyinit_token, mcuxClKey_init(
  /* mcuxClSession_Handle_t session         */ &session,
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer privKey is of the right type (mcuxClKey_Handle_t)")
  /* mcuxClKey_Handle_t key                 */ privKey,
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
  /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Priv,
  /* uint8_t * pKeyData                    */ pPrivateKeyData,
  /* uint32_t keyDataLength                */ sizeof(pPrivateKeyData)));

  if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != privkeyinit_token) || (MCUXCLKEY_STATUS_OK != privkeyinit_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /* Allocate space for and initialize public key handle for an Ed25519 public key */
  uint32_t pubKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  mcuxClKey_Handle_t pubKey = (mcuxClKey_Handle_t) pubKeyDesc;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  ALIGNED uint8_t pPublicKeyData[MCUXCLECC_EDDSA_ED25519_SIZE_PUBLICKEY];

  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(pubkeyinit_result, pubkeyinit_token, mcuxClKey_init(
  /* mcuxClSession_Handle_t session         */ &session,
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer pubKey is of the right type (mcuxClKey_Handle_t)")
  /* mcuxClKey_Handle_t key                 */ pubKey,
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
  /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_EdDSA_Ed25519_Pub,
  /* uint8_t * pKeyData                    */ pPublicKeyData,
  /* uint32_t keyDataLength                */ sizeof(pPublicKeyData)));

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
  /* mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *mode */ (mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t *)&privKeyInputDescriptor,
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

  /******************************************/
  /* Generate the protocol descriptor       */
  /******************************************/

  /* Allocate space for the hash prefix and a protocol descriptor for Ed25519ph. */
  uint32_t protocolDescBytes[MCUXCLECC_EDDSA_ED25519_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE_IN_WORD(0u)];
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *pProtocolDesc = (mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *) protocolDescBytes;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

  /* Generate Ed25519ph protocol descriptor */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(genProtocolDescr_result, protocolDescr_token, mcuxClEcc_EdDSA_GenerateProtocolDescriptor(
  /* mcuxClSession_Handle_t pSession                       */ &session,
  /* const mcuxClEcc_EdDSA_DomainParams_t *pDomainParams   */ &mcuxClEcc_EdDSA_DomainParams_Ed25519,
  /* mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *pProtocolDescriptor */ pProtocolDesc,
  /* uint32_t phflag                                      */ MCUXCLECC_EDDSA_PHFLAG_ONE,
  MCUX_CSSL_ANALYSIS_START_PATTERN_NULL_POINTER_CONSTANT()
  /* mcuxCl_InputBuffer_t pContext                         */ NULL,
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_NULL_POINTER_CONSTANT()
  /* uint32_t contextLen                                  */ 0u));

  if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateProtocolDescriptor) != protocolDescr_token)
      || (MCUXCLECC_STATUS_OK != genProtocolDescr_result))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /**************************************************************************/
  /* Ed25519ph signature generation                                         */
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

  /**************************************************************************/
  /* Ed25519ph signature verification                                       */
  /**************************************************************************/

  /* Call mcuxClEcc_EdDSA_VerifySignature to verify the signature. */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(verify_result, verify_token, mcuxClEcc_EdDSA_VerifySignature(
  /* mcuxClSession_Handle_t pSession                       */ &session,
  /* mcuxClKey_Handle_t key                                */ pubKey,
  /* const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t * */ pProtocolDesc,
  /* mcuxCl_InputBuffer_t pIn                              */ buffIn,
  /* uint32_t inSize                                      */ sizeof(pMessage),
  /* mcuxCl_InputBuffer_t pSignature                       */ buffSignature,
  /* uint32_t signatureSize                               */ signatureSize
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
