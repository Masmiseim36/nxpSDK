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
*  KK: Generated 25. March 2014
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p4mC.h>
#include <ph_RefDefs.h>
#include <phDriver.h>


#ifdef NXPBUILD__PHPAL_I14443P4MC_SW

#include "phpalI14443p4mC_Sw.h"
#include "phpalI14443p4mC_Sw_Int.h"

static const uint8_t bmcEventName[] = "mcEvent";
/* Frame size table */
static const uint16_t PH_MEMLOC_CONST_ROM aI14443p4mC_FsTable[PHPALI14443P4MC_FS_INDEX] = {14, 22, 30, 38, 46, 62, 94, 126, 254, 510, 1022, 2046, 4094};

phStatus_t phpalI14443p4mC_Sw_Init(
                                   phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                   uint16_t wSizeOfDataParams,
                                   void * pHalDataParams,
                                   pWtxTimerCallback pWtxCallback
                                   )
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    if (sizeof(phpalI14443p4mC_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_PAL_I14443P4MC);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* Initialize data parameters */
    pDataParams->wId                = PH_COMP_PAL_I14443P4MC | PHPAL_I14443P4MC_SW_ID;
    pDataParams->pHalDataParams     = pHalDataParams;
    pDataParams->pWtxTimerCallback  = pWtxCallback;

    pDataParams->bWtx               = PHPAL_I14443P4MC_SW_MIN_WTX_VALUE;
    pDataParams->bDisableWtx        = PH_OFF;
    /* Default operating mode is NFC */
    pDataParams->bOpMode            = RD_LIB_MODE_NFC;
    /* By default delta time to be subtracted from WTX timer time is set to 0 */
    pDataParams->wWtDelta           = 0;
    /* Default percentage of actual time to be used in WTX timer is set to 75% */
    pDataParams->bWtPercentage      = 75;

    /* Reset protocol to defaults */
    (void)phpalI14443p4mC_Sw_ResetProtocol(pDataParams);

    /* check whether event is already created. */
    pDataParams->mcEventObj.intialValue = 0;
    pDataParams->mcEventObj.pEvtName = (uint8_t *)bmcEventName;
    wStatus = phOsal_EventCreate(&pDataParams->mcEventObj.EventHandle, &pDataParams->mcEventObj);

    return wStatus;
}

phStatus_t phpalI14443p4mC_Sw_ResetProtocol(
                                            phpalI14443p4mC_Sw_DataParams_t * pDataParams
                                            )
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
    pDataParams->bStateNow        = PHPAL_I14443P4MC_STATE_NONE;
    /* Default block number is 1. ISO14443-4 7.5.3.2. Rule C */
    pDataParams->bBlockNr         = 1;
    pDataParams->bFsdi            = 0;
    /* Default FSCI value is 2 */
    pDataParams->bFsci            = PHPALI14443P4MC_FSCI_DEFAULT;
    pDataParams->bDr              = PHPAL_I14443P4MC_106;
    pDataParams->bDs              = PHPAL_I14443P4MC_106;
    pDataParams->bUnequalDrDs     = 0;
    pDataParams->bFwi             = PHPALI14443P4MC_FWI_DEFAULT;
    pDataParams->bSfgi            = PHPALI14443P4MC_SFGI_DEFAULT;
    pDataParams->bCid             = 0;
    pDataParams->bNad             = 0;
    pDataParams->bChainingRx      = 0;
    pDataParams->bChainingTx      = 0;
    /* Default CID supported ISO14443-4 5.2.6 */
    pDataParams->bCidEnable       = PH_ON;
    /* Default NAD not supported ISO14443-4 5.2.6 */
    pDataParams->bNadEnable       = PH_OFF;
    pDataParams->bCidPresence     = FALSE;
    pDataParams->bNadPresence     = FALSE;

    /* Reset the position in case the PICC stopped abruptly due to RF OFF during chaining */
    (void)phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0);

    return wStatus;
}

