/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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

#if (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#endif

#if FSL_FEATURE_SOC_TRNG_COUNT
    #include "fsl_trng.h"
#elif FSL_FEATURE_SOC_RNG_COUNT
    #ifdef CPU_QN908X
        #include "fsl_rng.h"
        #include "fsl_power.h"
    #else
        #include "fsl_rnga.h"
    #endif
#endif

#if defined(gRngSeedStorageAddr_d)
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

#define mPRNG_NoOfBits_c      (256)
#define mPRNG_NoOfBytes_c     (mPRNG_NoOfBits_c/8)
#define mPRNG_NoOfLongWords_c (mPRNG_NoOfBits_c/32)

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

#if FSL_FEATURE_SOC_TRNG_COUNT
uint8_t mRngDisallowMcuSleep = 0;
#endif

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
#if (FSL_FEATURE_SOC_RNG_COUNT == 0) && \
    (FSL_FEATURE_SOC_TRNG_COUNT == 0) && \
    (gRNG_UsePhyRngForInitialSeed_d)
extern void PhyGetRandomNo(uint32_t *pRandomNo);
#endif


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#if FSL_FEATURE_SOC_TRNG_COUNT
static void TRNG_ISR(void);
#endif

/* Crypto Library function prototypes. */
extern uint32_t SecLib_set_rng_seed (uint32_t seed);
extern uint32_t SecLib_get_random(void);


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
    uint32_t seed;
    uint8_t status = gRngSuccess_d;
    static uint8_t initialized = FALSE;

#if defined(gRngSeedStorageAddr_d)
    seed = *((uint32_t*)gRngSeedStorageAddr_d);

    if ( (seed !=0) && (seed != 0xFFFFFFFF) )
    {
        SecLib_set_rng_seed(seed+1);
        initialized = TRUE;
    }
