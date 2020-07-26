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
* Generic SNEP Protocol Management layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 05. Mar 2014
*
*/

#include <ph_Status.h>
#include <phnpSnep.h>

#ifdef NXPBUILD__PHNP_SNEP_SW

#include "phnpSnep_Sw.h"

phStatus_t phnpSnep_Sw_Init(
                            phnpSnep_Sw_DataParams_t * pDataParams,
                            uint16_t wSizeOfDataParams,
                            void * plnLlcpDataparams,
                            void * psSocket)
{
    if (sizeof(phnpSnep_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_NP_SNEP);
    }

    if((pDataParams == NULL) || (plnLlcpDataparams == NULL) || (psSocket == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* Initialize private data */
    pDataParams->wId = PH_COMP_NP_SNEP | PHNP_SNEP_SW_ID;
    pDataParams->plnLlcpDataParams = plnLlcpDataparams;
    pDataParams->psSocket = psSocket;

    return PH_ERR_SUCCESS;
}

/**
 * Assembles and returns the SNEP header for the specified Req/Res field.
 */
void phnpSnep_GetHeader(uint8_t eReqResField, uint32_t dwLength, uint8_t *pbHeader )
{
    pbHeader[0] = PHNP_SNEP_VER;
    pbHeader[1] = eReqResField;

    /* Transmission order SHALL be most significant octet first */
    pbHeader[2] = (uint8_t)(dwLength >> 24U);
    pbHeader[3] = (uint8_t)(dwLength >> 16U);
    pbHeader[4] = (uint8_t)(dwLength >> 8U);
    pbHeader[5] = (uint8_t)(dwLength);
}

phStatus_t phnpSnep_Sw_ClientInit(
                                  phnpSnep_Sw_DataParams_t * pDataParams,
                                  phnpSnep_Server_type_t eServerType,
                                  uint8_t *pNonDefaultUri,
                                  uint8_t *pbSnepRxBuffer,
                                  uint32_t dwSnepRxBufferSize
                                  )
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint8_t    PH_MEMLOC_REM aDefaultSnepURI[] = "urn:nfc:sn:snep";

    wStatus = PH_ERR_SUCCESS;
    /* Check for Valid input parameters. */
    if ((pbSnepRxBuffer == NULL) || (dwSnepRxBufferSize == 0U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* Check for Valid input parameters. */
    if ((pNonDefaultUri == NULL) && (eServerType == phnpSnep_NonDefault_Server))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Init(pDataParams->plnLlcpDataParams, pDataParams->psSocket,
        PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED, pbSnepRxBuffer, dwSnepRxBufferSize));

    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Register(pDataParams->plnLlcpDataParams, pDataParams->psSocket,
        PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED, 0, NULL));

    pDataParams->eServerType = eServerType;

    if (eServerType == phnpSnep_Default_Server)
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_ConnectViaUri(pDataParams->plnLlcpDataParams,
            pDataParams->psSocket, aDefaultSnepURI));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_ConnectViaUri(pDataParams->plnLlcpDataParams,
            pDataParams->psSocket, pNonDefaultUri));
    }

    return wStatus;
}


phStatus_t phnpSnep_Sw_ClientDeInit(phnpSnep_Sw_DataParams_t * pDataParams)
{
    phStatus_t PH_MEMLOC_REM wStatus;

    wStatus = phlnLlcp_Transport_Socket_Disconnect(pDataParams->plnLlcpDataParams, pDataParams->psSocket);

    /* Irrespective of Return status of Socket Disconnect, Socket Unregister needs to be done for Freeing memory. */
    (void)phlnLlcp_Transport_Socket_Unregister(pDataParams->plnLlcpDataParams, pDataParams->psSocket);
    return wStatus;
}

phStatus_t phnpSnep_Sw_ServerDeInit(phnpSnep_Sw_DataParams_t * pDataParams)
{
    return phlnLlcp_Transport_Socket_Unregister(pDataParams->plnLlcpDataParams, pDataParams->psSocket);
}