phStatus_t phpalI14443p4mC_Sw_Activate(
                                       phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                       uint8_t * pRats,
                                       uint8_t bRatsLength,
                                       uint8_t * pAts,
                                       uint16_t wAtsLength
                                       )
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    uint8_t PH_MEMLOC_REM bIndex;

    /* Check for valid state */
    if(pDataParams->bStateNow != PHPAL_I14443P4MC_STATE_NONE)
    {
        return (PH_ERR_USE_CONDITION | PH_COMP_PAL_I14443P4MC);
    }

    /* Validate RATS */
    if((bRatsLength != PHPALI14443P4MC_RATS_LEN) || (pRats[0] != PHPALI14443P4MC_RATS_CMD))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }

    /* FSDI RFU value check and update FSDI */
    if(pDataParams->bOpMode == RD_LIB_MODE_NFC)
    {
        if(((pRats[1] & 0xF0U) >> 0x04U) > 0x08U)
        {
            pDataParams->bFsdi = 0x08;
        }
        else
        {
            pDataParams->bFsdi = (uint8_t)((uint8_t)(pRats[1] & 0xF0U) >> 0x04U);
        }
    }
    else
    {
        if(((pRats[1] & 0xF0U) >> 0x04U) > 0x0CU)
        {
            pDataParams->bFsdi = 0x0C;
        }
        else
        {
            pDataParams->bFsdi = (uint8_t)((uint8_t)(pRats[1] & 0xF0U) >> 0x04U);
        }
    }

    /* Validate and update CID */
    if((pRats[1] & 0x0FU) > 0x0EU)
    {
        /* Device shall be put to HALT or IDLE state (i.e. application shall
         * restart listen mode) */
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I14443P4MC);
    }
    else
    {
        pDataParams->bCid = (pRats[1] & 0x0FU);
    }

    /* Validation of ATS length */
    if((!wAtsLength) || (wAtsLength !=  pAts[PHPALI14443P4MC_TL_IDX]))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }

    /* Validate ATS length */
    if(wAtsLength > aI14443p4mC_FsTable[pDataParams->bFsdi])
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }
    if(pDataParams->bOpMode == RD_LIB_MODE_NFC)
    {
        if((wAtsLength < PHPALI14443P4MC_NFC_MIN_ATS_LEN)
            || (wAtsLength > PHPALI14443P4MC_NFC_MAX_ATS_LEN))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }
    }

    /* Update the length of ATS validated till now */
    bIndex = 1;

    /* Check and validate (T0), TA(1), TB(1) and TC(1) */
    if(wAtsLength > 1U)
    {
        /* Increment length/index */
        bIndex++;

        /* Check if RFU bit of T0 is set */
        if(0U != (pAts[1] & 0x80U))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }

        /* FSCI RFU value check and update FSCI */
        if(pDataParams->bOpMode == RD_LIB_MODE_NFC)
        {
            /* Check for FSC maximum */
            if((pAts[1] & 0x0FU) > PHPALI14443P4MC_FSC_MAX_NFC_FORUM)
            {
                pDataParams->bFsci = PHPALI14443P4MC_FSC_MAX_NFC_FORUM;
            }
            else
            {
                pDataParams->bFsci = (pAts[1] & 0x0FU);
                /* Check if FSC (FSCT4AT,MIN) criteria is met and assign FSCT4AT,MIN if not.  */
                if (pDataParams->bFsci < PHPALI14443P4MC_FSC_MIN_NFC_FORUM)
                {
                    pDataParams->bFsci = PHPALI14443P4MC_FSC_MIN_NFC_FORUM;
                }
            }
        }
        else
        {
            if((pAts[1] & 0x0FU) > 0x0CU)
            {
                pDataParams->bFsci = 0x0C;
            }
            else
            {
                pDataParams->bFsci = (pAts[1] & 0x0FU);
            }
        }

        /* Check TA(1) */
        if(0U != (pAts[PHPALI14443P4MC_T0_IDX] & 0x10U))
        {
            /* Increment length/index */
            bIndex++;

            /* Validate length */
            if(wAtsLength < bIndex)
            {
                return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
            }

            /* Check if RFU bit is set */
            if(0U != (pAts[bIndex - 1] & 0x08U))
            {
                return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
            }

            /* For NFC mode, DR and DS shall be zero */
            if((pDataParams->bOpMode == RD_LIB_MODE_NFC) && ((pAts[bIndex - 1]) & 0x77U))
            {
                return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
            }

            /* Update DR and DS */
            pDataParams->bDr = pAts[bIndex - 1] & 0x07U;
            pDataParams->bDs = (pAts[bIndex - 1] & 0x70U) >> 4U;

            /* Check if DR and DS are same  */
            if(pDataParams->bDr != pDataParams->bDs)
            {
                pDataParams->bUnequalDrDs = 1;
            }
            else
            {
                pDataParams->bUnequalDrDs = 0;
            }
        }

        /* Check TB(1) */
        if(0U != (pAts[PHPALI14443P4MC_T0_IDX] & 0x20U))
        {
            /* Increment length/index */
            bIndex++;

            /* Validate length */
            if(wAtsLength < bIndex)
            {
                return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
            }

            /* Validate SFGI and FWI */
            if(pDataParams->bOpMode == RD_LIB_MODE_NFC)
            {
                /* For NFC, maximum SFGI shall be 8 */
                if(((pAts[bIndex - 1] & 0x0FU) > 0x08U) || (((pAts[bIndex - 1] & 0xF0U) >> 0x04U) > 0x08U))
                {
                    return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
                }
            }
            else
            {
                /* SFGI shall be from 0 - 14 */
                if(((pAts[bIndex - 1] & 0x0FU) > 0x0EU) || (((pAts[bIndex - 1] & 0xF0U) >> 0x04U) > 0x0EU))
                {
                    return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
                }
            }

            /* Update SFGI and FWI */
            pDataParams->bSfgi = (uint8_t)((uint8_t)pAts[bIndex - 1] & 0x0FU);
            pDataParams->bFwi  = (uint8_t)(((uint8_t)(pAts[bIndex - 1] & 0xF0U)) >> 0x04U);
        }

        /* Check TC(1) */
        if(0U != (pAts[PHPALI14443P4MC_T0_IDX] & 0x40U))
        {
            /* Increment length/index */
            bIndex++;

            /* Validate length */
            if(wAtsLength < bIndex)
            {
                return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
            }

            /* Check RFU bits */
            if(0U != (pAts[bIndex - 1] & 0xFCU))
            {
                return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
            }

            /* Check CID/DID support */
            if(0U != (pAts[bIndex - 1] & 0x02U))
            {
                pDataParams->bCidEnable = 1;
            }
            else
            {
                pDataParams->bCidEnable = 0;
            }

            /* Check NAD support */
            if(0U != (pAts[bIndex - 1] & 0x01U))
            {
                if(pDataParams->bOpMode == RD_LIB_MODE_NFC)
                {
                    /* NAD is not supported by NFC */
                    return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
                }
                pDataParams->bNadEnable = 1;
            }
            else
            {
                pDataParams->bNadEnable = 0;
            }
        }
    }

    /* Maximum supported historical bytes length is 15 for NFC */
    if((pDataParams->bOpMode == RD_LIB_MODE_NFC) && (wAtsLength - bIndex) > 15U)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }

    /* Send ATS */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_TRANSMIT_BUFFER_LAST,
        pAts,
        wAtsLength));

    /* Set internal state */
    pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_ATS;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Receive(
                                      phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t ** ppRxBuffer,
                                      uint16_t * pRxLength
                                      )
{
    phStatus_t PH_MEMLOC_REM status;
    uint16_t   PH_MEMLOC_REM wConfig = FALSE;
    uint8_t    PH_MEMLOC_REM bRxLoop = FALSE;
    uint8_t    PH_MEMLOC_REM bIndex = FALSE;
    uint8_t    PH_MEMLOC_REM bBackUpTxBuff[2] = {0};

    /* Used for Reception */
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t *   PH_MEMLOC_REM pPayloadResponse = NULL;
    uint16_t    PH_MEMLOC_REM wHeaderLength = 0;

    /* Validate receive option */
    if((wOption & (uint16_t)~(uint16_t)(PH_RECEIVE_DEFAULT | PH_RECEIVE_CHAINING
       | PH_RECEIVE_CHAINING_BUFSIZE)))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }

    /* If in chaining, sent ACK first */
    if(wOption == PH_RECEIVE_CHAINING)
    {
        PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_Ack(pDataParams));
    }
    else
    {
        /* Reset CID and NAD presence for current block to be received */
        pDataParams->bCidPresence = FALSE;
        pDataParams->bNadPresence = FALSE;
    }

    /* Loop to receive and validate block */
    do
    {
        /* Set receive loop condition */
        bRxLoop = TRUE;

        /* Be in receive mode, till successful receive */
        do
        {
            /* Start the first receive action */
            status = phhalHw_Receive(
                pDataParams->pHalDataParams,
                PH_RECEIVE_DEFAULT,
                ppRxBuffer,
                pRxLength);

            /* Validate received frame and update state */
            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                status = phpalI14443p4mC_Sw_Int_ValidateFrame(
                    pDataParams,
                    *ppRxBuffer,
                    *pRxLength,
                    &pPayloadResponse,
                    &wPayloadLength);
            }

            /* After first receive change state from ATS to Receive to disable
             * further PPS requests */
            if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_ATS)
            {
                pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_RX;
            }

            /* Exit on external RF off or buffer overflow */
            if(((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
               || ((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW))
            {
                /* Reset the position in case the PICC stopped abruptly due to RF OFF during chaining */
                (void)phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0);

                /* Application may reset HW and data parameters of all layers
                 * and may restart listen on RF off error */
                return status;
            }
        }while((status & PH_ERR_MASK) != PH_ERR_SUCCESS);

        /* Check what action to take based on state */
        switch(pDataParams->bStateNow)
        {
        case PHPAL_I14443P4MC_STATE_PPS:
            /* Validate PPS request and send PPS response */
            PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_PpsRes(
                pDataParams,
                *ppRxBuffer,
                (uint8_t)*pRxLength));
            break;

        case PHPAL_I14443P4MC_STATE_RXCHAINING:
            /* Calculate header length */
            wHeaderLength = *pRxLength - wPayloadLength;

            /* Receive all the chained blocks */
            status = phpalI14443p4mC_Sw_Int_HandleRxChaining(
                pDataParams,
                wOption,
                ppRxBuffer,
                pRxLength);

            /* Return error, if chaining is not success */
            if(((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
                 && ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
            {
                /* Reset the position in case the PICC stopped abruptly due to RF OFF during chaining */
                (void)phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0);
                return status;
            }

            /* Update pay-load length */
            wPayloadLength = *pRxLength - wHeaderLength;

            /* Exit loop */
            bRxLoop = FALSE;
            break;

        case PHPAL_I14443P4MC_STATE_RESEND:
            /* Use the length of last block send as Tx buffer length.
            * Re-send last I block */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                PH_TRANSMIT_PREV_FRAME,
                NULL,
                pDataParams->wLastTxLen));
            break;

        case PHPAL_I14443P4MC_STATE_TX_R:
            /*RAck is of 2-Bytes, Backup the contents of TxBuffer before sending ack*/
            for(bIndex = 0; bIndex < 2 ; bIndex ++)
            {
                PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bIndex));

                /* Backup header byte  */
                PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wConfig));
                bBackUpTxBuff[bIndex] = (uint8_t) wConfig;
            }

            /* Send ACK*/
            PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_Ack(pDataParams));

            /*Restore the TxBuffer*/
            for(bIndex = 0; bIndex < 2 ; bIndex ++)
            {
                /* Prepare access to byte position */
                PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bIndex));

                /* Restore the TxBuffer  */
                PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, (uint16_t) bBackUpTxBuff[bIndex]));
            }
            break;

        case PHPAL_I14443P4MC_STATE_DESELECT:
            /* Send S-Block DESELECT and exit */
            PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_DslRes(pDataParams));
            /* Exit loop */
            bRxLoop = FALSE;
            break;

        case PHPAL_I14443P4MC_STATE_TX_I:
            /* Exit loop */
            bRxLoop = FALSE;
            break;

        default:
            /* Exit loop */
            bRxLoop = FALSE;
            break;
        }
    }
    while(bRxLoop == TRUE);

    /* Return DESELECTED if state is NONE. State will be NONE after DESELECT */
    if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_NONE)
    {
        return (PH_ERR_SUCCESS_DESELECTED | PH_COMP_PAL_I14443P4MC);
    }


    /* Start WTX timer, if I-Block */
    if(((pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_TX_I)
        || (pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_RXCHAINING))
        && (!pDataParams->bDisableWtx))
    {
        PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_StartWtxTimer(pDataParams));
    }


    /* Update pay-load, if reception is success */
    *ppRxBuffer = pPayloadResponse;
    *pRxLength  = wPayloadLength;

    /* Return success chaining, if returning in between chaining */
    if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_RXCHAINING)
    {
        return (PH_ERR_SUCCESS_CHAINING | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_Transmit(
                                       phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint8_t * pTxBuffer,
                                       uint16_t  wTxLength
                                       )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Used for exchange */
    uint16_t   PH_MEMLOC_REM wTxSent = 0;
    uint8_t    PH_MEMLOC_REM bForceSend;
    uint8_t    PH_MEMLOC_REM aSoD[3] = {0};
    uint16_t   PH_MEMLOC_REM wSoDLen = 1;
    uint16_t   PH_MEMLOC_REM wTxCopyBytes;
    uint16_t   PH_MEMLOC_REM wTxFrameBytes;
    uint16_t   PH_MEMLOC_REM wTxBuffBytes;
    uint8_t    PH_MEMLOC_REM bTransmit = 0;
    uint16_t   PH_MEMLOC_REM wTxBufferSize;
    uint16_t   PH_MEMLOC_REM wPrologueField;
    uint8_t    PH_MEMLOC_REM bNadInclude;
    uint8_t    PH_MEMLOC_REM bBytePos;

    /* Used for exchange */
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxLength = 0;

    /* Validate transmit option */
    if (wOption &  (uint16_t)~(uint16_t)(PH_TRANSMIT_BUFFERED_BIT
        | PH_TRANSMIT_LEAVE_BUFFER_BIT | PH_TRANSMIT_TXCHAINING))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }

    /* Retrieve HAL TX buffer size */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE,
        &wTxBufferSize));

    /* ******************************** */
    /*         TRANSMISSION LOOP        */
    /* ******************************** */
    do
    {

        /* Stop WTX Timer */
        if(0U == (pDataParams->bDisableWtx))
        {
            PH_CHECK_SUCCESS_FCT(status, phDriver_TimerStop());
        }


        if (0U != (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
        {
            /* See if there is anything cached already in the buffer */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                &wTxBuffBytes));
        }
        else
        {
            wTxBuffBytes = 0;
        }

        /* If zero, we have new frame */
        if (wTxBuffBytes == 0U)
        {
            /* In case of chaining, NAD shall be present only in first block */
            bNadInclude = (uint8_t)((wTxSent == 0U)? PH_ON : PH_OFF);

            /* Get header length */
            PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_GetPduHeaderLength(
                pDataParams,
                bNadInclude,
                (uint8_t *)&wSoDLen));

            /* Put the new SoD into TX buffer */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                PH_TRANSMIT_BUFFER_FIRST,
                aSoD,
                wSoDLen));
        }

        /* Find the biggest possible frame size before chaining is needed. */
        wTxFrameBytes = (wTxBufferSize < (aI14443p4mC_FsTable[pDataParams->bFsdi] - wSoDLen)) ?
            wTxBufferSize : (aI14443p4mC_FsTable[pDataParams->bFsdi] - wSoDLen);

        /* Calculate how much space we have left */
        wTxFrameBytes = wTxFrameBytes - wTxBuffBytes;

        /* Check if must force send and/or chaining */
        bForceSend = 0;

        if(0 > (wTxFrameBytes - (wTxLength - wTxSent)))
        {
            wTxCopyBytes = wTxFrameBytes;

            bForceSend = 1;
            pDataParams->bChainingTx = 1;
            pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TXCHAINING;
        }
        else
        {
            wTxCopyBytes = (wTxLength - wTxSent);
            bTransmit = 0;
            if(0U != (wOption & PH_TRANSMIT_TXCHAINING))
            {
                bForceSend = 1;
                pDataParams->bChainingTx = 1;
                pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TXCHAINING;
            }
            else
            {
                pDataParams->bChainingTx = 0;
                pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TX_I;
            }
        }

        /* Buffer if requested and not filling up the max frame */
        if ((!bForceSend) && (0U != ((wOption & PH_EXCHANGE_BUFFERED_BIT))))
        {
            return phhalHw_Transmit(
                pDataParams->pHalDataParams,
                PH_TRANSMIT_BUFFER_CONT,
                &pTxBuffer[wTxSent],
                wTxCopyBytes);
        }

        /* Build I block SoD */
        aSoD[0] = PHPALI14443P4MC_BLOCK_I | pDataParams->bBlockNr;

        /* Are we chaining */
        if (0U != (pDataParams->bChainingTx))
        {
            aSoD[0] = aSoD[0] | PHPALI14443P4MC_BLOCK_IS_CHAIN;
        }

        /* Add CID */
        if(0U != (pDataParams->bCidPresence))
        {
            /* Set CID following bit in PCB */
            aSoD[0] = aSoD[0] | PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING;

            /* Set CID byte */
            aSoD[1] = pDataParams->bCid;

            /* Put CID to correct place */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_OFFSET,
                0x01));
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER,
                (uint16_t)aSoD[1]));
        }

        /* Add NAD (if chaining, add only for first block of chaining) */
        if((pDataParams->bNadPresence) && (wTxSent == 0U))
        {
            /* Set NAD following bit in PCB */
            aSoD[0] = aSoD[0] | PHPAL_I14443P4MC_SW_PCB_NAD_FOLLOWING;

            /* Get NAD position in header */
            bBytePos = (pDataParams->bCidPresence == TRUE)? 2: 1;

            /* Set NAD byte */
            aSoD[bBytePos] = pDataParams->bNad;

            /* Put NAD to correct place */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_OFFSET,
                bBytePos));
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER,
                (uint16_t)aSoD[bBytePos]));
        }

        /* Put PCB to correct place */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXBUFFER_OFFSET,
            0x00));

        /* Set PCB value in TX buffer */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXBUFFER,
            (uint16_t)aSoD[0]));

        /* Perform the exchange, in case of chaining */
        if (0U != (pDataParams->bChainingTx))
        {
            /* Get header length */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                &wPrologueField));

            /* Backup total TX length (needed in case of error, to re-send) */
            pDataParams->wLastTxLen = wTxCopyBytes + wPrologueField;

            /* Transmit data */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                PH_TRANSMIT_BUFFER_LAST,
                &pTxBuffer[wTxSent],
                wTxCopyBytes));

            /* Loop to re-send in case of error */
            do
            {
                /* Be in receive mode till we receive valid data */
                do
                {
                    status = phhalHw_Receive(
                        pDataParams->pHalDataParams,
                        PH_RECEIVE_DEFAULT,
                        &pRxBuffer,
                        &wRxLength);

                    if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                    {
                        /* Validate frame */
                        status = phpalI14443p4mC_Sw_Int_ValidateFrame(
                            pDataParams,
                            pRxBuffer,
                            wRxLength,
                            NULL,
                            NULL);

                        if(((status & PH_ERR_MASK) == PH_ERR_SUCCESS) &&
                            (pDataParams->bStateNow != PHPAL_I14443P4MC_STATE_RESEND))
                        {
                         if (pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_TX_R)
                          {
                                /* Send ACK and continue with receive */
                                PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Int_Ack(pDataParams));

                                /* Set the next state to TxChaining to have correct checks in phpalI14443p4mC_Sw_Int_ValidateFrame. */
                                pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_TXCHAINING;
                                /* Enter into Receive by corrupting status. */
                                status = PH_ERR_PROTOCOL_ERROR;
                           }
                          else
                           {
                             /* Update the number of bytes send */
                              wTxSent = wTxSent + wTxCopyBytes;
                           }
                        }

                        /* Set Exit option */
                        if((wTxSent == wTxLength) && (0U != ((wOption & PH_TRANSMIT_TXCHAINING))))
                        {
                            /* Exit the loop */
                            bTransmit = 0;
                        }
                        else
                        {
                            /* Continue TX chaining */
                            bTransmit = 1;
                        }
                    }
                    else if(((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
                         || ((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW))
                    {
                        /* Application may reset HW and data parameters of all layers
                         * and may restart listen on RF off error */
                        return status;
                    }
                    else
                    {
                        /* Continue in receive mode */
                    }
                }
                while((status & PH_ERR_MASK) != PH_ERR_SUCCESS);

                /* Re-send last I-Block */
                if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_RESEND)
                {
                    /* Use the length of last block send as Tx buffer length
                    * Re-send last I block */
                    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                        pDataParams->pHalDataParams,
                        PH_TRANSMIT_PREV_FRAME,
                        NULL,
                        pDataParams->wLastTxLen));
                }
            }
            while(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_RESEND);
        }
        else
        {
            /* Get header length */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                &wPrologueField));

            /* Backup total TX length (needed in case of error, to re-send) */
            pDataParams->wLastTxLen = wTxCopyBytes + wPrologueField;

            /* Only do transmit, if not chaining */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                PH_TRANSMIT_BUFFER_LAST,
                &pTxBuffer[wTxSent],
                wTxCopyBytes));

            /* Exit the loop */
            bTransmit = 0;
        }
    }
    while(0U != bTransmit);

    return status;
}

