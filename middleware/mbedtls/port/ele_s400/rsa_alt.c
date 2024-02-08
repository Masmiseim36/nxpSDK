/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_RSA_C)

#include <string.h>

#include "mbedtls/rsa.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_RSA_KEYGEN_ALT) || defined(MBEDTLS_PKCS1_V15_ALT) || \
    defined(MBEDTLS_PKCS1_V21_ALT)
#include "rsa_alt.h"
#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

#define RSA_VALIDATE_RET(cond) \
    MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_RSA_BAD_INPUT)
#define RSA_VALIDATE(cond) \
    MBEDTLS_INTERNAL_VALIDATE(cond)


#if defined(MBEDTLS_RSA_KEYGEN_ALT)
/*
 * Generate an RSA keypair
 *
 * This generation method follows the RSA key pair generation procedure of
 * FIPS 186-4 if 2^16 < exponent < 2^256 and nbits = 2048, 3072 or 4096.
 */
int mbedtls_rsa_gen_key(mbedtls_rsa_context *ctx,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng,
                        unsigned int nbits, int exponent)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    RSA_VALIDATE_RET(ctx != NULL);
    uint32_t *modulo_tmp = NULL, *priv_exp_tmp = NULL;
    uint32_t pub_exponent;

    /* Minimum nbit size is 2048 */
    if (nbits < 2048 || exponent < 3 || nbits % 2 != 0) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    mbedtls_mpi_init(&ctx->N);
    mbedtls_mpi_init(&ctx->D);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif


    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(1, nbits / 8u);
    if (modulo_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Alocate MPI structure for Private exponent */
    priv_exp_tmp = mbedtls_calloc(1, nbits / 8u);
    if (priv_exp_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Convert to Big Endian */
    pub_exponent = __REV(exponent);

    ele_generic_rsa_t GenericRsaKeygen;
    memset(&GenericRsaKeygen, 0, sizeof(ele_generic_rsa_t));

    GenericRsaKeygen.modulus            = (uint32_t) modulo_tmp;
    GenericRsaKeygen.priv_exponent      = (uint32_t) priv_exp_tmp;
    GenericRsaKeygen.priv_exponent_size = nbits / 8u;
    GenericRsaKeygen.modulus_size       = nbits / 8u;
    GenericRsaKeygen.pub_exponent       = (uint32_t) &pub_exponent;
    GenericRsaKeygen.pub_exponent_size  = sizeof(pub_exponent);
    GenericRsaKeygen.key_size           = nbits;

    MBEDTLS_MPI_CHK(ELE_GenericRsaKeygen(S3MU, &GenericRsaKeygen));

    /* Set Public Exponent in Ctx */
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&ctx->E, exponent));

    /* Read modulo in MPI */
    mbedtls_mpi_read_binary(&ctx->N, (const unsigned char *) modulo_tmp, nbits / 8u);

    /* Read private exonent in MPI */
    mbedtls_mpi_read_binary(&ctx->D, (const unsigned char *) priv_exp_tmp, nbits / 8u);

    /* Set Ctx length */
    ctx->len = mbedtls_mpi_size(&ctx->N);

    /* Compute P and Q in CTX. */
    /* Not needed for RSA operation, only to pass MbedTLS ctx check (not mandatory). */
    //mbedtls_rsa_complete(ctx);

    /* Double-check. No need in alt implementation, takes too long time */
    //MBEDTLS_MPI_CHK( mbedtls_rsa_check_privkey( ctx ) );

cleanup:
    if (priv_exp_tmp != NULL) {
        mbedtls_platform_zeroize(priv_exp_tmp, ctx->len);
    }
    if (modulo_tmp != NULL) {
        mbedtls_free(modulo_tmp);
    }
    if (priv_exp_tmp != NULL) {
        mbedtls_free(priv_exp_tmp);
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ret != 0) {
        mbedtls_rsa_free(ctx);

        if ((-ret & ~0x7f) == 0) {
            ret = MBEDTLS_ERROR_ADD(MBEDTLS_ERR_RSA_KEY_GEN_FAILED, ret);
        }
        return ret;
    }

    return 0;
}
#endif /* MBEDTLS_RSA_KEYGEN_ALT */

#if defined(MBEDTLS_PKCS1_V15_ALT)
/*
 * Implementation of the PKCS#1 v2.1 RSAES-PKCS1-V1_5-ENCRYPT function
 */
