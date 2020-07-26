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
* Internal functions of Software implementation of MIFARE (R) Ultralight contactless IC application layer.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 25. November 2009
*
*/

#include <ph_Status.h>
#include <phpalMifare.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFUL

#include <phalMful.h>
#include "phalMful_Int.h"

phStatus_t phalMful_Int_Read(
                             void * pPalMifareDataParams,
                             uint8_t bAddress,
                             uint8_t * pData
                             )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_READ;
    bCommand[1] = bAddress;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* check received length */
    if (wRxLength != PHAL_MFUL_READ_BLOCK_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* copy received data block */
    (void)memcpy(pData, pRxBuffer, wRxLength);

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_Write(
                              void * pPalMifareDataParams,
                              uint8_t bAddress,
                              uint8_t * pData
                              )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_WRITE;
    bCommand[1] = bAddress;

    /* buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* transmit the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pData,
        PHAL_MFUL_WRITE_BLOCK_LENGTH,
        &pRxBuffer,
        &wRxLength
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_FastWrite(
                                 void * pPalMifareDataParams,
                                 uint8_t * pData
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[3];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_FAST_WRITE;
    bCommand[1] = 0xF0; /* Start Address */
    bCommand[2] = 0xFF; /* End Address */

    /* buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCommand,
        3,
        &pRxBuffer,
        &wRxLength
        ));

    /* transmit the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pData,
        (16u * PHAL_MFUL_WRITE_BLOCK_LENGTH),
        &pRxBuffer,
        &wRxLength
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_CompatibilityWrite(
    void * pPalMifareDataParams,
    uint8_t bAddress,
    uint8_t * pData
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_COMPWRITE;
    bCommand[1] = bAddress;

    /* send the first part */
    status = phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength);

    /* Either ACK (newer UL cards) or TO (older UL cards) is expected */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* buffer the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        pData,
        PHAL_MFUL_COMPWRITE_BLOCK_LENGTH,
        &pRxBuffer,
        &wRxLength));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_IncrCnt(
                                void * pPalMifareDataParams,
                                uint8_t bCntNum,
                                uint8_t * pCnt
                                )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_INCR_CNT;
    bCommand[1] = bCntNum;

    /* Buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* transmit the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pCnt,
        PHAL_MFUL_COUNTER_WR_VALUE_LENGTH,
        &pRxBuffer,
        &wRxLength
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_ReadCnt(
                                void * pPalMifareDataParams,
                                uint8_t bCntNum,
                                uint8_t * pCntValue
                                )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_READ_CNT;
    bCommand[1] = bCntNum;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* check received length */
    if (wRxLength != PHAL_MFUL_COUNTER_RD_VALUE_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* copy received data block */
    (void)memcpy(pCntValue, pRxBuffer, wRxLength);

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_PwdAuth(
                                void * pPalMifareDataParams,
                                uint8_t * pPwd,
                                uint8_t * pPack
                                )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand = PHAL_MFUL_CMD_PWD_AUTH;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        &bCommand,
        1,
        &pRxBuffer,
        &wRxLength
        ));

    /* transmit the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pPwd,
        PHAL_MFUL_WRITE_BLOCK_LENGTH,
        &pRxBuffer,
        &wRxLength
        ));

    /* check received length */
    if (wRxLength != PHAL_MFUL_PACK_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* copy received data block */
    (void)memcpy(pPack, pRxBuffer, wRxLength);

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_GetVersion(
                                   void * pPalMifareDataParams,
                                   uint8_t * pVersion
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand = PHAL_MFUL_CMD_GET_VER;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        &bCommand,
        1,
        &pRxBuffer,
        &wRxLength
        ));

    /* check received length */
    if (wRxLength != PHAL_MFUL_VERSION_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* copy received data block */
    (void)memcpy(pVersion, pRxBuffer, wRxLength);

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_FastRead(
                                 void * pPalMifareDataParams,
                                 uint8_t  bStartAddr,
                                 uint8_t bEndAddr,
                                 uint8_t ** pData,
                                 uint16_t * pNumBytes
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[3];
    uint16_t    PH_MEMLOC_REM wBytesRead;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_FAST_READ;
    bCommand[1] = bStartAddr;
    bCommand[2] = bEndAddr;
    wBytesRead = (uint16_t)((((uint16_t)bEndAddr) - ((uint16_t)bStartAddr) + 1U) * 4U);

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        3,
        pData,
        pNumBytes
        ));

    /* check received length */
    if (*pNumBytes != wBytesRead)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_SectorSelect(
                                     void * pPalMifareDataParams,
                                     uint8_t bSecNo
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand1[2];
    uint8_t     PH_MEMLOC_REM bCommand2[4];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame packet 1 */
    bCommand1[0] = PHAL_MFUL_CMD_SECTOR_SELECT;
    bCommand1[1] = 0xFF;

    /* transmit the command frame packet 1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand1,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* Build command frame packet 2 */
    bCommand2[0] = bSecNo;
    bCommand2[1] = 0x00;
    bCommand2[2] = 0x00;
    bCommand2[3] = 0x00;

    /* transmit the command frame packet 2 */
    statusTmp = phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand2,
        4,
        &pRxBuffer,
        &wRxLength
        );

    /* No response expected for packet 2 */
    if((statusTmp & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_ReadSign(
                                 void * pPalMifareDataParams,
                                 uint8_t bAddr,
                                 uint8_t ** pSignature
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_READ_SIG;
    bCommand[1] = bAddr;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        pSignature,
        &wRxLength
        ));


    /* check received length */
    if (wRxLength != PHAL_MFUL_SIG_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_ChkTearingEvent(
                                        void * pPalMifareDataParams,
                                        uint8_t bCntNum,
                                        uint8_t * pValidFlag
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_CHK_TRG_EVT;
    bCommand[1] = bCntNum;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));


    /* check received length */
    if (wRxLength != 1U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* copy received data block */
    (void)memcpy(pValidFlag, pRxBuffer, wRxLength);


    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_WriteSign(
                                  void * pPalMifareDataParams,
                                  uint8_t bAddress,
                                  uint8_t * pSignature
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_WRITE_SIGN;
    bCommand[1] = bAddress;

    /* buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* transmit the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pSignature,
        PHAL_MFUL_WRITE_BLOCK_LENGTH,
        &pRxBuffer,
        &wRxLength
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_LockSign(
                                 void * pPalMifareDataParams,
                                 uint8_t bLockMode
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCommand[0] = PHAL_MFUL_CMD_LOCK_SIGN;
    bCommand[1] = bLockMode;

    /* buffer the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMful_Int_VirtualCardSelect(
                                          void * pPalMifareDataParams,
                                          uint8_t * pVCIID,
                                          uint8_t bVCIIDLen,
                                          uint8_t * pVCTID
                                          )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[1];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /*Frame the command buffer. */
    aCommand[0] = PHAL_MFUL_CMD_VCSL;

    /* Transmit the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCommand,
        1,
        &pRxBuffer,
        &wRxLength
        ));

    /* Transmit the IID data. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pVCIID,
        bVCIIDLen,
        &pRxBuffer,
        &wRxLength
        ));

    /* check received length */
    if (wRxLength != 1U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFUL);
    }

    /* copy received data block */
    (void)memcpy(pVCTID, pRxBuffer, wRxLength);


    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_MFUL */
