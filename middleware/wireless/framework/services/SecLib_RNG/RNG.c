/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2019, 2023-2024 NXP
 * All rights reserved.
 *
 * \file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#include <stdalign.h>
#include "RNG_Interface.h"
#include "FunctionLib.h"
#include "fsl_common.h" /* includes fsl_device_registers.h */
#include "fsl_os_abstraction.h"
#include "fwk_platform_rng.h"
#include "fwk_config.h"

#ifndef gRngUseSecLib_d
#define gRngUseSecLib_d 1
#endif

#if (gRngUseSecLib_d == 1)
#include "SecLib.h"
#else
/* If the PRNG is using a Lehmer linear congruential generator, no cryptographic method is used, neither SHA256, nor
   HMAC or AES-CTR. In this case the implementation avoids the dependency on the SecLib. Using the S200 TRNG, still
   pulls this dependency. Seeding the PRNG requires a proper entropy collection. The NBU uses the S200 EdgeLock TRNG
   service indirectly via RPMSG for seeding purposes.
 */
#define gRngUseLehmerGen_c 1
#endif

#if defined(CPU_MCXW345CHNA)
#define TRNG0      TRNG
#define TRNG0_IRQn TRNG_IRQn
#endif

#if defined gRngUseSecureSubSystem_d && (gRngUseSecureSubSystem_d != 0)
#include "sss_crypto.h"
#else
#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
#include "fsl_trng.h"
#elif ((defined(FSL_FEATURE_SOC_RNG_COUNT)) && (FSL_FEATURE_SOC_RNG_COUNT > 0U))
#if defined CPU_QN908X
#include "fsl_rng.h"
#include "fsl_power.h"
#else
#include "fsl_rnga.h"
#endif
#endif
#endif
#if defined(gRngSeedStorageAddr_d) || defined(gRngSeedHwParamStorage_d)
#include "HWParameter.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

#ifndef gRNG_UsePhyRngForInitialSeed_d
#define gRNG_UsePhyRngForInitialSeed_d 0
#endif

#define mPRNG_NoOfBits_c      (256U)
#define mPRNG_NoOfBytes_c     (mPRNG_NoOfBits_c / 8U)
#define mPRNG_NoOfLongWords_c (mPRNG_NoOfBits_c / 32U)

#ifndef RAISE_ERROR
#define RAISE_ERROR(_X_, _VAL_) \
    {                           \
        _X_ = _VAL_;            \
        break;                  \
    }
#endif

#if gRngHasSecLibDependency_d
/* Share mutex with SecLib if RNG involves crypto, or Secure subsystem */
extern osa_status_t SecLibMutexCreate(void);
extern osa_status_t SecLibMutexLock(void);
extern osa_status_t SecLibMutexUnlock(void);

#define RNG_MUTEX_CREATE() SecLibMutexCreate()
#define RNG_MUTEX_LOCK()   (void)SecLibMutexLock()
#define RNG_MUTEX_UNLOCK() (void)SecLibMutexUnlock()

#else /* gRngHasSecLibDependency_d */

#define RNG_MUTEX_CREATE() KOSA_StatusSuccess
#define RNG_MUTEX_LOCK()
#define RNG_MUTEX_UNLOCK()

#endif

#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
#if (defined(RW610_SERIES) || defined(RW612_SERIES))
#define TRNG0      TRNG
#define TRNG0_IRQn TRNG_IRQn
#endif
#endif

typedef struct rng_ctx_t
{
    bool_t Initialized;
    bool_t mPrngIsSeeded;
    bool_t needReseed;
    alignas(4) uint32_t PrngSeed[mPRNG_NoOfLongWords_c];
    uint32_t mPRNG_Requests;
} RNG_context_t;

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

static RNG_context_t rng_ctx = {
    .Initialized                               = FALSE,
    .mPrngIsSeeded                             = FALSE,
    .needReseed                                = FALSE,
    .PrngSeed[0 ... mPRNG_NoOfLongWords_c - 1] = 0U,
    .mPRNG_Requests                            = gRngMaxRequests_d,
};

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
#if ((!defined(FSL_FEATURE_SOC_TRNG_COUNT)) || (FSL_FEATURE_SOC_TRNG_COUNT == 0U)) && \
    ((!defined(FSL_FEATURE_SOC_RNG_COUNT)) || (FSL_FEATURE_SOC_RNG_COUNT == 0U)) && (gRNG_UsePhyRngForInitialSeed_d)
