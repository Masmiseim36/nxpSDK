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
*  KK: Generated 19. Feb 2015
*
*/

#include <phlnLlcp.h>

#ifdef NXPBUILD__PHLN_LLCP_SW

#include "phlnLlcp_Sw_Int.h"
#include "phlnLlcp_Sw_Mac.h"

/* *****************************************************************************************************************
* Extern Variables
* ***************************************************************************************************************** */
extern phlnLlcp_Transport_Socket_t *gpphlnLlcp_Socket_RegSockets;

#if PHLN_LLCP_AGF
/* To Handle AGF a Global Buffer is allocated to copy the AGF Frame and if #PHLN_LLCP_AGF
*  is set to Zero then code to handle AGF is not include and RAM size will be reduced. */
static uint8_t gphlnLlcp_AGFBuffer[PHLN_LLCP_MIU + PHLN_LLCP_HEADER_SIZE];
#endif /* PHLN_LLCP_AGF */

phStatus_t phlnLlcp_Sw_Int_Transport_Socket_Register(
    phlnLlcp_Transport_Socket_t* pSocket,
    phlnLlcp_Transport_Socket_Type_t eSocketType,
    uint8_t bLsap,
    uint8_t * pUri,
    phlnLlcp_SocketType_t eType)
{
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pNextSocket = NULL;

    /* Initializing URI Parameter in Socket to NULL. */
    pSocket->pUri = NULL;

    switch (eSocketType)
    {
    case PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED:
        pSocket->bLsap = bLsap;
        break;
    case PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED:
        pSocket->bLsap = bLsap;
        if (pUri != NULL)
        {
            pSocket->pUri = pUri;
        }
        break;
    case PHLN_LLCP_TRANSPORT_CONNECTIONLESS:
        pSocket->bLsap = bLsap;
        if (pUri != NULL)
        {
            pSocket->pUri = pUri;
        }
        break;
    } /* PRQA S 2002 */

    /* Update the fields & register it to the Sockets */
    pSocket->bRsap = 0;      /* default */
    pSocket->eSocketType = eSocketType;
    pSocket->sSeq.bSendState_Vs = 0;
    pSocket->sSeq.bSendAck_Vsa = 0;
    pSocket->sSeq.bRxState_Vr = 0;
    pSocket->sSeq.bRxAck_Vra = 0;
    pSocket->pNext = NULL;

    if ((eType != PHLN_LLCP_DEFAULTLINK_SOCKET) && (gpphlnLlcp_Socket_RegSockets == NULL))
    {
        return (PH_ERR_LLCP_SOCKET_REGISTER_FAILED | PH_COMP_LN_LLCP);
    }

    if (gpphlnLlcp_Socket_RegSockets == NULL)
    {
        gpphlnLlcp_Socket_RegSockets = pSocket;
        return PH_ERR_SUCCESS;
    }

    pNextSocket = gpphlnLlcp_Socket_RegSockets;

    /* Find the tail */
    if (pNextSocket->pNext != NULL)
    {
        do
        {
            pNextSocket = pNextSocket->pNext;

            /* Check if the Socket is already registered with some SAP. */
            if (pNextSocket == pSocket)
            {
                return (PH_ERR_LLCP_SOCKET_REGISTERED | PH_COMP_LN_LLCP);
            }

            /* Check if the passed Lsap is already associated with another registered socket. */
            if ((pNextSocket->bLsap == pSocket->bLsap) && (eType == PHLN_LLCP_LOGICALLINK_SOCKET))
            {
                return (PH_ERR_LLCP_SOCKET_REGISTER_FAILED | PH_COMP_LN_LLCP);
            }
        }while (pNextSocket->pNext != NULL);
    }

    pNextSocket->pNext = pSocket;
    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Int_CheckGenBytes(uint8_t *pGenBytes, uint16_t wLength, phlnLlcp_LMDataParams_t *pLMBytes)
{
    uint8_t gkphlnLlcp_MagicNum[PHLN_LLCP_HEADER_SIZE] = {0x46, 0x66, 0x6D};
    if(wLength >= 3U)
    {
        if(0 != memcmp(gkphlnLlcp_MagicNum, pGenBytes, PHLN_LLCP_HEADER_SIZE))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }
    }
    else
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
    }

    /* Parse the General bytes. */
    return phlnLlcp_Sw_Int_ParseGenBytes((uint8_t *)(pGenBytes + 3U), wLength - (uint16_t)3U, pLMBytes);
}

uint8_t * phlnLlcp_Sw_Int_GetUri(uint8_t *pGenBytes, uint16_t wLength, uint8_t *pbUriLen, uint8_t *pbTid, uint8_t *pbSDREQPresent)
{
    uint8_t * PH_MEMLOC_REM pUri = NULL;
    uint16_t  PH_MEMLOC_REM wIndex, wTemp;
    wIndex = 0;
    wTemp = 0;
    *pbUriLen = 0;

    while(wIndex < (wLength - (uint16_t)2U))
    {
        /* Check for pre-defined valid TLVs */
        if(pGenBytes[wIndex] > PHLN_LLCP_TLV_TYPE_SDRES)
        {
            return NULL;
        }

        switch(pGenBytes[wIndex])
        {
        case PHLN_LLCP_TLV_TYPE_VERSION:
        case PHLN_LLCP_TLV_TYPE_LTO:
        case PHLN_LLCP_TLV_TYPE_OPT:
        case PHLN_LLCP_TLV_TYPE_RW:
            wIndex += 3U;
            break;

        case PHLN_LLCP_TLV_TYPE_MIUX:
        case PHLN_LLCP_TLV_TYPE_WKS:
        case PHLN_LLCP_TLV_TYPE_SDRES:
            wIndex += 4U;
            break;

        case PHLN_LLCP_TLV_TYPE_SN:
            *pbUriLen = pGenBytes[wIndex + 1U];
            wTemp = *pbUriLen;
            pUri = &pGenBytes[wIndex + 2U];
            wIndex = wIndex + (2U + wTemp);
            break;

        case PHLN_LLCP_TLV_TYPE_SDREQ:
            *pbUriLen = pGenBytes[wIndex + 1U] - (uint8_t)1U;
            wTemp = *pbUriLen;
            if(pbTid != NULL)
            {
                *pbTid = pGenBytes[wIndex + (uint16_t)2U];
            }
            pUri = &pGenBytes[wIndex + (uint16_t)3U];
            wIndex = wIndex + (3U + wTemp);

            if (pbSDREQPresent != NULL)
            {
                *pbSDREQPresent = PH_ON;
            }
            break;

        default:
            /* Exit while loop */
            wIndex = wLength;
            break;
        }
    }

    return pUri;
}

