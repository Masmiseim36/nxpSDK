/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"

#if defined(FSL_FEATURE_SOC_CASPER_COUNT) && (FSL_FEATURE_SOC_CASPER_COUNT > 0)
#include "fsl_casper.h"

#if defined(MIMXRT685S_cm33_SERIES) || defined(MIMXRT595S_cm33_SERIES)
#include "fsl_power.h"
#endif


#if (defined(MBEDTLS_MCUX_CASPER_ECC) && (MBEDTLS_MCUX_CASPER_ECC == 1)) || (defined(MBEDTLS_MCUX_CASPER_PKHA) && (MBEDTLS_MCUX_CASPER_PKHA == 1) && defined(MBEDTLS_RSA_PUBLIC_ALT) && defined(MBEDTLS_RSA_C))
/* Initialize CASPER */
static volatile bool casper_init_is_done = false;
static void mbedtls_casper_init(void)
{
#if defined(MIMXRT685S_cm33_SERIES)
    /*Make sure casper ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_CASPER_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_CASPER_SRAM);
    POWER_ApplyPD();
#endif    

#if defined(MIMXRT595S_cm33_SERIES)
    /*Make sure casper ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_PPD_CASPER_SRAM);
    POWER_ApplyPD();
#endif    

    /* Initialize CASPER */
    CASPER_Init(CASPER);
    
    casper_init_is_done = true;
}

static void reverse_array(uint8_t *src, size_t src_len)
{
    int i;

    for (i = 0; i < src_len / 2; i++)
    {
        uint8_t tmp;

        tmp = src[i];
        src[i] = src[src_len - 1 - i];
        src[src_len - 1 - i] = tmp;
    }
}
#endif

/******************************************************************************/
/*************************** PKHA  ********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_CASPER_PKHA) && (MBEDTLS_MCUX_CASPER_PKHA == 1) && defined(MBEDTLS_RSA_PUBLIC_ALT) && defined(MBEDTLS_RSA_C)

#include "mbedtls/rsa.h"
#include "mbedtls/oid.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free   free
#endif

/* Parameter validation macros */
#define RSA_VALIDATE_RET( cond )                                       \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_RSA_BAD_INPUT_DATA )
/*
 * Do an RSA public key operation
 */
static int mbedtls_casper_mpi_exp_mod_shim(mbedtls_mpi *X,
                                    const mbedtls_mpi *A,
                                    const mbedtls_mpi *E,
                                    const mbedtls_mpi *N /*, mbedtls_mpi *_RR */)
{
    int ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
    size_t sizeA = mbedtls_mpi_size(A);
    size_t sizeN = mbedtls_mpi_size(N);
    uint8_t *ptrX = mbedtls_calloc(3, MBEDTLS_MCUX_PKHA_INT_MAX_BYTES);
    uint8_t *ptrA = ptrX + MBEDTLS_MCUX_PKHA_INT_MAX_BYTES;
    uint8_t *ptrN = ptrA + MBEDTLS_MCUX_PKHA_INT_MAX_BYTES;

    if (NULL == ptrX)
    {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;   
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(A, ptrA, sizeA));
    reverse_array(ptrA, sizeA);

    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(N, ptrN, sizeN));
    reverse_array(ptrN, sizeN);

    CASPER_ModExp(CASPER, ptrA, ptrN, sizeN / 4U, E->MBEDTLS_PRIVATE(p)[0], ptrX);

    reverse_array(ptrX, sizeN);
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(X, ptrX, sizeN));
cleanup:
    if (ptrX != NULL)
    {
        mbedtls_free(ptrX);
    }

    return ret;
}

/*
 * Do an RSA public key operation
 */
int mbedtls_rsa_public( mbedtls_rsa_context *ctx,
                const unsigned char *input,
                unsigned char *output )
{
    int ret;
    size_t olen;
    mbedtls_mpi T;
    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( input != NULL );
    RSA_VALIDATE_RET( output != NULL );
    
    if(casper_init_is_done == false)
    {
        mbedtls_casper_init();
    }

    mbedtls_mpi_init(&T);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&ctx->mutex)) != 0)
    {
        return (ret);
    }
#endif

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&T, input, ctx->MBEDTLS_PRIVATE(len)));

    if (mbedtls_mpi_cmp_mpi(&T, &ctx->MBEDTLS_PRIVATE(N)) >= 0)
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }

    olen = ctx->MBEDTLS_PRIVATE(len);
    MBEDTLS_MPI_CHK(mbedtls_casper_mpi_exp_mod_shim(&T, &T, &ctx->MBEDTLS_PRIVATE(E), &ctx->MBEDTLS_PRIVATE(N) /*, &ctx->RN */));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&T, output, olen));

cleanup:
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&ctx->mutex) != 0)
    {
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);
    }
#endif

    mbedtls_mpi_free(&T);

    if (ret != 0)
    {
        return (MBEDTLS_ERR_RSA_PUBLIC_FAILED + ret);
    }

    return (0);
}

