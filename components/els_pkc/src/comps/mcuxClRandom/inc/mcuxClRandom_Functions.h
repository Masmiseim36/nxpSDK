/*--------------------------------------------------------------------------*/
/* Copyright 2020-2022, 2024 NXP                                            */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxClRandom_Functions.h
 * @brief Top level APIs of mcuxClRandom component
 */

#ifndef MCUXCLRANDOM_FUNCTIONS_H_
#define MCUXCLRANDOM_FUNCTIONS_H_

#include <mcuxClConfig.h> // Exported features flags header

#include <mcuxClSession.h>
#include <mcuxClRandom_Types.h>
#include <mcuxClBuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* Public APIs of mcuxClRandom                             */
/**********************************************************/
/**
 * @defgroup mcuxClRandom_Functions mcuxClRandom_Functions
 * @brief Defines all functions of @ref mcuxClRandom
 * @ingroup mcuxClRandom
 * @{
 */

/**
 * @brief Random data generator initialization function.
 *
 * This function performs the initialization of a random data generator. This
 * operation initializes the Random context referenced in the session handle.
 *
 * @param [in]     pSession   Handle for the current CL session.
 * @param [in]     pContext   Pointer to a Random data context buffer large enough
 *                            to hold the context for the selected @p mode
 * @param [in]     mode       Mode of operation for random data generator.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_init)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_init(
    mcuxClSession_Handle_t pSession,
    mcuxClRandom_Context_t pContext,
    mcuxClRandom_Mode_t    mode
); /* init */

/**
 * @brief Random data generator reseed function.
 *
 * This function performs the reseeding of a random data generator. This
 * operation fetches a fresh seed from a TRNG and updates the state in the
 * Random context referenced in the session handle.
 *
 * @param [in]     pSession    Handle for the current CL session.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_reseed)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_reseed(
    mcuxClSession_Handle_t pSession
); /* reseed */

/**
 * @brief Random data generation function.
 *
 * This function generates random data based on the information contained in
 * the Random context referenced in the session handle.
 *
 * @param [in]     pSession  Handle for the current CL session.
 * @param [out]    pOut      Buffer in which the generated random data must be
 *                           written.
 * @param [in]     outLength Number of random data bytes that must be written in the
 *                           @p pOut buffer.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_generate)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_generate(
    mcuxClSession_Handle_t pSession,
    mcuxCl_Buffer_t        pOut,
    uint32_t              outLength
); /* generate */

/**
 * @brief Random data generator uninitialization function.
 *
 * This function performs the cleanup of a random data generator. This
 * operation cleans up the Random context referenced in the session handle.
 *
 * @param [in]     pSession    Handle for the current CL session.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_uninit)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_uninit(
    mcuxClSession_Handle_t pSession
); /* uninit */

/**
 * @brief Random data generator self-test function.
 *
 * This function performs a series of selft-tests on the random data
 * generator. These tests are performed on the random data generator defined
 * by Random context referenced in the session handle.
 *
 * @param [in]     pSession    Handle for the current CL session.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_selftest)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_selftest(
    mcuxClSession_Handle_t pSession,
    mcuxClRandom_Mode_t    mode
); /* health test */

/**
 * @brief Random data generator security strength check.
 *
 * This function reports whether the the random data generator can provide the
 * requested security strength.
 *
 * @param [in]     pSession            Handle for the current CL session.
 * @param [in]     securityStrength    Requested security strength in bits.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_checkSecurityStrength)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_checkSecurityStrength(
    mcuxClSession_Handle_t pSession,
    uint32_t              securityStrength
); /* security strength check */


/**
 * @brief Function to enable the PRNG patch mode. 
 * In PRNG patch mode all calls to mcuxClRandom_ncGenerate are mapped to prngPatchFunction.
 * pCustomPrngState can be used to allow a stateful prngPatchFunction.
 * mcuxClRandom_ncInit does not need to be called before this.
 * A subsequent call to mcuxClRandom_ncInit will deactivate patch mode.
 *
 * @param [in]      pSession             Session of the PRNG to be patched.
 * @param [in]      prngPatchFunction    Function pointer of the custom PRNG patch function.
 * @param [in]      pCustomPrngState     Pointer to a custom prng state to be maintained by prngPatchFunction
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_ncPatch)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_ncPatch(
    mcuxClSession_Handle_t pSession,
    mcuxClRandom_CustomNcGenerateAlgorithm_t prngPatchFunction,
    void *pCustomPrngState
);

/**
 * @brief Non-cryptographic PRNG initialization function.
 *
 * This function performs the initialization of the non-cryptographic random number
 * generator. If PRNG patching is active, this will also deactivate PRNG patch mode and
 * reset to normal PRNG operation mode. A potential custom prng state is removed from pSession,
 * but not cleared.
 *
 * @param [in]     pSession    Handle for the current CL session.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_ncInit)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_ncInit(
    mcuxClSession_Handle_t pSession
); /* LE init */

/**
 * @brief Non-cryptographic PRNG data generation function.
 *
 * This function generates non-cryptographic random data.
 * mcuxClRandom_ncGenerate should only be used for applications with low quality requirements
 * for the random numbers, e.g. countermeasures like randomization or masking, or filling
 * buffers with random numbers. It MUST not be used for any crypto operation, e.g. key
 * generation or nonce generation. If in doubt don’t use it!
 *
 * @param [in]     pSession  Handle for the current CL session.
 * @param [out]    pOut      Buffer in which the generated random data must be
 *                           written.
 * @param [in]     outLength Number of random data bytes that must be written in the
 *                           @p pOut buffer.
 *
 * @return status
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandom_ncGenerate)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandom_ncGenerate(
    mcuxClSession_Handle_t pSession,
    mcuxCl_Buffer_t        pOut,
    uint32_t              outLength
); /* LE generate */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * @}
 */ /* mcuxClRandom_Functions */

#endif /* MCUXCLRANDOM_FUNCTIONS_H_ */