extern void PhyGetRandomNo(uint32_t *pRandomNo);
#endif
/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#if (defined gRngUseLehmerGen_c && (gRngUseLehmerGen_c > 0))
static uint32_t RNG_LCG(uint32_t prev_state);
#endif

#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
static void TRNG_ISR(void);
static void TRNG_GoToSleep(void);
#endif

static int      RNG_Specific_Init(uint32_t *pSeed);
static uint16_t RNG_Specific_GetRandomData(uint8_t *pOut, uint16_t outBytes);

#if defined(gRngSeedHwParamStorage_d) || defined(gRngSeedStorageAddr_d)
static uint32_t Rng_read_seed_from_flash(void);
static void     Rng_save_seed_to_flash(uint32_t seed);
#endif

static int RNG_GetPseudoRandomDataWithContext(void *ctx_data, unsigned char *output, size_t len);

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Initialize the RNG HW module.
 *         The PRNG seeding may be asynchronous, depending on platforms.
 *
 * \return  Status of the RNG initialization sequence
 *
 ********************************************************************************** */
int RNG_Init(void)
{
    int status = gRngInternalError_d;
    do
    {
        /* Check if RNG is already initialized */
        if (rng_ctx.Initialized)
        {
            status = gRngSuccess_d;
            break;
        }
        /* Create mutex here in case it was not done already.
         * Does nothing without error otherwise.
         */
        if (RNG_MUTEX_CREATE() != KOSA_StatusSuccess)
        {
            break;
        }

        rng_ctx.mPrngIsSeeded = FALSE;

        status = RNG_Specific_Init(rng_ctx.PrngSeed);
        /* RNG_Specific_Init retuns with mPrngIsSeeded set to TRUE if the seed can be obtained synchronously */

        if (status != gRngSuccess_d)
        {
            break;
        }

        rng_ctx.Initialized = TRUE;
        /* If seed was not set during RNG_Specific_Init initialization, skip */
        if (!rng_ctx.mPrngIsSeeded)
        {
            break;
        }
        status = RNG_SetSeed();

    } while (false);

    return status;
}

int RNG_ReInit(void)
{
    int status = gRngSuccess_d;
#if gRngUseSecureSubSystem_d
    status = CRYPTO_ReinitHardware();
#endif
    return status;
}

int RNG_GetTrueRandomNumber(uint32_t *pRandomNo)
{
    int status = -1;

    uint16_t nb_bytes = 0u;
    if (rng_ctx.Initialized)
    {
        nb_bytes = RNG_Specific_GetRandomData((uint8_t *)pRandomNo, sizeof(uint32_t));
        if (nb_bytes == sizeof(uint32_t))
        {
            status = 0;
        }
    }
    return status;
}

/*! *********************************************************************************
 * \brief  Pseudo Random Number Generator (PRNG) implementation
 *         according to NIST FIPS Publication 186-2, APPENDIX 3
 *
 *         Let x be the signer's private key.
 *         The following may be used to generate m values of x:
 *           Step 1. Choose a new, secret value for the seed-key, XKEY.
 *           Step 2. In hexadecimal notation let
 *             t = 67452301 EFCDAB89 98BADCFE 10325476 C3D2E1F0.
 *             This is the initial value for H0 || H1 || H2 || H3 || H4 in the SHS.
 *           Step 3. For j = 0 to m - 1 do
 *             a. XSEEDj = optional user input.
 *             b. XVAL = (XKEY + XSEEDj) mod 2^b
 *             c. xj = G(t,XVAL) mod q
 *             d. XKEY = (1 + XKEY + xj) mod 2^b
 *
 * \param[out]    pOut - pointer to the output buffer
 * \param[in]     outBytes - the number of bytes to be copied (1-20)
 * \param[in]     pSeed - optional user SEED. Should be NULL if not used.
 *
 * \return  The number of bytes copied or -1 if reseed is needed
 *
 ********************************************************************************** */
