/*--------------------------------------------------------------------------*/
/* Copyright 2022, 2024 NXP                                                 */
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
 * @file  mcuxClTrng_SfrAccess.h
 * @brief Macros for abstracting TRNG hardware SFR access
 */


#ifndef MCUXCLTRNG_SFRACCESS_H_
#define MCUXCLTRNG_SFRACCESS_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <platform_specific_headers.h>

/****                                 ****/
/**** TRNG Hardware Abstraction Layer ****/
/****                                 ****/

/**
 * 2 different TRNG hardware definition headers are supported.
 * Only one of them should be used/included.
 */


/**
 * Definitions for accessing TRNG SFRs via, e.g., IP_TRNG->STATUS.
 */

/** Helper macros for constructing SFR field name constants */
#define MCUXCLTRNG_PASTE(a,b)  a ## b
#define MCUXCLTRNG_CONCAT(a,b) MCUXCLTRNG_PASTE(a,b)
#define MCUXCLTRNG_SFR_FIELD(prefix,sfr,field)  MCUXCLTRNG_CONCAT(prefix, sfr ## _ ## field)

/** Helper macros to get the mask and shift values for a specific TRNG SFR field */
#define MCUXCLTRNG_SFR_BITMSK(sfr, field)       MCUXCLTRNG_CONCAT(MCUXCLTRNG_SFR_FIELD(TRNG_SFR_PREFIX,sfr,field), TRNG_SFR_SUFFIX_MSK)
#define MCUXCLTRNG_SFR_BITPOS(sfr, field)       MCUXCLTRNG_CONCAT(MCUXCLTRNG_SFR_FIELD(TRNG_SFR_PREFIX,sfr,field), TRNG_SFR_SUFFIX_POS)


/**********************************************************/
/* Helper macros for TRNG SFR access                       */
/**********************************************************/

/** Read from TRNG SFR */
#define MCUXCLTRNG_SFR_READ(sfr) \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("TRNG base and SFR addresses will always be 32-bit aligned.") \
    (TRNG_SFR_BASE->TRNG_SFR_NAME(sfr)) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Write to TRNG SFR */
#define MCUXCLTRNG_SFR_WRITE(sfr, value)  \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("TRNG base and SFR addresses will always be 32-bit aligned.") \
    do{ TRNG_SFR_BASE->TRNG_SFR_NAME(sfr) = (value); } while(false) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Read from TRNG SFR bit field */
#define MCUXCLTRNG_SFR_BITREAD(sfr, bit)  \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("TRNG base and SFR addresses will always be 32-bit aligned.") \
    ((TRNG_SFR_BASE->TRNG_SFR_NAME(sfr) & MCUXCLTRNG_SFR_BITMSK(sfr, bit)) >> MCUXCLTRNG_SFR_BITPOS(sfr, bit)) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Set bit field of TRNG SFR (read-modify-write) */
#define MCUXCLTRNG_SFR_BITSET(sfr, bit)  \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("TRNG base and SFR addresses will always be 32-bit aligned.") \
    do{ TRNG_SFR_BASE->TRNG_SFR_NAME(sfr) |= MCUXCLTRNG_SFR_BITMSK(sfr, bit); } while(false) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Clear bit field of TRNG SFR (read-modify-write) */
#define MCUXCLTRNG_SFR_BITCLEAR(sfr, bit)  \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("TRNG base and SFR addresses will always be 32-bit aligned.") \
    do{ TRNG_SFR_BASE->TRNG_SFR_NAME(sfr) &= (~ (uint32_t) MCUXCLTRNG_SFR_BITMSK(sfr, bit)); } while(false) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Set value of multi-bit field of TRNG SFR (read-modify-write) */
#define MCUXCLTRNG_SFR_BITVALSET(sfr, bit, val)  \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("TRNG base and SFR addresses will always be 32-bit aligned.") \
    do{ uint32_t temp = TRNG_SFR_BASE->TRNG_SFR_NAME(sfr) & (~ (uint32_t) MCUXCLTRNG_SFR_BITMSK(sfr, bit)); \
        TRNG_SFR_BASE->TRNG_SFR_NAME(sfr) = ((val) << MCUXCLTRNG_SFR_BITPOS(sfr, bit)) & MCUXCLTRNG_SFR_BITMSK(sfr, bit) | temp; \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER() \
    } while(false)


/**** ------------------------------ ****/

#endif /* MCUXCLTRNG_SFRACCESS_H_ */
