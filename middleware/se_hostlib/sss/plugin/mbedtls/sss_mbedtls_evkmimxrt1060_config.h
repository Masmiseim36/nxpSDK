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
 * Disabling this altogather. May be there's a way to use AES256
 * on EVKB with mbedTLS but need to Check.
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

#endif /* SSS_MBEDTLS_EVKB_H */
