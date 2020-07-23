/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


#include "RNG_Interface.h"
#include "FunctionLib.h"
#include "SecLib.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include "CryptoLibSW.h"

#if (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#endif

#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
    #include "fsl_trng.h"
#elif ((defined(FSL_FEATURE_SOC_RNG_COUNT)) && (FSL_FEATURE_SOC_RNG_COUNT > 0U))
    #ifdef CPU_QN908X
        #include "fsl_rng.h"
        #include "fsl_power.h"
    #else
    #include "fsl_rnga.h"
    #endif
#endif

#if defined(gRngSeedStorageAddr_d) || defined(gRngSeedHwParamStorage_d)
#include "Flash_Adapter.h"
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
#define mPRNG_NoOfBytes_c     (mPRNG_NoOfBits_c/8U)
#define mPRNG_NoOfLongWords_c (mPRNG_NoOfBits_c/32U)

#if (cPWR_UsePowerDownMode)
#define RNG_DisallowDeviceToSleep() PWR_DisallowDeviceToSleep()
#define RNG_AllowDeviceToSleep()    PWR_AllowDeviceToSleep()
#else
#define RNG_DisallowDeviceToSleep()
#define RNG_AllowDeviceToSleep()
#endif

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
static uint32_t XKEY[mPRNG_NoOfLongWords_c];
static uint32_t mPRNG_Requests = gRngMaxRequests_d;

#ifndef FPGA_KW38
#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
static bool_t mRngDisallowMcuSleep = FALSE;
#endif
#endif
/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
#if ((!defined(FSL_FEATURE_SOC_TRNG_COUNT)) || (FSL_FEATURE_SOC_TRNG_COUNT == 0U)) && \
    ((!defined(FSL_FEATURE_SOC_RNG_COUNT)) || (FSL_FEATURE_SOC_RNG_COUNT == 0U)) && \
    (gRNG_UsePhyRngForInitialSeed_d)
extern void PhyGetRandomNo(uint32_t *pRandomNo);
#endif
/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#ifndef FPGA_KW38
#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
static void TRNG_ISR(void);
static void TRNG_GoToSleep(void);
#endif
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Initialize the RNG HW module
*
* \return  Status of the RNG initialization sequence
*
********************************************************************************** */

