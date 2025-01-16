/*--------------------------------------------------------------------------*/
/* Copyright 2022-2024 NXP                                                  */
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

#ifndef MCUXCLEXAMPLE_SESSION_HELPER_H_
#define MCUXCLEXAMPLE_SESSION_HELPER_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClCore_Platform.h>
#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <platform_specific_headers.h>

/**
 * Initialize Session via mcuxClSession_init.
 * [in]  pSession     : Pointer to the session handle.
 * [in]  cpuWaLength  : Size (in bytes) of the workarea for CPU operations. The size shall be a multiple of CPU wordsize.
 * [in]  pkcWaLength  : Size (in bytes) of the workarea for PKC operations. The size shall be a multiple of CPU wordsize.
 */

/* Macros to make sure some memory is always allocated to avoid issues */
#define MCUXCLEXAMPLE_ALLOCATE_CPUWA(cpuWaLength) \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT("Constant values are allowed as an argument to macro function") \
        ((((sizeof(uint32_t)) <= (cpuWaLength))?(cpuWaLength):sizeof(uint32_t)) / (sizeof(uint32_t))) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT()
#define MCUXCLEXAMPLE_ALLOCATE_PKCWA(pkcWaLength) \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT("Constant values are allowed as an argument to macro function") \
        ((((sizeof(uint32_t)) <= (pkcWaLength))?(pkcWaLength):sizeof(uint32_t)) / (sizeof(uint32_t))) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT()

#define MCUXCLEXAMPLE_MAX_WA(a, b) \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT("Constant values are allowed as an argument to macro function") \
        (((a) > (b)) ? (a) : (b)) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT()

#define MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(pSession, cpuWaLength, pkcWaLength)                              \
        uint32_t cpuWaBuffer[MCUXCLEXAMPLE_ALLOCATE_CPUWA(cpuWaLength)];                                               \
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(si_status, token, mcuxClSession_init(                                          \
            /* mcuxClSession_Handle_t session:      */ pSession,                                                       \
            /* uint32_t * const cpuWaBuffer:       */ cpuWaBuffer,                                                    \
            /* uint32_t cpuWaSize:                 */ cpuWaLength,                                                    \
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("The start address of the PKC RAM is 32-bit aligned, this conversion is safe.") \
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_DISCARD_CONST_QUALIFIER("Convert address to pointer")                    \
            /* uint32_t * const pkcWaBuffer:       */ (uint32_t *) PKC_RAM_ADDR,                                      \
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_DISCARD_CONST_QUALIFIER()                                                 \
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()                                             \
            /* uint32_t pkcWaSize:                 */ pkcWaLength                                                     \
            ));                                                                                                       \
        /* mcuxClSession_init is a flow-protected function: Check the protection token and the return value */         \
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != token) || (MCUXCLSESSION_STATUS_OK != si_status))        \
        {                                                                                                             \
            return false;                                                                                             \
        }                                                                                                             \
        MCUX_CSSL_FP_FUNCTION_CALL_END();

/**
 * Destroy Session and cleanup Session via mcuxClSession_cleanup and mcuxClSession_destroy
 * [in]  pSession: Pointer to the session handle.
 **/
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClExample_Session_Clean)
static inline bool mcuxClExample_Session_Clean(mcuxClSession_Handle_t pSession)
{
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(cleanup_result, cleanup_token, mcuxClSession_cleanup(pSession));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != cleanup_token || MCUXCLSESSION_STATUS_OK != cleanup_result)
    {
        return false;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(destroy_result, destroy_token, mcuxClSession_destroy(pSession));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != destroy_token || MCUXCLSESSION_STATUS_OK != destroy_result)
    {
        return false;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_END();
    return true;
}

#endif /* MCUXCLEXAMPLE_SESSION_HELPER_H_ */
