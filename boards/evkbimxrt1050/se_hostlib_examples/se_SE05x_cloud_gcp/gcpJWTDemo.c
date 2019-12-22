/**
 * @file gcpJWTDemo.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017,2018 NXP
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
 * Demo for GCP JWT
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "gcpJWTDemo.h"
#include "mbedtls/base64.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/md.h"
#include "mbedtls/platform.h"
#include "mbedtls/sha256.h"
#include "stdio.h"
#include "string.h"
#include "nxLog_App.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/* base64_encodeded {"alg":"ES256","typ":"JWT"}"*/
const char jwt_header[] = "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9";

#define CLAIM_JSON "{\"aud\":\"%s\",\"iat\":%ld,\"exp\":%ld}"

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

void getECDSASign_RandS(unsigned char *outSign,
    size_t *oLen,
    const unsigned char *b64TokenBuffer,
    const size_t b64TokenBufferLen,
    mbedtls_pk_context *pKey)
{
    int ret;
    unsigned int rdIndex = 0;
    unsigned int rdLength;
    unsigned int wrIndex = 0;
    unsigned char signature[MBEDTLS_ECDSA_MAX_LEN];
    size_t sig_len = sizeof(signature);
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    ret =
        mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)"XYZ", sizeof("PERS"));
    if (ret != 0) {
        LOG_E(" failed\n  ! mbedtls_ctr_drbg_seed returned -0x%04x\n", -ret);
        goto exit;
    }

    if ((ret = mbedtls_pk_sign(pKey,
             MBEDTLS_MD_SHA256,
             b64TokenBuffer,
             b64TokenBufferLen,
             signature,
             &sig_len,
             mbedtls_ctr_drbg_random,
             &ctr_drbg)) != 0) {
        LOG_E(" failed\n  ! mbedtls_pk_sign returned %d\n\n", ret);
    }
    /* This is in DER Format. We only need R || S */
    *oLen = 0;
    if (signature[rdIndex++] != 0x30)
        goto exit;
    if (signature[rdIndex++] != (sig_len - 2))
        goto exit;
    if (signature[rdIndex++] != 0x02)
        goto exit;
    rdLength = signature[rdIndex++];
    if (rdLength & 01) {
        rdIndex++;
        rdLength--;
    }
    for (; wrIndex < rdLength;) {
        outSign[wrIndex++] = signature[rdIndex++];
    }
    if (signature[rdIndex++] != 0x02)
        goto exit;
    rdLength = rdLength + signature[rdIndex++];
    if (rdLength & 01) {
        rdIndex++;
        rdLength--;
    }
    for (; wrIndex < rdLength;) {
        outSign[wrIndex++] = signature[rdIndex++];
    }
    *oLen = wrIndex;
exit:;
}

void convertB64toURLB64(unsigned char buffer[], int max_buf_size)
{
    int i;
    for (i = 0; (i < max_buf_size) && (buffer[i] != '\0'); i++) {
        if ('+' == buffer[i]) {
            buffer[i] = '-';
        }
        else if ('/' == buffer[i]) {
            buffer[i] = '_';
        }
    }
}

void addClaimb64(unsigned char *pBuffer,
    size_t max_len,
    size_t *enc_len,
    const char *aud,
    const unsigned long iat,
    const unsigned long exp)
{
    char claim_json[1024];
    snprintf(claim_json, sizeof(claim_json), CLAIM_JSON, aud, iat, exp);
    *enc_len = 0;
    mbedtls_base64_encode(pBuffer, max_len, enc_len, (unsigned char *)claim_json, strlen(claim_json));
}

#define REMOVE_TRAILING_EQUALS()                                       \
    do {                                                               \
        while (b64TokenBuffer[b64TokenBuffer_writeIndex - 1] == '=') { \
            b64TokenBuffer[b64TokenBuffer_writeIndex - 1] = '\0';      \
            b64TokenBuffer_writeIndex--;                               \
        }                                                              \
    } while (0)

void gcpCreateJWTTokenES256(unsigned char *b64TokenBuffer,
    size_t *b64tokenLen,
    const char *aud,
    const unsigned long iat,
    const int expdelta,
    mbedtls_pk_context *pKey)
{
    int ret;
    size_t enc_len;
    size_t sig_len = 0;
    int b64TokenBuffer_writeIndex = sizeof(jwt_header);
    unsigned char sha256_of_header_and_claim_b64[32];
    unsigned char signature_r_and_s[32 + 32];
    IOT_UNUSED(b64tokenLen);

    memcpy(b64TokenBuffer, jwt_header, sizeof(jwt_header) - 1);
    b64TokenBuffer[b64TokenBuffer_writeIndex - 1] = '.';
    addClaimb64(&b64TokenBuffer[b64TokenBuffer_writeIndex],
        sizeof(b64TokenBuffer) - b64TokenBuffer_writeIndex,
        &enc_len,
        aud,
        iat,
        iat + expdelta);

    b64TokenBuffer_writeIndex += enc_len;
    REMOVE_TRAILING_EQUALS();
    b64TokenBuffer[b64TokenBuffer_writeIndex++] = '.';
    convertB64toURLB64(b64TokenBuffer, b64TokenBuffer_writeIndex);

    mbedtls_sha256(b64TokenBuffer, b64TokenBuffer_writeIndex - 1, sha256_of_header_and_claim_b64, 0);
    getECDSASign_RandS(
        signature_r_and_s, &sig_len, sha256_of_header_and_claim_b64, sizeof(sha256_of_header_and_claim_b64), pKey);

    if ((ret = mbedtls_base64_encode((unsigned char *)&b64TokenBuffer[b64TokenBuffer_writeIndex],
             sizeof(b64TokenBuffer) - b64TokenBuffer_writeIndex,
             &enc_len,
             (unsigned char *)signature_r_and_s,
             sig_len)) != 0) {
        LOG_E(" failed\n  ! mbedtls_base64_encode returned -0x%X\n\n", -ret);
        goto exit;
    }
    b64TokenBuffer_writeIndex += enc_len;
    REMOVE_TRAILING_EQUALS();
    convertB64toURLB64(b64TokenBuffer, b64TokenBuffer_writeIndex);
    LOG_I("JWT TOKEN = %s\n", b64TokenBuffer);
exit:;
}