int RNG_GetPseudoRandomData(uint8_t *pOut, uint8_t outBytes, uint8_t *pSeed)
{
    uint16_t outputBytes = 0UL;
    if (rng_ctx.Initialized && rng_ctx.mPrngIsSeeded)
    {
        outputBytes = outBytes;

#if !defined gRngUseSecureSubSystem_d || (gRngUseSecureSubSystem_d == 0)
        uint32_t i;
        /* PRNG buffer used for both input and output */
        uint32_t prngBuffer[mPRNG_NoOfLongWords_c] = {0};

        if (pSeed != NULL)
        {
            rng_ctx.mPRNG_Requests = 1U;
        }

        if (rng_ctx.mPRNG_Requests == gRngMaxRequests_d)
        {
            RNG_NotifyReseedNeeded();
        }

        rng_ctx.mPRNG_Requests++;

        /* b. XVAL = (XKEY + XSEEDj) mod 2^b */
        for (i = 0u; i < mPRNG_NoOfLongWords_c; i++)
        {
            prngBuffer[i] = rng_ctx.PrngSeed[i]; /* PrngSeed is the XKEY */
        }
        /* a. XSEEDj = optional user input. */
        if (pSeed != NULL)
        {
            for (i = 0; i < mPRNG_NoOfBytes_c; i++)
            {
                ((uint8_t *)prngBuffer)[i] += pSeed[i];
            }
        }

        /* c. xj = G(t,XVAL) mod q
        ***************************/

#if (defined gRngUseLehmerGen_c && (gRngUseLehmerGen_c > 0))
        for (i = 0u; i < mPRNG_NoOfLongWords_c; i++)
        {
            uint32_t rand_val;
            rand_val      = RNG_LCG(rng_ctx.PrngSeed[i]); /* PrngSeed is the XKEY */
            prngBuffer[i] = rand_val;
        }
#else
        SHA256_Hash((uint8_t *)prngBuffer, mPRNG_NoOfBytes_c, (uint8_t *)prngBuffer);
#endif
        /* d. XKEY = (1 + XKEY + xj) mod 2^b */
        rng_ctx.PrngSeed[0] += 1U; /* PrngSeed is the XKEY */
        for (i = 0; i < mPRNG_NoOfLongWords_c; i++)
        {
            rng_ctx.PrngSeed[i] += prngBuffer[i];
        }

        /* Check if the length provided exceeds the output data size */
        if (outputBytes > mPRNG_NoOfBytes_c)
        {
            outputBytes = mPRNG_NoOfBytes_c;
        }
        /* Copy the generated number */
        for (i = 0; i < (uint32_t)outputBytes; i++)
        {
            pOut[i] = ((uint8_t *)prngBuffer)[i];
        }
#else /* gRngUseSecureSubSystem_d */
        if (outputBytes > mPRNG_NoOfBytes_c)
        {
            outputBytes = mPRNG_NoOfBytes_c;
        }
        outputBytes = RNG_Specific_GetRandomData(pOut, outputBytes);
#endif
    }
    return (int16_t)outputBytes;
}

static int RNG_GetPseudoRandomDataWithContext(void *ctx_data, unsigned char *output, size_t len)
{
    NOT_USED(ctx_data);
    int st = -1;
    int nb_produced;

    nb_produced = RNG_GetPseudoRandomData(output, (uint8_t)len, NULL);

    if (nb_produced == (int)len)
    {
        st = 0;
    }
    return st;
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
    return RNG_GetPseudoRandomDataWithContext;
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
    return (void *)&rng_ctx;
}