void phlnLlcp_Sw_Int_ParseSAP(uint8_t *pGenBytes, uint16_t wLength, uint8_t bSAPCount, uint8_t *pSAPVal)
{
    uint16_t   PH_MEMLOC_REM wIndex;
    uint8_t    PH_MEMLOC_REM bRxSAPCount;

    wIndex = 0;
    bRxSAPCount = 0;
    while(wIndex < (wLength - (uint16_t)2U))
    {
        /* Check for pre-defined valid TLVs */
        if(pGenBytes[wIndex] > PHLN_LLCP_TLV_TYPE_SDRES)
        {
            return;
        }

        switch(pGenBytes[wIndex])
        {
        case PHLN_LLCP_TLV_TYPE_VERSION:
        case PHLN_LLCP_TLV_TYPE_LTO:
        case PHLN_LLCP_TLV_TYPE_OPT:
        case PHLN_LLCP_TLV_TYPE_RW:
            wIndex += 3U;
            break;

        case PHLN_LLCP_TLV_TYPE_MIUX:
        case PHLN_LLCP_TLV_TYPE_WKS:
            wIndex += 4U;
            break;

        case PHLN_LLCP_TLV_TYPE_SN:
            wIndex = wIndex + (uint16_t)pGenBytes[wIndex + 1U] + 2U;
            break;

        case PHLN_LLCP_TLV_TYPE_SDREQ:
            wIndex = wIndex + (uint16_t)pGenBytes[wIndex + 1U] + 2U;
            break;

        case PHLN_LLCP_TLV_TYPE_SDRES:
            if (bRxSAPCount <= bSAPCount)
            {
                pSAPVal[bRxSAPCount++] = (pGenBytes[wIndex + 3U] & 0x3FU);
            }
            wIndex += 4U;
            break;

        default:
            /* Exit while loop */
            wIndex = wLength;
            break;
        }
    }
}

phStatus_t phlnLlcp_Sw_Int_ParseGenBytes(uint8_t *pGenBytes, uint16_t wLength, phlnLlcp_LMDataParams_t *pLMBytes)
{
    uint16_t   PH_MEMLOC_REM wIndex;

    pLMBytes->bVersion = PHLN_LLCP_INVALID_VERSION;
    wIndex = 0;

    while(wIndex < (wLength - (uint16_t)2U))
    {
        /* Check for pre-defined valid TLVs */
        if(pGenBytes[wIndex] > PHLN_LLCP_TLV_TYPE_SDRES)
        {
            return PH_ERR_SUCCESS;
        }

        pLMBytes->bAvailableTlv |= (uint8_t)(1U << pGenBytes[wIndex]);
        switch(pGenBytes[wIndex])
        {
        case PHLN_LLCP_TLV_TYPE_VERSION:
            if(pGenBytes[wIndex + 1U] == PHLN_LLCP_TLV_LENGTH_VERSION)
            {
                pLMBytes->bVersion = pGenBytes[wIndex + 2U];
            }
            else
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
            wIndex += 3U;
            break;

        case PHLN_LLCP_TLV_TYPE_MIUX:
            pLMBytes->wMiu = ((uint16_t)pGenBytes[wIndex + 2U] << 8U);
            pLMBytes->wMiu |= pGenBytes[wIndex + 3U];
            wIndex += 4U;
            break;

        case PHLN_LLCP_TLV_TYPE_WKS:
            pLMBytes->wWks = ((uint16_t)pGenBytes[wIndex + 2U] << 8U);
            pLMBytes->wWks |= pGenBytes[wIndex + 3U];
            wIndex += 4U;
            break;

        case PHLN_LLCP_TLV_TYPE_LTO:
            pLMBytes->bLto = pGenBytes[wIndex + 2U];
            wIndex += 3U;
            break;

        case PHLN_LLCP_TLV_TYPE_OPT:
            pLMBytes->bOpt = pGenBytes[wIndex + 2U];
            wIndex += 3U;
            break;

            /* discard RW as we are fixed to 1 */
        case PHLN_LLCP_TLV_TYPE_RW:
            wIndex += 3U;
            break;

        case PHLN_LLCP_TLV_TYPE_SN:
            wIndex += pGenBytes[wIndex + 1U];
            break;

        case PHLN_LLCP_TLV_TYPE_SDREQ:
        case PHLN_LLCP_TLV_TYPE_SDRES:
            /* TODO: Handle these TLV. */
            break;

        default:
            /* Exit while loop */
            wIndex = wLength;
            break;
        }
    }

    /* Check if a VERSION TLV has been provided. */
    if (pLMBytes->bVersion == PHLN_LLCP_INVALID_VERSION)
    {
        /* Protocol Error as Mandatory VERSION parameter not provided. */
        return PH_ERR_PROTOCOL_ERROR;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Int_VersionAgreement(uint8_t    bLocalVersion,
                                            uint8_t    bRemoteVersion,
                                            uint8_t   *pAgreedVersion)
{
    uint8_t PH_MEMLOC_REM bLocalMajor  = (uint8_t)(bLocalVersion  & PHLN_LLCP_VERSION_MAJOR_MASK);
    uint8_t PH_MEMLOC_REM bLocalMinor  = (uint8_t)(bLocalVersion  & PHLN_LLCP_VERSION_MINOR_MASK);
    uint8_t PH_MEMLOC_REM bRemoteMajor = (uint8_t)(bRemoteVersion & PHLN_LLCP_VERSION_MAJOR_MASK);
    uint8_t PH_MEMLOC_REM bRemoteMinor = (uint8_t)(bRemoteVersion & PHLN_LLCP_VERSION_MINOR_MASK);

    /* Compare Major numbers */
    if (bLocalMajor == bRemoteMajor)
    {
        /* Version Agreement succeed : use Lowest Version */
        *pAgreedVersion = bLocalMajor | ((bRemoteMinor < bLocalMinor)? bRemoteMinor : bLocalMinor);
    }
    else if (bLocalMajor > bRemoteMajor)
    {
        /* Decide if versions are compatible. */
        /* Currently, there is no backward compatibility to handle. */
        return PH_ERR_PROTOCOL_ERROR;
    }
    else /* if (localMajor < remoteMajor) */
    {
        /* It is up to the remote host to decide if versions are compatible. */
        /* Set the negotiated version and the remote will De-Activate the link if its version agreement fails. */
        *pAgreedVersion = bLocalVersion;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Int_Transport_Socket_GetSap(phlnLlcp_Sw_DataParams_t* pDataParams,
                                                   uint8_t *pbSap)
{
    if(pDataParams->bCurrentClientCnt <= 31U)
    {
        *pbSap = pDataParams->bCurrentClientCnt + 32U;
        pDataParams->bCurrentClientCnt++;
        return PH_ERR_SUCCESS;
    }

    return (PH_ERR_LLCP_SAP_EXCEEDED | PH_COMP_LN_LLCP);
}

/**
*
* LLC PDU Format
*
* | <------------------- LLCP Header --------------------> | LLCP Payload |
* | DSAP   | PTYPE  | SSAP   | Sequence                    | Information  |
* | 6 Bits | 4 Bits | 6 Bits | 0 or 8 Bits                 |  N - Bytes   |
*                            | N(S) 4 Bits | N(R) 4 Bits   |
*
*/
uint16_t phlnLlcp_Sw_Int_Pdu_FrameHeader(uint8_t ePType,
                                         uint8_t bDsap,
                                         uint8_t bSsap,
                                         uint8_t bNr,
                                         uint8_t bNs,
                                         uint8_t *pbPduFrame)
{
    uint8_t PH_MEMLOC_REM bLength = 2;

    /* Frame the LLCP header */
    pbPduFrame[0] = (uint8_t)(PHLN_LLCP_PDU_MERGE_DSAP(bDsap, ePType));
    pbPduFrame[1] = (uint8_t)(PHLN_LLCP_PDU_MERGE_SSAP(ePType, bSsap));

    switch(ePType)
    {
    case PHLN_LLCP_PTYPE_RR:
    case PHLN_LLCP_PTYPE_RNR:
        /* append sequence */
        pbPduFrame[bLength++] = bNr;
        break;

    case PHLN_LLCP_PTYPE_INFO:
        /* append sequence */
        pbPduFrame[bLength++] = (bNs << 4U) | bNr;
        break;

    default:
        break;
    }
    return bLength;
}

phlnLlcp_Transport_Socket_t *phlnLlcp_Transport_Socket_Search(uint8_t bLsap,
                                                              uint8_t bRsap,
                                                              uint8_t bIgnoreRsap)
{
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pSockets = NULL;

    if(gpphlnLlcp_Socket_RegSockets == NULL)
    {
        return NULL;
    }

    /* Check for bLsap */
    pSockets = gpphlnLlcp_Socket_RegSockets;

    /* Compares Lsap, Rsap Or with Uri by checking Rsap as 0x01 */
    while ((pSockets != NULL) &&
        (((pSockets->bLsap != bLsap)) || ( (bIgnoreRsap != (uint8_t)1U) && ((pSockets->bRsap != bRsap) && (pSockets->bRsap != 0x01U)))))
    {
        pSockets = pSockets->pNext;
    }

    return pSockets;
}

phlnLlcp_Transport_Socket_t *phlnLlcp_Transport_Socket_SearchUri(uint8_t *pUri,
                                                                 uint8_t bUriLen)
{
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM pSockets = NULL;

    if((gpphlnLlcp_Socket_RegSockets == NULL) || (pUri == NULL) || (bUriLen == 0U) )
    {
        return NULL;
    }

    /* Check for Uri */
    pSockets = gpphlnLlcp_Socket_RegSockets;

    while( pSockets!= NULL )
    {
        if(pSockets->pUri != NULL)
        {
            /* perform Uri compare and if success break */
            if (0 == memcmp(pSockets->pUri , pUri, (size_t)strlen((char *)pSockets->pUri)))
            {
                break;
            }
        }
        pSockets = pSockets->pNext;
    }

    return pSockets;
}

phStatus_t phlnLlcp_Sw_Int_HandleMsgQueue(uint8_t *pBuffer, uint16_t wLen, uint8_t bType)
{
    phTools_Q_t *     PH_MEMLOC_REM psMsgQueue = NULL;

    psMsgQueue = phTools_Q_Get((uint32_t)0xFFFFFFFFU, PH_ON);
    if (psMsgQueue == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }

    if (bType == PH_TOOLS_Q_DATA_TO_BE_SENT)
    {
        (void)memcpy(psMsgQueue->bLlcpBuf, pBuffer, wLen);
        psMsgQueue->dwLength = wLen;
        psMsgQueue->bType = bType;

        psMsgQueue->bLlcpData = PH_ON;

        return phTools_Q_Send(psMsgQueue, (uint32_t)0xFFFFFFFFU, PH_TRANSMIT_DEFAULT);
    }
    else
    {
        psMsgQueue->pbData = pBuffer;
        psMsgQueue->dwLength = wLen;
        psMsgQueue->bType = bType;

        return phTools_Q_SendFront(psMsgQueue, 0xFFFFFFFFU, PH_TRANSMIT_DEFAULT);
    }
}

phStatus_t phlnLlcp_Sw_Int_PostRxMsgQueue(uint8_t *pRxBuffer, uint16_t wRxLen)
{
#if PHLN_LLCP_AGF
  //  phlnLlcp_PType_t  PH_MEMLOC_REM epType;
	uint8_t			  PH_MEMLOC_REM epType;
    uint8_t           PH_MEMLOC_REM bRsap;
    uint8_t           PH_MEMLOC_REM bLsap;
    uint16_t          PH_MEMLOC_REM wCnt;
    uint16_t          PH_MEMLOC_REM wPduLen = 0;
    uint16_t          PH_MEMLOC_REM wAgfLen = 0;
    phStatus_t        PH_MEMLOC_REM wStatus;

    wPduLen = 0;
    wAgfLen = 0;
    /* check for AGF frame */
 //   epType = (phlnLlcp_PType_t)PHLN_LLCP_PDU_GET_PTYPE(pRxBuffer[0], pRxBuffer[1]);
    epType = PHLN_LLCP_PDU_GET_PTYPE(pRxBuffer[0], pRxBuffer[1]);
    if(epType == PHLN_LLCP_PTYPE_AGF)
    {
        bLsap = (uint8_t)(PHLN_LLCP_PDU_GET_DSAP(pRxBuffer[0]));
        bRsap = (uint8_t)(PHLN_LLCP_PDU_GET_SSAP(pRxBuffer[1]));

        /* Received AGF other than dsap = ssap = 0, then FRMR is sent over Link Management. */
        if ((bLsap != 0U) || (bRsap != 0U))
        {
            gphlnLlcp_AGFBuffer[0] = (uint8_t)(PHLN_LLCP_PDU_MERGE_DSAP(0u, epType));
            gphlnLlcp_AGFBuffer[1] = (uint8_t)(PHLN_LLCP_PDU_MERGE_SSAP(epType, 0u));
            /* If SAP values are wrong then post a message to Queue telling there is an error
            * in AGF PDU and this will be handled by thread Processing the Message Queue. */
            gphlnLlcp_AGFBuffer[2] = PH_ERR_LLCP_PDU_INVALID;

            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_HandleMsgQueue(gphlnLlcp_AGFBuffer, (uint16_t)3U, (uint8_t)PH_TOOLS_Q_RX_DATA));
        }
        else
        {
            if (wRxLen <= (PHLN_LLCP_MIU + PHLN_LLCP_HEADER_SIZE))
            {
                (void)memcpy(gphlnLlcp_AGFBuffer, pRxBuffer, wRxLen);
                wAgfLen = wRxLen;

                while(wAgfLen > PHLN_LLCP_AGF_HEADER_SIZE)
                {
                    wCnt = PHLN_LLCP_AGF_HEADER_SIZE;
                    while(wCnt < wAgfLen)
                    {
                        /* Update as per Length field */
                        wPduLen = (((uint16_t)gphlnLlcp_AGFBuffer[wCnt] << 8U) | (gphlnLlcp_AGFBuffer[wCnt + 1U]));
                        wCnt += (wPduLen + PHLN_LLCP_AGF_LEN_FIELD_SIZE);
                    }

                    if ((wCnt != wAgfLen) || ((wPduLen + PHLN_LLCP_AGF_LEN_FIELD_SIZE) > wAgfLen))
                    {
                        /* If wCnt != wAgfLen or then AGF PDU is Invalid. Do not Parse the frame and respond. Stay Mute. */
                        return PH_ERR_SUCCESS;
                    }

                    wAgfLen -= (wPduLen + PHLN_LLCP_AGF_LEN_FIELD_SIZE);

                    epType = (PHLN_LLCP_PDU_GET_PTYPE(gphlnLlcp_AGFBuffer[wAgfLen + PHLN_LLCP_AGF_LEN_FIELD_SIZE],
                        gphlnLlcp_AGFBuffer[wAgfLen + PHLN_LLCP_AGF_LEN_FIELD_SIZE + 1U]));
                    if ((epType == PHLN_LLCP_PTYPE_SYMM) || (epType == PHLN_LLCP_PTYPE_AGF))
                    {
                        /* If the AGF PDU consists SYMM or AGF PDU then Frames following this PDU are Discarded. */
                        return PH_ERR_SUCCESS;
                    }

                    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_HandleMsgQueue(
                        &gphlnLlcp_AGFBuffer[wAgfLen + PHLN_LLCP_AGF_LEN_FIELD_SIZE],
                        wPduLen,
                        (uint8_t)PH_TOOLS_Q_RX_DATA
                        ));
                }
            }
        }
    }
    else
#endif /* PHLN_LLCP_AGF */
    {
        /* Acquire llcpMsgQMutex, QueueSendToFront RxData, Release llcpMsgQMutex.
        * Signal llcpMsgQsema for the queued RxData. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_HandleMsgQueue(
            pRxBuffer,
            wRxLen,
            (uint8_t)PH_TOOLS_Q_RX_DATA
            ));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Sw_Int_Send(phTools_Q_t *psMsgQueue, uint8_t *pbPerformRx)
{
    phStatus_t                    PH_MEMLOC_REM wStatus = PH_ERR_INTERNAL_ERROR;
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM psSocket = psMsgQueue->pSender;
  //  phlnLlcp_PType_t              PH_MEMLOC_REM epType;
	uint8_t			 				 PH_MEMLOC_REM epType;
    phStatus_t bRetstatus;
    *pbPerformRx = (uint8_t) 1U;

    if (psMsgQueue->bLlcpData == PH_ON)
    {
        psMsgQueue->bLlcpData = PH_OFF;
        return phlnLlcp_MacTransmit(PH_TRANSMIT_DEFAULT, psMsgQueue->bLlcpBuf, (uint16_t)psMsgQueue->dwLength);
    }
    else
    {
        /* Check whether the message to be sent is internal to LLCP or APP based.
        * bLsap & bRsap are zeros if it is internal. */
        if((psSocket->bLsap == 0U) && (psSocket->bRsap == 0U))
        {
            epType = (PHLN_LLCP_PDU_GET_PTYPE(psMsgQueue->pbData[0], psMsgQueue->pbData[1]));
            if (epType == PHLN_LLCP_PTYPE_DISC)
            {
                psSocket->bState = PHLN_LLCP_SOCKET_DISC;
            }
            /* Forward the buffer as it is. */
            return phlnLlcp_MacTransmit(PH_TRANSMIT_DEFAULT, psMsgQueue->pbData, (uint16_t)psMsgQueue->dwLength);
        }
        else if (psSocket->eSocketType == PHLN_LLCP_TRANSPORT_CONNECTIONLESS)
        {
            if (psSocket->bState == PHLN_LLCP_SOCKET_SNL)
            {
                wStatus = phlnLlcp_Sw_Int_Pdu_SnlEx (psSocket, psMsgQueue->pbData, (uint16_t) psMsgQueue->dwLength, psMsgQueue->wFrameOpt);
                psMsgQueue->wFrameOpt = 0;
            }
            else
            {
                /* Send UI PDU. */
                wStatus = phlnLlcp_Sw_Int_Pdu_ConnectionLess_InfoEx(psSocket, psMsgQueue->pbData, (uint16_t)psMsgQueue->dwLength,
                    psMsgQueue->wFrameOpt, pbPerformRx);

                if ((psMsgQueue->wFrameOpt == PH_TRANSMIT_DEFAULT) || (psMsgQueue->wFrameOpt == PH_TRANSMIT_BUFFER_LAST))
                {
                    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
                }
            }
        }
        else
        {
            switch(psSocket->bState)
            {
            case PHLN_LLCP_SOCKET_INFO_SEND_EX :
                wStatus = phlnLlcp_Sw_Int_Pdu_InfoEx (psSocket, psMsgQueue->pbData, (uint16_t)psMsgQueue->dwLength, psMsgQueue->wFrameOpt,
                    pbPerformRx);
                break;

            case PHLN_LLCP_SOCKET_CONN_PEND  :
                /* Send connect PDU */
                wStatus = phlnLlcp_Sw_Int_Pdu_CcOrC (psSocket, PHLN_LLCP_PTYPE_CONNECT, psMsgQueue->pbData, (uint8_t)psMsgQueue->dwLength);
                break;

            case PHLN_LLCP_SOCKET_DISC_PEND  :
                wStatus = phlnLlcp_Sw_Int_Pdu_DiscOrDm (psSocket, PHLN_LLCP_PTYPE_DISC, 0);
                break;

            case PHLN_LLCP_SOCKET_SNL:
                wStatus = phlnLlcp_Sw_Int_Pdu_SnlEx (psSocket, psMsgQueue->pbData, (uint16_t)psMsgQueue->dwLength, psMsgQueue->wFrameOpt);
                psMsgQueue->wFrameOpt = 0;
                break;

            default:
                wStatus = (wStatus | PH_COMP_LN_LLCP);
                break;
            }
        }
    }

    return wStatus;
}

phStatus_t phlnLlcp_Sw_Int_Pdu_Process(phlnLlcp_Sw_DataParams_t * pDataParams,
                                       uint8_t *pRxBuffer,
                                       uint32_t dwLength,
									   uint8_t epType,
                                       phlnLlcp_Transport_Socket_t **ppsSocket)
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint8_t    PH_MEMLOC_REM bNr;
    uint8_t    PH_MEMLOC_REM bNs;
    uint8_t    PH_MEMLOC_REM bLsap;
    uint8_t    PH_MEMLOC_REM bRsap;
    uint8_t    PH_MEMLOC_REM bUriLen;
    uint8_t    PH_MEMLOC_REM bSDREQPresent;
    uint8_t *  PH_MEMLOC_REM pUri = NULL;
    phlnLlcp_Transport_Socket_t * PH_MEMLOC_REM psSocket = NULL;
    phlnLlcp_LMDataParams_t PH_MEMLOC_REM sLMBytes = {0};

    wStatus = PH_ERR_SUCCESS;
    bLsap = (uint8_t)(PHLN_LLCP_PDU_GET_DSAP(pRxBuffer[0]));
    bRsap = (uint8_t)(PHLN_LLCP_PDU_GET_SSAP(pRxBuffer[1]));
    psSocket = NULL;
    bSDREQPresent = PH_OFF;

    /* Search for Registered Socket using URI or LSAP. */
    if( (bLsap == 1U) && ((epType == PHLN_LLCP_PTYPE_CONNECT) || (epType == PHLN_LLCP_PTYPE_SNL)))
    {
        if ((pRxBuffer[2] == PHLN_LLCP_TLV_TYPE_SDRES) && (pDataParams->bSdpClientSAP != 0U))
        {
            /* Received SDRES TLV in SNL PDU in response to DiscoverService Call from APP.
            * Search for Registered Server Socket using LSAP of Socket requested to DIscovery Services on Remote Peer. */
            psSocket  = phlnLlcp_Transport_Socket_Search(pDataParams->bSdpClientSAP, bRsap, PH_ON);
        }
        else
        {
            /* Received CONNECT or SNL PDU with SDREQ TLV on SAP 0x01.
            * Search for socket using URI, and discard TID (Transaction Identifier) in case of SDREQ
            * and also check if SDREQ TLV is present in SNL PDU. */
            pUri = phlnLlcp_Sw_Int_GetUri(&pRxBuffer[2], (uint16_t)(dwLength - 2u), &bUriLen, NULL, &bSDREQPresent);
            psSocket  = phlnLlcp_Transport_Socket_SearchUri(pUri, bUriLen);
        }
    }
    else if (epType == PHLN_LLCP_PTYPE_UI)
    {
        /* Search for registered socket using LSAP Address. */
        psSocket  = phlnLlcp_Transport_Socket_Search(bLsap, bRsap, PH_ON);
    }
    else
    {
        /* Search for registered Server socket using SAP Address. */
        psSocket  = phlnLlcp_Transport_Socket_Search(bLsap, bRsap, ((epType == PHLN_LLCP_PTYPE_CONNECT) || (epType == PHLN_LLCP_PTYPE_CC)) ? (uint8_t)1U : (uint8_t)0U);
    }
    *ppsSocket = psSocket;

    /* If the Socket is not registered in SAP then return with Error.
    * If SNL is received on Lsap = 1 and the Socket search with SAP is failure then
    * return SNL PDU with SDRES parameter with SAP address as Zero. */
    if ((*ppsSocket == NULL) && ((bLsap != 1U) || (epType != PHLN_LLCP_PTYPE_SNL)))
    {
        wStatus = PH_ERR_LLCP_SOCKET_NOT_REGISTERED; /* if Rxd Connect PDU then send Disc Mode frame */
        return wStatus;
    }

    switch(epType)
    {
    case PHLN_LLCP_PTYPE_CONNECT:
        (*ppsSocket)->bState = PHLN_LLCP_SOCKET_INFO_EX;
        (*ppsSocket)->bRsap = bRsap;
        (void)phlnLlcp_Sw_Int_ParseGenBytes(&pRxBuffer[2], (uint16_t)(dwLength - 2u), &sLMBytes);
        (*ppsSocket)->wRMiu = sLMBytes.wMiu + 128U;
        break;

    case PHLN_LLCP_PTYPE_CC:
        (*ppsSocket)->bState = PHLN_LLCP_SOCKET_INFO_EX;
        (void)phlnLlcp_Sw_Int_ParseGenBytes(&pRxBuffer[2], (uint16_t)(dwLength - 2u), &sLMBytes);
        (*ppsSocket)->bRsap = bRsap; /* Helps while connected through Uri */
        (*ppsSocket)->wRMiu = sLMBytes.wMiu + 128U;
        break;

    case PHLN_LLCP_PTYPE_DISC:
        (*ppsSocket)->bState = PHLN_LLCP_SOCKET_DISC;
        if ((bLsap == 0U) && (bRsap == 0U))
        {
            /* Check if LLCP LM Socket status to check if we have sent DISC PDU
            * over LSAP = 0 and RSAP = 0 then perform MAC Link De-Activation. */
            wStatus = (PH_ERR_PEER_DISCONNECTED | PH_COMP_LN_LLCP);
        }
        break;

    case PHLN_LLCP_PTYPE_INFO:
        /* I PDU will have the N(S)/N(R) field */
        bNr = pRxBuffer[2] & 0x0FU;
        bNs = (uint8_t)((pRxBuffer[2] & 0xF0U) >> 4U);

        if(bNs != (*ppsSocket)->sSeq.bRxState_Vr)
        {
            wStatus = PH_ERR_LLCP_PDU_TX_SEQ_ERR;
        }
        else
        {
            if((*ppsSocket)->sSeq.bSendState_Vs == bNr)
            {
                /* Update the Vsa */
                (*ppsSocket)->sSeq.bSendAck_Vsa = bNr;

                /* CHeck for the ready flag to copy the received data to the sockets Rx buffer */
                if((*ppsSocket)->fReady == (uint8_t)1U)
                {
                    (*ppsSocket)->fReady = (uint8_t)0U;
                    /* If Received Data length is more than the Socket Buffer Size then throw
                    * Information Error and send FRMR PDU to Peer. */
                    if ((dwLength - PHLN_LLCP_HEADER_SIZE) <= (*ppsSocket)->dwBufLen)
                    {
                        (*ppsSocket)->dwLength = dwLength - PHLN_LLCP_HEADER_SIZE;
                        (void)memcpy((*ppsSocket)->pbRxBuffer, (uint8_t *)(pRxBuffer + PHLN_LLCP_HEADER_SIZE), (*ppsSocket)->dwLength);
                        PHLN_LLCP_SW_MOD16_INC((*ppsSocket)->sSeq.bRxState_Vr);
                    }
                    else
                    {
                        wStatus = PH_ERR_LLCP_PDU_INFO_ERR;
                    }
                }
                else
                {
                    wStatus = PH_ERR_LLCP_BUSY;
                }
            }
            else
            {
                wStatus = PH_ERR_LLCP_PDU_RX_SEQ_ERR;
            }
        }
        break;

    case PHLN_LLCP_PTYPE_SNL:
        if ((*ppsSocket != NULL) && (pRxBuffer[2] == PHLN_LLCP_TLV_TYPE_SDRES) && (pDataParams->bSdpClientSAP == (*ppsSocket)->bLsap))
        {
            if ((dwLength - PHLN_LLCP_SNL_HEADER_SIZE) <= (*ppsSocket)->dwBufLen)
            {
                (*ppsSocket)->dwLength = dwLength - PHLN_LLCP_SNL_HEADER_SIZE;
                (void)memcpy((*ppsSocket)->pbRxBuffer, (uint8_t *)(pRxBuffer + PHLN_LLCP_SNL_HEADER_SIZE), (*ppsSocket)->dwLength);
            }
            wStatus = PH_ERR_LLCP_RECEIVED_SDRES;
        }
        else if (0U == (bSDREQPresent))
        {
            wStatus = PH_ERR_LLCP_NO_SDREQ;
        }
        else
        {
            ;/* Do nothing */
        }
        break;

    case PHLN_LLCP_PTYPE_SYMM:
        if ((*ppsSocket)->bState == PHLN_LLCP_SOCKET_DISC)
        {
            /* Check if LLCP LM Socket status to check if we have sent DISC PDU
            * over LSAP = 0 and RSAP = 0 then perform MAC Link De-Activation. */
            wStatus = (PH_ERR_LLCP_DEACTIVATED | PH_COMP_LN_LLCP);
        }
        break;

    case PHLN_LLCP_PTYPE_AGF:
        /* Nothing to do */
        break;

    case PHLN_LLCP_PTYPE_PAX:
        /* TODO need to handle */
        break;

    case PHLN_LLCP_PTYPE_DM:
        (*ppsSocket)->bState = PHLN_LLCP_SOCKET_DISC;
        if(((*ppsSocket)->bLsap == 0U) && ((*ppsSocket)->bRsap == 0U))
        {
            wStatus = (PH_ERR_LLCP_DEACTIVATED | PH_COMP_LN_LLCP);
        }
        break;

    case PHLN_LLCP_PTYPE_FRMR:
        (*ppsSocket)->bState = PHLN_LLCP_SOCKET_DISC;
        if(((*ppsSocket)->bLsap == 0U) && ((*ppsSocket)->bRsap == 0U))
        {
            wStatus = (PH_ERR_LLCP_PDU_FRMR | PH_COMP_LN_LLCP);
        }
        break;

    case PHLN_LLCP_PTYPE_RR:
        bNr = pRxBuffer[2] & 0x0FU;
        if((*ppsSocket)->sSeq.bSendState_Vs == bNr)
        {
            /* Update the Vsa */
            (*ppsSocket)->sSeq.bSendAck_Vsa = bNr;
        }
        else
        {
            wStatus = PH_ERR_LLCP_PDU_RX_SEQ_ERR;
        }
        break;

    case PHLN_LLCP_PTYPE_RNR:
        bNr = pRxBuffer[2] & 0x0FU;

        if(((*ppsSocket)->sSeq.bSendState_Vs - (uint8_t)1U) == bNr)
        {
            PHLN_LLCP_SW_MOD16_DEC(psSocket->sSeq.bSendState_Vs);
        }
        break;

    case PHLN_LLCP_PTYPE_UI:
        /* CHeck for the ready flag to copy the received data to the sockets Rx buffer */
        if((*ppsSocket)->fReady == (uint8_t)1U)
        {
            (*ppsSocket)->fReady = (uint8_t)0U;

            /* If received LLC Packet is greater than Socket Buffer size then discard the Frame */
            if ((dwLength - PHLN_LLCP_UI_HEADER_SIZE) <= (*ppsSocket)->dwBufLen)
            {
                (void)memcpy((*ppsSocket)->pbRxBuffer, (uint8_t *)(pRxBuffer + PHLN_LLCP_UI_HEADER_SIZE), dwLength);
                (*ppsSocket)->dwLength = dwLength - PHLN_LLCP_UI_HEADER_SIZE;
            }
        }
        break;

    default:
        /* Nothing to do. */
        break;
    }

    return wStatus;
}

phStatus_t phlnLlcp_Sw_Int_Pdu_Handle(phStatus_t wProcessStatus,
                                      phlnLlcp_Transport_Socket_t *psSocket,
                                      uint8_t *pRxData,
                                      uint16_t wLength)
{
    phStatus_t                  PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;
    uint8_t                     PH_MEMLOC_REM bUriLen;
    uint8_t                     PH_MEMLOC_REM bTid;
    uint8_t                     PH_MEMLOC_REM bLsap;
    uint8_t                     PH_MEMLOC_REM bRsap;
    uint8_t                     PH_MEMLOC_REM bSeq;
    uint8_t                     PH_MEMLOC_REM aSDPURI[] = "urn:nfc:sn:sdp";
    phlnLlcp_Transport_Socket_t PH_MEMLOC_REM sSocket;
    uint8_t *                   PH_MEMLOC_REM pUri = NULL;
    uint8_t         			PH_MEMLOC_REM epType;

    epType = (PHLN_LLCP_PDU_GET_PTYPE(pRxData[0], pRxData[1]));
    bLsap = (uint8_t)(PHLN_LLCP_PDU_GET_DSAP(pRxData[0]));
    bRsap = (uint8_t)(PHLN_LLCP_PDU_GET_SSAP(pRxData[1]));
    bSeq = (uint8_t) ((epType >= PHLN_LLCP_PTYPE_INFO) ? pRxData[2] : 0U);

    if (wProcessStatus == PH_ERR_LLCP_SOCKET_NOT_REGISTERED)
    {

        sSocket.bLsap = bLsap;
        sSocket.bRsap = bRsap;
        psSocket = &sSocket;

        if(epType == PHLN_LLCP_PTYPE_CONNECT)
        {
            if (((wLength - 2u) == 0U) && (bLsap == 0x01U))
            {
                /* Send DM with reason 3 as TLV are not present in CONNECT PDU. */
                return phlnLlcp_Sw_Int_Pdu_DiscOrDm(&sSocket, PHLN_LLCP_PTYPE_DM, PHLN_LLCP_PDU_DM_CONN_REJ);
            }
            /* Send DM with reason 2 */
            return phlnLlcp_Sw_Int_Pdu_DiscOrDm(&sSocket, PHLN_LLCP_PTYPE_DM, PHLN_LLCP_PDU_DM_NO_SAP);
        }

        /* Send DM with reason 1 */
        return phlnLlcp_Sw_Int_Pdu_DiscOrDm(&sSocket, PHLN_LLCP_PTYPE_DM, PHLN_LLCP_PDU_DM_NO_CONN);
    }

    switch(epType)
    {
    case PHLN_LLCP_PTYPE_CONNECT:
        if (wProcessStatus == PH_ERR_SUCCESS)
        {
            /* Send CC PDU */
            wStatus = phlnLlcp_Sw_Int_Pdu_CcOrC(psSocket, PHLN_LLCP_PTYPE_CC, NULL, 0);
        }
        break;

    case PHLN_LLCP_PTYPE_DISC:
        if (wProcessStatus == PH_ERR_SUCCESS)
        {
            if((psSocket->bLsap == 0U) && (psSocket->bRsap == 0U))
            {
                wStatus = (PH_ERR_PEER_DISCONNECTED | PH_COMP_LN_LLCP);
            }
            else
            {
                /* Send DM PDU. */
                PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_Pdu_DiscOrDm(psSocket, PHLN_LLCP_PTYPE_DM, PHLN_LLCP_PDU_DM_DISC));
            }
        }
        break;

    case PHLN_LLCP_PTYPE_INFO:
        switch(wProcessStatus)
        {
        case PH_ERR_SUCCESS:
            /* Send RR PDU. */
            wStatus = phlnLlcp_Sw_Int_Pdu_RrOrRnr(psSocket, PHLN_LLCP_PTYPE_RR);
            break;

        case PH_ERR_LLCP_BUSY:
            /* Send RNR PDU. */
            wStatus = phlnLlcp_Sw_Int_Pdu_RrOrRnr(psSocket, PHLN_LLCP_PTYPE_RNR);
            break;

        case PH_ERR_LLCP_PDU_TX_SEQ_ERR:
        case PH_ERR_LLCP_PDU_RX_SEQ_ERR:
        case PH_ERR_LLCP_PDU_INFO_ERR:
            /* Send FRMR PDU. */
            wStatus = phlnLlcp_Sw_Int_Pdu_Frmr(psSocket, epType, bLsap, bRsap, bSeq, (uint8_t)wProcessStatus, PH_OFF);
            break;
            /* Other errors are not returned here if epType is I-PDU. */
        } /* PRQA S 2002 */
        break;

    case PHLN_LLCP_PTYPE_SNL:
        /* Get TID value of SDREQ parameter, discard URI. */
        pUri = phlnLlcp_Sw_Int_GetUri(&pRxData[2], wLength - (uint16_t)2U, &bUriLen, &bTid, NULL);
        if (psSocket != NULL)
        {
            wStatus = phlnLlcp_Sw_Int_Pdu_Snl(bTid, psSocket->bLsap);
        }
        else
        {
            if (0 == (memcmp(pUri, aSDPURI, ((uint32_t)sizeof(aSDPURI) - (uint32_t)1U))))
            {
                /* HardCoded to send SAP = 1 if received URI is SDP (urn:nfc:sn:sdp).
                * As we are not using additional Socket for SDP. */
                wStatus = phlnLlcp_Sw_Int_Pdu_Snl(bTid, 1);
            }
            else
            {
                /* Send SNL PDU with SAP address Zero as no service is registered with SAP. */
                wStatus = phlnLlcp_Sw_Int_Pdu_Snl(bTid, 0);
            }
        }
        break;

    case PHLN_LLCP_PTYPE_PAX:
        /* TODO need to handle. */
        break;

    case PHLN_LLCP_PTYPE_AGF:
        /* If AGF PDU is received over SAP other than zero then FRMR will be sent to Peer Link Management. */
        if (pRxData[2] == PH_ERR_LLCP_PDU_INVALID)
        {
            PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_Pdu_Frmr(psSocket, epType, 0, 0, 0, PH_ERR_LLCP_PDU_INVALID, PH_ON));
            wStatus = (PH_ERR_LLCP_PDU_INVALID | PH_COMP_LN_LLCP);
        }
        break;

    case PHLN_LLCP_PTYPE_CC:
    case PHLN_LLCP_PTYPE_DM:
    case PHLN_LLCP_PTYPE_FRMR:
    case PHLN_LLCP_PTYPE_SYMM:
        /* Nothing to do. */
        break;

    case PHLN_LLCP_PTYPE_RR:
    case PHLN_LLCP_PTYPE_RNR:
        if (wProcessStatus == PH_ERR_LLCP_PDU_RX_SEQ_ERR)
        {
            /* Send FRMR PDU. */
            wStatus = phlnLlcp_Sw_Int_Pdu_Frmr(psSocket, epType, bLsap, bRsap, bSeq, PH_ERR_LLCP_PDU_RX_SEQ_ERR, PH_OFF);
        }
        break;

    case PHLN_LLCP_PTYPE_UI:
        /* Nothing to do. */
        break;

    default:
        /* Send FRMR frame if reserved PDUs are sent on Data Link Connection. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Sw_Int_Pdu_Frmr(psSocket, epType, bLsap, bRsap, bSeq, PH_ERR_LLCP_PDU_INVALID, PH_OFF));
        break;
    }
    return wStatus;
}

uint16_t phlnLlcp_Sw_Int_PostEvents(phStatus_t wProcessStatus,
									uint8_t epType,
                                    phlnLlcp_Transport_Socket_t *psSocket,
                                    uint8_t *pRxData,
                                    uint8_t bVersion)
{
    uint16_t   PH_MEMLOC_REM wTxFlag = PH_ON;
    phStatus_t bRetstatus;

    if((wProcessStatus == PH_ERR_LLCP_SOCKET_NOT_REGISTERED) || (wProcessStatus == PH_ERR_LLCP_BUSY))
    {
        return wTxFlag;
    }
    else if ((wProcessStatus == PH_ERR_LLCP_PDU_RX_SEQ_ERR) || (wProcessStatus == PH_ERR_LLCP_PDU_TX_SEQ_ERR) ||
        (wProcessStatus == PH_ERR_LLCP_PDU_INFO_ERR))
    {
        psSocket->wStatus = wProcessStatus;
        psSocket->bState = PHLN_LLCP_SOCKET_DISC;
        PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
        PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
        return wTxFlag;
    }
    else
    {
        switch(epType)
        {
        case PHLN_LLCP_PTYPE_CONNECT:
            /* Post the event */
            psSocket->bState = PHLN_LLCP_SOCKET_INFO_EX;
            psSocket->wStatus = PH_ERR_SUCCESS;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            break;

        case PHLN_LLCP_PTYPE_DISC:
            /* Post the event */
            psSocket->wStatus = PH_ERR_PEER_DISCONNECTED;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            break;

        case PHLN_LLCP_PTYPE_INFO:
            if (psSocket->bState == PHLN_LLCP_SOCKET_INFO_SEND_EX)
            {
                psSocket->wStatus = PH_ERR_SUCCESS_INFO_RECEIVED;
                psSocket->bState = PHLN_LLCP_SOCKET_INFO_EX;
                PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            }
            else
            {
                psSocket->wStatus = PH_ERR_SUCCESS;
                PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
            }
            break;


        case PHLN_LLCP_PTYPE_SNL:
            if (wProcessStatus == PH_ERR_LLCP_RECEIVED_SDRES)
            {
                /* Post the event */
                wTxFlag = PH_OFF;
                psSocket->bState = PHLN_LLCP_SOCKET_SNL;
                psSocket->wStatus = PH_ERR_SUCCESS;
                PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            }

            /* If Version number is less than ver1.1 then SDP is not supported and SNL PDUs are neglected.
            *  Also if SNL PDU with no SDREQ TLV is received then not need to send SNL PDU back. */
            if ((bVersion < PHLN_LLCP_SNL_SUPPORTED_VERSION_VALUE) || (wProcessStatus == PH_ERR_LLCP_NO_SDREQ))
            {
                wTxFlag = PH_OFF;
            }
            break;

        case PHLN_LLCP_PTYPE_PAX:
        case PHLN_LLCP_PTYPE_AGF:
            /* TODO need to handle */
            break;

        case PHLN_LLCP_PTYPE_FRMR:
            /* Post the event */
            psSocket->wStatus = (((uint16_t)pRxData[2] >> 4U ) | PH_ERR_CUSTOM_BEGIN);
            psSocket->bState = PHLN_LLCP_SOCKET_DISC;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            wTxFlag = PH_OFF;
            break;

        case PHLN_LLCP_PTYPE_RR:
            wTxFlag = PH_OFF;
            psSocket->bState = PHLN_LLCP_SOCKET_INFO_EX;
            psSocket->wStatus = PH_ERR_SUCCESS;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            break;

        case PHLN_LLCP_PTYPE_RNR:
            wTxFlag = PH_OFF;
            psSocket->bState = PHLN_LLCP_SOCKET_INFO_EX;
            psSocket->wStatus = PH_ERR_TX_NAK_ERROR;
            break;

        case PHLN_LLCP_PTYPE_UI:
            wTxFlag = PH_OFF;
            psSocket->wStatus = PH_ERR_SUCCESS;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
            break;

        case PHLN_LLCP_PTYPE_CC:
            /* Post the event */
            wTxFlag = PH_OFF;
            psSocket->bState = PHLN_LLCP_SOCKET_INFO_EX;
            psSocket->wStatus = PH_ERR_SUCCESS;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            break;

        case PHLN_LLCP_PTYPE_DM:
            psSocket->wStatus = PH_ERR_LLCP_DEACTIVATED;
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            wTxFlag = PH_OFF;
            break;

        case PHLN_LLCP_PTYPE_SYMM:
            wTxFlag = PH_OFF;
            break;

        default:
            if ((psSocket->bLsap | psSocket->bRsap) == 0U)
            {
                /* Ignore reserve PDUs on default LLCP link. */
                wTxFlag = PH_OFF;
            }
            else
            {
                /* Release all semaphores of Data Link connections when Invalid PDU is received on
                 * Data link connection sockets and intimate remote Data link connection by sending FRMR. */
                psSocket->wStatus = PH_ERR_LLCP_PDU_INVALID;
                psSocket->bState = PHLN_LLCP_SOCKET_DISC;
                PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xRxSema.SemHandle, E_OS_SEM_OPT_NONE));
                PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&psSocket->xSema.SemHandle, E_OS_SEM_OPT_NONE));
            }
            break;
        }
    }
    return wTxFlag;
}

#endif /* NXPBUILD__PHLN_LLCP_SW */
