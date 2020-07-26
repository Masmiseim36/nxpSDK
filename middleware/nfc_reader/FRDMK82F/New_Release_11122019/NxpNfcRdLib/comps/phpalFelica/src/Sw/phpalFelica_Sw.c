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
* Software FeliCa Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 26. January 2010
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalFelica.h>
#include <ph_RefDefs.h>

#ifdef  NXPBUILD__PHPAL_FELICA_SW

#include "phpalFelica_Sw.h"
#include "phpalFelica_Sw_Int.h"

/* Table for 4^E evaluation (E is only 2 bits, so more values are not possible). */
static const uint8_t PH_MEMLOC_CONST_ROM aFelica_E_Table[] = {1, 4, 16, 64};

#define FDTF_LISTEN_SENSF_REQ(x, y)    ((PHPAL_FELICA_SW_T_DELAY_US + ((x) * PHPAL_FELICA_SW_T_TIMESLOT_US)) + y)    /**< NFC Forum Digital Protocol Technical Specification v1.1 : 8.7.1 : FDTF,LISTEN,SENSF_REQ */

static const uint16_t PH_MEMLOC_CONST_ROM aFelica_TR1_Value_Us[][2] = {
    {PHPAL_FELICA_SW_TR1_48BITS_AT212_US, PHPAL_FELICA_SW_TR1_48BITS_AT424_US},
    {PHPAL_FELICA_SW_TR1_56BITS_AT212_US, PHPAL_FELICA_SW_TR1_56BITS_AT424_US},
    {PHPAL_FELICA_SW_TR1_64BITS_AT212_US, PHPAL_FELICA_SW_TR1_64BITS_AT424_US},
    {PHPAL_FELICA_SW_TR1_72BITS_AT212_US, PHPAL_FELICA_SW_TR1_72BITS_AT424_US},
};

