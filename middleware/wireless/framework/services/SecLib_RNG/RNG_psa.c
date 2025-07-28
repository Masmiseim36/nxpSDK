/*! *********************************************************************************
 * Copyright 2025 NXP
 *
 * \file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#include "RNG_Interface.h"
#include "fwk_platform_crypto.h"
#include "entropy_poll.h"
#include "mbedtls/entropy.h"
#include "mbedtls/psa_util.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

#define mPRNG_NoOfBits_c  (256U)
#define mPRNG_NoOfBytes_c (mPRNG_NoOfBits_c / 8U)

typedef struct rng_ctx_t
{
    bool_t mRngCtxInitialized;
} RNG_context_t;

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

static RNG_context_t rng_ctx = {
    .mRngCtxInitialized = FALSE,
};

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
    int result = gRngInternalError_d;

    (void)PLATFORM_InitCrypto();

    do
    {
        if (rng_ctx.mRngCtxInitialized == TRUE)
        {
            result = gRngSuccess_d;
            break;
        }
        /* initialize psa crypto hardware */
        psa_status_t status = psa_crypto_init();
        if (status != PSA_SUCCESS)
        {
            break;
        }
        /* set global variable mRngCtxInitialized */
        rng_ctx.mRngCtxInitialized = TRUE;

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
    /* Reinitializing the RNG module */
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
}

/*! *********************************************************************************
 * \brief  Generates a 32-bit statistically random number if the hardware is enable
 *        else a PRNG number will be generated
 *         No random number will be generated if the RNG was not initialized
 *         or an error occurs.
 *
 * \param[out]  pRandomNo  Pointer to location where the value will be stored
 *
 ********************************************************************************** */
int RNG_GetTrueRandomNumber(uint32_t *pRandomNo)
{
    int status = gRngInternalError_d;
    do
    {
        /* checks on params */
        if (pRandomNo == NULL)
        {
            status = gRngBadArguments_d;
            break;
        }

        if (rng_ctx.mRngCtxInitialized != TRUE)
        {
            status = gRngNotInitialized_d;
            break;
        }
        /* use PSA to generate best random possible:
         * generate TRNG if possible else generate PRNG */
        if (psa_generate_random((uint8_t *)pRandomNo, sizeof(uint32_t)) == PSA_SUCCESS)
        {
            status = gRngSuccess_d;
        }
    } while (0 != 0);
    return status;
}

/*! *********************************************************************************
 * \brief  Generates a bit pseudo-random number up to 256 bits. The PRNG algorithm used depend
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
 *          -3 if the PRNG was not initialized OR
 *          -2 if 0 bytes were requested OR
 *          -1 if an error occurred
 *
 ********************************************************************************** */
int RNG_GetPseudoRandomData(uint8_t *pOut, uint8_t outBytes, uint8_t *pSeed)
{
    int ret = gRngInternalError_d;
    NOT_USED(pSeed);
    do
    {
        /* checks on params */
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
        else
        {
            if (outBytes > mPRNG_NoOfBytes_c)
            {
                outBytes = mPRNG_NoOfBytes_c;
            }

            /* use PSA to generate best random possible:
             * generate TRNG if possible else generate PRNG */
            if (psa_generate_random((uint8_t *)pOut, outBytes) == PSA_SUCCESS)
            {
                ret = (int)outBytes;
            }
        }
    } while (false);
    return ret;
}

/*! *********************************************************************************
 * \brief  not supported
 *
 ********************************************************************************** */
int RNG_SetSeed(void)
{
    return 0;
}

/*! *********************************************************************************
 * \brief  not supported
 *
 ********************************************************************************** */
int RNG_NotifyReseedNeeded(void)
{
    return 0;
}

/*! *********************************************************************************
 * \brief  not supported
 *
 ********************************************************************************** */
bool_t RNG_IsReseedNeeded(void)
{
    return FALSE;
}

/*! *********************************************************************************
 * \brief  Initialize seed for the PRNG algorithm with an external seed.
 *         If this function is called again, the PRNG will be reseeded.
 *
 *  \param[in]  external_seed  Pointer to 32 byte array used to set seed.
 *
 ********************************************************************************** */
int RNG_SetExternalSeed(uint8_t *external_seed)
{
    (void)external_seed;
    return 1; /* External seeding is not supported on psa */
}

/*! *********************************************************************************
 * \brief  not supported
 *
 ********************************************************************************** */
fpRngPrng_t RNG_GetPrngFunc(void)
{
    return NULL;
}

/*! *********************************************************************************
 * \brief  not supported
 *
 ********************************************************************************** */
void *RNG_GetPrngContext(void)
{
    return NULL;
}

#ifdef MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG
/** External random generator function, implemented by the platform.
 *
 * When the compile-time option #MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG is enabled,
 * this function replaces Mbed TLS's entropy and DRBG modules for all
 * random generation triggered via PSA crypto interfaces.
 *
 * \note This random generator must deliver random numbers with cryptographic
 *       quality and high performance. It must supply unpredictable numbers
 *       with a uniform distribution. The implementation of this function
 *       is responsible for ensuring that the random generator is seeded
 *       with sufficient entropy. If you have a hardware TRNG which is slow
 *       or delivers non-uniform output, declare it as an entropy source
 *       with mbedtls_entropy_add_source() instead of enabling this option.
 *
 * \param[in,out] context       Pointer to the random generator context.
 *                              This is all-bits-zero on the first call
 *                              and preserved between successive calls.
 * \param[out] output           Output buffer. On success, this buffer
 *                              contains random data with a uniform
 *                              distribution.
 * \param output_size           The size of the \p output buffer in bytes.
 * \param[out] output_length    On success, set this value to \p output_size.
 *
 * \retval #PSA_SUCCESS
 *         Success. The output buffer contains \p output_size bytes of
 *         cryptographic-quality random data, and \c *output_length is
 *         set to \p output_size.
 * \retval #PSA_ERROR_INSUFFICIENT_ENTROPY
 *         The random generator requires extra entropy and there is no
 *         way to obtain entropy under current environment conditions.
 *         This error should not happen under normal circumstances since
 *         this function is responsible for obtaining as much entropy as
 *         it needs. However implementations of this function may return
 *         #PSA_ERROR_INSUFFICIENT_ENTROPY if there is no way to obtain
 *         entropy without blocking indefinitely.
 * \retval #PSA_ERROR_HARDWARE_FAILURE
 *         A failure of the random generator hardware that isn't covered
 *         by #PSA_ERROR_INSUFFICIENT_ENTROPY.
 */
psa_status_t mbedtls_psa_external_get_random(mbedtls_psa_external_random_context_t *context,
                                             uint8_t                               *output,
                                             size_t                                 output_size,
                                             size_t                                *output_length)
{
    size_t gen_length;
    int    status;
    do
    {
        /* call to the platform's RNG module to generate randomness*/
        status = mbedtls_hardware_poll((void *)context, output, output_size, &gen_length);
        if (status != PSA_SUCCESS)
        {
            break;
        }

        /* Check for potential wrap-around */
        if (gen_length > output_size)
        {
            output_size = 0;
        }
        else
        {
            output_size -= gen_length;
        }

        if (*output_length > SIZE_MAX - gen_length)
        {
            *output_length = SIZE_MAX;
        }
        else
        {
            *output_length += gen_length;
        }

    } while (output_size > 0U); /* while length superior to 0 */

    return status;
}
#endif

/********************************** EOF ***************************************/
