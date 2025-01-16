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

/**
 * @file  mcuxClEcc_EdDSA_GenerateSignature.c
 * @brief Implementation of the EdDSA signature generation functionality
 */


#include <mcuxClCore_Platform.h>

#include <mcuxClSession.h>
#include <mcuxClKey.h>
#include <mcuxClBuffer.h>
#include <mcuxClPkc.h>
#include <mcuxClRandom.h>
#include <mcuxClHash.h>
#include <mcuxClMemory_Clear.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClCore_Macros.h>
#include <mcuxCsslAnalysis.h>
#include <mcuxClEcc.h>

#include <internal/mcuxClPkc_Macros.h>
#include <internal/mcuxClPkc_Operations.h>
#include <internal/mcuxClPkc_ImportExport.h>
#include <internal/mcuxClPkc_Resource.h>
#include <internal/mcuxClKey_Types_Internal.h>
#include <internal/mcuxClKey_Functions_Internal.h>
#include <internal/mcuxClSession_Internal.h>
#include <internal/mcuxClHash_Internal.h>
#include <internal/mcuxClEcc_EdDSA_Internal.h>
#include <internal/mcuxClEcc_EdDSA_Internal_Hash.h>
#include <internal/mcuxClEcc_EdDSA_Internal_FUP.h>