phStatus_t phpalI14443p4mC_Sw_Wtx(
                                  phpalI14443p4mC_Sw_DataParams_t * pDataParams
                                  )
{
    phStatus_t PH_MEMLOC_REM  status;
    uint8_t    PH_MEMLOC_REM  aCmd[3];
    uint16_t   PH_MEMLOC_REM  wWtxReqLength = 2;
    uint8_t *  PH_MEMLOC_REM  pWtxResp = NULL;
    uint16_t   PH_MEMLOC_REM  wWtxRespLength = 0;

    aCmd[0] = PHPAL_I14443P4MC_SW_S_BLOCK_WTX_FRAME;

    /* Add CID if enabled */
    if (0U != (pDataParams->bCidPresence))
    {
        aCmd[0] |= PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING;
        aCmd[1] = pDataParams->bCid;
        wWtxReqLength++;
    }
    aCmd[wWtxReqLength - 1u] = pDataParams->bWtx;

    /* Backup TX length (needed in case of error, to re-send) */
    pDataParams->wLastTxLen = wWtxReqLength;

    /* Send WTX request */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_TRANSMIT_LEAVE_BUFFER_BIT,
        &aCmd[0],
        wWtxReqLength));

    /* Update state */
    pDataParams->bStateNow = PHPAL_I14443P4MC_STATE_WTX;

    /* Receive WTX response */
    PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Sw_Receive(
        pDataParams,
        PH_RECEIVE_DEFAULT,
        &pWtxResp,
        &wWtxRespLength));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_SetConfig(
                                        phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                        uint16_t wConfig,
                                        uint16_t wValue
                                        )
{
    switch(wConfig)
    {
    case PHPAL_I14443P4MC_CONFIG_WTX:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }
        pDataParams->bDisableWtx = (uint8_t)(wValue ^ 0x0001U);
        break;

    case PHPAL_I14443P4MC_CONFIG_WTX_VALUE:
        if((wValue < PHPAL_I14443P4MC_SW_MIN_WTX_VALUE) || (wValue > PHPAL_I14443P4MC_SW_MAX_WTX_VALUE))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }
        pDataParams->bWtx = (uint8_t)(wValue);
        break;

    case PHPAL_I14443P4MC_CONFIG_MODE:
        if((wValue !=  RD_LIB_MODE_ISO) && (wValue != RD_LIB_MODE_NFC))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }
        pDataParams->bOpMode = (uint8_t)(wValue);
        break;

    case PHPAL_I14443P4MC_CONFIG_WT_PERCENTAGE:
        if((wValue > 100U) || (wValue < 10U))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }
        pDataParams->bWtPercentage = (uint8_t)(wValue);
        break;

    case PHPAL_I14443P4MC_CONFIG_WT_DELTA:
        if(pDataParams->bStateNow == PHPAL_I14443P4MC_STATE_NONE)
        {
            return (PH_ERR_USE_CONDITION | PH_COMP_PAL_I14443P4MC);
        }

        if((((PHPAL_I14443P4MC_SW_CALCULATE_FWT_US(pDataParams->bFwi))
           * pDataParams->bWtPercentage
           * pDataParams->bWtx) / 100) <  wValue)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I14443P4MC);
        }
        pDataParams->wWtDelta = wValue;
        break;

    default:
        return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4mC_Sw_GetConfig(
                                        phpalI14443p4mC_Sw_DataParams_t * pDataParams,
                                        uint16_t wConfig,
                                        uint16_t *pValue
                                        )
{
    switch(wConfig)
    {
    case PHPAL_I14443P4MC_CONFIG_WTX:
        *pValue = (uint16_t)(pDataParams->bDisableWtx ^ 0x01U);
        break;

    case PHPAL_I14443P4MC_CONFIG_BLOCKNO:
        *pValue = (uint16_t)(pDataParams->bBlockNr);
        break;

    case PHPAL_I14443P4MC_CONFIG_FSDI:
        *pValue = (uint16_t)(pDataParams->bFsdi);
        break;

    case PHPAL_I14443P4MC_CONFIG_FSD:
        *pValue = (uint16_t)aI14443p4mC_FsTable[pDataParams->bFsdi];
        break;

    case PHPAL_I14443P4MC_CONFIG_MODE:
        *pValue = (uint16_t)(pDataParams->bOpMode);
        break;

    case PHPAL_I14443P4MC_CONFIG_WT_PERCENTAGE:
        *pValue = (uint16_t)(pDataParams->bWtPercentage);
        break;

    case PHPAL_I14443P4MC_CONFIG_WT_DELTA:
        *pValue = (uint16_t)(pDataParams->wWtDelta);
        break;

    case PHPAL_I14443P4MC_CONFIG_CID_PRESENCE:
        *pValue = (uint16_t)(pDataParams->bCidPresence);
        break;

    case PHPAL_I14443P4MC_CONFIG_NAD_PRESENCE:
        *pValue = (uint16_t)(pDataParams->bNadPresence);
        break;

    default:
        return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_PAL_I14443P4MC);
    }

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */
