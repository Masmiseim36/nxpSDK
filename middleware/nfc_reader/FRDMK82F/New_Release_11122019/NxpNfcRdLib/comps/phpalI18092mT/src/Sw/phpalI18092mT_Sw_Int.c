/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Internal functions for software ISO18092 target mode component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 27. October 2014
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I18092MT_SW
#include "phpalI18092mT_Sw.h"
#include "phpalI18092mT_Sw_Int.h"
phStatus_t phpalI18092mT_Sw_WriteFrameHeader(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bCommandCode,
    uint8_t bDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aHeader[3];
    uint8_t     PH_MEMLOC_REM bLength = 0;

    bDataLength++; /* Include the length of Command Length byte */

    /* Set LEN byte (includes the length itself). */
    aHeader[bLength++] = (uint8_t)(bDataLength + 2U)/*Command Length*/;

    /* Only NFC initiator mode is supported so only requests are sent. */
    aHeader[bLength++] = PHPAL_I18092MT_SW_CMD_RESPONSE;

    /* Set the command code. */
    aHeader[bLength++] = bCommandCode;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                             pDataParams->pHalDataParams,
                             wOption,
                             aHeader,
                             (uint16_t)bLength
                             ));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pFrame,
    uint16_t wFrameLength,
    uint8_t bExpectedResponseCode,
    uint8_t ** ppValidatedFrame,
    uint16_t * pValidatedFrameLength,
    uint8_t ** ppPayload,
    uint16_t * pPayloadLength
    )
{
    uint16_t    PH_MEMLOC_REM bOffset = 0;
    uint8_t     PH_MEMLOC_REM bPfb;

    /* Check maximum frame length */
    if (wFrameLength > PHPAL_I18092MT_SW_MAX_FRAMESIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }
    /* Check minimum frame length */
    if (wFrameLength < PHPAL_I18092MT_SW_MIN_FRAMESIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    /* Remove Start-byte if present */
    *ppValidatedFrame = &pFrame[bOffset];
    *pValidatedFrameLength = wFrameLength;

    /* Compare length field with frame length. */
    if (pFrame[bOffset++] != (uint8_t)*pValidatedFrameLength)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    /* Check if the received frame is a response frame. */
    if (pFrame[bOffset++] != PHPAL_I18092MT_SW_CMD_REQUEST)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    /* Check if expected response code matches the received response code. */
    if (pFrame[bOffset++] != bExpectedResponseCode)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    /* In case the request is a DEP PDU also check for presence of DID, NAD and PNI. */
    if (bExpectedResponseCode == PHPAL_I18092MT_SW_CMD_DEP_REQ)
    {
        /* PFB should be present in frame. */
        if (wFrameLength < (bOffset + /* */ 1))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
        if(pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_DSL_REQ)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
        /* Retrieve PFB */
        bPfb = pFrame[bOffset++];

        /* DID enabled */
        if (pDataParams->bDid > 0U)
        {
            /* DID presence should be indicated. */
            if ((0U == ((bPfb & PHPAL_I18092MT_SW_PFB_DID_MASK))))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
            }
            /* DID should be present in frame and DID should match as passed in ATR_REQ. */
            if ((wFrameLength < (bOffset + /* */ 1))
                || (pFrame[bOffset++] != pDataParams->bDid))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
            }
        }
        /* DID disabled */
        else
        {
            /* DID presence should not be indicated. */
            if (0U != (bPfb & PHPAL_I18092MT_SW_PFB_DID_MASK))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
            }
        }

        /* Check NAD only for first PDU frames. */
        if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT))))
        {
            /* NAD enabled */
            if (0U != (pDataParams->bNadEnabled))
            {
                /* NAD presence should be indicated in PFB and NAD should be present in frame. */
                if (((0U == ((bPfb & PHPAL_I18092MT_SW_PFB_NAD_MASK))))
                    || (wFrameLength < (bOffset + /* */ 1)))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
                }

                /* Store initiator NAD value */
                pDataParams->bNadVal = (uint8_t)(pDataParams->bNadVal | ((uint8_t)(pFrame[bOffset++] & PHPAL_I18092MT_BIT_RATE_MASK)));
            }
            /* NAD disabled */
            else
            {
                /* NAD presence should not be indicated. */
                if (0U != (bPfb & PHPAL_I18092MT_SW_PFB_NAD_MASK))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
                }
            }
        }

        /* For information or ACK PDU also check for PNI. */
        switch (bPfb & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK)
        {
        case PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK:
            /* If NACK then don't validate PNI. In case of ACK PNI  */
            if ((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK)
            {
                break;
            }
        case PHPAL_I18092MT_SW_PFB_INFORMATION_PDU_MASK:
            if (((bPfb & PHPAL_I18092MT_SW_PFB_PNI_MASK) != pDataParams->bPni) && (pDataParams->bCmdtype != PHPAL_I18092MT_SW_CMD_ATN))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
            }
            else if ((bPfb & PHPAL_I18092MT_SW_PFB_PNI_MASK) != pDataParams->bPni)
            {
                pDataParams->bPni = (bPfb & PHPAL_I18092MT_SW_PFB_PNI_MASK);
            }
            else
            {
                /* Do nothing. */
            }
            break;

        case PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK:
        case PHPAL_I18092MT_SW_PFB_PROTECTED_PDU_MASK:
            /* To validate only PFB field */
            break;
        default:
            /* wrong combination of PFB bits 8 to 6 is enabled */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
    }

    /* Return Payload. */
    *ppPayload = &pFrame[bOffset];
    *pPayloadLength = (uint16_t)(wFrameLength - bOffset);

    return PH_ERR_SUCCESS;
}


phStatus_t phpalI18092mT_Sw_GetPduHeaderLength(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pProtLength
    )
{
    /* Init. protocol length, includes StartByte also. */
    *pProtLength = PHPAL_I18092MT_SW_MIN_FRAMESIZE + 1U;

    /* Add DID if used */
    if (pDataParams->bDid > 0U)
    {
        ++(*pProtLength);
    }

    /* Add NAD if enabled and chaining is not used. */
    if (0U != (pDataParams->bNadEnabled))
    {
        if ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT) ||
            (
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT)))
            ))
        {
            ++(*pProtLength);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_UpdatePduHeader(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t bDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wPfb;
    uint8_t     PH_MEMLOC_REM bLengthPos = 0;

    /* Prepare access to length position */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)bLengthPos));

    /* Write new length */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, (uint16_t)bDataLength));

    /* Set chaining bit if needed. */
    if (0U != (pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))
    {
        /* Prepare access to PFB position */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, (uint16_t)(bLengthPos + 3U)));

        /* Read current PFB */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wPfb));

        if ((0U == ((wPfb & PHPAL_I18092MT_SW_PFB_OPTION_MASK))))
        {
            wPfb |= PHPAL_I18092MT_SW_PFB_OPTION_MASK;

            /* Write new PFB */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, wPfb));
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_WritePduHeader(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
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
    if (bPduMask != PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK)
    {
        aCmd[0] |= pDataParams->bPni;
    }

    /* Add DID if enabled. */
    if (pDataParams->bDid > 0U)
    {
        aCmd[0] |= PHPAL_I18092MT_SW_PFB_DID_MASK;
        aCmd[bCmdLength++] = pDataParams->bDid;
    }

    /* Add NAD if enabled and chaining is not used. */
    if ((pDataParams->bNadEnabled) && (bPduMask != PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK))
    {
        if ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT) ||
            (
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT)))
            ))
        {
            aCmd[0] |= PHPAL_I18092MT_SW_PFB_NAD_MASK;
            aCmd[bCmdLength++] = pDataParams->bNadVal;
        }
    }

    /* Set option bit. */
    if (bOption != PH_OFF)
    {
        aCmd[0] |= PHPAL_I18092MT_SW_PFB_OPTION_MASK;
    }

    /* Add command to overall length */
    bDataLength = (uint8_t)(bDataLength + bCmdLength);


    /* Write frame header */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_WriteFrameHeader(pDataParams, PH_EXCHANGE_BUFFER_FIRST, PHPAL_I18092MT_SW_CMD_DEP_RES, bDataLength));

    /* Append command */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aCmd,
        (uint16_t)bCmdLength
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_PerformTxChaining(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t   wOption,
    uint16_t wBytesToSend,
    uint8_t * pTransportDataBytes,
    uint16_t wTransportDataBytesLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bPfb;
    uint8_t     PH_MEMLOC_REM bDataRec = PH_OFF;

    /* Used for Reception and Validation */
    uint8_t   * PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t   * PH_MEMLOC_REM pValidatedPayload = NULL;
    uint16_t    PH_MEMLOC_REM wValidatedLength = 0;

    /* Get the backup length that is already buffered into TX buffer. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
        &(pDataParams->wLastTxLen)));

    /* Update Backup length with Last Bytes that will be sent. */
    pDataParams->wLastTxLen = (uint16_t)(pDataParams->wLastTxLen + wBytesToSend);

    /* Perform transmit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        (wOption & (uint16_t)~(uint16_t)PH_TRANSMIT_MODE_MASK),
        pTransportDataBytes,
        wBytesToSend
        ));

    /* PNI should be incremented after a successful send response by the target */
    pDataParams->bPni = (uint8_t)PHPAL_I18092MT_SW_INCREMENT_PNI((uint8_t)pDataParams->bPni);

    /* When sending the Last Packet with option PH_TRANSMIT_TXCHAINING, receive should be performed using
     * 'phpalI18092mT_Receive'. After a valid ACK is processed by 'phpalI18092mT_Receive', 'phpalI18092mT_Transmit'
     * function can be called to perform next set of Tx Chaining. */
    if(wBytesToSend != wTransportDataBytesLength)
    {
        do
        {
            status = phhalHw_Receive(
                pDataParams->pHalDataParams,
                0x00,
                &pRxBuffer,
                &wRxLength
                );
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                bDataRec = PH_OFF;
                /* retrieve PFB */
                bPfb = pRxBuffer[3];

                /* validate DEP_REQ */
                status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
                    pDataParams,
                    pRxBuffer,
                    wRxLength,
                    PHPAL_I18092MT_SW_CMD_DEP_REQ,
                    &pValidatedPayload,
                    &wValidatedLength,
                    &pRxBuffer,
                    &wRxLength
                    );
                if (status == PH_ERR_SUCCESS)
                {
                    if(0U != (bPfb & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK))
                    {
                        status = phpalI18092mT_Sw_ValidatePfb(
                                                pDataParams,
                                                bPfb,
                                                &bDataRec);
                    }
                }
            }
            else if ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
            {
                /* HAL indicated either CRC or Parity error occurred. Remain in mute state by restarting receive. */
                bDataRec = PH_OFF;
            }
            else
            {
                /* Any other HAL error exit the function. */
                return status;
            }
        }
        while(bDataRec != PH_ON);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_ResponseToPfb(phpalI18092mT_Sw_DataParams_t * pDataParams,
                                          uint8_t bPfbMask)
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aCmd[3];
    uint8_t     PH_MEMLOC_REM bCmdLength = 0;

    aCmd[bCmdLength++] = bPfbMask;

    /* Add PNI only if PDU is not supervisory. */
    if (bPfbMask != PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK)
    {
        aCmd[0] |= pDataParams->bPni;
    }

    /* Add DID if enabled. */
    if (pDataParams->bDid > 0U)
    {
        aCmd[0] |= PHPAL_I18092MT_SW_PFB_DID_MASK;
        aCmd[bCmdLength++] = pDataParams->bDid;
    }

    /* Add NAD if enabled and chaining is not used. */
    if ((pDataParams->bNadEnabled) && (bPfbMask != PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK))
    {
        if ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT) ||
            (
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))) &&
            (0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT)))
            ))
        {
            aCmd[0] |= PHPAL_I18092MT_SW_PFB_NAD_MASK;
            aCmd[bCmdLength++] = pDataParams->bNadVal;
        }
    }

    /* Write frame header */
    PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_WriteFrameHeader(pDataParams, PH_EXCHANGE_BUFFER_FIRST, PHPAL_I18092MT_SW_CMD_DEP_RES, bCmdLength));

    /* Get the backup length that is already buffered into Tx buffer. */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
        &(pDataParams->wLastTxLen)));

    /* Add data buffer in HAL with data that will be transmitted next. */
    pDataParams->wLastTxLen += bCmdLength;

    /* Append command */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
        pDataParams->pHalDataParams,
        PH_TRANSMIT_BUFFER_LAST,
        aCmd,
        (uint16_t)bCmdLength
        ));

    return PH_ERR_SUCCESS;
}


phStatus_t phpalI18092mT_Sw_HandlechainedData(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pRxBuf,
    uint16_t  wRxLen,
    uint8_t * pRxOverlapped,
    uint16_t  *wRxOverlappedStartPos,
    uint16_t  wHeaderLen,
    uint16_t  wOption,
    uint8_t * bDataRec
    )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_PROTOCOL_ERROR;
    uint16_t   PH_MEMLOC_REM a18092T_FslTable[4] = {64, 128, 192, 254};
    uint16_t   PH_MEMLOC_REM wHalRxBufferSize;

    pDataParams->bStateNow = PHPAL_I18092MT_SW_STATE_CHAINING_BIT;

    /* Calculate start-position for overlapping */
    *wRxOverlappedStartPos = (uint16_t)(wRxLen - wHeaderLen);

    /* Backup overlapped bytes */
    (void)memcpy(pRxOverlapped, &pRxBuf[*wRxOverlappedStartPos - 1u], (uint32_t)(wHeaderLen + 1U));

    /* Adjust RX buffer start position. */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        *wRxOverlappedStartPos));

    /* Get remaining HAL RX buffer size. */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
        &wHalRxBufferSize));

    /* Return with chaining status if the next chain may not fit into our buffer */
    if ((wOption & PH_RECEIVE_CHAINING_BUFSIZE) != PH_RECEIVE_CHAINING_BUFSIZE)
    {
        if ((a18092T_FslTable[pDataParams->bLrt])  > wHalRxBufferSize)
        {
            /* Reset RxBuffer Start Position */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));
            *bDataRec = PH_ON;

            return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_PAL_I18092MT);
        }
    }

    return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MT);
}

