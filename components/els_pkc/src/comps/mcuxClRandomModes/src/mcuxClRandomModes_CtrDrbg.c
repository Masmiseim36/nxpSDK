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

#include <mcuxClRandom.h>
#include <mcuxClRandomModes.h>
#include <mcuxClSession.h>
#include <mcuxClMemory.h>
#include <mcuxClAes.h>
#include <mcuxCsslAnalysis.h>
#include <mcuxClCore_Macros.h>
#include <internal/mcuxClSession_Internal.h>
#include <internal/mcuxClRandom_Internal_Memory.h>
#include <internal/mcuxClRandomModes_Private_CtrDrbg.h>
#include <internal/mcuxClRandomModes_Private_NormalMode.h>
#include <internal/mcuxClRandomModes_Private_Drbg.h>
#include <internal/mcuxClRandomModes_Private_CtrDrbg_BlockCipher.h>
#include <internal/mcuxClRandomModes_Internal_SizeDefinitions.h>
#include <internal/mcuxClBuffer_Internal.h>


/**
 * \brief This function increments the counter V of the CTR_DRBG.
 *
 * This function increments the counter V considered as a big endian integer.
 *
 * \param  pV       Pointer to the counter V
 *
 * \return
 *   - The function returns the least significant word, i.e. highest word, of the counter V for integrity protection
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClRandomModes_CtrDrbg_incV)
static MCUX_CSSL_FP_PROTECTED_TYPE(uint32_t) mcuxClRandomModes_CtrDrbg_incV(uint32_t *pV)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_incV);

    /* Back up the least significant word of V for later return */
    const uint32_t beforeIncrement = pV[MCUXCLAES_BLOCK_SIZE_IN_WORDS - 1u];
    uint8_t *pVByte = (uint8_t *)pV;

    /* To increment the counter V considered as a big endian integer, increment the highest byte and propagate
     * a possible carry bit from right to left */
    for(int32_t i = (int32_t)MCUXCLAES_BLOCK_SIZE - 1; i >= 0; i--)
    {
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("A possible overflow is intended in the implementation")
        pVByte[i] += 1u;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

        /* If incrementing the current byte of V resulted in a zero byte, we continue propagating the carry.
         * Otherwise, we leave the for loop early. */
        if(0u != pVByte[i])
        {
            break;
        }
    }

    /* Exit the function and return the highest word of V for integrity checking in the calling function */
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_incV, beforeIncrement);
}

#if defined(MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION)
/**
 * \brief This function implements the BCC function as specified in NIST SP800-90A.
 *
 * \param  pSession             Handle for the current CL session
 * \param  mode[in]             Handle for the current Random Mode
 * \param  pKey[in]             Pointer to the block encryption key K
 * \param  pData[in]            Pointer to input data
 * \param  dataLen[in]          Byte length of input data
 * \param  pOut[out]            Pointer to output buffer
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK         if the CTR_DRBG instantiation finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR      if there was an RNG error or if a memory allocation error occured
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_bcc)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_bcc(
        mcuxClSession_Handle_t pSession,
        mcuxClRandom_Mode_t mode,
        uint32_t const *pKey,
        uint32_t * const pData,
        uint32_t dataLen,
        uint32_t *pOut)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_bcc);

    uint32_t keyLen = (uint32_t)(mode->securityStrength) / 8u;

    /* Initialize buffer in CPU workarea for the input block for the block cipher operations */
    uint32_t *pInputBlock = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS);
    if(NULL == pInputBlock)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_bcc, MCUXCLRANDOM_STATUS_ERROR);
    }

    /* Initialize the chaining value in the output buffer with zeros */
    uint32_t *pChainingValue = pOut;
    MCUXCLMEMORY_FP_MEMORY_SET((uint8_t *)pChainingValue, 0u, MCUXCLAES_BLOCK_SIZE);

    /* Get number of input blocks to be processed */
    uint32_t numBlocks = dataLen / MCUXCLAES_BLOCK_SIZE;
    uint32_t i, j;
    uint32_t blkSizeInWords = MCUXCLAES_BLOCK_SIZE/sizeof(uint32_t);

    for (i = 0; i < numBlocks; i++)
    {
        /* XOR chaining value to current input data block */
        for (j = 0; j < blkSizeInWords; j++)
        {
            /* TODO CLNS-7837 Replace with SecureXOR */
            pInputBlock[j] = pData[j + (i * blkSizeInWords)] ^ pChainingValue[j];
        }

        MCUXCLBUFFER_INIT(chainingValueBuf, NULL, (uint8_t *)pChainingValue, MCUXCLAES_BLOCK_SIZE);

        /* Encrypt the input block to obtain the new chaining value */
        MCUX_CSSL_FP_FUNCTION_CALL(ret_blockcipher,
            mcuxClRandomModes_DRBG_AES_Internal_blockcipher(pSession,
                                                           pInputBlock,
                                                           pKey,
                                                           chainingValueBuf,
                                                           keyLen));
        if(MCUXCLRANDOM_STATUS_ERROR == ret_blockcipher)
        {
            /* Free CPU workarea used by this function (pInputBlock) */
            mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS);

            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_bcc, MCUXCLRANDOM_STATUS_ERROR,
                (i + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher));
        }
        else if (MCUXCLRANDOM_STATUS_OK != ret_blockcipher)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_bcc, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
        else
        {
            /* Intentionally left empty */
        }
    }

    /* Free CPU workarea used by this function (pInputBlock) */
    mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_bcc, MCUXCLRANDOM_STATUS_OK,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
        i * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher));
}



