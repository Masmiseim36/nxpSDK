/*! *********************************************************************************
 * Copyright 2021-2025 NXP
 *
 * \file
 *
 * This is the Source file for the AES Matyas-Meyer-Oseas (AES-MMO) hash function.
 * Reference:
 * ZigBee specification, document 05-3474-21, Aug 2015,
 * section B.6 Block-Cipher-Based Cryptographic Hash Function.
 * If using AES 128, let's say N=128/8 i.e 16. The maximum message bit length can be 2^2N.
 * For AES-128 2^N bits is 2^13 i,e, 8192.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "SecLib.h"
#include "FunctionLib.h"
#include "fsl_component_mem_manager.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define AES_MMO_HASH_SIZE  16u
#define AES_MMO_BLOCK_SIZE AES_BLOCK_SIZE

#define AES_MMO_MAX_HASHED_BYTES ((uint32_t)(UINT32_MAX >> 3))

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
typedef struct
{
    tuAES_Block digest;
    tuAES_Block buffer;
    uint32_t    totalBytes;
    uint8_t     bytes;
} aesMmoContext_t;

typedef struct
{
    aesMmoContext_t hashCtx;
    uint8_t         pad[AES_MMO_BLOCK_SIZE];
} HMAC_AES_MMO_context_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public APIs implementation
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a AES MMO context structure
 *
 * \return    Address of the AES MMO context buffer
 *            Deallocate using AES_MMO_FreeCtx()
 *
 ********************************************************************************** */
void *AES_MMO_AllocCtx(void)
{
    void *aesMmoCtx = MEM_BufferAlloc(sizeof(aesMmoContext_t));

    return aesMmoCtx;
}

/*! *********************************************************************************
 * \brief  This function deallocates the memory buffer for the AES MMO context structure
 *
 * \param [in]    pContext    Address of the AES MMO context buffer
 *
 ********************************************************************************** */
void AES_MMO_FreeCtx(void *pContext)
{
    (void)MEM_BufferFree(pContext);
}

/*! *********************************************************************************
 * \brief  This function clones a AES_MMO context.
 *         Make sure the size of the allocated destination context buffer is appropriate.
 *
 * \param [in]    pDestCtx    Address of the destination AES_MMO context
 * \param [in]    pSourceCtx  Address of the source AES_MMO context
 *
 ********************************************************************************** */
void AES_MMO_CloneCtx(void *pDestCtx, void *pSourceCtx)
{
    FLib_MemCpy(pDestCtx, pSourceCtx, sizeof(aesMmoContext_t));
}

/*! *********************************************************************************
 * \brief  This function initializes the AES MMO context data
 *
 * \param [in]    pContext    Pointer to the AES MMO context data
 *                            Allocated using AES_MMO_AllocCtx()
 *
 ********************************************************************************** */
void AES_MMO_Init(void *pContext)
{
    aesMmoContext_t *context = (aesMmoContext_t *)pContext;

    context->bytes      = 0u;
    context->totalBytes = 0u;

    FLib_MemSet(context->digest.au8, 0u, sizeof(context->digest));
}

/****************************************************************************
 *
 * \brief  Perform an MMO Block Update on the hash
 *         H[j] = E(H[j-1], M[j]) ^ M[j]
 *         where E(K,x) = AES-128 block cipher, K=key, x=text
 *
 *  Uses the AES_128_Encrypt function from SecLib.c
 *
 * \param[in/out] puHash MMO output buffer
 * \param[in]     puBlock Block to hash
 *
 * \return none
 *
 * Note: This function works on 32 bit aligned input and output. The alignment has been
 * taken care of by the caller.
 *
 *
 ****************************************************************************/
void AES_MMO_BlockUpdate(tuAES_Block *puHash, tuAES_Block *puBlock)
{
    tuAES_Block uOut = {.au32 = {0uL}};

    /* Block cipher using Hash as key */
    AES_128_Encrypt((uint8_t *)puBlock, (uint8_t *)puHash, (uint8_t *)&uOut);

    /* Prepare next hash as (result XOR block) */
    for (uint8_t i = 0u; i < AES_MMO_BLOCK_SIZE / 4u; i++)
    {
        uOut.au32[i] ^= puBlock->au32[i];
        puHash->au32[i] = uOut.au32[i];
    }
}