int mbedtls_rsa_rsaes_pkcs1_v15_encrypt(mbedtls_rsa_context *ctx,
                                        int (*f_rng)(void *, unsigned char *, size_t),
                                        void *p_rng,
                                        int mode, size_t ilen,
                                        const unsigned char *input,
                                        unsigned char *output)
{
    size_t olen, nbits;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ele_generic_rsa_t GenericRsaEnc;
    uint32_t *modulo_tmp;
    uint32_t pub_exp;


    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE ||
                     mode == MBEDTLS_RSA_PUBLIC);
    RSA_VALIDATE_RET(output != NULL);
    RSA_VALIDATE_RET(ilen == 0 || input != NULL);

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    olen = ctx->len;
    nbits = ctx->len * 8u;

    /* first comparison checks for overflow */
    if (ilen + 11 < ilen || olen < ilen + 11) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        return MBEDTLS_ERR_MPI_ALLOC_FAILED;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, olen);

    /* Read public exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->E, (unsigned char *) &pub_exp, sizeof(uint32_t));

    memset(&GenericRsaEnc, 0, sizeof(ele_generic_rsa_t));

    /* Set ELE RSA structure */
    GenericRsaEnc.algo     = RSA_PKCS1_V1_5_CRYPT;
    GenericRsaEnc.mode     = kEncryption;
    GenericRsaEnc.key_size = nbits;
    /* Public exponent */
    GenericRsaEnc.pub_exponent      = (uint32_t) &pub_exp;
    GenericRsaEnc.pub_exponent_size = sizeof(pub_exp);
    /* Modulus */
    GenericRsaEnc.modulus      = (uint32_t) modulo_tmp;
    GenericRsaEnc.modulus_size = olen;
    /* Plaintext */
    GenericRsaEnc.plaintext      = (uint32_t) input;
    GenericRsaEnc.plaintext_size = ilen;
    /* Ciphertext */
    GenericRsaEnc.ciphertext      = (uint32_t) output;
    GenericRsaEnc.ciphertext_size = olen;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (ELE_GenericRsa(S3MU, &GenericRsaEnc) != kStatus_Success) {
        ret = MBEDTLS_ERR_RSA_PUBLIC_FAILED;
        goto cleanup;
    } else {
        ret = 0;
    }

cleanup:
    mbedtls_free(modulo_tmp);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-PKCS1-V1_5-DECRYPT function
 */
int mbedtls_rsa_rsaes_pkcs1_v15_decrypt(mbedtls_rsa_context *ctx,
                                        int (*f_rng)(void *, unsigned char *, size_t),
                                        void *p_rng,
                                        int mode,
                                        size_t *olen,
                                        const unsigned char *input,
                                        unsigned char *output,
                                        size_t output_max_len)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t ilen, nbits;
    ele_generic_rsa_t GenericRsaDec;
    uint32_t *modulo_tmp = NULL, *priv_exp_tmp = NULL;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE ||
                     mode == MBEDTLS_RSA_PUBLIC);
    RSA_VALIDATE_RET(output_max_len == 0 || output != NULL);
    RSA_VALIDATE_RET(input != NULL);
    RSA_VALIDATE_RET(olen != NULL);

    ilen = ctx->len;
    nbits = ctx->len * 8u;

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Alocate MPI structure for Private exponent */
    priv_exp_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (priv_exp_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, ilen);

    /* Read private exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->D, (unsigned char *) priv_exp_tmp, ilen);

    memset(&GenericRsaDec, 0, sizeof(ele_generic_rsa_t));

    /* Set ELE RSA structure */
    GenericRsaDec.algo     = RSA_PKCS1_V1_5_CRYPT;
    GenericRsaDec.mode     = kDecryption;
    GenericRsaDec.key_size = nbits;
    /* Public exponent */
    GenericRsaDec.priv_exponent      = (uint32_t) priv_exp_tmp;
    GenericRsaDec.priv_exponent_size = ilen;
    /* Modulus */
    GenericRsaDec.modulus      = (uint32_t) modulo_tmp;
    GenericRsaDec.modulus_size = ilen;
    /* Plaintext */
    GenericRsaDec.plaintext      = (uint32_t) output;
    GenericRsaDec.plaintext_size = (uint32_t) output_max_len;
    /* Ciphertext */
    GenericRsaDec.ciphertext      = (uint32_t) input;
    GenericRsaDec.ciphertext_size = ilen;

    if (ELE_GenericRsa(S3MU, &GenericRsaDec) != kStatus_Success) {
        ret = MBEDTLS_ERR_RSA_PUBLIC_FAILED;
        goto cleanup;
    } else {
        *olen = GenericRsaDec.out_plaintext_len;
        ret = 0;
    }


