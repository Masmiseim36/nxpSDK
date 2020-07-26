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
* Software MAC Layer of LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 18. Feb 2015
*
*/


/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <ph_NxpBuild.h>
#include <ph_Status.h>
#include <phlnLlcp.h>

#ifdef NXPBUILD__PHLN_LLCP_SW
#include "phlnLlcp_Sw_Mac.h"
#include "phlnLlcp_Timers.h"

#ifdef NXPBUILD__PHPAL_I18092MPI
#include "phpalI18092mPI.h"
#endif /* NXPBUILD__PHPAL_I18092MPI */

#ifdef NXPBUILD__PHPAL_I18092MT
#include "phpalI18092mT.h"
#endif /* NXPBUILD__PHPAL_I18092MT */

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
static void *gphlnLlcp_MacDataParams;
static uint32_t gphlnLlcp_MacType;

/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */
phStatus_t phlnLlcp_Sw_MacInit(phlnLlcp_Sw_DataParams_t * pDataParams, uint8_t bConfig, uint16_t *wSymmTime, uint16_t *wLtoTime)
{
    phStatus_t PH_MEMLOC_REM wStatus;
    uint16_t   PH_MEMLOC_REM wValue;
    uint32_t   PH_MEMLOC_REM dwValue = 0;

    gphlnLlcp_MacDataParams = pDataParams->pPalI18092DataParams;
    gphlnLlcp_MacType = bConfig;
    pDataParams->bMacType = bConfig;

    /* Perform check and configurations as per LLCP Specification 1.1 section 6.2.1. */
    if (bConfig == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        /* Check if DID is Disabled as per LLCP 1.2 section 6.2.1. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MPI_CONFIG_DID, &wValue));
        if (wValue != PH_OFF)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Check if NAD is Disabled as per LLCP 1.2 section 6.2.1 */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MPI_CONFIG_NAD, &wValue));
        if ((uint16_t)(wValue & 0xFF00U) != PH_OFF)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Check if FSL value is 11b as per LLCP 1.2 section 6.2.1. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MPI_CONFIG_FSL, &wValue));
        if (wValue != PHLN_LLCP_ALLOWED_FRAMESIZE)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Enable ATN Feature of PAL 18092mPI. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacSetConfig(PHPAL_I18092MPI_CONFIG_ATN, PH_ON));
#endif /* NXPBUILD__PHPAL_I18092MPI */
#if !defined(NXPBUILD__PHPAL_I18092MPI)
        return (PH_ERR_USE_CONDITION | PH_COMP_LN_LLCP);
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        /* Check if DID is Disabled as per LLCP 1.2 section 6.2.1. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MT_CONFIG_DID, &wValue));
        if (wValue != PH_OFF)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Check if DID is Disabled as per LLCP 1.2 section 6.2.1. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MT_CONFIG_NAD, &wValue));
        if ((uint16_t)(wValue & 0xFF00U) != PH_OFF)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Check if FSL value is 11b as per LLCP 1.2 section 6.2.1. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MT_CONFIG_FSL, &wValue));
        if (wValue != PHLN_LLCP_ALLOWED_FRAMESIZE)
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Check if LTO value is less 2 times of FWT (TO in ATR_RES) to support MAC error recovery for 2 times. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacGetConfig(PHPAL_I18092MT_CONFIG_TO, &wValue));
        wValue = wValue & 0x000FU;
        dwValue = (uint32_t)(PHLN_LLCP_RWT_MIN_US * ((uint32_t)(1U << wValue)));
        dwValue /= 1000;
        if ((pDataParams->sLocalLMParams.bLto * 10U) < (dwValue * 2U))
        {
            return (PH_ERR_PROTOCOL_ERROR | PH_COMP_LN_LLCP);
        }

        /* Disabled RTOX Feature of PAL 18092mT. */
        PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_MacSetConfig(PHPAL_I18092MT_CONFIG_RTOX, PH_OFF));
#endif /* NXPBUILD__PHPAL_I18092MT */
#if !defined(NXPBUILD__PHPAL_I18092MPI)
        return (PH_ERR_USE_CONDITION | PH_COMP_LN_LLCP);
#endif /* NXPBUILD__PHPAL_I18092MT */
    }

    /* Create LLC SYMM and LTO Timer during Init and never delete these timers. */
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_InitSym(pDataParams, &phlnLlcp_SymTimerCallback));
    PH_CHECK_SUCCESS_FCT(wStatus, phlnLlcp_Timers_InitLto(pDataParams, &phlnLlcp_LtoTimerCallback));

    /* Enable the timers in LTO. */
    if (pDataParams->sRemoteLMParams.bLto == 0U)
    {
        /* If Remote LTO is Zero, then default SYMM Timeout will be 100ms + delta. */
        *wLtoTime = 100 + PHLN_LLCP_LTO_DELTA_PERCENTAGE;
    }
    else
    {
        /* Wait for peer LTO + delta. */
        *wLtoTime = ((uint16_t)pDataParams->sRemoteLMParams.bLto * 10U) + (((uint16_t)pDataParams->sRemoteLMParams.bLto * PHLN_LLCP_LTO_DELTA_PERCENTAGE)/10U) ;
    }

    if (bConfig == PHLN_LLCP_TARGET)
    {
        /* Set SYMM timer value twice or thrice less then FWT. */
        *wSymmTime = (uint16_t) ((dwValue - ((dwValue * PHLN_LLCP_SYMM_DELTA_PERCENTAGE)/100U)) / 2U);
    }
    else
    {
        /* Enable the timers in Single shot mode for SYMM. */
        if (pDataParams->sLocalLMParams.bLto == 0U)
        {
            /* If LTO is Zero, then default SYMM will be 100ms - 10ms for sw overhead. */
            *wSymmTime = 90;
        }
        else
        {
            /* Set SYMM timeout using Local LTO value - delta for sw overhead. */
            *wSymmTime = ((uint16_t)(pDataParams->sLocalLMParams.bLto) * 10U) -
                (((uint16_t)(pDataParams->sLocalLMParams.bLto) * PHLN_LLCP_SYMM_DELTA_PERCENTAGE)/10U);
        }
    }

    return wStatus;
}

phStatus_t phlnLlcp_MacTransmit(uint16_t wFrameOpt, uint8_t* pTxBuffer, uint16_t wTxLength)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_USE_CONDITION;
    if (gphlnLlcp_MacType == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        wStatus = phpalI18092mPI_Transmit(gphlnLlcp_MacDataParams, wFrameOpt, pTxBuffer, wTxLength);
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        wStatus = phpalI18092mT_Transmit(gphlnLlcp_MacDataParams, wFrameOpt, pTxBuffer, wTxLength);
#endif /* NXPBUILD__PHPAL_I18092MT */
    }
    return wStatus;
}

phStatus_t phlnLlcp_MacReceive(uint16_t wFrameOpt, uint8_t **ppRxBuffer, uint16_t *pRxLength)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_USE_CONDITION;
    if (gphlnLlcp_MacType == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        wStatus = phpalI18092mPI_Receive(gphlnLlcp_MacDataParams, wFrameOpt, ppRxBuffer, pRxLength);
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        wStatus = phpalI18092mT_Receive(gphlnLlcp_MacDataParams, wFrameOpt, ppRxBuffer, pRxLength);
#endif /* NXPBUILD__PHPAL_I18092MT */
    }
    return wStatus;
}

phStatus_t phlnLlcp_MacSetConfig(uint16_t wConfig, uint16_t wValue)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_USE_CONDITION;
    if (gphlnLlcp_MacType == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        wStatus = phpalI18092mPI_SetConfig(gphlnLlcp_MacDataParams, wConfig, wValue);
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        wStatus = phpalI18092mT_SetConfig(gphlnLlcp_MacDataParams, wConfig, wValue);
#endif /* NXPBUILD__PHPAL_I18092MT */
    }
    return wStatus;
}

phStatus_t phlnLlcp_MacGetConfig(uint16_t wConfig, uint16_t* pwValue)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_USE_CONDITION;
    if (gphlnLlcp_MacType == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        wStatus = phpalI18092mPI_GetConfig(gphlnLlcp_MacDataParams, wConfig, pwValue);
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        wStatus = phpalI18092mT_GetConfig(gphlnLlcp_MacDataParams, wConfig, pwValue);
#endif /* NXPBUILD__PHPAL_I18092MT */
    }
    return wStatus;
}