uint8_t RNG_Init(void)
{
    uint8_t status = gRngSuccess_d;
#ifndef FPGA_KW38
    uint32_t seed = 0;

    static bool_t initialized = FALSE;

#if defined(gRngSeedHwParamStorage_d)
    (void)NV_ReadHWParameters(&gHardwareParameters);
    seed = gHardwareParameters.rngSeed;

    if ( (seed !=0U) && (seed != 0xFFFFFFFFU) )
    {
        (void)SecLib_set_rng_seed(seed+1U);
        initialized = TRUE;
    }
#elif defined(gRngSeedStorageAddr_d)
    seed = *((uint32_t*)gRngSeedStorageAddr_d);

    if ( (seed !=0) && (seed != 0xFFFFFFFFU) )
    {
        (void)SecLib_set_rng_seed(seed+1U);
        initialized = TRUE;
    }
#endif

    /* Check if RNG is already initialized */
    if( !initialized )
    {
#if ((defined(FSL_FEATURE_SOC_RNG_COUNT)) && (FSL_FEATURE_SOC_RNG_COUNT > 0U))
#ifndef CPU_QN908X
    RNGA_Init(RNG);

    /* Get seed for pseudo RNG */
    status = RNGA_GetRandomData(RNG, &seed, (int32_t)sizeof(uint32_t));
    assert(gRngSuccess_d == status);
#else
    POWER_EnableADC(true);
    RNG_Drv_Init(RNG);
    RNG_Enable(RNG, true);
    /* Get seed for pseudo RNG */
    if( RNG_GetRandomData(RNG, (uint8_t*)&seed, (int32_t)sizeof(uint32_t)) )
    {
        status = gRngInternalError_d;
    }
    RNG_Enable(RNG, false);
    POWER_EnableADC(false);
#endif /* CPU_QN908X */
#elif ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
    trng_config_t config;

    (void)TRNG_GetDefaultConfig(&config);
    config.frequencyCountLimit.minimum = 0x00000100U;
    config.frequencyCountLimit.maximum = 0x000F0000U;
    config.ringOscDiv = kTRNG_RingOscDiv0;
    config.entropyDelay = 1200U;

    OSA_InstallIntHandler((uint32_t)TRNG0_IRQn, TRNG_ISR);

    status = (uint8_t)TRNG_Init(TRNG0, (const trng_config_t *)&config);
    assert(0U == status);

    /* Get seed for pseudo RNG */

    status = (uint8_t)TRNG_GetRandomData(TRNG0, &seed, (size_t)sizeof(seed));
    assert(0U == status);

#if defined(gRngSeedStorageAddr_d) || defined(gRngSeedHwParamStorage_d)
    TRNG_Deinit(TRNG0);
#else
    /* Check if the entropy generation ongoing */
    TRNG_GoToSleep();

    /* Clear Interrupt status.*/
    TRNG0->INT_CTRL &= ~(TRNG_INT_CTRL_ENT_VAL_MASK |
                         TRNG_INT_CTRL_HW_ERR_MASK  |
                             TRNG_INT_CTRL_FRQ_CT_FAIL_MASK);

    /* Enable interrupt.*/
    NVIC_SetPriority(TRNG0_IRQn, (uint32_t)gRngIsrPrio_c >> (8U - (uint8_t)__NVIC_PRIO_BITS));
    (void)EnableIRQ(TRNG0_IRQn);
    TRNG0->INT_MASK |= TRNG_INT_MASK_ENT_VAL_MASK |
                       TRNG_INT_MASK_HW_ERR_MASK  |
                       TRNG_INT_MASK_FRQ_CT_FAIL_MASK;
#endif

#else

    #if gRNG_UsePhyRngForInitialSeed_d
    /* Use 802.15.4 PHY to generate the seed for RNG */
    PhyGetRandomNo(&seed);
    #else
    /* Use MCU unique Id for RNG seed */
    seed = SIM->UIDL;
    #endif

#endif

    /* Make sure that the seed is not zero */
    (void)SecLib_set_rng_seed(seed+1U);

    initialized = TRUE;

#if defined(gRngSeedHwParamStorage_d)
    NV_Init();
    gHardwareParameters.rngSeed = seed ;
    (void)NV_WriteHWParameters(&gHardwareParameters);

#elif defined(gRngSeedStorageAddr_d)
    NV_Init();
    (void)NV_FlashProgramUnaligned(gRngSeedStorageAddr_d,
                                   sizeof(seed),
                                   (uint8_t*)&seed);
#endif


    }
#endif /* FPGA_KW38 */
    /* Init Successfull */
    return status;
}


/*! *********************************************************************************
* \brief  Read a random number from the HW RNG module.
*         If the HW fails, the SW PRNG is used as backup.
*
* \param[out] pRandomNo - pointer to location where the value will be stored
*
* \return  status of the RNG module
*
********************************************************************************** */
uint8_t RNG_HwGetRandomNo(uint32_t* pRandomNo)
{
    uint8_t status = gRngSuccess_d;
#ifndef FPGA_KW38
    if(pRandomNo == NULL)
    {
        status =  gRngNullPointer_d;
    }
    else
    {
#if ((defined(FSL_FEATURE_SOC_RNG_COUNT)) && (FSL_FEATURE_SOC_RNG_COUNT > 0U))
#ifndef CPU_QN908X
        status = RNGA_GetRandomData(RNG, pRandomNo, (size_t)sizeof(uint32_t));
        assert(gRngSuccess_d == status);
#else
        POWER_EnableADC(true);
        RNG_Enable(RNG, true);
        if( RNG_GetRandomData(RNG, (uint8_t*)pRandomNo, (size_t)sizeof(uint32_t)) )
        {
            status = gRngInternalError_d;
        }
        RNG_Enable(RNG, false);
        POWER_EnableADC(false);
#endif

#elif ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
        if( (TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK) == 0U )
        {
            status = gRngInternalError_d;
        }
        else
        {
            status = (uint8_t)TRNG_GetRandomData(TRNG0, pRandomNo, (size_t)sizeof(uint32_t));
            assert(0U == status);
        }

        TRNG_GoToSleep();

#elif gRNG_UsePhyRngForInitialSeed_d
        PhyGetRandomNo(pRandomNo);
#else
        status = gRngInternalError_d;
#endif
    }
#endif /* FPGA_KW38 */
    return status;
}


