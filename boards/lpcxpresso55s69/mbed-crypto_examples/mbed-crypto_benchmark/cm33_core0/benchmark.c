/*
 *  Benchmark demonstration program
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
 
/* NOTE: This application is based on middleware\mbed-crypto\programs\test\benchmark.c */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "board.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/platform.h"
#if !defined(MBEDTLS_PLATFORM_C)
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_exit       exit
#define mbedtls_printf     printf
#define mbedtls_free       free
#endif

#include <string.h>
#include <stdlib.h>

#include "mbedtls/timing.h"

#include "mbedtls/md4.h"
#include "mbedtls/md5.h"
#include "mbedtls/ripemd160.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

#include "mbedtls/arc4.h"
#include "mbedtls/des.h"
#include "mbedtls/aes.h"
#include "mbedtls/aria.h"
#include "mbedtls/blowfish.h"
#include "mbedtls/camellia.h"
#include "mbedtls/chacha20.h"
#include "mbedtls/gcm.h"
#include "mbedtls/ccm.h"
#include "mbedtls/chachapoly.h"
#include "mbedtls/cmac.h"
#include "mbedtls/poly1305.h"

#include "mbedtls/havege.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/hmac_drbg.h"

#include "mbedtls/rsa.h"
#include "mbedtls/dhm.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/ecdh.h"

#include "mbedtls/error.h"

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

#ifndef fflush
#define fflush(x) \
    do            \
    {             \
    } while (0)
#endif 

/*
 * For heap usage estimates, we need an estimate of the overhead per allocated
 * block. ptmalloc2/3 (used in gnu libc for instance) uses 2 size_t per block,
 * so use that as our baseline.
 */
#define MEM_BLOCK_OVERHEAD  ( 2 * sizeof( size_t ) )

/*
 * Size to use for the alloc buffer if MEMORY_BUFFER_ALLOC_C is defined.
 */
#define HEAP_SIZE       (1u << 16)  /* 64k */

#define BUFSIZE         1024
#define HEADER_FORMAT   "  %-24s :  "
#define TITLE_LEN       25

#define OPTIONS                                                         \
    "md4, md5, ripemd160, sha1, sha256, sha512,\n"                      \
    "arc4, des3, des, camellia, blowfish, chacha20,\n"                  \
    "aes_cbc, aes_gcm, aes_ccm, aes_ctx, chachapoly,\n"                 \
    "aes_cmac, des3_cmac, poly1305\n"                                   \
    "havege, ctr_drbg, hmac_drbg\n"                                     \
    "rsa, dhm, ecdsa, ecdh.\n"

#if defined(MBEDTLS_ERROR_C)
#define PRINT_ERROR                                                     \
        mbedtls_strerror( ret, ( char * )tmp, sizeof( tmp ) );          \
        mbedtls_printf( "FAILED: %s\n", tmp );
#else
#define PRINT_ERROR                                                     \
        mbedtls_printf( "FAILED: -0x%04x\n", -ret );
#endif

#define TIME_AND_TSC( TITLE, CODE )                                     \
do {                                                                    \
    unsigned long ii, jj, tsc;                                          \
    int ret = 0;                                                        \
                                                                        \
    mbedtls_printf( HEADER_FORMAT, TITLE );                             \
    fflush( stdout );                                                   \
                                                                        \
    mbedtls_set_alarm( 1 );                                             \
    for( ii = 1; ret == 0 && ! mbedtls_timing_alarmed; ii++ )           \
    {                                                                   \
        ret = CODE;                                                     \
    }                                                                   \
                                                                        \
    tsc = mbedtls_timing_hardclock();                                   \
    for( jj = 0; ret == 0 && jj < 1024; jj++ )                          \
    {                                                                   \
        ret = CODE;                                                     \
    }                                                                   \
                                                                        \
    if( ret != 0 )                                                      \
    {                                                                   \
        PRINT_ERROR;                                                    \
    }                                                                   \
    else                                                                \
    {                                                                   \
        mbedtls_printf( "%9lu KiB/s,  %9lu cycles/byte\n",              \
                         ii * BUFSIZE / 1024,                           \
                         ( mbedtls_timing_hardclock() - tsc )           \
                         / ( jj * BUFSIZE ) );                          \
    }                                                                   \
} while( 0 )

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C) && defined(MBEDTLS_MEMORY_DEBUG)

#define MEMORY_MEASURE_INIT                                             \
    size_t max_used, max_blocks, max_bytes;                             \
    size_t prv_used, prv_blocks;                                        \
    mbedtls_memory_buffer_alloc_cur_get( &prv_used, &prv_blocks );      \
    mbedtls_memory_buffer_alloc_max_reset( );

#define MEMORY_MEASURE_PRINT( title_len )                               \
    mbedtls_memory_buffer_alloc_max_get( &max_used, &max_blocks );      \
    for( ii = 12 - (title_len); ii != 0; ii-- ) mbedtls_printf( " " );  \
    max_used -= prv_used;                                               \
    max_blocks -= prv_blocks;                                           \
    max_bytes = max_used + MEM_BLOCK_OVERHEAD * max_blocks;             \
    mbedtls_printf( "%6u heap bytes", (unsigned) max_bytes );

