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

phStatus_t phceT4T_Sw_Int_SetCcFile(
                                    phceT4T_Sw_DataParams_t *pDataParams,
                                    uint8_t *pCcFile,
                                    uint16_t wCcFileId,
                                    uint16_t wCcFileSize,
                                    uint16_t wCcContentLen
                                    )
{
#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    uint16_t PH_MEMLOC_COUNT wIndex;
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

    /* Check current state */
    if(pDataParams->bTagState != PHCE_T4T_STATE_NONE)
    {
        return (PH_ERR_USE_CONDITION | PH_COMP_CE_T4T);
    }

    /* Check for CC file size */
    if((wCcFileSize < 15U) || (wCcFileSize == 0xFFFFU))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Content length shall not be more than file size */
    if(wCcFileSize < wCcContentLen)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Validate file ID */
    if(wCcFileId != PHCE_T4T_CC_FILE_ID)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Validate content length */
    if((wCcContentLen > 0U) && (wCcContentLen < 15U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Initialize CC file with default values, if content length is '0' */
    if(wCcContentLen == 0U)
    {
        /* CC Length */
        pCcFile[0] = 0x00;
        pCcFile[1] = 0x0F;

        /* Mapping Version */
        pCcFile[2] = 0x20;

        /* MLe */
        pCcFile[3] = 0x00;
        pCcFile[4] = 0xFF;

        /* MLc */
        pCcFile[5] = 0x00;
        pCcFile[6] = 0xFF;

        /* Tag (T) of Default NDEF TLV */
        pCcFile[7]  = 0x04;

        /* Length (L) of Default NDEF TLV */
        pCcFile[8]  = 0x06;

        /* NDEF File ID */
        pCcFile[9]  = 0xE1;
        pCcFile[10] = 0x04;

        /* NDEF File Size */
        pCcFile[11] = 0x00;
        pCcFile[12] = 0x00;

        /* Read access */
        pCcFile[13] = 0x00;

        /* Write access */
        pCcFile[14] = 0x00;

        /* Update content length */
        wCcContentLen = 15;
    }
    else
    {
        /* Validate content of CCLEN field with provided CC content length */
        if((((uint16_t)pCcFile[0] << 8U ) | pCcFile[1]) != wCcContentLen)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate supported version numbers */
        if((pCcFile[2] != 0x30U) && (pCcFile[2] != 0x20U))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate content length and version number */
        if((pCcFile[2] == 0x30U) && (wCcContentLen < 17U))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Check for minimum MLe */
        if(((((uint16_t)pCcFile[3] << 8U) & 0xFF00U) | pCcFile[4]) < 0x0FU)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Check for minimum MLc */
        if(((((uint16_t)pCcFile[5] << 8U) & 0xFF00U) | pCcFile[6]) < 0x01U)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate NDEF file ID */
        if((((((uint16_t)pCcFile[9] << 8U) & 0xFF00U) | (pCcFile[10])) == 0x0000U)
           || (((((uint16_t)pCcFile[9] << 8U) & 0xFF00U) | (pCcFile[10])) == 0xE102U)
           || (((((uint16_t)pCcFile[9] << 8U) & 0xFF00U) | (pCcFile[10])) == 0xE103U)
           || (((((uint16_t)pCcFile[9] << 8U) & 0xFF00U) | (pCcFile[10]))== 0x3F00U)
           || (((((uint16_t)pCcFile[9] << 8U) & 0xFF00U) | (pCcFile[10])) == 0x3FFFU)
           || (((((uint16_t)pCcFile[9] << 8U) & 0xFF00U) | (pCcFile[10])) == 0xFFFFU))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate NDEF TLV and NDEF file size (5 - 0xFFFE) */
        if((pCcFile[2] == 0x20U) && ((pCcFile[7] != 0x04U) || (pCcFile[8] != 0x06U)
           || ((((((uint16_t)pCcFile[11]) << 8U) & 0xFF00U) | pCcFile[12]) < 0x05U)
           || ((((((uint16_t)pCcFile[11]) << 8U) & 0xFF00U) | pCcFile[12]) == 0xFFFFU)))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
#ifdef NXPBUILD__PHCE_T4T_EXT_NDEF
        /* Validate Extended NDEF TLV and NDEF file size (0xFFFF - 0xFFFFFFFE) */
        if((pCcFile[2] == 0x30U) && ((pCcFile[7] != 0x06U) || (pCcFile[8] != 0x08U)
           || (((((uint32_t)pCcFile[11] << 24U) & 0xFF000000U)
              | (((uint32_t)pCcFile[12] << 16U) & 0xFF0000U)
              | (((uint32_t)pCcFile[13] << 8U) & 0xFF00U) | pCcFile[14]) < 0xFFFFU)
           || (((((uint32_t)pCcFile[11] << 24U) & 0xFF000000U)
              | (((uint32_t)pCcFile[12] << 16U) & 0xFF0000U)
              | (((uint32_t)pCcFile[13] << 8U) & 0xFF00U) | pCcFile[14]) == 0xFFFFFFFFU)))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
#endif /* NXPBUILD__PHCE_T4T_EXT_NDEF */
        /* Validate NDEF read access */
        if(((pCcFile[2] == 0x20U) && (pCcFile[13] != 0x00U))
           || ((pCcFile[2] == 0x30U) && (pCcFile[15] != 0x00U)))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate NDEF write access */
        if(((pCcFile[2] == 0x20U) && (pCcFile[14] != 0x00U) && (pCcFile[14] != 0xFFU))
           || ((pCcFile[2] == 0x30U) && (pCcFile[16] != 0x00U) && (pCcFile[16] != 0xFFU)))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
    }

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    /* Check NDEF type to get content length till NDEF TLV end */
    if(pCcFile[2] == 0x20U)
    {
        wIndex = 15;
    }
    else
    {
        wIndex = 17;
    }

    /* Validate Proprietary TLVs length */
    if(((wCcContentLen - wIndex) % 8U) != 0U)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Validate Proprietary TLV content */
    while(wCcContentLen > wIndex)
    {
        /* Validate T and L field of proprietary TLV */
        if((pCcFile[wIndex] != 0x05U) || (pCcFile[wIndex + 1U] != 0x06U))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate file ID */
        if((((((uint16_t)pCcFile[wIndex + 2U] << 8U) & 0xFF00U) | (pCcFile[wIndex + 3U])) == 0x0000U)
           || (((((uint16_t)pCcFile[wIndex + 2U] << 8U) & 0xFF00U) | (pCcFile[wIndex + 3U])) == 0xE102U)
           || (((((uint16_t)pCcFile[wIndex + 2U] << 8U) & 0xFF00U) | (pCcFile[wIndex + 3U])) == 0xE103U)
           || (((((uint16_t)pCcFile[wIndex + 2U] << 8U) & 0xFF00U) | (pCcFile[wIndex + 3U]))== 0x3F00U)
           || (((((uint16_t)pCcFile[wIndex + 2U] << 8U) & 0xFF00U) | (pCcFile[wIndex + 3U])) == 0x3FFFU)
           || (((((uint16_t)pCcFile[wIndex + 2U] << 8U) & 0xFF00U) | (pCcFile[wIndex + 3U])) == 0xFFFFU))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate minimum file size */
        if((((((uint16_t)pCcFile[wIndex + 4U]) << 8U) & 0xFF00U) | pCcFile[wIndex + 5U]) < 0x03U)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate read access */
        if(pCcFile[wIndex + 6U] != 0x00U)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Validate write access */
        if((pCcFile[wIndex + 7U] != 0x00U) && (pCcFile[wIndex + 7U] != 0xFFU))
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Increment Index */
        wIndex += 8U;
    }
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */
#if !defined(NXPBUILD__PHCE_T4T_PROPRIETARY)
    /* If only NDEF TLV is present, content length shall be 15 or 17 */
    if((wCcContentLen != 15U) && (wCcContentLen != 17U))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

    pDataParams->pCcFile = pCcFile;
    pDataParams->wCcFileSize = wCcFileSize;
    pDataParams->wCcContentLen = wCcContentLen;

    /* Set '0' for remaining file content */
    (void)memset(&pCcFile[wCcContentLen], 0, (wCcFileSize - wCcContentLen));

    /* Update state */
    pDataParams->bTagState = PHCE_T4T_STATE_CC_FILE_CREATED;

    return PH_ERR_SUCCESS;
}

phStatus_t phceT4T_Sw_Int_SetNdefFile(
                                      phceT4T_Sw_DataParams_t *pDataParams,
                                      uint8_t *pNdefFile,
                                      uint16_t wNdefFileId,
                                      uint32_t dwNdefFileSize,
                                      uint32_t dwNdefContentLen
                                      )
{
    /* Check current state */
    if(pDataParams->bTagState != PHCE_T4T_STATE_CC_FILE_CREATED)
    {
        return (PH_ERR_USE_CONDITION | PH_COMP_CE_T4T);
    }

    /* Check for NDEF file size */
    if((dwNdefFileSize < 5U) || (dwNdefFileSize == 0xFFFFFFFFU))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Content length shall not be more than file size */
    if(dwNdefFileSize < dwNdefContentLen)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Validate file ID */
    if((wNdefFileId == 0x0000U) || (wNdefFileId == 0xE102U) || (wNdefFileId == 0xE103U)
       || (wNdefFileId == 0x3F00U) || (wNdefFileId == 0x3FFFU) || (wNdefFileId == 0xFFFFU))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Set NDEF file TLV in CC file */
    if(dwNdefFileSize < 0xFFFEU)
    {

        /* Reset CC content if SetCcfile content and SetNdefFile content are not
         * matching in NDEF TLV type / T4T version */
        if(pDataParams->pCcFile[2] != 0x20U)
        {
            /* CC Length */
            pDataParams->pCcFile[0] = 0x00;
            pDataParams->pCcFile[1] = 0x0F;

            /* Mapping Version */
            pDataParams->pCcFile[2] = 0x20;

            /* Tag field of TLV */
            pDataParams->pCcFile[7]  = 0x04;
            /* Length field of TLV */
            pDataParams->pCcFile[8]  = 0x06;

            /* Read access */
            pDataParams->pCcFile[13] = 0x00;
            /* Write access */
            pDataParams->pCcFile[14] = 0x00;
        }

        /* NDEF File Size */
        pDataParams->pCcFile[11] = (uint8_t)((dwNdefFileSize & 0xFF00U) >> 8U);
        pDataParams->pCcFile[12] = (uint8_t)(dwNdefFileSize & 0xFFU);

    }
#ifdef NXPBUILD__PHCE_T4T_EXT_NDEF
    else
    {
        if(pDataParams->wCcFileSize < 17U)
        {
            /* Extended NDEF TLV not supported with current CC file size */
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }

        /* Reset CC content if SetCcfile content and SetNdefFile content are not
         * matching in NDEF TLV type / T4T version */
        if(pDataParams->pCcFile[2] != 0x30U)
        {
            /* CC Length */
            pDataParams->pCcFile[0] = 0x00;
            pDataParams->pCcFile[1] = 0x11;

            /* Mapping Version */
            pDataParams->pCcFile[2] = 0x30;

            /* Tag field of TLV */
            pDataParams->pCcFile[7]  = 0x06;
            /* Length field of TLV */
            pDataParams->pCcFile[8]  = 0x08;

            /* Read access */
            pDataParams->pCcFile[15] = 0x00;
            /* Write access */
            pDataParams->pCcFile[16] = 0x00;
        }

        /* NDEF File Size */
        pDataParams->pCcFile[11] = (uint8_t)((dwNdefFileSize & 0xFF000000U) >> 24U);
        pDataParams->pCcFile[12] = (uint8_t)((dwNdefFileSize & 0xFF0000U) >> 16U);
        pDataParams->pCcFile[13] = (uint8_t)((dwNdefFileSize & 0xFF00U) >> 8U);
        pDataParams->pCcFile[14] = (uint8_t)(dwNdefFileSize & 0xFFU);
    }
#endif /* NXPBUILD__PHCE_T4T_EXT_NDEF */

    /* Update file ID in CC */
    pDataParams->pCcFile[9]  = (uint8_t)((wNdefFileId & 0xFF00U) >> 8U);
    pDataParams->pCcFile[10] = (uint8_t)(wNdefFileId & 0xFFU);

    pDataParams->pNdefFile = pNdefFile;
    pDataParams->dwNdefFileSize = dwNdefFileSize;

    /* Set '0' for remaining file content */
    (void)memset(&pNdefFile[dwNdefContentLen], 0, (dwNdefFileSize - dwNdefContentLen));

    /* Update state */
    pDataParams->bTagState = PHCE_T4T_STATE_NDEF_FILE_CREATED;

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
phStatus_t phceT4T_Sw_Int_SetProprietaryFile(
                                             phceT4T_Sw_DataParams_t *pDataParams,
                                             uint8_t *pFile,
                                             uint16_t wFileId,
                                             uint16_t wFileSize,
                                             uint16_t wContentLen
                                             )
{
    uint16_t PH_MEMLOC_COUNT wIndex;
    uint8_t  PH_MEMLOC_COUNT bCount;

    /* Check current state */
    if(pDataParams->bTagState != PHCE_T4T_STATE_NDEF_FILE_CREATED)
    {
        return (PH_ERR_USE_CONDITION | PH_COMP_CE_T4T);
    }

    /* Check for file size */
    if((wFileSize < 3U) || (wFileSize == 0xFFFFU))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Content length shall not be more than file size */
    if(wFileSize < wContentLen)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Check proprietary file count */
    if(pDataParams->bProprietaryFileCount == PHCE_T4T_MAX_PROPRIETARY_FILE)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Validate with reserved file ID */
    if((wFileId == 0x0000U) || (wFileId == 0xE102U) || (wFileId == 0xE103U)
       || (wFileId == 0x3F00U) || (wFileId == 0x3FFFU) || (wFileId == 0xFFFFU))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Check with the NDEF file ID */
    if((((((uint16_t)pDataParams->pCcFile[9] << 8U) & 0xFF00U)
       | (pDataParams->pCcFile[10])) == wFileId))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Check NDEF version to get CC content length till NDEF TLV end */
    if(pDataParams->pCcFile[2] == 0x20U)
    {
        wIndex = 15;
    }
    else
    {
        wIndex = 17;
    }

    /* Check file ID with already created proprietary file IDs */
    for(bCount = 0; bCount < pDataParams->bProprietaryFileCount; bCount++)
    {
        if(((((uint16_t)pDataParams->pCcFile[wIndex + (bCount * 8U) + 2U] << 8U) & 0xFF00U)
           | (pDataParams->pCcFile[wIndex + (bCount * 8U) + 3U])) == wFileId)
        {
            return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
        }
    }

    /* Increment CC index based on current proprietary file count */
    wIndex += pDataParams->bProprietaryFileCount * 8U;

    /* Check if CC file can hold one more TLV */
    if((wIndex + 8U) > pDataParams->wCcFileSize)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_CE_T4T);
    }

    /* Check if this file was specified in CC */
    if((pDataParams->wCcContentLen <= wIndex)
       || (pDataParams->pCcFile[wIndex + 2U] != (uint8_t)((wFileId & 0xFF00U) >> 8U))
       || (pDataParams->pCcFile[wIndex + 3U] != (uint8_t)(wFileId & 0xFFU)))
    {
        /* Tag */
        pDataParams->pCcFile[wIndex] = 0x05;
        /* Length */
        pDataParams->pCcFile[wIndex + 1U] = 0x06;

        /* File ID */
        pDataParams->pCcFile[wIndex + 2U] = (uint8_t)((wFileId & 0xFF00U) >> 8U);
        pDataParams->pCcFile[wIndex + 3U] = (uint8_t)(wFileId & 0xFFU);

        /* Read access */
        pDataParams->pCcFile[wIndex + 6U] = 0x00;
        /* Write access */
        pDataParams->pCcFile[wIndex + 7U] = 0x00;
    }

    /* File size */
    pDataParams->pCcFile[wIndex + 4U] = (uint8_t)((wFileSize & 0xFF00U) >> 8U);
    pDataParams->pCcFile[wIndex + 5U] = (uint8_t)(wFileSize & 0xFFU);

    /* Update index */
    wIndex += 8U;

    /* Update CC Length to indicate content length till this TLV */
    pDataParams->pCcFile[0] = (uint8_t)((wIndex & 0xFF00U) >> 8U);
    pDataParams->pCcFile[1] = (uint8_t)(wIndex & 0xFFU);

    /* Update proprietary file parameters */
    pDataParams->asProprietaryFile[pDataParams->bProprietaryFileCount].pFile = pFile;
    pDataParams->asProprietaryFile[pDataParams->bProprietaryFileCount].wFileSize = wFileSize;
    pDataParams->asProprietaryFile[pDataParams->bProprietaryFileCount].wContentenLen = wContentLen;

    /* Set '0' for remaining file content */
    (void)memset(&pFile[wContentLen], 0, (wFileSize - wContentLen));

    /* Increment file count */
    pDataParams->bProprietaryFileCount++;

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

phStatus_t phceT4T_Sw_Int_Select(
                                 phceT4T_Sw_DataParams_t *pDataParams,
                                 uint8_t *pRxData,
                                 uint16_t wRxDataLen,
                                 uint16_t *pStatusWord
                                 )
{
#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    uint16_t PH_MEMLOC_COUNT wIndex;
    uint8_t  PH_MEMLOC_COUNT bCount;
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */
    uint16_t PH_MEMLOC_REM   wFileId;
    uint8_t  PH_MEMLOC_BUF   bNdefAppName[7] = {0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01};

    /* Select file/application by name */
    if((pRxData[2] == 0x04U) && (pRxData[3] == 0x00U))
    {
        /* Check the command length and Lc for NDEF APP Select */
        if((wRxDataLen < 0x0DU) || (pRxData[4] != 0x07U))
        {
            /* Length of the command is invalid */
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }

        /* Validate NDEF Tag application name. */
        if(memcmp(&pRxData[5], bNdefAppName, 7))
        {
            /* Application name mismatch */
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_FILE_NOT_FOUND;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }
        else
        {
            /* NDEF Tag application Selected */
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
            *pStatusWord = pDataParams->wStatusWord;
            pDataParams->bTagState = PHCE_T4T_STATE_NDEF_APP_SELECTED;

            return (PH_CE_T4T_SELECT | PH_COMP_CE_T4T);
        }
    }
    /* Select file by ID */
    else if((pRxData[2] == 0x00U) && (pRxData[3] == 0x0CU))
    {
        if (pDataParams->bTagState < PHCE_T4T_STATE_NDEF_APP_SELECTED)
        {
            /* Files are present only in NDEF APP. */
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_FILE_NOT_FOUND;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }

        /* Check the length of select command and the Lc byte is 0x02. */
        if ((wRxDataLen > 0x07U) || ((pRxData[4] != 0x02U)))
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_FILE_NOT_FOUND;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }

        /* File ID to be selected */
        wFileId = (uint16_t)((((uint16_t)(pRxData[5])) << 8U) & 0xFF00U) | ((uint16_t)(pRxData[6]));

        if(wFileId == PHCE_T4T_CC_FILE_ID)
        {
            pDataParams->wSelectedFileId = wFileId;
            pDataParams->pSelectedFile = pDataParams->pCcFile;
            pDataParams->dwFileSize = pDataParams->wCcFileSize;
            pDataParams->bFileWriteAccess = 0xFF;
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
            pDataParams->bTagState = PHCE_T4T_STATE_FILE_SELECTED;

            return (PH_CE_T4T_SELECT | PH_COMP_CE_T4T);
        }
        else if(wFileId == (((((uint16_t)(pDataParams->pCcFile[9])) << 8U) & 0xFF00U)
                | (((uint16_t)(pDataParams->pCcFile[10])))))
        {
            pDataParams->wSelectedFileId = wFileId;
            pDataParams->pSelectedFile = pDataParams->pNdefFile;
            pDataParams->dwFileSize = pDataParams->dwNdefFileSize;

            if(pDataParams->pCcFile[2] == 0x20U)
            {
                /* NDEF File Write access */
                pDataParams->bFileWriteAccess = pDataParams->pCcFile[14];
            }
            else
            {
                /* Extended NDEF Writ access */
                pDataParams->bFileWriteAccess = pDataParams->pCcFile[16];
            }

            pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
            pDataParams->bTagState = PHCE_T4T_STATE_FILE_SELECTED;

            /* File selected */
            return (PH_CE_T4T_SELECT | PH_COMP_CE_T4T);
        }

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
        else if(0U != (pDataParams->bProprietaryFileCount))
        {
            if(pDataParams->pCcFile[2] == 0x20U)
            {
                /* Index of the first Proprietary TLV in version 2.0 */
                wIndex = 17;
            }
            else
            {
                /* Index of the first Proprietary TLV in version 3 .0 */
                wIndex = 19;
            }

            /* Check file ID exists in the application */
            for(bCount = 0; bCount < pDataParams->bProprietaryFileCount; bCount++)
            {
                if((((((uint16_t)pDataParams->pCcFile[wIndex + (bCount * 8U)]) << 8U) & 0xFF00U)
                        | (pDataParams->pCcFile[wIndex + (bCount * 8U) + 1U])) == wFileId)
                {
                    /* Proprietary file Selected */
                    pDataParams->wSelectedFileId = wFileId;
                    pDataParams->pSelectedFile = pDataParams->asProprietaryFile[bCount].pFile;
                    pDataParams->dwFileSize = pDataParams->asProprietaryFile[bCount].wFileSize;
                    pDataParams->bFileWriteAccess = (pDataParams->pCcFile[wIndex + (bCount * 8U) + 5U]);
                    pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
                    pDataParams->bTagState = PHCE_T4T_STATE_FILE_SELECTED;

                    /* File selected */
                    return (PH_CE_T4T_SELECT | PH_COMP_CE_T4T);
                }
            }

            /* File ID to be selected does not exist among proprietary files */
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_FILE_NOT_FOUND;
            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */
        else
        {
            /* Requested fileID to be selected does not exist */
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_FILE_NOT_FOUND;
            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }
    }
    else
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_PARAMETER;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }
}

phStatus_t phceT4T_Sw_Int_ReadBinary(
                                     phceT4T_Sw_DataParams_t *pDataParams,
                                     uint8_t *pRxData,
                                     uint16_t wRxDataLen,
                                     uint16_t *pStatusWord,
                                     uint8_t **ppTxData,
                                     uint16_t *pTxDataLen
                                     )
{
    uint16_t PH_MEMLOC_REM wOffset;
    uint16_t PH_MEMLOC_REM wLe;
    uint8_t  PH_MEMLOC_REM bCommandType;

    /* Check if the File is selected */
    if(pDataParams->pSelectedFile == NULL)
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_FUNC;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    /* Set the Offset value */
    wOffset = (uint16_t)((((uint16_t)pRxData[2] << 8U) & 0xFF00U) | pRxData[3]);

    /* Check for valid offset value */
    if(wOffset > 0x7FFFU)
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_PARAMETER;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    /* Check the Length of C-APDU (ISO 7816-4 IS-2005 section 5.1) */
    if(wRxDataLen == 0x07U)
    {
        bCommandType = PHCE_T4T_EXT_LEN_CMD;

        if(pRxData[4] != 0x00U)
        {
            pDataParams->wStatusWord =  PHCE_T4T_ISO7816_ERR_LENGTH;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }

        /* Set the Expected Length */
        wLe = (uint16_t)((((uint16_t)pRxData[wRxDataLen-2u] << 8U) & 0xFF00U) | pRxData[wRxDataLen - 1u]);

        /* Check for MLe and expected Length */
        if(((((uint16_t)pDataParams->pCcFile[3] << 8U) & 0xFF00U) | pDataParams->pCcFile[4]) < wLe)
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }
    }
    else if(wRxDataLen == 0x05U)
    {
        bCommandType = PHCE_T4T_SHORT_LEN_CMD;

        /* Set the Expected Length */
        wLe = pRxData[wRxDataLen - 1u];

        /* Check for MLe and Expected Length */
        if(((((uint16_t) pDataParams->pCcFile[3] << 8U) & 0xFF00U) | pDataParams->pCcFile[4]) < wLe)
        {
            pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
            *pStatusWord = pDataParams->wStatusWord;

            return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
        }
    }
    else
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    if((((pDataParams->dwFileSize) - wOffset) < wLe) && (wLe != 0x0000U))
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    /* Assign the the data pointer and the data length to be executed */
    *ppTxData = &(pDataParams->pSelectedFile[wOffset]);

    if(wLe != 0x0000U)
    {
        *pTxDataLen = wLe;
    }
    else
    {
        if(bCommandType == PHCE_T4T_SHORT_LEN_CMD)
        {
            *pTxDataLen = (uint16_t)(((pDataParams->dwFileSize - wOffset) > 0x0100U)?
                    0x0100 : (pDataParams->dwFileSize - wOffset));
        }
        else
        {
            *pTxDataLen = (uint16_t)(((pDataParams->dwFileSize - wOffset) > 0xFFFFU)?
                    0xFFFF : (pDataParams->dwFileSize - wOffset));
        }
    }

    pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
    *pStatusWord = pDataParams->wStatusWord;
    pDataParams->bTagState = PHCE_T4T_STATE_FILE_READ;

    return (PH_CE_T4T_READ_BINARY | PH_COMP_CE_T4T);
}