#endif /* MBEDTLS_MCUX_CASPER_PKHA && MBEDTLS_RSA_PUBLIC_ALT && MBEDTLS_RSA_C */


/******************************************************************************/
/*************************** ECP  *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/threading.h"
#include "mbedtls/platform_util.h"

#include <string.h>

#if defined(MBEDTLS_MCUX_CASPER_ECC) && (MBEDTLS_MCUX_CASPER_ECC == 1)


#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED) || defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
#error "CASPER hw acceleration supports only SECP256R1, SECP384R1  and SECP521R1."
#endif

/* CASPER driver allows usage of 256, 384 and 521 ECC */
#define CASPER_MAX_ECC_SIZE_BITS (521)
#define CASPER_MAX_ECC_SIZE_BYTES (72) /* 32 for 256 bits, 48 for 384 bits and 72 for 521 bits*/     
          
/* Parameter validation macros based on platform_util.h */
#define ECP_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECP_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

#if defined(MBEDTLS_ECP_MUL_COMB_ALT)
int ecp_mul_comb(mbedtls_ecp_group *grp,
                 mbedtls_ecp_point *R, //result
                 const mbedtls_mpi *m, //scalar
                 const mbedtls_ecp_point *P, // X & Y
                 int (*f_rng)(void *, unsigned char *, size_t),
                 void *p_rng,
                 mbedtls_ecp_restart_ctx *rs_ctx ) /* TBD: rs_ctx is not used MBEDTLS_ECP_RESTARTABLE is not supported */
{
    size_t size;
    
    if(casper_init_is_done == false)
    {
        mbedtls_casper_init();
    }
  
    size = mbedtls_mpi_size(&grp->P);
  
    uint32_t M[CASPER_MAX_ECC_SIZE_BYTES/ sizeof(uint32_t)] = {0};
    uint32_t X[CASPER_MAX_ECC_SIZE_BYTES/ sizeof(uint32_t)] = {0};
    uint32_t Y[CASPER_MAX_ECC_SIZE_BYTES/ sizeof(uint32_t)] = {0};

    
    //* Write MbedTLS mpi coordinates into binary buffer */
    mbedtls_mpi_write_binary( &P->MBEDTLS_PRIVATE(X), (unsigned char*)&X[0], size );
    mbedtls_mpi_write_binary( &P->MBEDTLS_PRIVATE(Y), (unsigned char*)&Y[0], size );
    
    /* Reverse endianness for CASPER */
    reverse_array((uint8_t *)X, size);
    reverse_array((uint8_t *)Y, size);
        
    /* Init CASPER */
    if (size == 32)
    {  
    CASPER_ecc_init(kCASPER_ECC_P256);
    }
    if (size == 48)
    {  
    CASPER_ecc_init(kCASPER_ECC_P384);
    }
    if (size == 66)
    {  
    CASPER_ecc_init(kCASPER_ECC_P521);
    }

    if (mbedtls_mpi_size(m) > sizeof(M))
    {
        __BKPT(0);
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
    }
    mbedtls_mpi_write_binary(m, (void *)M, size);
    reverse_array((void *)M, size);
    
    if (size == 32)
    { 
    CASPER_ECC_SECP256R1_Mul(CASPER, &X[0], &Y[0], &X[0], &Y[0], (void *)M);
    }
    
    if (size == 48)
    {  
    CASPER_ECC_SECP384R1_Mul(CASPER, &X[0], &Y[0], &X[0], &Y[0], (void *)M);
    }
	 if (size == 66)
    {  
    CASPER_ECC_SECP521R1_Mul(CASPER, &X[0], &Y[0], &X[0], &Y[0], (void *)M);
    }
    /* Reverse results back to MbedTLS format */
    reverse_array((uint8_t *)X, size);
    reverse_array((uint8_t *)Y, size);

    /* Write results into R MPI */
    mbedtls_mpi_read_binary( &R->MBEDTLS_PRIVATE(X), (void*)&X[0], size );
    mbedtls_mpi_read_binary( &R->MBEDTLS_PRIVATE(Y), (void*)&Y[0], size );
    mbedtls_mpi_lset( &R->MBEDTLS_PRIVATE(Z), 1 );


    return 0;
}
#endif /* MBEDTLS_ECP_MUL_COMB_ALT */

#if defined(MBEDTLS_ECP_MULADD_ALT)

extern int ecp_add( const mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                    const mbedtls_ecp_point *P,
                    const mbedtls_ecp_point *Q );
/*
 * Restartable linear combination
 * NOT constant-time
 */