#else
#define MEMORY_MEASURE_INIT
#define MEMORY_MEASURE_PRINT( title_len )
#endif

#define TIME_PUBLIC( TITLE, TYPE, CODE )                                \
do {                                                                    \
    unsigned long ii;                                                   \
    int ret;                                                            \
    MEMORY_MEASURE_INIT;                                                \
                                                                        \
    mbedtls_printf( HEADER_FORMAT, TITLE );                             \
    fflush( stdout );                                                   \
    mbedtls_set_alarm( 3 );                                             \
                                                                        \
    ret = 0;                                                            \
    for( ii = 1; ! mbedtls_timing_alarmed && ! ret ; ii++ )             \
    {                                                                   \
        CODE;                                                           \
    }                                                                   \
                                                                        \
    if( ret == MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED )               \
    {                                                                   \
        mbedtls_printf( "Feature Not Supported. Skipping.\n" );         \
        ret = 0;                                                        \
    }                                                                   \
    else if( ret != 0 )                                                 \
    {                                                                   \
        PRINT_ERROR;                                                    \
    }                                                                   \
    else                                                                \
    {                                                                   \
        mbedtls_printf( "%6lu " TYPE "/s", ii / 3 );                    \
        MEMORY_MEASURE_PRINT( sizeof( TYPE ) + 1 );                     \
        mbedtls_printf( "\n" );                                         \
    }                                                                   \
} while( 0 )

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/



/*******************************************************************************
 * Code
 ******************************************************************************/

#if !defined(MBEDTLS_TIMING_ALT) /* If not implemented, provide own alternate implementation 
                                    for mbedtls_timing_hardclock(), mbedtls_set_alarm() and mbedtls_timing_alarmed defined in timing.h */

static volatile unsigned long counter_ms; /* Milliseconds counter since last POR/reset */
static volatile unsigned long alarm_timeout_s; /* Delay before the "mbedtls_timing_alarmed" flag is set */
volatile int mbedtls_timing_alarmed; /* alarm flag */

/* Return the CPU cycle counter value */
unsigned long mbedtls_timing_hardclock(void)
{
    uint32_t curr_count_ms;
    uint32_t curr_tick;
    uint32_t tick_in_ms;

    do
    {
        curr_count_ms = counter_ms;
        curr_tick     = SysTick->VAL; /* the current value of the count down */
    } while (curr_count_ms != counter_ms);

    tick_in_ms = CLOCK_GetCoreSysClkFreq() / 1000U;
    return (curr_count_ms * tick_in_ms) + tick_in_ms - curr_tick;
}

/* Setup an alarm clock, delay before the "mbedtls_timing_alarmed" flag is set */
void mbedtls_set_alarm(int seconds)
{
    alarm_timeout_s = mbedtls_timing_hardclock() + (seconds * CLOCK_GetCoreSysClkFreq());
    mbedtls_timing_alarmed = 0;
}

/* Incremwent milliseconds counter since last POR/reset and check "alarm" */
void SysTick_Handler(void)
{
    counter_ms++;
    
    if((mbedtls_timing_alarmed == 0) && (alarm_timeout_s != 0))
    {
        if (mbedtls_timing_hardclock() > alarm_timeout_s)
        {
            mbedtls_timing_alarmed = 1;
        }    
    }
}

#endif
 
static int myrand( void *rng_state, unsigned char *output, size_t len )
{
    size_t use_len;
    int rnd;

    if( rng_state != NULL )
        rng_state  = NULL;

    while( len > 0 )
    {
        use_len = len;
        if( use_len > sizeof(int) )
            use_len = sizeof(int);

        rnd = rand();
        memcpy( output, &rnd, use_len );
        output += use_len;
        len -= use_len;
    }

    return( 0 );
}

#define CHECK_AND_CONTINUE( R )                                         \
    {                                                                   \
        int CHECK_AND_CONTINUE_ret = ( R );                             \
        if( CHECK_AND_CONTINUE_ret == MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED ) { \
            mbedtls_printf( "Feature not supported. Skipping.\n" );     \
            continue;                                                   \
        }                                                               \
        else if( CHECK_AND_CONTINUE_ret != 0 ) {                        \
            mbedtls_exit( 1 );                                          \
        }                                                               \
    }

/*
 * Clear some memory that was used to prepare the context
 */
#if defined(MBEDTLS_ECP_C)
void ecp_clear_precomputed( mbedtls_ecp_group *grp )
{
    if( grp->T != NULL )
    {
        size_t i;
        for( i = 0; i < grp->T_size; i++ )
            mbedtls_ecp_point_free( &grp->T[i] );
        mbedtls_free( grp->T );
    }
    grp->T = NULL;
    grp->T_size = 0;
}
#else
#define ecp_clear_precomputed( g )
#endif

unsigned char buf[BUFSIZE];

