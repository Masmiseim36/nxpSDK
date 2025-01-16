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

/** @file  mcuxClRsa_PrivateCrt.c
 *  @brief mcuxClRsa: implementation of RSA private CRT key operation
 */

#include <stdint.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClCore_Macros.h>
#include <mcuxCsslParamIntegrity.h>
#include <mcuxCsslMemory.h>
#include <mcuxClMemory.h>
#include <mcuxClPkc.h>
#include <mcuxClMath.h>
#include <mcuxClRandom.h>

#include <internal/mcuxClKey_Internal.h>

#include <mcuxClRsa.h>
#include <internal/mcuxClSession_Internal.h>
#include <internal/mcuxClPkc_Operations.h>
#include <internal/mcuxClPkc_ImportExport.h>
#include <internal/mcuxClPkc_Macros.h>
#include <internal/mcuxClRsa_Internal_PkcDefs.h>
#include <internal/mcuxClRsa_Internal_Functions.h>
#include <internal/mcuxClRsa_Internal_Types.h>
#include <internal/mcuxClRsa_Internal_Macros.h>
#include <internal/mcuxClRsa_Internal_MemoryConsumption.h>
#include <internal/mcuxClRsa_Internal_PkcTypes.h>
#include <internal/mcuxClRsa_PrivateCrt_FUP.h>

/**
 * @brief This function calculates the modulo of the CRT key
 *
 * \param[in]      pKey           Pointer to key structure of type @ref mcuxClRsa_Key
 * \param[in/out]  pKeyBitLength  Pointer to return key bit lenght
 *
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRsa_Calc_Modlen_From_CRTkey)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClRsa_Calc_Modlen_From_CRTkey(
                        const mcuxClRsa_Key * const pKey,
                        uint32_t *                 pKeyBitLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRsa_Calc_Modlen_From_CRTkey);

  ///< Note that the most significant byte of P and Q should be non-zero.
  MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(pKey->pMod1->keyEntryLength, 32U, ((MCUXCLRSA_MAX_MODLEN / 2U) + 1U), MCUXCLRSA_STATUS_INVALID_INPUT)
  MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(pKey->pMod2->keyEntryLength, 32U, ((MCUXCLRSA_MAX_MODLEN / 2U) + 1U), MCUXCLRSA_STATUS_INVALID_INPUT)

  uint32_t pBitLength = (pKey->pMod1->keyEntryLength - 1u) * 8u; /* Lengths without first byte */
  uint32_t qBitLength = (pKey->pMod2->keyEntryLength - 1u) * 8u;
  uint8_t tmpByte = (uint8_t) pKey->pMod1->pKeyEntryData[0];
  while (tmpByte != 0u) /* Iterate through first byte of p and add to bit length */
  {
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("Max 7 iterations, this cannot wrap.")
    ++pBitLength;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP()
    tmpByte = (uint8_t) (tmpByte >> 1u);
  }
  tmpByte = (uint8_t) (pKey)->pMod2->pKeyEntryData[0];
  while (tmpByte != 0u) /* Iterate through first byte of q and add to bit length */
  {
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("Max 7 iterations, this cannot wrap.")
    ++qBitLength;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP()
    tmpByte = (uint8_t) (tmpByte >> 1u);
  }

  MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("The sum of keys length cannot wrap.")
  *pKeyBitLength = pBitLength + qBitLength;
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP()

  MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClRsa_Calc_Modlen_From_CRTkey);
}

/**
 * @brief This function checks the DFA
 *
 * @param[in]   pSession
 * @param[in]   pKey
 * @param[in]   pInput
 * @param[in]   pModT2
 * @param[in]   exactModByteLength
 *
 * @return statusCode
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRsa_privateCRT_internal_DfaCheck)
static MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRsa_Status_t) mcuxClRsa_privateCRT_internal_DfaCheck(
                        mcuxClSession_Handle_t      pSession,
                        const mcuxClRsa_Key * const pKey,
                        uint8_t *                  pInput,
                        uint8_t *                  pModT2,
                        const uint32_t             exactModByteLength
                      )
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRsa_privateCRT_internal_DfaCheck);

  if(MCUXCLRSA_KEY_PRIVATECRT != pKey->keytype)
  {
    MCUX_CSSL_FP_FUNCTION_CALL(retPublicExp, mcuxClRsa_publicExp(pSession,
                                                               MCUXCLPKC_PACKARGS4(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT2,
                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_M,
                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N,
                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT1),
                                                               MCUXCLPKC_PACKARGS4(0, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT3,
                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT4,
                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND),
                                                               pKey->pExp3->keyEntryLength,
                                                               pKey->pExp3->pKeyEntryData));
    if (MCUXCLRSA_STATUS_INTERNAL_KEYOP_OK != retPublicExp)
    {
      MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT_internal_DfaCheck, MCUXCLRSA_STATUS_ERROR);
    }
    MCUX_CSSL_FP_FUNCTION_CALL(compare_result, mcuxCsslMemory_Compare(mcuxCsslParamIntegrity_Protect(3u, pInput, pModT2, exactModByteLength),
                                                                    pInput /* input C */,
                                                                    pModT2 /* calculated C' */,
                                                                    exactModByteLength));

    if (compare_result != MCUXCSSLMEMORY_STATUS_EQUAL)
    {
      MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT_internal_DfaCheck, MCUXCLRSA_STATUS_FAULT_ATTACK);
    }
  }

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT_internal_DfaCheck, MCUXCLRSA_STATUS_INTERNAL_KEYOP_OK,
          MCUX_CSSL_FP_CONDITIONAL((MCUXCLRSA_KEY_PRIVATECRT != pKey->keytype),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_publicExp),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Compare)
          ));
}

/**
 * @brief This function checks if C is less than the modulus
 *
 *
 * @param[in]   pN            Pointer to modulus
 * @param[in]   modAlignLen   Key length in bytes
 *
 * @return statusCode
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRsa_privateCRT_internal_CheckModulus)
static MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRsa_Status_t) mcuxClRsa_privateCRT_internal_CheckModulus(
                                  const uint8_t *  pN,
                                  const uint32_t   modAlignLen)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRsa_privateCRT_internal_CheckModulus);

  MCUXCLPKC_WAITFORFINISH();
  if(0U == (pN[0u] & 0x01U))
  {
      MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT_internal_CheckModulus, MCUXCLRSA_STATUS_INVALID_INPUT);
  }

  /************************************************************************************************/
  /* Check that input C < N                                                                       */
  /************************************************************************************************/

  /* Compare C and N */
  MCUXCLPKC_PS1_SETLENGTH(0u, modAlignLen);
  MCUXCLPKC_FP_CALC_OP1_CMP(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_INPUT /* C */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N /* N */);

  uint32_t carryFlag = MCUXCLPKC_WAITFORFINISH_GETCARRY();
  if(MCUXCLPKC_FLAG_CARRY != carryFlag)
  {
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT_internal_CheckModulus, MCUXCLRSA_STATUS_INVALID_INPUT,
                              MCUXCLPKC_FP_CALLED_CALC_OP1_CMP);
  }

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT_internal_CheckModulus, MCUXCLRSA_STATUS_INTERNAL_KEYOP_OK,
                            MCUXCLPKC_FP_CALLED_CALC_OP1_CMP);
}


MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRsa_privateCRT)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRsa_Status_t) mcuxClRsa_privateCRT(
  mcuxClSession_Handle_t      pSession,
  const mcuxClRsa_Key * const pKey,
  uint8_t                   *pInput,
  mcuxCl_Buffer_t             pOutput)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRsa_privateCRT);

  /************************************************************************************************/
  /* Check the key type and key data;                                                             */
  /* if they are incorrect, MCUXCLRSA_STATUS_INVALID_INPUT is returned                             */
  /************************************************************************************************/
  if((MCUXCLRSA_KEY_PRIVATECRT != pKey->keytype) && (MCUXCLRSA_KEY_PRIVATECRT_DFA != pKey->keytype))
  {
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_INVALID_INPUT);
  }

  /************************************************************************************************/
  /* Check that 64 < modulus length < 512 or 1024; otherwise return MCUXCLRSA_STATUS_INVALID_INPUT.*/
  /************************************************************************************************/
  /* Ensure the length won't overflow */
  MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(pKey->pMod1->keyEntryLength, 32U, ((MCUXCLRSA_MAX_MODLEN / 2U) + 1U), MCUXCLRSA_STATUS_INVALID_INPUT)
  MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(pKey->pMod2->keyEntryLength, 32U, ((MCUXCLRSA_MAX_MODLEN / 2U) + 1U), MCUXCLRSA_STATUS_INVALID_INPUT)

  /* Obtain modulus length by counting leading zeroes in P and Q */
  uint32_t keyBitLength = 0U;
  MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClRsa_Calc_Modlen_From_CRTkey(pKey, &keyBitLength));
  const uint32_t exactModByteLength = keyBitLength / 8u;

  MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(exactModByteLength, 64U, MCUXCLRSA_MAX_MODLEN, MCUXCLRSA_STATUS_INVALID_INPUT)
  /************************************************************************************************/
  /* Initialization - Prepare buffers in PKC workarea and clear PKC workarea                      */
  /************************************************************************************************/

  /* Size definitions */
  const uint32_t byteLenPQ = pKey->pMod1->keyEntryLength;  // P and Q have the same byte length
  const uint32_t byteLenQInv = pKey->pQInv->keyEntryLength;
  const uint32_t byteLenCeilN = 2u * byteLenPQ;  // rounded up byte length of N, necessary for calculations as N is obtained by multiplying P and Q
  const uint32_t blindLen = MCUXCLRSA_INTERNAL_MOD_BLINDING_SIZE;  // length in bytes of the random value used for blinding
  const uint32_t blindAlignLen = MCUXCLRSA_ALIGN_TO_PKC_WORDSIZE(blindLen);
  const uint32_t blindSquaredAlignLen = MCUXCLRSA_ALIGN_TO_PKC_WORDSIZE(blindLen * 2u);
  const uint32_t primeAlignLen = MCUXCLRSA_ALIGN_TO_PKC_WORDSIZE(byteLenPQ);
  const uint32_t modAlignLen = MCUXCLRSA_ALIGN_TO_PKC_WORDSIZE(byteLenCeilN);
  const uint32_t qInvAlignLen = MCUXCLRSA_ALIGN_TO_PKC_WORDSIZE(byteLenQInv);
  const uint32_t blindedPrimeAlignLen = primeAlignLen + blindAlignLen;
  const uint32_t blindedMessageAlignLen = modAlignLen + blindAlignLen;
  const uint32_t blindedModAlignLen = 2u * blindedPrimeAlignLen;

  /* PKC buffer sizes */
  const uint32_t bufferSizePrimeRand = blindAlignLen;  // size of buffer for random multiplicative blinding
  const uint32_t bufferSizePrimePQb = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of buffer for blinded P or Q, including PKW word for NDash
  const uint32_t bufferSizePrimeT0 = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer primeT0
  const uint32_t bufferSizePrimeT1 = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer primeT1
  const uint32_t bufferSizePrimeT2 = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer primeT2
  const uint32_t bufferSizePrimeT3 = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer primeT3
  const uint32_t bufferSizePrimeT4 = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer primeT4
  const uint32_t bufferSizePrimeTE = 6u*MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer primeTE
  const uint32_t bufferSizePrimeR = blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer R (result of the internal exponentiation)
  const uint32_t bufferSizePrimeT5 =  blindedPrimeAlignLen + MCUXCLRSA_PKC_WORDSIZE; // size of temporary buffer primeT5
  const uint32_t bufferSizeModM = blindedMessageAlignLen;  // size of buffer for result M
  const uint32_t bufferSizeModT1 = blindedMessageAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer modT1
  const uint32_t bufferSizeModT2 = modAlignLen + 2u * MCUXCLRSA_PKC_WORDSIZE;  // size of temporary buffer modT2
  const uint32_t bufferSizeModT3 = blindedMessageAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of blinded message modT3: blindedMessageAlignLen=modAlignLen + MCUXCLRSA_PKC_WORDSIZE
  const uint32_t bufferSizeModT4 = blindedModAlignLen;  // size of temporary buffer modT4: blindedModAlignLen = 2*blindedPrimeAlignLen = modAlignLen + 2*MCUXCLRSA_PKC_WORDSIZE
  const uint32_t bufferSizeModN = blindedModAlignLen + MCUXCLRSA_PKC_WORDSIZE;  // size of buffer for modulus N: blindedModAlignLen for the division, and one extra PKW word for NDash

  /* Setup session. */
  const uint32_t bufferSizeTotal = bufferSizePrimeRand +
          MCUXCLCORE_MAX(bufferSizePrimePQb + bufferSizePrimeT0 + bufferSizePrimeT1 + bufferSizePrimeT2 + bufferSizePrimeT3 + bufferSizePrimeT4 + bufferSizePrimeTE + bufferSizePrimeR + bufferSizePrimeT5,
                       bufferSizeModM + bufferSizeModT1 + bufferSizeModT2 + bufferSizeModT3 + bufferSizeModT4 + bufferSizeModN);
  const uint32_t pkcWaSizeWord = bufferSizeTotal / (sizeof(uint32_t));
  uint32_t *pPkcWorkarea = mcuxClSession_allocateWords_pkcWa(pSession, pkcWaSizeWord);
  if (NULL == pPkcWorkarea)
  {
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_FAULT_ATTACK);
  }

  /* PKC buffers used for operations modulo P,Q and modulo N */
  uint32_t *pBlind = pPkcWorkarea;

  /* PKC buffers for exponentiations and operations modulo primes P,Q */
  uint8_t *pPQ_b = (uint8_t *) pBlind + bufferSizePrimeRand + MCUXCLRSA_PKC_WORDSIZE;  // one extra PKC word for NDash
  uint8_t *pPrimeT0 = pPQ_b + bufferSizePrimePQb - MCUXCLRSA_PKC_WORDSIZE;  // size of NDash is included in bufferSizePrimePQb
  uint8_t *pPrimeT1 = pPrimeT0 + bufferSizePrimeT0;
  uint8_t *pPrimeT2 = pPrimeT1 + bufferSizePrimeT1;
  uint8_t *pPrimeT3 = pPrimeT2 + bufferSizePrimeT2;
  uint8_t *pPrimeT4 = pPrimeT3 + bufferSizePrimeT3;
  uint8_t *pPrimeTE = pPrimeT4 + bufferSizePrimeT4;
  uint8_t *pPrimeR = pPrimeTE + bufferSizePrimeTE;
  uint8_t *pPrimeT5 = pPrimeR + bufferSizePrimeR;

  /* PKC buffers for operations modulo modulus N */
  uint8_t *pM = pPQ_b - MCUXCLRSA_PKC_WORDSIZE;  // buffer M overwrites buffer PQb, including NDash of PQb
  uint8_t *pModT1 = pM + bufferSizeModM;
  uint8_t *pModT2 = pModT1 + bufferSizeModT1;
  uint8_t *pModT3 = pModT2 + bufferSizeModT2;
  uint8_t *pModT4 = pModT3 + bufferSizeModT3;
  uint8_t *pN = pModT4 + bufferSizeModT4 + MCUXCLRSA_PKC_WORDSIZE;  // one extra PKC word for NDash

  /* Setup UPTR table */
  const uint32_t cpuWaSizeWord = (((sizeof(uint16_t)) * MCUXCLRSA_INTERNAL_PRIVCRT_UPTRT_SIZE) + (sizeof(uint32_t)) - 1u) / (sizeof(uint32_t));
  uint32_t * pOperands32 = mcuxClSession_allocateWords_cpuWa(pSession, cpuWaSizeWord);
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_REINTERPRET_MEMORY_BETWEEN_INAPT_ESSENTIAL_TYPES("16-bit UPTRT table is assigned in CPU workarea")
  uint16_t * pOperands = (uint16_t *) pOperands32;
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_REINTERPRET_MEMORY_BETWEEN_INAPT_ESSENTIAL_TYPES()

  if (NULL == pOperands)
  {
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_FAULT_ATTACK);
  }

  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_INPUT]   = MCUXCLPKC_PTR2OFFSET(pInput);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND]    = MCUXCLPKC_PTR2OFFSET((uint8_t *) pBlind);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B]    = MCUXCLPKC_PTR2OFFSET(pPQ_b);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0] = MCUXCLPKC_PTR2OFFSET(pPrimeT0);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1] = MCUXCLPKC_PTR2OFFSET(pPrimeT1);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2] = MCUXCLPKC_PTR2OFFSET(pPrimeT2);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET3] = MCUXCLPKC_PTR2OFFSET(pPrimeT3);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4] = MCUXCLPKC_PTR2OFFSET(pPrimeT4);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_TE]      = MCUXCLPKC_PTR2OFFSET(pPrimeTE);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R]       = MCUXCLPKC_PTR2OFFSET(pPrimeR);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5] = MCUXCLPKC_PTR2OFFSET(pPrimeT5);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_M]       = MCUXCLPKC_PTR2OFFSET(pM);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT1]   = MCUXCLPKC_PTR2OFFSET(pModT1);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT2]   = MCUXCLPKC_PTR2OFFSET(pModT2);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT3]   = MCUXCLPKC_PTR2OFFSET(pModT3);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT4]   = MCUXCLPKC_PTR2OFFSET(pModT4);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N]       = MCUXCLPKC_PTR2OFFSET(pN);
  pOperands[MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_CONST0]  = 0u;


  /* Set UPTRT table */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_SETUPTRT(pOperands);

  /* Clear PKC workarea after the input */
  MCUXCLPKC_PS2_SETLENGTH(0u, bufferSizeTotal);
  MCUXCLPKC_PS1_SETLENGTH(0u, primeAlignLen);
  MCUXCLPKC_FP_CALC_OP2_CONST(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND, 0u);

  /* Prepare expTemp buffer in CPU workarea - aligned to CPU word, length=MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(byteLenExp) */
  uint32_t * pExpTemp = pOperands32 + MCUXCLCORE_NUM_OF_CPUWORDS_CEIL((MCUXCLRSA_INTERNAL_PRIVCRT_UPTRT_SIZE * sizeof(uint16_t)));

  /************************************************************************************************/
  /* Securely import and blind q                                                                  */
  /************************************************************************************************/

  /* Securely import q */
  MCUX_CSSL_FP_FUNCTION_CALL(ret_SecImport, mcuxClPkc_SecureImportBigEndianToPkc(pSession,
                                                                               MCUXCLPKC_PACKARGS2(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0,
                                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */),
                                                                               pKey->pMod2->pKeyEntryData,
                                                                               byteLenPQ));
  (void)ret_SecImport;

  /* Generate random number used for blinding and set LSB to 1, to ensure it is odd and non-null */
  MCUXCLBUFFER_INIT(pBufBlind, NULL, (uint8_t *) pBlind, blindLen);
  MCUX_CSSL_FP_FUNCTION_CALL(ret_Random_ncGenerate1, mcuxClRandom_ncGenerate(pSession, pBufBlind, blindLen));
  if (MCUXCLRANDOM_STATUS_OK != ret_Random_ncGenerate1)
  {
      MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_ERROR);
  }
  pBlind[0] |= 0x1u;

  /* Blind q to obtain q_b */
  MCUXCLPKC_FP_CALC_OP1_MUL(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND /* blind */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* q */);

  /************************************************************************************************/
  /* Prepare Montgomery parameters and convert parameters to Montgomery representation.           */
  /************************************************************************************************/

  /* Calculate Ndash of q_b */
  MCUXCLMATH_FP_NDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */);

  /* Calculate input Cq_b = C mod q_b */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(blindedPrimeAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(modAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_FP_CALC_MC2_MR(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* Cq_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_INPUT /* C */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */);

  /* Calculate QDash */
  MCUXCLMATH_FP_SHIFTMODULUS(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5 /* shifted modulus */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */);
  MCUXCLMATH_FP_QDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R /* QDash */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5 /* shifted modulus */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */, (uint16_t)(modAlignLen + blindedPrimeAlignLen));

  /* Convert input to Montgomery representation i.e. Cq_bm = Cq_b*QDash mod q_b */
  MCUXCLPKC_FP_CALC_MC1_MM(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* Cq*QDash mod q_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* Cq_b */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R /* QDash */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */);

  /************************************************************************************************/
  /* Perform secure exponentiation: Mq_bm = (Cq_bm^dq) mod q_b                                    */
  /************************************************************************************************/

  MCUX_CSSL_FP_FUNCTION_CALL(ret_SecModExpDQ,
      MCUXCLMATH_SECMODEXP(pSession,
                          pKey->pExp2->pKeyEntryData,  /* dq */
                          pExpTemp,
                          pKey->pExp2->keyEntryLength,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R,          /* Result */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0,    /* Montgomery representation of base number: Cq*QDash mod q_b */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B,       /* Modulus q_b (blinded q) */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_TE,         /* Temporary buffers */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1) );
  if (MCUXCLMATH_STATUS_OK != ret_SecModExpDQ)
  {
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_ERROR);
  }

  /************************************************************************************************/
  /* Convert result back to normal representation Mq_b                                            */
  /************************************************************************************************/

  /* Convert from Montgomery to normal representation */
  MCUXCLPKC_FP_CALC_MC1_MR(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET3 /* Mq_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R /* result of the exponentiation */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* q_b */);

  /************************************************************************************************/
  /* Securely import and blind p                                                                  */
  /************************************************************************************************/

  /* Clear PKC buffer before the import */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(0u, primeAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(0u, bufferSizePrimeT0);
  MCUXCLPKC_FP_CALC_OP2_CONST(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0, 0u);

  /* Securely import p */
  MCUX_CSSL_FP_FUNCTION_CALL(ret_SecImportP, mcuxClPkc_SecureImportBigEndianToPkc(pSession,
                                                                                MCUXCLPKC_PACKARGS2(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0,
                                                                                                  MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */),
                                                                               pKey->pMod1->pKeyEntryData,
                                                                               byteLenPQ));
  (void)ret_SecImportP;

  /* Generate random number used for blinding and set LSB to 1, to ensure it is odd and non-null */
  MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();  // avoid CPU accessing to PKC workarea when PKC is busy
  MCUX_CSSL_FP_FUNCTION_CALL(ret_Random_ncGenerate2, mcuxClRandom_ncGenerate(pSession, pBufBlind, blindLen));
  if (MCUXCLRANDOM_STATUS_OK != ret_Random_ncGenerate2)
  {
      MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_ERROR);
  }
  pBlind[0] |= 0x1u;

  /* Blind p */
  MCUXCLPKC_FP_CALC_OP1_MUL(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0);

  /************************************************************************************************/
  /* Prepare Montgomery parameters and convert parameters to Montgomery representation.           */
  /************************************************************************************************/

  /* Calculate Ndash of p */
  MCUXCLMATH_FP_NDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */);

  /* Calculate input Cp_b = C mod p_b */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(blindedPrimeAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(modAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_FP_CALC_MC2_MR(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* Cp */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_INPUT /* C */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */);

  /* Calculate QDash */
  MCUXCLMATH_FP_SHIFTMODULUS(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5 /* shifted modulus */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */);
  MCUXCLMATH_FP_QDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R /* QDash */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5 /* shifted modulus */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */, (uint16_t)(modAlignLen + blindedPrimeAlignLen));

  /* Convert input to Montgomery representation i.e. Cp_bm = Cp_b*QDash mod p_b */
  MCUXCLPKC_FP_CALC_MC1_MM(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* Cp_bm */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* Cp_b */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R /* QDash */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */);

  /************************************************************************************************/
  /* Perform secure exponentiation: Mp_bm = (Cp_bm^dq) mod p_b                                    */
  /************************************************************************************************/

  MCUX_CSSL_FP_FUNCTION_CALL(ret_SecModExpDP,
      MCUXCLMATH_SECMODEXP(pSession,
                          pKey->pExp1->pKeyEntryData,  /* dp */
                          pExpTemp,
                          pKey->pExp1->keyEntryLength,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_R,          /* Result */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0,    /* Montgomery representation of base number: Cp*QDash mod p_b */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B,       /* Modulus p_b (blinded p) */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_TE,         /* Temporary buffers */
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5,
                          MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1) );
  if (MCUXCLMATH_STATUS_OK != ret_SecModExpDP)
  {
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_ERROR);
  }

  /************************************************************************************************/
  /* Calculate QDash for p_b                                                                      */
  /************************************************************************************************/

  const uint32_t qDashAlignLen = MCUXCLCORE_MAX(blindedPrimeAlignLen, qInvAlignLen + MCUXCLRSA_PKC_WORDSIZE);
  MCUXCLMATH_FP_SHIFTMODULUS(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5 /* shifted modulus */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */);
  MCUXCLMATH_FP_QDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* QDash */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET5 /* shifted modulus */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* temp */, (uint16_t)qDashAlignLen);

  /************************************************************************************************/
  /* call the FUP to do the below steps                                                           */
  /* Calculate Mq_bm = Mq_b * QDash mod p_b                                                       */
  /* Calculate T1_mb = Mp_bm - Mq_bm mod p_b                                                      */
  /************************************************************************************************/
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(qDashAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(blindedPrimeAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_FP_CALCFUP(mcuxClRsa_PrivateCrt_T1mb_FUP,
        mcuxClRsa_PrivateCrt_T1mb_FUP_LEN);
  /************************************************************************************************/
  /* Securely import qInv and convert to Montgomery form with additive blinding                   */
  /************************************************************************************************/

  /* Clear buffers T0, T1 and T2 */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(0u, qInvAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(0u, bufferSizePrimeT0 + bufferSizePrimeT1 + bufferSizePrimeT2);
  MCUXCLPKC_FP_CALC_OP2_CONST(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0, 0u);

  /* Securely import qInv */
  MCUX_CSSL_FP_FUNCTION_CALL(ret_SecImportQInv, mcuxClPkc_SecureImportBigEndianToPkc(pSession,
                                                                                   MCUXCLPKC_PACKARGS2(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0,
                                                                                                      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* temp */),
                                                                                   pKey->pQInv->pKeyEntryData,
                                                                                   byteLenQInv));
  (void)ret_SecImportQInv;

  /* Generate random number R_qInv */
  MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();  // avoid CPU accessing to PKC workarea when PKC is busy
  MCUXCLBUFFER_INIT(pBufPrimeT1, NULL, pPrimeT1, qInvAlignLen);
  MCUX_CSSL_FP_FUNCTION_CALL(ret_Random_ncGenerate3, mcuxClRandom_ncGenerate(pSession, pBufPrimeT1, qInvAlignLen));
  if (MCUXCLRANDOM_STATUS_OK != ret_Random_ncGenerate3)
  {
      MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_ERROR);
  }

  /* Blind qInv with additive blinding: qInv_b = qInv + R_qInv */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(0u, qInvAlignLen + MCUXCLRSA_PKC_WORDSIZE /* size of output qInv_b */);
  MCUXCLPKC_FP_CALC_OP1_ADD(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET2 /* qInv_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* qInv */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1 /* R_qInv */);

  /* Calculate T2_mb = QDash*qInv_b mod p_b */
  /* Calculate T3_mb = QDash*R_qInv mod p_b */
  /* Calculate qInv_bm = T2_mb-T3_mb mod p_b */
  /* Calculate T4_mb = T1_mb*qInv_bm mod p_b */
  /* Convert back into normal representation: T4_b = T4_mb mod p_b */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(qDashAlignLen, blindedPrimeAlignLen);  /* LEN = blindedPrimeAlignLen is OK as buffer T1 has been cleared */
  MCUXCLPKC_PS2_SETLENGTH(blindedPrimeAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_FP_CALCFUP(mcuxClRsa_PrivateCrt_T2T3T4mb_FUP,
        mcuxClRsa_PrivateCrt_T2T3T4mb_FUP_LEN);

  /************************************************************************************************/
  /* Calculate Garner CRT recombination                                                           */
  /************************************************************************************************/

  /* Clear PKC buffer before the import of q */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(0u, primeAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(0u, bufferSizePrimeT0);
  MCUXCLPKC_FP_CALC_OP2_CONST(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0, 0u);

  /* Securely import q */
  MCUX_CSSL_FP_FUNCTION_CALL(ret_SecImportQ, mcuxClPkc_SecureImportBigEndianToPkc(pSession,
                                                                                MCUXCLPKC_PACKARGS2(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0,
                                                                                                      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET4 /* temp */),
                                                                                   pKey->pMod2->pKeyEntryData,
                                                                                   byteLenPQ));
  (void)ret_SecImportQ;

  /* Calculate T5_b = T4_b*q in MODT4 which has a size of (primeAlignLen + blindedPrimeAlignLen = blindedMessageAlignLen) */
  /* Calculate masked message M_b = T5_b + Mq_b */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(primeAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(0u, blindedMessageAlignLen);
  MCUXCLPKC_FP_CALCFUP(mcuxClRsa_PrivateCrt_CalcM_b_FUP,
        mcuxClRsa_PrivateCrt_CalcM_b_FUP_LEN);

  /************************************************************************************************/
  /* Calculate modulus N from P and Q                                                             */
  /************************************************************************************************/

  /* Blind q with same random as p, to obtain q_b */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(0u, primeAlignLen);
  MCUXCLPKC_FP_CALC_OP1_MUL(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1 /* q_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND /* blind */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* q */);

  /* Calculate blinded modulus: N_b = p_b*q_b */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(0u, blindAlignLen);
  MCUXCLPKC_PS2_SETLENGTH(blindedPrimeAlignLen, blindedPrimeAlignLen);
  MCUXCLPKC_FP_CALC_MC2_PM(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT4 /* N_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PQ_B /* p_b */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1 /*q_b */);

  /* Calculate square of blinding value (blind)^2 */
  MCUXCLPKC_FP_CALC_OP1_MUL(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* (blind)^2 */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND /* blind */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_RAND /* blind */);

  /* Calculate modulus N = N_b / ((blind)^2). Note that (blind)^2 is non-null and odd. */
  MCUXCLMATH_FP_EXACTDIVIDEODD(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N /* N */,
                              MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT4 /* N_b */,
                              MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET0 /* (blind)^2 */,
                              MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_PRIMET1 /* temp buffer */,
                              blindedModAlignLen, /* size of N_b */
                              blindSquaredAlignLen /* size of (blind)^2: one PKC word */);

  /************************************************************************************************/
  /* Check that modulus is odd; otherwise return MCUXCLRSA_STATUS_INVALID_INPUT.                   */
  /************************************************************************************************/

  MCUX_CSSL_FP_FUNCTION_CALL(retValCheckModulus, mcuxClRsa_privateCRT_internal_CheckModulus(pN, modAlignLen));
  if(MCUXCLRSA_STATUS_INTERNAL_KEYOP_OK != retValCheckModulus)
  {
     MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, retValCheckModulus,
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_Calc_Modlen_From_CRTkey),
          4u * MCUXCLPKC_FP_CALLED_CALC_OP1_CONST,
          4u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_SecureImportBigEndianToPkc),
          3u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
          4u * MCUXCLPKC_FP_CALLED_CALC_OP1_MUL,
          2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_NDash),
          3u * MCUXCLPKC_FP_CALLED_CALC_MC1_MR,
          3u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ShiftModulus),
          3u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_QDash),
          2u * MCUXCLPKC_FP_CALLED_CALC_MC1_MM,
          2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_SecModExp),
          3u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup),
          MCUXCLPKC_FP_CALLED_CALC_OP1_ADD,
          MCUXCLPKC_FP_CALLED_CALC_MC1_PM,
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_privateCRT_internal_CheckModulus),
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ExactDivideOdd));
  }

  /************************************************************************************************/
  /* Calculate message M from M_b                                                                 */
  /************************************************************************************************/

  /* Clear the M first to drop the garbage data following M */
  MCUXCLPKC_PS2_SETLENGTH(0u, blindedMessageAlignLen /* size of M */);
  MCUXCLPKC_FP_CALC_OP2_CONST(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_M, 0u);

  /* Calculate Ndash of N */
  MCUXCLMATH_FP_NDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N /* N */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT2 /* temp */);

  /* Calculate QDash of N */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS1_SETLENGTH(modAlignLen, modAlignLen);
  MCUXCLMATH_FP_SHIFTMODULUS(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT2 /* shifted modulus */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N /* N */);
  MCUXCLMATH_FP_QDASH(MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT1  /* QDash */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT2 /* shifted modulus */,
      MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_N /* N */, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_MODT4 /* temp */, (uint16_t)blindedMessageAlignLen /* size of M_b */);

  /* Calculate reduction M_br of M_b mod N */
  /* Calculate message M1 = M_br * QDash mod N  */
  /* Normalize result (case if M1 > N) */
  MCUXCLPKC_WAITFORREADY();
  MCUXCLPKC_PS2_SETLENGTH(blindedMessageAlignLen /* size of M_b */, modAlignLen);
  MCUXCLPKC_FP_CALCFUP(mcuxClRsa_PrivateCrt_CalcM1_FUP,
        mcuxClRsa_PrivateCrt_CalcM1_FUP_LEN);
  MCUXCLPKC_WAITFORFINISH();

  /************************************************************************************************/
  /* Protection against FA: in case of key type MCUXCLRSA_KEY_PRIVATECRT_DFA,                      */
  /* use obtained message M and public exponent to calculate C', and compare with input C         */
  /************************************************************************************************/

  MCUX_CSSL_FP_FUNCTION_CALL(retValDfaCheck, mcuxClRsa_privateCRT_internal_DfaCheck(pSession, pKey, pInput, pModT2, exactModByteLength));
  if(MCUXCLRSA_STATUS_INTERNAL_KEYOP_OK != retValDfaCheck)
  {
     MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, retValDfaCheck);
  }

  /************************************************************************************************/
  /* Export result                                                                                */
  /************************************************************************************************/

  /* Copy result to the output buffer */
  MCUXCLPKC_FP_EXPORTBIGENDIANFROMPKC_BUFFER(mcuxClRsa_privateCRT, pOutput, MCUXCLRSA_INTERNAL_UPTRTINDEX_PRIVCRT_M /* M */, exactModByteLength);

  /************************************************************************************************/
  /* Function exit                                                                                */
  /************************************************************************************************/
  mcuxClSession_freeWords_pkcWa(pSession, pkcWaSizeWord);
  mcuxClSession_freeWords_cpuWa(pSession, cpuWaSizeWord);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRsa_privateCRT, MCUXCLRSA_STATUS_INTERNAL_KEYOP_OK,
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_Calc_Modlen_From_CRTkey),
          4u * MCUXCLPKC_FP_CALLED_CALC_OP1_CONST,
          4u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_SecureImportBigEndianToPkc),
          3u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
          4u * MCUXCLPKC_FP_CALLED_CALC_OP1_MUL,
          3u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_NDash),
          3u * MCUXCLPKC_FP_CALLED_CALC_MC1_MR,
          4u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ShiftModulus),
          4u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_QDash),
          2u * MCUXCLPKC_FP_CALLED_CALC_MC1_MM,
          2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_SecModExp),
          4u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup),
          MCUXCLPKC_FP_CALLED_CALC_OP1_ADD,
          MCUXCLPKC_FP_CALLED_CALC_MC1_PM,
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMath_ExactDivideOdd),
          MCUXCLPKC_FP_CALLED_CALC_OP2_CONST,
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_privateCRT_internal_CheckModulus),
          MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_privateCRT_internal_DfaCheck),
          MCUXCLPKC_FP_CALLED_EXPORTBIGENDIANFROMPKC_BUFFER);
}