/*! *********************************************************************************
 * \brief  This function performs AES MMO on multiple bytes and updates the context data
 *
 * \param [in]    pContext    Pointer to the AES MMO context data
 *                            Allocated using AES_MMO_AllocCtx()
 * \param [in]    pData       Pointer to the input data (arbitrary alignment)
 * \param [in]    numBytes    Number of bytes to hash
 *
 ********************************************************************************** */
void AES_MMO_HashUpdate(void *pContext, const uint8_t *pData, uint32_t numBytes)
{
    secResultType_t  ret;
    aesMmoContext_t *context = (aesMmoContext_t *)pContext;

    do
    {
        uint8_t free_space_in_context_buffer;

        if (context->bytes >= AES_MMO_BLOCK_SIZE)
        {
            /* impossible whenever accumulated buffer reach a block size the block is consumed right away */
            ret = gSecError_c;
            break;
        }
        /* Ensure that less than UINT32_MAX / 8 bytes have been hashed so far (this guarantee that the sum with numBytes
         * will not overflow)*/
        if (context->totalBytes >= AES_MMO_MAX_HASHED_BYTES)
        {
            /* we never have to Hash so large data : context may be corrupted */
            ret = gSecError_c;
            break;
        }
        if (numBytes > AES_MMO_MAX_HASHED_BYTES)
        {
            /* we never have to Hash so large data : caller error */
            ret = gSecBadArgument_c;
            break;
        }

        /* update total byte count */
        context->totalBytes += numBytes;
        /* Must check that numBytes do not cause overflow of bit counter */
        if (context->totalBytes >= AES_MMO_MAX_HASHED_BYTES)
        {
            /* we never have to Hash so large data : context may be corrupted */
            ret = gSecBadArgument_c;
            break;
        }

        free_space_in_context_buffer = (uint8_t)AES_MMO_BLOCK_SIZE - context->bytes;
        ret                          = gSecSuccess_c;
        /* Check if we have at least 1 AES_MMO block */
        if (numBytes < free_space_in_context_buffer)
        {
            /* store bytes for later processing */
            FLib_MemCpy(&context->buffer.au8[context->bytes], pData, numBytes);
            context->bytes += (uint8_t)(numBytes & 0xffu);
        }
        else
        {
            /* Check for bytes leftover from previous update */
            if (context->bytes > 0u)
            {
                uint8_t copyBytes = free_space_in_context_buffer;

                FLib_MemCpy(&context->buffer.au8[context->bytes], pData, copyBytes);
                AES_MMO_BlockUpdate(&context->digest, &context->buffer);
                pData += copyBytes;
                numBytes -= copyBytes;
                context->bytes = 0u;
            }
            /* Hash 64 bytes blocks */
            while (numBytes >= AES_MMO_BLOCK_SIZE)
            {
                FLib_MemCpy(&context->buffer.au8, pData, AES_MMO_BLOCK_SIZE);
                AES_MMO_BlockUpdate(&context->digest, &context->buffer);
                pData += AES_MMO_BLOCK_SIZE;
                numBytes -= AES_MMO_BLOCK_SIZE;
            }
            /* Check for remaining bytes */
            if (numBytes > 0u)
            {
                context->bytes = (uint8_t)(numBytes & 0xffu);
                /* Coverity False positive : free_space_in_context_buffer can only but remain less than 16 so numBytes
                 * cannot underflow */
                /* coverity [ overflow_sink:FALSE] */
                FLib_MemCpy(context->buffer.au8, pData, numBytes);
            }
        }
    } while (false);
    NOT_USED(ret);
}