typedef struct {
    char md4, md5, ripemd160, sha1, sha256, sha512,
         arc4, des3, des,
         aes_cbc, aes_gcm, aes_ccm, aes_xts, chachapoly,
         aes_cmac, des3_cmac,
         aria, camellia, blowfish, chacha20,
         poly1305,
         havege, ctr_drbg, hmac_drbg,
         rsa, dhm, ecdsa, ecdh;
} todo_list;


int main( int argc, char *argv[] )
{
    int i;
    unsigned char tmp[200];
    char title[TITLE_LEN];
    todo_list todo;
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
    unsigned char alloc_buf[HEAP_SIZE] = { 0 };
#endif

    /* HW init */
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

    /* Init SysTick module */
    /* call CMSIS SysTick function. It enables the SysTick interrupt at low priority */
    SysTick_Config(CLOCK_GetCoreSysClkFreq() / 1000U); /* 1 ms period */
  
    /* Run all tests.*/
    memset(&todo, 1, sizeof(todo));

    mbedtls_printf( "\n" );

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
    mbedtls_memory_buffer_alloc_init( alloc_buf, sizeof( alloc_buf ) );
#endif
    memset( buf, 0xAA, sizeof( buf ) );
    memset( tmp, 0xBB, sizeof( tmp ) );

#if defined(MBEDTLS_MD4_C)
    if( todo.md4 )
        TIME_AND_TSC( "MD4", mbedtls_md4_ret( buf, BUFSIZE, tmp ) );
#endif

#if defined(MBEDTLS_MD5_C)
    if( todo.md5 )
        TIME_AND_TSC( "MD5", mbedtls_md5_ret( buf, BUFSIZE, tmp ) );
#endif

#if defined(MBEDTLS_RIPEMD160_C)
    if( todo.ripemd160 )
        TIME_AND_TSC( "RIPEMD160", mbedtls_ripemd160_ret( buf, BUFSIZE, tmp ) );
#endif

#if defined(MBEDTLS_SHA1_C)
    if( todo.sha1 )
        TIME_AND_TSC( "SHA-1", mbedtls_sha1_ret( buf, BUFSIZE, tmp ) );
#endif

#if defined(MBEDTLS_SHA256_C)
    if( todo.sha256 )
        TIME_AND_TSC( "SHA-256", mbedtls_sha256_ret( buf, BUFSIZE, tmp, 0 ) );
#endif

#if defined(MBEDTLS_SHA512_C)
    if( todo.sha512 )
        TIME_AND_TSC( "SHA-512", mbedtls_sha512_ret( buf, BUFSIZE, tmp, 0 ) );
#endif

#if defined(MBEDTLS_ARC4_C)
    if( todo.arc4 )
    {
        mbedtls_arc4_context arc4;
        mbedtls_arc4_init( &arc4 );
        mbedtls_arc4_setup( &arc4, tmp, 32 );
        TIME_AND_TSC( "ARC4", mbedtls_arc4_crypt( &arc4, BUFSIZE, buf, buf ) );
        mbedtls_arc4_free( &arc4 );
    }
#endif

#if defined(MBEDTLS_DES_C)
#if defined(MBEDTLS_CIPHER_MODE_CBC)
    if( todo.des3 )
    {
        mbedtls_des3_context des3;
        mbedtls_des3_init( &des3 );
        mbedtls_des3_set3key_enc( &des3, tmp );
        TIME_AND_TSC( "3DES",
                mbedtls_des3_crypt_cbc( &des3, MBEDTLS_DES_ENCRYPT, BUFSIZE, tmp, buf, buf ) );
        mbedtls_des3_free( &des3 );
    }

    if( todo.des )
    {
        mbedtls_des_context des;
        mbedtls_des_init( &des );
        mbedtls_des_setkey_enc( &des, tmp );
        TIME_AND_TSC( "DES",
                mbedtls_des_crypt_cbc( &des, MBEDTLS_DES_ENCRYPT, BUFSIZE, tmp, buf, buf ) );
        mbedtls_des_free( &des );
    }

#endif /* MBEDTLS_CIPHER_MODE_CBC */
#if defined(MBEDTLS_CMAC_C)
    if( todo.des3_cmac )
    {
        unsigned char output[8];
        const mbedtls_cipher_info_t *cipher_info;

        memset( buf, 0, sizeof( buf ) );
        memset( tmp, 0, sizeof( tmp ) );

        cipher_info = mbedtls_cipher_info_from_type( MBEDTLS_CIPHER_DES_EDE3_ECB );

        TIME_AND_TSC( "3DES-CMAC",
                      mbedtls_cipher_cmac( cipher_info, tmp, 192, buf,
                      BUFSIZE, output ) );
    }
#endif /* MBEDTLS_CMAC_C */
#endif /* MBEDTLS_DES_C */

#if defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_CIPHER_MODE_CBC)
    if( todo.aes_cbc )
    {
        int keysize;
        mbedtls_aes_context aes;
        mbedtls_aes_init( &aes );
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            mbedtls_snprintf( title, sizeof( title ), "AES-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_aes_setkey_enc( &aes, tmp, keysize );

            TIME_AND_TSC( title,
                mbedtls_aes_crypt_cbc( &aes, MBEDTLS_AES_ENCRYPT, BUFSIZE, tmp, buf, buf ) );
        }
        mbedtls_aes_free( &aes );
    }
#endif
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    if( todo.aes_xts )
    {
        int keysize;
        mbedtls_aes_xts_context ctx;

        mbedtls_aes_xts_init( &ctx );
        for( keysize = 128; keysize <= 256; keysize += 128 )
        {
            mbedtls_snprintf( title, sizeof( title ), "AES-XTS-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_aes_xts_setkey_enc( &ctx, tmp, keysize * 2 );

            TIME_AND_TSC( title,
                    mbedtls_aes_crypt_xts( &ctx, MBEDTLS_AES_ENCRYPT, BUFSIZE,
                                           tmp, buf, buf ) );

            mbedtls_aes_xts_free( &ctx );
        }
    }
#endif
#if defined(MBEDTLS_GCM_C)
    if( todo.aes_gcm )
    {
        int keysize;
        mbedtls_gcm_context gcm;

        mbedtls_gcm_init( &gcm );
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            mbedtls_snprintf( title, sizeof( title ), "AES-GCM-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_gcm_setkey( &gcm, MBEDTLS_CIPHER_ID_AES, tmp, keysize );

            TIME_AND_TSC( title,
                    mbedtls_gcm_crypt_and_tag( &gcm, MBEDTLS_GCM_ENCRYPT, BUFSIZE, tmp,
                        12, NULL, 0, buf, buf, 16, tmp ) );

            mbedtls_gcm_free( &gcm );
        }
    }
#endif
#if defined(MBEDTLS_CCM_C)
    if( todo.aes_ccm )
    {
        int keysize;
        mbedtls_ccm_context ccm;

        mbedtls_ccm_init( &ccm );
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            mbedtls_snprintf( title, sizeof( title ), "AES-CCM-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_ccm_setkey( &ccm, MBEDTLS_CIPHER_ID_AES, tmp, keysize );

            TIME_AND_TSC( title,
                    mbedtls_ccm_encrypt_and_tag( &ccm, BUFSIZE, tmp,
                        12, NULL, 0, buf, buf, tmp, 16 ) );

            mbedtls_ccm_free( &ccm );
        }
    }
#endif
#if defined(MBEDTLS_CHACHAPOLY_C)
    if( todo.chachapoly )
    {
        mbedtls_chachapoly_context chachapoly;

        mbedtls_chachapoly_init( &chachapoly );
        memset( buf, 0, sizeof( buf ) );
        memset( tmp, 0, sizeof( tmp ) );

        mbedtls_snprintf( title, sizeof( title ), "ChaCha20-Poly1305" );

        mbedtls_chachapoly_setkey( &chachapoly, tmp );

        TIME_AND_TSC( title,
                mbedtls_chachapoly_encrypt_and_tag( &chachapoly,
                    BUFSIZE, tmp, NULL, 0, buf, buf, tmp ) );

        mbedtls_chachapoly_free( &chachapoly );
    }
#endif
#if defined(MBEDTLS_CMAC_C)
    if( todo.aes_cmac )
    {
        unsigned char output[16];
        const mbedtls_cipher_info_t *cipher_info;
        mbedtls_cipher_type_t cipher_type;
        int keysize;

        for( keysize = 128, cipher_type = MBEDTLS_CIPHER_AES_128_ECB;
             keysize <= 256;
             keysize += 64, cipher_type++ )
        {
            mbedtls_snprintf( title, sizeof( title ), "AES-CMAC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );

            cipher_info = mbedtls_cipher_info_from_type( cipher_type );

            TIME_AND_TSC( title,
                          mbedtls_cipher_cmac( cipher_info, tmp, keysize,
                                               buf, BUFSIZE, output ) );
        }

        memset( buf, 0, sizeof( buf ) );
        memset( tmp, 0, sizeof( tmp ) );
        TIME_AND_TSC( "AES-CMAC-PRF-128",
                      mbedtls_aes_cmac_prf_128( tmp, 16, buf, BUFSIZE,
                                                output ) );
    }
#endif /* MBEDTLS_CMAC_C */
#endif /* MBEDTLS_AES_C */

#if defined(MBEDTLS_ARIA_C) && defined(MBEDTLS_CIPHER_MODE_CBC)
    if( todo.aria )
    {
        int keysize;
        mbedtls_aria_context aria;
        mbedtls_aria_init( &aria );
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            mbedtls_snprintf( title, sizeof( title ), "ARIA-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_aria_setkey_enc( &aria, tmp, keysize );

            TIME_AND_TSC( title,
                    mbedtls_aria_crypt_cbc( &aria, MBEDTLS_ARIA_ENCRYPT,
                        BUFSIZE, tmp, buf, buf ) );
        }
        mbedtls_aria_free( &aria );
    }
#endif

#if defined(MBEDTLS_CAMELLIA_C) && defined(MBEDTLS_CIPHER_MODE_CBC)
    if( todo.camellia )
    {
        int keysize;
        mbedtls_camellia_context camellia;
        mbedtls_camellia_init( &camellia );
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            mbedtls_snprintf( title, sizeof( title ), "CAMELLIA-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_camellia_setkey_enc( &camellia, tmp, keysize );

            TIME_AND_TSC( title,
                    mbedtls_camellia_crypt_cbc( &camellia, MBEDTLS_CAMELLIA_ENCRYPT,
                        BUFSIZE, tmp, buf, buf ) );
        }
        mbedtls_camellia_free( &camellia );
    }
#endif

#if defined(MBEDTLS_CHACHA20_C)
    if ( todo.chacha20 )
    {
        TIME_AND_TSC( "ChaCha20", mbedtls_chacha20_crypt( buf, buf, 0U, BUFSIZE, buf, buf ) );
    }
#endif

#if defined(MBEDTLS_POLY1305_C)
    if ( todo.poly1305 )
    {
        TIME_AND_TSC( "Poly1305", mbedtls_poly1305_mac( buf, buf, BUFSIZE, buf ) );
    }
#endif

#if defined(MBEDTLS_BLOWFISH_C) && defined(MBEDTLS_CIPHER_MODE_CBC)
    if( todo.blowfish )
    {
        int keysize;
        mbedtls_blowfish_context blowfish;
        mbedtls_blowfish_init( &blowfish );

        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            mbedtls_snprintf( title, sizeof( title ), "BLOWFISH-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            mbedtls_blowfish_setkey( &blowfish, tmp, keysize );

            TIME_AND_TSC( title,
                    mbedtls_blowfish_crypt_cbc( &blowfish, MBEDTLS_BLOWFISH_ENCRYPT, BUFSIZE,
                        tmp, buf, buf ) );
        }

        mbedtls_blowfish_free( &blowfish );
    }
#endif

#if defined(MBEDTLS_HAVEGE_C)
    if( todo.havege )
    {
        mbedtls_havege_state hs;
        mbedtls_havege_init( &hs );
        TIME_AND_TSC( "HAVEGE", mbedtls_havege_random( &hs, buf, BUFSIZE ) );
        mbedtls_havege_free( &hs );
    }
#endif

#if defined(MBEDTLS_CTR_DRBG_C)
    if( todo.ctr_drbg )
    {
        mbedtls_ctr_drbg_context ctr_drbg;

        mbedtls_ctr_drbg_init( &ctr_drbg );
        if( mbedtls_ctr_drbg_seed( &ctr_drbg, myrand, NULL, NULL, 0 ) != 0 )
            mbedtls_exit(1);
        TIME_AND_TSC( "CTR_DRBG (NOPR)",
                mbedtls_ctr_drbg_random( &ctr_drbg, buf, BUFSIZE ) );
        mbedtls_ctr_drbg_free( &ctr_drbg );

        mbedtls_ctr_drbg_init( &ctr_drbg );
        if( mbedtls_ctr_drbg_seed( &ctr_drbg, myrand, NULL, NULL, 0 ) != 0 )
            mbedtls_exit(1);
        mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg, MBEDTLS_CTR_DRBG_PR_ON );
        TIME_AND_TSC( "CTR_DRBG (PR)",
                mbedtls_ctr_drbg_random( &ctr_drbg, buf, BUFSIZE ) );
        mbedtls_ctr_drbg_free( &ctr_drbg );
    }
#endif

#if defined(MBEDTLS_HMAC_DRBG_C)
    if( todo.hmac_drbg )
    {
        mbedtls_hmac_drbg_context hmac_drbg;
        const mbedtls_md_info_t *md_info;

        mbedtls_hmac_drbg_init( &hmac_drbg );

#if defined(MBEDTLS_SHA1_C)
        if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 ) ) == NULL )
            mbedtls_exit(1);

        if( mbedtls_hmac_drbg_seed( &hmac_drbg, md_info, myrand, NULL, NULL, 0 ) != 0 )
            mbedtls_exit(1);
        TIME_AND_TSC( "HMAC_DRBG SHA-1 (NOPR)",
                mbedtls_hmac_drbg_random( &hmac_drbg, buf, BUFSIZE ) );

        if( mbedtls_hmac_drbg_seed( &hmac_drbg, md_info, myrand, NULL, NULL, 0 ) != 0 )
            mbedtls_exit(1);
        mbedtls_hmac_drbg_set_prediction_resistance( &hmac_drbg,
                                             MBEDTLS_HMAC_DRBG_PR_ON );
        TIME_AND_TSC( "HMAC_DRBG SHA-1 (PR)",
                mbedtls_hmac_drbg_random( &hmac_drbg, buf, BUFSIZE ) );
