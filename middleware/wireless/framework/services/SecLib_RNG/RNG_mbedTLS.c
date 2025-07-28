/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2025 NXP
 *
 * \file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#include "RNG_Interface.h"
#include "fwk_config.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include "fwk_platform.h"
#include "fwk_platform_rng.h"
#include "fwk_platform_crypto.h"
#include "mbedtls/entropy.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/md.h"
#if !defined(gPlatformIsNbu_d)
#include "fwk_workq.h"
#endif
#if defined(gPlatformHasNbu_d) || defined(gPlatformIsNbu_d)
#include "fwk_platform_ics.h"
#endif

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

typedef struct rng_ctx_t
{
    bool_t                  mRngCtxInitialized;
    bool_t                  mPrngIsSeeded;
    bool_t                  mPolyRngSeeded;
    uint32_t                mPolyRngRandom;
    uint32_t                mPRNG_Requests;
    bool_t                  mNeedReseed;
    mbedtls_entropy_context entropy_ctx;
} RNG_context_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */

static int RNG_entropy_func(void *data, unsigned char *output, size_t len);
#if !defined(gPlatformIsNbu_d)
static void RNG_seed_needed_handler(fwk_work_t *work);
#endif

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

static mbedtls_entropy_context   mRngEntropyCtx;
static mbedtls_hmac_drbg_context mRngHmacDrbgCtx;

static RNG_context_t rng_ctx = {
    .mRngCtxInitialized = FALSE,
    .mPrngIsSeeded      = FALSE,
    .mPolyRngSeeded     = FALSE,
    .mPolyRngRandom     = 0xDEADBEEF,
    .mPRNG_Requests     = gRngMaxRequests_d,
    .mNeedReseed        = FALSE,
};

#if !defined(gPlatformIsNbu_d)
static fwk_work_t seed_needed_work = {
    .handler = RNG_seed_needed_handler,
};
#endif

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Initialize the RNG Software Module
 *         Please call SecLib_Init() before calling this function to make sure
 *         RNG hardware is correctly initialized.
 *
 * \return  Status of the RNG initialization procedure.
 *
 ********************************************************************************** */
int RNG_Init(void)
{
    int rngResult = 0;
    int result    = gRngInternalError_d;

    (void)PLATFORM_InitCrypto();

    do
    {
        if (rng_ctx.mRngCtxInitialized == TRUE)
        {
            result = gRngSuccess_d;
            break;
        }
#if !defined(gPlatformIsNbu_d)
        /* The workqueue is used to post and schedule seed
         * trig upon user demand using RNG_NotifyReseedNeeded().
         */
        if (WORKQ_InitSysWorkQ() < 0)
        {
            break;
        }
#endif
        (void)SecLibMutexCreate();

        RNG_MUTEX_LOCK();

#if defined(gPlatformHasNbu_d)
        PLATFORM_RegisterReceivedSeedRequest(&RNG_NotifyReseedNeeded);
#endif

#if defined(gPlatformIsNbu_d)
        PLATFORM_RegisterSetNewSeed(&RNG_SetExternalSeed);
#endif
        mbedtls_entropy_init(&mRngEntropyCtx);
        mbedtls_hmac_drbg_init(&mRngHmacDrbgCtx);

        rngResult = mbedtls_entropy_func(&mRngEntropyCtx, (unsigned char *)&rng_ctx.mPolyRngRandom,
                                         sizeof(rng_ctx.mPolyRngRandom));
        RNG_MUTEX_UNLOCK();

        if (rngResult != 0)
        {
            break;
        }
        rng_ctx.mPolyRngSeeded     = TRUE;
        rng_ctx.mRngCtxInitialized = TRUE;

        /* Set seed for pseudo random number generation */
        (void)RNG_SetSeed();

        result = gRngSuccess_d;
    } while (0 != 0);

    return result;
}

/*! *********************************************************************************
 * \brief  Reinitialize the RNG module post-wakeup.
 *         May do nothing, action is dependent on platform.
 *
 * \return  gRngSuccess_d if successful, gRngInternalError_d if operation fails.
 *
 * Note: Failure only possible for specific platforms.
 *
 ********************************************************************************** */
int RNG_ReInit(void)
{
    int result = gRngSuccess_d;
    if (PLATFORM_ResetCrypto() != 0)
    {
        result = gRngInternalError_d;
    }
    return result;
}

/*! *********************************************************************************
 * \brief  DeInitialize the RNG module.
 *         Resets the RNG context variables. Only used for test purposes.
 *
 * \return none
 *
 ********************************************************************************** */
void RNG_DeInit(void)
{
    rng_ctx.mRngCtxInitialized = FALSE;
    rng_ctx.mPrngIsSeeded      = FALSE;
    rng_ctx.mPolyRngSeeded     = FALSE;
    rng_ctx.mPolyRngRandom     = 0xDEADBEEF;
    rng_ctx.mPRNG_Requests     = gRngMaxRequests_d;
    rng_ctx.mNeedReseed        = FALSE;
}

/*! *********************************************************************************
 * \brief  Generates a 32-bit statistically random number
 *         No random number will be generated if the RNG was not initialized
 *         or an error occurs.
 *
 * \param[out]  pRandomNo  Pointer to location where the value will be stored
 *
 ********************************************************************************** */
