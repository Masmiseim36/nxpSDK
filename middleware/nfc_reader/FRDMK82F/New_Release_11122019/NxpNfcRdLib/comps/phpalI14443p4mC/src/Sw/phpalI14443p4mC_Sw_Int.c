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
* Software ISO 14443p4mC mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p4mC.h>
#include <ph_RefDefs.h>
#include <phDriver.h>


#ifdef NXPBUILD__PHPAL_I14443P4MC_SW

#include "phpalI14443p4mC_Sw_Int.h"

/* Frame size table */
static const uint16_t PH_MEMLOC_CONST_ROM aI14443p4mC_FsTable[PHPALI14443P4MC_FS_INDEX] = {14, 22, 30, 38, 46, 62, 94, 126, 254, 510, 1022, 2046, 4094};
/* Data rate table */
static const uint16_t PH_MEMLOC_CONST_ROM aI14443p4mC_DrTable[PHPALI14443P4MC_DR_INDEX] = {PHHAL_HW_RF_DATARATE_106, PHHAL_HW_RF_DATARATE_212, PHHAL_HW_RF_DATARATE_424, PHHAL_HW_RF_DATARATE_848};

phStatus_t phpalI14443p4mC_Sw_Int_PpsRes(
                                         phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                         uint8_t * pPps,
                                         uint8_t bPpsLength
                                         )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Validate PPS length */
    if((bPpsLength > 3U) || (bPpsLength < 2U))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Validate PPS in start byte */
    if((uint8_t)(pPps[0] & 0xF0U) != PHPALI14443P4MC_PPS_CMD)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Validate CID */
    if(0U != (pDataParams->bCidEnable))
    {
        if((pDataParams->bCid != (pPps[0] & 0x0FU)))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }
    else
    {
        if(0U != (pPps[0] & 0x0FU))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* Check for RFU values of PPS0 */
    if((pPps[1] & PHPALI14443P4MC_VALID_PPS0_MASK) != 0x01U)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Check for PPS1 presence in PPS0 */
    if((pPps[1] & (~PHPALI14443P4MC_VALID_PPS0_MASK)))
    {
        /* Validate Length */
        if(bPpsLength != 3U)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }

        /* Check for RFU values of PPS1 */
        if(0U != (pPps[2] & PHPALI14443P4MC_PPS_PPS2_MASK))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }

        /* Check DR and DS */
        if(!(pDataParams->bUnequalDrDs) && ((pPps[2] & 0x03U) != ((pPps[2] & 0x0CU) >> 2U)))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }

        /* Check if this DR support was specified during ATS */
        if(0U != (pPps[2] & 0x03U))
        {
            if(pDataParams->bDr & (uint8_t)(1U << (uint8_t)((pPps[2] & 0x03U) - 1)))
            {
                pDataParams->bDr = pPps[2] & 0x03U;
            }
            else
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }
        }
        else
        {
            pDataParams->bDr = 0;
        }

        /* Check if this DS support was specified during ATS */
        if((pPps[2] & 0x0CU) >> 2U)
        {
            if(pDataParams->bDs & (uint8_t)(1U << (uint8_t)((uint8_t)((pPps[2] & 0x0CU) >> 2U) - 1)))
            {
                pDataParams->bDs = ((pPps[2] & 0x0CU) >> 2U);
            }
            else
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }
        }
        else
        {
            pDataParams->bDs = 0;
        }
    }
    else
    {
        pDataParams->bDs = 0;
        pDataParams->bDr = 0;
    }

    /* Send PPS response */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        pPps,
        PHPALI14443P4MC_PPS_LEN));

    /* Set DS data rate */
    if(0U != (pDataParams->bDs))
    {
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXDATARATE_FRAMING,
            aI14443p4mC_DrTable[pDataParams->bDs] | PHHAL_HW_RF_TYPE_A_FRAMING));
    }

    /* Set DR data rate */
    if(0U != (pDataParams->bDr))
    {
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            aI14443p4mC_DrTable[pDataParams->bDr] | PHHAL_HW_RF_TYPE_A_FRAMING));
    }

    /* Set internal state */
    pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_RX;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_DslRes(
                                         phpalI14443p4mC_Sw_DataParams_t * pDataParams
                                         )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM bDeSel[2] = {0xC2, 0x00};
    uint16_t   PH_MEMLOC_REM wDeSelLen = 1;

    /* Build S frame, add CID if enabled */
    if(0U != (pDataParams->bCidPresence))
    {
        bDeSel[0] = (bDeSel[0] | PHPALI14443P4MC_BLOCK_HAS_CID);
        bDeSel[1] = pDataParams->bCid;
        wDeSelLen = 2;
    }

    /* Send DSL response */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_TRANSMIT_DEFAULT,
        bDeSel,
        wDeSelLen));

    /* Set state to NONE / initial sate */
    pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_NONE;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_Ack(
                                      phpalI14443p4mC_Sw_DataParams_t * pDataParams
                                      )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM bRAck[2] = {PHPALI14443P4MC_BLOCK_R_PCB_ACK, 0x00};
    uint16_t   PH_MEMLOC_REM wRAckLen = 1;

    /* prepare new R frame */
    bRAck[0] = (bRAck[0] | pDataParams->bBlockNr);

    /* add CID, if required */
    if (0U != (pDataParams->bCidPresence))
    {
        bRAck[0] = (bRAck[0] | PHPALI14443P4MC_BLOCK_HAS_CID);
        bRAck[1] = pDataParams->bCid;
        wRAckLen = 2;
    }

    /* Transmit data with PCD */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_TRANSMIT_DEFAULT,
        bRAck,
        wRAckLen));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_StartWtxTimer(
                                                phpalI14443p4mC_Sw_DataParams_t * pDataParams
                                                )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Start the micro seconds Timer in single shot mode. */
    PH_CHECK_SUCCESS_FCT(status, phDriver_TimerStart( PH_DRIVER_TIMER_MICRO_SECS,
            (uint32_t)(((pDataParams->bWtPercentage
                    * (PHPAL_I14443P4MC_SW_CALCULATE_FWT_US(pDataParams->bFwi))
                    * pDataParams->bWtx) / 100) - pDataParams->wWtDelta),
                    (pphDriver_TimerCallBck_t)(pDataParams->pWtxTimerCallback)));

    return PH_ERR_SUCCESS;
}


