/*
*                     Copyright 2010-2019, NXP
*
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
* Software MIFARE DESFire EV1 contactless IC Application Component of Reader
* Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <ph_TypeDefs.h>
#include <string.h>
#include <phTools.h>
#include <phKeyStore.h>

#ifdef NXPBUILD__PH_CRYPTOSYM
#include <phCryptoSym.h>
#endif /* NXPBUILD__PH_CRYPTOSYM */
#ifdef NXPBUILD__PH_CRYPTORNG
#include <phCryptoRng.h>
#endif /* NXPBUILD__PH_CRYPTORNG */

#ifdef NXPBUILD__PHAL_MFDF_SW

#include "../phalMfdf_Int.h"
#include "phalMfdf_Sw.h"
#include "phalMfdf_Sw_Int.h"

phStatus_t phalMfdf_Sw_Init(
                            phalMfdf_Sw_DataParams_t * pDataParams,
                            uint16_t wSizeOfDataParams,
                            void * pPalMifareDataParams,
                            void * pKeyStoreDataParams,
                            void * pCryptoDataParamsEnc,
                            void * pCryptoRngDataParams,
                            void * pHalDataParams
                            )
{
    /* data param check */
    if (sizeof(phalMfdf_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalMifareDataParams);


    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFDF | PHAL_MFDF_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;
    pDataParams->pCryptoDataParamsEnc   = pCryptoDataParamsEnc;
    pDataParams->pCryptoRngDataParams   = pCryptoRngDataParams;
    pDataParams->bLastBlockIndex        = 0;
    pDataParams->pHalDataParams  = pHalDataParams;
    /* 2 Byte CRC initial value in Authenticate mode. */
    pDataParams->wCrc = PH_TOOLS_CRC16_PRESET_ISO14443A;

    /* 4 Byte CRC initial value in 0x1A, 0xAA mode. */
    pDataParams->dwCrc = PH_TOOLS_CRC32_PRESET_DF8;

    (void)memset(pDataParams->bSessionKey, 0x00, 24);
    pDataParams->bKeyNo = 0xFF; /* Set to invalid */
    (void)memset(pDataParams->bIv, 0x00, 16);
    (void)memset(pDataParams->pAid, 0x00, 3);
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED; /* Set to invalid */
    pDataParams->bWrappedMode = 0x00; /* Set to FALSE */
    pDataParams->bCryptoMethod = 0xFF; /* No crypto just after init */
    pDataParams->wAdditionalInfo = 0x0000;
    pDataParams->wPayLoadLen = 0;

    return PH_ERR_SUCCESS;
}


/**
* PICC level commands
*/
phStatus_t phalMfdf_Sw_CreateApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pAid,
    uint8_t bKeySettings1,
    uint8_t bKeySettings2,
    uint8_t * pISOFileId,
    uint8_t * pISODFName,
    uint8_t bISODFNameLen
    )
{
    /*
    If (0U != (bKeySettings2 & 0x03U))== 00 [DES, 3DES] then pDataParams->bAuthMode can be either
    0x0A or 0x1A.
    If (0U != (bKeySettings2 & 0x03U))== 01 [3K3DES] then pDataParams->bAuthMode can only be 0x1A.
    If (0U != (bKeySettings2 & 0x03U))== 10 [AES] then pDataParams->bAuthMode can only be 0xAA.
    */
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bISODFNameLen > 16U) || (bOption > 0x03U) || (bOption == 0x02U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_APPLN;

    (void)memcpy(&bCmdBuff[wCmdLen], pAid, 3);
    wCmdLen += 3U;

    bCmdBuff[wCmdLen++] = bKeySettings1;
    bCmdBuff[wCmdLen++] = bKeySettings2;

    if (0U != (bOption & 0x01U))
    {
        /* wISOFileId is present */
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }
    if (0U != (bOption & 0x02U))
    {
        /* pISODFName is present */
        (void)memcpy(&bCmdBuff[wCmdLen], pISODFName, bISODFNameLen);
        wCmdLen = wCmdLen + bISODFNameLen;
    }

    return phalMfdf_Sw_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}


phStatus_t phalMfdf_Sw_SelectApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pAppId
    )
{
    uint16_t  PH_MEMLOC_REM statusTmp;
    uint8_t   PH_MEMLOC_REM bCmdBuff[10];
    uint16_t  PH_MEMLOC_REM wRxlen;
    uint16_t  PH_MEMLOC_REM wCmdLen = 0;
    uint8_t * PH_MEMLOC_REM pRecv = NULL;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_SELECT_APPLN;
    (void)memcpy(&bCmdBuff[1], pAppId, PHAL_MFDF_DFAPPID_SIZE);
    wCmdLen = PHAL_MFDF_DFAPPID_SIZE + 1U;


    /* Send the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    /* Store the currently selected application Id */
    (void)memcpy(pDataParams->pAid, pAppId, 3);

    /* Select command never returns CMAC */
    return PH_ERR_SUCCESS;
}


phStatus_t phalMfdf_Sw_GetVersion(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t * pResponse
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_VERSION;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Sw_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    if(wRxlen > 0U)
    {
        (void)memcpy(pResponse, pRecv, (size_t)wRxlen);
    }

    if (wRxlen != 28U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    return PH_ERR_SUCCESS;
}


phStatus_t phalMfdf_Sw_CreateStdDataFile(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    )
{
    /*
    If (0U != (bKeySettings2 & 0x03U))== 00 [DES, 3DES] then pDataParams->bAuthMode can be either
    0x0A or 0x1A.
    If (0U != (bKeySettings2 & 0x03U))== 01 [3K3DES] then pDataParams->bAuthMode can only be 0x1A.
    If (0U != (bKeySettings2 & 0x03U))== 10 [AES] then pDataParams->bAuthMode can only be 0xAA.
    */
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1fU) || (bOption > 0x01U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }


#if !defined (NXPBUILD__PH_NDA_MFDF)

    if (bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

#endif /* NXPBUILD__PH_NDA_MFDF */

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_STD_DATAFILE;

    /* File Number */
    bCmdBuff[wCmdLen++] = bFileNo;

    /* Copy ISO Filed ID if present */
    if (bOption == 0x01U)
    {
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }

    /* Copy communication settings. communication settings in the first nibble so right shifting */
    bCmdBuff[wCmdLen++] = bCommSett >> 4U;

    /* Copy Access rights */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    /* Copy File size supplied by the user */
    (void)memcpy(&bCmdBuff[wCmdLen], pFileSize, 3);
    wCmdLen += 3U;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_Sw_Int_Write_Plain(pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}


phStatus_t phalMfdf_Sw_WriteData(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint8_t bCommOption,
                                 uint8_t bFileNo,
                                 uint8_t * pOffset,
                                 uint8_t * pData,
                                 uint8_t * pDataLen
                                 )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wDataLen = 0;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

#if !defined(NXPBUILD__PH_NDA_MFDF)

    PHAL_MFDF_UNUSED_VARIABLE(pData)
    PHAL_MFDF_UNUSED_VARIABLE(bCommOption)

#endif /* NXPBUILD__PH_NDA_MFDF */

    if (bFileNo > 0x1fU)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }


#if !defined (NXPBUILD__PH_NDA_MFDF)

    if (bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }

#endif /* NXPBUILD__PH_NDA_MFDF */

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_WRITE_DATA;
    bCmdBuff[wCmdLen++] = bFileNo;
    (void)memcpy(&bCmdBuff[wCmdLen], pOffset, 3);
    wCmdLen += 3U;
    (void)memcpy(&bCmdBuff[wCmdLen], pDataLen, 3);
    wCmdLen += 3U;

    /* Assuming here that the size can never go beyond FFFF.
    In fact it can never go beyond 8092 (1F9C) bytes */
    wDataLen = (uint16_t)pDataLen[1];
    wDataLen = wDataLen << 8U;
    wDataLen |= pDataLen[0];

    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDF_COMMUNICATION_PLAIN;
    }


#if !defined (NXPBUILD__PH_NDA_MFDF)

    return phalMfdf_Sw_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        bCommOption,
        pData,
        wDataLen
        );
