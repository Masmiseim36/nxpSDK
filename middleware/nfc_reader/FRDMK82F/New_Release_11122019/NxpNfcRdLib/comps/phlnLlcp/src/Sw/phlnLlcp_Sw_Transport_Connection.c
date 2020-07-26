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

phStatus_t phlnLlcp_Sw_Transport_Socket_Connect(
                                                phlnLlcp_Sw_DataParams_t  *pDataParams,
                                                phlnLlcp_Transport_Socket_t* pClientSocket,
                                                uint8_t bDsap
                                                )
{
    /* Check for Valid input parameters.
     * Value of Dsap cannot be 0 or 1 as these are reserved by LLCP. */
    if ((pClientSocket == NULL) || (bDsap < 2U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    pClientSocket->bRsap = bDsap;
    PH_UNUSED_VARIABLE(pDataParams);
    return phlnLlcp_Sw_Int_Socket_SendInt(pClientSocket, NULL, 0, PH_TRANSMIT_DEFAULT, PHLN_LLCP_SOCKET_CONN_PEND);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_ConnectViaUri(
                                                      phlnLlcp_Sw_DataParams_t  *pDataParams,
                                                      phlnLlcp_Transport_Socket_t* pClientSocket,
                                                      uint8_t *pUri
                                                      )
{
    uint32_t PH_MEMLOC_REM dwUriLen;

    /* Check for Valid input parameters. */
    if ((pClientSocket == NULL) || (pUri == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    dwUriLen = strlen((char *)(pUri));

    /* Should be always 0x01 to connect through Uri */
    pClientSocket->bRsap = 1;
    PH_UNUSED_VARIABLE(pDataParams);
    return phlnLlcp_Sw_Int_Socket_SendInt(pClientSocket, pUri, dwUriLen, PH_TRANSMIT_DEFAULT,
        PHLN_LLCP_SOCKET_CONN_PEND);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_WaitForConnection(
                                                          void * pDataParams,
                                                          phlnLlcp_Transport_Socket_t * pServerSocket,
                                                          phlnLlcp_Transport_Socket_t * pConnServerSocket
                                                          )
{
    phStatus_t PH_MEMLOC_REM wStatus;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;
    phOsal_TimerPeriodObj_t timePeriodToWait;

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = PHOSAL_MAX_DELAY;

    /* Check for Valid input parameters. */
    if (pServerSocket == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    pServerSocket->bState = PHLN_LLCP_SOCKET_WAITONCONN;

    /* Block on Semaphore until it gets a new remote connection. */
    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPend(&pServerSocket->xSema.SemHandle, timePeriodToWait));

    if ((pConnServerSocket != NULL) && ((pServerSocket->wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS))
    {
        /* Connected to the remote LLC */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_Transport_Socket_Register(pConnServerSocket,
            pServerSocket->eSocketType, pServerSocket->bLsap, pServerSocket->pUri, PHLN_LLCP_CONNECTIONSERVER_SOCKET));

        pConnServerSocket->bRsap = pServerSocket->bRsap;
        pServerSocket->bRsap = 0;
        pConnServerSocket->bState = pServerSocket->bState;
        pServerSocket->bState = PHLN_LLCP_SOCKET_WAITONCONN;
    }
    PH_UNUSED_VARIABLE(pDataParams);
    return PH_ADD_COMPCODE(pServerSocket->wStatus, PH_COMP_LN_LLCP);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_Send(
                                             phlnLlcp_Sw_DataParams_t * pDataParams,
                                             phlnLlcp_Transport_Socket_t * pClientSocket,
                                             uint8_t * pTxBuffer,
                                             uint32_t dwTxBufferSize,
                                             uint16_t wFrameOpt
                                             )
{
    /* Check for Valid input parameters. */
    if ((pClientSocket == NULL) || (pTxBuffer == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }
    PH_UNUSED_VARIABLE(pDataParams);
    return phlnLlcp_Sw_Int_Socket_SendInt(pClientSocket, pTxBuffer, dwTxBufferSize, wFrameOpt, PHLN_LLCP_SOCKET_INFO_SEND_EX);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_DiscoverServices(
                                                         phlnLlcp_Sw_DataParams_t * pDataParams,
                                                         phlnLlcp_Transport_Socket_t * pSocket,
                                                         uint8_t  bListSize,
                                                         uint8_t  bSNBuffSize,
                                                         uint8_t *  psServiceNameList,
                                                         uint8_t *  pSapList
                                                         )
{
    phTools_Q_t * PH_MEMLOC_REM psMsgQueue = NULL;
    uint32_t      PH_MEMLOC_REM dwBlockTime = (uint32_t)0xFFFFFFFFU;
    phStatus_t    PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;
    phOsal_TimerPeriodObj_t timePeriodToWait;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;

    /* Check for Valid input parameters. */
    if ((pSocket == NULL) || (bListSize == 0U) || (bSNBuffSize == 0U) ||
        (psServiceNameList == NULL) || (pSapList == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    if (pDataParams->bSdpClientSAP != 0U)
    {
        return (PH_ERR_USE_CONDITION | PH_COMP_LN_LLCP);
    }

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = PHOSAL_MAX_DELAY;
    pDataParams->bSdpClientSAP = pSocket->bLsap;

    /* Just forward the buffer data to LLCP context using LLCP Queue mechanism.
    * Get the Queue */
    psMsgQueue = phTools_Q_Get(dwBlockTime, PH_OFF);

    if (psMsgQueue == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }
    psMsgQueue->pbData = psServiceNameList;
    psMsgQueue->dwLength = bSNBuffSize;

    /* Send the Queue */
    psMsgQueue->wFrameOpt = bListSize;
    psMsgQueue->pSender = (void *)pSocket;
    psMsgQueue->bType = PH_TOOLS_Q_DATA_TO_BE_SENT;
    pSocket->bState =  PHLN_LLCP_SOCKET_SNL;
    PH_CHECK_SUCCESS_FCT(wStatus, phTools_Q_Send(psMsgQueue, dwBlockTime, PH_TRANSMIT_DEFAULT));

    /* Block on Semaphore until it gets a new remote connection. */
    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPend(&pSocket->xSema.SemHandle, timePeriodToWait));


    /* Reset Global DataParams bSdpClientSAP value to zero. */
    pDataParams->bSdpClientSAP = 0;

    if (pSocket->wStatus == PH_ERR_SUCCESS)
    {
        if (pSocket->dwLength != 0U)
        {
            /* Parse the Received SNL TLV Field for SDRES Value field for SAP Values. */
            phlnLlcp_Sw_Int_ParseSAP(pSocket->pbRxBuffer, (uint16_t)pSocket->dwLength, bListSize, pSapList);
        }
        return PH_ERR_SUCCESS;
    }

    return (pSocket->wStatus | PH_COMP_LN_LLCP);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_Disconnect(
                                                   phlnLlcp_Sw_DataParams_t* pDataParams,
                                                   phlnLlcp_Transport_Socket_t * pClientSocket
                                                   )
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;

    /* Check for Valid input parameters. */
    if (pClientSocket == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }
    wStatus = phlnLlcp_Sw_Int_Socket_SendInt(pClientSocket, NULL, 0, PH_TRANSMIT_DEFAULT, PHLN_LLCP_SOCKET_DISC_PEND);

    if (((wStatus & PH_ERR_MASK) == PH_ERR_LLCP_DEACTIVATED) || (pClientSocket->bState == PHLN_LLCP_SOCKET_DISC))
    {
        wStatus = PH_ERR_SUCCESS;
    }

    PH_UNUSED_VARIABLE(pDataParams);
    return wStatus;
}

phStatus_t phlnLlcp_Sw_Int_Pdu_SnlEx(phlnLlcp_Transport_Socket_t *psSocket,
                                     uint8_t *pbTxData,
                                     uint16_t wLength,
                                     uint16_t wNoURI)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_INTERNAL_ERROR;
    uint8_t    PH_MEMLOC_REM baLlcpHeader[5];
    uint8_t    PH_MEMLOC_REM bSDREQLen;
    uint8_t    PH_MEMLOC_REM bCount;
    uint8_t    PH_MEMLOC_REM bOffset;

    bOffset = 0;
    bCount = 0;
    for (; bCount < wNoURI; bCount++)
    {
        bSDREQLen = pbTxData[bOffset++];

        if (bCount == 0U)
        {
            PHLN_LLCP_PDU_FRAME_MANDATORY_HEADER(0x01U, PHLN_LLCP_PTYPE_SNL, 0x01U, baLlcpHeader);
            baLlcpHeader[2] = PHLN_LLCP_TLV_TYPE_SDREQ;
            baLlcpHeader[3] = bSDREQLen + 1U;
            baLlcpHeader[4] = PHLN_LLCP_SW_FIRST_TID;

            /* Send First SDREQ TLV with Header. */
            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_FIRST, baLlcpHeader, 5));
        }
        else
        {
            baLlcpHeader[0] = PHLN_LLCP_TLV_TYPE_SDREQ;
            baLlcpHeader[1] = bSDREQLen + 1U;
            baLlcpHeader[2] = (uint8_t)(PHLN_LLCP_SW_FIRST_TID + bCount);

            /* Send SDREQ TLV with Header. */
            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_CONT, baLlcpHeader, 3));
        }

        if (bCount == (wNoURI - 1u))
        {
            /* Send Last URI. */
            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_LAST, &pbTxData[bOffset], bSDREQLen));
        }
        else
        {
            /* Send URI. */
            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_CONT, &pbTxData[bOffset], bSDREQLen));
        }

        /* Update the Offset to point to next Length Field. */
        bOffset += bSDREQLen;
    }
    PH_UNUSED_VARIABLE(psSocket);
    PH_UNUSED_VARIABLE(wLength);
    return PH_ADD_COMPCODE(wStatus, PH_COMP_LN_LLCP);
}

phStatus_t phlnLlcp_Sw_Int_Pdu_InfoEx(phlnLlcp_Transport_Socket_t *psSocket,
                                      uint8_t *pbTxData,
                                      uint16_t wLength,
                                      uint16_t wFrameOpt,
                                      uint8_t *pbPerformRx)
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint8_t    PH_MEMLOC_REM baLlcpHeader[3];

    switch(wFrameOpt)
    {
    case PH_TRANSMIT_BUFFER_FIRST:
        /* frame I PDU */
        PHLN_LLCP_PDU_FRAME_HEADER(psSocket->bRsap, PHLN_LLCP_PTYPE_INFO, psSocket->bLsap, psSocket->sSeq.bSendState_Vs,
            psSocket->sSeq.bRxState_Vr, baLlcpHeader);
        /* Send I PDU frame Header */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_FIRST, baLlcpHeader, 3));
        /* Append above layers header, inform above layers header is NOT the last fragment */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_CONT, pbTxData, wLength);
        *pbPerformRx = (uint8_t) 0U;
        break;

    case PH_TRANSMIT_BUFFER_LAST:
        /* Just pass the buffer to the lower layer */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_LAST, pbTxData, wLength);
        PHLN_LLCP_SW_MOD16_INC(psSocket->sSeq.bSendState_Vs);
        psSocket->sSeq.bRxAck_Vra = psSocket->sSeq.bRxState_Vr;
        break;

    case PH_TRANSMIT_DEFAULT:
        /* frame I PDU */
        PHLN_LLCP_PDU_FRAME_HEADER(psSocket->bRsap, PHLN_LLCP_PTYPE_INFO, psSocket->bLsap, psSocket->sSeq.bSendState_Vs,
            psSocket->sSeq.bRxState_Vr, baLlcpHeader);
        PHLN_LLCP_SW_MOD16_INC(psSocket->sSeq.bSendState_Vs);
        psSocket->sSeq.bRxAck_Vra = psSocket->sSeq.bRxState_Vr;
        /* Send I PDU frame Header */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_FIRST, baLlcpHeader, 3));
        /* Append above layers data, inform above layers data is the last fragment */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_LAST, pbTxData, wLength);
        break;

    case PH_TRANSMIT_BUFFER_CONT :
        /* Just pass the buffer to the lower layer */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_CONT, pbTxData, wLength);
        *pbPerformRx = (uint8_t) 0U;
        break;

    default:
        wStatus = (PH_ERR_INTERNAL_ERROR | PH_COMP_LN_LLCP);
    }

    return wStatus;
}

#endif /* NXPBUILD__PHLN_LLCP_SW */
