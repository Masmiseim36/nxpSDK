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
* Software ISO18000-3 Mode3 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 1. February 2010
*
*/

#include <ph_Status.h>
#include <phpalI18000p3m3.h>
#include <phalI18000p3m3.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_I18000P3M3_SW

#include "phalI18000p3m3_Sw.h"
#include "phalI18000p3m3_Sw_Int.h"

phStatus_t phalI18000p3m3_Sw_Init(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  void * pPalI18000p3m3DataParams
                                  )
{
    if (sizeof(phalI18000p3m3_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalI18000p3m3DataParams);

    pDataParams->wId                        = PH_COMP_AL_I18000P3M3 | PHAL_I18000P3M3_SW_ID;
    pDataParams->pPalI18000p3m3DataParams   = pPalI18000p3m3DataParams;
    pDataParams->bHandleValid               = PH_OFF;
    pDataParams->abHandle[0]                = PH_OFF;
    pDataParams->abHandle[1]                = PH_OFF;

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_Ack(
                                 phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                 uint8_t ** ppRxBuffer,
                                 uint16_t * pRxBitLength
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Use Stored Handle for ACK command */
    if (pDataParams->bHandleValid != PH_OFF)
    {
        status = phpalI18000p3m3_Ack(
            pDataParams->pPalI18000p3m3DataParams,
            PHPAL_I18000P3M3_ACK_USE_HANDLE,
            pDataParams->abHandle,
            ppRxBuffer,
            pRxBitLength);
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
        /* Use Stored CRC for ACK command */
        status = phpalI18000p3m3_Ack(
            pDataParams->pPalI18000p3m3DataParams,
            PHPAL_I18000P3M3_ACK_USE_CRC,
            NULL,
            ppRxBuffer,
            pRxBitLength);

        /* Map component code for ERR_USE_CONDITION */
        if ((status & PH_ERR_MASK) == PH_ERR_USE_CONDITION)
        {
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
        }
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

#if !defined(NXPBUILD__PHPAL_I18000P3M3_DEPRECATED)
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */
    }

    return status;
}

phStatus_t phalI18000p3m3_Sw_ReqRn(
                                   phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                   uint8_t bOption,
                                   uint8_t ** ppRxBuffer
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bCmd[1];
    uint16_t    PH_MEMLOC_REM wRxByteLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Parameter check */
    if (bOption != PHAL_I18000P3M3_REQRN_USE_HANDLE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
    /* Handling for StoredCRC usage */
    if (bOption == PHAL_I18000P3M3_REQRN_USE_CRC)
    {
        /* Invalidate Stored Handle if a new one is requested */
        pDataParams->bHandleValid = PH_OFF;

        /* Retrieve StoredCRC */
        statusTmp = phpalI18000p3m3_GetSerialNo(
            pDataParams->pPalI18000p3m3DataParams,
            pDataParams->abHandle,
            &bRxLastBits);

        /* Change USE_CONDITION error Comp. Code*/
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_USE_CONDITION)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
        }
        else
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
    }
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* Prepare and buffer command header */
    bCmd[0] = PHAL_I18000P3M3_CMD_REQ_RN;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmd,
        1,
        0x00,
        NULL,
        NULL,
        NULL));

    /* Append Handle or StoredCRC and perform the Exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pDataParams->abHandle,
        2,
        0x00,
        ppRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length */
    if ((wRxByteLength != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
    /* Handling for StoredCRC usage */
    if (bOption == PHAL_I18000P3M3_REQRN_USE_CRC)
    {
        /* Invalidate Stored Handle if a new one is requested */
        pDataParams->bHandleValid = PH_ON;

        /* Store new Handle */
        (void)memcpy(pDataParams->abHandle, *ppRxBuffer, wRxByteLength);
    }
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_Read(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t bMemBank,
                                  uint8_t * pWordPtr,
                                  uint8_t bWordPtrLength,
                                  uint8_t bWordCount,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxBitLength
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_REM bCmd[10];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint16_t    PH_MEMLOC_REM wCmdLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;

    /* Parameter check */
    if ((bMemBank > 0x03U) || (bWordPtrLength > 3U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* Reset RxBitLength */
    *pRxBitLength = 0;

    /* prepare command header */
    wCmdLength = 0;
    bCmd[wCmdLength++] = PHAL_I18000P3M3_CMD_READ;
    bCmd[wCmdLength]   = (bMemBank << 6U) | (bWordPtrLength << 4U);  /* 4 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bWordPtrLength + 1U); ++bIndex)
    {
        bCmd[wCmdLength++] |= (pWordPtr[bIndex]) >> 4U;
        bCmd[wCmdLength]    = (pWordPtr[bIndex]) << 4U;
    }
    /* 4 bits left */

    /* append word-count */
    bCmd[wCmdLength++] |= bWordCount >> 4U;  /* 0 bits left */
    bCmd[wCmdLength]    = bWordCount << 4U;  /* 4 bits left */

    /* append handle */
    bCmd[wCmdLength++] |= pDataParams->abHandle[0] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength]    = pDataParams->abHandle[0] << 4U;    /* 4 bits left */
    bCmd[wCmdLength++] |= pDataParams->abHandle[1] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength++]  = pDataParams->abHandle[1] << 4U;    /* 4 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0C));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        wCmdLength,
        0x04,
        &pRecv,
        &wCmdLength,
        &bRxLastBits));

    /* Check received length and header bit */
    if (((bWordCount != 0x00U) && (wCmdLength != ((((uint16_t)bWordCount) << 1U) + 3U))) || (bRxLastBits != 1U) || (0U != (pRecv[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRecv, wCmdLength, bRxLastBits);
    }

    for (bIndex = 0; bIndex < (wCmdLength - 1u); bIndex++)
    {
        pRecv[bIndex] = (pRecv[bIndex] << 1U) | (pRecv[bIndex + 1U] >> 7U);
    }

    /* Status byte is removed */
    wCmdLength--;

    /* Check Handle */
    bHandle[0] = pRecv[wCmdLength - 2u];
    bHandle[1] = pRecv[wCmdLength - 1u];
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* Now we have whole last byte as valid */
    bRxLastBits = 0;

    /* Handle is not part of data */
    wCmdLength -= 2u;

    *ppRxBuffer = pRecv;

    /* Return length in bits */
    *pRxBitLength = ((uint16_t)(wCmdLength << 3U)) + bRxLastBits;

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_Write(
                                   phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                   uint8_t bOption,
                                   uint8_t bMemBank,
                                   uint8_t * pWordPtr,
                                   uint8_t bWordPtrLength,
                                   uint8_t * pData
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_REM bCmd[10];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint16_t    PH_MEMLOC_REM wCmdLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;

    /* Parameter check */
    if ((bMemBank > 0x03U) || (bWordPtrLength > 3U) ||
        ((bOption != PHAL_I18000P3M3_AC_NO_COVER_CODING) &&
        (bOption != PHAL_I18000P3M3_AC_USE_COVER_CODING)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* prepare command header */
    wCmdLength = 0;
    bCmd[wCmdLength++] = PHAL_I18000P3M3_CMD_WRITE;
    bCmd[wCmdLength]   = (bMemBank << 6U) | (bWordPtrLength << 4U);  /* 4 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bWordPtrLength + 1U); ++bIndex)
    {
        bCmd[wCmdLength++] |= (pWordPtr[bIndex]) >> 4U;
        bCmd[wCmdLength]    = (pWordPtr[bIndex]) << 4U;
    }
    /* 4 bits left */

    /* append data */
    bCmd[wCmdLength++] |= pData[0] >> 4U;                        /* 0 bits left */
    bCmd[wCmdLength++]  = (pData[0] << 4U) | (pData[1] >> 4U);    /* 0 bits left */
    bCmd[wCmdLength]    = pData[1] << 4U;                        /* 4 bits left */

    /* perform cover-coding if neccessary */
    if (bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
    {
        /* retrieve RN16 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalI18000p3m3_Sw_ReqRn(
            pDataParams,
            PHAL_I18000P3M3_REQRN_USE_HANDLE,
            &pRxBuffer));

        /* perform XOR operation */
        bCmd[wCmdLength - 2u] ^= pRxBuffer[0] >> 4U;
        bCmd[wCmdLength - 1u] ^= (pRxBuffer[0] << 4U) |  (pRxBuffer[1] >> 4U);
        bCmd[wCmdLength]     ^= pRxBuffer[1] << 4U;
    }

    /* append handle */
    bCmd[wCmdLength++] |= pDataParams->abHandle[0] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength]    = pDataParams->abHandle[0] << 4U;    /* 4 bits left */
    bCmd[wCmdLength++] |= pDataParams->abHandle[1] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength++]  = pDataParams->abHandle[1] << 4U;    /* 4 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0C));

    /* Set config for timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS,
        5));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        wCmdLength,
        0x04,
        &pRxBuffer,
        &wCmdLength,
        &bRxLastBits));

    /* Check header bit */
    if ((wCmdLength != 3U) || (bRxLastBits != 1U) || (0U != (pRxBuffer[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wCmdLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = (uint8_t)((pRxBuffer[0] << 1U) | (pRxBuffer[1] >> 7U));
    bHandle[1] = (uint8_t)((pRxBuffer[1] << 1U) | (pRxBuffer[2] >> 7U));
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_Kill(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption,
                                  uint8_t * pPassword,
                                  uint8_t bRecom
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[6];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxByteLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Parameter check */
    if (((bOption != PHAL_I18000P3M3_AC_NO_COVER_CODING) &&
        (bOption != PHAL_I18000P3M3_AC_USE_COVER_CODING)) ||
        (bRecom > 0x07U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* */
    /* Perform FIRST PART of KILL command */
    /* */

    /* prepare command header */
    bCmd[0] = PHAL_I18000P3M3_CMD_KILL;
    bCmd[1] = pPassword[0];
    bCmd[2] = pPassword[1];

    /* perform cover-coding if necessary */
    if (bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
    {
        /* retrieve RN16 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalI18000p3m3_Sw_ReqRn(
            pDataParams,
            PHAL_I18000P3M3_REQRN_USE_HANDLE,
            &pRxBuffer));

        /* perform XOR operation */
        bCmd[1] ^= pRxBuffer[0];
        bCmd[2] ^= pRxBuffer[1];
    }

    /* append RFU/Recom bits (000) and handle */
    bCmd[3]  = 0x00;                            /* 5 bits left */
    bCmd[3] |= pDataParams->abHandle[0] >> 3U;   /* 0 bits left */
    bCmd[4]  = pDataParams->abHandle[0] << 5U;   /* 5 bits left */
    bCmd[4] |= pDataParams->abHandle[1] >> 3U;   /* 0 bits left */
    bCmd[5]  = pDataParams->abHandle[1] << 5U;   /* 5 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0B));

    /* Set config for timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS,
        5));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        6,
        0x03,
        &pRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length */
    if ((wRxByteLength != 2U) || (bRxLastBits != 0U))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wRxByteLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = pRxBuffer[0];
    bHandle[1] = pRxBuffer[1];
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* */
    /* Perform SECOND PART of KILL command */
    /* */

    /* prepare command header */
    bCmd[0] = PHAL_I18000P3M3_CMD_KILL;
    bCmd[1] = pPassword[2];
    bCmd[2] = pPassword[3];

    /* perform cover-coding if neccessary */
    if (bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
    {
        /* retrieve RN16 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalI18000p3m3_Sw_ReqRn(
            pDataParams,
            PHAL_I18000P3M3_REQRN_USE_HANDLE,
            &pRxBuffer));

        /* perform XOR operation */
        bCmd[1] ^= pRxBuffer[0];
        bCmd[2] ^= pRxBuffer[1];
    }

    /* append RFU/Recom bits (000) and handle */
    bCmd[3]  = bRecom << 5U;                     /* 5 bits left */
    bCmd[3] |= pDataParams->abHandle[0] >> 3U;   /* 0 bits left */
    bCmd[4]  = pDataParams->abHandle[0] << 5U;   /* 5 bits left */
    bCmd[4] |= pDataParams->abHandle[1] >> 3U;   /* 0 bits left */
    bCmd[5]  = pDataParams->abHandle[1] << 5U;   /* 5 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0B));

    /* Set config for timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS,
        5));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        6,
        0x03,
        &pRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length and header */
    if ((wRxByteLength != 3U) ||
        (bRxLastBits != 1U) ||
        (0U != (pRxBuffer[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wRxByteLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = (uint8_t)((pRxBuffer[0] << 1U) | (pRxBuffer[1] >> 7U));
    bHandle[1] = (uint8_t)((pRxBuffer[1] << 1U) | (pRxBuffer[2] >> 7U));
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_Lock(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t * pMask,
                                  uint8_t * pAction
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[6];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxByteLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* prepare command header */
    bCmd[0] = PHAL_I18000P3M3_CMD_LOCK;                                     /* 0 bits left */
    bCmd[1] = pMask[0];                                                     /* 0 bits left */
    bCmd[2] = (uint8_t)((pMask[1] & 0xC0U) | (pAction[0] >> 2U));             /* 0 bits left */
    bCmd[3] = (uint8_t)((pAction[0] << 6U) | ((pAction[1] & 0xC0U) >> 2U));    /* 4 bits left */

    /* append handle */
    bCmd[3] |= pDataParams->abHandle[0] >> 4U;   /* 0 bits left */
    bCmd[4]  = pDataParams->abHandle[0] << 4U;   /* 4 bits left */
    bCmd[4] |= pDataParams->abHandle[1] >> 4U;   /* 0 bits left */
    bCmd[5]  = pDataParams->abHandle[1] << 4U;   /* 4 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0C));

    /* Set config for timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS,
        5));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        6,
        0x04,
        &pRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length and header */
    if ((wRxByteLength != 3U) ||
        (bRxLastBits != 1U) ||
        (0U != (pRxBuffer[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wRxByteLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = (uint8_t)((pRxBuffer[0] << 1U) | (pRxBuffer[1] >> 7U));
    bHandle[1] = (uint8_t)((pRxBuffer[1] << 1U) | (pRxBuffer[2] >> 7U));
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_Access(
                                    phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t * pPassword
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[5];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxByteLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Parameter check */
    if ((bOption != PHAL_I18000P3M3_AC_NO_COVER_CODING) &&
        (bOption != PHAL_I18000P3M3_AC_USE_COVER_CODING))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* */
    /* Perform FIRST PART of ACCESS command */
    /* */

    /* prepare command header */
    bCmd[0] = PHAL_I18000P3M3_CMD_ACCESS;
    bCmd[1] = pPassword[0];
    bCmd[2] = pPassword[1];

    /* perform cover-coding if neccessary */
    if (bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
    {
        /* retrieve RN16 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalI18000p3m3_Sw_ReqRn(
            pDataParams,
            PHAL_I18000P3M3_REQRN_USE_HANDLE,
            &pRxBuffer));

        /* perform XOR operation */
        bCmd[1] ^= pRxBuffer[0];
        bCmd[2] ^= pRxBuffer[1];
    }

    /* append handle */
    bCmd[3] = pDataParams->abHandle[0];
    bCmd[4] = pDataParams->abHandle[1];

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x08));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        5,
        0x00,
        &pRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length */
    if ((wRxByteLength != 2U) || (bRxLastBits != 0U))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wRxByteLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = pRxBuffer[0];
    bHandle[1] = pRxBuffer[1];
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* */
    /* Perform SECOND PART of ACCESS command */
    /* */

    /* prepare command header */
    bCmd[0] = PHAL_I18000P3M3_CMD_ACCESS;
    bCmd[1] = pPassword[2];
    bCmd[2] = pPassword[3];

    /* perform cover-coding if neccessary */
    if (bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
    {
        /* retrieve RN16 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalI18000p3m3_Sw_ReqRn(
            pDataParams,
            PHAL_I18000P3M3_REQRN_USE_HANDLE,
            &pRxBuffer));

        /* perform XOR operation */
        bCmd[1] ^= pRxBuffer[0];
        bCmd[2] ^= pRxBuffer[1];
    }

    /* append handle */
    bCmd[3] = pDataParams->abHandle[0];
    bCmd[4] = pDataParams->abHandle[1];

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        5,
        0x00,
        &pRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length */
    if ((wRxByteLength != 2U) || (bRxLastBits != 0U))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wRxByteLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = pRxBuffer[0];
    bHandle[1] = pRxBuffer[1];
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_BlockWrite(
                                        phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint8_t bMemBank,
                                        uint8_t * pWordPtr,
                                        uint8_t bWordPtrLength,
                                        uint8_t bWordCount,
                                        uint8_t * pData
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_REM bCmd[7];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint16_t    PH_MEMLOC_REM wCmdLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;

    /* Parameter check */
    if ((bMemBank > 0x03U) || (bWordPtrLength > 3U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* prepare command header */
    wCmdLength = 0;
    bCmd[wCmdLength++] = PHAL_I18000P3M3_CMD_BLOCKWRITE;
    bCmd[wCmdLength]   = (bMemBank << 6U) | (bWordPtrLength << 4U);  /* 4 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bWordPtrLength + 1U); ++bIndex)
    {
        bCmd[wCmdLength++] |= (pWordPtr[bIndex]) >> 4U;
        bCmd[wCmdLength]    = (pWordPtr[bIndex]) << 4U;
    }
    /* 4 bits left */

    /* append (4bits) of Word-Count */
    bCmd[wCmdLength++] |= bWordCount >> 4U;  /* 0 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0C));

    /* Set config for timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS,
        10));

    /* buffer the current contents */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmd,
        wCmdLength,
        0x00,
        NULL,
        NULL,
        NULL));

    /* reset command length*/
    wCmdLength = 0;

    /* append the rest bits of block-range*/
    bCmd[wCmdLength] = bWordCount << 4U;  /* 4 bits left */

    /* append data */
    for (bIndex = 0; bIndex < (bWordCount << 1U); ++bIndex)
    {
        bCmd[wCmdLength++] |= pData[bIndex] >> 4U;  /* 0 bits left */

        /* buffer the current contents if command buffer is full */
        if (wCmdLength == 7U)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
                pDataParams->pPalI18000p3m3DataParams,
                PH_EXCHANGE_BUFFER_CONT,
                bCmd,
                wCmdLength,
                0x00,
                NULL,
                NULL,
                NULL));

            /* reset command length*/
            wCmdLength = 0;
        }

        bCmd[wCmdLength] = pData[bIndex] << 4U;  /* 4 bits left */
    }

    /* append handle */
    bCmd[wCmdLength++] |= pDataParams->abHandle[0] >> 4U;    /* 0 bits left */

    /* buffer the current contents if command buffer is full */
    if (wCmdLength == 7U)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
            pDataParams->pPalI18000p3m3DataParams,
            PH_EXCHANGE_BUFFER_CONT,
            bCmd,
            wCmdLength,
            0x00,
            NULL,
            NULL,
            NULL));

        /* reset command length*/
        wCmdLength = 0;
    }

    bCmd[wCmdLength]    = pDataParams->abHandle[0] << 4U;    /* 4 bits left */
    bCmd[wCmdLength++] |= pDataParams->abHandle[1] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength++]  = pDataParams->abHandle[1] << 4U;    /* 4 bits left */

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCmd,
        wCmdLength,
        0x04,
        &pRxBuffer,
        &wCmdLength,
        &bRxLastBits));

    /* Check header bit */
    if ((wCmdLength != 3U) || (bRxLastBits != 1U) || (0U != (pRxBuffer[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wCmdLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = (uint8_t)((pRxBuffer[0] << 1U) | (pRxBuffer[1] >> 7U));
    bHandle[1] = (uint8_t)((pRxBuffer[1] << 1U) | (pRxBuffer[2] >> 7U));
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_BlockErase(
                                        phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint8_t bMemBank,
                                        uint8_t * pWordPtr,
                                        uint8_t bWordPtrLength,
                                        uint8_t bWordCount
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_REM bCmd[10];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint16_t    PH_MEMLOC_REM wCmdLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;

    /* Parameter check */
    if ((bMemBank > 0x03U) || (bWordPtrLength > 3U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* prepare command header */
    wCmdLength = 0;
    bCmd[wCmdLength++] = PHAL_I18000P3M3_CMD_BLOCKERASE;
    bCmd[wCmdLength]   = (bMemBank << 6U) | (bWordPtrLength << 4U);  /* 4 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bWordPtrLength + 1U); ++bIndex)
    {
        bCmd[wCmdLength++] |= (pWordPtr[bIndex]) >> 4U;
        bCmd[wCmdLength]    = (pWordPtr[bIndex]) << 4U;
    }
    /* 4 bits left */

    /* append word-count */
    bCmd[wCmdLength++] |= bWordCount >> 4U;  /* 0 bits left */
    bCmd[wCmdLength]    = bWordCount << 4U;  /* 4 bits left */

    /* append handle */
    bCmd[wCmdLength++] |= pDataParams->abHandle[0] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength]    = pDataParams->abHandle[0] << 4U;    /* 4 bits left */
    bCmd[wCmdLength++] |= pDataParams->abHandle[1] >> 4U;    /* 0 bits left */
    bCmd[wCmdLength++]  = pDataParams->abHandle[1] << 4U;    /* 4 bits left */

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        wCmdLength,
        0x04,
        &pRxBuffer,
        &wCmdLength,
        &bRxLastBits));

    /* Check received length and header bit */
    if ((wCmdLength != 3U) || (bRxLastBits != 1U) || (0U != (pRxBuffer[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRxBuffer, wCmdLength, bRxLastBits);
    }

    /* Check Handle */
    bHandle[0] = (uint8_t)((pRxBuffer[0] << 1U) | (pRxBuffer[1] >> 7U));
    bHandle[1] = (uint8_t)((pRxBuffer[1] << 1U) | (pRxBuffer[2] >> 7U));
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_BlockPermaLock(
    phalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bRFU,
    uint8_t bReadLock,
    uint8_t bMemBank,
    uint8_t * pBlockPtr,
    uint8_t bBlockPtrLength,
    uint8_t bBlockRange,
    uint8_t * pMask,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxBitLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_REM bCmd[7];
    uint8_t     PH_MEMLOC_REM bHandle[2];
    uint16_t    PH_MEMLOC_REM wCmdLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;

    /* Parameter check */
    if ((bReadLock > 0x01U) || (bMemBank > 0x03U) || (bBlockPtrLength > 3U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I18000P3M3);
    }

    /* Valid Handle is mandatory */
    if (pDataParams->bHandleValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_I18000P3M3);
    }

    /* Reset RxBitLength */
    *pRxBitLength = 0;

    /* prepare command header */
    wCmdLength = 0;
    bCmd[wCmdLength++] = PHAL_I18000P3M3_CMD_BLOCKPERMALOCK;
    bCmd[wCmdLength++] = bRFU;
    bCmd[wCmdLength]   = (bReadLock << 7U) | (bMemBank << 5U) | (bBlockPtrLength << 3U);  /* 3 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bBlockPtrLength + 1U); ++bIndex)
    {
        bCmd[wCmdLength++] |= (pBlockPtr[bIndex]) >> 5U;
        bCmd[wCmdLength]    = (pBlockPtr[bIndex]) << 3U;
    }
    /* 3 bits left */

    /* append (3bits) of block-range */
    bCmd[wCmdLength++] |= bBlockRange >> 5U;  /* 0 bits left */

    /* buffer the current contents */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmd,
        wCmdLength,
        0x00,
        NULL,
        NULL,
        NULL));

    /* reset command length*/
    wCmdLength = 0;

    /* append the rest bits of block-range*/
    bCmd[wCmdLength] = bBlockRange << 3U;  /* 3 bits left */

    /* append mask */
    if (bReadLock == 0x01U)
    {
        for (bIndex = 0; bIndex < (bBlockRange << 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= pMask[bIndex] >> 5U;  /* 0 bits left */

            /* buffer the current contents if command buffer is full */
            if (wCmdLength == 7U)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
                    pDataParams->pPalI18000p3m3DataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    bCmd,
                    wCmdLength,
                    0x00,
                    NULL,
                    NULL,
                    NULL));

                /* reset command length*/
                wCmdLength = 0;
            }

            bCmd[wCmdLength] = pMask[bIndex] << 3U;  /* 3 bits left */
        }
    }

    /* append handle */
    bCmd[wCmdLength++] |= pDataParams->abHandle[0] >> 5U;    /* 0 bits left */

    /* buffer the current contents if command buffer is full */
    if (wCmdLength == 7U)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
            pDataParams->pPalI18000p3m3DataParams,
            PH_EXCHANGE_BUFFER_CONT,
            bCmd,
            wCmdLength,
            0x00,
            NULL,
            NULL,
            NULL));

        /* reset command length*/
        wCmdLength = 0;
    }

    bCmd[wCmdLength]    = pDataParams->abHandle[0] << 3U;    /* 3 bits left */
    bCmd[wCmdLength++] |= pDataParams->abHandle[1] >> 5U;    /* 0 bits left */
    bCmd[wCmdLength++]  = pDataParams->abHandle[1] << 3U;    /* 3 bits left */

    /* Set config for TXLASTBITS */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TXLASTBITS,
        0x0D));

    /* Set config for timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_SetConfig(
        pDataParams->pPalI18000p3m3DataParams,
        PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS,
        500));

    /* perform the exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Exchange(
        pDataParams->pPalI18000p3m3DataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCmd,
        wCmdLength,
        0x05,
        &pRecv,
        &wCmdLength,
        &bRxLastBits));

    /* Check header bit */
    if ((wCmdLength < 3U) || (bRxLastBits != 1U) || (0U != (pRecv[0] & 0x80U)))
    {
        return phalI18000p3m3_Sw_ParseError(pDataParams, pRecv, wCmdLength, bRxLastBits);
    }

    /* Check data length */
    if (bReadLock == 0x00U)
    {
        if (wCmdLength != (3u + (((uint16_t)bBlockRange) << 1U)))
        {
            return phalI18000p3m3_Sw_ParseError(pDataParams, pRecv, wCmdLength, bRxLastBits);
        }
    }
    else
    {
        if (wCmdLength != 3U)
        {
            return phalI18000p3m3_Sw_ParseError(pDataParams, pRecv, wCmdLength, bRxLastBits);
        }
    }

    for (bIndex = 0; bIndex < (wCmdLength - 1u); bIndex++)
    {
        pRecv[bIndex] = (pRecv[bIndex] << 1U) | (pRecv[bIndex + 1U] >> 7U);
    }

    /* Status byte is removed */
    wCmdLength--;

    /* Check Handle */
    bHandle[0] = pRecv[wCmdLength - 2u];
    bHandle[1] = pRecv[wCmdLength - 1u];
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* Now we have whole last byte as valid */
    bRxLastBits = 0;

    /* Handle is not part of data */
    wCmdLength -= 2u;

    *ppRxBuffer = pRecv;

    /* Return length in bits */
    *pRxBitLength = ((uint16_t)(wCmdLength << 3U)) + bRxLastBits;

    return PH_ERR_SUCCESS;
}

phStatus_t phalI18000p3m3_Sw_SetHandle(
                                       phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                       uint8_t* pHandle
                                       )
{
    (void)memcpy(pDataParams->abHandle, pHandle, 2);
    pDataParams->bHandleValid = PH_ON;
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_I18000P3M3_SW */
