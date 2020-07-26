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

phStatus_t phlnLlcp_Sw_Transport_Socket_SendTo(
                                               phlnLlcp_Sw_DataParams_t * pDataParams,
                                               phlnLlcp_Transport_Socket_t * pClientSocket,
                                               uint8_t   bDsap,
                                               uint8_t * pTxBuffer,
                                               uint32_t dwTxBufferSize,
                                               uint16_t wFrameOpt
                                               )
{
    phStatus_t     PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;

    if ((pClientSocket == NULL) || (pTxBuffer == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }
    else if (pClientSocket->eSocketType != PHLN_LLCP_TRANSPORT_CONNECTIONLESS)
    {
        return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_LN_LLCP);
    }
    else if ((bDsap < 2U) || (bDsap > 63U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_LN_LLCP);
    }
    else
    {
        /* Copy bDsap into the Socket parameter. */
        pClientSocket->bRsap = bDsap;

        wStatus = phlnLlcp_Sw_Int_Socket_SendInt(pClientSocket, pTxBuffer, dwTxBufferSize, wFrameOpt, PHLN_LLCP_SOCKET_INFO_SEND_EX);
    }
    PH_UNUSED_VARIABLE(pDataParams);
    return wStatus;
}

phStatus_t phlnLlcp_Sw_Int_Pdu_ConnectionLess_InfoEx(phlnLlcp_Transport_Socket_t *psSocket,
                                                     uint8_t *pbTxData,
                                                     uint16_t wLength,
                                                     uint16_t wFrameOpt,
                                                     uint8_t *pbPerformRx)
{
    uint8_t    PH_MEMLOC_REM baLlcpHeader[2];
    phStatus_t PH_MEMLOC_REM wStatus;

    switch(wFrameOpt)
    {
    case PH_TRANSMIT_BUFFER_FIRST:
        /* Frame the UI PDU */
        PHLN_LLCP_PDU_FRAME_MANDATORY_HEADER(psSocket->bRsap, PHLN_LLCP_PTYPE_UI, psSocket->bLsap, baLlcpHeader);
        /* Send UI PDU frame Header */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_FIRST, baLlcpHeader, 2));
        /* Append above layers header, inform above layers header is NOT the last fragment */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_CONT, pbTxData, wLength);
        *pbPerformRx = 0U;
        break;

    case PH_TRANSMIT_BUFFER_LAST:
        /* Just pass the buffer to the lower layer. */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_LAST, pbTxData, wLength);
        break;

    case PH_TRANSMIT_DEFAULT:
        /* Frame the UI PDU */
        PHLN_LLCP_PDU_FRAME_MANDATORY_HEADER(psSocket->bRsap, PHLN_LLCP_PTYPE_UI, psSocket->bLsap, baLlcpHeader);
        /* Send UI PDU frame Header */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_FIRST, baLlcpHeader, 2));
        /* Append above layers data, inform above layers data is the last fragment */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_LAST, pbTxData, wLength);
        break;

    case PH_TRANSMIT_BUFFER_CONT :
        /* Just pass the buffer to the lower layer. */
        wStatus = phlnLlcp_MacTransmit(PH_TRANSMIT_BUFFER_CONT, pbTxData, wLength);
        *pbPerformRx = (uint8_t)0U;
        break;

    default:
        wStatus = (PH_ERR_INTERNAL_ERROR | PH_COMP_LN_LLCP);
    }

    return wStatus;
}

#endif /* NXPBUILD__PHLN_LLCP_SW */
