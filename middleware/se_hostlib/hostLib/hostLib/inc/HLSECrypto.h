/**
* @file HLSECrypto.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2017 NXP
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
* Host Lib wrapper API: Cryptographic functions
*
* @par HISTORY
*
*/
#ifndef _HLSE_CRYPTO_H_
#define _HLSE_CRYPTO_H_

#include "HLSETypes.h"

// HLSE stands for: Host Library Secure Element


/**
* Enumerates all the Cryptographic Mechanisms that are supported by the library. A list of mechanisms is returned in \p mechanisms.

If \p mechanisms is NULL, then all that the function does is return (in \p *mechanismNum) the number of HLSE_MECHANISM_TYPE which would suffice
to hold the returned list.  HLSE_SW_OK is returned by the function.

If \p mechanisms is not NULL, then \p *mechanismNum must contain the number of mechanisms in the buffer \p mechanisms.  If that buffer
is large enough to hold number of mechanisms to be returned, then the mechanisms are copied to \p mechanisms, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *mechanismNum is set to hold the exact number of
mechanisms to be returned.

* \param[in, out] mechanisms IN: caller passes a buffer of at least *mechanismNum; OUT: contains the mechanisms supported
* \param[in, out] mechanismNum IN: number of mechanisms in mechanisms; OUT: set to hold the exact number of mechanisms
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL Buffer is too small to return the mechanisms
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_GetSupportedMechanisms(HLSE_MECHANISM_TYPE* mechanisms, U16* mechanismNum);

/**
* Enumerates all the Cryptographic Mechanisms that are supported by the Object. A list of mechanisms is returned in \p mechanisms.

If \p mechanism is NULL, then all that the function does is return (in \p *mechanismLen) the number of HLSE_MECHANISM_TYPE which would suffice
to hold the returned list.  HLSE_SW_OK is returned by the function.

If \p mechanism is not NULL, then \p *mechanismLen must contain the number of mechanisms in the buffer \p mechanisms.  If that buffer
is large enough to hold number of mechanisms to be returned, then the mechanisms are copied to \p mechanisms, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *mechanismLen is set to hold the exact number of
mechanisms to be returned.

* \param[in] hObject The handle of the Object that the Mechanisms it supports should be returned
* \param[in, out] mechanism IN: caller passes a buffer of at least *mechanismNum; OUT: contains the mechanisms supported
* \param[in, out] mechanismLen IN: number of mechanisms in mechanisms. OUT: set to hold the exact number of mechanisms
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL Buffer is too small to return the mechanisms
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_GetSupportedMechanismsForObject(HLSE_OBJECT_HANDLE hObject, HLSE_MECHANISM_TYPE* mechanism, U16* mechanismLen);

/**
* Calculates the Digest (e.g. Sha256) value of the data provided as input.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.

If additional information is required by the specific digest mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outDigest is NULL, then all that the function does is return (in \p *outDigestLen) a number of bytes which would suffice
to hold the digest value.  HLSE_SW_OK is returned by the function.

If \p outDigest is not NULL, then \p *outDigestLen must contain the number of bytes in the buffer \p outDigest.  If that buffer
is large enough to hold the digest value be returned, then the data is copied to \p outDigest, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outDigestLen is set to hold the exact number of
bytes to be returned.

* \param[in] pMechanismType The Digest Cryptographic Mechanism to be used
* \param[in] inData    Data buffer for which the digest must be calculated
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outDigest    IN: caller passes a buffer to hold the digest value;
OUT: contains the calculated digest
* \param[in,out] outDigestLen IN: length of the \p outDigest buffer passed;
OUT: the number of bytes returned in \p outDigest
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outDigest is too small to return the digest
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_Digest(HLSE_MECHANISM_INFO* pMechanismType,
                            U8* inData, U16 inDataLen,
                            U8* outDigest, U16* outDigestLen);

/**
* Signs the data provided using the Object key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
A handle to the key to sign the data with is provided by \p hObject.

If additional information is required by the specific signing mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outSignature is NULL, then all that the function does is return (in \p *outSignatureLen) a number of bytes which would suffice
to hold the signature.  HLSE_SW_OK is returned by the function.

If \p outSignature is not NULL, then \p *outSignatureLen must contain the number of bytes in the buffer \p outSignature.  If that buffer
is large enough to hold the signature be returned, then the data is copied to \p outSignature, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outSignatureLen is set to hold the exact number of
bytes to be returned.

* \param[in] pMechanismType The signing Cryptographic Mechanism to be used
* \param[in] hObject The handle of the Object key to sign with
* \param[in] inData    Data buffer for that should be signed (e.g. a digest)
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outSignature    IN: caller passes a buffer to hold the signature;
OUT: contains the calculated signature
* \param[in,out] outSignatureLen IN: length of the \p outSignature buffer passed;
OUT: the number of bytes returned in \p outSignature
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outSignature is too small to return the signature
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_Sign(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                            U8* inData, U16 inDataLen,
                            U8* outSignature, U16* outSignatureLen);


/**
* Verifies whether \p inSignature is the signature of \p inData
using the public key object referenced by \p hObject as the verifying public key.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.

* \param[in] pMechanismType The signing Cryptographic Mechanism that was used
* \param[in] hObject The handle of the Object public key to verify with
* \param[in] inData    The data that was signed (e.g. a digest)
* \param[in] inDataLen The length of data passed as argument
* \param[in] inSignature Pointer to the provided signature.
* \param[in] inSignatureLen Length of the provided signature (\p pSignature)
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_GENERAL_ERROR if the verification fails
*/
HLSE_RET_CODE   HLSE_VerifySignature(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                                    U8* inData, U16 inDataLen,
                                    U8* inSignature, U16 inSignatureLen);