int RNG_GetTrueRandomNumber(uint32_t *pRandomNo)
{
    int ret;

    do
    {
        if (pRandomNo == NULL)
        {
            ret = gRngBadArguments_d;
            break;
        }

        if (rng_ctx.mRngCtxInitialized != TRUE)
        {
            ret = gRngNotInitialized_d;
            break;
        }

        if (RNG_entropy_func(&mRngEntropyCtx, (unsigned char *)pRandomNo, sizeof(uint32_t)) == 0)
        {
            ret = gRngSuccess_d;
        }
        else
        {
            ret = gRngInternalError_d;
        }
    } while (0 != 0);
    return ret;
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
int RNG_GetPseudoRandomData(uint8_t *pOut, uint8_t outBytes, uint8_t *pSeed)
{
    int ret;

    NOT_USED(pSeed);

    do
    {
        int drbgResult;

        if (pOut == NULL || outBytes == 0u)
        {
            ret = gRngBadArguments_d;
            break;
        }

        if (rng_ctx.mRngCtxInitialized != TRUE)
        {
            ret = gRngNotInitialized_d;
            break;
        }

        if (!rng_ctx.mPrngIsSeeded)
        {
            /* request reseed */
            if (RNG_NotifyReseedNeeded() < 0)
            {
                /* if request fails, bad news */
                ret = gRngInternalError_d;
            }
            else
            {
                /* wait for reseed to retry */
                ret = gRngReseedPending_d;
            }
            break;
        }
        else
        {
            if (rng_ctx.mPRNG_Requests == gRngMaxRequests_d)
            {
                if (RNG_NotifyReseedNeeded() < 0)
                {
                    ret = gRngInternalError_d;
                    break;
                }
            }
            /* Continue in spite of the gRngMaxRequests_d limit reached */
            rng_ctx.mPRNG_Requests++;

            if (outBytes > mPRNG_NoOfBytes_c)
            {
                outBytes = mPRNG_NoOfBytes_c;
            }
            else
            {
                ; // if (outBytes != 0U && outBytes <= mPRNG_NoOfBytes_c)
            }

            RNG_MUTEX_LOCK();
            drbgResult = mbedtls_hmac_drbg_random(&mRngHmacDrbgCtx, pOut, outBytes);
            RNG_MUTEX_UNLOCK();

            if (drbgResult != 0)
            {
                ret = gRngInternalError_d;
                break;
            }
            ret = (int)outBytes;
        }
    } while (false);

    return ret;
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
    if (rng_ctx.mPrngIsSeeded == TRUE)
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
    if (rng_ctx.mRngCtxInitialized == TRUE)
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
    if (rng_ctx.mRngCtxInitialized == TRUE)
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
    if (rng_ctx.mRngCtxInitialized == TRUE)
    {
        return &mRngEntropyCtx;
    }
    else
    {
        return NULL;
    }
}

int RNG_SetSeed(void)
{
    int status = gRngInternalError_d;

    int drbgResult;
    if (rng_ctx.mRngCtxInitialized == TRUE)
    {
        if (rng_ctx.mPrngIsSeeded == FALSE)
        {
            const mbedtls_md_info_t *pMdInfo;

            pMdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
            RNG_MUTEX_LOCK();
            /* Seed the HMAC DRBG from the true entropy source. */
            drbgResult = mbedtls_hmac_drbg_seed(&mRngHmacDrbgCtx, pMdInfo, RNG_entropy_func, &mRngEntropyCtx,
                                                mPrngPersonalizationString_c, sizeof(mPrngPersonalizationString_c));
            RNG_MUTEX_UNLOCK();
        }
        else
        {
            RNG_MUTEX_LOCK();
            /* Reseed the HMAC DRBG with no additional seed data. */
            drbgResult = mbedtls_hmac_drbg_reseed(&mRngHmacDrbgCtx, NULL, 0);
            RNG_MUTEX_UNLOCK();
        }

        assert(drbgResult == 0);
        if (drbgResult == 0)
        {
            status                = gRngSuccess_d;
            rng_ctx.mPrngIsSeeded = TRUE;
        }

        /* On RNG_mbedTLS as the seed is only managed by mbedTLS layer we cannot send it to another core */
        rng_ctx.mNeedReseed = FALSE;

        /* Reset to 1 the request to pseudo random number generation when reseeding */
        rng_ctx.mPRNG_Requests = 1U;
    }
    return status;
}

int RNG_NotifyReseedNeeded(void)
{
    int status = 1;

    rng_ctx.mNeedReseed = TRUE;

#if !defined(gPlatformIsNbu_d)
    status = WORKQ_Submit(&seed_needed_work);
#endif
    return status;
}

bool_t RNG_IsReseedNeeded(void)
{
    return rng_ctx.mNeedReseed;
}

int RNG_SetExternalSeed(uint8_t *external_seed)
{
    (void)external_seed;
    return 1; /* External seeding is not supported on mbedtls */
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
 *          required to provide the requested number of bytes. Though to limit hardware
 *          entropy depletion the output is limited to 2*MBEDTLS_ENTROPY_BLOCK_SIZE
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

#if !defined(gPlatformIsNbu_d)
static void RNG_seed_needed_handler(fwk_work_t *work)
{
    if (rng_ctx.mNeedReseed == TRUE)
    {
        (void)RNG_SetSeed();
    }
}
#endif
/********************************** EOF ***************************************/
