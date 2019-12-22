/**
 * @file HostCryptoAPI.h
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
 * Host Crypto wrapper API for the A7-series security IC's
 *
 * @par HISTORY
 *
 */

#ifndef _HOST_CRYPTO_API_H_
#define _HOST_CRYPTO_API_H_

#include "HLSETypes.h"


#define HOST_CRYPTO_NOT_SUPPORTED -1  //!< Crypto engine does not support implementation
#define HOST_CRYPTO_ERROR          0  //!< Failure
#define HOST_CRYPTO_OK             1  //!< Success


/**
* Enumerates all the Cryptographic Mechanisms that are supported. A list of mechanisms is returned in \p mechanisms.

If \p mechanism is NULL, then all that the function does is return (in \p *mechanismNum) the number of HLSE_MECHANISM_TYPE which would suffice
to hold the returned list.  HLSE_SW_OK is returned by the function.

If \p mechanism is not NULL, then \p *mechanismLen must contain the number of mechanisms in the buffer \p mechanism.  If that buffer
is large enough to hold number of mechanisms to be returned, then the mechanisms are copied to \p mechanism, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *mechanismLen is set to hold the exact number of
mechanisms to be returned.

* \param[in, out] mechanism IN: caller passes a buffer of at least *mechanismNum; OUT: contains the mechanisms supported
* \param[in, out] mechanismLen IN: number of mechanisms in mechanisms; OUT: set to hold the exact number of mechanisms
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL Buffer is too small to return the mechanisms
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLCRYPT_GetSupportedMechanisms(HLSE_MECHANISM_TYPE* mechanism, U32* mechanismLen);


/**
* Encrypts the data provided using the given key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
The key to encrypt the data with is provided by \p inKey.

If additional information is required by the specific encryption mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outEncryptedData is NULL, then all that the function does is return (in \p *outEncryptedDataLen) a number of bytes which would suffice
to hold the return value.  HLSE_SW_OK is returned by the function.

If \p outEncryptedData is not NULL, then \p *outEncryptedDataLen must contain the number of bytes in the buffer \p outEncryptedData.  If that buffer
is large enough to hold the data be returned, then the data is copied to \p outEncryptedData, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outEncryptedDataLen is set to hold the exact number of
bytes to be returned.

* \param[in] pMechanismType The encryption Cryptographic Mechanism to be used
* \param[in] inKey The key data to encrypt with
* \param[in] inKeyLen The length in bytes of \p inKey
* \param[in] inData    Data buffer for that should be encrypted
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outEncryptedData    IN: caller passes a buffer to hold the data to be returned;
OUT: contains the encrypted data
* \param[in,out] outEncryptedDataLen IN: length of the \p outEncryptedData buffer passed;
OUT: the number of bytes returned in \p outEncryptedData
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outEncryptedData is too small to return the data
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLCRYPT_Encrypt(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
                                U8* inData, U32 inDataLen,
                                U8* outEncryptedData, U32* outEncryptedDataLen);

/**
* Decrypts the data provided using the given key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
A handle to the key to decrypt the data with is provided by \p hObject.

If additional information is required by the specific decryption mechanism, is will be conveyed in \p pMechanismType->pParameter.

If \p outDecryptedData is NULL, then all that the function does is return (in \p *outDecryptedDataLen) a number of bytes which would suffice
to hold the return value.  HLSE_SW_OK is returned by the function.

If \p outData is not NULL, then \p *outDecryptedDataLen must contain the number of bytes in the buffer \p outDecryptedData.  If that buffer
is large enough to hold the data be returned, then the data is copied to \p outDecryptedData, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outDecryptedDataLen is set to hold the exact number of
bytes to be returned.

* \param[in] pMechanismType The decryption Cryptographic Mechanism to be used
* \param[in] inKey The key data to decrypt with
* \param[in] inKeyLen The length in bytes of \p inKey
* \param[in] inData    Data buffer for that should be decrypted
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outDecryptedData    IN: caller passes a buffer to hold the data to be returned;
OUT: contains the decrypted data
* \param[in,out] outDecryptedDataLen IN: length of the \p outDecryptedData buffer passed;
OUT: the number of bytes returned in \p outDecryptedData
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outData is too small to return the data
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLCRYPT_Decrypt(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
                                U8* inData, U32 inDataLen,
                                U8* outDecryptedData, U32* outDecryptedDataLen);

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
HLSE_RET_CODE   HLCRYPT_Digest(HLSE_MECHANISM_INFO* pMechanismType,
                            U8* inData, U32 inDataLen,
                            U8* outDigest, U32* outDigestLen);


/**
* Signs the data provided using the given key and the requested mechanism.

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
* \param[in] inKey The key data to sign with
* \param[in] inKeyLen The length in bytes of \p inKey
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
* \retval ::HLSE_ERR_CRYPTO_ENGINE_FAILED in case of failure
*/
HLSE_RET_CODE   HLCRYPT_Sign(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
                            U8* inData, U32 inDataLen,
                            U8* outSignature, U32* outSignatureLen);