/*! *********************************************************************************
 * \brief  This function finalizes the AES_MMO hash computation and clears the context data.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pContext    Pointer to the AES_MMO context data
 *                               Allocated using AES_MMO_AllocCtx()
 * \param [out]      pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void AES_MMO_HashFinish(void *pContext, uint8_t *pOutput)
{
    aesMmoContext_t *context = (aesMmoContext_t *)pContext;
    uint32_t         numBytes;
    uint8_t         *pu8Buf;
    uint32_t         uPad;
    uint32_t         uCount;
    uint32_t         uAdjust;
    uint32_t         uLen, uDataLen;
    uint32_t         u32DataLen;

    /* we have already ascertained so far that context->totalBytes was less than AES_MMO_MAX_HASHED_BYTES */
    uDataLen = context->totalBytes;
    /* update remaining bytes */
    numBytes = context->bytes;
    /* At this point, a remainder of at most 15 bytes is pending in the context buffer */
    pu8Buf    = &context->buffer.au8[numBytes];
    *pu8Buf++ = 0x80u;
    uLen      = numBytes;

    /*
     * Now we can start to put padding in.
     * New MMO (>= 8192 bytes): Need space for 6 octets for bitlength field (32 bits plus extra 0x0000)
     * Original MMO (< 8192 bytes): Need space for 2 octets for bitlength field (16 bits)
     * Calculate adjustment needed based on bitlength field space requirements
     */
    uAdjust = (uDataLen >= 8192u) ? (AES_MMO_BLOCK_SIZE - 6u) : (AES_MMO_BLOCK_SIZE - 2u);

    if (uAdjust < (1u + uLen))
    {
        /* Can't finish off on this block - pad the rest if any, transform and move on */
        uCount = AES_MMO_BLOCK_SIZE - (uLen + 1u);
        while (uCount-- > 0u)
        {
            *pu8Buf++ = 0u;
        }

        AES_MMO_BlockUpdate(&context->digest, &context->buffer);

        /* Reset padding and buffer pointer for final block */
        pu8Buf = &context->buffer.au8[0];
        uPad   = uAdjust;
    }
    else
    {
        uPad = uAdjust - (uLen + 1u);
    }
    /* Finish off block with length */
    if (uPad > 0u)
    {
        FLib_MemSet(pu8Buf, 0u, uPad);
        pu8Buf += uPad;
    }
    u32DataLen = (uint32_t)uDataLen << 3; /* in bits so x8 */
    if (uAdjust == (AES_MMO_BLOCK_SIZE - 6u))
    {
        *pu8Buf++ = (uint8_t)((u32DataLen >> 24) & 0xffu);
        *pu8Buf++ = (uint8_t)((u32DataLen >> 16) & 0xffu);
    }
    *pu8Buf++ = (uint8_t)((u32DataLen >> 8) & 0xffu);
    *pu8Buf++ = (uint8_t)(u32DataLen & 0xffu);
    if (uAdjust == (AES_MMO_BLOCK_SIZE - 6u))
    {
        *pu8Buf++ = 0u;
        *pu8Buf++ = 0u;
    }

    AES_MMO_BlockUpdate(&context->digest, &context->buffer);

    /* Copy the generated hash to the indicated output location */
    FLib_MemCpy(pOutput, context->digest.au8, AES_MMO_HASH_SIZE);
}

/*! *********************************************************************************
 * \brief  This function performs all AES_MMO steps on multiple bytes: initialize,
 *         update and finish.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pData       Pointer to the input data
 * \param [in]       numBytes    Number of bytes to hash
 * \param [out]      pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void AES_MMO_Hash(const uint8_t *pData, uint32_t numBytes, uint8_t *pOutput)
{
    aesMmoContext_t context;

    AES_MMO_Init(&context);
    AES_MMO_HashUpdate(&context, pData, numBytes);
    AES_MMO_HashFinish(&context, pOutput);
}

/* H[0] = 0; H[j] = E(H[j-1], M[j]) ^ M[j] for j=1..t */
/* E(K,x) = AES-128 block cipher, K=key, x=text */

/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a HMAC AES_MMO context structure
 *
 * \return    Address of the HMAC AES_MMO context buffer
 *            Deallocate using HMAC_AES_MMO_FreeCtx()
 *
 ********************************************************************************** */
void *HMAC_AES_MMO_AllocCtx(void)
{
    void *hmacCtx = MEM_BufferAlloc(sizeof(HMAC_AES_MMO_context_t));

    return hmacCtx;
}

/*! *********************************************************************************
 * \brief  This function deallocates the memory buffer for the HMAC AES_MMO context structure
 *
 * \param [in]    pContext    Address of the HMAC AES_MMO context buffer
 *
 ********************************************************************************** */
void HMAC_AES_MMO_FreeCtx(void *pContext)
{
    (void)MEM_BufferFree(pContext);
}

/*! *********************************************************************************
 * \brief  This function performs the initialization of the HMAC AES_MMO context data
 *
 * \param [in]    pContext    Pointer to the HMAC AES_MMO context data
 *                            Allocated using HMAC_AES_MMO_AllocCtx()
 * \param [in]    pKey        Pointer to the HMAC key
 * \param [in]    keyLen      Length of the HMAC key in bytes
 *
 ********************************************************************************** */
