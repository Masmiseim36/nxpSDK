/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2013
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/**
 *
 * \file phhalHw_PN7462AU.c : This file contains wrapper Api's which connects
 *                         the Reader library interfaces with the phhalRf interfaces.
 *
 * Project:  PN7462AU
 *
 * $Date$
 * $Author$
 * $Revision$ (v06.01.00)
 *
 */

/*********************************************************************************************************************/
/*   INCLUDES                                                                                                        */
/*********************************************************************************************************************/
#include "phhalHw.h"

#ifdef NXPBUILD__PHHAL_HW_PN7462AU

#include "phhalHw_PN7462AU.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phhalRf_LP_Int.h"
#include "phhalHw_PN7462AU_Int.h"

/*********************************************************************************************************************/
/*   DEFINES                                                                                                         */
/*********************************************************************************************************************/

phStatus_t phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
                                               phhalHw_PN7462AU_DataParams_t * pDataParams,
                                               uint16_t wOption,
                                               uint8_t * pTxBuffer,
                                               uint16_t wTxLength,
                                               uint8_t bTxLastBits,
                                               uint8_t ** ppRxBuffer,
                                               uint16_t * pRxLength,
                                               uint8_t * pRxLastBits
                                               )
{
    phStatus_t   statusTmp;
    phStatus_t   status;
    uint16_t     wValue;

    /* TxLastBits != 0 is invalid for buffered operation */
    if ((0U != ((wOption & PH_EXCHANGE_BUFFERED_BIT))) && (bTxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* TxLastBits > 7 is invalid  */
    if (bTxLastBits > 7U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Reset RxLastBits */
    if (pRxLastBits != NULL)
    {
        *pRxLastBits = 0;
    }

    /*  last Tx-Byte. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, bTxLastBits));

    /* Perform exchange */
    status = phhalHw_Exchange(
        pDataParams,
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
                pDataParams,
                PHHAL_HW_CONFIG_RXLASTBITS,
                &wValue));

            *pRxLastBits = (uint8_t)wValue;
        }

        /* Map to success status */
        status = PH_ERR_SUCCESS;
    }

    return status;
}