/*! *********************************************************************************
* \brief  Generates a 32-bit statistically random number
*         No random number will be generated if the RNG was not initialized
*         or an error occurs.
*
* \param[out]  pRandomNo  Pointer to location where the value will be stored
*
********************************************************************************** */
void RNG_GetRandomNo(uint32_t* pRandomNo)
{
    /* Check for NULL pointers */
    if (NULL != pRandomNo)
    {
        uint32_t n = SecLib_get_random();
        FLib_MemCpy(pRandomNo, &n, sizeof(uint32_t));
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
void RNG_SetPseudoRandomNoSeed(uint8_t* pSeed)
{
    uint8_t pseudoRNGSeed[mPRNG_NoOfBytes_c] = {0};
    uint32_t i;

    mPRNG_Requests = 1U;

    for (i = 0; i < mPRNG_NoOfBytes_c; i+=4U)
    {
        RNG_GetRandomNo((uint32_t*)(void *)(&(pseudoRNGSeed[i])));
    }
    FLib_MemCpy( XKEY, pseudoRNGSeed, mPRNG_NoOfBytes_c );
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
int16_t RNG_GetPseudoRandomNo(uint8_t* pOut, uint8_t outBytes, uint8_t* pSeed)
{
    uint32_t i;
    uint16_t outputBytes = outBytes;
    /* PRNG buffer used for both input and output */
    uint32_t prngBuffer[mPRNG_NoOfLongWords_c] = {0};

    if(pSeed != NULL)
    {
        mPRNG_Requests = 1U;
    }

    if (mPRNG_Requests == gRngMaxRequests_d)
    {
        outputBytes = 0xFFFF;
    }
    else
    {
        mPRNG_Requests++;

        /* a. XSEEDj = optional user input. */
        if(pSeed != NULL)
        {
            /* b. XVAL = (XKEY + XSEEDj) mod 2^b */
            for (i=0; i<mPRNG_NoOfBytes_c; i++)
            {
                ((uint8_t*)prngBuffer)[i] = ((uint8_t*)XKEY)[i] + pSeed[i];
            }
        }
        else
        {
            for (i=0; i<mPRNG_NoOfBytes_c; i++)
            {
                ((uint8_t*)prngBuffer)[i] = ((uint8_t*)XKEY)[i];
            }
        }

        /* c. xj = G(t,XVAL) mod q
        ***************************/
        SHA256_Hash((uint8_t*)prngBuffer, mPRNG_NoOfBytes_c, (uint8_t*)prngBuffer);

        /* d. XKEY = (1 + XKEY + xj) mod 2^b */
        XKEY[0] += 1U;
        for (i=0; i<mPRNG_NoOfLongWords_c; i++)
        {
            XKEY[i] += prngBuffer[i];
        }

        /* Check if the length provided exceeds the output data size */
        if (outputBytes > mPRNG_NoOfBytes_c)
        {
            outputBytes = mPRNG_NoOfBytes_c;
        }

        /* Copy the generated number */
        for (i=0; i < (uint32_t)outputBytes; i++)
        {
            pOut[i] = ((uint8_t*)prngBuffer)[i];
        }
    }

    return (int16_t)outputBytes;
}


/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#ifndef FPGA_KW38
#if ((defined(FSL_FEATURE_SOC_TRNG_COUNT)) && (FSL_FEATURE_SOC_TRNG_COUNT > 0U))
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
    TRNG0->INT_CTRL &= ~(TRNG_INT_CTRL_ENT_VAL_MASK |
                         TRNG_INT_CTRL_HW_ERR_MASK  |
                         TRNG_INT_CTRL_FRQ_CT_FAIL_MASK);

    TRNG_GoToSleep();
}
#endif
#endif

/********************************** EOF ***************************************/
