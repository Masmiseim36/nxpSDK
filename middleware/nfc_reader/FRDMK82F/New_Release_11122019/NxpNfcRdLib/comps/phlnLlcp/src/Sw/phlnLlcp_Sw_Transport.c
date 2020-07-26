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
* Software LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 14. April 2015
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phlnLlcp.h>

#ifdef NXPBUILD__PHLN_LLCP_SW

#include "phlnLlcp_Sw.h"
#include "phlnLlcp_Sw_Int.h"
#include "phlnLlcp_Sw_Mac.h"

phStatus_t phlnLlcp_Sw_Int_Socket_SendInt(phlnLlcp_Transport_Socket_t* psSocket,
                                          uint8_t* pTxBuffer,
                                          uint32_t dwLength,
                                          uint16_t wFrameOpt,
                                          uint8_t bState)
{
    phTools_Q_t * PH_MEMLOC_REM psMsgQueue = NULL;
    uint32_t      PH_MEMLOC_REM dwBlockTime = 0xFFFFFFFFU;
    phStatus_t    PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;
    phOsal_TimerPeriodObj_t timePeriodToWait;
    phStatus_t bRetstatus;

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = PHOSAL_MAX_DELAY;

    /* Just forward the buffer data to LLCP context using LLCP Queue mechanism.
    * Get the Queue */
    psMsgQueue = phTools_Q_Get(dwBlockTime, PH_OFF);

    if (psMsgQueue == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }
    psMsgQueue->pbData = pTxBuffer;
    psMsgQueue->dwLength = dwLength;

    /* Send the Queue */
    psMsgQueue->wFrameOpt = wFrameOpt;
    psMsgQueue->pSender = (void *)psSocket;
    psMsgQueue->bType = PH_TOOLS_Q_DATA_TO_BE_SENT;
    psSocket->bState = bState;
    PH_CHECK_SUCCESS_FCT(wStatus, phTools_Q_Send(psMsgQueue, dwBlockTime, wFrameOpt));

    /* Check for PHLN_LLCP_NO_MORE_FRAG and block on Queue(Semaphore) to be sent */
    if((wFrameOpt == PH_TRANSMIT_DEFAULT) || (wFrameOpt == PH_TRANSMIT_BUFFER_LAST))
    {
        /* Block on Semaphore until it gets a new remote connection. */
        PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPend(&psSocket->xSema.SemHandle, timePeriodToWait));

        wStatus = psSocket->wStatus;
    }
    return PH_ADD_COMPCODE(wStatus, PH_COMP_LN_LLCP);
}

phStatus_t phlnLlcp_Sw_Int_Pdu_CcOrC(phlnLlcp_Transport_Socket_t *psSocket,
                                     uint8_t bPtype,
                                     uint8_t *pServiceName,
                                     uint8_t bSnLength
                                     )
{
    uint16_t   PH_MEMLOC_REM wLength = 0;
    uint8_t    PH_MEMLOC_REM baPdu[12];
    phStatus_t PH_MEMLOC_REM wStatus;
    uint16_t   PH_MEMLOC_REM wLMiu;

    /* Frame the connect Or CC PDU */
    wLength = phlnLlcp_Sw_Int_Pdu_FrameHeader(bPtype, psSocket->bRsap, psSocket->bLsap, 0, 0, baPdu);

    baPdu[wLength++] = PHLN_LLCP_TLV_TYPE_MIUX;
    baPdu[wLength++] = PHLN_LLCP_TLV_LENGTH_MIUX;

    wLMiu = (uint16_t)((psSocket->dwBufLen > PHLN_LLCP_MIU) ? PHLN_LLCP_MIU: psSocket->dwBufLen);
    wLMiu -= 128u;

    baPdu[wLength++] = (uint8_t)((wLMiu & 0xFF00U) >> 8U);
    baPdu[wLength++] = (uint8_t)(wLMiu & 0xFFU);

    if((psSocket->bRsap == 0x01U) && (bPtype == PHLN_LLCP_PTYPE_CONNECT))
    {
        baPdu[wLength++] = PHLN_LLCP_TLV_TYPE_RW;
        baPdu[wLength++] = PHLN_LLCP_TLV_LENGTH_RW;
        baPdu[wLength++] = PHLN_LLCP_TLV_RW_VALUE;

        /* Add SN to TLV bytes sent during Connect PDU. */
        baPdu[wLength++] = PHLN_LLCP_TLV_TYPE_SN;
        baPdu[wLength++] = bSnLength;

        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_FIRST, baPdu, wLength);
        if(wStatus != PH_ERR_SUCCESS)
        {
            return wStatus;
        }

        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_LAST, pServiceName, bSnLength);
    }
    else if (bPtype == PHLN_LLCP_PTYPE_CONNECT)
    {
        baPdu[wLength++] = PHLN_LLCP_TLV_TYPE_RW;
        baPdu[wLength++] = PHLN_LLCP_TLV_LENGTH_RW;
        baPdu[wLength++] = PHLN_LLCP_TLV_RW_VALUE;

        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_DEFAULT, baPdu, wLength);
    }
    else
    {
        wStatus = phlnLlcp_Sw_Int_HandleMsgQueue(baPdu, wLength, (uint8_t)PH_TOOLS_Q_DATA_TO_BE_SENT);

        /* Change the socket state from Connect pending to Info Exchange */
        psSocket->bState = PHLN_LLCP_SOCKET_INFO_EX;
    }
    return wStatus;
}