#define MCUXCLRANDOM_MAX_DF_BITS        512u

static uint32_t const mcuxClRandomModes_CtrDrbg_df_key[8u] = {
    0x03020100u, 0x07060504u, 0x0b0a0908u, 0x0f0e0d0cu,
    0x13121110u, 0x17161514u, 0x1b1a1918u, 0x1f1e1d1cu
};

/**
 * \brief This function implements the Block_Cipher_df function as specified in NIST SP800-90A.
 *
 * This function instantiates a CTR_DRBG in following the lines of the function CTR_DRBG_Instantiate_algorithm as specified in NIST SP800-90A.
 * The function obtains entropy input for the DRBG seed from the TRNG.
 *
 * \param  pSession               Handle for the current CL session
 * \param  mode[in]               Handle for the current Random Mode
 * \param  pInputString[in/out]   Pointer to the input string and the output of the derivation function. The length is limited to (UINT32_MAX / 2).
 * \param  inputStringLen[in]     Byte length of the input string
 * \param  outputLen[in]          Byte length of the output
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK              if the CTR_DRBG instantiation finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR           if there was an RNG error or if a memory allocation error occured
 *   - MCUXCLRANDOM_STATUS_FAULT_ATTACK    if the CTR_DRBG instantiation failed due to other unexpected reasons
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_df)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_df(
        mcuxClSession_Handle_t pSession,
        mcuxClRandom_Mode_t mode,
        uint8_t *pInputString,
        uint32_t inputStringLen,
        uint32_t outputLen)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_df);
    MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(inputStringLen, 0u, UINT32_MAX / 2u, MCUXCLRANDOM_STATUS_ERROR)
    MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(outputLen, 1u, UINT32_MAX, MCUXCLRANDOM_STATUS_ERROR)

    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    const mcuxClRandomModes_DrbgModeDescriptor_t *pDrbgMode = (const mcuxClRandomModes_DrbgModeDescriptor_t *) mode->pDrbgMode;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    uint32_t seedLen = pDrbgMode->pDrbgVariant->seedLen;
    uint32_t keyLen = (uint32_t)(mode->securityStrength) / 8u;

    /*
     * Step 1 specified in NIST SP800-90A:
     *
     * Verify that seedLen is valid. Invalid values should not occur and will trigger a FAULT_ATTACK.
     */
    if (MCUXCLRANDOM_MAX_DF_BITS < seedLen * 8u)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }

    /*
     * Steps 2-7 specified in NIST SP800-90A:
     *
     * Prepare values S and IV (input for BCC function) in CPU workarea:
     * layout: IV || L || N || input_string || 0x80 || 0 padding
     * length: 16    4    4    seed size       1       (16-(4+4+seedSize+1)%16)%16
     */

    /* Allocate space for IV */
    uint32_t *pIV = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS);
    if(NULL == pIV)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_ERROR);
    }

    /* Determine the byte length of S: (4+4+seedSize+1 + ((16-(4+4+seedSize+1)%16)%16)) */
    uint32_t lenOfS = sizeof(uint32_t)+sizeof(uint32_t)+inputStringLen;
    uint32_t tempLen = lenOfS;
    /* add 1 for 0x80 */
    lenOfS += 1u;
    /* pad with zeros if to align with the block size */
    if (0u != (lenOfS % MCUXCLAES_BLOCK_SIZE))
    {
        lenOfS += (MCUXCLAES_BLOCK_SIZE - (lenOfS % MCUXCLAES_BLOCK_SIZE));
    }

    /* Allocate space for S */
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pS is always valid in work area.")
    uint32_t *pS = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(lenOfS));
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
    if(NULL == pS)
    {
        /* Free workarea (pIV) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS);
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_ERROR);
    }

    /* Pre-initialize the IV value with zeros to take care of the padding with zeros */
    MCUXCLMEMORY_FP_MEMORY_SET((uint8_t*)pIV, 0u, MCUXCLAES_BLOCK_SIZE);

    /* Pre-initialize S with zeros to take care of cases where padding with 0 is needed at the end */
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pSByte will be in the valid range pS[0 ~ lenOfS].");
    uint8_t *pSByte = (uint8_t *) pS;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
    MCUXCLMEMORY_FP_MEMORY_SET(pSByte, 0u, lenOfS);

    /* Calculate (big integer) values L and N and initialize value S as specified in NIST SP800-90A */
    uint32_t L = inputStringLen << 24;
    uint32_t S = outputLen << 24;
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pS is always valid in work area.")
    pS[0] = L;
    pS[1] = S;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pSByte+tempLen will be in the valid range pS[0 ~ lenOfS].");
    pSByte[tempLen] = 0x80;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

    MCUX_CSSL_FP_FUNCTION_CALL(copy_result, mcuxCsslMemory_Copy(
        mcuxCsslParamIntegrity_Protect(4u, pInputString, &pS[2], inputStringLen, inputStringLen),
            (uint8_t const *)pInputString,
            (uint8_t *)&pS[2],
            inputStringLen,
            inputStringLen)
    );

    if(MCUXCSSLMEMORY_STATUS_OK != copy_result)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }

    /*
     * Steps 8-11 specified in NIST SP800-90A:
     *
     * Call BCC function in a loop to determine intermediate values K and X.
     */

    /* Allocate space for a temp buffer (temp1) in CPU workarea for the output of the BCC loop.
     * This buffer will later contain the values K and X concatenated:
     * layout:   K      || X  || unused
     * length:   keylen    16    (keylen % 16)
     *
     * NOTE: Additional space is reserved behind K and X for the case keyLen is not a multiple of the block size
     */
    uint32_t temp1Len = MCUXCLRANDOMMODES_ALIGN_TO_AES_BLOCKSIZE(keyLen) + MCUXCLAES_BLOCK_SIZE;
    uint32_t *pTemp1 = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(temp1Len));
    if(NULL == pTemp1)
    {
        /* Free workarea (pS + pIV) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS + MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(lenOfS));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df,
            MCUXCLRANDOM_STATUS_ERROR,
            2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy));
    }

    /* Call BCC function in a loop */
    uint32_t outBlocks = temp1Len / MCUXCLAES_BLOCK_SIZE;
    uint32_t i;
    for (i = 0; i < outBlocks; i++)
    {
        /* Update IV value with the next loop counter converted to a (big endian) 32 bit integer padded with zeros */
        pIV[0] = i << 24;

        /* Call BCC function */
        MCUX_CSSL_FP_FUNCTION_CALL(result_bcc,
                                  mcuxClRandomModes_CtrDrbg_bcc(
                                          pSession,
                                          mode,
                                          mcuxClRandomModes_CtrDrbg_df_key,
                                          pIV,
                                          MCUXCLAES_BLOCK_SIZE + lenOfS,
                                          &pTemp1[i*MCUXCLAES_BLOCK_SIZE/sizeof(uint32_t)]));

        if(MCUXCLRANDOM_STATUS_ERROR == result_bcc)
        {
            /* Free allocated memory  (pTemp1 + pS + pIV) */
            mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS + MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(temp1Len + lenOfS));
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_ERROR,
                    MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
                    2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
                    (i + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_bcc));
        }
        else if (MCUXCLRANDOM_STATUS_OK != result_bcc)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
        else
        {
            /* Intentionally left empty */
        }
    }

    /* Initialize pointers to K and X */
    uint32_t *pK = pTemp1;
    uint32_t *pX = &pTemp1[keyLen/sizeof(uint32_t)];


    /*
     * Steps 12-15 specified in NIST SP800-90A:
     *
     * Compute and return the output of the derivation function.
     */

    /* Reuse CPU workarea buffer IV || S for value temp2, the result of the upcoming block encryption loop */
    uint32_t *pTemp2 = pIV;

    MCUXCLBUFFER_INIT(temp2Buf, NULL, (uint8_t *)pTemp2, MCUXCLAES_BLOCK_SIZE);

    /* Execute first block encryption and store the result directly in temp2 */
    MCUX_CSSL_FP_FUNCTION_CALL(ret_blockcipher1,
      mcuxClRandomModes_DRBG_AES_Internal_blockcipher(pSession,
                                                     pX,
                                                     pK,
                                                     temp2Buf,
                                                     keyLen));

    if(MCUXCLRANDOM_STATUS_ERROR == ret_blockcipher1)
    {
        /* Free CPU workarea allocated by this function (pTemp1 + pS + pIV) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS + MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(temp1Len + lenOfS));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_ERROR,
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
                2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
                i * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_bcc),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher));
    }
    else if (MCUXCLRANDOM_STATUS_OK != ret_blockcipher1)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Intentionally left empty */
    }

    /* Execute the remaining block encryption operations.
     *
     * NOTE: Different to the specification in NIST SP800-90A, input and output of the block encryption
     *       are stored directly in the temp2 buffer. */
    uint32_t j;
    outBlocks = (uint32_t) (((outputLen - 1U) / MCUXCLAES_BLOCK_SIZE) + 1u);
    MCUXCLBUFFER_INIT(temp2LoopBuf, NULL, (uint8_t *)pTemp2, MCUXCLAES_BLOCK_SIZE + lenOfS);

    for (j = 1u; j < outBlocks; j++)
    {
        MCUXCLBUFFER_UPDATE(temp2LoopBuf, (MCUXCLAES_BLOCK_SIZE));
        MCUX_CSSL_FP_FUNCTION_CALL(ret_blockcipher2,
            mcuxClRandomModes_DRBG_AES_Internal_blockcipher(pSession,
                                                           &pTemp2[(j-1u)*MCUXCLAES_BLOCK_SIZE/sizeof(uint32_t)],
                                                           pK,
                                                           temp2LoopBuf,
                                                           keyLen));

        if (MCUXCLRANDOM_STATUS_ERROR == ret_blockcipher2)
        {
            /* Free CPU workarea allocated by this function (pTemp1 + pS + pIV) */
            mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS + MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(temp1Len + lenOfS));
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_ERROR,
                    MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
                    2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
                    i * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_bcc),
                    (j + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher));
        }
        else if (MCUXCLRANDOM_STATUS_OK != ret_blockcipher2)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
        else
        {
            /* Intentionally left empty */
        }
    }

    /* Copy the result to the output buffer */
    MCUXCLRANDOM_SECURECOPY(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_FAULT_ATTACK, pInputString, (uint8_t const *)pIV, outputLen);

    /* Free CPU workarea allocated by this function (pTemp1 + pS + pIV) */
    mcuxClSession_freeWords_cpuWa(pSession, MCUXCLAES_BLOCK_SIZE_IN_WORDS + MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(temp1Len + lenOfS));
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_df, MCUXCLRANDOM_STATUS_OK,
            MCUXCLRANDOM_FP_CALLED_SECURECOPY,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            2u * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
            i * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_bcc),
            j  * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher));
}
#endif



