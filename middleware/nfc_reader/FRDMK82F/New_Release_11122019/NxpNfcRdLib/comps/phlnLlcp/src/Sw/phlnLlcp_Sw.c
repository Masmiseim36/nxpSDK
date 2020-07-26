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
*  KK: Generated 18. Feb 2015
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phlnLlcp.h>

#ifdef NXPBUILD__PHLN_LLCP_SW

#include "phlnLlcp_Sw.h"
#include "phlnLlcp_Sw_Int.h"
#include "phlnLlcp_Sw_Mac.h"
#include "phlnLlcp_Timers.h"

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
static const uint8_t bLlcpEventName[] = "Llcp Event";
static const uint8_t bLlcpxSemaName[] = "LlcpxSema";
static const uint8_t bLlcpxRxSemaName[] = "LlcpxRxSema";

/* LM (Head) socket to link all the registered sockets.
* Used to parse the registered sockets to get a particular Socket.  */
phlnLlcp_Transport_Socket_t *gpphlnLlcp_Socket_RegSockets = NULL;

/* Global socket used to handled LLC Link Management operations. */
phlnLlcp_Transport_Socket_t gsphlnLlcp_Socket;


/* *****************************************************************************************************************
* Functions
* ***************************************************************************************************************** */

phStatus_t phlnLlcp_Sw_Init(
                            phlnLlcp_Sw_DataParams_t  *pDataParams,
                            uint16_t  wSizeOfDataParams,
                            uint8_t * pLocalGenBytes,
                            uint8_t * pLocalGenBytesLen)
{
    uint8_t    PH_MEMLOC_REM wLength = 0;

    if (sizeof(phlnLlcp_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
    }

    if (pDataParams == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    /* Initialize private data */
    pDataParams->wId                     = PH_COMP_LN_LLCP | PHLN_LLCP_SW_ID;
    pDataParams->sLocalLMParams.bVersion = PHLN_LLCP_TLV_VERSION_VALUE;
    pDataParams->bCurrentClientCnt       = 0;
    pDataParams->bSdpClientSAP           = 0;

    pLocalGenBytes[wLength++] = PHLN_LLCP_MAGIC_NUM_BYTE1;
    pLocalGenBytes[wLength++] = PHLN_LLCP_MAGIC_NUM_BYTE2;
    pLocalGenBytes[wLength++] = PHLN_LLCP_MAGIC_NUM_BYTE3;

    /* Version number will be appended by default. */
    pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_TYPE_VERSION;
    pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_LENGTH_VERSION;
    pLocalGenBytes[wLength++] = pDataParams->sLocalLMParams.bVersion;

    if(0U != (pDataParams->sLocalLMParams.bAvailableTlv & PHLN_LLCP_TLV_MIUX_MASK))
    {
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_TYPE_MIUX;
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_LENGTH_MIUX;
        pLocalGenBytes[wLength++] = (uint8_t)((pDataParams->sLocalLMParams.wMiu & 0xFF00U) >> 8U);
        pLocalGenBytes[wLength++] = (uint8_t)(pDataParams->sLocalLMParams.wMiu & 0xFFU);
    }

    if (0U != (pDataParams->sLocalLMParams.bAvailableTlv & PHLN_LLCP_TLV_WKS_MASK))
    {
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_TYPE_WKS;
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_LENGTH_WKS;
        pLocalGenBytes[wLength++] = (uint8_t)((pDataParams->sLocalLMParams.wWks & 0xFF00U) >> 8U);
        pLocalGenBytes[wLength++] = (uint8_t)(pDataParams->sLocalLMParams.wWks & 0xFFU);
    }

    if (0U != (pDataParams->sLocalLMParams.bAvailableTlv & PHLN_LLCP_TLV_LTO_MASK))
    {
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_TYPE_LTO;
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_LENGTH_LTO;
        pLocalGenBytes[wLength++] = pDataParams->sLocalLMParams.bLto;
    }

    if (0U != (pDataParams->sLocalLMParams.bAvailableTlv & PHLN_LLCP_TLV_OPT_MASK))
    {
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_TYPE_OPT;
        pLocalGenBytes[wLength++] = PHLN_LLCP_TLV_LENGTH_OPT;
        pLocalGenBytes[wLength++] = pDataParams->sLocalLMParams.bOpt & 0x03U;
    }

    *pLocalGenBytesLen = wLength;

    /* Create the event. */
    pDataParams->LlcpEventObj.pEvtName =  (uint8_t *) bLlcpEventName;
    pDataParams->LlcpEventObj.intialValue = 0;
    return phOsal_EventCreate(&pDataParams->LlcpEventObj.EventHandle, &pDataParams->LlcpEventObj);
}

phStatus_t phlnLlcp_Sw_Activate(
                                phlnLlcp_Sw_DataParams_t * pDataParams,
                                uint8_t                  * pRemoteGnrlBytes,
                                uint8_t                    bRemoteGnrBytesLen,
                                uint8_t                    bDevType)
{
    phStatus_t                     PH_MEMLOC_REM statusTmp;
    phStatus_t                     PH_MEMLOC_REM wStatus;
    phTools_Q_t *                  PH_MEMLOC_REM psMsgQueue = NULL;
    uint8_t              		   PH_MEMLOC_REM epType;
    phlnLlcp_Transport_Socket_t *  PH_MEMLOC_REM psQueuedSocket = NULL;
    uint8_t *                      PH_MEMLOC_REM pRxBuffer = NULL;
    uint8_t *                      PH_MEMLOC_REM pTempBuffer = NULL;
    uint8_t                        PH_MEMLOC_REM bLlcpBuffer[32]; /* For internal LLCP use case */
    uint8_t                        PH_MEMLOC_REM bLlcpBuflen = 32;
    uint8_t                        PH_MEMLOC_REM bPerformRx;
    uint16_t                       PH_MEMLOC_REM wRxLen = 0;
    uint16_t                       PH_MEMLOC_REM wTempLen = 0;
    uint16_t                       PH_MEMLOC_REM wSymmTime;
    uint16_t                       PH_MEMLOC_REM wLtoTime;

    /* Reset Pointer to head of Registered Sockets to NULL. */
    gpphlnLlcp_Socket_RegSockets = NULL;

    /* Check for Magic number and parse the Parameters. */
    if (phlnLlcp_Sw_Int_CheckGenBytes(pRemoteGnrlBytes, bRemoteGnrBytesLen, &(pDataParams->sRemoteLMParams)) != PH_ERR_SUCCESS)
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
    }

    /* Version agreement procedure as per NFCForum LLCP 1.2 - Section 5.2.2. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Sw_Int_VersionAgreement(pDataParams->sLocalLMParams.bVersion , pDataParams->sRemoteLMParams.bVersion,
        &(pDataParams->bAgreedVersion)));

    /* Initialize MAC layer. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Sw_MacInit(pDataParams, bDevType, &wSymmTime, &wLtoTime));

    /* Initialize Tools for Message Queues. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phTools_Q_Init());

    /* Initialize LLCP Link Management socket. */
    (void)phlnLlcp_Sw_Transport_Socket_Init(pDataParams, &gsphlnLlcp_Socket, PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED, bLlcpBuffer, bLlcpBuflen);

    /* Register LLCP Link Management socket.*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Sw_Int_Transport_Socket_Register(&gsphlnLlcp_Socket, PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED, 0, NULL, PHLN_LLCP_DEFAULTLINK_SOCKET));
    gsphlnLlcp_Socket.bState = PHLN_LLCP_SOCKET_INFO_EX;

    /* Post LLCP Activated Event to Threads Blocked for LLCP Event. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phOsal_EventPost(&pDataParams->LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL));

    if (bDevType == PHLN_LLCP_INITIATOR)
    {
        //(wSymmTime > 2000U) ? : ;
        pDataParams->LlcpSymTimerObj.timePeriod.period = wSymmTime ;
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_SymStart(pDataParams));
    }
    else
    {
        pDataParams->LlcpLtoTimerObj.timePeriod.period = wLtoTime;
        /* Start LinkLoss Timer. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_LtoStart(pDataParams));

        /* Block on 18092 Receive */
        wStatus = phlnLlcp_MacReceive(PH_RECEIVE_DEFAULT, &pRxBuffer, &wRxLen);
        if (wStatus != PH_ERR_SUCCESS)
        {
            return wStatus;
        }

        /* Stop LinkLoss Timer & Start SYMM Timer. */
        pDataParams->LlcpSymTimerObj.timePeriod.period = wSymmTime ;
        pDataParams->LlcpLtoTimerObj.timePeriod.period = 0;
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_LtoStop(pDataParams));
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_SymStart(pDataParams));

        /* Post to the Queue PH_TOOLS_Q_RX_DATA */
        PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Sw_Int_PostRxMsgQueue(pRxBuffer, wRxLen));
    }

    while(TRUE)
    {
        /* Block on llcpMsgQsema, Acquire the llcpMsgQMutex and release */
        psMsgQueue = phTools_Q_Receive((uint32_t)0xFFFFFFFFU);
        if (psMsgQueue == NULL)
        {
            return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
        }

        switch(psMsgQueue->bType)
        {
        case PH_TOOLS_Q_DATA_TO_BE_SENT:
            /* Stop SYMM Timer & perform 18092 Transmit. */
            pDataParams->LlcpSymTimerObj.timePeriod.period = 0;
            PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Timers_SymStop(pDataParams));
            wStatus = phlnLlcp_Sw_Int_Send(psMsgQueue, &bPerformRx);
            if (wStatus != PH_ERR_SUCCESS)
            {
                (void)phlnLlcp_MacDeactivation(pDataParams);
                return wStatus;
            }

            /* Take backup of socket that is processed. */
            psQueuedSocket = psMsgQueue->pSender;

            /* Release the message queue */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_Q_Release(psMsgQueue, (uint32_t)0xFFFFFFFFU));

            if (bPerformRx == (uint8_t) 1U)
            {
                /* Start LinkLoss Timer & perform  18092 Receive. */
                pDataParams->LlcpLtoTimerObj.timePeriod.period = wLtoTime;
                PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Timers_LtoStart(pDataParams));
                wStatus = phlnLlcp_MacReceive(PH_RECEIVE_DEFAULT, &pRxBuffer, &wRxLen);
                if (wStatus != PH_ERR_SUCCESS)
                {
                    /* During MAC in Target mode, if MAC is Deactivated then LLCP should be intimated
                     * and skip MAC de-activation procedure and return to application.*/
                    if (((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_DESELECTED) ||
                        ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_RELEASED))
                    {
                        return (PH_ERR_PEER_DISCONNECTED | PH_COMP_LN_LLCP);
                    }

                    /* In active mode during LLCP link de-activation if peer device did not respond or if there is
                     * an error from HAL/MAC then return peer disconnected error and skip MAC de-activation.*/
                    if (((wStatus & PH_ERR_MASK) == PH_ERR_RF_ERROR) ||
                        ((wStatus & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR))
                    {
                        if ((psQueuedSocket->bState == PHLN_LLCP_SOCKET_DISC) &&
                            (psQueuedSocket->bLsap == 0U) &&
                            (psQueuedSocket->bRsap == 0U))
                        {
                            /* During LLCP Link de-activation using DISC PDU (SSAP/DSAP = 0), if any errors observed
                             * from HAL/MAC layer then return peer disconnected error. */
                            return (PH_ERR_PEER_DISCONNECTED | PH_COMP_LN_LLCP);
                        }
                        else
                        {
                            return wStatus;
                        }
                    }

                    if ((wStatus & PH_ERR_MASK) != PH_ERR_ABORTED)
                    {
                        PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_MacDeactivation(pDataParams));
                    }

                    /* In Target mode even if LTO has occurred, only LLCP should be de-activated.
                     * MAC should be active till error in RF/DSL_REQ is sent by Initiator. */
                    /* If external RF of initiator is stuck or initiator doesn't send any request
                     * within LTO or when an application forcefully aborts using HAL Asyncabort then
                     * this condition will occur and return "PH_ERR_ABORTED" status to application. */
                    if ((bDevType == PHLN_LLCP_TARGET) && ((wStatus & PH_ERR_MASK) == PH_ERR_ABORTED))
                    {
                        /* Start hard coded linkloss timer & perform MAC de-activation. */
                        pDataParams->LlcpLtoTimerObj.timePeriod.period = 4000;
                        PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Timers_LtoStart(pDataParams));
                        wStatus = phlnLlcp_MacReceive(PH_RECEIVE_DEFAULT, &pRxBuffer, &wRxLen);
                        if (wStatus == PH_ERR_SUCCESS)
                        {
                            /* Attempt MAC link de-activation. */
                            (void)phlnLlcp_MacDeactivation(pDataParams);
                            wStatus = (PH_ERR_PEER_DISCONNECTED | PH_COMP_LN_LLCP);
                        }
                    }

                    return wStatus;
                }

                /* Stop LinkLoss Timer & Start SymmTimer */
                pDataParams->LlcpLtoTimerObj.timePeriod.period = 0;
                pDataParams->LlcpSymTimerObj.timePeriod.period = wSymmTime;
                PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Timers_LtoStop(pDataParams));
                PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Timers_SymStart(pDataParams));

                /* Post the Received Data to the Message queue */
                PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Sw_Int_PostRxMsgQueue(pRxBuffer, wRxLen));
            }

            break;

        case PH_TOOLS_Q_RX_DATA:
            /* Get socket using DSAP & SSAP, Intimate to Server/Client by posting Semaphore. */
            epType = PHLN_LLCP_PDU_GET_PTYPE(psMsgQueue->pbData[0], psMsgQueue->pbData[1]);

            wStatus = phlnLlcp_Sw_Int_Pdu_Process(pDataParams, psMsgQueue->pbData, psMsgQueue->dwLength, epType, &psQueuedSocket);
            if (((wStatus & PH_ERR_MASK) == PH_ERR_LLCP_DEACTIVATED) ||
                ((wStatus & PH_ERR_MASK) == PH_ERR_PEER_DISCONNECTED) ||
                ((wStatus & PH_ERR_MASK) == PH_ERR_LLCP_PDU_FRMR))
            {
                if (bDevType == PHLN_LLCP_TARGET)
                {
                    /* Start LinkLoss Timer & perform MAC Link De-activation. */
                    pDataParams->LlcpLtoTimerObj.timePeriod.period = 4000;
                    PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_Timers_LtoStart(pDataParams));
                }

                PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_MacDeactivation(pDataParams));
                return wStatus;
            }

            /* release the message queue */
            pTempBuffer = psMsgQueue->pbData;
            wTempLen = (uint16_t)psMsgQueue->dwLength;
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_Q_Release(psMsgQueue, (uint32_t)0xFFFFFFFFU));

            statusTmp = phlnLlcp_Sw_Int_PostEvents(wStatus, epType, psQueuedSocket, pTempBuffer, pDataParams->bAgreedVersion);
            /* We can use RxBuffer until next Rx */
            if (statusTmp == PH_ON)
            {
                wStatus = phlnLlcp_Sw_Int_Pdu_Handle(wStatus, psQueuedSocket, pTempBuffer, wTempLen);

                /* Exit LLC Activate API after freeing the resources and intimating the waiting sockets. */
                if (wStatus != PH_ERR_SUCCESS)
                {
                    /* After receiving DISC PDU perform MAC Link De-Activation. */
                    if ((wStatus & PH_ERR_MASK) == PH_ERR_PEER_DISCONNECTED)
                    {
                        PH_CHECK_SUCCESS_FCT(statusTmp, phlnLlcp_MacDeactivation(pDataParams));
                    }
                    return wStatus;
                }
            }
            else
            {
                if (statusTmp != PH_OFF)
                {
                    return statusTmp;
                }
            }
            break;
        /* Queue type will only take Send/Receive Types. */
        } /* PRQA S 2002 */
    }
}