int mbedtls_ecp_muladd_restartable(
             mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             const mbedtls_mpi *n, const mbedtls_ecp_point *Q,
             mbedtls_ecp_restart_ctx *rs_ctx ) /* TBD restartable is not implemented */
{
    uint32_t X1[CASPER_MAX_ECC_SIZE_BYTES / sizeof(uint32_t)] = {0};
    uint32_t Y1[CASPER_MAX_ECC_SIZE_BYTES / sizeof(uint32_t)] = {0};
    uint32_t X2[CASPER_MAX_ECC_SIZE_BYTES / sizeof(uint32_t)] = {0};
    uint32_t Y2[CASPER_MAX_ECC_SIZE_BYTES / sizeof(uint32_t)] = {0};
    uint32_t M[CASPER_MAX_ECC_SIZE_BYTES / sizeof(uint32_t)] = {0};
    uint32_t N[CASPER_MAX_ECC_SIZE_BYTES / sizeof(uint32_t)] = {0};
    
    size_t size;
    
    if(casper_init_is_done == false)
    {
        mbedtls_casper_init();
    }

    /* shortcut for (m == 1) && (n == 1). this case is point addition. */
    /* this shortcut follows original mbedtls_ecp_muladd() implementation */
    /* and is required for ecjpake_ecp_add3(). */
    if ((mbedtls_mpi_cmp_int(m, 1) == 0) && (mbedtls_mpi_cmp_int(n, 1) == 0))
    {
        return ecp_add(grp, R, P, Q);
    }
    
    /* Write MbedTLS mpi coordinates into binary buffer */
    size = mbedtls_mpi_size(&grp->P);
       
    mbedtls_mpi_write_binary( &P->MBEDTLS_PRIVATE(X), (unsigned char*)&X1[0], size );
    mbedtls_mpi_write_binary( &P->MBEDTLS_PRIVATE(Y), (unsigned char*)&Y1[0], size );
    
    reverse_array((uint8_t *)X1, size);
    reverse_array((uint8_t *)Y1, size);
    
    
    /* Init CASPER */
    if (size == 32)
    {  
        CASPER_ecc_init(kCASPER_ECC_P256);
    }
    else if (size == 48)
    {  
        CASPER_ecc_init(kCASPER_ECC_P384);
    }
	else if (size == 66)
    {  
        CASPER_ecc_init(kCASPER_ECC_P521);
    }
    //TBD assert
    if (mbedtls_mpi_size(m) > sizeof(M))
    {
        __BKPT(0);
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
    }
    mbedtls_mpi_write_binary(m, (void *)M, size);
    reverse_array((void *)M, size);

    /* Write MbedTLS mpi coordinates into binary bufer */
    mbedtls_mpi_write_binary( &Q->MBEDTLS_PRIVATE(X), (unsigned char*)&X2[0], size );
    mbedtls_mpi_write_binary( &Q->MBEDTLS_PRIVATE(Y), (unsigned char*)&Y2[0], size );
    
    reverse_array((uint8_t *)X2, size);
    reverse_array((uint8_t *)Y2, size);

    if (mbedtls_mpi_size(n) > sizeof(N))
    {
        __BKPT(0);
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
    }
    mbedtls_mpi_write_binary(n, (void *)N, size);
    reverse_array((void *)N, size);
    
    if (size == 32)
    { 
        CASPER_ECC_SECP256R1_MulAdd(CASPER, &X1[0], &Y1[0], &X1[0], &Y1[0], (void *)M, &X2[0], &Y2[0], (void *)N);
    }
    else if (size == 48)
    { 
        CASPER_ECC_SECP384R1_MulAdd(CASPER, &X1[0], &Y1[0], &X1[0], &Y1[0], (void *)M, &X2[0], &Y2[0], (void *)N);
    } else if (size == 66)
    { 
        CASPER_ECC_SECP521R1_MulAdd(CASPER, &X1[0], &Y1[0], &X1[0], &Y1[0], (void *)M, &X2[0], &Y2[0], (void *)N);
    }
    //TBD assert
    
    /* Reverse results back to MbedTLS format */
    reverse_array((uint8_t *)X1, size);
    reverse_array((uint8_t *)Y1, size);

    /* Write results into R MPI */
    mbedtls_mpi_read_binary( &R->MBEDTLS_PRIVATE(X), (void*)&X1[0], size );
    mbedtls_mpi_read_binary( &R->MBEDTLS_PRIVATE(Y), (void*)&Y1[0], size );
    mbedtls_mpi_lset( &R->MBEDTLS_PRIVATE(Z), 1 );
    
    return 0;
}

/*
 * Linear combination
 * NOT constant-time
 */
int mbedtls_ecp_muladd( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             const mbedtls_mpi *n, const mbedtls_ecp_point *Q )
{
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( R   != NULL );
    ECP_VALIDATE_RET( m   != NULL );
    ECP_VALIDATE_RET( P   != NULL );
    ECP_VALIDATE_RET( n   != NULL );
    ECP_VALIDATE_RET( Q   != NULL );
    
    return( mbedtls_ecp_muladd_restartable( grp, R, m, P, n, Q, NULL ) );
}
#endif /* MBEDTLS_ECP_MULADD_ALT */

#endif /* MBEDTLS_MCUX_CASPER_ECC */

#endif /* MBEDTLS_ECP_C */


#endif /* FSL_FEATURE_SOC_CASPER_COUNT */
