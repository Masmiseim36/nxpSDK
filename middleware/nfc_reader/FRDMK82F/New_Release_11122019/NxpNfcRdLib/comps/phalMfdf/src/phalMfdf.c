/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic MIFARE DESFire EV1 contactless IC Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SA: Generated 02. Feb 2010
*
*/

#include <phalMfdf.h>
#include <ph_Status.h>
#include <ph_TypeDefs.h>

#ifdef NXPBUILD__PHAL_MFDF_SW
#include "Sw/phalMfdf_Sw.h"
#endif /* NXPBUILD__PHAL_MFDF_SW */



#ifdef NXPBUILD__PHAL_MFDF

#ifndef NXPRDLIB_REM_GEN_INTFS


/*
* PICC level commands
*/

phStatus_t phalMfdf_CreateApplication(
                                      void * pDataParams,
                                      uint8_t bOption,
                                      uint8_t * pAid,
                                      uint8_t bKeySettings1,
                                      uint8_t bKeySettings2,
                                      uint8_t * pISOFileId,
                                      uint8_t * pISODFName,
                                      uint8_t bISODFNameLen
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateApplication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeySettings1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeySettings2);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISOFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISODFName);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bISODFNameLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeySettings1_log, &bKeySettings1);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeySettings2_log, &bKeySettings2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bISODFNameLen_log, &bISODFNameLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAid_log, pAid, 3);
    if (0U != (bOption & 0x01U))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISOFileId_log, pISOFileId, 2);
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISODFName_log, pISODFName, bISODFNameLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    if (0U != (bOption & 0x01U)) PH_ASSERT_NULL (pISOFileId);
    if (bISODFNameLen > 0U) PH_ASSERT_NULL (pISODFName);
    PH_ASSERT_NULL (pAid);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateApplication((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            pAid,
            bKeySettings1,
            bKeySettings2,
            pISOFileId,
            pISODFName,
            bISODFNameLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_SelectApplication(
                                      void * pDataParams,
                                      uint8_t * pAppId
                                      )
{
    phStatus_t  PH_MEMLOC_REM status = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_SelectApplication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAppId);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAppId_log, pAppId, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAppId);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {

#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_SelectApplication((phalMfdf_Sw_DataParams_t *)pDataParams, pAppId);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_GetVersion(
                               void * pDataParams,
                               uint8_t * pVerInfo
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetVersion");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVerInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pVerInfo);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetVersion((phalMfdf_Sw_DataParams_t *)pDataParams, pVerInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVerInfo_log, pVerInfo, 28);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_CreateStdDataFile(
                                      void * pDataParams,
                                      uint8_t bOption,
                                      uint8_t bFileNo,
                                      uint8_t *pISOFileId,
                                      uint8_t bCommSett,
                                      uint8_t *pAccessRights,
                                      uint8_t * pFileSize
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateStdDataFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISOFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFileSize);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFileSize_log, pFileSize, 3);
    if (bOption == 0x01U)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISOFileId_log, pISOFileId, 2);
    }

    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (bOption == 0x01U) PH_ASSERT_NULL (pISOFileId);
    PH_ASSERT_NULL (pAccessRights);
    PH_ASSERT_NULL (pFileSize);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateStdDataFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_WriteData(
                              void * pDataParams,
                              uint8_t bCommOption,
                              uint8_t bFileNo,
                              uint8_t * pOffset,
                              uint8_t * pTxData,
                              uint8_t * pTxDataLen
                              )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_WriteData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset, 3);

    wDataLen = (uint16_t)pTxDataLen[1];
    wDataLen = wDataLen << 8U;
    wDataLen |= pTxDataLen[0];

    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxData_log, pTxData, wDataLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxDataLen_log, pTxDataLen, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pOffset);
    PH_ASSERT_NULL (pTxData);
    PH_ASSERT_NULL (pTxDataLen);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_WriteData((phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pTxData,
            pTxDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_IsoSelectFile(
                                  void * pDataParams,
                                  uint8_t bOption,
                                  uint8_t bSelector,
                                  uint8_t * pFid,
                                  uint8_t * pDFname,
                                  uint8_t bDFnameLen,
                                  uint8_t **ppFCI,
                                  uint16_t *pwFCILen
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoSelectFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSelector);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDFname);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDFnameLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSelector_log, &bSelector);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFid_log, pFid,2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDFnameLen_log, &bDFnameLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFname_log, pDFname,bDFnameLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoSelectFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bSelector,
            pFid,
            pDFname,
            bDFnameLen,
            ppFCI,
            pwFCILen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoReadBinary(
                                  void * pDataParams,
                                  uint16_t wOption,
                                  uint8_t bOffset,
                                  uint8_t bSfid,
                                  uint8_t bBytesToRead,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pBytesRead
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoReadBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBytesToRead);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBytesRead);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBytesToRead_log, &bBytesToRead);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pBytesRead);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoReadBinary((phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            bOffset,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, (*pBytesRead));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pBytesRead_log, pBytesRead);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoUpdateBinary(
                                    void * pDataParams,
                                    uint8_t bOffset,
                                    uint8_t bSfid,
                                    uint8_t * pData,
                                    uint8_t bDataLen
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoUpdateBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData,bDataLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bDataLen_log, &bDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pData);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoUpdateBinary((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOffset,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_GetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t * pValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetConfig((phalMfdf_Sw_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_SetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t wValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_SetConfig((phalMfdf_Sw_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHAL_MFDF */