#ifdef MCUXCL_FEATURE_RANDOMMODES_SECSTRENGTH_256
/* MISRA Ex. 20 - Rule 5.1 */
const mcuxClRandomModes_DrbgVariantDescriptor_t mcuxClRandomModes_DrbgVariantDescriptor_CtrDrbg_AES256 =
{
    .reseedInterval = MCUXCLRANDOMMODES_RESEED_INTERVAL_CTR_DRBG_AES256,
    .seedLen = MCUXCLRANDOMMODES_SEEDLEN_CTR_DRBG_AES256,
    .initSeedSize = MCUXCLRANDOMMODES_ENTROPYINPUT_SIZE_INIT_CTR_DRBG_AES256,
    .reseedSeedSize = MCUXCLRANDOMMODES_ENTROPYINPUT_SIZE_RESEED_CTR_DRBG_AES256
};
#endif // MCUXCL_FEATURE_RANDOMMODES_SECSTRENGTH_256

/**
 * \brief This function instantiates a CTR_DRBG following the lines of the function CTR_DRBG_Instantiate_algorithm as specified in NIST SP800-90A
 *
 * This function instantiates a CTR_DRBG in following the lines of the function CTR_DRBG_Instantiate_algorithm as specified in NIST SP800-90A.
 * The function obtains entropy input and nonce for the DRBG seed from the TRNG.
 *
 * \param  pSession                     Handle for the current CL session
 * \param  mode[in]                     Handle for the current Random Mode
 * \param  context[in]                  Handle for the current Random Context
 * \param  pEntropyInputAndNonce[in]    Pointer to entropy input and nonce
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK         if the CTR_DRBG instantiation finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR           if a memory allocation error occured
 *   - MCUXCLRANDOM_STATUS_FAULT_ATTACK    if the CTR_DRBG instantiation failed due to other unexpected reasons
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, mcuxClRandomModes_instantiateAlgorithm_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_instantiateAlgorithm(
        mcuxClSession_Handle_t pSession,
        mcuxClRandom_Mode_t mode,
        mcuxClRandom_Context_t context,
        uint8_t *pEntropyInputAndNonce
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm);

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("Cast to a more specific type is allowed")
    mcuxClRandomModes_Context_CtrDrbg_Generic_t *pRngCtxGeneric = (mcuxClRandomModes_Context_CtrDrbg_Generic_t *) context;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()

    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    const mcuxClRandomModes_DrbgModeDescriptor_t *pDrbgMode = (const mcuxClRandomModes_DrbgModeDescriptor_t *) mode->pDrbgMode;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    uint32_t seedLen = pDrbgMode->pDrbgVariant->seedLen;
    uint32_t initSeedSize = pDrbgMode->pDrbgVariant->initSeedSize;

    /* This max is needed as initSeedSize might be smaller than seedlen, but the df uses this buffer both for input and output. */
    uint32_t dfBufferSize = MCUXCLCORE_MAX(initSeedSize, seedLen);
    uint32_t *pSeedMaterial = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
    if(NULL == pSeedMaterial)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_ERROR);
    }

    /* Copy the seed to the seedMaterial buffer */
    MCUX_CSSL_FP_FUNCTION_CALL(copy_result, mcuxCsslMemory_Copy(
        mcuxCsslParamIntegrity_Protect(4u, (uint8_t const *)pEntropyInputAndNonce, (uint8_t *)pSeedMaterial, initSeedSize, initSeedSize),
            (uint8_t const *)pEntropyInputAndNonce,
            (uint8_t *)pSeedMaterial,
            initSeedSize,
            initSeedSize)
    );

    if(MCUXCSSLMEMORY_STATUS_OK != copy_result)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }

#ifdef MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION
    /* pSeedMaterial use as both input and output */
    MCUX_CSSL_FP_FUNCTION_CALL(ret_df, mcuxClRandomModes_CtrDrbg_df(
                    pSession,
                    mode,
                    (uint8_t *)pSeedMaterial,
                    initSeedSize,
                    seedLen));
    if(MCUXCLRANDOM_STATUS_ERROR == ret_df)
    {
        /* Free workarea (pSeedMaterial) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_df));
    }
    else if (MCUXCLRANDOM_STATUS_OK != ret_df)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Intentionally left empty */
    }
#endif

    /* Set to 0 counter V and key K in context
     * According to sp800-90A, V and Key need to be initialized with zeros.
     *
     * NOTE: V and K lie next to each other in the context */
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("pRngCtxGeneric is casted to mcuxClRandomModes_Context_CtrDrbg_Generic_t")
    uint32_t *pState = pRngCtxGeneric->state;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
    MCUXCLMEMORY_FP_MEMORY_SET((uint8_t *)pState, 0u, seedLen);

    /* Update the CTR_DRBG state
     *
     * NOTE: The size of the provided DRBG seed equals seedLen, so no padding with zeros is needed to derive the seedMaterial from the entryopInput
     */
    MCUX_CSSL_FP_FUNCTION_CALL(result_updatestate, mcuxClRandomModes_CtrDrbg_UpdateState(
                    pSession,
                    mode,
                    context,
                    pSeedMaterial));
    if(MCUXCLRANDOM_STATUS_ERROR == result_updatestate)
    {
        /* Free workarea (pSeedMaterial) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
#ifdef MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_df),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#else
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#endif
    }
    else if (MCUXCLRANDOM_STATUS_OK != result_updatestate)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Initialize the reseed counter */
        pRngCtxGeneric->reseedCounter = 1u;

        /* Free workarea (pSeedMaterial) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
    }
#ifdef MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_OK,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_df),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#else
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_instantiateAlgorithm, MCUXCLRANDOM_STATUS_OK,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#endif
}


/**
 * \brief This function reseeds a CTR_DRBG following the lines of the function CTR_DRBG_Reseed_algorithm as specified in NIST SP800-90A
 *
 * This function reseeds a CTR_DRBG following the lines of the function CTR_DRBG_Instantiate_algorithm as specified in NIST SP800-90A.
 * The function obtains entropy input for the DRBG seed from the TRNG.
 *
 * \param  pSession[in]         Handle for the current CL session
 * \param  mode[in]             Handle for the current Random Mode
 * \param  context[in]          Handle for the current Random Context
 * \param  pEntropyInput[in]    Pointer to entropy input
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK         if the CTR_DRBG instantiation finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR           if a memory allocation error occured
 *   - MCUXCLRANDOM_STATUS_FAULT_ATTACK    if the CTR_DRBG instantiation failed due to other unexpected reasons
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, mcuxClRandomModes_reseedAlgorithm_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_reseedAlgorithm(
        mcuxClSession_Handle_t pSession,
        mcuxClRandom_Mode_t mode,
        mcuxClRandom_Context_t context,
        uint8_t *pEntropyInput)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_reseedAlgorithm);

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("Cast to a more specific type is allowed")
    mcuxClRandomModes_Context_CtrDrbg_Generic_t *pRngCtxGeneric = (mcuxClRandomModes_Context_CtrDrbg_Generic_t *) context;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()

    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    const mcuxClRandomModes_DrbgModeDescriptor_t *pDrbgMode = (const mcuxClRandomModes_DrbgModeDescriptor_t *) mode->pDrbgMode;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    uint32_t seedLen = pDrbgMode->pDrbgVariant->seedLen;
    uint32_t reseedSeedSize = pDrbgMode->pDrbgVariant->reseedSeedSize;

    /* This max is needed as reseedSeedSize might be smaller than seedlen, but the df uses this buffer both for input and output. */
    uint32_t dfBufferSize = MCUXCLCORE_MAX(reseedSeedSize, seedLen);
    uint32_t *pSeedMaterial = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
    if(NULL == pSeedMaterial)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_ERROR);
    }

    /* Copy the seed to the seedMaterial buffer */
    MCUX_CSSL_FP_FUNCTION_CALL(copy_result, mcuxCsslMemory_Copy(
        mcuxCsslParamIntegrity_Protect(4u, (uint8_t const *)pEntropyInput, (uint8_t *)pSeedMaterial, reseedSeedSize, reseedSeedSize),
            (uint8_t const *)pEntropyInput,
            (uint8_t *)pSeedMaterial,
            reseedSeedSize,
            reseedSeedSize)
    );

    if(MCUXCSSLMEMORY_STATUS_OK != copy_result)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }

#ifdef MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION
    /* pSeedMaterial use as both input and output */
    MCUX_CSSL_FP_FUNCTION_CALL(ret_df, mcuxClRandomModes_CtrDrbg_df(
                pSession,
                mode,
                (uint8_t *)pSeedMaterial,
                reseedSeedSize,
                seedLen));
    if(MCUXCLRANDOM_STATUS_ERROR == ret_df)
    {
        /* Free workarea (pSeedMaterial) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_df));
    }
    else if (MCUXCLRANDOM_STATUS_OK != ret_df)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Intentionally left empty */
    }
#endif

    /* Update the CTR_DRBG state */
    MCUX_CSSL_FP_FUNCTION_CALL(result_updatestate, mcuxClRandomModes_CtrDrbg_UpdateState(
                pSession,
                mode,
                context,
                pSeedMaterial));
    if(MCUXCLRANDOM_STATUS_ERROR == result_updatestate)
    {
        /* Free workarea (pSeedMaterial) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
#ifdef MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_df),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#else
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#endif
    }
    else if (MCUXCLRANDOM_STATUS_OK != result_updatestate)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Reset the reseed counter */
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("pRngCtxGeneric was typecasted before")
        pRngCtxGeneric->reseedCounter = 1u;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()

        /* Free workarea (pSeedMaterial) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(dfBufferSize));
    }

#ifdef MCUXCL_FEATURE_RANDOMMODES_DERIVATION_FUNCTION
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_OK,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_df),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#else
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_reseedAlgorithm, MCUXCLRANDOM_STATUS_OK,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
#endif
}


/**
 * \brief This function generates random numbers from a CTR_DRBG following the lines of the function CTR_DRBG_Generate_algorithm as specified in NIST SP800-90A
 *
 * \param  pSession             Handle for the current CL session
 * \param  mode[in]             Handle for the current Random Mode
 * \param  context[in]          Handle for the current Random Context
 * \param  pOut[out]            Output buffer to which the generated randomness will be written
 * \param  outLength[in]        Number of requested random bytes
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK         if the random number generation finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR           if a memory allocation error occured
 *   - MCUXCLRANDOM_STATUS_FAULT_ATTACK    if the random number generation failed due to other unexpected reasons
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_generateAlgorithm, mcuxClRandomModes_generateAlgorithm_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_generateAlgorithm(
        mcuxClSession_Handle_t pSession,
        mcuxClRandom_Mode_t mode,
        mcuxClRandom_Context_t context,
        mcuxCl_Buffer_t pOut,
        uint32_t outLength)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_generateAlgorithm);

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("Cast to a more specific type is allowed")
    mcuxClRandomModes_Context_CtrDrbg_Generic_t *pRngCtxGeneric = (mcuxClRandomModes_Context_CtrDrbg_Generic_t *) context;
    /* The number of reseed attempts is limited to 2^48 according to NIST SP800-90A */
    MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER(pRngCtxGeneric->reseedCounter, 0u, (1uLL << 48u), MCUXCLRANDOM_STATUS_ERROR)
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()

    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    const mcuxClRandomModes_DrbgModeDescriptor_t *pDrbgMode = (const mcuxClRandomModes_DrbgModeDescriptor_t *) mode->pDrbgMode;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    uint32_t seedLen = pDrbgMode->pDrbgVariant->seedLen;

    MCUX_CSSL_FP_FUNCTION_CALL(result_generate,
        mcuxClRandomModes_CtrDrbg_generateOutput(
                    pSession,
                    mode,
                    context,
                    pOut,
                    outLength));
    if(MCUXCLRANDOM_STATUS_ERROR == result_generate)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_generateOutput));
    }
    else if(MCUXCLRANDOM_STATUS_OK != result_generate)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Intentionally left empty */
    }

    /* Init additionalInput for state update in CPU workarea to all zeros and update the CTR_DRBG state */
    uint32_t *pAdditionalInput = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(seedLen));
    if(NULL == pAdditionalInput)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_generateOutput));
    }

    MCUXCLMEMORY_FP_MEMORY_SET((uint8_t *)pAdditionalInput, 0u, seedLen);

    MCUX_CSSL_FP_FUNCTION_CALL(result_updatestate, mcuxClRandomModes_CtrDrbg_UpdateState(
                    pSession,
                    mode,
                    context,
                    pAdditionalInput));
    if(MCUXCLRANDOM_STATUS_ERROR == result_updatestate)
    {
        /* Free workarea (pAdditionalInput) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(seedLen));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateAlgorithm, MCUXCLRANDOM_STATUS_ERROR,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_generateOutput),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set));
    }
    else if (MCUXCLRANDOM_STATUS_OK != result_updatestate)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateAlgorithm, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
    }
    else
    {
        /* Increment the reseed counter */
        pRngCtxGeneric->reseedCounter += 1u;

        /* Free workarea (pAdditionalInput) */
        mcuxClSession_freeWords_cpuWa(pSession, MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(seedLen));

        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateAlgorithm, MCUXCLRANDOM_STATUS_OK,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_generateOutput),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_UpdateState));
    }
}