#endif

#if defined(MBEDTLS_SHA256_C)
        if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 ) ) == NULL )
            mbedtls_exit(1);

        if( mbedtls_hmac_drbg_seed( &hmac_drbg, md_info, myrand, NULL, NULL, 0 ) != 0 )
            mbedtls_exit(1);
        TIME_AND_TSC( "HMAC_DRBG SHA-256 (NOPR)",
                mbedtls_hmac_drbg_random( &hmac_drbg, buf, BUFSIZE ) );

        if( mbedtls_hmac_drbg_seed( &hmac_drbg, md_info, myrand, NULL, NULL, 0 ) != 0 )
            mbedtls_exit(1);
        mbedtls_hmac_drbg_set_prediction_resistance( &hmac_drbg,
                                             MBEDTLS_HMAC_DRBG_PR_ON );
        TIME_AND_TSC( "HMAC_DRBG SHA-256 (PR)",
                mbedtls_hmac_drbg_random( &hmac_drbg, buf, BUFSIZE ) );
#endif
        mbedtls_hmac_drbg_free( &hmac_drbg );
    }
#endif

#if defined(MBEDTLS_RSA_C) && defined(MBEDTLS_GENPRIME)
    if( todo.rsa )
    {
        int keysize;
        mbedtls_rsa_context rsa;

        /* Skip 2048 and 4096 bit keys, generating takes too long */
        //NXP for( keysize = 2048; keysize <= 4096; keysize *= 2 )
        for (keysize = 1024; keysize <= 1024; keysize *= 2)
        {
            mbedtls_snprintf( title, sizeof( title ), "RSA-%d", keysize );

            mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
            mbedtls_rsa_gen_key( &rsa, myrand, NULL, keysize, 65537 );

            TIME_PUBLIC( title, " public",
                    buf[0] = 0;
                    ret = mbedtls_rsa_public( &rsa, buf, buf ) );

            TIME_PUBLIC( title, "private",
                    buf[0] = 0;
                    ret = mbedtls_rsa_private( &rsa, myrand, NULL, buf, buf ) );

            mbedtls_rsa_free( &rsa );
        }
    }