phStatus_t phnpSnep_Sw_ServerInit(
                                  phnpSnep_Sw_DataParams_t * pDataParams,
                                  phnpSnep_Server_type_t eServerType,
                                  uint8_t *pNonDefaultUri,
                                  uint8_t *pbSnepRxBuffer,
                                  uint32_t dwSnepRxBufferSize
                                  )
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint8_t    PH_MEMLOC_REM aDefaultSnepURI[] = "urn:nfc:sn:snep";

    /* Check for Valid input parameters. */
    if ((pbSnepRxBuffer == NULL) || (dwSnepRxBufferSize == 0U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* Check for Valid input parameters. */
    if ((pNonDefaultUri == NULL) && (eServerType == phnpSnep_NonDefault_Server))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Init(pDataParams->plnLlcpDataParams, pDataParams->psSocket,
        PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED, pbSnepRxBuffer, dwSnepRxBufferSize));

    if (eServerType == phnpSnep_Default_Server)
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Register(pDataParams->plnLlcpDataParams, pDataParams->psSocket,
            PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED, PHNP_SNEP_DEFAULT_SERVER_SAP, aDefaultSnepURI));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Register(pDataParams->plnLlcpDataParams, pDataParams->psSocket,
            PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED, PHNP_SNEP_NONDEFAULT_SERVER_SAP, pNonDefaultUri));
    }

    pDataParams->eServerType = eServerType;

    return phlnLlcp_Transport_Socket_WaitForConnection(pDataParams->plnLlcpDataParams, pDataParams->psSocket, NULL);
}