phStatus_t phlnLlcp_Sw_Int_Pdu_Frmr(phlnLlcp_Transport_Socket_t *psSocket,
                                    uint8_t bPtype,
                                    uint8_t bLsap,
                                    uint8_t bRsap,
                                    uint8_t bSeq,
                                    uint8_t bReason,
                                    uint8_t bSend)
{
    uint8_t  PH_MEMLOC_REM baFrmr[6];
    uint16_t PH_MEMLOC_REM wLength;

    wLength = phlnLlcp_Sw_Int_Pdu_FrameHeader(PHLN_LLCP_PTYPE_FRMR, bRsap, bLsap, 0, 0, baFrmr);

    baFrmr[wLength++] = ((uint8_t)(bReason << 4U) | (uint8_t)bPtype);
    baFrmr[wLength++] = bSeq;
    baFrmr[wLength++] = (psSocket->sSeq.bSendState_Vs << 4U) | psSocket->sSeq.bRxState_Vr;
    baFrmr[wLength++] = (psSocket->sSeq.bSendAck_Vsa << 4U) | psSocket->sSeq.bRxAck_Vra;

    if (bSend == PH_OFF)
    {
        return phlnLlcp_Sw_Int_HandleMsgQueue(baFrmr, wLength, (uint8_t)PH_TOOLS_Q_DATA_TO_BE_SENT);
    }
    else
    {
        return phlnLlcp_MacTransmit(PH_TRANSMIT_DEFAULT, baFrmr, wLength);
    }
}

phStatus_t phlnLlcp_Sw_Int_Pdu_DiscOrDm(phlnLlcp_Transport_Socket_t *psSocket,
                                        uint8_t bPtype,
                                        uint8_t bReason)
{
    uint16_t PH_MEMLOC_REM wLength;
    uint8_t  PH_MEMLOC_REM pDiscPdu[3];

    /* Frame the Disconnect pdu */
    wLength = phlnLlcp_Sw_Int_Pdu_FrameHeader(bPtype, psSocket->bRsap, psSocket->bLsap, 0, 0, (uint8_t *)pDiscPdu);

    if(bPtype == PHLN_LLCP_PTYPE_DISC)
    {
        return phlnLlcp_MacTransmit(PH_TRANSMIT_DEFAULT, pDiscPdu, wLength);
    }
    else
    {
        pDiscPdu[wLength++] = bReason;
    }
    return phlnLlcp_Sw_Int_HandleMsgQueue(pDiscPdu, wLength, (uint8_t)PH_TOOLS_Q_DATA_TO_BE_SENT);
}

phStatus_t phlnLlcp_Sw_Int_Pdu_Snl(uint8_t bTid, uint8_t bSap)
{
    uint16_t PH_MEMLOC_REM wLength;
    uint8_t  PH_MEMLOC_REM pSnlPdu[6];

    /* Frame the Disconnect pdu */
    wLength = phlnLlcp_Sw_Int_Pdu_FrameHeader(PHLN_LLCP_PTYPE_SNL, 0x01, 0x01, 0, 0, (uint8_t *)pSnlPdu);

    pSnlPdu[wLength++] = PHLN_LLCP_TLV_TYPE_SDRES;
    pSnlPdu[wLength++] = PHLN_LLCP_TLV_LENGTH_SDRES;
    pSnlPdu[wLength++] = bTid;
    pSnlPdu[wLength++] = bSap;

    return phlnLlcp_Sw_Int_HandleMsgQueue(pSnlPdu, wLength, (uint8_t)PH_TOOLS_Q_DATA_TO_BE_SENT);
}

phStatus_t phlnLlcp_Sw_Int_Pdu_RrOrRnr(phlnLlcp_Transport_Socket_t *psSocket,
                                       uint8_t bPtype)
{
    uint16_t PH_MEMLOC_REM wLength;
    uint8_t  PH_MEMLOC_REM pResPdu[3];

    wLength = phlnLlcp_Sw_Int_Pdu_FrameHeader(bPtype, psSocket->bRsap, psSocket->bLsap, psSocket->sSeq.bRxState_Vr,
        0, pResPdu);

    psSocket->sSeq.bRxAck_Vra = psSocket->sSeq.bRxState_Vr;

    return phlnLlcp_Sw_Int_HandleMsgQueue(pResPdu, wLength, (uint8_t)PH_TOOLS_Q_DATA_TO_BE_SENT);
}


#endif /* NXPBUILD__PHLN_LLCP_SW */