cleanup:
    if (priv_exp_tmp != NULL) {
        mbedtls_platform_zeroize(priv_exp_tmp, ctx->len);
    }
    if (modulo_tmp != NULL) {
        mbedtls_free(modulo_tmp);
    }
    if (priv_exp_tmp != NULL) {
        mbedtls_free(priv_exp_tmp);
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

/*
 * Do an RSA operation to sign the message digest
 */
int mbedtls_rsa_rsassa_pkcs1_v15_sign(mbedtls_rsa_context *ctx,
                                      int (*f_rng)(void *, unsigned char *, size_t),
                                      void *p_rng,
                                      int mode,
                                      mbedtls_md_type_t md_alg,
                                      unsigned int hashlen,
                                      const unsigned char *hash,
                                      unsigned char *sig)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t olen, nbits;
    ele_generic_rsa_t GenericRsaPssSign;
    uint32_t *modulo_tmp = NULL, *priv_exp_tmp = NULL;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE ||
                     mode == MBEDTLS_RSA_PUBLIC);
    RSA_VALIDATE_RET((md_alg  == MBEDTLS_MD_NONE &&
                      hashlen == 0) ||
                     hash != NULL);
    RSA_VALIDATE_RET(sig != NULL);

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    olen = ctx->len;
    nbits = ctx->len * 8u;

    memset(sig, 0, olen);

    memset(&GenericRsaPssSign, 0, sizeof(ele_generic_rsa_t));

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Alocate MPI structure for Private exponent */
    priv_exp_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (priv_exp_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, olen);

    /* Read private exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->D, (unsigned char *) priv_exp_tmp, olen);

    /* Set MGF (HASH) algo */
    switch (md_alg) {
        case (MBEDTLS_MD_SHA224):
            GenericRsaPssSign.algo     = RSA_PKCS1_V1_5_SHA224_SIGN;
            break;
        case (MBEDTLS_MD_SHA256):
            GenericRsaPssSign.algo     = RSA_PKCS1_V1_5_SHA256_SIGN;
            break;
        case (MBEDTLS_MD_SHA384):
            GenericRsaPssSign.algo     = RSA_PKCS1_V1_5_SHA384_SIGN;
            break;
        case (MBEDTLS_MD_SHA512):
            GenericRsaPssSign.algo     = RSA_PKCS1_V1_5_SHA512_SIGN;
            break;
        case (MBEDTLS_MD_NONE):
            ret = MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
            goto cleanup;
        default:
            goto cleanup;
    }

    /* Set ELE structure */
    GenericRsaPssSign.mode     = kSignGen;
    GenericRsaPssSign.key_size = nbits;
    /* Private exponent */
    GenericRsaPssSign.priv_exponent      = (uint32_t) priv_exp_tmp;
    GenericRsaPssSign.priv_exponent_size = olen;
    /* Modulus */
    GenericRsaPssSign.modulus      = (uint32_t) modulo_tmp;
    GenericRsaPssSign.modulus_size = olen;
    /* Digest */
    GenericRsaPssSign.digest      = (uint32_t) hash;
    GenericRsaPssSign.digest_size = hashlen;
    /* Signature destination */
    GenericRsaPssSign.signature      = (uint32_t) sig;
    GenericRsaPssSign.signature_size = olen;
    /* Flags */
    GenericRsaPssSign.flags = kFlagDigest;


    if (ELE_GenericRsa(S3MU, &GenericRsaPssSign) != kStatus_Success) {
        ret = MBEDTLS_ERR_RSA_PUBLIC_FAILED;
        goto cleanup;
    } else {
        ret = 0u;
    }

