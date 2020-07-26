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
* Internal functions for software ISO18092 passive initiator mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CEn: Generated 07. September 2010
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I18092MPI_SW

#include "phpalI18092mPI.h"
#include "phpalI18092mPI_Sw.h"
#include "phpalI18092mPI_Sw_Int.h"

phStatus_t phpalI18092mPI_Sw_WriteFrameHeader(
        phpalI18092mPI_Sw_DataParams_t * pDataParams,
        uint16_t wOption,
        uint8_t bCommandCode,
        uint8_t bDataLength
)
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aHeader[4];
    uint8_t     PH_MEMLOC_REM bLength = 0;

    /* Set LEN byte (includes the length itself). */
    aHeader[bLength++] = bDataLength + 3U;

    /* Only NFC initiator mode is supported so only requests are sent. */
    aHeader[bLength++] = PHPAL_I18092MPI_SW_CMD_REQUEST;

    /* Set the command code. */
    aHeader[bLength++] = bCommandCode;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        aHeader,
        (uint16_t)bLength,
        NULL,
        NULL));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_WritePduHeader(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint8_t bPduMask,
    uint8_t bOption,
    uint8_t bDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[3];
    uint8_t     PH_MEMLOC_REM bCmdLength = 0;

    aCmd[bCmdLength++] = bPduMask;

    /* Add PNI only if PDU is not supervisory. */
    if (bPduMask != PHPAL_I18092MPI_SW_PFB_SUPERVISORY_PDU_MASK)
    {
        aCmd[0] |= pDataParams->bPni;
    }

    /* Add DID if enabled. */
    if (pDataParams->bDid > 0U)
    {
        aCmd[0] |= PHPAL_I18092MPI_SW_PFB_DID_MASK;
        aCmd[bCmdLength++] = pDataParams->bDid;
    }

    /* Add NAD if enabled and chaining is not used. */
    if (((pDataParams->bNadEnabled) != 0U)
        && (bPduMask != PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK))
    {
        if (((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT) != 0U) ||
            (
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT)))
            ))
        {
            aCmd[0] |= PHPAL_I18092MPI_SW_PFB_NAD_MASK;
            aCmd[bCmdLength++] = pDataParams->bNad;
        }
    }

    /* Set option bit. */
    if (bOption != PH_OFF)
    {
        aCmd[0] |= PHPAL_I18092MPI_SW_PFB_OPTION_MASK;
    }

    /* Add command to overall length */
    bDataLength = bDataLength + bCmdLength;

    /* Write frame header */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WriteFrameHeader(pDataParams, PH_EXCHANGE_BUFFER_FIRST, PHPAL_I18092MPI_SW_CMD_DEP_REQ, bDataLength));

    /* Append command */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aCmd,
        (uint16_t)bCmdLength,
        NULL,
        NULL));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_UpdatePduHeader(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint8_t bDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wPfb = 0;
    uint8_t     PH_MEMLOC_REM bLengthPos = 0;
    uint8_t     PH_MEMLOC_REM bProtLength;

    /* Retrieve protocol header length */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_GetPduHeaderLength(pDataParams, &bProtLength));

    /* Add protocol header length to overall length */
    bDataLength = bProtLength + bDataLength;

    /* Prepare access to length position */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bLengthPos));

    /* Write new length */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, (uint16_t)bDataLength));

    /* Prepare access to PFB position */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)(((uint16_t)bLengthPos) + 3U)));

    /* Read current PFB */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wPfb));

    /* Set chaining bit if needed. */
    if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
    {
        if ((0U == ((wPfb & PHPAL_I18092MPI_SW_PFB_OPTION_MASK))))
        {
            wPfb |= PHPAL_I18092MPI_SW_PFB_OPTION_MASK;

            /* Write new PFB */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, wPfb));
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_GetPduHeaderLength(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint8_t * pProtLength
    )
{
    /* init. protocol lentgh */
    *pProtLength = 4;

    /* Add DID if used */
    if (pDataParams->bDid > 0U)
    {
        ++(*pProtLength);
    }
    /* Add NAD if enabled and chaining is not used. */
    if (0U != (pDataParams->bNadEnabled))
    {
        if (((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT) != 0U) ||
            (
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT)))
            ))
        {
            ++(*pProtLength);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint8_t * pFrame,
    uint16_t wFrameLength,
    uint8_t bExpectedResponseCode,
    uint8_t ** ppValidatedFrame,
    uint16_t * pValidatedFrameLength,
    uint8_t ** ppPayload,
    uint16_t * pPayloadLength
    )
{
    phStatus_t  PH_MEMLOC_REM bOffset = 0;
    uint8_t     PH_MEMLOC_REM bPfb;

    /* Check maximum frame length */
    if (wFrameLength > PHPAL_I18092MPI_SW_MAX_FRAMESIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* Check minimum frame length */
    if (wFrameLength < PHPAL_I18092MPI_SW_MIN_FRAMESIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* Remove Startbyte if present */
    *ppValidatedFrame = &pFrame[bOffset];
    *pValidatedFrameLength = wFrameLength - (uint16_t)bOffset;

    /* Compare length field with frame length. */
    if (pFrame[bOffset++] != (uint8_t)*pValidatedFrameLength)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* Check if the received frame is a reponse frame. */
    if (pFrame[bOffset++] != PHPAL_I18092MPI_SW_CMD_RESPONSE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* Check if expected response code matches the received response code. */
    if (pFrame[bOffset++] != bExpectedResponseCode)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* In case the response is a DEP PDU also check for presence of DID, NAD and PNI. */
    if (bExpectedResponseCode == PHPAL_I18092MPI_SW_CMD_DEP_RES)
    {
        /* PFB should be present in frame. */
        if (wFrameLength < (bOffset + /* */ 1U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
        }

        /* Retrieve PFB */
        bPfb = pFrame[bOffset++];

        /* DID enabled */
        if (pDataParams->bDid > 0U)
        {
            /* DID presence should be indicated. */
            if ((0U == ((bPfb & PHPAL_I18092MPI_SW_PFB_DID_MASK))))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            /* DID should be present in frame. */
            if (wFrameLength < (bOffset + /* */ 1U))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            /* DID should match. */
            if (pFrame[bOffset++] != pDataParams->bDid)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
        }
        /* DID disabled */
        else
        {
            /* DID presence should not be indicated. */
            if (0U != (bPfb & PHPAL_I18092MPI_SW_PFB_DID_MASK))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
        }

        /* Check NAD only for first PDU frames. */
        if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT))))
        {
            /* NAD enabled */
            if (0U != (pDataParams->bNadEnabled))
            {
                /* NAD presence should be indicated. */
                if ((0U == ((bPfb & PHPAL_I18092MPI_SW_PFB_NAD_MASK))))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
                /* NAD should be present in frame. */
                if (wFrameLength < (bOffset + /* */ 1U))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
                /* NAD should match. */
                if (pFrame[bOffset++] != pDataParams->bNad)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
            /* NAD disabled */
            else
            {
                /* NAD presence should not be indicated. */
                if (0U != (bPfb & PHPAL_I18092MPI_SW_PFB_NAD_MASK))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
        }

        /* For information or ACK PDU also check for PNI. */
        switch (bPfb & PHPAL_I18092MPI_SW_PFB_PDU_TYPE_MASK)
        {
        case PHPAL_I18092MPI_SW_PFB_INFORMATION_PDU_MASK:
        case PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK:
            if ((bPfb & PHPAL_I18092MPI_SW_PFB_PNI_MASK) != pDataParams->bPni)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            break;

        default:
            /* nothing. */
            break;
        }
    }

    /* Return Payload. */
    *ppPayload = &pFrame[bOffset];
    *pPayloadLength = wFrameLength - bOffset;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_ConvertDatarate(
    uint16_t wHalDatarate,
    uint8_t * pI18092Datarate
    )
{
    wHalDatarate = wHalDatarate & PHHAL_HW_RF_DATARATE_OPTION_MASK;
    switch (wHalDatarate)
    {
    case PHHAL_HW_RF_DATARATE_106:
        *pI18092Datarate = PHPAL_I18092MPI_DATARATE_106;
        break;
    case PHHAL_HW_RF_DATARATE_212:
        *pI18092Datarate = PHPAL_I18092MPI_DATARATE_212;
        break;
    case PHHAL_HW_RF_DATARATE_424:
        *pI18092Datarate = PHPAL_I18092MPI_DATARATE_424;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_I18092MPI);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_TransceivePdu(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRawResponse,
    uint16_t * pRawResponseLength,
    uint8_t ** ppNormalizedResponse,
    uint16_t * pNormalizedResponseLength,
    uint8_t ** ppPayload,
    uint16_t * pPayloadLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;
    uint16_t    PH_MEMLOC_REM wTimeoutDefault = 0;
    uint8_t     PH_MEMLOC_REM bRtox = 0;
    uint8_t     PH_MEMLOC_COUNT bRetryCount = 0;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bSendNack = PH_OFF;
    uint8_t     PH_MEMLOC_REM bSendAtn = PH_OFF;
    uint8_t     PH_MEMLOC_REM bNackSent = PH_OFF;
    uint8_t     PH_MEMLOC_REM bPfb = 0;
    uint16_t    PH_MEMLOC_REM wRxBufferStartPos = 0;
    uint16_t    PH_MEMLOC_REM wRxBufferStartPosTmp;
    uint16_t    PH_MEMLOC_REM wTxBufferPosTmp = 0;
    uint8_t     PH_MEMLOC_REM abHeaderBackup[7];
    uint16_t    PH_MEMLOC_REM wBackupTxLength = 0;
    uint8_t     PH_MEMLOC_REM bCounter = 0;
    uint16_t    PH_MEMLOC_REM wConfig = 0;
    uint8_t     PH_MEMLOC_REM bResPending = 0;
    uint8_t     PH_MEMLOC_REM bRtoxBkp = 0;

    /* Reset returned lengths */
    *pPayloadLength = 0;
    *pNormalizedResponseLength = 0;

    /* Before starting transmission get current read buffer position. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        &wRxBufferStartPos));
    wRxBufferStartPosTmp = wRxBufferStartPos;

    /* Reset response received flag */
    bResponseReceived = 0;

    /* This is absolutely not allowed ;-) */
    if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* Calculate Response Waiting Time. */
    dwShift = 1U;
    dwTimeout = (uint32_t)(PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt)));
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
    dwTimeout /= 1000U;
    dwTimeout += 1U;
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

    /* Set Response Waiting Time. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
        (uint16_t)dwTimeout));

    wTimeoutDefault = (uint16_t)(dwTimeout);

    /* Retry loop */
    do
    {
        bResPending = 0;
        bResponseReceived = 0;
        /* Set temporary RTOX timeout if requested */
        if (bRtox > 0U)
        {
            /* Calculate Response Waiting Time. */
            dwShift = 1U;
            dwTimeout = (uint32_t)((PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt))) * (uint32_t)bRtox);
            dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
            dwTimeout /= 1000U;

            /* If Response Waiting Time extension should be less than or equal to the Max RXT. NFCForum-TS-DigitalProtocol-1.0 section 16.8.4.3. */
            if(dwTimeout > PHPAL_I18092MPI_SW_RWT_MAX_MS)
            {
                dwTimeout = (uint32_t)PHPAL_I18092MPI_SW_RWT_MAX_MS;
            }
            /* Add delta time as per NFCForum-TS-DigitalProtocol-1.0 section 16.11.2.5. */
            dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

            /* Set Response Waiting Time. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                (uint16_t)dwTimeout));
        }

        /* Backup length of buffered Tx-Data. */
        if ((bSendNack == PH_OFF)
            && (bSendAtn == PH_OFF))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                &wTxBufferPosTmp));

            if(0u != (PHPAL_I18092MPI_SW_IS_TIMEOUT(bPfb)))
            {
                bRtoxBkp = bRtox;
            }
        }

        /* Copy payload to internal HAL buffer and start transmission. */
        status = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            wOption,
            pTxBuffer,
            wTxLength,
            ppRawResponse,
            pRawResponseLength);

        /* Only internal buffer was written. */
        if (0U != (wOption & PH_EXCHANGE_BUFFERED_BIT))
        {
            return status;
        }

        /* Prepare Exchange Option for retry. */
        wOption = PH_EXCHANGE_BUFFER_LAST;

        /* Backup current Tx length in case of timeout error, before sending ATN */
        if (((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            && (wTxLength != 0U))
        {
            wBackupTxLength = wTxLength;
        }

        /* If the evaluation below succeeds this leads to a successful loop termination */
        wTxLength = 0;

        /* RTOX Timeout - restore previous value */
        if (bRtox > 0U)
        {
            /* Restore previous timeout. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                wTimeoutDefault));
            bRtox = 0;
        }

        /* Reset SendNack flag */
        bSendNack = PH_OFF;

        /* Exchange was successful */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            if(0u != (PHPAL_I18092MPI_SW_IS_TIMEOUT(bPfb)))
            {
                bRtoxBkp = 0;
            }

            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Check for valid response frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                &(*ppRawResponse)[wRxBufferStartPosTmp],
                (*pRawResponseLength - wRxBufferStartPosTmp),
                PHPAL_I18092MPI_SW_CMD_DEP_RES,
                ppNormalizedResponse,
                pNormalizedResponseLength,
                ppPayload,
                pPayloadLength));

            /* retrieve PFB */
            bPfb = (*ppNormalizedResponse)[3];

            /* If a NACK is received bail out with protocol error. NFCForum-TS-DigitalProtocol-1.0 section 14.12.4.5. */
            if ((PHPAL_I18092MPI_SW_IS_NACK(bPfb)) != 0U)
            {
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            else if ((PHPAL_I18092MPI_SW_IS_ACK(bPfb)) != 0U)
            {
                /* ACK only allowed as an reply to a chained PDU. NFCForum-TS-DigitalProtocol-1.0 section 14.12.4.3. */
                if (((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) == PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX) &&
                    (0U != ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))))
                {
                    /* Valid ACK received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
                    pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
                }
                else
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
            else if ((PHPAL_I18092MPI_SW_IS_ATTENTION(bPfb)) != 0U)
            {
                /* Supervisory attention not allowed as a response to a command different than ATN. NFCForum-TS-DigitalProtocol-1.1 section 16.12.4.5. */
                if (bSendAtn == PH_OFF)
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
                /* If ATN is success then re-transmit the previous packet. */
                else
                {
                    wTxLength = wBackupTxLength;
                    wBackupTxLength = 0;

                    if(0U != (bRtoxBkp))
                    {
                        *pTxBuffer = bRtoxBkp;
                        bRtoxBkp = 0;
                    }
                }
            }
            /* Timeout extension, RTOX. */
            else if ((PHPAL_I18092MPI_SW_IS_TIMEOUT(bPfb)) != 0U)
            {
                /* Timeout extension, RTOX. */
                if ((*pPayloadLength) == 1U)
                {
                    /* Retrieve RTOX. */
                    bRtox = (*ppPayload)[0];

                    if ((bRtox < PHPAL_I18092MPI_SW_RTOX_MIN_VALUE) ||
                        (bRtox > PHPAL_I18092MPI_SW_RTOX_MAX_VALUE))
                    {
                        /* Set error code */
                        status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                    }
                    else
                    {
                        /* Set TxData. */
                        pTxBuffer = &bRtox;
                        wTxLength = 1;

                        /* Prepare for RTOX response. */
                        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                            pDataParams,
                            PHPAL_I18092MPI_SW_PFB_SUPERVISORY_PDU_MASK,
                            PH_ON,
                            (uint8_t)wTxLength));
                    }
                }
                else
                {
                    /* Invalid frame received, send NACK. */
                    bSendNack = PH_ON;

                    /* Set error code */
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
            else if ((PHPAL_I18092MPI_SW_IS_INFORMATION_PDU(bPfb)) != 0U)
            {
                /* During Tx-Chaining only ACKs are accepted. */
                if (((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) == PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX) &&
                    (0U != ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))))
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
                else
                {
                    /* Evaluate chaining bit */
                    if ((PHPAL_I18092MPI_SW_IS_CHAINING(bPfb)) != 0U)
                    {
                        /* Chaining bit set - ack to be sent, set chaining state. */
                        pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
                    }
                    else
                    {
                        /* Chaining bit not set - no ack to be sent, transmission is over. */
                        pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
                    }

                    /* Valid information PDU received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
                    pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
                }
            }
            else
            {
                /* Set error code */
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }

            bSendAtn = PH_OFF;
        }
        /*
        When a timeout occurs, an attention command shall be sent (except a NACK has been sent before).
        ISO/IEC 18092:2004(E) section 12.6.1.3.2.
        */
        else if (((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            && (bNackSent == PH_OFF)
        && ((pDataParams->bAtnDisabled) == 0U))
        {
            bSendAtn = PH_ON;
            bPfb = 0;
        }
        /* Invalid PDU received, send NACK. ISO/IEC 18092:2004(E) section 12.6.1.3.2. */
        else if (0u != (PHPAL_I18092MPI_SW_IS_INVALID_BLOCK_STATUS(status)))
        {
            bSendNack = PH_ON;
            bSendAtn = PH_OFF;
            bPfb = 0;
        }
        else
        {
            /* Force protocol error on NAK */
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            bSendAtn = PH_OFF;
            bPfb = 0;
        }

        /* If we retry during transmission make sure our data to transmit does not get overwritten. */
        if ((bResponseReceived == 1U)
            && (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
        && (bRetryCount > 0U)
        && (pDataParams->bMaxRetryCount > 0U))
        {
            /* Restore Header */
            for (bCounter = 0; bCounter < PHPAL_I18092MPI_SW_MAX_HEADER_LENGTH; ++bCounter)
            {
                /* Prepare access to byte position */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bCounter));

                /* Restore header byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, (uint16_t)abHeaderBackup[bCounter])); /* PRQA S 3321 */
            }

            /* Restore length of buffered Tx-Data. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                wTxBufferPosTmp));
            bResPending = 1;

            if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
            {
                pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
            }

        }

        /* We need to retry */
        if (/*(bSendNack != PH_OFF) || */(bSendAtn != PH_OFF))
        {
            /* Retransmission requested. */
            if ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) == PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX)
            {
                pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
            }

            /* If we retry during transmission make sure our data to transmit does not get overwritten. */
            if ((0U != ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT)))
                && (bRetryCount == 0U)
            && (pDataParams->bMaxRetryCount > 0U))
            {
                /* Backup Header */
                for (bCounter = 0; bCounter < PHPAL_I18092MPI_SW_MAX_HEADER_LENGTH; ++bCounter)
                {
                    /* Prepare access to header byte position */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bCounter));

                    /* Backup header byte  */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wConfig));
                    abHeaderBackup[bCounter] = (uint8_t)wConfig;
                }
            }
        }

        /* NACK Sending */
        if (bSendNack != PH_OFF)
        {
            /* Signal that NACK has been sent */
            bNackSent = PH_ON;

            /* Check if we shall perform another retry. */
            if (bRetryCount < pDataParams->bMaxRetryCount)
            {
                /* Increment retry count */
                ++bRetryCount;

                /* Prepare NACK PDU. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK,
                    PH_ON,
                    0));
            }
            /* Throw timeout protocol exception. NFCForum-TS-DigitalProtocol-1.0 section 14.12.5.6. */
            else
            {
                /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behavior) */
                if ((pDataParams->bMaxRetryCount > 0U)
                    && (0u != bResponseReceived))
                {
                    status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                }

                /* Reset SendNack flag */
                bSendNack = PH_OFF;
            }
        }
        /* ATN sending */
        else if (bSendAtn != PH_OFF)
        {
            /* Check if we shall perform another retry. */
            if (bRetryCount < pDataParams->bMaxRetryCount)
            {
                /* Increment retry count */
                ++bRetryCount;

                /* Prepare Attention PDU. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MPI_SW_PFB_SUPERVISORY_PDU_MASK,
                    PH_OFF,
                    0));
            }
            /* Throw timeout protocol exception. NFCForum-TS-DigitalProtocol-1.0 section 14.12.5.6. */
            else
            {
                /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behavior) */
                if ((pDataParams->bMaxRetryCount > 0U)
                    && (0u != bResponseReceived))
                {
                    status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                }

                /* Reset bSendAtn flag */
                bSendAtn = PH_OFF;
            }
        }
        else
        {
            /* Clear bNackSent flag */
            bNackSent = PH_OFF;

            /* Reset retry count. */
            bRetryCount = 0;
        }
    }
    while ((wTxLength != 0U) || (bSendNack != 0U) || (bSendAtn != 0U) || (bResPending != 0U));

    return status;
}