phStatus_t phpalFelica_Sw_Init(
                               phpalFelica_Sw_DataParams_t * pDataParams,
                               uint16_t wSizeOfDataParams,
                               void * pHalDataParams
                               )
{
    /* parameter structure length check */
    if (sizeof(phpalFelica_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId            = PH_COMP_PAL_FELICA | PHPAL_FELICA_SW_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->bIDmPMmValid   = PHPAL_FELICA_SW_IDMPMM_INVALID;
    pDataParams->bLength        = 0x00;
    pDataParams->bRequestCode   = 0x00;
    pDataParams->bPreambleLen   = PHPAL_FELICA_PREAMBLE_LEN_48BITS;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalFelica_Sw_ReqC(
                               phpalFelica_Sw_DataParams_t* pDataParams,
                               uint8_t * pSystemCode,
                               uint8_t  bNumTimeSlots,
                               uint8_t ** ppRxBuffer,
                               uint16_t * pRxLength
                               )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[6];
    uint8_t *   PH_MEMLOC_REM pRxBufferTmp = NULL;
    uint16_t    PH_MEMLOC_REM wRxLengthTmp = 0;
    uint16_t    PH_MEMLOC_REM wTimeOutUs;
    uint16_t    PH_MEMLOC_REM wCardType = 0;
    uint16_t    PH_MEMLOC_REM wTR1ValueUs;

    /* reset validity of stored card IDs */
    pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_INVALID;

    /* Reset received frame count */
    pDataParams->bTotalFrames = 0;

    /* parameter check and timeout setting */
    if(!((bNumTimeSlots == PHPAL_FELICA_NUMSLOTS_1) ||
        (bNumTimeSlots == PHPAL_FELICA_NUMSLOTS_2) ||
        (bNumTimeSlots == PHPAL_FELICA_NUMSLOTS_4) ||
        (bNumTimeSlots == PHPAL_FELICA_NUMSLOTS_8) ||
        (bNumTimeSlots == PHPAL_FELICA_NUMSLOTS_16))
      )
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_FELICA);
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_CARD_TYPE,
            &wCardType));
        /* NFC Forum Digital Protocol Technical Specification v1.1
         * 8.7.1.3 : wTimeOutUs = FDTF,LISTEN,SENSF_REQ(TSN+1U) - TR1 + (delta)TF,POLL
         * Using TR1 value @212kbps */
        if ((wCardType != PHHAL_HW_CARDTYPE_FELICA_212) && (wCardType != PHHAL_HW_CARDTYPE_FELICA_424))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_FELICA);
        }

        if (wCardType == PHHAL_HW_CARDTYPE_FELICA_212)
        {
            wTR1ValueUs = aFelica_TR1_Value_Us[pDataParams->bPreambleLen][0];
        }
        else
        {
            wTR1ValueUs = aFelica_TR1_Value_Us[pDataParams->bPreambleLen][1];
        }

        wTimeOutUs = FDTF_LISTEN_SENSF_REQ((((uint16_t)bNumTimeSlots) + 1U), wTR1ValueUs) - wTR1ValueUs + PHPAL_FELICA_SW_T_DELTA_F_POLL_US;
    }

    /* build the command frame */
    aCmd[0] = 6;
    aCmd[1] = PHPAL_FELICA_SW_CMD_REQC;
    aCmd[2] = pSystemCode[0];
    aCmd[3] = pSystemCode[1];
    aCmd[4] = pDataParams->bRequestCode;
    aCmd[5] = bNumTimeSlots;

    /* Set ReqC timeout according to the number of slots */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        wTimeOutUs));

    if(bNumTimeSlots > PHPAL_FELICA_NUMSLOTS_1)
    {
        /* Set RX Multiple feature of HW */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXMULTIPLE,
                PH_ON));
    }

    /* Exchange command */
     status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        6,
        &pRxBufferTmp,
        &wRxLengthTmp);

    if(bNumTimeSlots > PHPAL_FELICA_NUMSLOTS_1)
    {
        /* Re-Set RX Multiple feature of HW */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXMULTIPLE,
                PH_OFF));
    }

    /* Error check */
    PH_CHECK_SUCCESS(status);

    /* Check length and update info*/
    if(bNumTimeSlots > PHPAL_FELICA_NUMSLOTS_1)
    {
        /* Rx Length should be multiple of response frame size */
        if(0U != (wRxLengthTmp % PHPAL_FELICA_SW_RESP_FRAME_SIZE))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
        }
        /* assign and update IDm and PMm and length */
        *ppRxBuffer = pRxBufferTmp;
        *pRxLength = wRxLengthTmp;

        pDataParams->bTotalFrames = (uint8_t)(wRxLengthTmp / PHPAL_FELICA_SW_RESP_FRAME_SIZE);
    }
    else
    {
        if (((wRxLengthTmp != (PHPAL_FELICA_ATQC_LENGTH + 1U)) &&
            (wRxLengthTmp != (PHPAL_FELICA_ATQC_LENGTH + PHPAL_FELICA_RD_LENGTH + 1U))) ||
            (pRxBufferTmp[0] != wRxLengthTmp) ||
            (pRxBufferTmp[1] != PHPAL_FELICA_SW_RSP_REQC))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
        }

        /* RC = 0 and card response with RD information */
        if((wRxLengthTmp == (PHPAL_FELICA_ATQC_LENGTH + PHPAL_FELICA_RD_LENGTH + 1U)) && (pDataParams->bRequestCode == 0U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
        }
        /* Store IDm and PMm */
        (void)memcpy(pDataParams->aIDmPMm, &pRxBufferTmp[2], (((uint32_t)(pRxBufferTmp[0])) - 2U));
        pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_VALID;

        /* Remove LEN Byte and Response Byte from response and assign start position of ID */
        *ppRxBuffer = &pRxBufferTmp[PHPAL_FELICA_SW_LEN_BYTE_SIZE + PHPAL_FELICA_SW_RESP_REQC_SIZE];
        *pRxLength = (uint16_t)(((uint16_t)pRxBufferTmp[0]) - (PHPAL_FELICA_SW_LEN_BYTE_SIZE + PHPAL_FELICA_SW_RESP_REQC_SIZE));
    }

    pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_VALID;

    return PH_ERR_SUCCESS;
}
phStatus_t  phpalFelica_Sw_GetFrameInfo(
                                        phpalFelica_Sw_DataParams_t* pDataParams,
                                        uint8_t bFrameNum,
                                        uint8_t * pResponseBuffer,
                                        uint16_t *pwStatus,
                                        uint8_t** ppID,
                                        uint8_t * pLen
                                        )
{
    uint8_t PH_MEMLOC_REM       bError;
    uint8_t PH_MEMLOC_REM       bStartPos;

    /* check frame number */
    if((bFrameNum > pDataParams->bTotalFrames) || (bFrameNum == 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_FELICA);
    }

    *pwStatus = PH_ERR_SUCCESS;

    /* get start position of frame */
    bStartPos = (uint8_t)(PHPAL_FELICA_SW_RESP_FRAME_SIZE * (bFrameNum - 1U));

    /* Check length */
    if( ((pResponseBuffer[bStartPos + PHHAL_HW_STATUS_FRAME_LEN_BYTE_POS] != (PHPAL_FELICA_ATQC_LENGTH + PHPAL_FELICA_SW_LEN_BYTE_SIZE)) &&
        (pResponseBuffer[bStartPos + PHHAL_HW_STATUS_FRAME_LEN_BYTE_POS] != (PHPAL_FELICA_ATQC_LENGTH + PHPAL_FELICA_RD_LENGTH + PHPAL_FELICA_SW_LEN_BYTE_SIZE))) ||
        (pResponseBuffer[bStartPos + 1U] != PHPAL_FELICA_SW_RSP_REQC))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
    }

    /* RC = 0 and card response with RD information */
    if((pResponseBuffer[bStartPos + PHHAL_HW_STATUS_FRAME_LEN_BYTE_POS] == (PHPAL_FELICA_ATQC_LENGTH + PHPAL_FELICA_RD_LENGTH + PHPAL_FELICA_SW_LEN_BYTE_SIZE)) && (pDataParams->bRequestCode == 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
    }

    /* Extract error from Status byte */
    bError = pResponseBuffer[(bStartPos) + PHHAL_HW_STATUS_FRAME_ERR_BYTE_POS ] & PHHAL_HW_STATUS_FRAME_ERR_MASK;

    if(bError != 0x00U)
    {
        /* check for contact less error */
        if(0U != (bError & PHHAL_HW_STATUS_FRAME_CL_ERR_POS))
        {
            /* check for protocol error */
            if(0U != (bError & PHHAL_HW_STATUS_FRAME_PROT_ERR_POS))
            {
                *pwStatus = PH_ERR_PROTOCOL_ERROR;
            }
            /* check for collision error */
            else if(0U != (bError & PHHAL_HW_STATUS_FRAME_COLL_ERR_POS))
            {
                *pwStatus = PH_ERR_COLLISION_ERROR;
            }
            /* check for Integrity error */
            else if(0U != (bError & PHHAL_HW_STATUS_FRAME_DATA_ERR_POS))
            {
                *pwStatus = PH_ERR_INTEGRITY_ERROR;
            }
            else
            {
                /* If CL Error is set and no Protocol, Collision, Integrity flag is set */
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_FELICA);
            }
        }
        else
        {   /* Length error */
            *pwStatus = PH_ERR_LENGTH_ERROR;
        }

    }
    else
    {
      /* Remove LEN Byte and Response Byte (0x01) from response and assign start position of ID */
      *ppID = &pResponseBuffer[bStartPos + (PHPAL_FELICA_SW_LEN_BYTE_SIZE + PHPAL_FELICA_SW_RESP_REQC_SIZE)];
      *pLen = pResponseBuffer[bStartPos + PHHAL_HW_STATUS_FRAME_LEN_BYTE_POS] - (PHPAL_FELICA_SW_LEN_BYTE_SIZE + PHPAL_FELICA_SW_RESP_REQC_SIZE);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalFelica_Sw_SetSerialNo(
                                      phpalFelica_Sw_DataParams_t * pDataParams,
                                      uint8_t * pIDmPMm
                                      )
{

    pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_VALID;
    /* copy IDm and PMm */
    (void)memcpy(pDataParams->aIDmPMm, pIDmPMm, (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH));

    return PH_ERR_SUCCESS;

}

phStatus_t phpalFelica_Sw_ActivateCard(
                                       phpalFelica_Sw_DataParams_t* pDataParams,
                                       uint8_t* pIDmPMm,
                                       uint8_t bIDmPMmLength,
                                       uint8_t * pSystemCode,
                                       uint8_t bNumTimeSlots,
                                       uint8_t * pRxBuffer,
                                       uint8_t * pRxLength,
                                       uint8_t * pMoreCardsAvailable
                                       )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t *  PH_MEMLOC_REM pId = NULL;
    uint8_t *  PH_MEMLOC_REM pRxBuff = NULL;
    uint16_t   PH_MEMLOC_REM wRxLen = 0;
    uint8_t    PH_MEMLOC_REM bTemp;
    uint16_t   PH_MEMLOC_REM wFrameStatus;

    *pRxLength = 0;
    *pMoreCardsAvailable = 0;

    /* if serial number is given only store it */
    if (0U != (bIDmPMmLength))
    {
        /* check if IDm length is valid. */
        if (bIDmPMmLength != (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_FELICA);
        }

        /* Store IDm/PMm */
        (void)memcpy(pDataParams->aIDmPMm, pIDmPMm, bIDmPMmLength);
        pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_VALID;

        return PH_ERR_SUCCESS;
    }

    do{
        /* RequestC == ReqC command (not requesting the system code) */
        status = phpalFelica_Sw_ReqC(
            pDataParams,
            pSystemCode,
            bNumTimeSlots,
            &pRxBuff,
            &wRxLen);

        if(bNumTimeSlots > PHPAL_FELICA_NUMSLOTS_1)
        {
            if(pDataParams->bTotalFrames > 1U)
            {
                *pMoreCardsAvailable = 1;
            }

            /* get total number of response frame */
            bTemp = pDataParams->bTotalFrames;

            while(0U != bTemp)
            {
                *pRxLength = 0x00;
                wFrameStatus = 0x00;
                /* retrieve IDmPMm one by one for all frames */
                PH_CHECK_SUCCESS_FCT(status, phpalFelica_Sw_GetFrameInfo(
                                                    pDataParams,
                                                    bTemp,
                                                    pRxBuff,
                                                    &wFrameStatus,
                                                    &pId,
                                                    pRxLength));

                status = wFrameStatus;
                /* store only one valid response and return */
                if(wFrameStatus == PH_ERR_SUCCESS)
                {
                    *pRxLength = PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH;
                    (void)memcpy(pRxBuffer, pId, (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH));
                    (void)memcpy(pDataParams->aIDmPMm, pId, (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH));
                    pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_VALID;
                    return status;
                }
                bTemp--;
            }
        }
        else
        {
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                *pRxLength = PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH;
                (void)memcpy(pRxBuffer, pRxBuff, (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH));
                (void)memcpy(pDataParams->aIDmPMm, pRxBuff, (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH));
                pDataParams->bIDmPMmValid = PHPAL_FELICA_SW_IDMPMM_VALID;

                return status;
            }
        }
        /* check for availability of more card */
        if((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
        {
            *pMoreCardsAvailable = 1;
            bNumTimeSlots = (bNumTimeSlots << 1U) + 1U;
        }
        else
        {
            return status;
        }
    }while (bNumTimeSlots <= PHPAL_FELICA_NUMSLOTS_16);

    return status;
}

phStatus_t phpalFelica_Sw_Exchange(
                                   phpalFelica_Sw_DataParams_t* pDataParams,
                                   uint16_t wOption,
                                   uint16_t wN,
                                   uint8_t* pTxBuffer,
                                   uint16_t wTxLength,
                                   uint8_t ** ppRxBuffer,
                                   uint16_t * pRxLength
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aHeader[2];
    uint16_t    PH_MEMLOC_REM wCmd;
    uint32_t    PH_MEMLOC_REM dwTimeoutUs;
    uint8_t     PH_MEMLOC_REM bTparam;
    uint8_t     PH_MEMLOC_REM bA;
    uint8_t     PH_MEMLOC_REM bB;
    uint8_t     PH_MEMLOC_REM bE;
    uint8_t *   PH_MEMLOC_REM pPMm = NULL;
    uint32_t    PH_MEMLOC_REM dwTOut;

    /* [len][cmd] [IDm ... 8 bytes] [cmd parameters ...] */

    /* Check if we do have an IDm */
    if (pDataParams->bIDmPMmValid == PHPAL_FELICA_SW_IDMPMM_INVALID)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
    }

    /* Parameter check */
    if ((wTxLength + /* */ 2U) > PHPAL_FELICA_TXLENGTH_MAX)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_FELICA);
    }

    /* If the PH_EXCHANGE_LEAVE_BUFFER_BIT is NOT set, wOption is:
    ~ PH_EXCHANGE_DEFAULT:      we have the command byte and eventually parameter bytes (data),
    how ever, we already have all data to exchange the complete frame.
    ~ PH_EXCHANGE_BUFFER_FIRST: we have the command byte and eventually parameter bytes (data),
    but the callee wants to transmit some more.
    pDataParams->bLength < 2 -> the cmd byte was not transmitted yet
    */
    if ((0U == ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))) || (pDataParams->bLength < 2U))
    {
        /* If there is nothing to transmit, we are done. */
        if (wTxLength == 0U)
        {
            return PH_ERR_SUCCESS;
        }

        /* Prepare header */
        aHeader[0] = 0x00;          /* This is just a dummy which is updated with the correct value later on,
                                    when the last chunk is ready to transmit. */
        aHeader[1] = pTxBuffer[0];

        /* Buffer the header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aHeader,
            2,
            ppRxBuffer,
            pRxLength));

        /* until now, we have transmitted 2 bytes: */
        pDataParams->bLength = 2;

        /* following Hw_Echange calls shall not overwrite already buffered data */
        wOption |= PH_EXCHANGE_LEAVE_BUFFER_BIT;

        /* Check if IDm has to be send */
        wCmd = (uint16_t)pTxBuffer[0];
        switch (wCmd)
        {
        case PHPAL_FELICA_SW_CMD_REQ_RESP:
        case PHPAL_FELICA_SW_CMD_REQ_SERVICE:
        case PHPAL_FELICA_SW_CMD_READ:
        case PHPAL_FELICA_SW_CMD_WRITE:
        case PHPAL_FELICA_SW_CMD_AUTH1:
        case PHPAL_FELICA_SW_CMD_AUTH2:

            /* If there is no more parameter bytes (data) to send, we adjust pointer and length
            for the LAST Exchange, the IDm */
            if (wTxLength == 1U)
            {
                pTxBuffer = pDataParams->aIDmPMm;
                wTxLength = PHPAL_FELICA_IDM_LENGTH;
            }
            else
            {
                /* Set pointer and length for the LAST exchange. */
                pTxBuffer++;
                wTxLength = wTxLength - 1u;

                /* Transmit the IDm */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
                    pDataParams->pHalDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    pDataParams->aIDmPMm,
                    PHPAL_FELICA_IDM_LENGTH,
                    ppRxBuffer,
                    pRxLength));

                pDataParams->bLength = pDataParams->bLength + PHPAL_FELICA_IDM_LENGTH;
            }

            break;
        default:

            /* Set pointer and length for the LAST exchange. */
            pTxBuffer++;
            wTxLength = wTxLength - 1u;
            break;
        }
    }
    else
    {
        /* command byte already buffered */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, 0x01));
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wCmd));
    }

    /* Length check */
    if ((pDataParams->bLength + wTxLength) > PHPAL_FELICA_TXLENGTH_MAX)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_LENGTH_ERROR, PH_COMP_PAL_FELICA);
    }

    /* If the PH_EXCHANGE_BUFFERED_BIT IS set, wOption is:
    ~ PH_EXCHANGE_BUFFER_FIRST, or
    ~ PH_EXCHANGE_BUFFER_CONT: This means that there is data which shall be put into the
    buffer but not to be sent yet since more data will follow. */
    if (0U != (wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        if (wTxLength > 0U)
        {
            /* Exchange command */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
                pDataParams->pHalDataParams,
                wOption,
                pTxBuffer,
                wTxLength,
                ppRxBuffer,
                pRxLength));

            pDataParams->bLength = pDataParams->bLength + (uint8_t)wTxLength;
        }

        return PH_ERR_SUCCESS;
    }

    /* If the PH_EXCHANGE_BUFFERED_BIT is NOT set, wOption is:
    ~ PH_EXCHANGE_DEFAULT, or
    ~ PH_EXCHANGE_BUFFER_LAST: This means that either this is the last (or only) chunk
    of command parameters (data), which also could be of zero bytes.

    So this is the last chunk and we should received a response,
    which we extrakt the IDm bytes from and also check them. */
    pDataParams->bLength = pDataParams->bLength + (uint8_t)(wTxLength);

    /* Update the length byte which had been sent before */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, 0x00));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, (uint16_t)pDataParams->bLength));

    /* Reset precached length */
    pDataParams->bLength = 0x00;

    /* Get stored PMm */
    pPMm = &pDataParams->aIDmPMm[PHPAL_FELICA_IDM_LENGTH];

    /* Command and PMm evaluation */
    switch(wCmd)
    {
    case PHPAL_FELICA_SW_CMD_REQ_SERVICE:
        bTparam = pPMm[2];
        break;
    case PHPAL_FELICA_SW_CMD_REQ_RESP:
        bTparam = pPMm[3];
        break;
    case PHPAL_FELICA_SW_CMD_AUTH1:
    case PHPAL_FELICA_SW_CMD_AUTH2:
        bTparam = pPMm[4];
        break;
    case PHPAL_FELICA_SW_CMD_READ:
    case PHPAL_FELICA_SW_CMD_READ_SECF:
        bTparam = pPMm[5];
        break;
    case PHPAL_FELICA_SW_CMD_WRITE:
    case PHPAL_FELICA_SW_CMD_WRITE_SECF:
        bTparam = pPMm[6];
        break;
    default:
        bTparam = pPMm[7];
        break;
    }

    /* Get timing parameters */
    bA = bTparam & 0x07U;
    bB = (bTparam >> 3U) & 0x07U;
    bE = (bTparam >> 6U) & 0x03U;

    /* Timeout calculation */
    /* Response time = T x [(B + 1U) x n + (A + 1U)] x 4^E */
    dwTOut = ((((uint32_t)bB + 1U) * (uint32_t)wN) + ((uint32_t)bA + 1U));
    dwTimeoutUs = (uint32_t)(PHPAL_FELICA_SW_TIME_T_US * dwTOut * aFelica_E_Table[bE]);

    /* Timeout setting in Milliseconds */
    if (dwTimeoutUs > 0xFFFFU)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
            (uint16_t)(dwTimeoutUs / 1000U)));
    }
    /* Timeout setting in Microseconds */
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            (uint16_t)dwTimeoutUs));
    }

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength));

    /* Check length */
    if ((*pRxLength < 1U) || ((*ppRxBuffer)[0] != *pRxLength))
    {
        *pRxLength = 0;
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
    }

    /* Skip length */
    --(*pRxLength);
    ++(*ppRxBuffer);

    /* Check response code */
    if ((*pRxLength < 1U) || ((*ppRxBuffer)[0] != (wCmd + 1U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
    }

    /* Skip response code */
    --(*pRxLength);
    ++(*ppRxBuffer);

    /* Check and skip IDm if received */
    switch (wCmd)
    {
    case PHPAL_FELICA_SW_CMD_REQ_RESP:
    case PHPAL_FELICA_SW_CMD_REQ_SERVICE:
    case PHPAL_FELICA_SW_CMD_READ:
    case PHPAL_FELICA_SW_CMD_WRITE:
    case PHPAL_FELICA_SW_CMD_AUTH1:

        /* Protocol error check */
        if (*pRxLength < PHPAL_FELICA_IDM_LENGTH)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
        }

        /* We can only check the IDm if we have a valid one stored */
        if (memcmp(*ppRxBuffer, pDataParams->aIDmPMm, PHPAL_FELICA_IDM_LENGTH) != 0)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_FELICA);
        }

        /* Skip IDm */
        *pRxLength = *pRxLength - PHPAL_FELICA_IDM_LENGTH;
        *ppRxBuffer = *ppRxBuffer + PHPAL_FELICA_IDM_LENGTH;
        break;
    default:
        break;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalFelica_Sw_SetConfig(
                                    phpalFelica_Sw_DataParams_t* pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    )
{
    switch(wConfig)
    {
    case PHPAL_FELICA_CONFIG_RC:
        if(((uint8_t)wValue != 0x00U) && ((uint8_t)wValue != 0x01U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_FELICA);
        }
        pDataParams->bRequestCode = (uint8_t)(wValue);
        break;

    case PHPAL_FELICA_PREAMBLE_LENGTH:
        if(!((wValue == PHPAL_FELICA_PREAMBLE_LEN_48BITS) ||
            (wValue == PHPAL_FELICA_PREAMBLE_LEN_56BITS) ||
            (wValue == PHPAL_FELICA_PREAMBLE_LEN_64BITS) ||
            (wValue == PHPAL_FELICA_PREAMBLE_LEN_72BITS))
          )
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_FELICA);
        }
        pDataParams->bPreambleLen = (uint8_t)(wValue);
        break;

    default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_FELICA);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalFelica_Sw_GetConfig(
                                    phpalFelica_Sw_DataParams_t* pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    )
{
    switch(wConfig)
    {
    case PHPAL_FELICA_CONFIG_RC:
        *pValue = (uint16_t)pDataParams->bRequestCode;
        break;

    case PH_PALFELICA_CONFIG_NUM_RESPONSE_FRAMES:
        *pValue = (uint16_t)pDataParams->bTotalFrames;
        break;
    default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_FELICA);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalFelica_Sw_GetSerialNo(
                                      phpalFelica_Sw_DataParams_t* pDataParams,
                                      uint8_t * pIDmPMmOut,
                                      uint8_t * pLenIDmPMmOut
                                      )
{
    /* Return an error if IDm and PMm are not valid */
    if (pDataParams->bIDmPMmValid == PHPAL_FELICA_SW_IDMPMM_INVALID)
    {
        *pLenIDmPMmOut = 0;
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_FELICA);
    }

    /* copy the IDm and PMm */
    (void)memcpy(pIDmPMmOut, pDataParams->aIDmPMm, PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH);
    *pLenIDmPMmOut = PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH;

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHPAL_FELICA_SW */
