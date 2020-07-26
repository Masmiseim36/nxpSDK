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
* Internal functions for Type 4 Tag Operation component of Reader Library
* Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#include <ph_TypeDefs.h>
#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#include <phalMfdf.h>
#include <phalTop.h>

#ifdef NXPBUILD__PHAL_TOP_SW
#ifdef NXPBUILD__PHAL_MFDF_SW

#include "phalTop_Sw_Int_T4T.h"

phStatus_t phalTop_Sw_Int_T4T_ClearState(
                                         phalTop_Sw_DataParams_t * pDataParams,
                                         phalTop_T4T_t * pT4T
                                         )
{
    uint8_t  PH_MEMLOC_BUF aFidMApp[2] = {0x00, 0x3F};
    uint8_t  PH_MEMLOC_REM * pFCI = NULL;
    uint16_t PH_MEMLOC_REM wFCILen = 0;

    /* Workaround for MIFARE DESFire contactless IC tags to enable selection of NDEF application
     * again, when CheckNdef is called multiple times. By default selection of
     * same application again will return error. To avoid this, Master
     * application is selection is done.
     *
     * NOTE: As per NFC specification calling CheckNdef multiple times is not
     * needed under any use case. So during normal use cases this selection of
     * master application file will not be executed. */
    if(pT4T->bCurrentSelectedFile != 0U)
    {
        /* Select the Master application (file ID 0x3F00) */
        (void)phalMfdf_IsoSelectFile(
            pT4T->pAlT4TDataParams,
            0x0C,
            0x00,
            aFidMApp,
            NULL,
            0x00,
            &pFCI,
            &wFCILen);
    }

    /* Reset parameters */
    pT4T->wCCLEN = 0;
    pDataParams->bVno = 0;
    pT4T->wMLe = 0;
    pT4T->wMLc = 0;

    pT4T->aNdefFileID[0] = 0;
    pT4T->aNdefFileID[1] = 0;
    pT4T->wMaxFileSize = 0;
    pT4T->bRa = 0;
    pT4T->bWa = 0;
    pDataParams->wNdefLength = 0;

    pDataParams->bTagState = 0;
    pT4T->bCurrentSelectedFile = 0;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T4T_CheckNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pTagState
                                        )
{
    phStatus_t    PH_MEMLOC_REM status;
    uint16_t      PH_MEMLOC_REM wBytesRead;
    uint8_t       PH_MEMLOC_REM * pRxBuffer = NULL;
    uint8_t       PH_MEMLOC_BUF aFidCc[2] = {0x03, 0xE1};
    uint8_t       PH_MEMLOC_BUF aNdefAppName[7] = {0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01};
    uint8_t       PH_MEMLOC_REM * pFCI = NULL;
    uint16_t      PH_MEMLOC_REM wFCILen = 0;
    phalTop_T4T_t PH_MEMLOC_REM * pT4T = &pDataParams->ualTop.salTop_T4T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT4T->pAlT4TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Reset tag state */
    *pTagState = PHAL_TOP_STATE_NONE;

    /* Set Wrapped mode */
    ((phalMfdf_Sw_DataParams_t *)(pT4T->pAlT4TDataParams))->bWrappedMode = 1;

    /* Clear values from previous detection, if any */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T4T_ClearState(pDataParams, pT4T));

    /* Select the NDEF Tag Application */
    status = phalMfdf_IsoSelectFile(
        pT4T->pAlT4TDataParams,
        PH_EXCHANGE_DEFAULT,
        0x04,
        NULL,
        aNdefAppName,
        0x07,
        &pFCI,
        &wFCILen);

    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_NON_NDEF_TAG, PH_COMP_AL_TOP);
    }

    pT4T->bCurrentSelectedFile = PHAL_TOP_T4T_SELECTED_NDEF_APP;

    /* Select the Capability Container (CC) file */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoSelectFile(
        pT4T->pAlT4TDataParams,
        0x0C,
        0x00,
        aFidCc,
        NULL,
        0x00,
        &pFCI,
        &wFCILen));

    pT4T->bCurrentSelectedFile = PHAL_TOP_T4T_SELECTED_CC_FILE;

    /* Read the CC file */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoReadBinary(
        pT4T->pAlT4TDataParams,
        PH_EXCHANGE_DEFAULT,
        0x00,
        0x00,
        0x0F,
        &pRxBuffer,
        &wBytesRead));

    /* Validate CC length (CCLEN) */
    if((((uint16_t)pRxBuffer[0] << 8U) | pRxBuffer[1]) < 0x0FU)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Update version number */
    pDataParams->bVno = pRxBuffer[PHAL_TOP_T4T_VERSION_OFFSET];

    /* Check for supported version number */
    if((uint8_t)(pDataParams->bVno & 0xF0U) != (uint8_t)(PHAL_TOP_T4T_NDEF_SUPPORTED_VNO & 0xF0U))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_VERSION, PH_COMP_AL_TOP);
    }

    /* Update MLe and MLc */
    pT4T->wMLe = (uint16_t)((uint16_t)pRxBuffer[3] << 8U) | pRxBuffer[4];
    pT4T->wMLc = (uint16_t)((uint16_t)pRxBuffer[5] << 8U) | pRxBuffer[6];

    /* Validate MLe and MLc */
    if((pT4T->wMLe < 0x0FU) || (pT4T->wMLc < 0x01U))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Validate CC file length and check for NDEF TLV */
    if(pRxBuffer[PHAL_TOP_T4T_NDEFTLV_OFFSET] == PHAL_TOP_T4T_NDEF_TLV)
    {
        /* Update read access */
        pT4T->bRa = pRxBuffer[PHAL_TOP_T4T_NDEFTLV_OFFSET + 6U];

        /* Update write access */
        pT4T->bWa = pRxBuffer[PHAL_TOP_T4T_NDEFTLV_OFFSET + 7U];

        /* Validate read access */
        if(pT4T->bRa == PHAL_TOP_T4T_NDEF_FILE_READ_ACCESS)
        {
            /* Max NDEF file size */
            pT4T->wMaxFileSize = ((uint16_t)pRxBuffer[11] << 8U) | pRxBuffer[12];

            /* Validate max file size */
            if((pT4T->wMaxFileSize < 0x0005U) || (pT4T->wMaxFileSize == 0xFFFFU))
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
            }

            /* Read NDEF File ID */
            pT4T->aNdefFileID[0] = pRxBuffer[PHAL_TOP_T4T_NDEFTLV_OFFSET + 3U];
            pT4T->aNdefFileID[1] = pRxBuffer[PHAL_TOP_T4T_NDEFTLV_OFFSET + 2U];

            /* Validate NDEF file ID */
            if(!(((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0x0000U) ||
                ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0xE102U) ||
                ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0xE103U) ||
                ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0x3F00U) ||
                ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0x3FFFU)))
            {
                /* Select the NDEF file  */
                PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoSelectFile(
                    pT4T->pAlT4TDataParams,
                    0x0C,
                    0x00,
                    pT4T->aNdefFileID,
                    NULL,
                    0x00,
                    &pFCI,
                    &wFCILen));

                pT4T->bCurrentSelectedFile = PHAL_TOP_T4T_SELECTED_NDEF_FILE;

                /* Read the NDEF file */
                PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoReadBinary(
                    pT4T->pAlT4TDataParams,
                    PH_EXCHANGE_DEFAULT,
                    0x00,
                    0x00,
                    0x02,
                    &pRxBuffer,
                    &wBytesRead));

                /* Update NDEF message length */
                pDataParams->wNdefLength = ((uint16_t)pRxBuffer[0] << 8U) | pRxBuffer[1];

                /* Validate NDEF length */
                if((pDataParams->wNdefLength > 0x0000U) && (pDataParams->wNdefLength <= (pT4T->wMaxFileSize - 2U)))
                {
                    if(pT4T->bWa == PHAL_TOP_T4T_NDEF_FILE_NO_WRITE_ACCESS)
                    {
                        pDataParams->bTagState = PHAL_TOP_STATE_READONLY;
                    }
                    else if(pT4T->bWa == PHAL_TOP_T4T_NDEF_FILE_WRITE_ACCESS)
                    {
                        pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
                    }
                    else
                    {
                        /* Proprietary options; Not supported */
                        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
                    }
                }
                else if((pDataParams->wNdefLength == 0x0000U))
                {
                    /* Check write access */
                    if(pT4T->bWa == PHAL_TOP_T4T_NDEF_FILE_WRITE_ACCESS)
                    {
                        pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
                    }
                    else
                    {
                        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
                    }
                }
                else
                {
                    /* NDEF length / file size not properly configured */
                    return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
                }
            }
            else
            {
                /* Wrong NDEF file ID */
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
            }
        }
        else
        {
            /* Proprietary read options; Not supported */
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
        }
    }
    else
    {
        /* NDEF TLV nor present */
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Update max. NDEF size */
    pDataParams->wMaxNdefLength = pT4T->wMaxFileSize - 2u;

    /* Update state in out parameter */
    *pTagState = pDataParams->bTagState;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T4T_FormatNdef(
                                         phalTop_Sw_DataParams_t * pDataParams
                                         )
{
    phStatus_t    PH_MEMLOC_REM status;
    uint8_t       PH_MEMLOC_BUF aFidMApp[2] = {0x00, 0x3F};
    uint8_t       PH_MEMLOC_BUF aCcFileLen[3] = {0x0F, 0x00, 0x00};
    uint8_t       PH_MEMLOC_BUF aNdefFileLen[3] = {0x00, 0x04, 0x00};
    uint8_t       PH_MEMLOC_BUF aAccessRights[2] = {0xEE, 0xEE};
    uint8_t       PH_MEMLOC_BUF aAid[3] = {0x01, 0x00, 0x00};
    uint8_t       PH_MEMLOC_BUF aFidApp[2] = {0x02, 0xE1};
    uint8_t       PH_MEMLOC_BUF aFidCc[2] = {0x03, 0xE1};
    uint8_t       PH_MEMLOC_REM * pFCI = NULL;
    uint16_t      PH_MEMLOC_REM wFCILen = 0;
    /* NDEF application name */
    uint8_t    PH_MEMLOC_REM aNdefAppName[7] = {0xD2, 0x76, 0x00, 0x00,
                                                0x85, 0x01, 0x01};
    /* Default CC File (0xE104 as NDEF File ID and 1KB as NDEF File Size) */
    uint8_t    PH_MEMLOC_REM aCcData[15] = {0x00, 0x0F, 0x20, 0x00, 0x3B,
                                          0x00, 0x34, 0x04, 0x06, 0xE1,
                                          0x04, 0x04, 0x00, 0x00, 0x00};

    /* Default NDEF file */
    uint8_t    PH_MEMLOC_REM aNdefData[5] = {0x00, 0x03, 0xD0, 0x00, 0x00};
    phalTop_T4T_t PH_MEMLOC_REM * pT4T = &pDataParams->ualTop.salTop_T4T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT4T->pAlT4TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check for NDEF presence */
    if(pDataParams->bTagState != PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_FORMATTED_TAG, PH_COMP_AL_TOP);
    }

    /* Validate and update NDEF file ID */
    if(!(((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0x0000U) ||
        ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0xE102U) ||
        ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0xE103U) ||
        ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0x3F00U) ||
        ((((uint16_t)pT4T->aNdefFileID[1] << 8U) | pT4T->aNdefFileID[0]) == 0x3FFFU)))
    {
        aCcData[0x0A] = pT4T->aNdefFileID[0];
        aCcData[0x09] = pT4T->aNdefFileID[1];
    }
    else
    {
        pT4T->aNdefFileID[0] = aCcData[0x0A];
        pT4T->aNdefFileID[1] = aCcData[0x09];
    }

    /* Validate and update file size */
    if(!((pT4T->wMaxFileSize < 0x0005U) ||
        (pT4T->wMaxFileSize > 0x80FEU)))
    {
        aCcData[0x0B] = (uint8_t)((uint16_t)(pT4T->wMaxFileSize & 0xFF00U) >> 8U);
        aCcData[0x0C] = (uint8_t)(pT4T->wMaxFileSize & 0x00FFU);
    }
    else
    {
        pT4T->wMaxFileSize = ((uint16_t)aCcData[0x0B] << 8U) | aCcData[0x0C];
    }

    /* Update CC length */
    pT4T->wCCLEN = 0x0F;

    /* Version number, default 2.0 */
    pDataParams->bVno = 0x20;

    /* Validate and update MLe */
    if(pT4T->wMLe >= 0x0FU)
    {
        aCcData[3] = (uint8_t)((pT4T->wMLe & 0xFF00U) >> 8U);
        aCcData[4] = (uint8_t)(pT4T->wMLe & 0x00FFU);
    }
    else
    {
        pT4T->wMLe = 0x3B;
    }

    /* Validate and update MLc */
    if(pT4T->wMLc >= 0x01U)
    {
        aCcData[5] = (uint8_t)((pT4T->wMLc & 0xFF00U) >> 8U);
        aCcData[6] = (uint8_t)(pT4T->wMLc & 0x00FFU);
    }
    else
    {
        pT4T->wMLc = 0x34;
    }

    /* By default, tag is formated in READ-WRITE state */
    pT4T->bRa = aCcData[0x0D];
    pT4T->bWa = aCcData[0x0E];

    /* Select the Master application (file ID 0x3F00) for MIFARE DESFire */
    status = phalMfdf_IsoSelectFile(
        pT4T->pAlT4TDataParams,
        0x0C,
        0x00,
        aFidMApp,
        NULL,
        0x00,
        &pFCI,
        &wFCILen);

    /* Check for success (for MIFARE DESFire)*/
    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
    }

    /* Select the NDEF Tag Application */
    status = phalMfdf_IsoSelectFile(
        pT4T->pAlT4TDataParams,
        PH_EXCHANGE_DEFAULT,
        0x04,
        NULL,
        aNdefAppName,
        0x07,
        &pFCI,
        &wFCILen);

    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* Create NDEF tag application */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_CreateApplication(
            pT4T->pAlT4TDataParams,
            0x03,
            aAid,
            0x0F,
            0x21,
            aFidApp,
            aNdefAppName,
            0x07));

        /* Select the NDEF Tag Application */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoSelectFile(
            pT4T->pAlT4TDataParams,
            PH_EXCHANGE_DEFAULT,
            0x04,
            NULL,
            aNdefAppName,
            0x07,
            &pFCI,
            &wFCILen));
    }

    /* Select the Capability Container (CC) file */
    status = phalMfdf_IsoSelectFile(
        pT4T->pAlT4TDataParams,
        0x0C,
        0x00,
        aFidCc,
        NULL,
        0x00,
        &pFCI,
        &wFCILen);

    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* Create CC file */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_CreateStdDataFile(
            pT4T->pAlT4TDataParams,
            0x01,
            0x01,
            aFidCc,
            PHAL_MFDF_COMMUNICATION_PLAIN,
            aAccessRights,
            aCcFileLen));

        /* Select the Capability Container (CC) file */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoSelectFile(
            pT4T->pAlT4TDataParams,
            0x0C,
            0x00,
            aFidCc,
            NULL,
            0x00,
            &pFCI,
            &wFCILen));
    }

    /* Write data to CC file */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
        pT4T->pAlT4TDataParams,
        0x00,
        0x00,
        aCcData,
        0x0F));

    /* Select the NDEF file */
    status = phalMfdf_IsoSelectFile(
        pT4T->pAlT4TDataParams,
        0x0C,
        0x00,
        pT4T->aNdefFileID,
        NULL,
        0x00,
        &pFCI,
        &wFCILen);

    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* Get NDEF file size */
        aNdefFileLen[0] = aCcData[0x0C];
        aNdefFileLen[1] = aCcData[0x0B];

        /* Create NDEF file */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_CreateStdDataFile(
            pT4T->pAlT4TDataParams,
            0x01,
            0x02,
            pT4T->aNdefFileID,
            PHAL_MFDF_COMMUNICATION_PLAIN,
            aAccessRights,
            aNdefFileLen));

        /* Select the NDEF file */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoSelectFile(
            pT4T->pAlT4TDataParams,
            0x0C,
            0x00,
            pT4T->aNdefFileID,
            NULL,
            0x00,
            &pFCI,
            &wFCILen));
    }

    /* Write Empty NDEF message to NDEF file */

        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
            pT4T->pAlT4TDataParams,
            0x00,
            0x00,
            aNdefData,
            0x05));

        /* Select the Master application (file ID 0x3F00) for MIFARE DESFire */
        /* This is being performed again to go to the master application back to perform different operations */
        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoSelectFile(
            pT4T->pAlT4TDataParams,
            0x0C,
            0x00,
            aFidMApp,
            NULL,
            0x00,
            &pFCI,
            &wFCILen));

        /* Update state parameters */
        pDataParams->wNdefLength = 0x03;
        pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T4T_ReadNdef(
                                       phalTop_Sw_DataParams_t * pDataParams,
                                       uint8_t * pData,
                                       uint16_t * pLength
                                       )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint16_t      PH_MEMLOC_REM   wBytesRead;
    uint16_t      PH_MEMLOC_REM   wNdefLen = 0;
    uint16_t      PH_MEMLOC_REM   wOffset;
    uint8_t       PH_MEMLOC_REM   P1;
    uint8_t       PH_MEMLOC_REM   P2;
    uint8_t       PH_MEMLOC_REM   * pRxBuffer = NULL;
    uint16_t      PH_MEMLOC_COUNT wCount;
    uint16_t      PH_MEMLOC_REM   wReadLength = 0;
    phalTop_T4T_t PH_MEMLOC_REM * pT4T = &pDataParams->ualTop.salTop_T4T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT4T->pAlT4TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Reset NDEF length */
    *pLength = 0;

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check for NDEF length > 0 (in initialized state NDEF length is 0) */
    if(pDataParams->bTagState == PHAL_TOP_STATE_INITIALIZED)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_EMPTY_NDEF, PH_COMP_AL_TOP);
    }

    /* Buffer allocated by user is less than NDEF message size on Tag. */
    if (pDataParams->wNdefLength > PH_NXPNFCRDLIB_CONFIG_MAX_NDEF_DATA)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_TOP);
    }

    /* NDEF length to be read */
    wNdefLen = pDataParams->wNdefLength;

    /* Max read length in single command is from 1 - 255 */
    wReadLength = (pT4T->wMLe < 0xFFU)? pT4T->wMLe : 0xFFU;
    wCount = 0;
    while(wNdefLen > wReadLength)
    {
        /* NDEF message starts from offset 0x02 */
        wOffset = wCount + 2U;

        P1 = (uint8_t)((wOffset & 0xFF00U) >> 8U);
        P2 = (uint8_t)(wOffset & 0x00FFU);

        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoReadBinary(
            pT4T->pAlT4TDataParams,
            PH_EXCHANGE_DEFAULT,
            P2,
            P1,
            (uint8_t)wReadLength,
            &pRxBuffer,
            &wBytesRead));
        if(wReadLength > 0U)
        {
            (void)memcpy(&pData[wCount], pRxBuffer, (size_t)wReadLength);
        }

        wNdefLen -= wReadLength;
        wCount += wReadLength;
    }

    /* NDEF message starts from offset 0x02 */
    wOffset = wCount + 2U;

    /* Offset */
    P1 = (uint8_t)((wOffset & 0xFF00U) >> 8U);
    P2 = (uint8_t)(wOffset & 0x00FFU);

    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoReadBinary(
        pT4T->pAlT4TDataParams,
        PH_EXCHANGE_DEFAULT,
        P2,
        P1,
        (uint8_t)wNdefLen,
        &pRxBuffer,
        &wBytesRead));

    (void)memcpy(&pData[wCount], pRxBuffer, wNdefLen);

    /* Return NDEF length */
    *pLength = pDataParams->wNdefLength;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T4T_WriteNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pData,
                                        uint16_t wLength
                                        )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint16_t      PH_MEMLOC_REM   wNdefLen = 0;
    uint16_t      PH_MEMLOC_COUNT wCount;
    uint16_t      PH_MEMLOC_REM   wOffset;
    uint8_t       PH_MEMLOC_BUF   aNDEFLength[2];
    uint16_t      PH_MEMLOC_REM   wWriteLength = 0;
    uint8_t       PH_MEMLOC_REM   P1;
    uint8_t       PH_MEMLOC_REM   P2;
    phalTop_T4T_t PH_MEMLOC_REM * pT4T = &pDataParams->ualTop.salTop_T4T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT4T->pAlT4TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check if tag is read-only */
    if(pDataParams->bTagState == PHAL_TOP_STATE_READONLY)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_READONLY_TAG, PH_COMP_AL_TOP);
    }

    /* Validate input parameters */
    if((wLength == 0U) || (wLength > (pT4T->wMaxFileSize - 2u)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Set NDEF Length (NLEN) in NDEF file as 0 */
    aNDEFLength[0] = 0;
    aNDEFLength[1] = 0;

    /* Write 0 in the NLEN field */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
        pT4T->pAlT4TDataParams,
        0x00,
        0x00,
        aNDEFLength,
        0x02));

    /* NDEF length to be written */
    wNdefLen = wLength;

    /* Max write length in single command is from 1 - 255 */
    wWriteLength = (pT4T->wMLc < 0xFFU)? pT4T->wMLc : 0xFFU;

    /* Write the NDEF message from 2nd Byte in memory */
    wCount = 0;
   while(wNdefLen > wWriteLength)
    {
        wOffset = wCount + 2U;

        P1 = (uint8_t)((wOffset & 0xFF00U) >> 8U);
        P2 = (uint8_t)(wOffset & 0x00FFU);

        PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
            pT4T->pAlT4TDataParams,
            P2,
            P1,
            &pData[wCount],
            (uint8_t)wWriteLength));

        wNdefLen -= wWriteLength;
        wCount += wWriteLength;
    }

    /* Write remaining NDEF message */
    wOffset = wCount + 2U;

    P1 = (uint8_t)((wOffset & 0xFF00U) >> 8U);
    P2 = (uint8_t)(wOffset & 0x00FFU);

    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
        pT4T->pAlT4TDataParams,
        P2,
        P1,
        &pData[wCount],
        (uint8_t)wNdefLen));

    /* Update NDEF length in NDEF file */
    aNDEFLength[0] = (uint8_t)((wLength & 0xFF00U) >> 8U);
    aNDEFLength[1] = (uint8_t)(wLength & 0x00FFU);

    /* Write the length of NDEF message in the NLEN field */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
        pT4T->pAlT4TDataParams,
        0x00,
        0x00,
        aNDEFLength,
        0x02));

    pDataParams->wNdefLength = wLength;
    pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T4T_EraseNdef(
                                        phalTop_Sw_DataParams_t * pDataParams
                                        )
{
    phStatus_t    PH_MEMLOC_REM status;
    uint8_t       PH_MEMLOC_BUF aNDEFLength[2] = {0};
    phalTop_T4T_t PH_MEMLOC_REM * pT4T = &pDataParams->ualTop.salTop_T4T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT4T->pAlT4TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check if tag is read-only */
    if(pDataParams->bTagState == PHAL_TOP_STATE_READONLY)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_READONLY_TAG, PH_COMP_AL_TOP);
    }

    /* Check if tag is already in initialized state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_INITIALIZED)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_EMPTY_NDEF, PH_COMP_AL_TOP);
    }

    /* Set NDEF length as 0 (i.e. initialized state) */
    PH_CHECK_SUCCESS_FCT(status, phalMfdf_IsoUpdateBinary(
        pT4T->pAlT4TDataParams,
        0x00,
        0x00,
        aNDEFLength,
        0x02));

    /* Update state variables */
    pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
    pDataParams->wNdefLength = 0;

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_MFDF_SW */
#endif /* NXPBUILD__PHAL_TOP_SW */
