/* crypto.h for openSSL */

#ifndef WOLFSSL_CRYPTO_H_
#define WOLFSSL_CRYPTO_H_


#include <wolfssl/wolfcrypt/settings.h>

#ifdef WOLFSSL_PREFIX
#include "prefix_crypto.h"
#endif


WOLFSSL_API const char*   wolfSSLeay_version(int type);
WOLFSSL_API unsigned long wolfSSLeay(void);

#define CRYPTO_THREADID void

#define SSLeay_version wolfSSLeay_version
#define SSLeay wolfSSLeay


#define SSLEAY_VERSION 0x0090600fL
#define SSLEAY_VERSION_NUMBER SSLEAY_VERSION

#ifdef HAVE_STUNNEL
#define CRYPTO_set_mem_ex_functions      wolfSSL_CRYPTO_set_mem_ex_functions
#define FIPS_mode                        wolfSSL_FIPS_mode
#define FIPS_mode_set                    wolfSSL_FIPS_mode_set
typedef struct CRYPTO_EX_DATA            CRYPTO_EX_DATA;
typedef void (CRYPTO_free_func)(void*parent, void*ptr, CRYPTO_EX_DATA *ad, int idx,
        long argl, void* argp);
#define CRYPTO_THREADID_set_callback wolfSSL_THREADID_set_callback
#define CRYPTO_THREADID_set_numeric wolfSSL_THREADID_set_numeric
#endif /* HAVE_STUNNEL */

#endif /* header */