phStatus_t phlnLlcp_Sw_Deactivate(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phTools_Q_t *  PH_MEMLOC_REM psMsgQueue = NULL;
    uint16_t       PH_MEMLOC_REM wLength;
    phStatus_t     PH_MEMLOC_REM wStatus;
    static uint8_t PH_MEMLOC_REM baDisc[2];

    wLength = phlnLlcp_Sw_Int_Pdu_FrameHeader(PHLN_LLCP_PTYPE_DISC, gsphlnLlcp_Socket.bRsap, gsphlnLlcp_Socket.bLsap, 0,
        0, baDisc);

    psMsgQueue = phTools_Q_Get((uint32_t)0xFFFFFFFFU, PH_OFF);
    if (psMsgQueue == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }
    gsphlnLlcp_Socket.bState = PHLN_LLCP_SOCKET_DISC_PEND;

    psMsgQueue->pbData = baDisc;
    psMsgQueue->dwLength = wLength;
    psMsgQueue->bType = PH_TOOLS_Q_DATA_TO_BE_SENT;
    psMsgQueue->pSender = (void *)&gsphlnLlcp_Socket;
    PH_CHECK_SUCCESS_FCT(wStatus, phTools_Q_SendFront(psMsgQueue, (uint32_t)0xFFFFFFFFU, PH_TRANSMIT_DEFAULT));
    PH_UNUSED_VARIABLE(pDataParams);
    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_WaitForActivation(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    if (phOsal_EventPend((volatile phOsal_Event_t * )(&pDataParams->LlcpEventObj.EventHandle), E_OS_EVENT_OPT_PEND_SET_ANY, PHOSAL_MAX_DELAY, E_PH_OSAL_EVT_LLCP_ACTIVATED,
        NULL) != PH_ERR_SUCCESS)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Transport_Socket_Init(
                                             phlnLlcp_Sw_DataParams_t  *pDataParams,
                                             phlnLlcp_Transport_Socket_t* psSocket,
                                             phlnLlcp_Transport_Socket_Type_t eSocketType,
                                             uint8_t* pSocketRxBuffer,
                                             uint32_t dwSocketRxBufferSize)
{
    phStatus_t bRetstatus;
    PH_UNUSED_VARIABLE(pDataParams);
    /* Check for Valid input parameters. */
    if (psSocket == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    psSocket->eSocketType = eSocketType;
    psSocket->dwBufLen = dwSocketRxBufferSize;
    psSocket->pbRxBuffer = pSocketRxBuffer;

    psSocket->fReady = (uint8_t) 1U;

    psSocket->xSema.pSemName = (uint8_t *)bLlcpxSemaName;
    psSocket->xSema.semInitialCount = 0;
    psSocket->xSema.semMaxCount = 1;

    bRetstatus = phOsal_SemCreate(&psSocket->xSema.SemHandle, &psSocket->xSema, E_OS_SEM_OPT_SIGNALLING_SEM);
    if (bRetstatus != PH_ERR_SUCCESS)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }

    psSocket->xRxSema.pSemName = (uint8_t *)bLlcpxRxSemaName;
    psSocket->xRxSema.semInitialCount = 0;
    psSocket->xRxSema.semMaxCount = 1;

    bRetstatus = phOsal_SemCreate(&psSocket->xRxSema.SemHandle, &psSocket->xRxSema, E_OS_SEM_OPT_SIGNALLING_SEM);
    if (bRetstatus != PH_ERR_SUCCESS)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }
    psSocket->pNext = NULL;

    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Transport_Socket_Register(
                                                 phlnLlcp_Sw_DataParams_t* pDataParams,
                                                 phlnLlcp_Transport_Socket_t* pSocket,
                                                 phlnLlcp_Transport_Socket_Type_t eSocketType,
                                                 uint8_t bLsap,
                                                 uint8_t * pUri)
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint8_t    PH_MEMLOC_REM bClientLsap;

    /* Check for Valid input parameters. */
    if (pSocket == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    if ((eSocketType == PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED) ||
        (eSocketType == PHLN_LLCP_TRANSPORT_CONNECTIONLESS))
    {
        if ((bLsap < 2U) || (bLsap > 63U))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
        }
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_Transport_Socket_GetSap(pDataParams, &bClientLsap));
        bLsap = bClientLsap;
    }

    return phlnLlcp_Sw_Int_Transport_Socket_Register(pSocket, eSocketType, bLsap, pUri, PHLN_LLCP_LOGICALLINK_SOCKET);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_Receive(
                                                phlnLlcp_Sw_DataParams_t * pDataParams,
                                                phlnLlcp_Transport_Socket_t* psSocket
                                                )
{
    phOsal_TimerPeriodObj_t timePeriodToWait;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;
    /* Check for Valid input parameters. */
    if (psSocket == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = PHOSAL_MAX_DELAY;

    /* Block on receive semaphore */
    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPend(&psSocket->xRxSema.SemHandle, timePeriodToWait));
    PH_UNUSED_VARIABLE(pDataParams);
    return PH_ADD_COMPCODE(psSocket->wStatus, PH_COMP_LN_LLCP);
}

phStatus_t phlnLlcp_Sw_Transport_Socket_Unregister(
                                                   phlnLlcp_Sw_DataParams_t * pDataParams,
                                                   phlnLlcp_Transport_Socket_t * pSocket
                                                   )
{
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pSocketList = NULL;

    /* Check for Valid input parameters. */
    if (pSocket == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }

    if ((pSocket->eSocketType == PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED) && (pDataParams->bCurrentClientCnt != 0U))
    {
        pDataParams->bCurrentClientCnt--;
    }

    /* Release acquired memories */
    if (pSocket->xSema.SemHandle != NULL)
    {
        (void)phOsal_SemDelete(&pSocket->xSema.SemHandle);
        pSocket->xSema.SemHandle = NULL;
    }
    if (pSocket->xRxSema.SemHandle != NULL)
    {
        (void)phOsal_SemDelete(&pSocket->xRxSema.SemHandle);
        pSocket->xRxSema.SemHandle = NULL;
    }

    /* Remove socket from the registered list */
    if (gpphlnLlcp_Socket_RegSockets == pSocket)
    {
        gpphlnLlcp_Socket_RegSockets = gpphlnLlcp_Socket_RegSockets->pNext;
        return PH_ERR_SUCCESS;
    }

    if (gpphlnLlcp_Socket_RegSockets != NULL)
    {
        pSocketList = gpphlnLlcp_Socket_RegSockets;
        while (pSocketList->pNext != pSocket)
        {
            if (pSocketList->pNext == NULL)
            {
                /* Socket is not Registered. */
                return (PH_ERR_LLCP_SOCKET_NOT_REGISTERED | PH_COMP_LN_LLCP);
            }
            pSocketList = pSocketList->pNext;
        }
        pSocketList->pNext = pSocket->pNext;
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Transport_Socket_UnblockAll(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phlnLlcp_Transport_Socket_t PH_MEMLOC_REM *psSockets = NULL;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;

    if (gpphlnLlcp_Socket_RegSockets == NULL)
    {
        return PH_ERR_SUCCESS;
    }

    /* Check for bLsap */
    psSockets = gpphlnLlcp_Socket_RegSockets;

    while (psSockets != NULL)
    {
        psSockets->bState = PHLN_LLCP_SOCKET_DISC;
        psSockets->wStatus = PH_ERR_LLCP_DEACTIVATED;

        PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSockets->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
        PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSockets->xSema.SemHandle, E_OS_SEM_OPT_NONE));

        psSockets = psSockets->pNext;
    }

    /* Before Link De-activation assign NULL to Global  */
    gpphlnLlcp_Socket_RegSockets = NULL;
    PH_UNUSED_VARIABLE(pDataParams);
    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_DeInit(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;

    /* Stop LTO Timer. */
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_StopLto(pDataParams));

    /* Consume LLCP Activated Event. */
    PH_CHECK_SUCCESS_FCT(wStatus, phOsal_EventClear(&pDataParams->LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL));

    /* Delete LLC SYMM and LTO Timers. */
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_DeInitSym(pDataParams));
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_DeInitLto(pDataParams));

    /* De-Initialize the Tools. */
    phTools_Q_DeInit();

    /* Unblock all the blocked Sockets as LLC will be closed. */
    (void)phlnLlcp_Sw_Transport_Socket_UnblockAll(pDataParams);

    /* Delete Semaphore of Socket used for LLC Link Management. */
    if (gsphlnLlcp_Socket.xSema.SemHandle != NULL)
    {
        (void)phOsal_SemDelete(&gsphlnLlcp_Socket.xSema.SemHandle);
        gsphlnLlcp_Socket.xSema.SemHandle = NULL;
    }
    if (gsphlnLlcp_Socket.xRxSema.SemHandle != NULL)
    {
        (void)phOsal_SemDelete(&gsphlnLlcp_Socket.xRxSema.SemHandle);
        gsphlnLlcp_Socket.xRxSema.SemHandle = NULL;
    }

    /* Reset the current registered client count. */
    pDataParams->bCurrentClientCnt = 0;

    return wStatus;
}

#endif /* NXPBUILD__PHLN_LLCP_SW */
