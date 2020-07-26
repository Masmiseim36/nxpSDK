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
#include <phTools.h>
#include <phhalHw.h>
#include <phpalI18000p3m3.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW

#include "phpalI18000p3m3_Sw.h"
#include "phpalI18000p3m3_Sw_Int.h"

phStatus_t phpalI18000p3m3_Sw_Init(
                                   phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                   uint16_t wSizeOfDataParams,
                                   void * pHalDataParams
                                   )
{
    if (sizeof(phpalI18000p3m3_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    pDataParams->wId                = PH_COMP_PAL_I18000P3M3 | PHPAL_I18000P3M3_SW_ID;
    pDataParams->pHalDataParams     = pHalDataParams;
    pDataParams->bSession           = PHPAL_I18000P3M3_SESSION_S0;
    pDataParams->bStoredCRCValid    = PH_OFF;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_Exchange(
                                       phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint8_t * pTxBuffer,
                                       uint16_t wTxLength,
                                       uint8_t bTxLastBits,
                                       uint8_t ** ppRxBuffer,
                                       uint16_t * pRxLength,
                                       uint8_t * pRxLastBits
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wValue = 0;

    /* TxLastBits != 0 is invalid for buffered operation */
    if ((0U != ((wOption & PH_EXCHANGE_BUFFERED_BIT))) && (bTxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* TxLastBits > 7 is invalid  */
    if (bTxLastBits > 7U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Reset RxLastBits */
    if (pRxLastBits != NULL)
    {
        *pRxLastBits = 0;
    }

    /*  last Tx-Byte. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, bTxLastBits));

    /* Perform exchange */
    status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength);

    /* Ignore incomplete byte status, bail out on error */
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        /* Get RxLastBits */
        if (pRxLastBits != NULL)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXLASTBITS,
                &wValue));

            *pRxLastBits = (uint8_t)wValue;
        }

        /* Map to success status */
        status = PH_ERR_SUCCESS;
    }

    return status;
}

phStatus_t phpalI18000p3m3_Sw_Select(
                                     phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                     uint8_t bTarget,
                                     uint8_t bAction,
                                     uint8_t bMemBank,
                                     uint8_t * pPointer,
                                     uint8_t bPointerLength,
                                     uint8_t * pMask,
                                     uint8_t bMaskBitLength,
                                     uint8_t bTruncate
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_BUF bCmd[39];
    uint8_t     PH_MEMLOC_REM bCmdLength;
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t     PH_MEMLOC_REM bMaskByteLength;
    uint16_t    PH_MEMLOC_REM wCmdBitLength = 0;
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Parameter check */
    if ((bTarget > 0x07U) || (bAction > 0x07U) || (bMemBank > 0x03U) || (bPointerLength > 3U) || (bTruncate > 1U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Build Command frame */
    bCmdLength = 0;
    bCmd[bCmdLength++] = (PHPAL_I18000P3M3_CMD_SELECT << 4U) | (bTarget << 1U) | (bAction >> 2U);  /* 0 bits left */
    bCmd[bCmdLength]   = (bAction << 6U) | (bMemBank <<  4U) | (bPointerLength << 2U);             /* 2 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bPointerLength + /* */ 1U); ++bIndex)
    {
        bCmd[bCmdLength++] |= (pPointer[bIndex]) >> 6U;
        bCmd[bCmdLength]    = (pPointer[bIndex]) << 2U;
    }
    /* 2 bits left */

    bCmd[bCmdLength++] |= bMaskBitLength >> 6U;
    bCmd[bCmdLength]    = bMaskBitLength << 2U;  /* 2 bits left */

    /* calculate intermediate bitlength */
    wCmdBitLength = ((uint16_t)bCmdLength << 3U) + 6U;

    /* get byte-length of mask */
    bMaskByteLength = bMaskBitLength >> 3U;
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }

    /* append mask value */
    for (bIndex = 0; bIndex < bMaskByteLength; ++bIndex)
    {
        bTmp = pMask[bIndex];

        /* Mask out invalid bits of last byte */
        if ((bIndex == (bMaskByteLength - 1U)) && ((bMaskBitLength & 0x07U) > 0U))
        {
            bTmp &= (uint8_t)(0xFFU << (8U - (bMaskBitLength & 0x07U)));
        }

        /* Append mask value */
        bCmd[bCmdLength++] |= bTmp >> 6U;
        bCmd[bCmdLength]    = bTmp << 2U;
    }
    /* calculate new bit length */
    wCmdBitLength = wCmdBitLength + bMaskBitLength;

    /* decrement length if we incremented too much */
    if ((wCmdBitLength >> 3U) != bCmdLength)
    {
        --bCmdLength;
    }

    /* append truncate bit */
    if (bTruncate != PH_OFF)
    {
        bCmd[bCmdLength] |= (uint8_t)(0x80u >> (wCmdBitLength & 0x07U));
    }
    ++bCmdLength;
    ++wCmdBitLength;

    /* Set frame sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Disable TxWait */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        0));

    /* Set T4 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I18000P3M3_SW_T4_MIN_US));

    /* Switch on HAL Tx-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));

    /* Switch off HAL Rx-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* set Tx-CRC as CRC16  */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC16|PHHAL_HW_CRCOPTION_INVERTED));

    /* Exchange command */
    status = phpalI18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        bCmdLength,
        (uint8_t)(wCmdBitLength & 0x07U),
        &pResponse,
        &wRxLength,
        &bRxLastBits);

    /* Switch off HAL Tx-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));

    /* Timeout is the correct behavior */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* if data was received return an error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18000P3M3);
        }
        else
        {
            /* Return the received error */
            return status;
        }
    }

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
phStatus_t phpalI18000p3m3_Sw_BeginRound(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bDr,
    uint8_t bM,
    uint8_t bTRext,
    uint8_t bSel,
    uint8_t bSession,
    uint8_t bRfu,
    uint8_t bQ,
    uint8_t * pStoredCRC
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF aCmd[3];
    uint16_t    PH_MEMLOC_REM wCmdBitLength = 0;
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Parameter check */
    if ((bDr > 0x01U) || (bM < 0x02U) || (bM > 0x03U) || (bTRext > 1U) || (bSel > 0x03U) || (bSession > 0x03U) || (bRfu > 0x01U) || (bQ > 0x0FU))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Store new session parameter */
    pDataParams->bSession = bSession;

    /* Map TRext to 0 or 1 */
    if (bTRext != PH_OFF)
    {
        bTRext = 1;
    }
    else
    {
        bTRext = 0;
    }

    /* Build Command frame */
    aCmd[0] = (PHPAL_I18000P3M3_CMD_BEGIN_ROUND << 4U) | (bDr << 3U) | (bM << 1U) | (bTRext);  /* 0 bits left */
    aCmd[1] = (bSel << 6U) | (bSession << 4U) | (bRfu << 3U) | (bQ >> 1U);                      /* 0 bits left */
    aCmd[2] = (bQ << 7U);                                                                    /* 7 bits left */

    /* Calculate BitLength */
    wCmdBitLength = 17;

    /* Set Manchester-2 coding */
    if (bM == PHPAL_I18000P3M3_M_MANCHESTER_2)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            PHHAL_HW_RF_RX_DATARATE_I18000P3M3_MAN2));
    }
    /* Set Manchester-4 coding */
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            PHHAL_HW_RF_RX_DATARATE_I18000P3M3_MAN4));
    }

    /* Set Subcarrier */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SUBCARRIER,
        (bDr == 0U) ? PHHAL_HW_SUBCARRIER_DUAL : PHHAL_HW_SUBCARRIER_QUAD));

    /* Set preamble as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_PREAMBLE));

    /* Set TxWait to T2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        PHPAL_I18000P3M3_SW_T2_MIN_US));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I18000P3M3_SW_T1_MAX_US + PHPAL_I18000P3M3_SW_T3_MIN_US + PHPAL_I18000P3M3_SW_DELTA_MIN_US));

    /* Switch ON HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC5));

    /* Reset bStoredCRCValid flag */
    pDataParams->bStoredCRCValid = PH_OFF;

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        3,
        (uint8_t)(wCmdBitLength & 0x07U),
        &pResponse,
        &wRxLength,
        &bRxLastBits));

    /* Check response length */
    if ((wRxLength != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18000P3M3);
    }

    /* Switch off HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Return StoredCRC */
    (void)memcpy(pStoredCRC, pResponse, wRxLength);

    /* Save StoredCRC */
    (void)memcpy(pDataParams->abStoredCRC, pResponse, wRxLength);

    /* Set bStoredCRCValid flag */
    pDataParams->bStoredCRCValid = PH_ON;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_ResizeRound(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bUpDn,
    uint8_t * pStoredCRC
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF aCmd[2];
    uint16_t    PH_MEMLOC_REM wCmdBitLength = 0;
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Parameter check */
    switch (bUpDn)
    {
    case PHPAL_I18000P3M3_UPDN_INCREMENT:
    case PHPAL_I18000P3M3_UPDN_NOCHANGE:
    case PHPAL_I18000P3M3_UPDN_DECREMENT:
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Build Command frame */
    aCmd[0] = (PHPAL_I18000P3M3_CMD_RESIZE_ROUND << 4U) | (pDataParams->bSession << 2U) | (bUpDn >> 1U);   /* 0 bits left */
    aCmd[1] = (bUpDn << 7U);                                                                             /* 7 bits left */

    /* Calculate BitLength */
    wCmdBitLength = 9;

    /* Set frame-sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Set TxWait to T2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        PHPAL_I18000P3M3_SW_T2_MIN_US));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I18000P3M3_SW_T1_MAX_US + PHPAL_I18000P3M3_SW_T3_MIN_US));

    /* Reset bStoredCRCValid flag */
    pDataParams->bStoredCRCValid = PH_OFF;

    /* Switch off TX and on RX HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC5));

    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        2,
        (uint8_t)(wCmdBitLength & 0x07U),
        &pResponse,
        &wRxLength,
        &bRxLastBits));

    /* Check response length */
    if ((wRxLength != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18000P3M3);
    }
    /* Switch off HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Return StoredCRC */
    (void)memcpy(pStoredCRC, pResponse, wRxLength);

    /* Save StoredCRC */
    (void)memcpy(pDataParams->abStoredCRC, pResponse, wRxLength);

    /* Set bStoredCRCValid flag */
    pDataParams->bStoredCRCValid = PH_ON;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_NextSlot(
                                       phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                       uint8_t * pStoredCRC
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF aCmd[1];
    uint16_t    PH_MEMLOC_REM wCmdBitLength = 0;
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Build Command frame */
    aCmd[0] = (PHPAL_I18000P3M3_CMD_NEXT_SLOT << 4U) | (pDataParams->bSession << 2U); /* 2 bits left */

    /* Calculate BitLength */
    wCmdBitLength = 6;

    /* Set frame-sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Set TxWait to T2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        PHPAL_I18000P3M3_SW_T2_MIN_US));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I18000P3M3_SW_T1_MAX_US + PHPAL_I18000P3M3_SW_T3_MIN_US + PHPAL_I18000P3M3_SW_DELTA_MIN_US));

    /* Reset bStoredCRCValid flag */
    pDataParams->bStoredCRCValid = PH_OFF;

    /* Switch off TX and on RX HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC5));

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        (uint8_t)(wCmdBitLength & 0x07U),
        &pResponse,
        &wRxLength,
        &bRxLastBits));

    /* Check response length */
    if ((wRxLength != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18000P3M3);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));
    /* Return StoredCRC */
    (void)memcpy(pStoredCRC, pResponse, wRxLength);

    /* Save StoredCRC */
    (void)memcpy(pDataParams->abStoredCRC, pResponse, wRxLength);

    /* Set bStoredCRCValid flag */
    pDataParams->bStoredCRCValid = PH_ON;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_GetSerialNo(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t * pRxBuffer,
    uint8_t * pRxLength
    )
{
    /* Return an error if StoredCRC is not valid */
    if (0U == (pDataParams->bStoredCRCValid))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_I18000P3M3);
    }

    /* Copy StoredCRC */
    (void)memcpy(pRxBuffer, pDataParams->abStoredCRC, sizeof(pDataParams->abStoredCRC));
    *pRxLength = sizeof(pDataParams->abStoredCRC);

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

phStatus_t phpalI18000p3m3_Sw_CreateSelectCmd(
                                         phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                         uint8_t bTarget,
                                         uint8_t bAction,
                                         uint8_t bMemBank,
                                         uint8_t * pPointer,
                                         uint8_t bPointerLength,
                                         uint8_t * pMask,
                                         uint8_t bMaskBitLength,
                                         uint8_t bTruncate,
                                         uint8_t * pSelectCmd,
                                         uint8_t * bLen,
                                         uint8_t * bValidBits
                                        )
{
    uint8_t     PH_MEMLOC_COUNT bIndex;
    uint8_t     PH_MEMLOC_REM bCmdLength;
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t     PH_MEMLOC_REM bMaskByteLength;
    uint16_t    PH_MEMLOC_REM wCmdBitLength = 0;
    PH_UNUSED_VARIABLE(pDataParams);
    /* Parameter check */
    if ((bTarget > 0x07U) || (bAction > 0x07U) || (bMemBank > 0x03U) || (bPointerLength > 3U) || (bTruncate > 1U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Build Command frame */
    bCmdLength = 0;
    *(pSelectCmd + bCmdLength++) = (PHPAL_I18000P3M3_CMD_SELECT << 4U) | (bTarget << 1U) | (bAction >> 2U);  /* 0 bits left */
    *(pSelectCmd + bCmdLength)   = (bAction << 6U) | (bMemBank <<  4U) | (bPointerLength << 2U);             /* 2 bits left */

    /* append pointer */
    for (bIndex = 0; bIndex < (bPointerLength + /* */ 1U); ++bIndex)
    {
        *(pSelectCmd + bCmdLength++) |= (pPointer[bIndex]) >> 6U;
        *(pSelectCmd + bCmdLength)    = (pPointer[bIndex]) << 2U;
    }
    /* 2 bits left */

    *(pSelectCmd + bCmdLength++) |= bMaskBitLength >> 6U;
    *(pSelectCmd + bCmdLength)    = bMaskBitLength << 2U;  /* 2 bits left */

    /* calculate intermediate bitlength */
    wCmdBitLength = ((uint16_t)bCmdLength << 3U) + 6U;

    /* get byte-length of mask */
    bMaskByteLength = bMaskBitLength >> 3U;
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }

    /* append mask value */
    for (bIndex = 0; bIndex < bMaskByteLength; ++bIndex)
    {
        bTmp = pMask[bIndex];

        /* Mask out invalid bits of last byte */
        if ((bIndex == (bMaskByteLength - 1U)) && ((bMaskBitLength & 0x07U) > 0U))
        {
            bTmp &= (uint8_t)(0xFFU << (8U - (bMaskBitLength & 0x07U)));
        }

        /* Append mask value */
        *(pSelectCmd + bCmdLength++) |= bTmp >> 6U;
        *(pSelectCmd + bCmdLength)    = bTmp << 2U;
    }
    /* calculate new bit length */
    wCmdBitLength = wCmdBitLength + bMaskBitLength;

    /* decrement length if we incremented too much */
    if ((wCmdBitLength >> 3U) != bCmdLength)
    {
        --bCmdLength;
    }

    /* append truncate bit */
    if (bTruncate != PH_OFF)
    {
        *(pSelectCmd + bCmdLength) |= (uint8_t)(0x80U >> (wCmdBitLength & 0x07U));
    }
    ++bCmdLength;
    ++wCmdBitLength;
    *bLen = bCmdLength;
    *bValidBits = (uint8_t)(wCmdBitLength & 0x07U);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_CreateBeginRoundCmd(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bDr,
    uint8_t bM,
    uint8_t bTRext,
    uint8_t bSel,
    uint8_t bSession,
    uint8_t bRfu,
    uint8_t bQ,
    uint8_t * pBeginRnd
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t PH_MEMLOC_REM wValue = 0;
    /* Parameter check */
    if ((bDr > 0x01U) || (bM < 0x02U) || (bM > 0x03U) || (bTRext > 1U) || (bSel > 0x03U) || (bSession > 0x03U) || (bRfu > 0x01U) || (bQ > 0x0FU))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Store new session parameter */
   pDataParams->bSession = bSession;

    /* Map TRext to 0 or 1 */
    if (bTRext != PH_OFF)
    {
        bTRext = 1;
    }
    else
    {
        bTRext = 0;
    }

    /* Build Command frame */
    *pBeginRnd = (PHPAL_I18000P3M3_CMD_BEGIN_ROUND << 4U) | (bDr << 3U) | (bM << 1U) | (bTRext);  /* 0 bits left */
    *(pBeginRnd+1U) = (bSel << 6U) | (bSession << 4U) | (bRfu << 3U) | (bQ >> 1U);                      /* 0 bits left */
    *(pBeginRnd+2U) = (bQ << 7U);                                                                    /* 7 bits left */

    if(bDr == 0U)
    {
        if(bM == 2U)
        {
            wValue = PHHAL_HW_RX_I18000P3M3_FL_423_MAN2;
        }
        else
        {
            wValue = PHHAL_HW_RX_I18000P3M3_FL_423_MAN4;
        }
    }
    else
    {
        if(bM == 2U)
        {
            wValue = PHHAL_HW_RX_I18000P3M3_FL_847_MAN2;
        }
        else
        {
            wValue = PHHAL_HW_RX_I18000P3M3_FL_847_MAN4;
        }
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
                wValue));

    /* Set Subcarrier */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SUBCARRIER,
        (bDr == 0U) ? PHHAL_HW_SUBCARRIER_DUAL : PHHAL_HW_SUBCARRIER_QUAD));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_Ack(
                                  phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption,
                                  uint8_t * pHandle,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxBitLength
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF aCmd[3];
    uint16_t    PH_MEMLOC_REM wCmdBitLength = 0;
    uint8_t *   PH_MEMLOC_REM pData = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Send given handle */
    if (bOption == PHPAL_I18000P3M3_ACK_USE_HANDLE)
    {
        pData = pHandle;
    }
    /* Send StoredCRC */
    else if (bOption == PHPAL_I18000P3M3_ACK_USE_CRC)
    {
        /* StoredCRC should be valid */
        if (pDataParams->bStoredCRCValid == PH_OFF)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_I18000P3M3);
        }

        pData = pDataParams->abStoredCRC;
    }
    /* Invalid parameter */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }

    /* Build Command frame */
    aCmd[0] = (PHPAL_I18000P3M3_CMD_ACK << 6U) | (pData[0] >> 2U);    /* 0 bits left */
    aCmd[1] = (pData[0] << 6U) | (pData[1] >> 2U);                    /* 0 bits left */
    aCmd[2] = (pData[1] << 6U);                                      /* 6 bits left */

    /* Calculate BitLength */
    wCmdBitLength = 18;

    /* Set frame-sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Set TxWait to T2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        PHPAL_I18000P3M3_SW_T2_MIN_US));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I18000P3M3_SW_T1_MAX_US + PHPAL_I18000P3M3_SW_T3_MIN_US + PHPAL_I18000P3M3_SW_DELTA_MIN_US));

    /* Switch off HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Exchange command */
    status = phpalI18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        3,
        (uint8_t)(wCmdBitLength & 0x07U),
        ppRxBuffer,
        &wRxLength,
        &bRxLastBits);

    /* Switch on HAL-CRC from now on */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));

    /* set Tx-CRC as CRC16  */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC16|PHHAL_HW_CRCOPTION_INVERTED));

    /* Some error that can't be handled */
    PH_CHECK_SUCCESS(status);

    /* Get bit length */
    if (0U != (bRxLastBits & 0x07U))
    {
        --wRxLength;
    }
    *pRxBitLength = (uint16_t)((wRxLength << 3U) + bRxLastBits);

    /* Check response length */
    if ((*pRxBitLength < 5U) || (*pRxBitLength > 528U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_Nak(
                                  phpalI18000p3m3_Sw_DataParams_t * pDataParams
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bRxLastBits;

    /* Build Command frame */
    aCmd[0] = PHPAL_I18000P3M3_CMD_NAK; /* 0 bits left */

    /* Set frame-sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Disable TxWait */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        0));

    /* Set minimum Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        1));

    /* Switch off HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Exchange command */
    status = phpalI18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        0,
        &pResponse,
        &wRxLength,
        &bRxLastBits);

    /* Timeout is the correct behavior */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* if data was received return an error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18000P3M3);
        }
        else
        {
            /* Return the received error */
            return status;
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_ActivateCard(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t * pSelCmd,
    uint8_t bSelCmdLen,
    uint8_t bNumValidBitsinLastByte,
    uint8_t * pBeginRndCmd,
    uint8_t bTSprocessing,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxBufferLen,
    uint8_t * pMoreCardsAvailable
    )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_COUNT bQ = 0;
    uint8_t    PH_MEMLOC_REM *pRxBuffer = NULL;
    uint8_t    PH_MEMLOC_REM bCollisionFlag;
    uint16_t   PH_MEMLOC_REM wIndex;
    uint16_t   PH_MEMLOC_REM wTimeoutCount;
    uint8_t    PH_MEMLOC_REM bPcLength;
    uint8_t    PH_MEMLOC_REM bPacketCrcLen;
    uint8_t    PH_MEMLOC_REM bUiiStartAddress = 0x20;
    uint8_t    PH_MEMLOC_REM aUii[58] = {0};
    uint8_t    PH_MEMLOC_REM bCardFound = 0;
    uint8_t    PH_MEMLOC_REM bUiiLength = 0;
    uint8_t    PH_MEMLOC_REM mask_bByte1, mask_bByte2;
    uint8_t    PH_MEMLOC_REM bBuff1, bBuff2;
    uint8_t    PH_MEMLOC_REM pSelCmd_Int[39] = {0};
    uint8_t    PH_MEMLOC_REM bSelCmdLen_Int;
    uint16_t   PH_MEMLOC_REM wshift = 1U;
    uint8_t    PH_MEMLOC_REM bBreakFromLoop;

    PH_UNUSED_VARIABLE(bTSprocessing);

    pRxBuffer = *ppRxBuffer;

    /* Extract bQ value from pBeginRndCmd */
    bBuff1 = *(pBeginRndCmd + 1U) & (uint8_t)0x7;
    bBuff2 = *(pBeginRndCmd + 2U) & (uint8_t)0x80;

    bQ = (bBuff1 << 1U) | (bBuff2 >> 7U);

    /* Inventory 2^bQ slots */
    status = phhalHw_I18000p3m3Inventory(
        pDataParams->pHalDataParams,
        pSelCmd,
        (uint8_t)bSelCmdLen,
        bNumValidBitsinLastByte,
        pBeginRndCmd,
        PHHAL_HW_I18000P3M3_GET_MAX_RESPS,
        &pRxBuffer,
        pRxBufferLen);
    do
    {
        bCardFound = 0;

        /* Reset collision flag */
        bCollisionFlag = PH_OFF;

        /* Reset receive buffer index */
        wIndex = 0;

        /* Number of slots with timeout error */
        wTimeoutCount = 0;

        /* Return on timeout */
        if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            return status;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
        {
            /* Update more tags available on collision */
            bCollisionFlag = PH_ON;
        }
        else if(((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                || ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING))
        {
            bBreakFromLoop = 0U;
            while(wIndex < *pRxBufferLen)
            {
                if(pRxBuffer[wIndex] == 0U)
                {
                    if(0U == (bCardFound))
                    {
                        /* Partial bits shall be 0 as truncation is not set */
                        if(pRxBuffer[wIndex + 2U] != 0U)
                        {
                            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18000P3M3);
                        }

                        /* StoredPC/PacketPC length */
                        bPcLength = 2;

                        /* PacketCRC length */
                        bPacketCrcLen = 0;

                        /* Check for XPC Indicator (XI) */
                        if(0u != (pRxBuffer[wIndex + 3U] & 0x02U))
                        {
                            /* XPC_W1 length */
                            bPcLength += 2U;

                            /* PacketCRC length */
                            bPacketCrcLen = 2;

                            /* Check for XPC Extension Bit (XEB) */
                            if(0u != (pRxBuffer[wIndex + 5U] & 0x80U))
                            {
                                /* XPC_W2 length */
                                bPcLength += 2U;
                            }
                        }

                        if(pRxBuffer[wIndex + 1U] > (bPcLength + bPacketCrcLen))
                        {
                            /* To avoid case of corruption */
                            if((pRxBuffer[wIndex + 1U] - (bPcLength + bPacketCrcLen)) <= (uint8_t)sizeof(aUii))
                            {
                                /* Copy UII */
                                (void)memcpy(
                                    aUii,
                                    &pRxBuffer[wIndex + 3U + bPcLength],
                                    (size_t)(((uint32_t)(pRxBuffer[wIndex + 1U])) - ((uint32_t)bPcLength + (uint32_t)bPacketCrcLen)));

                                bUiiLength = pRxBuffer[wIndex + 1U] - (bPcLength + bPacketCrcLen);
                            }
                            else
                            {
                                /* For a compliant card this should never come */
                                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18000P3M3);
                            }
                        }
                        else
                        {
                            bUiiLength = 0;
                        }
                        bCardFound = 1;

                        /* Increment index to next slot */
                        wIndex =  (uint16_t)(wIndex + pRxBuffer[wIndex + 1U] + 3U);
                    }
                    else
                    {
                        *pMoreCardsAvailable = 1;
                        bBreakFromLoop = 1U;
                    }
                }
                else if(pRxBuffer[wIndex] == 1U)
                {
                    /* This should not come as ReqRN is not send */
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18000P3M3);
                }
                else if(pRxBuffer[wIndex] == 2U)
                {
                    /* Timeout */
                    wIndex = wIndex + 3U;
                    wTimeoutCount++;
                }
                else
                {
                    /* Collision */
                    *pMoreCardsAvailable = 1;

                    if(0U != (bCardFound))
                    {
                        bBreakFromLoop = 1U;
                    }
                    else
                    {
                        wIndex = wIndex + 3U;
                        bCollisionFlag = PH_ON;
                    }
                }

                if (bBreakFromLoop == 1U)
                {
                    /* Break from the Loop */
                    break;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

        if(0U == (bCardFound))
        {
            if(wTimeoutCount == (wshift << bQ))
            {
                /* Timeout */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_IO_TIMEOUT, PH_COMP_PAL_I18000P3M3);
            }

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
            {
                status = phhalHw_I18000p3m3ResumeInventory(
                    pDataParams->pHalDataParams,
                    &pRxBuffer,
                    pRxBufferLen);
            }
            else
            {
                /* Increment number of slots */
                bQ++;

                /* update pBeginRndCmd buffer with new bQ value */
                mask_bByte2 = (bQ & 0x1U) << 7U; /* bQ == 4bit MNOP */
                mask_bByte1 = (bQ & 0xEU) >> 1U; /* mask_bByte1 last 3 bits == MNO and mask_bByte2 msb bit == P */

                *(pBeginRndCmd + 1U) &= (uint8_t)~0x07u; /* Clear the bQ field from Cmd */
                *(pBeginRndCmd +2U) &= (uint8_t)~0x80u; /* Write the new bQ field */

                *(pBeginRndCmd +1U)|= mask_bByte1; /* Write the update value to pBeginRndCmd */
                *(pBeginRndCmd + 2U)|= mask_bByte2;

                /* Inventory */
                status = phhalHw_I18000p3m3Inventory(
                    pDataParams->pHalDataParams,
                    pSelCmd,
                    (uint8_t)bSelCmdLen,
                    bNumValidBitsinLastByte,
                    pBeginRndCmd,
                    PHHAL_HW_I18000P3M3_GET_MAX_RESPS,
                    &pRxBuffer,
                    pRxBufferLen);
            }
        }
    }
    while((bCollisionFlag != PH_OFF) && (bQ <= 0x0FU) && (bCardFound != 1U));

    /* Maximum mask length possible is 255 */
    if((bUiiLength * 8U) > 255U)
    {
        bUiiLength = 255;
    }
    else
    {
        bUiiLength = (uint8_t)(bUiiLength * 8U);
    }

    /* Create select command frame (with selected UII in mask) */
    PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateSelectCmd(
        pDataParams,
        PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
        0,
        PHPAL_I18000P3M3_MEMBANK_UII,
        &bUiiStartAddress,
        0,
        aUii,
        bUiiLength,
        0,
        pSelCmd_Int,
        &bSelCmdLen_Int,
        &bNumValidBitsinLastByte));

    /* Update pBeginRndCmd buffer with new bQ as '0' */
    pBeginRndCmd[1] &= (uint8_t)0xF8u;
    pBeginRndCmd[2] &= (uint8_t)0x00u;

    /* Activate tag with specified UII */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_I18000p3m3Inventory(
        pDataParams->pHalDataParams,
        pSelCmd_Int,
        (uint8_t)bSelCmdLen_Int,
        bNumValidBitsinLastByte,
        pBeginRndCmd,
        PHHAL_HW_I18000P3M3_GET_TAG_HANDLE,
        &pRxBuffer,
        pRxBufferLen));

    *ppRxBuffer = &pRxBuffer[*pRxBufferLen - 2U];
    *pRxBufferLen = 2U;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18000p3m3_Sw_SetConfig(
                                        phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint16_t wConfig,
                                        uint16_t wValue
                                        )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM bVal;

    switch (wConfig)
    {
    case PHPAL_I18000P3M3_CONFIG_TXLASTBITS:

        if (wValue > PHPAL_I18000P3M3_TXLASTBITS_MAX)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18000P3M3);
        }

        if (wValue > PHPAL_I18000P3M3_VALID_TXLASTBITS)
        {
            bVal = PH_OFF;
        }
        else
        {
            bVal = PH_ON;
        }
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_SET_SYMBOL_SEND,
            (uint16_t)bVal));

        status = phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXLASTBITS,
            (wValue & PHPAL_I18000P3M3_VALID_TXLASTBITS));
        break;

    case PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS:
        status = phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
            wValue);
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_I18000P3M3);
    }
    return status;
}

#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */
