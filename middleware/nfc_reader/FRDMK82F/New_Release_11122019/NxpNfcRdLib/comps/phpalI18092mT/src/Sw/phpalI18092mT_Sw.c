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
* Software ISO18092 target mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 27. October 2014
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI18092mT.h>
#include <ph_RefDefs.h>


#include <phDriver.h>


#ifdef NXPBUILD__PHPAL_I18092MT_SW

#include "phpalI18092mT_Sw.h"
#include "phpalI18092mT_Sw_Int.h"

static const uint8_t bmtEventName[] = "mtEvent";
phStatus_t phpalI18092mT_Sw_Init(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t wSizeOfDataParams,
    void * pHalDataParams,
    pRtoxTimerCallback pRtoxCallback
    )
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_SUCCESS;

    if (sizeof(phpalI18092mT_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18092MT);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* Init private data */
    pDataParams->wId            = PH_COMP_PAL_I18092MT | PHPAL_I18092MT_SW_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->pRtoxCallback  = pRtoxCallback;
    pDataParams->bCmdtype       = PHPAL_I18092MT_SW_CMD_RLS_REQ;

    /* Reset protocol to defaults */
    (void)phpalI18092mT_Sw_ResetProtocol(pDataParams);

    /* Check whether event is already created. */
    pDataParams->Pal18092mTEventObj.pEvtName = (uint8_t *)bmtEventName;
    pDataParams->Pal18092mTEventObj.intialValue = 0;
    wStatus = phOsal_EventCreate(&pDataParams->Pal18092mTEventObj.EventHandle, &pDataParams->Pal18092mTEventObj);

    return wStatus;
}

phStatus_t phpalI18092mT_Sw_ResetProtocol(
    phpalI18092mT_Sw_DataParams_t * pDataParams
    )
{
    /* Apply default parameters */
    pDataParams->bStateNow          = PHPAL_I18092MT_SW_STATE_INFORMATION_PDU_TX;
    pDataParams->bDid               = 0;    /* set DID to 0, i.e. disable usage of DID */
    pDataParams->bNadEnabled        = PH_OFF;
    pDataParams->bNadVal            = 0;
    pDataParams->bFsl               = PHPAL_I18092MT_FRAMESIZE_64;
    pDataParams->bPni               = 0;    /* NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.1 */
    pDataParams->bDst               = PHPAL_I18092MT_DATARATE_106;
    pDataParams->bDrt               = PHPAL_I18092MT_DATARATE_106;
    pDataParams->bBsi               = 0;
    pDataParams->bBri               = 0;
    pDataParams->bLri               = PHPAL_I18092MT_FRAMESIZE_64;
    pDataParams->bBst               = 0;
    pDataParams->bBrt               = 0;
    pDataParams->bLrt               = PHPAL_I18092MT_FRAMESIZE_64;
    pDataParams->bTo                = 0;
    pDataParams->bRtoxDisabled      = PH_OFF;
    pDataParams->ovrTask            = NULL;
    pDataParams->bCmdtype           = PHPAL_I18092MT_SW_CMD_RLS_REQ;
    pDataParams->bRtoxExtnVal       = PHPAL_I18092MT_SW_MIN_RTOX_VALUE;
    pDataParams->wLastTxLen         = 0;
    pDataParams->bOpeMode           = RD_LIB_MODE_NFC;
    pDataParams->bPropPSLMode       = PH_OFF;

    (void)memset(pDataParams->aNfcid3i, 0, PHPAL_I18092MT_NFCID3_LENGTH);
    (void)memset(pDataParams->aNfcid3t, 0, PHPAL_I18092MT_NFCID3_LENGTH);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_DslRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pDslReq,
    uint8_t wDslReqLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t     PH_MEMLOC_REM *pFrame = NULL;
    uint8_t     PH_MEMLOC_REM *pPayload = NULL;
    uint8_t     PH_MEMLOC_REM aCmd[1U+3U];
    uint8_t     PH_MEMLOC_REM bDid;
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;

    if(pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_DSL_REQ)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    /* Validate the Packet for DSL_REQ and also DID and NAD values. */
    status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
        pDataParams,
        pDslReq,
        wDslReqLength,
        PHPAL_I18092MT_SW_CMD_DSL_REQ,
        &pFrame,
        &wFrameLength,
        &pPayload,
        &wPayloadLength
        );

    /* wait for next data frame */
    if(status != PH_ERR_SUCCESS)
    {
        return status;
    }

    /* Ignore the length byte */
    ++pFrame;
    --wFrameLength;

    /* NFC Digital Protocol 1.1 - 16.9.1.2*/
    if(pDataParams->bDid == 0U)
    {
        if (wFrameLength != PHPAL_I18092MT_SW_DSL_REQ_LENGTH)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
    }
    else
    {
        if (wFrameLength != (PHPAL_I18092MT_SW_DSL_REQ_LENGTH + 1U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
        else
        {
            bDid = pFrame[PHPAL_I18092MT_SW_DSL_REQ_DID_POS];

            if (pDataParams->bDid != bDid && pDataParams->bDid != 0U)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
            }

            aCmd[bCmdLen++] = pDataParams->bDid;
        }
    }

    pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_DSL_REQ;
    /*
       The frame for DSL_RES 12.7.1.2
       +--------+--------+--------+
       | CMD 0  | CMD 1  | Byte 0 |
       +--------+--------+--------+
       |  (D5)  |  (09)  | [DID]  |
       +--------+--------+--------+
    */

    /* Write frame header. */
    PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_WriteFrameHeader(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        PHPAL_I18092MT_SW_CMD_DSL_RES,
        bCmdLen));

    status = phhalHw_Transmit(pDataParams->pHalDataParams,
        PH_TRANSMIT_BUFFER_LAST,
        aCmd,
        (uint16_t)bCmdLen
        );

    return status;
}

phStatus_t phpalI18092mT_Sw_PslRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pPslReq,
    uint16_t  wPslReqLength,
    uint16_t  wActiveMode
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM *pFrame = NULL;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint8_t     PH_MEMLOC_REM *pPayload = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t     PH_MEMLOC_REM aCmd[1U+3U];
    uint8_t     PH_MEMLOC_REM bBrs;
    uint8_t     PH_MEMLOC_REM bDst;
    uint8_t     PH_MEMLOC_REM bDrt;
    uint8_t     PH_MEMLOC_REM bFsl;
    uint16_t    PH_MEMLOC_REM wTargetFraming;
    uint16_t    PH_MEMLOC_REM wTxSettings = 0;
    uint16_t    PH_MEMLOC_REM wRxSettings = 0;
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCheckBaudrate = PHHAL_HW_RF_DATARATE_424;
    uint16_t    PH_MEMLOC_REM a18092T_DrTable[4] = {PHHAL_HW_RF_DATARATE_106, PHHAL_HW_RF_DATARATE_212, PHHAL_HW_RF_DATARATE_424, PHHAL_HW_RF_DATARATE_848};
    uint8_t     PH_MEMLOC_REM bNFCMode = PH_OFF;

    wTxSettings = 0;

    if (wActiveMode == PH_ON)
    {
        if (((pDataParams->bCmdtype != PHPAL_I18092MT_SW_CMD_ATR_REQ) &&
            (pDataParams->bCmdtype != PHPAL_I18092MT_SW_CMD_WUP_REQ)) ||
            (pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_PSL_REQ))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
    }
    else
    {
        if ((pDataParams->bCmdtype != PHPAL_I18092MT_SW_CMD_ATR_REQ) ||
            (pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_PSL_REQ))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
    }

    status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
        pDataParams,
        pPslReq,
        wPslReqLength,
        PHPAL_I18092MT_SW_CMD_PSL_REQ,
        &pFrame,
        &wFrameLength,
        &pPayload,
        &wPayloadLength
        );
    pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_PSL_REQ;

    /* wait for next data frame */
    if(status != PH_ERR_SUCCESS)
    {
        return status;
    }

    /* Ignore the length byte */
    ++pFrame;
    --wFrameLength;

    /* Check the frame length of the PSL command */
    if (wFrameLength != (PHPAL_I18092MT_SW_PSL_REQ_LENGTH))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    /*
      Format of the PSL_REQ command, 12.5.3.1
      +--------+--------+--------+--------+--------+
      | CMD 0  | CMD 1  | Byte 0 | Byte 1 | Byte 2 |
      +--------+--------+--------+--------+--------+
      |  (D4)  |  (04)  |   DID  |   BRS  |  FSL   |
      +--------+--------+--------+--------+--------+
    */

    if(pDataParams->bDid != pFrame[PHPAL_I18092MT_SW_PSL_REQ_DID_POS])
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    bBrs = pFrame[PHPAL_I18092MT_SW_PSL_REQ_BRS_POS];

    if(0U != (bBrs & 0xC0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    bDst = (bBrs >> 3U)&0x07U;
    bDrt = bBrs & 0x07U;

    bFsl = pFrame[PHPAL_I18092MT_SW_PSL_REQ_FSL_POS];

    /* FSL shall be less than or equal to lowest of LRi and LRt */
    if((bFsl > pDataParams->bLri) || (bFsl > pDataParams->bLrt))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }


    /* Parameter check */
    if ((bDst > bCheckBaudrate) ||
        (bDrt > bCheckBaudrate) ||
        (bFsl > PHPAL_I18092MT_FRAMESIZE_254))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
    }

    /* Form the response and send back the response */
    /*
       Format of the PSL_RES command, 12.5.3.2
       +--------+--------+--------+
       | CMD 0  | CMD 1  | Byte 0 |
       +--------+--------+--------+
       |  (D5)  |  (05)  |   DID  |
       +--------+--------+--------+
     */
    aCmd[bCmdLen++] = pDataParams->bDid;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TARGET_DATARATE_FRAMING,
        &wTargetFraming));

    if ((wTargetFraming & PHHAL_HW_RF_DATARATE_OPTION_MASK) != a18092T_DrTable[bDrt])
    {
        if (wActiveMode == PH_ON)
        {
            /* Set the active mode framing */
            wTxSettings = a18092T_DrTable[bDrt] | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING;
            wRxSettings = a18092T_DrTable[bDst] | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING;
        }
        /* For Passive Mode Framing set corresponding Technology Framing */
        else
        {
            if (pDataParams->bPropPSLMode == PH_OFF)
            {
                if (a18092T_DrTable[bDrt] == PHHAL_HW_RF_DATARATE_106)
                {
                    wTxSettings = a18092T_DrTable[bDrt] | PHHAL_HW_RF_TYPE_A_FRAMING;
                }
                else
                {
                    wTxSettings = a18092T_DrTable[bDrt] | PHHAL_HW_RF_TYPE_F_FRAMING;
                }

                if (a18092T_DrTable[bDst] == PHHAL_HW_RF_DATARATE_106)
                {
                    wRxSettings = a18092T_DrTable[bDst] | PHHAL_HW_RF_TYPE_A_FRAMING;
                }
                else
                {
                    wRxSettings = a18092T_DrTable[bDst] | PHHAL_HW_RF_TYPE_F_FRAMING;
                }
            }
        }
        /* Apply Rx data-rate to HAL. */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            wRxSettings));
    }

    /* Write frame header. */
    PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_WriteFrameHeader(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        PHPAL_I18092MT_SW_CMD_PSL_RES,
        bCmdLen));

    PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(pDataParams->pHalDataParams,
        PH_TRANSMIT_BUFFER_LAST,
        aCmd,
        (uint16_t)bCmdLen));

    if ((wTargetFraming & PHHAL_HW_RF_DATARATE_OPTION_MASK) != a18092T_DrTable[bDrt])
    {
        /* Apply Tx-datarate to HAL. */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXDATARATE_FRAMING,
            wTxSettings));


        /* Enable/Disable handling of StartByte in 106kbps baudrate. */
        if ((a18092T_DrTable[bDrt] == PHHAL_HW_RF_DATARATE_106) || (bNFCMode == PH_ON))
        {
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_ON));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_OFF));
        }
    }

    /* All OK, store the Dsi, Dri and FSL value */
    pDataParams->bDst = bDst;
    pDataParams->bDrt = bDrt;
    pDataParams->bFsl = bFsl;

    /* Set LRi and LRt to FSL */
    pDataParams->bLri = bFsl;
    pDataParams->bLrt = bFsl;

    return status;
}

phStatus_t phpalI18092mT_Sw_RlsRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pRlsReq,
    uint16_t wRlsReqLength
    )
{
    phStatus_t PH_MEMLOC_REM status;

    phStatus_t PH_MEMLOC_REM statusTmp;

    uint8_t    PH_MEMLOC_REM *pFrame = NULL;
    uint16_t   PH_MEMLOC_REM wFrameLength = 0;
    uint8_t    PH_MEMLOC_REM *pPayload = NULL;
    uint16_t   PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t    PH_MEMLOC_REM aCmd[1U+3U];
    uint8_t    PH_MEMLOC_REM bDid;
    uint8_t    PH_MEMLOC_REM bCmdLen = 0;

    if(pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_RLS_REQ)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
        pDataParams,
        pRlsReq,
        wRlsReqLength,
        PHPAL_I18092MT_SW_CMD_RLS_REQ,
        &pFrame,
        &wFrameLength,
        &pPayload,
        &wPayloadLength
        );

    /* wait for next data frame */
    if(status != PH_ERR_SUCCESS)
    {
        return status;
    }

    /* Ignore the length byte */
    ++pFrame;
    --wFrameLength;

    /* NFC Digital Protocol 1.1 16.10.1.2 */
    if(pDataParams->bDid == 0U)
    {
        if (wFrameLength != PHPAL_I18092MT_SW_RLS_REQ_LENGTH)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
    }
    else
    {
        if (wFrameLength != (PHPAL_I18092MT_SW_RLS_REQ_LENGTH + 1U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
        else
        {
            bDid = pFrame[PHPAL_I18092MT_SW_RLS_REQ_DID_POS];

            if (pDataParams->bDid != bDid && pDataParams->bDid != 0U)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
            }

            aCmd[bCmdLen++] = pDataParams->bDid;
        }
    }

    pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_RLS_REQ;
    /*
       The frame for RL_RES, 12.7.2.2
       +--------+--------+--------+
       | CMD 0  | CMD 1  | Byte 0 |
       +--------+--------+--------+
       |  (D5)  |  (0B)  | [DID]  |
       +--------+--------+--------+
    */

    /* Write frame header. */
    PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_WriteFrameHeader(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        PHPAL_I18092MT_SW_CMD_RLS_RES,
        bCmdLen));

    status =  phhalHw_Transmit(pDataParams->pHalDataParams,
        PH_TRANSMIT_BUFFER_LAST,
        aCmd,
        (uint16_t)bCmdLen
        );


    if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        /* Stop timer */
        if(0U == (pDataParams->bRtoxDisabled))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_TimerStop());
        }
    }


    return status;

}

phStatus_t phpalI18092mT_Sw_WupRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pWupReq,
    uint16_t  wWupReqLength,
    uint16_t  wActiveMode
    )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM *pFrame = NULL;
    uint16_t   PH_MEMLOC_REM wFrameLength = 0;
    uint8_t    PH_MEMLOC_REM *pPayload = NULL;
    uint16_t   PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t    PH_MEMLOC_REM aCmd[1U+3U];
    uint8_t    PH_MEMLOC_REM bCmdLen = 0;

    if((wActiveMode == PH_OFF) || (pDataParams->bCmdtype != PHPAL_I18092MT_SW_CMD_DSL_REQ))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
        pDataParams,
        pWupReq,
        wWupReqLength,
        PHPAL_I18092MT_SW_CMD_WUP_REQ,
        &pFrame,
        &wFrameLength,
        &pPayload,
        &wPayloadLength
        );

    /* wait for next data frame */
    if( status != PH_ERR_SUCCESS)
    {
        return status;
    }

    /* Ignore the length byte */
    ++pFrame;
    --wFrameLength;

    if(memcmp(&pFrame[2], pDataParams->aNfcid3t, PHPAL_I18092MT_NFCID3_LENGTH) != 0)
    {
        return PH_ERR_PROTOCOL_ERROR;
    }

    if (wFrameLength != PHPAL_I18092MT_SW_WUP_REQ_LENGTH )
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }
    if(pDataParams->bDid != pFrame[PHPAL_I18092MT_SW_WUP_REQ_LENGTH- 1u])
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
    }

    pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_WUP_REQ;
    /*
       The frame for WUP_RES, 12.5.2.2
       +--------+--------+--------+
       | CMD 0  | CMD 1  | Byte 0 |
       +--------+--------+--------+
       |  (D5)  |  (03)  | DID    |
       +--------+--------+--------+
    */

    aCmd[bCmdLen++] = pDataParams->bDid;


    /* Write frame header. */
    PH_CHECK_SUCCESS_FCT( status, phpalI18092mT_Sw_WriteFrameHeader(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        PHPAL_I18092MT_SW_CMD_WUP_RES,
        bCmdLen));

    return phhalHw_Transmit(pDataParams->pHalDataParams,
        PH_TRANSMIT_BUFFER_LAST,
        aCmd,
        (uint16_t)bCmdLen
        );

}

phStatus_t phpalI18092mT_Sw_Activate(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pAtrReq,
    uint16_t wAtrReqLength,
    uint8_t * pAtrRes,
    uint16_t wAtrResLength,
    uint8_t ** pGeneralDataBytes,
    uint16_t *wGeneralDataBytesLength
    )
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM *pFrame = NULL;
    uint8_t     PH_MEMLOC_REM *pPayload = NULL;
    uint8_t     PH_MEMLOC_REM *pRxBuf = NULL;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint16_t    PH_MEMLOC_REM wDataRate;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint16_t    PH_MEMLOC_REM wRxLen = 0;
    uint16_t    PH_MEMLOC_REM wActiveMode;
    uint8_t     PH_MEMLOC_REM aCmd[18];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bValidRespRec = 0x01;
    uint8_t     PH_MEMLOC_REM bGtLength = 0x00;

    if ((wAtrReqLength == 0x00U) || (pAtrReq == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_PAL_I18092MT);
    }

    /* value should be updated only if general bytes are present */
    *wGeneralDataBytesLength = 0;

    /* Retrieve framing and datarate at which Target is been activated. */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TARGET_DATARATE_FRAMING,
        &wDataRate));

    /* If active communication is selected, after WUP_RES we need to wait for PSL_REQ as per ISO/IEC 18092:2013(E)
    * Figure 15: Activation Protocol in Active communication mode and phpalI18092mT_Activate should not be called
    * by the Application developer. */
    /* Ref ISO/IEC 18092:2013(E) 12.5.1.3.2 - Receives a valid ATR_REQ, the Target,
    * shall disable to receive a subsequent ATR_REQ. */
    if((((wDataRate & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_ACTIVE_FRAMING) && (pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_WUP_REQ)) ||
        (pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_ATR_REQ))
    {
        return (PH_ERR_PROTOCOL_ERROR | PH_COMP_PAL_I18092MT);
    }

    do
    {
        /* validate the ATR_REQ command */
        status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
            pDataParams,
            pAtrReq,
            wAtrReqLength,
            PHPAL_I18092MT_SW_CMD_ATR_REQ,
            &pFrame,
            &wFrameLength,
            &pPayload,
            &wPayloadLength
            );
        if (status == PH_ERR_SUCCESS)
        {
            /* verify NFCID in case of FeliCa. NFCForum-TS-DigitalProtocol-1.1 section 16.6.2.2*/
            if ((wDataRate & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_F_FRAMING)
            {
                if (memcmp(&pFrame[PHPAL_I18092MT_SW_ATR_REQ_NFCID3_POS + 1U], &pAtrRes[0], (PHPAL_I18092MT_NFCID3_LENGTH - 2)) != 0)
                {
                    status = PH_ERR_PROTOCOL_ERROR;
                }
            }

            /* Ignore length byte */
            ++pFrame;
            --wFrameLength;

            /* Check for minimum and maximum frame size. NFCForum-TS-DigitalProtocol-1.0 section 14.6.1.3. */
            if ((wFrameLength > PHPAL_I18092MT_SW_MAX_ACTIVATION_FRAME_LENGTH) ||
                (wFrameLength < PHPAL_I18092MT_SW_MIN_ACTIVATION_FRAME_LENGTH))
            {
                status = PH_ERR_PROTOCOL_ERROR;
            }

            /* NFCForum-TS-DigitalProtocol-1.1 section 16.6.2.4 */
            if((pFrame[PHPAL_I18092MT_SW_ATR_REQ_DID_POS] > PHPAL_I18092MT_DID_MAX) && (status == PH_ERR_SUCCESS))
            {
                status = PH_ERR_PROTOCOL_ERROR;
            }

            /* Check for Valid BSi and BRi values. */
            if ((status == PH_ERR_SUCCESS) &&
                ((pFrame[PHPAL_I18092MT_SW_ATR_REQ_BSI_POS] & PHPAL_I18092MT_BIT_RATE_MASK) ||
                (pFrame[PHPAL_I18092MT_SW_ATR_REQ_BRI_POS] & PHPAL_I18092MT_BIT_RATE_MASK)))
            {
                status = PH_ERR_PROTOCOL_ERROR;
            }

            bValidRespRec = 1;
        }

        /* if ART_REQ is not received properly, read it again*/
        while (status != PH_ERR_SUCCESS)
        {
            /* If First received ATR_REQ from Autocoll/Discovery Loop is an Invalid Request then in Active Mode
             * exit immediately with an error. Then Autocoll can be called immediately if in Listen Mode. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_ACTIVEMODE,
                &wActiveMode));
            if (wActiveMode == PH_ON)
            {
                return (status | PH_COMP_PAL_I18092MT);
            }

            /* Enable handling of StartByte in 106kbps baudrate. */
            if((wDataRate & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_DATARATE_106)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_ON));
            }

            /*Restart transceive */
            status = phhalHw_Receive(
                pDataParams->pHalDataParams,
                0x00,
                &pRxBuf,
                &wRxLen);
            pAtrReq =pRxBuf;
            wAtrReqLength = wRxLen;
            if((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
            {
                /* Reset ISO 18092 protocol settings*/
                PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_ResetProtocol(
                    pDataParams
                    ));
                return (PH_ERR_EXT_RF_ERROR | PH_COMP_PAL_I18092MT);
            }
            bValidRespRec = 0;
        }
    }while(!bValidRespRec);

    /* Enable/Disable handling of StartByte in 106kbps baudrate. */
    if((wDataRate & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_DATARATE_106)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_ON));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_OFF));
    }

    /* The format for ATR_REQ, 12.5.1.1 */
    /*
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
      | CMD 0  | CMD 1  | Byte 0 | Byte 9 | Byte 10| Byte 11| Byte 12| Byte 13| Byte 14| Byte n |
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
      |  (D4)  |  (00)  |nfcid3i0|nfcid3i9| DIDi   |  BSi   |  BRi   |  PPi   | [Gi[0]]| [Gi[n]|
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
    */
    /* Skip the command bytes, CMD0 and CMD1
       and copy the NFCID3 of the initiator store that info */
    (void)memmove(pDataParams->aNfcid3i, &pFrame[PHPAL_I18092MT_SW_ATR_REQ_NFCID3_POS], PHPAL_I18092MT_NFCID3_LENGTH);  /* PRQA S 3200 */

    /* Copy the DID, BSI and BRI */
    pDataParams->bDid = pFrame[PHPAL_I18092MT_SW_ATR_REQ_DID_POS];
    pDataParams->bBsi = pFrame[PHPAL_I18092MT_SW_ATR_REQ_BSI_POS] & ((uint8_t)(~(uint8_t)PHPAL_I18092MT_BIT_RATE_MASK));
    pDataParams->bBri = pFrame[PHPAL_I18092MT_SW_ATR_REQ_BRI_POS] & ((uint8_t)(~(uint8_t)PHPAL_I18092MT_BIT_RATE_MASK));

    /* Extract LRi bits,  Gi and NADen bit , 12.5.1.1
       +--------+--------+--------+--------+--------+--------+--------+--------+
       | bit 7  | bit 6  | bit 5  | bit 4  | bit 3  | bit 2  | bit 1  | bit 0  |
       +--------+--------+--------+--------+--------+--------+--------+--------+
       |   0    |   0    |  LRi   |  LRi   |   0    |   0    |   Gi   |  NAD   |
       +--------+--------+--------+--------+--------+--------+--------+--------+
    */
    /* copy LRi */
    pDataParams->bLri = (pFrame[PHPAL_I18092MT_SW_ATR_REQ_PPI_POS] >> PHPAL_I18092MT_SW_ATR_PPT_LRT_BIT) & 0x03U;

    if((pFrame[PHPAL_I18092MT_SW_ATR_REQ_PPI_POS] & PHPAL_I18092MT_SW_ATR_REQ_PPI_NAD_USED))
    {
        pDataParams->bNadEnabled = PH_ON;
    }
    else
    {
        /* No NAD support */
        pDataParams->bNadVal = 0;
        pDataParams->bNadEnabled = PH_OFF;
    }

    /* Check Gi presence/absence */
    if (0U != (pFrame[PHPAL_I18092MT_SW_ATR_REQ_PPI_POS] & PHPAL_I18092MT_SW_ATR_REQ_PPI_GT_AVAILABLE))
    {
        /* Gi should be present */
        if (wFrameLength <= PHPAL_I18092MT_SW_MIN_ACTIVATION_FRAME_LENGTH)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
        /* assign Gt ptr to and length*/
        *pGeneralDataBytes = &pFrame[PHPAL_I18092MT_SW_ATR_REQ_GT_POS];
        *wGeneralDataBytesLength = (uint16_t)(wFrameLength - PHPAL_I18092MT_SW_ATR_REQ_GT_POS);
    }
    else
    {
        /* Gi should be absent */
        if (wFrameLength > PHPAL_I18092MT_SW_MIN_ACTIVATION_FRAME_LENGTH)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MT);
        }
    }

    /* Form the AtrRes frame */
    /* The format of ATR RES
       +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
       | CMD 0  | CMD 1  | Byte 0...Byte 9 | Byte 10| Byte 11| Byte 12| Byte 13| Byte 14| Byte 15| Byte n |
       +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
       |  (D5)  |  (01)  |nfcid3i0|nfcid3i9| DIDt   |  BSt   |  BRt   |   TO   |  PPt   | [Gi[0]]| [Gi[n] |
       +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
    */
    /* Copy the NFC ID3 */
    (void)memmove(pDataParams->aNfcid3t, &pAtrRes[0], PHPAL_I18092MT_NFCID3_LENGTH); /* PRQA S 3200 */
    (void)memmove(&aCmd[bCmdLen], &pAtrRes[0], PHPAL_I18092MT_NFCID3_LENGTH);  /* PRQA S 3200 */
    bCmdLen = PHPAL_I18092MT_NFCID3_LENGTH;

    /* Copy the DIDt.The DIDt shall have the same value as the DIDi*/
    aCmd[bCmdLen++] = pDataParams->bDid;

    /* copy BSt from 11th position */
    if (!(pAtrRes[bCmdLen - 1] & ((uint8_t)(~(uint8_t)PHPAL_I18092MT_BIT_RATE_MASK))))
    {
        pDataParams->bBst = pAtrRes[bCmdLen - 1];
        aCmd[bCmdLen] = pAtrRes[bCmdLen - 1];
        bCmdLen++;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
    }
    /* copy Brt from 12th position */
    if (!(pAtrRes[bCmdLen - 1] & 0x0FU))
    {
        pDataParams->bBrt = pAtrRes[bCmdLen - 1];
        aCmd[bCmdLen] = pAtrRes[bCmdLen - 1];
        bCmdLen++;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
    }

    /* check for timeout value */
    if (pAtrRes[bCmdLen - 1] > PHPAL_I18092MT_WT_MAX)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
    }
    else
    {
        pDataParams->bTo = pAtrRes[bCmdLen - 1];
        aCmd[bCmdLen]  = pAtrRes[bCmdLen - 1];
        bCmdLen++;
    }

    /* copy PPt:Presence of Optional Parameters.
     * At bit 6 and bit 5: Length Reduction value
     * bit 2: If bit is set to ONE then it indicates General bytes available.
     * bit 1: If bit is set to ONE then it indicates the Target uses NAD.
     */
    pDataParams->bLrt = (pAtrRes[bCmdLen - 1]  >> PHPAL_I18092MT_SW_ATR_PPT_LRT_BIT) & 0x03U;
    aCmd[bCmdLen] = pAtrRes[bCmdLen - 1] & 0x30U;

    if (0U != (pDataParams->bNadEnabled))
    {
        aCmd[bCmdLen] |= PHPAL_I18092MT_SW_ATR_REQ_PPI_NAD_USED;
    }
    if (0U != (pAtrRes[bCmdLen - 1] & PHPAL_I18092MT_SW_ATR_RES_PPT_GT_AVAILABLE))
    {
        bGtLength = (uint8_t)(wAtrResLength - bCmdLen);
        /* NFCForum-TS-DigitalProtocol-1.1 section 16.6.1.2 The Target SHALL treat an ATR_REQ Command
         * with a number of bytes greater than 64 as a Protocol Error.
         */
        if (bGtLength > PHPAL_I18092MT_MAX_GI_LENGTH)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
        }
        if (bGtLength > 0U)
        {
            aCmd[bCmdLen] |= PHPAL_I18092MT_SW_ATR_RES_PPT_GT_AVAILABLE;
        }
    }
    /* increment to add PPt data*/
    bCmdLen++;

    /* Set FSL to the lowest frame size (lowest of LRi and LRt) */
    pDataParams->bFsl = (pDataParams->bLrt < pDataParams->bLri) ? pDataParams->bLrt : pDataParams->bLri;

    pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_ATR_REQ;

    /* Write frame header. */
    PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_WriteFrameHeader(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        PHPAL_I18092MT_SW_CMD_ATR_RES,
        (uint8_t)(bCmdLen + bGtLength)
        ));

    if (bGtLength > 0U)
    {
        /* Write data to internal buffer. */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
            pDataParams->pHalDataParams,
            PH_TRANSMIT_BUFFER_CONT,
            aCmd,
            (uint16_t)bCmdLen
            ));

        /* Write general bytes, transmit the whole buffer and receive response. */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(pDataParams->pHalDataParams,
            PH_TRANSMIT_BUFFER_LAST,
            &pAtrRes[bCmdLen - 1],
            bGtLength
            ));

    }
    else
    {
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
            pDataParams->pHalDataParams,
            PH_TRANSMIT_BUFFER_LAST,
            aCmd,
            (uint16_t)bCmdLen
            ));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_Receive(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t   wOption,
    uint8_t ** ppTransportDataBytes,
    uint16_t * pTransportDataBytesLength
    )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_PROTOCOL_ERROR;
    phStatus_t PH_MEMLOC_REM statusTmp = PH_ERR_PROTOCOL_ERROR;
    uint16_t   PH_MEMLOC_REM wRxLen = 0;
    uint16_t   PH_MEMLOC_REM wDepReqLen = 0;
    uint16_t   PH_MEMLOC_REM wPayloadLength = 0;
    uint16_t   PH_MEMLOC_REM wFrameLength = 0;
    uint16_t   PH_MEMLOC_REM wRxOverlappedStartPos = 0;
    uint16_t   PH_MEMLOC_REM wActiveMode = 0;
    uint16_t   PH_MEMLOC_REM wOptionTmp = wOption;
    uint16_t   PH_MEMLOC_REM wPrePayloadLength = 0;
    uint8_t    PH_MEMLOC_REM bOffset = 0x02;
    uint8_t    PH_MEMLOC_REM bPfb;
    uint8_t    PH_MEMLOC_REM bProtLength = 0;
    uint8_t    PH_MEMLOC_REM *pPayload = NULL;
    uint8_t    PH_MEMLOC_REM *pFrame = NULL;
    uint8_t    PH_MEMLOC_REM bDataRec = PH_OFF;
    uint8_t    PH_MEMLOC_REM bNadCheck = 1;
    uint8_t    PH_MEMLOC_REM *pRxBuf = NULL;
    uint8_t    PH_MEMLOC_REM aRxOverlapped[8];
    uint8_t    PH_MEMLOC_REM bInvalidPsl;
    uint8_t    PH_MEMLOC_REM bWakeup;
    uint8_t    PH_MEMLOC_REM bPrevPni;
    uint16_t   PH_MEMLOC_REM a18092T_FslTable[4] = {65, 129, 193, 255};

    /* Reset Previous Payload length */
    wPrePayloadLength = 0;
    bInvalidPsl = PH_OFF;
    bWakeup = PH_OFF;
    bPrevPni = 0;

    if (wOption == PH_RECEIVE_CHAINING)
    {
        bInvalidPsl = PH_ON;

        /* Stop timer */
        if(0U == (pDataParams->bRtoxDisabled))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_TimerStop());
        }
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_ACTIVEMODE,
        &wActiveMode));

    do
    {
        switch(wOption)
        {
        case PH_RECEIVE_DEFAULT:
        case PH_RECEIVE_CHAINING_BUFSIZE:
            /* wait for next data frame */
            while(status != PH_ERR_SUCCESS)
            {
                /*Restart transcieve*/
                status = phhalHw_Receive(
                    pDataParams->pHalDataParams,
                    PH_RECEIVE_DEFAULT,
                    &pRxBuf,
                    &wRxLen);

                if (((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR) ||
                    ((status & PH_ERR_MASK) == PH_ERR_ABORTED) ||
                    ((status & PH_ERR_MASK) == PH_ERR_RF_ERROR))
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_ResetProtocol(
                        pDataParams
                        ));

                    /* Reset RxBuffer Start Position :this is added to take care if RF is lost during DEP chaining*/
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

                    return status;
                }

                if ((((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
                    ((wOptionTmp & PH_RECEIVE_CHAINING_BUFSIZE) == PH_RECEIVE_CHAINING_BUFSIZE)) ||
                    ((wRxLen - wDepReqLen) > a18092T_FslTable[pDataParams->bLrt]))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_I18092MT);
                }
            }
            bDataRec = PH_ON;
            break;
        case PH_RECEIVE_CHAINING:
            /* send ACK first and then wait for response */
            /* Chaining information PDU received, send ACK. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_ResponseToPfb(
                pDataParams,
                PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK
                ));

            pDataParams->bPni =(uint8_t) PHPAL_I18092MT_SW_INCREMENT_PNI((uint8_t)pDataParams->bPni);
            bDataRec = PH_OFF;
            wOption = PH_RECEIVE_DEFAULT;
            pDataParams->bStateNow = PHPAL_I18092MT_SW_STATE_CHAINING_BIT;
            break;

        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
        }

        if(bDataRec == PH_ON)
        {
            bDataRec = PH_OFF;
            wOption = PH_RECEIVE_DEFAULT;
            /* to suppress Qmore warnings */
            if(pRxBuf == NULL)
            {
                break;
            }

            switch(pRxBuf[bOffset + wDepReqLen])
            {
            case PHPAL_I18092MT_SW_CMD_PSL_REQ:
                /* ref ISO/IEC 18092:2013 section 12.5.3.3.2.b */
                if(bInvalidPsl == PH_OFF)
                {
                    /* after receiving valid PSL, we should wait till we get valid DEP_REQ or
                    * any other valid frame */
                    status = phpalI18092mT_Sw_PslRes(pDataParams,
                        pRxBuf,
                        (uint8_t)wRxLen,
                        wActiveMode
                        );
                }
                status = PH_ERR_INTERNAL_ERROR;
                break;
            case PHPAL_I18092MT_SW_CMD_DSL_REQ:
                /* after receiving valid DSL_REQ, send DSL_RES and return with PH_ERR_DESELECTED as status.
                * after receiving invalid DSL_REQ, wait for next valid frame.
                */
                status = phpalI18092mT_Sw_DslRes(pDataParams,
                    &pRxBuf[wDepReqLen],
                    (uint16_t)(wRxLen - wDepReqLen)
                    );
                /* in case of active mode we need to wait for */
                if((status == PH_ERR_SUCCESS) && (wActiveMode == PH_OFF))
                {
                    bDataRec = PH_ON;
                    status =  PH_ADD_COMPCODE(PH_ERR_SUCCESS_DESELECTED, PH_COMP_PAL_I18092MT);
                }
                else
                {
                    /* wait for Wakeup cmd */
                    status = PH_ERR_INTERNAL_ERROR;
                }
                break;
            case PHPAL_I18092MT_SW_CMD_RLS_REQ:
                status = phpalI18092mT_Sw_RlsRes(pDataParams,
                    &pRxBuf[wDepReqLen],
                    (uint16_t)(wRxLen - wDepReqLen)
                    );
                if(status == PH_ERR_SUCCESS)
                {
                    bDataRec = PH_ON;
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_RELEASED, PH_COMP_PAL_I18092MT);
                }
                break;
            case PHPAL_I18092MT_SW_CMD_WUP_REQ:
                /* If Active Mode is set then send wake-up response. */
                if (wActiveMode == PH_ON)
                {
                    status = phpalI18092mT_Sw_WupRes(pDataParams,
                        pRxBuf,
                        wRxLen,
                        wActiveMode
                        );

                    if (status == PH_ERR_SUCCESS)
                    {
                        bWakeup = PH_ON;
                    }
                }
                /* Wait in receive mode itself. */
                status = PH_ERR_INTERNAL_ERROR;
                break;
            case PHPAL_I18092MT_SW_CMD_DEP_REQ:
                /* Backup previous PNI value. */
                bPrevPni = pDataParams->bPni;

                /* validate DEP_REQ */
                status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
                    pDataParams,
                    &pRxBuf[wDepReqLen],
                    (uint16_t)(wRxLen - wDepReqLen),
                    PHPAL_I18092MT_SW_CMD_DEP_REQ,
                    &pFrame,
                    &wFrameLength,
                    &pPayload,
                    &wPayloadLength
                    );

                /* NAD is removed by the phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame in case of First DEP_REQ. */
                if(0U != (bNadCheck))
                {
                    if (wRxLen >= wPayloadLength)
                    {
                        bProtLength = (uint8_t)(wRxLen - wPayloadLength);
                    }
                    bNadCheck = 0;
                }

                if (status == PH_ERR_SUCCESS)
                {
                    bPfb = pRxBuf[bOffset + 1 + wDepReqLen];
                    wDepReqLen = (uint16_t)(wDepReqLen + wPayloadLength);

                    if(0U != (bPfb & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK))
                    {
                        status = phpalI18092mT_Sw_ValidatePfb(
                            pDataParams,
                            bPfb,
                            &bDataRec);
                        break;
                    }

                    if (0U != (wRxOverlappedStartPos))
                    {
                        /* Restore overlapped bytes */
                        (void)memcpy(&pRxBuf[wRxOverlappedStartPos - 1u],
                            &aRxOverlapped[0],
                            (uint32_t)((wRxLen - wDepReqLen) + /* */ 1));  /* PRQA S 3200 */

                        /* After ATN, same payload received before ATN is received again, overwrite the previous data. */
                        if ((pDataParams->bCmdtype == PHPAL_I18092MT_SW_CMD_ATN) && (bPrevPni != pDataParams->bPni))
                        {
                            wDepReqLen = wDepReqLen - wPrePayloadLength;
                            wRxLen = wRxLen - wPrePayloadLength;
                            (void)memcpy(&pRxBuf[wRxLen], pPayload, wPayloadLength);
                        }
                    }
                    wOption = PH_RECEIVE_CHAINING;

                    /* Update the Command Type in Dataparams. */
                    pDataParams->bCmdtype = PHPAL_I18092MT_SW_CMD_DEP_REQ;

                    if (PHPAL_I18092MT_SW_IS_CHAINING(bPfb))
                    {
                        status = phpalI18092mT_Sw_HandlechainedData(
                            pDataParams,
                            &pRxBuf[0],
                            wRxLen,
                            &aRxOverlapped[0],
                            &wRxOverlappedStartPos,
                            (uint16_t)(wRxLen - wDepReqLen),
                            wOptionTmp,
                            &bDataRec
                            );

                        /* Backup previous length */
                        wPrePayloadLength = wPayloadLength;

                        break;
                    }
                    bDataRec = PH_ON;

                    /* we have received single frame
                    * give the response to application layer  */
                    pDataParams->bStateNow = PHPAL_I18092MT_SW_STATE_NONE;
                    status = PH_ERR_SUCCESS;
                }
                break;

            default:
                status = PH_ERR_PROTOCOL_ERROR;
                break;
            }
            if (bWakeup == PH_ON)
            {
                bInvalidPsl = PH_OFF;
                bWakeup = PH_OFF;
            }
            else
            {
                bInvalidPsl = PH_ON;
            }
        }
    } while(!bDataRec);

    /* Reset RxBuffer Start Position, in case RxBuffer start position is not zero. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0x00));

    /* Other than DEP_REQ, removed NAD Byte if Present. */
    if(0U != (bNadCheck))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_GetPduHeaderLength(
            pDataParams,
            &bProtLength
            ));
    }

    *pTransportDataBytesLength = (uint16_t)(wRxLen - bProtLength);
    if(pRxBuf != NULL)
    {
        *ppTransportDataBytes = pRxBuf + bProtLength;
    }

    if((!pDataParams->bRtoxDisabled) &&
        (((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)))
    {

        PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_TimerStart(PH_DRIVER_TIMER_MICRO_SECS,
                        ((PHPAL_I18092MT_SW_CALCULATE_RWT_US(pDataParams->bTo)) * pDataParams->bRtoxExtnVal),
                        (pphDriver_TimerCallBck_t) pDataParams->pRtoxCallback));
    }

    return PH_ADD_COMPCODE(status, PH_COMP_PAL_I18092MT);
}

phStatus_t phpalI18092mT_Sw_Transmit(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t   wOption,
    uint8_t * pTransportDataBytes,
    uint16_t wTransportDataBytesLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxOption;

    /* Used to build block frames */
    uint16_t    PH_MEMLOC_REM wTxBufferSize;
    uint16_t    PH_MEMLOC_REM wTxBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wMaxFrameSize;
    uint16_t    PH_MEMLOC_REM wBytesToSend = 0;
    uint8_t     PH_MEMLOC_REM bForceSend;
    uint8_t     PH_MEMLOC_REM bHeaderLength = 0;
    uint16_t   PH_MEMLOC_REM a18092T_FslTable[4] = {65, 129, 193, 255};

    /* Option parameter check */
    if (wOption &  (uint16_t)~(uint16_t)
        (PH_TRANSMIT_BUFFERED_BIT | PH_TRANSMIT_LEAVE_BUFFER_BIT | PH_TRANSMIT_TXCHAINING))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
    }

    /* Save the received Option. */
    wRxOption = wOption;


    /* Stop timer */
    if(0U == (pDataParams->bRtoxDisabled))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_TimerStop());
    }


    /* Retrieve HAL Tx buffer size. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE, &wTxBufferSize));

    /* Find the biggest possible frame size before chaining is needed. */
    wMaxFrameSize = a18092T_FslTable[pDataParams->bLri];

    pDataParams->bStateNow = PHPAL_I18092MT_SW_STATE_TX;

    /* ******************************** */
    /*       PDU TRANSMISSION LOOP      */
    /* ******************************** */
    do
    {
        /* Reset Preloaded bytes and ForceSend */
        wTxBufferLen = 0;
        bForceSend = 0;

        /* Set initial bytes to send length to (remaining) input data length */
        wBytesToSend = wTransportDataBytesLength;

        /* Retrieve Number of preloaded bytes */
        if (0U != (wOption & PH_TRANSMIT_LEAVE_BUFFER_BIT))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                &wTxBufferLen));
        }

        /* Frame has already been preloaded -> don't perform Forecast of header. */
        if (wTxBufferLen > 0U)
        {
            /* Reset forecast header length to zero. */
            bHeaderLength = 0;
        }
        else
        {
            /* Forecast header length */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_GetPduHeaderLength(pDataParams, &bHeaderLength));
        }

        /* Check if we need to chain -> Data to be sent exceeds maximum frame size or HAL buffer */
        if (((uint16_t)bHeaderLength + wTxBufferLen + wBytesToSend) > wMaxFrameSize)
        {
            /* Force frame transmit */
            bForceSend = 1;

            /* Limit bytes to send */
            wBytesToSend = (uint16_t)(wMaxFrameSize - wTxBufferLen - (uint16_t)(bHeaderLength));

            /* If there was no chaining indicate that this is the first chain */
            if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))))
            {
                /* Indicate first chaining */
                pDataParams->bStateNow |= (PHPAL_I18092MT_SW_STATE_CHAINING_BIT | PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT);
            }
        }

        /* If received Data exceeds TxBuffer Size then chain to send data. */
        if ((0U != ((wOption & PH_TRANSMIT_BUFFERED_BIT))) &&
            (((uint16_t)bHeaderLength + wTxBufferLen + wBytesToSend) >= wTxBufferSize))
        {
            /* Force frame transmit */
            bForceSend = 1;

            /* force chaining */
            pDataParams->bStateNow |= PHPAL_I18092MT_SW_STATE_CHAINING_BIT;
        }

        /* Check if chaining is intended or not */
        if ((wOption & PH_TRANSMIT_MODE_MASK) == PH_TRANSMIT_TXCHAINING)
        {
            /* If there was no chaining indicate that this is the first chain */
            if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))))
            {
                /* Indicate first chaining */
                pDataParams->bStateNow |= (PHPAL_I18092MT_SW_STATE_CHAINING_BIT | PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT);
            }
        }
        else
        {
            /* If the option is not TxChaining and this is the last packet of Chained Data,
             * then mark it as Last chaining Data, and receive operation should not be performed. */
            if ((wBytesToSend == wTransportDataBytesLength) && (0U == ((wOption & PH_TRANSMIT_BUFFERED_BIT))))
            {
                /* But chaining has been performed -> this is the last chain */
                if (0U != (pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))
                {
                    pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MT_SW_STATE_CHAINING_BIT;
                    pDataParams->bStateNow |= PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT;
                }
            }
        }

        /* Write PDU header if this is the First Frame or in case of Chaining. */
        if (((0U == ((wOption & PH_TRANSMIT_LEAVE_BUFFER_BIT)))) ||
            (pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT) ||
            (0U != ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT))))
        {
            /* If received wOption is Buffering mode and Data length to be transmitted exceeds Frame size then Header is already
            * updated in HAL and only Length and PFB field should be updated. Skip below Header update.
            * Header should be only be updated once again from next Chained packet. */
            if ((0U == ((wRxOption & PH_TRANSMIT_LEAVE_BUFFER_BIT))))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MT_SW_PFB_INFORMATION_PDU_MASK,
                    (pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT) ? PH_ON : PH_OFF,
                    (uint8_t)(wBytesToSend + wTxBufferLen)));

                /* Get the backup length that is already buffered into TX buffer. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                    &(pDataParams->wLastTxLen)));

                /* Preserve buffered data from now on */
                wOption |= PH_TRANSMIT_LEAVE_BUFFER_BIT;
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_UpdatePduHeader(pDataParams, (uint8_t)(wBytesToSend + wTxBufferLen)));

                /* Reset the received Option as header needs to be appended from next chaining packet. */
                wRxOption = PH_TRANSMIT_DEFAULT;
            }
        }
        /* PDU Header has already been written */
        else
        {
            /* Transmit is imminent, update length byte in HAL buffer */
            if ((0U == ((wOption & PH_TRANSMIT_BUFFERED_BIT))))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_UpdatePduHeader(pDataParams, (uint8_t)(wBytesToSend + wTxBufferLen)));
            }
        }

        /* Tx-Buffering mode to store TxData into HAL TxBuffer and no forced transmit is required. */
        if ((!bForceSend) && (0U != ((wOption & PH_TRANSMIT_BUFFERED_BIT))))
        {
            /* Preload the data into the TxBuffer */
            return phhalHw_Transmit(
                pDataParams->pHalDataParams,
                (wOption & (uint16_t)~(uint16_t)PH_TRANSMIT_MODE_MASK),
                pTransportDataBytes,
                wBytesToSend
                );
        }

        /* If no chaining, just do a transmit */
        if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_CHAINING_BIT))))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                wOption,
                pTransportDataBytes,
                wBytesToSend
                ));

            /* Advance to RX state */
            pDataParams->bStateNow = PHPAL_I18092MT_SW_STATE_INFORMATION_PDU_RX;

            /* PNI should be incremented after a successful send response by the target */
            pDataParams->bPni = (uint8_t)PHPAL_I18092MT_SW_INCREMENT_PNI((uint8_t)pDataParams->bPni);
        }
        /* Transmit and receive data until Tx chaining is complete and state is Receive. */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mT_Sw_PerformTxChaining(
                pDataParams,
                wOption,
                wBytesToSend,
                pTransportDataBytes,
                wTransportDataBytesLength
                ));
        }

        /* Clean-up state */
        pDataParams->bStateNow &= (uint8_t)~(uint8_t)(PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT | PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT);

        /* Chaining is active */
        if (pDataParams->bStateNow == (PHPAL_I18092MT_SW_STATE_TX | PHPAL_I18092MT_SW_STATE_CHAINING_BIT))
        {
            /* Bytes to send cannot be less than sent bytes in-case Chaining is still active. */
            if(wTransportDataBytesLength < wBytesToSend)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MT);
            }
            else
            {
                /* Advance Input Data buffer by number of bytes sent. */
                pTransportDataBytes += wBytesToSend;

                /* Below check is added for Qmore compliant. */
                if (wTransportDataBytesLength > 0U)
                {
                    /* Subtract Data Bytes sent from the total number of bytes to be sent. */
                    wTransportDataBytesLength = (uint16_t)(wTransportDataBytesLength - wBytesToSend);
                }
            }
        }

        /* Buffered / TxChaining mode -> finished after sending */
        if ((wTransportDataBytesLength == 0U) &&
            (
            (wOption & PH_TRANSMIT_BUFFERED_BIT) ||
            ((wOption & PH_TRANSMIT_MODE_MASK) == PH_TRANSMIT_TXCHAINING)
            ))
        {
            /* Advance to RX state */
            pDataParams->bStateNow = PHPAL_I18092MT_SW_STATE_INFORMATION_PDU_RX;
        }
    }
    /* Loop as long as the state does not transit to RX mode. */
    while ((pDataParams->bStateNow & PHPAL_I18092MT_SW_STATE_MASK) != PHPAL_I18092MT_SW_STATE_INFORMATION_PDU_RX);

    /* Backup length (for re-transmission in case of NACK)*/
    pDataParams->wLastTxLen =(uint16_t) (wBytesToSend + pDataParams->wLastTxLen + wTxBufferLen);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_Rtox(
    phpalI18092mT_Sw_DataParams_t * pDataParams
    )
{
    phStatus_t PH_MEMLOC_REM  status;
    uint8_t    PH_MEMLOC_REM  bDataRec = PH_OFF;
    uint8_t    PH_MEMLOC_REM  bRetransmit = PH_OFF;
    uint8_t    PH_MEMLOC_REM  bPfb;
    uint8_t    PH_MEMLOC_REM  bPni;

    uint16_t   PH_MEMLOC_REM  NormalizedResponseLength;
    uint8_t *  PH_MEMLOC_REM  pNormalizedResponse = NULL ;

    /* Used for Transmission and Reception. */
    uint8_t *  PH_MEMLOC_REM  pRtoxReqBuf = &pDataParams->bRtoxExtnVal;
    uint16_t   PH_MEMLOC_REM  wRtoxResLen = 0;
    uint8_t *  PH_MEMLOC_REM  pRtoxResBuf = NULL;

    do
    {
        /* Write PDU header for RTOX. */
        PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_WritePduHeader(
            pDataParams,
            PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK,
            PH_ON,
            PHPAL_I18092MT_SW_RTOX_LENGTH));

        /* Transmit RTOX_REQ. */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Transmit(
            pDataParams->pHalDataParams,
            PH_TRANSMIT_LEAVE_BUFFER_BIT,
            pRtoxReqBuf,
            PHPAL_I18092MT_SW_RTOX_LENGTH
            ));

        bRetransmit = PH_OFF;

        do
        {
            /* Wait for valid RTOX_RES. */
            status = phhalHw_Receive(
                pDataParams->pHalDataParams,
                0x00,
                &pRtoxResBuf,
                &wRtoxResLen
                );
            bDataRec = PH_OFF;

            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* Retrieve PFB */
                bPfb = pRtoxResBuf[3];

                /* Validate RTOX Response. */
                status = phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
                    pDataParams,
                    pRtoxResBuf,
                    wRtoxResLen,
                    PHPAL_I18092MT_SW_CMD_DEP_REQ,
                    &pNormalizedResponse,
                    &NormalizedResponseLength,
                    &pRtoxResBuf,
                    &wRtoxResLen);

                if (status == PH_ERR_SUCCESS)
                {
                    /* Validate RTOX Response. */
                    if(PHPAL_I18092MT_SW_IS_TIMEOUT(bPfb))
                    {
                        /* Check if received RTOX Response is having same RTOX value. */
                        if(pRtoxResBuf[0] == pDataParams->bRtoxExtnVal)
                        {
                            /* Received RTOX Value different from what is sent in RTOX Request.
                             * Treat this as an Protocol error and mute till a valid RTOX value is received. */
                            bDataRec = PH_ON;
                        }
                    }
                    /* If a NACK is received, check PNI if the PNI is equal to the PNI of the previous sent PDU, the previous
                     * block shall be re-transmitted, ISO_IEC_18092-2013 rule-12.6.1.3.3 */
                    if (PHPAL_I18092MT_SW_IS_NACK(bPfb))
                    {
                        bPni = bPfb & 0x03U;
                        if (bPni == pDataParams->bPni)
                        {
                            bRetransmit = PH_ON;
                            bDataRec = PH_ON;
                        }
                    }
                    if (PHPAL_I18092MT_SW_IS_ATTENTION(bPfb))
                    {
                        PH_CHECK_SUCCESS_FCT(status, phpalI18092mT_Sw_ResponseToPfb(
                            pDataParams,
                            PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK
                            ));
                    }
                }
            }
            else if((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
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
        while (bDataRec != PH_ON);
    }
    while (0U != bRetransmit);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_SetConfig(
                                      phpalI18092mT_Sw_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t wValue
                                      )
{
    switch (wConfig)
    {

    case PHPAL_I18092MT_CONFIG_RTOX:
        if ((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
        }
        pDataParams->bRtoxDisabled = (uint8_t)(wValue ^ 0x0001U);
        break;

    case PHPAL_I18092MT_CONFIG_RTOX_VALUE:
        if ((wValue < PHPAL_I18092MT_SW_MIN_RTOX_VALUE) || (wValue > PHPAL_I18092MT_SW_MAX_RTOX_VALUE))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
        }
        pDataParams->bRtoxExtnVal = (uint8_t)(wValue);
        break;

    case PHPAL_I18092MT_CONFIG_NAD:
        if (0U != (wValue & PHPAL_I18092MT_MASK_NAD_ENABLE))
        {
            pDataParams->bNadEnabled = 1;
            if ((wValue & PHPAL_I18092MT_MASK_NAD_VALUE) <= PHPAL_I18092MT_NAD_MAX)
            {
                pDataParams->bNadVal = (uint8_t)(pDataParams->bNadVal | ((uint8_t)(wValue & PHPAL_I18092MT_NAD_MAX)));
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MT);
            }
        }
        else
        {
            pDataParams->bNadEnabled = 0;
            pDataParams->bNadVal = 0;
        }
        break;


    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_I18092MT);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mT_Sw_GetConfig(
                                      phpalI18092mT_Sw_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t * pValue
                                      )
{
    switch (wConfig)
    {
    case PHPAL_I18092MT_CONFIG_RTOX:
        *pValue = (uint16_t)(pDataParams->bRtoxDisabled ^ 0x01U);
        break;

    case PHPAL_I18092MT_CONFIG_NAD:
        if (0U != (pDataParams->bNadEnabled))
        {
            *pValue = (uint16_t)(0x0100u | pDataParams->bNadVal);
        }
        else
        {
            *pValue = 0;
        }
        break;

    case PHPAL_I18092MT_CONFIG_DID:
        *pValue = (uint16_t) pDataParams->bDid;
        break;

    case PHPAL_I18092MT_CONFIG_FSL:
        *pValue = (uint16_t) pDataParams->bFsl;
        break;

    case PHPAL_I18092MT_CONFIG_TO:
        *pValue = (uint16_t) pDataParams->bTo;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_I18092MT);
    }

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHPAL_I18092MT_SW */
