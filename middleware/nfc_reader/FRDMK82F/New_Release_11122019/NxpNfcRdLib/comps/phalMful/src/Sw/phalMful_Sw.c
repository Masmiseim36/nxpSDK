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
* Software MIFARE(R) Ultralight contactless IC Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 05. October 2009
*
*/

#include <ph_Status.h>
#include <phalMful.h>
#include <phpalMifare.h>
#include <phKeyStore.h>
#ifdef NXPBUILD__PH_CRYPTOSYM
#include <phCryptoSym.h>
#endif /* NXPBUILD__PH_CRYPTOSYM */
#ifdef NXPBUILD__PH_CRYPTORNG
#include <phCryptoRng.h>
#endif /* NXPBUILD__PH_CRYPTORNG */
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFUL_SW

#include "phalMful_Sw.h"
#include "../phalMful_Int.h"

/*
* Private constants
*/
#ifdef NXPBUILD__PH_CRYPTOSYM
static const uint8_t PH_MEMLOC_CONST_ROM phalMful_Sw_FirstIv[PHAL_MFUL_DES_BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif /* NXPBUILD__PH_CRYPTOSYM */
phStatus_t phalMful_Sw_Init(
                            phalMful_Sw_DataParams_t * pDataParams,
                            uint16_t wSizeOfDataParams,
                            void * pPalMifareDataParams,
                            void * pKeyStoreDataParams,
                            void * pCryptoDataParams,
                            void * pCryptoRngDataParams
                            )
{
    if (sizeof(phalMful_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFUL);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFUL | PHAL_MFUL_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;
    pDataParams->pCryptoDataParams      = pCryptoDataParams;
    pDataParams->pCryptoRngDataParams   = pCryptoRngDataParams;

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Sw_UlcAuthenticate(
                                       phalMful_Sw_DataParams_t * pDataParams,
                                       uint16_t wKeyNumber,
                                       uint16_t wKeyVersion
                                       )
{
#ifdef NXPBUILD__PH_CRYPTOSYM
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFUL_DES_BLOCK_SIZE+1U];
    uint8_t     PH_MEMLOC_REM bRndA[PHAL_MFUL_DES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRndB[PHAL_MFUL_DES_BLOCK_SIZE+1U];
    uint8_t     PH_MEMLOC_REM bKey[PHAL_MFUL_DES_KEY_LENGTH];
    uint16_t    PH_MEMLOC_REM wKeyType = 0;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* Parameter check */
    if ((pDataParams->pKeyStoreDataParams == NULL) ||
        (pDataParams->pCryptoDataParams == NULL) ||
        (pDataParams->pCryptoRngDataParams == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_MFUL);
    }

    /* retrieve key from keystore */
    statusTmp = phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNumber,
        wKeyVersion,
        PHAL_MFUL_DES_KEY_LENGTH,
        bKey,
        &wKeyType);

    /* check return code and map to key error */
    if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFUL);
    }

    /* check for correct key type */
    if (wKeyType != PH_KEYSTORE_KEY_TYPE_2K3DES)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFUL);
    }

    /* build the authentication request */
    bFrame[0] = PHAL_MFUL_CMD_AUTH;
    bFrame[1] = 0x00;

    /* transmit the request */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bFrame,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* check the format of the received data */
    if ((wRxLength != PHAL_MFUL_DES_BLOCK_SIZE+1U) ||
        (pRxBuffer[0] != PHAL_MFUL_PREAMBLE_TX))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* load the key into the crypto */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParams,
        bKey,
        PH_CRYPTOSYM_KEY_TYPE_2K3DES));

    /* Load first IV*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParams,
        phalMful_Sw_FirstIv,
        PHAL_MFUL_DES_BLOCK_SIZE));

    /* decrypt RndB */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParams,
        PH_EXCHANGE_BUFFER_FIRST | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        &pRxBuffer[1],
        PHAL_MFUL_DES_BLOCK_SIZE,
        bRndB));

    /* generate RndA  */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(
        pDataParams->pCryptoRngDataParams,
        PHAL_MFUL_DES_BLOCK_SIZE,
        bRndA));

    /* buffer the preamble */
    bFrame[0] = PHAL_MFUL_PREAMBLE_TX;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bFrame,
        1,
        &pRxBuffer,
        &wRxLength));

    /* encrypt RndA */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParams,
        PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        bRndA,
        PHAL_MFUL_DES_BLOCK_SIZE,
        bFrame));

    /* buffer the first part of the cryptogram (Enc(RndA)) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bFrame,
        PHAL_MFUL_DES_BLOCK_SIZE,
        &pRxBuffer,
        &wRxLength));

    /* rotate RndB to the left to generate RndB' */
    bRndB[PHAL_MFUL_DES_BLOCK_SIZE] = bRndB[0];

    /* encrypt RndB' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParams,
        PH_EXCHANGE_BUFFER_CONT | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        &bRndB[1],
        PHAL_MFUL_DES_BLOCK_SIZE,
        bFrame));

    /* append the second part of the cryptogram (Enc(RndB')) and send the whole stream. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bFrame,
        PHAL_MFUL_DES_BLOCK_SIZE,
        &pRxBuffer,
        &wRxLength));

    /* check the format of the received data */
    if ((wRxLength != PHAL_MFUL_DES_BLOCK_SIZE+1U) ||
        (pRxBuffer[0] != PHAL_MFUL_PREAMBLE_RX))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* decrypt RndA' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParams,
        PH_EXCHANGE_BUFFER_LAST | PH_CRYPTOSYM_CIPHER_MODE_CBC,
        &pRxBuffer[1],
        PHAL_MFUL_DES_BLOCK_SIZE,
        &bFrame[1]));

    /* rotate RndA' to the right to retrieve RndA */
    bFrame[0] = bFrame[PHAL_MFUL_DES_BLOCK_SIZE];

    /* compare the received RndA with our RndA */
    if (memcmp(bRndA, bFrame, PHAL_MFUL_DES_BLOCK_SIZE) != 0)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFUL);
    }

    return PH_ERR_SUCCESS;
