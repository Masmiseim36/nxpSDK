/**
 * @file tstHostCrypto.h
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
 *  Module implementing host based crypto functionality used in example programs.
 * This module relies on the availability of OpenSSL / mbed TLS on the Host plaform.
 * @par HISTORY
 *
 */

#ifndef _TST_HOST_CRYPTO_H_
#define _TST_HOST_CRYPTO_H_

#include "sm_types.h"

#ifdef OPENSSL
#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>
#include <openssl/ecdh.h>
#include <openssl/sha.h>
#endif /* OPENSSL */

#ifdef MBEDTLS
#include <mbedtls/ecp.h>
#include <mbedtls/pk.h>
#include <mbedtls/ecdsa.h>
typedef mbedtls_pk_context EC_KEY;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Structure storing an RSA key in a crypto library independent way
 * Only ECC keys upto 512 bit (64 byte) supported
 */
typedef struct
{
    U8 pub[256];
    U16 pubLen;
    U8 privP[128];
    U16 privPLen;
    U8 privQ[128];
    U16 privQLen;
    U8 privDp[128];
    U16 privDpLen;
    U8 privDq[128];
    U16 privDqLen;
    U8 privIpq[128];
    U16 privIpqLen;
    U16 bits; // Length of modulus in bits
} rsaKeyComponents_t;

/**
 * Structure storing an ECC key in a crypto library independent way
 * Only ECC keys upto 512 bit (64 byte) supported
 */
typedef struct
{
    U8 pub[(64 << 1)+1+8]; //!< Public key stored as byte array + 8 for wrapping
    U16 pubLen;          //!< Length of pub (expressed as number of byte)
    U8 priv[64+8];       //!< Private key stored as byte array + 8 for wrapping
    U16 privLen;         //!< Length of priv (expressed as number of byte)
    U16 bits;            //!< Length of private key expressed in bits
    ECCCurve_t curve;    //!< Curve type (e.g. ::ECCCurve_NIST_P256)
} eccKeyComponents_t;

#define SIGNATURE_ASN_WRAPPED 0
#define SIGNATURE_RAW         1

#if defined(OPENSSL) || defined(MBEDTLS)
/// @cond Not used in A71CH example programs
U16 HOSTCRYPTO_Sign(EC_KEY* pKey, U8* pInputData, U16 inputLength, U8* pSignature, U16* pSignatureLength, U8 signatureFormat);
/// @endcond
U16 HOSTCRYPTO_ECC_ComputeSharedSecret(EC_KEY *pKey, U8 *pubKey, U16 pubKeyLen, U8 *pSharedSecretData, U16 *pSharedSecretDataLen);
U16 HOSTCRYPTO_GetPublicKey(EC_KEY* pKey, U8* pPublicKeyData, U16* pPublicKeyLen, U16 maxPublicKeyLen);
U16 HOSTCRYPTO_GetPrivateKey(EC_KEY* pKey, U8* pPrivateKeyData, U16* pPrivateKeyLen, U16 maxPrivateKeyLen);
U16 HOSTCRYPTO_GenerateEccKey(ECCCurve_t curveType, EC_KEY** ppKey);
void HOSTCRYPTO_FreeEccKey(EC_KEY** ppKey);
U16 HOSTCRYPTO_EccCreateOpenSslEccFromComponents(EC_KEY **eccRef, eccKeyComponents_t *eccKc);
#endif /* defined(OPENSSL) || defined(MBEDTLS) */

U16 HOSTCRYPTO_AesUnwrapKeyRFC3394(const U8 *wrapKey, U16 wrapKeyLen, U8 *out, U16 *outLen, const U8 *in, U16 inLen);
U16 HOSTCRYPTO_AesWrapKeyRFC3394(const U8 *wrapKey, U16 wrapKeyLen, U8 *out, U16 *outLen, const U8 *in, U16 inLen);

