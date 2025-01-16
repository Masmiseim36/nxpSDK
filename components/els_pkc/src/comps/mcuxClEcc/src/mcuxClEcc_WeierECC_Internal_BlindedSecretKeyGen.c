/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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
 * @file  mcuxClEcc_WeierECC_Internal_BlindedSecretKeyGen.c
 * @brief WeierECC internal functions for generating random secret keys in a blinded way (as e.g. needed for ECDSA)
 */


#include <stdint.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslDataIntegrity.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClCore_Macros.h>

#include <mcuxClPkc.h>
#include <mcuxClMath.h>
#include <mcuxClSession.h>
#include <mcuxClBuffer.h>
#include <mcuxClEcc.h>
#include <mcuxClRandom.h>

#include <internal/mcuxClMath_Internal_Utils.h>
#include <internal/mcuxClPkc_Operations.h>
#include <internal/mcuxClEcc_Internal_Random.h>
#include <internal/mcuxClEcc_Weier_Internal.h>
#include <internal/mcuxClEcc_Weier_Internal_FUP.h>


#ifdef MCUXCL_FEATURE_ECC_STRENGTH_CHECK
#define MCUXCLECC_FP_WEIER_KEYGEN_SECSTRENGTH  MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_checkSecurityStrength)
#else
#define MCUXCLECC_FP_WEIER_KEYGEN_SECSTRENGTH  (0u)
#endif


/**
 * This function implements secure ECDSA private key / ephemeral key generation,
 * according to FIPS 186-4. It outputs multiplicative split d0 and d1,
 * satisfying d0*d1 mod n = d = (c mod (n-1)) + 1, in which d is the private key
 * derived from a (bitLen(n)+64)-bit true (DRBG) random number c and d0 is a 64-bit
 * random number (with bit 63 set).
 *
 * Inputs:
 *   byteLenN: byte length of n (base point order).
 *
 * Inputs in pOperands[] and PKC workarea: N/A.
 *
 * Prerequisites:
 *   ps1Len = (operandSize, operandSize);
 *   curve order n in N, NDash of n in NFULL;
 *   no on-going calculation on N;
 *   buffers S1, S2 and S3 are with doubled-size (2*operandSize).
 *
 * Result in PKC workarea:
 *   buffers S0 and S1 contain multiplicative split private key d0 and d1 (operandSize);
 *   buffer S3 contains a random value usable for blinding operations of size opLen.
 *
 * Other modifications:
 *   buffers T0, XA, YA, ZA and Z are modified (as temp, buffer size);
 *   buffers S1, S2 and S3 are modified (as temp, double buffer size);
 *   offsets pOperands[VT] is modified.
 *
 * @attention The PKC calculation might be still on-going, call #mcuxClPkc_WaitForFinish before CPU accesses to the result.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits(
    mcuxClSession_Handle_t pSession,
    uint32_t byteLenN)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits,
        MCUXCLPKC_FP_CALLED_CALC_OP1_CONST,
        MCUXCLPKC_FP_CALLED_CALC_OP2_CONST );

    // TODO CLNS-10988: Follow-up enable DI ALLOC/INIT at the public API level.
    MCUX_CSSL_DI_RECORD(sumOfRandomGenerateParams, (uint32_t)pSession);

    uint16_t *pOperands = MCUXCLPKC_GETUPTRT();
    const uint32_t mcLen_opLen = MCUXCLPKC_PS1_GETLENGTH_REG();
    const uint32_t opLen = MCUXCLPKC_PS1_UNPACK_OPLEN(mcLen_opLen);

    /* Count leading zeros in most significant word of n. */
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("MISRA Ex. 9 to Rule 11.3 - PKC word is CPU word aligned.")
    const uint32_t *ptr32N = (const uint32_t *) MCUXCLPKC_OFFSET2PTR(pOperands[ECC_N]);
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()
    MCUX_CSSL_ANALYSIS_COVERITY_ASSERT(byteLenN, MCUXCLECC_WEIERECC_BRAINPOOLP160T1_SIZE_BASEPOINTORDER, MCUXCLECC_WEIERECC_SECP521R1_SIZE_BASEPOINTORDER, MCUXCLECC_STATUS_FAULT_ATTACK)
    const uint32_t wordNumN = (byteLenN + (sizeof(uint32_t)) - 1u) / (sizeof(uint32_t));
    MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();  // avoid CPU accessing to PKC workarea when PKC is busy
    uint32_t nMSWord = ptr32N[wordNumN - 1u];
    uint32_t nMSWord_LeadZeros = mcuxClMath_CountLeadingZerosWord(nMSWord);
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("'nMSWord_LeadZeros' is less than 'wordNumN', cannot wrap")
    uint32_t bitLenN65 = (wordNumN * (sizeof(uint32_t)) * 8u) - nMSWord_LeadZeros + 65u;
    uint32_t pkcByteLenN65 = (bitLenN65 + (MCUXCLPKC_WORDSIZE * 8u) - 1u) / (MCUXCLPKC_WORDSIZE * 8u) * MCUXCLPKC_WORDSIZE;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP()

    /* Clear buffer S0. */
    MCUXCLPKC_FP_CALC_OP1_CONST(ECC_S0, 0u);

    /* Clear buffer S3, with OPLEN = pkcByteLenN65. */
//  MCUXCLPKC_WAITFORREADY();  <== not necessary when setting PS2 after submitting a PS1 computation via mcuxClPkc_Calc(...)
    MCUXCLPKC_PS2_SETLENGTH(0u, pkcByteLenN65);
    MCUXCLPKC_FP_CALC_OP2_CONST(ECC_S3, 0u);


    /**********************************************************/
    /* Step 1: generate 64-bit random d0, (MSb set);          */
    /*         compute v = ModInv(d0) = d0^(-1) mod n.        */
    /**********************************************************/

    /* Generate S0 = 64-bit random d0, with PRNG. */
    {
        uint8_t * const ptrS0 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_S0]);
        MCUXCLBUFFER_INIT(buffS0, NULL, ptrS0, 8u);
        MCUXCLPKC_WAITFORFINISH();
        MCUX_CSSL_FP_FUNCTION_CALL(ret_PRNG_GetRandom_d0, mcuxClRandom_ncGenerate(pSession, buffS0, 8u));
        if (MCUXCLRANDOM_STATUS_OK != ret_PRNG_GetRandom_d0)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, MCUXCLECC_STATUS_RNG_ERROR,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate) );
        }
    } /* buffS0 scope. */

    /* Set MSBit of d0 (to ensure d0 != 0) using the PKC.
     *
     * NOTES:
     *   - PKC PS1 can be used, because operandSize >= 64.
     *   - The LSWord of S3 has already been cleared above */
    uint32_t *ptr32S3 = MCUXCLPKC_OFFSET2PTRWORD(pOperands[ECC_S3]);
    ptr32S3[1] = 0x80000000u;
    MCUXCLPKC_FP_CALC_OP1_OR(ECC_S0, ECC_S0, ECC_S3);


    /**********************************************************/
    /* Step 2: generate (bitLenN+64)-bit DRBG random seed c.  */
    /* a. c is in a buffer of size, pkcSize(bitLenN+65).      */
    /**********************************************************/

    /* Prepare buffer S2 for key seed: Initialize it with PRNG data and clear garbage bytes */
    uint8_t *ptrS2 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_S2]);
    const uint32_t keySeedLength = (wordNumN * (sizeof(uint32_t))) + 8u;
    MCUX_CSSL_DI_RECORD(sumOfRandomGenerateParams, (uint32_t)ptrS2 + keySeedLength);
    MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();
    MCUXCLMEMORY_FP_MEMORY_CLEAR(&ptrS2[keySeedLength], pkcByteLenN65 - keySeedLength);
    {
        MCUXCLBUFFER_INIT(buffS2, NULL, ptrS2, keySeedLength);
        MCUX_CSSL_FP_FUNCTION_CALL(ret_PRNG_GetRandom_InitKeySeed, mcuxClRandom_ncGenerate(pSession, buffS2, keySeedLength));
        if (MCUXCLRANDOM_STATUS_OK != ret_PRNG_GetRandom_InitKeySeed)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, MCUXCLECC_STATUS_RNG_ERROR,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
                MCUXCLPKC_FP_CALLED_CALC_OP1_OR,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate));
        }
    } /* buffS2 scope. */

#ifdef MCUXCL_FEATURE_ECC_STRENGTH_CHECK
    /* Derive the security strength required for the RNG from bitLenN / 2 and check whether it can be provided. */
    MCUX_CSSL_FP_FUNCTION_CALL(ret_checkSecurityStrength, mcuxClRandom_checkSecurityStrength(pSession, MCUXCLCORE_MIN((byteLenN * 8u) / 2u, 256u)));
    if (MCUXCLRANDOM_STATUS_OK != ret_checkSecurityStrength)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, MCUXCLECC_STATUS_RNG_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
            MCUXCLPKC_FP_CALLED_CALC_OP1_OR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_checkSecurityStrength));
    }
#endif

    /* Generate (wordNumN * 4 + 8)-byte random c with DRBG in buffer S2. */
    uint32_t hqRngRet = MCUXCLECC_STATUS_FAULT_ATTACK;
    MCUXCLECC_FP_RANDOM_HQRNG_PKCWA(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, pSession, ptrS2, keySeedLength, hqRngRet);
    if (MCUXCLECC_STATUS_OK != hqRngRet)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, hqRngRet,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
            MCUXCLPKC_FP_CALLED_CALC_OP1_OR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
            MCUXCLECC_FP_WEIER_KEYGEN_SECSTRENGTH,
            MCUXCLECC_FP_CALLED_RANDOM_HQRNG_PKCWA);
    }


    /**********************************************************/
    /* Step 3: generate (bitLenN+64)-bit PRNG random number r;*/
    /*         compute c' = c + r.                            */
    /* a. OPLEN = pkcSize(bitLenN+65 bit).                    */
    /**********************************************************/

    /* S3 = (wordNumN * 4 + 8)-byte random r, with PRNG. */
    {
        uint8_t * const ptrS3 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_S3]);
        const uint32_t lenS3Random = (wordNumN * (sizeof(uint32_t))) + 8u;
        MCUXCLBUFFER_INIT(buffS3, NULL, ptrS3, lenS3Random);
        MCUXCLPKC_WAITFORFINISH();
        MCUX_CSSL_FP_FUNCTION_CALL(ret_PRNG_GetRandom_r, mcuxClRandom_ncGenerate(pSession, buffS3, lenS3Random));
        if (MCUXCLRANDOM_STATUS_OK != ret_PRNG_GetRandom_r)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, MCUXCLECC_STATUS_RNG_ERROR,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
                MCUXCLPKC_FP_CALLED_CALC_OP1_OR,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
                MCUXCLECC_FP_WEIER_KEYGEN_SECSTRENGTH,
                MCUXCLECC_FP_CALLED_RANDOM_HQRNG_PKCWA,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate) );
        }
    } /* buffS3 scope. */

    /* Step 3: Truncate c and r to (bitLenN + 64)-bit, with OPLEN = pkcByteLenN65.                */
    /*         S2 = c' = c + r, the result fits in (bitLenN+65) bits, with OPLEN = pkcByteLenN65. */
    /*         ZA = d0, duplicate because ModInv(d0) will destroy input d0.                       */
    /* Step 4: Z = n-1. */
    pOperands[WEIER_VT] = (uint16_t) (nMSWord_LeadZeros & 0xFFFFU);
    MCUXCLPKC_FP_CALCFUP(mcuxClEcc_FUP_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits_Steps34,
                        mcuxClEcc_FUP_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits_Steps34_LEN);

    /* T0 = v = ModInv(d0), with temp S1. */
    MCUXCLMATH_FP_MODINV(ECC_T0, WEIER_ZA, ECC_N, ECC_S1);

    /* Now, S0 = d0 (opLen), T0 = v (opLen), Z = n-1 (opLen), */
    /*      S2 = c' (pkcByteLenN65), S3 = r (pkcByteLenN65).  */


    /**********************************************************/
    /* Step 4: use ReduceModEven to compute:                  */
    /*         s = r mod (n-1);                               */
    /*         c" = c' mod (n-1) = (c+s) mod (n-1).           */
    /* a. ReduceModEven guarantees the result is < (n-1).     */
    /**********************************************************/

    MCUXCLPKC_WAITFORREADY();
    MCUXCLPKC_PS1_SETLENGTH(pkcByteLenN65, opLen);

    /* S1 = c" = ReduceModEven(c', n-1). */
    MCUXCLMATH_FP_REDUCEMODEVEN(ECC_S1, ECC_S2, WEIER_Z, WEIER_XA, ECC_S1, WEIER_YA, WEIER_ZA);
    /* S2 = s = ReduceModEven(r, n-1). */
    MCUXCLMATH_FP_REDUCEMODEVEN(ECC_S2, ECC_S3, WEIER_Z, WEIER_XA, ECC_S2, WEIER_YA, WEIER_ZA);


    /**********************************************************/
    /* Step 5: compute d' = (v*c") - (v*s) mod (v*(n-1)),     */
    /*                    = v * (c mod (n-1)).                */
    /* a. 3 plain multiplications (PM):                       */
    /*    MCLEN = OPLEN = opLen, RLEN = 2 * opLen;            */
    /* b. modular subtraction (MS): OPLEN = 2 * opLen.        */
    /**********************************************************/

    /**********************************************************/
    /* Step 6: compute d" = d' + v,                           */
    /*                    = v * ((c mod (n-1)) + 1) = v * d.  */
    /* a. plain addition (ADD): OPLEN = 2 * opLen;            */
    /* b. v shall be prepared in buffer of size, 2*opLen.     */
    /**********************************************************/

    MCUXCLPKC_WAITFORREADY();
    MCUXCLPKC_PS1_SETLENGTH_REG(mcLen_opLen);  /* MCLEN = OPLEN = opLen. */
    MCUXCLPKC_PS2_SETLENGTH(0u, opLen * 2u);

    /* Step 5: S2 = d' = (v*c") - (v*s) mod (v*(n-1)); */
    /*         S3 = v * s, as another random number.   */
    /* Step 6: S2 = d" = d' + v.                       */
    MCUXCLPKC_FP_CALCFUP(mcuxClEcc_FUP_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits_Steps56,
                        mcuxClEcc_FUP_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits_Steps56_LEN);


    /**********************************************************/
    /* Step 7: compute d1 = d" mod n < n.                     */
    /* a. length of d" is (2 * opLen).                        */
    /**********************************************************/

    MCUXCLPKC_WAITFORREADY();
    MCUXCLPKC_PS2_SETLENGTH(opLen * 2u, opLen);

    /* Step 7: S1 = d1 = (d" mod n) < n. */
    MCUXCLPKC_FP_CALCFUP(mcuxClEcc_FUP_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits_Step7,
                        mcuxClEcc_FUP_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits_Step7_LEN);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_WeierECC_BlindedSecretKeyGen_RandomWithExtraBits, MCUXCLECC_STATUS_OK,
        /* Step 1 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
        MCUXCLPKC_FP_CALLED_CALC_OP1_OR,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
        /* Step 2 */
        MCUXCLECC_FP_WEIER_KEYGEN_SECSTRENGTH,
        MCUXCLECC_FP_CALLED_RANDOM_HQRNG_PKCWA,
        /* Step 3 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ModInv),
        /* Step 4 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ReduceModEven),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ReduceModEven),
        /* Steps 5/6 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup),
        /* Steps 7 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup) );
}
