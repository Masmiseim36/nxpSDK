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
* Card Emulation component for Type 4A Tag.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#include <ph_RefDefs.h>
#include <ph_Status.h>
#include <phpalI14443p4mC.h>
#include <phOsal.h>

#ifdef NXPBUILD__PHCE_T4T_SW
#include "phceT4T_Sw.h"
#include "phceT4T_Sw_Int.h"

static const uint8_t bceT4TEventName[] = "HceT4T";

phStatus_t phceT4T_Sw_Init(
                           phceT4T_Sw_DataParams_t *pDataParams,
                           uint16_t wSizeOfDataParams,
                           void *pPalI14443p4mCDataParams,
                           uint8_t *pAppBuffer,
                           uint16_t wAppBufferSize
                           )
{
    phStatus_t status;

    /* Parameter check */
    if(sizeof(phceT4T_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_CE_T4T);
    }
    PH_ASSERT_NULL(pDataParams);
    PH_ASSERT_NULL(pPalI14443p4mCDataParams);

    /* Initialize data parameters */
    pDataParams->wId = PH_COMP_CE_T4T | PHCE_T4T_SW_ID;

    pDataParams->pPalI14443p4mCDataParams = pPalI14443p4mCDataParams;
    pDataParams->pAppBuffer               = pAppBuffer;
    pDataParams->wAppBufferSize           = wAppBufferSize;

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    pDataParams->bProprietaryFileCount    = 0;
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

    pDataParams->bHandleUpdateCmd         = PH_OFF;
    pDataParams->bSupportProprietaryCmd   = PH_OFF;

    /* Reset tag state */
    pDataParams->bTagState                = PHCE_T4T_STATE_NONE;

    /* Reset data parameters with default values */
    PH_CHECK_SUCCESS_FCT(status, phceT4T_Sw_Reset(pDataParams));

    pDataParams->T4TEventObj.pEvtName = (uint8_t *)bceT4TEventName;
    pDataParams->T4TEventObj.intialValue = 0;
    return phOsal_EventCreate(&pDataParams->T4TEventObj.EventHandle, (pphOsal_EventObj_t)&pDataParams->T4TEventObj);
}

phStatus_t phceT4T_Sw_Reset(
                            phceT4T_Sw_DataParams_t *pDataParams
                            )
{
    /* If reset is called before NDEF file is set, do not reset tag state. */
    if(pDataParams->bTagState > PHCE_T4T_STATE_NDEF_FILE_CREATED)
    {
        pDataParams->bTagState = PHCE_T4T_STATE_NDEF_FILE_CREATED;
    }

    pDataParams->bRxOption              = 0;
    pDataParams->pSelectedFile          = NULL;
    pDataParams->wSelectedFileId        = 0;
    pDataParams->bFileWriteAccess       = 0;
    pDataParams->dwFileOffset           = 0;
    pDataParams->dwFileSize             = 0;
    pDataParams->pSharedData            = NULL;
    pDataParams->wSharedDataLen         = 0;
    pDataParams->wLc                    = 0;
    pDataParams->wStatusWord            = 0;
    pDataParams->wExitStatus            = 0;

    return PH_ERR_SUCCESS;
}

