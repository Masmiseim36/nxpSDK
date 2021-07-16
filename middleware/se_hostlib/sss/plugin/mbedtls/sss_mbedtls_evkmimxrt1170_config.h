/* Copyright 2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SSS_MBEDTLS_EVKB_H
#define SSS_MBEDTLS_EVKB_H

#include <sss_mbedtls_frdmk64f_config.h>

#ifndef MBEDTLS_CTR_DRBG_C
/* We need this for mbedTLS sw crypto counterpart */
#define MBEDTLS_CTR_DRBG_C
#endif

/* IF We use ALT from the EVKB, anything that is related to
 * AES256 would get skipped and because of this TLS Handshake
 * would not pass
 *
 * Disabling this altogether. Maybe there's a way to use AES256
 * on EVKB with mbedTLS but need to check.
 */

#undef MBEDTLS_AES_ALT
#undef MBEDTLS_AES_SETKEY_ENC_ALT
#undef MBEDTLS_AES_SETKEY_DEC_ALT
#undef MBEDTLS_AES_ENCRYPT_ALT
#undef MBEDTLS_AES_DECRYPT_ALT
#undef MBEDTLS_AES_ALT_NO_192
#undef MBEDTLS_AES_CRYPT_CBC_ALT
#undef MBEDTLS_AES_ALT_NO_256
#undef MBEDTLS_FREESCALE_DCP_AES

#if defined(MBEDTLS_FREESCALE_LTC_AES)
#undef MBEDTLS_FREESCALE_LTC_AES
#endif // MBEDTLS_FREESCALE_LTC_AES
#if defined(MBEDTLS_FREESCALE_MMCAU_AES)
#undef MBEDTLS_FREESCALE_MMCAU_AES
#endif // MBEDTLS_FREESCALE_MMCAU_AES
#if defined(MBEDTLS_FREESCALE_LPC_AES)
#undef MBEDTLS_FREESCALE_LPC_AES
#endif // MBEDTLS_FREESCALE_LPC_AES
#if defined(MBEDTLS_FREESCALE_CAU3_AES)
#undef MBEDTLS_FREESCALE_CAU3_AES
#endif // MBEDTLS_FREESCALE_CAU3_AES
#if defined(MBEDTLS_FREESCALE_CAAM_AES)
#undef MBEDTLS_FREESCALE_CAAM_AES
#endif // MBEDTLS_FREESCALE_CAAM_AES
#if defined(MBEDTLS_FREESCALE_DCP_AES)
#undef MBEDTLS_FREESCALE_DCP_AES
#endif // MBEDTLS_FREESCALE_DCP_AES
#if defined(MBEDTLS_CCM_CRYPT_ALT)
#undef MBEDTLS_CCM_CRYPT_ALT
#endif // MBEDTLS_CCM_CRYPT_ALT
#if defined(MBEDTLS_GCM_CRYPT_ALT)
#undef MBEDTLS_GCM_CRYPT_ALT
#endif // MBEDTLS_GCM_CRYPT_ALT

#endif /* SSS_MBEDTLS_EVKB_H */
