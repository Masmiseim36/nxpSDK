/*
*                     Copyright 2019, NXP
*
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
* Wait Routines for Pn5190 specific HAL-Component of Reader Library Framework.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_PN5190

#ifndef _WIN32
#include <phOsal.h>
#include <phDriver.h>
#include <phNxpNfcRdLib_Config.h>
#endif
#include <phhalHw.h>
#include "phhalHw_Pn5190_Int.h"
#include "phhalHw_Pn5190_InstrMngr.h"
#include "phhalHw_Pn5190_Wait.h"


#ifndef _WIN32
static phStatus_t phhalHw_Pn5190_EventWait_AbortHandling(phhalHw_Pn5190_DataParams_t *pDataParams,uint32_t dwEvtsWaitFor);
#endif

phStatus_t phhalHw_Pn5190_WaitIrq(
    phhalHw_Pn5190_DataParams_t * pDataParams,
    uint32_t * pReceivedEvents
    )
{
    phStatus_t         PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;

    /* Wait for event */
    PH_CHECK_SUCCESS_FCT(status, phOsal_EventPend(
        (volatile phOsal_Event_t *)(&pDataParams->HwEventObj.EventHandle),
        E_OS_EVENT_OPT_PEND_SET_ANY | E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT,
        PHOSAL_MAX_DELAY,
        (E_PH_OSAL_EVT_RF | E_PH_OSAL_EVT_ABORT),
        pReceivedEvents));
#ifndef _WIN32
    if (E_PH_OSAL_EVT_ABORT & *pReceivedEvents)
    {
        return PH_ERR_ABORTED;
    }
#endif
    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Pn5190_WaitForEvent(
        phhalHw_Pn5190_DataParams_t * pDataParams,
        uint32_t dwEvtsWaitFor,
        uint32_t dwEvtsTimeOut,
        uint8_t bPayloadPresnt,
        uint32_t *dwEventsReceived,
        uint8_t ** ppEvtPayLoad
        )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    phOsal_EventBits_t  PH_MEMLOC_REM tReceivedEvents = 0x0U;
    uint16_t  PH_MEMLOC_REM wRemLen = 0x0U;
    uint32_t  PH_MEMLOC_REM dwEventStatusReg = 0x0U;
    uint8_t   PH_MEMLOC_REM bBackup = 0x0U;

    do
    {
        /* Wait for event */
        PH_CHECK_SUCCESS_FCT(status, phOsal_EventPend(
            (volatile phOsal_Event_t *)(&pDataParams->HwEventObj.EventHandle),
            E_OS_EVENT_OPT_PEND_SET_ANY | E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT,
            dwEvtsTimeOut,
            (E_PH_OSAL_EVT_SIG | E_PH_OSAL_EVT_ABORT),
            &tReceivedEvents));

        if (E_PH_OSAL_EVT_SIG & tReceivedEvents)
        {
            /* Reading length form the ISR buffer, in which we have read first part of response */
            wRemLen = pDataParams->sIrqResp.pIsrEvtBuffPtr[2];
            wRemLen <<= 8;
            wRemLen |= pDataParams->sIrqResp.pIsrEvtBuffPtr[3];

            bBackup = pDataParams->sIrqResp.pIsrEvtBuffPtr[4];
            /* By using wRemLen, We are reading One byte extra, for spi header. Actually
             * we should have deducted one byte from it.
             */
            phhalHw_Pn5190_Read(
                pDataParams,
                &pDataParams->sIrqResp.pIsrEvtBuffPtr[4],
                wRemLen);

            pDataParams->sIrqResp.pIsrEvtBuffPtr[4] = bBackup;

            dwEventStatusReg = (uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[4];
            dwEventStatusReg |= ((uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[5]) << 8U ;
            dwEventStatusReg |= ((uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[6]) << 16U;
            dwEventStatusReg |= ((uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[7]) << 24U;
        }

        if (E_PH_OSAL_EVT_ABORT & tReceivedEvents)
        {
#ifndef _WIN32
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Pn5190_Int_UserAbort(pDataParams));

            PH_CHECK_SUCCESS_FCT(status, phhalHw_Pn5190_EventWait_AbortHandling(pDataParams,dwEvtsWaitFor));
#endif
            return PH_ERR_ABORTED;
        }
    }
    while(0 == (dwEventStatusReg & dwEvtsWaitFor));

    *dwEventsReceived = dwEventStatusReg;

    if (dwEventStatusReg != PH_PN5190_EVT_CTS)
    {
        if(bPayloadPresnt)
        {
            *ppEvtPayLoad = &pDataParams->sIrqResp.pIsrEvtBuffPtr[ 4 + sizeof(dwEventStatusReg)];
        }
        else
        {
            if (*ppEvtPayLoad != NULL)
            {
                *ppEvtPayLoad = NULL;
            }
        }
    }

    return PH_ERR_SUCCESS;
}

#ifndef _WIN32
phStatus_t  phhalHw_Pn5190_Wait_IrqAbortHandling(phhalHw_Pn5190_DataParams_t *pDataParams,phhalHw_InstMngr_CmdParams_t *pCmdParams)
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    uint8_t *  PH_MEMLOC_REM pEvtPayload = NULL;
    uint32_t   PH_MEMLOC_REM dwEventReceived = 0x0U;
    phOsal_EventBits_t  PH_MEMLOC_REM tReceivedEvents = 0x0U;


    /* Wait for event, not clearing EVT here */
    PH_CHECK_SUCCESS_FCT(status, phOsal_EventPend(
        (volatile phOsal_Event_t *)(&pDataParams->HwEventObj.EventHandle),
        E_OS_EVENT_OPT_PEND_SET_ANY,
        PHOSAL_MAX_DELAY,
        (E_PH_OSAL_EVT_SIG | E_PH_OSAL_EVT_RF),
        &tReceivedEvents));

    if (tReceivedEvents & E_PH_OSAL_EVT_RF)
    {
        phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RF, NULL);

        /*Clear the FW res by reading*/
        phhalHw_Pn5190_InstMngr_ReadRsp(pDataParams, pCmdParams);
    }

    /*Wait for event*/
    status = phhalHw_Pn5190_WaitForEvent(
        pDataParams,
        PH_PN5190_EVT_IDLE | PH_PN5190_EVT_BOOT | PH_PN5190_EVT_GENERAL_ERROR,
        PHOSAL_MAX_DELAY,
        true,
        &dwEventReceived,
        &pEvtPayload);

    if(status == PH_ERR_SUCCESS)
    {
        pDataParams->dwBootStatus = (uint32_t) pEvtPayload[0];
        pDataParams->dwBootStatus |= ((uint32_t) pEvtPayload[1]) << 8U ;
        pDataParams->dwBootStatus |= ((uint32_t) pEvtPayload[2]) << 16U;
        pDataParams->dwBootStatus |= ((uint32_t) pEvtPayload[3]) << 24U;

        if(dwEventReceived & PH_PN5190_EVT_GENERAL_ERROR)
        {
            pDataParams->dwGenError = (uint32_t) pEvtPayload[4];
            pDataParams->dwGenError |= ((uint32_t) pEvtPayload[5]) << 8U ;
            pDataParams->dwGenError |= ((uint32_t) pEvtPayload[6]) << 16U;
            pDataParams->dwGenError |= ((uint32_t) pEvtPayload[7]) << 24U;

            status = PH_ERR_INTERNAL_ERROR;
        }
        else if ((dwEventReceived == PH_PN5190_EVT_BOOT) ||
            (dwEventReceived == PH_PN5190_EVT_IDLE))
        {
            status = PH_ERR_SUCCESS;
        }
        else
        {
            /* Do nothing */
        }
    }
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}


