/* ssl.c
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * http://www.wolfssl.com
 */



#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#ifndef WOLFCRYPT_ONLY

#ifdef HAVE_ERRNO_H
    #include <errno.h>
#endif

#include <wolfssl/internal.h>
#include <wolfssl/error-ssl.h>
#include <wolfssl/wolfcrypt/coding.h>
#ifdef NO_INLINE
    #include <wolfssl/wolfcrypt/misc.h>
#else
    #define WOLFSSL_MISC_INCLUDED
    #include <wolfcrypt/src/misc.c>
#endif


#ifndef WOLFSSL_ALLOW_NO_SUITES
    #if defined(NO_DH) && !defined(HAVE_ECC) && !defined(WOLFSSL_STATIC_RSA) \
                  && !defined(WOLFSSL_STATIC_DH) && !defined(WOLFSSL_STATIC_PSK)
        #error "No cipher suites defined because DH disabled, ECC disabled, and no static suites defined. Please see top of README"
    #endif
#endif

#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER) || \
                              defined(WOLFSSL_KEY_GEN)
    #include <wolfssl/openssl/evp.h>
    /* openssl headers end, wolfssl internal headers next */
    #include <wolfssl/wolfcrypt/wc_encrypt.h>
#endif

#ifdef OPENSSL_EXTRA
    /* openssl headers begin */
    #include <wolfssl/openssl/hmac.h>
    #include <wolfssl/openssl/crypto.h>
    #include <wolfssl/openssl/des.h>
    #include <wolfssl/openssl/bn.h>
    #include <wolfssl/openssl/dh.h>
    #include <wolfssl/openssl/rsa.h>
    #include <wolfssl/openssl/pem.h>
    #include <wolfssl/openssl/ec.h>
    #include <wolfssl/openssl/ec25519.h>
    #include <wolfssl/openssl/ed25519.h>
    #include <wolfssl/openssl/ecdsa.h>
    #include <wolfssl/openssl/ecdh.h>
    /* openssl headers end, wolfssl internal headers next */
    #include <wolfssl/wolfcrypt/hmac.h>
    #include <wolfssl/wolfcrypt/random.h>
    #include <wolfssl/wolfcrypt/des3.h>
    #include <wolfssl/wolfcrypt/md4.h>
    #include <wolfssl/wolfcrypt/md5.h>
    #include <wolfssl/wolfcrypt/arc4.h>
    #include <wolfssl/wolfcrypt/idea.h>
    #include <wolfssl/wolfcrypt/curve25519.h>
    #include <wolfssl/wolfcrypt/ed25519.h>
    #ifdef HAVE_STUNNEL
        #include <wolfssl/openssl/ocsp.h>
    #endif /* WITH_STUNNEL */
    #ifdef WOLFSSL_SHA512
        #include <wolfssl/wolfcrypt/sha512.h>
    #endif
#endif

#ifdef NO_ASN
    #include <wolfssl/wolfcrypt/dh.h>
#endif

#ifndef NO_FILESYSTEM
    #if !defined(USE_WINDOWS_API) && !defined(NO_WOLFSSL_DIR) \
            && !defined(EBSNET)
        #include <dirent.h>
        #include <sys/stat.h>
    #endif
    #ifdef EBSNET
        #include "vfapi.h"
        #include "vfile.h"
    #endif
#endif /* NO_FILESYSTEM */

#ifndef TRUE
    #define TRUE  1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

#ifndef WOLFSSL_HAVE_MIN
#define WOLFSSL_HAVE_MIN

    static INLINE word32 min(word32 a, word32 b)
    {
        return a > b ? b : a;
    }

#endif /* WOLFSSSL_HAVE_MIN */

#if defined(WOLFSSL_DTLS) && !defined(WOLFSSL_HAVE_MAX)
#define WOLFSSL_HAVE_MAX

    static INLINE word32 max(word32 a, word32 b)
    {
        return a > b ? a : b;
    }

#endif /* WOLFSSL_DTLS && !WOLFSSL_HAVE_MAX */


#ifndef WOLFSSL_LEANPSK
char* mystrnstr(const char* s1, const char* s2, unsigned int n)
{
    unsigned int s2_len = (unsigned int)XSTRLEN(s2);

    if (s2_len == 0)
        return (char*)s1;

    while (n >= s2_len && s1[0]) {
        if (s1[0] == s2[0])
            if (XMEMCMP(s1, s2, s2_len) == 0)
                return (char*)s1;
        s1++;
        n--;
    }

    return NULL;
}
#endif

#ifdef WOLFSSL_SESSION_EXPORT
#ifdef WOLFSSL_DTLS
int wolfSSL_dtls_import(WOLFSSL* ssl, unsigned char* buf, unsigned int sz)
{
    WOLFSSL_ENTER("wolfSSL_session_import");

    if (ssl == NULL || buf == NULL) {
        return BAD_FUNC_ARG;
    }

    /* sanity checks on buffer and protocol are done in internal function */
    return wolfSSL_dtls_import_internal(ssl, buf, sz);
}


/* Sets the function to call for serializing the session. This function is
 * called right after the handshake is completed. */
int wolfSSL_CTX_dtls_set_export(WOLFSSL_CTX* ctx, wc_dtls_export func)
{

    WOLFSSL_ENTER("wolfSSL_CTX_dtls_set_export");

    /* purposefully allow func to be NULL */
    if (ctx == NULL) {
        return BAD_FUNC_ARG;
    }

    ctx->dtls_export = func;

    return SSL_SUCCESS;
}


/* Sets the function in WOLFSSL struct to call for serializing the session. This
 * function is called right after the handshake is completed. */
int wolfSSL_dtls_set_export(WOLFSSL* ssl, wc_dtls_export func)
{

    WOLFSSL_ENTER("wolfSSL_dtls_set_export");

    /* purposefully allow func to be NULL */
    if (ssl == NULL) {
        return BAD_FUNC_ARG;
    }

    ssl->dtls_export = func;

    return SSL_SUCCESS;
}


/* This function allows for directly serializing a session rather than using
 * callbacks. It has less overhead by removing a temporary buffer and gives
 * control over when the session gets serialized. When using callbacks the
 * session is always serialized immediatly after the handshake is finished.
 *
 * buf is the argument to contain the serialized session
 * sz  is the size of the buffer passed in
 * ssl is the WOLFSSL struct to serialize
 * returns the size of serialized session on success, 0 on no action, and
 *         negative value on error */
int wolfSSL_dtls_export(WOLFSSL* ssl, unsigned char* buf, unsigned int* sz)
{
    WOLFSSL_ENTER("wolfSSL_dtls_export");

    if (ssl == NULL || sz == NULL) {
        return BAD_FUNC_ARG;
    }

    if (buf == NULL) {
        *sz = MAX_EXPORT_BUFFER;
        return 0;
    }

    /* if not DTLS do nothing */
    if (!ssl->options.dtls) {
        WOLFSSL_MSG("Currently only DTLS export is supported");
        return 0;
    }

    /* copy over keys, options, and dtls state struct */
    return wolfSSL_dtls_export_internal(ssl, buf, *sz);
}


/* returns 0 on success */
int wolfSSL_send_session(WOLFSSL* ssl)
{
    int ret;
    byte* buf;
    word16 bufSz = MAX_EXPORT_BUFFER;

    WOLFSSL_ENTER("wolfSSL_send_session");

    if (ssl == NULL) {
        return BAD_FUNC_ARG;
    }

    buf = (byte*)XMALLOC(bufSz, ssl->heap, DYNAMIC_TYPE_TMP_BUFFER);
    if (buf == NULL) {
        return MEMORY_E;
    }

    /* if not DTLS do nothing */
    if (!ssl->options.dtls) {
        XFREE(buf, ssl->heap, DYNAMIC_TYPE_TMP_BUFFER);
        WOLFSSL_MSG("Currently only DTLS export is supported");
        return 0;
    }

    /* copy over keys, options, and dtls state struct */
    ret = wolfSSL_dtls_export_internal(ssl, buf, bufSz);
    if (ret < 0) {
        XFREE(buf, ssl->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return ret;
    }

    /* if no error ret has size of buffer */
    ret = ssl->dtls_export(ssl, buf, ret, NULL);
    if (ret != SSL_SUCCESS) {
        XFREE(buf, ssl->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return ret;
    }

    XFREE(buf, ssl->heap, DYNAMIC_TYPE_TMP_BUFFER);
    return 0;
}
#endif /* WOLFSSL_DTLS */
#endif /* WOLFSSL_SESSION_EXPORT */


/* prevent multiple mutex initializations */
static volatile int initRefCount = 0;
static wolfSSL_Mutex count_mutex;   /* init ref count mutex */


/* Create a new WOLFSSL_CTX struct and return the pointer to created struct.
   WOLFSSL_METHOD pointer passed in is given to ctx to manage.
   This function frees the passed in WOLFSSL_METHOD struct on failure and on
   success is freed when ctx is freed.
 */
WOLFSSL_CTX* wolfSSL_CTX_new_ex(WOLFSSL_METHOD* method, void* heap)
{
    WOLFSSL_CTX* ctx = NULL;

    WOLFSSL_ENTER("WOLFSSL_CTX_new_ex");

    if (initRefCount == 0) {
        /* user no longer forced to call Init themselves */
        int ret = wolfSSL_Init();
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("wolfSSL_Init failed");
            WOLFSSL_LEAVE("WOLFSSL_CTX_new", 0);
            if (method != NULL) {
                XFREE(method, heap, DYNAMIC_TYPE_METHOD);
            }
            return NULL;
        }
    }

    if (method == NULL)
        return ctx;

    ctx = (WOLFSSL_CTX*) XMALLOC(sizeof(WOLFSSL_CTX), heap, DYNAMIC_TYPE_CTX);
    if (ctx) {
        if (InitSSL_Ctx(ctx, method, heap) < 0) {
            WOLFSSL_MSG("Init CTX failed");
            wolfSSL_CTX_free(ctx);
            ctx = NULL;
        }
    }
    else {
        WOLFSSL_MSG("Alloc CTX failed, method freed");
        XFREE(method, heap, DYNAMIC_TYPE_METHOD);
    }

    WOLFSSL_LEAVE("WOLFSSL_CTX_new", 0);
    return ctx;
}


WOLFSSL_CTX* wolfSSL_CTX_new(WOLFSSL_METHOD* method)
{
#ifdef WOLFSSL_HEAP_TEST
    /* if testing the heap hint then set top level CTX to have test value */
    return wolfSSL_CTX_new_ex(method, (void*)WOLFSSL_HEAP_TEST);
#else
    return wolfSSL_CTX_new_ex(method, NULL);
#endif
}


void wolfSSL_CTX_free(WOLFSSL_CTX* ctx)
{
    WOLFSSL_ENTER("SSL_CTX_free");
    if (ctx)
        FreeSSL_Ctx(ctx);
    WOLFSSL_LEAVE("SSL_CTX_free", 0);
}


WOLFSSL* wolfSSL_new(WOLFSSL_CTX* ctx)
{
    WOLFSSL* ssl = NULL;
    int ret = 0;

    (void)ret;
    WOLFSSL_ENTER("SSL_new");

    if (ctx == NULL)
        return ssl;

    ssl = (WOLFSSL*) XMALLOC(sizeof(WOLFSSL), ctx->heap, DYNAMIC_TYPE_SSL);
    if (ssl)
        if ( (ret = InitSSL(ssl, ctx)) < 0) {
            FreeSSL(ssl, ctx->heap);
            ssl = 0;
        }

    WOLFSSL_LEAVE("SSL_new", ret);
    return ssl;
}


void wolfSSL_free(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_free");
    if (ssl)
        FreeSSL(ssl, ssl->ctx->heap);
    WOLFSSL_LEAVE("SSL_free", 0);
}

#ifdef HAVE_POLY1305
/* set if to use old poly 1 for yes 0 to use new poly */
int wolfSSL_use_old_poly(WOLFSSL* ssl, int value)
{
    WOLFSSL_ENTER("SSL_use_old_poly");
    WOLFSSL_MSG("Warning SSL connection auto detects old/new and this function"
            "is depriciated");
    ssl->options.oldPoly = (word16)value;
    WOLFSSL_LEAVE("SSL_use_old_poly", 0);
    return 0;
}
#endif

int wolfSSL_set_fd(WOLFSSL* ssl, int fd)
{
    WOLFSSL_ENTER("SSL_set_fd");

    if (ssl == NULL) {
        return BAD_FUNC_ARG;
    }

    ssl->rfd = fd;      /* not used directly to allow IO callbacks */
    ssl->wfd = fd;

    ssl->IOCB_ReadCtx  = &ssl->rfd;
    ssl->IOCB_WriteCtx = &ssl->wfd;

    #ifdef WOLFSSL_DTLS
        if (ssl->options.dtls) {
            ssl->IOCB_ReadCtx = &ssl->buffers.dtlsCtx;
            ssl->IOCB_WriteCtx = &ssl->buffers.dtlsCtx;
            ssl->buffers.dtlsCtx.fd = fd;
        }
    #endif

    WOLFSSL_LEAVE("SSL_set_fd", SSL_SUCCESS);
    return SSL_SUCCESS;
}


/**
  * Get the name of cipher at priority level passed in.
  */
char* wolfSSL_get_cipher_list(int priority)
{
    const char* const* ciphers = GetCipherNames();

    if (priority >= GetCipherNamesSize() || priority < 0) {
        return 0;
    }

    return (char*)ciphers[priority];
}


int wolfSSL_get_ciphers(char* buf, int len)
{
    const char* const* ciphers = GetCipherNames();
    int  totalInc = 0;
    int  step     = 0;
    char delim    = ':';
    int  size     = GetCipherNamesSize();
    int  i;

    if (buf == NULL || len <= 0)
        return BAD_FUNC_ARG;

    /* Add each member to the buffer delimited by a : */
    for (i = 0; i < size; i++) {
        step = (int)(XSTRLEN(ciphers[i]) + 1);  /* delimiter */
        totalInc += step;

        /* Check to make sure buf is large enough and will not overflow */
        if (totalInc < len) {
            XSTRNCPY(buf, ciphers[i], XSTRLEN(ciphers[i]));
            buf += XSTRLEN(ciphers[i]);

            if (i < size - 1)
                *buf++ = delim;
            else
                *buf++ = '\0';
        }
        else
            return BUFFER_E;
    }
    return SSL_SUCCESS;
}


int wolfSSL_get_fd(const WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_get_fd");
    WOLFSSL_LEAVE("SSL_get_fd", ssl->rfd);
    return ssl->rfd;
}


int wolfSSL_get_using_nonblock(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_get_using_nonblock");
    WOLFSSL_LEAVE("wolfSSL_get_using_nonblock", ssl->options.usingNonblock);
    return ssl->options.usingNonblock;
}


int wolfSSL_dtls(WOLFSSL* ssl)
{
    return ssl->options.dtls;
}


#ifndef WOLFSSL_LEANPSK
void wolfSSL_set_using_nonblock(WOLFSSL* ssl, int nonblock)
{
    WOLFSSL_ENTER("wolfSSL_set_using_nonblock");
    ssl->options.usingNonblock = (nonblock != 0);
}


int wolfSSL_dtls_set_peer(WOLFSSL* ssl, void* peer, unsigned int peerSz)
{
#ifdef WOLFSSL_DTLS
    void* sa = (void*)XMALLOC(peerSz, ssl->heap, DYNAMIC_TYPE_SOCKADDR);
    if (sa != NULL) {
        if (ssl->buffers.dtlsCtx.peer.sa != NULL)
            XFREE(ssl->buffers.dtlsCtx.peer.sa,ssl->heap,DYNAMIC_TYPE_SOCKADDR);
        XMEMCPY(sa, peer, peerSz);
        ssl->buffers.dtlsCtx.peer.sa = sa;
        ssl->buffers.dtlsCtx.peer.sz = peerSz;
        return SSL_SUCCESS;
    }
    return SSL_FAILURE;
#else
    (void)ssl;
    (void)peer;
    (void)peerSz;
    return SSL_NOT_IMPLEMENTED;
#endif
}

int wolfSSL_dtls_get_peer(WOLFSSL* ssl, void* peer, unsigned int* peerSz)
{
#ifdef WOLFSSL_DTLS
    if (ssl == NULL) {
        return SSL_FAILURE;
    }

    if (peer != NULL && peerSz != NULL
            && *peerSz >= ssl->buffers.dtlsCtx.peer.sz
            && ssl->buffers.dtlsCtx.peer.sa != NULL) {
        *peerSz = ssl->buffers.dtlsCtx.peer.sz;
        XMEMCPY(peer, ssl->buffers.dtlsCtx.peer.sa, *peerSz);
        return SSL_SUCCESS;
    }
    return SSL_FAILURE;
#else
    (void)ssl;
    (void)peer;
    (void)peerSz;
    return SSL_NOT_IMPLEMENTED;
#endif
}
#endif /* WOLFSSL_LEANPSK */


/* return underlying connect or accept, SSL_SUCCESS on ok */
int wolfSSL_negotiate(WOLFSSL* ssl)
{
    int err = SSL_FATAL_ERROR;

    WOLFSSL_ENTER("wolfSSL_negotiate");
#ifndef NO_WOLFSSL_SERVER
    if (ssl->options.side == WOLFSSL_SERVER_END)
        err = wolfSSL_accept(ssl);
#endif

#ifndef NO_WOLFSSL_CLIENT
    if (ssl->options.side == WOLFSSL_CLIENT_END)
        err = wolfSSL_connect(ssl);
#endif

    WOLFSSL_LEAVE("wolfSSL_negotiate", err);

    return err;
}


WC_RNG* wolfSSL_GetRNG(WOLFSSL* ssl)
{
    if (ssl) {
        return ssl->rng;
    }

    return NULL;
}


#ifndef WOLFSSL_LEANPSK
/* object size based on build */
int wolfSSL_GetObjectSize(void)
{
#ifdef SHOW_SIZES
    printf("sizeof suites           = %lu\n", sizeof(Suites));
    printf("sizeof ciphers(2)       = %lu\n", sizeof(Ciphers));
#ifndef NO_RC4
    printf("    sizeof arc4         = %lu\n", sizeof(Arc4));
#endif
    printf("    sizeof aes          = %lu\n", sizeof(Aes));
#ifndef NO_DES3
    printf("    sizeof des3         = %lu\n", sizeof(Des3));
#endif
#ifndef NO_RABBIT
    printf("    sizeof rabbit       = %lu\n", sizeof(Rabbit));
#endif
#ifdef HAVE_CHACHA
    printf("    sizeof chacha       = %lu\n", sizeof(ChaCha));
#endif
    printf("sizeof cipher specs     = %lu\n", sizeof(CipherSpecs));
    printf("sizeof keys             = %lu\n", sizeof(Keys));
    printf("sizeof Hashes(2)        = %lu\n", sizeof(Hashes));
#ifndef NO_MD5
    printf("    sizeof MD5          = %lu\n", sizeof(Md5));
#endif
#ifndef NO_SHA
    printf("    sizeof SHA          = %lu\n", sizeof(Sha));
#endif
#ifndef NO_SHA256
    printf("    sizeof SHA256       = %lu\n", sizeof(Sha256));
#endif
#ifdef WOLFSSL_SHA384
    printf("    sizeof SHA384       = %lu\n", sizeof(Sha384));
#endif
#ifdef WOLFSSL_SHA384
    printf("    sizeof SHA512       = %lu\n", sizeof(Sha512));
#endif
    printf("sizeof Buffers          = %lu\n", sizeof(Buffers));
    printf("sizeof Options          = %lu\n", sizeof(Options));
    printf("sizeof Arrays           = %lu\n", sizeof(Arrays));
#ifndef NO_RSA
    printf("sizeof RsaKey           = %lu\n", sizeof(RsaKey));
#endif
#ifdef HAVE_ECC
    printf("sizeof ecc_key          = %lu\n", sizeof(ecc_key));
#endif
    printf("sizeof WOLFSSL_CIPHER    = %lu\n", sizeof(WOLFSSL_CIPHER));
    printf("sizeof WOLFSSL_SESSION   = %lu\n", sizeof(WOLFSSL_SESSION));
    printf("sizeof WOLFSSL           = %lu\n", sizeof(WOLFSSL));
    printf("sizeof WOLFSSL_CTX       = %lu\n", sizeof(WOLFSSL_CTX));
#endif

    return sizeof(WOLFSSL);
}
#endif
#ifdef WOLFSSL_STATIC_MEMORY


int wolfSSL_init_memory_heap(WOLFSSL_HEAP* heap)
{
    word32 wc_MemSz[WOLFMEM_DEF_BUCKETS] = { WOLFMEM_BUCKETS };
    word32 wc_Dist[WOLFMEM_DEF_BUCKETS]  = { WOLFMEM_DIST };

    if (heap == NULL) {
        return BAD_FUNC_ARG;
    }

    XMEMSET(heap, 0, sizeof(WOLFSSL_HEAP));

    XMEMCPY(heap->sizeList, wc_MemSz, sizeof(wc_MemSz));
    XMEMCPY(heap->distList, wc_Dist,  sizeof(wc_Dist));

    if (InitMutex(&(heap->memory_mutex)) != 0) {
        WOLFSSL_MSG("Error creating heap memory mutex");
        return BAD_MUTEX_E;
    }

    return SSL_SUCCESS;
}


int wolfSSL_CTX_load_static_memory(WOLFSSL_CTX** ctx, wolfSSL_method_func method,
                                   unsigned char* buf, unsigned int sz,
                                   int flag, int max)
{
    WOLFSSL_HEAP*      heap;
    WOLFSSL_HEAP_HINT* hint;
    word32 idx = 0;

    if (ctx == NULL || buf == NULL) {
        return BAD_FUNC_ARG;
    }

    if (*ctx == NULL && method == NULL) {
        return BAD_FUNC_ARG;
    }

    if (*ctx == NULL) {
        if (sizeof(WOLFSSL_HEAP) + sizeof(WOLFSSL_HEAP_HINT) > sz - idx) {
            return BUFFER_E; /* not enough memory for structures */
        }
        heap = (WOLFSSL_HEAP*)buf;
        idx += sizeof(WOLFSSL_HEAP);
        if (wolfSSL_init_memory_heap(heap) != SSL_SUCCESS) {
            return SSL_FAILURE;
        }
        hint = (WOLFSSL_HEAP_HINT*)(buf + idx);
        idx += sizeof(WOLFSSL_HEAP_HINT);
        XMEMSET(hint, 0, sizeof(WOLFSSL_HEAP_HINT));
        hint->memory = heap;
    }
    else if ((*ctx)->heap == NULL) {
        if (sizeof(WOLFSSL_HEAP) + sizeof(WOLFSSL_HEAP_HINT) > sz - idx) {
            return BUFFER_E; /* not enough memory for structures */
        }
        heap = (WOLFSSL_HEAP*)buf;
        idx += sizeof(WOLFSSL_HEAP);
        if (wolfSSL_init_memory_heap(heap) != SSL_SUCCESS) {
            return SSL_FAILURE;
        }
        hint = (WOLFSSL_HEAP_HINT*)(buf + idx);
        idx += sizeof(WOLFSSL_HEAP_HINT);
        XMEMSET(hint, 0, sizeof(WOLFSSL_HEAP_HINT));
        hint->memory = heap;
        (*ctx)->heap = (void*)hint;
    }
    else {
#ifdef WOLFSSL_HEAP_TEST
        /* do not load in memory if test has been set */
        if ((*ctx)->heap == (void*)WOLFSSL_HEAP_TEST) {
            return SSL_SUCCESS;
        }
#endif
        hint = (WOLFSSL_HEAP_HINT*)((*ctx)->heap);
        heap = hint->memory;
    }

    if (wolfSSL_load_static_memory(buf + idx, sz - idx, flag, heap) != 1) {
        WOLFSSL_MSG("Error partitioning memory");
        return SSL_FAILURE;
    }

    /* create ctx if needed */
    if (*ctx == NULL) {
        *ctx = wolfSSL_CTX_new_ex(method(hint), hint);
        if (*ctx == NULL) {
            WOLFSSL_MSG("Error creating ctx");
            return SSL_FAILURE;
        }
    }

    /* determine what max applies too */
    if (flag & WOLFMEM_IO_POOL || flag & WOLFMEM_IO_POOL_FIXED) {
        heap->maxIO = max;
    }
    else { /* general memory used in handshakes */
        heap->maxHa = max;
    }

    heap->flag |= flag;

    (void)max;
    (void)method;

    return SSL_SUCCESS;
}


int wolfSSL_is_static_memory(WOLFSSL* ssl, WOLFSSL_MEM_CONN_STATS* mem_stats)
{
    if (ssl == NULL) {
        return BAD_FUNC_ARG;
    }
    WOLFSSL_ENTER("wolfSSL_is_static_memory");

    /* fill out statistics if wanted and WOLFMEM_TRACK_STATS flag */
    if (mem_stats != NULL && ssl->heap != NULL) {
        WOLFSSL_HEAP_HINT* hint = ((WOLFSSL_HEAP_HINT*)(ssl->heap));
        WOLFSSL_HEAP* heap      = hint->memory;
        if (heap->flag & WOLFMEM_TRACK_STATS && hint->stats != NULL) {
            XMEMCPY(mem_stats, hint->stats, sizeof(WOLFSSL_MEM_CONN_STATS));
        }
    }

    return (ssl->heap)? 1 : 0;
}


int wolfSSL_CTX_is_static_memory(WOLFSSL_CTX* ctx, WOLFSSL_MEM_STATS* mem_stats)
{
    if (ctx == NULL) {
        return BAD_FUNC_ARG;
    }
    WOLFSSL_ENTER("wolfSSL_CTX_is_static_memory");

    /* fill out statistics if wanted */
    if (mem_stats != NULL && ctx->heap != NULL) {
        WOLFSSL_HEAP* heap = ((WOLFSSL_HEAP_HINT*)(ctx->heap))->memory;
        if (wolfSSL_GetMemStats(heap, mem_stats) != 1) {
            return MEMORY_E;
        }
    }

    return (ctx->heap)? 1 : 0;
}

#endif /* WOLFSSL_STATIC_MEMORY */


/* return max record layer size plaintext input size */
int wolfSSL_GetMaxOutputSize(WOLFSSL* ssl)
{
    int maxSize = OUTPUT_RECORD_SIZE;

    WOLFSSL_ENTER("wolfSSL_GetMaxOutputSize");

    if (ssl == NULL)
        return BAD_FUNC_ARG;

    if (ssl->options.handShakeState != HANDSHAKE_DONE) {
        WOLFSSL_MSG("Handshake not complete yet");
        return BAD_FUNC_ARG;
    }

#ifdef HAVE_MAX_FRAGMENT
    maxSize = min(maxSize, ssl->max_fragment);
#endif

#ifdef WOLFSSL_DTLS
    if (ssl->options.dtls) {
        maxSize = min(maxSize, MAX_UDP_SIZE);
    }
#endif

    return maxSize;
}


/* return record layer size of plaintext input size */
int wolfSSL_GetOutputSize(WOLFSSL* ssl, int inSz)
{
    int maxSize;

    WOLFSSL_ENTER("wolfSSL_GetOutputSize");

    if (inSz < 0)
        return BAD_FUNC_ARG;

    maxSize = wolfSSL_GetMaxOutputSize(ssl);
    if (maxSize < 0)
        return maxSize;   /* error */
    if (inSz > maxSize)
        return INPUT_SIZE_E;

    return BuildMessage(ssl, NULL, 0, NULL, inSz, application_data, 0, 1);
}


#ifdef HAVE_ECC
int wolfSSL_CTX_SetMinEccKey_Sz(WOLFSSL_CTX* ctx, short keySz)
{
    if (ctx == NULL || keySz < 0 || keySz % 8 != 0) {
        WOLFSSL_MSG("Key size must be divisable by 8 or ctx was null");
        return BAD_FUNC_ARG;
    }

    ctx->minEccKeySz     = keySz / 8;
    ctx->cm->minEccKeySz = keySz / 8;
    return SSL_SUCCESS;
}


int wolfSSL_SetMinEccKey_Sz(WOLFSSL* ssl, short keySz)
{
    if (ssl == NULL || keySz < 0 || keySz % 8 != 0) {
        WOLFSSL_MSG("Key size must be divisable by 8 or ssl was null");
        return BAD_FUNC_ARG;
    }

    ssl->options.minEccKeySz = keySz / 8;
    return SSL_SUCCESS;
}
#endif /* !NO_RSA */

#ifndef NO_RSA
int wolfSSL_CTX_SetMinRsaKey_Sz(WOLFSSL_CTX* ctx, short keySz)
{
    if (ctx == NULL || keySz < 0 || keySz % 8 != 0) {
        WOLFSSL_MSG("Key size must be divisable by 8 or ctx was null");
        return BAD_FUNC_ARG;
    }

    ctx->minRsaKeySz     = keySz / 8;
    ctx->cm->minRsaKeySz = keySz / 8;
    return SSL_SUCCESS;
}


int wolfSSL_SetMinRsaKey_Sz(WOLFSSL* ssl, short keySz)
{
    if (ssl == NULL || keySz < 0 || keySz % 8 != 0) {
        WOLFSSL_MSG("Key size must be divisable by 8 or ssl was null");
        return BAD_FUNC_ARG;
    }

    ssl->options.minRsaKeySz = keySz / 8;
    return SSL_SUCCESS;
}
#endif /* !NO_RSA */

#ifndef NO_DH
/* server Diffie-Hellman parameters, SSL_SUCCESS on ok */
int wolfSSL_SetTmpDH(WOLFSSL* ssl, const unsigned char* p, int pSz,
                    const unsigned char* g, int gSz)
{
    word16 havePSK = 0;
    word16 haveRSA = 1;

    WOLFSSL_ENTER("wolfSSL_SetTmpDH");
    if (ssl == NULL || p == NULL || g == NULL) return BAD_FUNC_ARG;

    if (pSz < ssl->options.minDhKeySz)
        return DH_KEY_SIZE_E;

    if (ssl->options.side != WOLFSSL_SERVER_END)
        return SIDE_ERROR;

    if (ssl->buffers.serverDH_P.buffer && ssl->buffers.weOwnDH)
        XFREE(ssl->buffers.serverDH_P.buffer, ssl->heap, DYNAMIC_TYPE_DH);
    if (ssl->buffers.serverDH_G.buffer && ssl->buffers.weOwnDH)
        XFREE(ssl->buffers.serverDH_G.buffer, ssl->heap, DYNAMIC_TYPE_DH);

    ssl->buffers.weOwnDH = 1;  /* SSL owns now */
    ssl->buffers.serverDH_P.buffer = (byte*)XMALLOC(pSz, ssl->heap,
                                                    DYNAMIC_TYPE_DH);
    if (ssl->buffers.serverDH_P.buffer == NULL)
        return MEMORY_E;

    ssl->buffers.serverDH_G.buffer = (byte*)XMALLOC(gSz, ssl->heap,
                                                    DYNAMIC_TYPE_DH);
    if (ssl->buffers.serverDH_G.buffer == NULL) {
        XFREE(ssl->buffers.serverDH_P.buffer, ssl->heap, DYNAMIC_TYPE_DH);
        return MEMORY_E;
    }

    ssl->buffers.serverDH_P.length = pSz;
    ssl->buffers.serverDH_G.length = gSz;

    XMEMCPY(ssl->buffers.serverDH_P.buffer, p, pSz);
    XMEMCPY(ssl->buffers.serverDH_G.buffer, g, gSz);

    ssl->options.haveDH = 1;
    #ifndef NO_PSK
        havePSK = ssl->options.havePSK;
    #endif
    #ifdef NO_RSA
        haveRSA = 0;
    #endif
    InitSuites(ssl->suites, ssl->version, haveRSA, havePSK, ssl->options.haveDH,
               ssl->options.haveNTRU, ssl->options.haveECDSAsig,
               ssl->options.haveECC, ssl->options.haveStaticECC,
               ssl->options.side);

    WOLFSSL_LEAVE("wolfSSL_SetTmpDH", 0);
    return SSL_SUCCESS;
}

/* server ctx Diffie-Hellman parameters, SSL_SUCCESS on ok */
int wolfSSL_CTX_SetTmpDH(WOLFSSL_CTX* ctx, const unsigned char* p, int pSz,
                         const unsigned char* g, int gSz)
{
    WOLFSSL_ENTER("wolfSSL_CTX_SetTmpDH");
    if (ctx == NULL || p == NULL || g == NULL) return BAD_FUNC_ARG;

    if (pSz < ctx->minDhKeySz)
        return DH_KEY_SIZE_E;

    XFREE(ctx->serverDH_P.buffer, ctx->heap, DYNAMIC_TYPE_DH);
    XFREE(ctx->serverDH_G.buffer, ctx->heap, DYNAMIC_TYPE_DH);

    ctx->serverDH_P.buffer = (byte*)XMALLOC(pSz, ctx->heap, DYNAMIC_TYPE_DH);
    if (ctx->serverDH_P.buffer == NULL)
       return MEMORY_E;

    ctx->serverDH_G.buffer = (byte*)XMALLOC(gSz, ctx->heap, DYNAMIC_TYPE_DH);
    if (ctx->serverDH_G.buffer == NULL) {
        XFREE(ctx->serverDH_P.buffer, ctx->heap, DYNAMIC_TYPE_DH);
        return MEMORY_E;
    }

    ctx->serverDH_P.length = pSz;
    ctx->serverDH_G.length = gSz;

    XMEMCPY(ctx->serverDH_P.buffer, p, pSz);
    XMEMCPY(ctx->serverDH_G.buffer, g, gSz);

    ctx->haveDH = 1;

    WOLFSSL_LEAVE("wolfSSL_CTX_SetTmpDH", 0);
    return SSL_SUCCESS;
}


int wolfSSL_CTX_SetMinDhKey_Sz(WOLFSSL_CTX* ctx, word16 keySz)
{
    if (ctx == NULL || keySz > 16000 || keySz % 8 != 0)
        return BAD_FUNC_ARG;

    ctx->minDhKeySz = keySz / 8;
    return SSL_SUCCESS;
}


int wolfSSL_SetMinDhKey_Sz(WOLFSSL* ssl, word16 keySz)
{
    if (ssl == NULL || keySz > 16000 || keySz % 8 != 0)
        return BAD_FUNC_ARG;

    ssl->options.minDhKeySz = keySz / 8;
    return SSL_SUCCESS;
}


int wolfSSL_GetDhKey_Sz(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return (ssl->options.dhKeySz * 8);
}

#endif /* !NO_DH */


int wolfSSL_write(WOLFSSL* ssl, const void* data, int sz)
{
    int ret;

    WOLFSSL_ENTER("SSL_write()");

    if (ssl == NULL || data == NULL || sz < 0)
        return BAD_FUNC_ARG;

#ifdef HAVE_ERRNO_H
    errno = 0;
#endif

    ret = SendData(ssl, data, sz);

    WOLFSSL_LEAVE("SSL_write()", ret);

    if (ret < 0)
        return SSL_FATAL_ERROR;
    else
        return ret;
}


static int wolfSSL_read_internal(WOLFSSL* ssl, void* data, int sz, int peek)
{
    int ret;

    WOLFSSL_ENTER("wolfSSL_read_internal()");

    if (ssl == NULL || data == NULL || sz < 0)
        return BAD_FUNC_ARG;

#ifdef HAVE_ERRNO_H
        errno = 0;
#endif
#ifdef WOLFSSL_DTLS
    if (ssl->options.dtls)
        ssl->dtls_expected_rx = max(sz + 100, MAX_MTU);
#endif

#ifdef HAVE_MAX_FRAGMENT
    ret = ReceiveData(ssl, (byte*)data,
                      min(sz, min(ssl->max_fragment, OUTPUT_RECORD_SIZE)),peek);
#else
    ret = ReceiveData(ssl, (byte*)data, min(sz, OUTPUT_RECORD_SIZE), peek);
#endif

    WOLFSSL_LEAVE("wolfSSL_read_internal()", ret);

    if (ret < 0)
        return SSL_FATAL_ERROR;
    else
        return ret;
}


int wolfSSL_peek(WOLFSSL* ssl, void* data, int sz)
{
    WOLFSSL_ENTER("wolfSSL_peek()");

    return wolfSSL_read_internal(ssl, data, sz, TRUE);
}


int wolfSSL_read(WOLFSSL* ssl, void* data, int sz)
{
    WOLFSSL_ENTER("wolfSSL_read()");

    return wolfSSL_read_internal(ssl, data, sz, FALSE);
}


#ifdef HAVE_CAVIUM

/* let's use cavium, SSL_SUCCESS on ok */
int wolfSSL_UseCavium(WOLFSSL* ssl, int devId)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    ssl->devId = devId;

    return SSL_SUCCESS;
}


/* let's use cavium, SSL_SUCCESS on ok */
int wolfSSL_CTX_UseCavium(WOLFSSL_CTX* ctx, int devId)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    ctx->devId = devId;

    return SSL_SUCCESS;
}


#endif /* HAVE_CAVIUM */

#ifdef HAVE_SNI

int wolfSSL_UseSNI(WOLFSSL* ssl, byte type, const void* data, word16 size)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseSNI(&ssl->extensions, type, data, size, ssl->heap);
}


int wolfSSL_CTX_UseSNI(WOLFSSL_CTX* ctx, byte type, const void* data,
                                                                    word16 size)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseSNI(&ctx->extensions, type, data, size, ctx->heap);
}

#ifndef NO_WOLFSSL_SERVER

void wolfSSL_SNI_SetOptions(WOLFSSL* ssl, byte type, byte options)
{
    if (ssl && ssl->extensions)
        TLSX_SNI_SetOptions(ssl->extensions, type, options);
}


void wolfSSL_CTX_SNI_SetOptions(WOLFSSL_CTX* ctx, byte type, byte options)
{
    if (ctx && ctx->extensions)
        TLSX_SNI_SetOptions(ctx->extensions, type, options);
}


byte wolfSSL_SNI_Status(WOLFSSL* ssl, byte type)
{
    return TLSX_SNI_Status(ssl ? ssl->extensions : NULL, type);
}


word16 wolfSSL_SNI_GetRequest(WOLFSSL* ssl, byte type, void** data)
{
    if (data)
        *data = NULL;

    if (ssl && ssl->extensions)
        return TLSX_SNI_GetRequest(ssl->extensions, type, data);

    return 0;
}


int wolfSSL_SNI_GetFromBuffer(const byte* clientHello, word32 helloSz,
                              byte type, byte* sni, word32* inOutSz)
{
    if (clientHello && helloSz > 0 && sni && inOutSz && *inOutSz > 0)
        return TLSX_SNI_GetFromBuffer(clientHello, helloSz, type, sni, inOutSz);

    return BAD_FUNC_ARG;
}

#endif /* NO_WOLFSSL_SERVER */

#endif /* HAVE_SNI */


#ifdef HAVE_MAX_FRAGMENT
#ifndef NO_WOLFSSL_CLIENT

int wolfSSL_UseMaxFragment(WOLFSSL* ssl, byte mfl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseMaxFragment(&ssl->extensions, mfl, ssl->heap);
}


int wolfSSL_CTX_UseMaxFragment(WOLFSSL_CTX* ctx, byte mfl)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseMaxFragment(&ctx->extensions, mfl, ctx->heap);
}

#endif /* NO_WOLFSSL_CLIENT */
#endif /* HAVE_MAX_FRAGMENT */

#ifdef HAVE_TRUNCATED_HMAC
#ifndef NO_WOLFSSL_CLIENT

int wolfSSL_UseTruncatedHMAC(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseTruncatedHMAC(&ssl->extensions, ssl->heap);
}


int wolfSSL_CTX_UseTruncatedHMAC(WOLFSSL_CTX* ctx)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseTruncatedHMAC(&ctx->extensions, ctx->heap);
}

#endif /* NO_WOLFSSL_CLIENT */
#endif /* HAVE_TRUNCATED_HMAC */

#ifdef HAVE_CERTIFICATE_STATUS_REQUEST

int wolfSSL_UseOCSPStapling(WOLFSSL* ssl, byte status_type, byte options)
{
    if (ssl == NULL || ssl->options.side != WOLFSSL_CLIENT_END)
        return BAD_FUNC_ARG;

    return TLSX_UseCertificateStatusRequest(&ssl->extensions, status_type,
                                                            options, ssl->heap);
}


int wolfSSL_CTX_UseOCSPStapling(WOLFSSL_CTX* ctx, byte status_type,
                                                                   byte options)
{
    if (ctx == NULL || ctx->method->side != WOLFSSL_CLIENT_END)
        return BAD_FUNC_ARG;

    return TLSX_UseCertificateStatusRequest(&ctx->extensions, status_type,
                                                            options, ctx->heap);
}

#endif /* HAVE_CERTIFICATE_STATUS_REQUEST */

#ifdef HAVE_CERTIFICATE_STATUS_REQUEST_V2

int wolfSSL_UseOCSPStaplingV2(WOLFSSL* ssl, byte status_type, byte options)
{
    if (ssl == NULL || ssl->options.side != WOLFSSL_CLIENT_END)
        return BAD_FUNC_ARG;

    return TLSX_UseCertificateStatusRequestV2(&ssl->extensions, status_type,
                                                            options, ssl->heap);
}


int wolfSSL_CTX_UseOCSPStaplingV2(WOLFSSL_CTX* ctx,
                                                 byte status_type, byte options)
{
    if (ctx == NULL || ctx->method->side != WOLFSSL_CLIENT_END)
        return BAD_FUNC_ARG;

    return TLSX_UseCertificateStatusRequestV2(&ctx->extensions, status_type,
                                                            options, ctx->heap);
}

#endif /* HAVE_CERTIFICATE_STATUS_REQUEST_V2 */

/* Elliptic Curves */
#ifdef HAVE_SUPPORTED_CURVES
#ifndef NO_WOLFSSL_CLIENT

int wolfSSL_UseSupportedCurve(WOLFSSL* ssl, word16 name)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    switch (name) {
        case WOLFSSL_ECC_SECP160K1:
        case WOLFSSL_ECC_SECP160R1:
        case WOLFSSL_ECC_SECP160R2:
        case WOLFSSL_ECC_SECP192K1:
        case WOLFSSL_ECC_SECP192R1:
        case WOLFSSL_ECC_SECP224K1:
        case WOLFSSL_ECC_SECP224R1:
        case WOLFSSL_ECC_SECP256K1:
        case WOLFSSL_ECC_SECP256R1:
        case WOLFSSL_ECC_SECP384R1:
        case WOLFSSL_ECC_SECP521R1:
        case WOLFSSL_ECC_BRAINPOOLP256R1:
        case WOLFSSL_ECC_BRAINPOOLP384R1:
        case WOLFSSL_ECC_BRAINPOOLP512R1:
            break;

        default:
            return BAD_FUNC_ARG;
    }

    return TLSX_UseSupportedCurve(&ssl->extensions, name, ssl->heap);
}


int wolfSSL_CTX_UseSupportedCurve(WOLFSSL_CTX* ctx, word16 name)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    switch (name) {
        case WOLFSSL_ECC_SECP160K1:
        case WOLFSSL_ECC_SECP160R1:
        case WOLFSSL_ECC_SECP160R2:
        case WOLFSSL_ECC_SECP192K1:
        case WOLFSSL_ECC_SECP192R1:
        case WOLFSSL_ECC_SECP224K1:
        case WOLFSSL_ECC_SECP224R1:
        case WOLFSSL_ECC_SECP256K1:
        case WOLFSSL_ECC_SECP256R1:
        case WOLFSSL_ECC_SECP384R1:
        case WOLFSSL_ECC_SECP521R1:
        case WOLFSSL_ECC_BRAINPOOLP256R1:
        case WOLFSSL_ECC_BRAINPOOLP384R1:
        case WOLFSSL_ECC_BRAINPOOLP512R1:
            break;

        default:
            return BAD_FUNC_ARG;
    }

    return TLSX_UseSupportedCurve(&ctx->extensions, name, ctx->heap);
}

#endif /* NO_WOLFSSL_CLIENT */
#endif /* HAVE_SUPPORTED_CURVES */

/* QSH quantum safe handshake */
#ifdef HAVE_QSH
/* returns 1 if QSH has been used 0 otherwise */
int wolfSSL_isQSH(WOLFSSL* ssl)
{
    /* if no ssl struct than QSH was not used */
    if (ssl == NULL)
        return 0;

    return ssl->isQSH;
}


int wolfSSL_UseSupportedQSH(WOLFSSL* ssl, word16 name)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    switch (name) {
    #ifdef HAVE_NTRU
        case WOLFSSL_NTRU_EESS439:
        case WOLFSSL_NTRU_EESS593:
        case WOLFSSL_NTRU_EESS743:
            break;
    #endif
        default:
            return BAD_FUNC_ARG;
    }

    ssl->user_set_QSHSchemes = 1;

    return TLSX_UseQSHScheme(&ssl->extensions, name, NULL, 0, ssl->heap);
}

#ifndef NO_WOLFSSL_CLIENT
    /* user control over sending client public key in hello
       when flag = 1 will send keys if flag is 0 or function is not called
       then will not send keys in the hello extension
       return 0 on success
     */
    int wolfSSL_UseClientQSHKeys(WOLFSSL* ssl, unsigned char flag)
    {
        if (ssl == NULL)
            return BAD_FUNC_ARG;

        ssl->sendQSHKeys = flag;

        return 0;
    }
#endif /* NO_WOLFSSL_CLIENT */
#endif /* HAVE_QSH */


/* Application-Layer Protocol Negotiation */
#ifdef HAVE_ALPN

int wolfSSL_UseALPN(WOLFSSL* ssl, char *protocol_name_list,
                    word32 protocol_name_listSz, byte options)
{
    char    *list, *ptr, *token[10];
    word16  len;
    int     idx = 0;
    int     ret = SSL_FAILURE;

    WOLFSSL_ENTER("wolfSSL_UseALPN");

    if (ssl == NULL || protocol_name_list == NULL)
        return BAD_FUNC_ARG;

    if (protocol_name_listSz > (WOLFSSL_MAX_ALPN_NUMBER *
                                WOLFSSL_MAX_ALPN_PROTO_NAME_LEN +
                                WOLFSSL_MAX_ALPN_NUMBER)) {
        WOLFSSL_MSG("Invalid arguments, protocol name list too long");
        return BAD_FUNC_ARG;
    }

    if (!(options & WOLFSSL_ALPN_CONTINUE_ON_MISMATCH) &&
        !(options & WOLFSSL_ALPN_FAILED_ON_MISMATCH)) {
            WOLFSSL_MSG("Invalid arguments, options not supported");
            return BAD_FUNC_ARG;
        }


    list = (char *)XMALLOC(protocol_name_listSz+1, ssl->heap,
                           DYNAMIC_TYPE_TMP_BUFFER);
    if (list == NULL) {
        WOLFSSL_MSG("Memory failure");
        return MEMORY_ERROR;
    }

    XMEMSET(list, 0, protocol_name_listSz+1);
    XSTRNCPY(list, protocol_name_list, protocol_name_listSz);

    /* read all protocol name from the list */
    token[idx] = XSTRTOK(list, ",", &ptr);
    while (token[idx] != NULL)
        token[++idx] = XSTRTOK(NULL, ",", &ptr);

    /* add protocol name list in the TLS extension in reverse order */
    while ((idx--) > 0) {
        len = (word16)XSTRLEN(token[idx]);

        ret = TLSX_UseALPN(&ssl->extensions, token[idx], len, options,
                                                                     ssl->heap);
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("TLSX_UseALPN failure");
            break;
        }
    }

    XFREE(list, ssl->heap, DYNAMIC_TYPE_TMP_BUFFER);

    return ret;
}

int wolfSSL_ALPN_GetProtocol(WOLFSSL* ssl, char **protocol_name, word16 *size)
{
    return TLSX_ALPN_GetRequest(ssl ? ssl->extensions : NULL,
                               (void **)protocol_name, size);
}

int wolfSSL_ALPN_GetPeerProtocol(WOLFSSL* ssl, char **list, word16 *listSz)
{
    if (list == NULL || listSz == NULL)
        return BAD_FUNC_ARG;

    if (ssl->alpn_client_list == NULL)
        return BUFFER_ERROR;

    *listSz = (word16)XSTRLEN(ssl->alpn_client_list);
    if (*listSz == 0)
        return BUFFER_ERROR;

    *list = (char *)XMALLOC((*listSz)+1, ssl->heap, DYNAMIC_TYPE_TLSX);
    if (*list == NULL)
        return MEMORY_ERROR;

    XSTRNCPY(*list, ssl->alpn_client_list, (*listSz)+1);
    (*list)[*listSz] = 0;

    return SSL_SUCCESS;
}


/* used to free memory allocated by wolfSSL_ALPN_GetPeerProtocol */
int wolfSSL_ALPN_FreePeerProtocol(WOLFSSL* ssl, char **list)
{
    if (ssl == NULL) {
        return BAD_FUNC_ARG;
    }

    XFREE(*list, ssl->heap, DYNAMIC_TYPE_TLSX);
    *list = NULL;

    return SSL_SUCCESS;
}

#endif /* HAVE_ALPN */

/* Secure Renegotiation */
#ifdef HAVE_SECURE_RENEGOTIATION

/* user is forcing ability to use secure renegotiation, we discourage it */
int wolfSSL_UseSecureRenegotiation(WOLFSSL* ssl)
{
    int ret = BAD_FUNC_ARG;

    if (ssl)
        ret = TLSX_UseSecureRenegotiation(&ssl->extensions, ssl->heap);

    if (ret == SSL_SUCCESS) {
        TLSX* extension = TLSX_Find(ssl->extensions, TLSX_RENEGOTIATION_INFO);

        if (extension)
            ssl->secure_renegotiation = (SecureRenegotiation*)extension->data;
    }

    return ret;
}


/* do a secure renegotiation handshake, user forced, we discourage */
int wolfSSL_Rehandshake(WOLFSSL* ssl)
{
    int ret;

    if (ssl == NULL)
        return BAD_FUNC_ARG;

    if (ssl->secure_renegotiation == NULL) {
        WOLFSSL_MSG("Secure Renegotiation not forced on by user");
        return SECURE_RENEGOTIATION_E;
    }

    if (ssl->secure_renegotiation->enabled == 0) {
        WOLFSSL_MSG("Secure Renegotiation not enabled at extension level");
        return SECURE_RENEGOTIATION_E;
    }

    if (ssl->options.handShakeState != HANDSHAKE_DONE) {
        WOLFSSL_MSG("Can't renegotiate until previous handshake complete");
        return SECURE_RENEGOTIATION_E;
    }

#ifndef NO_FORCE_SCR_SAME_SUITE
    /* force same suite */
    if (ssl->suites) {
        ssl->suites->suiteSz = SUITE_LEN;
        ssl->suites->suites[0] = ssl->options.cipherSuite0;
        ssl->suites->suites[1] = ssl->options.cipherSuite;
    }
#endif

    /* reset handshake states */
    ssl->options.serverState = NULL_STATE;
    ssl->options.clientState = NULL_STATE;
    ssl->options.connectState  = CONNECT_BEGIN;
    ssl->options.acceptState   = ACCEPT_BEGIN;
    ssl->options.handShakeState = NULL_STATE;
    ssl->options.processReply  = 0;  /* TODO, move states in internal.h */

    XMEMSET(&ssl->msgsReceived, 0, sizeof(ssl->msgsReceived));

    ssl->secure_renegotiation->cache_status = SCR_CACHE_NEEDED;

#ifndef NO_OLD_TLS
#ifndef NO_MD5
    wc_InitMd5(&ssl->hsHashes->hashMd5);
#endif
#ifndef NO_SHA
    ret = wc_InitSha(&ssl->hsHashes->hashSha);
    if (ret !=0)
        return ret;
#endif
#endif /* NO_OLD_TLS */
#ifndef NO_SHA256
    ret = wc_InitSha256(&ssl->hsHashes->hashSha256);
    if (ret !=0)
        return ret;
#endif
#ifdef WOLFSSL_SHA384
    ret = wc_InitSha384(&ssl->hsHashes->hashSha384);
    if (ret !=0)
        return ret;
#endif
#ifdef WOLFSSL_SHA512
    ret = wc_InitSha512(&ssl->hsHashes->hashSha512);
    if (ret !=0)
        return ret;
#endif

    ret = wolfSSL_negotiate(ssl);
    return ret;
}

#endif /* HAVE_SECURE_RENEGOTIATION */

/* Session Ticket */
#if !defined(NO_WOLFSSL_SERVER) && defined(HAVE_SESSION_TICKET)
/* SSL_SUCCESS on ok */
int wolfSSL_CTX_set_TicketEncCb(WOLFSSL_CTX* ctx, SessionTicketEncCb cb)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    ctx->ticketEncCb = cb;

    return SSL_SUCCESS;
}

/* set hint interval, SSL_SUCCESS on ok */
int wolfSSL_CTX_set_TicketHint(WOLFSSL_CTX* ctx, int hint)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    ctx->ticketHint = hint;

    return SSL_SUCCESS;
}

/* set user context, SSL_SUCCESS on ok */
int wolfSSL_CTX_set_TicketEncCtx(WOLFSSL_CTX* ctx, void* userCtx)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    ctx->ticketEncCtx = userCtx;

    return SSL_SUCCESS;
}

#endif /* !defined(NO_WOLFSSL_CLIENT) && defined(HAVE_SESSION_TICKET) */

/* Session Ticket */
#if !defined(NO_WOLFSSL_CLIENT) && defined(HAVE_SESSION_TICKET)
int wolfSSL_UseSessionTicket(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseSessionTicket(&ssl->extensions, NULL, ssl->heap);
}

int wolfSSL_CTX_UseSessionTicket(WOLFSSL_CTX* ctx)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    return TLSX_UseSessionTicket(&ctx->extensions, NULL, ctx->heap);
}

WOLFSSL_API int wolfSSL_get_SessionTicket(WOLFSSL* ssl,
                                          byte* buf, word32* bufSz)
{
    if (ssl == NULL || buf == NULL || bufSz == NULL || *bufSz == 0)
        return BAD_FUNC_ARG;

    if (ssl->session.ticketLen <= *bufSz) {
        XMEMCPY(buf, ssl->session.ticket, ssl->session.ticketLen);
        *bufSz = ssl->session.ticketLen;
    }
    else
        *bufSz = 0;

    return SSL_SUCCESS;
}

WOLFSSL_API int wolfSSL_set_SessionTicket(WOLFSSL* ssl, byte* buf, word32 bufSz)
{
    if (ssl == NULL || (buf == NULL && bufSz > 0))
        return BAD_FUNC_ARG;

    if (bufSz > 0) {
        /* Ticket will fit into static ticket */
        if(bufSz <= SESSION_TICKET_LEN) {
            if (ssl->session.isDynamic) {
                XFREE(ssl->session.ticket, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
                ssl->session.isDynamic = 0;
                ssl->session.ticket = ssl->session.staticTicket;
            }
        } else { /* Ticket requires dynamic ticket storage */
            if (ssl->session.ticketLen < bufSz) { /* is dyn buffer big enough */
                if(ssl->session.isDynamic)
                    XFREE(ssl->session.ticket, ssl->heap,
                            DYNAMIC_TYPE_SESSION_TICK);
                ssl->session.ticket = XMALLOC(bufSz, ssl->heap,
                        DYNAMIC_TYPE_SESSION_TICK);
                if(!ssl->session.ticket) {
                    ssl->session.ticket = ssl->session.staticTicket;
                    ssl->session.isDynamic = 0;
                    return MEMORY_ERROR;
                }
                ssl->session.isDynamic = 1;
            }
        }
        XMEMCPY(ssl->session.ticket, buf, bufSz);
    }
    ssl->session.ticketLen = (word16)bufSz;

    return SSL_SUCCESS;
}


WOLFSSL_API int wolfSSL_set_SessionTicket_cb(WOLFSSL* ssl,
                                            CallbackSessionTicket cb, void* ctx)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    ssl->session_ticket_cb = cb;
    ssl->session_ticket_ctx = ctx;

    return SSL_SUCCESS;
}
#endif

#ifndef WOLFSSL_LEANPSK

int wolfSSL_send(WOLFSSL* ssl, const void* data, int sz, int flags)
{
    int ret;
    int oldFlags;

    WOLFSSL_ENTER("wolfSSL_send()");

    if (ssl == NULL || data == NULL || sz < 0)
        return BAD_FUNC_ARG;

    oldFlags = ssl->wflags;

    ssl->wflags = flags;
    ret = wolfSSL_write(ssl, data, sz);
    ssl->wflags = oldFlags;

    WOLFSSL_LEAVE("wolfSSL_send()", ret);

    return ret;
}


int wolfSSL_recv(WOLFSSL* ssl, void* data, int sz, int flags)
{
    int ret;
    int oldFlags;

    WOLFSSL_ENTER("wolfSSL_recv()");

    if (ssl == NULL || data == NULL || sz < 0)
        return BAD_FUNC_ARG;

    oldFlags = ssl->rflags;

    ssl->rflags = flags;
    ret = wolfSSL_read(ssl, data, sz);
    ssl->rflags = oldFlags;

    WOLFSSL_LEAVE("wolfSSL_recv()", ret);

    return ret;
}
#endif


/* SSL_SUCCESS on ok */
int wolfSSL_shutdown(WOLFSSL* ssl)
{
    int  ret = SSL_FATAL_ERROR;
    byte tmp;
    WOLFSSL_ENTER("SSL_shutdown()");

    if (ssl == NULL)
        return SSL_FATAL_ERROR;

    if (ssl->options.quietShutdown) {
        WOLFSSL_MSG("quiet shutdown, no close notify sent");
        return SSL_SUCCESS;
    }

    /* try to send close notify, not an error if can't */
    if (!ssl->options.isClosed && !ssl->options.connReset &&
                                  !ssl->options.sentNotify) {
        ssl->error = SendAlert(ssl, alert_warning, close_notify);
        if (ssl->error < 0) {
            WOLFSSL_ERROR(ssl->error);
            return SSL_FATAL_ERROR;
        }
        ssl->options.sentNotify = 1;  /* don't send close_notify twice */
        if (ssl->options.closeNotify)
            ret = SSL_SUCCESS;
        else
            ret = SSL_SHUTDOWN_NOT_DONE;

        WOLFSSL_LEAVE("SSL_shutdown()", ret);
        return ret;
    }

    /* call wolfSSL_shutdown again for bidirectional shutdown */
    if (ssl->options.sentNotify && !ssl->options.closeNotify) {
        ret = wolfSSL_read(ssl, &tmp, 0);
        if (ret < 0) {
            WOLFSSL_ERROR(ssl->error);
            ret = SSL_FATAL_ERROR;
        } else if (ssl->options.closeNotify) {
            ssl->error = SSL_ERROR_SYSCALL;   /* simulate OpenSSL behavior */
            ret = SSL_SUCCESS;
        }
    }

    WOLFSSL_LEAVE("SSL_shutdown()", ret);

    return ret;
}


int wolfSSL_get_error(WOLFSSL* ssl, int ret)
{
    WOLFSSL_ENTER("SSL_get_error");

    if (ret > 0)
        return SSL_ERROR_NONE;
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    WOLFSSL_LEAVE("SSL_get_error", ssl->error);

    /* make sure converted types are handled in SetErrorString() too */
    if (ssl->error == WANT_READ)
        return SSL_ERROR_WANT_READ;         /* convert to OpenSSL type */
    else if (ssl->error == WANT_WRITE)
        return SSL_ERROR_WANT_WRITE;        /* convert to OpenSSL type */
    else if (ssl->error == ZERO_RETURN)
        return SSL_ERROR_ZERO_RETURN;       /* convert to OpenSSL type */
    return ssl->error;
}


/* retrive alert history, SSL_SUCCESS on ok */
int wolfSSL_get_alert_history(WOLFSSL* ssl, WOLFSSL_ALERT_HISTORY *h)
{
    if (ssl && h) {
        *h = ssl->alert_history;
    }
    return SSL_SUCCESS;
}


/* return TRUE if current error is want read */
int wolfSSL_want_read(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_want_read");
    if (ssl->error == WANT_READ)
        return 1;

    return 0;
}


/* return TRUE if current error is want write */
int wolfSSL_want_write(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_want_write");
    if (ssl->error == WANT_WRITE)
        return 1;

    return 0;
}


char* wolfSSL_ERR_error_string(unsigned long errNumber, char* data)
{
    static const char* msg = "Please supply a buffer for error string";

    WOLFSSL_ENTER("ERR_error_string");
    if (data) {
        SetErrorString((int)errNumber, data);
        return data;
    }

    return (char*)msg;
}


void wolfSSL_ERR_error_string_n(unsigned long e, char* buf, unsigned long len)
{
    WOLFSSL_ENTER("wolfSSL_ERR_error_string_n");
    if (len >= WOLFSSL_MAX_ERROR_SZ)
        wolfSSL_ERR_error_string(e, buf);
    else {
        char tmp[WOLFSSL_MAX_ERROR_SZ];

        WOLFSSL_MSG("Error buffer too short, truncating");
        if (len) {
            wolfSSL_ERR_error_string(e, tmp);
            XMEMCPY(buf, tmp, len-1);
            buf[len-1] = '\0';
        }
    }
}


/* don't free temporary arrays at end of handshake */
void wolfSSL_KeepArrays(WOLFSSL* ssl)
{
    if (ssl)
        ssl->options.saveArrays = 1;
}


/* user doesn't need temporary arrays anymore, Free */
void wolfSSL_FreeArrays(WOLFSSL* ssl)
{
    if (ssl && ssl->options.handShakeState == HANDSHAKE_DONE) {
        ssl->options.saveArrays = 0;
        FreeArrays(ssl, 1);
    }
}


const byte* wolfSSL_GetMacSecret(WOLFSSL* ssl, int verify)
{
    if (ssl == NULL)
        return NULL;

    if ( (ssl->options.side == WOLFSSL_CLIENT_END && !verify) ||
         (ssl->options.side == WOLFSSL_SERVER_END &&  verify) )
        return ssl->keys.client_write_MAC_secret;
    else
        return ssl->keys.server_write_MAC_secret;
}


#ifdef ATOMIC_USER

void  wolfSSL_CTX_SetMacEncryptCb(WOLFSSL_CTX* ctx, CallbackMacEncrypt cb)
{
    if (ctx)
        ctx->MacEncryptCb = cb;
}


void  wolfSSL_SetMacEncryptCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->MacEncryptCtx = ctx;
}


void* wolfSSL_GetMacEncryptCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->MacEncryptCtx;

    return NULL;
}


void  wolfSSL_CTX_SetDecryptVerifyCb(WOLFSSL_CTX* ctx, CallbackDecryptVerify cb)
{
    if (ctx)
        ctx->DecryptVerifyCb = cb;
}


void  wolfSSL_SetDecryptVerifyCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->DecryptVerifyCtx = ctx;
}


void* wolfSSL_GetDecryptVerifyCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->DecryptVerifyCtx;

    return NULL;
}


const byte* wolfSSL_GetClientWriteKey(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->keys.client_write_key;

    return NULL;
}


const byte* wolfSSL_GetClientWriteIV(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->keys.client_write_IV;

    return NULL;
}


const byte* wolfSSL_GetServerWriteKey(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->keys.server_write_key;

    return NULL;
}


const byte* wolfSSL_GetServerWriteIV(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->keys.server_write_IV;

    return NULL;
}


int wolfSSL_GetKeySize(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->specs.key_size;

    return BAD_FUNC_ARG;
}


int wolfSSL_GetIVSize(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->specs.iv_size;

    return BAD_FUNC_ARG;
}


int wolfSSL_GetBulkCipher(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->specs.bulk_cipher_algorithm;

    return BAD_FUNC_ARG;
}


int wolfSSL_GetCipherType(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    if (ssl->specs.cipher_type == block)
        return WOLFSSL_BLOCK_TYPE;
    if (ssl->specs.cipher_type == stream)
        return WOLFSSL_STREAM_TYPE;
    if (ssl->specs.cipher_type == aead)
        return WOLFSSL_AEAD_TYPE;

    return -1;
}


int wolfSSL_GetCipherBlockSize(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return ssl->specs.block_size;
}


int wolfSSL_GetAeadMacSize(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return ssl->specs.aead_mac_size;
}


int wolfSSL_IsTLSv1_1(WOLFSSL* ssl)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    if (ssl->options.tls1_1)
        return 1;

    return 0;
}


int wolfSSL_GetSide(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->options.side;

    return BAD_FUNC_ARG;
}


int wolfSSL_GetHmacSize(WOLFSSL* ssl)
{
    /* AEAD ciphers don't have HMAC keys */
    if (ssl)
        return (ssl->specs.cipher_type != aead) ? ssl->specs.hash_size : 0;

    return BAD_FUNC_ARG;
}

#endif /* ATOMIC_USER */

#ifndef NO_CERTS

int AllocDer(DerBuffer** pDer, word32 length, int type, void* heap)
{
    int ret = BAD_FUNC_ARG;
    if (pDer) {
        int dynType = 0;
        DerBuffer* der;

        /* Determine dynamic type */
        switch (type) {
            case CA_TYPE:   dynType = DYNAMIC_TYPE_CA;   break;
            case CERT_TYPE: dynType = DYNAMIC_TYPE_CERT; break;
            case CRL_TYPE:  dynType = DYNAMIC_TYPE_CRL;  break;
            case DSA_TYPE:  dynType = DYNAMIC_TYPE_DSA;  break;
            case ECC_TYPE:  dynType = DYNAMIC_TYPE_ECC;  break;
            case RSA_TYPE:  dynType = DYNAMIC_TYPE_RSA;  break;
            default:        dynType = DYNAMIC_TYPE_KEY;  break;
        }

        /* Setup new buffer */
        *pDer = (DerBuffer*)XMALLOC(sizeof(DerBuffer) + length, heap, dynType);
        if (*pDer == NULL) {
            return MEMORY_ERROR;
        }

        der = *pDer;
        der->type = type;
        der->dynType = dynType; /* Cache this for FreeDer */
        der->heap = heap;
        der->buffer = (byte*)der + sizeof(DerBuffer);
        der->length = length;
        ret = 0; /* Success */
    }
    return ret;
}

void FreeDer(DerBuffer** pDer)
{
    if (pDer && *pDer)
    {
        DerBuffer* der = (DerBuffer*)*pDer;

        /* ForceZero private keys */
        if (der->type == PRIVATEKEY_TYPE) {
            ForceZero(der->buffer, der->length);
        }
        der->buffer = NULL;
        der->length = 0;
        XFREE(der, der->heap, der->dynType);

        *pDer = NULL;
    }
}


WOLFSSL_CERT_MANAGER* wolfSSL_CertManagerNew_ex(void* heap)
{
    WOLFSSL_CERT_MANAGER* cm = NULL;

    WOLFSSL_ENTER("wolfSSL_CertManagerNew");

    cm = (WOLFSSL_CERT_MANAGER*) XMALLOC(sizeof(WOLFSSL_CERT_MANAGER), heap,
                                         DYNAMIC_TYPE_CERT_MANAGER);
    if (cm) {
        XMEMSET(cm, 0, sizeof(WOLFSSL_CERT_MANAGER));

        if (InitMutex(&cm->caLock) != 0) {
            WOLFSSL_MSG("Bad mutex init");
            wolfSSL_CertManagerFree(cm);
            return NULL;
        }

        #ifdef WOLFSSL_TRUST_PEER_CERT
        if (InitMutex(&cm->tpLock) != 0) {
            WOLFSSL_MSG("Bad mutex init");
            wolfSSL_CertManagerFree(cm);
            return NULL;
        }
        #endif

        /* set default minimum key size allowed */
        #ifndef NO_RSA
            cm->minRsaKeySz = MIN_RSAKEY_SZ;
        #endif
        #ifdef HAVE_ECC
            cm->minEccKeySz = MIN_ECCKEY_SZ;
        #endif
            cm->heap = heap;
    }

    return cm;
}


WOLFSSL_CERT_MANAGER* wolfSSL_CertManagerNew(void)
{
    return wolfSSL_CertManagerNew_ex(NULL);
}


void wolfSSL_CertManagerFree(WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerFree");

    if (cm) {
        #ifdef HAVE_CRL
            if (cm->crl)
                FreeCRL(cm->crl, 1);
        #endif
        #ifdef HAVE_OCSP
            if (cm->ocsp)
                FreeOCSP(cm->ocsp, 1);
        #if defined(HAVE_CERTIFICATE_STATUS_REQUEST) \
         || defined(HAVE_CERTIFICATE_STATUS_REQUEST_V2)
            if (cm->ocsp_stapling)
                FreeOCSP(cm->ocsp_stapling, 1);
        #endif
        #endif
        FreeSignerTable(cm->caTable, CA_TABLE_SIZE, cm->heap);
        FreeMutex(&cm->caLock);

        #ifdef WOLFSSL_TRUST_PEER_CERT
        FreeTrustedPeerTable(cm->tpTable, TP_TABLE_SIZE, cm->heap);
        FreeMutex(&cm->tpLock);
        #endif

        XFREE(cm, cm->heap, DYNAMIC_TYPE_CERT_MANAGER);
    }

}


/* Unload the CA signer list */
int wolfSSL_CertManagerUnloadCAs(WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerUnloadCAs");

    if (cm == NULL)
        return BAD_FUNC_ARG;

    if (LockMutex(&cm->caLock) != 0)
        return BAD_MUTEX_E;

    FreeSignerTable(cm->caTable, CA_TABLE_SIZE, NULL);

    UnLockMutex(&cm->caLock);


    return SSL_SUCCESS;
}


#ifdef WOLFSSL_TRUST_PEER_CERT
int wolfSSL_CertManagerUnload_trust_peers(WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerUnload_trust_peers");

    if (cm == NULL)
        return BAD_FUNC_ARG;

    if (LockMutex(&cm->tpLock) != 0)
        return BAD_MUTEX_E;

    FreeTrustedPeerTable(cm->tpTable, TP_TABLE_SIZE, NULL);

    UnLockMutex(&cm->tpLock);


    return SSL_SUCCESS;
}
#endif /* WOLFSSL_TRUST_PEER_CERT */


/* Return bytes written to buff or < 0 for error */
int wolfSSL_CertPemToDer(const unsigned char* pem, int pemSz,
                        unsigned char* buff, int buffSz, int type)
{
    int            eccKey = 0;
    int            ret;
    DerBuffer*     der = NULL;
#ifdef WOLFSSL_SMALL_STACK
    EncryptedInfo* info = NULL;
#else
    EncryptedInfo  info[1];
#endif

    WOLFSSL_ENTER("wolfSSL_CertPemToDer");

    if (pem == NULL || buff == NULL || buffSz <= 0) {
        WOLFSSL_MSG("Bad pem der args");
        return BAD_FUNC_ARG;
    }

    if (type != CERT_TYPE && type != CA_TYPE && type != CERTREQ_TYPE) {
        WOLFSSL_MSG("Bad cert type");
        return BAD_FUNC_ARG;
    }

#ifdef WOLFSSL_SMALL_STACK
    info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), NULL,
                                   DYNAMIC_TYPE_TMP_BUFFER);
    if (info == NULL)
        return MEMORY_E;
#endif

    info->set      = 0;
    info->ctx      = NULL;
    info->consumed = 0;

    ret = PemToDer(pem, pemSz, type, &der, NULL, info, &eccKey);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (ret < 0) {
        WOLFSSL_MSG("Bad Pem To Der");
    }
    else {
        if (der->length <= (word32)buffSz) {
            XMEMCPY(buff, der->buffer, der->length);
            ret = der->length;
        }
        else {
            WOLFSSL_MSG("Bad der length");
            ret = BAD_FUNC_ARG;
        }
    }

    FreeDer(&der);
    return ret;
}

#endif /* NO_CERTS */

#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)

static const char *EVP_AES_128_CBC = "AES-128-CBC";
static const char *EVP_AES_192_CBC = "AES-192-CBC";
static const char *EVP_AES_256_CBC = "AES-256-CBC";
#if defined(OPENSSL_EXTRA)
    static const char *EVP_AES_128_CTR = "AES-128-CTR";
    static const char *EVP_AES_192_CTR = "AES-192-CTR";
    static const char *EVP_AES_256_CTR = "AES-256-CTR";
#endif
static const int  EVP_AES_SIZE = 11;

#ifndef NO_DES3
static const char *EVP_DES_CBC = "DES-CBC";
static const int  EVP_DES_SIZE = 7;

static const char *EVP_DES_EDE3_CBC = "DES-EDE3-CBC";
static const int  EVP_DES_EDE3_SIZE = 12;
#endif

#ifdef HAVE_IDEA
static const char *EVP_IDEA_CBC = "IDEA-CBC";
static const int  EVP_IDEA_SIZE = 8;
#endif

/* our KeyPemToDer password callback, password in userData */
static INLINE int OurPasswordCb(char* passwd, int sz, int rw, void* userdata)
{
    (void)rw;

    if (userdata == NULL)
        return 0;

    XSTRNCPY(passwd, (char*)userdata, sz);
    return min((word32)sz, (word32)XSTRLEN((char*)userdata));
}

#endif /* OPENSSL_EXTRA || HAVE_WEBSERVER */

#ifndef NO_CERTS

/* Return bytes written to buff or < 0 for error */
int wolfSSL_KeyPemToDer(const unsigned char* pem, int pemSz,
                        unsigned char* buff, int buffSz, const char* pass)
{
    int            eccKey = 0;
    int            ret;
    DerBuffer*     der = NULL;
#ifdef WOLFSSL_SMALL_STACK
    EncryptedInfo* info = NULL;
#else
    EncryptedInfo  info[1];
#endif

    WOLFSSL_ENTER("wolfSSL_KeyPemToDer");

    if (pem == NULL || buff == NULL || buffSz <= 0) {
        WOLFSSL_MSG("Bad pem der args");
        return BAD_FUNC_ARG;
    }

#ifdef WOLFSSL_SMALL_STACK
    info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), NULL,
                                   DYNAMIC_TYPE_TMP_BUFFER);
    if (info == NULL)
        return MEMORY_E;
#endif

    info->set      = 0;
    info->ctx      = NULL;
    info->consumed = 0;

#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)
    if (pass) {
        info->ctx = wolfSSL_CTX_new(wolfSSLv23_client_method());
        if (info->ctx == NULL) {
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            return MEMORY_E;
        }

        wolfSSL_CTX_set_default_passwd_cb(info->ctx, OurPasswordCb);
        wolfSSL_CTX_set_default_passwd_cb_userdata(info->ctx, (void*)pass);
    }
#else
    (void)pass;
#endif

    ret = PemToDer(pem, pemSz, PRIVATEKEY_TYPE, &der, NULL, info, &eccKey);

    if (info->ctx)
        wolfSSL_CTX_free(info->ctx);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (ret < 0) {
        WOLFSSL_MSG("Bad Pem To Der");
    }
    else {
        if (der->length <= (word32)buffSz) {
            XMEMCPY(buff, der->buffer, der->length);
            ret = der->length;
        }
        else {
            WOLFSSL_MSG("Bad der length");
            ret = BAD_FUNC_ARG;
        }
    }

    FreeDer(&der);
    return ret;
}

#endif /* !NO_CERTS */



#if !defined(NO_FILESYSTEM) && !defined(NO_STDIO_FILESYSTEM)

void wolfSSL_ERR_print_errors_fp(FILE* fp, int err)
{
    char data[WOLFSSL_MAX_ERROR_SZ + 1];

    WOLFSSL_ENTER("wolfSSL_ERR_print_errors_fp");
    SetErrorString(err, data);
    fprintf(fp, "%s", data);
}

#endif


int wolfSSL_pending(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_pending");
    return ssl->buffers.clearOutputBuffer.length;
}


#ifndef WOLFSSL_LEANPSK
/* turn on handshake group messages for context */
int wolfSSL_CTX_set_group_messages(WOLFSSL_CTX* ctx)
{
    if (ctx == NULL)
       return BAD_FUNC_ARG;

    ctx->groupMessages = 1;

    return SSL_SUCCESS;
}
#endif


#ifndef NO_WOLFSSL_CLIENT
/* connect enough to get peer cert chain */
int wolfSSL_connect_cert(WOLFSSL* ssl)
{
    int  ret;

    if (ssl == NULL)
        return SSL_FAILURE;

    ssl->options.certOnly = 1;
    ret = wolfSSL_connect(ssl);
    ssl->options.certOnly   = 0;

    return ret;
}
#endif


#ifndef WOLFSSL_LEANPSK
/* turn on handshake group messages for ssl object */
int wolfSSL_set_group_messages(WOLFSSL* ssl)
{
    if (ssl == NULL)
       return BAD_FUNC_ARG;

    ssl->options.groupMessages = 1;

    return SSL_SUCCESS;
}


/* make minVersion the internal equivalent SSL version */
static int SetMinVersionHelper(byte* minVersion, int version)
{
#ifdef NO_TLS
    (void)minVersion;
#endif

    switch (version) {
#if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
        case WOLFSSL_SSLV3:
            *minVersion = SSLv3_MINOR;
            break;
#endif

#ifndef NO_TLS
    #ifndef NO_OLD_TLS
        case WOLFSSL_TLSV1:
            *minVersion = TLSv1_MINOR;
            break;

        case WOLFSSL_TLSV1_1:
            *minVersion = TLSv1_1_MINOR;
            break;
    #endif
        case WOLFSSL_TLSV1_2:
            *minVersion = TLSv1_2_MINOR;
            break;
#endif

        default:
            WOLFSSL_MSG("Bad function argument");
            return BAD_FUNC_ARG;
    }

    return SSL_SUCCESS;
}


/* Set minimum downgrade version allowed, SSL_SUCCESS on ok */
int wolfSSL_CTX_SetMinVersion(WOLFSSL_CTX* ctx, int version)
{
    WOLFSSL_ENTER("wolfSSL_CTX_SetMinVersion");

    if (ctx == NULL) {
        WOLFSSL_MSG("Bad function argument");
        return BAD_FUNC_ARG;
    }

    return SetMinVersionHelper(&ctx->minDowngrade, version);
}


/* Set minimum downgrade version allowed, SSL_SUCCESS on ok */
int wolfSSL_SetMinVersion(WOLFSSL* ssl, int version)
{
    WOLFSSL_ENTER("wolfSSL_SetMinVersion");

    if (ssl == NULL) {
        WOLFSSL_MSG("Bad function argument");
        return BAD_FUNC_ARG;
    }

    return SetMinVersionHelper(&ssl->options.minDowngrade, version);
}


int wolfSSL_SetVersion(WOLFSSL* ssl, int version)
{
    word16 haveRSA = 1;
    word16 havePSK = 0;

    WOLFSSL_ENTER("wolfSSL_SetVersion");

    if (ssl == NULL) {
        WOLFSSL_MSG("Bad function argument");
        return BAD_FUNC_ARG;
    }

    switch (version) {
#if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
        case WOLFSSL_SSLV3:
            ssl->version = MakeSSLv3();
            break;
#endif

#ifndef NO_TLS
    #ifndef NO_OLD_TLS
        case WOLFSSL_TLSV1:
            ssl->version = MakeTLSv1();
            break;

        case WOLFSSL_TLSV1_1:
            ssl->version = MakeTLSv1_1();
            break;
    #endif
        case WOLFSSL_TLSV1_2:
            ssl->version = MakeTLSv1_2();
            break;
#endif

        default:
            WOLFSSL_MSG("Bad function argument");
            return BAD_FUNC_ARG;
    }

    #ifdef NO_RSA
        haveRSA = 0;
    #endif
    #ifndef NO_PSK
        havePSK = ssl->options.havePSK;
    #endif

    InitSuites(ssl->suites, ssl->version, haveRSA, havePSK, ssl->options.haveDH,
                ssl->options.haveNTRU, ssl->options.haveECDSAsig,
                ssl->options.haveECC, ssl->options.haveStaticECC,
                ssl->options.side);

    return SSL_SUCCESS;
}
#endif /* !leanpsk */


#if !defined(NO_CERTS) || !defined(NO_SESSION_CACHE)

/* Make a work from the front of random hash */
static INLINE word32 MakeWordFromHash(const byte* hashID)
{
    return (hashID[0] << 24) | (hashID[1] << 16) | (hashID[2] <<  8) |
            hashID[3];
}

#endif /* !NO_CERTS || !NO_SESSION_CACHE */


#ifndef NO_CERTS

/* hash is the SHA digest of name, just use first 32 bits as hash */
static INLINE word32 HashSigner(const byte* hash)
{
    return MakeWordFromHash(hash) % CA_TABLE_SIZE;
}


/* does CA already exist on signer list */
int AlreadySigner(WOLFSSL_CERT_MANAGER* cm, byte* hash)
{
    Signer* signers;
    int     ret = 0;
    word32  row = HashSigner(hash);

    if (LockMutex(&cm->caLock) != 0)
        return  ret;
    signers = cm->caTable[row];
    while (signers) {
        byte* subjectHash;
        #ifndef NO_SKID
            subjectHash = signers->subjectKeyIdHash;
        #else
            subjectHash = signers->subjectNameHash;
        #endif
        if (XMEMCMP(hash, subjectHash, SIGNER_DIGEST_SIZE) == 0) {
            ret = 1;
            break;
        }
        signers = signers->next;
    }
    UnLockMutex(&cm->caLock);

    return ret;
}


#ifdef WOLFSSL_TRUST_PEER_CERT
/* hash is the SHA digest of name, just use first 32 bits as hash */
static INLINE word32 TrustedPeerHashSigner(const byte* hash)
{
    return MakeWordFromHash(hash) % TP_TABLE_SIZE;
}

/* does trusted peer already exist on signer list */
int AlreadyTrustedPeer(WOLFSSL_CERT_MANAGER* cm, byte* hash)
{
    TrustedPeerCert* tp;
    int     ret = 0;
    word32  row = TrustedPeerHashSigner(hash);

    if (LockMutex(&cm->tpLock) != 0)
        return  ret;
    tp = cm->tpTable[row];
    while (tp) {
        byte* subjectHash;
        #ifndef NO_SKID
            subjectHash = tp->subjectKeyIdHash;
        #else
            subjectHash = tp->subjectNameHash;
        #endif
        if (XMEMCMP(hash, subjectHash, SIGNER_DIGEST_SIZE) == 0) {
            ret = 1;
            break;
        }
        tp = tp->next;
    }
    UnLockMutex(&cm->tpLock);

    return ret;
}


/* return Trusted Peer if found, otherwise NULL
    type is what to match on
 */
TrustedPeerCert* GetTrustedPeer(void* vp, byte* hash, int type)
{
    WOLFSSL_CERT_MANAGER* cm = (WOLFSSL_CERT_MANAGER*)vp;
    TrustedPeerCert* ret = NULL;
    TrustedPeerCert* tp  = NULL;
    word32  row;

    if (cm == NULL || hash == NULL)
        return NULL;

    row = TrustedPeerHashSigner(hash);

    if (LockMutex(&cm->tpLock) != 0)
        return ret;

    tp = cm->tpTable[row];
    while (tp) {
        byte* subjectHash;
        switch (type) {
            #ifndef NO_SKID
            case WC_MATCH_SKID:
                subjectHash = tp->subjectKeyIdHash;
                break;
            #endif
            case WC_MATCH_NAME:
                subjectHash = tp->subjectNameHash;
                break;
            default:
                WOLFSSL_MSG("Unknown search type");
                UnLockMutex(&cm->tpLock);
                return NULL;
        }
        if (XMEMCMP(hash, subjectHash, SIGNER_DIGEST_SIZE) == 0) {
            ret = tp;
            break;
        }
        tp = tp->next;
    }
    UnLockMutex(&cm->tpLock);

    return ret;
}


int MatchTrustedPeer(TrustedPeerCert* tp, DecodedCert* cert)
{
    if (tp == NULL || cert == NULL)
        return BAD_FUNC_ARG;

    /* subject key id or subject hash has been compared when searching
       tpTable for the cert from function GetTrustedPeer */

    /* compare signatures */
    if (tp->sigLen == cert->sigLength) {
        if (XMEMCMP(tp->sig, cert->signature, cert->sigLength)) {
            return SSL_FAILURE;
        }
    }
    else {
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}
#endif /* WOLFSSL_TRUST_PEER_CERT */


/* return CA if found, otherwise NULL */
Signer* GetCA(void* vp, byte* hash)
{
    WOLFSSL_CERT_MANAGER* cm = (WOLFSSL_CERT_MANAGER*)vp;
    Signer* ret = NULL;
    Signer* signers;
    word32  row = HashSigner(hash);

    if (cm == NULL)
        return NULL;

    if (LockMutex(&cm->caLock) != 0)
        return ret;

    signers = cm->caTable[row];
    while (signers) {
        byte* subjectHash;
        #ifndef NO_SKID
            subjectHash = signers->subjectKeyIdHash;
        #else
            subjectHash = signers->subjectNameHash;
        #endif
        if (XMEMCMP(hash, subjectHash, SIGNER_DIGEST_SIZE) == 0) {
            ret = signers;
            break;
        }
        signers = signers->next;
    }
    UnLockMutex(&cm->caLock);

    return ret;
}


#ifndef NO_SKID
/* return CA if found, otherwise NULL. Walk through hash table. */
Signer* GetCAByName(void* vp, byte* hash)
{
    WOLFSSL_CERT_MANAGER* cm = (WOLFSSL_CERT_MANAGER*)vp;
    Signer* ret = NULL;
    Signer* signers;
    word32  row;

    if (cm == NULL)
        return NULL;

    if (LockMutex(&cm->caLock) != 0)
        return ret;

    for (row = 0; row < CA_TABLE_SIZE && ret == NULL; row++) {
        signers = cm->caTable[row];
        while (signers && ret == NULL) {
            if (XMEMCMP(hash, signers->subjectNameHash,
                        SIGNER_DIGEST_SIZE) == 0) {
                ret = signers;
            }
            signers = signers->next;
        }
    }
    UnLockMutex(&cm->caLock);

    return ret;
}
#endif


#ifdef WOLFSSL_TRUST_PEER_CERT
/* add a trusted peer cert to linked list */
int AddTrustedPeer(WOLFSSL_CERT_MANAGER* cm, DerBuffer** pDer, int verify)
{
    int ret, row;
    TrustedPeerCert* peerCert;
    DecodedCert* cert = NULL;
    DerBuffer*   der = *pDer;
    byte* subjectHash = NULL;

    WOLFSSL_MSG("Adding a Trusted Peer Cert");

    cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), cm->heap,
                                 DYNAMIC_TYPE_TMP_BUFFER);
    if (cert == NULL)
        return MEMORY_E;

    InitDecodedCert(cert, der->buffer, der->length, cm->heap);
    if ((ret = ParseCert(cert, TRUSTED_PEER_TYPE, verify, cm)) != 0) {
        XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return ret;
    }
    WOLFSSL_MSG("    Parsed new trusted peer cert");

    peerCert = (TrustedPeerCert*)XMALLOC(sizeof(TrustedPeerCert), cm->heap,
                                                             DYNAMIC_TYPE_CERT);
    if (peerCert == NULL) {
        FreeDecodedCert(cert);
        XFREE(cert, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
        return MEMORY_E;
    }
    XMEMSET(peerCert, 0, sizeof(TrustedPeerCert));

#ifndef NO_SKID
    if (cert->extAuthKeyIdSet) {
        subjectHash = cert->extSubjKeyId;
    }
    else {
        subjectHash = cert->subjectHash;
    }
#else
    subjectHash = cert->subjectHash;
#endif

    #ifndef IGNORE_NAME_CONSTRAINTS
        if (peerCert->permittedNames)
            FreeNameSubtrees(peerCert->permittedNames, cm->heap);
        if (peerCert->excludedNames)
            FreeNameSubtrees(peerCert->excludedNames, cm->heap);
    #endif

    if (AlreadyTrustedPeer(cm, subjectHash)) {
        WOLFSSL_MSG("    Already have this CA, not adding again");
        (void)ret;
    }
    else {
        /* add trusted peer signature */
        peerCert->sigLen = cert->sigLength;
        peerCert->sig = XMALLOC(cert->sigLength, cm->heap,
                                                        DYNAMIC_TYPE_SIGNATURE);
        if (peerCert->sig == NULL) {
            FreeDecodedCert(cert);
            XFREE(cert, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
            FreeTrustedPeer(peerCert, cm->heap);
            return MEMORY_E;
        }
        XMEMCPY(peerCert->sig, cert->signature, cert->sigLength);

        /* add trusted peer name */
        peerCert->nameLen = cert->subjectCNLen;
        peerCert->name    = cert->subjectCN;
        #ifndef IGNORE_NAME_CONSTRAINTS
            peerCert->permittedNames = cert->permittedNames;
            peerCert->excludedNames  = cert->excludedNames;
        #endif

        /* add SKID when available and hash of name */
        #ifndef NO_SKID
            XMEMCPY(peerCert->subjectKeyIdHash, cert->extSubjKeyId,
                    SIGNER_DIGEST_SIZE);
        #endif
            XMEMCPY(peerCert->subjectNameHash, cert->subjectHash,
                    SIGNER_DIGEST_SIZE);
            peerCert->next    = NULL; /* If Key Usage not set, all uses valid. */
            cert->subjectCN = 0;
        #ifndef IGNORE_NAME_CONSTRAINTS
            cert->permittedNames = NULL;
            cert->excludedNames = NULL;
        #endif

        #ifndef NO_SKID
            if (cert->extAuthKeyIdSet) {
                row = TrustedPeerHashSigner(peerCert->subjectKeyIdHash);
            }
            else {
                row = TrustedPeerHashSigner(peerCert->subjectNameHash);
            }
        #else
            row = TrustedPeerHashSigner(peerCert->subjectNameHash);
        #endif

            if (LockMutex(&cm->tpLock) == 0) {
                peerCert->next = cm->tpTable[row];
                cm->tpTable[row] = peerCert;   /* takes ownership */
                UnLockMutex(&cm->tpLock);
            }
            else {
                WOLFSSL_MSG("    Trusted Peer Cert Mutex Lock failed");
                FreeDecodedCert(cert);
                XFREE(cert, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
                FreeTrustedPeer(peerCert, cm->heap);
                return BAD_MUTEX_E;
            }
        }

    WOLFSSL_MSG("    Freeing parsed trusted peer cert");
    FreeDecodedCert(cert);
    XFREE(cert, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
    WOLFSSL_MSG("    Freeing der trusted peer cert");
    FreeDer(&der);
    WOLFSSL_MSG("        OK Freeing der trusted peer cert");
    WOLFSSL_LEAVE("AddTrustedPeer", ret);

    return SSL_SUCCESS;
}
#endif /* WOLFSSL_TRUST_PEER_CERT */


/* owns der, internal now uses too */
/* type flag ids from user or from chain received during verify
   don't allow chain ones to be added w/o isCA extension */
int AddCA(WOLFSSL_CERT_MANAGER* cm, DerBuffer** pDer, int type, int verify)
{
    int         ret;
    Signer*     signer = 0;
    word32      row;
    byte*       subjectHash;
#ifdef WOLFSSL_SMALL_STACK
    DecodedCert* cert = NULL;
#else
    DecodedCert  cert[1];
#endif
    DerBuffer*   der = *pDer;

    WOLFSSL_MSG("Adding a CA");

#ifdef WOLFSSL_SMALL_STACK
    cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), NULL,
                                 DYNAMIC_TYPE_TMP_BUFFER);
    if (cert == NULL)
        return MEMORY_E;
#endif

    InitDecodedCert(cert, der->buffer, der->length, cm->heap);
    ret = ParseCert(cert, CA_TYPE, verify, cm);
    WOLFSSL_MSG("    Parsed new CA");

#ifndef NO_SKID
    subjectHash = cert->extSubjKeyId;
#else
    subjectHash = cert->subjectHash;
#endif

    /* check CA key size */
    if (verify) {
        switch (cert->keyOID) {
            #ifndef NO_RSA
            case RSAk:
                if (cm->minRsaKeySz < 0 ||
                                   cert->pubKeySize < (word16)cm->minRsaKeySz) {
                    ret = RSA_KEY_SIZE_E;
                    WOLFSSL_MSG("    CA RSA key size error");
                }
                break;
            #endif /* !NO_RSA */
            #ifdef HAVE_ECC
            case ECDSAk:
                if (cm->minEccKeySz < 0 ||
                                   cert->pubKeySize < (word16)cm->minEccKeySz) {
                    ret = ECC_KEY_SIZE_E;
                    WOLFSSL_MSG("    CA ECC key size error");
                }
                break;
            #endif /* HAVE_ECC */

            default:
                WOLFSSL_MSG("    No key size check done on CA");
                break; /* no size check if key type is not in switch */
        }
    }

    if (ret == 0 && cert->isCA == 0 && type != WOLFSSL_USER_CA) {
        WOLFSSL_MSG("    Can't add as CA if not actually one");
        ret = NOT_CA_ERROR;
    }
#ifndef ALLOW_INVALID_CERTSIGN
    else if (ret == 0 && cert->isCA == 1 && type != WOLFSSL_USER_CA &&
             (cert->extKeyUsage & KEYUSE_KEY_CERT_SIGN) == 0) {
        /* Intermediate CA certs are required to have the keyCertSign
        * extension set. User loaded root certs are not. */
        WOLFSSL_MSG("    Doesn't have key usage certificate signing");
        ret = NOT_CA_ERROR;
    }
#endif
    else if (ret == 0 && AlreadySigner(cm, subjectHash)) {
        WOLFSSL_MSG("    Already have this CA, not adding again");
        (void)ret;
    }
    else if (ret == 0) {
        /* take over signer parts */
        signer = MakeSigner(cm->heap);
        if (!signer)
            ret = MEMORY_ERROR;
        else {
            signer->keyOID         = cert->keyOID;
            signer->publicKey      = cert->publicKey;
            signer->pubKeySize     = cert->pubKeySize;
            signer->nameLen        = cert->subjectCNLen;
            signer->name           = cert->subjectCN;
        #ifndef IGNORE_NAME_CONSTRAINTS
            signer->permittedNames = cert->permittedNames;
            signer->excludedNames  = cert->excludedNames;
        #endif
        #ifndef NO_SKID
            XMEMCPY(signer->subjectKeyIdHash, cert->extSubjKeyId,
                    SIGNER_DIGEST_SIZE);
        #endif
            XMEMCPY(signer->subjectNameHash, cert->subjectHash,
                    SIGNER_DIGEST_SIZE);
            signer->keyUsage = cert->extKeyUsageSet ? cert->extKeyUsage
                                                    : 0xFFFF;
            signer->next    = NULL; /* If Key Usage not set, all uses valid. */
            cert->publicKey = 0;    /* in case lock fails don't free here.   */
            cert->subjectCN = 0;
        #ifndef IGNORE_NAME_CONSTRAINTS
            cert->permittedNames = NULL;
            cert->excludedNames = NULL;
        #endif

        #ifndef NO_SKID
            row = HashSigner(signer->subjectKeyIdHash);
        #else
            row = HashSigner(signer->subjectNameHash);
        #endif

            if (LockMutex(&cm->caLock) == 0) {
                signer->next = cm->caTable[row];
                cm->caTable[row] = signer;   /* takes ownership */
                UnLockMutex(&cm->caLock);
                if (cm->caCacheCallback)
                    cm->caCacheCallback(der->buffer, (int)der->length, type);
            }
            else {
                WOLFSSL_MSG("    CA Mutex Lock failed");
                ret = BAD_MUTEX_E;
                FreeSigner(signer, cm->heap);
            }
        }
    }

    WOLFSSL_MSG("    Freeing Parsed CA");
    FreeDecodedCert(cert);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
    WOLFSSL_MSG("    Freeing der CA");
    FreeDer(pDer);
    WOLFSSL_MSG("        OK Freeing der CA");

    WOLFSSL_LEAVE("AddCA", ret);

    return ret == 0 ? SSL_SUCCESS : ret;
}

#endif /* !NO_CERTS */


#ifndef NO_SESSION_CACHE

    /* basic config gives a cache with 33 sessions, adequate for clients and
       embedded servers

       MEDIUM_SESSION_CACHE allows 1055 sessions, adequate for servers that
       aren't under heavy load, basically allows 200 new sessions per minute

       BIG_SESSION_CACHE yields 20,027 sessions

       HUGE_SESSION_CACHE yields 65,791 sessions, for servers under heavy load,
       allows over 13,000 new sessions per minute or over 200 new sessions per
       second

       SMALL_SESSION_CACHE only stores 6 sessions, good for embedded clients
       or systems where the default of nearly 3kB is too much RAM, this define
       uses less than 500 bytes RAM

       default SESSION_CACHE stores 33 sessions (no XXX_SESSION_CACHE defined)
    */
    #ifdef HUGE_SESSION_CACHE
        #define SESSIONS_PER_ROW 11
        #define SESSION_ROWS 5981
    #elif defined(BIG_SESSION_CACHE)
        #define SESSIONS_PER_ROW 7
        #define SESSION_ROWS 2861
    #elif defined(MEDIUM_SESSION_CACHE)
        #define SESSIONS_PER_ROW 5
        #define SESSION_ROWS 211
    #elif defined(SMALL_SESSION_CACHE)
        #define SESSIONS_PER_ROW 2
        #define SESSION_ROWS 3
    #else
        #define SESSIONS_PER_ROW 3
        #define SESSION_ROWS 11
    #endif

    typedef struct SessionRow {
        int nextIdx;                           /* where to place next one   */
        int totalCount;                        /* sessions ever on this row */
        WOLFSSL_SESSION Sessions[SESSIONS_PER_ROW];
    } SessionRow;

    static SessionRow SessionCache[SESSION_ROWS];

    #if defined(WOLFSSL_SESSION_STATS) && defined(WOLFSSL_PEAK_SESSIONS)
        static word32 PeakSessions;
    #endif

    static wolfSSL_Mutex session_mutex;   /* SessionCache mutex */

    #ifndef NO_CLIENT_CACHE

        typedef struct ClientSession {
            word16 serverRow;            /* SessionCache Row id */
            word16 serverIdx;            /* SessionCache Idx (column) */
        } ClientSession;

        typedef struct ClientRow {
            int nextIdx;                /* where to place next one   */
            int totalCount;             /* sessions ever on this row */
            ClientSession Clients[SESSIONS_PER_ROW];
        } ClientRow;

        static ClientRow ClientCache[SESSION_ROWS];  /* Client Cache */
                                                     /* uses session mutex */
    #endif  /* NO_CLIENT_CACHE */

#endif /* NO_SESSION_CACHE */

int wolfSSL_Init(void)
{
    WOLFSSL_ENTER("wolfSSL_Init");

    if (initRefCount == 0) {
        /* Initialize crypto for use with TLS connection */
        if (wolfCrypt_Init() != 0) {
            WOLFSSL_MSG("Bad wolfCrypt Init");
            return WC_INIT_E;
        }
#ifndef NO_SESSION_CACHE
        if (InitMutex(&session_mutex) != 0) {
            WOLFSSL_MSG("Bad Init Mutex session");
            return BAD_MUTEX_E;
        }
#endif
        if (InitMutex(&count_mutex) != 0) {
            WOLFSSL_MSG("Bad Init Mutex count");
            return BAD_MUTEX_E;
        }
    }

    if (LockMutex(&count_mutex) != 0) {
        WOLFSSL_MSG("Bad Lock Mutex count");
        return BAD_MUTEX_E;
    }

    initRefCount++;
    UnLockMutex(&count_mutex);

    return SSL_SUCCESS;
}


#if (defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)) && !defined(NO_CERTS)

/* SSL_SUCCESS if ok, <= 0 else */
static int wolfssl_decrypt_buffer_key(DerBuffer* der, byte* password,
                                      int passwordSz, EncryptedInfo* info)
{
    int ret = SSL_BAD_FILE;

#ifdef WOLFSSL_SMALL_STACK
    byte* key      = NULL;
#else
    byte  key[AES_256_KEY_SIZE];
#endif

    WOLFSSL_ENTER("wolfssl_decrypt_buffer_key");

    if (der == NULL || password == NULL || info == NULL) {
        WOLFSSL_MSG("bad arguments");
        return SSL_FATAL_ERROR;
    }

    /* use file's salt for key derivation, hex decode first */
    if (Base16_Decode(info->iv, info->ivSz, info->iv, &info->ivSz) != 0) {
        WOLFSSL_MSG("base16 decode failed");
        return SSL_FATAL_ERROR;
    }

#ifndef NO_MD5

#ifdef WOLFSSL_SMALL_STACK
    key = (byte*)XMALLOC(AES_256_KEY_SIZE, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (key == NULL) {
        WOLFSSL_MSG("memory failure");
        return SSL_FATAL_ERROR;
    }
#endif /* WOLFSSL_SMALL_STACK */

    if ((ret = wolfSSL_EVP_BytesToKey(info->name, "MD5", info->iv,
                              password, passwordSz, 1, key, NULL)) <= 0) {
        WOLFSSL_MSG("bytes to key failure");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(key, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return SSL_FATAL_ERROR;
    }
#else
    (void) passwordSz;
#endif /* NO_MD5 */

#ifndef NO_DES3
    if (XSTRNCMP(info->name, EVP_DES_CBC, EVP_DES_SIZE) == 0)
        ret = wc_Des_CbcDecryptWithKey(der->buffer, der->buffer, der->length,
                                       key, info->iv);
    else if (XSTRNCMP(info->name, EVP_DES_EDE3_CBC, EVP_DES_EDE3_SIZE) == 0)
        ret = wc_Des3_CbcDecryptWithKey(der->buffer, der->buffer, der->length,
                                        key, info->iv);
#endif /* NO_DES3 */
#if !defined(NO_AES) && defined(HAVE_AES_CBC)
    if (XSTRNCMP(info->name, EVP_AES_128_CBC, EVP_AES_SIZE) == 0)
        ret = wc_AesCbcDecryptWithKey(der->buffer, der->buffer, der->length,
                                      key, AES_128_KEY_SIZE, info->iv);
    else if (XSTRNCMP(info->name, EVP_AES_192_CBC, EVP_AES_SIZE) == 0)
        ret = wc_AesCbcDecryptWithKey(der->buffer, der->buffer, der->length,
                                      key, AES_192_KEY_SIZE, info->iv);
    else if (XSTRNCMP(info->name, EVP_AES_256_CBC, EVP_AES_SIZE) == 0)
        ret = wc_AesCbcDecryptWithKey(der->buffer, der->buffer, der->length,
                                      key, AES_256_KEY_SIZE, info->iv);
#endif /* !NO_AES && HAVE_AES_CBC */

#ifdef WOLFSSL_SMALL_STACK
    XFREE(key, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (ret == MP_OKAY)
        return SSL_SUCCESS;
    else if (ret == SSL_BAD_FILE)
        return SSL_BAD_FILE;

    return SSL_FATAL_ERROR;
}
#endif /* defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER) */


#if defined(WOLFSSL_KEY_GEN) && defined(OPENSSL_EXTRA)
static int wolfssl_encrypt_buffer_key(byte* der, word32 derSz, byte* password,
                                      int passwordSz, EncryptedInfo* info)
{
    int ret = SSL_BAD_FILE;

#ifdef WOLFSSL_SMALL_STACK
    byte* key      = NULL;
#else
    byte  key[AES_256_KEY_SIZE];
#endif

    WOLFSSL_ENTER("wolfssl_encrypt_buffer_key");

    if (der == NULL || password == NULL || info == NULL || info->ivSz == 0) {
        WOLFSSL_MSG("bad arguments");
        return SSL_FATAL_ERROR;
    }

#ifndef NO_MD5

#ifdef WOLFSSL_SMALL_STACK
    key = (byte*)XMALLOC(AES_256_KEY_SIZE, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (key == NULL) {
        WOLFSSL_MSG("memory failure");
        return SSL_FATAL_ERROR;
    }
#endif /* WOLFSSL_SMALL_STACK */

    if ((ret = wolfSSL_EVP_BytesToKey(info->name, "MD5", info->iv,
                              password, passwordSz, 1, key, NULL)) <= 0) {
        WOLFSSL_MSG("bytes to key failure");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(key, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return SSL_FATAL_ERROR;
    }
#else
    (void) passwordSz;
#endif /* NO_MD5 */

#ifndef NO_DES3
    if (XSTRNCMP(info->name, EVP_DES_CBC, EVP_DES_SIZE) == 0)
        ret = wc_Des_CbcEncryptWithKey(der, der, derSz, key, info->iv);
    else if (XSTRNCMP(info->name, EVP_DES_EDE3_CBC, EVP_DES_EDE3_SIZE) == 0)
        ret = wc_Des3_CbcEncryptWithKey(der, der, derSz, key, info->iv);
#endif /* NO_DES3 */
#ifndef NO_AES
    if (XSTRNCMP(info->name, EVP_AES_128_CBC, EVP_AES_SIZE) == 0)
        ret = wc_AesCbcEncryptWithKey(der, der, derSz,
                                      key, AES_128_KEY_SIZE, info->iv);
    else if (XSTRNCMP(info->name, EVP_AES_192_CBC, EVP_AES_SIZE) == 0)
        ret = wc_AesCbcEncryptWithKey(der, der, derSz,
                                      key, AES_192_KEY_SIZE, info->iv);
    else if (XSTRNCMP(info->name, EVP_AES_256_CBC, EVP_AES_SIZE) == 0)
        ret = wc_AesCbcEncryptWithKey(der, der, derSz,
                                      key, AES_256_KEY_SIZE, info->iv);
#endif /* NO_AES */

#ifdef WOLFSSL_SMALL_STACK
    XFREE(key, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (ret == MP_OKAY)
        return SSL_SUCCESS;
    else if (ret == SSL_BAD_FILE)
        return SSL_BAD_FILE;

    return SSL_FATAL_ERROR;
}
#endif /* defined(WOLFSSL_KEY_GEN) */


#ifndef NO_CERTS

/* Remove PEM header/footer, convert to ASN1, store any encrypted data
   info->consumed tracks of PEM bytes consumed in case multiple parts */
int PemToDer(const unsigned char* buff, long longSz, int type,
              DerBuffer** pDer, void* heap, EncryptedInfo* info, int* eccKey)
{
    const char* header      = NULL;
    const char* footer      = NULL;
    char*       headerEnd;
    char*       footerEnd;
    char*       consumedEnd;
    char*       bufferEnd   = (char*)(buff + longSz);
    long        neededSz;
    int         ret         = 0;
    int         sz          = (int)longSz;
    int         encrypted_key = 0;
    DerBuffer*  der;

    WOLFSSL_ENTER("PemToDer");

    switch (type) {
        case CA_TYPE:       /* same as below */
        case TRUSTED_PEER_TYPE:
        case CERT_TYPE:      header=BEGIN_CERT;     footer=END_CERT;     break;
        case CRL_TYPE:       header=BEGIN_X509_CRL; footer=END_X509_CRL; break;
        case DH_PARAM_TYPE:  header=BEGIN_DH_PARAM; footer=END_DH_PARAM; break;
        case CERTREQ_TYPE:   header=BEGIN_CERT_REQ; footer=END_CERT_REQ; break;
        case DSA_TYPE:       header=BEGIN_DSA_PRIV; footer=END_DSA_PRIV; break;
        case ECC_TYPE:       header=BEGIN_EC_PRIV;  footer=END_EC_PRIV;  break;
        case RSA_TYPE:       header=BEGIN_RSA_PRIV; footer=END_RSA_PRIV; break;
        case PUBLICKEY_TYPE: header=BEGIN_PUB_KEY;  footer=END_PUB_KEY;  break;
        default:             header=BEGIN_RSA_PRIV; footer=END_RSA_PRIV; break;
    }

    /* find header */
    for (;;) {
        headerEnd = XSTRNSTR((char*)buff, header, sz);

        if (headerEnd || type != PRIVATEKEY_TYPE) {
            break;
        } else if (header == BEGIN_RSA_PRIV) {
                   header =  BEGIN_PRIV_KEY;       footer = END_PRIV_KEY;
        } else if (header == BEGIN_PRIV_KEY) {
                   header =  BEGIN_ENC_PRIV_KEY;   footer = END_ENC_PRIV_KEY;
        } else if (header == BEGIN_ENC_PRIV_KEY) {
                   header =  BEGIN_EC_PRIV;        footer = END_EC_PRIV;
        } else if (header == BEGIN_EC_PRIV) {
                   header =  BEGIN_DSA_PRIV;       footer = END_DSA_PRIV;
        } else
            break;
    }

    if (!headerEnd) {
        WOLFSSL_MSG("Couldn't find PEM header");
        return SSL_NO_PEM_HEADER;
    }

    headerEnd += XSTRLEN(header);

    if ((headerEnd + 1) >= bufferEnd)
        return SSL_BAD_FILE;

    /* eat end of line */
    if (headerEnd[0] == '\n')
        headerEnd++;
    else if (headerEnd[1] == '\n')
        headerEnd += 2;
    else {
        if (info)
            info->consumed = (long)(headerEnd+2 - (char*)buff);
        return SSL_BAD_FILE;
    }

    if (type == PRIVATEKEY_TYPE) {
        if (eccKey)
            *eccKey = header == BEGIN_EC_PRIV;
    }

#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)
    {
        /* remove encrypted header if there */
        char encHeader[] = "Proc-Type";
        char* line = XSTRNSTR(headerEnd, encHeader, PEM_LINE_LEN);
        if (line) {
            char* newline;
            char* finish;
            char* start  = XSTRNSTR(line, "DES", PEM_LINE_LEN);

            if (!start)
                start = XSTRNSTR(line, "AES", PEM_LINE_LEN);

            if (!start) return SSL_BAD_FILE;
            if (!info)  return SSL_BAD_FILE;

            finish = XSTRNSTR(start, ",", PEM_LINE_LEN);

            if (start && finish && (start < finish)) {
                newline = XSTRNSTR(finish, "\r", PEM_LINE_LEN);

                if (XMEMCPY(info->name, start, finish - start) == NULL)
                    return SSL_FATAL_ERROR;
                info->name[finish - start] = 0;
                if (XMEMCPY(info->iv, finish + 1, sizeof(info->iv)) == NULL)
                    return SSL_FATAL_ERROR;

                if (!newline) newline = XSTRNSTR(finish, "\n", PEM_LINE_LEN);
                if (newline && (newline > finish)) {
                    info->ivSz = (word32)(newline - (finish + 1));
                    info->set = 1;
                }
                else
                    return SSL_BAD_FILE;
            }
            else
                return SSL_BAD_FILE;

            /* eat blank line */
            while (*newline == '\r' || *newline == '\n')
                newline++;
            headerEnd = newline;

            encrypted_key = 1;
        }
    }
#endif /* OPENSSL_EXTRA || HAVE_WEBSERVER */

    /* find footer */
    footerEnd = XSTRNSTR((char*)buff, footer, sz);
    if (!footerEnd) {
        if (info)
            info->consumed = longSz; /* No more certs if no footer */
        return SSL_BAD_FILE;
    }

    consumedEnd = footerEnd + XSTRLEN(footer);

    if (consumedEnd < bufferEnd) {  /* handle no end of line on last line */
        /* eat end of line */
        if (consumedEnd[0] == '\n')
            consumedEnd++;
        else if ((consumedEnd + 1 < bufferEnd) && consumedEnd[1] == '\n')
            consumedEnd += 2;
        else {
            if (info)
                info->consumed = (long)(consumedEnd+2 - (char*)buff);
            return SSL_BAD_FILE;
        }
    }

    if (info)
        info->consumed = (long)(consumedEnd - (char*)buff);

    /* set up der buffer */
    neededSz = (long)(footerEnd - headerEnd);
    if (neededSz > sz || neededSz <= 0)
        return SSL_BAD_FILE;

    ret = AllocDer(pDer, (word32)neededSz, type, heap);
    if (ret < 0) {
        return ret;
    }
    der = *pDer;

    if (Base64_Decode((byte*)headerEnd, (word32)neededSz,
                      der->buffer, &der->length) < 0)
        return SSL_BAD_FILE;

    if (header == BEGIN_PRIV_KEY && !encrypted_key) {
        /* pkcs8 key, convert and adjust length */
        if ((ret = ToTraditional(der->buffer, der->length)) < 0)
            return ret;

        der->length = ret;
        return 0;
    }

#if (defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)) && !defined(NO_PWDBASED)
    if (encrypted_key || header == BEGIN_ENC_PRIV_KEY) {
        int   passwordSz;
    #ifdef WOLFSSL_SMALL_STACK
        char* password = NULL;
    #else
        char  password[80];
    #endif

        if (!info || !info->ctx || !info->ctx->passwd_cb)
            return SSL_BAD_FILE;  /* no callback error */

    #ifdef WOLFSSL_SMALL_STACK
        password = (char*)XMALLOC(80, heap, DYNAMIC_TYPE_TMP_BUFFER);
        if (password == NULL)
            return MEMORY_E;
    #endif
        passwordSz = info->ctx->passwd_cb(password, sizeof(password), 0,
                                          info->ctx->userdata);
        /* convert and adjust length */
        if (header == BEGIN_ENC_PRIV_KEY) {
            ret = ToTraditionalEnc(der->buffer, der->length,
                                   password, passwordSz);
    #ifdef WOLFSSL_SMALL_STACK
            XFREE(password, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
            if (ret < 0) {
                return ret;
            }

            der->length = ret;
        }
        /* decrypt the key */
        else {
            ret = wolfssl_decrypt_buffer_key(der, (byte*)password,
                                             passwordSz, info);
    #ifdef WOLFSSL_SMALL_STACK
            XFREE(password, heap, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
            if (ret != SSL_SUCCESS) {
                return ret;
            }
        }
    }
#endif  /* OPENSSL_EXTRA || HAVE_WEBSERVER || NO_PWDBASED */

    return 0;
}


/* process the buffer buff, length sz, into ctx of format and type
   used tracks bytes consumed, userChain specifies a user cert chain
   to pass during the handshake */
static int ProcessBuffer(WOLFSSL_CTX* ctx, const unsigned char* buff,
                         long sz, int format, int type, WOLFSSL* ssl,
                         long* used, int userChain)
{
    DerBuffer*    der = NULL;        /* holds DER or RAW (for NTRU) */
    int           ret;
    int           eccKey = 0;
    int           rsaKey = 0;
    void*         heap = ctx ? ctx->heap : ((ssl) ? ssl->heap : NULL);
#ifdef WOLFSSL_SMALL_STACK
    EncryptedInfo* info = NULL;
#else
    EncryptedInfo  info[1];
#endif

    (void)rsaKey;

    if (used)
        *used = sz;     /* used bytes default to sz, PEM chain may shorten*/

    if (format != SSL_FILETYPE_ASN1 && format != SSL_FILETYPE_PEM
                                    && format != SSL_FILETYPE_RAW)
        return SSL_BAD_FILETYPE;

    if (ctx == NULL && ssl == NULL)
        return BAD_FUNC_ARG;

#ifdef WOLFSSL_SMALL_STACK
    info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), heap,
                                   DYNAMIC_TYPE_TMP_BUFFER);
    if (info == NULL)
        return MEMORY_E;
#endif

    info->set      = 0;
    info->ctx      = ctx;
    info->consumed = 0;

    if (format == SSL_FILETYPE_PEM) {
        ret = PemToDer(buff, sz, type, &der, heap, info, &eccKey);

        if (used)
            *used = info->consumed;

        if (ret < 0) {
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            FreeDer(&der);
            return ret;
        }

        /* we may have a user cert chain, try to consume */
        if (userChain && type == CERT_TYPE && info->consumed < sz) {
        #ifdef WOLFSSL_SMALL_STACK
            byte   staticBuffer[1];                 /* force heap usage */
        #else
            byte   staticBuffer[FILE_BUFFER_SIZE];  /* tmp chain buffer */
        #endif
            byte*  chainBuffer = staticBuffer;
            int    dynamicBuffer = 0;
            word32 bufferSz = sizeof(staticBuffer);
            long   consumed = info->consumed;
            word32 idx = 0;
            int    gotOne = 0;

            if ( (sz - consumed) > (int)bufferSz) {
                WOLFSSL_MSG("Growing Tmp Chain Buffer");
                bufferSz = (word32)(sz - consumed);
                           /* will shrink to actual size */
                chainBuffer = (byte*)XMALLOC(bufferSz, heap, DYNAMIC_TYPE_FILE);
                if (chainBuffer == NULL) {
                #ifdef WOLFSSL_SMALL_STACK
                    XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
                #endif
                    FreeDer(&der);
                    return MEMORY_E;
                }
                dynamicBuffer = 1;
            }

            WOLFSSL_MSG("Processing Cert Chain");
            while (consumed < sz) {
                DerBuffer* part = NULL;
                info->consumed = 0;

                ret = PemToDer(buff + consumed, sz - consumed, type, &part,
                               heap, info, &eccKey);
                if (ret == 0) {
                    gotOne = 1;
                    if ( (idx + part->length) > bufferSz) {
                        WOLFSSL_MSG("   Cert Chain bigger than buffer");
                        ret = BUFFER_E;
                    }
                    else {
                        c32to24(part->length, &chainBuffer[idx]);
                        idx += CERT_HEADER_SZ;
                        XMEMCPY(&chainBuffer[idx], part->buffer, part->length);
                        idx += part->length;
                        consumed  += info->consumed;
                        if (used)
                            *used += info->consumed;
                    }
                }
                FreeDer(&part);

                if (ret == SSL_NO_PEM_HEADER && gotOne) {
                    WOLFSSL_MSG("We got one good PEM so stuff at end ok");
                    break;
                }

                if (ret < 0) {
                    WOLFSSL_MSG("   Error in Cert in Chain");
                    if (dynamicBuffer)
                        XFREE(chainBuffer, heap, DYNAMIC_TYPE_FILE);
                #ifdef WOLFSSL_SMALL_STACK
                    XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
                #endif
                    FreeDer(&der);
                    return ret;
                }
                WOLFSSL_MSG("   Consumed another Cert in Chain");
            }
            WOLFSSL_MSG("Finished Processing Cert Chain");

            /* only retain actual size used */
            ret = 0;
            if (idx > 0) {
                if (ssl) {
                    if (ssl->buffers.weOwnCertChain) {
                        FreeDer(&ssl->buffers.certChain);
                    }
                    ret = AllocDer(&ssl->buffers.certChain, idx, type, heap);
                    if (ret == 0) {
                        XMEMCPY(ssl->buffers.certChain->buffer, chainBuffer, idx);
                        ssl->buffers.weOwnCertChain = 1;
                    }
                } else if (ctx) {
                    FreeDer(&ctx->certChain);
                    ret = AllocDer(&ctx->certChain, idx, type, heap);
                    if (ret == 0) {
                        XMEMCPY(ctx->certChain->buffer, chainBuffer, idx);
                    }
                }
            }

            if (dynamicBuffer)
                XFREE(chainBuffer, heap, DYNAMIC_TYPE_FILE);

            if (ret < 0) {
            #ifdef WOLFSSL_SMALL_STACK
                XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
            #endif
                FreeDer(&der);
                return ret;
            }
        }
    }
    else {  /* ASN1 (DER) or RAW (NTRU) */
        ret = AllocDer(&der, (word32)sz, type, heap);
        if (ret < 0) {
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            return ret;
        }

        XMEMCPY(der->buffer, buff, sz);
    }

#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)
    /* for SSL_FILETYPE_PEM, PemToDer manage the decryption if required */
    if (info->set && (format != SSL_FILETYPE_PEM)) {
        /* decrypt */
        int   passwordSz;
#ifdef WOLFSSL_SMALL_STACK
        char* password = NULL;
#else
        char  password[80];
#endif

    #ifdef WOLFSSL_SMALL_STACK
        password = (char*)XMALLOC(80, heap, DYNAMIC_TYPE_TMP_BUFFER);
        if (password == NULL)
            ret = MEMORY_E;
        else
    #endif
        if (!ctx || !ctx->passwd_cb) {
            ret = NO_PASSWORD;
        }
        else {
            passwordSz = ctx->passwd_cb(password, sizeof(password),
                                        0, ctx->userdata);

            /* decrypt the key */
            ret = wolfssl_decrypt_buffer_key(der, (byte*)password,
                                             passwordSz, info);
        }

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(password, heap, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        if (ret != SSL_SUCCESS) {
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            FreeDer(&der);
            return ret;
        }
    }
#endif /* OPENSSL_EXTRA || HAVE_WEBSERVER */

#ifdef WOLFSSL_SMALL_STACK
    XFREE(info, heap, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    /* Handle DER owner */
    if (type == CA_TYPE) {
        if (ctx == NULL) {
            WOLFSSL_MSG("Need context for CA load");
            FreeDer(&der);
            return BAD_FUNC_ARG;
        }
        /* verify CA unless user set to no verify */
        return AddCA(ctx->cm, &der, WOLFSSL_USER_CA, !ctx->verifyNone);
    }
#ifdef WOLFSSL_TRUST_PEER_CERT
    else if (type == TRUSTED_PEER_TYPE) {
        if (ctx == NULL) {
            WOLFSSL_MSG("Need context for trusted peer cert load");
            FreeDer(&der);
            return BAD_FUNC_ARG;
        }
        /* add trusted peer cert */
        return AddTrustedPeer(ctx->cm, &der, !ctx->verifyNone);
    }
#endif /* WOLFSSL_TRUST_PEER_CERT */
    else if (type == CERT_TYPE) {
        if (ssl) {
             /* Make sure previous is free'd */
            if (ssl->buffers.weOwnCert) {
                FreeDer(&ssl->buffers.certificate);
                #ifdef KEEP_OUR_CERT
                    FreeX509(ssl->ourCert);
                    if (ssl->ourCert) {
                        XFREE(ssl->ourCert, ssl->heap, DYNAMIC_TYPE_X509);
                        ssl->ourCert = NULL;
                    }
                #endif
            }
            ssl->buffers.certificate = der;
            #ifdef KEEP_OUR_CERT
                ssl->keepCert = 1; /* hold cert for ssl lifetime */
            #endif
            ssl->buffers.weOwnCert = 1;
        }
        else if (ctx) {
            FreeDer(&ctx->certificate); /* Make sure previous is free'd */
            #ifdef KEEP_OUR_CERT
                FreeX509(ctx->ourCert);
                if (ctx->ourCert) {
                    XFREE(ctx->ourCert, ctx->heap, DYNAMIC_TYPE_X509);
                    ctx->ourCert = NULL;
                }
            #endif
            ctx->certificate = der;
        }
    }
    else if (type == PRIVATEKEY_TYPE) {
        if (ssl) {
             /* Make sure previous is free'd */
            if (ssl->buffers.weOwnKey) {
                FreeDer(&ssl->buffers.key);
            }
            ssl->buffers.key = der;
            ssl->buffers.weOwnKey = 1;
        }
        else if (ctx) {
            FreeDer(&ctx->privateKey);
            ctx->privateKey = der;
        }
    }
    else {
        FreeDer(&der);
        return SSL_BAD_CERTTYPE;
    }

    if (type == PRIVATEKEY_TYPE && format != SSL_FILETYPE_RAW) {
    #ifndef NO_RSA
        if (!eccKey) {
            /* make sure RSA key can be used */
            word32 idx = 0;
        #ifdef WOLFSSL_SMALL_STACK
            RsaKey* key = NULL;
        #else
            RsaKey  key[1];
        #endif

        #ifdef WOLFSSL_SMALL_STACK
            key = (RsaKey*)XMALLOC(sizeof(RsaKey), heap,
                                   DYNAMIC_TYPE_TMP_BUFFER);
            if (key == NULL)
                return MEMORY_E;
        #endif

            ret = wc_InitRsaKey(key, 0);
            if (ret == 0) {
                if (wc_RsaPrivateKeyDecode(der->buffer, &idx, key, der->length)
                    != 0) {
                #ifdef HAVE_ECC
                    /* could have DER ECC (or pkcs8 ecc), no easy way to tell */
                    eccKey = 1;  /* so try it out */
                #endif
                    if (!eccKey)
                        ret = SSL_BAD_FILE;
                } else {
                    /* check that the size of the RSA key is enough */
                    int RsaSz = wc_RsaEncryptSize((RsaKey*)key);
                    if (ssl) {
                        if (RsaSz < ssl->options.minRsaKeySz) {
                            ret = RSA_KEY_SIZE_E;
                            WOLFSSL_MSG("Private Key size too small");
                        }
                    }
                    else if(ctx) {
                        if (RsaSz < ctx->minRsaKeySz) {
                            ret = RSA_KEY_SIZE_E;
                            WOLFSSL_MSG("Private Key size too small");
                        }
                    }
                    rsaKey = 1;
                    (void)rsaKey;  /* for no ecc builds */
                }
            }

            wc_FreeRsaKey(key);

        #ifdef WOLFSSL_SMALL_STACK
            XFREE(key, heap, DYNAMIC_TYPE_TMP_BUFFER);
        #endif

            if (ret != 0)
                return ret;
        }
    #endif
    #ifdef HAVE_ECC
        if (!rsaKey) {
            /* make sure ECC key can be used */
            word32  idx = 0;
            ecc_key key;

            wc_ecc_init(&key);
            if (wc_EccPrivateKeyDecode(der->buffer, &idx, &key,
                                                        der->length) != 0) {
                wc_ecc_free(&key);
                return SSL_BAD_FILE;
            }

            /* check for minimum ECC key size and then free */
            if (ssl) {
                if (wc_ecc_size(&key) < ssl->options.minEccKeySz) {
                    wc_ecc_free(&key);
                    WOLFSSL_MSG("ECC private key too small");
                    return ECC_KEY_SIZE_E;
                }
            }
            else if (ctx) {
                if (wc_ecc_size(&key) < ctx->minEccKeySz) {
                    wc_ecc_free(&key);
                    WOLFSSL_MSG("ECC private key too small");
                    return ECC_KEY_SIZE_E;
                }
            }

            wc_ecc_free(&key);
            eccKey = 1;
            if (ctx)
                ctx->haveStaticECC = 1;
            if (ssl)
                ssl->options.haveStaticECC = 1;
        }
    #endif /* HAVE_ECC */
    }
    else if (type == CERT_TYPE) {
    #ifdef WOLFSSL_SMALL_STACK
        DecodedCert* cert = NULL;
    #else
        DecodedCert  cert[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), heap,
                                     DYNAMIC_TYPE_TMP_BUFFER);
        if (cert == NULL)
            return MEMORY_E;
    #endif

        WOLFSSL_MSG("Checking cert signature type");
        InitDecodedCert(cert, der->buffer, der->length, heap);

        if (DecodeToKey(cert, 0) < 0) {
            WOLFSSL_MSG("Decode to key failed");
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(cert, heap, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            return SSL_BAD_FILE;
        }
        switch (cert->signatureOID) {
            case CTC_SHAwECDSA:
            case CTC_SHA256wECDSA:
            case CTC_SHA384wECDSA:
            case CTC_SHA512wECDSA:
                WOLFSSL_MSG("ECDSA cert signature");
                if (ctx)
                    ctx->haveECDSAsig = 1;
                if (ssl)
                    ssl->options.haveECDSAsig = 1;
                break;
            default:
                WOLFSSL_MSG("Not ECDSA cert signature");
                break;
        }

    #ifdef HAVE_ECC
        if (ctx) {
            ctx->pkCurveOID = cert->pkCurveOID;
        #ifndef WC_STRICT_SIG
            if (cert->keyOID == ECDSAk) {
                ctx->haveECC = 1;
            }
        #else
            ctx->haveECC = ctx->haveECDSAsig;
        #endif
        }
        if (ssl) {
            ssl->pkCurveOID = cert->pkCurveOID;
        #ifndef WC_STRICT_SIG
            if (cert->keyOID == ECDSAk) {
                ssl->options.haveECC = 1;
            }
        #else
            ssl->options.haveECC = ssl->options.haveECDSAsig;
        #endif
        }
    #endif

        /* check key size of cert unless specified not to */
        switch (cert->keyOID) {
        #ifndef NO_RSA
            case RSAk:
                if (ssl && !ssl->options.verifyNone) {
                    if (ssl->options.minRsaKeySz < 0 ||
                          cert->pubKeySize < (word16)ssl->options.minRsaKeySz) {
                        ret = RSA_KEY_SIZE_E;
                        WOLFSSL_MSG("Certificate RSA key size too small");
                    }
                }
                else if (ctx && !ctx->verifyNone) {
                    if (ctx->minRsaKeySz < 0 ||
                                  cert->pubKeySize < (word16)ctx->minRsaKeySz) {
                        ret = RSA_KEY_SIZE_E;
                        WOLFSSL_MSG("Certificate RSA key size too small");
                    }
                }
                break;
            #endif /* !NO_RSA */
        #ifdef HAVE_ECC
            case ECDSAk:
                if (ssl && !ssl->options.verifyNone) {
                    if (ssl->options.minEccKeySz < 0 ||
                          cert->pubKeySize < (word16)ssl->options.minEccKeySz) {
                        ret = ECC_KEY_SIZE_E;
                        WOLFSSL_MSG("Certificate ECC key size error");
                    }
                }
                else if (ctx && !ctx->verifyNone) {
                    if (ctx->minEccKeySz < 0 ||
                                  cert->pubKeySize < (word16)ctx->minEccKeySz) {
                        ret = ECC_KEY_SIZE_E;
                        WOLFSSL_MSG("Certificate ECC key size error");
                    }
                }
                break;
            #endif /* HAVE_ECC */

            default:
                WOLFSSL_MSG("No key size check done on certificate");
                break; /* do no check if not a case for the key */
        }

        FreeDecodedCert(cert);
    #ifdef WOLFSSL_SMALL_STACK
        XFREE(cert, heap, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        if (ret != 0) {
            return ret;
        }
    }

    return SSL_SUCCESS;
}


/* CA PEM file for verification, may have multiple/chain certs to process */
static int ProcessChainBuffer(WOLFSSL_CTX* ctx, const unsigned char* buff,
                            long sz, int format, int type, WOLFSSL* ssl)
{
    long used   = 0;
    int  ret    = 0;
    int  gotOne = 0;

    WOLFSSL_MSG("Processing CA PEM file");
    while (used < sz) {
        long consumed = 0;

        ret = ProcessBuffer(ctx, buff + used, sz - used, format, type, ssl,
                            &consumed, 0);

        if (ret < 0)
        {
            if(consumed > 0) { /* Made progress in file */
                WOLFSSL_ERROR(ret);
                WOLFSSL_MSG("CA Parse failed, with progress in file.");
                WOLFSSL_MSG("Search for other certs in file");
            } else {
                WOLFSSL_MSG("CA Parse failed, no progress in file.");
                WOLFSSL_MSG("Do not continue search for other certs in file");
                break;
            }
        } else {
            WOLFSSL_MSG("   Processed a CA");
            gotOne = 1;
        }
        used += consumed;
    }

    if(gotOne)
    {
        WOLFSSL_MSG("Processed at least one valid CA. Other stuff OK");
        return SSL_SUCCESS;
    }
    return ret;
}


static INLINE WOLFSSL_METHOD* cm_pick_method(void)
{
    #ifndef NO_WOLFSSL_CLIENT
        #if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
            return wolfSSLv3_client_method();
        #else
            return wolfTLSv1_2_client_method();
        #endif
    #elif !defined(NO_WOLFSSL_SERVER)
        #if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
            return wolfSSLv3_server_method();
        #else
            return wolfTLSv1_2_server_method();
        #endif
    #else
        return NULL;
    #endif
}


/* like load verify locations, 1 for success, < 0 for error */
int wolfSSL_CertManagerLoadCABuffer(WOLFSSL_CERT_MANAGER* cm,
                                   const unsigned char* in, long sz, int format)
{
    int ret = SSL_FATAL_ERROR;
    WOLFSSL_CTX* tmp;

    WOLFSSL_ENTER("wolfSSL_CertManagerLoadCABuffer");

    if (cm == NULL) {
        WOLFSSL_MSG("No CertManager error");
        return ret;
    }
    tmp = wolfSSL_CTX_new(cm_pick_method());

    if (tmp == NULL) {
        WOLFSSL_MSG("CTX new failed");
        return ret;
    }

    /* for tmp use */
    wolfSSL_CertManagerFree(tmp->cm);
    tmp->cm = cm;

    ret = wolfSSL_CTX_load_verify_buffer(tmp, in, sz, format);

    /* don't loose our good one */
    tmp->cm = NULL;
    wolfSSL_CTX_free(tmp);

    return ret;
}

#ifdef HAVE_CRL

int wolfSSL_CertManagerLoadCRLBuffer(WOLFSSL_CERT_MANAGER* cm,
                                   const unsigned char* buff, long sz, int type)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerLoadCRLBuffer");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    if (cm->crl == NULL) {
        if (wolfSSL_CertManagerEnableCRL(cm, 0) != SSL_SUCCESS) {
            WOLFSSL_MSG("Enable CRL failed");
            return SSL_FATAL_ERROR;
        }
    }

    return BufferLoadCRL(cm->crl, buff, sz, type);
}


int wolfSSL_CTX_LoadCRLBuffer(WOLFSSL_CTX* ctx, const unsigned char* buff,
                              long sz, int type)
{
    WOLFSSL_ENTER("wolfSSL_CTX_LoadCRLBuffer");

    if (ctx == NULL)
        return BAD_FUNC_ARG;

    return wolfSSL_CertManagerLoadCRLBuffer(ctx->cm, buff, sz, type);
}


int wolfSSL_LoadCRLBuffer(WOLFSSL* ssl, const unsigned char* buff,
                          long sz, int type)
{
    WOLFSSL_ENTER("wolfSSL_LoadCRLBuffer");

    if (ssl == NULL || ssl->ctx == NULL)
        return BAD_FUNC_ARG;

    return wolfSSL_CertManagerLoadCRLBuffer(ssl->ctx->cm, buff, sz, type);
}


#endif /* HAVE_CRL */

/* turn on CRL if off and compiled in, set options */
int wolfSSL_CertManagerEnableCRL(WOLFSSL_CERT_MANAGER* cm, int options)
{
    int ret = SSL_SUCCESS;

    (void)options;

    WOLFSSL_ENTER("wolfSSL_CertManagerEnableCRL");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    #ifdef HAVE_CRL
        if (cm->crl == NULL) {
            cm->crl = (WOLFSSL_CRL*)XMALLOC(sizeof(WOLFSSL_CRL), cm->heap,
                                           DYNAMIC_TYPE_CRL);
            if (cm->crl == NULL)
                return MEMORY_E;

            if (InitCRL(cm->crl, cm) != 0) {
                WOLFSSL_MSG("Init CRL failed");
                FreeCRL(cm->crl, 1);
                cm->crl = NULL;
                return SSL_FAILURE;
            }
        }
        cm->crlEnabled = 1;
        if (options & WOLFSSL_CRL_CHECKALL)
            cm->crlCheckAll = 1;
    #else
        ret = NOT_COMPILED_IN;
    #endif

    return ret;
}


int wolfSSL_CertManagerDisableCRL(WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerDisableCRL");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    cm->crlEnabled = 0;

    return SSL_SUCCESS;
}
/* Verify the certificate, SSL_SUCCESS for ok, < 0 for error */
int wolfSSL_CertManagerVerifyBuffer(WOLFSSL_CERT_MANAGER* cm, const byte* buff,
                                    long sz, int format)
{
    int ret = 0;
    DerBuffer* der = NULL;
#ifdef WOLFSSL_SMALL_STACK
    DecodedCert* cert = NULL;
#else
    DecodedCert  cert[1];
#endif

    WOLFSSL_ENTER("wolfSSL_CertManagerVerifyBuffer");

#ifdef WOLFSSL_SMALL_STACK
    cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), cm->heap,
                                 DYNAMIC_TYPE_TMP_BUFFER);
    if (cert == NULL)
        return MEMORY_E;
#endif

    if (format == SSL_FILETYPE_PEM) {
        int eccKey = 0; /* not used */
    #ifdef WOLFSSL_SMALL_STACK
        EncryptedInfo* info = NULL;
    #else
        EncryptedInfo  info[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), cm->heap,
                                       DYNAMIC_TYPE_TMP_BUFFER);
        if (info == NULL) {
            XFREE(cert, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
            return MEMORY_E;
        }
    #endif

        info->set      = 0;
        info->ctx      = NULL;
        info->consumed = 0;

        ret = PemToDer(buff, sz, CERT_TYPE, &der, cm->heap, info, &eccKey);
        if (ret != 0) {
            FreeDer(&der);
            #ifdef WOLFSSL_SMALL_STACK
                XFREE(info, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
            #endif
            return ret;
        }
        InitDecodedCert(cert, der->buffer, der->length, cm->heap);

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(info, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
    }
    else
        InitDecodedCert(cert, (byte*)buff, (word32)sz, cm->heap);

    if (ret == 0)
        ret = ParseCertRelative(cert, CERT_TYPE, 1, cm);

#ifdef HAVE_CRL
    if (ret == 0 && cm->crlEnabled)
        ret = CheckCertCRL(cm->crl, cert);
#endif

    FreeDecodedCert(cert);
    FreeDer(&der);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(cert, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret == 0 ? SSL_SUCCESS : ret;
}


/* turn on OCSP if off and compiled in, set options */
int wolfSSL_CertManagerEnableOCSP(WOLFSSL_CERT_MANAGER* cm, int options)
{
    int ret = SSL_SUCCESS;

    (void)options;

    WOLFSSL_ENTER("wolfSSL_CertManagerEnableOCSP");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    #ifdef HAVE_OCSP
        if (cm->ocsp == NULL) {
            cm->ocsp = (WOLFSSL_OCSP*)XMALLOC(sizeof(WOLFSSL_OCSP), cm->heap,
                                              DYNAMIC_TYPE_OCSP);
            if (cm->ocsp == NULL)
                return MEMORY_E;

            if (InitOCSP(cm->ocsp, cm) != 0) {
                WOLFSSL_MSG("Init OCSP failed");
                FreeOCSP(cm->ocsp, 1);
                cm->ocsp = NULL;
                return SSL_FAILURE;
            }
        }
        cm->ocspEnabled = 1;
        if (options & WOLFSSL_OCSP_URL_OVERRIDE)
            cm->ocspUseOverrideURL = 1;
        if (options & WOLFSSL_OCSP_NO_NONCE)
            cm->ocspSendNonce = 0;
        else
            cm->ocspSendNonce = 1;
        if (options & WOLFSSL_OCSP_CHECKALL)
            cm->ocspCheckAll = 1;
        #ifndef WOLFSSL_USER_IO
            cm->ocspIOCb = EmbedOcspLookup;
            cm->ocspRespFreeCb = EmbedOcspRespFree;
            cm->ocspIOCtx = cm->heap;
        #endif /* WOLFSSL_USER_IO */
    #else
        ret = NOT_COMPILED_IN;
    #endif

    return ret;
}


int wolfSSL_CertManagerDisableOCSP(WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerDisableOCSP");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    cm->ocspEnabled = 0;

    return SSL_SUCCESS;
}

/* turn on OCSP Stapling if off and compiled in, set options */
int wolfSSL_CertManagerEnableOCSPStapling(WOLFSSL_CERT_MANAGER* cm)
{
    int ret = SSL_SUCCESS;

    WOLFSSL_ENTER("wolfSSL_CertManagerEnableOCSPStapling");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    #if defined(HAVE_CERTIFICATE_STATUS_REQUEST) \
     || defined(HAVE_CERTIFICATE_STATUS_REQUEST_V2)
        if (cm->ocsp_stapling == NULL) {
            cm->ocsp_stapling = (WOLFSSL_OCSP*)XMALLOC(sizeof(WOLFSSL_OCSP),
                                                   cm->heap, DYNAMIC_TYPE_OCSP);
            if (cm->ocsp_stapling == NULL)
                return MEMORY_E;

            if (InitOCSP(cm->ocsp_stapling, cm) != 0) {
                WOLFSSL_MSG("Init OCSP failed");
                FreeOCSP(cm->ocsp_stapling, 1);
                cm->ocsp_stapling = NULL;
                return SSL_FAILURE;
            }
        }
        cm->ocspStaplingEnabled = 1;

        #ifndef WOLFSSL_USER_IO
            cm->ocspIOCb = EmbedOcspLookup;
            cm->ocspRespFreeCb = EmbedOcspRespFree;
            cm->ocspIOCtx = cm->heap;
        #endif /* WOLFSSL_USER_IO */
    #else
        ret = NOT_COMPILED_IN;
    #endif

    return ret;
}


#ifdef HAVE_OCSP


/* check CRL if enabled, SSL_SUCCESS  */
int wolfSSL_CertManagerCheckOCSP(WOLFSSL_CERT_MANAGER* cm, byte* der, int sz)
{
    int ret;
#ifdef WOLFSSL_SMALL_STACK
    DecodedCert* cert = NULL;
#else
    DecodedCert  cert[1];
#endif

    WOLFSSL_ENTER("wolfSSL_CertManagerCheckOCSP");

    if (cm == NULL)
        return BAD_FUNC_ARG;

    if (cm->ocspEnabled == 0)
        return SSL_SUCCESS;

#ifdef WOLFSSL_SMALL_STACK
    cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), NULL,
                                 DYNAMIC_TYPE_TMP_BUFFER);
    if (cert == NULL)
        return MEMORY_E;
#endif

    InitDecodedCert(cert, der, sz, NULL);

    if ((ret = ParseCertRelative(cert, CERT_TYPE, NO_VERIFY, cm)) != 0) {
        WOLFSSL_MSG("ParseCert failed");
    }
    else if ((ret = CheckCertOCSP(cm->ocsp, cert, NULL)) != 0) {
        WOLFSSL_MSG("CheckCertOCSP failed");
    }

    FreeDecodedCert(cert);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret == 0 ? SSL_SUCCESS : ret;
}


int wolfSSL_CertManagerSetOCSPOverrideURL(WOLFSSL_CERT_MANAGER* cm,
                                          const char* url)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerSetOCSPOverrideURL");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    XFREE(cm->ocspOverrideURL, cm->heap, DYNAMIC_TYPE_URL);
    if (url != NULL) {
        int urlSz = (int)XSTRLEN(url) + 1;
        cm->ocspOverrideURL = (char*)XMALLOC(urlSz, cm->heap, DYNAMIC_TYPE_URL);
        if (cm->ocspOverrideURL != NULL) {
            XMEMCPY(cm->ocspOverrideURL, url, urlSz);
        }
        else
            return MEMORY_E;
    }
    else
        cm->ocspOverrideURL = NULL;

    return SSL_SUCCESS;
}


int wolfSSL_CertManagerSetOCSP_Cb(WOLFSSL_CERT_MANAGER* cm,
                        CbOCSPIO ioCb, CbOCSPRespFree respFreeCb, void* ioCbCtx)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerSetOCSP_Cb");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    cm->ocspIOCb = ioCb;
    cm->ocspRespFreeCb = respFreeCb;
    cm->ocspIOCtx = ioCbCtx;

    return SSL_SUCCESS;
}


int wolfSSL_EnableOCSP(WOLFSSL* ssl, int options)
{
    WOLFSSL_ENTER("wolfSSL_EnableOCSP");
    if (ssl)
        return wolfSSL_CertManagerEnableOCSP(ssl->ctx->cm, options);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_DisableOCSP(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_DisableOCSP");
    if (ssl)
        return wolfSSL_CertManagerDisableOCSP(ssl->ctx->cm);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_SetOCSP_OverrideURL(WOLFSSL* ssl, const char* url)
{
    WOLFSSL_ENTER("wolfSSL_SetOCSP_OverrideURL");
    if (ssl)
        return wolfSSL_CertManagerSetOCSPOverrideURL(ssl->ctx->cm, url);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_SetOCSP_Cb(WOLFSSL* ssl,
                        CbOCSPIO ioCb, CbOCSPRespFree respFreeCb, void* ioCbCtx)
{
    WOLFSSL_ENTER("wolfSSL_SetOCSP_Cb");
    if (ssl)
        return wolfSSL_CertManagerSetOCSP_Cb(ssl->ctx->cm,
                                             ioCb, respFreeCb, ioCbCtx);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_EnableOCSP(WOLFSSL_CTX* ctx, int options)
{
    WOLFSSL_ENTER("wolfSSL_CTX_EnableOCSP");
    if (ctx)
        return wolfSSL_CertManagerEnableOCSP(ctx->cm, options);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_DisableOCSP(WOLFSSL_CTX* ctx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_DisableOCSP");
    if (ctx)
        return wolfSSL_CertManagerDisableOCSP(ctx->cm);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_SetOCSP_OverrideURL(WOLFSSL_CTX* ctx, const char* url)
{
    WOLFSSL_ENTER("wolfSSL_SetOCSP_OverrideURL");
    if (ctx)
        return wolfSSL_CertManagerSetOCSPOverrideURL(ctx->cm, url);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_SetOCSP_Cb(WOLFSSL_CTX* ctx, CbOCSPIO ioCb,
                           CbOCSPRespFree respFreeCb, void* ioCbCtx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_SetOCSP_Cb");
    if (ctx)
        return wolfSSL_CertManagerSetOCSP_Cb(ctx->cm, ioCb,
                                             respFreeCb, ioCbCtx);
    else
        return BAD_FUNC_ARG;
}

#if defined(HAVE_CERTIFICATE_STATUS_REQUEST) \
 || defined(HAVE_CERTIFICATE_STATUS_REQUEST_V2)
int wolfSSL_CTX_EnableOCSPStapling(WOLFSSL_CTX* ctx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_EnableOCSPStapling");
    if (ctx)
        return wolfSSL_CertManagerEnableOCSPStapling(ctx->cm);
    else
        return BAD_FUNC_ARG;
}
#endif

#endif /* HAVE_OCSP */


#ifndef NO_FILESYSTEM

/* process a file with name fname into ctx of format and type
   userChain specifies a user certificate chain to pass during handshake */
int ProcessFile(WOLFSSL_CTX* ctx, const char* fname, int format, int type,
                WOLFSSL* ssl, int userChain, WOLFSSL_CRL* crl)
{
#ifdef WOLFSSL_SMALL_STACK
    byte   staticBuffer[1]; /* force heap usage */
#else
    byte   staticBuffer[FILE_BUFFER_SIZE];
#endif
    byte*  myBuffer = staticBuffer;
    int    dynamic = 0;
    int    ret;
    long   sz = 0;
    XFILE  file;
    void*  heapHint = ctx ? ctx->heap : ((ssl) ? ssl->heap : NULL);

    (void)crl;
    (void)heapHint;

    if (fname == NULL) return SSL_BAD_FILE;

    file = XFOPEN(fname, "rb");
    if (file == XBADFILE) return SSL_BAD_FILE;
    XFSEEK(file, 0, XSEEK_END);
    sz = XFTELL(file);
    XREWIND(file);

    if (sz > (long)sizeof(staticBuffer)) {
        WOLFSSL_MSG("Getting dynamic buffer");
        myBuffer = (byte*)XMALLOC(sz, heapHint, DYNAMIC_TYPE_FILE);
        if (myBuffer == NULL) {
            XFCLOSE(file);
            return SSL_BAD_FILE;
        }
        dynamic = 1;
    }
    else if (sz < 0) {
        XFCLOSE(file);
        return SSL_BAD_FILE;
    }

    if ( (ret = (int)XFREAD(myBuffer, sz, 1, file)) < 0)
        ret = SSL_BAD_FILE;
    else {
        if ((type == CA_TYPE || type == TRUSTED_PEER_TYPE)
                                                  && format == SSL_FILETYPE_PEM)
            ret = ProcessChainBuffer(ctx, myBuffer, sz, format, type, ssl);
#ifdef HAVE_CRL
        else if (type == CRL_TYPE)
            ret = BufferLoadCRL(crl, myBuffer, sz, format);
#endif
        else
            ret = ProcessBuffer(ctx, myBuffer, sz, format, type, ssl, NULL,
                                userChain);
    }

    XFCLOSE(file);
    if (dynamic)
        XFREE(myBuffer, heapHint, DYNAMIC_TYPE_FILE);

    return ret;
}


/* loads file then loads each file in path, no c_rehash */
int wolfSSL_CTX_load_verify_locations(WOLFSSL_CTX* ctx, const char* file,
                                     const char* path)
{
    int ret = SSL_SUCCESS;

    WOLFSSL_ENTER("wolfSSL_CTX_load_verify_locations");
    (void)path;

    if (ctx == NULL || (file == NULL && path == NULL) )
        return SSL_FAILURE;

    if (file)
        ret = ProcessFile(ctx, file, SSL_FILETYPE_PEM, CA_TYPE, NULL, 0, NULL);

    if (ret == SSL_SUCCESS && path) {
        /* try to load each regular file in path */
    #ifdef USE_WINDOWS_API
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind;
    #ifdef WOLFSSL_SMALL_STACK
        char*  name = NULL;
    #else
        char   name[MAX_FILENAME_SZ];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        name = (char*)XMALLOC(MAX_FILENAME_SZ, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (name == NULL)
            return MEMORY_E;
    #endif

        XMEMSET(name, 0, MAX_FILENAME_SZ);
        XSTRNCPY(name, path, MAX_FILENAME_SZ - 4);
        XSTRNCAT(name, "\\*", 3);

        hFind = FindFirstFileA(name, &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            WOLFSSL_MSG("FindFirstFile for path verify locations failed");
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(name, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            return BAD_PATH_ERROR;
        }

        do {
            if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
                XSTRNCPY(name, path, MAX_FILENAME_SZ/2 - 3);
                XSTRNCAT(name, "\\", 2);
                XSTRNCAT(name, FindFileData.cFileName, MAX_FILENAME_SZ/2);

                ret = ProcessFile(ctx, name, SSL_FILETYPE_PEM, CA_TYPE,
                                  NULL, 0, NULL);
            }
        } while (ret == SSL_SUCCESS && FindNextFileA(hFind, &FindFileData));

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(name, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        FindClose(hFind);
    #elif !defined(NO_WOLFSSL_DIR)
        struct dirent* entry;
        DIR*   dir = opendir(path);
    #ifdef WOLFSSL_SMALL_STACK
        char*  name = NULL;
    #else
        char   name[MAX_FILENAME_SZ];
    #endif

        if (dir == NULL) {
            WOLFSSL_MSG("opendir path verify locations failed");
            return BAD_PATH_ERROR;
        }

    #ifdef WOLFSSL_SMALL_STACK
        name = (char*)XMALLOC(MAX_FILENAME_SZ, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (name == NULL) {
            closedir(dir);
            return MEMORY_E;
        }
    #endif

        while ( ret == SSL_SUCCESS && (entry = readdir(dir)) != NULL) {
            struct stat s;

            XMEMSET(name, 0, MAX_FILENAME_SZ);
            XSTRNCPY(name, path, MAX_FILENAME_SZ/2 - 2);
            XSTRNCAT(name, "/", 1);
            XSTRNCAT(name, entry->d_name, MAX_FILENAME_SZ/2);

            if (stat(name, &s) != 0) {
                WOLFSSL_MSG("stat on name failed");
                ret = BAD_PATH_ERROR;
            } else if (s.st_mode & S_IFREG)
                ret = ProcessFile(ctx, name, SSL_FILETYPE_PEM, CA_TYPE,
                                  NULL, 0, NULL);
        }

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(name, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        closedir(dir);
    #endif
    }

    return ret;
}


#ifdef WOLFSSL_TRUST_PEER_CERT
/* Used to specify a peer cert to match when connecting
    ctx : the ctx structure to load in peer cert
    file: the string name of cert file
    type: type of format such as PEM/DER
 */
int wolfSSL_CTX_trust_peer_cert(WOLFSSL_CTX* ctx, const char* file, int type)
{
    WOLFSSL_ENTER("wolfSSL_CTX_trust_peer_cert");

    if (ctx == NULL || file == NULL) {
        return SSL_FAILURE;
    }

    return ProcessFile(ctx, file, type, TRUSTED_PEER_TYPE, NULL, 0, NULL);
}
#endif /* WOLFSSL_TRUST_PEER_CERT */


/* Verify the certificate, SSL_SUCCESS for ok, < 0 for error */
int wolfSSL_CertManagerVerify(WOLFSSL_CERT_MANAGER* cm, const char* fname,
                             int format)
{
    int    ret = SSL_FATAL_ERROR;
#ifdef WOLFSSL_SMALL_STACK
    byte   staticBuffer[1]; /* force heap usage */
#else
    byte   staticBuffer[FILE_BUFFER_SIZE];
#endif
    byte*  myBuffer = staticBuffer;
    int    dynamic = 0;
    long   sz = 0;
    XFILE  file = XFOPEN(fname, "rb");

    WOLFSSL_ENTER("wolfSSL_CertManagerVerify");

    if (file == XBADFILE) return SSL_BAD_FILE;
    XFSEEK(file, 0, XSEEK_END);
    sz = XFTELL(file);
    XREWIND(file);

    if (sz > MAX_WOLFSSL_FILE_SIZE || sz < 0) {
        WOLFSSL_MSG("CertManagerVerify file bad size");
        XFCLOSE(file);
        return SSL_BAD_FILE;
    }

    if (sz > (long)sizeof(staticBuffer)) {
        WOLFSSL_MSG("Getting dynamic buffer");
        myBuffer = (byte*) XMALLOC(sz, cm->heap, DYNAMIC_TYPE_FILE);
        if (myBuffer == NULL) {
            XFCLOSE(file);
            return SSL_BAD_FILE;
        }
        dynamic = 1;
    }

    if ( (ret = (int)XFREAD(myBuffer, sz, 1, file)) < 0)
        ret = SSL_BAD_FILE;
    else
        ret = wolfSSL_CertManagerVerifyBuffer(cm, myBuffer, sz, format);

    XFCLOSE(file);
    if (dynamic)
        XFREE(myBuffer, cm->heap, DYNAMIC_TYPE_FILE);

    return ret;
}


/* like load verify locations, 1 for success, < 0 for error */
int wolfSSL_CertManagerLoadCA(WOLFSSL_CERT_MANAGER* cm, const char* file,
                             const char* path)
{
    int ret = SSL_FATAL_ERROR;
    WOLFSSL_CTX* tmp;

    WOLFSSL_ENTER("wolfSSL_CertManagerLoadCA");

    if (cm == NULL) {
        WOLFSSL_MSG("No CertManager error");
        return ret;
    }
    tmp = wolfSSL_CTX_new(cm_pick_method());

    if (tmp == NULL) {
        WOLFSSL_MSG("CTX new failed");
        return ret;
    }

    /* for tmp use */
    wolfSSL_CertManagerFree(tmp->cm);
    tmp->cm = cm;

    ret = wolfSSL_CTX_load_verify_locations(tmp, file, path);

    /* don't loose our good one */
    tmp->cm = NULL;
    wolfSSL_CTX_free(tmp);

    return ret;
}




int wolfSSL_CTX_check_private_key(WOLFSSL_CTX* ctx)
{
    /* TODO: check private against public for RSA match */
    (void)ctx;
    WOLFSSL_ENTER("SSL_CTX_check_private_key");
    return SSL_SUCCESS;
}


#ifdef HAVE_CRL


/* check CRL if enabled, SSL_SUCCESS  */
int wolfSSL_CertManagerCheckCRL(WOLFSSL_CERT_MANAGER* cm, byte* der, int sz)
{
    int ret = 0;
#ifdef WOLFSSL_SMALL_STACK
    DecodedCert* cert = NULL;
#else
    DecodedCert  cert[1];
#endif

    WOLFSSL_ENTER("wolfSSL_CertManagerCheckCRL");

    if (cm == NULL)
        return BAD_FUNC_ARG;

    if (cm->crlEnabled == 0)
        return SSL_SUCCESS;

#ifdef WOLFSSL_SMALL_STACK
    cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), NULL,
                                 DYNAMIC_TYPE_TMP_BUFFER);
    if (cert == NULL)
        return MEMORY_E;
#endif

    InitDecodedCert(cert, der, sz, NULL);

    if ((ret = ParseCertRelative(cert, CERT_TYPE, NO_VERIFY, cm)) != 0) {
        WOLFSSL_MSG("ParseCert failed");
    }
    else if ((ret = CheckCertCRL(cm->crl, cert)) != 0) {
        WOLFSSL_MSG("CheckCertCRL failed");
    }

    FreeDecodedCert(cert);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret == 0 ? SSL_SUCCESS : ret;
}


int wolfSSL_CertManagerSetCRL_Cb(WOLFSSL_CERT_MANAGER* cm, CbMissingCRL cb)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerSetCRL_Cb");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    cm->cbMissingCRL = cb;

    return SSL_SUCCESS;
}


int wolfSSL_CertManagerLoadCRL(WOLFSSL_CERT_MANAGER* cm, const char* path,
                              int type, int monitor)
{
    WOLFSSL_ENTER("wolfSSL_CertManagerLoadCRL");
    if (cm == NULL)
        return BAD_FUNC_ARG;

    if (cm->crl == NULL) {
        if (wolfSSL_CertManagerEnableCRL(cm, 0) != SSL_SUCCESS) {
            WOLFSSL_MSG("Enable CRL failed");
            return SSL_FATAL_ERROR;
        }
    }

    return LoadCRL(cm->crl, path, type, monitor);
}


int wolfSSL_EnableCRL(WOLFSSL* ssl, int options)
{
    WOLFSSL_ENTER("wolfSSL_EnableCRL");
    if (ssl)
        return wolfSSL_CertManagerEnableCRL(ssl->ctx->cm, options);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_DisableCRL(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_DisableCRL");
    if (ssl)
        return wolfSSL_CertManagerDisableCRL(ssl->ctx->cm);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_LoadCRL(WOLFSSL* ssl, const char* path, int type, int monitor)
{
    WOLFSSL_ENTER("wolfSSL_LoadCRL");
    if (ssl)
        return wolfSSL_CertManagerLoadCRL(ssl->ctx->cm, path, type, monitor);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_SetCRL_Cb(WOLFSSL* ssl, CbMissingCRL cb)
{
    WOLFSSL_ENTER("wolfSSL_SetCRL_Cb");
    if (ssl)
        return wolfSSL_CertManagerSetCRL_Cb(ssl->ctx->cm, cb);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_EnableCRL(WOLFSSL_CTX* ctx, int options)
{
    WOLFSSL_ENTER("wolfSSL_CTX_EnableCRL");
    if (ctx)
        return wolfSSL_CertManagerEnableCRL(ctx->cm, options);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_DisableCRL(WOLFSSL_CTX* ctx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_DisableCRL");
    if (ctx)
        return wolfSSL_CertManagerDisableCRL(ctx->cm);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_LoadCRL(WOLFSSL_CTX* ctx, const char* path,
                        int type, int monitor)
{
    WOLFSSL_ENTER("wolfSSL_CTX_LoadCRL");
    if (ctx)
        return wolfSSL_CertManagerLoadCRL(ctx->cm, path, type, monitor);
    else
        return BAD_FUNC_ARG;
}


int wolfSSL_CTX_SetCRL_Cb(WOLFSSL_CTX* ctx, CbMissingCRL cb)
{
    WOLFSSL_ENTER("wolfSSL_CTX_SetCRL_Cb");
    if (ctx)
        return wolfSSL_CertManagerSetCRL_Cb(ctx->cm, cb);
    else
        return BAD_FUNC_ARG;
}


#endif /* HAVE_CRL */


#ifdef WOLFSSL_DER_LOAD

/* Add format parameter to allow DER load of CA files */
int wolfSSL_CTX_der_load_verify_locations(WOLFSSL_CTX* ctx, const char* file,
                                          int format)
{
    WOLFSSL_ENTER("wolfSSL_CTX_der_load_verify_locations");
    if (ctx == NULL || file == NULL)
        return SSL_FAILURE;

    if (ProcessFile(ctx, file, format, CA_TYPE, NULL, 0, NULL) == SSL_SUCCESS)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}

#endif /* WOLFSSL_DER_LOAD */


#ifdef WOLFSSL_CERT_GEN

/* load pem cert from file into der buffer, return der size or error */
int wolfSSL_PemCertToDer(const char* fileName, unsigned char* derBuf, int derSz)
{
#ifdef WOLFSSL_SMALL_STACK
    EncryptedInfo* info = NULL;
    byte   staticBuffer[1]; /* force XMALLOC */
#else
    EncryptedInfo info[1];
    byte   staticBuffer[FILE_BUFFER_SIZE];
#endif
    byte*  fileBuf = staticBuffer;
    int    dynamic = 0;
    int    ret     = 0;
    int    ecc     = 0;
    long   sz      = 0;
    XFILE  file    = XFOPEN(fileName, "rb");
    DerBuffer* converted = NULL;

    WOLFSSL_ENTER("wolfSSL_PemCertToDer");

    if (file == XBADFILE) {
        ret = SSL_BAD_FILE;
    }
    else {
        XFSEEK(file, 0, XSEEK_END);
        sz = XFTELL(file);
        XREWIND(file);

        if (sz < 0) {
            ret = SSL_BAD_FILE;
        }
        else if (sz > (long)sizeof(staticBuffer)) {
        #ifdef WOLFSSL_STATIC_MEMORY
            WOLFSSL_MSG("File was larger then static buffer");
            return MEMORY_E;
        #endif
            fileBuf = (byte*)XMALLOC(sz, 0, DYNAMIC_TYPE_FILE);
            if (fileBuf == NULL)
                ret = MEMORY_E;
            else
                dynamic = 1;
        }

        if (ret == 0) {
            if ( (ret = (int)XFREAD(fileBuf, sz, 1, file)) < 0) {
                ret = SSL_BAD_FILE;
            }
            else {
            #ifdef WOLFSSL_SMALL_STACK
                info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), NULL,
                                               DYNAMIC_TYPE_TMP_BUFFER);
                if (info == NULL)
                    ret = MEMORY_E;
                else
            #endif
                {
                    ret = PemToDer(fileBuf, sz, CA_TYPE, &converted,
                                   0, info, &ecc);
                #ifdef WOLFSSL_SMALL_STACK
                    XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
                #endif
                }
            }

            if (ret == 0) {
                if (converted->length < (word32)derSz) {
                    XMEMCPY(derBuf, converted->buffer, converted->length);
                    ret = converted->length;
                }
                else
                    ret = BUFFER_E;
            }

            FreeDer(&converted);
        }

        XFCLOSE(file);
        if (dynamic)
            XFREE(fileBuf, 0, DYNAMIC_TYPE_FILE);
    }

    return ret;
}

#endif /* WOLFSSL_CERT_GEN */

#ifdef WOLFSSL_CERT_EXT
#ifndef NO_FILESYSTEM
/* load pem public key from file into der buffer, return der size or error */
int wolfSSL_PemPubKeyToDer(const char* fileName,
                           unsigned char* derBuf, int derSz)
{
#ifdef WOLFSSL_SMALL_STACK
    byte   staticBuffer[1]; /* force XMALLOC */
#else
    byte   staticBuffer[FILE_BUFFER_SIZE];
#endif
    byte*  fileBuf = staticBuffer;
    int    dynamic = 0;
    int    ret     = 0;
    long   sz      = 0;
    XFILE  file    = XFOPEN(fileName, "rb");
    DerBuffer* converted = NULL;

    WOLFSSL_ENTER("wolfSSL_PemPubKeyToDer");

    if (file == XBADFILE) {
        ret = SSL_BAD_FILE;
    }
    else {
        XFSEEK(file, 0, XSEEK_END);
        sz = XFTELL(file);
        XREWIND(file);

        if (sz < 0) {
            ret = SSL_BAD_FILE;
        }
        else if (sz > (long)sizeof(staticBuffer)) {
        #ifdef WOLFSSL_STATIC_MEMORY
            WOLFSSL_MSG("File was larger then static buffer");
            return MEMORY_E;
        #endif
            fileBuf = (byte*)XMALLOC(sz, 0, DYNAMIC_TYPE_FILE);
            if (fileBuf == NULL)
                ret = MEMORY_E;
            else
                dynamic = 1;
        }
        if (ret == 0) {
            if ( (ret = (int)XFREAD(fileBuf, sz, 1, file)) < 0)
                ret = SSL_BAD_FILE;
            else
                ret = PemToDer(fileBuf, sz, PUBLICKEY_TYPE, &converted,
                               0, NULL, NULL);

            if (ret == 0) {
                if (converted->length < (word32)derSz) {
                    XMEMCPY(derBuf, converted->buffer, converted->length);
                    ret = converted->length;
                }
                else
                    ret = BUFFER_E;
            }

            FreeDer(&converted);
        }

        XFCLOSE(file);
        if (dynamic)
            XFREE(fileBuf, 0, DYNAMIC_TYPE_FILE);
    }

    return ret;
}
#endif /* NO_FILESYSTEM */

/* Return bytes written to buff or < 0 for error */
int wolfSSL_PubKeyPemToDer(const unsigned char* pem, int pemSz,
                           unsigned char* buff, int buffSz)
{
    int ret;
    DerBuffer* der = NULL;

    WOLFSSL_ENTER("wolfSSL_PubKeyPemToDer");

    if (pem == NULL || buff == NULL || buffSz <= 0) {
        WOLFSSL_MSG("Bad pem der args");
        return BAD_FUNC_ARG;
    }

    ret = PemToDer(pem, pemSz, PUBLICKEY_TYPE, &der, NULL, NULL, NULL);
    if (ret < 0) {
        WOLFSSL_MSG("Bad Pem To Der");
    }
    else {
        if (der->length <= (word32)buffSz) {
            XMEMCPY(buff, der->buffer, der->length);
            ret = der->length;
        }
        else {
            WOLFSSL_MSG("Bad der length");
            ret = BAD_FUNC_ARG;
        }
    }

    FreeDer(&der);
    return ret;
}

#endif /* WOLFSSL_CERT_EXT */

int wolfSSL_CTX_use_certificate_file(WOLFSSL_CTX* ctx, const char* file,
                                     int format)
{
    WOLFSSL_ENTER("wolfSSL_CTX_use_certificate_file");
    if (ProcessFile(ctx, file, format, CERT_TYPE, NULL, 0, NULL) == SSL_SUCCESS)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}


int wolfSSL_CTX_use_PrivateKey_file(WOLFSSL_CTX* ctx, const char* file,
                                    int format)
{
    WOLFSSL_ENTER("wolfSSL_CTX_use_PrivateKey_file");
    if (ProcessFile(ctx, file, format, PRIVATEKEY_TYPE, NULL, 0, NULL)
                    == SSL_SUCCESS)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}


/* get cert chaining depth using ssl struct */
long wolfSSL_get_verify_depth(WOLFSSL* ssl)
{
    if(ssl == NULL) {
        return BAD_FUNC_ARG;
    }
    return MAX_CHAIN_DEPTH;
}


/* get cert chaining depth using ctx struct */
long wolfSSL_CTX_get_verify_depth(WOLFSSL_CTX* ctx)
{
    if(ctx == NULL) {
        return BAD_FUNC_ARG;
    }
    return MAX_CHAIN_DEPTH;
}


int wolfSSL_CTX_use_certificate_chain_file(WOLFSSL_CTX* ctx, const char* file)
{
   /* process up to MAX_CHAIN_DEPTH plus subject cert */
   WOLFSSL_ENTER("wolfSSL_CTX_use_certificate_chain_file");
   if (ProcessFile(ctx, file, SSL_FILETYPE_PEM,CERT_TYPE,NULL,1, NULL)
                   == SSL_SUCCESS)
       return SSL_SUCCESS;

   return SSL_FAILURE;
}


#ifndef NO_DH

/* server Diffie-Hellman parameters */
static int wolfSSL_SetTmpDH_file_wrapper(WOLFSSL_CTX* ctx, WOLFSSL* ssl,
                                        const char* fname, int format)
{
#ifdef WOLFSSL_SMALL_STACK
    byte   staticBuffer[1]; /* force heap usage */
#else
    byte   staticBuffer[FILE_BUFFER_SIZE];
#endif
    byte*  myBuffer = staticBuffer;
    int    dynamic = 0;
    int    ret;
    long   sz = 0;
    XFILE  file;

    if (ctx == NULL || fname == NULL)
        return BAD_FUNC_ARG;

    file = XFOPEN(fname, "rb");
    if (file == XBADFILE) return SSL_BAD_FILE;
    XFSEEK(file, 0, XSEEK_END);
    sz = XFTELL(file);
    XREWIND(file);

    if (sz > (long)sizeof(staticBuffer)) {
        WOLFSSL_MSG("Getting dynamic buffer");
        myBuffer = (byte*) XMALLOC(sz, ctx->heap, DYNAMIC_TYPE_FILE);
        if (myBuffer == NULL) {
            XFCLOSE(file);
            return SSL_BAD_FILE;
        }
        dynamic = 1;
    }
    else if (sz < 0) {
        XFCLOSE(file);
        return SSL_BAD_FILE;
    }

    if ( (ret = (int)XFREAD(myBuffer, sz, 1, file)) < 0)
        ret = SSL_BAD_FILE;
    else {
        if (ssl)
            ret = wolfSSL_SetTmpDH_buffer(ssl, myBuffer, sz, format);
        else
            ret = wolfSSL_CTX_SetTmpDH_buffer(ctx, myBuffer, sz, format);
    }

    XFCLOSE(file);
    if (dynamic)
        XFREE(myBuffer, ctx->heap, DYNAMIC_TYPE_FILE);

    return ret;
}

/* server Diffie-Hellman parameters */
int wolfSSL_SetTmpDH_file(WOLFSSL* ssl, const char* fname, int format)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    return wolfSSL_SetTmpDH_file_wrapper(ssl->ctx, ssl, fname, format);
}


/* server Diffie-Hellman parameters */
int wolfSSL_CTX_SetTmpDH_file(WOLFSSL_CTX* ctx, const char* fname, int format)
{
    return wolfSSL_SetTmpDH_file_wrapper(ctx, NULL, fname, format);
}

#endif /* NO_DH */


#ifdef OPENSSL_EXTRA
/* put SSL type in extra for now, not very common */

int wolfSSL_use_certificate_file(WOLFSSL* ssl, const char* file, int format)
{
    WOLFSSL_ENTER("wolfSSL_use_certificate_file");
    if (ProcessFile(ssl->ctx, file, format, CERT_TYPE,
                    ssl, 0, NULL) == SSL_SUCCESS)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}


int wolfSSL_use_PrivateKey_file(WOLFSSL* ssl, const char* file, int format)
{
    WOLFSSL_ENTER("wolfSSL_use_PrivateKey_file");
    if (ProcessFile(ssl->ctx, file, format, PRIVATEKEY_TYPE,
                    ssl, 0, NULL) == SSL_SUCCESS)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}


int wolfSSL_use_certificate_chain_file(WOLFSSL* ssl, const char* file)
{
   /* process up to MAX_CHAIN_DEPTH plus subject cert */
   WOLFSSL_ENTER("wolfSSL_use_certificate_chain_file");
   if (ProcessFile(ssl->ctx, file, SSL_FILETYPE_PEM, CERT_TYPE,
                   ssl, 1, NULL) == SSL_SUCCESS)
       return SSL_SUCCESS;

   return SSL_FAILURE;
}



#ifdef HAVE_ECC

/* Set Temp CTX EC-DHE size in octets, should be 20 - 66 for 160 - 521 bit */
int wolfSSL_CTX_SetTmpEC_DHE_Sz(WOLFSSL_CTX* ctx, word16 sz)
{
    if (ctx == NULL || sz < ECC_MINSIZE || sz > ECC_MAXSIZE)
        return BAD_FUNC_ARG;

    ctx->eccTempKeySz = sz;

    return SSL_SUCCESS;
}


/* Set Temp SSL EC-DHE size in octets, should be 20 - 66 for 160 - 521 bit */
int wolfSSL_SetTmpEC_DHE_Sz(WOLFSSL* ssl, word16 sz)
{
    if (ssl == NULL || sz < ECC_MINSIZE || sz > ECC_MAXSIZE)
        return BAD_FUNC_ARG;

    ssl->eccTempKeySz = sz;

    return SSL_SUCCESS;
}

#endif /* HAVE_ECC */




int wolfSSL_CTX_use_RSAPrivateKey_file(WOLFSSL_CTX* ctx,const char* file,
                                   int format)
{
    WOLFSSL_ENTER("SSL_CTX_use_RSAPrivateKey_file");

    return wolfSSL_CTX_use_PrivateKey_file(ctx, file, format);
}


int wolfSSL_use_RSAPrivateKey_file(WOLFSSL* ssl, const char* file, int format)
{
    WOLFSSL_ENTER("wolfSSL_use_RSAPrivateKey_file");

    return wolfSSL_use_PrivateKey_file(ssl, file, format);
}

#endif /* OPENSSL_EXTRA */

#ifdef HAVE_NTRU

int wolfSSL_CTX_use_NTRUPrivateKey_file(WOLFSSL_CTX* ctx, const char* file)
{
    WOLFSSL_ENTER("wolfSSL_CTX_use_NTRUPrivateKey_file");
    if (ctx == NULL)
        return SSL_FAILURE;

    if (ProcessFile(ctx, file, SSL_FILETYPE_RAW, PRIVATEKEY_TYPE, NULL, 0, NULL)
                         == SSL_SUCCESS) {
        ctx->haveNTRU = 1;
        return SSL_SUCCESS;
    }

    return SSL_FAILURE;
}

#endif /* HAVE_NTRU */


#endif /* NO_FILESYSTEM */


void wolfSSL_CTX_set_verify(WOLFSSL_CTX* ctx, int mode, VerifyCallback vc)
{
    WOLFSSL_ENTER("wolfSSL_CTX_set_verify");
    if (mode & SSL_VERIFY_PEER) {
        ctx->verifyPeer = 1;
        ctx->verifyNone = 0;  /* in case previously set */
    }

    if (mode == SSL_VERIFY_NONE) {
        ctx->verifyNone = 1;
        ctx->verifyPeer = 0;  /* in case previously set */
    }

    if (mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT)
        ctx->failNoCert = 1;

    if (mode & SSL_VERIFY_FAIL_EXCEPT_PSK) {
        ctx->failNoCert    = 0; /* fail on all is set to fail on PSK */
        ctx->failNoCertxPSK = 1;
    }

    ctx->verifyCallback = vc;
}


void wolfSSL_set_verify(WOLFSSL* ssl, int mode, VerifyCallback vc)
{
    WOLFSSL_ENTER("wolfSSL_set_verify");
    if (mode & SSL_VERIFY_PEER) {
        ssl->options.verifyPeer = 1;
        ssl->options.verifyNone = 0;  /* in case previously set */
    }

    if (mode == SSL_VERIFY_NONE) {
        ssl->options.verifyNone = 1;
        ssl->options.verifyPeer = 0;  /* in case previously set */
    }

    if (mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT)
        ssl->options.failNoCert = 1;

    if (mode & SSL_VERIFY_FAIL_EXCEPT_PSK) {
        ssl->options.failNoCert    = 0; /* fail on all is set to fail on PSK */
        ssl->options.failNoCertxPSK = 1;
    }

    ssl->verifyCallback = vc;
}


/* store user ctx for verify callback */
void wolfSSL_SetCertCbCtx(WOLFSSL* ssl, void* ctx)
{
    WOLFSSL_ENTER("wolfSSL_SetCertCbCtx");
    if (ssl)
        ssl->verifyCbCtx = ctx;
}


/* store context CA Cache addition callback */
void wolfSSL_CTX_SetCACb(WOLFSSL_CTX* ctx, CallbackCACache cb)
{
    if (ctx && ctx->cm)
        ctx->cm->caCacheCallback = cb;
}


#if defined(PERSIST_CERT_CACHE)

#if !defined(NO_FILESYSTEM)

/* Persist cert cache to file */
int wolfSSL_CTX_save_cert_cache(WOLFSSL_CTX* ctx, const char* fname)
{
    WOLFSSL_ENTER("wolfSSL_CTX_save_cert_cache");

    if (ctx == NULL || fname == NULL)
        return BAD_FUNC_ARG;

    return CM_SaveCertCache(ctx->cm, fname);
}


/* Persist cert cache from file */
int wolfSSL_CTX_restore_cert_cache(WOLFSSL_CTX* ctx, const char* fname)
{
    WOLFSSL_ENTER("wolfSSL_CTX_restore_cert_cache");

    if (ctx == NULL || fname == NULL)
        return BAD_FUNC_ARG;

    return CM_RestoreCertCache(ctx->cm, fname);
}

#endif /* NO_FILESYSTEM */

/* Persist cert cache to memory */
int wolfSSL_CTX_memsave_cert_cache(WOLFSSL_CTX* ctx, void* mem,
                                   int sz, int* used)
{
    WOLFSSL_ENTER("wolfSSL_CTX_memsave_cert_cache");

    if (ctx == NULL || mem == NULL || used == NULL || sz <= 0)
        return BAD_FUNC_ARG;

    return CM_MemSaveCertCache(ctx->cm, mem, sz, used);
}


/* Restore cert cache from memory */
int wolfSSL_CTX_memrestore_cert_cache(WOLFSSL_CTX* ctx, const void* mem, int sz)
{
    WOLFSSL_ENTER("wolfSSL_CTX_memrestore_cert_cache");

    if (ctx == NULL || mem == NULL || sz <= 0)
        return BAD_FUNC_ARG;

    return CM_MemRestoreCertCache(ctx->cm, mem, sz);
}


/* get how big the the cert cache save buffer needs to be */
int wolfSSL_CTX_get_cert_cache_memsize(WOLFSSL_CTX* ctx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_get_cert_cache_memsize");

    if (ctx == NULL)
        return BAD_FUNC_ARG;

    return CM_GetCertCacheMemSize(ctx->cm);
}

#endif /* PERSISTE_CERT_CACHE */
#endif /* !NO_CERTS */


#ifndef NO_SESSION_CACHE

WOLFSSL_SESSION* wolfSSL_get_session(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_get_session");
    if (ssl)
        return GetSession(ssl, 0, 0);

    return NULL;
}


int wolfSSL_set_session(WOLFSSL* ssl, WOLFSSL_SESSION* session)
{
    WOLFSSL_ENTER("SSL_set_session");
    if (session)
        return SetSession(ssl, session);

    return SSL_FAILURE;
}


#ifndef NO_CLIENT_CACHE

/* Associate client session with serverID, find existing or store for saving
   if newSession flag on, don't reuse existing session
   SSL_SUCCESS on ok */
int wolfSSL_SetServerID(WOLFSSL* ssl, const byte* id, int len, int newSession)
{
    WOLFSSL_SESSION* session = NULL;

    WOLFSSL_ENTER("wolfSSL_SetServerID");

    if (ssl == NULL || id == NULL || len <= 0)
        return BAD_FUNC_ARG;

    if (newSession == 0) {
        session = GetSessionClient(ssl, id, len);
        if (session) {
            if (SetSession(ssl, session) != SSL_SUCCESS) {
                WOLFSSL_MSG("SetSession failed");
                session = NULL;
            }
        }
    }

    if (session == NULL) {
        WOLFSSL_MSG("Valid ServerID not cached already");

        ssl->session.idLen = (word16)min(SERVER_ID_LEN, (word32)len);
        XMEMCPY(ssl->session.serverID, id, ssl->session.idLen);
    }

    return SSL_SUCCESS;
}

#endif /* NO_CLIENT_CACHE */

#if defined(PERSIST_SESSION_CACHE)

/* for persistence, if changes to layout need to increment and modify
   save_session_cache() and restore_session_cache and memory versions too */
#define WOLFSSL_CACHE_VERSION 2

/* Session Cache Header information */
typedef struct {
    int version;     /* cache layout version id */
    int rows;        /* session rows */
    int columns;     /* session columns */
    int sessionSz;   /* sizeof WOLFSSL_SESSION */
} cache_header_t;

/* current persistence layout is:

   1) cache_header_t
   2) SessionCache
   3) ClientCache

   update WOLFSSL_CACHE_VERSION if change layout for the following
   PERSISTENT_SESSION_CACHE functions
*/


/* get how big the the session cache save buffer needs to be */
int wolfSSL_get_session_cache_memsize(void)
{
    int sz  = (int)(sizeof(SessionCache) + sizeof(cache_header_t));

    #ifndef NO_CLIENT_CACHE
        sz += (int)(sizeof(ClientCache));
    #endif

    return sz;
}


/* Persist session cache to memory */
int wolfSSL_memsave_session_cache(void* mem, int sz)
{
    int i;
    cache_header_t cache_header;
    SessionRow*    row  = (SessionRow*)((byte*)mem + sizeof(cache_header));
#ifndef NO_CLIENT_CACHE
    ClientRow*     clRow;
#endif

    WOLFSSL_ENTER("wolfSSL_memsave_session_cache");

    if (sz < wolfSSL_get_session_cache_memsize()) {
        WOLFSSL_MSG("Memory buffer too small");
        return BUFFER_E;
    }

    cache_header.version   = WOLFSSL_CACHE_VERSION;
    cache_header.rows      = SESSION_ROWS;
    cache_header.columns   = SESSIONS_PER_ROW;
    cache_header.sessionSz = (int)sizeof(WOLFSSL_SESSION);
    XMEMCPY(mem, &cache_header, sizeof(cache_header));

    if (LockMutex(&session_mutex) != 0) {
        WOLFSSL_MSG("Session cache mutex lock failed");
        return BAD_MUTEX_E;
    }

    for (i = 0; i < cache_header.rows; ++i)
        XMEMCPY(row++, SessionCache + i, sizeof(SessionRow));

#ifndef NO_CLIENT_CACHE
    clRow = (ClientRow*)row;
    for (i = 0; i < cache_header.rows; ++i)
        XMEMCPY(clRow++, ClientCache + i, sizeof(ClientRow));
#endif

    UnLockMutex(&session_mutex);

    WOLFSSL_LEAVE("wolfSSL_memsave_session_cache", SSL_SUCCESS);

    return SSL_SUCCESS;
}


/* Restore the persistent session cache from memory */
int wolfSSL_memrestore_session_cache(const void* mem, int sz)
{
    int    i;
    cache_header_t cache_header;
    SessionRow*    row  = (SessionRow*)((byte*)mem + sizeof(cache_header));
#ifndef NO_CLIENT_CACHE
    ClientRow*     clRow;
#endif

    WOLFSSL_ENTER("wolfSSL_memrestore_session_cache");

    if (sz < wolfSSL_get_session_cache_memsize()) {
        WOLFSSL_MSG("Memory buffer too small");
        return BUFFER_E;
    }

    XMEMCPY(&cache_header, mem, sizeof(cache_header));
    if (cache_header.version   != WOLFSSL_CACHE_VERSION ||
        cache_header.rows      != SESSION_ROWS ||
        cache_header.columns   != SESSIONS_PER_ROW ||
        cache_header.sessionSz != (int)sizeof(WOLFSSL_SESSION)) {

        WOLFSSL_MSG("Session cache header match failed");
        return CACHE_MATCH_ERROR;
    }

    if (LockMutex(&session_mutex) != 0) {
        WOLFSSL_MSG("Session cache mutex lock failed");
        return BAD_MUTEX_E;
    }

    for (i = 0; i < cache_header.rows; ++i)
        XMEMCPY(SessionCache + i, row++, sizeof(SessionRow));

#ifndef NO_CLIENT_CACHE
    clRow = (ClientRow*)row;
    for (i = 0; i < cache_header.rows; ++i)
        XMEMCPY(ClientCache + i, clRow++, sizeof(ClientRow));
#endif

    UnLockMutex(&session_mutex);

    WOLFSSL_LEAVE("wolfSSL_memrestore_session_cache", SSL_SUCCESS);

    return SSL_SUCCESS;
}

#if !defined(NO_FILESYSTEM)

/* Persist session cache to file */
/* doesn't use memsave because of additional memory use */
int wolfSSL_save_session_cache(const char *fname)
{
    XFILE  file;
    int    ret;
    int    rc = SSL_SUCCESS;
    int    i;
    cache_header_t cache_header;

    WOLFSSL_ENTER("wolfSSL_save_session_cache");

    file = XFOPEN(fname, "w+b");
    if (file == XBADFILE) {
        WOLFSSL_MSG("Couldn't open session cache save file");
        return SSL_BAD_FILE;
    }
    cache_header.version   = WOLFSSL_CACHE_VERSION;
    cache_header.rows      = SESSION_ROWS;
    cache_header.columns   = SESSIONS_PER_ROW;
    cache_header.sessionSz = (int)sizeof(WOLFSSL_SESSION);

    /* cache header */
    ret = (int)XFWRITE(&cache_header, sizeof cache_header, 1, file);
    if (ret != 1) {
        WOLFSSL_MSG("Session cache header file write failed");
        XFCLOSE(file);
        return FWRITE_ERROR;
    }

    if (LockMutex(&session_mutex) != 0) {
        WOLFSSL_MSG("Session cache mutex lock failed");
        XFCLOSE(file);
        return BAD_MUTEX_E;
    }

    /* session cache */
    for (i = 0; i < cache_header.rows; ++i) {
        ret = (int)XFWRITE(SessionCache + i, sizeof(SessionRow), 1, file);
        if (ret != 1) {
            WOLFSSL_MSG("Session cache member file write failed");
            rc = FWRITE_ERROR;
            break;
        }
    }

#ifndef NO_CLIENT_CACHE
    /* client cache */
    for (i = 0; i < cache_header.rows; ++i) {
        ret = (int)XFWRITE(ClientCache + i, sizeof(ClientRow), 1, file);
        if (ret != 1) {
            WOLFSSL_MSG("Client cache member file write failed");
            rc = FWRITE_ERROR;
            break;
        }
    }
#endif /* NO_CLIENT_CACHE */

    UnLockMutex(&session_mutex);

    XFCLOSE(file);
    WOLFSSL_LEAVE("wolfSSL_save_session_cache", rc);

    return rc;
}


/* Restore the persistent session cache from file */
/* doesn't use memstore because of additional memory use */
int wolfSSL_restore_session_cache(const char *fname)
{
    XFILE  file;
    int    rc = SSL_SUCCESS;
    int    ret;
    int    i;
    cache_header_t cache_header;

    WOLFSSL_ENTER("wolfSSL_restore_session_cache");

    file = XFOPEN(fname, "rb");
    if (file == XBADFILE) {
        WOLFSSL_MSG("Couldn't open session cache save file");
        return SSL_BAD_FILE;
    }
    /* cache header */
    ret = (int)XFREAD(&cache_header, sizeof cache_header, 1, file);
    if (ret != 1) {
        WOLFSSL_MSG("Session cache header file read failed");
        XFCLOSE(file);
        return FREAD_ERROR;
    }
    if (cache_header.version   != WOLFSSL_CACHE_VERSION ||
        cache_header.rows      != SESSION_ROWS ||
        cache_header.columns   != SESSIONS_PER_ROW ||
        cache_header.sessionSz != (int)sizeof(WOLFSSL_SESSION)) {

        WOLFSSL_MSG("Session cache header match failed");
        XFCLOSE(file);
        return CACHE_MATCH_ERROR;
    }

    if (LockMutex(&session_mutex) != 0) {
        WOLFSSL_MSG("Session cache mutex lock failed");
        XFCLOSE(file);
        return BAD_MUTEX_E;
    }

    /* session cache */
    for (i = 0; i < cache_header.rows; ++i) {
        ret = (int)XFREAD(SessionCache + i, sizeof(SessionRow), 1, file);
        if (ret != 1) {
            WOLFSSL_MSG("Session cache member file read failed");
            XMEMSET(SessionCache, 0, sizeof SessionCache);
            rc = FREAD_ERROR;
            break;
        }
    }

#ifndef NO_CLIENT_CACHE
    /* client cache */
    for (i = 0; i < cache_header.rows; ++i) {
        ret = (int)XFREAD(ClientCache + i, sizeof(ClientRow), 1, file);
        if (ret != 1) {
            WOLFSSL_MSG("Client cache member file read failed");
            XMEMSET(ClientCache, 0, sizeof ClientCache);
            rc = FREAD_ERROR;
            break;
        }
    }

#endif /* NO_CLIENT_CACHE */

    UnLockMutex(&session_mutex);

    XFCLOSE(file);
    WOLFSSL_LEAVE("wolfSSL_restore_session_cache", rc);

    return rc;
}

#endif /* !NO_FILESYSTEM */
#endif /* PERSIST_SESSION_CACHE */
#endif /* NO_SESSION_CACHE */


void wolfSSL_load_error_strings(void)   /* compatibility only */
{}


int wolfSSL_library_init(void)
{
    WOLFSSL_ENTER("SSL_library_init");
    if (wolfSSL_Init() == SSL_SUCCESS)
        return SSL_SUCCESS;
    else
        return SSL_FATAL_ERROR;
}


#ifdef HAVE_SECRET_CALLBACK

int wolfSSL_set_session_secret_cb(WOLFSSL* ssl, SessionSecretCb cb, void* ctx)
{
    WOLFSSL_ENTER("wolfSSL_set_session_secret_cb");
    if (ssl == NULL)
        return SSL_FATAL_ERROR;

    ssl->sessionSecretCb = cb;
    ssl->sessionSecretCtx = ctx;
    /* If using a pre-set key, assume session resumption. */
    ssl->session.sessionIDSz = 0;
    ssl->options.resuming = 1;

    return SSL_SUCCESS;
}

#endif


#ifndef NO_SESSION_CACHE

/* on by default if built in but allow user to turn off */
long wolfSSL_CTX_set_session_cache_mode(WOLFSSL_CTX* ctx, long mode)
{
    WOLFSSL_ENTER("SSL_CTX_set_session_cache_mode");
    if (mode == SSL_SESS_CACHE_OFF)
        ctx->sessionCacheOff = 1;

    if (mode == SSL_SESS_CACHE_NO_AUTO_CLEAR)
        ctx->sessionCacheFlushOff = 1;

    return SSL_SUCCESS;
}

#endif /* NO_SESSION_CACHE */


#if !defined(NO_CERTS)
#if defined(PERSIST_CERT_CACHE)


#define WOLFSSL_CACHE_CERT_VERSION 1

typedef struct {
    int version;                 /* cache cert layout version id */
    int rows;                    /* hash table rows, CA_TABLE_SIZE */
    int columns[CA_TABLE_SIZE];  /* columns per row on list */
    int signerSz;                /* sizeof Signer object */
} CertCacheHeader;

/* current cert persistence layout is:

   1) CertCacheHeader
   2) caTable

   update WOLFSSL_CERT_CACHE_VERSION if change layout for the following
   PERSIST_CERT_CACHE functions
*/


/* Return memory needed to persist this signer, have lock */
static INLINE int GetSignerMemory(Signer* signer)
{
    int sz = sizeof(signer->pubKeySize) + sizeof(signer->keyOID)
           + sizeof(signer->nameLen)    + sizeof(signer->subjectNameHash);

#if !defined(NO_SKID)
        sz += (int)sizeof(signer->subjectKeyIdHash);
#endif

    /* add dynamic bytes needed */
    sz += signer->pubKeySize;
    sz += signer->nameLen;

    return sz;
}


/* Return memory needed to persist this row, have lock */
static INLINE int GetCertCacheRowMemory(Signer* row)
{
    int sz = 0;

    while (row) {
        sz += GetSignerMemory(row);
        row = row->next;
    }

    return sz;
}


/* get the size of persist cert cache, have lock */
static INLINE int GetCertCacheMemSize(WOLFSSL_CERT_MANAGER* cm)
{
    int sz;
    int i;

    sz = sizeof(CertCacheHeader);

    for (i = 0; i < CA_TABLE_SIZE; i++)
        sz += GetCertCacheRowMemory(cm->caTable[i]);

    return sz;
}


/* Store cert cache header columns with number of items per list, have lock */
static INLINE void SetCertHeaderColumns(WOLFSSL_CERT_MANAGER* cm, int* columns)
{
    int     i;
    Signer* row;

    for (i = 0; i < CA_TABLE_SIZE; i++) {
        int count = 0;
        row = cm->caTable[i];

        while (row) {
            ++count;
            row = row->next;
        }
        columns[i] = count;
    }
}


/* Restore whole cert row from memory, have lock, return bytes consumed,
   < 0 on error, have lock */
static INLINE int RestoreCertRow(WOLFSSL_CERT_MANAGER* cm, byte* current,
                                 int row, int listSz, const byte* end)
{
    int idx = 0;

    if (listSz < 0) {
        WOLFSSL_MSG("Row header corrupted, negative value");
        return PARSE_ERROR;
    }

    while (listSz) {
        Signer* signer;
        byte*   start = current + idx;  /* for end checks on this signer */
        int     minSz = sizeof(signer->pubKeySize) + sizeof(signer->keyOID) +
                      sizeof(signer->nameLen) + sizeof(signer->subjectNameHash);
        #ifndef NO_SKID
                minSz += (int)sizeof(signer->subjectKeyIdHash);
        #endif

        if (start + minSz > end) {
            WOLFSSL_MSG("Would overread restore buffer");
            return BUFFER_E;
        }
        signer = MakeSigner(cm->heap);
        if (signer == NULL)
            return MEMORY_E;

        /* pubKeySize */
        XMEMCPY(&signer->pubKeySize, current + idx, sizeof(signer->pubKeySize));
        idx += (int)sizeof(signer->pubKeySize);

        /* keyOID */
        XMEMCPY(&signer->keyOID, current + idx, sizeof(signer->keyOID));
        idx += (int)sizeof(signer->keyOID);

        /* pulicKey */
        if (start + minSz + signer->pubKeySize > end) {
            WOLFSSL_MSG("Would overread restore buffer");
            FreeSigner(signer, cm->heap);
            return BUFFER_E;
        }
        signer->publicKey = (byte*)XMALLOC(signer->pubKeySize, cm->heap,
                                           DYNAMIC_TYPE_KEY);
        if (signer->publicKey == NULL) {
            FreeSigner(signer, cm->heap);
            return MEMORY_E;
        }

        XMEMCPY(signer->publicKey, current + idx, signer->pubKeySize);
        idx += signer->pubKeySize;

        /* nameLen */
        XMEMCPY(&signer->nameLen, current + idx, sizeof(signer->nameLen));
        idx += (int)sizeof(signer->nameLen);

        /* name */
        if (start + minSz + signer->pubKeySize + signer->nameLen > end) {
            WOLFSSL_MSG("Would overread restore buffer");
            FreeSigner(signer, cm->heap);
            return BUFFER_E;
        }
        signer->name = (char*)XMALLOC(signer->nameLen, cm->heap,
                                      DYNAMIC_TYPE_SUBJECT_CN);
        if (signer->name == NULL) {
            FreeSigner(signer, cm->heap);
            return MEMORY_E;
        }

        XMEMCPY(signer->name, current + idx, signer->nameLen);
        idx += signer->nameLen;

        /* subjectNameHash */
        XMEMCPY(signer->subjectNameHash, current + idx, SIGNER_DIGEST_SIZE);
        idx += SIGNER_DIGEST_SIZE;

        #ifndef NO_SKID
            /* subjectKeyIdHash */
            XMEMCPY(signer->subjectKeyIdHash, current + idx,SIGNER_DIGEST_SIZE);
            idx += SIGNER_DIGEST_SIZE;
        #endif

        signer->next = cm->caTable[row];
        cm->caTable[row] = signer;

        --listSz;
    }

    return idx;
}


/* Store whole cert row into memory, have lock, return bytes added */
static INLINE int StoreCertRow(WOLFSSL_CERT_MANAGER* cm, byte* current, int row)
{
    int     added  = 0;
    Signer* list   = cm->caTable[row];

    while (list) {
        XMEMCPY(current + added, &list->pubKeySize, sizeof(list->pubKeySize));
        added += (int)sizeof(list->pubKeySize);

        XMEMCPY(current + added, &list->keyOID,     sizeof(list->keyOID));
        added += (int)sizeof(list->keyOID);

        XMEMCPY(current + added, list->publicKey, list->pubKeySize);
        added += list->pubKeySize;

        XMEMCPY(current + added, &list->nameLen, sizeof(list->nameLen));
        added += (int)sizeof(list->nameLen);

        XMEMCPY(current + added, list->name, list->nameLen);
        added += list->nameLen;

        XMEMCPY(current + added, list->subjectNameHash, SIGNER_DIGEST_SIZE);
        added += SIGNER_DIGEST_SIZE;

        #ifndef NO_SKID
            XMEMCPY(current + added, list->subjectKeyIdHash,SIGNER_DIGEST_SIZE);
            added += SIGNER_DIGEST_SIZE;
        #endif

        list = list->next;
    }

    return added;
}


/* Persist cert cache to memory, have lock */
static INLINE int DoMemSaveCertCache(WOLFSSL_CERT_MANAGER* cm,
                                     void* mem, int sz)
{
    int realSz;
    int ret = SSL_SUCCESS;
    int i;

    WOLFSSL_ENTER("DoMemSaveCertCache");

    realSz = GetCertCacheMemSize(cm);
    if (realSz > sz) {
        WOLFSSL_MSG("Mem output buffer too small");
        ret = BUFFER_E;
    }
    else {
        byte*           current;
        CertCacheHeader hdr;

        hdr.version  = WOLFSSL_CACHE_CERT_VERSION;
        hdr.rows     = CA_TABLE_SIZE;
        SetCertHeaderColumns(cm, hdr.columns);
        hdr.signerSz = (int)sizeof(Signer);

        XMEMCPY(mem, &hdr, sizeof(CertCacheHeader));
        current = (byte*)mem + sizeof(CertCacheHeader);

        for (i = 0; i < CA_TABLE_SIZE; ++i)
            current += StoreCertRow(cm, current, i);
    }

    return ret;
}


#if !defined(NO_FILESYSTEM)

/* Persist cert cache to file */
int CM_SaveCertCache(WOLFSSL_CERT_MANAGER* cm, const char* fname)
{
    XFILE file;
    int   rc = SSL_SUCCESS;
    int   memSz;
    byte* mem;

    WOLFSSL_ENTER("CM_SaveCertCache");

    file = XFOPEN(fname, "w+b");
    if (file == XBADFILE) {
       WOLFSSL_MSG("Couldn't open cert cache save file");
       return SSL_BAD_FILE;
    }

    if (LockMutex(&cm->caLock) != 0) {
        WOLFSSL_MSG("LockMutex on caLock failed");
        XFCLOSE(file);
        return BAD_MUTEX_E;
    }

    memSz = GetCertCacheMemSize(cm);
    mem   = (byte*)XMALLOC(memSz, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
    if (mem == NULL) {
        WOLFSSL_MSG("Alloc for tmp buffer failed");
        rc = MEMORY_E;
    } else {
        rc = DoMemSaveCertCache(cm, mem, memSz);
        if (rc == SSL_SUCCESS) {
            int ret = (int)XFWRITE(mem, memSz, 1, file);
            if (ret != 1) {
                WOLFSSL_MSG("Cert cache file write failed");
                rc = FWRITE_ERROR;
            }
        }
        XFREE(mem, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
    }

    UnLockMutex(&cm->caLock);
    XFCLOSE(file);

    return rc;
}


/* Restore cert cache from file */
int CM_RestoreCertCache(WOLFSSL_CERT_MANAGER* cm, const char* fname)
{
    XFILE file;
    int   rc = SSL_SUCCESS;
    int   ret;
    int   memSz;
    byte* mem;

    WOLFSSL_ENTER("CM_RestoreCertCache");

    file = XFOPEN(fname, "rb");
    if (file == XBADFILE) {
       WOLFSSL_MSG("Couldn't open cert cache save file");
       return SSL_BAD_FILE;
    }

    XFSEEK(file, 0, XSEEK_END);
    memSz = (int)XFTELL(file);
    XREWIND(file);

    if (memSz <= 0) {
        WOLFSSL_MSG("Bad file size");
        XFCLOSE(file);
        return SSL_BAD_FILE;
    }

    mem = (byte*)XMALLOC(memSz, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
    if (mem == NULL) {
        WOLFSSL_MSG("Alloc for tmp buffer failed");
        XFCLOSE(file);
        return MEMORY_E;
    }

    ret = (int)XFREAD(mem, memSz, 1, file);
    if (ret != 1) {
        WOLFSSL_MSG("Cert file read error");
        rc = FREAD_ERROR;
    } else {
        rc = CM_MemRestoreCertCache(cm, mem, memSz);
        if (rc != SSL_SUCCESS) {
            WOLFSSL_MSG("Mem restore cert cache failed");
        }
    }

    XFREE(mem, cm->heap, DYNAMIC_TYPE_TMP_BUFFER);
    XFCLOSE(file);

    return rc;
}

#endif /* NO_FILESYSTEM */


/* Persist cert cache to memory */
int CM_MemSaveCertCache(WOLFSSL_CERT_MANAGER* cm, void* mem, int sz, int* used)
{
    int ret = SSL_SUCCESS;

    WOLFSSL_ENTER("CM_MemSaveCertCache");

    if (LockMutex(&cm->caLock) != 0) {
        WOLFSSL_MSG("LockMutex on caLock failed");
        return BAD_MUTEX_E;
    }

    ret = DoMemSaveCertCache(cm, mem, sz);
    if (ret == SSL_SUCCESS)
        *used  = GetCertCacheMemSize(cm);

    UnLockMutex(&cm->caLock);

    return ret;
}


/* Restore cert cache from memory */
int CM_MemRestoreCertCache(WOLFSSL_CERT_MANAGER* cm, const void* mem, int sz)
{
    int ret = SSL_SUCCESS;
    int i;
    CertCacheHeader* hdr = (CertCacheHeader*)mem;
    byte*            current = (byte*)mem + sizeof(CertCacheHeader);
    byte*            end     = (byte*)mem + sz;  /* don't go over */

    WOLFSSL_ENTER("CM_MemRestoreCertCache");

    if (current > end) {
        WOLFSSL_MSG("Cert Cache Memory buffer too small");
        return BUFFER_E;
    }

    if (hdr->version  != WOLFSSL_CACHE_CERT_VERSION ||
        hdr->rows     != CA_TABLE_SIZE ||
        hdr->signerSz != (int)sizeof(Signer)) {

        WOLFSSL_MSG("Cert Cache Memory header mismatch");
        return CACHE_MATCH_ERROR;
    }

    if (LockMutex(&cm->caLock) != 0) {
        WOLFSSL_MSG("LockMutex on caLock failed");
        return BAD_MUTEX_E;
    }

    FreeSignerTable(cm->caTable, CA_TABLE_SIZE, cm->heap);

    for (i = 0; i < CA_TABLE_SIZE; ++i) {
        int added = RestoreCertRow(cm, current, i, hdr->columns[i], end);
        if (added < 0) {
            WOLFSSL_MSG("RestoreCertRow error");
            ret = added;
            break;
        }
        current += added;
    }

    UnLockMutex(&cm->caLock);

    return ret;
}


/* get how big the the cert cache save buffer needs to be */
int CM_GetCertCacheMemSize(WOLFSSL_CERT_MANAGER* cm)
{
    int sz;

    WOLFSSL_ENTER("CM_GetCertCacheMemSize");

    if (LockMutex(&cm->caLock) != 0) {
        WOLFSSL_MSG("LockMutex on caLock failed");
        return BAD_MUTEX_E;
    }

    sz = GetCertCacheMemSize(cm);

    UnLockMutex(&cm->caLock);

    return sz;
}

#endif /* PERSIST_CERT_CACHE */
#endif /* NO_CERTS */


int wolfSSL_CTX_set_cipher_list(WOLFSSL_CTX* ctx, const char* list)
{
    WOLFSSL_ENTER("wolfSSL_CTX_set_cipher_list");

    /* alloc/init on demand only */
    if (ctx->suites == NULL) {
        ctx->suites = (Suites*)XMALLOC(sizeof(Suites), ctx->heap,
                                       DYNAMIC_TYPE_SUITES);
        if (ctx->suites == NULL) {
            WOLFSSL_MSG("Memory alloc for Suites failed");
            return SSL_FAILURE;
        }
        XMEMSET(ctx->suites, 0, sizeof(Suites));
    }

    return (SetCipherList(ctx->suites, list)) ? SSL_SUCCESS : SSL_FAILURE;
}


int wolfSSL_set_cipher_list(WOLFSSL* ssl, const char* list)
{
    WOLFSSL_ENTER("wolfSSL_set_cipher_list");
    return (SetCipherList(ssl->suites, list)) ? SSL_SUCCESS : SSL_FAILURE;
}


#ifndef WOLFSSL_LEANPSK
#ifdef WOLFSSL_DTLS

int wolfSSL_dtls_get_current_timeout(WOLFSSL* ssl)
{
    (void)ssl;

    return ssl->dtls_timeout;
}


/* user may need to alter init dtls recv timeout, SSL_SUCCESS on ok */
int wolfSSL_dtls_set_timeout_init(WOLFSSL* ssl, int timeout)
{
    if (ssl == NULL || timeout < 0)
        return BAD_FUNC_ARG;

    if (timeout > ssl->dtls_timeout_max) {
        WOLFSSL_MSG("Can't set dtls timeout init greater than dtls timeout max");
        return BAD_FUNC_ARG;
    }

    ssl->dtls_timeout_init = timeout;
    ssl->dtls_timeout = timeout;

    return SSL_SUCCESS;
}


/* user may need to alter max dtls recv timeout, SSL_SUCCESS on ok */
int wolfSSL_dtls_set_timeout_max(WOLFSSL* ssl, int timeout)
{
    if (ssl == NULL || timeout < 0)
        return BAD_FUNC_ARG;

    if (timeout < ssl->dtls_timeout_init) {
        WOLFSSL_MSG("Can't set dtls timeout max less than dtls timeout init");
        return BAD_FUNC_ARG;
    }

    ssl->dtls_timeout_max = timeout;

    return SSL_SUCCESS;
}


int wolfSSL_dtls_got_timeout(WOLFSSL* ssl)
{
    int result = SSL_SUCCESS;

    if (!ssl->options.handShakeDone &&
        (DtlsPoolTimeout(ssl) < 0 || DtlsPoolSend(ssl) < 0)) {

        result = SSL_FATAL_ERROR;
    }
    return result;
}

#endif /* DTLS */
#endif /* LEANPSK */


#if defined(WOLFSSL_DTLS) && !defined(NO_WOLFSSL_SERVER)

/* Not an SSL function, return 0 for success, error code otherwise */
/* Prereq: ssl's RNG needs to be initialized. */
int wolfSSL_DTLS_SetCookieSecret(WOLFSSL* ssl,
                                 const byte* secret, word32 secretSz)
{
    WOLFSSL_ENTER("wolfSSL_DTLS_SetCookieSecret");

    if (ssl == NULL) {
        WOLFSSL_MSG("need a SSL object");
        return BAD_FUNC_ARG;
    }

    if (secret != NULL && secretSz == 0) {
        WOLFSSL_MSG("can't have a new secret without a size");
        return BAD_FUNC_ARG;
    }

    /* If secretSz is 0, use the default size. */
    if (secretSz == 0)
        secretSz = COOKIE_SECRET_SZ;

    if (secretSz != ssl->buffers.dtlsCookieSecret.length) {
        byte* newSecret;

        if (ssl->buffers.dtlsCookieSecret.buffer != NULL) {
            ForceZero(ssl->buffers.dtlsCookieSecret.buffer,
                      ssl->buffers.dtlsCookieSecret.length);
            XFREE(ssl->buffers.dtlsCookieSecret.buffer,
                  ssl->heap, DYNAMIC_TYPE_NONE);
        }

        newSecret = (byte*)XMALLOC(secretSz, ssl->heap,DYNAMIC_TYPE_COOKIE_PWD);
        if (newSecret == NULL) {
            ssl->buffers.dtlsCookieSecret.buffer = NULL;
            ssl->buffers.dtlsCookieSecret.length = 0;
            WOLFSSL_MSG("couldn't allocate new cookie secret");
            return MEMORY_ERROR;
        }
        ssl->buffers.dtlsCookieSecret.buffer = newSecret;
        ssl->buffers.dtlsCookieSecret.length = secretSz;
    }

    /* If the supplied secret is NULL, randomly generate a new secret. */
    if (secret == NULL)
        wc_RNG_GenerateBlock(ssl->rng,
                             ssl->buffers.dtlsCookieSecret.buffer, secretSz);
    else
        XMEMCPY(ssl->buffers.dtlsCookieSecret.buffer, secret, secretSz);

    WOLFSSL_LEAVE("wolfSSL_DTLS_SetCookieSecret", 0);
    return 0;
}

#endif /* WOLFSSL_DTLS && !NO_WOLFSSL_SERVER */


/* client only parts */
#ifndef NO_WOLFSSL_CLIENT

    #if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
    WOLFSSL_METHOD* wolfSSLv3_client_method(void)
    {
        WOLFSSL_ENTER("SSLv3_client_method");
        return wolfSSLv3_client_method_ex(NULL);
    }
    #endif

    #ifdef WOLFSSL_DTLS

        #ifndef NO_OLD_TLS
        WOLFSSL_METHOD* wolfDTLSv1_client_method(void)
        {
            WOLFSSL_ENTER("DTLSv1_client_method");
            return wolfDTLSv1_client_method_ex(NULL);
        }
        #endif  /* NO_OLD_TLS */

        WOLFSSL_METHOD* wolfDTLSv1_2_client_method(void)
        {
            WOLFSSL_ENTER("DTLSv1_2_client_method");
            return wolfDTLSv1_2_client_method_ex(NULL);
        }
    #endif

    #if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
    WOLFSSL_METHOD* wolfSSLv3_client_method_ex(void* heap)
    {
        WOLFSSL_METHOD* method =
                              (WOLFSSL_METHOD*) XMALLOC(sizeof(WOLFSSL_METHOD),
                                                     heap, DYNAMIC_TYPE_METHOD);
        WOLFSSL_ENTER("SSLv3_client_method_ex");
        if (method)
            InitSSL_Method(method, MakeSSLv3());
        return method;
    }
    #endif

    #ifdef WOLFSSL_DTLS

        #ifndef NO_OLD_TLS
        WOLFSSL_METHOD* wolfDTLSv1_client_method_ex(void* heap)
        {
            WOLFSSL_METHOD* method =
                              (WOLFSSL_METHOD*) XMALLOC(sizeof(WOLFSSL_METHOD),
                                                     heap, DYNAMIC_TYPE_METHOD);
            WOLFSSL_ENTER("DTLSv1_client_method_ex");
            if (method)
                InitSSL_Method(method, MakeDTLSv1());
            return method;
        }
        #endif  /* NO_OLD_TLS */

        WOLFSSL_METHOD* wolfDTLSv1_2_client_method_ex(void* heap)
        {
            WOLFSSL_METHOD* method =
                              (WOLFSSL_METHOD*) XMALLOC(sizeof(WOLFSSL_METHOD),
                                                     heap, DYNAMIC_TYPE_METHOD);
            WOLFSSL_ENTER("DTLSv1_2_client_method_ex");
            if (method)
                InitSSL_Method(method, MakeDTLSv1_2());
            return method;
        }
    #endif

    /* please see note at top of README if you get an error from connect */
    int wolfSSL_connect(WOLFSSL* ssl)
    {
        int neededState;

        WOLFSSL_ENTER("SSL_connect()");

        #ifdef HAVE_ERRNO_H
            errno = 0;
        #endif

        if (ssl->options.side != WOLFSSL_CLIENT_END) {
            WOLFSSL_ERROR(ssl->error = SIDE_ERROR);
            return SSL_FATAL_ERROR;
        }

        #ifdef WOLFSSL_DTLS
            if (ssl->version.major == DTLS_MAJOR) {
                ssl->options.dtls   = 1;
                ssl->options.tls    = 1;
                ssl->options.tls1_1 = 1;

                if (DtlsPoolInit(ssl) != 0) {
                    ssl->error = MEMORY_ERROR;
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            }
        #endif

        if (ssl->buffers.outputBuffer.length > 0) {
            if ( (ssl->error = SendBuffered(ssl)) == 0) {
                /* fragOffset is non-zero when sending fragments. On the last
                 * fragment, fragOffset is zero again, and the state can be
                 * advanced. */
                if (ssl->fragOffset == 0) {
                    ssl->options.connectState++;
                    WOLFSSL_MSG("connect state: "
                                "Advanced from last buffered fragment send");
                }
                else {
                    WOLFSSL_MSG("connect state: "
                                "Not advanced, more fragments to send");
                }
            }
            else {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
        }

        switch (ssl->options.connectState) {

        case CONNECT_BEGIN :
            /* always send client hello first */
            if ( (ssl->error = SendClientHello(ssl)) != 0) {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
            ssl->options.connectState = CLIENT_HELLO_SENT;
            WOLFSSL_MSG("connect state: CLIENT_HELLO_SENT");

        case CLIENT_HELLO_SENT :
            neededState = ssl->options.resuming ? SERVER_FINISHED_COMPLETE :
                                          SERVER_HELLODONE_COMPLETE;
            #ifdef WOLFSSL_DTLS
                /* In DTLS, when resuming, we can go straight to FINISHED,
                 * or do a cookie exchange and then skip to FINISHED, assume
                 * we need the cookie exchange first. */
                if (ssl->options.dtls)
                    neededState = SERVER_HELLOVERIFYREQUEST_COMPLETE;
            #endif
            /* get response */
            while (ssl->options.serverState < neededState) {
                if ( (ssl->error = ProcessReply(ssl)) < 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
                /* if resumption failed, reset needed state */
                else if (neededState == SERVER_FINISHED_COMPLETE)
                    if (!ssl->options.resuming) {
                        if (!ssl->options.dtls)
                            neededState = SERVER_HELLODONE_COMPLETE;
                        else
                            neededState = SERVER_HELLOVERIFYREQUEST_COMPLETE;
                    }
            }

            ssl->options.connectState = HELLO_AGAIN;
            WOLFSSL_MSG("connect state: HELLO_AGAIN");

        case HELLO_AGAIN :
            if (ssl->options.certOnly)
                return SSL_SUCCESS;

            #ifdef WOLFSSL_DTLS
                if (ssl->options.dtls) {
                    /* re-init hashes, exclude first hello and verify request */
#ifndef NO_OLD_TLS
                    wc_InitMd5(&ssl->hsHashes->hashMd5);
                    if ( (ssl->error = wc_InitSha(&ssl->hsHashes->hashSha))
                                                                         != 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }
#endif
                    if (IsAtLeastTLSv1_2(ssl)) {
                        #ifndef NO_SHA256
                            if ( (ssl->error = wc_InitSha256(
                                            &ssl->hsHashes->hashSha256)) != 0) {
                                WOLFSSL_ERROR(ssl->error);
                                return SSL_FATAL_ERROR;
                            }
                        #endif
                        #ifdef WOLFSSL_SHA384
                            if ( (ssl->error = wc_InitSha384(
                                            &ssl->hsHashes->hashSha384)) != 0) {
                                WOLFSSL_ERROR(ssl->error);
                                return SSL_FATAL_ERROR;
                            }
                        #endif
                        #ifdef WOLFSSL_SHA512
                            if ( (ssl->error = wc_InitSha512(
                                            &ssl->hsHashes->hashSha512)) != 0) {
                                WOLFSSL_ERROR(ssl->error);
                                return SSL_FATAL_ERROR;
                            }
                        #endif
                    }
                    if ( (ssl->error = SendClientHello(ssl)) != 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }
                }
            #endif

            ssl->options.connectState = HELLO_AGAIN_REPLY;
            WOLFSSL_MSG("connect state: HELLO_AGAIN_REPLY");

        case HELLO_AGAIN_REPLY :
            #ifdef WOLFSSL_DTLS
                if (ssl->options.dtls) {
                    neededState = ssl->options.resuming ?
                           SERVER_FINISHED_COMPLETE : SERVER_HELLODONE_COMPLETE;

                    /* get response */
                    while (ssl->options.serverState < neededState) {
                        if ( (ssl->error = ProcessReply(ssl)) < 0) {
                                WOLFSSL_ERROR(ssl->error);
                                return SSL_FATAL_ERROR;
                        }
                        /* if resumption failed, reset needed state */
                        else if (neededState == SERVER_FINISHED_COMPLETE)
                            if (!ssl->options.resuming)
                                neededState = SERVER_HELLODONE_COMPLETE;
                    }
                }
            #endif

            ssl->options.connectState = FIRST_REPLY_DONE;
            WOLFSSL_MSG("connect state: FIRST_REPLY_DONE");

        case FIRST_REPLY_DONE :
            #ifndef NO_CERTS
                if (ssl->options.sendVerify) {
                    if ( (ssl->error = SendCertificate(ssl)) != 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }
                    WOLFSSL_MSG("sent: certificate");
                }

            #endif
            ssl->options.connectState = FIRST_REPLY_FIRST;
            WOLFSSL_MSG("connect state: FIRST_REPLY_FIRST");

        case FIRST_REPLY_FIRST :
            if (!ssl->options.resuming) {
                if ( (ssl->error = SendClientKeyExchange(ssl)) != 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
                WOLFSSL_MSG("sent: client key exchange");
            }

            ssl->options.connectState = FIRST_REPLY_SECOND;
            WOLFSSL_MSG("connect state: FIRST_REPLY_SECOND");

        case FIRST_REPLY_SECOND :
            #ifndef NO_CERTS
                if (ssl->options.sendVerify) {
                    if ( (ssl->error = SendCertificateVerify(ssl)) != 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }
                    WOLFSSL_MSG("sent: certificate verify");
                }
            #endif
            ssl->options.connectState = FIRST_REPLY_THIRD;
            WOLFSSL_MSG("connect state: FIRST_REPLY_THIRD");

        case FIRST_REPLY_THIRD :
            if ( (ssl->error = SendChangeCipher(ssl)) != 0) {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
            WOLFSSL_MSG("sent: change cipher spec");
            ssl->options.connectState = FIRST_REPLY_FOURTH;
            WOLFSSL_MSG("connect state: FIRST_REPLY_FOURTH");

        case FIRST_REPLY_FOURTH :
            if ( (ssl->error = SendFinished(ssl)) != 0) {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
            WOLFSSL_MSG("sent: finished");
            ssl->options.connectState = FINISHED_DONE;
            WOLFSSL_MSG("connect state: FINISHED_DONE");

        case FINISHED_DONE :
            /* get response */
            while (ssl->options.serverState < SERVER_FINISHED_COMPLETE)
                if ( (ssl->error = ProcessReply(ssl)) < 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }

            ssl->options.connectState = SECOND_REPLY_DONE;
            WOLFSSL_MSG("connect state: SECOND_REPLY_DONE");

        case SECOND_REPLY_DONE:
#ifndef NO_HANDSHAKE_DONE_CB
            if (ssl->hsDoneCb) {
                int cbret = ssl->hsDoneCb(ssl, ssl->hsDoneCtx);
                if (cbret < 0) {
                    ssl->error = cbret;
                    WOLFSSL_MSG("HandShake Done Cb don't continue error");
                    return SSL_FATAL_ERROR;
                }
            }
#endif /* NO_HANDSHAKE_DONE_CB */

            if (!ssl->options.dtls) {
                FreeHandshakeResources(ssl);
            }
#ifdef WOLFSSL_DTLS
            else {
                ssl->options.dtlsHsRetain = 1;
            }
#endif /* WOLFSSL_DTLS */

            WOLFSSL_LEAVE("SSL_connect()", SSL_SUCCESS);
            return SSL_SUCCESS;

        default:
            WOLFSSL_MSG("Unknown connect state ERROR");
            return SSL_FATAL_ERROR; /* unknown connect state */
        }
    }

#endif /* NO_WOLFSSL_CLIENT */


/* server only parts */
#ifndef NO_WOLFSSL_SERVER

    #if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
    WOLFSSL_METHOD* wolfSSLv3_server_method(void)
    {
        WOLFSSL_ENTER("SSLv3_server_method");
        return wolfSSLv3_server_method_ex(NULL);
    }
    #endif


    #ifdef WOLFSSL_DTLS

        #ifndef NO_OLD_TLS
        WOLFSSL_METHOD* wolfDTLSv1_server_method(void)
        {
            WOLFSSL_ENTER("DTLSv1_server_method");
            return wolfDTLSv1_server_method_ex(NULL);
        }
        #endif /* NO_OLD_TLS */

        WOLFSSL_METHOD* wolfDTLSv1_2_server_method(void)
        {
            WOLFSSL_ENTER("DTLSv1_2_server_method");
            return wolfDTLSv1_2_server_method_ex(NULL);
        }
    #endif

    #if defined(WOLFSSL_ALLOW_SSLV3) && !defined(NO_OLD_TLS)
    WOLFSSL_METHOD* wolfSSLv3_server_method_ex(void* heap)
    {
        WOLFSSL_METHOD* method =
                              (WOLFSSL_METHOD*) XMALLOC(sizeof(WOLFSSL_METHOD),
                                                     heap, DYNAMIC_TYPE_METHOD);
        WOLFSSL_ENTER("SSLv3_server_method_ex");
        if (method) {
            InitSSL_Method(method, MakeSSLv3());
            method->side = WOLFSSL_SERVER_END;
        }
        return method;
    }
    #endif


    #ifdef WOLFSSL_DTLS

        #ifndef NO_OLD_TLS
        WOLFSSL_METHOD* wolfDTLSv1_server_method_ex(void* heap)
        {
            WOLFSSL_METHOD* method =
                              (WOLFSSL_METHOD*) XMALLOC(sizeof(WOLFSSL_METHOD),
                                                     heap, DYNAMIC_TYPE_METHOD);
            WOLFSSL_ENTER("DTLSv1_server_method_ex");
            if (method) {
                InitSSL_Method(method, MakeDTLSv1());
                method->side = WOLFSSL_SERVER_END;
            }
            return method;
        }
        #endif /* NO_OLD_TLS */

        WOLFSSL_METHOD* wolfDTLSv1_2_server_method_ex(void* heap)
        {
            WOLFSSL_METHOD* method =
                              (WOLFSSL_METHOD*) XMALLOC(sizeof(WOLFSSL_METHOD),
                                                     heap, DYNAMIC_TYPE_METHOD);
            WOLFSSL_ENTER("DTLSv1_2_server_method_ex");
            if (method) {
                InitSSL_Method(method, MakeDTLSv1_2());
                method->side = WOLFSSL_SERVER_END;
            }
            return method;
        }
    #endif

    int wolfSSL_accept(WOLFSSL* ssl)
    {
        word16 havePSK = 0;
        word16 haveAnon = 0;
        WOLFSSL_ENTER("SSL_accept()");

        #ifdef HAVE_ERRNO_H
            errno = 0;
        #endif

        #ifndef NO_PSK
            havePSK = ssl->options.havePSK;
        #endif
        (void)havePSK;

        #ifdef HAVE_ANON
            haveAnon = ssl->options.haveAnon;
        #endif
        (void)haveAnon;

        if (ssl->options.side != WOLFSSL_SERVER_END) {
            WOLFSSL_ERROR(ssl->error = SIDE_ERROR);
            return SSL_FATAL_ERROR;
        }

        #ifndef NO_CERTS
            /* in case used set_accept_state after init */
            if (!havePSK && !haveAnon &&
                (!ssl->buffers.certificate ||
                 !ssl->buffers.certificate->buffer ||
                 !ssl->buffers.key ||
                 !ssl->buffers.key->buffer)) {
                WOLFSSL_MSG("accept error: don't have server cert and key");
                ssl->error = NO_PRIVATE_KEY;
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
        #endif

        #ifdef WOLFSSL_DTLS
            if (ssl->version.major == DTLS_MAJOR) {
                ssl->options.dtls   = 1;
                ssl->options.tls    = 1;
                ssl->options.tls1_1 = 1;

                if (DtlsPoolInit(ssl) != 0) {
                    ssl->error = MEMORY_ERROR;
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            }
        #endif

        if (ssl->buffers.outputBuffer.length > 0) {
            if ( (ssl->error = SendBuffered(ssl)) == 0) {
                /* fragOffset is non-zero when sending fragments. On the last
                 * fragment, fragOffset is zero again, and the state can be
                 * advanced. */
                if (ssl->fragOffset == 0) {
                    ssl->options.acceptState++;
                    WOLFSSL_MSG("accept state: "
                                "Advanced from last buffered fragment send");
                }
                else {
                    WOLFSSL_MSG("accept state: "
                                "Not advanced, more fragments to send");
                }
            }
            else {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
        }

        switch (ssl->options.acceptState) {

        case ACCEPT_BEGIN :
            /* get response */
            while (ssl->options.clientState < CLIENT_HELLO_COMPLETE)
                if ( (ssl->error = ProcessReply(ssl)) < 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            ssl->options.acceptState = ACCEPT_CLIENT_HELLO_DONE;
            WOLFSSL_MSG("accept state ACCEPT_CLIENT_HELLO_DONE");

        case ACCEPT_CLIENT_HELLO_DONE :
            ssl->options.acceptState = ACCEPT_FIRST_REPLY_DONE;
            WOLFSSL_MSG("accept state ACCEPT_FIRST_REPLY_DONE");

        case ACCEPT_FIRST_REPLY_DONE :
            if ( (ssl->error = SendServerHello(ssl)) != 0) {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
            ssl->options.acceptState = SERVER_HELLO_SENT;
            WOLFSSL_MSG("accept state SERVER_HELLO_SENT");

        case SERVER_HELLO_SENT :
            #ifndef NO_CERTS
                if (!ssl->options.resuming)
                    if ( (ssl->error = SendCertificate(ssl)) != 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }
            #endif
            ssl->options.acceptState = CERT_SENT;
            WOLFSSL_MSG("accept state CERT_SENT");

        case CERT_SENT :
            #ifndef NO_CERTS
            if (!ssl->options.resuming)
                if ( (ssl->error = SendCertificateStatus(ssl)) != 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            #endif
            ssl->options.acceptState = CERT_STATUS_SENT;
            WOLFSSL_MSG("accept state CERT_STATUS_SENT");

        case CERT_STATUS_SENT :
            if (!ssl->options.resuming)
                if ( (ssl->error = SendServerKeyExchange(ssl)) != 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            ssl->options.acceptState = KEY_EXCHANGE_SENT;
            WOLFSSL_MSG("accept state KEY_EXCHANGE_SENT");

        case KEY_EXCHANGE_SENT :
            #ifndef NO_CERTS
                if (!ssl->options.resuming)
                    if (ssl->options.verifyPeer)
                        if ( (ssl->error = SendCertificateRequest(ssl)) != 0) {
                            WOLFSSL_ERROR(ssl->error);
                            return SSL_FATAL_ERROR;
                        }
            #endif
            ssl->options.acceptState = CERT_REQ_SENT;
            WOLFSSL_MSG("accept state CERT_REQ_SENT");

        case CERT_REQ_SENT :
            if (!ssl->options.resuming)
                if ( (ssl->error = SendServerHelloDone(ssl)) != 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            ssl->options.acceptState = SERVER_HELLO_DONE;
            WOLFSSL_MSG("accept state SERVER_HELLO_DONE");

        case SERVER_HELLO_DONE :
            if (!ssl->options.resuming) {
                while (ssl->options.clientState < CLIENT_FINISHED_COMPLETE)
                    if ( (ssl->error = ProcessReply(ssl)) < 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }
            }
            ssl->options.acceptState = ACCEPT_SECOND_REPLY_DONE;
            WOLFSSL_MSG("accept state  ACCEPT_SECOND_REPLY_DONE");

        case ACCEPT_SECOND_REPLY_DONE :
#ifdef HAVE_SESSION_TICKET
            if (ssl->options.createTicket) {
                if ( (ssl->error = SendTicket(ssl)) != 0) {
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            }
#endif /* HAVE_SESSION_TICKET */
            ssl->options.acceptState = TICKET_SENT;
            WOLFSSL_MSG("accept state  TICKET_SENT");

        case TICKET_SENT:
            if ( (ssl->error = SendChangeCipher(ssl)) != 0) {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }
            ssl->options.acceptState = CHANGE_CIPHER_SENT;
            WOLFSSL_MSG("accept state  CHANGE_CIPHER_SENT");

        case CHANGE_CIPHER_SENT :
            if ( (ssl->error = SendFinished(ssl)) != 0) {
                WOLFSSL_ERROR(ssl->error);
                return SSL_FATAL_ERROR;
            }

            ssl->options.acceptState = ACCEPT_FINISHED_DONE;
            WOLFSSL_MSG("accept state ACCEPT_FINISHED_DONE");

        case ACCEPT_FINISHED_DONE :
            if (ssl->options.resuming)
                while (ssl->options.clientState < CLIENT_FINISHED_COMPLETE)
                    if ( (ssl->error = ProcessReply(ssl)) < 0) {
                        WOLFSSL_ERROR(ssl->error);
                        return SSL_FATAL_ERROR;
                    }

            ssl->options.acceptState = ACCEPT_THIRD_REPLY_DONE;
            WOLFSSL_MSG("accept state ACCEPT_THIRD_REPLY_DONE");

        case ACCEPT_THIRD_REPLY_DONE :
#ifndef NO_HANDSHAKE_DONE_CB
            if (ssl->hsDoneCb) {
                int cbret = ssl->hsDoneCb(ssl, ssl->hsDoneCtx);
                if (cbret < 0) {
                    ssl->error = cbret;
                    WOLFSSL_MSG("HandShake Done Cb don't continue error");
                    return SSL_FATAL_ERROR;
                }
            }
#endif /* NO_HANDSHAKE_DONE_CB */

            if (!ssl->options.dtls) {
                FreeHandshakeResources(ssl);
            }
#ifdef WOLFSSL_DTLS
            else {
                ssl->options.dtlsHsRetain = 1;
            }
#endif /* WOLFSSL_DTLS */

#ifdef WOLFSSL_SESSION_EXPORT
            if (ssl->dtls_export) {
                if ((ssl->error = wolfSSL_send_session(ssl)) != 0) {
                    WOLFSSL_MSG("Export DTLS session error");
                    WOLFSSL_ERROR(ssl->error);
                    return SSL_FATAL_ERROR;
                }
            }
#endif

            WOLFSSL_LEAVE("SSL_accept()", SSL_SUCCESS);
            return SSL_SUCCESS;

        default :
            WOLFSSL_MSG("Unknown accept state ERROR");
            return SSL_FATAL_ERROR;
        }
    }

#endif /* NO_WOLFSSL_SERVER */


#ifndef NO_HANDSHAKE_DONE_CB

int wolfSSL_SetHsDoneCb(WOLFSSL* ssl, HandShakeDoneCb cb, void* user_ctx)
{
    WOLFSSL_ENTER("wolfSSL_SetHsDoneCb");

    if (ssl == NULL)
        return BAD_FUNC_ARG;

    ssl->hsDoneCb  = cb;
    ssl->hsDoneCtx = user_ctx;


    return SSL_SUCCESS;
}

#endif /* NO_HANDSHAKE_DONE_CB */


int wolfSSL_Cleanup(void)
{
    int ret = SSL_SUCCESS;
    int release = 0;

    WOLFSSL_ENTER("wolfSSL_Cleanup");

    if (initRefCount == 0)
        return ret;  /* possibly no init yet, but not failure either way */

    if (LockMutex(&count_mutex) != 0) {
        WOLFSSL_MSG("Bad Lock Mutex count");
        return BAD_MUTEX_E;
    }

    release = initRefCount-- == 1;
    if (initRefCount < 0)
        initRefCount = 0;

    UnLockMutex(&count_mutex);

    if (!release)
        return ret;

#ifndef NO_SESSION_CACHE
    if (FreeMutex(&session_mutex) != 0)
        ret = BAD_MUTEX_E;
#endif
    if (FreeMutex(&count_mutex) != 0)
        ret = BAD_MUTEX_E;

#if defined(HAVE_ECC) && defined(FP_ECC)
    wc_ecc_fp_free();
#endif

    return ret;
}


#ifndef NO_SESSION_CACHE


/* some session IDs aren't random after all, let's make them random */
static INLINE word32 HashSession(const byte* sessionID, word32 len, int* error)
{
    byte digest[MAX_DIGEST_SIZE];

#ifndef NO_MD5
    *error =  wc_Md5Hash(sessionID, len, digest);
#elif !defined(NO_SHA)
    *error =  wc_ShaHash(sessionID, len, digest);
#elif !defined(NO_SHA256)
    *error =  wc_Sha256Hash(sessionID, len, digest);
#else
    #error "We need a digest to hash the session IDs"
#endif

    return *error == 0 ? MakeWordFromHash(digest) : 0; /* 0 on failure */
}


void wolfSSL_flush_sessions(WOLFSSL_CTX* ctx, long tm)
{
    /* static table now, no flushing needed */
    (void)ctx;
    (void)tm;
}


/* set ssl session timeout in seconds */
int wolfSSL_set_timeout(WOLFSSL* ssl, unsigned int to)
{
    if (ssl == NULL)
        return BAD_FUNC_ARG;

    ssl->timeout = to;

    return SSL_SUCCESS;
}


/* set ctx session timeout in seconds */
int wolfSSL_CTX_set_timeout(WOLFSSL_CTX* ctx, unsigned int to)
{
    if (ctx == NULL)
        return BAD_FUNC_ARG;

    ctx->timeout = to;

    return SSL_SUCCESS;
}


#ifndef NO_CLIENT_CACHE

/* Get Session from Client cache based on id/len, return NULL on failure */
WOLFSSL_SESSION* GetSessionClient(WOLFSSL* ssl, const byte* id, int len)
{
    WOLFSSL_SESSION* ret = NULL;
    word32          row;
    int             idx;
    int             count;
    int             error = 0;

    WOLFSSL_ENTER("GetSessionClient");

    if (ssl->options.side == WOLFSSL_SERVER_END)
        return NULL;

    len = min(SERVER_ID_LEN, (word32)len);
    row = HashSession(id, len, &error) % SESSION_ROWS;
    if (error != 0) {
        WOLFSSL_MSG("Hash session failed");
        return NULL;
    }

    if (LockMutex(&session_mutex) != 0) {
        WOLFSSL_MSG("Lock session mutex failed");
        return NULL;
    }

    /* start from most recently used */
    count = min((word32)ClientCache[row].totalCount, SESSIONS_PER_ROW);
    idx = ClientCache[row].nextIdx - 1;
    if (idx < 0)
        idx = SESSIONS_PER_ROW - 1; /* if back to front, the previous was end */

    for (; count > 0; --count, idx = idx ? idx - 1 : SESSIONS_PER_ROW - 1) {
        WOLFSSL_SESSION* current;
        ClientSession   clSess;

        if (idx >= SESSIONS_PER_ROW || idx < 0) { /* sanity check */
            WOLFSSL_MSG("Bad idx");
            break;
        }

        clSess = ClientCache[row].Clients[idx];

        current = &SessionCache[clSess.serverRow].Sessions[clSess.serverIdx];
        if (XMEMCMP(current->serverID, id, len) == 0) {
            WOLFSSL_MSG("Found a serverid match for client");
            if (LowResTimer() < (current->bornOn + current->timeout)) {
                WOLFSSL_MSG("Session valid");
                ret = current;
                break;
            } else {
                WOLFSSL_MSG("Session timed out");  /* could have more for id */
            }
        } else {
            WOLFSSL_MSG("ServerID not a match from client table");
        }
    }

    UnLockMutex(&session_mutex);

    return ret;
}

#endif /* NO_CLIENT_CACHE */


WOLFSSL_SESSION* GetSession(WOLFSSL* ssl, byte* masterSecret,
        byte restoreSessionCerts)
{
    WOLFSSL_SESSION* ret = 0;
    const byte*  id = NULL;
    word32       row;
    int          idx;
    int          count;
    int          error = 0;

    (void)       restoreSessionCerts;

    if (ssl->options.sessionCacheOff)
        return NULL;

    if (ssl->options.haveSessionId == 0)
        return NULL;

#ifdef HAVE_SESSION_TICKET
    if (ssl->options.side == WOLFSSL_SERVER_END && ssl->options.useTicket == 1)
        return NULL;
#endif

    if (ssl->arrays)
        id = ssl->arrays->sessionID;
    else
        id = ssl->session.sessionID;

    row = HashSession(id, ID_LEN, &error) % SESSION_ROWS;
    if (error != 0) {
        WOLFSSL_MSG("Hash session failed");
        return NULL;
    }

    if (LockMutex(&session_mutex) != 0)
        return 0;

    /* start from most recently used */
    count = min((word32)SessionCache[row].totalCount, SESSIONS_PER_ROW);
    idx = SessionCache[row].nextIdx - 1;
    if (idx < 0)
        idx = SESSIONS_PER_ROW - 1; /* if back to front, the previous was end */

    for (; count > 0; --count, idx = idx ? idx - 1 : SESSIONS_PER_ROW - 1) {
        WOLFSSL_SESSION* current;

        if (idx >= SESSIONS_PER_ROW || idx < 0) { /* sanity check */
            WOLFSSL_MSG("Bad idx");
            break;
        }

        current = &SessionCache[row].Sessions[idx];
        if (XMEMCMP(current->sessionID, id, ID_LEN) == 0) {
            WOLFSSL_MSG("Found a session match");
            if (LowResTimer() < (current->bornOn + current->timeout)) {
                WOLFSSL_MSG("Session valid");
                ret = current;
                if (masterSecret)
                    XMEMCPY(masterSecret, current->masterSecret, SECRET_LEN);
#ifdef SESSION_CERTS
                /* If set, we should copy the session certs into the ssl object
                 * from the session we are returning so we can resume */
                if (restoreSessionCerts) {
                    ssl->session.chain        = ret->chain;
                    ssl->session.version      = ret->version;
                    ssl->session.cipherSuite0 = ret->cipherSuite0;
                    ssl->session.cipherSuite  = ret->cipherSuite;
                }
#endif /* SESSION_CERTS */

            } else {
                WOLFSSL_MSG("Session timed out");
            }
            break;  /* no more sessionIDs whether valid or not that match */
        } else {
            WOLFSSL_MSG("SessionID not a match at this idx");
        }
    }

    UnLockMutex(&session_mutex);

    return ret;
}


int GetDeepCopySession(WOLFSSL* ssl, WOLFSSL_SESSION* copyFrom)
{
    WOLFSSL_SESSION* copyInto = &ssl->session;
    void* tmpBuff             = NULL;
    int ticketLen             = 0;
    int doDynamicCopy         = 0;
    int ret                   = SSL_SUCCESS;

    (void)ticketLen;
    (void)doDynamicCopy;
    (void)tmpBuff;

    if (!ssl || !copyFrom)
        return BAD_FUNC_ARG;

#ifdef HAVE_SESSION_TICKET
    /* Free old dynamic ticket if we had one to avoid leak */
    if (copyInto->isDynamic) {
        XFREE(copyInto->ticket, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
        copyInto->ticket = copyInto->staticTicket;
        copyInto->isDynamic = 0;
    }
#endif

    if (LockMutex(&session_mutex) != 0)
        return BAD_MUTEX_E;

#ifdef HAVE_SESSION_TICKET
    /* Size of ticket to alloc if needed; Use later for alloc outside lock */
    doDynamicCopy = copyFrom->isDynamic;
    ticketLen = copyFrom->ticketLen;
#endif

    *copyInto = *copyFrom;

    /* Default ticket to non dynamic. This will avoid crash if we fail below */
#ifdef HAVE_SESSION_TICKET
    copyInto->ticket = copyInto->staticTicket;
    copyInto->isDynamic = 0;
#endif

    if (UnLockMutex(&session_mutex) != 0) {
        return BAD_MUTEX_E;
    }

#ifdef HAVE_SESSION_TICKET
    /* If doing dynamic copy, need to alloc outside lock, then inside a lock
     * confirm the size still matches and memcpy */
    if (doDynamicCopy) {
        tmpBuff = XMALLOC(ticketLen, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
        if (!tmpBuff)
            return MEMORY_ERROR;

        if (LockMutex(&session_mutex) != 0) {
            XFREE(tmpBuff, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
            return BAD_MUTEX_E;
        }

        if (ticketLen != copyFrom->ticketLen) {
            /* Another thread modified the ssl-> session ticket during alloc.
             * Treat as error, since ticket different than when copy requested */
            ret = VAR_STATE_CHANGE_E;
        }

        if (ret == SSL_SUCCESS) {
            copyInto->ticket = tmpBuff;
            copyInto->isDynamic = 1;
            XMEMCPY(copyInto->ticket, copyFrom->ticket, ticketLen);
        }
    } else {
        /* Need to ensure ticket pointer gets updated to own buffer
         * and is not pointing to buff of session copied from */
        copyInto->ticket = copyInto->staticTicket;
    }

    if (UnLockMutex(&session_mutex) != 0) {
        if (ret == SSL_SUCCESS)
            ret = BAD_MUTEX_E;
    }

    if (ret != SSL_SUCCESS) {
        /* cleanup */
        if (tmpBuff)
            XFREE(tmpBuff, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
        copyInto->ticket = copyInto->staticTicket;
        copyInto->isDynamic = 0;
    }
#endif /* HAVE_SESSION_TICKET */
    return ret;
}


int SetSession(WOLFSSL* ssl, WOLFSSL_SESSION* session)
{
    if (ssl->options.sessionCacheOff)
        return SSL_FAILURE;

    if (LowResTimer() < (session->bornOn + session->timeout)) {
        GetDeepCopySession(ssl, session);
        ssl->options.resuming = 1;

#ifdef SESSION_CERTS
        ssl->version              = session->version;
        ssl->options.cipherSuite0 = session->cipherSuite0;
        ssl->options.cipherSuite  = session->cipherSuite;
#endif

        return SSL_SUCCESS;
    }
    return SSL_FAILURE;  /* session timed out */
}


#ifdef WOLFSSL_SESSION_STATS
static int get_locked_session_stats(word32* active, word32* total,
                                    word32* peak);
#endif

int AddSession(WOLFSSL* ssl)
{
    word32 row, idx;
    int    error = 0;
#ifdef HAVE_SESSION_TICKET
    byte*  tmpBuff = NULL;
    int    ticLen  = 0;
#endif

    if (ssl->options.sessionCacheOff)
        return 0;

    if (ssl->options.haveSessionId == 0)
        return 0;

#ifdef HAVE_SESSION_TICKET
    if (ssl->options.side == WOLFSSL_SERVER_END && ssl->options.useTicket == 1)
        return 0;
#endif

    row = HashSession(ssl->arrays->sessionID, ID_LEN, &error) % SESSION_ROWS;
    if (error != 0) {
        WOLFSSL_MSG("Hash session failed");
        return error;
    }

#ifdef HAVE_SESSION_TICKET
    ticLen = ssl->session.ticketLen;
    /* Alloc Memory here so if Malloc fails can exit outside of lock */
    if(ticLen > SESSION_TICKET_LEN) {
        tmpBuff = XMALLOC(ticLen, ssl->heap,
                DYNAMIC_TYPE_SESSION_TICK);
        if(!tmpBuff)
            return MEMORY_E;
    }
#endif

    if (LockMutex(&session_mutex) != 0) {
#ifdef HAVE_SESSION_TICKET
        XFREE(tmpBuff, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
#endif
        return BAD_MUTEX_E;
    }

    idx = SessionCache[row].nextIdx++;
#ifdef SESSION_INDEX
    ssl->sessionIndex = (row << SESSIDX_ROW_SHIFT) | idx;
#endif

    XMEMCPY(SessionCache[row].Sessions[idx].masterSecret,
           ssl->arrays->masterSecret, SECRET_LEN);
    XMEMCPY(SessionCache[row].Sessions[idx].sessionID, ssl->arrays->sessionID,
           ID_LEN);
    SessionCache[row].Sessions[idx].sessionIDSz = ssl->arrays->sessionIDSz;

    SessionCache[row].Sessions[idx].timeout = ssl->timeout;
    SessionCache[row].Sessions[idx].bornOn  = LowResTimer();

#ifdef HAVE_SESSION_TICKET
    /* Check if another thread modified ticket since alloc */
    if (ticLen != ssl->session.ticketLen) {
        error = VAR_STATE_CHANGE_E;
    }

    if (error == 0) {
        /* Cleanup cache row's old Dynamic buff if exists */
        if(SessionCache[row].Sessions[idx].isDynamic) {
            XFREE(SessionCache[row].Sessions[idx].ticket,
                   ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
            SessionCache[row].Sessions[idx].ticket = NULL;
        }

        /* If too large to store in static buffer, use dyn buffer */
        if (ticLen > SESSION_TICKET_LEN) {
            SessionCache[row].Sessions[idx].ticket = tmpBuff;
            SessionCache[row].Sessions[idx].isDynamic = 1;
        } else {
            SessionCache[row].Sessions[idx].ticket =
                    SessionCache[row].Sessions[idx].staticTicket;
            SessionCache[row].Sessions[idx].isDynamic = 0;
        }
    }

    if (error == 0) {
        SessionCache[row].Sessions[idx].ticketLen     = ticLen;
        XMEMCPY(SessionCache[row].Sessions[idx].ticket,
                                   ssl->session.ticket, ticLen);
    } else { /* cleanup, reset state */
        SessionCache[row].Sessions[idx].ticket    =
            SessionCache[row].Sessions[idx].staticTicket;
        SessionCache[row].Sessions[idx].isDynamic = 0;
        SessionCache[row].Sessions[idx].ticketLen = 0;
        if (tmpBuff) {
            XFREE(tmpBuff, ssl->heap, DYNAMIC_TYPE_SESSION_TICK);
            tmpBuff = NULL;
        }
    }
#endif

#ifdef SESSION_CERTS
    if (error == 0) {
        SessionCache[row].Sessions[idx].chain.count = ssl->session.chain.count;
        XMEMCPY(SessionCache[row].Sessions[idx].chain.certs,
               ssl->session.chain.certs, sizeof(x509_buffer) * MAX_CHAIN_DEPTH);

        SessionCache[row].Sessions[idx].version      = ssl->version;
        SessionCache[row].Sessions[idx].cipherSuite0 = ssl->options.cipherSuite0;
        SessionCache[row].Sessions[idx].cipherSuite  = ssl->options.cipherSuite;
    }
#endif /* SESSION_CERTS */
    if (error == 0) {
        SessionCache[row].totalCount++;
        if (SessionCache[row].nextIdx == SESSIONS_PER_ROW)
            SessionCache[row].nextIdx = 0;
    }
#ifndef NO_CLIENT_CACHE
    if (error == 0) {
        if (ssl->options.side == WOLFSSL_CLIENT_END && ssl->session.idLen) {
            word32 clientRow, clientIdx;

            WOLFSSL_MSG("Adding client cache entry");

            SessionCache[row].Sessions[idx].idLen = ssl->session.idLen;
            XMEMCPY(SessionCache[row].Sessions[idx].serverID,
                    ssl->session.serverID, ssl->session.idLen);

            clientRow = HashSession(ssl->session.serverID, ssl->session.idLen,
                                    &error) % SESSION_ROWS;
            if (error != 0) {
                WOLFSSL_MSG("Hash session failed");
            } else {
                clientIdx = ClientCache[clientRow].nextIdx++;

                ClientCache[clientRow].Clients[clientIdx].serverRow =
                                                                   (word16)row;
                ClientCache[clientRow].Clients[clientIdx].serverIdx =
                                                                   (word16)idx;

                ClientCache[clientRow].totalCount++;
                if (ClientCache[clientRow].nextIdx == SESSIONS_PER_ROW)
                    ClientCache[clientRow].nextIdx = 0;
            }
        }
        else
            SessionCache[row].Sessions[idx].idLen = 0;
    }
#endif /* NO_CLIENT_CACHE */

#if defined(WOLFSSL_SESSION_STATS) && defined(WOLFSSL_PEAK_SESSIONS)
    if (error == 0) {
        word32 active = 0;

        error = get_locked_session_stats(&active, NULL, NULL);
        if (error == SSL_SUCCESS) {
            error = 0;  /* back to this function ok */

            if (active > PeakSessions)
                PeakSessions = active;
        }
    }
#endif /* defined(WOLFSSL_SESSION_STATS) && defined(WOLFSSL_PEAK_SESSIONS) */

    if (UnLockMutex(&session_mutex) != 0)
        return BAD_MUTEX_E;

    return error;
}


#ifdef SESSION_INDEX

int wolfSSL_GetSessionIndex(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_GetSessionIndex");
    WOLFSSL_LEAVE("wolfSSL_GetSessionIndex", ssl->sessionIndex);
    return ssl->sessionIndex;
}


int wolfSSL_GetSessionAtIndex(int idx, WOLFSSL_SESSION* session)
{
    int row, col, result = SSL_FAILURE;

    WOLFSSL_ENTER("wolfSSL_GetSessionAtIndex");

    row = idx >> SESSIDX_ROW_SHIFT;
    col = idx & SESSIDX_IDX_MASK;

    if (LockMutex(&session_mutex) != 0) {
        return BAD_MUTEX_E;
    }

    if (row < SESSION_ROWS &&
        col < (int)min(SessionCache[row].totalCount, SESSIONS_PER_ROW)) {
        XMEMCPY(session,
                 &SessionCache[row].Sessions[col], sizeof(WOLFSSL_SESSION));
        result = SSL_SUCCESS;
    }

    if (UnLockMutex(&session_mutex) != 0)
        result = BAD_MUTEX_E;

    WOLFSSL_LEAVE("wolfSSL_GetSessionAtIndex", result);
    return result;
}

#endif /* SESSION_INDEX */

#if defined(SESSION_INDEX) && defined(SESSION_CERTS)

WOLFSSL_X509_CHAIN* wolfSSL_SESSION_get_peer_chain(WOLFSSL_SESSION* session)
{
    WOLFSSL_X509_CHAIN* chain = NULL;

    WOLFSSL_ENTER("wolfSSL_SESSION_get_peer_chain");
    if (session)
        chain = &session->chain;

    WOLFSSL_LEAVE("wolfSSL_SESSION_get_peer_chain", chain ? 1 : 0);
    return chain;
}

#endif /* SESSION_INDEX && SESSION_CERTS */


#ifdef WOLFSSL_SESSION_STATS

/* requires session_mutex lock held, SSL_SUCCESS on ok */
static int get_locked_session_stats(word32* active, word32* total, word32* peak)
{
    int result = SSL_SUCCESS;
    int i;
    int count;
    int idx;
    word32 now   = 0;
    word32 seen  = 0;
    word32 ticks = LowResTimer();

    (void)peak;

    WOLFSSL_ENTER("get_locked_session_stats");

    for (i = 0; i < SESSION_ROWS; i++) {
        seen += SessionCache[i].totalCount;

        if (active == NULL)
            continue;  /* no need to calculate what we can't set */

        count = min((word32)SessionCache[i].totalCount, SESSIONS_PER_ROW);
        idx   = SessionCache[i].nextIdx - 1;
        if (idx < 0)
            idx = SESSIONS_PER_ROW - 1; /* if back to front previous was end */

        for (; count > 0; --count, idx = idx ? idx - 1 : SESSIONS_PER_ROW - 1) {
            if (idx >= SESSIONS_PER_ROW || idx < 0) {  /* sanity check */
                WOLFSSL_MSG("Bad idx");
                break;
            }

            /* if not expried then good */
            if (ticks < (SessionCache[i].Sessions[idx].bornOn +
                         SessionCache[i].Sessions[idx].timeout) ) {
                now++;
            }
        }
    }

    if (active)
        *active = now;

    if (total)
        *total = seen;

#ifdef WOLFSSL_PEAK_SESSIONS
    if (peak)
        *peak = PeakSessions;
#endif

    WOLFSSL_LEAVE("get_locked_session_stats", result);

    return result;
}


/* return SSL_SUCCESS on ok */
int wolfSSL_get_session_stats(word32* active, word32* total, word32* peak,
                              word32* maxSessions)
{
    int result = SSL_SUCCESS;

    WOLFSSL_ENTER("wolfSSL_get_session_stats");

    if (maxSessions) {
        *maxSessions = SESSIONS_PER_ROW * SESSION_ROWS;

        if (active == NULL && total == NULL && peak == NULL)
            return result;  /* we're done */
    }

    /* user must provide at least one query value */
    if (active == NULL && total == NULL && peak == NULL)
        return BAD_FUNC_ARG;

    if (LockMutex(&session_mutex) != 0) {
        return BAD_MUTEX_E;
    }

    result = get_locked_session_stats(active, total, peak);

    if (UnLockMutex(&session_mutex) != 0)
        result = BAD_MUTEX_E;

    WOLFSSL_LEAVE("wolfSSL_get_session_stats", result);

    return result;
}

#endif /* WOLFSSL_SESSION_STATS */


    #ifdef PRINT_SESSION_STATS

    /* SSL_SUCCESS on ok */
    int wolfSSL_PrintSessionStats(void)
    {
        word32 totalSessionsSeen = 0;
        word32 totalSessionsNow = 0;
        word32 peak = 0;
        word32 maxSessions = 0;
        int    i;
        int    ret;
        double E;               /* expected freq */
        double chiSquare = 0;

        ret = wolfSSL_get_session_stats(&totalSessionsNow, &totalSessionsSeen,
                                        &peak, &maxSessions);
        if (ret != SSL_SUCCESS)
            return ret;
        printf("Total Sessions Seen = %d\n", totalSessionsSeen);
        printf("Total Sessions Now  = %d\n", totalSessionsNow);
#ifdef WOLFSSL_PEAK_SESSIONS
        printf("Peak  Sessions      = %d\n", peak);
#endif
        printf("Max   Sessions      = %d\n", maxSessions);

        E = (double)totalSessionsSeen / SESSION_ROWS;

        for (i = 0; i < SESSION_ROWS; i++) {
            double diff = SessionCache[i].totalCount - E;
            diff *= diff;                /* square    */
            diff /= E;                   /* normalize */

            chiSquare += diff;
        }
        printf("  chi-square = %5.1f, d.f. = %d\n", chiSquare,
                                                     SESSION_ROWS - 1);
        #if (SESSION_ROWS == 11)
            printf(" .05 p value =  18.3, chi-square should be less\n");
        #elif (SESSION_ROWS == 211)
            printf(".05 p value  = 244.8, chi-square should be less\n");
        #elif (SESSION_ROWS == 5981)
            printf(".05 p value  = 6161.0, chi-square should be less\n");
        #elif (SESSION_ROWS == 3)
            printf(".05 p value  =   6.0, chi-square should be less\n");
        #elif (SESSION_ROWS == 2861)
            printf(".05 p value  = 2985.5, chi-square should be less\n");
        #endif
        printf("\n");

        return ret;
    }

    #endif /* SESSION_STATS */

#else  /* NO_SESSION_CACHE */

/* No session cache version */
WOLFSSL_SESSION* GetSession(WOLFSSL* ssl, byte* masterSecret,
        byte restoreSessionCerts)
{
    (void)ssl;
    (void)masterSecret;
    (void)restoreSessionCerts;

    return NULL;
}

#endif /* NO_SESSION_CACHE */


/* call before SSL_connect, if verifying will add name check to
   date check and signature check */
int wolfSSL_check_domain_name(WOLFSSL* ssl, const char* dn)
{
    WOLFSSL_ENTER("wolfSSL_check_domain_name");
    if (ssl->buffers.domainName.buffer)
        XFREE(ssl->buffers.domainName.buffer, ssl->heap, DYNAMIC_TYPE_DOMAIN);

    ssl->buffers.domainName.length = (word32)XSTRLEN(dn) + 1;
    ssl->buffers.domainName.buffer = (byte*) XMALLOC(
                ssl->buffers.domainName.length, ssl->heap, DYNAMIC_TYPE_DOMAIN);

    if (ssl->buffers.domainName.buffer) {
        XSTRNCPY((char*)ssl->buffers.domainName.buffer, dn,
                ssl->buffers.domainName.length);
        return SSL_SUCCESS;
    }
    else {
        ssl->error = MEMORY_ERROR;
        return SSL_FAILURE;
    }
}


/* turn on wolfSSL zlib compression
   returns SSL_SUCCESS for success, else error (not built in)
*/
int wolfSSL_set_compression(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_set_compression");
    (void)ssl;
#ifdef HAVE_LIBZ
    ssl->options.usingCompression = 1;
    return SSL_SUCCESS;
#else
    return NOT_COMPILED_IN;
#endif
}


#ifndef USE_WINDOWS_API
    #ifndef NO_WRITEV

        /* simulate writev semantics, doesn't actually do block at a time though
           because of SSL_write behavior and because front adds may be small */
        int wolfSSL_writev(WOLFSSL* ssl, const struct iovec* iov, int iovcnt)
        {
        #ifdef WOLFSSL_SMALL_STACK
            byte   staticBuffer[1]; /* force heap usage */
        #else
            byte   staticBuffer[FILE_BUFFER_SIZE];
        #endif
            byte* myBuffer  = staticBuffer;
            int   dynamic   = 0;
            int   sending   = 0;
            int   idx       = 0;
            int   i;
            int   ret;

            WOLFSSL_ENTER("wolfSSL_writev");

            for (i = 0; i < iovcnt; i++)
                sending += (int)iov[i].iov_len;

            if (sending > (int)sizeof(staticBuffer)) {
                myBuffer = (byte*)XMALLOC(sending, ssl->heap,
                                                           DYNAMIC_TYPE_WRITEV);
                if (!myBuffer)
                    return MEMORY_ERROR;

                dynamic = 1;
            }

            for (i = 0; i < iovcnt; i++) {
                XMEMCPY(&myBuffer[idx], iov[i].iov_base, iov[i].iov_len);
                idx += (int)iov[i].iov_len;
            }

            ret = wolfSSL_write(ssl, myBuffer, sending);

            if (dynamic)
                XFREE(myBuffer, ssl->heap, DYNAMIC_TYPE_WRITEV);

            return ret;
        }
    #endif
#endif


#ifdef WOLFSSL_CALLBACKS

    typedef struct itimerval Itimerval;

    /* don't keep calling simple functions while setting up timer and signals
       if no inlining these are the next best */

    #define AddTimes(a, b, c)                       \
        do {                                        \
            c.tv_sec  = a.tv_sec  + b.tv_sec;       \
            c.tv_usec = a.tv_usec + b.tv_usec;      \
            if (c.tv_usec >=  1000000) {            \
                c.tv_sec++;                         \
                c.tv_usec -= 1000000;               \
            }                                       \
        } while (0)


    #define SubtractTimes(a, b, c)                  \
        do {                                        \
            c.tv_sec  = a.tv_sec  - b.tv_sec;       \
            c.tv_usec = a.tv_usec - b.tv_usec;      \
            if (c.tv_usec < 0) {                    \
                c.tv_sec--;                         \
                c.tv_usec += 1000000;               \
            }                                       \
        } while (0)

    #define CmpTimes(a, b, cmp)                     \
        ((a.tv_sec  ==  b.tv_sec) ?                 \
            (a.tv_usec cmp b.tv_usec) :             \
            (a.tv_sec  cmp b.tv_sec))               \


    /* do nothing handler */
    static void myHandler(int signo)
    {
        (void)signo;
        return;
    }


    static int wolfSSL_ex_wrapper(WOLFSSL* ssl, HandShakeCallBack hsCb,
                                 TimeoutCallBack toCb, Timeval timeout)
    {
        int       ret        = SSL_FATAL_ERROR;
        int       oldTimerOn = 0;   /* was timer already on */
        Timeval   startTime;
        Timeval   endTime;
        Timeval   totalTime;
        Itimerval myTimeout;
        Itimerval oldTimeout; /* if old timer adjust from total time to reset */
        struct sigaction act, oact;

        #define ERR_OUT(x) { ssl->hsInfoOn = 0; ssl->toInfoOn = 0; return x; }

        if (hsCb) {
            ssl->hsInfoOn = 1;
            InitHandShakeInfo(&ssl->handShakeInfo);
        }
        if (toCb) {
            ssl->toInfoOn = 1;
            InitTimeoutInfo(&ssl->timeoutInfo);

            if (gettimeofday(&startTime, 0) < 0)
                ERR_OUT(GETTIME_ERROR);

            /* use setitimer to simulate getitimer, init 0 myTimeout */
            myTimeout.it_interval.tv_sec  = 0;
            myTimeout.it_interval.tv_usec = 0;
            myTimeout.it_value.tv_sec     = 0;
            myTimeout.it_value.tv_usec    = 0;
            if (setitimer(ITIMER_REAL, &myTimeout, &oldTimeout) < 0)
                ERR_OUT(SETITIMER_ERROR);

            if (oldTimeout.it_value.tv_sec || oldTimeout.it_value.tv_usec) {
                oldTimerOn = 1;

                /* is old timer going to expire before ours */
                if (CmpTimes(oldTimeout.it_value, timeout, <)) {
                    timeout.tv_sec  = oldTimeout.it_value.tv_sec;
                    timeout.tv_usec = oldTimeout.it_value.tv_usec;
                }
            }
            myTimeout.it_value.tv_sec  = timeout.tv_sec;
            myTimeout.it_value.tv_usec = timeout.tv_usec;

            /* set up signal handler, don't restart socket send/recv */
            act.sa_handler = myHandler;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
#ifdef SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;
#endif
            if (sigaction(SIGALRM, &act, &oact) < 0)
                ERR_OUT(SIGACT_ERROR);

            if (setitimer(ITIMER_REAL, &myTimeout, 0) < 0)
                ERR_OUT(SETITIMER_ERROR);
        }

        /* do main work */
#ifndef NO_WOLFSSL_CLIENT
        if (ssl->options.side == WOLFSSL_CLIENT_END)
            ret = wolfSSL_connect(ssl);
#endif
#ifndef NO_WOLFSSL_SERVER
        if (ssl->options.side == WOLFSSL_SERVER_END)
            ret = wolfSSL_accept(ssl);
#endif

        /* do callbacks */
        if (toCb) {
            if (oldTimerOn) {
                gettimeofday(&endTime, 0);
                SubtractTimes(endTime, startTime, totalTime);
                /* adjust old timer for elapsed time */
                if (CmpTimes(totalTime, oldTimeout.it_value, <))
                    SubtractTimes(oldTimeout.it_value, totalTime,
                                  oldTimeout.it_value);
                else {
                    /* reset value to interval, may be off */
                    oldTimeout.it_value.tv_sec = oldTimeout.it_interval.tv_sec;
                    oldTimeout.it_value.tv_usec =oldTimeout.it_interval.tv_usec;
                }
                /* keep iter the same whether there or not */
            }
            /* restore old handler */
            if (sigaction(SIGALRM, &oact, 0) < 0)
                ret = SIGACT_ERROR;    /* more pressing error, stomp */
            else
                /* use old settings which may turn off (expired or not there) */
                if (setitimer(ITIMER_REAL, &oldTimeout, 0) < 0)
                    ret = SETITIMER_ERROR;

            /* if we had a timeout call callback */
            if (ssl->timeoutInfo.timeoutName[0]) {
                ssl->timeoutInfo.timeoutValue.tv_sec  = timeout.tv_sec;
                ssl->timeoutInfo.timeoutValue.tv_usec = timeout.tv_usec;
                (toCb)(&ssl->timeoutInfo);
            }
            /* clean up */
            FreeTimeoutInfo(&ssl->timeoutInfo, ssl->heap);
            ssl->toInfoOn = 0;
        }
        if (hsCb) {
            FinishHandShakeInfo(&ssl->handShakeInfo, ssl);
            (hsCb)(&ssl->handShakeInfo);
            ssl->hsInfoOn = 0;
        }
        return ret;
    }


#ifndef NO_WOLFSSL_CLIENT

    int wolfSSL_connect_ex(WOLFSSL* ssl, HandShakeCallBack hsCb,
                          TimeoutCallBack toCb, Timeval timeout)
    {
        WOLFSSL_ENTER("wolfSSL_connect_ex");
        return wolfSSL_ex_wrapper(ssl, hsCb, toCb, timeout);
    }

#endif


#ifndef NO_WOLFSSL_SERVER

    int wolfSSL_accept_ex(WOLFSSL* ssl, HandShakeCallBack hsCb,
                         TimeoutCallBack toCb,Timeval timeout)
    {
        WOLFSSL_ENTER("wolfSSL_accept_ex");
        return wolfSSL_ex_wrapper(ssl, hsCb, toCb, timeout);
    }

#endif

#endif /* WOLFSSL_CALLBACKS */


#ifndef NO_PSK

    void wolfSSL_CTX_set_psk_client_callback(WOLFSSL_CTX* ctx,
                                         wc_psk_client_callback cb)
    {
        WOLFSSL_ENTER("SSL_CTX_set_psk_client_callback");
        ctx->havePSK = 1;
        ctx->client_psk_cb = cb;
    }


    void wolfSSL_set_psk_client_callback(WOLFSSL* ssl,wc_psk_client_callback cb)
    {
        byte haveRSA = 1;

        WOLFSSL_ENTER("SSL_set_psk_client_callback");
        ssl->options.havePSK = 1;
        ssl->options.client_psk_cb = cb;

        #ifdef NO_RSA
            haveRSA = 0;
        #endif
        InitSuites(ssl->suites, ssl->version, haveRSA, TRUE,
                   ssl->options.haveDH, ssl->options.haveNTRU,
                   ssl->options.haveECDSAsig, ssl->options.haveECC,
                   ssl->options.haveStaticECC, ssl->options.side);
    }


    void wolfSSL_CTX_set_psk_server_callback(WOLFSSL_CTX* ctx,
                                         wc_psk_server_callback cb)
    {
        WOLFSSL_ENTER("SSL_CTX_set_psk_server_callback");
        ctx->havePSK = 1;
        ctx->server_psk_cb = cb;
    }


    void wolfSSL_set_psk_server_callback(WOLFSSL* ssl,wc_psk_server_callback cb)
    {
        byte haveRSA = 1;

        WOLFSSL_ENTER("SSL_set_psk_server_callback");
        ssl->options.havePSK = 1;
        ssl->options.server_psk_cb = cb;

        #ifdef NO_RSA
            haveRSA = 0;
        #endif
        InitSuites(ssl->suites, ssl->version, haveRSA, TRUE,
                   ssl->options.haveDH, ssl->options.haveNTRU,
                   ssl->options.haveECDSAsig, ssl->options.haveECC,
                   ssl->options.haveStaticECC, ssl->options.side);
    }


    const char* wolfSSL_get_psk_identity_hint(const WOLFSSL* ssl)
    {
        WOLFSSL_ENTER("SSL_get_psk_identity_hint");

        if (ssl == NULL || ssl->arrays == NULL)
            return NULL;

        return ssl->arrays->server_hint;
    }


    const char* wolfSSL_get_psk_identity(const WOLFSSL* ssl)
    {
        WOLFSSL_ENTER("SSL_get_psk_identity");

        if (ssl == NULL || ssl->arrays == NULL)
            return NULL;

        return ssl->arrays->client_identity;
    }


    int wolfSSL_CTX_use_psk_identity_hint(WOLFSSL_CTX* ctx, const char* hint)
    {
        WOLFSSL_ENTER("SSL_CTX_use_psk_identity_hint");
        if (hint == 0)
            ctx->server_hint[0] = 0;
        else {
            XSTRNCPY(ctx->server_hint, hint, MAX_PSK_ID_LEN);
            ctx->server_hint[MAX_PSK_ID_LEN - 1] = '\0';
        }
        return SSL_SUCCESS;
    }


    int wolfSSL_use_psk_identity_hint(WOLFSSL* ssl, const char* hint)
    {
        WOLFSSL_ENTER("SSL_use_psk_identity_hint");

        if (ssl == NULL || ssl->arrays == NULL)
            return SSL_FAILURE;

        if (hint == 0)
            ssl->arrays->server_hint[0] = 0;
        else {
            XSTRNCPY(ssl->arrays->server_hint, hint, MAX_PSK_ID_LEN);
            ssl->arrays->server_hint[MAX_PSK_ID_LEN - 1] = '\0';
        }
        return SSL_SUCCESS;
    }

#endif /* NO_PSK */


#ifdef HAVE_ANON

    int wolfSSL_CTX_allow_anon_cipher(WOLFSSL_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_allow_anon_cipher");

        if (ctx == NULL)
            return SSL_FAILURE;

        ctx->haveAnon = 1;

        return SSL_SUCCESS;
    }

#endif /* HAVE_ANON */


#ifndef NO_CERTS
/* used to be defined on NO_FILESYSTEM only, but are generally useful */

    /* wolfSSL extension allows DER files to be loaded from buffers as well */
    int wolfSSL_CTX_load_verify_buffer(WOLFSSL_CTX* ctx,
                                       const unsigned char* in,
                                       long sz, int format)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_load_verify_buffer");
        if (format == SSL_FILETYPE_PEM)
            return ProcessChainBuffer(ctx, in, sz, format, CA_TYPE, NULL);
        else
            return ProcessBuffer(ctx, in, sz, format, CA_TYPE, NULL,NULL,0);
    }


#ifdef WOLFSSL_TRUST_PEER_CERT
    int wolfSSL_CTX_trust_peer_buffer(WOLFSSL_CTX* ctx,
                                       const unsigned char* in,
                                       long sz, int format)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_trust_peer_buffer");

        /* sanity check on arguments */
        if (sz < 0 || in == NULL || ctx == NULL) {
            return BAD_FUNC_ARG;
        }

        if (format == SSL_FILETYPE_PEM)
            return ProcessChainBuffer(ctx, in, sz, format,
                                                       TRUSTED_PEER_TYPE, NULL);
        else
            return ProcessBuffer(ctx, in, sz, format, TRUSTED_PEER_TYPE,
                                                                   NULL,NULL,0);
    }
#endif /* WOLFSSL_TRUST_PEER_CERT */


    int wolfSSL_CTX_use_certificate_buffer(WOLFSSL_CTX* ctx,
                                 const unsigned char* in, long sz, int format)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_use_certificate_buffer");
        return ProcessBuffer(ctx, in, sz, format, CERT_TYPE, NULL, NULL, 0);
    }


    int wolfSSL_CTX_use_PrivateKey_buffer(WOLFSSL_CTX* ctx,
                                 const unsigned char* in, long sz, int format)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_use_PrivateKey_buffer");
        return ProcessBuffer(ctx, in, sz, format, PRIVATEKEY_TYPE, NULL,NULL,0);
    }


    int wolfSSL_CTX_use_certificate_chain_buffer(WOLFSSL_CTX* ctx,
                                 const unsigned char* in, long sz)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_use_certificate_chain_buffer");
        return ProcessBuffer(ctx, in, sz, SSL_FILETYPE_PEM, CERT_TYPE, NULL,
                             NULL, 1);
    }


#ifndef NO_DH

    /* server wrapper for ctx or ssl Diffie-Hellman parameters */
    static int wolfSSL_SetTmpDH_buffer_wrapper(WOLFSSL_CTX* ctx, WOLFSSL* ssl,
                                               const unsigned char* buf,
                                               long sz, int format)
    {
        DerBuffer* der = NULL;
        int    ret      = 0;
        word32 pSz = MAX_DH_SIZE;
        word32 gSz = MAX_DH_SIZE;
    #ifdef WOLFSSL_SMALL_STACK
        byte*  p = NULL;
        byte*  g = NULL;
    #else
        byte   p[MAX_DH_SIZE];
        byte   g[MAX_DH_SIZE];
    #endif

        if (ctx == NULL || buf == NULL)
            return BAD_FUNC_ARG;

        ret = AllocDer(&der, 0, DH_PARAM_TYPE, ctx->heap);
        if (ret != 0) {
            return ret;
        }
        der->buffer = (byte*)buf;
        der->length = (word32)sz;

    #ifdef WOLFSSL_SMALL_STACK
        p = (byte*)XMALLOC(pSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        g = (byte*)XMALLOC(gSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);

        if (p == NULL || g == NULL) {
            XFREE(p, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            XFREE(g, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            return MEMORY_E;
        }
    #endif

        if (format != SSL_FILETYPE_ASN1 && format != SSL_FILETYPE_PEM)
            ret = SSL_BAD_FILETYPE;
        else {
            if (format == SSL_FILETYPE_PEM) {
                FreeDer(&der);
                ret = PemToDer(buf, sz, DH_PARAM_TYPE, &der, ctx->heap,
                               NULL, NULL);
            }

            if (ret == 0) {
                if (wc_DhParamsLoad(der->buffer, der->length, p, &pSz, g, &gSz) < 0)
                    ret = SSL_BAD_FILETYPE;
                else if (ssl)
                    ret = wolfSSL_SetTmpDH(ssl, p, pSz, g, gSz);
                else
                    ret = wolfSSL_CTX_SetTmpDH(ctx, p, pSz, g, gSz);
            }
        }

        FreeDer(&der);

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(p, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(g, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        return ret;
    }


    /* server Diffie-Hellman parameters, SSL_SUCCESS on ok */
    int wolfSSL_SetTmpDH_buffer(WOLFSSL* ssl, const unsigned char* buf, long sz,
                               int format)
    {
        if (ssl == NULL)
            return BAD_FUNC_ARG;

        return wolfSSL_SetTmpDH_buffer_wrapper(ssl->ctx, ssl, buf, sz, format);
    }


    /* server ctx Diffie-Hellman parameters, SSL_SUCCESS on ok */
    int wolfSSL_CTX_SetTmpDH_buffer(WOLFSSL_CTX* ctx, const unsigned char* buf,
                                   long sz, int format)
    {
        return wolfSSL_SetTmpDH_buffer_wrapper(ctx, NULL, buf, sz, format);
    }

#endif /* NO_DH */


    int wolfSSL_use_certificate_buffer(WOLFSSL* ssl,
                                 const unsigned char* in, long sz, int format)
    {
        WOLFSSL_ENTER("wolfSSL_use_certificate_buffer");
        return ProcessBuffer(ssl->ctx, in, sz, format,CERT_TYPE,ssl,NULL,0);
    }


    int wolfSSL_use_PrivateKey_buffer(WOLFSSL* ssl,
                                 const unsigned char* in, long sz, int format)
    {
        WOLFSSL_ENTER("wolfSSL_use_PrivateKey_buffer");
        return ProcessBuffer(ssl->ctx, in, sz, format, PRIVATEKEY_TYPE,
                             ssl, NULL, 0);
    }


    int wolfSSL_use_certificate_chain_buffer(WOLFSSL* ssl,
                                 const unsigned char* in, long sz)
    {
        WOLFSSL_ENTER("wolfSSL_use_certificate_chain_buffer");
        return ProcessBuffer(ssl->ctx, in, sz, SSL_FILETYPE_PEM, CERT_TYPE,
                             ssl, NULL, 1);
    }


    /* unload any certs or keys that SSL owns, leave CTX as is
       SSL_SUCCESS on ok */
    int wolfSSL_UnloadCertsKeys(WOLFSSL* ssl)
    {
        if (ssl == NULL) {
            WOLFSSL_MSG("Null function arg");
            return BAD_FUNC_ARG;
        }

        if (ssl->buffers.weOwnCert && !ssl->keepCert) {
            WOLFSSL_MSG("Unloading cert");
            FreeDer(&ssl->buffers.certificate);
            #ifdef KEEP_OUR_CERT
                FreeX509(ssl->ourCert);
                if (ssl->ourCert) {
                    XFREE(ssl->ourCert, ssl->heap, DYNAMIC_TYPE_X509);
                    ssl->ourCert = NULL;
                }
            #endif
            ssl->buffers.weOwnCert = 0;
        }

        if (ssl->buffers.weOwnCertChain) {
            WOLFSSL_MSG("Unloading cert chain");
            FreeDer(&ssl->buffers.certChain);
            ssl->buffers.weOwnCertChain = 0;
        }

        if (ssl->buffers.weOwnKey) {
            WOLFSSL_MSG("Unloading key");
            FreeDer(&ssl->buffers.key);
            ssl->buffers.weOwnKey = 0;
        }

        return SSL_SUCCESS;
    }


    int wolfSSL_CTX_UnloadCAs(WOLFSSL_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_UnloadCAs");

        if (ctx == NULL)
            return BAD_FUNC_ARG;

        return wolfSSL_CertManagerUnloadCAs(ctx->cm);
    }


#ifdef WOLFSSL_TRUST_PEER_CERT
    int wolfSSL_CTX_Unload_trust_peers(WOLFSSL_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_Unload_trust_peers");

        if (ctx == NULL)
            return BAD_FUNC_ARG;

        return wolfSSL_CertManagerUnload_trust_peers(ctx->cm);
    }
#endif /* WOLFSSL_TRUST_PEER_CERT */
/* old NO_FILESYSTEM end */
#endif /* !NO_CERTS */


#if defined(OPENSSL_EXTRA) || defined(GOAHEAD_WS)


    int wolfSSL_add_all_algorithms(void)
    {
        WOLFSSL_ENTER("wolfSSL_add_all_algorithms");
        if (wolfSSL_Init() == SSL_SUCCESS)
            return SSL_SUCCESS;
        else
            return SSL_FATAL_ERROR;
    }


    long wolfSSL_CTX_sess_set_cache_size(WOLFSSL_CTX* ctx, long sz)
    {
        /* cache size fixed at compile time in wolfSSL */
        (void)ctx;
        (void)sz;
        return 0;
    }


    void wolfSSL_CTX_set_quiet_shutdown(WOLFSSL_CTX* ctx, int mode)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_set_quiet_shutdown");
        if (mode)
            ctx->quietShutdown = 1;
    }


    void wolfSSL_set_quiet_shutdown(WOLFSSL* ssl, int mode)
    {
        WOLFSSL_ENTER("wolfSSL_CTX_set_quiet_shutdown");
        if (mode)
            ssl->options.quietShutdown = 1;
    }


    void wolfSSL_set_bio(WOLFSSL* ssl, WOLFSSL_BIO* rd, WOLFSSL_BIO* wr)
    {
        WOLFSSL_ENTER("SSL_set_bio");
        wolfSSL_set_rfd(ssl, rd->fd);
        wolfSSL_set_wfd(ssl, wr->fd);

        ssl->biord = rd;
        ssl->biowr = wr;
    }


    void wolfSSL_CTX_set_client_CA_list(WOLFSSL_CTX* ctx,
                                       STACK_OF(WOLFSSL_X509_NAME)* names)
    {
        (void)ctx;
        (void)names;
    }


    STACK_OF(WOLFSSL_X509_NAME)* wolfSSL_load_client_CA_file(const char* fname)
    {
        (void)fname;
        return 0;
    }


    int wolfSSL_CTX_set_default_verify_paths(WOLFSSL_CTX* ctx)
    {
        /* TODO:, not needed in goahead */
        (void)ctx;
        return SSL_NOT_IMPLEMENTED;
    }


    /* keyblock size in bytes or -1 */
    int wolfSSL_get_keyblock_size(WOLFSSL* ssl)
    {
        if (ssl == NULL)
            return SSL_FATAL_ERROR;

        return 2 * (ssl->specs.key_size + ssl->specs.iv_size +
                    ssl->specs.hash_size);
    }


    /* store keys returns SSL_SUCCESS or -1 on error */
    int wolfSSL_get_keys(WOLFSSL* ssl, unsigned char** ms, unsigned int* msLen,
                                     unsigned char** sr, unsigned int* srLen,
                                     unsigned char** cr, unsigned int* crLen)
    {
        if (ssl == NULL || ssl->arrays == NULL)
            return SSL_FATAL_ERROR;

        *ms = ssl->arrays->masterSecret;
        *sr = ssl->arrays->serverRandom;
        *cr = ssl->arrays->clientRandom;

        *msLen = SECRET_LEN;
        *srLen = RAN_LEN;
        *crLen = RAN_LEN;

        return SSL_SUCCESS;
    }


    void wolfSSL_set_accept_state(WOLFSSL* ssl)
    {
        word16 haveRSA = 1;
        word16 havePSK = 0;

        WOLFSSL_ENTER("SSL_set_accept_state");
        ssl->options.side = WOLFSSL_SERVER_END;
        /* reset suites in case user switched */

        #ifdef NO_RSA
            haveRSA = 0;
        #endif
        #ifndef NO_PSK
            havePSK = ssl->options.havePSK;
        #endif
        InitSuites(ssl->suites, ssl->version, haveRSA, havePSK,
                   ssl->options.haveDH, ssl->options.haveNTRU,
                   ssl->options.haveECDSAsig, ssl->options.haveECC,
                   ssl->options.haveStaticECC, ssl->options.side);
    }
#endif

    /* return true if connection established */
    int wolfSSL_is_init_finished(WOLFSSL* ssl)
    {
        if (ssl == NULL)
            return 0;

        if (ssl->options.handShakeState == HANDSHAKE_DONE)
            return 1;

        return 0;
    }

#if defined(OPENSSL_EXTRA) || defined(GOAHEAD_WS)
    void wolfSSL_CTX_set_tmp_rsa_callback(WOLFSSL_CTX* ctx,
                                      WOLFSSL_RSA*(*f)(WOLFSSL*, int, int))
    {
        /* wolfSSL verifies all these internally */
        (void)ctx;
        (void)f;
    }


    void wolfSSL_set_shutdown(WOLFSSL* ssl, int opt)
    {
        WOLFSSL_ENTER("wolfSSL_set_shutdown");
        if(ssl==NULL) {
            WOLFSSL_MSG("Shutdown not set. ssl is null");
            return;
        }

        ssl->options.sentNotify =  (opt&SSL_SENT_SHUTDOWN) > 0;
        ssl->options.closeNotify = (opt&SSL_RECEIVED_SHUTDOWN) > 0;
    }


    long wolfSSL_CTX_set_options(WOLFSSL_CTX* ctx, long opt)
    {
        /* goahead calls with 0, do nothing */
        WOLFSSL_ENTER("SSL_CTX_set_options");
        (void)ctx;
        return opt;
    }


    int wolfSSL_set_rfd(WOLFSSL* ssl, int rfd)
    {
        WOLFSSL_ENTER("SSL_set_rfd");
        ssl->rfd = rfd;      /* not used directly to allow IO callbacks */

        ssl->IOCB_ReadCtx  = &ssl->rfd;

        return SSL_SUCCESS;
    }


    int wolfSSL_set_wfd(WOLFSSL* ssl, int wfd)
    {
        WOLFSSL_ENTER("SSL_set_wfd");
        ssl->wfd = wfd;      /* not used directly to allow IO callbacks */

        ssl->IOCB_WriteCtx  = &ssl->wfd;

        return SSL_SUCCESS;
    }


    WOLFSSL_RSA* wolfSSL_RSA_generate_key(int len, unsigned long bits,
                                          void(*f)(int, int, void*), void* data)
    {
        /* no tmp key needed, actual generation not supported */
        WOLFSSL_ENTER("RSA_generate_key");
        (void)len;
        (void)bits;
        (void)f;
        (void)data;
        return NULL;
    }



    WOLFSSL_X509* wolfSSL_X509_STORE_CTX_get_current_cert(
                                                    WOLFSSL_X509_STORE_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_X509_STORE_CTX_get_current_cert");
        if(ctx)
            return ctx->current_cert;
        return NULL;
    }


    int wolfSSL_X509_STORE_CTX_get_error(WOLFSSL_X509_STORE_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_X509_STORE_CTX_get_error");
        if (ctx != NULL)
            return ctx->error;
        return 0;
    }


    int wolfSSL_X509_STORE_CTX_get_error_depth(WOLFSSL_X509_STORE_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_X509_STORE_CTX_get_error_depth");
        if(ctx)
            return ctx->error_depth;
        return SSL_FATAL_ERROR;
    }


    WOLFSSL_BIO_METHOD* wolfSSL_BIO_f_buffer(void)
    {
        static WOLFSSL_BIO_METHOD meth;

        WOLFSSL_ENTER("BIO_f_buffer");
        meth.type = BIO_BUFFER;

        return &meth;
    }


    long wolfSSL_BIO_set_write_buffer_size(WOLFSSL_BIO* bio, long size)
    {
        /* wolfSSL has internal buffer, compatibility only */
        WOLFSSL_ENTER("BIO_set_write_buffer_size");
        (void)bio;
        return size;
    }


    WOLFSSL_BIO_METHOD* wolfSSL_BIO_f_ssl(void)
    {
        static WOLFSSL_BIO_METHOD meth;

        WOLFSSL_ENTER("BIO_f_ssl");
        meth.type = BIO_SSL;

        return &meth;
    }


    WOLFSSL_BIO* wolfSSL_BIO_new_socket(int sfd, int closeF)
    {
        WOLFSSL_BIO* bio = (WOLFSSL_BIO*) XMALLOC(sizeof(WOLFSSL_BIO), 0,
                                                DYNAMIC_TYPE_OPENSSL);

        WOLFSSL_ENTER("BIO_new_socket");
        if (bio) {
            bio->type  = BIO_SOCKET;
            bio->close = (byte)closeF;
            bio->eof   = 0;
            bio->ssl   = 0;
            bio->fd    = sfd;
            bio->prev  = 0;
            bio->next  = 0;
            bio->mem   = NULL;
            bio->memLen = 0;
        }
        return bio;
    }


    int wolfSSL_BIO_eof(WOLFSSL_BIO* b)
    {
        WOLFSSL_ENTER("BIO_eof");
        if (b->eof)
            return 1;

        return 0;
    }


    long wolfSSL_BIO_set_ssl(WOLFSSL_BIO* b, WOLFSSL* ssl, int closeF)
    {
        WOLFSSL_ENTER("BIO_set_ssl");
        b->ssl   = ssl;
        b->close = (byte)closeF;
    /* add to ssl for bio free if SSL_free called before/instead of free_all? */

        return 0;
    }


    WOLFSSL_BIO* wolfSSL_BIO_new(WOLFSSL_BIO_METHOD* method)
    {
        WOLFSSL_BIO* bio = (WOLFSSL_BIO*) XMALLOC(sizeof(WOLFSSL_BIO), 0,
                                                DYNAMIC_TYPE_OPENSSL);
        WOLFSSL_ENTER("BIO_new");
        if (bio) {
            bio->type   = method->type;
            bio->close  = 0;
            bio->eof    = 0;
            bio->ssl    = NULL;
            bio->mem    = NULL;
            bio->memLen = 0;
            bio->fd     = 0;
            bio->prev   = NULL;
            bio->next   = NULL;
        }
        return bio;
    }


    int wolfSSL_BIO_get_mem_data(WOLFSSL_BIO* bio, const byte** p)
    {
        if (bio == NULL || p == NULL)
            return SSL_FATAL_ERROR;

        *p = bio->mem;

        return bio->memLen;
    }


    WOLFSSL_BIO* wolfSSL_BIO_new_mem_buf(void* buf, int len)
    {
        WOLFSSL_BIO* bio = NULL;
        if (buf == NULL)
            return bio;

        bio = wolfSSL_BIO_new(wolfSSL_BIO_s_mem());
        if (bio == NULL)
            return bio;

        bio->memLen = len;
        bio->mem    = (byte*)XMALLOC(len, 0, DYNAMIC_TYPE_OPENSSL);
        if (bio->mem == NULL) {
            XFREE(bio, 0, DYNAMIC_TYPE_OPENSSL);
            return NULL;
        }

        XMEMCPY(bio->mem, buf, len);

        return bio;
    }


#ifdef USE_WINDOWS_API
    #define CloseSocket(s) closesocket(s)
#elif defined(WOLFSSL_MDK_ARM)  || defined(WOLFSSL_KEIL_TCP_NET)
    #define CloseSocket(s) closesocket(s)
    extern int closesocket(int) ;
#else
    #define CloseSocket(s) close(s)
#endif

    int wolfSSL_BIO_free(WOLFSSL_BIO* bio)
    {
        /* unchain?, doesn't matter in goahead since from free all */
        WOLFSSL_ENTER("BIO_free");
        if (bio) {
            if (bio->close) {
                if (bio->ssl)
                    wolfSSL_free(bio->ssl);
                if (bio->fd)
                    CloseSocket(bio->fd);
            }
            if (bio->mem)
                XFREE(bio->mem, 0, DYNAMIC_TYPE_OPENSSL);
            XFREE(bio, 0, DYNAMIC_TYPE_OPENSSL);
        }
        return 0;
    }


    int wolfSSL_BIO_free_all(WOLFSSL_BIO* bio)
    {
        WOLFSSL_ENTER("BIO_free_all");
        while (bio) {
            WOLFSSL_BIO* next = bio->next;
            wolfSSL_BIO_free(bio);
            bio = next;
        }
        return 0;
    }


    int wolfSSL_BIO_read(WOLFSSL_BIO* bio, void* buf, int len)
    {
        int  ret;
        WOLFSSL* ssl = 0;
        WOLFSSL_BIO* front = bio;

        WOLFSSL_ENTER("BIO_read");
        /* already got eof, again is error */
        if (front->eof)
            return SSL_FATAL_ERROR;

        while(bio && ((ssl = bio->ssl) == 0) )
            bio = bio->next;

        if (ssl == 0) return BAD_FUNC_ARG;

        ret = wolfSSL_read(ssl, buf, len);
        if (ret == 0)
            front->eof = 1;
        else if (ret < 0) {
            int err = wolfSSL_get_error(ssl, 0);
            if ( !(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) )
                front->eof = 1;
        }
        return ret;
    }


    int wolfSSL_BIO_write(WOLFSSL_BIO* bio, const void* data, int len)
    {
        int  ret;
        WOLFSSL* ssl = 0;
        WOLFSSL_BIO* front = bio;

        WOLFSSL_ENTER("BIO_write");
        /* already got eof, again is error */
        if (front->eof)
            return SSL_FATAL_ERROR;

        while(bio && ((ssl = bio->ssl) == 0) )
            bio = bio->next;

        if (ssl == 0) return BAD_FUNC_ARG;

        ret = wolfSSL_write(ssl, data, len);
        if (ret == 0)
            front->eof = 1;
        else if (ret < 0) {
            int err = wolfSSL_get_error(ssl, 0);
            if ( !(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) )
                front->eof = 1;
        }

        return ret;
    }


    WOLFSSL_BIO* wolfSSL_BIO_push(WOLFSSL_BIO* top, WOLFSSL_BIO* append)
    {
        WOLFSSL_ENTER("BIO_push");
        top->next    = append;
        append->prev = top;

        return top;
    }


    int wolfSSL_BIO_flush(WOLFSSL_BIO* bio)
    {
        /* for wolfSSL no flushing needed */
        WOLFSSL_ENTER("BIO_flush");
        (void)bio;
        return 1;
    }


#endif /* OPENSSL_EXTRA || GOAHEAD_WS */


#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)

    void wolfSSL_CTX_set_default_passwd_cb_userdata(WOLFSSL_CTX* ctx,
                                                   void* userdata)
    {
        WOLFSSL_ENTER("SSL_CTX_set_default_passwd_cb_userdata");
        ctx->userdata = userdata;
    }


    void wolfSSL_CTX_set_default_passwd_cb(WOLFSSL_CTX* ctx, pem_password_cb cb)
    {
        WOLFSSL_ENTER("SSL_CTX_set_default_passwd_cb");
        ctx->passwd_cb = cb;
    }

    int wolfSSL_num_locks(void)
    {
        return 0;
    }

    void wolfSSL_set_locking_callback(void (*f)(int, int, const char*, int))
    {
        (void)f;
    }

    void wolfSSL_set_id_callback(unsigned long (*f)(void))
    {
        (void)f;
    }

    unsigned long wolfSSL_ERR_get_error(void)
    {
        /* TODO: */
        return 0;
    }

#ifndef NO_MD5

    int wolfSSL_EVP_BytesToKey(const WOLFSSL_EVP_CIPHER* type,
                       const WOLFSSL_EVP_MD* md, const byte* salt,
                       const byte* data, int sz, int count, byte* key, byte* iv)
    {
        int  keyLen = 0;
        int  ivLen  = 0;
        int  j;
        int  keyLeft;
        int  ivLeft;
        int  keyOutput = 0;
        byte digest[MD5_DIGEST_SIZE];
    #ifdef WOLFSSL_SMALL_STACK
        Md5* md5 = NULL;
    #else
        Md5  md5[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        md5 = (Md5*)XMALLOC(sizeof(Md5), NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (md5 == NULL)
            return 0;
    #endif

        WOLFSSL_ENTER("wolfSSL_EVP_BytesToKey");
        wc_InitMd5(md5);

        /* only support MD5 for now */
        if (XSTRNCMP(md, "MD5", 3) != 0) return 0;

        /* only support CBC DES and AES for now */
        #ifndef NO_DES3
        if (XSTRNCMP(type, EVP_DES_CBC, EVP_DES_SIZE) == 0) {
            keyLen = DES_KEY_SIZE;
            ivLen  = DES_IV_SIZE;
        }
        else if (XSTRNCMP(type, EVP_DES_EDE3_CBC, EVP_DES_EDE3_SIZE) == 0) {
            keyLen = DES3_KEY_SIZE;
            ivLen  = DES_IV_SIZE;
        }
        else
        #endif /* NO_DES3 */
        if (XSTRNCMP(type, EVP_AES_128_CBC, EVP_AES_SIZE) == 0) {
            keyLen = AES_128_KEY_SIZE;
            ivLen  = AES_IV_SIZE;
        }
        else if (XSTRNCMP(type, EVP_AES_192_CBC, EVP_AES_SIZE) == 0) {
            keyLen = AES_192_KEY_SIZE;
            ivLen  = AES_IV_SIZE;
        }
        else if (XSTRNCMP(type, EVP_AES_256_CBC, EVP_AES_SIZE) == 0) {
            keyLen = AES_256_KEY_SIZE;
            ivLen  = AES_IV_SIZE;
        }
        else {
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(md5, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
            return 0;
        }

        keyLeft   = keyLen;
        ivLeft    = ivLen;

        while (keyOutput < (keyLen + ivLen)) {
            int digestLeft = MD5_DIGEST_SIZE;
            /* D_(i - 1) */
            if (keyOutput)                      /* first time D_0 is empty */
                wc_Md5Update(md5, digest, MD5_DIGEST_SIZE);
            /* data */
            wc_Md5Update(md5, data, sz);
            /* salt */
            if (salt)
                wc_Md5Update(md5, salt, EVP_SALT_SIZE);
            wc_Md5Final(md5, digest);
            /* count */
            for (j = 1; j < count; j++) {
                wc_Md5Update(md5, digest, MD5_DIGEST_SIZE);
                wc_Md5Final(md5, digest);
            }

            if (keyLeft) {
                int store = min(keyLeft, MD5_DIGEST_SIZE);
                XMEMCPY(&key[keyLen - keyLeft], digest, store);

                keyOutput  += store;
                keyLeft    -= store;
                digestLeft -= store;
            }

            if (ivLeft && digestLeft) {
                int store = min(ivLeft, digestLeft);
                if (iv != NULL)
                    XMEMCPY(&iv[ivLen - ivLeft],
                            &digest[MD5_DIGEST_SIZE - digestLeft], store);
                keyOutput += store;
                ivLeft    -= store;
            }
        }

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(md5, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        return keyOutput == (keyLen + ivLen) ? keyOutput : 0;
    }

#endif /* NO_MD5 */

#endif /* OPENSSL_EXTRA || HAVE_WEBSERVER */


#ifdef OPENSSL_EXTRA

    unsigned long wolfSSLeay(void)
    {
        return SSLEAY_VERSION_NUMBER;
    }


    const char* wolfSSLeay_version(int type)
    {
        static const char* version = "SSLeay wolfSSL compatibility";
        (void)type;
        return version;
    }


#ifndef NO_MD5
    void wolfSSL_MD5_Init(WOLFSSL_MD5_CTX* md5)
    {
        typedef char md5_test[sizeof(MD5_CTX) >= sizeof(Md5) ? 1 : -1];
        (void)sizeof(md5_test);

        WOLFSSL_ENTER("MD5_Init");
        wc_InitMd5((Md5*)md5);
    }


    void wolfSSL_MD5_Update(WOLFSSL_MD5_CTX* md5, const void* input,
                           unsigned long sz)
    {
        WOLFSSL_ENTER("wolfSSL_MD5_Update");
        wc_Md5Update((Md5*)md5, (const byte*)input, (word32)sz);
    }


    void wolfSSL_MD5_Final(byte* input, WOLFSSL_MD5_CTX* md5)
    {
        WOLFSSL_ENTER("MD5_Final");
        wc_Md5Final((Md5*)md5, input);
    }
#endif /* NO_MD5 */


#ifndef NO_SHA
    void wolfSSL_SHA_Init(WOLFSSL_SHA_CTX* sha)
    {
        typedef char sha_test[sizeof(SHA_CTX) >= sizeof(Sha) ? 1 : -1];
        (void)sizeof(sha_test);

        WOLFSSL_ENTER("SHA_Init");
        wc_InitSha((Sha*)sha);  /* OpenSSL compat, no ret */
    }


    void wolfSSL_SHA_Update(WOLFSSL_SHA_CTX* sha, const void* input,
                           unsigned long sz)
    {
        WOLFSSL_ENTER("SHA_Update");
        wc_ShaUpdate((Sha*)sha, (const byte*)input, (word32)sz);
    }


    void wolfSSL_SHA_Final(byte* input, WOLFSSL_SHA_CTX* sha)
    {
        WOLFSSL_ENTER("SHA_Final");
        wc_ShaFinal((Sha*)sha, input);
    }


    void wolfSSL_SHA1_Init(WOLFSSL_SHA_CTX* sha)
    {
        WOLFSSL_ENTER("SHA1_Init");
        SHA_Init(sha);
    }


    void wolfSSL_SHA1_Update(WOLFSSL_SHA_CTX* sha, const void* input,
                            unsigned long sz)
    {
        WOLFSSL_ENTER("SHA1_Update");
        SHA_Update(sha, input, sz);
    }


    void wolfSSL_SHA1_Final(byte* input, WOLFSSL_SHA_CTX* sha)
    {
        WOLFSSL_ENTER("SHA1_Final");
        SHA_Final(input, sha);
    }
#endif /* NO_SHA */


    void wolfSSL_SHA256_Init(WOLFSSL_SHA256_CTX* sha256)
    {
        typedef char sha_test[sizeof(SHA256_CTX) >= sizeof(Sha256) ? 1 : -1];
        (void)sizeof(sha_test);

        WOLFSSL_ENTER("SHA256_Init");
        wc_InitSha256((Sha256*)sha256);  /* OpenSSL compat, no error */
    }


    void wolfSSL_SHA256_Update(WOLFSSL_SHA256_CTX* sha, const void* input,
                              unsigned long sz)
    {
        WOLFSSL_ENTER("SHA256_Update");
        wc_Sha256Update((Sha256*)sha, (const byte*)input, (word32)sz);
        /* OpenSSL compat, no error */
    }


    void wolfSSL_SHA256_Final(byte* input, WOLFSSL_SHA256_CTX* sha)
    {
        WOLFSSL_ENTER("SHA256_Final");
        wc_Sha256Final((Sha256*)sha, input);
        /* OpenSSL compat, no error */
    }


    #ifdef WOLFSSL_SHA384

    void wolfSSL_SHA384_Init(WOLFSSL_SHA384_CTX* sha)
    {
        typedef char sha_test[sizeof(SHA384_CTX) >= sizeof(Sha384) ? 1 : -1];
        (void)sizeof(sha_test);

        WOLFSSL_ENTER("SHA384_Init");
        wc_InitSha384((Sha384*)sha);   /* OpenSSL compat, no error */
    }


    void wolfSSL_SHA384_Update(WOLFSSL_SHA384_CTX* sha, const void* input,
                           unsigned long sz)
    {
        WOLFSSL_ENTER("SHA384_Update");
        wc_Sha384Update((Sha384*)sha, (const byte*)input, (word32)sz);
        /* OpenSSL compat, no error */
    }


    void wolfSSL_SHA384_Final(byte* input, WOLFSSL_SHA384_CTX* sha)
    {
        WOLFSSL_ENTER("SHA384_Final");
        wc_Sha384Final((Sha384*)sha, input);
        /* OpenSSL compat, no error */
    }

    #endif /* WOLFSSL_SHA384 */


   #ifdef WOLFSSL_SHA512

    void wolfSSL_SHA512_Init(WOLFSSL_SHA512_CTX* sha)
    {
        typedef char sha_test[sizeof(SHA512_CTX) >= sizeof(Sha512) ? 1 : -1];
        (void)sizeof(sha_test);

        WOLFSSL_ENTER("SHA512_Init");
        wc_InitSha512((Sha512*)sha);  /* OpenSSL compat, no error */
    }


    void wolfSSL_SHA512_Update(WOLFSSL_SHA512_CTX* sha, const void* input,
                           unsigned long sz)
    {
        WOLFSSL_ENTER("SHA512_Update");
        wc_Sha512Update((Sha512*)sha, (const byte*)input, (word32)sz);
        /* OpenSSL compat, no error */
    }


    void wolfSSL_SHA512_Final(byte* input, WOLFSSL_SHA512_CTX* sha)
    {
        WOLFSSL_ENTER("SHA512_Final");
        wc_Sha512Final((Sha512*)sha, input);
        /* OpenSSL compat, no error */
    }

    #endif /* WOLFSSL_SHA512 */


    #ifndef NO_MD5

    const WOLFSSL_EVP_MD* wolfSSL_EVP_md5(void)
    {
        static const char* type = "MD5";
        WOLFSSL_ENTER("EVP_md5");
        return type;
    }

    #endif /* NO_MD5 */


#ifndef NO_SHA
    const WOLFSSL_EVP_MD* wolfSSL_EVP_sha1(void)
    {
        static const char* type = "SHA";
        WOLFSSL_ENTER("EVP_sha1");
        return type;
    }
#endif /* NO_SHA */


    const WOLFSSL_EVP_MD* wolfSSL_EVP_sha256(void)
    {
        static const char* type = "SHA256";
        WOLFSSL_ENTER("EVP_sha256");
        return type;
    }

    #ifdef WOLFSSL_SHA384

    const WOLFSSL_EVP_MD* wolfSSL_EVP_sha384(void)
    {
        static const char* type = "SHA384";
        WOLFSSL_ENTER("EVP_sha384");
        return type;
    }

    #endif /* WOLFSSL_SHA384 */

    #ifdef WOLFSSL_SHA512

    const WOLFSSL_EVP_MD* wolfSSL_EVP_sha512(void)
    {
        static const char* type = "SHA512";
        WOLFSSL_ENTER("EVP_sha512");
        return type;
    }

    #endif /* WOLFSSL_SHA512 */


    void wolfSSL_EVP_MD_CTX_init(WOLFSSL_EVP_MD_CTX* ctx)
    {
        WOLFSSL_ENTER("EVP_CIPHER_MD_CTX_init");
        (void)ctx;
        /* do nothing */
    }

    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_aes_128_cbc(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_aes_128_cbc");
        return EVP_AES_128_CBC;
    }


    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_aes_192_cbc(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_aes_192_cbc");
        return EVP_AES_192_CBC;
    }


    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_aes_256_cbc(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_aes_256_cbc");
        return EVP_AES_256_CBC;
    }


    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_aes_128_ctr(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_aes_128_ctr");
        return EVP_AES_128_CTR;
    }


    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_aes_192_ctr(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_aes_192_ctr");
        return EVP_AES_192_CTR;
    }


    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_aes_256_ctr(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_aes_256_ctr");
        return EVP_AES_256_CTR;
    }


#ifndef NO_DES3
    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_des_cbc(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_des_cbc");
        return EVP_DES_CBC;
    }


    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_des_ede3_cbc(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_des_ede3_cbc");
        return EVP_DES_EDE3_CBC;
    }
#endif /* NO_DES3 */

    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_rc4(void)
    {
        static const char* type = "ARC4";
        WOLFSSL_ENTER("wolfSSL_EVP_rc4");
        return type;
    }

#ifdef HAVE_IDEA
    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_idea_cbc(void)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_idea_cbc");
        return EVP_IDEA_CBC;
    }
#endif
    const WOLFSSL_EVP_CIPHER* wolfSSL_EVP_enc_null(void)
    {
        static const char* type = "NULL";
        WOLFSSL_ENTER("wolfSSL_EVP_enc_null");
        return type;
    }


    int wolfSSL_EVP_MD_CTX_cleanup(WOLFSSL_EVP_MD_CTX* ctx)
    {
        WOLFSSL_ENTER("EVP_MD_CTX_cleanup");
        (void)ctx;
        return 0;
    }



    void wolfSSL_EVP_CIPHER_CTX_init(WOLFSSL_EVP_CIPHER_CTX* ctx)
    {
        WOLFSSL_ENTER("EVP_CIPHER_CTX_init");
        if (ctx) {
            ctx->cipherType = 0xff;   /* no init */
            ctx->keyLen     = 0;
            ctx->enc        = 1;      /* start in encrypt mode */
        }
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_CIPHER_CTX_cleanup(WOLFSSL_EVP_CIPHER_CTX* ctx)
    {
        WOLFSSL_ENTER("EVP_CIPHER_CTX_cleanup");
        if (ctx) {
            ctx->cipherType = 0xff;  /* no more init */
            ctx->keyLen     = 0;
        }

        return SSL_SUCCESS;
    }


    /* return SSL_SUCCESS on ok, 0 on failure to match API compatibility */
    int  wolfSSL_EVP_CipherInit(WOLFSSL_EVP_CIPHER_CTX* ctx,
                               const WOLFSSL_EVP_CIPHER* type, byte* key,
                               byte* iv, int enc)
    {
        int ret = -1;  /* failure local, during function 0 means success
                          because internal functions work that way */
        (void)iv;
        (void)enc;

        WOLFSSL_ENTER("wolfSSL_EVP_CipherInit");
        if (ctx == NULL) {
            WOLFSSL_MSG("no ctx");
            return 0;   /* failure */
        }

        if (type == NULL && ctx->cipherType == 0xff) {
            WOLFSSL_MSG("no type set");
            return 0;   /* failure */
        }

#ifndef NO_AES
        if (ctx->cipherType == AES_128_CBC_TYPE ||
            (type && XSTRNCMP(type, EVP_AES_128_CBC, EVP_AES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_AES_128_CBC);
            ctx->cipherType = AES_128_CBC_TYPE;
            ctx->keyLen     = 16;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_AesSetKey(&ctx->cipher.aes, key, ctx->keyLen, iv,
                                ctx->enc ? AES_ENCRYPTION : AES_DECRYPTION);
                if (ret != 0)
                    return ret;
            }
            if (iv && key == NULL) {
                ret = wc_AesSetIV(&ctx->cipher.aes, iv);
                if (ret != 0)
                    return ret;
            }
        }
        else if (ctx->cipherType == AES_192_CBC_TYPE ||
                 (type && XSTRNCMP(type, EVP_AES_192_CBC, EVP_AES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_AES_192_CBC);
            ctx->cipherType = AES_192_CBC_TYPE;
            ctx->keyLen     = 24;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_AesSetKey(&ctx->cipher.aes, key, ctx->keyLen, iv,
                                ctx->enc ? AES_ENCRYPTION : AES_DECRYPTION);
                if (ret != 0)
                    return ret;
            }
            if (iv && key == NULL) {
                ret = wc_AesSetIV(&ctx->cipher.aes, iv);
                if (ret != 0)
                    return ret;
            }
        }
        else if (ctx->cipherType == AES_256_CBC_TYPE ||
                 (type && XSTRNCMP(type, EVP_AES_256_CBC, EVP_AES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_AES_256_CBC);
            ctx->cipherType = AES_256_CBC_TYPE;
            ctx->keyLen     = 32;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_AesSetKey(&ctx->cipher.aes, key, ctx->keyLen, iv,
                                ctx->enc ? AES_ENCRYPTION : AES_DECRYPTION);
                if (ret != 0)
                    return ret;
            }
            if (iv && key == NULL) {
                ret = wc_AesSetIV(&ctx->cipher.aes, iv);
                if (ret != 0)
                    return ret;
            }
        }
#ifdef WOLFSSL_AES_COUNTER
        else if (ctx->cipherType == AES_128_CTR_TYPE ||
                 (type && XSTRNCMP(type, EVP_AES_128_CTR, EVP_AES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_AES_128_CTR);
            ctx->cipherType = AES_128_CTR_TYPE;
            ctx->keyLen     = 16;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_AesSetKey(&ctx->cipher.aes, key, ctx->keyLen, iv,
                                AES_ENCRYPTION);
                if (ret != 0)
                    return ret;
            }
            if (iv && key == NULL) {
                ret = wc_AesSetIV(&ctx->cipher.aes, iv);
                if (ret != 0)
                    return ret;
            }
        }
        else if (ctx->cipherType == AES_192_CTR_TYPE ||
                 (type && XSTRNCMP(type, EVP_AES_192_CTR, EVP_AES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_AES_192_CTR);
            ctx->cipherType = AES_192_CTR_TYPE;
            ctx->keyLen     = 24;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_AesSetKey(&ctx->cipher.aes, key, ctx->keyLen, iv,
                                AES_ENCRYPTION);
                if (ret != 0)
                    return ret;
            }
            if (iv && key == NULL) {
                ret = wc_AesSetIV(&ctx->cipher.aes, iv);
                if (ret != 0)
                    return ret;
            }
        }
        else if (ctx->cipherType == AES_256_CTR_TYPE ||
                 (type && XSTRNCMP(type, EVP_AES_256_CTR, EVP_AES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_AES_256_CTR);
            ctx->cipherType = AES_256_CTR_TYPE;
            ctx->keyLen     = 32;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_AesSetKey(&ctx->cipher.aes, key, ctx->keyLen, iv,
                                AES_ENCRYPTION);
                if (ret != 0)
                    return ret;
            }
            if (iv && key == NULL) {
                ret = wc_AesSetIV(&ctx->cipher.aes, iv);
                if (ret != 0)
                    return ret;
            }
        }
#endif /* WOLFSSL_AES_CTR */
#endif /* NO_AES */

#ifndef NO_DES3
        if (ctx->cipherType == DES_CBC_TYPE ||
                 (type && XSTRNCMP(type, EVP_DES_CBC, EVP_DES_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_DES_CBC);
            ctx->cipherType = DES_CBC_TYPE;
            ctx->keyLen     = 8;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_Des_SetKey(&ctx->cipher.des, key, iv,
                          ctx->enc ? DES_ENCRYPTION : DES_DECRYPTION);
                if (ret != 0)
                    return ret;
            }

            if (iv && key == NULL)
                wc_Des_SetIV(&ctx->cipher.des, iv);
        }
        else if (ctx->cipherType == DES_EDE3_CBC_TYPE ||
                 (type &&
                  XSTRNCMP(type, EVP_DES_EDE3_CBC, EVP_DES_EDE3_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_DES_EDE3_CBC);
            ctx->cipherType = DES_EDE3_CBC_TYPE;
            ctx->keyLen     = 24;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_Des3_SetKey(&ctx->cipher.des3, key, iv,
                          ctx->enc ? DES_ENCRYPTION : DES_DECRYPTION);
                if (ret != 0)
                    return ret;
            }

            if (iv && key == NULL) {
                ret = wc_Des3_SetIV(&ctx->cipher.des3, iv);
                if (ret != 0)
                    return ret;
            }
        }
#endif /* NO_DES3 */
#ifndef NO_RC4
        if (ctx->cipherType == ARC4_TYPE || (type &&
                                     XSTRNCMP(type, "ARC4", 4) == 0)) {
            WOLFSSL_MSG("ARC4");
            ctx->cipherType = ARC4_TYPE;
            if (ctx->keyLen == 0)  /* user may have already set */
                ctx->keyLen = 16;  /* default to 128 */
            if (key)
                wc_Arc4SetKey(&ctx->cipher.arc4, key, ctx->keyLen);
            ret = 0;  /* success */
        }
#endif /* NO_RC4 */
#ifdef HAVE_IDEA
        if (ctx->cipherType == IDEA_CBC_TYPE ||
                 (type && XSTRNCMP(type, EVP_IDEA_CBC, EVP_IDEA_SIZE) == 0)) {
            WOLFSSL_MSG(EVP_IDEA_CBC);
            ctx->cipherType = IDEA_CBC_TYPE;
            ctx->keyLen     = IDEA_KEY_SIZE;
            if (enc == 0 || enc == 1)
                ctx->enc = enc ? 1 : 0;
            if (key) {
                ret = wc_IdeaSetKey(&ctx->cipher.idea, key, (word16)ctx->keyLen,
                                    iv, ctx->enc ? IDEA_ENCRYPTION :
                                                   IDEA_DECRYPTION);
                if (ret != 0)
                    return ret;
            }

            if (iv && key == NULL)
                wc_IdeaSetIV(&ctx->cipher.idea, iv);
        }
#endif /* HAVE_IDEA */
        if (ctx->cipherType == NULL_CIPHER_TYPE || (type &&
                                     XSTRNCMP(type, "NULL", 4) == 0)) {
            WOLFSSL_MSG("NULL cipher");
            ctx->cipherType = NULL_CIPHER_TYPE;
            ctx->keyLen = 0;
            ret = 0;  /* success */
        }

        if (ret == 0)
            return SSL_SUCCESS;
        else
            return 0;  /* overall failure */
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_CIPHER_CTX_key_length(WOLFSSL_EVP_CIPHER_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_CIPHER_CTX_key_length");
        if (ctx)
            return ctx->keyLen;

        return 0;   /* failure */
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_CIPHER_CTX_set_key_length(WOLFSSL_EVP_CIPHER_CTX* ctx,
                                             int keylen)
    {
        WOLFSSL_ENTER("wolfSSL_EVP_CIPHER_CTX_set_key_length");
        if (ctx)
            ctx->keyLen = keylen;
        else
            return 0;  /* failure */

        return SSL_SUCCESS;
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_Cipher(WOLFSSL_EVP_CIPHER_CTX* ctx, byte* dst, byte* src,
                          word32 len)
    {
        int ret = 0;
        WOLFSSL_ENTER("wolfSSL_EVP_Cipher");

        if (ctx == NULL || dst == NULL || src == NULL) {
            WOLFSSL_MSG("Bad function argument");
            return 0;  /* failure */
        }

        if (ctx->cipherType == 0xff) {
            WOLFSSL_MSG("no init");
            return 0;  /* failure */
        }

        switch (ctx->cipherType) {

#ifndef NO_AES
#ifdef HAVE_AES_CBC
            case AES_128_CBC_TYPE :
            case AES_192_CBC_TYPE :
            case AES_256_CBC_TYPE :
                WOLFSSL_MSG("AES CBC");
                if (ctx->enc)
                    ret = wc_AesCbcEncrypt(&ctx->cipher.aes, dst, src, len);
                else
                    ret = wc_AesCbcDecrypt(&ctx->cipher.aes, dst, src, len);
                break;
#endif /* HAVE_AES_CBC */
#ifdef WOLFSSL_AES_COUNTER
            case AES_128_CTR_TYPE :
            case AES_192_CTR_TYPE :
            case AES_256_CTR_TYPE :
                    WOLFSSL_MSG("AES CTR");
                    wc_AesCtrEncrypt(&ctx->cipher.aes, dst, src, len);
                break;
#endif /* WOLFSSL_AES_COUNTER */
#endif /* NO_AES */

#ifndef NO_DES3
            case DES_CBC_TYPE :
                if (ctx->enc)
                    wc_Des_CbcEncrypt(&ctx->cipher.des, dst, src, len);
                else
                    wc_Des_CbcDecrypt(&ctx->cipher.des, dst, src, len);
                break;

            case DES_EDE3_CBC_TYPE :
                if (ctx->enc)
                    ret = wc_Des3_CbcEncrypt(&ctx->cipher.des3, dst, src, len);
                else
                    ret = wc_Des3_CbcDecrypt(&ctx->cipher.des3, dst, src, len);
                break;
#endif

#ifndef NO_RC4
            case ARC4_TYPE :
                wc_Arc4Process(&ctx->cipher.arc4, dst, src, len);
                break;
#endif

#ifdef HAVE_IDEA
            case IDEA_CBC_TYPE :
                if (ctx->enc)
                    wc_IdeaCbcEncrypt(&ctx->cipher.idea, dst, src, len);
                else
                    wc_IdeaCbcDecrypt(&ctx->cipher.idea, dst, src, len);
                break;
#endif
            case NULL_CIPHER_TYPE :
                XMEMCPY(dst, src, len);
                break;

            default: {
                WOLFSSL_MSG("bad type");
                return 0;  /* failure */
            }
        }

        if (ret != 0) {
            WOLFSSL_MSG("wolfSSL_EVP_Cipher failure");
            return 0;  /* failuer */
        }

        WOLFSSL_MSG("wolfSSL_EVP_Cipher success");
        return SSL_SUCCESS;  /* success */
    }


    /* store for external read of iv, SSL_SUCCESS on success */
    int  wolfSSL_StoreExternalIV(WOLFSSL_EVP_CIPHER_CTX* ctx)
    {
        WOLFSSL_ENTER("wolfSSL_StoreExternalIV");

        if (ctx == NULL) {
            WOLFSSL_MSG("Bad function argument");
            return SSL_FATAL_ERROR;
        }

        switch (ctx->cipherType) {

#ifndef NO_AES
            case AES_128_CBC_TYPE :
            case AES_192_CBC_TYPE :
            case AES_256_CBC_TYPE :
                WOLFSSL_MSG("AES CBC");
                XMEMCPY(ctx->iv, &ctx->cipher.aes.reg, AES_BLOCK_SIZE);
                break;

#ifdef WOLFSSL_AES_COUNTER
            case AES_128_CTR_TYPE :
            case AES_192_CTR_TYPE :
            case AES_256_CTR_TYPE :
                WOLFSSL_MSG("AES CTR");
                XMEMCPY(ctx->iv, &ctx->cipher.aes.reg, AES_BLOCK_SIZE);
                break;
#endif /* WOLFSSL_AES_COUNTER */

#endif /* NO_AES */

#ifndef NO_DES3
            case DES_CBC_TYPE :
                WOLFSSL_MSG("DES CBC");
                XMEMCPY(ctx->iv, &ctx->cipher.des.reg, DES_BLOCK_SIZE);
                break;

            case DES_EDE3_CBC_TYPE :
                WOLFSSL_MSG("DES EDE3 CBC");
                XMEMCPY(ctx->iv, &ctx->cipher.des3.reg, DES_BLOCK_SIZE);
                break;
#endif

#ifdef HAVE_IDEA
            case IDEA_CBC_TYPE :
                WOLFSSL_MSG("IDEA CBC");
                XMEMCPY(ctx->iv, &ctx->cipher.idea.reg, IDEA_BLOCK_SIZE);
                break;
#endif
            case ARC4_TYPE :
                WOLFSSL_MSG("ARC4");
                break;

            case NULL_CIPHER_TYPE :
                WOLFSSL_MSG("NULL");
                break;

            default: {
                WOLFSSL_MSG("bad type");
                return SSL_FATAL_ERROR;
            }
        }
        return SSL_SUCCESS;
    }


    /* set internal IV from external, SSL_SUCCESS on success */
    int  wolfSSL_SetInternalIV(WOLFSSL_EVP_CIPHER_CTX* ctx)
    {

        WOLFSSL_ENTER("wolfSSL_SetInternalIV");

        if (ctx == NULL) {
            WOLFSSL_MSG("Bad function argument");
            return SSL_FATAL_ERROR;
        }

        switch (ctx->cipherType) {

#ifndef NO_AES
            case AES_128_CBC_TYPE :
            case AES_192_CBC_TYPE :
            case AES_256_CBC_TYPE :
                WOLFSSL_MSG("AES CBC");
                XMEMCPY(&ctx->cipher.aes.reg, ctx->iv, AES_BLOCK_SIZE);
                break;

#ifdef WOLFSSL_AES_COUNTER
            case AES_128_CTR_TYPE :
            case AES_192_CTR_TYPE :
            case AES_256_CTR_TYPE :
                WOLFSSL_MSG("AES CTR");
                XMEMCPY(&ctx->cipher.aes.reg, ctx->iv, AES_BLOCK_SIZE);
                break;
#endif

#endif /* NO_AES */

#ifndef NO_DES3
            case DES_CBC_TYPE :
                WOLFSSL_MSG("DES CBC");
                XMEMCPY(&ctx->cipher.des.reg, ctx->iv, DES_BLOCK_SIZE);
                break;

            case DES_EDE3_CBC_TYPE :
                WOLFSSL_MSG("DES EDE3 CBC");
                XMEMCPY(&ctx->cipher.des3.reg, ctx->iv, DES_BLOCK_SIZE);
                break;
#endif

#ifdef HAVE_IDEA
            case IDEA_CBC_TYPE :
                WOLFSSL_MSG("IDEA CBC");
                XMEMCPY(&ctx->cipher.idea.reg, ctx->iv, IDEA_BLOCK_SIZE);
                break;
#endif
            case ARC4_TYPE :
                WOLFSSL_MSG("ARC4");
                break;

            case NULL_CIPHER_TYPE :
                WOLFSSL_MSG("NULL");
                break;

            default: {
                WOLFSSL_MSG("bad type");
                return SSL_FATAL_ERROR;
            }
        }
        return SSL_SUCCESS;
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_DigestInit(WOLFSSL_EVP_MD_CTX* ctx,
                               const WOLFSSL_EVP_MD* type)
    {
        WOLFSSL_ENTER("EVP_DigestInit");
        if (XSTRNCMP(type, "SHA256", 6) == 0) {
             ctx->macType = SHA256;
             wolfSSL_SHA256_Init((SHA256_CTX*)&ctx->hash);
        }
    #ifdef WOLFSSL_SHA384
        else if (XSTRNCMP(type, "SHA384", 6) == 0) {
             ctx->macType = SHA384;
             wolfSSL_SHA384_Init((SHA384_CTX*)&ctx->hash);
        }
    #endif
    #ifdef WOLFSSL_SHA512
        else if (XSTRNCMP(type, "SHA512", 6) == 0) {
             ctx->macType = SHA512;
             wolfSSL_SHA512_Init((SHA512_CTX*)&ctx->hash);
        }
    #endif
    #ifndef NO_MD5
        else if (XSTRNCMP(type, "MD5", 3) == 0) {
            ctx->macType = MD5;
            wolfSSL_MD5_Init((MD5_CTX*)&ctx->hash);
        }
    #endif
    #ifndef NO_SHA
        /* has to be last since would pick or 256, 384, or 512 too */
        else if (XSTRNCMP(type, "SHA", 3) == 0) {
             ctx->macType = SHA;
             wolfSSL_SHA_Init((SHA_CTX*)&ctx->hash);
        }
    #endif /* NO_SHA */
        else
             return BAD_FUNC_ARG;

        return SSL_SUCCESS;
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_DigestUpdate(WOLFSSL_EVP_MD_CTX* ctx, const void* data,
                                unsigned long sz)
    {
        WOLFSSL_ENTER("EVP_DigestUpdate");

        switch (ctx->macType) {
#ifndef NO_MD5
            case MD5:
                wolfSSL_MD5_Update((MD5_CTX*)&ctx->hash, data,
                                  (unsigned long)sz);
                break;
#endif
#ifndef NO_SHA
            case SHA:
                wolfSSL_SHA_Update((SHA_CTX*)&ctx->hash, data,
                                  (unsigned long)sz);
                break;
#endif
#ifndef NO_SHA256
            case SHA256:
                wolfSSL_SHA256_Update((SHA256_CTX*)&ctx->hash, data,
                                     (unsigned long)sz);
                break;
#endif
#ifdef WOLFSSL_SHA384
            case SHA384:
                wolfSSL_SHA384_Update((SHA384_CTX*)&ctx->hash, data,
                                     (unsigned long)sz);
                break;
#endif
#ifdef WOLFSSL_SHA512
            case SHA512:
                wolfSSL_SHA512_Update((SHA512_CTX*)&ctx->hash, data,
                                     (unsigned long)sz);
                break;
#endif
            default:
                return BAD_FUNC_ARG;
        }

        return SSL_SUCCESS;
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_DigestFinal(WOLFSSL_EVP_MD_CTX* ctx, unsigned char* md,
                               unsigned int* s)
    {
        WOLFSSL_ENTER("EVP_DigestFinal");
        switch (ctx->macType) {
#ifndef NO_MD5
            case MD5:
                wolfSSL_MD5_Final(md, (MD5_CTX*)&ctx->hash);
                if (s) *s = MD5_DIGEST_SIZE;
                break;
#endif
#ifndef NO_SHA
            case SHA:
                wolfSSL_SHA_Final(md, (SHA_CTX*)&ctx->hash);
                if (s) *s = SHA_DIGEST_SIZE;
                break;
#endif
#ifndef NO_SHA256
            case SHA256:
                wolfSSL_SHA256_Final(md, (SHA256_CTX*)&ctx->hash);
                if (s) *s = SHA256_DIGEST_SIZE;
                break;
#endif
#ifdef WOLFSSL_SHA384
            case SHA384:
                wolfSSL_SHA384_Final(md, (SHA384_CTX*)&ctx->hash);
                if (s) *s = SHA384_DIGEST_SIZE;
                break;
#endif
#ifdef WOLFSSL_SHA512
            case SHA512:
                wolfSSL_SHA512_Final(md, (SHA512_CTX*)&ctx->hash);
                if (s) *s = SHA512_DIGEST_SIZE;
                break;
#endif
            default:
                return BAD_FUNC_ARG;
        }

        return SSL_SUCCESS;
    }


    /* SSL_SUCCESS on ok */
    int wolfSSL_EVP_DigestFinal_ex(WOLFSSL_EVP_MD_CTX* ctx, unsigned char* md,
                                   unsigned int* s)
    {
        WOLFSSL_ENTER("EVP_DigestFinal_ex");
        return EVP_DigestFinal(ctx, md, s);
    }


    unsigned char* wolfSSL_HMAC(const WOLFSSL_EVP_MD* evp_md, const void* key,
                                int key_len, const unsigned char* d, int n,
                                unsigned char* md, unsigned int* md_len)
    {
        int type;
        unsigned char* ret = NULL;
#ifdef WOLFSSL_SMALL_STACK
        Hmac* hmac = NULL;
#else
        Hmac  hmac[1];
#endif

        WOLFSSL_ENTER("HMAC");
        if (!md)
            return NULL;  /* no static buffer support */

        if (XSTRNCMP(evp_md, "MD5", 3) == 0)
            type = MD5;
        else if (XSTRNCMP(evp_md, "SHA", 3) == 0)
            type = SHA;
        else
            return NULL;

    #ifdef WOLFSSL_SMALL_STACK
        hmac = (Hmac*)XMALLOC(sizeof(Hmac), NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (hmac == NULL)
            return NULL;
    #endif

        if (wc_HmacSetKey(hmac, type, (const byte*)key, key_len) == 0)
            if (wc_HmacUpdate(hmac, d, n) == 0)
                if (wc_HmacFinal(hmac, md) == 0) {
                    if (md_len)
                        *md_len = (type == MD5) ? (int)MD5_DIGEST_SIZE
                                                : (int)SHA_DIGEST_SIZE;
                    ret = md;
                }

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(hmac, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif

        return ret;
    }

    void wolfSSL_ERR_clear_error(void)
    {
        /* TODO: */
    }


    int wolfSSL_RAND_status(void)
    {
        return SSL_SUCCESS;  /* wolfCrypt provides enough seed internally */
    }



    void wolfSSL_RAND_add(const void* add, int len, double entropy)
    {
        (void)add;
        (void)len;
        (void)entropy;

        /* wolfSSL seeds/adds internally, use explicit RNG if you want
           to take control */
    }


#ifndef NO_DES3
    /* SSL_SUCCESS on ok */
    int wolfSSL_DES_key_sched(WOLFSSL_const_DES_cblock* key,
                              WOLFSSL_DES_key_schedule* schedule)
    {
        WOLFSSL_ENTER("DES_key_sched");
        XMEMCPY(schedule, key, sizeof(const_DES_cblock));
        return SSL_SUCCESS;
    }


    void wolfSSL_DES_cbc_encrypt(const unsigned char* input,
                                 unsigned char* output, long length,
                                 WOLFSSL_DES_key_schedule* schedule,
                                 WOLFSSL_DES_cblock* ivec, int enc)
    {
        Des myDes;

        WOLFSSL_ENTER("DES_cbc_encrypt");

        /* OpenSSL compat, no ret */
        wc_Des_SetKey(&myDes, (const byte*)schedule, (const byte*)ivec, !enc);

        if (enc)
            wc_Des_CbcEncrypt(&myDes, output, input, (word32)length);
        else
            wc_Des_CbcDecrypt(&myDes, output, input, (word32)length);
    }


    /* WOLFSSL_DES_key_schedule is a unsigned char array of size 8 */
    void wolfSSL_DES_ede3_cbc_encrypt(const unsigned char* input,
                                      unsigned char* output, long sz,
                                      WOLFSSL_DES_key_schedule* ks1,
                                      WOLFSSL_DES_key_schedule* ks2,
                                      WOLFSSL_DES_key_schedule* ks3,
                                      WOLFSSL_DES_cblock* ivec, int enc)
    {
        Des3 des;
        byte key[24];/* EDE uses 24 size key */

        WOLFSSL_ENTER("wolfSSL_DES_ede3_cbc_encrypt");

        XMEMSET(key, 0, sizeof(key));
        XMEMCPY(key, *ks1, DES_BLOCK_SIZE);
        XMEMCPY(&key[DES_BLOCK_SIZE], *ks2, DES_BLOCK_SIZE);
        XMEMCPY(&key[DES_BLOCK_SIZE * 2], *ks3, DES_BLOCK_SIZE);

        if (enc) {
            wc_Des3_SetKey(&des, key, (const byte*)ivec, DES_ENCRYPTION);
            wc_Des3_CbcEncrypt(&des, output, input, (word32)sz);
        }
        else {
            wc_Des3_SetKey(&des, key, (const byte*)ivec, DES_DECRYPTION);
            wc_Des3_CbcDecrypt(&des, output, input, (word32)sz);
        }
    }


    /* correctly sets ivec for next call */
    void wolfSSL_DES_ncbc_encrypt(const unsigned char* input,
                     unsigned char* output, long length,
                     WOLFSSL_DES_key_schedule* schedule, WOLFSSL_DES_cblock* ivec,
                     int enc)
    {
        Des myDes;

        WOLFSSL_ENTER("DES_ncbc_encrypt");

        /* OpenSSL compat, no ret */
        wc_Des_SetKey(&myDes, (const byte*)schedule, (const byte*)ivec, !enc);

        if (enc)
            wc_Des_CbcEncrypt(&myDes, output, input, (word32)length);
        else
            wc_Des_CbcDecrypt(&myDes, output, input, (word32)length);

        XMEMCPY(ivec, output + length - sizeof(DES_cblock), sizeof(DES_cblock));
    }

#endif /* NO_DES3 */


    void wolfSSL_ERR_free_strings(void)
    {
        /* handled internally */
    }


    void wolfSSL_ERR_remove_state(unsigned long state)
    {
        /* TODO: GetErrors().Remove(); */
        (void)state;
    }


    void wolfSSL_EVP_cleanup(void)
    {
        /* nothing to do here */
    }


    void wolfSSL_cleanup_all_ex_data(void)
    {
        /* nothing to do here */
    }


    int wolfSSL_clear(WOLFSSL* ssl)
    {
        (void)ssl;
        /* TODO: GetErrors().Remove(); */
        return SSL_SUCCESS;
    }


    long wolfSSL_SSL_SESSION_set_timeout(WOLFSSL_SESSION* ses, long t)
    {
        word32 tmptime;
        if (!ses || t < 0)
            return BAD_FUNC_ARG;

        tmptime = t & 0xFFFFFFFF;

        ses->timeout = tmptime;

        return SSL_SUCCESS;
    }


    long wolfSSL_CTX_set_mode(WOLFSSL_CTX* ctx, long mode)
    {
        /* SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER is wolfSSL default mode */

        WOLFSSL_ENTER("SSL_CTX_set_mode");
        if (mode == SSL_MODE_ENABLE_PARTIAL_WRITE)
            ctx->partialWrite = 1;

        return mode;
    }


    long wolfSSL_SSL_get_mode(WOLFSSL* ssl)
    {
        /* TODO: */
        (void)ssl;
        return 0;
    }


    long wolfSSL_CTX_get_mode(WOLFSSL_CTX* ctx)
    {
        /* TODO: */
        (void)ctx;
        return 0;
    }


    void wolfSSL_CTX_set_default_read_ahead(WOLFSSL_CTX* ctx, int m)
    {
        /* TODO: maybe? */
        (void)ctx;
        (void)m;
    }


    int wolfSSL_CTX_set_session_id_context(WOLFSSL_CTX* ctx,
                                           const unsigned char* sid_ctx,
                                           unsigned int sid_ctx_len)
    {
        /* No application specific context needed for wolfSSL */
        (void)ctx;
        (void)sid_ctx;
        (void)sid_ctx_len;
        return SSL_SUCCESS;
    }


    long wolfSSL_CTX_sess_get_cache_size(WOLFSSL_CTX* ctx)
    {
        /* TODO: maybe? */
        (void)ctx;
        return (~0);
    }

    unsigned long wolfSSL_ERR_get_error_line_data(const char** file, int* line,
                                                  const char** data, int *flags)
    {
        /* Not implemented */
        (void)file;
        (void)line;
        (void)data;
        (void)flags;
        return 0;
    }

#endif /* OPENSSL_EXTRA */


#if defined(KEEP_PEER_CERT)

    WOLFSSL_X509* wolfSSL_get_peer_certificate(WOLFSSL* ssl)
    {
        WOLFSSL_ENTER("SSL_get_peer_certificate");
        if (ssl->peerCert.issuer.sz)
            return &ssl->peerCert;
        else
            return 0;
    }

#endif /* KEEP_PEER_CERT */


#ifndef NO_CERTS
#if defined(KEEP_PEER_CERT) || defined(SESSION_CERTS) || defined(OPENSSL_EXTRA)

/* user externally called free X509, if dynamic go ahead with free, otherwise
 * don't */
static void ExternalFreeX509(WOLFSSL_X509* x509)
{
    WOLFSSL_ENTER("ExternalFreeX509");
    if (x509) {
        if (x509->dynamicMemory) {
            FreeX509(x509);
            XFREE(x509, x509->heap, DYNAMIC_TYPE_X509);
        } else {
            WOLFSSL_MSG("free called on non dynamic object, not freeing");
        }
    }
}

#endif /* KEEP_PEER_CERT || SESSION_CERTS || OPENSSSL_EXTRA */

#if defined(KEEP_PEER_CERT) || defined(SESSION_CERTS)

    void wolfSSL_FreeX509(WOLFSSL_X509* x509)
    {
        WOLFSSL_ENTER("wolfSSL_FreeX509");
        ExternalFreeX509(x509);
    }

    /* return the next, if any, altname from the peer cert */
    char* wolfSSL_X509_get_next_altname(WOLFSSL_X509* cert)
    {
        char* ret = NULL;
        WOLFSSL_ENTER("wolfSSL_X509_get_next_altname");

        /* don't have any to work with */
        if (cert == NULL || cert->altNames == NULL)
            return NULL;

        /* already went through them */
        if (cert->altNamesNext == NULL)
            return NULL;

        ret = cert->altNamesNext->name;
        cert->altNamesNext = cert->altNamesNext->next;

        return ret;
    }


    WOLFSSL_X509_NAME* wolfSSL_X509_get_issuer_name(WOLFSSL_X509* cert)
    {
        WOLFSSL_ENTER("X509_get_issuer_name");
        if(cert)
            return &cert->issuer;
        return NULL;
    }


    WOLFSSL_X509_NAME* wolfSSL_X509_get_subject_name(WOLFSSL_X509* cert)
    {
        WOLFSSL_ENTER("wolfSSL_X509_get_subject_name");
        if(cert)
            return &cert->subject;
        return NULL;
    }


    int wolfSSL_X509_get_isCA(WOLFSSL_X509* x509)
    {
        int isCA = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_isCA");

        if (x509 != NULL)
            isCA = x509->isCa;

        WOLFSSL_LEAVE("wolfSSL_X509_get_isCA", isCA);

        return isCA;
    }


#ifdef OPENSSL_EXTRA
    int wolfSSL_X509_ext_isSet_by_NID(WOLFSSL_X509* x509, int nid)
    {
        int isSet = 0;

        WOLFSSL_ENTER("wolfSSL_X509_ext_isSet_by_NID");

        if (x509 != NULL) {
            switch (nid) {
                case BASIC_CA_OID: isSet = x509->basicConstSet; break;
                case ALT_NAMES_OID: isSet = x509->subjAltNameSet; break;
                case AUTH_KEY_OID: isSet = x509->authKeyIdSet; break;
                case SUBJ_KEY_OID: isSet = x509->subjKeyIdSet; break;
                case KEY_USAGE_OID: isSet = x509->keyUsageSet; break;
                #ifdef WOLFSSL_SEP
                    case CERT_POLICY_OID: isSet = x509->certPolicySet; break;
                #endif /* WOLFSSL_SEP */
            }
        }

        WOLFSSL_LEAVE("wolfSSL_X509_ext_isSet_by_NID", isSet);

        return isSet;
    }


    int wolfSSL_X509_ext_get_critical_by_NID(WOLFSSL_X509* x509, int nid)
    {
        int crit = 0;

        WOLFSSL_ENTER("wolfSSL_X509_ext_get_critical_by_NID");

        if (x509 != NULL) {
            switch (nid) {
                case BASIC_CA_OID: crit = x509->basicConstCrit; break;
                case ALT_NAMES_OID: crit = x509->subjAltNameCrit; break;
                case AUTH_KEY_OID: crit = x509->authKeyIdCrit; break;
                case SUBJ_KEY_OID: crit = x509->subjKeyIdCrit; break;
                case KEY_USAGE_OID: crit = x509->keyUsageCrit; break;
                #ifdef WOLFSSL_SEP
                    case CERT_POLICY_OID: crit = x509->certPolicyCrit; break;
                #endif /* WOLFSSL_SEP */
            }
        }

        WOLFSSL_LEAVE("wolfSSL_X509_ext_get_critical_by_NID", crit);

        return crit;
    }


    int wolfSSL_X509_get_isSet_pathLength(WOLFSSL_X509* x509)
    {
        int isSet = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_isSet_pathLength");

        if (x509 != NULL)
            isSet = x509->basicConstPlSet;

        WOLFSSL_LEAVE("wolfSSL_X509_get_isSet_pathLength", isSet);

        return isSet;
    }


    word32 wolfSSL_X509_get_pathLength(WOLFSSL_X509* x509)
    {
        word32 pathLength = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_pathLength");

        if (x509 != NULL)
            pathLength = x509->pathLength;

        WOLFSSL_LEAVE("wolfSSL_X509_get_pathLength", pathLength);

        return pathLength;
    }


    unsigned int wolfSSL_X509_get_keyUsage(WOLFSSL_X509* x509)
    {
        word16 usage = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_keyUsage");

        if (x509 != NULL)
            usage = x509->keyUsage;

        WOLFSSL_LEAVE("wolfSSL_X509_get_keyUsage", usage);

        return usage;
    }


    byte* wolfSSL_X509_get_authorityKeyID(WOLFSSL_X509* x509,
                                          byte* dst, int* dstLen)
    {
        byte *id = NULL;
        int copySz = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_authorityKeyID");

        if (x509 != NULL) {
            if (x509->authKeyIdSet) {
                copySz = min(dstLen != NULL ? *dstLen : 0,
                             (int)x509->authKeyIdSz);
                id = x509->authKeyId;
            }

            if (dst != NULL && dstLen != NULL && id != NULL && copySz > 0) {
                XMEMCPY(dst, id, copySz);
                id = dst;
                *dstLen = copySz;
            }
        }

        WOLFSSL_LEAVE("wolfSSL_X509_get_authorityKeyID", copySz);

        return id;
    }


    byte* wolfSSL_X509_get_subjectKeyID(WOLFSSL_X509* x509,
                                        byte* dst, int* dstLen)
    {
        byte *id = NULL;
        int copySz = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_subjectKeyID");

        if (x509 != NULL) {
            if (x509->subjKeyIdSet) {
                copySz = min(dstLen != NULL ? *dstLen : 0,
                                                        (int)x509->subjKeyIdSz);
                id = x509->subjKeyId;
            }

            if (dst != NULL && dstLen != NULL && id != NULL && copySz > 0) {
                XMEMCPY(dst, id, copySz);
                id = dst;
                *dstLen = copySz;
            }
        }

        WOLFSSL_LEAVE("wolfSSL_X509_get_subjectKeyID", copySz);

        return id;
    }


    int wolfSSL_X509_NAME_entry_count(WOLFSSL_X509_NAME* name)
    {
        int count = 0;

        WOLFSSL_ENTER("wolfSSL_X509_NAME_entry_count");

        if (name != NULL)
            count = name->fullName.entryCount;

        WOLFSSL_LEAVE("wolfSSL_X509_NAME_entry_count", count);
        return count;
    }


    int wolfSSL_X509_NAME_get_text_by_NID(WOLFSSL_X509_NAME* name,
                                          int nid, char* buf, int len)
    {
        char *text = NULL;
        int textSz = 0;

        WOLFSSL_ENTER("wolfSSL_X509_NAME_get_text_by_NID");

        switch (nid) {
            case ASN_COMMON_NAME:
                text = name->fullName.fullName + name->fullName.cnIdx;
                textSz = name->fullName.cnLen;
                break;
            case ASN_SUR_NAME:
                text = name->fullName.fullName + name->fullName.snIdx;
                textSz = name->fullName.snLen;
                break;
            case ASN_SERIAL_NUMBER:
                text = name->fullName.fullName + name->fullName.serialIdx;
                textSz = name->fullName.serialLen;
                break;
            case ASN_COUNTRY_NAME:
                text = name->fullName.fullName + name->fullName.cIdx;
                textSz = name->fullName.cLen;
                break;
            case ASN_LOCALITY_NAME:
                text = name->fullName.fullName + name->fullName.lIdx;
                textSz = name->fullName.lLen;
                break;
            case ASN_STATE_NAME:
                text = name->fullName.fullName + name->fullName.stIdx;
                textSz = name->fullName.stLen;
                break;
            case ASN_ORG_NAME:
                text = name->fullName.fullName + name->fullName.oIdx;
                textSz = name->fullName.oLen;
                break;
            case ASN_ORGUNIT_NAME:
                text = name->fullName.fullName + name->fullName.ouIdx;
                textSz = name->fullName.ouLen;
                break;
            default:
                break;
        }

        if (buf != NULL && text != NULL) {
            textSz = min(textSz, len);
            XMEMCPY(buf, text, textSz);
            buf[textSz] = '\0';
        }

        WOLFSSL_LEAVE("wolfSSL_X509_NAME_get_text_by_NID", textSz);
        return textSz;
    }

    int wolfSSL_X509_NAME_get_index_by_NID(WOLFSSL_X509_NAME* name,
                                          int nid, int pos)
    {
        int ret    = -1;

        WOLFSSL_ENTER("wolfSSL_X509_NAME_get_index_by_NID");

        if (name == NULL) {
            return BAD_FUNC_ARG;
        }

        /* these index values are already stored in DecodedName
           use those when available */
        if (name->fullName.fullName && name->fullName.fullNameLen > 0) {
            switch (nid) {
                case ASN_COMMON_NAME:
                    ret = name->fullName.cnIdx;
                    break;
                default:
                    WOLFSSL_MSG("NID not yet implemented");
                    break;
            }
        }

        WOLFSSL_LEAVE("wolfSSL_X509_NAME_get_index_by_NID", ret);

        (void)pos;
        (void)nid;

        return ret;
    }


    WOLFSSL_ASN1_STRING*  wolfSSL_X509_NAME_ENTRY_get_data(
                                                    WOLFSSL_X509_NAME_ENTRY* in)
    {
        WOLFSSL_ENTER("wolfSSL_X509_NAME_ENTRY_get_data");
        return in->value;
    }


    char* wolfSSL_ASN1_STRING_data(WOLFSSL_ASN1_STRING* asn)
    {
        WOLFSSL_ENTER("wolfSSL_ASN1_STRING_data");

        if (asn) {
            return asn->data;
        }
        else {
            return NULL;
        }
    }


    int wolfSSL_ASN1_STRING_length(WOLFSSL_ASN1_STRING* asn)
    {
        WOLFSSL_ENTER("wolfSSL_ASN1_STRING_length");

        if (asn) {
            return asn->length;
        }
        else {
            return 0;
        }
    }
#endif


    /* copy name into in buffer, at most sz bytes, if buffer is null will
       malloc buffer, call responsible for freeing                     */
    char* wolfSSL_X509_NAME_oneline(WOLFSSL_X509_NAME* name, char* in, int sz)
    {
        int copySz = min(sz, name->sz);

        WOLFSSL_ENTER("wolfSSL_X509_NAME_oneline");
        if (!name->sz) return in;

        if (!in) {
        #ifdef WOLFSSL_STATIC_MEMORY
            WOLFSSL_MSG("Using static memory -- please pass in a buffer");
            return NULL;
        #else
            in = (char*)XMALLOC(name->sz, NULL, DYNAMIC_TYPE_OPENSSL);
            if (!in ) return in;
            copySz = name->sz;
        #endif
        }

        if (copySz == 0)
            return in;

        XMEMCPY(in, name->name, copySz - 1);
        in[copySz - 1] = 0;

        return in;
    }


    int wolfSSL_X509_get_signature_type(WOLFSSL_X509* x509)
    {
        int type = 0;

        WOLFSSL_ENTER("wolfSSL_X509_get_signature_type");

        if (x509 != NULL)
            type = x509->sigOID;

        return type;
    }


    int wolfSSL_X509_get_signature(WOLFSSL_X509* x509,
                                                 unsigned char* buf, int* bufSz)
    {
        WOLFSSL_ENTER("wolfSSL_X509_get_signature");
        if (x509 == NULL || bufSz == NULL || *bufSz < (int)x509->sig.length)
            return SSL_FATAL_ERROR;

        if (buf != NULL)
            XMEMCPY(buf, x509->sig.buffer, x509->sig.length);
        *bufSz = x509->sig.length;

        return SSL_SUCCESS;
    }


    /* write X509 serial number in unsigned binary to buffer
       buffer needs to be at least EXTERNAL_SERIAL_SIZE (32) for all cases
       return SSL_SUCCESS on success */
    int wolfSSL_X509_get_serial_number(WOLFSSL_X509* x509,
                                       byte* in, int* inOutSz)
    {
        WOLFSSL_ENTER("wolfSSL_X509_get_serial_number");
        if (x509 == NULL || in == NULL ||
                                   inOutSz == NULL || *inOutSz < x509->serialSz)
            return BAD_FUNC_ARG;

        XMEMCPY(in, x509->serial, x509->serialSz);
        *inOutSz = x509->serialSz;

        return SSL_SUCCESS;
    }


    const byte* wolfSSL_X509_get_der(WOLFSSL_X509* x509, int* outSz)
    {
        WOLFSSL_ENTER("wolfSSL_X509_get_der");

        if (x509 == NULL || outSz == NULL)
            return NULL;

        *outSz = (int)x509->derCert->length;
        return x509->derCert->buffer;
    }


    int wolfSSL_X509_version(WOLFSSL_X509* x509)
    {
        WOLFSSL_ENTER("wolfSSL_X509_version");

        if (x509 == NULL)
            return 0;

        return x509->version;
    }


    const byte* wolfSSL_X509_notBefore(WOLFSSL_X509* x509)
    {
        WOLFSSL_ENTER("wolfSSL_X509_notBefore");

        if (x509 == NULL)
            return NULL;

        return x509->notBefore;
    }


    const byte* wolfSSL_X509_notAfter(WOLFSSL_X509* x509)
    {
        WOLFSSL_ENTER("wolfSSL_X509_notAfter");

        if (x509 == NULL)
            return NULL;

        return x509->notAfter;
    }


#ifdef WOLFSSL_SEP

/* copy oid into in buffer, at most *inOutSz bytes, if buffer is null will
   malloc buffer, call responsible for freeing. Actual size returned in
   *inOutSz. Requires inOutSz be non-null */
byte* wolfSSL_X509_get_device_type(WOLFSSL_X509* x509, byte* in, int *inOutSz)
{
    int copySz;

    WOLFSSL_ENTER("wolfSSL_X509_get_dev_type");
    if (inOutSz == NULL) return NULL;
    if (!x509->deviceTypeSz) return in;

    copySz = min(*inOutSz, x509->deviceTypeSz);

    if (!in) {
    #ifdef WOLFSSL_STATIC_MEMORY
        WOLFSSL_MSG("Using static memory -- please pass in a buffer");
        return NULL;
    #else
        in = (byte*)XMALLOC(x509->deviceTypeSz, 0, DYNAMIC_TYPE_OPENSSL);
        if (!in) return in;
        copySz = x509->deviceTypeSz;
    #endif
    }

    XMEMCPY(in, x509->deviceType, copySz);
    *inOutSz = copySz;

    return in;
}


byte* wolfSSL_X509_get_hw_type(WOLFSSL_X509* x509, byte* in, int* inOutSz)
{
    int copySz;

    WOLFSSL_ENTER("wolfSSL_X509_get_hw_type");
    if (inOutSz == NULL) return NULL;
    if (!x509->hwTypeSz) return in;

    copySz = min(*inOutSz, x509->hwTypeSz);

    if (!in) {
    #ifdef WOLFSSL_STATIC_MEMORY
        WOLFSSL_MSG("Using static memory -- please pass in a buffer");
        return NULL;
    #else
        in = (byte*)XMALLOC(x509->hwTypeSz, 0, DYNAMIC_TYPE_OPENSSL);
        if (!in) return in;
        copySz = x509->hwTypeSz;
    #endif
    }

    XMEMCPY(in, x509->hwType, copySz);
    *inOutSz = copySz;

    return in;
}


byte* wolfSSL_X509_get_hw_serial_number(WOLFSSL_X509* x509,byte* in,
                                        int* inOutSz)
{
    int copySz;

    WOLFSSL_ENTER("wolfSSL_X509_get_hw_serial_number");
    if (inOutSz == NULL) return NULL;
    if (!x509->hwTypeSz) return in;

    copySz = min(*inOutSz, x509->hwSerialNumSz);

    if (!in) {
    #ifdef WOLFSSL_STATIC_MEMORY
        WOLFSSL_MSG("Using static memory -- please pass in a buffer");
        return NULL;
    #else
        in = (byte*)XMALLOC(x509->hwSerialNumSz, 0, DYNAMIC_TYPE_OPENSSL);
        if (!in) return in;
        copySz = x509->hwSerialNumSz;
    #endif
    }

    XMEMCPY(in, x509->hwSerialNum, copySz);
    *inOutSz = copySz;

    return in;
}

#endif /* WOLFSSL_SEP */


WOLFSSL_X509* wolfSSL_X509_d2i(WOLFSSL_X509** x509, const byte* in, int len)
{
    WOLFSSL_X509 *newX509 = NULL;

    WOLFSSL_ENTER("wolfSSL_X509_d2i");

    if (in != NULL && len != 0) {
    #ifdef WOLFSSL_SMALL_STACK
        DecodedCert* cert = NULL;
    #else
        DecodedCert  cert[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), NULL,
                                     DYNAMIC_TYPE_TMP_BUFFER);
        if (cert == NULL)
            return NULL;
    #endif

        InitDecodedCert(cert, (byte*)in, len, NULL);
        if (ParseCertRelative(cert, CERT_TYPE, 0, NULL) == 0) {
            newX509 = (WOLFSSL_X509*)XMALLOC(sizeof(WOLFSSL_X509), NULL,
                                             DYNAMIC_TYPE_X509);
            if (newX509 != NULL) {
                InitX509(newX509, 1, NULL);
                if (CopyDecodedToX509(newX509, cert) != 0) {
                    XFREE(newX509, NULL, DYNAMIC_TYPE_X509);
                    newX509 = NULL;
                }
            }
        }
        FreeDecodedCert(cert);
    #ifdef WOLFSSL_SMALL_STACK
        XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
    }

    if (x509 != NULL)
        *x509 = newX509;

    return newX509;
}


#ifndef NO_FILESYSTEM

#ifndef NO_STDIO_FILESYSTEM

WOLFSSL_X509* wolfSSL_X509_d2i_fp(WOLFSSL_X509** x509, XFILE file)
{
    WOLFSSL_X509* newX509 = NULL;

    WOLFSSL_ENTER("wolfSSL_X509_d2i_fp");

    if (file != XBADFILE) {
        byte* fileBuffer = NULL;
        long sz = 0;

        XFSEEK(file, 0, XSEEK_END);
        sz = XFTELL(file);
        XREWIND(file);

        if (sz < 0) {
            WOLFSSL_MSG("Bad tell on FILE");
            return NULL;
        }

        fileBuffer = (byte*)XMALLOC(sz, NULL, DYNAMIC_TYPE_FILE);
        if (fileBuffer != NULL) {
            int ret = (int)XFREAD(fileBuffer, sz, 1, file);
            if (ret > 0) {
                newX509 = wolfSSL_X509_d2i(NULL, fileBuffer, (int)sz);
            }
            XFREE(fileBuffer, NULL, DYNAMIC_TYPE_FILE);
        }
    }

    if (x509 != NULL)
        *x509 = newX509;

    return newX509;
}

#endif /* NO_STDIO_FILESYSTEM */

WOLFSSL_X509* wolfSSL_X509_load_certificate_file(const char* fname, int format)
{
#ifdef WOLFSSL_SMALL_STACK
    byte  staticBuffer[1]; /* force heap usage */
#else
    byte  staticBuffer[FILE_BUFFER_SIZE];
#endif
    byte* fileBuffer = staticBuffer;
    int   dynamic = 0;
    int   ret;
    long  sz = 0;
    XFILE file;

    WOLFSSL_X509* x509 = NULL;
    DerBuffer* der = NULL;

    WOLFSSL_ENTER("wolfSSL_X509_load_certificate");

    /* Check the inputs */
    if ((fname == NULL) ||
        (format != SSL_FILETYPE_ASN1 && format != SSL_FILETYPE_PEM))
        return NULL;

    file = XFOPEN(fname, "rb");
    if (file == XBADFILE)
        return NULL;

    XFSEEK(file, 0, XSEEK_END);
    sz = XFTELL(file);
    XREWIND(file);

    if (sz > (long)sizeof(staticBuffer)) {
        fileBuffer = (byte*)XMALLOC(sz, NULL, DYNAMIC_TYPE_FILE);
        if (fileBuffer == NULL) {
            XFCLOSE(file);
            return NULL;
        }
        dynamic = 1;
    }
    else if (sz < 0) {
        XFCLOSE(file);
        return NULL;
    }

    ret = (int)XFREAD(fileBuffer, sz, 1, file);
    if (ret < 0) {
        XFCLOSE(file);
        if (dynamic)
            XFREE(fileBuffer, NULL, DYNAMIC_TYPE_FILE);
        return NULL;
    }

    XFCLOSE(file);

    if (format == SSL_FILETYPE_PEM) {
        int ecc = 0;
    #ifdef WOLFSSL_SMALL_STACK
        EncryptedInfo* info = NULL;
    #else
        EncryptedInfo  info[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), NULL,
                                                      DYNAMIC_TYPE_TMP_BUFFER);
        if (info == NULL) {
            if (dynamic)
                XFREE(fileBuffer, NULL, DYNAMIC_TYPE_FILE);

            return NULL;
        }
    #endif

        info->set = 0;
        info->ctx = NULL;
        info->consumed = 0;

        if (PemToDer(fileBuffer, sz, CERT_TYPE, &der, NULL, info, &ecc) != 0) {
            FreeDer(&der);
        }

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
    }
    else {
        ret = AllocDer(&der, (word32)sz, CERT_TYPE, NULL);
        if (ret == 0) {
            XMEMCPY(der->buffer, fileBuffer, sz);
        }
    }

    if (dynamic)
        XFREE(fileBuffer, NULL, DYNAMIC_TYPE_FILE);

    /* At this point we want `der` to have the certificate in DER format */
    /* ready to be decoded. */
    if (der != NULL && der->buffer != NULL) {
    #ifdef WOLFSSL_SMALL_STACK
        DecodedCert* cert = NULL;
    #else
        DecodedCert  cert[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), NULL,
                                                       DYNAMIC_TYPE_TMP_BUFFER);
        if (cert != NULL)
    #endif
        {
            InitDecodedCert(cert, der->buffer, der->length, NULL);
            if (ParseCertRelative(cert, CERT_TYPE, 0, NULL) == 0) {
                x509 = (WOLFSSL_X509*)XMALLOC(sizeof(WOLFSSL_X509), NULL,
                                                             DYNAMIC_TYPE_X509);
                if (x509 != NULL) {
                    InitX509(x509, 1, NULL);
                    if (CopyDecodedToX509(x509, cert) != 0) {
                        XFREE(x509, NULL, DYNAMIC_TYPE_X509);
                        x509 = NULL;
                    }
                }
            }

            FreeDecodedCert(cert);
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
        }

        FreeDer(&der);
    }

    return x509;
}

#endif /* NO_FILESYSTEM */

#endif /* KEEP_PEER_CERT || SESSION_CERTS */

/* OPENSSL_EXTRA is needed for wolfSSL_X509_d21 function
   KEEP_OUR_CERT is to insure ability for returning ssl certificate */
#if defined(OPENSSL_EXTRA) && defined(KEEP_OUR_CERT)
WOLFSSL_X509* wolfSSL_get_certificate(WOLFSSL* ssl)
{
    if (ssl == NULL) {
        return NULL;
    }

    if (ssl->buffers.weOwnCert) {
        if (ssl->ourCert == NULL) {
            ssl->ourCert = wolfSSL_X509_d2i(NULL,
                                              ssl->buffers.certificate->buffer,
                                              ssl->buffers.certificate->length);
        }
        return ssl->ourCert;
    }
    else { /* if cert not owned get parent ctx cert or return null */
        if (ssl->ctx) {
            if (ssl->ctx->ourCert == NULL) {
                ssl->ctx->ourCert = wolfSSL_X509_d2i(NULL,
                                               ssl->ctx->certificate->buffer,
                                               ssl->ctx->certificate->length);
            }
            return ssl->ctx->ourCert;
        }
        else {
            return NULL;
        }
    }
}
#endif /* OPENSSL_EXTRA && KEEP_OUR_CERT */
#endif /* NO_CERTS */


#ifdef OPENSSL_EXTRA
int wolfSSL_set_session_id_context(WOLFSSL* ssl, const unsigned char* id,
                               unsigned int len)
{
    (void)ssl;
    (void)id;
    (void)len;
    return 0;
}


void wolfSSL_set_connect_state(WOLFSSL* ssl)
{
    (void)ssl;
    /* client by default */
}
#endif

int wolfSSL_get_shutdown(const WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_get_shutdown");
    /* in OpenSSL, SSL_SENT_SHUTDOWN = 1, when closeNotifySent   *
     * SSL_RECEIVED_SHUTDOWN = 2, from close notify or fatal err */
    return ((ssl->options.closeNotify||ssl->options.connReset) << 1)
            | (ssl->options.sentNotify);
}


int wolfSSL_session_reused(WOLFSSL* ssl)
{
    return ssl->options.resuming;
}

#ifdef OPENSSL_EXTRA
void wolfSSL_SESSION_free(WOLFSSL_SESSION* session)
{
    /* No need to free since cache is static */
    (void)session;
}
#endif

const char* wolfSSL_get_version(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_get_version");
    if (ssl->version.major == SSLv3_MAJOR) {
        switch (ssl->version.minor) {
            case SSLv3_MINOR :
                return "SSLv3";
            case TLSv1_MINOR :
                return "TLSv1";
            case TLSv1_1_MINOR :
                return "TLSv1.1";
            case TLSv1_2_MINOR :
                return "TLSv1.2";
            default:
                return "unknown";
        }
    }
    else if (ssl->version.major == DTLS_MAJOR) {
        switch (ssl->version.minor) {
            case DTLS_MINOR :
                return "DTLS";
            case DTLSv1_2_MINOR :
                return "DTLSv1.2";
            default:
                return "unknown";
        }
    }
    return "unknown";
}


/* current library version */
const char* wolfSSL_lib_version(void)
{
    return LIBWOLFSSL_VERSION_STRING;
}


/* current library version in hex */
word32 wolfSSL_lib_version_hex(void)
{
    return LIBWOLFSSL_VERSION_HEX;
}


int wolfSSL_get_current_cipher_suite(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_get_current_cipher_suite");
    if (ssl)
        return (ssl->options.cipherSuite0 << 8) | ssl->options.cipherSuite;
    return 0;
}

WOLFSSL_CIPHER* wolfSSL_get_current_cipher(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("SSL_get_current_cipher");
    if (ssl)
        return &ssl->cipher;
    else
        return NULL;
}


const char* wolfSSL_CIPHER_get_name(const WOLFSSL_CIPHER* cipher)
{
    (void)cipher;

    WOLFSSL_ENTER("SSL_CIPHER_get_name");
#ifndef NO_ERROR_STRINGS
    if (cipher) {
#if defined(HAVE_CHACHA)
        if (cipher->ssl->options.cipherSuite0 == CHACHA_BYTE) {
        /* ChaCha suites */
        switch (cipher->ssl->options.cipherSuite) {
#ifdef HAVE_POLY1305
#ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256 :
                return "TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256";

            case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256 :
                return "TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256";

            case TLS_ECDHE_RSA_WITH_CHACHA20_OLD_POLY1305_SHA256 :
                return "TLS_ECDHE_RSA_WITH_CHACHA20_OLD_POLY1305_SHA256";

            case TLS_DHE_RSA_WITH_CHACHA20_OLD_POLY1305_SHA256 :
                return "TLS_DHE_RSA_WITH_CHACHA20_OLD_POLY1305_SHA256";
#endif
            case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 :
                return "TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256";

            case TLS_ECDHE_ECDSA_WITH_CHACHA20_OLD_POLY1305_SHA256 :
                return "TLS_ECDHE_ECDSA_WITH_CHACHA20_OLD_POLY1305_SHA256";
#ifndef NO_PSK
            case TLS_ECDHE_PSK_WITH_CHACHA20_POLY1305_SHA256 :
                return "TLS_ECDHE_PSK_WITH_CHACHA20_POLY1305_SHA256";
            case TLS_PSK_WITH_CHACHA20_POLY1305_SHA256 :
                return "TLS_PSK_WITH_CHACHA20_POLY1305_SHA256";
            case TLS_DHE_PSK_WITH_CHACHA20_POLY1305_SHA256 :
                return "TLS_DHE_PSK_WITH_CHACHA20_POLY1305_SHA256";
#endif /* NO_PSK */
#endif /* HAVE_POLY1305 */
            }
        }
#endif

#if defined(HAVE_ECC) || defined(HAVE_AESCCM)
        /* Awkwardly, the ECC cipher suites use the ECC_BYTE as expected,
         * but the AES-CCM cipher suites also use it, even the ones that
         * aren't ECC. */
        if (cipher->ssl->options.cipherSuite0 == ECC_BYTE) {
        /* ECC suites */
        switch (cipher->ssl->options.cipherSuite) {
#ifdef HAVE_ECC
#ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 :
                return "TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256";
#endif
            case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 :
                return "TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256";
#ifndef NO_RSA
            case TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256 :
                return "TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256";
#endif
            case TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256 :
                return "TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256";
#ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 :
                return "TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384";
#endif
            case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 :
                return "TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384";
#ifndef NO_RSA
            case TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384 :
                return "TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384";
#endif
            case TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384 :
                return "TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384";
#ifndef NO_SHA
#ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA :
                return "TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA";
            case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA :
                return "TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA";
#endif
            case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA :
                return "TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA";
            case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA :
                return "TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA";
#ifndef NO_RC4
    #ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_RC4_128_SHA :
                return "TLS_ECDHE_RSA_WITH_RC4_128_SHA";
    #endif
            case TLS_ECDHE_ECDSA_WITH_RC4_128_SHA :
                return "TLS_ECDHE_ECDSA_WITH_RC4_128_SHA";
#endif
#ifndef NO_DES3
    #ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA :
                return "TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA";
    #endif
            case TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA :
                return "TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA";
#endif

#ifndef NO_RSA
            case TLS_ECDH_RSA_WITH_AES_128_CBC_SHA :
                return "TLS_ECDH_RSA_WITH_AES_128_CBC_SHA";
            case TLS_ECDH_RSA_WITH_AES_256_CBC_SHA :
                return "TLS_ECDH_RSA_WITH_AES_256_CBC_SHA";
#endif
            case TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA :
                return "TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA";
            case TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA :
                return "TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA";
#ifndef NO_RC4
    #ifndef NO_RSA
            case TLS_ECDH_RSA_WITH_RC4_128_SHA :
                return "TLS_ECDH_RSA_WITH_RC4_128_SHA";
    #endif
            case TLS_ECDH_ECDSA_WITH_RC4_128_SHA :
                return "TLS_ECDH_ECDSA_WITH_RC4_128_SHA";
#endif
#ifndef NO_DES3
    #ifndef NO_RSA
            case TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA :
                return "TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA";
    #endif
            case TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA :
                return "TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA";
#endif
#endif /* NO_SHA */

#ifdef HAVE_AESGCM
#ifndef NO_RSA
            case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 :
                return "TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256";
            case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 :
                return "TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384";
#endif
            case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 :
                return "TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256";
            case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 :
                return "TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384";
#ifndef NO_RSA
            case TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256 :
                return "TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256";
            case TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384 :
                return "TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384";
#endif
            case TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256 :
                return "TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256";
            case TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384 :
                return "TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384";
#endif
            case TLS_ECDHE_ECDSA_WITH_NULL_SHA :
                return "TLS_ECDHE_ECDSA_WITH_NULL_SHA";
#ifndef NO_PSK
            case TLS_ECDHE_PSK_WITH_NULL_SHA256 :
                return "TLS_ECDHE_PSK_WITH_NULL_SHA256";
            case TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256 :
                return "TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256";
#endif
#endif /* HAVE_ECC */

#ifdef HAVE_AESCCM
#ifndef NO_RSA
            case TLS_RSA_WITH_AES_128_CCM_8 :
                return "TLS_RSA_WITH_AES_128_CCM_8";
            case TLS_RSA_WITH_AES_256_CCM_8 :
                return "TLS_RSA_WITH_AES_256_CCM_8";
#endif
#ifndef NO_PSK
            case TLS_PSK_WITH_AES_128_CCM_8 :
                return "TLS_PSK_WITH_AES_128_CCM_8";
            case TLS_PSK_WITH_AES_256_CCM_8 :
                return "TLS_PSK_WITH_AES_256_CCM_8";
            case TLS_PSK_WITH_AES_128_CCM :
                return "TLS_PSK_WITH_AES_128_CCM";
            case TLS_PSK_WITH_AES_256_CCM :
                return "TLS_PSK_WITH_AES_256_CCM";
            case TLS_DHE_PSK_WITH_AES_128_CCM :
                return "TLS_DHE_PSK_WITH_AES_128_CCM";
            case TLS_DHE_PSK_WITH_AES_256_CCM :
                return "TLS_DHE_PSK_WITH_AES_256_CCM";
#endif
#ifdef HAVE_ECC
            case TLS_ECDHE_ECDSA_WITH_AES_128_CCM:
                return "TLS_ECDHE_ECDSA_WITH_AES_128_CCM";
            case TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8:
                return "TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8";
            case TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8 :
                return "TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8";
#endif
#endif

            default:
                return "NONE";
        }
        }
#endif  /* ECC */
        if (cipher->ssl->options.cipherSuite0 != ECC_BYTE &&
            cipher->ssl->options.cipherSuite0 != CHACHA_BYTE) {

            /* normal suites */
        switch (cipher->ssl->options.cipherSuite) {
#ifndef NO_RSA
#ifndef NO_RC4
    #ifndef NO_SHA
            case SSL_RSA_WITH_RC4_128_SHA :
                return "SSL_RSA_WITH_RC4_128_SHA";
    #endif
    #ifndef NO_MD5
            case SSL_RSA_WITH_RC4_128_MD5 :
                return "SSL_RSA_WITH_RC4_128_MD5";
    #endif
#endif
#ifndef NO_SHA
    #ifndef NO_DES3
            case SSL_RSA_WITH_3DES_EDE_CBC_SHA :
                return "SSL_RSA_WITH_3DES_EDE_CBC_SHA";
    #endif
    #ifdef HAVE_IDEA
            case SSL_RSA_WITH_IDEA_CBC_SHA :
                return "SSL_RSA_WITH_IDEA_CBC_SHA";
    #endif

            case TLS_RSA_WITH_AES_128_CBC_SHA :
                return "TLS_RSA_WITH_AES_128_CBC_SHA";
            case TLS_RSA_WITH_AES_256_CBC_SHA :
                return "TLS_RSA_WITH_AES_256_CBC_SHA";
#endif
            case TLS_RSA_WITH_AES_128_CBC_SHA256 :
                return "TLS_RSA_WITH_AES_128_CBC_SHA256";
            case TLS_RSA_WITH_AES_256_CBC_SHA256 :
                return "TLS_RSA_WITH_AES_256_CBC_SHA256";
    #ifdef HAVE_BLAKE2
            case TLS_RSA_WITH_AES_128_CBC_B2B256:
                return "TLS_RSA_WITH_AES_128_CBC_B2B256";
            case TLS_RSA_WITH_AES_256_CBC_B2B256:
                return "TLS_RSA_WITH_AES_256_CBC_B2B256";
    #endif
#ifndef NO_SHA
            case TLS_RSA_WITH_NULL_SHA :
                return "TLS_RSA_WITH_NULL_SHA";
#endif
            case TLS_RSA_WITH_NULL_SHA256 :
                return "TLS_RSA_WITH_NULL_SHA256";
#endif /* NO_RSA */
#ifndef NO_PSK
#ifndef NO_SHA
            case TLS_PSK_WITH_AES_128_CBC_SHA :
                return "TLS_PSK_WITH_AES_128_CBC_SHA";
            case TLS_PSK_WITH_AES_256_CBC_SHA :
                return "TLS_PSK_WITH_AES_256_CBC_SHA";
#endif
#ifndef NO_SHA256
            case TLS_PSK_WITH_AES_128_CBC_SHA256 :
                return "TLS_PSK_WITH_AES_128_CBC_SHA256";
            case TLS_PSK_WITH_NULL_SHA256 :
                return "TLS_PSK_WITH_NULL_SHA256";
            case TLS_DHE_PSK_WITH_AES_128_CBC_SHA256 :
                return "TLS_DHE_PSK_WITH_AES_128_CBC_SHA256";
            case TLS_DHE_PSK_WITH_NULL_SHA256 :
                return "TLS_DHE_PSK_WITH_NULL_SHA256";
    #ifdef HAVE_AESGCM
            case TLS_PSK_WITH_AES_128_GCM_SHA256 :
                return "TLS_PSK_WITH_AES_128_GCM_SHA256";
            case TLS_DHE_PSK_WITH_AES_128_GCM_SHA256 :
                return "TLS_DHE_PSK_WITH_AES_128_GCM_SHA256";
    #endif
#endif
#ifdef WOLFSSL_SHA384
            case TLS_PSK_WITH_AES_256_CBC_SHA384 :
                return "TLS_PSK_WITH_AES_256_CBC_SHA384";
            case TLS_PSK_WITH_NULL_SHA384 :
                return "TLS_PSK_WITH_NULL_SHA384";
            case TLS_DHE_PSK_WITH_AES_256_CBC_SHA384 :
                return "TLS_DHE_PSK_WITH_AES_256_CBC_SHA384";
            case TLS_DHE_PSK_WITH_NULL_SHA384 :
                return "TLS_DHE_PSK_WITH_NULL_SHA384";
    #ifdef HAVE_AESGCM
            case TLS_PSK_WITH_AES_256_GCM_SHA384 :
                return "TLS_PSK_WITH_AES_256_GCM_SHA384";
            case TLS_DHE_PSK_WITH_AES_256_GCM_SHA384 :
                return "TLS_DHE_PSK_WITH_AES_256_GCM_SHA384";
    #endif
#endif
#ifndef NO_SHA
            case TLS_PSK_WITH_NULL_SHA :
                return "TLS_PSK_WITH_NULL_SHA";
#endif
#endif /* NO_PSK */
#ifndef NO_RSA
            case TLS_DHE_RSA_WITH_AES_128_CBC_SHA256 :
                return "TLS_DHE_RSA_WITH_AES_128_CBC_SHA256";
            case TLS_DHE_RSA_WITH_AES_256_CBC_SHA256 :
                return "TLS_DHE_RSA_WITH_AES_256_CBC_SHA256";
#ifndef NO_SHA
            case TLS_DHE_RSA_WITH_AES_128_CBC_SHA :
                return "TLS_DHE_RSA_WITH_AES_128_CBC_SHA";
            case TLS_DHE_RSA_WITH_AES_256_CBC_SHA :
                return "TLS_DHE_RSA_WITH_AES_256_CBC_SHA";
        #ifndef NO_DES3
            case TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA:
                return "TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA";
         #endif
#endif
#ifndef NO_HC128
    #ifndef NO_MD5
            case TLS_RSA_WITH_HC_128_MD5 :
                return "TLS_RSA_WITH_HC_128_MD5";
    #endif
    #ifndef NO_SHA
            case TLS_RSA_WITH_HC_128_SHA :
                return "TLS_RSA_WITH_HC_128_SHA";
    #endif
    #ifdef HAVE_BLAKE2
            case TLS_RSA_WITH_HC_128_B2B256:
                return "TLS_RSA_WITH_HC_128_B2B256";
    #endif
#endif /* NO_HC128 */
#ifndef NO_SHA
    #ifndef NO_RABBIT
            case TLS_RSA_WITH_RABBIT_SHA :
                return "TLS_RSA_WITH_RABBIT_SHA";
    #endif
    #ifdef HAVE_NTRU
        #ifndef NO_RC4
            case TLS_NTRU_RSA_WITH_RC4_128_SHA :
                return "TLS_NTRU_RSA_WITH_RC4_128_SHA";
        #endif
        #ifndef NO_DES3
            case TLS_NTRU_RSA_WITH_3DES_EDE_CBC_SHA :
                return "TLS_NTRU_RSA_WITH_3DES_EDE_CBC_SHA";
        #endif
            case TLS_NTRU_RSA_WITH_AES_128_CBC_SHA :
                return "TLS_NTRU_RSA_WITH_AES_128_CBC_SHA";
            case TLS_NTRU_RSA_WITH_AES_256_CBC_SHA :
                return "TLS_NTRU_RSA_WITH_AES_256_CBC_SHA";
    #endif /* HAVE_NTRU */
    #ifdef HAVE_QSH
            case TLS_QSH :
                return "TLS_QSH";
    #endif /* HAVE_QSH*/
#endif /* NO_SHA */
            case TLS_RSA_WITH_AES_128_GCM_SHA256 :
                return "TLS_RSA_WITH_AES_128_GCM_SHA256";
            case TLS_RSA_WITH_AES_256_GCM_SHA384 :
                return "TLS_RSA_WITH_AES_256_GCM_SHA384";
            case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256 :
                return "TLS_DHE_RSA_WITH_AES_128_GCM_SHA256";
            case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384 :
                return "TLS_DHE_RSA_WITH_AES_256_GCM_SHA384";
#ifndef NO_SHA
            case TLS_RSA_WITH_CAMELLIA_128_CBC_SHA :
                return "TLS_RSA_WITH_CAMELLIA_128_CBC_SHA";
            case TLS_RSA_WITH_CAMELLIA_256_CBC_SHA :
                return "TLS_RSA_WITH_CAMELLIA_256_CBC_SHA";
#endif
            case TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256 :
                return "TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256";
            case TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256 :
                return "TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256";
#ifndef NO_SHA
            case TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA :
                return "TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA";
            case TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA :
                return "TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA";
#endif
            case TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256 :
                return "TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256";
            case TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256 :
                return "TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256";
#endif /* NO_RSA */
#ifdef BUILD_TLS_DH_anon_WITH_AES_128_CBC_SHA
            case TLS_DH_anon_WITH_AES_128_CBC_SHA :
                return "TLS_DH_anon_WITH_AES_128_CBC_SHA";
#endif
            default:
                return "NONE";
        }  /* switch */
        }  /* normal / ECC */
    }
#endif /* NO_ERROR_STRINGS */
    return "NONE";
}


const char* wolfSSL_get_cipher(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_get_cipher");
    return wolfSSL_CIPHER_get_name(wolfSSL_get_current_cipher(ssl));
}

/* gets cipher name in the format DHE-RSA-... rather then TLS_DHE... */
const char* wolfSSL_get_cipher_name(WOLFSSL* ssl)
{
    /* get access to cipher_name_idx in internal.c */
    return wolfSSL_get_cipher_name_internal(ssl);
}
#ifdef OPENSSL_EXTRA



char* wolfSSL_CIPHER_description(WOLFSSL_CIPHER* cipher, char* in, int len)
{
    (void)cipher;
    (void)in;
    (void)len;
    return 0;
}


#ifndef NO_SESSION_CACHE

WOLFSSL_SESSION* wolfSSL_get1_session(WOLFSSL* ssl)
{
    /* sessions are stored statically, no need for reference count */
    return wolfSSL_get_session(ssl);
}

#endif /* NO_SESSION_CACHE */

#ifndef NO_CERTS
void wolfSSL_X509_free(WOLFSSL_X509* x509)
{
    WOLFSSL_ENTER("wolfSSL_X509_free");
    ExternalFreeX509(x509);
}
#endif /* NO_CERTS */


/* was do nothing */
/*
void OPENSSL_free(void* buf)
{
    (void)buf;
}
*/


int wolfSSL_OCSP_parse_url(char* url, char** host, char** port, char** path,
                   int* ssl)
{
    (void)url;
    (void)host;
    (void)port;
    (void)path;
    (void)ssl;
    return 0;
}


WOLFSSL_METHOD* wolfSSLv2_client_method(void)
{
    return 0;
}


WOLFSSL_METHOD* wolfSSLv2_server_method(void)
{
    return 0;
}


#ifndef NO_MD4

void wolfSSL_MD4_Init(WOLFSSL_MD4_CTX* md4)
{
    /* make sure we have a big enough buffer */
    typedef char ok[sizeof(md4->buffer) >= sizeof(Md4) ? 1 : -1];
    (void) sizeof(ok);

    WOLFSSL_ENTER("MD4_Init");
    wc_InitMd4((Md4*)md4);
}


void wolfSSL_MD4_Update(WOLFSSL_MD4_CTX* md4, const void* data,
                       unsigned long len)
{
    WOLFSSL_ENTER("MD4_Update");
    wc_Md4Update((Md4*)md4, (const byte*)data, (word32)len);
}


void wolfSSL_MD4_Final(unsigned char* digest, WOLFSSL_MD4_CTX* md4)
{
    WOLFSSL_ENTER("MD4_Final");
    wc_Md4Final((Md4*)md4, digest);
}

#endif /* NO_MD4 */


WOLFSSL_BIO* wolfSSL_BIO_pop(WOLFSSL_BIO* top)
{
    (void)top;
    return 0;
}


int wolfSSL_BIO_pending(WOLFSSL_BIO* bio)
{
    (void)bio;
    return 0;
}



WOLFSSL_BIO_METHOD* wolfSSL_BIO_s_mem(void)
{
    static WOLFSSL_BIO_METHOD meth;

    WOLFSSL_ENTER("BIO_s_mem");
    meth.type = BIO_MEMORY;

    return &meth;
}


WOLFSSL_BIO_METHOD* wolfSSL_BIO_f_base64(void)
{
    return 0;
}


void wolfSSL_BIO_set_flags(WOLFSSL_BIO* bio, int flags)
{
    (void)bio;
    (void)flags;
}



void wolfSSL_RAND_screen(void)
{

}


const char* wolfSSL_RAND_file_name(char* fname, unsigned long len)
{
    (void)fname;
    (void)len;
    return 0;
}


int wolfSSL_RAND_write_file(const char* fname)
{
    (void)fname;
    return 0;
}


int wolfSSL_RAND_load_file(const char* fname, long len)
{
    (void)fname;
    /* wolfCrypt provides enough entropy internally or will report error */
    if (len == -1)
        return 1024;
    else
        return (int)len;
}


int wolfSSL_RAND_egd(const char* path)
{
    (void)path;
    return 0;
}



WOLFSSL_COMP_METHOD* wolfSSL_COMP_zlib(void)
{
    return 0;
}


WOLFSSL_COMP_METHOD* wolfSSL_COMP_rle(void)
{
    return 0;
}


int wolfSSL_COMP_add_compression_method(int method, void* data)
{
    (void)method;
    (void)data;
    return 0;
}


void wolfSSL_set_dynlock_create_callback(WOLFSSL_dynlock_value* (*f)(
                                                          const char*, int))
{
    (void)f;
}


void wolfSSL_set_dynlock_lock_callback(
             void (*f)(int, WOLFSSL_dynlock_value*, const char*, int))
{
    (void)f;
}


void wolfSSL_set_dynlock_destroy_callback(
                  void (*f)(WOLFSSL_dynlock_value*, const char*, int))
{
    (void)f;
}



const char* wolfSSL_X509_verify_cert_error_string(long err)
{
    return wolfSSL_ERR_reason_error_string(err);
}



int wolfSSL_X509_LOOKUP_add_dir(WOLFSSL_X509_LOOKUP* lookup, const char* dir,
                               long len)
{
    (void)lookup;
    (void)dir;
    (void)len;
    return 0;
}


int wolfSSL_X509_LOOKUP_load_file(WOLFSSL_X509_LOOKUP* lookup,
                                 const char* file, long len)
{
    (void)lookup;
    (void)file;
    (void)len;
    return 0;
}


WOLFSSL_X509_LOOKUP_METHOD* wolfSSL_X509_LOOKUP_hash_dir(void)
{
    return 0;
}


WOLFSSL_X509_LOOKUP_METHOD* wolfSSL_X509_LOOKUP_file(void)
{
    return 0;
}



WOLFSSL_X509_LOOKUP* wolfSSL_X509_STORE_add_lookup(WOLFSSL_X509_STORE* store,
                                               WOLFSSL_X509_LOOKUP_METHOD* m)
{
    (void)store;
    (void)m;
    return 0;
}


#ifndef NO_CERTS
int wolfSSL_X509_STORE_add_cert(WOLFSSL_X509_STORE* store, WOLFSSL_X509* x509)
{
    int result = SSL_FATAL_ERROR;

    WOLFSSL_ENTER("wolfSSL_X509_STORE_add_cert");
    if (store != NULL && store->cm != NULL && x509 != NULL
                                                && x509->derCert != NULL) {
        DerBuffer* derCert = NULL;

        result = AllocDer(&derCert, x509->derCert->length,
            x509->derCert->type, NULL);
        if (result == 0) {
            /* AddCA() frees the buffer. */
            XMEMCPY(derCert->buffer,
                            x509->derCert->buffer, x509->derCert->length);
            result = AddCA(store->cm, &derCert, WOLFSSL_USER_CA, 1);
        }
    }

    WOLFSSL_LEAVE("wolfSSL_X509_STORE_add_cert", result);

    if (result != SSL_SUCCESS) {
        result = SSL_FATAL_ERROR;
    }

    return result;
}


WOLFSSL_X509_STORE* wolfSSL_X509_STORE_new(void)
{
    WOLFSSL_X509_STORE* store = NULL;

    store = (WOLFSSL_X509_STORE*)XMALLOC(sizeof(WOLFSSL_X509_STORE), NULL,
                                         DYNAMIC_TYPE_X509_STORE);
    if (store != NULL) {
        store->cm = wolfSSL_CertManagerNew();
        if (store->cm == NULL) {
            XFREE(store, NULL, DYNAMIC_TYPE_X509_STORE);
            store = NULL;
        }
    }

    return store;
}


void wolfSSL_X509_STORE_free(WOLFSSL_X509_STORE* store)
{
    if (store != NULL) {
        if (store->cm != NULL)
        wolfSSL_CertManagerFree(store->cm);
        XFREE(store, NULL, DYNAMIC_TYPE_X509_STORE);
    }
}


int wolfSSL_X509_STORE_set_default_paths(WOLFSSL_X509_STORE* store)
{
    (void)store;
    return SSL_SUCCESS;
}


int wolfSSL_X509_STORE_get_by_subject(WOLFSSL_X509_STORE_CTX* ctx, int idx,
                            WOLFSSL_X509_NAME* name, WOLFSSL_X509_OBJECT* obj)
{
    (void)ctx;
    (void)idx;
    (void)name;
    (void)obj;
    return 0;
}


WOLFSSL_X509_STORE_CTX* wolfSSL_X509_STORE_CTX_new(void)
{
    WOLFSSL_X509_STORE_CTX* ctx = (WOLFSSL_X509_STORE_CTX*)XMALLOC(
                                    sizeof(WOLFSSL_X509_STORE_CTX), NULL,
                                    DYNAMIC_TYPE_X509_CTX);
    if (ctx != NULL)
        wolfSSL_X509_STORE_CTX_init(ctx, NULL, NULL, NULL);

    return ctx;
}


int wolfSSL_X509_STORE_CTX_init(WOLFSSL_X509_STORE_CTX* ctx,
     WOLFSSL_X509_STORE* store, WOLFSSL_X509* x509, STACK_OF(WOLFSSL_X509)* sk)
{
    (void)sk;
    WOLFSSL_ENTER("wolfSSL_X509_STORE_CTX_init");
    if (ctx != NULL) {
        ctx->store = store;
        ctx->current_cert = x509;
        ctx->domain = NULL;
        ctx->ex_data = NULL;
        ctx->userCtx = NULL;
        ctx->error = 0;
        ctx->error_depth = 0;
        ctx->discardSessionCerts = 0;
        return SSL_SUCCESS;
    }
    return SSL_FATAL_ERROR;
}


void wolfSSL_X509_STORE_CTX_free(WOLFSSL_X509_STORE_CTX* ctx)
{
    if (ctx != NULL) {
        if (ctx->store != NULL)
            wolfSSL_X509_STORE_free(ctx->store);
        if (ctx->current_cert != NULL)
            wolfSSL_FreeX509(ctx->current_cert);
        XFREE(ctx, NULL, DYNAMIC_TYPE_X509_CTX);
    }
}


void wolfSSL_X509_STORE_CTX_cleanup(WOLFSSL_X509_STORE_CTX* ctx)
{
    (void)ctx;
}


int wolfSSL_X509_verify_cert(WOLFSSL_X509_STORE_CTX* ctx)
{
    if (ctx != NULL && ctx->store != NULL && ctx->store->cm != NULL
         && ctx->current_cert != NULL && ctx->current_cert->derCert != NULL) {
        return wolfSSL_CertManagerVerifyBuffer(ctx->store->cm,
                    ctx->current_cert->derCert->buffer,
                    ctx->current_cert->derCert->length,
                    SSL_FILETYPE_ASN1);
    }
    return SSL_FATAL_ERROR;
}
#endif /* NO_CERTS */


WOLFSSL_ASN1_TIME* wolfSSL_X509_CRL_get_lastUpdate(WOLFSSL_X509_CRL* crl)
{
    (void)crl;
    return 0;
}


WOLFSSL_ASN1_TIME* wolfSSL_X509_CRL_get_nextUpdate(WOLFSSL_X509_CRL* crl)
{
    (void)crl;
    return 0;
}



WOLFSSL_EVP_PKEY* wolfSSL_X509_get_pubkey(WOLFSSL_X509* x509)
{
    WOLFSSL_EVP_PKEY* key = NULL;
    if (x509 != NULL) {
        key = (WOLFSSL_EVP_PKEY*)XMALLOC(
                    sizeof(WOLFSSL_EVP_PKEY), x509->heap,
                                                       DYNAMIC_TYPE_PUBLIC_KEY);
        if (key != NULL) {
            key->type = x509->pubKeyOID;
            key->save_type = 0;
            key->pkey.ptr = (char*)XMALLOC(
                        x509->pubKey.length, x509->heap,
                                                       DYNAMIC_TYPE_PUBLIC_KEY);
            if (key->pkey.ptr == NULL) {
                XFREE(key, x509->heap, DYNAMIC_TYPE_PUBLIC_KEY);
                return NULL;
            }
            XMEMCPY(key->pkey.ptr,
                                  x509->pubKey.buffer, x509->pubKey.length);
            key->pkey_sz = x509->pubKey.length;
            #ifdef HAVE_ECC
                key->pkey_curve = (int)x509->pkCurveOID;
            #endif /* HAVE_ECC */
        }
    }
    return key;
}


int wolfSSL_X509_CRL_verify(WOLFSSL_X509_CRL* crl, WOLFSSL_EVP_PKEY* key)
{
    (void)crl;
    (void)key;
    return 0;
}


void wolfSSL_X509_STORE_CTX_set_error(WOLFSSL_X509_STORE_CTX* ctx, int err)
{
    (void)ctx;
    (void)err;
}


void wolfSSL_X509_OBJECT_free_contents(WOLFSSL_X509_OBJECT* obj)
{
    (void)obj;
}


void wolfSSL_EVP_PKEY_free(WOLFSSL_EVP_PKEY* key)
{
    if (key != NULL) {
        if (key->pkey.ptr != NULL)
            XFREE(key->pkey.ptr, NULL, DYNAMIC_TYPE_PUBLIC_KEY);
        XFREE(key, NULL, DYNAMIC_TYPE_PUBLIC_KEY);
    }
}


int wolfSSL_X509_cmp_current_time(const WOLFSSL_ASN1_TIME* asnTime)
{
    (void)asnTime;
    return 0;
}


int wolfSSL_sk_X509_REVOKED_num(WOLFSSL_X509_REVOKED* revoked)
{
    (void)revoked;
    return 0;
}



WOLFSSL_X509_REVOKED* wolfSSL_X509_CRL_get_REVOKED(WOLFSSL_X509_CRL* crl)
{
    (void)crl;
    return 0;
}


WOLFSSL_X509_REVOKED* wolfSSL_sk_X509_REVOKED_value(
                                    WOLFSSL_X509_REVOKED* revoked, int value)
{
    (void)revoked;
    (void)value;
    return 0;
}



WOLFSSL_ASN1_INTEGER* wolfSSL_X509_get_serialNumber(WOLFSSL_X509* x509)
{
    (void)x509;
    return 0;
}


int wolfSSL_ASN1_TIME_print(WOLFSSL_BIO* bio, const WOLFSSL_ASN1_TIME* asnTime)
{
    (void)bio;
    (void)asnTime;
    return 0;
}


#if defined(WOLFSSL_MYSQL_COMPATIBLE)
char* wolfSSL_ASN1_TIME_to_string(WOLFSSL_ASN1_TIME* time, char* buf, int len)
{
    int format;
    int dateLen;
    byte* date = (byte*)time;

    WOLFSSL_ENTER("wolfSSL_ASN1_TIME_to_string");

    if (time == NULL || buf == NULL || len < 5) {
        WOLFSSL_MSG("Bad argument");
        return NULL;
    }

    format  = *date; date++;
    dateLen = *date; date++;
    if (dateLen > len) {
        WOLFSSL_MSG("Length of date is longer then buffer");
        return NULL;
    }

    if (!GetTimeString(date, format, buf, len)) {
        return NULL;
    }

    return buf;
}
#endif /* WOLFSSL_MYSQL_COMPATIBLE */


int wolfSSL_ASN1_INTEGER_cmp(const WOLFSSL_ASN1_INTEGER* a,
                            const WOLFSSL_ASN1_INTEGER* b)
{
    (void)a;
    (void)b;
    return 0;
}


long wolfSSL_ASN1_INTEGER_get(const WOLFSSL_ASN1_INTEGER* i)
{
    (void)i;
    return 0;
}



void* wolfSSL_X509_STORE_CTX_get_ex_data(WOLFSSL_X509_STORE_CTX* ctx, int idx)
{
    WOLFSSL_ENTER("wolfSSL_X509_STORE_CTX_get_ex_data");
#if defined(FORTRESS) || defined(HAVE_STUNNEL)
    if (ctx != NULL && idx == 0)
        return ctx->ex_data;
#else
    (void)ctx;
    (void)idx;
#endif
    return 0;
}


int wolfSSL_get_ex_data_X509_STORE_CTX_idx(void)
{
    WOLFSSL_ENTER("wolfSSL_get_ex_data_X509_STORE_CTX_idx");
    return 0;
}


void wolfSSL_CTX_set_info_callback(WOLFSSL_CTX* ctx,
       void (*f)(const WOLFSSL* ssl, int type, int val))
{
    (void)ctx;
    (void)f;
}


unsigned long wolfSSL_ERR_peek_error(void)
{
    return 0;
}


int wolfSSL_ERR_GET_REASON(unsigned long err)
{
    (void)err;
    return 0;
}


char* wolfSSL_alert_type_string_long(int alertID)
{
    (void)alertID;
    return 0;
}


char* wolfSSL_alert_desc_string_long(int alertID)
{
    (void)alertID;
    return 0;
}


char* wolfSSL_state_string_long(const WOLFSSL* ssl)
{
    (void)ssl;
    return 0;
}


int wolfSSL_PEM_def_callback(char* name, int num, int w, void* key)
{
    (void)name;
    (void)num;
    (void)w;
    (void)key;
    return 0;
}


long wolfSSL_CTX_sess_accept(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_connect(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_accept_good(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_connect_good(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_accept_renegotiate(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_connect_renegotiate(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_hits(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_cb_hits(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_cache_full(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_misses(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_timeouts(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}


long wolfSSL_CTX_sess_number(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    return 0;
}

#ifndef NO_DES3

void wolfSSL_DES_set_key_unchecked(WOLFSSL_const_DES_cblock* myDes,
                                               WOLFSSL_DES_key_schedule* key)
{
    if (myDes != NULL && key != NULL) {
        XMEMCPY(key, myDes, sizeof(WOLFSSL_const_DES_cblock));
    }
}


void wolfSSL_DES_set_odd_parity(WOLFSSL_DES_cblock* myDes)
{
    (void)myDes;
    WOLFSSL_STUB("wolfSSL_DES_set_odd_parity");
}


void wolfSSL_DES_ecb_encrypt(WOLFSSL_DES_cblock* desa,
             WOLFSSL_DES_cblock* desb, WOLFSSL_DES_key_schedule* key, int len)
{
    (void)desa;
    (void)desb;
    (void)key;
    (void)len;
    WOLFSSL_STUB("wolfSSL_DES_ecb_encrypt");
}

#endif /* NO_DES3 */

int wolfSSL_BIO_printf(WOLFSSL_BIO* bio, const char* format, ...)
{
    (void)bio;
    (void)format;
    return 0;
}


int wolfSSL_ASN1_UTCTIME_print(WOLFSSL_BIO* bio, const WOLFSSL_ASN1_UTCTIME* a)
{
    (void)bio;
    (void)a;
    return 0;
}


int  wolfSSL_sk_num(WOLFSSL_X509_REVOKED* rev)
{
    (void)rev;
    return 0;
}


void* wolfSSL_sk_value(WOLFSSL_X509_REVOKED* rev, int i)
{
    (void)rev;
    (void)i;
    return 0;
}


/* stunnel 4.28 needs */
void wolfSSL_CTX_sess_set_get_cb(WOLFSSL_CTX* ctx,
                    WOLFSSL_SESSION*(*f)(WOLFSSL*, unsigned char*, int, int*))
{
    (void)ctx;
    (void)f;
}


void wolfSSL_CTX_sess_set_new_cb(WOLFSSL_CTX* ctx,
                             int (*f)(WOLFSSL*, WOLFSSL_SESSION*))
{
    (void)ctx;
    (void)f;
}


void wolfSSL_CTX_sess_set_remove_cb(WOLFSSL_CTX* ctx, void (*f)(WOLFSSL_CTX*,
                                                        WOLFSSL_SESSION*))
{
    (void)ctx;
    (void)f;
}


int wolfSSL_i2d_SSL_SESSION(WOLFSSL_SESSION* sess, unsigned char** p)
{
    (void)sess;
    (void)p;
    return sizeof(WOLFSSL_SESSION);
}


WOLFSSL_SESSION* wolfSSL_d2i_SSL_SESSION(WOLFSSL_SESSION** sess,
                                const unsigned char** p, long i)
{
    (void)p;
    (void)i;
    if (sess)
        return *sess;
    return NULL;
}


long wolfSSL_SESSION_get_timeout(const WOLFSSL_SESSION* sess)
{
    WOLFSSL_ENTER("wolfSSL_SESSION_get_timeout");
    return sess->timeout;
}


long wolfSSL_SESSION_get_time(const WOLFSSL_SESSION* sess)
{
    WOLFSSL_ENTER("wolfSSL_SESSION_get_time");
    return sess->bornOn;
}


#endif /* OPENSSL_EXTRA */


#ifdef KEEP_PEER_CERT
char*  wolfSSL_X509_get_subjectCN(WOLFSSL_X509* x509)
{
    if (x509 == NULL)
        return NULL;

    return x509->subjectCN;
}
#endif /* KEEP_PEER_CERT */

#ifdef OPENSSL_EXTRA

#ifdef FORTRESS
int wolfSSL_cmp_peer_cert_to_file(WOLFSSL* ssl, const char *fname)
{
    int ret = SSL_FATAL_ERROR;

    WOLFSSL_ENTER("wolfSSL_cmp_peer_cert_to_file");
    if (ssl != NULL && fname != NULL)
    {
    #ifdef WOLFSSL_SMALL_STACK
        EncryptedInfo* info = NULL;
        byte           staticBuffer[1]; /* force heap usage */
    #else
        EncryptedInfo  info[1];
        byte           staticBuffer[FILE_BUFFER_SIZE];
    #endif
        byte*          myBuffer  = staticBuffer;
        int            dynamic   = 0;
        XFILE          file      = XBADFILE;
        long           sz        = 0;
        int            eccKey    = 0;
        WOLFSSL_CTX*   ctx       = ssl->ctx;
        WOLFSSL_X509*  peer_cert = &ssl->peerCert;
        DerBuffer*     fileDer = NULL;

        file = XFOPEN(fname, "rb");
        if (file == XBADFILE)
            return SSL_BAD_FILE;

        XFSEEK(file, 0, XSEEK_END);
        sz = XFTELL(file);
        XREWIND(file);

        if (sz > (long)sizeof(staticBuffer)) {
            WOLFSSL_MSG("Getting dynamic buffer");
            myBuffer = (byte*)XMALLOC(sz, ctx->heap, DYNAMIC_TYPE_FILE);
            dynamic = 1;
        }

    #ifdef WOLFSSL_SMALL_STACK
        info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), NULL,
                                                   DYNAMIC_TYPE_TMP_BUFFER);
        if (info == NULL)
            ret = MEMORY_E;
        else
    #endif
        {
            info->set = 0;
            info->ctx = ctx;
            info->consumed = 0;

            if ((myBuffer != NULL) &&
                (sz > 0) &&
                (XFREAD(myBuffer, sz, 1, file) > 0) &&
                (PemToDer(myBuffer, sz, CERT_TYPE,
                          &fileDer, ctx->heap, info, &eccKey) == 0) &&
                (fileDer->length != 0) &&
                (fileDer->length == peer_cert->derCert->length) &&
                (XMEMCMP(peer_cert->derCert->buffer, fileDer->buffer,
                                                    fileDer->length) == 0))
            {
                ret = 0;
            }

        #ifdef WOLFSSL_SMALL_STACK
            XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
        }

        FreeDer(&fileDer);

        if (dynamic)
            XFREE(myBuffer, ctx->heap, DYNAMIC_TYPE_FILE);

        XFCLOSE(file);
    }

    return ret;
}
#endif


static WC_RNG globalRNG;
static int initGlobalRNG = 0;

/* SSL_SUCCESS on ok */
int wolfSSL_RAND_seed(const void* seed, int len)
{

    WOLFSSL_MSG("wolfSSL_RAND_seed");

    (void)seed;
    (void)len;

    if (initGlobalRNG == 0) {
        if (wc_InitRng(&globalRNG) < 0) {
            WOLFSSL_MSG("wolfSSL Init Global RNG failed");
            return 0;
        }
        initGlobalRNG = 1;
    }

    return SSL_SUCCESS;
}


/* SSL_SUCCESS on ok */
int wolfSSL_RAND_bytes(unsigned char* buf, int num)
{
    int     ret = 0;
    int     initTmpRng = 0;
    WC_RNG* rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG* tmpRNG = NULL;
#else
    WC_RNG  tmpRNG[1];
#endif

    WOLFSSL_ENTER("wolfSSL_RAND_bytes");

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return ret;
#endif

    if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else if (initGlobalRNG)
        rng = &globalRNG;

    if (rng) {
        if (wc_RNG_GenerateBlock(rng, buf, num) != 0)
            WOLFSSL_MSG("Bad wc_RNG_GenerateBlock");
        else
            ret = SSL_SUCCESS;
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}

WOLFSSL_BN_CTX* wolfSSL_BN_CTX_new(void)
{
    static int ctx;  /* wolfcrypt doesn't now need ctx */

    WOLFSSL_MSG("wolfSSL_BN_CTX_new");

    return (WOLFSSL_BN_CTX*)&ctx;
}

void wolfSSL_BN_CTX_init(WOLFSSL_BN_CTX* ctx)
{
    (void)ctx;
    WOLFSSL_MSG("wolfSSL_BN_CTX_init");
}


void wolfSSL_BN_CTX_free(WOLFSSL_BN_CTX* ctx)
{
    (void)ctx;
    WOLFSSL_MSG("wolfSSL_BN_CTX_free");

    /* do free since static ctx that does nothing */
}


static void InitwolfSSL_BigNum(WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("InitwolfSSL_BigNum");
    if (bn) {
        bn->neg      = 0;
        bn->internal = NULL;
    }
}


WOLFSSL_BIGNUM* wolfSSL_BN_new(void)
{
    WOLFSSL_BIGNUM* external;
    mp_int*        mpi;

    WOLFSSL_MSG("wolfSSL_BN_new");

    mpi = (mp_int*) XMALLOC(sizeof(mp_int), NULL, DYNAMIC_TYPE_BIGINT);
    if (mpi == NULL) {
        WOLFSSL_MSG("wolfSSL_BN_new malloc mpi failure");
        return NULL;
    }

    external = (WOLFSSL_BIGNUM*) XMALLOC(sizeof(WOLFSSL_BIGNUM), NULL,
                                        DYNAMIC_TYPE_BIGINT);
    if (external == NULL) {
        WOLFSSL_MSG("wolfSSL_BN_new malloc WOLFSSL_BIGNUM failure");
        XFREE(mpi, NULL, DYNAMIC_TYPE_BIGINT);
        return NULL;
    }

    InitwolfSSL_BigNum(external);
    external->internal = mpi;
    if (mp_init(mpi) != MP_OKAY) {
        wolfSSL_BN_free(external);
        return NULL;
    }

    return external;
}


void wolfSSL_BN_free(WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_free");
    if (bn) {
        if (bn->internal) {
            mp_clear((mp_int*)bn->internal);
            XFREE(bn->internal, NULL, DYNAMIC_TYPE_BIGINT);
            bn->internal = NULL;
        }
        XFREE(bn, NULL, DYNAMIC_TYPE_BIGINT);
        bn = NULL;
    }
}


void wolfSSL_BN_clear_free(WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_clear_free");

    wolfSSL_BN_free(bn);
}


/* SSL_SUCCESS on ok */
int wolfSSL_BN_sub(WOLFSSL_BIGNUM* r, const WOLFSSL_BIGNUM* a,
                  const WOLFSSL_BIGNUM* b)
{
    WOLFSSL_MSG("wolfSSL_BN_sub");

    if (r == NULL || a == NULL || b == NULL)
        return 0;

    if (mp_sub((mp_int*)a->internal,(mp_int*)b->internal,
               (mp_int*)r->internal) == MP_OKAY)
        return SSL_SUCCESS;

    WOLFSSL_MSG("wolfSSL_BN_sub mp_sub failed");
    return 0;
}


/* SSL_SUCCESS on ok */
int wolfSSL_BN_mod(WOLFSSL_BIGNUM* r, const WOLFSSL_BIGNUM* a,
                  const WOLFSSL_BIGNUM* b, const WOLFSSL_BN_CTX* c)
{
    (void)c;
    WOLFSSL_MSG("wolfSSL_BN_mod");

    if (r == NULL || a == NULL || b == NULL)
        return 0;

    if (mp_mod((mp_int*)a->internal,(mp_int*)b->internal,
               (mp_int*)r->internal) == MP_OKAY)
        return SSL_SUCCESS;

    WOLFSSL_MSG("wolfSSL_BN_mod mp_mod failed");
    return 0;
}


const WOLFSSL_BIGNUM* wolfSSL_BN_value_one(void)
{
    static WOLFSSL_BIGNUM* bn_one = NULL;

    WOLFSSL_MSG("wolfSSL_BN_value_one");

    if (bn_one == NULL) {
        bn_one = wolfSSL_BN_new();
        if (bn_one)
            mp_set_int((mp_int*)bn_one->internal, 1);
    }

    return bn_one;
}

/* return compliant with OpenSSL
 *   size of BIGNUM in bytes, 0 if error */
int wolfSSL_BN_num_bytes(const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_num_bytes");

    if (bn == NULL || bn->internal == NULL)
        return SSL_FAILURE;

    return mp_unsigned_bin_size((mp_int*)bn->internal);
}

/* return compliant with OpenSSL
 *   size of BIGNUM in bits, 0 if error */
int wolfSSL_BN_num_bits(const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_num_bits");

    if (bn == NULL || bn->internal == NULL)
        return SSL_FAILURE;

    return mp_count_bits((mp_int*)bn->internal);
}

/* return compliant with OpenSSL
 *   1 if BIGNUM is zero, 0 else */
int wolfSSL_BN_is_zero(const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_is_zero");

    if (bn == NULL || bn->internal == NULL)
        return SSL_FAILURE;

    if (mp_iszero((mp_int*)bn->internal) == MP_YES)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}

/* return compliant with OpenSSL
 *   1 if BIGNUM is one, 0 else */
int wolfSSL_BN_is_one(const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_is_one");

    if (bn == NULL || bn->internal == NULL)
        return SSL_FAILURE;

    if (mp_cmp_d((mp_int*)bn->internal, 1) == MP_EQ)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}

/* return compliant with OpenSSL
 *   1 if BIGNUM is odd, 0 else */
int wolfSSL_BN_is_odd(const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_is_odd");

    if (bn == NULL || bn->internal == NULL)
        return SSL_FAILURE;

    if (mp_isodd((mp_int*)bn->internal) == MP_YES)
        return SSL_SUCCESS;

    return SSL_FAILURE;
}

/* return compliant with OpenSSL
 *   -1 if a < b, 0 if a == b and 1 if a > b
 */
int wolfSSL_BN_cmp(const WOLFSSL_BIGNUM* a, const WOLFSSL_BIGNUM* b)
{
    int ret;

    WOLFSSL_MSG("wolfSSL_BN_cmp");

    if (a == NULL || a->internal == NULL || b == NULL || b->internal == NULL)
        return SSL_FATAL_ERROR;

    ret = mp_cmp((mp_int*)a->internal, (mp_int*)b->internal);

    return (ret == MP_EQ ? 0 : (ret == MP_GT ? 1 : -1));
}

/* return compliant with OpenSSL
 *   length of BIGNUM in bytes, -1 if error */
int wolfSSL_BN_bn2bin(const WOLFSSL_BIGNUM* bn, unsigned char* r)
{
    WOLFSSL_MSG("wolfSSL_BN_bn2bin");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("NULL bn error");
        return SSL_FATAL_ERROR;
    }

    if (r == NULL)
        return mp_unsigned_bin_size((mp_int*)bn->internal);

    if (mp_to_unsigned_bin((mp_int*)bn->internal, r) != MP_OKAY) {
        WOLFSSL_MSG("mp_to_unsigned_bin error");
        return SSL_FATAL_ERROR;
    }

    return mp_unsigned_bin_size((mp_int*)bn->internal);
}


WOLFSSL_BIGNUM* wolfSSL_BN_bin2bn(const unsigned char* str, int len,
                            WOLFSSL_BIGNUM* ret)
{
    int weOwn = 0;

    WOLFSSL_MSG("wolfSSL_BN_bin2bn");

    /* if ret is null create a BN */
    if (ret == NULL) {
        ret = wolfSSL_BN_new();
        weOwn = 1;
        if (ret == NULL)
            return NULL;
    }

    /* check ret and ret->internal then read in value */
    if (ret && ret->internal) {
        if (mp_read_unsigned_bin((mp_int*)ret->internal, str, len) != 0) {
            WOLFSSL_MSG("mp_read_unsigned_bin failure");
            if (weOwn)
                wolfSSL_BN_free(ret);
            return NULL;
        }
    }

    return ret;
}

/* return compliant with OpenSSL
 *   1 if success, 0 if error */
int wolfSSL_mask_bits(WOLFSSL_BIGNUM* bn, int n)
{
    (void)bn;
    (void)n;
    WOLFSSL_MSG("wolfSSL_BN_mask_bits");

    return SSL_FAILURE;
}


/* SSL_SUCCESS on ok */
int wolfSSL_BN_rand(WOLFSSL_BIGNUM* bn, int bits, int top, int bottom)
{
    int           ret    = 0;
    int           len    = bits / 8;
    int           initTmpRng = 0;
    WC_RNG*       rng    = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG*       tmpRNG = NULL;
    byte*         buff   = NULL;
#else
    WC_RNG        tmpRNG[1];
    byte          buff[1024];
#endif

    (void)top;
    (void)bottom;
    WOLFSSL_MSG("wolfSSL_BN_rand");

    if (bits % 8)
        len++;

#ifdef WOLFSSL_SMALL_STACK
    buff   = (byte*)XMALLOC(1024,        NULL, DYNAMIC_TYPE_TMP_BUFFER);
    tmpRNG = (WC_RNG*) XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (buff == NULL || tmpRNG == NULL) {
        XFREE(buff,   NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return ret;
    }
#endif

    if (bn == NULL || bn->internal == NULL)
        WOLFSSL_MSG("Bad function arguments");
    else if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else if (initGlobalRNG)
        rng = &globalRNG;

    if (rng) {
        if (wc_RNG_GenerateBlock(rng, buff, len) != 0)
            WOLFSSL_MSG("Bad wc_RNG_GenerateBlock");
        else {
            buff[0]     |= 0x80 | 0x40;
            buff[len-1] |= 0x01;

            if (mp_read_unsigned_bin((mp_int*)bn->internal,buff,len) != MP_OKAY)
                WOLFSSL_MSG("mp read bin failed");
            else
                ret = SSL_SUCCESS;
        }
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(buff,   NULL, DYNAMIC_TYPE_TMP_BUFFER);
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}

/* return code compliant with OpenSSL :
 *   1 if bit set, 0 else
 */
int wolfSSL_BN_is_bit_set(const WOLFSSL_BIGNUM* bn, int n)
{
    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    if (n > DIGIT_BIT) {
        WOLFSSL_MSG("input bit count too large");
        return SSL_FAILURE;
    }

    return mp_is_bit_set((mp_int*)bn->internal, (mp_digit)n);
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 else
 */
int wolfSSL_BN_set_bit(WOLFSSL_BIGNUM* bn, int n)
{
    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    if (mp_set_bit((mp_int*)bn->internal, n) != MP_OKAY) {
        WOLFSSL_MSG("mp_set_int error");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}


/* SSL_SUCCESS on ok */
int wolfSSL_BN_hex2bn(WOLFSSL_BIGNUM** bn, const char* str)
{
    int     ret     = 0;
    word32  decSz   = 1024;
#ifdef WOLFSSL_SMALL_STACK
    byte*   decoded = NULL;
#else
    byte    decoded[1024];
#endif

    WOLFSSL_MSG("wolfSSL_BN_hex2bn");

#ifdef WOLFSSL_SMALL_STACK
    decoded = (byte*)XMALLOC(decSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (decoded == NULL)
        return ret;
#endif

    if (str == NULL)
        WOLFSSL_MSG("Bad function argument");
    else if (Base16_Decode((byte*)str, (int)XSTRLEN(str), decoded, &decSz) < 0)
        WOLFSSL_MSG("Bad Base16_Decode error");
    else if (bn == NULL)
        ret = decSz;
    else {
        if (*bn == NULL)
            *bn = wolfSSL_BN_new();

        if (*bn == NULL)
            WOLFSSL_MSG("BN new failed");
        else if (wolfSSL_BN_bin2bn(decoded, decSz, *bn) == NULL)
            WOLFSSL_MSG("Bad bin2bn error");
        else
            ret = SSL_SUCCESS;
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(decoded, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}


WOLFSSL_BIGNUM* wolfSSL_BN_dup(const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_BIGNUM* ret;

    WOLFSSL_MSG("wolfSSL_BN_dup");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return NULL;
    }

    ret = wolfSSL_BN_new();
    if (ret == NULL) {
        WOLFSSL_MSG("bn new error");
        return NULL;
    }

    if (mp_copy((mp_int*)bn->internal, (mp_int*)ret->internal) != MP_OKAY) {
        WOLFSSL_MSG("mp_copy error");
        wolfSSL_BN_free(ret);
        return NULL;
    }

    ret->neg = bn->neg;

    return ret;
}


WOLFSSL_BIGNUM* wolfSSL_BN_copy(WOLFSSL_BIGNUM* r, const WOLFSSL_BIGNUM* bn)
{
    WOLFSSL_MSG("wolfSSL_BN_copy");

    if (mp_copy((mp_int*)bn->internal, (mp_int*)r->internal) != MP_OKAY) {
        WOLFSSL_MSG("mp_copy error");
        return NULL;
    }

    r->neg = bn->neg;

    return r;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 else
 */
int wolfSSL_BN_set_word(WOLFSSL_BIGNUM* bn, WOLFSSL_BN_ULONG w)
{
    WOLFSSL_MSG("wolfSSL_BN_set_word");

    if (mp_set_int((mp_int*)bn->internal, w) != MP_OKAY) {
        WOLFSSL_MSG("mp_init_set_int error");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   number length in decimal if success, 0 if error
 */
int wolfSSL_BN_dec2bn(WOLFSSL_BIGNUM** bn, const char* str)
{
    (void)bn;
    (void)str;

    WOLFSSL_MSG("wolfSSL_BN_dec2bn");

    return SSL_FAILURE;
}


#if defined(WOLFSSL_KEY_GEN) || defined(HAVE_COMP_KEY)
char *wolfSSL_BN_bn2dec(const WOLFSSL_BIGNUM *bn)
{
    int len = 0;
    char *buf;

    WOLFSSL_MSG("wolfSSL_BN_bn2dec");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return NULL;
    }

    if (mp_radix_size((mp_int*)bn->internal, 10, &len) != MP_OKAY) {
        WOLFSSL_MSG("mp_radix_size failure");
        return NULL;
    }

    buf = (char*) XMALLOC(len, NULL, DYNAMIC_TYPE_ECC);
    if (buf == NULL) {
        WOLFSSL_MSG("wolfSSL_BN_bn2hex malloc buffer failure");
        return NULL;
    }

    if (mp_toradix((mp_int*)bn->internal, buf, 10) != MP_OKAY) {
        XFREE(buf, NULL, DYNAMIC_TYPE_ECC);
        return NULL;
    }

    return buf;
}
#else
char* wolfSSL_BN_bn2dec(const WOLFSSL_BIGNUM* bn)
{
    (void)bn;

    WOLFSSL_MSG("wolfSSL_BN_bn2dec");

    return NULL;
}
#endif /* defined(WOLFSSL_KEY_GEN) || defined(HAVE_COMP_KEY) */

/* return code compliant with OpenSSL :
 *   1 if success, 0 else
 */
int wolfSSL_BN_lshift(WOLFSSL_BIGNUM *r, const WOLFSSL_BIGNUM *bn, int n)
{
    WOLFSSL_MSG("wolfSSL_BN_lshift");

    if (r == NULL || r->internal == NULL || bn == NULL || bn->internal == NULL){
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    if (mp_mul_2d((mp_int*)bn->internal, n, (mp_int*)r->internal) != MP_OKAY) {
        WOLFSSL_MSG("mp_mul_2d error");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 else
 */
int wolfSSL_BN_rshift(WOLFSSL_BIGNUM *r, const WOLFSSL_BIGNUM *bn, int n)
{
    WOLFSSL_MSG("wolfSSL_BN_rshift");

    if (r == NULL || r->internal == NULL || bn == NULL || bn->internal == NULL){
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    if (mp_div_2d((mp_int*)bn->internal, n,
                  (mp_int*)r->internal, NULL) != MP_OKAY) {
        WOLFSSL_MSG("mp_mul_2d error");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 else
 */
int wolfSSL_BN_add_word(WOLFSSL_BIGNUM *bn, WOLFSSL_BN_ULONG w)
{
    WOLFSSL_MSG("wolfSSL_BN_add_word");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    if (mp_add_d((mp_int*)bn->internal, w, (mp_int*)bn->internal) != MP_OKAY) {
        WOLFSSL_MSG("mp_add_d error");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 else
 */
int wolfSSL_BN_add(WOLFSSL_BIGNUM *r, WOLFSSL_BIGNUM *a, WOLFSSL_BIGNUM *b)
{
    WOLFSSL_MSG("wolfSSL_BN_add");

    if (r == NULL || r->internal == NULL || a == NULL || a->internal == NULL ||
        b == NULL || b->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    if (mp_add((mp_int*)a->internal, (mp_int*)b->internal,
               (mp_int*)r->internal) != MP_OKAY) {
        WOLFSSL_MSG("mp_add_d error");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

#ifdef WOLFSSL_KEY_GEN

/* return code compliant with OpenSSL :
 *   1 if prime, 0 if not, -1 if error
 */
int wolfSSL_BN_is_prime_ex(const WOLFSSL_BIGNUM *bn, int nbchecks,
                           WOLFSSL_BN_CTX *ctx, WOLFSSL_BN_GENCB *cb)
{
    int res;

    (void)ctx;
    (void)cb;

    WOLFSSL_MSG("wolfSSL_BN_is_prime_ex");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FATAL_ERROR;
    }

    if (mp_prime_is_prime((mp_int*)bn->internal, nbchecks, &res) != MP_OKAY) {
        WOLFSSL_MSG("mp_prime_is_prime error");
        return SSL_FATAL_ERROR;
    }

    if (res != MP_YES) {
        WOLFSSL_MSG("mp_prime_is_prime not prime");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   (bn mod w) if success, -1 if error
 */
WOLFSSL_BN_ULONG wolfSSL_BN_mod_word(const WOLFSSL_BIGNUM *bn,
                                     WOLFSSL_BN_ULONG w)
{
    WOLFSSL_BN_ULONG ret = 0;

    WOLFSSL_MSG("wolfSSL_BN_mod_word");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return (WOLFSSL_BN_ULONG)SSL_FATAL_ERROR;
    }

    if (mp_mod_d((mp_int*)bn->internal, w, &ret) != MP_OKAY) {
        WOLFSSL_MSG("mp_add_d error");
        return (WOLFSSL_BN_ULONG)SSL_FATAL_ERROR;
    }

    return ret;
}
#endif /* #ifdef WOLFSSL_KEY_GEN */

#if defined(WOLFSSL_KEY_GEN) || defined(HAVE_COMP_KEY)
char *wolfSSL_BN_bn2hex(const WOLFSSL_BIGNUM *bn)
{
    int len = 0;
    char *buf;

    WOLFSSL_MSG("wolfSSL_BN_bn2hex");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return NULL;
    }

    if (mp_radix_size((mp_int*)bn->internal, 16, &len) != MP_OKAY) {
        WOLFSSL_MSG("mp_radix_size failure");
        return NULL;
    }

    buf = (char*) XMALLOC(len, NULL, DYNAMIC_TYPE_ECC);
    if (buf == NULL) {
        WOLFSSL_MSG("wolfSSL_BN_bn2hex malloc buffer failure");
        return NULL;
    }

    if (mp_toradix((mp_int*)bn->internal, buf, 16) != MP_OKAY) {
        XFREE(buf, NULL, DYNAMIC_TYPE_ECC);
        return NULL;
    }

    return buf;
}

#ifndef NO_FILESYSTEM
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_BN_print_fp(FILE *fp, const WOLFSSL_BIGNUM *bn)
{
    char *buf;

    WOLFSSL_MSG("wolfSSL_BN_print_fp");

    if (fp == NULL || bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FAILURE;
    }

    buf = wolfSSL_BN_bn2hex(bn);
    if (buf == NULL) {
        WOLFSSL_MSG("wolfSSL_BN_bn2hex failure");
        return SSL_FAILURE;
    }

    fprintf(fp, "%s", buf);
    XFREE(buf, NULL, DYNAMIC_TYPE_ECC);

    return SSL_SUCCESS;
}
#endif /* !defined(NO_FILESYSTEM) */

#else /* defined(WOLFSSL_KEY_GEN) || defined(HAVE_COMP_KEY) */

char *wolfSSL_BN_bn2hex(const WOLFSSL_BIGNUM *bn)
{
    (void)bn;

    WOLFSSL_MSG("wolfSSL_BN_bn2hex not implemented");

    return (char*)"";
}

#ifndef NO_FILESYSTEM
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_BN_print_fp(FILE *fp, const WOLFSSL_BIGNUM *bn)
{
    (void)fp;
    (void)bn;

    WOLFSSL_MSG("wolfSSL_BN_print_fp not implemented");

    return SSL_SUCCESS;
}
#endif /* !defined(NO_FILESYSTEM) */

#endif /* defined(WOLFSSL_KEY_GEN) || defined(HAVE_COMP_KEY) */

WOLFSSL_BIGNUM *wolfSSL_BN_CTX_get(WOLFSSL_BN_CTX *ctx)
{
    /* ctx is not used, return new Bignum */
    (void)ctx;

    WOLFSSL_ENTER("wolfSSL_BN_CTX_get");

    return wolfSSL_BN_new();
}

void wolfSSL_BN_CTX_start(WOLFSSL_BN_CTX *ctx)
{
    (void)ctx;

    WOLFSSL_ENTER("wolfSSL_BN_CTX_start");
    WOLFSSL_MSG("wolfSSL_BN_CTX_start TBD");
}

#ifndef NO_DH

static void InitwolfSSL_DH(WOLFSSL_DH* dh)
{
    if (dh) {
        dh->p        = NULL;
        dh->g        = NULL;
        dh->pub_key  = NULL;
        dh->priv_key = NULL;
        dh->internal = NULL;
        dh->inSet    = 0;
        dh->exSet    = 0;
    }
}


WOLFSSL_DH* wolfSSL_DH_new(void)
{
    WOLFSSL_DH* external;
    DhKey*     key;

    WOLFSSL_MSG("wolfSSL_DH_new");

    key = (DhKey*) XMALLOC(sizeof(DhKey), NULL, DYNAMIC_TYPE_DH);
    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_DH_new malloc DhKey failure");
        return NULL;
    }

    external = (WOLFSSL_DH*) XMALLOC(sizeof(WOLFSSL_DH), NULL,
                                    DYNAMIC_TYPE_DH);
    if (external == NULL) {
        WOLFSSL_MSG("wolfSSL_DH_new malloc WOLFSSL_DH failure");
        XFREE(key, NULL, DYNAMIC_TYPE_DH);
        return NULL;
    }

    InitwolfSSL_DH(external);
    wc_InitDhKey(key);
    external->internal = key;

    return external;
}


void wolfSSL_DH_free(WOLFSSL_DH* dh)
{
    WOLFSSL_MSG("wolfSSL_DH_free");

    if (dh) {
        if (dh->internal) {
            wc_FreeDhKey((DhKey*)dh->internal);
            XFREE(dh->internal, NULL, DYNAMIC_TYPE_DH);
            dh->internal = NULL;
        }
        wolfSSL_BN_free(dh->priv_key);
        wolfSSL_BN_free(dh->pub_key);
        wolfSSL_BN_free(dh->g);
        wolfSSL_BN_free(dh->p);
        InitwolfSSL_DH(dh);  /* set back to NULLs for safety */

        XFREE(dh, NULL, DYNAMIC_TYPE_DH);
    }
}


static int SetDhInternal(WOLFSSL_DH* dh)
{
    int            ret = SSL_FATAL_ERROR;
    int            pSz = 1024;
    int            gSz = 1024;
#ifdef WOLFSSL_SMALL_STACK
    unsigned char* p   = NULL;
    unsigned char* g   = NULL;
#else
    unsigned char  p[1024];
    unsigned char  g[1024];
#endif

    WOLFSSL_ENTER("SetDhInternal");

    if (dh == NULL || dh->p == NULL || dh->g == NULL)
        WOLFSSL_MSG("Bad function arguments");
    else if (wolfSSL_BN_bn2bin(dh->p, NULL) > pSz)
        WOLFSSL_MSG("Bad p internal size");
    else if (wolfSSL_BN_bn2bin(dh->g, NULL) > gSz)
        WOLFSSL_MSG("Bad g internal size");
    else {
    #ifdef WOLFSSL_SMALL_STACK
        p = (unsigned char*)XMALLOC(pSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        g = (unsigned char*)XMALLOC(gSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);

        if (p == NULL || g == NULL) {
            XFREE(p, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            XFREE(g, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            return ret;
        }
    #endif

        pSz = wolfSSL_BN_bn2bin(dh->p, p);
        gSz = wolfSSL_BN_bn2bin(dh->g, g);

        if (pSz <= 0 || gSz <= 0)
            WOLFSSL_MSG("Bad BN2bin set");
        else if (wc_DhSetKey((DhKey*)dh->internal, p, pSz, g, gSz) < 0)
            WOLFSSL_MSG("Bad DH SetKey");
        else {
            dh->inSet = 1;
            ret = SSL_SUCCESS;
        }

    #ifdef WOLFSSL_SMALL_STACK
        XFREE(p, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(g, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
    }


    return ret;
}

/* return code compliant with OpenSSL :
 *   DH prime size in bytes if success, 0 if error
 */
int wolfSSL_DH_size(WOLFSSL_DH* dh)
{
    WOLFSSL_MSG("wolfSSL_DH_size");

    if (dh == NULL)
        return SSL_FATAL_ERROR;

    return wolfSSL_BN_num_bytes(dh->p);
}


/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_DH_generate_key(WOLFSSL_DH* dh)
{
    int            ret    = SSL_FAILURE;
    word32         pubSz  = 768;
    word32         privSz = 768;
    int            initTmpRng = 0;
    WC_RNG*        rng    = NULL;
#ifdef WOLFSSL_SMALL_STACK
    unsigned char* pub    = NULL;
    unsigned char* priv   = NULL;
    WC_RNG*        tmpRNG = NULL;
#else
    unsigned char  pub [768];
    unsigned char  priv[768];
    WC_RNG         tmpRNG[1];
#endif

    WOLFSSL_MSG("wolfSSL_DH_generate_key");

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    pub    = (unsigned char*)XMALLOC(pubSz,   NULL, DYNAMIC_TYPE_TMP_BUFFER);
    priv   = (unsigned char*)XMALLOC(privSz,  NULL, DYNAMIC_TYPE_TMP_BUFFER);

    if (tmpRNG == NULL || pub == NULL || priv == NULL) {
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(pub,    NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(priv,   NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return ret;
    }
#endif

    if (dh == NULL || dh->p == NULL || dh->g == NULL)
        WOLFSSL_MSG("Bad function arguments");
    else if (dh->inSet == 0 && SetDhInternal(dh) != SSL_SUCCESS)
            WOLFSSL_MSG("Bad DH set internal");
    else if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("Bad RNG Init, trying global");
        if (initGlobalRNG == 0)
            WOLFSSL_MSG("Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng) {
       if (wc_DhGenerateKeyPair((DhKey*)dh->internal, rng, priv, &privSz,
                                                               pub, &pubSz) < 0)
            WOLFSSL_MSG("Bad wc_DhGenerateKeyPair");
       else {
            if (dh->pub_key)
                wolfSSL_BN_free(dh->pub_key);

            dh->pub_key = wolfSSL_BN_new();
            if (dh->pub_key == NULL) {
                WOLFSSL_MSG("Bad DH new pub");
            }
            if (dh->priv_key)
                wolfSSL_BN_free(dh->priv_key);

            dh->priv_key = wolfSSL_BN_new();

            if (dh->priv_key == NULL) {
                WOLFSSL_MSG("Bad DH new priv");
            }

            if (dh->pub_key && dh->priv_key) {
               if (wolfSSL_BN_bin2bn(pub, pubSz, dh->pub_key) == NULL)
                   WOLFSSL_MSG("Bad DH bn2bin error pub");
               else if (wolfSSL_BN_bin2bn(priv, privSz, dh->priv_key) == NULL)
                   WOLFSSL_MSG("Bad DH bn2bin error priv");
               else
                   ret = SSL_SUCCESS;
            }
        }
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    XFREE(pub,    NULL, DYNAMIC_TYPE_TMP_BUFFER);
    XFREE(priv,   NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}


/* return code compliant with OpenSSL :
 *   size of shared secret if success, -1 if error
 */
int wolfSSL_DH_compute_key(unsigned char* key, WOLFSSL_BIGNUM* otherPub,
                          WOLFSSL_DH* dh)
{
    int            ret    = SSL_FATAL_ERROR;
    word32         keySz  = 0;
    word32         pubSz  = 1024;
    word32         privSz = 1024;
#ifdef WOLFSSL_SMALL_STACK
    unsigned char* pub    = NULL;
    unsigned char* priv   = NULL;
#else
    unsigned char  pub [1024];
    unsigned char  priv[1024];
#endif

    WOLFSSL_MSG("wolfSSL_DH_compute_key");

#ifdef WOLFSSL_SMALL_STACK
    pub = (unsigned char*)XMALLOC(pubSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (pub == NULL)
        return ret;

    priv = (unsigned char*)XMALLOC(privSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (priv == NULL) {
        XFREE(pub, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return ret;
    }
#endif

    if (dh == NULL || dh->priv_key == NULL || otherPub == NULL)
        WOLFSSL_MSG("Bad function arguments");
    else if ((keySz = (word32)DH_size(dh)) == 0)
        WOLFSSL_MSG("Bad DH_size");
    else if (wolfSSL_BN_bn2bin(dh->priv_key, NULL) > (int)privSz)
        WOLFSSL_MSG("Bad priv internal size");
    else if (wolfSSL_BN_bn2bin(otherPub, NULL) > (int)pubSz)
        WOLFSSL_MSG("Bad otherPub size");
    else {
        privSz = wolfSSL_BN_bn2bin(dh->priv_key, priv);
        pubSz  = wolfSSL_BN_bn2bin(otherPub, pub);

        if (privSz <= 0 || pubSz <= 0)
            WOLFSSL_MSG("Bad BN2bin set");
        else if (wc_DhAgree((DhKey*)dh->internal, key, &keySz,
                            priv, privSz, pub, pubSz) < 0)
            WOLFSSL_MSG("wc_DhAgree failed");
        else
            ret = (int)keySz;
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(pub,  NULL, DYNAMIC_TYPE_TMP_BUFFER);
    XFREE(priv, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}
#endif /* NO_DH */


#ifndef NO_DSA
static void InitwolfSSL_DSA(WOLFSSL_DSA* dsa)
{
    if (dsa) {
        dsa->p        = NULL;
        dsa->q        = NULL;
        dsa->g        = NULL;
        dsa->pub_key  = NULL;
        dsa->priv_key = NULL;
        dsa->internal = NULL;
        dsa->inSet    = 0;
        dsa->exSet    = 0;
    }
}


WOLFSSL_DSA* wolfSSL_DSA_new(void)
{
    WOLFSSL_DSA* external;
    DsaKey*     key;

    WOLFSSL_MSG("wolfSSL_DSA_new");

    key = (DsaKey*) XMALLOC(sizeof(DsaKey), NULL, DYNAMIC_TYPE_DSA);
    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_DSA_new malloc DsaKey failure");
        return NULL;
    }

    external = (WOLFSSL_DSA*) XMALLOC(sizeof(WOLFSSL_DSA), NULL,
                                    DYNAMIC_TYPE_DSA);
    if (external == NULL) {
        WOLFSSL_MSG("wolfSSL_DSA_new malloc WOLFSSL_DSA failure");
        XFREE(key, NULL, DYNAMIC_TYPE_DSA);
        return NULL;
    }

    InitwolfSSL_DSA(external);
    InitDsaKey(key);
    external->internal = key;

    return external;
}


void wolfSSL_DSA_free(WOLFSSL_DSA* dsa)
{
    WOLFSSL_MSG("wolfSSL_DSA_free");

    if (dsa) {
        if (dsa->internal) {
            FreeDsaKey((DsaKey*)dsa->internal);
            XFREE(dsa->internal, NULL, DYNAMIC_TYPE_DSA);
            dsa->internal = NULL;
        }
        wolfSSL_BN_free(dsa->priv_key);
        wolfSSL_BN_free(dsa->pub_key);
        wolfSSL_BN_free(dsa->g);
        wolfSSL_BN_free(dsa->q);
        wolfSSL_BN_free(dsa->p);
        InitwolfSSL_DSA(dsa);  /* set back to NULLs for safety */

        XFREE(dsa, NULL, DYNAMIC_TYPE_DSA);
        dsa = NULL;
    }
}
#endif /* NO_DSA */

#ifndef NO_RSA
static void InitwolfSSL_Rsa(WOLFSSL_RSA* rsa)
{
    if (rsa) {
        rsa->n        = NULL;
        rsa->e        = NULL;
        rsa->d        = NULL;
        rsa->p        = NULL;
        rsa->q        = NULL;
        rsa->dmp1     = NULL;
        rsa->dmq1     = NULL;
        rsa->iqmp     = NULL;
        rsa->internal = NULL;
        rsa->inSet    = 0;
        rsa->exSet    = 0;
    }
}


WOLFSSL_RSA* wolfSSL_RSA_new(void)
{
    WOLFSSL_RSA* external;
    RsaKey*     key;

    WOLFSSL_MSG("wolfSSL_RSA_new");

    key = (RsaKey*) XMALLOC(sizeof(RsaKey), NULL, DYNAMIC_TYPE_RSA);
    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_RSA_new malloc RsaKey failure");
        return NULL;
    }

    external = (WOLFSSL_RSA*) XMALLOC(sizeof(WOLFSSL_RSA), NULL,
                                     DYNAMIC_TYPE_RSA);
    if (external == NULL) {
        WOLFSSL_MSG("wolfSSL_RSA_new malloc WOLFSSL_RSA failure");
        XFREE(key, NULL, DYNAMIC_TYPE_RSA);
        return NULL;
    }

    InitwolfSSL_Rsa(external);
    if (wc_InitRsaKey(key, NULL) != 0) {
        WOLFSSL_MSG("InitRsaKey WOLFSSL_RSA failure");
        XFREE(external, NULL, DYNAMIC_TYPE_RSA);
        XFREE(key, NULL, DYNAMIC_TYPE_RSA);
        return NULL;
    }
    external->internal = key;

    return external;
}


void wolfSSL_RSA_free(WOLFSSL_RSA* rsa)
{
    WOLFSSL_MSG("wolfSSL_RSA_free");

    if (rsa) {
        if (rsa->internal) {
            wc_FreeRsaKey((RsaKey*)rsa->internal);
            XFREE(rsa->internal, NULL, DYNAMIC_TYPE_RSA);
            rsa->internal = NULL;
        }
        wolfSSL_BN_free(rsa->iqmp);
        wolfSSL_BN_free(rsa->dmq1);
        wolfSSL_BN_free(rsa->dmp1);
        wolfSSL_BN_free(rsa->q);
        wolfSSL_BN_free(rsa->p);
        wolfSSL_BN_free(rsa->d);
        wolfSSL_BN_free(rsa->e);
        wolfSSL_BN_free(rsa->n);
        InitwolfSSL_Rsa(rsa);  /* set back to NULLs for safety */

        XFREE(rsa, NULL, DYNAMIC_TYPE_RSA);
        rsa = NULL;
    }
}
#endif /* NO_RSA */


#if (!defined(NO_RSA) && !defined(HAVE_USER_RSA) && !defined(HAVE_FAST_RSA)) \
    || !defined(NO_DSA) || defined(HAVE_ECC)
static int SetIndividualExternal(WOLFSSL_BIGNUM** bn, mp_int* mpi)
{
    WOLFSSL_MSG("Entering SetIndividualExternal");

    if (mpi == NULL || bn == NULL) {
        WOLFSSL_MSG("mpi NULL error");
        return SSL_FATAL_ERROR;
    }

    if (*bn == NULL) {
        *bn = wolfSSL_BN_new();
        if (*bn == NULL) {
            WOLFSSL_MSG("SetIndividualExternal alloc failed");
            return SSL_FATAL_ERROR;
        }
    }

    if (mp_copy(mpi, (mp_int*)((*bn)->internal)) != MP_OKAY) {
        WOLFSSL_MSG("mp_copy error");
        return SSL_FATAL_ERROR;
    }

    return SSL_SUCCESS;
}

static int SetIndividualInternal(WOLFSSL_BIGNUM* bn, mp_int* mpi)
{
    WOLFSSL_MSG("Entering SetIndividualInternal");

    if (bn == NULL || bn->internal == NULL) {
        WOLFSSL_MSG("bn NULL error");
        return SSL_FATAL_ERROR;
    }

    if (mpi == NULL || (mp_init(mpi) != MP_OKAY)) {
        WOLFSSL_MSG("mpi NULL error");
        return SSL_FATAL_ERROR;
    }

    if (mp_copy((mp_int*)bn->internal, mpi) != MP_OKAY) {
        WOLFSSL_MSG("mp_copy error");
        return SSL_FATAL_ERROR;
    }

    return SSL_SUCCESS;
}
#endif /* !NO_RSA && !NO_DSA */


#ifndef NO_DSA
/* wolfSSL -> OpenSSL */
static int SetDsaExternal(WOLFSSL_DSA* dsa)
{
    DsaKey* key;
    WOLFSSL_MSG("Entering SetDsaExternal");

    if (dsa == NULL || dsa->internal == NULL) {
        WOLFSSL_MSG("dsa key NULL error");
        return SSL_FATAL_ERROR;
    }

    key = (DsaKey*)dsa->internal;

    if (SetIndividualExternal(&dsa->p, &key->p) != SSL_SUCCESS) {
        WOLFSSL_MSG("dsa p key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&dsa->q, &key->q) != SSL_SUCCESS) {
        WOLFSSL_MSG("dsa q key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&dsa->g, &key->g) != SSL_SUCCESS) {
        WOLFSSL_MSG("dsa g key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&dsa->pub_key, &key->y) != SSL_SUCCESS) {
        WOLFSSL_MSG("dsa y key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&dsa->priv_key, &key->x) != SSL_SUCCESS) {
        WOLFSSL_MSG("dsa x key error");
        return SSL_FATAL_ERROR;
    }

    dsa->exSet = 1;

    return SSL_SUCCESS;
}

/* Openssl -> WolfSSL */
static int SetDsaInternal(WOLFSSL_DSA* dsa)
{
    DsaKey* key;
    WOLFSSL_MSG("Entering SetDsaInternal");

    if (dsa == NULL || dsa->internal == NULL) {
        WOLFSSL_MSG("dsa key NULL error");
        return SSL_FATAL_ERROR;
    }

    key = (DsaKey*)dsa->internal;

    if (dsa->p != NULL &&
        SetIndividualInternal(dsa->p, &key->p) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa p key error");
        return SSL_FATAL_ERROR;
    }

    if (dsa->q != NULL &&
        SetIndividualInternal(dsa->q, &key->q) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa q key error");
        return SSL_FATAL_ERROR;
    }

    if (dsa->g != NULL &&
        SetIndividualInternal(dsa->g, &key->g) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa g key error");
        return SSL_FATAL_ERROR;
    }

    if (dsa->pub_key != NULL) {
        if (SetIndividualInternal(dsa->pub_key, &key->y) != SSL_SUCCESS) {
            WOLFSSL_MSG("rsa pub_key error");
            return SSL_FATAL_ERROR;
        }

        /* public key */
        key->type = DSA_PUBLIC;
    }

    if (dsa->priv_key != NULL) {
        if (SetIndividualInternal(dsa->priv_key, &key->x) != SSL_SUCCESS) {
            WOLFSSL_MSG("rsa priv_key error");
            return SSL_FATAL_ERROR;
        }

        /* private key */
        key->type = DSA_PRIVATE;
    }

    dsa->inSet = 1;

    return SSL_SUCCESS;
}
#endif /* NO_DSA */


#if !defined(NO_RSA)
#if !defined(HAVE_USER_RSA) && !defined(HAVE_FAST_RSA)
/* WolfSSL -> OpenSSL */
static int SetRsaExternal(WOLFSSL_RSA* rsa)
{
    RsaKey* key;
    WOLFSSL_MSG("Entering SetRsaExternal");

    if (rsa == NULL || rsa->internal == NULL) {
        WOLFSSL_MSG("rsa key NULL error");
        return SSL_FATAL_ERROR;
    }

    key = (RsaKey*)rsa->internal;

    if (SetIndividualExternal(&rsa->n, &key->n) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa n key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->e, &key->e) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa e key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->d, &key->d) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa d key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->p, &key->p) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa p key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->q, &key->q) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa q key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->dmp1, &key->dP) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa dP key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->dmq1, &key->dQ) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa dQ key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&rsa->iqmp, &key->u) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa u key error");
        return SSL_FATAL_ERROR;
    }

    rsa->exSet = 1;

    return SSL_SUCCESS;
}

/* Openssl -> WolfSSL */
static int SetRsaInternal(WOLFSSL_RSA* rsa)
{
    RsaKey* key;
    WOLFSSL_MSG("Entering SetRsaInternal");

    if (rsa == NULL || rsa->internal == NULL) {
        WOLFSSL_MSG("rsa key NULL error");
        return SSL_FATAL_ERROR;
    }

    key = (RsaKey*)rsa->internal;

    if (SetIndividualInternal(rsa->n, &key->n) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa n key error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualInternal(rsa->e, &key->e) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa e key error");
        return SSL_FATAL_ERROR;
    }

    /* public key */
    key->type = RSA_PUBLIC;

    if (rsa->d != NULL) {
        if (SetIndividualInternal(rsa->d, &key->d) != SSL_SUCCESS) {
            WOLFSSL_MSG("rsa d key error");
            return SSL_FATAL_ERROR;
        }

        /* private key */
        key->type = RSA_PRIVATE;
    }

    if (rsa->p != NULL &&
        SetIndividualInternal(rsa->p, &key->p) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa p key error");
        return SSL_FATAL_ERROR;
    }

    if (rsa->q != NULL &&
        SetIndividualInternal(rsa->q, &key->q) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa q key error");
        return SSL_FATAL_ERROR;
    }

    if (rsa->dmp1 != NULL &&
        SetIndividualInternal(rsa->dmp1, &key->dP) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa dP key error");
        return SSL_FATAL_ERROR;
    }

    if (rsa->dmq1 != NULL &&
        SetIndividualInternal(rsa->dmq1, &key->dQ) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa dQ key error");
        return SSL_FATAL_ERROR;
    }

    if (rsa->iqmp != NULL &&
        SetIndividualInternal(rsa->iqmp, &key->u) != SSL_SUCCESS) {
        WOLFSSL_MSG("rsa u key error");
        return SSL_FATAL_ERROR;
    }

    rsa->inSet = 1;

    return SSL_SUCCESS;
}
#endif /* HAVE_USER_RSA */

/* return compliant with OpenSSL
 *   1 if success, 0 if error
 */
int wolfSSL_RSA_generate_key_ex(WOLFSSL_RSA* rsa, int bits, WOLFSSL_BIGNUM* bn,
                                void* cb)
{
    int ret = SSL_FAILURE;

    (void)cb;
    (void)bn;
    (void)bits;

    WOLFSSL_ENTER("wolfSSL_RSA_generate_key_ex");

    if (rsa == NULL || rsa->internal == NULL) {
        /* bit size checked during make key call */
        WOLFSSL_MSG("bad arguments");
        return SSL_FAILURE;
    }

#ifdef WOLFSSL_KEY_GEN
    {
    #ifdef WOLFSSL_SMALL_STACK
        WC_RNG* rng = NULL;
    #else
        WC_RNG  rng[1];
    #endif

    #ifdef WOLFSSL_SMALL_STACK
        rng = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (rng == NULL)
            return SSL_FAILURE;
    #endif

        if (wc_InitRng(rng) < 0)
            WOLFSSL_MSG("RNG init failed");
        else if (wc_MakeRsaKey((RsaKey*)rsa->internal,
                               bits, 65537, rng) != MP_OKAY)
            WOLFSSL_MSG("wc_MakeRsaKey failed");
        else if (SetRsaExternal(rsa) != SSL_SUCCESS)
            WOLFSSL_MSG("SetRsaExternal failed");
        else {
            rsa->inSet = 1;
            ret = SSL_SUCCESS;
        }

        wc_FreeRng(rng);
    #ifdef WOLFSSL_SMALL_STACK
        XFREE(rng, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    #endif
    }
#else
    WOLFSSL_MSG("No Key Gen built in");
#endif
    return ret;
}


/* SSL_SUCCESS on ok */
int wolfSSL_RSA_blinding_on(WOLFSSL_RSA* rsa, WOLFSSL_BN_CTX* bn)
{
    (void)rsa;
    (void)bn;

    WOLFSSL_MSG("wolfSSL_RSA_blinding_on");

    return SSL_SUCCESS;  /* on by default */
}

/* return compliant with OpenSSL
 *   size of encrypted data if success , -1 if error
 */
int wolfSSL_RSA_public_encrypt(int len, unsigned char* fr,
                            unsigned char* to, WOLFSSL_RSA* rsa, int padding)
{
    (void)len;
    (void)fr;
    (void)to;
    (void)rsa;
    (void)padding;

    WOLFSSL_MSG("wolfSSL_RSA_public_encrypt");

    return SSL_FATAL_ERROR;
}

/* return compliant with OpenSSL
 *   size of plain recovered data if success , -1 if error
 */
int wolfSSL_RSA_private_decrypt(int len, unsigned char* fr,
                            unsigned char* to, WOLFSSL_RSA* rsa, int padding)
{
    (void)len;
    (void)fr;
    (void)to;
    (void)rsa;
    (void)padding;

    WOLFSSL_MSG("wolfSSL_RSA_private_decrypt");

    return SSL_FATAL_ERROR;
}

/* return compliant with OpenSSL
 *   RSA modulus size in bytes, -1 if error
 */
int wolfSSL_RSA_size(const WOLFSSL_RSA* rsa)
{
    WOLFSSL_MSG("wolfSSL_RSA_size");

    if (rsa == NULL)
        return SSL_FATAL_ERROR;

    return wolfSSL_BN_num_bytes(rsa->n);
}
#endif /* NO_RSA */

#ifndef NO_DSA
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_DSA_generate_key(WOLFSSL_DSA* dsa)
{
    int ret = SSL_FAILURE;

    WOLFSSL_ENTER("wolfSSL_DSA_generate_key");

    if (dsa == NULL || dsa->internal == NULL) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

    if (dsa->inSet == 0) {
        WOLFSSL_MSG("No DSA internal set, do it");

        if (SetDsaInternal(dsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetDsaInternal failed");
            return ret;
        }
    }

#ifdef WOLFSSL_KEY_GEN
    {
        int initTmpRng = 0;
        WC_RNG *rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
        WC_RNG *tmpRNG = NULL;
#else
        WC_RNG tmpRNG[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
        tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (tmpRNG == NULL)
            return SSL_FATAL_ERROR;
#endif
        if (wc_InitRng(tmpRNG) == 0) {
            rng = tmpRNG;
            initTmpRng = 1;
        }
        else {
            WOLFSSL_MSG("Bad RNG Init, trying global");
            if (initGlobalRNG == 0)
                WOLFSSL_MSG("Global RNG no Init");
            else
                rng = &globalRNG;
        }

        if (rng) {
            if (wc_MakeDsaKey(rng, (DsaKey*)dsa->internal) != MP_OKAY)
                WOLFSSL_MSG("wc_MakeDsaKey failed");
            else if (SetDsaExternal(dsa) != SSL_SUCCESS)
                WOLFSSL_MSG("SetDsaExternal failed");
            else
                ret = SSL_SUCCESS;
        }

        if (initTmpRng)
            wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
    }
#else /* WOLFSSL_KEY_GEN */
    WOLFSSL_MSG("No Key Gen built in");
#endif
    return ret;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_DSA_generate_parameters_ex(WOLFSSL_DSA* dsa, int bits,
                                       unsigned char* seed, int seedLen,
                                       int* counterRet,
                                       unsigned long* hRet, void* cb)
{
    int ret = SSL_FAILURE;

    (void)bits;
    (void)seed;
    (void)seedLen;
    (void)counterRet;
    (void)hRet;
    (void)cb;

    WOLFSSL_ENTER("wolfSSL_DSA_generate_parameters_ex");

    if (dsa == NULL || dsa->internal == NULL) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

#ifdef WOLFSSL_KEY_GEN
    {
        int initTmpRng = 0;
        WC_RNG *rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
        WC_RNG *tmpRNG = NULL;
#else
        WC_RNG tmpRNG[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
        tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (tmpRNG == NULL)
            return SSL_FATAL_ERROR;
#endif
        if (wc_InitRng(tmpRNG) == 0) {
            rng = tmpRNG;
            initTmpRng = 1;
        }
        else {
            WOLFSSL_MSG("Bad RNG Init, trying global");
            if (initGlobalRNG == 0)
                WOLFSSL_MSG("Global RNG no Init");
            else
                rng = &globalRNG;
        }

        if (rng) {
            if (wc_MakeDsaParameters(rng, bits,
                                     (DsaKey*)dsa->internal) != MP_OKAY)
                WOLFSSL_MSG("wc_MakeDsaParameters failed");
            else if (SetDsaExternal(dsa) != SSL_SUCCESS)
                WOLFSSL_MSG("SetDsaExternal failed");
            else
                ret = SSL_SUCCESS;
        }

        if (initTmpRng)
            wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
    }
#else /* WOLFSSL_KEY_GEN */
    WOLFSSL_MSG("No Key Gen built in");
#endif

    return ret;
}

/* return SSL_SUCCESS on success, < 0 otherwise */
int wolfSSL_DSA_do_sign(const unsigned char* d, unsigned char* sigRet,
                       WOLFSSL_DSA* dsa)
{
    int     ret = SSL_FATAL_ERROR;
    int     initTmpRng = 0;
    WC_RNG* rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG* tmpRNG = NULL;
#else
    WC_RNG  tmpRNG[1];
#endif

    WOLFSSL_ENTER("wolfSSL_DSA_do_sign");

    if (d == NULL || sigRet == NULL || dsa == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return ret;
    }

    if (dsa->inSet == 0)
    {
        WOLFSSL_MSG("No DSA internal set, do it");

        if (SetDsaInternal(dsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetDsaInternal failed");
            return ret;
        }
    }

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return SSL_FATAL_ERROR;
#endif

    if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("Bad RNG Init, trying global");
        if (initGlobalRNG == 0)
            WOLFSSL_MSG("Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng) {
        if (DsaSign(d, sigRet, (DsaKey*)dsa->internal, rng) < 0)
            WOLFSSL_MSG("DsaSign failed");
        else
            ret = SSL_SUCCESS;
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}


int wolfSSL_DSA_do_verify(const unsigned char* d, unsigned char* sig,
                        WOLFSSL_DSA* dsa, int *dsacheck)
{
    int    ret = SSL_FATAL_ERROR;

    WOLFSSL_ENTER("wolfSSL_DSA_do_verify");

    if (d == NULL || sig == NULL || dsa == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FATAL_ERROR;
    }
    if (dsa->inSet == 0)
    {
        WOLFSSL_MSG("No DSA internal set, do it");

        if (SetDsaInternal(dsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetDsaInternal failed");
            return SSL_FATAL_ERROR;
        }
    }

    ret = DsaVerify(d, sig, (DsaKey*)dsa->internal, dsacheck);
    if (ret != 0 || *dsacheck != 1) {
        WOLFSSL_MSG("DsaVerify failed");
        return ret;
    }

    return SSL_SUCCESS;
}
#endif /* NO_DSA */


#ifndef NO_RSA
/* return SSL_SUCCES on ok, 0 otherwise */
int wolfSSL_RSA_sign(int type, const unsigned char* m,
                           unsigned int mLen, unsigned char* sigRet,
                           unsigned int* sigLen, WOLFSSL_RSA* rsa)
{
    word32  outLen;
    word32  signSz;
    int     initTmpRng = 0;
    WC_RNG* rng        = NULL;
    int     ret        = 0;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG* tmpRNG     = NULL;
    byte*   encodedSig = NULL;
#else
    WC_RNG  tmpRNG[1];
    byte    encodedSig[MAX_ENCODED_SIG_SZ];
#endif

    WOLFSSL_MSG("wolfSSL_RSA_sign");

    if (m == NULL || sigRet == NULL || sigLen == NULL || rsa == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return 0;
    }

    switch (type) {
    #ifdef WOLFSSL_MD2
        case NID_md2:       type = MD2h;    break;
    #endif
    #ifndef NO_MD5
        case NID_md5:       type = MD5h;    break;
    #endif
    #ifndef NO_SHA
        case NID_sha1:      type = SHAh;    break;
    #endif
    #ifndef NO_SHA256
        case NID_sha256:    type = SHA256h; break;
    #endif
    #ifdef WOLFSSL_SHA384
        case NID_sha384:    type = SHA384h; break;
    #endif
    #ifdef WOLFSSL_SHA512
        case NID_sha512:    type = SHA512h; break;
    #endif
        default:
            WOLFSSL_MSG("This NID (md type) not configured or not implemented");
            return 0;
    }

    if (rsa->inSet == 0)
    {
        WOLFSSL_MSG("No RSA internal set, do it");

        if (SetRsaInternal(rsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetRsaInternal failed");
            return 0;
        }
    }

    outLen = (word32)wolfSSL_BN_num_bytes(rsa->n);

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return 0;

    encodedSig = (byte*)XMALLOC(MAX_ENCODED_SIG_SZ, NULL,
                                                   DYNAMIC_TYPE_TMP_BUFFER);
    if (encodedSig == NULL) {
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return 0;
    }
#endif

    if (outLen == 0)
        WOLFSSL_MSG("Bad RSA size");
    else if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("Bad RNG Init, trying global");

        if (initGlobalRNG == 0)
            WOLFSSL_MSG("Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng) {

        signSz = wc_EncodeSignature(encodedSig, m, mLen, type);
        if (signSz == 0) {
            WOLFSSL_MSG("Bad Encode Signature");
        }
        else {
            *sigLen = wc_RsaSSL_Sign(encodedSig, signSz, sigRet, outLen,
                                  (RsaKey*)rsa->internal, rng);
            if (*sigLen <= 0)
                WOLFSSL_MSG("Bad Rsa Sign");
            else
                ret = SSL_SUCCESS;
        }

    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG,     NULL, DYNAMIC_TYPE_TMP_BUFFER);
    XFREE(encodedSig, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (ret == SSL_SUCCESS)
        WOLFSSL_MSG("wolfSSL_RSA_sign success");
    else {
        WOLFSSL_MSG("wolfSSL_RSA_sign failed");
    }
    return ret;
}


int wolfSSL_RSA_public_decrypt(int flen, unsigned char* from,
                          unsigned char* to, WOLFSSL_RSA* rsa, int padding)
{
    int tlen = 0;

    WOLFSSL_MSG("wolfSSL_RSA_public_decrypt");

    if (rsa == NULL || rsa->internal == NULL || from == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return 0;
    }

    if (padding != RSA_PKCS1_PADDING) {
        WOLFSSL_MSG("wolfSSL_RSA_public_decrypt unsupported padding");
        return 0;
    }

    if (rsa->inSet == 0)
    {
        WOLFSSL_MSG("No RSA internal set, do it");

        if (SetRsaInternal(rsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetRsaInternal failed");
            return 0;
        }
    }

    /* size of 'to' buffer must be size of RSA key */
    tlen = wc_RsaSSL_Verify(from, flen, to, wolfSSL_RSA_size(rsa),
                            (RsaKey*)rsa->internal);
    if (tlen <= 0)
        WOLFSSL_MSG("wolfSSL_RSA_public_decrypt failed");
    else {
        WOLFSSL_MSG("wolfSSL_RSA_public_decrypt success");
    }
    return tlen;
}


/* generate p-1 and q-1, SSL_SUCCESS on ok */
int wolfSSL_RSA_GenAdd(WOLFSSL_RSA* rsa)
{
    int    err;
    mp_int tmp;

    WOLFSSL_MSG("wolfSSL_RsaGenAdd");

    if (rsa == NULL || rsa->p == NULL || rsa->q == NULL || rsa->d == NULL ||
                       rsa->dmp1 == NULL || rsa->dmq1 == NULL) {
        WOLFSSL_MSG("rsa no init error");
        return SSL_FATAL_ERROR;
    }

    if (mp_init(&tmp) != MP_OKAY) {
        WOLFSSL_MSG("mp_init error");
        return SSL_FATAL_ERROR;
    }

    err = mp_sub_d((mp_int*)rsa->p->internal, 1, &tmp);
    if (err != MP_OKAY) {
        WOLFSSL_MSG("mp_sub_d error");
    }
    else
        err = mp_mod((mp_int*)rsa->d->internal, &tmp,
                     (mp_int*)rsa->dmp1->internal);

    if (err != MP_OKAY) {
        WOLFSSL_MSG("mp_mod error");
    }
    else
        err = mp_sub_d((mp_int*)rsa->q->internal, 1, &tmp);
    if (err != MP_OKAY) {
        WOLFSSL_MSG("mp_sub_d error");
    }
    else
        err = mp_mod((mp_int*)rsa->d->internal, &tmp,
                     (mp_int*)rsa->dmq1->internal);

    mp_clear(&tmp);

    if (err == MP_OKAY)
        return SSL_SUCCESS;
    else
        return SSL_FATAL_ERROR;
}
#endif /* NO_RSA */


void wolfSSL_HMAC_Init(WOLFSSL_HMAC_CTX* ctx, const void* key, int keylen,
                  const EVP_MD* type)
{
    WOLFSSL_MSG("wolfSSL_HMAC_Init");

    if (ctx == NULL) {
        WOLFSSL_MSG("no ctx on init");
        return;
    }

    if (type) {
        WOLFSSL_MSG("init has type");

        if (XSTRNCMP(type, "MD5", 3) == 0) {
            WOLFSSL_MSG("md5 hmac");
            ctx->type = MD5;
        }
        else if (XSTRNCMP(type, "SHA256", 6) == 0) {
            WOLFSSL_MSG("sha256 hmac");
            ctx->type = SHA256;
        }

        /* has to be last since would pick or 256, 384, or 512 too */
        else if (XSTRNCMP(type, "SHA", 3) == 0) {
            WOLFSSL_MSG("sha hmac");
            ctx->type = SHA;
        }
        else {
            WOLFSSL_MSG("bad init type");
        }
    }

    if (key && keylen) {
        WOLFSSL_MSG("keying hmac");
        wc_HmacSetKey(&ctx->hmac, ctx->type, (const byte*)key, (word32)keylen);
        /* OpenSSL compat, no error */
    }
}


void wolfSSL_HMAC_Update(WOLFSSL_HMAC_CTX* ctx, const unsigned char* data,
                    int len)
{
    WOLFSSL_MSG("wolfSSL_HMAC_Update");

    if (ctx && data) {
        WOLFSSL_MSG("updating hmac");
        wc_HmacUpdate(&ctx->hmac, data, (word32)len);
        /* OpenSSL compat, no error */
    }
}


void wolfSSL_HMAC_Final(WOLFSSL_HMAC_CTX* ctx, unsigned char* hash,
                   unsigned int* len)
{
    WOLFSSL_MSG("wolfSSL_HMAC_Final");

    if (ctx && hash) {
        WOLFSSL_MSG("final hmac");
        wc_HmacFinal(&ctx->hmac, hash);
        /* OpenSSL compat, no error */

        if (len) {
            WOLFSSL_MSG("setting output len");
            switch (ctx->type) {
                case MD5:
                    *len = MD5_DIGEST_SIZE;
                    break;

                case SHA:
                    *len = SHA_DIGEST_SIZE;
                    break;

                case SHA256:
                    *len = SHA256_DIGEST_SIZE;
                    break;

                default:
                    WOLFSSL_MSG("bad hmac type");
            }
        }
    }
}


void wolfSSL_HMAC_cleanup(WOLFSSL_HMAC_CTX* ctx)
{
    (void)ctx;

    WOLFSSL_MSG("wolfSSL_HMAC_cleanup");
}


const WOLFSSL_EVP_MD* wolfSSL_EVP_get_digestbynid(int id)
{
    WOLFSSL_MSG("wolfSSL_get_digestbynid");

    switch(id) {
#ifndef NO_MD5
        case NID_md5:
            return wolfSSL_EVP_md5();
#endif
#ifndef NO_SHA
        case NID_sha1:
            return wolfSSL_EVP_sha1();
#endif
        default:
            WOLFSSL_MSG("Bad digest id value");
    }

    return NULL;
}


WOLFSSL_RSA* wolfSSL_EVP_PKEY_get1_RSA(WOLFSSL_EVP_PKEY* key)
{
    (void)key;
    WOLFSSL_MSG("wolfSSL_EVP_PKEY_get1_RSA not implemented");

    return NULL;
}


WOLFSSL_DSA* wolfSSL_EVP_PKEY_get1_DSA(WOLFSSL_EVP_PKEY* key)
{
    (void)key;
    WOLFSSL_MSG("wolfSSL_EVP_PKEY_get1_DSA not implemented");

    return NULL;
}


WOLFSSL_EC_KEY* wolfSSL_EVP_PKEY_get1_EC_KEY(WOLFSSL_EVP_PKEY* key)
{
    (void)key;
    WOLFSSL_MSG("wolfSSL_EVP_PKEY_get1_EC_KEY not implemented");

    return NULL;
}


void* wolfSSL_EVP_X_STATE(const WOLFSSL_EVP_CIPHER_CTX* ctx)
{
    WOLFSSL_MSG("wolfSSL_EVP_X_STATE");

    if (ctx) {
        switch (ctx->cipherType) {
            case ARC4_TYPE:
                WOLFSSL_MSG("returning arc4 state");
                return (void*)&ctx->cipher.arc4.x;

            default:
                WOLFSSL_MSG("bad x state type");
                return 0;
        }
    }

    return NULL;
}


int wolfSSL_EVP_X_STATE_LEN(const WOLFSSL_EVP_CIPHER_CTX* ctx)
{
    WOLFSSL_MSG("wolfSSL_EVP_X_STATE_LEN");

    if (ctx) {
        switch (ctx->cipherType) {
            case ARC4_TYPE:
                WOLFSSL_MSG("returning arc4 state size");
                return sizeof(Arc4);

            default:
                WOLFSSL_MSG("bad x state type");
                return 0;
        }
    }

    return 0;
}


#ifndef NO_DES3

void wolfSSL_3des_iv(WOLFSSL_EVP_CIPHER_CTX* ctx, int doset,
                            unsigned char* iv, int len)
{
    (void)len;

    WOLFSSL_MSG("wolfSSL_3des_iv");

    if (ctx == NULL || iv == NULL) {
        WOLFSSL_MSG("Bad function argument");
        return;
    }

    if (doset)
        wc_Des3_SetIV(&ctx->cipher.des3, iv);  /* OpenSSL compat, no ret */
    else
        XMEMCPY(iv, &ctx->cipher.des3.reg, DES_BLOCK_SIZE);
}

#endif /* NO_DES3 */


#ifndef NO_AES

void wolfSSL_aes_ctr_iv(WOLFSSL_EVP_CIPHER_CTX* ctx, int doset,
                      unsigned char* iv, int len)
{
    (void)len;

    WOLFSSL_MSG("wolfSSL_aes_ctr_iv");

    if (ctx == NULL || iv == NULL) {
        WOLFSSL_MSG("Bad function argument");
        return;
    }

    if (doset)
        wc_AesSetIV(&ctx->cipher.aes, iv);  /* OpenSSL compat, no ret */
    else
        XMEMCPY(iv, &ctx->cipher.aes.reg, AES_BLOCK_SIZE);
}

#endif /* NO_AES */


const WOLFSSL_EVP_MD* wolfSSL_EVP_ripemd160(void)
{
    WOLFSSL_MSG("wolfSSL_ripemd160");

    return NULL;
}


int wolfSSL_EVP_MD_size(const WOLFSSL_EVP_MD* type)
{
    WOLFSSL_MSG("wolfSSL_EVP_MD_size");

    if (type == NULL) {
        WOLFSSL_MSG("No md type arg");
        return BAD_FUNC_ARG;
    }

    if (XSTRNCMP(type, "SHA256", 6) == 0) {
        return SHA256_DIGEST_SIZE;
    }
#ifndef NO_MD5
    else if (XSTRNCMP(type, "MD5", 3) == 0) {
        return MD5_DIGEST_SIZE;
    }
#endif
#ifdef WOLFSSL_SHA384
    else if (XSTRNCMP(type, "SHA384", 6) == 0) {
        return SHA384_DIGEST_SIZE;
    }
#endif
#ifdef WOLFSSL_SHA512
    else if (XSTRNCMP(type, "SHA512", 6) == 0) {
        return SHA512_DIGEST_SIZE;
    }
#endif
#ifndef NO_SHA
    /* has to be last since would pick or 256, 384, or 512 too */
    else if (XSTRNCMP(type, "SHA", 3) == 0) {
        return SHA_DIGEST_SIZE;
    }
#endif

    return BAD_FUNC_ARG;
}


int wolfSSL_EVP_CIPHER_CTX_iv_length(const WOLFSSL_EVP_CIPHER_CTX* ctx)
{
    WOLFSSL_MSG("wolfSSL_EVP_CIPHER_CTX_iv_length");

    switch (ctx->cipherType) {

        case AES_128_CBC_TYPE :
        case AES_192_CBC_TYPE :
        case AES_256_CBC_TYPE :
            WOLFSSL_MSG("AES CBC");
            return AES_BLOCK_SIZE;

#ifdef WOLFSSL_AES_COUNTER
        case AES_128_CTR_TYPE :
        case AES_192_CTR_TYPE :
        case AES_256_CTR_TYPE :
            WOLFSSL_MSG("AES CTR");
            return AES_BLOCK_SIZE;
#endif

        case DES_CBC_TYPE :
            WOLFSSL_MSG("DES CBC");
            return DES_BLOCK_SIZE;

        case DES_EDE3_CBC_TYPE :
            WOLFSSL_MSG("DES EDE3 CBC");
            return DES_BLOCK_SIZE;
#ifdef HAVE_IDEA
        case IDEA_CBC_TYPE :
            WOLFSSL_MSG("IDEA CBC");
            return IDEA_BLOCK_SIZE;
#endif
        case ARC4_TYPE :
            WOLFSSL_MSG("ARC4");
            return 0;

        case NULL_CIPHER_TYPE :
            WOLFSSL_MSG("NULL");
            return 0;

        default: {
            WOLFSSL_MSG("bad type");
        }
    }
    return 0;
}


void wolfSSL_OPENSSL_free(void* p)
{
    WOLFSSL_MSG("wolfSSL_OPENSSL_free");

    XFREE(p, NULL, DYNAMIC_TYPE_OPENSSL);
}

#if defined(WOLFSSL_KEY_GEN)

static int EncryptDerKey(byte *der, int *derSz, const EVP_CIPHER* cipher,
                         unsigned char* passwd, int passwdSz, byte **cipherInfo)
{
    int ret, paddingSz;
    word32 idx, cipherInfoSz;
#ifdef WOLFSSL_SMALL_STACK
    EncryptedInfo* info = NULL;
#else
    EncryptedInfo  info[1];
#endif

    WOLFSSL_ENTER("EncryptDerKey");

    if (der == NULL || derSz == NULL || cipher == NULL ||
        passwd == NULL || cipherInfo == NULL)
        return BAD_FUNC_ARG;

#ifdef WOLFSSL_SMALL_STACK
    info = (EncryptedInfo*)XMALLOC(sizeof(EncryptedInfo), NULL,
                                   DYNAMIC_TYPE_TMP_BUFFER);
    if (info == NULL) {
        WOLFSSL_MSG("malloc failed");
        return SSL_FAILURE;
    }
#endif
    info->set      = 0;
    info->ctx      = NULL;
    info->consumed = 0;

    /* set iv size */
    if (XSTRNCMP(cipher, "DES", 3) == 0)
        info->ivSz = DES_IV_SIZE;
    else if (XSTRNCMP(cipher, "AES", 3) == 0)
        info->ivSz = AES_IV_SIZE;
    else {
        WOLFSSL_MSG("unsupported cipher");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return SSL_FAILURE;
    }

    /* set the cipher name on info */
    XSTRNCPY(info->name, cipher, NAME_SZ);

    /* Generate a random salt */
    if (wolfSSL_RAND_bytes(info->iv, info->ivSz) != SSL_SUCCESS) {
        WOLFSSL_MSG("generate iv failed");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return SSL_FAILURE;
    }

    /* add the padding before encryption */
    paddingSz = ((*derSz)/info->ivSz + 1) * info->ivSz - (*derSz);
    if (paddingSz == 0)
        paddingSz = info->ivSz;
    XMEMSET(der+(*derSz), (byte)paddingSz, paddingSz);
    (*derSz) += paddingSz;

    /* encrypt buffer */
    if (wolfssl_encrypt_buffer_key(der, *derSz,
                                   passwd, passwdSz, info) != SSL_SUCCESS) {
        WOLFSSL_MSG("encrypt key failed");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return SSL_FAILURE;
    }

    /* create cipher info : 'cipher_name,Salt(hex)' */
    cipherInfoSz = (word32)(2*info->ivSz + XSTRLEN(info->name) + 2);
    *cipherInfo = (byte*)XMALLOC(cipherInfoSz, NULL,
                                DYNAMIC_TYPE_TMP_BUFFER);
    if (*cipherInfo == NULL) {
        WOLFSSL_MSG("malloc failed");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return SSL_FAILURE;
    }
    XSTRNCPY((char*)*cipherInfo, info->name, cipherInfoSz);
    XSTRNCAT((char*)*cipherInfo, ",", 1);

    idx = (word32)XSTRLEN((char*)*cipherInfo);
    cipherInfoSz -= idx;
    ret = Base16_Encode(info->iv, info->ivSz, *cipherInfo+idx, &cipherInfoSz);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(info, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
    if (ret != 0) {
        WOLFSSL_MSG("Base16_Encode failed");
        XFREE(*cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}
#endif /* defined(WOLFSSL_KEY_GEN) */

#if defined(WOLFSSL_KEY_GEN) && !defined(NO_RSA)

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_mem_RSAPrivateKey(RSA* rsa, const EVP_CIPHER* cipher,
                                        unsigned char* passwd, int passwdSz,
                                        unsigned char **pem, int *plen)
{
    byte *derBuf, *tmp, *cipherInfo = NULL;
    int  der_max_len = 0, derSz = 0;

    WOLFSSL_ENTER("wolfSSL_PEM_write_mem_RSAPrivateKey");

    if (pem == NULL || plen == NULL || rsa == NULL || rsa->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FAILURE;
    }

    if (rsa->inSet == 0) {
        WOLFSSL_MSG("No RSA internal set, do it");

        if (SetRsaInternal(rsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetRsaInternal failed");
            return SSL_FAILURE;
        }
    }

    /* 5 > size of n, d, p, q, d%(p-1), d(q-1), 1/q%p, e + ASN.1 additional
     *  informations
     */
    der_max_len = 5 * wolfSSL_RSA_size(rsa) + AES_BLOCK_SIZE;

    derBuf = (byte*)XMALLOC(der_max_len, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (derBuf == NULL) {
        WOLFSSL_MSG("malloc failed");
        return SSL_FAILURE;
    }

    /* Key to DER */
    derSz = wc_RsaKeyToDer((RsaKey*)rsa->internal, derBuf, der_max_len);
    if (derSz < 0) {
        WOLFSSL_MSG("wc_RsaKeyToDer failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    /* encrypt DER buffer if required */
    if (passwd != NULL && passwdSz > 0 && cipher != NULL) {
        int ret;

        ret = EncryptDerKey(derBuf, &derSz, cipher,
                            passwd, passwdSz, &cipherInfo);
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("EncryptDerKey failed");
            XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            return ret;
        }

        /* tmp buffer with a max size */
        *plen = (derSz * 2) + sizeof(BEGIN_RSA_PRIV) +
                sizeof(END_RSA_PRIV) + HEADER_ENCRYPTED_KEY_SIZE;
    }
    else /* tmp buffer with a max size */
        *plen = (derSz * 2) + sizeof(BEGIN_RSA_PRIV) + sizeof(END_RSA_PRIV);

    tmp = (byte*)XMALLOC(*plen, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmp == NULL) {
        WOLFSSL_MSG("malloc failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (cipherInfo != NULL)
            XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    /* DER to PEM */
    *plen = wc_DerToPemEx(derBuf, derSz, tmp, *plen, cipherInfo, PRIVATEKEY_TYPE);
    if (*plen <= 0) {
        WOLFSSL_MSG("wc_DerToPemEx failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (cipherInfo != NULL)
            XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (cipherInfo != NULL)
        XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);

    *pem = (byte*)XMALLOC((*plen)+1, NULL, DYNAMIC_TYPE_KEY);
    if (*pem == NULL) {
        WOLFSSL_MSG("malloc failed");
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XMEMSET(*pem, 0, (*plen)+1);

    if (XMEMCPY(*pem, tmp, *plen) == NULL) {
        WOLFSSL_MSG("XMEMCPY failed");
        XFREE(pem, NULL, DYNAMIC_TYPE_KEY);
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);

    return SSL_SUCCESS;
}


#ifndef NO_FILESYSTEM
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_RSAPrivateKey(FILE *fp, WOLFSSL_RSA *rsa,
                                    const EVP_CIPHER *enc,
                                    unsigned char *kstr, int klen,
                                    pem_password_cb *cb, void *u)
{
    byte *pem;
    int  plen, ret;

    (void)cb;
    (void)u;

    WOLFSSL_MSG("wolfSSL_PEM_write_RSAPrivateKey");

    if (fp == NULL || rsa == NULL || rsa->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FAILURE;
    }

    ret = wolfSSL_PEM_write_mem_RSAPrivateKey(rsa, enc, kstr, klen, &pem, &plen);
    if (ret != SSL_SUCCESS) {
        WOLFSSL_MSG("wolfSSL_PEM_write_mem_RSAPrivateKey failed");
        return SSL_FAILURE;
    }

    ret = (int)XFWRITE(pem, plen, 1, fp);
    if (ret != 1) {
        WOLFSSL_MSG("RSA private key file write failed");
        return SSL_FAILURE;
    }

    XFREE(pem, NULL, DYNAMIC_TYPE_KEY);
    return SSL_SUCCESS;
}
#endif /* NO_FILESYSTEM */

int wolfSSL_PEM_write_bio_RSAPrivateKey(WOLFSSL_BIO* bio, RSA* rsa,
                                        const EVP_CIPHER* cipher,
                                        unsigned char* passwd, int len,
                                        pem_password_cb cb, void* arg)
{
    (void)bio;
    (void)rsa;
    (void)cipher;
    (void)passwd;
    (void)len;
    (void)cb;
    (void)arg;

    WOLFSSL_MSG("wolfSSL_PEM_write_bio_RSAPrivateKey not implemented");

    return SSL_FAILURE;
}
#endif /* defined(WOLFSSL_KEY_GEN) && !defined(NO_RSA) */

#ifdef HAVE_ECC

/* EC_POINT Openssl -> WolfSSL */
static int SetECPointInternal(WOLFSSL_EC_POINT *p)
{
    ecc_point* point;
    WOLFSSL_ENTER("SetECPointInternal");

    if (p == NULL || p->internal == NULL) {
        WOLFSSL_MSG("ECPoint NULL error");
        return SSL_FATAL_ERROR;
    }

    point = (ecc_point*)p->internal;

    if (p->X != NULL && SetIndividualInternal(p->X, point->x) != SSL_SUCCESS) {
        WOLFSSL_MSG("ecc point X error");
        return SSL_FATAL_ERROR;
    }

    if (p->Y != NULL && SetIndividualInternal(p->Y, point->y) != SSL_SUCCESS) {
        WOLFSSL_MSG("ecc point Y error");
        return SSL_FATAL_ERROR;
    }

    if (p->Z != NULL && SetIndividualInternal(p->Z, point->z) != SSL_SUCCESS) {
        WOLFSSL_MSG("ecc point Z error");
        return SSL_FATAL_ERROR;
    }

    p->inSet = 1;

    return SSL_SUCCESS;
}

/* EC_POINT WolfSSL -> OpenSSL */
static int SetECPointExternal(WOLFSSL_EC_POINT *p)
{
    ecc_point* point;

    WOLFSSL_ENTER("SetECPointExternal");

    if (p == NULL || p->internal == NULL) {
        WOLFSSL_MSG("ECPoint NULL error");
        return SSL_FATAL_ERROR;
    }

    point = (ecc_point*)p->internal;

    if (SetIndividualExternal(&p->X, point->x) != SSL_SUCCESS) {
        WOLFSSL_MSG("ecc point X error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&p->Y, point->y) != SSL_SUCCESS) {
        WOLFSSL_MSG("ecc point Y error");
        return SSL_FATAL_ERROR;
    }

    if (SetIndividualExternal(&p->Z, point->z) != SSL_SUCCESS) {
        WOLFSSL_MSG("ecc point Z error");
        return SSL_FATAL_ERROR;
    }

    p->exSet = 1;

    return SSL_SUCCESS;
}

/* EC_KEY wolfSSL -> OpenSSL */
static int SetECKeyExternal(WOLFSSL_EC_KEY* eckey)
{
    ecc_key* key;

    WOLFSSL_ENTER("SetECKeyExternal");

    if (eckey == NULL || eckey->internal == NULL) {
        WOLFSSL_MSG("ec key NULL error");
        return SSL_FATAL_ERROR;
    }

    key = (ecc_key*)eckey->internal;

    /* set group (nid and idx) */
    eckey->group->curve_nid = ecc_sets[key->idx].id;
    eckey->group->curve_idx = key->idx;

    if (eckey->pub_key->internal != NULL) {
        /* set the internal public key */
        if (wc_ecc_copy_point(&key->pubkey,
                             (ecc_point*)eckey->pub_key->internal) != MP_OKAY) {
            WOLFSSL_MSG("SetECKeyExternal ecc_copy_point failed");
            return SSL_FATAL_ERROR;
        }

        /* set the external pubkey (point) */
        if (SetECPointExternal(eckey->pub_key) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECKeyExternal SetECPointExternal failed");
            return SSL_FATAL_ERROR;
        }
    }

    /* set the external privkey */
    if (key->type == ECC_PRIVATEKEY) {
        if (SetIndividualExternal(&eckey->priv_key, &key->k) != SSL_SUCCESS) {
            WOLFSSL_MSG("ec priv key error");
            return SSL_FATAL_ERROR;
        }
    }

    eckey->exSet = 1;

    return SSL_SUCCESS;
}

/* EC_KEY Openssl -> WolfSSL */
static int SetECKeyInternal(WOLFSSL_EC_KEY* eckey)
{
    ecc_key* key;

    WOLFSSL_ENTER("SetECKeyInternal");

    if (eckey == NULL || eckey->internal == NULL) {
        WOLFSSL_MSG("ec key NULL error");
        return SSL_FATAL_ERROR;
    }

    key = (ecc_key*)eckey->internal;

    /* validate group */
    if ((eckey->group->curve_idx < 0) ||
        (wc_ecc_is_valid_idx(eckey->group->curve_idx) == 0)) {
        WOLFSSL_MSG("invalid curve idx");
        return SSL_FATAL_ERROR;
    }

    /* set group (idx of curve and corresponding domain parameters) */
    key->idx = eckey->group->curve_idx;
    key->dp = &ecc_sets[key->idx];

    /* set pubkey (point) */
    if (eckey->pub_key != NULL) {
        if (SetECPointInternal(eckey->pub_key) != SSL_SUCCESS) {
            WOLFSSL_MSG("ec key pub error");
            return SSL_FATAL_ERROR;
        }

        /* public key */
        key->type = ECC_PUBLICKEY;
    }

    /* set privkey */
    if (eckey->priv_key != NULL) {
        if (SetIndividualInternal(eckey->priv_key, &key->k) != SSL_SUCCESS) {
            WOLFSSL_MSG("ec key priv error");
            return SSL_FATAL_ERROR;
        }

        /* private key */
        key->type = ECC_PRIVATEKEY;
    }

    eckey->inSet = 1;

    return SSL_SUCCESS;
}

WOLFSSL_EC_POINT *wolfSSL_EC_KEY_get0_public_key(const WOLFSSL_EC_KEY *key)
{
    WOLFSSL_ENTER("wolfSSL_EC_KEY_get0_public_key");

    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_get0_group Bad arguments");
        return NULL;
    }

    return key->pub_key;
}

const WOLFSSL_EC_GROUP *wolfSSL_EC_KEY_get0_group(const WOLFSSL_EC_KEY *key)
{
    WOLFSSL_ENTER("wolfSSL_EC_KEY_get0_group");

    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_get0_group Bad arguments");
        return NULL;
    }

    return key->group;
}


/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_EC_KEY_set_private_key(WOLFSSL_EC_KEY *key,
                                   const WOLFSSL_BIGNUM *priv_key)
{
    WOLFSSL_ENTER("wolfSSL_EC_KEY_set_private_key");

    if (key == NULL || priv_key == NULL) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

    /* free key if previously set */
    if (key->priv_key != NULL)
        wolfSSL_BN_free(key->priv_key);

    key->priv_key = wolfSSL_BN_dup(priv_key);
    if (key->priv_key == NULL) {
        WOLFSSL_MSG("key ecc priv key NULL");
        return SSL_FAILURE;
    }

    if (SetECKeyInternal(key) != SSL_SUCCESS) {
        WOLFSSL_MSG("SetECKeyInternal failed");
        wolfSSL_BN_free(key->priv_key);
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}


WOLFSSL_BIGNUM *wolfSSL_EC_KEY_get0_private_key(const WOLFSSL_EC_KEY *key)
{
    WOLFSSL_ENTER("wolfSSL_EC_KEY_get0_private_key");

    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_get0_private_key Bad arguments");
        return NULL;
    }

    return key->priv_key;
}

WOLFSSL_EC_KEY *wolfSSL_EC_KEY_new_by_curve_name(int nid)
{
    WOLFSSL_EC_KEY *key;
    int x;

    WOLFSSL_ENTER("wolfSSL_EC_KEY_new_by_curve_name");

    key = wolfSSL_EC_KEY_new();
    if (key == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_new failure");
        return NULL;
    }

    /* set the nid of the curve */
    key->group->curve_nid = nid;

    /* search and set the corresponding internal curve idx */
    for (x = 0; ecc_sets[x].size != 0; x++)
        if (ecc_sets[x].id == key->group->curve_nid) {
            key->group->curve_idx = x;
            break;
        }

    return key;
}

static void InitwolfSSL_ECKey(WOLFSSL_EC_KEY* key)
{
    if (key) {
        key->group    = NULL;
        key->pub_key  = NULL;
        key->priv_key = NULL;
        key->internal = NULL;
        key->inSet    = 0;
        key->exSet    = 0;
    }
}

WOLFSSL_EC_KEY *wolfSSL_EC_KEY_new(void)
{
    WOLFSSL_EC_KEY *external;
    ecc_key* key;

    WOLFSSL_ENTER("wolfSSL_EC_KEY_new");

    external = (WOLFSSL_EC_KEY*)XMALLOC(sizeof(WOLFSSL_EC_KEY), NULL,
                                        DYNAMIC_TYPE_ECC);
    if (external == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_new malloc WOLFSSL_EC_KEY failure");
        return NULL;
    }
    XMEMSET(external, 0, sizeof(WOLFSSL_EC_KEY));

    InitwolfSSL_ECKey(external);

    external->internal = (ecc_key*)XMALLOC(sizeof(ecc_key), NULL,
                                           DYNAMIC_TYPE_ECC);
    if (external->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_new malloc ecc key failure");
        wolfSSL_EC_KEY_free(external);
        return NULL;
    }
    XMEMSET(external->internal, 0, sizeof(ecc_key));

    wc_ecc_init((ecc_key*)external->internal);

    /* public key */
    external->pub_key = (WOLFSSL_EC_POINT*)XMALLOC(sizeof(WOLFSSL_EC_POINT),
                                                   NULL, DYNAMIC_TYPE_ECC);
    if (external->pub_key == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_new malloc WOLFSSL_EC_POINT failure");
        wolfSSL_EC_KEY_free(external);
        return NULL;
    }
    XMEMSET(external->pub_key, 0, sizeof(WOLFSSL_EC_POINT));

    key = (ecc_key*)external->internal;
    external->pub_key->internal = (ecc_point*)&key->pubkey;

    /* curve group */
    external->group = (WOLFSSL_EC_GROUP*)XMALLOC(sizeof(WOLFSSL_EC_GROUP), NULL,
                                                 DYNAMIC_TYPE_ECC);
    if (external->group == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_new malloc WOLFSSL_EC_GROUP failure");
        wolfSSL_EC_KEY_free(external);
        return NULL;
    }
    XMEMSET(external->group, 0, sizeof(WOLFSSL_EC_GROUP));

    /* private key */
    external->priv_key = wolfSSL_BN_new();
    if (external->priv_key == NULL) {
        WOLFSSL_MSG("wolfSSL_BN_new failure");
        wolfSSL_EC_KEY_free(external);
        return NULL;
    }

    return external;
}

void wolfSSL_EC_KEY_free(WOLFSSL_EC_KEY *key)
{
    WOLFSSL_ENTER("wolfSSL_EC_KEY_free");

    if (key != NULL) {
        if (key->internal != NULL) {
            wc_ecc_free((ecc_key*)key->internal);
            XFREE(key->internal, NULL, DYNAMIC_TYPE_ECC);
        }
        wolfSSL_BN_free(key->priv_key);
        wolfSSL_EC_POINT_free(key->pub_key);
        wolfSSL_EC_GROUP_free(key->group);
        InitwolfSSL_ECKey(key); /* set back to NULLs for safety */

        XFREE(key, NULL, DYNAMIC_TYPE_ECC);
        key = NULL;
    }
}

int wolfSSL_EC_KEY_set_group(WOLFSSL_EC_KEY *key, WOLFSSL_EC_GROUP *group)
{
    (void)key;
    (void)group;

    WOLFSSL_ENTER("wolfSSL_EC_KEY_set_group");
    WOLFSSL_MSG("wolfSSL_EC_KEY_set_group TBD");

    return -1;
}

int wolfSSL_EC_KEY_generate_key(WOLFSSL_EC_KEY *key)
{
    int     initTmpRng = 0;
    WC_RNG* rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG* tmpRNG = NULL;
#else
    WC_RNG  tmpRNG[1];
#endif

    WOLFSSL_ENTER("wolfSSL_EC_KEY_generate_key");

    if (key == NULL || key->internal == NULL ||
        key->group == NULL || key->group->curve_idx < 0) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_generate_key Bad arguments");
        return 0;
    }

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return 0;
#endif

    if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("Bad RNG Init, trying global");
        if (initGlobalRNG == 0)
            WOLFSSL_MSG("Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_generate_key failed to set RNG");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return 0;
    }

    if (wc_ecc_make_key_ex(rng, 0, (ecc_key*)key->internal,
                                        key->group->curve_nid) != MP_OKAY) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_generate_key wc_ecc_make_key failed");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return 0;
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (SetECKeyExternal(key) != SSL_SUCCESS) {
        WOLFSSL_MSG("wolfSSL_EC_KEY_generate_key SetECKeyExternal failed");
        return 0;
    }

    return 1;
}

void wolfSSL_EC_KEY_set_asn1_flag(WOLFSSL_EC_KEY *key, int asn1_flag)
{
    (void)key;
    (void)asn1_flag;

    WOLFSSL_ENTER("wolfSSL_EC_KEY_set_asn1_flag");
    WOLFSSL_MSG("wolfSSL_EC_KEY_set_asn1_flag TBD");
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_EC_KEY_set_public_key(WOLFSSL_EC_KEY *key,
                                  const WOLFSSL_EC_POINT *pub)
{
    ecc_point *pub_p, *key_p;

    WOLFSSL_ENTER("wolfSSL_EC_KEY_set_public_key");

    if (key == NULL || key->internal == NULL ||
        pub == NULL || pub->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_get_order Bad arguments");
        return SSL_FAILURE;
    }

    if (key->inSet == 0) {
        if (SetECKeyInternal(key) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECKeyInternal failed");
            return SSL_FAILURE;
        }
    }

    if (pub->inSet == 0) {
        if (SetECPointInternal((WOLFSSL_EC_POINT *)pub) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECPointInternal failed");
            return SSL_FAILURE;
        }
    }

    pub_p = (ecc_point*)pub->internal;
    key_p = (ecc_point*)key->pub_key->internal;

    /* create new point if required */
    if (key_p == NULL)
        key_p = wc_ecc_new_point();

    if (key_p == NULL) {
        WOLFSSL_MSG("key ecc point NULL");
        return SSL_FAILURE;
    }

    if (wc_ecc_copy_point(pub_p, key_p) != MP_OKAY) {
        WOLFSSL_MSG("ecc_copy_point failure");
        return SSL_FAILURE;
    }

    if (SetECKeyExternal(key) != SSL_SUCCESS) {
        WOLFSSL_MSG("SetECKeyInternal failed");
        return SSL_FAILURE;
    }

#if defined(DEBUG_WOLFSSL) && !defined(NO_FILESYSTEM)
    wolfssl_EC_POINT_dump("pub", pub);
    wolfssl_EC_POINT_dump("key->pub_key", key->pub_key);
#endif
    return SSL_SUCCESS;
}
/* End EC_KEY */

#if defined(DEBUG_WOLFSSL) && !defined(NO_FILESYSTEM)
void wolfssl_EC_POINT_dump(const char *msg, const WOLFSSL_EC_POINT *p)
{
    char *num;

    WOLFSSL_ENTER("wolfssl_EC_POINT_dump");

    if (p == NULL) {
        fprintf(stderr, "%s = NULL", msg);
        return ;
    }

    fprintf(stderr, "%s:\n\tinSet=%d, exSet=%d\n", msg, p->inSet, p->exSet);
    num = wolfSSL_BN_bn2hex(p->X);
    fprintf(stderr, "\tX = %s\n", num);
    XFREE(num, NULL, DYNAMIC_TYPE_ECC);
    num = wolfSSL_BN_bn2hex(p->Y);
    fprintf(stderr, "\tY = %s\n", num);
    XFREE(num, NULL, DYNAMIC_TYPE_ECC);
}
#endif

/* Start EC_GROUP */

/* return code compliant with OpenSSL :
 *   0 if equal, 1 if not and -1 in case of error
 */
int wolfSSL_EC_GROUP_cmp(const WOLFSSL_EC_GROUP *a, const WOLFSSL_EC_GROUP *b,
                         WOLFSSL_BN_CTX *ctx)
{
    (void)ctx;

    WOLFSSL_ENTER("wolfSSL_EC_GROUP_cmp");

    if (a == NULL || b == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_cmp Bad arguments");
        return SSL_FATAL_ERROR;
    }

    /* ok */
    if ((a->curve_idx == b->curve_idx) && (a->curve_nid == b->curve_nid))
        return 0;

    /* ko */
    return 1;
}

void wolfSSL_EC_GROUP_free(WOLFSSL_EC_GROUP *group)
{
    WOLFSSL_ENTER("wolfSSL_EC_GROUP_free");

    XFREE(group, NULL, DYNAMIC_TYPE_ECC);
    group = NULL;
}

void wolfSSL_EC_GROUP_set_asn1_flag(WOLFSSL_EC_GROUP *group, int flag)
{
    (void)group;
    (void)flag;

    WOLFSSL_ENTER("wolfSSL_EC_GROUP_set_asn1_flag");
    WOLFSSL_MSG("wolfSSL_EC_GROUP_set_asn1_flag TBD");
}

WOLFSSL_EC_GROUP *wolfSSL_EC_GROUP_new_by_curve_name(int nid)
{
    WOLFSSL_EC_GROUP *g;
    int x;

    WOLFSSL_ENTER("wolfSSL_EC_GROUP_new_by_curve_name");

    /* curve group */
    g = (WOLFSSL_EC_GROUP*) XMALLOC(sizeof(WOLFSSL_EC_GROUP), NULL,
                                    DYNAMIC_TYPE_ECC);
    if (g == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_new_by_curve_name malloc failure");
        return NULL;
    }
    XMEMSET(g, 0, sizeof(WOLFSSL_EC_GROUP));

    /* set the nid of the curve */
    g->curve_nid = nid;

    /* search and set the corresponding internal curve idx */
    for (x = 0; ecc_sets[x].size != 0; x++)
        if (ecc_sets[x].id == g->curve_nid) {
            g->curve_idx = x;
            break;
        }

    return g;
}

/* return code compliant with OpenSSL :
 *   the curve nid if success, 0 if error
 */
int wolfSSL_EC_GROUP_get_curve_name(const WOLFSSL_EC_GROUP *group)
{
    WOLFSSL_ENTER("wolfSSL_EC_GROUP_get_curve_name");

    if (group == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_get_curve_name Bad arguments");
        return SSL_FAILURE;
    }

    return group->curve_nid;
}

/* return code compliant with OpenSSL :
 *   the degree of the curve if success, 0 if error
 */
int wolfSSL_EC_GROUP_get_degree(const WOLFSSL_EC_GROUP *group)
{
    WOLFSSL_ENTER("wolfSSL_EC_GROUP_get_degree");

    if (group == NULL || group->curve_idx < 0) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_get_degree Bad arguments");
        return SSL_FAILURE;
    }

    switch(group->curve_nid) {
        case NID_secp112r1:
        case NID_secp112r2:
            return 112;
        case NID_secp128r1:
        case NID_secp128r2:
            return 128;
        case NID_secp160k1:
        case NID_secp160r1:
        case NID_secp160r2:
        case NID_brainpoolP160r1:
            return 160;
        case NID_secp192k1:
        case NID_brainpoolP192r1:
        case NID_X9_62_prime192v1:
            return 192;
        case NID_secp224k1:
        case NID_secp224r1:
        case NID_brainpoolP224r1:
            return 224;
        case NID_secp256k1:
        case NID_brainpoolP256r1:
        case NID_X9_62_prime256v1:
            return 256;
        case NID_brainpoolP320r1:
            return 320;
        case NID_secp384r1:
        case NID_brainpoolP384r1:
            return 384;
        case NID_secp521r1:
        case NID_brainpoolP512r1:
            return 521;
        default:
            return SSL_FAILURE;
    }
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_EC_GROUP_get_order(const WOLFSSL_EC_GROUP *group,
                               WOLFSSL_BIGNUM *order, WOLFSSL_BN_CTX *ctx)
{
    (void)ctx;

    if (group == NULL || order == NULL || order->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_get_order NULL error");
        return SSL_FAILURE;
    }

    if (mp_init((mp_int*)order->internal) != MP_OKAY) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_get_order mp_init failure");
        return SSL_FAILURE;
    }

    if (mp_read_radix((mp_int*)order->internal,
                      ecc_sets[group->curve_idx].order, 16) != MP_OKAY) {
        WOLFSSL_MSG("wolfSSL_EC_GROUP_get_order mp_read order failure");
        mp_clear((mp_int*)order->internal);
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}
/* End EC_GROUP */

/* Start EC_POINT */

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_ECPoint_i2d(const WOLFSSL_EC_GROUP *group,
                        const WOLFSSL_EC_POINT *p,
                        unsigned char *out, unsigned int *len)
{
    int err;

    WOLFSSL_ENTER("wolfSSL_ECPoint_i2d");

    if (group == NULL || p == NULL || len == NULL) {
        WOLFSSL_MSG("wolfSSL_ECPoint_i2d NULL error");
        return SSL_FAILURE;
    }

    if (p->inSet == 0) {
        WOLFSSL_MSG("No ECPoint internal set, do it");

        if (SetECPointInternal((WOLFSSL_EC_POINT *)p) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECPointInternal SetECPointInternal failed");
            return SSL_FAILURE;
        }
    }

#if defined(DEBUG_WOLFSSL) && !defined(NO_FILESYSTEM)
    if (out != NULL) {
        wolfssl_EC_POINT_dump("i2d p", p);
    }
#endif
    err = wc_ecc_export_point_der(group->curve_idx, (ecc_point*)p->internal,
                                  out, len);
    if (err != MP_OKAY && !(out == NULL && err == LENGTH_ONLY_E)) {
        WOLFSSL_MSG("wolfSSL_ECPoint_i2d wc_ecc_export_point_der failed");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_ECPoint_d2i(unsigned char *in, unsigned int len,
                        const WOLFSSL_EC_GROUP *group, WOLFSSL_EC_POINT *p)
{
    WOLFSSL_ENTER("wolfSSL_ECPoint_d2i");

    if (group == NULL || p == NULL || p->internal == NULL || in == NULL) {
        WOLFSSL_MSG("wolfSSL_ECPoint_d2i NULL error");
        return SSL_FAILURE;
    }

    if (wc_ecc_import_point_der(in, len, group->curve_idx,
                                (ecc_point*)p->internal) != MP_OKAY) {
        WOLFSSL_MSG("wc_ecc_import_point_der failed");
        return SSL_FAILURE;
    }

    if (p->exSet == 0) {
        WOLFSSL_MSG("No ECPoint external set, do it");

        if (SetECPointExternal(p) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECPointExternal failed");
            return SSL_FAILURE;
        }
    }

#if defined(DEBUG_WOLFSSL) && !defined(NO_FILESYSTEM)
    wolfssl_EC_POINT_dump("d2i p", p);
#endif
    return SSL_SUCCESS;
}

WOLFSSL_EC_POINT *wolfSSL_EC_POINT_new(const WOLFSSL_EC_GROUP *group)
{
    WOLFSSL_EC_POINT *p;

    WOLFSSL_ENTER("wolfSSL_EC_POINT_new");

    if (group == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_new NULL error");
        return NULL;
    }

    p = (WOLFSSL_EC_POINT *)XMALLOC(sizeof(WOLFSSL_EC_POINT), NULL,
                                    DYNAMIC_TYPE_ECC);
    if (p == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_new malloc ecc point failure");
        return NULL;
    }
    XMEMSET(p, 0, sizeof(WOLFSSL_EC_POINT));

    p->internal = wc_ecc_new_point();
    if (p->internal == NULL) {
        WOLFSSL_MSG("ecc_new_point failure");
        XFREE(p, NULL, DYNAMIC_TYPE_ECC);
        return NULL;
    }

    return p;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_EC_POINT_get_affine_coordinates_GFp(const WOLFSSL_EC_GROUP *group,
                                                const WOLFSSL_EC_POINT *point,
                                                WOLFSSL_BIGNUM *x,
                                                WOLFSSL_BIGNUM *y,
                                                WOLFSSL_BN_CTX *ctx)
{
    (void)ctx;

    WOLFSSL_ENTER("wolfSSL_EC_POINT_get_affine_coordinates_GFp");

    if (group == NULL || point == NULL || point->internal == NULL ||
        x == NULL || y == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_get_affine_coordinates_GFp NULL error");
        return SSL_FAILURE;
    }

    if (point->inSet == 0) {
        WOLFSSL_MSG("No ECPoint internal set, do it");

        if (SetECPointInternal((WOLFSSL_EC_POINT *)point) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECPointInternal failed");
            return SSL_FAILURE;
        }
    }

    BN_copy(x, point->X);
    BN_copy(y, point->Y);

    return SSL_SUCCESS;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_EC_POINT_mul(const WOLFSSL_EC_GROUP *group, WOLFSSL_EC_POINT *r,
                         const WOLFSSL_BIGNUM *n, const WOLFSSL_EC_POINT *q,
                         const WOLFSSL_BIGNUM *m, WOLFSSL_BN_CTX *ctx)
{
    mp_int a, prime;

    (void)ctx;
    (void)n;

    WOLFSSL_ENTER("wolfSSL_EC_POINT_mul");

    if (group == NULL || r == NULL || r->internal == NULL ||
        q == NULL || q->internal == NULL || m == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_mul NULL error");
        return SSL_FAILURE;
    }

    if (q->inSet == 0) {
        WOLFSSL_MSG("No ECPoint internal set, do it");

        if (SetECPointInternal((WOLFSSL_EC_POINT *)q) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECPointInternal failed");
            return SSL_FAILURE;
        }
    }

    /* read the curve prime and a */
    if (mp_init_multi(&prime, &a, NULL, NULL, NULL, NULL) != MP_OKAY) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_mul init 'prime/A' failed");
        return SSL_FAILURE;
    }
    if (mp_read_radix(&prime, ecc_sets[group->curve_idx].prime, 16) != MP_OKAY){
        WOLFSSL_MSG("wolfSSL_EC_POINT_mul read 'prime' curve value failed");
        return SSL_FAILURE;
    }
    if (mp_read_radix(&a, ecc_sets[group->curve_idx].Af, 16) != MP_OKAY){
        WOLFSSL_MSG("wolfSSL_EC_POINT_mul read 'A' curve value failed");
        return SSL_FAILURE;
    }

    /* r = q * m % prime */
    if (wc_ecc_mulmod((mp_int*)m->internal, (ecc_point*)q->internal,
                      (ecc_point*)r->internal, &a, &prime, 1) != MP_OKAY) {
        WOLFSSL_MSG("ecc_mulmod failure");
        mp_clear(&prime);
        return SSL_FAILURE;
    }

    mp_clear(&a);
    mp_clear(&prime);

    /* set the external value for the computed point */
    if (SetECPointInternal(r) != SSL_SUCCESS) {
        WOLFSSL_MSG("SetECPointInternal failed");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

void wolfSSL_EC_POINT_clear_free(WOLFSSL_EC_POINT *p)
{
    WOLFSSL_ENTER("wolfSSL_EC_POINT_clear_free");

    wolfSSL_EC_POINT_free(p);
}

/* return code compliant with OpenSSL :
 *   0 if equal, 1 if not and -1 in case of error
 */
int wolfSSL_EC_POINT_cmp(const WOLFSSL_EC_GROUP *group,
                         const WOLFSSL_EC_POINT *a, const WOLFSSL_EC_POINT *b,
                         WOLFSSL_BN_CTX *ctx)
{
    int ret;

	(void)ctx;

    WOLFSSL_ENTER("wolfSSL_EC_POINT_cmp");

    if (group == NULL || a == NULL || a->internal == NULL || b == NULL ||
        b->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_cmp Bad arguments");
        return SSL_FATAL_ERROR;
    }

    ret = wc_ecc_cmp_point((ecc_point*)a->internal, (ecc_point*)b->internal);
    if (ret == MP_EQ)
        return 0;
    else if (ret == MP_LT || ret == MP_GT)
        return 1;

    return SSL_FATAL_ERROR;
}

void wolfSSL_EC_POINT_free(WOLFSSL_EC_POINT *p)
{
    WOLFSSL_ENTER("wolfSSL_EC_POINT_free");

    if (p != NULL) {
        if (p->internal == NULL) {
            wc_ecc_del_point((ecc_point*)p->internal);
            XFREE(p->internal, NULL, DYNAMIC_TYPE_ECC);
            p->internal = NULL;
        }

        wolfSSL_BN_free(p->X);
        wolfSSL_BN_free(p->Y);
        wolfSSL_BN_free(p->Z);
        p->X = NULL;
        p->Y = NULL;
        p->Z = NULL;
        p->inSet = p->exSet = 0;

        XFREE(p, NULL, DYNAMIC_TYPE_ECC);
        p = NULL;
    }
}

/* return code compliant with OpenSSL :
 *   1 if point at infinity, 0 else
 */
int wolfSSL_EC_POINT_is_at_infinity(const WOLFSSL_EC_GROUP *group,
                                    const WOLFSSL_EC_POINT *point)
{
    int ret;

    WOLFSSL_ENTER("wolfSSL_EC_POINT_is_at_infinity");

    if (group == NULL || point == NULL || point->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_EC_POINT_is_at_infinity NULL error");
        return SSL_FAILURE;
    }
    if (point->inSet == 0) {
        WOLFSSL_MSG("No ECPoint internal set, do it");

        if (SetECPointInternal((WOLFSSL_EC_POINT *)point) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECPointInternal failed");
            return SSL_FAILURE;
        }
    }

    ret = wc_ecc_point_is_at_infinity((ecc_point*)point->internal);
    if (ret <= 0) {
        WOLFSSL_MSG("ecc_point_is_at_infinity failure");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}

/* End EC_POINT */

/* Start ECDSA_SIG */
void wolfSSL_ECDSA_SIG_free(WOLFSSL_ECDSA_SIG *sig)
{
    WOLFSSL_ENTER("wolfSSL_ECDSA_SIG_free");

    if (sig) {
        wolfSSL_BN_free(sig->r);
        wolfSSL_BN_free(sig->s);

        XFREE(sig, NULL, DYNAMIC_TYPE_ECC);
    }
}

WOLFSSL_ECDSA_SIG *wolfSSL_ECDSA_SIG_new(void)
{
    WOLFSSL_ECDSA_SIG *sig;

    WOLFSSL_ENTER("wolfSSL_ECDSA_SIG_new");

    sig = (WOLFSSL_ECDSA_SIG*) XMALLOC(sizeof(WOLFSSL_ECDSA_SIG), NULL,
                                       DYNAMIC_TYPE_ECC);
    if (sig == NULL) {
        WOLFSSL_MSG("wolfSSL_ECDSA_SIG_new malloc ECDSA signature failure");
        return NULL;
    }

    sig->s = NULL;
    sig->r = wolfSSL_BN_new();
    if (sig->r == NULL) {
        WOLFSSL_MSG("wolfSSL_ECDSA_SIG_new malloc ECDSA r failure");
        wolfSSL_ECDSA_SIG_free(sig);
        return NULL;
    }

    sig->s = wolfSSL_BN_new();
    if (sig->s == NULL) {
        WOLFSSL_MSG("wolfSSL_ECDSA_SIG_new malloc ECDSA s failure");
        wolfSSL_ECDSA_SIG_free(sig);
        return NULL;
    }

    return sig;
}

/* return signature structure on success, NULL otherwise */
WOLFSSL_ECDSA_SIG *wolfSSL_ECDSA_do_sign(const unsigned char *d, int dlen,
                                         WOLFSSL_EC_KEY *key)
{
    WOLFSSL_ECDSA_SIG *sig = NULL;
    int     initTmpRng = 0;
    WC_RNG* rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG* tmpRNG = NULL;
#else
    WC_RNG  tmpRNG[1];
#endif

    WOLFSSL_ENTER("wolfSSL_ECDSA_do_sign");

    if (d == NULL || key == NULL || key->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_ECDSA_do_sign Bad arguments");
        return NULL;
    }

    /* set internal key if not done */
    if (key->inSet == 0)
    {
        WOLFSSL_MSG("wolfSSL_ECDSA_do_sign No EC key internal set, do it");

        if (SetECKeyInternal(key) != SSL_SUCCESS) {
            WOLFSSL_MSG("wolfSSL_ECDSA_do_sign SetECKeyInternal failed");
            return NULL;
        }
    }

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return NULL;
#endif

    if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("wolfSSL_ECDSA_do_sign Bad RNG Init, trying global");
        if (initGlobalRNG == 0)
            WOLFSSL_MSG("wolfSSL_ECDSA_do_sign Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng) {
        mp_int sig_r, sig_s;

        if (mp_init_multi(&sig_r, &sig_s, NULL, NULL, NULL, NULL) == MP_OKAY) {
            if (wc_ecc_sign_hash_ex(d, dlen, rng, (ecc_key*)key->internal,
                                    &sig_r, &sig_s) != MP_OKAY) {
                WOLFSSL_MSG("wc_ecc_sign_hash_ex failed");
            }
            else {
                /* put signature blob in ECDSA structure */
                sig = wolfSSL_ECDSA_SIG_new();
                if (sig == NULL)
                    WOLFSSL_MSG("wolfSSL_ECDSA_SIG_new failed");
                else if (SetIndividualExternal(&(sig->r), &sig_r)!=SSL_SUCCESS){
                    WOLFSSL_MSG("ecdsa r key error");
                    wolfSSL_ECDSA_SIG_free(sig);
                    sig = NULL;
                }
                else if (SetIndividualExternal(&(sig->s), &sig_s)!=SSL_SUCCESS){
                    WOLFSSL_MSG("ecdsa s key error");
                    wolfSSL_ECDSA_SIG_free(sig);
                    sig = NULL;
                }

                mp_clear(&sig_r);
                mp_clear(&sig_s);
            }
        }
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);
#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return sig;
}

/* return code compliant with OpenSSL :
 *   1 for a valid signature, 0 for an invalid signature and -1 on error
 */
int wolfSSL_ECDSA_do_verify(const unsigned char *d, int dlen,
                            const WOLFSSL_ECDSA_SIG *sig, WOLFSSL_EC_KEY *key)
{
    int check_sign = 0;

    WOLFSSL_ENTER("wolfSSL_ECDSA_do_verify");

    if (d == NULL || sig == NULL || key == NULL || key->internal == NULL) {
        WOLFSSL_MSG("wolfSSL_ECDSA_do_verify Bad arguments");
        return SSL_FATAL_ERROR;
    }

    /* set internal key if not done */
    if (key->inSet == 0)
    {
        WOLFSSL_MSG("No EC key internal set, do it");

        if (SetECKeyInternal(key) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECKeyInternal failed");
            return SSL_FATAL_ERROR;
        }
    }

    if (wc_ecc_verify_hash_ex((mp_int*)sig->r->internal,
                              (mp_int*)sig->s->internal, d, dlen, &check_sign,
                              (ecc_key *)key->internal) != MP_OKAY) {
        WOLFSSL_MSG("wc_ecc_verify_hash failed");
        return SSL_FATAL_ERROR;
    }
    else if (check_sign == 0) {
        WOLFSSL_MSG("wc_ecc_verify_hash incorrect signature detected");
        return SSL_FAILURE;
    }

    return SSL_SUCCESS;
}
/* End ECDSA_SIG */

/* Start ECDH */
/* return code compliant with OpenSSL :
 *   length of computed key if success, -1 if error
 */
int wolfSSL_ECDH_compute_key(void *out, size_t outlen,
                             const WOLFSSL_EC_POINT *pub_key,
                             WOLFSSL_EC_KEY *ecdh,
                             void *(*KDF) (const void *in, size_t inlen,
                                           void *out, size_t *outlen))
{
    word32 len;
    (void)KDF;

    (void)KDF;

	WOLFSSL_ENTER("wolfSSL_ECDH_compute_key");

    if (out == NULL || pub_key == NULL || pub_key->internal == NULL ||
        ecdh == NULL || ecdh->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FATAL_ERROR;
    }

    /* set internal key if not done */
    if (ecdh->inSet == 0)
    {
        WOLFSSL_MSG("No EC key internal set, do it");

        if (SetECKeyInternal(ecdh) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetECKeyInternal failed");
            return SSL_FATAL_ERROR;
        }
    }

    len = (word32)outlen;

    if (wc_ecc_shared_secret_ssh((ecc_key*)ecdh->internal,
                                 (ecc_point*)pub_key->internal,
                                 (byte *)out, &len) != MP_OKAY) {
        WOLFSSL_MSG("wc_ecc_shared_secret failed");
        return SSL_FATAL_ERROR;
    }

    return len;
}
/* End ECDH */

#if !defined(NO_FILESYSTEM)
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_EC_PUBKEY(FILE *fp, WOLFSSL_EC_KEY *x)
{
    (void)fp;
    (void)x;

    WOLFSSL_MSG("wolfSSL_PEM_write_EC_PUBKEY not implemented");

    return SSL_FAILURE;
}
#endif /* NO_FILESYSTEM */

#if defined(WOLFSSL_KEY_GEN)

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_bio_ECPrivateKey(WOLFSSL_BIO* bio, WOLFSSL_EC_KEY* ecc,
                                       const EVP_CIPHER* cipher,
                                       unsigned char* passwd, int len,
                                       pem_password_cb cb, void* arg)
{
    (void)bio;
    (void)ecc;
    (void)cipher;
    (void)passwd;
    (void)len;
    (void)cb;
    (void)arg;

    WOLFSSL_MSG("wolfSSL_PEM_write_bio_ECPrivateKey not implemented");

    return SSL_FAILURE;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_mem_ECPrivateKey(WOLFSSL_EC_KEY* ecc,
                                       const EVP_CIPHER* cipher,
                                       unsigned char* passwd, int passwdSz,
                                       unsigned char **pem, int *plen)
{
    byte *derBuf, *tmp, *cipherInfo = NULL;
    int  der_max_len = 0, derSz = 0;

    WOLFSSL_MSG("wolfSSL_PEM_write_mem_ECPrivateKey");

    if (pem == NULL || plen == NULL || ecc == NULL || ecc->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FAILURE;
    }

    if (ecc->inSet == 0) {
        WOLFSSL_MSG("No ECC internal set, do it");

        if (SetECKeyInternal(ecc) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetDsaInternal failed");
            return SSL_FAILURE;
        }
    }

    /* 4 > size of pub, priv + ASN.1 additional informations
     */
    der_max_len = 4 * wc_ecc_size((ecc_key*)ecc->internal) + AES_BLOCK_SIZE;

    derBuf = (byte*)XMALLOC(der_max_len, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (derBuf == NULL) {
        WOLFSSL_MSG("malloc failed");
        return SSL_FAILURE;
    }

    /* Key to DER */
    derSz = wc_EccKeyToDer((ecc_key*)ecc->internal, derBuf, der_max_len);
    if (derSz < 0) {
        WOLFSSL_MSG("wc_DsaKeyToDer failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    /* encrypt DER buffer if required */
    if (passwd != NULL && passwdSz > 0 && cipher != NULL) {
        int ret;

        ret = EncryptDerKey(derBuf, &derSz, cipher,
                            passwd, passwdSz, &cipherInfo);
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("EncryptDerKey failed");
            XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            return ret;
        }

        /* tmp buffer with a max size */
        *plen = (derSz * 2) + sizeof(BEGIN_EC_PRIV) +
        sizeof(END_EC_PRIV) + HEADER_ENCRYPTED_KEY_SIZE;
    }
    else /* tmp buffer with a max size */
        *plen = (derSz * 2) + sizeof(BEGIN_EC_PRIV) + sizeof(END_EC_PRIV);

    tmp = (byte*)XMALLOC(*plen, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmp == NULL) {
        WOLFSSL_MSG("malloc failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (cipherInfo != NULL)
            XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    /* DER to PEM */
    *plen = wc_DerToPemEx(derBuf, derSz, tmp, *plen, cipherInfo, ECC_PRIVATEKEY_TYPE);
    if (*plen <= 0) {
        WOLFSSL_MSG("wc_DerToPemEx failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (cipherInfo != NULL)
            XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (cipherInfo != NULL)
        XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);

    *pem = (byte*)XMALLOC((*plen)+1, NULL, DYNAMIC_TYPE_KEY);
    if (*pem == NULL) {
        WOLFSSL_MSG("malloc failed");
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XMEMSET(*pem, 0, (*plen)+1);

    if (XMEMCPY(*pem, tmp, *plen) == NULL) {
        WOLFSSL_MSG("XMEMCPY failed");
        XFREE(pem, NULL, DYNAMIC_TYPE_KEY);
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);

    return SSL_SUCCESS;
}

#ifndef NO_FILESYSTEM
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_ECPrivateKey(FILE *fp, WOLFSSL_EC_KEY *ecc,
                                   const EVP_CIPHER *enc,
                                   unsigned char *kstr, int klen,
                                   pem_password_cb *cb, void *u)
{
    byte *pem;
    int  plen, ret;

    (void)cb;
    (void)u;

    WOLFSSL_MSG("wolfSSL_PEM_write_ECPrivateKey");

    if (fp == NULL || ecc == NULL || ecc->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FAILURE;
    }

    ret = wolfSSL_PEM_write_mem_ECPrivateKey(ecc, enc, kstr, klen, &pem, &plen);
    if (ret != SSL_SUCCESS) {
        WOLFSSL_MSG("wolfSSL_PEM_write_mem_ECPrivateKey failed");
        return SSL_FAILURE;
    }

    ret = (int)XFWRITE(pem, plen, 1, fp);
    if (ret != 1) {
        WOLFSSL_MSG("ECC private key file write failed");
        return SSL_FAILURE;
    }

    XFREE(pem, NULL, DYNAMIC_TYPE_KEY);
    return SSL_SUCCESS;
}

#endif /* NO_FILESYSTEM */
#endif /* defined(WOLFSSL_KEY_GEN) */

#endif /* HAVE_ECC */


#ifndef NO_DSA

#if defined(WOLFSSL_KEY_GEN)

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_bio_DSAPrivateKey(WOLFSSL_BIO* bio, WOLFSSL_DSA* dsa,
                                       const EVP_CIPHER* cipher,
                                       unsigned char* passwd, int len,
                                       pem_password_cb cb, void* arg)
{
    (void)bio;
    (void)dsa;
    (void)cipher;
    (void)passwd;
    (void)len;
    (void)cb;
    (void)arg;

    WOLFSSL_MSG("wolfSSL_PEM_write_bio_DSAPrivateKey not implemented");

    return SSL_FAILURE;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_mem_DSAPrivateKey(WOLFSSL_DSA* dsa,
                                        const EVP_CIPHER* cipher,
                                        unsigned char* passwd, int passwdSz,
                                        unsigned char **pem, int *plen)
{
    byte *derBuf, *tmp, *cipherInfo = NULL;
    int  der_max_len = 0, derSz = 0;

    WOLFSSL_MSG("wolfSSL_PEM_write_mem_DSAPrivateKey");

    if (pem == NULL || plen == NULL || dsa == NULL || dsa->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FAILURE;
    }

    if (dsa->inSet == 0) {
        WOLFSSL_MSG("No DSA internal set, do it");

        if (SetDsaInternal(dsa) != SSL_SUCCESS) {
            WOLFSSL_MSG("SetDsaInternal failed");
            return SSL_FAILURE;
        }
    }

    /* 4 > size of pub, priv, p, q, g + ASN.1 additional informations
     */
    der_max_len = 4 * wolfSSL_BN_num_bytes(dsa->g) + AES_BLOCK_SIZE;

    derBuf = (byte*)XMALLOC(der_max_len, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (derBuf == NULL) {
        WOLFSSL_MSG("malloc failed");
        return SSL_FAILURE;
    }

    /* Key to DER */
    derSz = wc_DsaKeyToDer((DsaKey*)dsa->internal, derBuf, der_max_len);
    if (derSz < 0) {
        WOLFSSL_MSG("wc_DsaKeyToDer failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    /* encrypt DER buffer if required */
    if (passwd != NULL && passwdSz > 0 && cipher != NULL) {
        int ret;

        ret = EncryptDerKey(derBuf, &derSz, cipher,
                            passwd, passwdSz, &cipherInfo);
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("EncryptDerKey failed");
            XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
            return ret;
        }

        /* tmp buffer with a max size */
        *plen = (derSz * 2) + sizeof(BEGIN_DSA_PRIV) +
        sizeof(END_DSA_PRIV) + HEADER_ENCRYPTED_KEY_SIZE;
    }
    else /* tmp buffer with a max size */
        *plen = (derSz * 2) + sizeof(BEGIN_DSA_PRIV) + sizeof(END_DSA_PRIV);

    tmp = (byte*)XMALLOC(*plen, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmp == NULL) {
        WOLFSSL_MSG("malloc failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (cipherInfo != NULL)
            XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }

    /* DER to PEM */
    *plen = wc_DerToPemEx(derBuf, derSz, tmp, *plen, cipherInfo, DSA_PRIVATEKEY_TYPE);
    if (*plen <= 0) {
        WOLFSSL_MSG("wc_DerToPemEx failed");
        XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (cipherInfo != NULL)
            XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XFREE(derBuf, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (cipherInfo != NULL)
        XFREE(cipherInfo, NULL, DYNAMIC_TYPE_TMP_BUFFER);

    *pem = (byte*)XMALLOC((*plen)+1, NULL, DYNAMIC_TYPE_KEY);
    if (*pem == NULL) {
        WOLFSSL_MSG("malloc failed");
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XMEMSET(*pem, 0, (*plen)+1);

    if (XMEMCPY(*pem, tmp, *plen) == NULL) {
        WOLFSSL_MSG("XMEMCPY failed");
        XFREE(pem, NULL, DYNAMIC_TYPE_KEY);
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        return SSL_FAILURE;
    }
    XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);

    return SSL_SUCCESS;
}

#ifndef NO_FILESYSTEM
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_DSAPrivateKey(FILE *fp, WOLFSSL_DSA *dsa,
                                    const EVP_CIPHER *enc,
                                    unsigned char *kstr, int klen,
                                    pem_password_cb *cb, void *u)
{
    byte *pem;
    int  plen, ret;

    (void)cb;
    (void)u;

    WOLFSSL_MSG("wolfSSL_PEM_write_DSAPrivateKey");

    if (fp == NULL || dsa == NULL || dsa->internal == NULL) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FAILURE;
    }

    ret = wolfSSL_PEM_write_mem_DSAPrivateKey(dsa, enc, kstr, klen, &pem, &plen);
    if (ret != SSL_SUCCESS) {
        WOLFSSL_MSG("wolfSSL_PEM_write_mem_DSAPrivateKey failed");
        return SSL_FAILURE;
    }

    ret = (int)XFWRITE(pem, plen, 1, fp);
    if (ret != 1) {
        WOLFSSL_MSG("DSA private key file write failed");
        return SSL_FAILURE;
    }

    XFREE(pem, NULL, DYNAMIC_TYPE_KEY);
    return SSL_SUCCESS;
}

#endif /* NO_FILESYSTEM */
#endif /* defined(WOLFSSL_KEY_GEN) */

#ifndef NO_FILESYSTEM
/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_DSA_PUBKEY(FILE *fp, WOLFSSL_DSA *x)
{
    (void)fp;
    (void)x;

    WOLFSSL_MSG("wolfSSL_PEM_write_DSA_PUBKEY not implemented");

    return SSL_FAILURE;
}
#endif /* NO_FILESYSTEM */

#endif /* #ifndef NO_DSA */

WOLFSSL_EVP_PKEY* wolfSSL_PEM_read_bio_PrivateKey(WOLFSSL_BIO* bio,
                    WOLFSSL_EVP_PKEY** key, pem_password_cb cb, void* arg)
{
    (void)bio;
    (void)key;
    (void)cb;
    (void)arg;

    WOLFSSL_MSG("wolfSSL_PEM_read_bio_PrivateKey not implemented");

    return NULL;
}


int wolfSSL_EVP_PKEY_type(int type)
{
    (void)type;

    WOLFSSL_MSG("wolfSSL_EVP_PKEY_type not implemented");

    return SSL_FATAL_ERROR;
}


#if !defined(NO_FILESYSTEM)
WOLFSSL_EVP_PKEY *wolfSSL_PEM_read_PUBKEY(FILE *fp, EVP_PKEY **x,
                                          pem_password_cb *cb, void *u)
{
    (void)fp;
    (void)x;
    (void)cb;
    (void)u;

    WOLFSSL_MSG("wolfSSL_PEM_read_PUBKEY not implemented");

    return NULL;
}
#endif /* NO_FILESYSTEM */

#ifndef NO_RSA

#if !defined(NO_FILESYSTEM)
WOLFSSL_RSA *wolfSSL_PEM_read_RSAPublicKey(FILE *fp, WOLFSSL_RSA **x,
                                           pem_password_cb *cb, void *u)
{
    (void)fp;
    (void)x;
    (void)cb;
    (void)u;

    WOLFSSL_MSG("wolfSSL_PEM_read_RSAPublicKey not implemented");

    return NULL;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_RSAPublicKey(FILE *fp, WOLFSSL_RSA *x)
{
    (void)fp;
    (void)x;

    WOLFSSL_MSG("wolfSSL_PEM_write_RSAPublicKey not implemented");

    return SSL_FAILURE;
}

/* return code compliant with OpenSSL :
 *   1 if success, 0 if error
 */
int wolfSSL_PEM_write_RSA_PUBKEY(FILE *fp, WOLFSSL_RSA *x)
{
    (void)fp;
    (void)x;

    WOLFSSL_MSG("wolfSSL_PEM_write_RSA_PUBKEY not implemented");

    return SSL_FAILURE;
}
#endif /* NO_FILESYSTEM */

/* return SSL_SUCCESS if success, SSL_FATAL_ERROR if error */
int wolfSSL_RSA_LoadDer(WOLFSSL_RSA* rsa, const unsigned char* derBuf, int derSz)
{
    word32 idx = 0;
    int    ret;

    WOLFSSL_ENTER("wolfSSL_RSA_LoadDer");

    if (rsa == NULL || rsa->internal == NULL || derBuf == NULL || derSz <= 0) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FATAL_ERROR;
    }

    ret = wc_RsaPrivateKeyDecode(derBuf, &idx, (RsaKey*)rsa->internal, derSz);
    if (ret < 0) {
        WOLFSSL_MSG("RsaPrivateKeyDecode failed");
        return SSL_FATAL_ERROR;
    }

    if (SetRsaExternal(rsa) != SSL_SUCCESS) {
        WOLFSSL_MSG("SetRsaExternal failed");
        return SSL_FATAL_ERROR;
    }

    rsa->inSet = 1;

    return SSL_SUCCESS;
}
#endif /* NO_RSA */


#ifndef NO_DSA
/* return SSL_SUCCESS if success, SSL_FATAL_ERROR if error */
int wolfSSL_DSA_LoadDer(WOLFSSL_DSA* dsa, const unsigned char* derBuf, int derSz)
{
    word32 idx = 0;
    int    ret;

    WOLFSSL_ENTER("wolfSSL_DSA_LoadDer");

    if (dsa == NULL || dsa->internal == NULL || derBuf == NULL || derSz <= 0) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FATAL_ERROR;
    }

    ret = DsaPrivateKeyDecode(derBuf, &idx, (DsaKey*)dsa->internal, derSz);
    if (ret < 0) {
        WOLFSSL_MSG("DsaPrivateKeyDecode failed");
        return SSL_FATAL_ERROR;
    }

    if (SetDsaExternal(dsa) != SSL_SUCCESS) {
        WOLFSSL_MSG("SetDsaExternal failed");
        return SSL_FATAL_ERROR;
    }

    dsa->inSet = 1;

    return SSL_SUCCESS;
}
#endif /* NO_DSA */

#ifdef HAVE_ECC
/* return SSL_SUCCESS if success, SSL_FATAL_ERROR if error */
int wolfSSL_EC_KEY_LoadDer(WOLFSSL_EC_KEY* key,
                           const unsigned char* derBuf,  int derSz)
{
    word32 idx = 0;
    int    ret;

    WOLFSSL_ENTER("wolfSSL_EC_KEY_LoadDer");

    if (key == NULL || key->internal == NULL || derBuf == NULL || derSz <= 0) {
        WOLFSSL_MSG("Bad function arguments");
        return SSL_FATAL_ERROR;
    }

    ret = wc_EccPrivateKeyDecode(derBuf, &idx, (ecc_key*)key->internal, derSz);
    if (ret < 0) {
        WOLFSSL_MSG("wc_EccPrivateKeyDecode failed");
        return SSL_FATAL_ERROR;
    }

    if (SetECKeyExternal(key) != SSL_SUCCESS) {
        WOLFSSL_MSG("SetECKeyExternal failed");
        return SSL_FATAL_ERROR;
    }

    key->inSet = 1;

    return SSL_SUCCESS;
}
#endif /* HAVE_ECC */

#endif /* OPENSSL_EXTRA */


#ifdef SESSION_CERTS


/* Get peer's certificate chain */
WOLFSSL_X509_CHAIN* wolfSSL_get_peer_chain(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_get_peer_chain");
    if (ssl)
        return &ssl->session.chain;

    return 0;
}


/* Get peer's certificate chain total count */
int wolfSSL_get_chain_count(WOLFSSL_X509_CHAIN* chain)
{
    WOLFSSL_ENTER("wolfSSL_get_chain_count");
    if (chain)
        return chain->count;

    return 0;
}


/* Get peer's ASN.1 DER certificate at index (idx) length in bytes */
int wolfSSL_get_chain_length(WOLFSSL_X509_CHAIN* chain, int idx)
{
    WOLFSSL_ENTER("wolfSSL_get_chain_length");
    if (chain)
        return chain->certs[idx].length;

    return 0;
}


/* Get peer's ASN.1 DER certificate at index (idx) */
byte* wolfSSL_get_chain_cert(WOLFSSL_X509_CHAIN* chain, int idx)
{
    WOLFSSL_ENTER("wolfSSL_get_chain_cert");
    if (chain)
        return chain->certs[idx].buffer;

    return 0;
}


/* Get peer's wolfSSL X509 certificate at index (idx) */
WOLFSSL_X509* wolfSSL_get_chain_X509(WOLFSSL_X509_CHAIN* chain, int idx)
{
    int          ret;
    WOLFSSL_X509* x509 = NULL;
#ifdef WOLFSSL_SMALL_STACK
    DecodedCert* cert = NULL;
#else
    DecodedCert  cert[1];
#endif

    WOLFSSL_ENTER("wolfSSL_get_chain_X509");
    if (chain != NULL) {
    #ifdef WOLFSSL_SMALL_STACK
        cert = (DecodedCert*)XMALLOC(sizeof(DecodedCert), NULL,
                                                       DYNAMIC_TYPE_TMP_BUFFER);
        if (cert != NULL)
    #endif
        {
            InitDecodedCert(cert, chain->certs[idx].buffer,
                                  chain->certs[idx].length, NULL);

            if ((ret = ParseCertRelative(cert, CERT_TYPE, 0, NULL)) != 0)
                WOLFSSL_MSG("Failed to parse cert");
            else {
                x509 = (WOLFSSL_X509*)XMALLOC(sizeof(WOLFSSL_X509), NULL,
                                                             DYNAMIC_TYPE_X509);
                if (x509 == NULL) {
                    WOLFSSL_MSG("Failed alloc X509");
                }
                else {
                    InitX509(x509, 1, NULL);

                    if ((ret = CopyDecodedToX509(x509, cert)) != 0) {
                        WOLFSSL_MSG("Failed to copy decoded");
                        XFREE(x509, NULL, DYNAMIC_TYPE_X509);
                        x509 = NULL;
                    }
                }
            }

            FreeDecodedCert(cert);
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(cert, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif
        }
    }

    return x509;
}


/* Get peer's PEM certificate at index (idx), output to buffer if inLen big
   enough else return error (-1). If buffer is NULL only calculate
   outLen. Output length is in *outLen SSL_SUCCESS on ok */
int  wolfSSL_get_chain_cert_pem(WOLFSSL_X509_CHAIN* chain, int idx,
                               unsigned char* buf, int inLen, int* outLen)
{
    const char header[] = "-----BEGIN CERTIFICATE-----\n";
    const char footer[] = "-----END CERTIFICATE-----\n";

    int headerLen = sizeof(header) - 1;
    int footerLen = sizeof(footer) - 1;
    int i;
    int err;
    word32 szNeeded = 0;

    WOLFSSL_ENTER("wolfSSL_get_chain_cert_pem");
    if (!chain || !outLen || idx < 0 || idx >= wolfSSL_get_chain_count(chain))
        return BAD_FUNC_ARG;

    /* Null output buffer return size needed in outLen */
    if(!buf) {
        if(Base64_Encode(chain->certs[idx].buffer, chain->certs[idx].length,
                    NULL, &szNeeded) != LENGTH_ONLY_E)
            return SSL_FAILURE;
        *outLen = szNeeded + headerLen + footerLen;
        return LENGTH_ONLY_E;
    }

    /* don't even try if inLen too short */
    if (inLen < headerLen + footerLen + chain->certs[idx].length)
        return BAD_FUNC_ARG;

    /* header */
    if (XMEMCPY(buf, header, headerLen) == NULL)
        return SSL_FATAL_ERROR;

    i = headerLen;

    /* body */
    *outLen = inLen;  /* input to Base64_Encode */
    if ( (err = Base64_Encode(chain->certs[idx].buffer,
                       chain->certs[idx].length, buf + i, (word32*)outLen)) < 0)
        return err;
    i += *outLen;

    /* footer */
    if ( (i + footerLen) > inLen)
        return BAD_FUNC_ARG;
    if (XMEMCPY(buf + i, footer, footerLen) == NULL)
        return SSL_FATAL_ERROR;
    *outLen += headerLen + footerLen;

    return SSL_SUCCESS;
}


/* get session ID */
const byte* wolfSSL_get_sessionID(const WOLFSSL_SESSION* session)
{
    WOLFSSL_ENTER("wolfSSL_get_sessionID");
    if (session)
        return session->sessionID;

    return NULL;
}


#endif /* SESSION_CERTS */

#ifdef HAVE_FUZZER
void wolfSSL_SetFuzzerCb(WOLFSSL* ssl, CallbackFuzzer cbf, void* fCtx)
{
    if (ssl) {
        ssl->fuzzerCb  = cbf;
        ssl->fuzzerCtx = fCtx;
    }
}
#endif

#ifndef NO_CERTS
#ifdef  HAVE_PK_CALLBACKS

#ifdef HAVE_ECC

void  wolfSSL_CTX_SetEccSignCb(WOLFSSL_CTX* ctx, CallbackEccSign cb)
{
    if (ctx)
        ctx->EccSignCb = cb;
}


void  wolfSSL_SetEccSignCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->EccSignCtx = ctx;
}


void* wolfSSL_GetEccSignCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->EccSignCtx;

    return NULL;
}


void  wolfSSL_CTX_SetEccVerifyCb(WOLFSSL_CTX* ctx, CallbackEccVerify cb)
{
    if (ctx)
        ctx->EccVerifyCb = cb;
}


void  wolfSSL_SetEccVerifyCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->EccVerifyCtx = ctx;
}


void* wolfSSL_GetEccVerifyCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->EccVerifyCtx;

    return NULL;
}

#endif /* HAVE_ECC */

#ifndef NO_RSA

void  wolfSSL_CTX_SetRsaSignCb(WOLFSSL_CTX* ctx, CallbackRsaSign cb)
{
    if (ctx)
        ctx->RsaSignCb = cb;
}


void  wolfSSL_SetRsaSignCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->RsaSignCtx = ctx;
}


void* wolfSSL_GetRsaSignCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->RsaSignCtx;

    return NULL;
}


void  wolfSSL_CTX_SetRsaVerifyCb(WOLFSSL_CTX* ctx, CallbackRsaVerify cb)
{
    if (ctx)
        ctx->RsaVerifyCb = cb;
}


void  wolfSSL_SetRsaVerifyCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->RsaVerifyCtx = ctx;
}


void* wolfSSL_GetRsaVerifyCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->RsaVerifyCtx;

    return NULL;
}

void  wolfSSL_CTX_SetRsaEncCb(WOLFSSL_CTX* ctx, CallbackRsaEnc cb)
{
    if (ctx)
        ctx->RsaEncCb = cb;
}


void  wolfSSL_SetRsaEncCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->RsaEncCtx = ctx;
}


void* wolfSSL_GetRsaEncCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->RsaEncCtx;

    return NULL;
}

void  wolfSSL_CTX_SetRsaDecCb(WOLFSSL_CTX* ctx, CallbackRsaDec cb)
{
    if (ctx)
        ctx->RsaDecCb = cb;
}


void  wolfSSL_SetRsaDecCtx(WOLFSSL* ssl, void *ctx)
{
    if (ssl)
        ssl->RsaDecCtx = ctx;
}


void* wolfSSL_GetRsaDecCtx(WOLFSSL* ssl)
{
    if (ssl)
        return ssl->RsaDecCtx;

    return NULL;
}


#endif /* NO_RSA */

#endif /* HAVE_PK_CALLBACKS */
#endif /* NO_CERTS */


#ifdef WOLFSSL_HAVE_WOLFSCEP
    /* Used by autoconf to see if wolfSCEP is available */
    void wolfSSL_wolfSCEP(void) {}
#endif


#ifdef WOLFSSL_HAVE_CERT_SERVICE
    /* Used by autoconf to see if cert service is available */
    void wolfSSL_cert_service(void) {}
#endif


#ifdef OPENSSL_EXTRA /*Lighttp compatibility*/
#if defined(HAVE_LIGHTY) || defined(WOLFSSL_MYSQL_COMPATIBLE)

    unsigned char *wolfSSL_SHA1(const unsigned char *d, size_t n, unsigned char *md)
    {
        (void) *d; (void) n; (void) *md;
        WOLFSSL_ENTER("wolfSSL_SHA1");
        WOLFSSL_STUB("wolfssl_SHA1");

        return NULL;
    }

    char wolfSSL_CTX_use_certificate(WOLFSSL_CTX *ctx, WOLFSSL_X509 *x) {
        (void)ctx;
        (void)x;
        WOLFSSL_ENTER("wolfSSL_CTX_use_certificate");
        WOLFSSL_STUB("wolfSSL_CTX_use_certificate");

        return 0;
    }

    int wolfSSL_CTX_use_PrivateKey(WOLFSSL_CTX *ctx, WOLFSSL_EVP_PKEY *pkey) {
        (void)ctx;
        (void)pkey;
        WOLFSSL_ENTER("wolfSSL_CTX_use_PrivateKey");
        WOLFSSL_STUB("wolfSSL_CTX_use_PrivateKey");

        return 0;
    }


    int wolfSSL_BIO_read_filename(WOLFSSL_BIO *b, const char *name) {
        (void)b;
        (void)name;
        WOLFSSL_ENTER("wolfSSL_BIO_read_filename");
        WOLFSSL_STUB("wolfSSL_BIO_read_filename");

        return 0;
    }

    WOLFSSL_BIO_METHOD* wolfSSL_BIO_s_file(void) {
        WOLFSSL_ENTER("wolfSSL_BIO_s_file");
        WOLFSSL_STUB("wolfSSL_BIO_s_file");

        return NULL;
    }

    const char * wolf_OBJ_nid2sn(int n) {
        (void)n;
        WOLFSSL_ENTER("wolf_OBJ_nid2sn");
        WOLFSSL_STUB("wolf_OBJ_nid2sn");

        return 0;
    }

    int wolf_OBJ_obj2nid(const WOLFSSL_ASN1_OBJECT *o) {
        (void)o;
        WOLFSSL_ENTER("wolf_OBJ_obj2nid");
        WOLFSSL_STUB("wolf_OBJ_obj2nid");

        return 0;
    }

    int wolf_OBJ_sn2nid(const char *sn) {
        (void)sn;
        WOLFSSL_ENTER("wolf_OBJ_osn2nid");
        WOLFSSL_STUB("wolf_OBJ_osn2nid");

        return 0;
    }


    WOLFSSL_X509 *PEM_read_bio_WOLFSSL_X509(WOLFSSL_BIO *bp, WOLFSSL_X509 **x, pem_password_cb *cb, void *u) {
        (void)bp;
        (void)x;
        (void)cb;
        (void)u;
        WOLFSSL_ENTER("PEM_read_bio_WOLFSSL_X509");
        WOLFSSL_STUB("PEM_read_bio_WOLFSSL_X509");

        return NULL;
    }

    void wolfSSL_CTX_set_verify_depth(WOLFSSL_CTX *ctx, int depth) {
        (void)ctx;
        (void)depth;
        WOLFSSL_ENTER("wolfSSL_CTX_set_verify_depth");
        WOLFSSL_STUB("wolfSSL_CTX_set_verify_depth");

    }

    void* wolfSSL_get_app_data( const WOLFSSL *ssl)
    {
        /* checkout exdata stuff... */
        (void)ssl;
        WOLFSSL_ENTER("wolfSSL_get_app_data");
        WOLFSSL_STUB("wolfSSL_get_app_data");

        return 0;
    }

    void wolfSSL_set_app_data(WOLFSSL *ssl, void *arg) {
        (void)ssl;
        (void)arg;
        WOLFSSL_ENTER("wolfSSL_set_app_data");
        WOLFSSL_STUB("wolfSSL_set_app_data");
    }

    WOLFSSL_ASN1_OBJECT * wolfSSL_X509_NAME_ENTRY_get_object(WOLFSSL_X509_NAME_ENTRY *ne) {
        (void)ne;
        WOLFSSL_ENTER("wolfSSL_X509_NAME_ENTRY_get_object");
        WOLFSSL_STUB("wolfSSL_X509_NAME_ENTRY_get_object");

        return NULL;
    }

    WOLFSSL_X509_NAME_ENTRY *wolfSSL_X509_NAME_get_entry(
                                             WOLFSSL_X509_NAME *name, int loc) {

        int maxLoc = name->fullName.fullNameLen;

        WOLFSSL_ENTER("wolfSSL_X509_NAME_get_entry");

        if (loc < 0 || loc > maxLoc) {
            WOLFSSL_MSG("Bad argument");
            return NULL;
        }

        /* common name index case */
        if (loc == name->fullName.cnIdx) {
            /* get CN shortcut from x509 since it has null terminator */
            name->cnEntry.data.data   = name->x509->subjectCN;
            name->cnEntry.data.length = name->fullName.cnLen;
            name->cnEntry.data.type   = ASN_COMMON_NAME;
            name->cnEntry.set  = 1;
            return &(name->cnEntry);
        }

        /* additionall cases to check for go here */

        WOLFSSL_MSG("Entry not found or implemented");
        (void)name;
        (void)loc;

        return NULL;
    }

#ifndef NO_CERTS
    void wolfSSL_X509_NAME_free(WOLFSSL_X509_NAME *name){
        FreeX509Name(name, NULL);
        WOLFSSL_ENTER("wolfSSL_X509_NAME_free");
        WOLFSSL_STUB("wolfSSL_X509_NAME_free");
    }
#endif /* NO_CERTS */

    void wolfSSL_sk_X509_NAME_pop_free(STACK_OF(WOLFSSL_X509_NAME)* sk, void f (WOLFSSL_X509_NAME*)){
        (void) sk;
        (void) f;
        WOLFSSL_ENTER("wolfSSL_sk_X509_NAME_pop_free");
        WOLFSSL_STUB("wolfSSL_sk_X509_NAME_pop_free");
    }

    int wolfSSL_X509_check_private_key(WOLFSSL_X509 *x509, WOLFSSL_EVP_PKEY *key){
        (void) x509;
        (void) key;
        WOLFSSL_ENTER("wolfSSL_X509_check_private_key");
        WOLFSSL_STUB("wolfSSL_X509_check_private_key");

        return SSL_SUCCESS;
    }

    STACK_OF(WOLFSSL_X509_NAME) *wolfSSL_dup_CA_list( STACK_OF(WOLFSSL_X509_NAME) *sk ){
        (void) sk;
        WOLFSSL_ENTER("wolfSSL_dup_CA_list");
        WOLFSSL_STUB("wolfSSL_dup_CA_list");

        return NULL;
    }

#endif /* HAVE_LIGHTY || WOLFSSL_MYSQL_COMPATIBLE */
#endif


#ifdef OPENSSL_EXTRA
void* wolfSSL_CTX_get_ex_data(const WOLFSSL_CTX* ctx, int idx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_get_ex_data");
    #ifdef HAVE_STUNNEL
    if(ctx != NULL && idx < MAX_EX_DATA && idx >= 0) {
        return ctx->ex_data[idx];
    }
    #else
    (void)ctx;
    (void)idx;
    #endif
    return NULL;
}


int wolfSSL_CTX_get_ex_new_index(long idx, void* arg, void* a, void* b,
                                void* c)
{
    WOLFSSL_ENTER("wolfSSL_CTX_get_ex_new_index");
    (void)idx;
    (void)arg;
    (void)a;
    (void)b;
    (void)c;
    return 0;
}


int wolfSSL_CTX_set_ex_data(WOLFSSL_CTX* ctx, int idx, void* data)
{
    WOLFSSL_ENTER("wolfSSL_CTX_set_ex_data");
    #ifdef HAVE_STUNNEL
    if (ctx != NULL && idx < MAX_EX_DATA)
    {
        ctx->ex_data[idx] = data;
        return SSL_SUCCESS;
    }
    #else
    (void)ctx;
    (void)idx;
    (void)data;
    #endif
    return SSL_FAILURE;
}


int wolfSSL_set_ex_data(WOLFSSL* ssl, int idx, void* data)
{
    WOLFSSL_ENTER("wolfSSL_set_ex_data");
#if defined(FORTRESS) || defined(HAVE_STUNNEL)
    if (ssl != NULL && idx < MAX_EX_DATA)
    {
        ssl->ex_data[idx] = data;
        return SSL_SUCCESS;
    }
#else
    (void)ssl;
    (void)idx;
    (void)data;
#endif
    return SSL_FAILURE;
}


int wolfSSL_get_ex_new_index(long idx, void* data, void* cb1, void* cb2,
                         void* cb3)
{
    WOLFSSL_ENTER("wolfSSL_get_ex_new_index");
    (void)idx;
    (void)data;
    (void)cb1;
    (void)cb2;
    (void)cb3;
    return 0;
}


void* wolfSSL_get_ex_data(const WOLFSSL* ssl, int idx)
{
    WOLFSSL_ENTER("wolfSSL_get_ex_data");
#if defined(FORTRESS) || defined(HAVE_STUNNEL)
    if (ssl != NULL && idx < MAX_EX_DATA && idx >= 0)
        return ssl->ex_data[idx];
#else
    (void)ssl;
    (void)idx;
#endif
    return 0;
}
#endif /* OPENSSL_EXTRA */


#if defined(HAVE_LIGHTY) || defined(HAVE_STUNNEL) \
    || defined(WOLFSSL_MYSQL_COMPATIBLE)
char * wolf_OBJ_nid2ln(int n) {
    (void)n;
    WOLFSSL_ENTER("wolf_OBJ_nid2ln");
    WOLFSSL_STUB("wolf_OBJ_nid2ln");

    return NULL;
}

int wolf_OBJ_txt2nid(const char* s) {
    (void)s;
    WOLFSSL_ENTER("wolf_OBJ_txt2nid");
    WOLFSSL_STUB("wolf_OBJ_txt2nid");

    return 0;
}


WOLFSSL_BIO *wolfSSL_BIO_new_file(const char *filename, const char *mode) {
    (void)filename;
    (void)mode;
    WOLFSSL_ENTER("wolfSSL_BIO_new_file");
    WOLFSSL_STUB("wolfSSL_BIO_new_file");

    return NULL;
}


WOLFSSL_DH *wolfSSL_PEM_read_bio_DHparams(WOLFSSL_BIO *bp, WOLFSSL_DH **x, pem_password_cb *cb, void *u)
{
    (void) bp;
    (void) x;
    (void) cb;
    (void) u;

    WOLFSSL_ENTER("wolfSSL_PEM_read_bio_DHparams");
    WOLFSSL_STUB("wolfSSL_PEM_read_bio_DHparams");

    return NULL;
}

int PEM_write_bio_WOLFSSL_X509(WOLFSSL_BIO *bp, WOLFSSL_X509 *x) {
    (void)bp;
    (void)x;
    WOLFSSL_ENTER("PEM_write_bio_WOLFSSL_X509");
    WOLFSSL_STUB("PEM_write_bio_WOLFSSL_X509");

    return 0;
}


#ifndef NO_DH
/* Intialize ctx->dh with dh's params. Return SSL_SUCCESS on ok */
long wolfSSL_CTX_set_tmp_dh(WOLFSSL_CTX* ctx, WOLFSSL_DH* dh)
{
    int pSz, gSz;
    byte *p, *g;
    int ret=0;

    WOLFSSL_ENTER("wolfSSL_CTX_set_tmp_dh");

    if(!ctx || !dh)
        return BAD_FUNC_ARG;

    /* Get needed size for p and g */
    pSz = wolfSSL_BN_bn2bin(dh->p, NULL);
    gSz = wolfSSL_BN_bn2bin(dh->g, NULL);

    if(pSz <= 0 || gSz <= 0)
        return SSL_FATAL_ERROR;

    p = (byte*)XMALLOC(pSz, ctx->heap, DYNAMIC_TYPE_DH);
    if(!p)
        return MEMORY_E;

    g = (byte*)XMALLOC(gSz, ctx->heap, DYNAMIC_TYPE_DH);
    if(!g) {
        XFREE(p, ctx->heap, DYNAMIC_TYPE_DH);
        return MEMORY_E;
    }

    pSz = wolfSSL_BN_bn2bin(dh->p, p);
    gSz = wolfSSL_BN_bn2bin(dh->g, g);

    if(pSz >= 0 && gSz >= 0) /* Conversion successful */
        ret = wolfSSL_CTX_SetTmpDH(ctx, p, pSz, g, gSz);

    XFREE(p, ctx->heap, DYNAMIC_TYPE_DH);
    XFREE(g, ctx->heap, DYNAMIC_TYPE_DH);

    return pSz > 0 && gSz > 0 ? ret : SSL_FATAL_ERROR;
}
#endif /* NO_DH */
#endif /* HAVE_LIGHTY || HAVE_STUNNEL || WOLFSSL_MYSQL_COMPATIBLE */


/* stunnel compatibility functions*/
#if defined(OPENSSL_EXTRA) && defined(HAVE_STUNNEL)
void WOLFSSL_ERR_remove_thread_state(void* pid)
{
    (void) pid;
    return;
}


int wolfSSL_SESSION_set_ex_data(WOLFSSL_SESSION* session, int idx, void* data)
{
    WOLFSSL_ENTER("wolfSSL_SESSION_set_ex_data");
    if(session != NULL && idx < MAX_EX_DATA) {
        session->ex_data[idx] = data;
        return SSL_SUCCESS;
    }
    return SSL_FAILURE;
}


int wolfSSL_SESSION_get_ex_new_index(long idx, void* data, void* cb1,
       void* cb2, CRYPTO_free_func* cb3)
{
    WOLFSSL_ENTER("wolfSSL_SESSION_get_ex_new_index");
    (void)idx;
    (void)cb1;
    (void)cb2;
    (void)cb3;
    if(XSTRNCMP((const char*)data, "redirect index", 14) == 0) {
        return 0;
    }
    else if(XSTRNCMP((const char*)data, "addr index", 10) == 0) {
        return 1;
    }
    return SSL_FAILURE;
}


void* wolfSSL_SESSION_get_ex_data(const WOLFSSL_SESSION* session, int idx)
{
    WOLFSSL_ENTER("wolfSSL_SESSION_get_ex_data");
    if (session != NULL && idx < MAX_EX_DATA && idx >= 0)
        return session->ex_data[idx];
    return NULL;
}


int wolfSSL_CRYPTO_set_mem_ex_functions(void *(*m) (size_t, const char *, int),
                                void *(*r) (void *, size_t, const char *,
                                            int), void (*f) (void *))
{
    (void) m;
    (void) r;
    (void) f;
    WOLFSSL_ENTER("wolfSSL_CRYPTO_set_mem_ex_functions");
    WOLFSSL_STUB("wolfSSL_CRYPTO_set_mem_ex_functions");

    return SSL_FAILURE;
}


WOLFSSL_DH *wolfSSL_DH_generate_parameters(int prime_len, int generator,
                           void (*callback) (int, int, void *), void *cb_arg)
{
    (void)prime_len;
    (void)generator;
    (void)callback;
    (void)cb_arg;
    WOLFSSL_ENTER("wolfSSL_DH_generate_parameters");
    WOLFSSL_STUB("wolfSSL_DH_generate_parameters");

    return NULL;
}

int wolfSSL_DH_generate_parameters_ex(WOLFSSL_DH* dh, int prime_len, int generator,
                           void (*callback) (int, int, void *))
{
    (void)prime_len;
    (void)generator;
    (void)callback;
    (void)dh;
    WOLFSSL_ENTER("wolfSSL_DH_generate_parameters_ex");
    WOLFSSL_STUB("wolfSSL_DH_generate_parameters_ex");

    return -1;
}


void wolfSSL_ERR_load_crypto_strings(void)
{
    WOLFSSL_ENTER("wolfSSL_ERR_load_crypto_strings");
    WOLFSSL_ENTER("wolfSSL_ERR_load_crypto_strings");
    return;
}


unsigned long wolfSSL_ERR_peek_last_error(void)
{
    unsigned long l = 0UL;
    WOLFSSL_ENTER("wolfSSL_ERR_peek_last_error");
    WOLFSSL_STUB("wolfSSL_ERR_peek_last_error");

    return l;
}


int wolfSSL_FIPS_mode(void)
{
    WOLFSSL_ENTER("wolfSSL_FIPS_mode");
    WOLFSSL_STUB("wolfSSL_FIPS_mode");

    return SSL_FAILURE;
}

int wolfSSL_FIPS_mode_set(int r)
{
    (void)r;
    WOLFSSL_ENTER("wolfSSL_FIPS_mode_set");
    WOLFSSL_STUB("wolfSSL_FIPS_mode_set");

    return SSL_FAILURE;
}


int wolfSSL_RAND_set_rand_method(const void *meth)
{
    (void) meth;
    WOLFSSL_ENTER("wolfSSL_RAND_set_rand_method");
    WOLFSSL_STUB("wolfSSL_RAND_set_rand_method");

    return SSL_FAILURE;
}


int wolfSSL_CIPHER_get_bits(const WOLFSSL_CIPHER *c, int *alg_bits)
{
    int ret = SSL_FAILURE;
    WOLFSSL_ENTER("wolfSSL_CIPHER_get_bits");
    if(c != NULL && c->ssl != NULL) {
        ret = 8 * c->ssl->specs.key_size;
        if(alg_bits != NULL) {
            *alg_bits = ret;
        }
    }
    return ret;
}


int wolfSSL_sk_X509_NAME_num(const STACK_OF(WOLFSSL_X509_NAME) *s)
{
    (void) s;
    WOLFSSL_ENTER("wolfSSL_sk_X509_NAME_num");
    WOLFSSL_STUB("wolfSSL_sk_X509_NAME_num");

    return SSL_FAILURE;
}


int wolfSSL_sk_X509_num(const STACK_OF(WOLFSSL_X509) *s)
{
    (void) s;
    WOLFSSL_ENTER("wolfSSL_sk_X509_num");
    WOLFSSL_STUB("wolfSSL_sk_X509_num");

    return SSL_FAILURE;
}


int wolfSSL_X509_NAME_print_ex(WOLFSSL_BIO* bio, WOLFSSL_X509_NAME* nm,
                int indent, unsigned long flags)
{
    (void)bio;
    (void)nm;
    (void)indent;
    (void)flags;
    WOLFSSL_ENTER("wolfSSL_X509_NAME_print_ex");
    WOLFSSL_STUB("wolfSSL_X509_NAME_print_ex");

    return SSL_FAILURE;
}


WOLFSSL_ASN1_BIT_STRING* wolfSSL_X509_get0_pubkey_bitstr(const WOLFSSL_X509* x)
{
    (void)x;
    WOLFSSL_ENTER("wolfSSL_X509_get0_pubkey_bitstr");
    WOLFSSL_STUB("wolfSSL_X509_get0_pubkey_bitstr");

    return NULL;
}


int wolfSSL_CTX_add_session(WOLFSSL_CTX* ctx, WOLFSSL_SESSION* session)
{
    (void)ctx;
    (void)session;
    WOLFSSL_ENTER("wolfSSL_CTX_add_session");
    WOLFSSL_STUB("wolfSSL_CTX_add_session");

    return SSL_SUCCESS;
}


int wolfSSL_get_state(const WOLFSSL* ssl)
{
    (void)ssl;
    WOLFSSL_ENTER("wolfSSL_get_state");
    WOLFSSL_STUB("wolfSSL_get_state");

    return SSL_FAILURE;
}


void* wolfSSL_sk_X509_NAME_value(STACK_OF(WOLFSSL_X509_NAME)* sk, int i)
{
    (void)sk;
    (void)i;
    WOLFSSL_ENTER("wolfSSL_sk_X509_NAME_value");
    WOLFSSL_STUB("wolfSSL_sk_X509_NAME_value");

    return NULL;
}


void* wolfSSL_sk_X509_value(STACK_OF(WOLFSSL_X509)* sk, int i)
{
    (void)sk;
    (void)i;
    WOLFSSL_ENTER("wolfSSL_sk_X509_value");
    WOLFSSL_STUB("wolfSSL_sk_X509_value");

    return NULL;
}


int wolfSSL_version(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_version");
    if (ssl->version.major == SSLv3_MAJOR) {
        switch (ssl->version.minor) {
            case SSLv3_MINOR :
                return SSL3_VERSION;
            case TLSv1_MINOR :
            case TLSv1_1_MINOR :
            case TLSv1_2_MINOR :
                return TLS1_VERSION;
            default:
                return SSL_FAILURE;
        }
    }
    else if (ssl->version.major == DTLS_MAJOR) {
        switch (ssl->version.minor) {
            case DTLS_MINOR :
            case DTLSv1_2_MINOR :
                return DTLS1_VERSION;
            default:
                return SSL_FAILURE;
        }
    }
    return SSL_FAILURE;
}


STACK_OF(WOLFSSL_X509)* wolfSSL_get_peer_cert_chain(const WOLFSSL* ssl)
{
    (void)ssl;
    WOLFSSL_ENTER("wolfSSL_get_peer_cert_chain");
    WOLFSSL_STUB("wolfSSL_get_peer_cert_chain");

    return NULL;
}


long wolfSSL_CTX_get_options(WOLFSSL_CTX* ctx)
{
    (void)ctx;
    WOLFSSL_ENTER("wolfSSL_CTX_get_options");
    WOLFSSL_STUB("wolfSSL_CTX_get_options");

    return 0;
}


WOLFSSL_CTX* wolfSSL_get_SSL_CTX(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_get_SSL_CTX");
    return ssl->ctx;
}

int wolfSSL_X509_NAME_get_sz(WOLFSSL_X509_NAME* name)
{
    WOLFSSL_ENTER("wolfSSL_X509_NAME_get_sz");
    if(!name)
        return -1;
    return name->sz;
}


const byte* wolfSSL_SESSION_get_id(WOLFSSL_SESSION* sess, unsigned int* idLen)
{
    WOLFSSL_ENTER("wolfSSL_SESSION_get_id");
    WOLFSSL_STUB("wolfSSL_SESSION_get_id");
    if(!sess || !idLen) {
        WOLFSSL_MSG("Bad func args. Please provide idLen");
        return NULL;
    }
    *idLen = sess->sessionIDSz;
    return sess->sessionID;
}

#ifdef HAVE_SNI
int wolfSSL_set_tlsext_host_name(WOLFSSL* ssl, const char* host_name)
{
    int ret;
    WOLFSSL_ENTER("wolfSSL_set_tlsext_host_name");
    ret = wolfSSL_UseSNI(ssl, WOLFSSL_SNI_HOST_NAME,
            host_name, XSTRLEN(host_name));
    WOLFSSL_LEAVE("wolfSSL_set_tlsext_host_name", ret);
    return ret;
}


#ifndef NO_WOLFSSL_SERVER
const char * wolfSSL_get_servername(WOLFSSL* ssl, byte type)
{
    void * serverName = NULL;
    if (ssl == NULL)
        return NULL;
    TLSX_SNI_GetRequest(ssl->extensions, type, &serverName);
    return (const char *)serverName;
}
#endif /* NO_WOLFSSL_SERVER */
#endif /* HAVE_SNI */


WOLFSSL_CTX* wolfSSL_set_SSL_CTX(WOLFSSL* ssl, WOLFSSL_CTX* ctx)
{
    if (ssl && ctx && SetSSL_CTX(ssl, ctx) == SSL_SUCCESS)
        return ssl->ctx;
    return NULL;
}


VerifyCallback wolfSSL_CTX_get_verify_callback(WOLFSSL_CTX* ctx)
{
    WOLFSSL_ENTER("wolfSSL_CTX_get_verify_callback");
    if(ctx)
        return ctx->verifyCallback;
    return NULL;
}


void wolfSSL_CTX_set_servername_callback(WOLFSSL_CTX* ctx, CallbackSniRecv cb)
{
    WOLFSSL_ENTER("wolfSSL_CTX_set_servername_callback");
    if (ctx)
        ctx->sniRecvCb = cb;
}


void wolfSSL_CTX_set_servername_arg(WOLFSSL_CTX* ctx, void* arg)
{
    WOLFSSL_ENTER("wolfSSL_CTX_set_servername_arg");
    if (ctx)
        ctx->sniRecvCbArg = arg;
}


long wolfSSL_CTX_clear_options(WOLFSSL_CTX* ctx, long opt)
{
    WOLFSSL_ENTER("SSL_CTX_clear_options");
    WOLFSSL_STUB("SSL_CTX_clear_options");
    (void)ctx;
    (void)opt;
    return opt;
}

void wolfSSL_THREADID_set_callback(void(*threadid_func)(void*))
{
    WOLFSSL_ENTER("wolfSSL_THREADID_set_callback");
    WOLFSSL_STUB("wolfSSL_THREADID_set_callback");
    (void)threadid_func;
    return;
}

void wolfSSL_THREADID_set_numeric(void* id, unsigned long val)
{
    WOLFSSL_ENTER("wolfSSL_THREADID_set_numeric");
    WOLFSSL_STUB("wolfSSL_THREADID_set_numeric");
    (void)id;
    (void)val;
    return;
}


WOLFSSL_X509* wolfSSL_X509_STORE_get1_certs(WOLFSSL_X509_STORE_CTX* ctx,
                                                WOLFSSL_X509_NAME* name)
{
    WOLFSSL_ENTER("wolfSSL_X509_STORE_get1_certs");
    WOLFSSL_STUB("wolfSSL_X509_STORE_get1_certs");
    (void)ctx;
    (void)name;
    return NULL;
}

void wolfSSL_sk_X509_pop_free(STACK_OF(WOLFSSL_X509)* sk, void f (WOLFSSL_X509*)){
    (void) sk;
    (void) f;
    WOLFSSL_ENTER("wolfSSL_sk_X509_pop_free");
    WOLFSSL_STUB("wolfSSL_sk_X509_pop_free");
}

#endif /* OPENSSL_EXTRA and HAVE_STUNNEL */


#if (defined(OPENSSL_EXTRA) && defined(HAVE_STUNNEL)) \
    || defined(WOLFSSL_MYSQL_COMPATIBLE)
int wolfSSL_CTX_get_verify_mode(WOLFSSL_CTX* ctx)
{
    int mode = 0;
    WOLFSSL_ENTER("wolfSSL_CTX_get_verify_mode");

    if(!ctx)
        return SSL_FATAL_ERROR;

    if (ctx->verifyPeer)
        mode |= SSL_VERIFY_PEER;
    else if (ctx->verifyNone)
        mode |= SSL_VERIFY_NONE;

    if (ctx->failNoCert)
        mode |= SSL_VERIFY_FAIL_IF_NO_PEER_CERT;

    if (ctx->failNoCertxPSK)
        mode |= SSL_VERIFY_FAIL_EXCEPT_PSK;

    WOLFSSL_LEAVE("wolfSSL_CTX_get_verify_mode", mode);
    return mode;
}
#endif

#if defined(OPENSSL_EXTRA) && defined(HAVE_CURVE25519)
/* return 1 if success, 0 if error
 * output keys are little endian format
 */
int wolfSSL_EC25519_generate_key(unsigned char *priv, unsigned int *privSz,
                                 unsigned char *pub, unsigned int *pubSz)
{
#ifndef WOLFSSL_KEY_GEN
    WOLFSSL_MSG("No Key Gen built in");
    (void) priv;
    (void) privSz;
    (void) pub;
    (void) pubSz;
    return SSL_FAILURE;
#else /* WOLFSSL_KEY_GEN */
    int ret = SSL_FAILURE;
    int initTmpRng = 0;
    WC_RNG *rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG *tmpRNG = NULL;
#else
    WC_RNG tmpRNG[1];
#endif

    WOLFSSL_ENTER("wolfSSL_EC25519_generate_key");

    if (priv == NULL || privSz == NULL || *privSz < CURVE25519_KEYSIZE ||
        pub == NULL || pubSz == NULL || *pubSz < CURVE25519_KEYSIZE) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return SSL_FAILURE;
#endif
    if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("Bad RNG Init, trying global");
        if (initGlobalRNG == 0)
            WOLFSSL_MSG("Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng) {
        curve25519_key key;

        if (wc_curve25519_init(&key) != MP_OKAY)
            WOLFSSL_MSG("wc_curve25519_init failed");
        else if (wc_curve25519_make_key(rng, CURVE25519_KEYSIZE, &key)!=MP_OKAY)
            WOLFSSL_MSG("wc_curve25519_make_key failed");
        /* export key pair */
        else if (wc_curve25519_export_key_raw_ex(&key, priv, privSz, pub,
                                                 pubSz, EC25519_LITTLE_ENDIAN)
                 != MP_OKAY)
            WOLFSSL_MSG("wc_curve25519_export_key_raw_ex failed");
        else
            ret = SSL_SUCCESS;

        wc_curve25519_free(&key);
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
#endif /* WOLFSSL_KEY_GEN */
}

/* return 1 if success, 0 if error
 * input and output keys are little endian format
 */
int wolfSSL_EC25519_shared_key(unsigned char *shared, unsigned int *sharedSz,
                               const unsigned char *priv, unsigned int privSz,
                               const unsigned char *pub, unsigned int pubSz)
{
#ifndef WOLFSSL_KEY_GEN
    WOLFSSL_MSG("No Key Gen built in");
    (void) shared;
    (void) sharedSz;
    (void) priv;
    (void) privSz;
    (void) pub;
    (void) pubSz;
    return SSL_FAILURE;
#else /* WOLFSSL_KEY_GEN */
    int ret = SSL_FAILURE;
    curve25519_key privkey, pubkey;

    WOLFSSL_ENTER("wolfSSL_EC25519_shared_key");

    if (shared == NULL || sharedSz == NULL || *sharedSz < CURVE25519_KEYSIZE ||
        priv == NULL || privSz < CURVE25519_KEYSIZE ||
        pub == NULL || pubSz < CURVE25519_KEYSIZE) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

    /* import private key */
    if (wc_curve25519_init(&privkey) != MP_OKAY) {
        WOLFSSL_MSG("wc_curve25519_init privkey failed");
        return ret;
    }
    if (wc_curve25519_import_private_ex(priv, privSz, &privkey,
                                        EC25519_LITTLE_ENDIAN) != MP_OKAY) {
        WOLFSSL_MSG("wc_curve25519_import_private_ex failed");
        wc_curve25519_free(&privkey);
        return ret;
    }

    /* import public key */
    if (wc_curve25519_init(&pubkey) != MP_OKAY) {
        WOLFSSL_MSG("wc_curve25519_init pubkey failed");
        wc_curve25519_free(&privkey);
        return ret;
    }
    if (wc_curve25519_import_public_ex(pub, pubSz, &pubkey,
                                       EC25519_LITTLE_ENDIAN) != MP_OKAY) {
        WOLFSSL_MSG("wc_curve25519_import_public_ex failed");
        wc_curve25519_free(&privkey);
        wc_curve25519_free(&pubkey);
        return ret;
    }

    if (wc_curve25519_shared_secret_ex(&privkey, &pubkey,
                                       shared, sharedSz,
                                       EC25519_LITTLE_ENDIAN) != MP_OKAY)
        WOLFSSL_MSG("wc_curve25519_shared_secret_ex failed");
    else
        ret = SSL_SUCCESS;

    wc_curve25519_free(&privkey);
    wc_curve25519_free(&pubkey);

    return ret;
#endif /* WOLFSSL_KEY_GEN */
}
#endif /* OPENSSL_EXTRA && HAVE_CURVE25519 */

#if defined(OPENSSL_EXTRA) && defined(HAVE_ED25519)
/* return 1 if success, 0 if error
 * output keys are little endian format
 */
int wolfSSL_ED25519_generate_key(unsigned char *priv, unsigned int *privSz,
                                 unsigned char *pub, unsigned int *pubSz)
{
#ifndef WOLFSSL_KEY_GEN
    WOLFSSL_MSG("No Key Gen built in");
    (void) priv;
    (void) privSz;
    (void) pub;
    (void) pubSz;
    return SSL_FAILURE;
#else /* WOLFSSL_KEY_GEN */
    int ret = SSL_FAILURE;
    int initTmpRng = 0;
    WC_RNG *rng = NULL;
#ifdef WOLFSSL_SMALL_STACK
    WC_RNG *tmpRNG = NULL;
#else
    WC_RNG tmpRNG[1];
#endif

    WOLFSSL_ENTER("wolfSSL_ED25519_generate_key");

    if (priv == NULL || privSz == NULL || *privSz < ED25519_PRV_KEY_SIZE ||
        pub == NULL || pubSz == NULL || *pubSz < ED25519_PUB_KEY_SIZE) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

#ifdef WOLFSSL_SMALL_STACK
    tmpRNG = (WC_RNG*)XMALLOC(sizeof(WC_RNG), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmpRNG == NULL)
        return SSL_FATAL_ERROR;
#endif
    if (wc_InitRng(tmpRNG) == 0) {
        rng = tmpRNG;
        initTmpRng = 1;
    }
    else {
        WOLFSSL_MSG("Bad RNG Init, trying global");
        if (initGlobalRNG == 0)
            WOLFSSL_MSG("Global RNG no Init");
        else
            rng = &globalRNG;
    }

    if (rng) {
        ed25519_key key;

        if (wc_ed25519_init(&key) != MP_OKAY)
            WOLFSSL_MSG("wc_ed25519_init failed");
        else if (wc_ed25519_make_key(rng, ED25519_KEY_SIZE, &key)!=MP_OKAY)
            WOLFSSL_MSG("wc_ed25519_make_key failed");
        /* export private key */
        else if (wc_ed25519_export_key(&key, priv, privSz, pub, pubSz)!=MP_OKAY)
            WOLFSSL_MSG("wc_ed25519_export_key failed");
        else
            ret = SSL_SUCCESS;

        wc_ed25519_free(&key);
    }

    if (initTmpRng)
        wc_FreeRng(tmpRNG);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmpRNG, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
#endif /* WOLFSSL_KEY_GEN */
}

/* return 1 if success, 0 if error
 * input and output keys are little endian format
 * priv is a buffer containing private and public part of key
 */
int wolfSSL_ED25519_sign(const unsigned char *msg, unsigned int msgSz,
                         const unsigned char *priv, unsigned int privSz,
                         unsigned char *sig, unsigned int *sigSz)
{
#ifndef WOLFSSL_KEY_GEN
    WOLFSSL_MSG("No Key Gen built in");
    (void) msg;
    (void) msgSz;
    (void) priv;
    (void) privSz;
    (void) sig;
    (void) sigSz;
    return SSL_FAILURE;
#else /* WOLFSSL_KEY_GEN */
    ed25519_key key;
    int ret = SSL_FAILURE;

    WOLFSSL_ENTER("wolfSSL_ED25519_sign");

    if (priv == NULL || privSz != ED25519_PRV_KEY_SIZE ||
        msg == NULL || sig == NULL || *sigSz < ED25519_SIG_SIZE) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

    /* import key */
    if (wc_ed25519_init(&key) != MP_OKAY) {
        WOLFSSL_MSG("wc_curve25519_init failed");
        return ret;
    }
    if (wc_ed25519_import_private_key(priv, privSz/2,
                                      priv+(privSz/2), ED25519_PUB_KEY_SIZE,
                                      &key) != MP_OKAY){
        WOLFSSL_MSG("wc_ed25519_import_private failed");
        wc_ed25519_free(&key);
        return ret;
    }

    if (wc_ed25519_sign_msg(msg, msgSz, sig, sigSz, &key) != MP_OKAY)
        WOLFSSL_MSG("wc_curve25519_shared_secret_ex failed");
    else
        ret = SSL_SUCCESS;

    wc_ed25519_free(&key);

    return ret;
#endif /* WOLFSSL_KEY_GEN */
}

/* return 1 if success, 0 if error
 * input and output keys are little endian format
 * pub is a buffer containing public part of key
 */
int wolfSSL_ED25519_verify(const unsigned char *msg, unsigned int msgSz,
                           const unsigned char *pub, unsigned int pubSz,
                           const unsigned char *sig, unsigned int sigSz)
{
#ifndef WOLFSSL_KEY_GEN
    WOLFSSL_MSG("No Key Gen built in");
    (void) msg;
    (void) msgSz;
    (void) pub;
    (void) pubSz;
    (void) sig;
    (void) sigSz;
    return SSL_FAILURE;
#else /* WOLFSSL_KEY_GEN */
    ed25519_key key;
    int ret = SSL_FAILURE, check = 0;

    WOLFSSL_ENTER("wolfSSL_ED25519_verify");

    if (pub == NULL || pubSz != ED25519_PUB_KEY_SIZE ||
        msg == NULL || sig == NULL || sigSz != ED25519_SIG_SIZE) {
        WOLFSSL_MSG("Bad arguments");
        return SSL_FAILURE;
    }

    /* import key */
    if (wc_ed25519_init(&key) != MP_OKAY) {
        WOLFSSL_MSG("wc_curve25519_init failed");
        return ret;
    }
    if (wc_ed25519_import_public(pub, pubSz, &key) != MP_OKAY){
        WOLFSSL_MSG("wc_ed25519_import_public failed");
        wc_ed25519_free(&key);
        return ret;
    }

    if ((ret = wc_ed25519_verify_msg((byte*)sig, sigSz, msg, msgSz,
                                     &check, &key)) != MP_OKAY) {
        WOLFSSL_MSG("wc_ed25519_verify_msg failed");
    }
    else if (!check)
        WOLFSSL_MSG("wc_ed25519_verify_msg failed (signature invalid)");
    else
        ret = SSL_SUCCESS;

    wc_ed25519_free(&key);

    return ret;
#endif /* WOLFSSL_KEY_GEN */
}

#endif /* OPENSSL_EXTRA && HAVE_ED25519 */

#ifdef WOLFSSL_JNI

int wolfSSL_set_jobject(WOLFSSL* ssl, void* objPtr)
{
    WOLFSSL_ENTER("wolfSSL_set_jobject");
    if (ssl != NULL)
    {
        ssl->jObjectRef = objPtr;
        return SSL_SUCCESS;
    }
    return SSL_FAILURE;
}

void* wolfSSL_get_jobject(WOLFSSL* ssl)
{
    WOLFSSL_ENTER("wolfSSL_get_jobject");
    if (ssl != NULL)
        return ssl->jObjectRef;
    return NULL;
}

#endif /* WOLFSSL_JNI */

#ifdef HAVE_WOLF_EVENT
static int _wolfSSL_CTX_poll(WOLFSSL_CTX* ctx, WOLFSSL* ssl, WOLF_EVENT* events,
    int maxEvents, unsigned char flags, int* eventCount)
{
    WOLF_EVENT* event, *event_prev = NULL;
    int count = 0, ret = SSL_ERROR_NONE;

    if (ctx == NULL || maxEvents <= 0) {
        return BAD_FUNC_ARG;
    }

    /* Events arg can be NULL only if peek */
    if (events == NULL && !(flags & WOLF_POLL_FLAG_PEEK)) {
        return BAD_FUNC_ARG;
    }

#ifndef SINGLE_THREADED
    /* In single threaded mode "event_queue.lock" doesn't exist */
    if (LockMutex(&ctx->event_queue.lock) != 0) {
        return BAD_MUTEX_E;
    }
#endif

    /* Itterate event queue */
    for (event = ctx->event_queue.head; event != NULL; event = event->next)
    {
        byte removeEvent = 0;

        /* Optionally filter by ssl object pointer */
        if (ssl == NULL || (ssl == event->ssl)) {
            if (flags & WOLF_POLL_FLAG_PEEK) {
                if (events) {
                    /* Copy event data to provided buffer */
                    XMEMCPY(&events[count], event, sizeof(WOLF_EVENT));
                }
                count++;
            }
            else {
                /* Check hardware */
                if (flags & WOLF_POLL_FLAG_CHECK_HW) {
                #ifdef WOLFSSL_ASYNC_CRYPT
                    if (event->type >= WOLF_EVENT_TYPE_ASYNC_FIRST &&
                        event->type <= WOLF_EVENT_TYPE_ASYNC_LAST)
                    {
                        ret = wolfSSL_async_poll(event, flags);
                    }
                #endif /* WOLFSSL_ASYNC_CRYPT */
                }

                /* If event is done then return in 'events' argument */
                if (event->done) {
                    /* Copy event data to provided buffer */
                    XMEMCPY(&events[count], event, sizeof(WOLF_EVENT));
                    count++;
                    removeEvent = 1;
                }
            }
        }

        if (removeEvent) {
            /* Remove from queue list */
            if (event_prev == NULL) {
                ctx->event_queue.head = event->next;
                if (ctx->event_queue.head == NULL) {
                    ctx->event_queue.tail = NULL;
                }
            }
            else {
                event_prev->next = event->next;
            }
        }
        else {
            /* Leave in queue, save prev pointer */
            event_prev = event;
        }

        /* Check to make sure our event list isn't full */
        if (events && count >= maxEvents) {
            break; /* Exit for */
        }

        /* Check for error */
        if (ret < 0) {
            break; /* Exit for */
        }
    }

#ifndef SINGLE_THREADED
    UnLockMutex(&ctx->event_queue.lock);
#endif

    /* Return number of properly populated events */
    if (eventCount) {
        *eventCount = count;
    }

    return ret;
}

int wolfSSL_CTX_poll(WOLFSSL_CTX* ctx, WOLF_EVENT* events,
    int maxEvents, unsigned char flags, int* eventCount)
{
    return _wolfSSL_CTX_poll(ctx, NULL, events, maxEvents, flags, eventCount);
}

int wolfSSL_poll(WOLFSSL* ssl, WOLF_EVENT* events,
    int maxEvents, unsigned char flags, int* eventCount)
{
    return _wolfSSL_CTX_poll(ssl->ctx, ssl, events, maxEvents, flags,
        eventCount);
}

#endif /* HAVE_WOLF_EVENT */

#endif /* WOLFCRYPT_ONLY */