phStatus_t phpalI18092mT_Sw_ValidatePfb(phpalI18092mT_Sw_DataParams_t * pDataParams,
                                    uint8_t bPfb,
                                    uint8_t *bDataRec
                                    )
{
    uint8_t    PH_MEMLOC_REM bPniTmp = 0;
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    bPniTmp = pDataParams->bPni;

    /* If NACK, Re-Transmit last response */
    if(PHPAL_I18092MT_SW_IS_NACK(bPfb))
    {
        if((bPfb & PHPAL_I18092MT_SW_PFB_PNI_MASK) == PHPAL_I18092MT_SW_DECREMENT_PNI(bPniTmp))
        {
            /* As per 12.6.1.3.3 When an NACK PDU is received, if the PNI is equal
             * to the PNI of the previous sent PDU, the previous block shall be re-transmitted.
             */
            /* Use the length of last block send as Tx buffer length
            * Re-send last DEP RES */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                PH_TRANSMIT_PREV_FRAME,
                NULL,
                pDataParams->wLastTxLen
                ));
        }
        status = PH_ERR_INTERNAL_ERROR;
        pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_NACK;
    }
    if (PHPAL_I18092MT_SW_IS_ATTENTION(bPfb))
    {
        PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_ResponseToPfb(
            pDataParams,
            PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK
            ));
        status = PH_ERR_INTERNAL_ERROR;

        /* Update the received command type in Dataparams. */
        pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_ATN;
    }
    /* If ACK, just validate the packet and come out of receive mode */
    if(PHPAL_I18092MT_SW_IS_ACK(bPfb))
    {
        pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_ACK;
        *bDataRec = PH_ON;
        status = PH_ERR_SUCCESS_ACK;
    }

    return PH_ADD_COMPCODE(status, PH_COMP_PAL_I18092MT);
}

#endif  /* NXPBUILD__PHPAL_I18092MT_SW */