cleanup:
    if (priv_exp_tmp != NULL) {
        mbedtls_platform_zeroize(priv_exp_tmp, ctx->len);
    }
    if (modulo_tmp != NULL) {
        mbedtls_free(modulo_tmp);
    }
    if (priv_exp_tmp != NULL) {
        mbedtls_free(priv_exp_tmp);
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PKCS1-v1_5-VERIFY function
 */
int mbedtls_rsa_rsassa_pkcs1_v15_verify(mbedtls_rsa_context *ctx,
                                        int (*f_rng)(void *, unsigned char *, size_t),
                                        void *p_rng,
                                        int mode,
                                        mbedtls_md_type_t md_alg,
                                        unsigned int hashlen,
                                        const unsigned char *hash,
                                        const unsigned char *sig)
{
    int ret = 0;
    size_t sig_len, nbits;
    ele_generic_rsa_t GenericRsaPssVerif;
    uint32_t *modulo_tmp;
    uint32_t pub_exp;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PUBLIC);
    RSA_VALIDATE_RET(sig != NULL);
    RSA_VALIDATE_RET((md_alg  == MBEDTLS_MD_NONE &&
                      hashlen == 0) ||
                     hash != NULL);

    sig_len = ctx->len;
    nbits = ctx->len * 8u;

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    memset(&GenericRsaPssVerif, 0, sizeof(ele_generic_rsa_t));

    /* Set MGF (HASH) algo */
    switch (md_alg) {
        case (MBEDTLS_MD_SHA224):
            GenericRsaPssVerif.algo     = RSA_PKCS1_V1_5_SHA224_SIGN;
            break;
        case (MBEDTLS_MD_SHA256):
            GenericRsaPssVerif.algo     = RSA_PKCS1_V1_5_SHA256_SIGN;
            break;
        case (MBEDTLS_MD_SHA384):
            GenericRsaPssVerif.algo     = RSA_PKCS1_V1_5_SHA384_SIGN;
            break;
        case (MBEDTLS_MD_SHA512):
            GenericRsaPssVerif.algo     = RSA_PKCS1_V1_5_SHA512_SIGN;
            break;
        case (MBEDTLS_MD_NONE):
            return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
        default:
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        return MBEDTLS_ERR_MPI_ALLOC_FAILED;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, sig_len);

    /* Read public exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->E, (unsigned char *) &pub_exp, sizeof(uint32_t));

    GenericRsaPssVerif.mode     = kVerification;
    GenericRsaPssVerif.key_size = nbits;
    /* Public exponent */
    GenericRsaPssVerif.pub_exponent      = (uint32_t) &pub_exp;
    GenericRsaPssVerif.pub_exponent_size = sizeof(pub_exp);
    /* Modulus */
    GenericRsaPssVerif.modulus      = (uint32_t) modulo_tmp;
    GenericRsaPssVerif.modulus_size = sig_len;
    /* Digest */
    GenericRsaPssVerif.digest      = (uint32_t) hash;
    GenericRsaPssVerif.digest_size = hashlen;
    /* Signature destination */
    GenericRsaPssVerif.signature      = (uint32_t) sig;
    GenericRsaPssVerif.signature_size = sig_len;
    /* Flags */
    GenericRsaPssVerif.flags = kFlagDigest;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if ((ELE_GenericRsa(S3MU, &GenericRsaPssVerif) == kStatus_Success) &&
        (GenericRsaPssVerif.verify_status == kVerifySuccess)) {
        ret = 0u;
    } else {
        ret = MBEDTLS_ERR_RSA_VERIFY_FAILED;
        goto cleanup;
    }

cleanup:
    if (modulo_tmp != NULL) {
        mbedtls_free(modulo_tmp);
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

#endif /* MBEDTLS_PKCS1_V15_ALT */


#if defined(MBEDTLS_PKCS1_V21_ALT)
/*
 * Implementation of the PKCS#1 v2.1 RSAES-OAEP-ENCRYPT function
 */
int mbedtls_rsa_rsaes_oaep_encrypt(mbedtls_rsa_context *ctx,
                                   int (*f_rng)(void *, unsigned char *, size_t),
                                   void *p_rng,
                                   int mode,
                                   const unsigned char *label, size_t label_len,
                                   size_t ilen,
                                   const unsigned char *input,
                                   unsigned char *output)
{
    size_t olen, nbits;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned int hlen;
    const mbedtls_md_info_t *md_info;
    ele_generic_rsa_t GenericRsaEnc;
    uint32_t *modulo_tmp;
    uint32_t pub_exp;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE ||
                     mode == MBEDTLS_RSA_PUBLIC);
    RSA_VALIDATE_RET(output != NULL);
    RSA_VALIDATE_RET(ilen == 0 || input != NULL);
    RSA_VALIDATE_RET(label_len == 0 || label != NULL);

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    md_info = mbedtls_md_info_from_type((mbedtls_md_type_t) ctx->hash_id);
    if (md_info == NULL) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    olen = ctx->len;
    nbits = ctx->len * 8u;
    hlen = mbedtls_md_get_size(md_info);

    /* first comparison checks for overflow */
    if (ilen + 2 * hlen + 2 < ilen || olen < ilen + 2 * hlen + 2) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    memset(output, 0, olen);
    memset(&GenericRsaEnc, 0, sizeof(ele_generic_rsa_t));

    /* Set MGF (HASH) algo */
    switch (ctx->hash_id) {
        case (MBEDTLS_MD_SHA1):
            GenericRsaEnc.algo     = RSA_PKCS1_OAEP_SHA1;
            break;
        case (MBEDTLS_MD_SHA224):
            GenericRsaEnc.algo     = RSA_PKCS1_OAEP_SHA224;
            break;
        case (MBEDTLS_MD_SHA256):
            GenericRsaEnc.algo     = RSA_PKCS1_OAEP_SHA256;
            break;
        case (MBEDTLS_MD_SHA384):
            GenericRsaEnc.algo     = RSA_PKCS1_OAEP_SHA384;
            break;
        case (MBEDTLS_MD_SHA512):
            GenericRsaEnc.algo     = RSA_PKCS1_OAEP_SHA512;
            break;
        case (MBEDTLS_MD_NONE):
        default:
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        return MBEDTLS_ERR_MPI_ALLOC_FAILED;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, olen);

    /* Read public exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->E, (unsigned char *) &pub_exp, sizeof(uint32_t));

    /* Set ELE RSA structure */
    GenericRsaEnc.mode     = kEncryption;
    GenericRsaEnc.key_size = nbits;
    /* Public exponent */
    GenericRsaEnc.pub_exponent      = (uint32_t) &pub_exp;
    GenericRsaEnc.pub_exponent_size = sizeof(pub_exp);
    /* Modulus */
    GenericRsaEnc.modulus      = (uint32_t) modulo_tmp;
    GenericRsaEnc.modulus_size = olen;
    /* Plaintext */
    GenericRsaEnc.plaintext      = (uint32_t) input;
    GenericRsaEnc.plaintext_size = ilen;
    /* Ciphertext */
    GenericRsaEnc.ciphertext      = (uint32_t) output;
    GenericRsaEnc.ciphertext_size = olen;
    /* Label */
    GenericRsaEnc.label      = (uint32_t) label;
    GenericRsaEnc.label_size = label_len;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (ELE_GenericRsa(S3MU, &GenericRsaEnc) != kStatus_Success) {
        ret = MBEDTLS_ERR_RSA_PUBLIC_FAILED;
        goto exit;
    } else {
        ret = 0u;
    }

exit:
    mbedtls_free(modulo_tmp);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ret != 0) {
        return ret;
    }

    return 0;
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-OAEP-DECRYPT function
 */
int mbedtls_rsa_rsaes_oaep_decrypt(mbedtls_rsa_context *ctx,
                                   int (*f_rng)(void *, unsigned char *, size_t),
                                   void *p_rng,
                                   int mode,
                                   const unsigned char *label, size_t label_len,
                                   size_t *olen,
                                   const unsigned char *input,
                                   unsigned char *output,
                                   size_t output_max_len)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t ilen, nbits;
    unsigned char buf[MBEDTLS_MPI_MAX_SIZE];
    unsigned int hlen;
    const mbedtls_md_info_t *md_info;
    ele_generic_rsa_t GenericRsaDec;
    uint32_t *modulo_tmp = NULL, *priv_exp_tmp = NULL;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE);
    RSA_VALIDATE_RET(output_max_len == 0 || output != NULL);
    RSA_VALIDATE_RET(label_len == 0 || label != NULL);
    RSA_VALIDATE_RET(input != NULL);
    RSA_VALIDATE_RET(olen != NULL);

    /*
     * Parameters sanity checks
     */
    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    ilen = ctx->len;
    nbits = ctx->len * 8u;

    if (ilen < 16 || ilen > sizeof(buf)) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    md_info = mbedtls_md_info_from_type((mbedtls_md_type_t) ctx->hash_id);
    if (md_info == NULL) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    hlen = mbedtls_md_get_size(md_info);

    // checking for integer underflow
    if (2 * hlen + 2 > ilen) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    memset(&GenericRsaDec, 0, sizeof(ele_generic_rsa_t));

    /* Set MGF (HASH) algo */
    switch (ctx->hash_id) {
        case (MBEDTLS_MD_SHA1):
            GenericRsaDec.algo     = RSA_PKCS1_OAEP_SHA1;
            break;
        case (MBEDTLS_MD_SHA224):
            GenericRsaDec.algo     = RSA_PKCS1_OAEP_SHA224;
            break;
        case (MBEDTLS_MD_SHA256):
            GenericRsaDec.algo     = RSA_PKCS1_OAEP_SHA256;
            break;
        case (MBEDTLS_MD_SHA384):
            GenericRsaDec.algo     = RSA_PKCS1_OAEP_SHA384;
            break;
        case (MBEDTLS_MD_SHA512):
            GenericRsaDec.algo     = RSA_PKCS1_OAEP_SHA512;
            break;
        case (MBEDTLS_MD_NONE):
        default:
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Alocate MPI structure for Private exponent */
    priv_exp_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (priv_exp_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, ilen);

    /* Read private exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->D, (unsigned char *) priv_exp_tmp, ilen);

    /* Set ELE RSA structure */
    GenericRsaDec.mode     = kDecryption;
    GenericRsaDec.key_size = nbits;
    /* Public exponent */
    GenericRsaDec.priv_exponent      = (uint32_t) priv_exp_tmp;
    GenericRsaDec.priv_exponent_size = ilen;
    /* Modulus */
    GenericRsaDec.modulus      = (uint32_t) modulo_tmp;
    GenericRsaDec.modulus_size = ilen;
    /* Plaintext */
    GenericRsaDec.plaintext      = (uint32_t) output;
    GenericRsaDec.plaintext_size = (uint32_t) output_max_len;
    /* Ciphertext */
    GenericRsaDec.ciphertext      = (uint32_t) input;
    GenericRsaDec.ciphertext_size = ilen;
    /* Label */
    GenericRsaDec.label      = (uint32_t) label;
    GenericRsaDec.label_size = label_len;

    if (ELE_GenericRsa(S3MU, &GenericRsaDec) != kStatus_Success) {
        ret = MBEDTLS_ERR_RSA_PUBLIC_FAILED;
        goto cleanup;
    } else {
        *olen = GenericRsaDec.out_plaintext_len;
        ret = 0;
    }

cleanup:
    if (priv_exp_tmp != NULL) {
        mbedtls_platform_zeroize(priv_exp_tmp, ctx->len);
    }
    if (modulo_tmp != NULL) {
        mbedtls_free(modulo_tmp);
    }
    if (priv_exp_tmp != NULL) {
        mbedtls_free(priv_exp_tmp);
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

int rsa_rsassa_pss_sign(mbedtls_rsa_context *ctx,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng,
                        int mode,
                        mbedtls_md_type_t md_alg,
                        unsigned int hashlen,
                        const unsigned char *hash,
                        int saltlen,
                        unsigned char *sig)
{
    size_t olen;
    size_t slen, min_slen, hlen, nbits;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const mbedtls_md_info_t *md_info;
    ele_generic_rsa_t GenericRsaPssSign;
    uint32_t *modulo_tmp = NULL, *priv_exp_tmp = NULL;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE);
    RSA_VALIDATE_RET((md_alg  == MBEDTLS_MD_NONE &&
                      hashlen == 0) ||
                     hash != NULL);
    RSA_VALIDATE_RET(sig != NULL);

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    olen = ctx->len;
    nbits = ctx->len * 8u;

    if (md_alg != MBEDTLS_MD_NONE) {
        /* Gather length of hash to sign */
        md_info = mbedtls_md_info_from_type(md_alg);
        if (md_info == NULL) {
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
        }

        hashlen = mbedtls_md_get_size(md_info);
    }

    md_info = mbedtls_md_info_from_type((mbedtls_md_type_t) ctx->hash_id);
    if (md_info == NULL) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    hlen = mbedtls_md_get_size(md_info);

    if (saltlen == MBEDTLS_RSA_SALT_LEN_ANY) {
        /* Calculate the largest possible salt length, up to the hash size.
         * Normally this is the hash length, which is the maximum salt length
         * according to FIPS 185-4 §5.5 (e) and common practice. If there is not
         * enough room, use the maximum salt length that fits. The constraint is
         * that the hash length plus the salt length plus 2 bytes must be at most
         * the key length. This complies with FIPS 186-4 §5.5 (e) and RFC 8017
         * (PKCS#1 v2.2) §9.1.1 step 3. */
        min_slen = hlen - 2;
        if (olen < hlen + min_slen + 2) {
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
        } else if (olen >= hlen + hlen + 2) {
            slen = hlen;
        } else {
            slen = olen - hlen - 2;
        }
    } else if ((saltlen < 0) || (saltlen + hlen + 2 > olen)) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    } else {
        slen = (size_t) saltlen;
    }

    memset(sig, 0, olen);

    memset(&GenericRsaPssSign, 0, sizeof(ele_generic_rsa_t));

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto exit;
    }

    /* Alocate MPI structure for Private exponent */
    priv_exp_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (priv_exp_tmp == NULL) {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto exit;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, olen);

    /* Read private exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->D, (unsigned char *) priv_exp_tmp, olen);

    /* Set MGF (HASH) algo */
    switch (ctx->hash_id) {
        case (MBEDTLS_MD_SHA224):
            GenericRsaPssSign.algo     = RSA_PKCS1_PSS_MGF1_SHA224;
            break;
        case (MBEDTLS_MD_SHA256):
            GenericRsaPssSign.algo     = RSA_PKCS1_PSS_MGF1_SHA256;
            break;
        case (MBEDTLS_MD_SHA384):
            GenericRsaPssSign.algo     = RSA_PKCS1_PSS_MGF1_SHA384;
            break;
        case (MBEDTLS_MD_SHA512):
            GenericRsaPssSign.algo     = RSA_PKCS1_PSS_MGF1_SHA512;
            break;
        case (MBEDTLS_MD_NONE):
        default:
            ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
            goto exit;
    }

    /* Set ELE structure */
    GenericRsaPssSign.mode     = kSignGen;
    GenericRsaPssSign.key_size = nbits;
    /* Private exponent */
    GenericRsaPssSign.priv_exponent      = (uint32_t) priv_exp_tmp;
    GenericRsaPssSign.priv_exponent_size = olen;
    /* Modulus */
    GenericRsaPssSign.modulus      = (uint32_t) modulo_tmp;
    GenericRsaPssSign.modulus_size = olen;
    /* Digest */
    GenericRsaPssSign.digest      = (uint32_t) hash;
    GenericRsaPssSign.digest_size = hashlen;
    /* Signature destination */
    GenericRsaPssSign.signature      = (uint32_t) sig;
    GenericRsaPssSign.signature_size = olen;
    /* Salt */
    GenericRsaPssSign.salt_size = slen;
    /* Flags */
    GenericRsaPssSign.flags = kFlagDigest;

    if (ELE_GenericRsa(S3MU, &GenericRsaPssSign) != kStatus_Success) {
        ret = MBEDTLS_ERR_RSA_PUBLIC_FAILED;
        goto exit;
    } else {
        ret = 0u;
    }