phStatus_t phnpSnep_Sw_ServerListen(
                                    phnpSnep_Sw_DataParams_t * pDataParams,
                                    uint32_t   dwGetReqBuffSize,
                                    uint8_t  * pGetReqData,
                                    uint32_t * dwGetReqLength,
                                    uint8_t  * pClientReq
                                    )
{
    phStatus_t                    PH_MEMLOC_REM wStatus;
    uint8_t                       PH_MEMLOC_REM bRxVersion;
    uint8_t                       PH_MEMLOC_REM bClientReq;
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pLocSocket = NULL;

    /* Check for Valid input parameters. */
    if (pClientReq == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* Check for Valid input parameters. */
    if ((pDataParams->eServerType == phnpSnep_NonDefault_Server) &&
        ((pGetReqData == NULL) || (dwGetReqLength == NULL) || (dwGetReqBuffSize == 0U)))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    pLocSocket = (phlnLlcp_Transport_Socket_t *)pDataParams->psSocket;

    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams, pLocSocket));

    bRxVersion = pLocSocket->pbRxBuffer[0];
    /* SNEP Version check. */
    if ((bRxVersion & PHNP_SNEP_VERSION_MAJOR_MASK) != (PHNP_SNEP_VER  & PHNP_SNEP_VERSION_MAJOR_MASK))
    {
        /* If SNEP Major version number doesn't match then Unsupported Version response will be sent. */
        PH_CHECK_SUCCESS_FCT(wStatus, phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_VER_ERR));

        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Disconnect(pDataParams->plnLlcpDataParams, pDataParams->psSocket));
        return (PH_ERR_INVALID_VERSION | PH_COMP_NP_SNEP);
    }

    /* Agreed SNEP version number. Currently as SNEP 1.0 version is supported and is directly assigned to Agreed SNEP version as minor number is already 0. */
    pDataParams->bSnepVersion = PHNP_SNEP_VER;

    bClientReq = pLocSocket->pbRxBuffer[1];
    if ((bClientReq > (uint8_t)PHNP_SNEP_REQ_PUT) && (bClientReq < (uint8_t)PHNP_SNEP_REQ_REJECT))
    {
        /* First Request from Client should either be PUT or GET. */
        PH_CHECK_SUCCESS_FCT(wStatus, phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_BAD_REQ));

        /* LLCP Socket Link should be De-Activated. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Disconnect(pDataParams->plnLlcpDataParams, pDataParams->psSocket));

        return (PH_ERR_SNEP_FAILED | PH_COMP_NP_SNEP);
    }
    else if (bClientReq > (uint8_t)PHNP_SNEP_REQ_REJECT)
    {
        /* LLCP Socket Link should be De-Activated. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Disconnect(pDataParams->plnLlcpDataParams, pDataParams->psSocket));

        return (PH_ERR_SNEP_FAILED | PH_COMP_NP_SNEP);
    }
    else if ((bClientReq == (uint8_t)PHNP_SNEP_REQ_GET) && (pDataParams->eServerType != phnpSnep_NonDefault_Server))
    {
        /* If Initialized Server Type is Default Server then Not Implemented response will be sent for GET Request. */
        PH_CHECK_SUCCESS_FCT(wStatus, phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_NOT_IMP));
        return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_NP_SNEP);
    }
    else
    {
        *pClientReq = bClientReq;
        if (bClientReq == (uint8_t)PHNP_SNEP_REQ_GET)
        {
            /* Save the Message in GET Request to Buffer. */
            if ((pLocSocket->dwLength - PHNP_SNEP_GET_HEADER_SIZE) <= dwGetReqBuffSize)
            {
                *dwGetReqLength = pLocSocket->dwLength - PHNP_SNEP_GET_HEADER_SIZE;
                (void)memcpy(pGetReqData, &pLocSocket->pbRxBuffer[PHNP_SNEP_GET_HEADER_SIZE], *dwGetReqLength);
            }
            else
            {
                /* Return Buffer Overflow in case the Buffer Size is less than length received Message in GET Request. */
                PH_CHECK_SUCCESS_FCT(wStatus, phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_REJECT));
                return (PH_ERR_BUFFER_OVERFLOW | PH_COMP_NP_SNEP);
            }
        }

        /* If SNEP Continue is received as first request from client then perform socket disconnect and return with error. */
        if (bClientReq == (uint8_t)PHNP_SNEP_REQ_CONT)
        {
            /* Received I-PDU in between sending Get Fragments. LLCP Socket Link should be De-Activated. */
            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Disconnect(pDataParams->plnLlcpDataParams, pDataParams->psSocket));
            return (PH_ERR_SNEP_FAILED | PH_COMP_NP_SNEP);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phnpSnep_Sw_ServerSendResponse(
                                          phnpSnep_Sw_DataParams_t * pDataParams,
                                          uint8_t bClientReq,
                                          uint8_t *pRespDataForGet,
                                          uint32_t dwRespDataLen,
                                          uint32_t dwPutBuffSize,
                                          uint8_t *pPutData,
                                          uint32_t *pdwPutDataLen
                                          )
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint8_t    PH_MEMLOC_BUF baResLen[6];
    uint32_t   PH_MEMLOC_REM dwMsgLen;
    uint32_t   PH_MEMLOC_REM dwLength;
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pLocSocket = NULL;
    uint8_t    PH_MEMLOC_REM bClientRequest;
    uint8_t    PH_MEMLOC_REM bRxFlag;

    bClientRequest = bClientReq;
    wStatus = PH_ERR_SUCCESS;
    pLocSocket = (phlnLlcp_Transport_Socket_t *)pDataParams->psSocket;

    /* Check for Valid input parameters in case of GET. */
    if ((bClientRequest == (uint8_t)PHNP_SNEP_REQ_GET) && (pDataParams->eServerType == phnpSnep_NonDefault_Server))
    {
        if ((pRespDataForGet == NULL) || (dwRespDataLen == 0U))
        {
            /* If the Input data is not provided then Not Found response will be sent. */
            return phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_NOT_FOUND);
        }
    }

    /* Check for Valid input parameters. */
    if ((pPutData == NULL) || (pdwPutDataLen == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    do
    {
        bRxFlag = 0;
        switch (bClientRequest)
        {
        case PHNP_SNEP_REQ_PUT:
            /* First SNEP Packet should contain entire SNEP Header. */
            if (pLocSocket->dwLength < PHNP_SNEP_HEADER_SIZE)
            {
                return (PH_ERR_PROTOCOL_ERROR | PH_COMP_NP_SNEP);
            }

            /* Extract PUT Message Length. */
            dwMsgLen = ((uint32_t)pLocSocket->pbRxBuffer[2] << 24U) | ((uint32_t)pLocSocket->pbRxBuffer[3] << 16U) |
                ((uint32_t)pLocSocket->pbRxBuffer[4] << 8U) | pLocSocket->pbRxBuffer[5];

            /* If the received PUT Request Message length is more than Buffer Size then Reject Response will be sent. */
            if (dwMsgLen > dwPutBuffSize)
            {
                PH_CHECK_SUCCESS_FCT(wStatus, phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_REJECT));
                return (PH_ERR_BUFFER_OVERFLOW | PH_COMP_NP_SNEP);
            }

            /* Copy to Data message. */
            dwLength = pLocSocket->dwLength - PHNP_SNEP_HEADER_SIZE;
            (void)memcpy(pPutData, &pLocSocket->pbRxBuffer[PHNP_SNEP_HEADER_SIZE], dwLength);
            pLocSocket->fReady = (uint8_t) 1U;

            if(dwMsgLen > dwLength)
            {
                wStatus = phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_CONT);
                if ((wStatus != PH_ERR_SUCCESS) && ((wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INFO_RECEIVED))
                {
                    /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
                    pLocSocket->fReady = (uint8_t) 1U;
                    return wStatus;
                }
                while(dwLength < dwMsgLen)
                {
                    if ((wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INFO_RECEIVED)
                    {
                        /*Rx from client*/
                        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams, pLocSocket));
                    }
                    wStatus = PH_ERR_SUCCESS;

                    (void)memcpy(&pPutData[dwLength], pLocSocket->pbRxBuffer, pLocSocket->dwLength);
                    /* Set the Ready flag in Socket to True to receive next Packet. */
                    pLocSocket->fReady = (uint8_t) 1U;
                    dwLength += pLocSocket->dwLength;
                }
            }
            /* Update the received length & Send success */
            *pdwPutDataLen = dwLength;
            wStatus = phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_SUCCESS);
            if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_INFO_RECEIVED)
            {
                bRxFlag = 1;
                bClientRequest = pLocSocket->pbRxBuffer[1];
            }
            break;

        case PHNP_SNEP_REQ_GET:
            /* Extract the Acceptable Length of the Client. */
            dwMsgLen = ((uint32_t)pLocSocket->pbRxBuffer[6] << 24U) | ((uint32_t)pLocSocket->pbRxBuffer[7] << 16U) |
            ((uint32_t)pLocSocket->pbRxBuffer[8] << 8U) | pLocSocket->pbRxBuffer[9];
            pLocSocket->fReady = (uint8_t) 1U;
            if (dwMsgLen < dwRespDataLen)
            {
                wStatus = phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_RES_EXCESS_DATA);
            }
            else
            {
                /* Transmission order SHALL be most significant octet first */
                baResLen[0] = PHNP_SNEP_VER;
                baResLen[1] = (uint8_t)PHNP_SNEP_RES_SUCCESS;
                baResLen[2] = (uint8_t)(dwRespDataLen >> 24U);
                baResLen[3] = (uint8_t)(dwRespDataLen >> 16U);
                baResLen[4] = (uint8_t)(dwRespDataLen >> 8U);
                baResLen[5] = (uint8_t)(dwRespDataLen);
                PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket,
                    baResLen, PHNP_SNEP_HEADER_SIZE, PH_TRANSMIT_BUFFER_FIRST));

                dwLength = (((dwRespDataLen + PHNP_SNEP_HEADER_SIZE) <= ((uint32_t)(pLocSocket->wRMiu))))? dwRespDataLen :  ((uint32_t)pLocSocket->wRMiu - PHNP_SNEP_HEADER_SIZE);
                wStatus = phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket,
                    pRespDataForGet, dwLength, PH_TRANSMIT_BUFFER_LAST);
                if ((wStatus != PH_ERR_SUCCESS) && ((wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INFO_RECEIVED))
                {
                    /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
                    pLocSocket->fReady = (uint8_t)1U;
                    return wStatus;
                }

                if(dwLength < dwRespDataLen)
                {
                    if ((wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INFO_RECEIVED)
                    {
                        /*Rx from client*/
                        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams,
                            pLocSocket));
                    }
                    /* Set the Ready flag in Socket to True to receive next Packet. */
                    pLocSocket->fReady = (uint8_t) 1U;

                    /* Check for the CONTINUE response is received by the SNEP server. */
                    if ((pLocSocket->dwLength != PHNP_SNEP_HEADER_SIZE)
                        || (pLocSocket->pbRxBuffer[1] != ((uint8_t)PHNP_SNEP_REQ_CONT)))
                    {
                        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_NP_SNEP);
                    }

                    do{
                        dwMsgLen = dwRespDataLen - dwLength;
                        dwMsgLen = (dwMsgLen > pLocSocket->wRMiu)? pLocSocket->wRMiu: dwMsgLen;
                        wStatus = phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, &pRespDataForGet[dwLength],
                            dwMsgLen, PH_TRANSMIT_DEFAULT);
                        if (wStatus == PH_ERR_SUCCESS)
                        {
                            /* Continue to send next Get Fragments. */
                            dwLength += dwMsgLen;
                        }
                        else if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_INFO_RECEIVED)
                        {
                            /* Received I-PDU in between sending Get Fragments. LLCP Socket Link should be De-Activated. */
                            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Disconnect(pDataParams->plnLlcpDataParams, pDataParams->psSocket));
                            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_NP_SNEP);
                        }
                        else
                        {
                            return wStatus;
                        }
                    } while(dwLength < dwRespDataLen);
                }
            }
            break;

        default:
            wStatus = (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
        }
    } while (bRxFlag == 1U);

    return wStatus;
}