phStatus_t phceT4T_Sw_SetElementaryFile(
                                        phceT4T_Sw_DataParams_t *pDataParams,
                                        uint8_t bFileType,
                                        uint8_t *pFile,
                                        uint16_t wFileId,
                                        uint32_t dwFileSize,
                                        uint32_t dwContentLen
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Validate maximum size */
    if(((bFileType == PHCE_T4T_FILE_CC) || (bFileType == PHCE_T4T_FILE_PROPRIETARY))
       && (dwFileSize > 0xFFFEU))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* NULL check */
    if(pFile == NULL)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Check file type and call corresponding internal function */
    if(bFileType == PHCE_T4T_FILE_CC)
    {
        PH_CHECK_SUCCESS_FCT(status, phceT4T_Sw_Int_SetCcFile(
            pDataParams,
            pFile,
            wFileId,
            (uint16_t)dwFileSize,
            (uint16_t)dwContentLen));
    }
    else if(bFileType == PHCE_T4T_FILE_NDEF)
    {
        PH_CHECK_SUCCESS_FCT(status, phceT4T_Sw_Int_SetNdefFile(
            pDataParams,
            pFile,
            wFileId,
            dwFileSize,
            dwContentLen));
    }

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    else if(bFileType == PHCE_T4T_FILE_PROPRIETARY)
    {
        PH_CHECK_SUCCESS_FCT(status, phceT4T_Sw_Int_SetProprietaryFile(
            pDataParams,
            pFile,
            wFileId,
            (uint16_t)dwFileSize,
            (uint16_t)dwContentLen));
    }
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

    else
    {
        /* Unsupported elementary file type */
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phceT4T_Sw_ProcessCmd(
                                 phceT4T_Sw_DataParams_t *pDataParams,
                                 uint16_t wOption,
                                 uint8_t *pRxData,
                                 uint16_t wRxDataLen,
                                 uint16_t *pStatusWord,
                                 uint8_t **ppTxData,
                                 uint16_t *pTxDataLen
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Assign the default out Parameters */
    *ppTxData = NULL;
    *pTxDataLen = 0;
    *pStatusWord = PHCE_T4T_ISO7816_NO_PRECISE_DIAGNOSIS;
    pDataParams->wStatusWord = *pStatusWord;

    /* Check State */
    if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_INCOMPATIBLE_FILE_STRUCT;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    /* Validate input data buffer */
    if(pRxData == NULL)
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
        *pStatusWord = pDataParams->wStatusWord;
        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    switch(wOption)
    {
    case PHCE_T4T_RXDEFAULT:
    case PHCE_T4T_RXCHAINING_BUFFER_FIRST:
    {
        /* Validate input data length */
        if((wRxDataLen < 4U))
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
            *pStatusWord = pDataParams->wStatusWord;
            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }

        /* Check for the Class Byte */
        if(pRxData[0] != 0x00U)
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_CLASS;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }

        /* Check for Instruction Byte */
        switch(pRxData[1])
        {
        case PHCE_T4T_INS_SELECT:
            /* Select file based on file ID */
            status = phceT4T_Sw_Int_Select(
                pDataParams,
                pRxData,
                wRxDataLen,
                pStatusWord);
            break;

        case PHCE_T4T_INS_READ:
            /* Read from file */
            status = phceT4T_Sw_Int_ReadBinary(
                pDataParams,
                pRxData,
                wRxDataLen,
                pStatusWord,
                ppTxData,
                pTxDataLen);
            break;

        case PHCE_T4T_INS_UPDATE:
            /* Update file */
            status = phceT4T_Sw_Int_UpdateBinary(
                pDataParams,
                wOption,
                pRxData,
                wRxDataLen,
                pStatusWord,
                ppTxData,
                pTxDataLen);
            break;

        default:
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_INSTRUCTION;
            *pStatusWord = pDataParams->wStatusWord;
            *ppTxData = pRxData;
            *pTxDataLen = wRxDataLen;

            status = (PH_CE_T4T_PROPRIETARY | PH_COMP_CE_T4T);
            break;
        }
    }
    break;

    case PHCE_T4T_RXCHAINING_BUFFER_CONT:
        if(pDataParams->bTagState == PHCE_T4T_STATE_FILE_UPDATE)
        {
            /* Check for the length error */
            if(!((wRxDataLen) <= pDataParams->wLc))
            {
                /* Change error code to length error */
                pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
                *pStatusWord = pDataParams->wStatusWord;
                return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
            }

            /* Update received data count */
            pDataParams->wLc = pDataParams->wLc - wRxDataLen;

            /* Update file if configured to */
            if(pDataParams->bHandleUpdateCmd == PH_ON)
            {
                phceT4T_Sw_Int_UpdateFile(
                    pDataParams,
                    pRxData,
                    wRxDataLen);
            }
            else
            {
                *ppTxData = pRxData;
                *pTxDataLen = wRxDataLen;
            }

            /* Change Status to Success */
            pDataParams->bTagState = PHCE_T4T_STATE_FILE_UPDATE;
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
            *pStatusWord = pDataParams->wStatusWord;

            status = (PH_CE_T4T_UPDATE_BINARY | PH_COMP_CE_T4T);
        }
        else
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_INSTRUCTION;
            *pStatusWord = pDataParams->wStatusWord;
            *ppTxData = pRxData;
            *pTxDataLen = wRxDataLen;

            status = (PH_CE_T4T_PROPRIETARY | PH_COMP_CE_T4T);
        }
        break;

    case PHCE_T4T_RXCHAINING_BUFFER_LAST:
        if(pDataParams->bTagState == PHCE_T4T_STATE_FILE_UPDATE)
        {
            /* Check for the length error */
            if((wRxDataLen) != pDataParams->wLc)
            {
                /* Change error code to length error */
                pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
                *pStatusWord = pDataParams->wStatusWord;
                return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
            }

            pDataParams->wLc = pDataParams->wLc - wRxDataLen;

            /* Update file if configured to */
            if(pDataParams->bHandleUpdateCmd == PH_ON)
            {
                phceT4T_Sw_Int_UpdateFile(
                    pDataParams,
                    pRxData,
                    wRxDataLen);
            }
            else
            {
                *ppTxData = pRxData;
                *pTxDataLen = wRxDataLen;
            }

            /* Change Status to Success */
            pDataParams->bTagState = PHCE_T4T_STATE_FILE_UPDATE;
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
            *pStatusWord = pDataParams->wStatusWord;

            status = (PH_CE_T4T_UPDATE_BINARY | PH_COMP_CE_T4T);
        }
        else
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_INSTRUCTION;
            *pStatusWord = pDataParams->wStatusWord;
            *ppTxData = pRxData;
            *pTxDataLen = wRxDataLen;

            status = (PH_CE_T4T_PROPRIETARY | PH_COMP_CE_T4T);
        }
        break;

    default:
        status = (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        break;
    }

    return (status);
}

phStatus_t phceT4T_Sw_Activate(
                               phceT4T_Sw_DataParams_t *pDataParams
                               )
{
#ifndef _WIN32
    phStatus_t PH_MEMLOC_REM status = 0;

    /* Call the actual activate implementation */
    status = phceT4T_Sw_Int_Activate(pDataParams);

    /* Update exit status */
    pDataParams->wExitStatus = status;

    /* Set RX Data available event with error details */
    phOsal_EventPost(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_ERROR, NULL);

    /* Return error */
    return status;
#endif /* _WIN32 */
#ifdef _WIN32
    PH_UNUSED_VARIABLE(pDataParams);
    return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_CE_T4T);
#endif /* _WIN32 */
}

phStatus_t phceT4T_Sw_AppProcessCmd(
                                    phceT4T_Sw_DataParams_t *pDataParams,
                                    phceT4T_AppCallback_t pAppCallback
                                    )
{
#ifndef _WIN32
    phStatus_t           PH_MEMLOC_REM status;
    phOsal_EventBits_t   PH_MEMLOC_REM events;
    uint8_t              PH_MEMLOC_BUF *pTxData;
    uint16_t             PH_MEMLOC_REM wTxDataLen = 0;
    uint16_t             PH_MEMLOC_REM wStatusWord;
    uint8_t              PH_MEMLOC_REM bDataOffset = 0;
    uint8_t              PH_MEMLOC_REM bExitLoop = FALSE;

    /* Loop till aborted, DESELECTED or error */
    do
    {
        /* Wait for RX Data */
        PH_CHECK_SUCCESS_FCT(status, phOsal_EventPend((volatile phOsal_Event_t * )(&pDataParams->T4TEventObj.EventHandle), E_OS_EVENT_OPT_PEND_SET_ANY, 0xFFFFFFFFU,
            (E_PH_OSAL_EVT_RXDATA_AVAILABLE | E_PH_OSAL_EVT_ABORT | E_PH_OSAL_EVT_ERROR), &events));

        if((events & E_PH_OSAL_EVT_ERROR) == E_PH_OSAL_EVT_ERROR)
        {
            /* Clear event */
            PH_CHECK_SUCCESS_FCT(status,
                phOsal_EventClear(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_ERROR, NULL));

            /* Exit on error */
            status = pDataParams->wExitStatus;

            /* Exit loop */
            break;
        }
        else if((events & E_PH_OSAL_EVT_RXDATA_AVAILABLE) == E_PH_OSAL_EVT_RXDATA_AVAILABLE)
        {
            /* Clear event */
            PH_CHECK_SUCCESS_FCT(status,
                phOsal_EventClear(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RXDATA_AVAILABLE, NULL));

            /* For UPDATE BINARY send only the data to be updated, not whole C-APDU */
            if(pDataParams->bTagState == PHCE_T4T_STATE_FILE_UPDATE)
            {
                /* Update the data offset */
                if((pDataParams->bRxOption == PHCE_T4T_RXCHAINING_BUFFER_CONT)
                   || (pDataParams->bRxOption == PHCE_T4T_RXCHAINING_BUFFER_LAST))
                {
                    bDataOffset = 0;
                }
                else
                {
                    if(pDataParams->wLc > 0xFFU)
                    {
                        bDataOffset = 7;
                    }
                    else
                    {
                        bDataOffset = 5;
                    }
                }
            }
            else
            {
                bDataOffset = 0;
            }

            /* If application callback is not provided, handle internally. */
            if(pAppCallback == NULL)
            {
                /* Update file with RX Data */
                if(pDataParams->bTagState == PHCE_T4T_STATE_FILE_UPDATE)
                {
                    phceT4T_Sw_Int_UpdateFile(
                        pDataParams,
                        &pDataParams->pSharedData[bDataOffset],
                        (pDataParams->wSharedDataLen - bDataOffset));

                    /* For UPDATE BINARY, there is no TX data. */
                    pDataParams->pSharedData = NULL;
                    pDataParams->wSharedDataLen = 0;

                    /* Update status word */
                    pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
                }
                else
                {
                    /* For proprietary command send only Status Word */
                    pDataParams->pSharedData = NULL;
                    pDataParams->wSharedDataLen = 0;

                    /* Update status word */
                    pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_INSTRUCTION;
                }
            }
            else
            {
                /* Call application callback */
                status = pAppCallback(
                    pDataParams->bTagState,
                    pDataParams->bRxOption,
                    &pDataParams->pSharedData[bDataOffset],
                    (pDataParams->wSharedDataLen - bDataOffset),
                    &wStatusWord,
                    &pTxData,
                    &wTxDataLen);

                if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    /* Update file offset (for proprietary, callback shall handle) */
                    if(pDataParams->bTagState == PHCE_T4T_STATE_FILE_UPDATE)
                    {
                        pDataParams->dwFileOffset += (pDataParams->wSharedDataLen - bDataOffset);
                    }

                    /* Update TX Data */
                    pDataParams->pSharedData = pTxData;
                    pDataParams->wSharedDataLen = wTxDataLen;

                    /* Update status word */
                    pDataParams->wStatusWord = wStatusWord;
                }
                else
                {
                    /* Update TX Data */
                    pDataParams->pSharedData = NULL;
                    pDataParams->wSharedDataLen = 0;

                    /* Update status word */
                    pDataParams->wStatusWord = wStatusWord;
                }
            }

            /* Set TX Data available event */
            PH_CHECK_SUCCESS_FCT(status,
                phOsal_EventPost(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_TXDATA_AVAILABLE, NULL));
        }
        else
        {
            /* Aborted */
            status = (PH_ERR_ABORTED | PH_COMP_CE_T4T);

            /* Exit loop */
            bExitLoop = TRUE;
        }
    }
    while(bExitLoop == FALSE);

    return status;
#endif /* _WIN32 */
#ifdef _WIN32
    PH_UNUSED_VARIABLE(pDataParams);
    PH_UNUSED_VARIABLE(pAppCallback);
    return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_CE_T4T);
#endif /* _WIN32 */
}

phStatus_t phceT4T_Sw_GetSelectedFileInfo(
                                          phceT4T_Sw_DataParams_t *pDataParams,
                                          uint16_t *pFileId,
                                          uint8_t **ppFile,
                                          uint32_t *pFileSize,
                                          uint8_t *pWriteAccess,
                                          uint32_t *pFileOffset
                                          )
{
    if(pDataParams->bTagState >= PHCE_T4T_STATE_FILE_SELECTED)
    {
        *pFileId      = pDataParams->wSelectedFileId;
        *ppFile       = pDataParams->pSelectedFile;
        *pFileSize    = pDataParams->dwFileSize;
        *pWriteAccess = pDataParams->bFileWriteAccess;
        *pFileOffset  = pDataParams->dwFileOffset;
    }
    else
    {
        return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phceT4T_Sw_SetConfig(
                                phceT4T_Sw_DataParams_t *pDataParams,
                                uint16_t wConfig,
                                uint16_t wValue
                                )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    switch(wConfig)
    {
    case PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        pDataParams->bHandleUpdateCmd = (uint8_t)wValue;
        break;

    case PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        pDataParams->bSupportProprietaryCmd = (uint8_t)wValue;
        break;

    case PHCE_T4T_CONFIG_NDEF_FILE_ID:
        if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
        {
           return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        /* Validate file ID */
        if((wValue == 0x0000U) || (wValue == 0xE102U) || (wValue == 0xE103U)
           || (wValue == 0x3F00U) || (wValue == 0x3FFFU) || (wValue == 0xFFFFU))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        pDataParams->pCcFile[9]  = (uint8_t)((wValue & 0xFF00U) >> 8U);
        pDataParams->pCcFile[10] = (uint8_t)(wValue & 0xFFU);
        break;

    case PHCE_T4T_CONFIG_NDEF_FILE_READ_ACCESS:
        if(wValue != 0x00U)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        if(pDataParams->pCcFile[2] == 0x20U)
        {
            pDataParams->pCcFile[13] = (uint8_t)wValue;
        }
#ifdef NXPBUILD__PHCE_T4T_EXT_NDEF
        else
        {
            pDataParams->pCcFile[15] = (uint8_t)wValue;
        }
#endif /* NXPBUILD__PHCE_T4T_EXT_NDEF */
        break;

    case PHCE_T4T_CONFIG_NDEF_FILE_WRITE_ACCESS:
        if((wValue != 0x00U) && (wValue != 0xFFU))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        if(pDataParams->pCcFile[2] == 0x20U)
        {
             pDataParams->pCcFile[14] = (uint8_t)wValue;
        }
#ifdef NXPBUILD__PHCE_T4T_EXT_NDEF
        else
        {
            pDataParams->pCcFile[16] = (uint8_t)wValue;
        }
#endif /* NXPBUILD__PHCE_T4T_EXT_NDEF */
        break;

    case PHCE_T4T_CONFIG_MLE:
        if(wValue < 0x000FU)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        if(pDataParams->bTagState < PHCE_T4T_STATE_CC_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        pDataParams->pCcFile[3] = (uint8_t)((wValue & 0xFF00U) >> 8U);
        pDataParams->pCcFile[4] = (uint8_t)(wValue & 0xFFU);
        break;

    case PHCE_T4T_CONFIG_MLC:
        if(wValue == 0x00U)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
        if(pDataParams->bTagState < PHCE_T4T_STATE_CC_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        pDataParams->pCcFile[5] = (uint8_t)((wValue & 0xFF00U) >> 8U);
        pDataParams->pCcFile[6] = (uint8_t)(wValue & 0xFFU);
        break;

    default:
        status = (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        break;
    }

    return status;
}

phStatus_t phceT4T_Sw_GetConfig(
                                phceT4T_Sw_DataParams_t *pDataParams,
                                uint16_t wConfig,
                                uint16_t *pValue
                                )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    switch(wConfig)
    {
    case PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY:
        *pValue = pDataParams->bHandleUpdateCmd;
        break;

    case PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY:
        *pValue = pDataParams->bSupportProprietaryCmd;
        break;

    case PHCE_T4T_CONFIG_NDEF_FILE_ID:
        if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        *pValue = (uint16_t)((((uint16_t)pDataParams->pCcFile[9] << 8U) & 0xFF00U)
                  | (pDataParams->pCcFile[10]));
        break;

    case PHCE_T4T_CONFIG_NDEF_FILE_READ_ACCESS:
        if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        if(pDataParams->pCcFile[2] == 0x20U)
        {
            *pValue = pDataParams->pCcFile[13];
        }
#ifdef NXPBUILD__PHCE_T4T_EXT_NDEF
        else
        {
             *pValue = pDataParams->pCcFile[15];
        }
#endif /* NXPBUILD__PHCE_T4T_EXT_NDEF */
        break;

    case PHCE_T4T_CONFIG_NDEF_FILE_WRITE_ACCESS:
        if(pDataParams->bTagState < PHCE_T4T_STATE_NDEF_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        if(pDataParams->pCcFile[2] == 0x20U)
        {
            *pValue = pDataParams->pCcFile[14];
        }
#ifdef NXPBUILD__PHCE_T4T_EXT_NDEF
        else
        {
            *pValue = pDataParams->pCcFile[16];
        }
#endif /* NXPBUILD__PHCE_T4T_EXT_NDEF */
        break;

    case PHCE_T4T_CONFIG_MLE:
        if(pDataParams->bTagState < PHCE_T4T_STATE_CC_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        *pValue = (uint16_t)((((uint16_t)pDataParams->pCcFile[3] << 8U) & 0xFF00U)
                  | (pDataParams->pCcFile[4]));
        break;

    case PHCE_T4T_CONFIG_MLC:
        if(pDataParams->bTagState < PHCE_T4T_STATE_CC_FILE_CREATED)
        {
            return (PH_CE_T4T_INVALID_STATE | PH_COMP_CE_T4T);
        }
        *pValue = (uint16_t)((((uint16_t)pDataParams->pCcFile[5] << 8U) & 0xFF00U)
                  | (pDataParams->pCcFile[6]));
        break;

    default:
        status = (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        break;
    }

    return status;
}

#endif /* NXPBUILD__PHCE_T4T_SW */