phStatus_t phpalI18092mPI_Sw_ValidateReceivedPdu(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    phStatus_t statusRx,
    uint8_t ** ppRawResponse,
    uint16_t * pRawResponseLength,
    uint8_t ** ppNormalizedResponse,
    uint16_t * pNormalizedResponseLength,
    uint8_t ** ppPayload,
    uint16_t * pPayloadLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint16_t    PH_MEMLOC_REM wTimeoutPrev = 0;
    uint16_t    PH_MEMLOC_REM wRetransmit = 0;
    uint16_t    PH_MEMLOC_REM wConfig = 0;
    uint8_t     PH_MEMLOC_REM bRtox = 0;
    uint8_t     PH_MEMLOC_REM bRetryCount = 0;
    uint8_t     PH_MEMLOC_REM bResponseReceived = 0;
    uint8_t     PH_MEMLOC_REM bResPending = 0;
    uint8_t     PH_MEMLOC_REM bSendNack = PH_OFF;
    uint8_t     PH_MEMLOC_REM bSendAtn = PH_OFF;
    uint8_t     PH_MEMLOC_REM bNackSent = PH_OFF;
    uint8_t     PH_MEMLOC_REM bPfb;
    uint8_t     PH_MEMLOC_REM abHeaderBackup[7];
    uint8_t     PH_MEMLOC_REM bCounter = 0;
    uint8_t     PH_MEMLOC_REM bValidateOnly = PH_ON;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;

    /* Initialize the Status, to retain the status of previous HAL Receive. */
    status = statusRx;

    do
    {
        bResPending = 0;
        bResponseReceived = 0;

        if(bValidateOnly == PH_OFF)
        {
            /* Set temporary RTOX timeout if requested */
            if (bRtox > 0U)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                    &wTimeoutPrev));

                /* Calculate Response Waiting Time. */
                dwShift = 1U;
                dwTimeout = (uint32_t)((PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt))) * (uint32_t)bRtox);
                dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
                dwTimeout /= 1000U;

                /* If Response Waiting Time extension should be less than or equal to the Max RXT. NFCForum-TS-DigitalProtocol-1.0 section 16.8.4.3. */
                if(dwTimeout > PHPAL_I18092MPI_SW_RWT_MAX_MS)
                {
                    dwTimeout = (uint32_t)PHPAL_I18092MPI_SW_RWT_MAX_MS;
                }
                /* Add delta time as per NFCForum-TS-DigitalProtocol-1.0 section 16.11.2.5. */
                dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

                /* Set Response Waiting Time. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                    (uint16_t)dwTimeout));
            }

            /* Send Data buffered in HAL Buffer and receive the response for the same. */
            statusRx = phhalHw_Exchange(
                pDataParams->pHalDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                ppRawResponse,
                pRawResponseLength);

            /* If the evaluation below succeeds this leads to a successful loop termination */
            wRetransmit = 0;

            /* Update status, to retain the status of previous HAL Exchange. */
            status = statusRx;

            /* RTOX Timeout - restore previous value */
            if (bRtox > 0U)
            {
                /* Restore previous timeout. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                    wTimeoutPrev));
                bRtox = 0;
            }

            /* Reset SendNack flag */
            bSendNack = PH_OFF;
        }

        /* If the evaluation below doesn't succeed during First Loop then perform Transceive. */
        bValidateOnly = PH_OFF;

        /* Exchange was successful */
        if ((statusRx & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Check for valid response frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                &(*ppRawResponse)[0],
                *pRawResponseLength,
                PHPAL_I18092MPI_SW_CMD_DEP_RES,
                ppNormalizedResponse,
                pNormalizedResponseLength,
                ppPayload,
                pPayloadLength));

            /* retrieve PFB */
            bPfb = (*ppNormalizedResponse)[3];

            /* If a NACK is received bail out with protocol error. NFCForum-TS-DigitalProtocol-1.0 section 14.12.4.5.
            * Bail-out is ACK, as ACK is only allowed as an reply to a chained PDU. NFCForum-TS-DigitalProtocol-1.0 section 14.12.4.3. */
            if ((0u != (PHPAL_I18092MPI_SW_IS_NACK(bPfb))) || (0u != (PHPAL_I18092MPI_SW_IS_ACK(bPfb))))
            {
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            else if (0u != (PHPAL_I18092MPI_SW_IS_ATTENTION(bPfb)))
            {
                /* Supervisory attention not allowed as a response to a command different than ATN. NFCForum-TS-DigitalProtocol-1.1 section 16.12.4.5. */
                if (bSendAtn == PH_OFF)
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
                /* If ATN is success then re-transmit the previous packet. */
                else
                {
                    /* Set the length of last block send as TX buffer length */
                    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
                        pDataParams->pHalDataParams,
                        PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                        pDataParams->wLastTxLen));

                    /* Set Flag to perform Re-transmit Data. */
                    wRetransmit = 1;
                }
            }
            /* Timeout extension, RTOX. */
            else if ((PHPAL_I18092MPI_SW_IS_TIMEOUT(bPfb)) != 0U)
            {
                /* Timeout extension, RTOX. */
                if ((*pPayloadLength) == 1U)
                {
                    /* Retrieve RTOX. */
                    bRtox = (*ppPayload)[0];

                    if ((bRtox < PHPAL_I18092MPI_SW_RTOX_MIN_VALUE) ||
                        (bRtox > PHPAL_I18092MPI_SW_RTOX_MAX_VALUE))
                    {
                        /* Set error code */
                        status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                    }
                    else
                    {
                        /* Prepare for RTOX response. */
                        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                            pDataParams,
                            PHPAL_I18092MPI_SW_PFB_SUPERVISORY_PDU_MASK,
                            PH_ON,
                            PHPAL_I18092MPI_SW_RTOX_PAYLOAD_LENGTH));

                        /* Append RTOX Response into HAL Buffer. */
                        PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
                            pDataParams->pHalDataParams,
                            PH_EXCHANGE_BUFFER_CONT,
                            &bRtox,
                            PHPAL_I18092MPI_SW_RTOX_PAYLOAD_LENGTH,
                            NULL,
                            NULL));

                        /* Set Flag to send RTOX Response. */
                        wRetransmit = 1;
                    }
                }
                else
                {
                    /* Invalid frame received, send NACK. */
                    bSendNack = PH_ON;

                    /* Set error code */
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
            else if ((PHPAL_I18092MPI_SW_IS_INFORMATION_PDU(bPfb)) != 0U)
            {
                /* Evaluate chaining bit */
                if (0u != (PHPAL_I18092MPI_SW_IS_CHAINING(bPfb)))
                {
                    /* Chaining bit set - ack to be sent, set chaining state. */
                    pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
                }
                else
                {
                    /* Chaining bit not set - no ack to be sent, transmission is over. */
                    pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
                }

                /* Valid information PDU received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
                pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
            }
            else
            {
                /* Set error code */
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }

            bSendAtn = PH_OFF;
        }
        /*
        When a timeout occurs, an attention command shall be sent (except a NACK has been sent before).
        ISO/IEC 18092:2004(E) section 12.6.1.3.2.
        */
        else if (((statusRx & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            && (bNackSent == PH_OFF)
        && ((pDataParams->bAtnDisabled) == 0U))
        {
            bSendAtn = PH_ON;
        }
        /* Invalid PDU received, send NACK. ISO/IEC 18092:2004(E) section 12.6.1.3.2. */
        else if (0u != (PHPAL_I18092MPI_SW_IS_INVALID_BLOCK_STATUS(statusRx)))
        {
            bSendNack = PH_ON;
            bSendAtn = PH_OFF;
        }
        else
        {
            /* Force protocol error on NAK */
            if ((statusRx & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
            bSendAtn = PH_OFF;
        }

        /* If we retry during transmission make sure our data to transmit does not get overwritten. */
        if ((bResponseReceived == 1U)
            && (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
        && (bRetryCount > 0U)
        && (pDataParams->bMaxRetryCount > 0U))
        {
            /* Restore Header */
            for (bCounter = 0; bCounter < PHPAL_I18092MPI_SW_MAX_HEADER_LENGTH; ++bCounter)
            {
                /* Prepare access to byte position */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bCounter));

                /* Restore header byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, (uint16_t)abHeaderBackup[bCounter])); /* PRQA S 3321 */
            }

            /* Restore length of buffered Tx-Data. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                pDataParams->wLastTxLen));
            bResPending = 1;

            /* Reset the Re-Transmit state of the Dataparam state. */
            pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
        }

        /* We need to retry */
        if (bSendAtn != PH_OFF)
        {
            /* Retransmission requested. */
            pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;

            /* To retry, backup the bytes in Tx Buffer that may get overwritten on data to be Retransmitted. */
            if ((bRetryCount == 0U)
                && (pDataParams->bMaxRetryCount > 0U))
            {
                /* Backup Header */
                for (bCounter = 0; bCounter < PHPAL_I18092MPI_SW_MAX_HEADER_LENGTH; ++bCounter)
                {
                    /* Prepare access to header byte position */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bCounter));

                    /* Backup header byte  */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wConfig));
                    abHeaderBackup[bCounter] = (uint8_t)wConfig;
                }
            }
        }

        /* NACK Sending */
        if (bSendNack != PH_OFF)
        {
            /* Signal that NACK has been sent */
            bNackSent = PH_ON;

            /* Check if we shall perform another retry. */
            if (bRetryCount < pDataParams->bMaxRetryCount)
            {
                /* Increment retry count */
                ++bRetryCount;

                /* Prepare NACK PDU. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK,
                    PH_ON,
                    0));
            }
            /* Throw timeout protocol exception. NFCForum-TS-DigitalProtocol-1.0 section 14.12.5.6. */
            else
            {
                /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behavior) */
                if ((pDataParams->bMaxRetryCount > 0U)
                    && (0u != bResponseReceived))
                {
                    status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                }

                /* Reset SendNack flag */
                bSendNack = PH_OFF;
            }
        }
        /* ATN sending */
        else if (bSendAtn != PH_OFF)
        {
            /* Check if we shall perform another retry. */
            if (bRetryCount < pDataParams->bMaxRetryCount)
            {
                /* Increment retry count */
                ++bRetryCount;

                /* Prepare Attention PDU. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MPI_SW_PFB_SUPERVISORY_PDU_MASK,
                    PH_OFF,
                    0));
            }
            /* Throw timeout protocol exception. NFCForum-TS-DigitalProtocol-1.0 section 14.12.5.6. */
            else
            {
                /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behavior) */
                if ((pDataParams->bMaxRetryCount > 0U)
                    && (0u != bResponseReceived))
                {
                    status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                }

                /* Reset bSendAtn flag */
                bSendAtn = PH_OFF;
            }
        }
        else
        {
            /* Clear bNackSent flag */
            bNackSent = PH_OFF;

            /* Reset retry count. */
            bRetryCount = 0;
        }
    }
    while ((0U != (wRetransmit)) || (0U != (bSendNack)) || (0U != (bSendAtn)) || (0U != (bResPending)));

    return status;
}

#endif /* NXPBUILD__PHPAL_I18092MPI_SW */