MCUX_CSSL_ANALYSIS_START_PATTERN_DESCRIPTIVE_IDENTIFIER()
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_EdDSA_GenerateSignature_Core)
static MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_EdDSA_GenerateSignature_Core(
MCUX_CSSL_ANALYSIS_STOP_PATTERN_DESCRIPTIVE_IDENTIFIER()
    mcuxClSession_Handle_t pSession,
    mcuxClKey_Handle_t key,
    const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *mode,
    mcuxCl_InputBuffer_t pIn,
    uint32_t inSize,
    mcuxCl_Buffer_t pSignature,
    uint32_t * const pSignatureSize )
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_EdDSA_GenerateSignature_Core);

    /*
     * Step 1: Set up the environment
     */

    /* Derive the pointer to the public key handle and verify that the key handles are correctly initialized for the EdDSA use case */
    mcuxClKey_Handle_t pubKey = (mcuxClKey_Handle_t) mcuxClKey_getLinkedData(key);
    if((MCUXCLKEY_ALGO_ID_ECC_EDDSA != mcuxClKey_getAlgorithm(key))
                || (MCUXCLKEY_ALGO_ID_ECC_EDDSA != mcuxClKey_getAlgorithm(pubKey))
                || (mcuxClKey_getTypeInfo(key) != mcuxClKey_getTypeInfo(pubKey))
                || (MCUXCLKEY_ALGO_ID_PRIVATE_KEY != mcuxClKey_getKeyUsage(key))
                || (MCUXCLKEY_ALGO_ID_PUBLIC_KEY != mcuxClKey_getKeyUsage(pubKey)) )
    {
        MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_INVALID_PARAMS, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    /* mcuxClEcc_CpuWa_t will be allocated and placed in the beginning of CPU workarea free space by SetupEnvironment. */
    mcuxClEcc_CpuWa_t * const pCpuWorkarea = mcuxClEcc_castToEccCpuWorkarea(mcuxClSession_getCpuWaBuffer(pSession));

    mcuxClEcc_EdDSA_DomainParams_t * const pDomainParams = (mcuxClEcc_EdDSA_DomainParams_t *) mcuxClKey_getTypeInfo(key);

    MCUX_CSSL_FP_FUNCTION_CALL(retSetupEnvironment,
        mcuxClEcc_EdDSA_SetupEnvironment(pSession,
                                        pDomainParams,
                                        ECC_EDDSA_NO_OF_BUFFERS) );
    if (MCUXCLECC_STATUS_OK != retSetupEnvironment)
    {
        MCUXCLECC_HANDLE_HW_UNAVAILABLE(retSetupEnvironment, mcuxClEcc_EdDSA_GenerateSignature_Core);
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    /*
     * Step 2: Derive the hash prefix from the mode parameter and calculate the secret scalar
     *
     *           r = H(prefix || (h_b,...,h_{2b-1}) || m')
     *
     *         using
     *           - the hash function algoSecHash to hash the blocks containing the secret (h_b,\dots,h_{2b-1}), and
     *           - the hash function algoHash to hash the remaining part of the hash input
     *         and store the hash output in buffers ECC_S3 and ECC_T3.
     */

    /* Generate digest m' from m in case phflag is set */
    const uint8_t *pMessage = NULL;
    uint32_t messageSize = 0u;
    MCUX_CSSL_FP_FUNCTION_CALL(retPreHash, mcuxClEcc_EdDSA_PreHashMessage(pSession, pDomainParams, pCpuWorkarea, mode->phflag, pIn, inSize, &pMessage, &messageSize));
    if (MCUXCLECC_STATUS_OK != retPreHash)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    /* Initialize hash context buffer in CPU workarea (used for all secure and non-secure hash operations) */
    uint32_t hashContextSizeInWords = MCUXCLCORE_MAX(mcuxClHash_getContextWordSize(pDomainParams->algoHash), mcuxClHash_getContextWordSize(pDomainParams->algoSecHash));
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("Return pointer is 32-bit aligned and satisfies the requirement of mcuxClHash_Context_t");
    mcuxClHash_Context_t pCtx = (mcuxClHash_Context_t) mcuxClSession_allocateWords_cpuWa(pSession, hashContextSizeInWords);
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()
    if (NULL == pCtx)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    /* Calculate 2b-bit hash H(prefix || (h_b,\dots,h_{2b-1}) || m') and store it in the concatenated buffers ECC_S3 and ECC_T3. */
    uint16_t *pOperands = MCUXCLPKC_GETUPTRT();
    uint8_t *pS3 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_S3]);
    uint8_t *pPrivData = mcuxClKey_getKeyData(key);
    const uint32_t keyLength = (uint32_t) pDomainParams->b / 8u;
    const uint32_t scalarSLength = ((uint32_t) pDomainParams->t + 7u) >> 3u;

    MCUXCLBUFFER_INIT_RO(buffMessage1, NULL, pMessage, messageSize);
    mcuxCl_InputBuffer_t buffMessage = NULL;
    if (MCUXCLECC_EDDSA_PHFLAG_ONE == mode->phflag)
    {
        buffMessage = buffMessage1;
    }
    else if (MCUXCLECC_EDDSA_PHFLAG_ZERO == mode->phflag)
    {
        buffMessage = pIn;
    }
    else
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    mcuxClEcc_CommonDomainParams_t *pCommonDomainParams = (mcuxClEcc_CommonDomainParams_t *) &pDomainParams->common;
    const uint32_t byteLenP = (uint32_t) pCommonDomainParams->byteLenP;
    const uint32_t operandSize = MCUXCLPKC_ALIGN_TO_PKC_WORDSIZE(byteLenP); /* Note that n < p for EdDSA */
    const uint32_t bufferSize = operandSize + MCUXCLPKC_WORDSIZE;

    MCUXCLECC_FP_EDDSA_SIGN_CALC_SCALAR(pSession,
                                       pCtx,
                                       pDomainParams->algoHash,
                                       pDomainParams->algoSecHash,
                                       mode->pHashPrefix,
                                       mode->hashPrefixLen,
                                       &pPrivData[keyLength + scalarSLength],
                                       keyLength,
                                       buffMessage,
                                       messageSize,
                                       pS3,
                                       2u * bufferSize); /* 2*bufferSize satisfies both Ed25519 and Ed448. */

    /* Clear upper bytes of buffer of stored H(prefix || (h_b,...,h_{2b-1}) || m') which is considered of size (operandSize + bufferSize). */
    /* NOTE: This will clear potential data in memory after the hash,
     *       but for Ed448 it is needed because SHAKE256 returns a hash larger than 114 bytes. */
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("bytesToClearH is 8 for Ed25519 and 6 for Ed448.")
    const uint32_t bytesToClearH = operandSize + bufferSize - (2u * keyLength);
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
    MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();  // avoid CPU accessing to PKC workarea when PKC is busy
    MCUXCLMEMORY_FP_MEMORY_CLEAR(&pS3[2u * keyLength], bytesToClearH);


    /*
     * Step 3:  Reduce the scalar r modulo n in a blinded way and store the result in buffer ECC_S2.
     */

    /* Generate a blinding value rndR of byte length (operandSize + bufferSize - 1) in buffer ECC_T0 for an additive blinding of r.
     *
     * NOTES:
     *  - Irrespective of the PKC word size, we have byteLen(r) <= operandSize + bufferSize - 1, so one can additively
     *    blind r with rndR (both considered as operands of size (operandSize + bufferSize)) using a plain addition instead
     *    of a modular one in order to avoid a carry to the next PKC word.
     *  - This will also overwrite part of ECC_S1 which lies on top of ECC_T0. */

    uint8_t *pT0 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_T0]);
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("pT0 is PKC operand address and is aligned to 32 bit boundry")
    uint32_t *pT0MSWord =  & ((uint32_t *) pT0)[((operandSize + bufferSize) / sizeof(uint32_t)) - 1u];
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()
    *pT0MSWord = 0u; /* Clear most significant word of ECC_T0 considered as buffer of byte size (bufferSize + operandSize). */
    {
        MCUXCLBUFFER_INIT(buffT0, NULL, pT0, operandSize + bufferSize - 1u);
        MCUX_CSSL_FP_FUNCTION_CALL(ret_ncGenerate_rndR, mcuxClRandom_ncGenerate(pSession, buffT0, operandSize + bufferSize - 1u));
        if (MCUXCLRANDOM_STATUS_OK != ret_ncGenerate_rndR)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_RNG_ERROR);
        }
    }

    /* Additively blind r with rndR (considered of size (operandSize + bufferSize)) */
    MCUXCLPKC_PS2_SETLENGTH(0u, operandSize + bufferSize);
    MCUXCLPKC_FP_CALC_OP2_ADD(ECC_S3, ECC_S3, ECC_T0);       /* ECC_S3 = r + rndR */

    /* Calculate r mod n in a blinded way and store it in ECC_S2.
     *
     * NOTES:
     *  - The scalar r is of byte length b/4, i.e. we have, irrespective of the underlying PKC word size
     *     - for Ed25519: byteLen(r) = 64  = 2*operandSize < operandSize + bufferSize
     *     - for Ed448:   byteLen(r) = 114 < operandSize + bufferSize.
     *    Thus, in both cases, we can obtain r mod n by a Montgomery multiplication of r (considered as operand of size
     *    (operandSize + bufferSize)) with Q' = 2 ^ (8*(operandSize + bufferSize)) mod n. */
    const uint8_t * pT1 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_T1]);
    MCUXCLPKC_WAITFORREADY();
    pOperands[ECC_V0] = MCUXCLPKC_PTR2OFFSET(&pT1[MCUXCLPKC_WORDSIZE]);
    MCUXCLPKC_PS2_SETLENGTH(operandSize + bufferSize, operandSize);
    MCUXCLPKC_FP_CALCFUP(mcuxClEcc_FUP_EdDSA_GenerateSignature_ReduceScalarModN,
                        mcuxClEcc_FUP_EdDSA_GenerateSignature_ReduceScalarModN_LEN);


    /*
     * Step 4: Perform a blinded scalar multiplication R = (r mod n)*G and store the resulting point in encoded form R^enc in buffer ECC_COORD02.
     */

    /* Call the BlindedScalarMult function.
     * If the function returns OK, ECC_COORD00 and ECC_COORD01 contain the affine x- and y-coordinates of R.
     * If the function returns NEUTRAL_POINT, ECC_COORD00 and ECC_COORD01 are set to the coordinates of the neutral point (0,1). */
    MCUX_CSSL_FP_FUNCTION_CALL(ret_BlindedScalarMult, mcuxClEcc_BlindedScalarMult(pSession, pCommonDomainParams) );
    if (MCUXCLECC_STATUS_RNG_ERROR == ret_BlindedScalarMult)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_RNG_ERROR);
    }
    else if (MCUXCLECC_STATUS_NEUTRAL_POINT == ret_BlindedScalarMult)
    {
        MCUXCLPKC_FP_CALC_OP1_CONST(ECC_COORD00, 0u);
        MCUXCLPKC_FP_CALC_OP1_CONST(ECC_COORD01, 0u);
        MCUXCLPKC_FP_CALC_OP1_ADD_CONST(ECC_COORD01, ECC_COORD01, 1u);
    }
    else if (MCUXCLECC_STATUS_OK != ret_BlindedScalarMult)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Intentionally left empty */
    }

    /* Derive the encoding R_enc of R and store it in buffer ECC_COORD02.
     *
     * NOTE: PS2 lengths are still set to (0u, keyLengthPkc) */
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClEcc_EdDSA_EncodePoint(keyLength));


    /*
     * Step 5: Calculate H(prefix || R^{enc} || Q^{enc} || m') mod n using the hash function algoHash specified in the EdDSA domain parameters
     *         and store it in buffer ECC_S0.
     */

    /* Derive the pointer to the public key data */
    const uint8_t *pPubData = mcuxClKey_getKeyData(pubKey);

    /* Calculate H(prefix || R^{enc} || Q^{enc} || m') mod n */
    const uint8_t *pSignatureR = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_COORD02]);
    MCUXCLBUFFER_INIT_RO(buffSignatureR, NULL, pSignatureR, keyLength);
    MCUX_CSSL_FP_FUNCTION_CALL(ret_CalcHashModN, mcuxClEcc_EdDSA_CalcHashModN(pSession,
                                                                            pCtx,
                                                                            pDomainParams,
                                                                            mode->pHashPrefix,
                                                                            mode->hashPrefixLen,
                                                                            buffSignatureR,
                                                                            pPubData,
                                                                            buffMessage,
                                                                            messageSize));
    if (MCUXCLECC_STATUS_OK != ret_CalcHashModN)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    /* Free the hash context (it's not needed anymore) */
    mcuxClSession_freeWords_cpuWa(pSession, hashContextSizeInWords);

    /*
     * Step 6: Securely import the secret scalar s and securely calculate signature component
     *
     *           S = r + H(prefix || R^{enc} || Q^{enc} || m') * s mod n
     *
     *         and store it in buffer ECC_T0.
     */

    /* Clear buffers ECC_S1 and ECC_S3 considered of size 2*operandSize to prepare them for the blinded calculations below.
     *
     * NOTE: The clearing ECC_S1 and ECC_S3 will also clear parts of ECC_T1 and ECC_T3. */
    MCUXCLPKC_WAITFORREADY();
    MCUXCLPKC_PS2_SETLENGTH(0u, 2u * operandSize);
    MCUXCLPKC_FP_CALC_OP2_CONST(ECC_S1, 0u);
    MCUXCLPKC_FP_CALC_OP2_CONST(ECC_S3, 0u);

    /* Generate an additive blinding rndS in ECC_S1 for blinding the secret scalar s (and r implicitly) of byte size 2*operandSize - 1.
     *
     * NOTE: In the following, we will consider s and rndS as values of byte length 2*operandSize as this allows to
     *       use a plain addition for the additive blinding and makes it easier to compensate for Montgomery factors added by Montgomery multiplication. */
    uint8_t *pS1 = MCUXCLPKC_OFFSET2PTR(pOperands[ECC_S1]);
    MCUXCLPKC_WAITFORFINISH();
    {
        MCUXCLBUFFER_INIT(buffS1, NULL, pS1, (2u * operandSize) - 1u);
        MCUX_CSSL_FP_FUNCTION_CALL(ret_ncGenerate_rndS, mcuxClRandom_ncGenerate(pSession, buffS1, (2u * operandSize) - 1u));
        if (MCUXCLRANDOM_STATUS_OK != ret_ncGenerate_rndS)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_RNG_ERROR);
        }
    }

    /* Securely import the secret scalar s to ECC_S3. */
    MCUX_CSSL_FP_FUNCTION_CALL(ret_SecImportScalarS, mcuxClPkc_SecureImportLittleEndianToPkc(ECC_S3, &pPrivData[keyLength], scalarSLength));
    if (MCUXCLPKC_STATUS_OK != ret_SecImportScalarS)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    /* Calculate S = r + H(prefix || R^{enc} || Q^{enc} || m') * s mod n in a blinded way and store the result in ECC_T0. */
    MCUXCLPKC_FP_CALC_OP2_ADD(ECC_S3, ECC_S1, ECC_S3);           /* ECC_S3 = s + rndS */
    MCUXCLPKC_WAITFORREADY();
    MCUXCLPKC_PS2_SETLENGTH(2u * operandSize, operandSize);
    MCUXCLPKC_FP_CALCFUP(mcuxClEcc_FUP_EdDSA_GenerateSignature_Compute_S,
                        mcuxClEcc_FUP_EdDSA_GenerateSignature_Compute_S_LEN);

    /* Clear the result S after operandSize with 0's, so ECC_T0 will only contain S and afterwards halfSignatureSize bytes
     * can be copied.
     *
     * NOTE: This will clear potential data in ECC_T0 after S, but for Ed448 it is needed because
     *       mcuxClEcc_FUP_EdDSA_GenerateSignature_Compute_S returns S (of operandSize) 1 byte smaller than keyLength and
     *       S will be copied in Step 7 with keyLength. */
    MCUXCLPKC_WAITFORFINISH();
    MCUXCLMEMORY_FP_MEMORY_CLEAR(&pT0[operandSize], 1u);

    /*
     * Step 7: Copy
     *           - the signature component R^{enc} to the beginning of buffer pSignature
     *           - the signature component S on behind R^{enc} in the pSignature buffer
     *
     * NOTE: No need to wait for the PKC here, as this will be done in the export functions. */
    uint32_t halfSignatureSize = keyLength;
    MCUXCLPKC_FP_EXPORTLITTLEENDIANFROMPKC_BUFFER(mcuxClEcc_EdDSA_GenerateSignature_Core, pSignature, ECC_COORD02, halfSignatureSize);
    MCUXCLPKC_FP_EXPORTLITTLEENDIANFROMPKC_BUFFEROFFSET(mcuxClEcc_EdDSA_GenerateSignature_Core, pSignature, ECC_T0, halfSignatureSize, halfSignatureSize);


    /*
     * Step 8: Set the size *pSignatureSize to the size of the generated signature.
     */
    *pSignatureSize = 2u * halfSignatureSize;


    /* Clean up and exit */
    mcuxClSession_freeWords_pkcWa(pSession, pCpuWorkarea->wordNumPkcWa);
    MCUXCLPKC_FP_DEINITIALIZE_RELEASE(pSession, &pCpuWorkarea->pkcStateBackup,
        mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_FAULT_ATTACK);

    mcuxClSession_freeWords_cpuWa(pSession, pCpuWorkarea->wordNumCpuWa);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateSignature_Core, MCUXCLECC_STATUS_OK,
        /* Step 1 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_SetupEnvironment),
        /* Step 2 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_PreHashMessage),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
        MCUXCLECC_FP_CALLED_EDDSA_SIGN_CALC_SCALAR,
        /* Step 3 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
        MCUXCLPKC_FP_CALLED_CALC_OP2_ADD,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup),
        /* Step 4 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_BlindedScalarMult),
        MCUX_CSSL_FP_CONDITIONAL((MCUXCLECC_STATUS_NEUTRAL_POINT == ret_BlindedScalarMult),
            MCUXCLPKC_FP_CALLED_CALC_OP1_CONST * 2u,
            MCUXCLPKC_FP_CALLED_CALC_OP1_ADD_CONST),
        MCUXCLPKC_FP_CALLED_CALC_OP2_CONST,
        MCUXCLPKC_FP_CALLED_CALC_OP1_OR_CONST,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_EncodePoint),
        /* Step 5 */
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_CalcHashModN),
        /* Step 6 */
        MCUXCLPKC_FP_CALLED_CALC_OP2_CONST * 2u,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncGenerate),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_SecureImportLittleEndianToPkc),
        MCUXCLPKC_FP_CALLED_CALC_OP2_ADD,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_clear),
        /* Step 7 */
        MCUXCLPKC_FP_CALLED_EXPORTLITTLEENDIANFROMPKC_BUFFER,
        MCUXCLPKC_FP_CALLED_EXPORTLITTLEENDIANFROMPKC_BUFFEROFFSET,
        /* Step 8 */
        MCUXCLPKC_FP_CALLED_DEINITIALIZE_RELEASE);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_EdDSA_GenerateSignature)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_EdDSA_GenerateSignature(
    mcuxClSession_Handle_t pSession,
    mcuxClKey_Handle_t key,
    const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *mode,
    mcuxCl_InputBuffer_t pIn,
    uint32_t inSize,
    mcuxCl_Buffer_t pSignature,
    uint32_t * const pSignatureSize )
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_EdDSA_GenerateSignature);

    /* Call core function to calculate EdDSA signature */
    MCUX_CSSL_FP_FUNCTION_CALL(sign_result, mcuxClEcc_EdDSA_GenerateSignature_Core(
    /* mcuxClSession_Handle_t pSession:                          */ pSession,
    /* mcuxClKey_Handle_t key                                    */ key,
    /* const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t *mode */ mode,
    /* mcuxCl_InputBuffer_t pIn                                  */ pIn,
    /* uint32_t inSize                                          */ inSize,
    /* mcuxCl_Buffer_t pSignature                                */ pSignature,
    /* uint32_t * const pSignatureSize                          */ pSignatureSize));

    MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK(mcuxClEcc_EdDSA_GenerateSignature, sign_result, MCUXCLECC_STATUS_FAULT_ATTACK,
                                         MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_EdDSA_GenerateSignature_Core));
}