#if defined(TGT_A71CH) || defined (TGT_A71CL)
U16 HOSTCRYPTO_HkdfExpandSha256(const U8 *secret, U16 secretLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen);
U16 HOSTCRYPTO_HkdfFullSha256(const U8 *salt, U16 saltLen, const U8 *secret, U16 secretLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen);
U16 HOSTCRYPTO_TlsPskCreatePremasterSecret(const U8 *secret, U16 secretLen, U8 *premasterSecret, U16 *premasterSecretLen);
U16 HOSTCRYPTO_TlsEcdhPskCreatePremasterSecret(const U8 *ecdhSS, U16 ecdhSSLen, const U8 *secret, U16 secretLen, U8 *premasterSecret, U16 *premasterSecretLen);
U16 HOSTCRYPTO_HmacSha256(const U8 *secret, U16 secretLen, const U8 *data, U16 dataLen, U8 *hmacData);
U16 HOSTCRYPTO_Tls1_2_P_Sha256(const U8 *secret, U16 secretLen, const U8 *seed, U16 seedLen, U8 *derivedData, U16 derivedDataLen);
#else
U16 HOSTCRYPTO_RsaGenerateKeyPairComponents(int bits, U8 *pN, U16 *pNLen, U8 *pP, U16 *pPLen, U8 *pQ, U16 *pQLen, U8 *pDp, U16 *pDpLen, U8 *pDq, U16 *pDqLen, U8 *pIpq, U16 *pIpqLen);
U16 HOSTCRYPTO_RsaGenerateKeyPair(int bits, rsaKeyComponents_t *rsaKeypair);
U16 HOSTCRYPTO_RsaSign(HASHAlgo_t hType, U8 *m, U16 mLen, U8 *sig, U16 *sigLen, rsaKeyComponents_t *rsaKc);
U16 HOSTCRYPTO_RSA_Sign(HASHAlgo_t hType, U8 *msg, U32 msgLen, U8 *sig, U16 *sigLen, rsaKeyComponents_t *rsaKc);
U16 HOSTCRYPTO_RSA_Verify(HASHAlgo_t hType, U8 *msg, U32 msgLen, U8 *sig, U32 sigLen, rsaKeyComponents_t *rsaKc, U8 *pVerified);
#endif

#ifdef TGT_A70CM
//
// Currently only 128 bits KEK and 128 bit keyToWrap is supported
//
U16 HOSTCRYPTO_WrapKeyDsmr40(U8 *kek, U16 kekLen, U8 *keyToWrap, U16 keyToWrapLen, U8 *iv, U16 ivLen, U8 *wrappedKey, U16 *wrappedKeyLen);
U16 HOSTCRYPTO_UnwrapKeyDsmr40(U8 *kek, U16 kekLen, U8 *keyToUnwrap, U16 keyToUnwrapLen, U8 *iv, U16 ivLen, U8 *unwrappedKey, U16 *unwrappedKeyLen);
#endif

#ifdef TGT_A70CI
U16 HOSTCRYPTO_GenerateEccKeyByName(int curveName, EC_KEY** ppKey);
int HOST_ECC_Verify(U8 * pHash, U16 hashLen, U8 * pPubKey, U16 pubKeyLen, U8 * pSig, U16 sigLen);
int HOST_ECC_GenerateKeyPair(U8 *pPublicKey, U16 *pPublicKeyLength, U8 *pPrivateKey, U16 *pPrivateKeyLength);
int HOST_ECC_ComputeSharedSecret(U8 * pPrvKey, U32 prvKeyLength, U8 * pPubKey, U16 pubKeyLen, U8* pSharedSecretData, U16* pSharedSecretLength, U16 maxSharedSecretLength);

int HOST_RSA_GenerateKeyPair(U8 * pN, U16 * pNLen, U8 * pP, U16 * pPLen, U8 * pQ, U16 * pQLen, U8 * pDp, U16 * pDpLen, U8 * pDq, U16 * pDqLen, U8 * pIpq, U16 * pIpqLen);
int HOST_RSA_Sign(U8 hashFunction, U8 *msg, U32 msgLen, U8 *sig, U32 * sigLen,
    U8 * pP, U16 lengthP, U8 * pQ, U16 lengthQ, U8 * pDp, U16 lengthDp, U8 * pDq, U16 lengthDq, U8 * pIpq, U16 lengthIpq) ;
int HOST_RSA_Verify(U8 hashFunction, U8 *msg, U32 mlen, U8 *sig, U32 siglen, U8 * pPubkey, U16 pubKeyLen);
// OAEP decryption (using PKCS #1 v2.1) - only decryption (OAEP on host)
int HOST_RSA_Decrypt(U8 *userId,
    U32 inDataLength,
    U8 *pP,
    U16 lengthP,
    U8 *pQ,
    U16 lengthQ,
    U8 *pDp,
    U16 lengthDp,
    U8 *pDq,
    U16 lengthDq,
    U8 *pIpq,
    U16 lengthIpq,
    U8 *pOutData,
    U16 *pOutDataLength);
// OAEP encryption (using PKCS #1 v2.1) - only encryption (OAEP on host)
int HOST_RSA_Encrypt(U8 *userId, U32 inDataLength, U8 *pPubKey, U16 lengthPubKey, U8 *pOutData, U16 *pOutDataLength);
#endif

#ifdef __cplusplus
}
#endif
#endif // _TST_HOST_CRYPTO_H_