int RNG_SetSeed(void)
{
    int status = gRngInternalError_d;
    if (rng_ctx.Initialized == TRUE)
    {
        uint16_t nb_bytes = 0U;
        nb_bytes          = RNG_Specific_GetRandomData((uint8_t *)rng_ctx.PrngSeed, mPRNG_NoOfBytes_c);
        if (nb_bytes == mPRNG_NoOfBytes_c)
        {
            rng_ctx.mPRNG_Requests = 1U;
            rng_ctx.mPrngIsSeeded  = TRUE;

            if (PLATFORM_SendRngSeed((uint8_t *)rng_ctx.PrngSeed, mPRNG_NoOfBytes_c) >= 0)
            {
                rng_ctx.needReseed = FALSE;
                status             = gRngSuccess_d;
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            assert(0);
        }
    }
    return status;
}

int RNG_NotifyReseedNeeded(void)
{
    int status;
    /* On NBU side, PLATFORM_RequestRngSeed sends an RPMSG to the Host core,
     * however on the the Host side, RNG_NotifyReseedNeeded is also called from ISR
     * but PLATFORM_RequestRngSeed just returns 1 */
    status = PLATFORM_RequestRngSeed();
    assert(status >= 0);
    rng_ctx.needReseed = TRUE;

    return status;
}

bool_t RNG_IsReseedNeeded(void)
{
    return rng_ctx.needReseed;
}

int RNG_SetExternalSeed(uint8_t *external_seed)
{
    int status = gRngInternalError_d;
    FLib_MemCpy(rng_ctx.PrngSeed, external_seed, mPRNG_NoOfBytes_c);
    status = PLATFORM_SendRngSeed(external_seed, mPRNG_NoOfBytes_c);
    if (status >= 0)
    {
        rng_ctx.mPRNG_Requests = 1U;
        rng_ctx.mPrngIsSeeded  = TRUE;
        status                 = gRngSuccess_d;
    }
    else
    {
        assert(0);
    }
    return status;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#if defined(gRngSeedHwParamStorage_d) || defined(gRngSeedStorageAddr_d)
static uint32_t Rng_read_seed_from_flash(void)
{
    uint32_t seed = 0U;
#if defined(gRngSeedHwParamStorage_d)
    hardwareParameters_t *pHWParams = NULL;
    (void)NV_ReadHWParameters(&pHWParams);
    seed = pHWParams->rngSeed;
#elif defined(gRngSeedStorageAddr_d)
    seed = *((uint32_t *)gRngSeedStorageAddr_d);
#endif
    return seed;
}

static void Rng_save_seed_to_flash(uint32_t seed)
{
    NV_Init();

#if defined(gRngSeedHwParamStorage_d)
    hardwareParameters_t *pHWParams = NULL;
    (void)NV_ReadHWParameters(&pHWParams);
    pHWParams->rngSeed = seed;
    (void)NV_WriteHWParameters();

#elif defined(gRngSeedStorageAddr_d)
    (void)NV_FlashProgramUnaligned(gRngSeedStorageAddr_d, sizeof(seed), (uint8_t *)&seed);
#endif
}
#endif

#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
static bool_t mRngDisallowMcuSleep = FALSE;
/* The Allow/Disallow to sleep order is related to the use of a HW TRNG only */
#if (cPWR_UsePowerDownMode)
#define RNG_DisallowDeviceToSleep() PWR_DisallowDeviceToSleep()
#define RNG_AllowDeviceToSleep()    PWR_AllowDeviceToSleep()
#else
#define RNG_DisallowDeviceToSleep()
#define RNG_AllowDeviceToSleep()
#endif
#if (defined(RW610_SERIES) || defined(RW612_SERIES))
#define TRNG0 TRNG
#endif
static void TRNG_GoToSleep(void)
{
    if ((TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK) != 0U)
    {
        if (mRngDisallowMcuSleep)
        {
            mRngDisallowMcuSleep = FALSE;
            RNG_AllowDeviceToSleep();
        }
    }
    else
    {
        if (!mRngDisallowMcuSleep)
        {
            mRngDisallowMcuSleep = TRUE;
            RNG_DisallowDeviceToSleep();
        }
    }
}

static void TRNG_ISR(void)
{
    /* Clear Interrupt flags */
    TRNG0->INT_CTRL &= ~(TRNG_INT_CTRL_ENT_VAL_MASK | TRNG_INT_CTRL_HW_ERR_MASK | TRNG_INT_CTRL_FRQ_CT_FAIL_MASK);

    TRNG_GoToSleep();
}
#endif /* FSL_FEATURE_SOC_TRNG_COUNT  */

#if ((defined(FSL_FEATURE_SOC_RNG_COUNT)) && (FSL_FEATURE_SOC_RNG_COUNT > 0U))
#ifndef CPU_QN908X
static int RNG_Specific_Init(uint32_t *pSeed)
{
    int status = gRngInternalError_d;

    RNGA_Init(RNG);

    /* Get seed for pseudo RNG */
    if (RNGA_GetRandomData(RNG, pSeed, (int32_t)mPRNG_NoOfBytes_c) == 0)
    {
        rng_ctx.mPrngIsSeeded = TRUE;
        status                = gRngSuccess_d;
    }
    assert(gRngSuccess_d == status);

    return status;
}

static uint16_t RNG_Specific_GetRandomData(uint8_t *pOut, uint16_t outBytes)
{
    uint16_t nb_bytes = 0U;
    if (RNGA_GetRandomData(RNG, pOut, (size_t)outBytes) == 0)
    {
        nb_bytes = outBytes;
    }
    assert(nb_bytes == outBytes);

    return nb_bytes;
}
#else /* CPU_QN908X */
static int RNG_Specific_Init(uint32_t *pSeed)
{
    int status = gRngInternalError_d;

    POWER_EnableADC(true);
    RNG_Drv_Init(RNG);
    RNG_Enable(RNG, true);
    /* Get seed for pseudo RNG */
    if (RNG_GetRandomData(RNG, (uint8_t *)pSeed, (int32_t)mPRNG_NoOfBytes_c) != 0)
    {
        rng_ctx.mPrngIsSeeded = TRUE;
        status                = gRngSuccess_d;
    }
    RNG_Enable(RNG, false);
    POWER_EnableADC(false);

    return status;
}

static uint16_t RNG_Specific_GetRandomData(uint8_t *pOut, uint16_t outBytes)
{
    uint16_t nb_bytes = 0U;
    POWER_EnableADC(true);
    RNG_Enable(RNG, true);
    if (RNG_GetRandomData(RNG, (uint8_t *)pOut, (size_t)outBytes) == 0)
    {
        nb_bytes = outBytes;
    }
    RNG_Enable(RNG, false);
    POWER_EnableADC(false);
    return nb_bytes;
}

#endif
#elif ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
static int RNG_Specific_Init(uint32_t *pSeed)
{
    int           status = gRngInternalError_d;
    trng_config_t config;

    do
    {
        (void)TRNG_GetDefaultConfig(&config);

        OSA_InstallIntHandler((uint32_t)TRNG0_IRQn, TRNG_ISR);

        if (TRNG_Init(TRNG0, (const trng_config_t *)&config) != 0)
        {
            assert(0);
            RAISE_ERROR(status, gRngInternalError_d);
        }

        /* Get seed for pseudo RNG */

        if (TRNG_GetRandomData(TRNG0, pSeed, (size_t)mPRNG_NoOfBytes_c) != 0)
        {
            assert(0);
            RAISE_ERROR(status, gRngInternalError_d);
        }

#if defined(gRngSeedStorageAddr_d) || defined(gRngSeedHwParamStorage_d)
        TRNG_Deinit(TRNG0);
#else
        /* Check if the entropy generation ongoing */
        TRNG_GoToSleep();

        /* Clear Interrupt status.*/
        TRNG0->INT_CTRL &= ~(TRNG_INT_CTRL_ENT_VAL_MASK | TRNG_INT_CTRL_HW_ERR_MASK | TRNG_INT_CTRL_FRQ_CT_FAIL_MASK);

        /* Enable interrupt.*/
        NVIC_SetPriority(TRNG0_IRQn, (uint32_t)gRngIsrPrio_c >> (8U - (uint8_t)__NVIC_PRIO_BITS));
        (void)EnableIRQ(TRNG0_IRQn);
        TRNG0->INT_MASK |= TRNG_INT_MASK_ENT_VAL_MASK | TRNG_INT_MASK_HW_ERR_MASK | TRNG_INT_MASK_FRQ_CT_FAIL_MASK;
#endif
        rng_ctx.mPrngIsSeeded = TRUE;
        status                = gRngSuccess_d;

    } while (false);

    return status;
}

static uint16_t RNG_Specific_GetRandomData(uint8_t *pOut, uint16_t outBytes)
{
    uint16_t nb_bytes = 0U;
    do
    {
        if ((TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK) == 0U)
        {
            break;
        }
        if ((uint8_t)TRNG_GetRandomData(TRNG0, pOut, (size_t)outBytes) != 0U)
        {
            break;
        }
        nb_bytes = outBytes;

    } while (false);

    TRNG_GoToSleep();

    return nb_bytes;
}
#elif gRNG_UsePhyRngForInitialSeed_d
static int RNG_Specific_Init(uint32_t *pSeed)
{
    PhyGetRandomNo(pSeed);
    rng_ctx.mPrngIsSeeded = TRUE;
    return gRngSuccess_d;
}

static uint16_t RNG_Specific_GetRandomData(uint32_t *pOut, uint16_t outBytes)
{
    assert(outBytes == sizeof(uint32_t));
    PhyGetRandomNo(pOut);
    nb_bytes = sizeof(uint32_t);
    return nb_bytes;
}
#elif defined(FSL_FEATURE_SOC_SIM_COUNT) && (FSL_FEATURE_SOC_SIM_COUNT > 1)
static int RNG_Specific_Init(uint32_t *pSeed)
{
    /* Lousy RNG seed based on MCU unique Id */
    int status            = gRngSuccess_d;
    rng_ctx.mPrngIsSeeded = TRUE;
    *pSeed                = SIM->UIDL;
    *returned_bytes       = sizeof(uint32_t);

    return status;
}

static int RNG_Specific_GetRandomU32(uint32_t *pRandomNo, int16_t *returned_bytes)
{
    (void)pRandomNo;
    (void)returned_bytes;
    return gRngInternalError_d;
}

#elif defined gRngUseSecureSubSystem_d && (gRngUseSecureSubSystem_d != 0)

static uint16_t RNG_Specific_GetRandomData(uint8_t *pOut, uint16_t outBytes)
{
    uint16_t       outputBytes = 0;
    sss_sscp_rng_t rctx;
    bool           init_passed = false;
    do
    {
        RNG_MUTEX_LOCK();

        if ((outBytes == 0U) || (pOut == NULL))
        {
            break;
        }
        if (CRYPTO_InitHardware() != kStatus_Success)
        {
            break;
        }

        if (sss_sscp_rng_context_init(&g_sssSession, &rctx, SSS_HIGH_QUALITY_RNG) != kStatus_SSS_Success)
        {
            break;
        }
        init_passed = true;
        if (sss_sscp_rng_get_random(&rctx, pOut, outBytes) != kStatus_SSS_Success)
        {
            break;
        }
        outputBytes = outBytes;
    } while (false);
    if (init_passed)
    {
        if (sss_sscp_rng_free(&rctx) != kStatus_SSS_Success)
        {
            assert(0);
        }
    }
    RNG_MUTEX_UNLOCK();
    return outputBytes;
}

static int RNG_Specific_Init(uint32_t *pSeed)
{
    int status;
    do
    {
        uint16_t Obtained_size;

        Obtained_size = RNG_Specific_GetRandomData((uint8_t *)pSeed, mPRNG_NoOfBytes_c);
        if (Obtained_size != mPRNG_NoOfBytes_c)
        {
            RAISE_ERROR(status, gRngInternalError_d);
        }

        rng_ctx.mPrngIsSeeded = TRUE; /* synchronous in the case of App core using SSS */
        status                = gRngSuccess_d;

    } while (false);

    return status;
}

#else

static int RNG_Specific_Init(uint32_t *pSeed)
{
    /* no HW specific init is required */
    (void)pSeed;

    RNG_NotifyReseedNeeded();

    return gRngSuccess_d;
}

static uint16_t RNG_Specific_GetRandomData(uint8_t *pOut, uint16_t outBytes)
{
    (void)pOut;
    (void)outBytes;
    return 0U;
}
#endif

#if (defined gRngUseLehmerGen_c && (gRngUseLehmerGen_c > 0))

/*! *********************************************************************************
 *  Optimized version of modulus 2^32 -5 (0xfffffffb is prime).
 *  Method equivalent to x % 0xfffffffb, but saves more than 3000 cycles
 *
 * \param[in]   x64  64 bit input value to be divided by 2^32-5
 *
 * \return  32 bit value of modulus 0xfffffffb.
 *
 ********************************************************************************** */
static inline uint32_t MOD_fffffffb(uint64_t x64)
{
    uint32_t res;
    x64 = (x64 & ~0UL) + (5U * (uint32_t)(x64 >> 32));
    x64 += 4U;
    res = (uint32_t)x64 + (5U * (uint32_t)(x64 >> 32));
    res -= 4U;

    return res;
}

/* */
/*! *********************************************************************************
 *  RNG using a Lehmer Linear Congruential Generator.
 *
 * \param[in]   prev_state  32 bit seed or previous value to be churned.
 *
 * \return  32 bit value of modulus 0xfffffffb.
 *
 ********************************************************************************** */
static uint32_t RNG_LCG(uint32_t prev_state)
{
    uint32_t val;
#define RNG_MULTIPLICAND 279470273UL
    uint64_t prod64 = (uint64_t)prev_state * RNG_MULTIPLICAND;

    val = MOD_fffffffb(prod64);

    return val;
}
#endif

/********************************** EOF ***************************************/