phStatus_t phceT4T_Sw_Int_UpdateBinary(
                                       phceT4T_Sw_DataParams_t *pDataParams,
                                       uint16_t wOption,
                                       uint8_t *pRxData,
                                       uint16_t wRxDataLen,
                                       uint16_t *pStatusWord,
                                       uint8_t **ppTxData,
                                       uint16_t *pTxDataLen
                                       )
{
    uint16_t PH_MEMLOC_REM wOffset;
    uint8_t  PH_MEMLOC_REM bCommandType;

    if((pDataParams->bTagState < PHCE_T4T_STATE_FILE_SELECTED) || (pDataParams->pSelectedFile == NULL))
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_FUNC;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    if(pDataParams->bFileWriteAccess != 0x00U)
    {
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_UNSUPPORTED_FUNC;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE  | PH_COMP_CE_T4T);
    }

    /* Set the offset from the Parameter bytes */
    wOffset = (uint16_t)((((uint16_t)pRxData[2] << 8U) & 0xFF00U) | pRxData[3]);

    /* Update the file offset in data parameter */
    pDataParams->dwFileOffset = wOffset;

    if(wOption == PHCE_T4T_RXDEFAULT)
    {
        /* Set the wLc bytes */
        if(pRxData[4] != 0x00U)
        {
            bCommandType = PHCE_T4T_SHORT_LEN_CMD;
            pDataParams->wLc = pRxData[4];
            if((wRxDataLen - 5u) != pDataParams->wLc)
            {
                /* Change error code to length error */
                pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;

                return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
            }
        }
        else
        {
            bCommandType = PHCE_T4T_EXT_LEN_CMD;
            pDataParams->wLc = (uint16_t)((((uint16_t)pRxData[5] << 8U) & 0xFF00U) | pRxData[6]);

            if((wRxDataLen - 7u) != pDataParams->wLc)
            {
                /* Change error code to length error */
                pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;

                return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
            }
        }
    }
    else
    {
        /* Set the wLc bytes */
        if(pRxData[4] != 0x00U)
        {
            bCommandType = PHCE_T4T_SHORT_LEN_CMD;
            pDataParams->wLc = pRxData[4];

            if(!((wRxDataLen - 5u) <= pDataParams->wLc))
            {
                /* Change error code to length error */
                pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;

                return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
            }
        }
        else
        {
            bCommandType = PHCE_T4T_EXT_LEN_CMD;
            pDataParams->wLc = (uint16_t)((((uint16_t)pRxData[5] << 8U) & 0xFF00U) | pRxData[6]);

            if(!((wRxDataLen - 7u) <= pDataParams->wLc))
            {
                /* Change error code to length error */
                pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;

                return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
            }
        }
    }

    /* Check for Max File size and the data to be written */
    if((uint32_t)(pDataParams->dwFileSize) < (uint32_t)(wOffset + pDataParams->wLc))
    {
        /* Change error code to length error */
        pDataParams->wStatusWord = PHCE_T4T_ISO7816_ERR_LENGTH;
        *pStatusWord = pDataParams->wStatusWord;

        return (PH_CE_T4T_FAILURE | PH_COMP_CE_T4T);
    }

    /* Assign the default out parameters */
    if(bCommandType == PHCE_T4T_SHORT_LEN_CMD)
    {
        *ppTxData = &pRxData[5];
        *pTxDataLen = (wRxDataLen - 5u);
        pDataParams->wLc = pDataParams->wLc - (wRxDataLen - 5u);
    }
    else
    {
        *ppTxData = &pRxData[7];
        *pTxDataLen = (wRxDataLen - 7u);
        pDataParams->wLc = pDataParams->wLc - (wRxDataLen - 7u);
    }

    /* Update file if configured to */
    if(pDataParams->bHandleUpdateCmd == PH_ON)
    {
        phceT4T_Sw_Int_UpdateFile(
            pDataParams,
            *ppTxData,
            *pTxDataLen);

        /* Set TX data to 0 */
        *ppTxData = NULL;
        *pTxDataLen = 0;
    }

    pDataParams->wStatusWord = PHCE_T4T_ISO7816_SUCCESS;
    *pStatusWord = pDataParams->wStatusWord;
    pDataParams->bTagState = PHCE_T4T_STATE_FILE_UPDATE;

    return (PH_CE_T4T_UPDATE_BINARY | PH_COMP_CE_T4T);
}