phStatus_t phhalHw_PN7462AU_18000p3m3_Sw_Select(
                                             phhalHw_PN7462AU_DataParams_t * pDataParams,
                                             uint8_t * pSelCmd,
                                             uint8_t bSelCmdLen,
                                             uint8_t bNumValidBitsinLastByte
                                             )
{
    phStatus_t   statusTmp;
    phStatus_t   status;
    uint8_t *    pResponse = NULL;
    uint16_t     wRxLength = 0;
    uint8_t      bRxLastBits;

    /* Set frame sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Disable TxWait */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        0));

    /* Set T4 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHHAL_HW_PN7462AU_I18000P3M3_SW_T4_MIN_US));

    /* Switch on HAL Tx-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));

    /* Switch off HAL Rx-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* set Tx-CRC as CRC16  */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC16|PHHAL_HW_CRCOPTION_INVERTED));

    /* Set CRC Preset for Select */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_PN7462AU_CRCPRESET_SEL, PHHAL_HW_PN7462AU_I18000P3M3_SELECT_PRESET));


    /* Exchange command */
    status = phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        pSelCmd,
        bSelCmdLen,
        (bNumValidBitsinLastByte & 0x07U),
        &pResponse,
        &wRxLength,
        &bRxLastBits);

    /* Reset back the CRC Preset for non-select commands */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_PN7462AU_CRCPRESET_SEL, PHHAL_HW_PN7462AU_I18000P3M3_NONSELECT_PRESET));

    /* Switch off HAL Tx-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));

    /* Timeout is the correct behavior */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* if data was received return an error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
        }
        else
        {
            /* Return the received error */
            return status;
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_PN7462AU_18000p3m3_Sw_BeginRound(
                                                 phhalHw_PN7462AU_DataParams_t * pDataParams,
                                                 uint8_t * pBeginRndCmd
                                                 )
{
    phStatus_t   statusTmp;
    uint16_t     wCmdBitLength = 0;
    uint8_t *    pResponse = NULL;
    uint16_t     wRxLength = 0;
    uint8_t      bRxLastBits;

    pDataParams->bSession = pBeginRndCmd[1] & (3U << 3U);

    /* Set preamble as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_PREAMBLE));

    /* Set TxWait to T2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        PHHAL_HW_PN7462AU_I18000P3M3_SW_T2_MIN_US));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHHAL_HW_PN7462AU_I18000P3M3_SW_T1_MAX_US + PHHAL_HW_PN7462AU_I18000P3M3_SW_T3_MIN_US + PHHAL_HW_PN7462AU_I18000P3M3_SW_DELTA_MIN_US));

    /* Switch ON HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC5));

    wCmdBitLength = 17;

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        pBeginRndCmd,
        3,
        (uint8_t)(wCmdBitLength & 0x07U),/* valid bits in last byte of beginround command */
        &pResponse,
        &wRxLength,
        &bRxLastBits));

    /* Check response length */
    if (((wRxLength - pDataParams->wRxBufStartPos) != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
    }

    /* Switch off HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Save StoredCRC */
    phUser_MemCpy(pDataParams->abStoredCRC, &pResponse[pDataParams->wRxBufStartPos], 2);  /* PRQA S 3200 */

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_PN7462AU_18000p3m3_Sw_NextSlot(
                                               phhalHw_PN7462AU_DataParams_t * pDataParams
                                               )
{
    phStatus_t   statusTmp;
    uint8_t      aCmd[1];
    uint16_t     wCmdBitLength = 0;
    uint8_t *    pResponse = NULL;
    uint16_t     wRxLength = 0;
    uint8_t      bRxLastBits;

    /* Build Command frame */
    aCmd[0] = (PHHAL_HW_PN7462AU_I18000P3M3_CMD_NEXT_SLOT << 4U) | (pDataParams->bSession << 2U); /* 2 bits left */

    /* Calculate BitLength */
    wCmdBitLength = 6;

    /* Set frame-sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Set TxWait to T2 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        PHHAL_HW_PN7462AU_I18000P3M3_SW_T2_MIN_US));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHHAL_HW_PN7462AU_I18000P3M3_SW_T1_MAX_US + PHHAL_HW_PN7462AU_I18000P3M3_SW_T3_MIN_US + PHHAL_HW_PN7462AU_I18000P3M3_SW_DELTA_MIN_US));

    /* Switch off TX and on RX HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC5));

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        (uint8_t)(wCmdBitLength & 0x07U),
        &pResponse,
        &wRxLength,
        &bRxLastBits));

    /* Check response length */
    if (((wRxLength - pDataParams->wRxBufStartPos) != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Save StoredCRC */
    phUser_MemCpy(pDataParams->abStoredCRC, &pResponse[pDataParams->wRxBufStartPos], 2);  /* PRQA S 3200 */

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_PN7462AU_18000p3m3_Sw_Ack(
                                          phhalHw_PN7462AU_DataParams_t *pDataParams,
                                          uint8_t **ppRxBuffer,
                                          uint16_t *pRxLength,
                                          uint8_t *pRxLastBits
                                          )
{
    phStatus_t   status;
    phStatus_t   statusTmp;
    uint8_t      aCmd[3];
    uint16_t     wCmdBitLength = 0;
    uint16_t     wRxLength = 0;
    uint8_t      bRxLastBits;

    /* Build Command frame */
    aCmd[0] = (PHHAL_HW_PN7462AU_I18000P3M3_CMD_ACK << 6U) | (pDataParams->abStoredCRC[0] >> 2U);    /* 0 bits left */
    aCmd[1] = (pDataParams->abStoredCRC[0] << 6U) | (pDataParams->abStoredCRC[1] >> 2U);                    /* 0 bits left */
    aCmd[2] = (pDataParams->abStoredCRC[1] << 6U);                                      /* 6 bits left */

    /* Calculate BitLength */
    wCmdBitLength = 18;

    /* Set frame-sync as start symbol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

    /* Set T1+T3 Timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHHAL_HW_PN7462AU_I18000P3M3_SW_T1_MAX_US + PHHAL_HW_PN7462AU_I18000P3M3_SW_T3_MIN_US + PHHAL_HW_PN7462AU_I18000P3M3_SW_DELTA_MIN_US));

    /* Switch off HAL-CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Exchange command */
    status = phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        3,
        (uint8_t)(wCmdBitLength & 0x07U),
        ppRxBuffer,
        &wRxLength,
        &bRxLastBits);

    /* Switch on HAL-CRC from now on */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));

    /* set Tx-CRC as CRC16  */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC16|PHHAL_HW_CRCOPTION_INVERTED));

    /* Incomplete byte is allowed */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        PH_CHECK_SUCCESS(status);
    }

    *pRxLength = wRxLength - pDataParams->wRxBufStartPos;
    *pRxLastBits = bRxLastBits;

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        wRxLength = (wRxLength - pDataParams->wRxBufStartPos) - 1U;
    }
    else
    {
        wRxLength = wRxLength - pDataParams->wRxBufStartPos;
    }

    /* Check response length */
    if ((((wRxLength * 8U) + *pRxLastBits) < 5U) ||
        (((wRxLength * 8U) + *pRxLastBits) > 528U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_PN7462AU_lI18000p3m3ReqRn(
                                          phhalHw_PN7462AU_DataParams_t * pDataParams,
                                          uint8_t ** ppRxBuffer
                                          )
{
    phStatus_t   statusTmp;
    uint8_t      bCmd[1];
    uint16_t     wRxByteLength = 0;
    uint8_t      bRxLastBits;

    /* Prepare and buffer command header */
    bCmd[0] = PHHAL_HW_PN7462AU_I18000P3M3_CMD_REQ_RN;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmd,
        1,
        0x00,
        NULL,
        NULL,
        NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_PN7462AU_CRCPRESET_SEL, 0x03));

    /* Append StoredCRC and perform the Exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_18000p3m3_Sw_Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pDataParams->abStoredCRC,
        2,
        0x00,
        ppRxBuffer,
        &wRxByteLength,
        &bRxLastBits));

    /* Check received length */
    if (((wRxByteLength - pDataParams->wRxBufStartPos) != 2U) || (bRxLastBits != 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    return PH_ERR_SUCCESS;
}


#endif /* NXPBUILD__PHHAL_HW_PN7462AU*/