#endif

#if defined(MBEDTLS_DHM_C) && defined(MBEDTLS_BIGNUM_C)
    if( todo.dhm )
    {
        int dhm_sizes[] = { 2048, 3072 };
        static const unsigned char dhm_P_2048[] =
            MBEDTLS_DHM_RFC3526_MODP_2048_P_BIN;
        static const unsigned char dhm_P_3072[] =
            MBEDTLS_DHM_RFC3526_MODP_3072_P_BIN;
        static const unsigned char dhm_G_2048[] =
            MBEDTLS_DHM_RFC3526_MODP_2048_G_BIN;
        static const unsigned char dhm_G_3072[] =
            MBEDTLS_DHM_RFC3526_MODP_3072_G_BIN;

        const unsigned char *dhm_P[] = { dhm_P_2048, dhm_P_3072 };
        const size_t dhm_P_size[] = { sizeof( dhm_P_2048 ),
                                      sizeof( dhm_P_3072 ) };

        const unsigned char *dhm_G[] = { dhm_G_2048, dhm_G_3072 };
        const size_t dhm_G_size[] = { sizeof( dhm_G_2048 ),
                                      sizeof( dhm_G_3072 ) };

        mbedtls_dhm_context dhm;
        size_t olen;
        for( i = 0; (size_t) i < sizeof( dhm_sizes ) / sizeof( dhm_sizes[0] ); i++ )
        {
            mbedtls_dhm_init( &dhm );

            if( mbedtls_mpi_read_binary( &dhm.P, dhm_P[i],
                                         dhm_P_size[i] ) != 0 ||
                mbedtls_mpi_read_binary( &dhm.G, dhm_G[i],
                                         dhm_G_size[i] ) != 0 )
            {
                mbedtls_exit( 1 );
            }

            dhm.len = mbedtls_mpi_size( &dhm.P );
            mbedtls_dhm_make_public( &dhm, (int) dhm.len, buf, dhm.len, myrand, NULL );
            if( mbedtls_mpi_copy( &dhm.GY, &dhm.GX ) != 0 )
                mbedtls_exit( 1 );

            mbedtls_snprintf( title, sizeof( title ), "DHE-%d", dhm_sizes[i] );
            TIME_PUBLIC( title, "handshake",
                    ret |= mbedtls_dhm_make_public( &dhm, (int) dhm.len, buf, dhm.len,
                                            myrand, NULL );
                    ret |= mbedtls_dhm_calc_secret( &dhm, buf, sizeof( buf ), &olen, myrand, NULL ) );

            mbedtls_snprintf( title, sizeof( title ), "DH-%d", dhm_sizes[i] );
            TIME_PUBLIC( title, "handshake",
                    ret |= mbedtls_dhm_calc_secret( &dhm, buf, sizeof( buf ), &olen, myrand, NULL ) );

            mbedtls_dhm_free( &dhm );
        }
    }
#endif

#if defined(MBEDTLS_ECDSA_C) && defined(MBEDTLS_SHA256_C)
    if( todo.ecdsa )
    {
        mbedtls_ecdsa_context ecdsa;
        const mbedtls_ecp_curve_info *curve_info;
        size_t sig_len;

        memset( buf, 0x2A, sizeof( buf ) );

        for( curve_info = mbedtls_ecp_curve_list();
             curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
             curve_info++ )
        {
            if( ! mbedtls_ecdsa_can_do( curve_info->grp_id ) )
                continue;

            mbedtls_ecdsa_init( &ecdsa );

            if( mbedtls_ecdsa_genkey( &ecdsa, curve_info->grp_id, myrand, NULL ) != 0 )
                mbedtls_exit( 1 );
            ecp_clear_precomputed( &ecdsa.grp );

            mbedtls_snprintf( title, sizeof( title ), "ECDSA-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "sign",
                    ret = mbedtls_ecdsa_write_signature( &ecdsa, MBEDTLS_MD_SHA256, buf, curve_info->bit_size,
                                                tmp, &sig_len, myrand, NULL ) );

            mbedtls_ecdsa_free( &ecdsa );
        }

        for( curve_info = mbedtls_ecp_curve_list();
             curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
             curve_info++ )
        {
            if( ! mbedtls_ecdsa_can_do( curve_info->grp_id ) )
                continue;

            mbedtls_ecdsa_init( &ecdsa );

            if( mbedtls_ecdsa_genkey( &ecdsa, curve_info->grp_id, myrand, NULL ) != 0 ||
                mbedtls_ecdsa_write_signature( &ecdsa, MBEDTLS_MD_SHA256, buf, curve_info->bit_size,
                                               tmp, &sig_len, myrand, NULL ) != 0 )
            {
                mbedtls_exit( 1 );
            }
            ecp_clear_precomputed( &ecdsa.grp );

            mbedtls_snprintf( title, sizeof( title ), "ECDSA-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "verify",
                    ret = mbedtls_ecdsa_read_signature( &ecdsa, buf, curve_info->bit_size,
                                                tmp, sig_len ) );

            mbedtls_ecdsa_free( &ecdsa );
        }
    }