#endif

    /* Check if RNG is already initialized */
    if( !initialized )
    {
#if FSL_FEATURE_SOC_RNG_COUNT
#ifndef CPU_QN908X
        RNGA_Init(RNG);
        
        /* Get seed for pseudo RNG */
        if( RNGA_GetRandomData(RNG, &seed, sizeof(uint32_t)) )
        {
            status = gRngInternalError_d;
        }
#else
        POWER_EnableADC(true);
        RNG_Drv_Init(RNG);
        RNG_Enable(RNG, true);
        /* Get seed for pseudo RNG */
        if( RNG_GetRandomData(RNG, (uint8_t*)&seed, sizeof(uint32_t)) )
        {
            status = gRngInternalError_d;
        }
        RNG_Enable(RNG, false);
        POWER_EnableADC(false);
#endif /* CPU_QN908X */
#elif FSL_FEATURE_SOC_TRNG_COUNT
        trng_config_t trngConfig;

        /* 512 samples * 3200 clocks_per_sample ~ 1600000 clocks ~ 8ms @200Mhz */
        /* uint32_t clocks_per_sample = 3200u; */ /* default clocks to generate each bit of entropy */
        uint32_t clocks_per_sample = 800u; /* scaled down to generate each bit of entropy */

        TRNG_GetDefaultConfig(&trngConfig);

        /* Set freq_min and freq_max based on clocks_per_sample */
        trngConfig.frequencyCountLimit.minimum = clocks_per_sample / 4u;
        trngConfig.frequencyCountLimit.maximum = clocks_per_sample * 16u;
        trngConfig.entropyDelay = clocks_per_sample;
        trngConfig.sampleSize = 512u;/* number of bits to generate and test */

        /* Load new statistical test values */
        /*RNG TRNG Poker Maximum Limit Register (RTPKRMAX) defines Maximum
          Limit allowable during the TRNG Statistical Check Poker Test. Note
          that this address (0xBASE_060C) is used as RTPKRMAX only if
          RTMCTL[PRGM] is 1. If RTMCTL[PRGM] is 0, this address is used as
          RTPKRSQ readback register, as described in the next
          section. a.k.a. poker sum square register*/
        trngConfig.retryCount = 1; /* if self-test fails, allow 1 retry */
        trngConfig.longRunMaxLimit = 32;
        trngConfig.pokerLimit.maximum = 1600;
        trngConfig.pokerLimit.minimum = 1030;
        trngConfig.monobitLimit.maximum = 317;
        trngConfig.monobitLimit.minimum = 195;
        trngConfig.runBit1Limit.maximum = 107;
        trngConfig.runBit1Limit.minimum = 27;
        trngConfig.runBit2Limit.maximum = 62;
        trngConfig.runBit2Limit.minimum = 7;
        trngConfig.runBit3Limit.maximum = 39;
        trngConfig.runBit3Limit.minimum = 0;
        trngConfig.runBit4Limit.maximum = 26;
        trngConfig.runBit4Limit.minimum = 0;
        trngConfig.runBit5Limit.maximum = 18;
        trngConfig.runBit5Limit.minimum = 0;
        trngConfig.runBit6PlusLimit.maximum = 17;
        trngConfig.runBit6PlusLimit.minimum = 0;

        /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.*/
        trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

        OSA_InstallIntHandler(TRNG0_IRQn, TRNG_ISR);

        if( kStatus_Success != TRNG_Init(TRNG0, &trngConfig) )
        {
            status = gRngInternalError_d;
        }
        else
        {
            /* Get seed for pseudo RNG */
            if( kStatus_Success != TRNG_GetRandomData(TRNG0, &seed, sizeof(seed)) )
            {
                status = gRngInternalError_d;
            }

#if defined(gRngSeedStorageAddr_d)
            TRNG_Deinit(TRNG0);
#else
            /* Check if the entropy generation ongoing */
            if( (!(TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK)) && (!mRngDisallowMcuSleep) )
            {
                mRngDisallowMcuSleep = 1;
                RNG_DisallowDeviceToSleep();
            }
            
            /* Clear Interrupt status.*/
            TRNG0->INT_CTRL &= ~(TRNG_INT_CTRL_ENT_VAL_MASK | 
                                 TRNG_INT_CTRL_HW_ERR_MASK  | 
                                     TRNG_INT_CTRL_FRQ_CT_FAIL_MASK);
            
            /* Enable interrupt.*/
            NVIC_SetPriority(TRNG0_IRQn, gRngIsrPrio_c >> (8 - __NVIC_PRIO_BITS));
            EnableIRQ(TRNG0_IRQn);
            TRNG0->INT_MASK |= TRNG_INT_MASK_ENT_VAL_MASK | 
                               TRNG_INT_MASK_HW_ERR_MASK  | 
                               TRNG_INT_MASK_FRQ_CT_FAIL_MASK;
#endif
        }

#else
    #if gRNG_UsePhyRngForInitialSeed_d
        /* Use 802.15.4 PHY to generate the seed for RNG */
        PhyGetRandomNo(&seed);
    #else
        /* Use MCU unique Id for RNG seed */
        seed = SIM->UIDL;
    #endif
#endif

        if( status == gRngSuccess_d )
        {
            /* Make sure that the seed is not zero */
            SecLib_set_rng_seed(seed+1);
            initialized = TRUE;
#if defined(gRngSeedStorageAddr_d)
            (void)NV_FlashProgramUnaligned(gRngSeedStorageAddr_d,
                                           sizeof(seed),
                                           (uint8_t*)&seed);
#endif
        }
    }

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

    if(!pRandomNo)
    {
        status =  gRngNullPointer_d;
    }
    else
    {
#if FSL_FEATURE_SOC_RNG_COUNT
#ifndef CPU_QN908X
        if( RNGA_GetRandomData(RNG, pRandomNo, sizeof(uint32_t)) )
        {
            status = gRngInternalError_d;
        }
#else
        POWER_EnableADC(true);
        RNG_Enable(RNG, true);
        if( RNG_GetRandomData(RNG, (uint8_t*)pRandomNo, sizeof(uint32_t)) )
        {
            status = gRngInternalError_d;
        }
        RNG_Enable(RNG, false);
        POWER_EnableADC(false);
#endif
        
#elif FSL_FEATURE_SOC_TRNG_COUNT
        if( !(TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK) )
        {
            status = gRngInternalError_d;
        }
        else if( kStatus_Success != TRNG_GetRandomData(TRNG0, pRandomNo, sizeof(uint32_t)) )
        {
            status = gRngInternalError_d;
        }
        else
        {
            if( (!(TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK)) && (!mRngDisallowMcuSleep) )
            {
                mRngDisallowMcuSleep = 1;
                RNG_DisallowDeviceToSleep();
            }
        }
        
#elif gRNG_UsePhyRngForInitialSeed_d
        PhyGetRandomNo(pRandomNo);
#else
        status = gRngInternalError_d;
#endif
    }
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
    
    mPRNG_Requests = 1;
    
    for (i = 0; i < mPRNG_NoOfBytes_c; i+=4)
    {
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[i])));
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
* \param[in]     outBytes - the number of bytes to be copyed (1-20)
* \param[in]     pXSEED - optional user SEED. Should be NULL if not used.
*
* \return  The number of bytes copied or -1 if reseed is needed
*
********************************************************************************** */
int16_t RNG_GetPseudoRandomNo(uint8_t* pOut, uint8_t outBytes, uint8_t* pXSEED)
{
    uint32_t i;
    int16_t outputBytes = outBytes;
    /* PRNG buffer used for both input and output */
    uint32_t prngBuffer[mPRNG_NoOfLongWords_c] = {0};

    if(pXSEED)
    {
        mPRNG_Requests = 1;
    }

    if (mPRNG_Requests == gRngMaxRequests_d)
    {
        outputBytes = -1;
    }
    else
    {
        mPRNG_Requests++;
        
        /* a. XSEEDj = optional user input. */
        if (pXSEED)
        {
            /* b. XVAL = (XKEY + XSEEDj) mod 2^b */
            for (i=0; i<mPRNG_NoOfBytes_c; i++)
            {
                ((uint8_t*)prngBuffer)[i] = ((uint8_t*)XKEY)[i] + pXSEED[i];
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
        XKEY[0] += 1;
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
        for (i=0; i < outputBytes; i++)
        {
            pOut[i] = ((uint8_t*)prngBuffer)[i];
        }
    }

    return outputBytes;
}


/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#if FSL_FEATURE_SOC_TRNG_COUNT
static void TRNG_ISR(void)
{
    /* Clear Interrupt flags */
    TRNG0->INT_CTRL &= ~(TRNG_INT_CTRL_ENT_VAL_MASK | 
                         TRNG_INT_CTRL_HW_ERR_MASK  | 
                         TRNG_INT_CTRL_FRQ_CT_FAIL_MASK);

    if( (TRNG0->MCTL & TRNG_MCTL_ENT_VAL_MASK) && (mRngDisallowMcuSleep) )
    {
        mRngDisallowMcuSleep = 0;
        RNG_AllowDeviceToSleep();
    }
}
#endif

/********************************** EOF ***************************************/