/**
 * \brief This function updates the internal state of a CTR_DRBG as specified in NIST SP800-90A
 *
 * \param  pSession             Handle for the current CL session
 * \param  mode[in]             Handle for the current Random Mode
 * \param  context[in/out]      Handle for the current Random Context
 * \param  pProvidedData[in]    Additional data to incorporate into the state
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK            if the state update finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR         if a memory allocation error or SGI HW error occured
 *   - MCUXCLRANDOM_STATUS_FAULT_ATTACK  if the state update failed due to other unexpected reasons
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_UpdateState)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_UpdateState(
        mcuxClSession_Handle_t pSession,
        mcuxClRandom_Mode_t mode,
        mcuxClRandom_Context_t context,
        uint32_t *pProvidedData
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_UpdateState);

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("Cast to a more specific type is allowed")
    mcuxClRandomModes_Context_CtrDrbg_Generic_t *pCtx = (mcuxClRandomModes_Context_CtrDrbg_Generic_t *) context;
    uint32_t *pState = pCtx->state;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()

    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    const mcuxClRandomModes_DrbgModeDescriptor_t *pDrbgMode = (const mcuxClRandomModes_DrbgModeDescriptor_t *) mode->pDrbgMode;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    uint32_t seedLen = pDrbgMode->pDrbgVariant->seedLen;
    uint32_t securityStrength = (uint32_t)(mode->securityStrength);
    uint32_t *pKey = pState;
    uint32_t *pV = &pState[securityStrength/32u];

    /* produce the new Key and V */
    uint32_t *pTemp = mcuxClSession_allocateWords_cpuWa(pSession, MCUXCLRANDOMMODES_ALIGN_TO_AES_BLOCKSIZE(seedLen) / sizeof(uint32_t));
    if(NULL == pTemp)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_UpdateState, MCUXCLRANDOM_STATUS_ERROR);
    }

    uint32_t seedLenInBlkSize = MCUXCLRANDOMMODES_ALIGN_TO_AES_BLOCKSIZE(seedLen) / MCUXCLAES_BLOCK_SIZE;
    uint32_t i, j;

    MCUXCLBUFFER_INIT(tempBuf, NULL, (uint8_t *) pTemp, (MCUXCLRANDOMMODES_ALIGN_TO_AES_BLOCKSIZE(seedLen)));

    for (i = 0u; i < seedLenInBlkSize; i++)
    {
        MCUX_CSSL_FP_FUNCTION_CALL(checkIncrement, mcuxClRandomModes_CtrDrbg_incV(pV));
        if(checkIncrement == pV[(MCUXCLAES_BLOCK_SIZE/sizeof(uint32_t)) - 1u])
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_UpdateState, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }

        MCUX_CSSL_FP_FUNCTION_CALL(ret_blockcipher,
            mcuxClRandomModes_DRBG_AES_Internal_blockcipher(pSession, pV, pKey, tempBuf, securityStrength/8u));

        if(MCUXCLRANDOM_STATUS_ERROR == ret_blockcipher)
        {
            /* Free workarea (pTemp) */
            mcuxClSession_freeWords_cpuWa(pSession, MCUXCLRANDOMMODES_ALIGN_TO_AES_BLOCKSIZE(seedLen) / sizeof(uint32_t));
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_UpdateState, MCUXCLRANDOM_STATUS_ERROR,
                (i + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher),
                (i + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_incV));
        }
        else if (MCUXCLRANDOM_STATUS_OK != ret_blockcipher)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_UpdateState, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
        else
        {
            /* move tempBuf as mcuxClRandomModes_DRBG_AES_Internal_blockcipher writes consecutive in tempBuf */
            MCUXCLBUFFER_UPDATE(tempBuf, MCUXCLAES_BLOCK_SIZE);
        }
    }

    MCUX_CSSL_FP_LOOP_DECL(xorLoop);
    for (j = 0; j < (seedLen / sizeof(uint32_t)); j++)
    {
        /* TODO CLNS-7837 Replace with SecureXOR */
        pTemp[j] = pTemp[j] ^ pProvidedData[j];
        MCUX_CSSL_FP_LOOP_ITERATION(xorLoop);
    }

    /* update the key V in context */
    MCUXCLRANDOM_SECURECOPY(mcuxClRandomModes_CtrDrbg_UpdateState,
                    MCUXCLRANDOM_STATUS_FAULT_ATTACK,
                    (uint8_t *)pKey,
                    (uint8_t const *)pTemp,
                    securityStrength/8u + MCUXCLAES_BLOCK_SIZE);

    /* Free workarea (pTemp) */
    mcuxClSession_freeWords_cpuWa(pSession, MCUXCLRANDOMMODES_ALIGN_TO_AES_BLOCKSIZE(seedLen) / sizeof(uint32_t));

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_UpdateState, MCUXCLRANDOM_STATUS_OK,
                i * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher),
                i * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_incV),
                MCUX_CSSL_FP_LOOP_ITERATIONS(xorLoop, (seedLen / sizeof(uint32_t))),
                MCUXCLRANDOM_FP_CALLED_SECURECOPY);
}


