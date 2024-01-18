/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#include "RNG_Interface.h"
#include "fwk_config.h"
#include "FunctionLib.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"

#if !defined gRngUseSecureSubSystem_d || (gRngUseSecureSubSystem_d == 0)
#include "mbedtls/entropy.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/md.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

#define mPRNG_NoOfBits_c      (256U)
#define mPRNG_NoOfBytes_c     (mPRNG_NoOfBits_c / 8U)
#define mPRNG_NoOfLongWords_c (mPRNG_NoOfBits_c / 32U)

#if defined(cPRNGPersonalizationString_d)
static const unsigned char mPrngPersonalizationString_c[] = "cPRNGPersonalizationString_d";
#else
static const unsigned char mPrngPersonalizationString_c[] = "PRNG Personalization String";
#endif /* defined(cPRNGPersonalizationString_c) */

extern osa_status_t SecLibMutexCreate(void);
extern osa_status_t SecLibMutexLock(void);
extern osa_status_t SecLibMutexUnlock(void);

#define RNG_MUTEX_LOCK()   (void)SecLibMutexLock()
#define RNG_MUTEX_UNLOCK() (void)SecLibMutexUnlock()

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

static mbedtls_entropy_context   mRngEntropyCtx;
static mbedtls_hmac_drbg_context mRngHmacDrbgCtx;

static bool_t mRngCtxInitialized = FALSE;
static bool_t mPrngSeeded        = FALSE;

static bool_t   mPolyRngSeeded = FALSE;
static uint32_t mPolyRngRandom = 0xDEADBEEF;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static int RNG_entropy_func(void *data, unsigned char *output, size_t len);

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Initialize the RNG Software Module
 *         Please call SecLib_Init() befoare calling this function to make sure
 *         RNG hardware is correctly initlaized.
 *
 * \return  Status of the RNG initialization procedure.
 *
 ********************************************************************************** */
uint8_t RNG_Init(void)
{
    int     rngResult = 0;
    uint8_t result    = gRngInternalError_d;

    mbedtls_entropy_init(&mRngEntropyCtx);
    mbedtls_hmac_drbg_init(&mRngHmacDrbgCtx);
    mRngCtxInitialized = TRUE;
    do
    {
        (void)SecLibMutexCreate();

        RNG_MUTEX_LOCK();
        rngResult = mbedtls_entropy_func(&mRngEntropyCtx, (unsigned char *)&mPolyRngRandom, sizeof(mPolyRngRandom));
        RNG_MUTEX_UNLOCK();

        if (rngResult != 0)
        {
            break;
        }
        mPolyRngSeeded = TRUE;
        result         = gRngSuccess_d;
    } while (0 != 0);
    return result;
}

void RNG_ReInit(void)
{
#if USE_SENTINEL_RNG
    (void)CRYPTO_ReinitHardware();
#endif
    return;
}

/*! *********************************************************************************
 * \brief  Generates a 32-bit statistically random number
 *         No random number will be generated if the RNG was not initialized
 *         or an error occurs.
 *
 * \param[out]  pRandomNo  Pointer to location where the value will be stored
 *
 ********************************************************************************** */
void RNG_GetRandomNo(uint32_t *pRandomNo)
{
    if ((mPolyRngSeeded == TRUE) && (pRandomNo != NULL))
    {
        mPolyRngRandom = (uint32_t)(((uint64_t)mPolyRngRandom * 279470273UL) % 4294967291UL);
        *pRandomNo     = mPolyRngRandom;
    }
}

/*! *********************************************************************************
 * \brief  Initialize seed for the PRNG algorithm.
 *         If this function is called again, even with a NULL argument,
 *         the PRNG will be reseeded.
 *
 * \param[in]  pSeed  Ignored - please set to NULL
 *             This parameter is ignored because it is no longer needed.
 *             The PRNG is automatically seeded from the true random source.
 *
 ********************************************************************************** */
void RNG_SetPseudoRandomNoSeed(uint8_t *pSeed)
{
    int drbgResult;

    if (mPrngSeeded == FALSE)
    {
        const mbedtls_md_info_t *pMdInfo;

        pMdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
        RNG_MUTEX_LOCK();
        /* Seed the HMAC DRBG from the true entropy source. */
        drbgResult = mbedtls_hmac_drbg_seed(&mRngHmacDrbgCtx, pMdInfo, RNG_entropy_func, &mRngEntropyCtx,
                                            mPrngPersonalizationString_c, sizeof(mPrngPersonalizationString_c));
        RNG_MUTEX_UNLOCK();
        assert(drbgResult == 0);
        (void)drbgResult;

        mPrngSeeded = TRUE;
    }
    else
    {
        RNG_MUTEX_LOCK();
        /* Reseed the HMAC DRBG with no additional seed data. */
        drbgResult = mbedtls_hmac_drbg_reseed(&mRngHmacDrbgCtx, NULL, 0);
        assert(drbgResult == 0);
        (void)drbgResult;
        RNG_MUTEX_UNLOCK();
    }
}

/*! *********************************************************************************
 * \brief  Generates an 256 bit pseudo-random number. The PRNG algorithm used depend
 *         platform's cryptographic hardware and software capabilities.
 *
 * \param[out]  pOut  Pointer to the output buffer (max 32 bytes)
 * \param[in]   outBytes  The number of bytes to be copied (1-32)
 * \param[in]   pSeed  Ignored - please set to NULL
 *              This parameter is ignored because it is no longer needed.
 *              The PRNG is automatically seeded from the true random source.
 *              The length of the seed if present is 32 bytes.
 *
 * \return  The number of bytes copied OR
 *          -1 if reseed is needed OR
 *          0 if he PRNG was not initialized or 0 bytes were requested or an error occurred
 *
 ********************************************************************************** */
