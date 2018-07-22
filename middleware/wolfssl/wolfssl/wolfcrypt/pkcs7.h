/* pkcs7.h
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * http://www.wolfssl.com
 */



#ifndef WOLF_CRYPT_PKCS7_H
#define WOLF_CRYPT_PKCS7_H

#include <wolfssl/wolfcrypt/types.h>

#ifdef HAVE_PKCS7

#ifndef NO_ASN
    #include <wolfssl/wolfcrypt/asn.h>
#endif
#include <wolfssl/wolfcrypt/asn_public.h>
#include <wolfssl/wolfcrypt/random.h>
#ifndef NO_DES3
    #include <wolfssl/wolfcrypt/des3.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/* PKCS#7 content types, ref RFC 2315 (Section 14) */
enum PKCS7_TYPES {
    PKCS7_MSG                 = 650,   /* 1.2.840.113549.1.7   */
    DATA                      = 651,   /* 1.2.840.113549.1.7.1 */
    SIGNED_DATA               = 652,   /* 1.2.840.113549.1.7.2 */
    ENVELOPED_DATA            = 653,   /* 1.2.840.113549.1.7.3 */
    SIGNED_AND_ENVELOPED_DATA = 654,   /* 1.2.840.113549.1.7.4 */
    DIGESTED_DATA             = 655,   /* 1.2.840.113549.1.7.5 */
    ENCRYPTED_DATA            = 656    /* 1.2.840.113549.1.7.6 */
};

enum Pkcs7_Misc {
    PKCS7_NONCE_SZ       = 16,
    MAX_ENCRYPTED_KEY_SZ = 512,           /* max enc. key size, RSA <= 4096 */
    MAX_CONTENT_KEY_LEN  = DES3_KEYLEN,   /* highest current cipher is 3DES */
    MAX_RECIP_SZ         = MAX_VERSION_SZ +
                           MAX_SEQ_SZ + ASN_NAME_MAX + MAX_SN_SZ +
                           MAX_SEQ_SZ + MAX_ALGO_SZ + 1 + MAX_ENCRYPTED_KEY_SZ
};


typedef struct PKCS7Attrib {
    byte* oid;
    word32 oidSz;
    byte* value;
    word32 valueSz;
} PKCS7Attrib;


typedef struct PKCS7 {
    byte* content;                /* inner content, not owner             */
    word32 contentSz;             /* content size                         */
    int contentOID;               /* PKCS#7 content type OID sum          */

    WC_RNG* rng;

    int hashOID;
    int encryptOID;               /* key encryption algorithm OID         */

    void*  heap;                  /* heap hint for dynamic memory         */
    byte*  singleCert;            /* recipient cert, DER, not owner       */
    word32 singleCertSz;          /* size of recipient cert buffer, bytes */
    byte issuerHash[KEYID_SIZE];  /* hash of all alt Names                */
    byte*  issuer;                /* issuer name of singleCert            */
    word32 issuerSz;              /* length of issuer name                */
    byte issuerSn[MAX_SN_SZ];     /* singleCert's serial number           */
    word32 issuerSnSz;            /* length of serial number              */
    byte publicKey[512];
    word32 publicKeySz;
    byte*  privateKey;            /* private key, DER, not owner          */
    word32 privateKeySz;          /* size of private key buffer, bytes    */

    PKCS7Attrib* signedAttribs;
    word32 signedAttribsSz;
} PKCS7;


WOLFSSL_LOCAL int wc_PKCS7_SetHeap(PKCS7* pkcs7, void* heap);
WOLFSSL_LOCAL int wc_SetContentType(int pkcs7TypeOID, byte* output);
WOLFSSL_LOCAL int wc_GetContentType(const byte* input, word32* inOutIdx,
                                word32* oid, word32 maxIdx);
WOLFSSL_LOCAL int wc_CreateRecipientInfo(const byte* cert, word32 certSz,
                                     int keyEncAlgo, int blockKeySz,
                                     WC_RNG* rng, byte* contentKeyPlain,
                                     byte* contentKeyEnc, int* keyEncSz,
                                     byte* out, word32 outSz, void* heap);

WOLFSSL_API int  wc_PKCS7_InitWithCert(PKCS7* pkcs7, byte* cert, word32 certSz);
WOLFSSL_API void wc_PKCS7_Free(PKCS7* pkcs7);
WOLFSSL_API int  wc_PKCS7_EncodeData(PKCS7* pkcs7, byte* output, word32 outputSz);
WOLFSSL_API int  wc_PKCS7_EncodeSignedData(PKCS7* pkcs7,
                                       byte* output, word32 outputSz);
WOLFSSL_API int  wc_PKCS7_VerifySignedData(PKCS7* pkcs7,
                                       byte* pkiMsg, word32 pkiMsgSz);
WOLFSSL_API int  wc_PKCS7_EncodeEnvelopedData(PKCS7* pkcs7,
                                          byte* output, word32 outputSz);
WOLFSSL_API int  wc_PKCS7_DecodeEnvelopedData(PKCS7* pkcs7, byte* pkiMsg,
                                          word32 pkiMsgSz, byte* output,
                                          word32 outputSz);

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* HAVE_PKCS7 */
#endif /* WOLF_CRYPT_PKCS7_H */