phStatus_t phnpSnep_Sw_Put(
                           phnpSnep_Sw_DataParams_t * pDataParams,
                           uint8_t * pTxBuffer,
                           uint32_t dwTxBufferSize
                           )
{
    uint8_t                       PH_MEMLOC_REM baSnepHeader[6];
    phStatus_t                    PH_MEMLOC_REM wStatus;
    uint32_t                      PH_MEMLOC_REM dwRemBytesToSend = dwTxBufferSize;
    uint32_t                      PH_MEMLOC_REM dwLength;
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pLocSocket = NULL;
    uint8_t                       PH_MEMLOC_REM bRxResponse;

    bRxResponse = 0;
    pLocSocket = (phlnLlcp_Transport_Socket_t *)pDataParams->psSocket;

    /* Check for Valid input parameters. */
    if ((pTxBuffer == NULL) || (dwTxBufferSize == 0U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* Send/Append the Header and the NDEF Message*/
    phnpSnep_GetHeader((uint8_t)PHNP_SNEP_REQ_PUT, dwTxBufferSize, baSnepHeader);
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, baSnepHeader,
        PHNP_SNEP_HEADER_SIZE, PH_TRANSMIT_BUFFER_FIRST));

    if ((dwTxBufferSize + PHNP_SNEP_HEADER_SIZE) > pLocSocket->wRMiu)
    {
        /* Fragmentation is required as the frame size is greater than MIU */
        /* Send/Append the Header and the NDEF Message*/
        dwLength = ((uint32_t)(pLocSocket->wRMiu)) - PHNP_SNEP_HEADER_SIZE;
        wStatus = phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, pTxBuffer,
            dwLength, PH_TRANSMIT_BUFFER_LAST);

        if (wStatus == PH_ERR_SUCCESS)
        {
            /* Wait for the CONTINUE response from  the server */
            wStatus = phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams, pLocSocket);
            if (wStatus != PH_ERR_SUCCESS)
            {
                /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
                pLocSocket->fReady = (uint8_t) 1U;
                return wStatus;
            }
        }
        else if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_INFO_RECEIVED)
        {
            /* Do nothing as next I-PDU is received as response to the sent I-PDU. */
        }
        else
        {
            /* Return in case of any other error. */
            return wStatus;
        }

        bRxResponse = pLocSocket->pbRxBuffer[1];
        /* Set the Ready flag in Socket to True to receive next Packet. */
        pLocSocket->fReady = (uint8_t) 1U;

        /* Check for the CONTINUE response from the SNEP server */
        if ((pLocSocket->dwLength != PHNP_SNEP_HEADER_SIZE)
            || (bRxResponse != (uint8_t)PHNP_SNEP_RES_CONT))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_NP_SNEP);
        }

        /* Continue sending the remaining bytes to the SNEP server */
        dwRemBytesToSend -= dwLength;
        while (0U != dwRemBytesToSend)
        {
            dwLength = (dwRemBytesToSend > pLocSocket->wRMiu) ? pLocSocket->wRMiu : dwRemBytesToSend;
            if (dwRemBytesToSend == dwLength)
            {
                wStatus = phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket,
                    &pTxBuffer[dwTxBufferSize - dwRemBytesToSend], dwLength, PH_TRANSMIT_DEFAULT);
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket,
                    &pTxBuffer[dwTxBufferSize - dwRemBytesToSend], dwLength, PH_TRANSMIT_DEFAULT));
            }
            dwRemBytesToSend -= dwLength;
        }
    }
    else
    {
        /* No need of Fragmentation, just use lower layer to send the SNEP frame. */
        wStatus = phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, pTxBuffer,
            dwTxBufferSize, PH_TRANSMIT_BUFFER_LAST);
    }

    if (wStatus == PH_ERR_SUCCESS)
    {
        /* Wait for the SUCCESS response from  the SNEP server. */
        wStatus = phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams, pLocSocket);
        if (wStatus != PH_ERR_SUCCESS)
        {
            /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
            pLocSocket->fReady = (uint8_t) 1U;
            return wStatus;
        }
    }
    else if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_INFO_RECEIVED)
    {
        /* Do nothing as next I-PDU is received as response to the sent I-PDU. */
    }
    else
    {
        /* Return in case of any other error. */
        return wStatus;
    }

    bRxResponse = pLocSocket->pbRxBuffer[1];
    /* Set the Ready flag in Socket to True to receive next Packet. */
    pLocSocket->fReady = (uint8_t)1U;

    /* Check for the SUCCESS response from the SNEP server. */
    if (!(bRxResponse == (uint8_t)PHNP_SNEP_RES_SUCCESS))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_NP_SNEP);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phnpSnep_Sw_Get(
                           phnpSnep_Sw_DataParams_t * pDataParams,
                           uint8_t *pNdefMsg,
                           uint32_t dwNdefMsgLen,
                           uint8_t *pRxBuffer,
                           uint32_t *pdwRxLen,
                           uint32_t dwAppBufSize
                           )
{
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pLocSocket = NULL;
    uint8_t                       PH_MEMLOC_BUF baSnepHeader[10];
    phStatus_t                    PH_MEMLOC_REM wStatus;
    uint32_t                      PH_MEMLOC_REM dwLength;

    wStatus = PH_ERR_SUCCESS;
    pLocSocket = (phlnLlcp_Transport_Socket_t *)pDataParams->psSocket;

    /* Check for Valid input parameters. */
    if ((pNdefMsg == NULL) || (pRxBuffer == NULL) || (dwAppBufSize == 0U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* If the GET Message + Header Size is more than Remote MIU then this GET Request should not be sent. */
    if ((dwNdefMsgLen + PHNP_SNEP_ACCEPTABLE_LEN_FIELD + PHNP_SNEP_HEADER_SIZE) > pLocSocket->wRMiu)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_NP_SNEP);
    }

    /* Form SNEP Header. */
    dwLength = dwNdefMsgLen + PHNP_SNEP_ACCEPTABLE_LEN_FIELD;
    phnpSnep_GetHeader((uint8_t)PHNP_SNEP_REQ_GET, dwLength, baSnepHeader);

    /* Prepend Acceptable Length field after SNEP Header. Transmission order SHALL be most significant octet first. */
    baSnepHeader[6] = (uint8_t)(dwAppBufSize >> 24U);
    baSnepHeader[7] = (uint8_t)(dwAppBufSize >> 16U);
    baSnepHeader[8] = (uint8_t)(dwAppBufSize >> 8U);
    baSnepHeader[9] = (uint8_t)(dwAppBufSize);

    /* copy baSnepHeader and Acceptable Length field to HAL buffer using PH_TRANSMIT_BUFFER_FIRST option. */
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, baSnepHeader,
        (PHNP_SNEP_HEADER_SIZE + PHNP_SNEP_ACCEPTABLE_LEN_FIELD), PH_TRANSMIT_BUFFER_FIRST));

    wStatus = phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, pNdefMsg,
        dwNdefMsgLen, PH_TRANSMIT_BUFFER_LAST);

    if (wStatus == PH_ERR_SUCCESS)
    {
        /* Wait for the response Fragment from  the server */
        wStatus = phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams, pLocSocket);
        if (wStatus != PH_ERR_SUCCESS)
        {
            /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
            pLocSocket->fReady = (uint8_t) 1U;
            return wStatus;
        }
    }
    else if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_INFO_RECEIVED)
    {
        /* Do nothing as next I-PDU is received as response to the sent I-PDU. */
    }
    else
    {
        /* Return in case of any other error. */
        return wStatus;
    }

    /* Check server returned Excess data response. */
    if ((pLocSocket->pbRxBuffer[0] == PHNP_SNEP_VER) && (pLocSocket->pbRxBuffer[1] == (uint8_t)PHNP_SNEP_RES_EXCESS_DATA)
        && (pLocSocket->dwLength == PHNP_SNEP_HEADER_SIZE))
    {
        /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
        pLocSocket->fReady = (uint8_t)1U;
        return (PH_ERR_BUFFER_OVERFLOW | PH_COMP_NP_SNEP);
    }

    if (pLocSocket->pbRxBuffer[1] != ((uint8_t)PHNP_SNEP_RES_SUCCESS))
    {
        /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
        pLocSocket->fReady = (uint8_t)1U;
        return (PH_ERR_SNEP_GET_FAILED | PH_COMP_NP_SNEP);
    }

    if (pLocSocket->dwLength > PHNP_SNEP_HEADER_SIZE)
    {
        /* Extract Length Field in SUCCESS Response. */
        dwLength = ((uint32_t)pLocSocket->pbRxBuffer[2] << 24U) | ((uint32_t)pLocSocket->pbRxBuffer[3] << 16U) |
            ((uint32_t)pLocSocket->pbRxBuffer[4] << 8U) | pLocSocket->pbRxBuffer[5];

        /* If the received Response length is greater than Application buffer Size then REJECT Request will be sent to server. */
        if (dwLength > dwAppBufSize)
        {
            /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
            pLocSocket->fReady = (uint8_t)1U;
            return (PH_ERR_BUFFER_OVERFLOW | PH_COMP_NP_SNEP);
        }

        *pdwRxLen = pLocSocket->dwLength - PHNP_SNEP_HEADER_SIZE;
        (void)memcpy(pRxBuffer, &pLocSocket->pbRxBuffer[PHNP_SNEP_HEADER_SIZE], *pdwRxLen);
        /* Set the Ready flag in Socket to True to receive next Packet. */
        pLocSocket->fReady =(uint8_t)1U;

        if (dwLength > *pdwRxLen)
        {
            wStatus = phnpSnep_Sw_SendCmds(pDataParams, (uint8_t)PHNP_SNEP_REQ_CONT);
            if ((wStatus != PH_ERR_SUCCESS) && ((wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INFO_RECEIVED))
            {
                /* Set the Ready flag in Socket to True to receive next Packet and return with error. */
                pLocSocket->fReady = (uint8_t)1U;
                return wStatus;
            }
        }

        while (dwLength > *pdwRxLen)
        {
            if ((wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INFO_RECEIVED)
            {
                /* Wait for the response Fragment from  the server */
                PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Receive(pDataParams->plnLlcpDataParams, pLocSocket));
            }
            wStatus = PH_ERR_SUCCESS;

            if (dwLength >= (*pdwRxLen + pLocSocket->dwLength))
            {
                (void)memcpy(&pRxBuffer[*pdwRxLen], pLocSocket->pbRxBuffer, pLocSocket->dwLength);
                *pdwRxLen += pLocSocket->dwLength;
            }
            /* Set the Ready flag in Socket to True to receive next Packet. */
            pLocSocket->fReady = (uint8_t)1U;
        }
    }
    else
    {
        /* If the Received Response Length from Server is less than SNEP Header Size then throw Protocol Error. */
        phnpSnep_GetHeader((uint8_t)PHNP_SNEP_REQ_REJECT, dwLength, baSnepHeader);
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, baSnepHeader,
            PHNP_SNEP_HEADER_SIZE, PH_TRANSMIT_DEFAULT));
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_NP_SNEP);
    }

    return wStatus;
}

phStatus_t phnpSnep_Sw_SendCmds(
                                phnpSnep_Sw_DataParams_t * pDataParams,
                                uint8_t eReqResField
                                )
{
    static uint8_t PH_MEMLOC_REM baHeader[6];
    phlnLlcp_Transport_Socket_t* pLocSocket = (phlnLlcp_Transport_Socket_t *) pDataParams->psSocket;

    phnpSnep_GetHeader(eReqResField, 0, baHeader);

    return phlnLlcp_Transport_Socket_Send(pDataParams->plnLlcpDataParams, pLocSocket, baHeader, PHNP_SNEP_HEADER_SIZE, PH_TRANSMIT_DEFAULT);
}

#endif /* NXPBUILD__PHNP_SNEP_SW */