int16_t RNG_GetPseudoRandomNo(uint8_t *pOut, uint8_t outBytes, uint8_t *pSeed)
{
    int drbgResult;

    if (mRngCtxInitialized == TRUE)
    {
        if (mPrngSeeded == TRUE)
        {
            if (outBytes == 0U)
            {
                return 0;
            }
            else if (outBytes > mPRNG_NoOfBytes_c)
            {
                outBytes = mPRNG_NoOfBytes_c;
            }
            else
            {
                ; // if (outBytes != 0U && outBytes <= mPRNG_NoOfBytes_c)
            }

            if (pOut == NULL)
            {
                return 0;
            }

            RNG_MUTEX_LOCK();
            drbgResult = mbedtls_hmac_drbg_random(&mRngHmacDrbgCtx, pOut, outBytes);
            RNG_MUTEX_UNLOCK();

            if (drbgResult == 0)
            {
                return (int16_t)outBytes;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return 0;
    }
}

/*! *********************************************************************************
 * \brief  Returns a pointer to the general PRNG function
 *         Call RNG_SetPseudoRandomNoSeed() before calling this function.
 *
 * \return  Function pointer to the general PRNG function or NULL if it
 *          was not seeded.
 *
 ********************************************************************************** */
fpRngPrng_t RNG_GetPrngFunc(void)
{
    if (mPrngSeeded == TRUE)
    {
        return &mbedtls_hmac_drbg_random;
    }
    else
    {
        return NULL;
    }
}

/*! *********************************************************************************
 * \brief  Returns a pointer to the general PRNG context
 *         Call RNG_SetPseudoRandomNoSeed() before calling this function.
 *
 * \return  Function pointer to the general PRNG context or NULL if it
 *          was not initialized correctly.
 *
 ********************************************************************************** */
void *RNG_GetPrngContext(void)
{
    if (mRngCtxInitialized == TRUE)
    {
        return &mRngHmacDrbgCtx;
    }
    else
    {
        return NULL;
    }
}

/*! *********************************************************************************
 * \brief  Returns a pointer to the general RNG Entropy function
 *         Call RNG_Init() before calling this function.
 *
 * \return  Function pointer to the general RNG Entropy function
 *          or NULL if it was not initialized correctly.
 *
 ********************************************************************************** */
fpRngEntropy_t RNG_GetEntropyFunc(void)
{
    if (mRngCtxInitialized == TRUE)
    {
        return &RNG_entropy_func;
    }
    else
    {
        return NULL;
    }
}

/*! *********************************************************************************
 * \brief  Returns a pointer to the general RNG Entropy context
 *         Call RNG_Init() before calling this function.
 *
 * \return  Function pointer to the general RNG Entropy context
 *          or NULL if it was not initialized correctly.
 *
 ********************************************************************************** */
void *RNG_GetEntropyContext(void)
{
    if (mRngCtxInitialized == TRUE)
    {
        return &mRngEntropyCtx;
    }
    else
    {
        return NULL;
    }
}

/*! *********************************************************************************
 * \brief  This function is designed as replacement for the mbedtls_entropy_func()
 *         function for cases where more than MBEDTLS_ENTROPY_BLOCK_SIZE bytes are
 *         required from the entropy function.
 *
 * \param[in]        data    Pointer to a mbedtls_entropy_context structure.
 * \param[in, out]   output  Pointer to the entropy output buffer
 * \param[in]        len     Requested number of entropy bytes.
 *                           Maximum 2*MBEDTLS_ENTROPY_BLOCK_SIZE
 *
 * \return  0 if successful, error code otherwise
 *
 * \remarks This function will call mbedtls_entropy_func() as many times as it is
 *          requiredto provide the requested number of bytes. Though to limit hardware
 *          entropy depeletion the output is limited to 2*MBEDTLS_ENTROPY_BLOCK_SIZE
 *
 ********************************************************************************** */
static int RNG_entropy_func(void *data, unsigned char *output, size_t len)
{
    int result = 0;

    if (len > (2U * (size_t)(MBEDTLS_ENTROPY_BLOCK_SIZE)))
    {
        result = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    }

    if (result == 0)
    {
        while (len >= (size_t)(MBEDTLS_ENTROPY_BLOCK_SIZE))
        {
            RNG_MUTEX_LOCK();
            result = mbedtls_entropy_func(data, output, MBEDTLS_ENTROPY_BLOCK_SIZE);
            RNG_MUTEX_UNLOCK();

            if (result != 0)
            {
                break;
            }
            len    = len - (size_t)MBEDTLS_ENTROPY_BLOCK_SIZE;
            output = output + MBEDTLS_ENTROPY_BLOCK_SIZE;
        }

        /* If no errors were encountered and more bytes are needed call the entropy function again. */
        if ((result == 0) && (len > 0U))
        {
            RNG_MUTEX_LOCK();
            result = mbedtls_entropy_func(data, output, len);
            RNG_MUTEX_UNLOCK();
        }
    }

    return result;
}
#endif /* USE_SENTINEL_RNG */
/********************************** EOF ***************************************/