phStatus_t phpalI14443p4mC_Sw_Int_HandleRxChaining(
                                                   phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                                   uint16_t wOption,
                                                   uint8_t ** ppRxBuffer,
                                                   uint16_t * pRxLength
                                                   )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wHalRxBufferSize;
    uint8_t     PH_MEMLOC_REM bHeaderLength;

    uint8_t     PH_MEMLOC_REM aRxOverlapped[4];
    uint16_t    PH_MEMLOC_REM wRxOverlappedStartPos = 0;
    uint8_t     PH_MEMLOC_REM bBufferOverflow = 0;
    uint8_t   * PH_MEMLOC_REM pRawResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRawResponseLength = 0;
    uint8_t   * PH_MEMLOC_REM pPayloadResponse = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t     PH_MEMLOC_REM bRestartReceive = 0;

    /* Get HAL RX buffer size */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
        &wHalRxBufferSize));

    /* Forecast header length */
    PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_GetPduHeaderLength(
        pDataParams,
        PH_OFF,
        &bHeaderLength));

    wRawResponseLength = *pRxLength;
    pRawResponse = *ppRxBuffer;

    /* ******************************** */
    /*          RECEPTION LOOP          */
    /* ******************************** */
    do
    {
        if((!bBufferOverflow) && !(bRestartReceive))
        {
            /* Return with chaining status if the next chain may not fit into our buffer */
            if((wOption & PH_EXCHANGE_MODE_MASK) != PH_RECEIVE_CHAINING_BUFSIZE)
            {
                if((*pRxLength + aI14443p4mC_FsTable[pDataParams->bFsci]) > wHalRxBufferSize)
                {
                    /* Reset RxBuffer Start Position */
                    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                            pDataParams->pHalDataParams,
                            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                            0));

                    return (PH_ERR_SUCCESS_CHAINING | PH_COMP_PAL_I14443P4MC);
                }
            }

            /* Calculate start-position for overlapping */
            wRxOverlappedStartPos = wRawResponseLength - (uint16_t)bHeaderLength;

            /* Backup overlapped bytes */
            (void)memcpy(aRxOverlapped, &pRawResponse[wRxOverlappedStartPos], bHeaderLength);

            /* Adjust RX buffer start position. */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wRxOverlappedStartPos));
        }

        /* Send ACK */
        if(0U == (bRestartReceive))
        {
            status = phpalI14443p4mC_Sw_Int_Ack(pDataParams);
        }

        if(0U == (bBufferOverflow))
        {
            /* Adjust RX buffer start position. */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wRxOverlappedStartPos));
        }

        /* Receive chained data */
        status = phhalHw_Receive(
             pDataParams->pHalDataParams,
             PH_RECEIVE_DEFAULT,
             &pRawResponse,
             &wRawResponseLength);

        /* Reset RX restart flag */
        bRestartReceive = 0;

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Validate received frame */
            status = phpalI14443p4mC_Sw_Int_ValidateFrame(
                pDataParams,
                &pRawResponse[wRxOverlappedStartPos],
                (wRawResponseLength - wRxOverlappedStartPos),
                &pPayloadResponse,
                &wPayloadLength);
        }

        if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_DESELECT)
        {
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                0));

            /* Send S-Block DESELECT and exit */
            PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_DslRes(pDataParams));
            return (PH_ERR_SUCCESS_DESELECTED | PH_COMP_PAL_I14443P4MC);
        }

        /* Exit on EXT RF off */
        if((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
        {
            /* Application shall reset data parameters of all layers and
             * shall restart listen/discovery loop on RF error */
            return (PH_ERR_EXT_RF_ERROR | PH_COMP_PAL_I14443P4MC);
        }
        else if((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
        {
            /* Application shall reset data parameters of all layers and
             * shall restart listen/discovery loop on RF error */
            return (PH_ERR_EXT_RF_ERROR | PH_COMP_PAL_I14443P4MC);
        }
        else if ((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW)
        {
            /* Complete chaining even if buffer is full */
            if((wOption & PH_EXCHANGE_MODE_MASK) == PH_RECEIVE_CHAINING_BUFSIZE)
            {
                /* Reset wRxStartPos */
                PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                    0));

                /* Indicate Buffer Overflow */
                bBufferOverflow = 1;
            }
            else
            {
                return status;
            }
        }

        else
        {
            /* On all other errors, restart receive and back to receive mode */
            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
            {
                bRestartReceive = 1;
            }

            /* Next chunk of PDU has been received */
            if ((!bBufferOverflow) && !(bRestartReceive))
            {
                /* Restore overlapped bytes */
                (void)memcpy(&pRawResponse[wRxOverlappedStartPos], aRxOverlapped, bHeaderLength);  /* PRQA S 3200 */ /* PRQA S 3354 */

                /* Increment length */
                *pRxLength = *pRxLength + wPayloadLength;
            }
        }
    }
    while(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_RXCHAINING ||
           pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_TX_R);

    /* Reset RxBuffer Start Position */
    if (wRxOverlappedStartPos != 0U)
    {
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            0));
    }

    /* Return data */
    if (0U != (bBufferOverflow))
    {
        *pRxLength = 0;
        return (PH_ERR_BUFFER_OVERFLOW | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_GetPduHeaderLength(
                                                     phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                                     uint8_t bNadInclude,
                                                     uint8_t * pProtLength
                                                     )
{
    /* PCB length */
    *pProtLength = 1;

    /* Add CID if enabled */
    if(0U != (pDataParams->bCidPresence))
    {
        ++(*pProtLength);
    }

    /* Add NAD if enabled and chaining is not used. */
    if ((pDataParams->bNadPresence) && (bNadInclude))
    {
        ++(*pProtLength);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_ValidateFrame(
                                                phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                                uint8_t * pRxBuffer,
                                                uint16_t wRxLength,
                                                uint8_t ** ppPayload,
                                                uint16_t * pPayloadLength
                                                )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM bCheckNad;
    uint8_t    PH_MEMLOC_REM bBytePos;
   uint16_t   PH_MEMLOC_REM wHeaderLength = 1; /* PCB (1 byte) */

    /* Check for empty frame */
    if((0U == wRxLength) || (NULL == pRxBuffer))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Check for PPS Request */
    if(PHPALI14443P4MC_PPS_CMD == (uint8_t)(pRxBuffer[0] & 0xF0U))
    {
        if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_ATS)
        {
            /* Change the State to PPS */
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_PPS;
            return PH_ERR_SUCCESS;
        }
        else
        {
            /* Protocol error, as we have not received PPS after RATS */
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* Check for WTX */
    if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_WTX)
    {
        /* Return protocol error for frames other than WTX and NAK */
        if(((pRxBuffer[0] & 0xF0U) != 0xF0U) && ((pRxBuffer[0] & 0xF0U) != 0xB0U))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* Check for TX chaining */
    if((pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_TXCHAINING)
       && ((pRxBuffer[0] & PHPALI14443P4MC_BLOCK_MASK)
           != (PHPALI14443P4MC_BLOCK_R & PHPALI14443P4MC_BLOCK_MASK)))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Check if CID is present */
    switch (pRxBuffer[0] & PHPALI14443P4MC_BLOCK_MASK)
    {
    case (PHPALI14443P4MC_BLOCK_I & PHPALI14443P4MC_BLOCK_MASK):
    case (PHPALI14443P4MC_BLOCK_R & PHPALI14443P4MC_BLOCK_MASK):
    case (PHPALI14443P4MC_BLOCK_S & PHPALI14443P4MC_BLOCK_MASK):
        if (0U != (pRxBuffer[0] & PHPALI14443P4MC_BLOCK_HAS_CID))
        {
            /* Check if CID is supported */
            if(pDataParams->bCidEnable == PH_OFF)
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }

            /* Check received block length */
            if (wRxLength < 2U)
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }

            /* Check if CID value is correct */
            if ((pRxBuffer[1] & PHPALI14443P4MC_CID_MASK) != pDataParams->bCid)
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }

            /* Update CID presence in command */
            pDataParams->bCidPresence = TRUE;
        }
        else
        {
            if((pDataParams->bCidEnable) && (pDataParams->bCid != 0U))
            {
                    return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }

            /* Update CID absence in command */
            pDataParams->bCidPresence = FALSE;
        }

        /* Validate received length */
        if(wRxLength > aI14443p4mC_FsTable[pDataParams->bFsci])
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
        break;

    default:
        /* Unknown block format */
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    switch (pRxBuffer[0] & PHPALI14443P4MC_BLOCK_MASK)
    {
        /* Process I frame */
    case (PHPALI14443P4MC_BLOCK_I & PHPALI14443P4MC_BLOCK_MASK):
        if (0U != (pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_NAD_FOLLOWING))
        {
            /* NAD present only in normal I-Block and first Block of chaining */
            if((pDataParams->bNadEnable) && !(pDataParams->bChainingRx))
            {
                bBytePos = (pDataParams->bCidPresence == TRUE)? 2: 1;

                /* Validate received block length */
                if(wRxLength < (bBytePos + 1U))
                {
                    return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
                }

                /* Validate NAD RFU bits b4 and b8 */
                if((pRxBuffer[bBytePos] & 0x88U) != 0x00U)
                {
                    return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
                }

                /* Validate SAD and DAD */
                if(((pRxBuffer[bBytePos] & 0x07U) == ((pRxBuffer[bBytePos] & 0x70U) >> 4U))
                   && (pRxBuffer[bBytePos] != 0x00U))
                {
                    return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
                }

                bCheckNad = 1;
                pDataParams->bNad = pRxBuffer[bBytePos];
                pDataParams->bNadPresence = TRUE;
            }
            else
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }
        }
        else
        {
            bCheckNad = 0;

            /* If NAD bit is not set in PCB and not in RX chaining, reset NAD presence. */
            if(!(pDataParams->bChainingRx))
            {
                pDataParams->bNadPresence = FALSE;
            }
        }

        /* Check if I-Block is valid */
        PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_IsValidIBlock(
            pDataParams->bOpMode,
            pDataParams->bCidPresence,
            pDataParams->bCid,
            bCheckNad,
            pDataParams->bNad,
            pRxBuffer,
            wRxLength));

        /* ISO14443-4 7.5.3.2 Rule D */
        pDataParams->bBlockNr = !(pDataParams->bBlockNr);

        if (0U != (pRxBuffer[0] & PHPALI14443P4MC_BLOCK_IS_CHAIN))
        {
            /* if PCD is chaining return data and set next state to R frame send */
            pDataParams->bChainingRx = 1;
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_RXCHAINING;
        }
        else
        {
            /* normal frame received, next state is send I frame */
            pDataParams->bChainingRx = 0;
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_I;
        }

        /* Calculate header length */
        if ((pDataParams->bNadPresence) && (bCheckNad == 1U))
        {
            wHeaderLength++;
        }
        break;

    case (PHPALI14443P4MC_BLOCK_R & PHPALI14443P4MC_BLOCK_MASK):
        /* Check if R-Block is valid */
        PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_IsValidRBlock(
            pDataParams->bOpMode,
            pDataParams->bCidPresence,
            pDataParams->bCid,
            pRxBuffer,
            wRxLength));

        if (0U != (pDataParams->bChainingTx))
        {
            /* PICC is chaining data, check if PCD received data.
             * ISO14443-4 7.5.4.3 Rule 12 and 13. */
            if (((pRxBuffer[0] & PHPALI14443P4MC_R_NAK_MASK) == 0U) &&
                (pRxBuffer[0] & PHPALI14443P4MC_BLOCKNUMBER_MASK) != pDataParams->bBlockNr)
            {
                pDataParams->bBlockNr = !pDataParams->bBlockNr;
                pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_I;
            }
            else if (((pRxBuffer[0] & PHPALI14443P4MC_R_NAK_MASK) != 0U) &&
                (pRxBuffer[0] & PHPALI14443P4MC_BLOCKNUMBER_MASK) != pDataParams->bBlockNr)
            {
            /* As per rule 7.5.4.3, Rule 12
            / * Received R(NAK) with block number not equal to the PICC’s current block number*/
            /* Send R(ACK) Block without toggling block number as per Sec 7.5.4.2 NOTE 2. */
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_R;
            }
            else
            {
                /* Something is wrong, re-send current frame.
                 * ISO14443-4 7.5.4.3 Rule 11. */
                pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_RESEND;
            }
        }
        else if (0U != (pDataParams->bChainingRx))
        {
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_R;
        }
        else if ((pRxBuffer[0] & PHPALI14443P4MC_BLOCKNUMBER_MASK) == pDataParams->bBlockNr)
        {
            /* Something is wrong, re-send current frame.
             * ISO14443-4 7.5.4.3 Rule 11. */
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_RESEND;
        }
        else if ((pRxBuffer[0] & PHPALI14443P4MC_R_NAK_MASK)
                 && (pRxBuffer[0] & PHPALI14443P4MC_BLOCKNUMBER_MASK) != pDataParams->bBlockNr)
        {
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_R;
        }
        else
        {
            /* Do Nothing */
        }
        break;

    case (PHPALI14443P4MC_BLOCK_S & PHPALI14443P4MC_BLOCK_MASK):
        /* Check if S-Block is valid */
        PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_IsValidSBlock(
            pDataParams->bOpMode,
            pDataParams->bCidPresence,
            pDataParams->bCid,
            pDataParams->bWtx,
            pRxBuffer,
            wRxLength));

        if((pRxBuffer[0] & PHPALI14443P4MC_BLOCK_S_MASK) == PHPALI14443P4MC_BLOCK_S_DESELECT)
        {
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_DESELECT;
        }
        else if((pRxBuffer[0] & PHPALI14443P4MC_BLOCK_S_MASK) == PHPALI14443P4MC_BLOCK_S_WTX)
        {
            /* Check if WTX response is received after a valid WTX request */
            if((pDataParams->bStateNow != PHPAL_I14443P4MC_STATE_WTX)
               && (pDataParams->bStateNow != PHPAL_I14443P4MC_STATE_RESEND))
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
            }
            else
            {
                /* Restore previous state (state before WTX) */
                if(0U != (pDataParams->bChainingRx))
                {
                    pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_RXCHAINING;
                }
                else
                {
                    pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_I;
                }
            }
        }
        else
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
        break;

    default:
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Calculate header length */
    if (0U != (pDataParams->bCidPresence))
    {
        wHeaderLength++;
    }

    /* Update pay-load */
    if((ppPayload != NULL) && (pPayloadLength != NULL))
    {
        *ppPayload = &pRxBuffer[wHeaderLength];

        if(wRxLength >= wHeaderLength)
        {
            *pPayloadLength = (uint16_t) (wRxLength - wHeaderLength);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_IsValidIBlock(
                                                uint8_t bOpMode,
                                                uint8_t bCheckCid,
                                                uint8_t bCid,
                                                uint8_t bCheckNad,
                                                uint8_t bNad,
                                                uint8_t * pRxBuffer,
                                                uint16_t wRxLength
                                                )
{
    uint16_t PH_MEMLOC_REM wExpRxLength = 0;

    /* Check constant bit b2 of PCB is set to '1' */
    if ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & 0x02U) != 0x02U)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Check RFU bits only for ISO mode */
    if(bOpMode == RD_LIB_MODE_ISO)
    {
        /* Check RFU bit b6 of PCB is set to '0' */
        if ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & 0x20U) != 0x00U)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* We always expect the PCB byte to be present */
    wExpRxLength = 1;

    /* Enable CID checking if necessary */
    if (0U != (bCheckCid))
    {
        wExpRxLength++;
    }

    /* Enable NAD checking if necessary */
    if (0U != (bCheckNad))
    {
        wExpRxLength++;
    }

    /* The frame should have the minimum frame length */
    if (wRxLength < wExpRxLength)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* CID presence check */
    if ((0u != bCheckCid) &&
        (pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING) &&
        ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS+1U] & PHPALI14443P4MC_CID_MASK) == bCid))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID absence check */
    else if ((0u == bCheckCid) && !(pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID protocol error */
    else
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* NAD presence check */
    if ((bCheckNad) &&
        (pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_NAD_FOLLOWING) &&
        ((pRxBuffer[wExpRxLength-1u] & PHPALI14443P4MC_NAD_MASK) == bNad))
    {
        /* CHECK SUCCEEDED */
    }
    /* NAD absence check */
    else if ((0u == bCheckNad) && !(pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_NAD_FOLLOWING))
    {
        /* CHECK SUCCEEDED */
    }
    /* NAD protocol error */
    else
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_IsValidRBlock(
                                                uint8_t bOpMode,
                                                uint8_t bCheckCid,
                                                uint8_t bCid,
                                                uint8_t * pRxBuffer,
                                                uint16_t wRxLength
                                                )
{
    uint16_t PH_MEMLOC_REM wExpRxLength = 0;

    /* Check constant bits of PCB - b6 is set to '1' and b3 to '0' */
    if ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & 0x24U) != 0x20U)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Check RFU bits only for ISO mode */
    if(bOpMode == RD_LIB_MODE_ISO)
    {
        /* Check RFU bit b2 of PCB is set to '1' */
        if ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & 0x02U) != 0x02U)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* We always expect the PCB byte to be present */
    wExpRxLength = 1;

    /* If CID is enabled, we always expect it */
    if (0U != (bCheckCid))
    {
        wExpRxLength++;
    }

    /* The frame should have the exact frame length */
    if (wRxLength != wExpRxLength)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* CID presence check */
    if ((0u != bCheckCid) &&
        (pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING) &&
        ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS+1U] & PHPALI14443P4MC_CID_MASK) == bCid))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID absence check */
    else if ((0u == bCheckCid) && !(pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID protocol error */
    else
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Int_IsValidSBlock(
                                                uint8_t bOpMode,
                                                uint8_t bCheckCid,
                                                uint8_t bCid,
                                                uint8_t bWtx,
                                                uint8_t * pRxBuffer,
                                                uint16_t wRxLength
                                                )
{
    uint16_t PH_MEMLOC_REM wExpRxLength = 0;

    /* Check constant bit b3 of PCB is set to '0' */
    if ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & 0x04U) != 0x00U)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* Check RFU bits only for ISO mode */
    if(bOpMode == RD_LIB_MODE_ISO)
    {
        /* Check RFU bits of PCB - b2 is set to '1' and b1 is set to 0 */
        if ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & 0x03U) != 0x02U)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* Check if DESELECT or WTX bits */
    if (!((((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_WTX) != PHPAL_I14443P4MC_SW_PCB_DESELECT)
            || (pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_WTX) != PHPAL_I14443P4MC_SW_PCB_WTX)))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* We always expect the PCB byte to be present */
    wExpRxLength = 1;

    /* If CID is enabled, we always expect it */
    if (0U != (bCheckCid))
    {
        wExpRxLength++;
    }

    /* If this is a WTX request, we expect an additional INF byte */
    if (PHPAL_I14443P4MC_SW_IS_WTX(pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS]))
    {
        if (((pRxBuffer[wExpRxLength] & PHPAL_I14443P4MC_SW_WTX_VALUE_MASK) != bWtx) ||
                ((pRxBuffer[wExpRxLength] & PHPAL_I14443P4MC_SW_WTX_RFU_MASK) != 0U))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
        }
        wExpRxLength++;
    }

    /* The frame should have the exact frame length */
    if (wRxLength != wExpRxLength)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* CID presence check */
    if ((0u != bCheckCid) &&
        (pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING) &&
        ((pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS+1U] & PHPALI14443P4MC_CID_MASK) == bCid))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID absence check */
    else if ((0u == bCheckCid) && !(pRxBuffer[PHPAL_I14443P4MC_SW_PCB_POS] & PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID protocol error */
    else
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */
