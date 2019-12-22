/**
* @file a71cl_api.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2016 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*
* @par Description
* This file provides the public interface of the A71CL module.
* @par History
* 1.0   1-oct-2016 : Initial version
*
*****************************************************************************/
#ifndef _A71CL_API_
#define _A71CL_API_

#include "scp.h"
#include "sm_api.h"
#include "sm_types.h"
#include "ax_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------ */
typedef U8 SST_Index_t;

#define A71CL_SCP02_MAX_PAYLOAD_SIZE  239  //!< Maximum size of command/response data in case SCP02 in enabled
#define A71CL_KEK_MAX_DATA_CHUNK      224 //!< Maximum size of command data in case KEK in enabled
#define A71CL_SHA256_MAX_DATA_CHUNK      (A71CL_SCP02_MAX_PAYLOAD_SIZE) //!< SHA256 specific implementation parameter

#define A71CL_CL_STRING_SIZE          12

//#define API_ONLY_ASYMMETRIC_CRYPT


typedef enum digestAlgorithm
{
  eSHA1 = 0,
  eSHA224 = 1,
  eSHA256 = 2,
  eSHA384 = 3,
  eSHA512 = 4,
  eSM3 = 5,
  eEND
} eDigestAlgorithm;

typedef enum clKeyType {
  e3DES = 0,
  eAES = 1,
  eRSA = 2,
  eRSA_CRT = 3,
  eSM2 = 4,
  eSM4 = 5,
  eSSF33 =  6,
  eSM7 = 7,
  eSM9 = 8,
  eSM1 = 9,
  eECC = 10
} eCLKeyType;

typedef enum clKeyTag {
  etag3ES = 0x40,
  etagAES = 0x41,
  etagSM4 = 0x42,
  etagSSF33 = 0x43,
  etagSM7 = 0x44,
  etagSM9 = 0x45,
  etagSM1 = 0x46,
  etagRSA_CRT_INVQ = 0x49,
  etagRSA_CRT_DP = 0x50,
  etagRSA_CRT_DQ = 0x51,
  etagSM2_W_X = 0x60,
  etagSM2_W_Y = 0x60,
  etagSM2_S = 0x62,
  etagRSA_D = 0x64,
  etagRSA_E = 0x65,
  etagRSA_N = 0x6E,
  etagRSA_CRT_P = 0x70,
  etagRSA_CRT_Q = 0x71,
  etagECC_W_X = 0x72,
  etagECC_W_Y = 0x73,
  etagECC_S = 0x74
} eCLKeyTag;

typedef enum clSymCryptMode {
  eEncrypt = 0x51,
  eDecrypt = 0x52,
  eSignMac = 0x53,
  eVerifyMac = 0x54
} eCLSymCryptMode;

typedef enum clSymCryptType {
    eDES_CBC_NOPADDING = 0x00,
    eDES_ECB_NOPADDING = 0x01,
    eAES_CBC_NOPADDING = 0x02,
    eAES_ECB_NOPADDING = 0x03,
    eDES_CBC_ISO9797_M1 = 0x04,
    eDES_CBC_ISO9797_M2 = 0x05,
    eAES_CBC_ISO9797_M1 = 0x06,
    eAES_CBC_ISO9797_M2 = 0x07,
    eSM4_CBC_NOPADDING = 0x10,
    eSM4_ECB_NOPADDING = 0x11,
    eSM7_CBC_NOPADDING = 0x12,
    eSM7_ECB_NOPADDING = 0x13,
    eSM4_CBC_ISO9797_M1 = 0x14,
    eSM4_CBC_ISO9797_M2 = 0x15,
    eSM7_CBC_ISO9797_M1 = 0x16,
    eSM7_CBC_ISO9797_M2 = 0x17
} eCLSymCryptType;

typedef enum clAsymCryptMode {
    eAsymEncrypt = 0x51,
    eAsymDecrypt = 0x52,
    eAsymSign = 0x53,
    eAsymVerifySign = 0x54
} eCLAsymCryptMode;

typedef enum clAsymCryptType {
    eRSA_NOPADDING = 0x00,
    eRSA_SHA1_PKCS1 = 0x01,
    eRSA_SHA256_PKCS1 = 0x02,
    eRSA_SHA384_PKCS1 = 0x03,
    eRSA_SHA512_PKCS1 = 0x04,
    eSM2_SM3 = 0x05,
    eECDSA = 0x06
} eCLAsymCryptType;

#ifdef WIN32
#pragma pack(push, 1)
typedef struct cl_ID {
  U16 vendor;
  U8  length;
  U8  id[A71CL_CL_STRING_SIZE];
} sCL_ID;

typedef struct CL_keyHeader
{
  U8 cLIdLen;
  U8 cLId[A71CL_CL_STRING_SIZE];
  U8 keyType;
  U8 keyId;
  // U32* keyElement;
} sCL_KeyHeader;

typedef struct CL_keyElement
{
  U8 keyElTag;
  U16 keyElLen;
  // U32* keyValue;
} sCL_KeyElement;

typedef struct CL_VendorInfo {
    U16 oemId;
    U8  version[8];
    U8  configuration[4];
    U16 availableSpace;
    U8  extentionBits[4];
} sCL_VendorInfo;

#pragma pack(pop)
#else

//#ifdef LINUX
typedef struct __attribute__((packed)) cl_ID {
  U16 vendor;
  U8  length;
  U8  id[A71CL_CL_STRING_SIZE];
} sCL_ID;

typedef struct CL_keyHeader
{
  U8 cLIdLen;
  U8 cLId[A71CL_CL_STRING_SIZE];
  U8 keyType;
  U8 keyId;
  // U32* keyElement;
} sCL_KeyHeader;

typedef struct CL_keyElement
{
  U8 keyElTag;
  U16 keyElLen;
  // U32* keyValue;
} sCL_KeyElement;

typedef struct CL_VendorInfo {
    U16 oemId;
    U8  version[8];
    U8  configuration[4];
    U16 availableSpace;
    U8  extentionBits[4];
} sCL_VendorInfo;

#endif
/* ------------------------------ */

#ifndef API_ONLY_ASYMMETRIC_CRYPT

/** \name Module functions
   @{ */
U16 CL_CalCRC(const U8* dataBuf, const U32 len, const U16 initValue);
U16 CL_GetID(U8 *idData, const U16 idDataLen);
U16 CL_GetVendorInfo(sCL_VendorInfo* vendorInfoStruct);
U16 CL_DisablePlainInjectionMode();

U16 CL_FreeReadServiceData(U8 recordNum, U8 *outData, U16* outDataLen);

U8  CL_IsAppletInAuthenticationMode();
/** @}*/

/** \name Host Authentication functions
@{ */
U16 CL_GetChallenge(U8 *random, const U8 randomLen);
U16 CL_ExternalAuthenticate(U8 *cryptogram, U16 cryptogramLen);
/** @}*/

/** \name Sensitive Data Storage functions
@{ */
U16 CL_SecurityStorage(U8 *keyData, const U16 keyDataLen);
U16 CL_SecurityStorageWithKEK(U8 *keyData, const U16 keyDataLen, U8 *kekValue, U16 kekValueLen);

U16 CL_SecurityStorageUpdateServiceData(U8 *inData, U16 inDataLen);
U16 CL_SecurityStorageReadServiceData(U8* outData, U16* outDataLen);

U16 CL_GenerateKeyPair(U8 *publicKey, const U16 keyDataLen, const U8 keyType, const U8 keyID);
U16 CL_GenerateKeyPairWithKEK(U8 *publicKey, const U16 keyDataLen,
    const U8 keyType, const U8 keyID,
    U8 *kekValue, U16 kekValueLen);

U16 CL_SetKEKValue(U8 *currentKekValue, U16 currentKekValueLen, U8 *newKekValue, U16 newKekValueLen);
U16 CL_UpdateServiceData(U8 recordNum, U8 *kekValue, U16 kekValueLen, U8 *inData, U16 inDataLen);
/** @}*/

/** \name Cryptography functions
@{ */
U16 CL_ComputeDigest(U8 *data, const U16 dataLen,
                      U8 *digest, U16 *digestLen,
                      const U8 hashType);

U16 CL_SymmetricCrypt(U8 *inData, U16 inDataLen,
                       U8 symmMode, U8 symmType, U8 keyIndex,
                       U8 *outData, U16* outDataLen);

#endif

U16 CL_AsymmetricCrypt(U8 *inData, U16 inDataLen,
    U8 asymmMode, U8 asymmType, U8 keyIndex,
    U8 *outData, U16* outDataLen);
/** @}*/

#ifndef API_ONLY_ASYMMETRIC_CRYPT
/** \name Debug functions
@{ */
U16 CL_ResetContents();
/** @}*/
#endif

// The Link Layer used to communicate between Host and Secure Element may introduce
// additional constraints on the packetsize. This constraint is set by defining MAX_CHUNK_LENGTH_LINK.
//   NOTE: In sm_apdu.h MAX_APDU_BUF_LENGTH is set. This value corresponds to the maximum APDU
//   buffer length of the Security Module.
// On memory constrained hosts these values can be reduced to a lower value.
// #define MAX_CHUNK_LENGTH_LINK    1024    // Compromise value; can be reduced to about 650
// The A71CL has been tested in combination with: SCI2C interface & SMCOM_JRCP_V1

// TODO: Have the MAX_APDU_BUF_LENGTH take precedence on MAX_CHUNK_LENGTH_LINK
// for the time being the cap on the APDU size is enforced through MAX_CHUNK_LENGTH_LINK

#if defined(SCI2C) || defined(T1oI2C)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CL applet capability
#elif defined(SPI)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CL applet capability
#elif defined(PCSC)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CL applet capability
#elif defined(TDA8029_UART)
#error "Not yet tested"
#elif defined(SMCOM_JRCP_V1)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CL applet capability
#elif defined(RJCT_VCOM)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CL applet capability
#elif defined(SMCOM_JRCP_V2)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CL applet capability

#else
#error "Define a communication layer as a preprocessor constant"
#endif

#ifdef __cplusplus
}
#endif
#endif //_A71CL_API_
