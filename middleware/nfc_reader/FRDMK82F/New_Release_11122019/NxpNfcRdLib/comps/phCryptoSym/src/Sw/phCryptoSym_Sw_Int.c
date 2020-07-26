/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Internal functions for SW functionality of the Symmetric Cryptography component.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

#ifdef NXPBUILD__PH_CRYPTOSYM_SW

#include "phCryptoSym_Sw.h"
#ifdef PH_CRYPTOSYM_SW_AES
#include "phCryptoSym_Sw_Aes.h"
#endif
#ifdef PH_CRYPTOSYM_SW_DES
#include "phCryptoSym_Sw_Des.h"
#endif
#include "phCryptoSym_Sw_Int.h"

phStatus_t phCryptoSym_Sw_CMAC_GenerateK1K2(
    phCryptoSym_Sw_DataParams_t * pDataParams,
    uint8_t * pSubKey1,
    uint8_t * pSubKey2
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_CRYTOSYM_SW_FAST_RAM aBuffer[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];
    uint16_t    PH_MEMLOC_REM wBlockSize;
    uint8_t     PH_MEMLOC_REM bR_b;

    (void)memset(aBuffer, 0x00, (size_t)sizeof(aBuffer));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_GetConfig(pDataParams, PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, &wBlockSize));

    /* Calculate xor value according to Seq. 5.3 of SP_800-38B */
    /* R128 = 0exp(120) || 10000111, and R64 = 0exp(59) || 11011. */
    switch (wBlockSize)
    {
    case PH_CRYPTOSYM_AES_BLOCK_SIZE:
        bR_b = 0x87U;
        break;
    case PH_CRYPTOSYM_DES_BLOCK_SIZE:
        bR_b = 0x1BU;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_CRYPTOSYM);
    }

    /* Encrypt zero block*/
    /* 1. Let L = CIPHK(0 exp b). */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_EncryptBlock(pDataParams, aBuffer));

    /* Shift the pSubKey array according to NIST SP_800-38B */
    /* 2. If MSB1(L) = 0, then K1 = L << 1U; */
    /* Else K1 = (L << 1U) xor Rb; see Sec. 5.3 for the definition of Rb. */
    phCryptoSym_Sw_CMAC_LeftShift(aBuffer, (uint8_t)wBlockSize, pSubKey1);
    if (0U != (aBuffer[0] & 0x80U))
    {
        /* We need to perform the XOR operation with the R_b array */
        pSubKey1[wBlockSize - 1U ]  ^= bR_b;
    }
    else
    {
        /* We are done with key1 generation */
    }

    /* Now let's continue with Key 2 */
    /* Shift the pSubKey array according to NIST SP_800-38B*/
    /* 3. If MSB1(K1) = 0, then K2 = K1 << 1U; */
    /* Else K2 = (K1 << 1U) xor Rb. */
    phCryptoSym_Sw_CMAC_LeftShift(pSubKey1, (uint8_t)wBlockSize, pSubKey2);

    if (0U != (pSubKey1[0] & 0x80U))
    {
        /* We need to perform the XOR operation with the R_b array */
        pSubKey2[wBlockSize - 1U ]  ^= bR_b;
    }
    else
    {
        /* We are done with key2 generation */
    }

#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    pDataParams->bCMACSubKeysInitialized    = PH_ON;
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */

    /* Clear buffer for security reasons */
    (void)memset(aBuffer, 0x00, (size_t)sizeof(aBuffer));

    return PH_ERR_SUCCESS;
}

void phCryptoSym_Sw_CMAC_LeftShift(
                                   const uint8_t * pInBuffer,
                                   uint8_t bInputLen,
                                   uint8_t * pOutBuffer
                                   )
{
    uint8_t PH_MEMLOC_REM bOverflow = 0;

    do
    {
        bInputLen--;
        pOutBuffer[bInputLen] = pInBuffer[bInputLen] << 1U;
        pOutBuffer[bInputLen] |= bOverflow;
        bOverflow = (uint8_t) (((pInBuffer[bInputLen] & 0x80U) != 0U) ? 0x01U : 0x00U);
    }
    while(0U != bInputLen);
}

phStatus_t phCryptoSym_Sw_EncryptBlock(
                                       phCryptoSym_Sw_DataParams_t * pDataParams,
                                       uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock
                                       )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    switch (pDataParams->wKeyType)
    {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_EncryptBlock(
            pDataParams,
            pBlock,
            PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_128));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_EncryptBlock(
            pDataParams,
            pBlock,
            PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_192));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_EncryptBlock(
            pDataParams,
            pBlock,
            PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_256));
        break;
#endif /* PH_CRYPTOSYM_SW_AES */
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            0));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            0));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            1));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            0));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            0));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            1));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            2));
        break;
#endif /* PH_CRYPTOSYM_SW_DES */
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phCryptoSym_Sw_DecryptBlock(
                                       phCryptoSym_Sw_DataParams_t * pDataParams,
                                       uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock
                                       )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    switch (pDataParams->wKeyType)
    {
#ifdef PH_CRYPTOSYM_SW_AES
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_DecryptBlock(
            pDataParams,
            pBlock,
            PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_128));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_DecryptBlock(
            pDataParams,
            pBlock,
            PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_192));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Aes_DecryptBlock(
            pDataParams,
            pBlock,
            PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_256));
        break;
#endif /* PH_CRYPTOSYM_SW_AES */
#ifdef PH_CRYPTOSYM_SW_DES
    case PH_CRYPTOSYM_KEY_TYPE_DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            0));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            0));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            1));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            0));
        break;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            2));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_EncryptBlock(
            pDataParams,
            pBlock,
            1));
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Des_DecryptBlock(
            pDataParams,
            pBlock,
            0));
        break;
#endif /* PH_CRYPTOSYM_SW_DES */
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phCryptoSym_Sw_Diversify_CMAC(
    phCryptoSym_Sw_DataParams_t * pDataParams,
    const uint8_t * pData,
    uint16_t  wDataLength,
    uint8_t * pMac,
    uint8_t * pMacLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wBlockSize;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint8_t     PH_MEMLOC_REM aTmpData[PH_CRYPTOSYM_AES_BLOCK_SIZE * 2U];
#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    uint8_t * pSubKey1 = pDataParams->pCMACSubKey1;
    uint8_t * pSubKey2 = pDataParams->pCMACSubKey2;
#else
    uint8_t pSubKey1[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];
    uint8_t pSubKey2[PH_CRYPTOSYM_SW_MAX_BLOCK_SIZE];

    (void)memset(pSubKey1, 0x00, (size_t)sizeof(pSubKey1));
    (void)memset(pSubKey2, 0x00, (size_t)sizeof(pSubKey2));
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */

    /* Clear MAC length */
    *pMacLength = 0;

    /* Get the block size of the currently loaded key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_GetConfig(pDataParams, PH_CRYPTOSYM_CONFIG_BLOCK_SIZE, &wBlockSize));

    /* Check input length */
    if (wDataLength > (wBlockSize << 1U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_LENGTH_ERROR, PH_COMP_CRYPTOSYM);
    }

    /* Clear IV */
    (void)memset(pDataParams->pIV, 0x00, wBlockSize);

    /* Now we may start with  MAC calculation */

#ifndef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    /* Do we need to compute the sub keys? */
    if (pDataParams->bCMACSubKeysInitialized == 0x00U)
    {
        /* start with sub key generation */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_CMAC_GenerateK1K2(pDataParams, pSubKey1, pSubKey2));
    }
#else
    /* Always perform with sub key generation */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_CMAC_GenerateK1K2(pDataParams, pSubKey1, pSubKey2));
#endif /* PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION */

    /* Copy data to temporary buffer */
    (void)memcpy(aTmpData, pData, wDataLength);

    /* Two full blocks -> NO PADDING, K1 */
    if (wDataLength == (wBlockSize << 1U))
    {
        /* XOR with K1 */
        for (wIndex = 0; wIndex < wBlockSize; ++wIndex)
        {
            aTmpData[wBlockSize + wIndex] ^= pSubKey1[wIndex];
        }
    }
    /* Otherwise APPLY PADDING, K2 */
    else
    {
        /* Apply padding */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
            PH_CRYPTOSYM_PADDING_MODE_2,
            aTmpData,
            wDataLength,
            (uint8_t)(wBlockSize << 1U),
            (uint16_t)(sizeof(aTmpData)),
            aTmpData,
            &wDataLength));

        /* XOR with K2 */
        for (wIndex = 0; wIndex < wBlockSize; ++wIndex)
        {
            aTmpData[wBlockSize + wIndex] ^= pSubKey2[wIndex];
        }
    }

    /* Perform encryption */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Sw_Encrypt(
        pDataParams,
        PH_CRYPTOSYM_CIPHER_MODE_CBC,
        aTmpData,
        (PH_CRYPTOSYM_AES_BLOCK_SIZE * 2U),
        aTmpData));

    /* Return MAC */
    (void)memcpy(pMac, &aTmpData[wBlockSize], wBlockSize);
    *pMacLength = (uint8_t)wBlockSize;

    /* Clear the IV for security reasons */
    (void)memset(pDataParams->pIV, 0, wBlockSize);

#ifdef PH_CRYPTOSYM_SW_ONLINE_CMAC_SUBKEY_CALCULATION
    /* Clear key arrays */
    (void)memset(pSubKey1, 0x00, (size_t)sizeof(pSubKey1));
    (void)memset(pSubKey2, 0x00, (size_t)sizeof(pSubKey2));
#endif

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PH_CRYPTOSYM_SW */