#endif /* NXPBUILD__PH_CRYPTOSYM */

#if !defined(NXPBUILD__PH_CRYPTOSYM)

    /* satisfy compiler */
    PH_UNUSED_VARIABLE (pDataParams);
    PH_UNUSED_VARIABLE( wKeyNumber);
    PH_UNUSED_VARIABLE( wKeyVersion);

    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFUL);

#endif /* NXPBUILD__PH_CRYPTOSYM */
}

phStatus_t phalMful_Sw_Read(
                            phalMful_Sw_DataParams_t * pDataParams,
                            uint8_t bAddress,
                            uint8_t * pData
                            )
{
    return phalMful_Int_Read(pDataParams->pPalMifareDataParams, bAddress, pData);
}

phStatus_t phalMful_Sw_Write(
                             phalMful_Sw_DataParams_t * pDataParams,
                             uint8_t bAddress,
                             uint8_t * pData
                             )
{
    return phalMful_Int_Write(pDataParams->pPalMifareDataParams, bAddress, pData);
}

phStatus_t phalMful_Sw_FastWrite(
                                 phalMful_Sw_DataParams_t * pDataParams,
                                 uint8_t * pData
                                 )
{
    return phalMful_Int_FastWrite(pDataParams->pPalMifareDataParams, pData);
}

phStatus_t phalMful_Sw_CompatibilityWrite(
    phalMful_Sw_DataParams_t * pDataParams,
    uint8_t bAddress,
    uint8_t * pData
    )
{
    return phalMful_Int_CompatibilityWrite(pDataParams->pPalMifareDataParams, bAddress, pData);
}

phStatus_t phalMful_Sw_IncrCnt(
                               phalMful_Sw_DataParams_t * pDataParams,
                               uint8_t bCntNum,
                               uint8_t * pCnt
                               )
{
    return phalMful_Int_IncrCnt(pDataParams->pPalMifareDataParams, bCntNum, pCnt);
}

phStatus_t phalMful_Sw_ReadCnt(
                               phalMful_Sw_DataParams_t * pDataParams,
                               uint8_t bCntNum,
                               uint8_t * pCntValue
                               )
{
    return phalMful_Int_ReadCnt(pDataParams->pPalMifareDataParams, bCntNum, pCntValue);
}

phStatus_t phalMful_Sw_PwdAuth(
                               phalMful_Sw_DataParams_t * pDataParams,
                               uint8_t * pPwd,
                               uint8_t * pPack
                               )
{
    return phalMful_Int_PwdAuth(pDataParams->pPalMifareDataParams, pPwd, pPack);
}

phStatus_t phalMful_Sw_GetVersion(
                                  phalMful_Sw_DataParams_t * pDataParams,
                                  uint8_t * pPwd
                                  )
{
    return phalMful_Int_GetVersion(pDataParams->pPalMifareDataParams, pPwd);
}

phStatus_t phalMful_Sw_FastRead(
                                phalMful_Sw_DataParams_t * pDataParams,
                                uint8_t  bStartAddr,
                                uint8_t bEndAddr,
                                uint8_t ** pData,
                                uint16_t * pNumBytes
                                )
{
    return phalMful_Int_FastRead(pDataParams->pPalMifareDataParams, bStartAddr,
        bEndAddr, pData, pNumBytes);
}

phStatus_t phalMful_Sw_SectorSelect(
                                    phalMful_Sw_DataParams_t * pDataParams,
                                    uint8_t bSecNo
                                    )
{
    return phalMful_Int_SectorSelect(pDataParams->pPalMifareDataParams, bSecNo);
}

phStatus_t phalMful_Sw_ReadSign(
                                phalMful_Sw_DataParams_t * pDataParams,
                                uint8_t bAddr,
                                uint8_t ** pSignature
                                )
{
    return phalMful_Int_ReadSign(pDataParams->pPalMifareDataParams, bAddr, pSignature);
}

phStatus_t phalMful_Sw_ChkTearingEvent(
                                       phalMful_Sw_DataParams_t * pDataParams,
                                       uint8_t bCntNum,
                                       uint8_t * pValidFlag
                                       )
{
    return phalMful_Int_ChkTearingEvent(pDataParams->pPalMifareDataParams, bCntNum, pValidFlag);
}

phStatus_t phalMful_Sw_WriteSign(
                                 phalMful_Sw_DataParams_t * pDataParams,
                                 uint8_t bAddress,
                                 uint8_t * pSignature
                                 )
{
    return phalMful_Int_WriteSign(pDataParams->pPalMifareDataParams, bAddress, pSignature);
}

phStatus_t phalMful_Sw_LockSign(
                                phalMful_Sw_DataParams_t * pDataParams,
                                uint8_t bLockMode
                                )
{
    return phalMful_Int_LockSign(pDataParams->pPalMifareDataParams, bLockMode);
}

phStatus_t phalMful_Sw_VirtualCardSelect(
                                         phalMful_Sw_DataParams_t * pDataParams,
                                         uint8_t * pVCIID,
                                         uint8_t bVCIIDLen,
                                         uint8_t * pVCTID
                                         )
{
    return phalMful_Int_VirtualCardSelect (pDataParams->pPalMifareDataParams, pVCIID, bVCIIDLen, pVCTID);
}

#endif /* NXPBUILD__PHAL_MFUL_SW */
