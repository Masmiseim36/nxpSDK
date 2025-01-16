/*--------------------------------------------------------------------------*/
/* Copyright 2022-2023 NXP                                                  */
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
 * @file  mcuxClOsccaPkc_SfrAccess.h
 * @brief Macros for abstracting PKC hardware SFR access
 */


#ifndef MCUXCLOSCCAPKC_SFRACCESS_H_
#define MCUXCLOSCCAPKC_SFRACCESS_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <platform_specific_headers.h>

/****                                ****/
/**** PKC Hardware Abstraction Layer ****/
/****                                ****/

/**
 * 2 different PKC hardware definition headers are supported.
 * Only one of them should be used/included.
 */


/** Special definitions of multi-bit bit field. */
#define MCUXCLOSCCAPKC_SFR_CFG_RNDDLY_NODLY   (((uint32_t)(((uint32_t)(0u)) << MCUXCLOSCCAPKC_SFR_BITPOS(CFG,RNDDLY))) & MCUXCLOSCCAPKC_SFR_BITMSK(CFG,RNDDLY))
#define MCUXCLOSCCAPKC_SFR_CTRL_REDMUL_FULLSZ (((uint32_t)(((uint32_t)(0u)) << MCUXCLOSCCAPKC_SFR_BITPOS(CFG,RNDDLY))) & MCUXCLOSCCAPKC_SFR_BITMSK(CFG,RNDDLY))


/**
 * Definitions for accessing PKC SFRs via, e.g., IP_PKC->STATUS.
 */

/** Helper macros for constructing SFR field name constants */
#define MCUXCLOSCCAPKC_PASTE(a,b)  a ## b
#define MCUXCLOSCCAPKC_CONCAT(a,b) MCUXCLOSCCAPKC_PASTE(a,b)
#define MCUXCLOSCCAPKC_SFR_FIELD(prefix,sfr,field)        MCUXCLOSCCAPKC_CONCAT(prefix, sfr ## _ ## field)

/** Helper macros to get the mask and shift values for a specific PKC SFR field */
#define MCUXCLOSCCAPKC_SFR_BITMSK(sfr, field)        MCUXCLOSCCAPKC_CONCAT(MCUXCLOSCCAPKC_SFR_FIELD(PKC_SFR_PREFIX,sfr,field), PKC_SFR_SUFFIX_MSK)
#define MCUXCLOSCCAPKC_SFR_BITPOS(sfr, field)        MCUXCLOSCCAPKC_CONCAT(MCUXCLOSCCAPKC_SFR_FIELD(PKC_SFR_PREFIX,sfr,field), PKC_SFR_SUFFIX_POS)
#define MCUXCLOSCCAPKC_SFR_BITFMT(sfr, field, val)  (MCUXCLOSCCAPKC_SFR_FIELD(PKC_SFR_PREFIX,sfr,field) (val))


/**********************************************************/
/* Helper macros for PKC SFR access                       */
/**********************************************************/

/** Read from PKC SFR */
#define MCUXCLOSCCAPKC_SFR_READ(sfr) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("PKC base and SFR addresses will always be 32-bit aligned.") \
  (PKC_SFR_BASE->PKC_SFR_NAME(sfr)) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Write to PKC SFR */
/** Type cast false to bool to avoid mirsa violation */
#define MCUXCLOSCCAPKC_SFR_WRITE(sfr, value)  \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("PKC base and SFR addresses will always be 32-bit aligned.") \
  MCUX_CSSL_ANALYSIS_START_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION() \
  do{ PKC_SFR_BASE->PKC_SFR_NAME(sfr) = (value); } while(false) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION() \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Read from PKC SFR bit field */
#define MCUXCLOSCCAPKC_SFR_BITREAD(sfr, bit) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("PKC base and SFR addresses will always be 32-bit aligned.") \
  ((PKC_SFR_BASE->PKC_SFR_NAME(sfr) & MCUXCLOSCCAPKC_SFR_BITMSK(sfr, bit)) >> MCUXCLOSCCAPKC_SFR_BITPOS(sfr, bit)) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Set bit field of PKC SFR (read-modify-write) */
#define MCUXCLOSCCAPKC_SFR_BITSET(sfr, bit) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("PKC base and SFR addresses will always be 32-bit aligned.") \
  do{ PKC_SFR_BASE->PKC_SFR_NAME(sfr) |= MCUXCLOSCCAPKC_SFR_BITMSK(sfr, bit); } while(false) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Clear bit field of PKC SFR (read-modify-write) */
#define MCUXCLOSCCAPKC_SFR_BITCLEAR(sfr, bit) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("PKC base and SFR addresses will always be 32-bit aligned.") \
  do{ PKC_SFR_BASE->PKC_SFR_NAME(sfr) &= (~ (uint32_t) MCUXCLOSCCAPKC_SFR_BITMSK(sfr, bit)); } while(false) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Set value of multi-bit field of PKC SFR (read-modify-write) */
#define MCUXCLOSCCAPKC_SFR_BITVALSET(sfr, bit, val)  \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("PKC base and SFR addresses will always be 32-bit aligned.") \
  do{ uint32_t temp = PKC_SFR_BASE->PKC_SFR_NAME(sfr) & (~ (uint32_t) MCUXCLOSCCAPKC_SFR_BITMSK(sfr, bit)); \
      PKC_SFR_BASE->PKC_SFR_NAME(sfr) = (((val) << MCUXCLOSCCAPKC_SFR_BITPOS(sfr, bit)) & MCUXCLOSCCAPKC_SFR_BITMSK(sfr, bit)) | temp; \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER() \
  } while(false)


/**** ------------------------------ ****/

#endif /* MCUXCLOSCCAPKC_SFRACCESS_H_ */