phStatus_t phhalHw_Pn5190_EventWait_AbortHandling(phhalHw_Pn5190_DataParams_t *pDataParams,uint32_t dwEvtsWaitFor)
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    phOsal_EventBits_t  PH_MEMLOC_REM tReceivedEvents = 0x0U;
    uint16_t  PH_MEMLOC_REM wRemLen = 0x0U;
    uint32_t  PH_MEMLOC_REM dwEventStatusReg = 0x0U;
    uint8_t   PH_MEMLOC_REM bBackup = 0x0U;
    uint8_t   PH_MEMLOC_REM bLoopCount = 0x0U;

    do
    {
        PH_CHECK_SUCCESS_FCT(status, phOsal_EventPend(
            (volatile phOsal_Event_t *)(&pDataParams->HwEventObj.EventHandle),
            E_OS_EVENT_OPT_PEND_SET_ANY | E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT,
            PHOSAL_MAX_DELAY,
            (E_PH_OSAL_EVT_SIG | E_PH_OSAL_EVT_RF),
            &tReceivedEvents));

        if (tReceivedEvents & E_PH_OSAL_EVT_RF)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
        }

        if (E_PH_OSAL_EVT_SIG & tReceivedEvents)
        {
            /* Reading length form the ISR buffer, in which we have read first part of response */
            wRemLen = pDataParams->sIrqResp.pIsrEvtBuffPtr[2];
            wRemLen <<= 8;
            wRemLen |= pDataParams->sIrqResp.pIsrEvtBuffPtr[3];

            bBackup = pDataParams->sIrqResp.pIsrEvtBuffPtr[4];
            /* By using wRemLen, We are reading One byte extra, for spi header. Actually
             * we should have deducted one byte from it.
             */
            phhalHw_Pn5190_Read(
                pDataParams,
                &pDataParams->sIrqResp.pIsrEvtBuffPtr[4],
                wRemLen);

            pDataParams->sIrqResp.pIsrEvtBuffPtr[4] = bBackup;

            dwEventStatusReg = (uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[4];
            dwEventStatusReg |= ((uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[5]) << 8U ;
            dwEventStatusReg |= ((uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[6]) << 16U;
            dwEventStatusReg |= ((uint32_t) pDataParams->sIrqResp.pIsrEvtBuffPtr[7]) << 24U;
        }

        bLoopCount++;
        if(bLoopCount > 2)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
        }
    }
    while((dwEventStatusReg & (PH_PN5190_EVT_IDLE | PH_PN5190_EVT_BOOT | PH_PN5190_EVT_GENERAL_ERROR)) == 0);

    /*Abort Event received successfully*/
    status = PH_ERR_SUCCESS;

    if(dwEventStatusReg & PH_PN5190_EVT_GENERAL_ERROR)
    {
        status = PH_ERR_INTERNAL_ERROR;
    }

    return PH_ADD_COMPCODE_FIXED(status, PH_COMP_HAL);
}
#endif  /*#ifndef _WIN32*/

#endif  /* NXPBUILD__PHHAL_HW_PN5190 */