#endif

#if defined(MBEDTLS_ECDH_C) && defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    if( todo.ecdh )
    {
        mbedtls_ecdh_context ecdh;
        mbedtls_mpi z;
        const mbedtls_ecp_curve_info montgomery_curve_list[] = {
#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
            { MBEDTLS_ECP_DP_CURVE25519, 0, 0, "Curve25519" },
#endif
#if defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
            { MBEDTLS_ECP_DP_CURVE448, 0, 0, "Curve448" },
#endif
            { MBEDTLS_ECP_DP_NONE, 0, 0, 0 }
        };
        const mbedtls_ecp_curve_info *curve_info;
        size_t olen;

        for( curve_info = mbedtls_ecp_curve_list();
             curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
             curve_info++ )
        {
            if( ! mbedtls_ecdh_can_do( curve_info->grp_id ) )
                continue;

            mbedtls_ecdh_init( &ecdh );

            CHECK_AND_CONTINUE( mbedtls_ecp_group_load( &ecdh.grp, curve_info->grp_id ) );
            CHECK_AND_CONTINUE( mbedtls_ecdh_make_public( &ecdh, &olen, buf, sizeof( buf),
                                                    myrand, NULL ) );
            CHECK_AND_CONTINUE( mbedtls_ecp_copy( &ecdh.Qp, &ecdh.Q ) );
            ecp_clear_precomputed( &ecdh.grp );

            mbedtls_snprintf( title, sizeof( title ), "ECDHE-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "handshake",
                    CHECK_AND_CONTINUE( mbedtls_ecdh_make_public( &ecdh, &olen, buf, sizeof( buf),
                                             myrand, NULL ) );
                    CHECK_AND_CONTINUE( mbedtls_ecdh_calc_secret( &ecdh, &olen, buf, sizeof( buf ),
                                             myrand, NULL ) ) );
            mbedtls_ecdh_free( &ecdh );
        }

        /* Montgomery curves need to be handled separately */
        for ( curve_info = montgomery_curve_list;
              curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
              curve_info++ )
        {
            mbedtls_ecdh_init( &ecdh );
            mbedtls_mpi_init( &z );

            CHECK_AND_CONTINUE( mbedtls_ecp_group_load( &ecdh.grp, curve_info->grp_id ) );
            CHECK_AND_CONTINUE( mbedtls_ecdh_gen_public( &ecdh.grp, &ecdh.d, &ecdh.Qp, myrand, NULL ) );

            mbedtls_snprintf( title, sizeof(title), "ECDHE-%s",
                              curve_info->name );
            TIME_PUBLIC(  title, "handshake",
                    CHECK_AND_CONTINUE( mbedtls_ecdh_gen_public( &ecdh.grp, &ecdh.d, &ecdh.Q,
                                            myrand, NULL ) );
                    CHECK_AND_CONTINUE( mbedtls_ecdh_compute_shared( &ecdh.grp, &z, &ecdh.Qp, &ecdh.d,
                                                myrand, NULL ) ) );

            mbedtls_ecdh_free( &ecdh );
            mbedtls_mpi_free( &z );
        }

        for( curve_info = mbedtls_ecp_curve_list();
             curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
             curve_info++ )
        {
            if( ! mbedtls_ecdh_can_do( curve_info->grp_id ) )
                continue;

            mbedtls_ecdh_init( &ecdh );

            CHECK_AND_CONTINUE( mbedtls_ecp_group_load( &ecdh.grp, curve_info->grp_id ) );
            CHECK_AND_CONTINUE( mbedtls_ecdh_make_public( &ecdh, &olen, buf, sizeof( buf),
                                  myrand, NULL ) );
            CHECK_AND_CONTINUE( mbedtls_ecp_copy( &ecdh.Qp, &ecdh.Q ) );
            CHECK_AND_CONTINUE( mbedtls_ecdh_make_public( &ecdh, &olen, buf, sizeof( buf),
                                  myrand, NULL ) );
            ecp_clear_precomputed( &ecdh.grp );

            mbedtls_snprintf( title, sizeof( title ), "ECDH-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "handshake",
                    CHECK_AND_CONTINUE( mbedtls_ecdh_calc_secret( &ecdh, &olen, buf, sizeof( buf ),
                                             myrand, NULL ) ) );
            mbedtls_ecdh_free( &ecdh );
        }

        /* Montgomery curves need to be handled separately */
        for ( curve_info = montgomery_curve_list;
              curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
              curve_info++)
        {
            mbedtls_ecdh_init( &ecdh );
            mbedtls_mpi_init( &z );

            CHECK_AND_CONTINUE( mbedtls_ecp_group_load( &ecdh.grp, curve_info->grp_id ) );
            CHECK_AND_CONTINUE( mbedtls_ecdh_gen_public( &ecdh.grp, &ecdh.d, &ecdh.Qp,
                                 myrand, NULL ) );
            CHECK_AND_CONTINUE( mbedtls_ecdh_gen_public( &ecdh.grp, &ecdh.d, &ecdh.Q, myrand, NULL ) );

            mbedtls_snprintf( title, sizeof(title), "ECDH-%s",
                              curve_info->name );
            TIME_PUBLIC(  title, "handshake",
                    CHECK_AND_CONTINUE( mbedtls_ecdh_compute_shared( &ecdh.grp, &z, &ecdh.Qp, &ecdh.d,
                                                myrand, NULL ) ) );

            mbedtls_ecdh_free( &ecdh );
            mbedtls_mpi_free( &z );
        }
    }