/**
 * \brief This function generates output from the internal state of a CTR_DRBG as specified in NIST SP800-90A
 *
 * \param  pSession             Handle for the current CL session
 * \param  mode[in]             Handle for the current Random Mode
 * \param  context[in]          Handle for the current Random Context
 * \param  pOut[out]            Output buffer to which the generated randomness will be written
 * \param  outLength[in]        Number of requested random bytes
 *
 * \return
 *   - MCUXCLRANDOM_STATUS_OK            if the random number generation finished successfully
 *   - MCUXCLRANDOM_STATUS_ERROR         if a memory allocation error occured
 *   - MCUXCLRANDOM_STATUS_FAULT_ATTACK  if the random number generation failed due to other unexpected reasons
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClRandomModes_CtrDrbg_generateOutput)
MCUX_CSSL_ANALYSIS_START_PATTERN_DESCRIPTIVE_IDENTIFIER()
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClRandom_Status_t) mcuxClRandomModes_CtrDrbg_generateOutput(
MCUX_CSSL_ANALYSIS_STOP_PATTERN_DESCRIPTIVE_IDENTIFIER()
        mcuxClSession_Handle_t pSession UNUSED_PARAM,
        mcuxClRandom_Mode_t mode,
        mcuxClRandom_Context_t context,
        mcuxCl_Buffer_t pOut,
        uint32_t outLength
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClRandomModes_CtrDrbg_generateOutput);

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("Cast to a more specific type is allowed")
    mcuxClRandomModes_Context_CtrDrbg_Generic_t *pCtx = (mcuxClRandomModes_Context_CtrDrbg_Generic_t *) context;
    uint32_t *pState = pCtx->state;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()

    uint32_t securityStrength = (uint32_t)(mode->securityStrength);

    uint32_t *pKey = pState;
    uint32_t *pV = &pState[securityStrength/32u];
    const uint32_t requestSizeRemainingBytes = outLength % MCUXCLAES_BLOCK_SIZE;
    uint32_t requestSizeFullBlocksBytes = outLength - requestSizeRemainingBytes;
    MCUX_CSSL_FP_COUNTER_STMT(uint32_t requestSizeFullBlocks = requestSizeFullBlocksBytes / MCUXCLAES_BLOCK_SIZE);
    uint32_t outIndex = 0u;
    MCUXCLBUFFER_DERIVE_RW(pOutCopy, pOut, 0u);

    /* Request as many random bytes as possible with full word size. */
    while (requestSizeFullBlocksBytes > 0u)
    {
        MCUX_CSSL_FP_FUNCTION_CALL(checkIncrement, mcuxClRandomModes_CtrDrbg_incV(pV));
        if(checkIncrement == pV[(MCUXCLAES_BLOCK_SIZE/sizeof(uint32_t)) - 1u])
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }

        MCUX_CSSL_FP_FUNCTION_CALL(ret_Internal_blockcipher,
            mcuxClRandomModes_DRBG_AES_Internal_blockcipher(pSession, pV, pKey, pOutCopy, securityStrength/8u));
        if (MCUXCLRANDOM_STATUS_ERROR == ret_Internal_blockcipher)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_ERROR,
                (outIndex/MCUXCLAES_BLOCK_SIZE + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher),
                (outIndex/MCUXCLAES_BLOCK_SIZE + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_incV));
        }
        else if (MCUXCLRANDOM_STATUS_OK != ret_Internal_blockcipher)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
        else
        {
            /* Intentionally left empty */
        }
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("The range of outIndex must be [0 : outLength - (outLength % MCUXCLAES_BLOCK_SIZE)]")
        outIndex += MCUXCLAES_BLOCK_SIZE;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP()
        requestSizeFullBlocksBytes -= MCUXCLAES_BLOCK_SIZE;
        MCUXCLBUFFER_UPDATE(pOutCopy, MCUXCLAES_BLOCK_SIZE);
    }

    /* If requested size is not a multiple of block size, request one (additional) block and use it only partially. */
    if (requestSizeRemainingBytes > 0u)
    {
        uint8_t pRequestRemaining[MCUXCLAES_BLOCK_SIZE] = {0u};

        MCUX_CSSL_FP_FUNCTION_CALL(checkIncrement, mcuxClRandomModes_CtrDrbg_incV(pV));
        if(checkIncrement == pV[(MCUXCLAES_BLOCK_SIZE/sizeof(uint32_t)) - 1u])
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }

        MCUXCLBUFFER_INIT(requestRemainingBuf, NULL, pRequestRemaining, MCUXCLAES_BLOCK_SIZE);
        MCUX_CSSL_FP_FUNCTION_CALL(ret_Internal_blockcipher,
            mcuxClRandomModes_DRBG_AES_Internal_blockcipher(pSession, pV, pKey, requestRemainingBuf, securityStrength/8u));
        if(MCUXCLRANDOM_STATUS_ERROR == ret_Internal_blockcipher)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_ERROR,
                (requestSizeFullBlocks + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher),
                (requestSizeFullBlocks + 1u) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_incV));
        }
        else if (MCUXCLRANDOM_STATUS_OK != ret_Internal_blockcipher)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
        else
        {
            /* Intentionally left empty */
        }

        /* Copy the remaining bytes from the buffer to output. */
        MCUX_CSSL_FP_FUNCTION_CALL(copy_result, mcuxCsslMemory_Copy(
            mcuxCsslParamIntegrity_Protect(4u, pRequestRemaining, MCUXCLBUFFER_GET(pOut) + outIndex, requestSizeRemainingBytes, requestSizeRemainingBytes),
                pRequestRemaining,
                MCUXCLBUFFER_GET(pOut) + outIndex,
                requestSizeRemainingBytes,
                requestSizeRemainingBytes)
        );
        if(MCUXCSSLMEMORY_STATUS_OK != copy_result)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_FAULT_ATTACK);
        }
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClRandomModes_CtrDrbg_generateOutput, MCUXCLRANDOM_STATUS_OK,
            (requestSizeFullBlocks) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher),
            (requestSizeFullBlocks) * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_incV),
            MCUX_CSSL_FP_CONDITIONAL((requestSizeRemainingBytes > 0u),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Copy),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_DRBG_AES_Internal_blockcipher),
                MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandomModes_CtrDrbg_incV))
    );
}