phStatus_t phceT4T_Sw_Int_Activate(
                                   phceT4T_Sw_DataParams_t *pDataParams
                                   )
{
#ifndef _WIN32
    phOsal_EventBits_t PH_MEMLOC_REM events;
    phStatus_t         PH_MEMLOC_REM status = 0;
    uint16_t           PH_MEMLOC_REM wOption = PH_RECEIVE_DEFAULT;
    uint8_t            PH_MEMLOC_BUF *pRxData = NULL;
    uint16_t           PH_MEMLOC_REM wRxDataLen = 0;
    uint8_t            PH_MEMLOC_BUF *pTxData;
    uint16_t           PH_MEMLOC_REM wTxDataLen = 0;
    uint16_t           PH_MEMLOC_REM wTxChainingLen = 0;
    uint16_t           PH_MEMLOC_REM wStatusWord;
    uint16_t           PH_MEMLOC_REM wFsd;
    uint16_t           PH_MEMLOC_REM wCidPresence;
    uint16_t           PH_MEMLOC_REM wNadPresence;
    uint8_t            PH_MEMLOC_REM bTxType = PHCE_SEND_NO_DATA;
    uint8_t            PH_MEMLOC_REM bExitLoop = FALSE;
    uint8_t            PH_MEMLOC_REM bWaitForData;
    uint8_t            PH_MEMLOC_BUF aSw[2];

    /* Reset RX option */
    pDataParams->bRxOption = PH_RECEIVE_DEFAULT;

    /* Be in HCE state till DESELECTED or receive error */
    while(bExitLoop != TRUE)
    {
        /* Receive loop for RX chaining */
        do
        {
            /* Reset Data to be send flag */
            bTxType = PHCE_SEND_NO_DATA;

            /* Receive Data */
            status = phpalI14443p4mC_Receive(
                pDataParams->pPalI14443p4mCDataParams,
                wOption,
                &pRxData,
                &wRxDataLen);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                if(wOption == PH_RECEIVE_DEFAULT)
                {
                    /* Default processing option */
                    pDataParams->bRxOption = PHCE_T4T_RXDEFAULT;
                }
                else
                {
                    /* Last buffer of RX chaining */
                    pDataParams->bRxOption = PHCE_T4T_RXCHAINING_BUFFER_LAST;
                }
                /* Set to default receive option */
                wOption = PH_RECEIVE_DEFAULT;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
            {
                if(wOption == PH_RECEIVE_DEFAULT)
                {
                    /* First buffer of RX chaining */
                    pDataParams->bRxOption = PHCE_T4T_RXCHAINING_BUFFER_FIRST;
                }
                else
                {
                    /* Subsequent buffer of RX chaining */
                    pDataParams->bRxOption = PHCE_T4T_RXCHAINING_BUFFER_CONT;
                }
                /* Set chaining receive option */
                wOption = PH_RECEIVE_CHAINING;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_SUCCESS_DESELECTED)
            {
                /* DESELECTED */
                status = (PH_ERR_SUCCESS_DESELECTED | PH_COMP_CE_T4T);

                /* Exit */
                break;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
            {
                /* External RF off */
                status = (PH_ERR_EXT_RF_ERROR | PH_COMP_CE_T4T);

                /* Exit */
                break;
            }
            else
            {
                /* Exit */
                break;
            }

            /* Process the received command */
            status = phceT4T_Sw_ProcessCmd(
                pDataParams,
                pDataParams->bRxOption,
                pRxData,
                wRxDataLen,
                &wStatusWord,
                &pTxData,
                &wTxDataLen);

            switch(status & PH_ERR_MASK)
            {
            case PH_CE_T4T_UPDATE_BINARY:
            case PH_CE_T4T_PROPRIETARY:
                /* If UPDATE BINARY is handled by ProcessCmd, send Status Word. */
                if(((status & PH_ERR_MASK) == PH_CE_T4T_UPDATE_BINARY)
                   && (pDataParams->bHandleUpdateCmd == PH_ON))
                {
                    /* Update TX data */
                    pTxData = NULL;
                    wTxDataLen = 0;

                    /* Set data to be send flag */
                    bTxType = PHCE_SEND_DATA;
                    break;
                }

                /* If proprietary command handling is disabled, send the Status
                 * Word (indicating unsupported command). */
                if(((status & PH_ERR_MASK) == PH_CE_T4T_PROPRIETARY)
                   && (pDataParams->bSupportProprietaryCmd == PH_OFF))
                {
                    /* Update TX data */
                    pTxData = NULL;
                    wTxDataLen = 0;

                    /* Set data to be send flag */
                    bTxType = PHCE_SEND_DATA;
                    break;
                }

                /* Copy data to application buffer */
                if(pDataParams->wAppBufferSize < wRxDataLen)
                {
                    /* Return buffer overflow error */
                    return (PH_ERR_BUFFER_OVERFLOW | PH_COMP_CE_T4T);
                }
                else
                {
                    (void)memcpy(pDataParams->pAppBuffer, pRxData, wRxDataLen);

                    /* Update data to be shared with AppProcessCmd */
                    pDataParams->pSharedData = pDataParams->pAppBuffer;
                    pDataParams->wSharedDataLen = wRxDataLen;
                }

                /* Set RX Data available event */
                PH_CHECK_SUCCESS_FCT(status,
                    phOsal_EventPost(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RXDATA_AVAILABLE, NULL));

                /* Loop till TX data is received from AppProcessCmd */
                do
                {
                    /* Wait for TX Data or WTX */
                    PH_CHECK_SUCCESS_FCT(status, phOsal_EventPend((volatile phOsal_Event_t * )(&pDataParams->T4TEventObj.EventHandle), E_OS_EVENT_OPT_PEND_SET_ANY, 0xFFFFFFFFU,
                            (E_PH_OSAL_EVT_TXDATA_AVAILABLE | E_PH_OSAL_EVT_RTO_TIMEOUT | E_PH_OSAL_EVT_ABORT), &events));

                    if((events & E_PH_OSAL_EVT_RTO_TIMEOUT) == E_PH_OSAL_EVT_RTO_TIMEOUT)
                    {
                        /* Clear event */
                        PH_CHECK_SUCCESS_FCT(status,
                            phOsal_EventClear(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RTO_TIMEOUT, NULL));

                        /* Send WTX */
                        PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Wtx(
                            pDataParams->pPalI14443p4mCDataParams));

                        /* Back to wait TX data or WTX */
                        bWaitForData = TRUE;
                    }
                    else if((events & E_PH_OSAL_EVT_TXDATA_AVAILABLE) == E_PH_OSAL_EVT_TXDATA_AVAILABLE)
                    {
                        /* Clear event */
                        PH_CHECK_SUCCESS_FCT(status,
                            phOsal_EventClear(&pDataParams->T4TEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_TXDATA_AVAILABLE, NULL));

                        /* Update TX Data */
                        pTxData = pDataParams->pSharedData;
                        wTxDataLen = pDataParams->wSharedDataLen;

                        /* Set data to be send flag */
                        bTxType = PHCE_SEND_DATA;

                        /* Data received, exit and transmit the data */
                        bWaitForData = FALSE;
                    }
                    else
                    {
                        /* Aborted */
                        return (PH_ERR_ABORTED | PH_COMP_CE_T4T);
                    }
                }
                while(0U != bWaitForData);
                break;

            case PH_CE_T4T_SELECT:
            case PH_CE_T4T_FAILURE:
            case PH_CE_T4T_READ_BINARY:
                /* For select and failure, only status word is send */
                if(((status & PH_ERR_MASK) == PH_CE_T4T_FAILURE)
                   || ((status & PH_ERR_MASK) == PH_CE_T4T_SELECT))
                {
                    pTxData = NULL;
                    wTxDataLen = 0;
                }

                /* Enable transmission of response */
                bTxType = PHCE_SEND_DATA;
                break;

            default:
                /* Control will never come here */
                break;
            }
        }
        while(wOption == PH_RECEIVE_CHAINING);

        if(bTxType == PHCE_SEND_DATA)
        {
            /* Reset data length used for TX chaining */
            wTxChainingLen = 0;

            if(0U != (wTxDataLen))
            {
                /* Get FSD from PAL */
                PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_GetConfig(
                    pDataParams->pPalI14443p4mCDataParams,
                    PHPAL_I14443P4MC_CONFIG_FSD,
                    &wFsd));

                /* Decrement PCB length (1 byte) from frame size */
                wFsd--;

                /* Check if CID is present */
                PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_GetConfig(
                    pDataParams->pPalI14443p4mCDataParams,
                    PHPAL_I14443P4MC_CONFIG_CID_PRESENCE,
                    &wCidPresence));

                /* Decrement CID length (1 byte) from frame size if present */
                if(0U != (wCidPresence))
                {
                    wFsd--;
                }

                /* Check if NAD is present */
                PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_GetConfig(
                    pDataParams->pPalI14443p4mCDataParams,
                    PHPAL_I14443P4MC_CONFIG_NAD_PRESENCE,
                    &wNadPresence));

                /* Calculate Data length to be send with TXCHAINING option */
                wTxChainingLen = (wFsd * (((((wTxDataLen + 2U + wNadPresence) % wFsd) == 0U)?
                    (((wTxDataLen + 2U + wNadPresence) / wFsd) - 1)
                    : ((wTxDataLen + 2U +  wNadPresence) / wFsd)))) - wNadPresence;

                /* Transmit first part of chain data */
                if(0U != (wTxChainingLen))
                {
                    PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Transmit(
                        pDataParams->pPalI14443p4mCDataParams,
                        PH_TRANSMIT_TXCHAINING,
                        pTxData,
                        (wTxChainingLen > wTxDataLen)? --wTxChainingLen : wTxChainingLen));
                }

                /* Buffer balance TX data */
                if(wTxDataLen > wTxChainingLen)
                {
                    PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Transmit(
                        pDataParams->pPalI14443p4mCDataParams,
                        PH_TRANSMIT_BUFFER_FIRST,
                        &pTxData[wTxChainingLen],
                        (wTxDataLen - wTxChainingLen)));
                }
            }

            /* Copy Status word from data parameter for transmitting */
            aSw[0] = (uint8_t)((pDataParams->wStatusWord & 0xFF00U) >> 8U);
            aSw[1] = (uint8_t)(pDataParams->wStatusWord & 0xFFU);

            /* Send status word */
            PH_CHECK_SUCCESS_FCT(status, phpalI14443p4mC_Transmit(
                pDataParams->pPalI14443p4mCDataParams,
                PH_TRANSMIT_BUFFER_LAST,
                aSw,
                2));
        }
        else
        {
            /* Error from lower layer or DESELECTED, return back to application */
            bExitLoop = TRUE;
        }
    }

    return status;
#endif /* _WIN32 */
#ifdef _WIN32
    PH_UNUSED_VARIABLE(pDataParams);
    return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_CE_T4T);
#endif /* _WIN32 */
}

void phceT4T_Sw_Int_UpdateFile(
                               phceT4T_Sw_DataParams_t *pDataParams,
                               uint8_t *pData,
                               uint32_t dwDataLen
                               )
{
    /* Update File */
    (void)memcpy(
        &pDataParams->pSelectedFile[pDataParams->dwFileOffset],
        pData,
        dwDataLen);

    /* Update file offset */
    pDataParams->dwFileOffset += dwDataLen;
}

#endif /* NXPBUILD__PHCE_T4T_SW */