#else
    /* If control reaches here, it's a case of mismatch between NDA/NON NDA flavour */
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);;
#endif /* NXPBUILD__PH_NDA_MFDF */
}


phStatus_t phalMfdf_Sw_IsoSelectFile(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t bOption,
                                     uint8_t bSelector,
                                     uint8_t * pFid,
                                     uint8_t * pDFname,
                                     uint8_t bDFnameLen,
                                     uint8_t **ppRecv,
                                     uint16_t * pwRxlen
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bData[24];
    uint8_t     PH_MEMLOC_REM bLc = 0;

    if (bDFnameLen > 16U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bOption != 0x00U) && (bOption != 0x0CU))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    switch(bSelector)
    {
    case 0x00:
    case 0x02:
        /* Selection by EF Id*/
        /* Send MSB first to card */
        bData[0] = pFid[1];
        bData[1] = pFid[0];
        bLc = 2;
        break;

    case 0x04:
        /* Selection by DF Name */
        (void)memcpy(bData, pDFname, bDFnameLen);
        bLc = bDFnameLen;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }


    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        (bOption == 0x0CU) ? 0x01U : 0x03U,             /* As per Table 40-P2 in ISO/IEC FDIS 7816-4 */
        PHAL_MFDF_CMD_ISO7816_SELECT_FILE,
        bSelector,
        bOption,
        bLc,
        bData,
        0x00,
        ppRecv,
        pwRxlen));

    /* ISO wrapped mode is on */
    pDataParams->bWrappedMode = 1;

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdf_Sw_IsoReadBinary(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t bOffset,
                                     uint8_t bSfid,
                                     uint8_t bBytesToRead,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pBytesRead
                                     )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLe = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (wOption == PH_EXCHANGE_DEFAULT)
    {
        if (0U != (bSfid & 0x80U))
        {
            /* Short file id is supplied */
            if ((bSfid & 0x7FU) > 0x1FU)
            {
                /* Error condition */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
            }
            bP1 = bSfid;
            bP2 = bOffset;
        }
        else
        {
            /* P1 and P2 code the offset */
            bP1 = bSfid;
            bP2 = bOffset;
        }
        bLe = bBytesToRead;
        bCmdBuff[wCmdLen++] = 0x00; /* Class */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_BINARY; /* Ins */
        bCmdBuff[wCmdLen++] = bP1;
        bCmdBuff[wCmdLen++] = bP2;
        bCmdBuff[wCmdLen++] = bLe; /* Le */
    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        wCmdLen = 0;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    return phalMfdf_Sw_Int_IsoRead(
        pDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );
}

phStatus_t phalMfdf_Sw_IsoUpdateBinary(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bOffset,
                                       uint8_t bSfid,
                                       uint8_t * pData,
                                       uint8_t bDataLen
                                       )

{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    phStatus_t  PH_MEMLOC_REM status;

    if (0U != (bSfid & 0x80U))
    {
        /* Short file id is supplied */
        if ((bSfid & 0x7FU) > 0x1FU)
        {
            /* Error condition */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        bP2 = bOffset;
    }
    else
    {
        /* P1 and P2 code the offset */
        bP1 = bSfid;
        bP2 = bOffset;
    }

    bLc = bDataLen;
    status = phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_UPDATE_BINARY,
        bP1,
        bP2,
        bLc,
        pData,
        0x00,
        NULL,
        NULL);


    return status;
}


phStatus_t phalMfdf_Sw_GetConfig(
                                 phalMfdf_Sw_DataParams_t *pDataParams,
                                 uint16_t wConfig,
                                 uint16_t *pValue
                                 )
{
    switch (wConfig)
    {
    case PHAL_MFDF_ADDITIONAL_INFO:
        *pValue = pDataParams->wAdditionalInfo;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }
    return PH_ERR_SUCCESS;
}
phStatus_t phalMfdf_Sw_SetConfig(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t wValue
                                 )
{
    switch (wConfig)
    {
    case PHAL_MFDF_ADDITIONAL_INFO:
        pDataParams->wAdditionalInfo = wValue;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }
    return PH_ERR_SUCCESS;
}


#endif /* NXPBUILD__PHAL_MFDF_SW */