/**
* Verifies the data provided using the given key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
A handle to the key to verify the data with is provided by \p hObject.

If additional information is required by the specific verifying mechanism, is will be conveyed in \p pMechanismType->pParameter.

* \param[in] pMechanismType The verifying Cryptographic Mechanism to be used
* \param[in] inKey The key data to verify with
* \param[in] inKeyLen The length in bytes of \p inKey
* \param[in] inData    The mesage hash
* \param[in] inDataLen The length of hash
* \param[in] inSignature    IN: caller passes a buffer with signature to read and verify;
* \param[in] inSignatureLen IN: length of the \p inSignature buffer passed;
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outSignature is too small to return the signature
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
* \retval ::HLSE_ERR_CRYPTO_ENGINE_FAILED in case of failure
*/
HLSE_RET_CODE   HLCRYPT_Verify(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
                            U8* inData, U32 inDataLen,
                            U8* inSignature, U32 inSignatureLen);

/**
* Initialize a Signature operation using the given key and the requested mechanism.

The Cryptographic Mechanism to be used is passed in the \p type member of the \p pMechanismType parameter.
A handle to the key to sign the data with is provided by \p hObject.

If additional information is required by the specific signing mechanism, is will be conveyed in \p pMechanismType->pParameter.

* \param[in] pMechanismType The signing Cryptographic Mechanism to be used
* \param[in] inKey The key data to sign with
* \param[in] inKeyLen The length in bytes of \p inKey
* \param[out] hContext Pointer to context (double indirection)
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outSignature is too small to return the signature
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLCRYPT_SignInit(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen, HLSE_CONTEXT_HANDLE* hContext);

/**
* Update/Append to data to be signed.

* \param[in] hContext The Context handle
* \param[in] inDataPart    Data buffer for that should be signed (e.g. a digest)
* \param[in] inDataPartLen The length of data passed as argument
*
* \retval ::HOST_CRYPTO_OK
* \retval ::HOST_CRYPTO_ERROR
*/
HLSE_RET_CODE   HLCRYPT_SignUpdate(HLSE_CONTEXT_HANDLE hContext, U8* inDataPart, U32 inDataPartLen);

/**
* Retrieves the Signature and clean up/free the context handle.

If \p outSignature is NULL, then all that the function does is return (in \p *outSignatureLen) a number of bytes which would suffice
to hold the signature.  HLSE_SW_OK is returned by the function.

If \p outSignature is not NULL, then \p *outSignatureLen must contain the number of bytes in the buffer \p outSignature.  If that buffer
is large enough to hold the signature be returned, then the data is copied to \p outSignature, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outSignatureLen is set to hold the exact number of
bytes to be returned.

* @param[in]  hContext The Context handle
* \param[in,out] outSignature    IN: caller passes a buffer to hold the signature;
OUT: contains the calculated signature
* \param[in,out] outSignatureLen IN: length of the \p outSignature buffer passed;
OUT: the number of bytes returned in \p outSignature
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outSignature is too small to return the signature
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
* \retval ::HOST_CRYPTO_OK
* \retval ::HOST_CRYPTO_ERROR
*/
HLSE_RET_CODE   HLCRYPT_SignFinal(HLSE_CONTEXT_HANDLE hContext, U8* outSignature, U32* outSignatureLen);

/**
* Fill up the provided buffer \p pRandomData with \p inLen byte of random data
* \param[in]     inLen    number (in bytes) of random data requested
* \param[in,out] pRandom  IN: buffer of at least inLen size; OUT: retrieved random data
*
* \retval ::HOST_CRYPTO_OK      Success
* \retval ::HOST_CRYPTO_ERROR   Random not supported by Crypto Engine
* \retval ::RET_SUCCESS
* \retval ::RET_FAILURE
* \retval ::RET_INSUFFICIENT_BUFFER_SIZE
*/
HLSE_RET_CODE    HLCRYPT_GetRandom(U32 inLen, U8 * pRandom);

/**
* Encrypts the data provided using the given key using single Des CBC mechanism.

If \p outData is NULL, then all that the function does is return (in \p *outEncryptedDataLen) a number of bytes which would suffice
to hold the return value.  HLSE_SW_OK is returned by the function.

If \p outData is not NULL, then \p *outDataLen must contain the number of bytes in the buffer \p outData.  If that buffer is large enough to hold the 
data be returned, then the data is copied to \p outData, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *outDataLen is set to hold the exact number of
bytes to be returned.

* \param[in] Key The key data to encrypt with
* \param[in] KeyLen The length in bytes of \p inKey
* \param[in] iv ICV to be used by CBC mechanism
* \param[in] ivlen The length in bytes of \p iv
* \param[in] inData    Data buffer for that should be encrypted
* \param[in] inDataLen The length of data passed as argument
* \param[in,out] outData    IN: caller passes a buffer to hold the data to be returned;
OUT: contains the encrypted data
* \param[in,out] outDataLen IN: length of the \p outEncryptedData buffer passed;
OUT: the number of bytes returned in \p outEncryptedData
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p outEncryptedData is too small to return the data
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLCRYPT_Single_DES_CBC_Encrypt(U8 *key, U32 keylen, 
             U8 *iv, 
             U16 ivlen, 
             U8 *inData, 
             U32 inDatalen, 
             U8 * outData,
             U32 *outDatalen);
#endif