phStatus_t phlnLlcp_MacDeactivation(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;
#ifdef NXPBUILD__PHPAL_I18092MPI
    uint16_t   PH_MEMLOC_REM wValue = 0;
#endif /* NXPBUILD__PHPAL_I18092MPI */
#ifdef NXPBUILD__PHPAL_I18092MT
    uint16_t   PH_MEMLOC_REM wRxBuffLen = 0;
    uint8_t    PH_MEMLOC_REM aTxBuff[2] = {0x00, 0x00};
    uint8_t    PH_MEMLOC_REM *pRxBuff = NULL;

    wRxBuffLen = 0;
#endif /* NXPBUILD__PHPAL_I18092MT */

    if (gphlnLlcp_MacType == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        /* Get initiator mode activation status. */
        PH_CHECK_SUCCESS_FCT(wStatus, phpalI18092mPI_GetConfig(gphlnLlcp_MacDataParams,
            PHPAL_I18092MPI_CONFIG_ACTIVEMODE, &wValue));
        if (wValue != PH_ON)
        {
            /* Perform DSL_REQ if LLCP is activated in passive initiator. */
            (void)phpalI18092mPI_Deselect(gphlnLlcp_MacDataParams, PHPAL_I18092MPI_DESELECT_DSL);
        }
        else
        {
            /* Perform RLS_REQ if LLCP is activated in active initiator. */
            (void)phpalI18092mPI_Deselect(gphlnLlcp_MacDataParams, PHPAL_I18092MPI_DESELECT_RLS);
        }
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        /* Update Tx buffer if agreed NFC Forum LLCP version is 1.2 */
        if (pDataParams->bAgreedVersion == 0x12U)
        {
            aTxBuff[0] = 0x01;
            aTxBuff[1] = 0x40;
        }

        do
        {
            wStatus = phpalI18092mT_Transmit(gphlnLlcp_MacDataParams, PH_TRANSMIT_DEFAULT, aTxBuff, 2);
            if (wStatus == PH_ERR_SUCCESS)
            {
                wStatus = phpalI18092mT_Receive(gphlnLlcp_MacDataParams, PH_RECEIVE_DEFAULT, &pRxBuff, &wRxBuffLen);
            }
        }while (wStatus == PH_ERR_SUCCESS);

        if (((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_DESELECTED) || ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_RELEASED))
        {
            wStatus = PH_ERR_SUCCESS;
        }
#endif /* NXPBUILD__PHPAL_I18092MT */
    }

    return wStatus;
}

void phlnLlcp_Sw_MacHAL_ShutDown(void)
{
#ifdef NXPBUILD__PHPAL_I18092MPI
    phpalI18092mPI_Sw_DataParams_t PH_MEMLOC_REM * pPalI18092mPIDataParams = NULL;
#endif /* NXPBUILD__PHPAL_I18092MPI */

#ifdef NXPBUILD__PHPAL_I18092MT
    phpalI18092mT_Sw_DataParams_t  PH_MEMLOC_REM * pPalI18092mTDataParams = NULL;
#endif /* NXPBUILD__PHPAL_I18092MT */

    /* Assign global Dataparam to corresponding PAL Param to access HAL Dataparam. */
    if (gphlnLlcp_MacType == PHLN_LLCP_INITIATOR)
    {
#ifdef NXPBUILD__PHPAL_I18092MPI
        pPalI18092mPIDataParams = (phpalI18092mPI_Sw_DataParams_t *)gphlnLlcp_MacDataParams;
        (void)phhalHw_AsyncAbort(pPalI18092mPIDataParams->pHalDataParams);
#endif /* NXPBUILD__PHPAL_I18092MPI */
    }
    else
    {
#ifdef NXPBUILD__PHPAL_I18092MT
        pPalI18092mTDataParams = (phpalI18092mT_Sw_DataParams_t *)gphlnLlcp_MacDataParams;
        (void)phhalHw_AsyncAbort(pPalI18092mTDataParams->pHalDataParams);
#endif /* NXPBUILD__PHPAL_I18092MT */
    }
}

#endif /* NXPBUILD__PHLN_LLCP_SW */