exit:
    if (priv_exp_tmp != NULL) {
        mbedtls_platform_zeroize(priv_exp_tmp, ctx->len);
    }
    if (modulo_tmp != NULL) {
        mbedtls_free(modulo_tmp);
    }
    if (priv_exp_tmp != NULL) {
        mbedtls_free(priv_exp_tmp);
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ret != 0) {
        return ret;
    }

    return 0;
}

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PSS-VERIFY function
 * Note: In this alt implementation, md_alg is always same as mgf1_hash function
 */
int mbedtls_rsa_rsassa_pss_verify_ext(mbedtls_rsa_context *ctx,
                                      int (*f_rng)(void *, unsigned char *, size_t),
                                      void *p_rng,
                                      int mode,
                                      mbedtls_md_type_t md_alg,
                                      unsigned int hashlen,
                                      const unsigned char *hash,
                                      mbedtls_md_type_t mgf1_hash_id,
                                      int expected_salt_len,
                                      const unsigned char *sig)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t siglen, nbits;
    unsigned int hlen;
    const mbedtls_md_info_t *md_info;
    ele_generic_rsa_t GenericRsaPssVerif;
    uint32_t *modulo_tmp;
    uint32_t pub_exp;

    RSA_VALIDATE_RET(ctx != NULL);
    RSA_VALIDATE_RET(mode == MBEDTLS_RSA_PRIVATE ||
                     mode == MBEDTLS_RSA_PUBLIC);
    RSA_VALIDATE_RET(sig != NULL);
    RSA_VALIDATE_RET((md_alg  == MBEDTLS_MD_NONE &&
                      hashlen == 0) ||
                     hash != NULL);

    if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    siglen = ctx->len;
    nbits = ctx->len * 8u;

    if (md_alg != MBEDTLS_MD_NONE) {
        /* Gather length of hash to sign */
        md_info = mbedtls_md_info_from_type(md_alg);
        if (md_info == NULL) {
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
        }

        hashlen = mbedtls_md_get_size(md_info);
    }

    md_info = mbedtls_md_info_from_type(mgf1_hash_id);
    if (md_info == NULL) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    hlen = mbedtls_md_get_size(md_info);

    if (siglen < hlen + 2) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    memset(&GenericRsaPssVerif, 0, sizeof(ele_generic_rsa_t));

    /* Set MGF (HASH) algo - in rsa_rsassa_pss_sign() this is wrapped in ctx->hash_id */
    switch (mgf1_hash_id) {
        case (MBEDTLS_MD_SHA224):
            GenericRsaPssVerif.algo     = RSA_PKCS1_PSS_MGF1_SHA224;
            break;
        case (MBEDTLS_MD_SHA256):
            GenericRsaPssVerif.algo     = RSA_PKCS1_PSS_MGF1_SHA256;
            break;
        case (MBEDTLS_MD_SHA384):
            GenericRsaPssVerif.algo     = RSA_PKCS1_PSS_MGF1_SHA384;
            break;
        case (MBEDTLS_MD_SHA512):
            GenericRsaPssVerif.algo     = RSA_PKCS1_PSS_MGF1_SHA512;
            break;
        case (MBEDTLS_MD_NONE):
        default:
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    /* Alocate MPI structure for Public modulus */
    modulo_tmp = mbedtls_calloc(nbits / 8u, 8u);
    if (modulo_tmp == NULL) {
        return MBEDTLS_ERR_MPI_ALLOC_FAILED;
    }

    /* Read modulus data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->N, (unsigned char *) modulo_tmp, siglen);

    /* Read public exponent data from MPI ctx structure */
    mbedtls_mpi_write_binary(&ctx->E, (unsigned char *) &pub_exp, sizeof(uint32_t));

    GenericRsaPssVerif.mode     = kVerification;
    GenericRsaPssVerif.key_size = nbits;
    /* Public exponent */
    GenericRsaPssVerif.pub_exponent      = (uint32_t) &pub_exp;
    GenericRsaPssVerif.pub_exponent_size = sizeof(pub_exp);
    /* Modulus */
    GenericRsaPssVerif.modulus      = (uint32_t) modulo_tmp;
    GenericRsaPssVerif.modulus_size = siglen;
    /* Digest */
    GenericRsaPssVerif.digest      = (uint32_t) hash;
    GenericRsaPssVerif.digest_size = hashlen;
    /* Signature destination */
    GenericRsaPssVerif.signature      = (uint32_t) sig;
    GenericRsaPssVerif.signature_size = siglen;
    /* Salt size */
    GenericRsaPssVerif.salt_size = expected_salt_len;
    /* Flags */
    GenericRsaPssVerif.flags = kFlagDigest;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if ((ELE_GenericRsa(S3MU, &GenericRsaPssVerif) == kStatus_Success) &&
        (GenericRsaPssVerif.verify_status == kVerifySuccess)) {
        ret = 0u;
    } else {
        ret = MBEDTLS_ERR_RSA_VERIFY_FAILED;
        goto exit;
    }

exit:
    mbedtls_free(modulo_tmp);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

#endif /* MBEDTLS_PKCS1_V21_ALT */

#endif /* MBEDTLS_RSA_ALT || MBEDTLS_PKCS1_V15_ALT || MBEDTLS_PKCS1_V21_ALT */

#endif /* MBEDTLS_RSA_C */