#endif

#if defined(MBEDTLS_ECDH_C)
    if( todo.ecdh )
    {
        mbedtls_ecdh_context ecdh_srv, ecdh_cli;
        unsigned char buf_srv[BUFSIZE], buf_cli[BUFSIZE];
        const mbedtls_ecp_curve_info * curve_list = mbedtls_ecp_curve_list();
        const mbedtls_ecp_curve_info *curve_info;
        size_t olen;

        for( curve_info = curve_list;
            curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
            curve_info++ )
        {
            if( ! mbedtls_ecdh_can_do( curve_info->grp_id ) )
                continue;

            mbedtls_ecdh_init( &ecdh_srv );
            mbedtls_ecdh_init( &ecdh_cli );

            mbedtls_snprintf( title, sizeof( title ), "ECDHE-%s", curve_info->name );
            TIME_PUBLIC( title, "full handshake",
                const unsigned char * p_srv = buf_srv;

                CHECK_AND_CONTINUE( mbedtls_ecdh_setup( &ecdh_srv, curve_info->grp_id ) );
                CHECK_AND_CONTINUE( mbedtls_ecdh_make_params( &ecdh_srv, &olen, buf_srv, sizeof( buf_srv ), myrand, NULL ) );

                CHECK_AND_CONTINUE( mbedtls_ecdh_read_params( &ecdh_cli, &p_srv, p_srv + olen ) );
                CHECK_AND_CONTINUE( mbedtls_ecdh_make_public( &ecdh_cli, &olen, buf_cli, sizeof( buf_cli ), myrand, NULL ) );

                CHECK_AND_CONTINUE( mbedtls_ecdh_read_public( &ecdh_srv, buf_cli, olen ) );
                CHECK_AND_CONTINUE( mbedtls_ecdh_calc_secret( &ecdh_srv, &olen, buf_srv, sizeof( buf_srv ), myrand, NULL ) );

                CHECK_AND_CONTINUE( mbedtls_ecdh_calc_secret( &ecdh_cli, &olen, buf_cli, sizeof( buf_cli ), myrand, NULL ) );
                mbedtls_ecdh_free( &ecdh_cli );

                mbedtls_ecdh_free( &ecdh_srv );
            );

        }
    }
#endif

    mbedtls_printf( "\n[END]\n" );

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
    mbedtls_memory_buffer_alloc_free();
#endif

    while (1)
    {
        char ch = GETCHAR();
        PUTCHAR(ch);
    }
}