/**
* Verifies whether \p inSignature is the signature of \p inData
using an external public key object as the verifying public key.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.

* \param[in] pMechanismType The signing Cryptographic Mechanism that was used
* \param[in] inExtKey The value of the external public key to verify with
* \param[in] inExtKeyLen The length in bytes of the external key
* \param[in] inData    The data that was signed (e.g. a digest)
* \param[in] inDataLen The length of data passed as argument
* \param[in] inSignature Pointer to the provided signature.
* \param[in] inSignatureLen Length of the provided signature (\p pSignature)
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_GENERAL_ERROR if the verification fails
*/
HLSE_RET_CODE   HLSE_VerifySignatureWithExternalKey(HLSE_MECHANISM_INFO* pMechanismType,
                                                    U8* inExtKey, U16 inExtKeyLen,
                                                    U8* inData, U16 inDataLen,
                                                    U8* inSignature, U16 inSignatureLen);

/**
* Derives the key referenced by the \p hObject handle using the requested mechanism and return
the derived key in \p outDerivedKey.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.

If additional information is required by the specific signing mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outDerivedKey is NULL, then all that the function does is return (in \p *outDerivedKeyLen) a number of bytes which would suffice
to hold the derived key.  HLSE_SW_OK is returned by the function.

If \p outDerivedKey is not NULL, then \p *outDerivedKeyLen must contain the number of bytes in the buffer \p outDerivedKey.  If that buffer
is large enough to hold the derived key, then the data is copied to \p outDerivedKey, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outDerivedKeyLen is set to hold the exact number of
bytes of the derived key.

* \param[in] pMechanismType The signing Cryptographic Mechanism to be used
* \param[in] hObject The handle of the Object key to be derived
* \param[in,out] outDerivedKey    IN: caller passes a buffer to hold the derived key;
OUT: contains the derived key
* \param[in,out] outDerivedKeyLen IN: length of the \p outDerivedKey buffer passed;
OUT: the number of bytes returned in \p outDerivedKey
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outDerivedKey is too small to return the derived key
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_DeriveKey(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                                U8* outDerivedKey, U16* outDerivedKeyLen);

/**
* Encrypts the data provided using the Object key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
A handle to the key to encrypt the data with is provided by \p hObject.

If additional information is required by the specific encryption mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outData is NULL, then all that the function does is return (in \p *outDataLen) a number of bytes which would suffice
to hold the return value.  HLSE_SW_OK is returned by the function.

If \p outData is not NULL, then \p *outDataLen must contain the number of bytes in the buffer \p outData.  If that buffer
is large enough to hold the data be returned, then the data is copied to \p outData, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outDataLen is set to hold the exact number of
bytes to be returned.

* \param[in] pMechanismType The encryption Cryptographic Mechanism to be used
* \param[in] hObject The handle of the Object key to encrypt with
* \param[in] inData    Data buffer for that should be encrypted
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outData    IN: caller passes a buffer to hold the data to be returned;
OUT: contains the encrypted data
* \param[in,out] outDataLen IN: length of the \p outData buffer passed;
OUT: the number of bytes returned in \p outData
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outData is too small to return the data
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_Encrypt(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                            U8* inData, U16 inDataLen,
                            U8* outData, U16* outDataLen);

/**
* Decrypts the data provided using the Object key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
A handle to the key to decrypt the data with is provided by \p hObject.

If additional information is required by the specific decryption mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outData is NULL, then all that the function does is return (in \p *outDataLen) a number of bytes which would suffice
to hold the return value.  HLSE_SW_OK is returned by the function.

If \p outData is not NULL, then \p *outDataLen must contain the number of bytes in the buffer \p outData.  If that buffer
is large enough to hold the data be returned, then the data is copied to \p outData, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outDataLen is set to hold the exact number of
bytes to be returned.

* \param[in] pMechanismType The decryption Cryptographic Mechanism to be used
* \param[in] hObject The handle of the Object key to decrypt with
* \param[in] inData    Data buffer for that should be decrypted
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outData    IN: caller passes a buffer to hold the data to be returned;
OUT: contains the decrypted data
* \param[in,out] outDataLen IN: length of the \p outData buffer passed;
OUT: the number of bytes returned in \p outData
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outData is too small to return the data
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_Decrypt(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                            U8* inData, U16 inDataLen,
                            U8* outData, U16* outDataLen);


#endif // _HLSE_CRYPTO_H_