void HMAC_AES_MMO_Init(void *pContext, const uint8_t *pKey, uint16_t keyLen)
{
    uint8_t                 i;
    HMAC_AES_MMO_context_t *context;
    uint8_t                 HashKeyBuffer[AES_MMO_HASH_SIZE];

    context = (HMAC_AES_MMO_context_t *)pContext;
    FLib_MemSet(HashKeyBuffer, 0u, AES_MMO_HASH_SIZE);

    /* If key is longer than block size hash it */
    if (keyLen > AES_MMO_BLOCK_SIZE)
    {
        AES_MMO_Hash(pKey, keyLen, HashKeyBuffer);
        pKey   = HashKeyBuffer;
        keyLen = AES_MMO_HASH_SIZE;
    }

    /* Create i_pad */
    for (i = 0u; i < keyLen; i++)
    {
        context->pad[i] = pKey[i] ^ gHmacIpad_c;
    }

    for (i = (uint8_t)(keyLen & 0xffu); i < AES_MMO_BLOCK_SIZE; i++)
    {
        context->pad[i] = gHmacIpad_c;
    }
    /* start hashing of the i_key_pad */
    AES_MMO_Init(&context->hashCtx);
    AES_MMO_HashUpdate(&context->hashCtx, context->pad, AES_MMO_BLOCK_SIZE);

    /* create o_pad by xor-ing pad[i] with 0x36 ^ 0x5C: */
    for (i = 0u; i < AES_MMO_BLOCK_SIZE; i++)
    {
        context->pad[i] ^= (gHmacIpad_c ^ gHmacOpad_c);
    }
}

/*! *********************************************************************************
 * \brief  This function performs HMAC update with the input data.
 *
 * It is to be called as new data get appended to the message covered by the MAC.
 *
 * \param [in]    pContext    Pointer to the HMAC AES_MMO context data
 *                            Allocated using HMAC_AES_MMO_AllocCtx()
 * \param [in]    pData       Pointer to the input data
 * \param [in]    numBytes    Number of bytes to hash
 *
 ********************************************************************************** */
void HMAC_AES_MMO_Update(void *pContext, const uint8_t *pData, uint32_t numBytes)
{
    HMAC_AES_MMO_context_t *context = (HMAC_AES_MMO_context_t *)pContext;

    AES_MMO_HashUpdate(&context->hashCtx, pData, numBytes);
}

/*! *********************************************************************************
 * \brief  This function finalizes the HMAC AES_MMO computation and clears the context data.
 *         The final MAC value is stored at the provided output location.
 *
 * \param [in]       pContext    Pointer to the HMAC AES_MMO context data
 *                               Allocated using HMAC_AES_MMO_AllocCtx()
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void HMAC_AES_MMO_Finish(void *pContext, uint8_t *pOutput)
{
    uint8_t                 hash1[AES_MMO_HASH_SIZE];
    HMAC_AES_MMO_context_t *context = (HMAC_AES_MMO_context_t *)pContext;

    /* finalize the hash of the i_key_pad and message */
    AES_MMO_HashFinish(&context->hashCtx, hash1);
    /* perform hash of the o_key_pad and hash1 */
    AES_MMO_Init(&context->hashCtx);
    AES_MMO_HashUpdate(&context->hashCtx, context->pad, AES_MMO_BLOCK_SIZE);
    AES_MMO_HashUpdate(&context->hashCtx, hash1, AES_MMO_HASH_SIZE);
    AES_MMO_HashFinish(&context->hashCtx, pOutput);
}

/*! *********************************************************************************
 * \brief  This function performs all HMAC AES_MMO steps on multiple bytes: initialize,
 *         update, finish, and update context data.
 *         The final HMAC value is stored at the provided output location.
 *
 * \param [in]       pKey        Pointer to the HMAC key
 * \param [in]       keyLen      Length of the HMAC key in bytes
 * \param [in]       pData       Pointer to the input data
 * \param [in]       numBytes    Number of bytes to perform HMAC on
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void HMAC_AES_MMO(const uint8_t *pKey, uint16_t keyLen, const uint8_t *pData, uint32_t numBytes, uint8_t *pOutput)
{
    HMAC_AES_MMO_context_t context;

    HMAC_AES_MMO_Init(&context, pKey, keyLen);
    HMAC_AES_MMO_Update(&context, pData, numBytes);
    HMAC_AES_MMO_Finish(&context, pOutput);
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */