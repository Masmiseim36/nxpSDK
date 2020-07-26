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
* Generic ISO18000-3 Mode3 Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 27. January 2010
*
*/

#include <ph_Status.h>
#include <phalI18000p3m3.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_I18000P3M3_SW
#include "Sw/phalI18000p3m3_Sw.h"
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

#ifdef NXPBUILD__PHAL_I18000P3M3

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phalI18000p3m3_Ack(
                              void * pDataParams,
                              uint8_t ** ppRxBuffer,
                              uint16_t * pRxBitLength
                              )
{
    phStatus_t  PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint16_t    PH_MEMLOC_REM wByteLength = 0;
#endif
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_Ack");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBitLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pRxBitLength);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_Ack(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            ppRxBuffer,
            pRxBitLength);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
        (*ppRxBuffer != NULL))
    {
        wByteLength = (*pRxBitLength & 0x07U) ? ((*pRxBitLength >> 3U) + 1U) : (*pRxBitLength >> 3U);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, wByteLength);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pRxBitLength_log, pRxBitLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_ReqRn(
                                void * pDataParams,
                                uint8_t bOption,
                                uint8_t ** ppRxBuffer
                                )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_ReqRn");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (ppRxBuffer);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_ReqRn(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bOption,
            ppRxBuffer);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
        (*ppRxBuffer != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, 2);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_Read(
                               void * pDataParams,
                               uint8_t bMemBank,
                               uint8_t * pWordPtr,
                               uint8_t bWordPtrLength,
                               uint8_t bWordCount,
                               uint8_t ** ppRxBuffer,
                               uint16_t * pRxBitLength
                               )
{
    phStatus_t  PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint16_t    PH_MEMLOC_REM wByteLength = 0;
#endif
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_Read");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pWordPtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordPtrLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordCount);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBitLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pWordPtr);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pRxBitLength);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pWordPtr_log, pWordPtr, bWordPtrLength + 1U);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordPtrLength_log, &bWordPtrLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordCount_log, &bWordCount);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_Read(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bMemBank,
            pWordPtr,
            bWordPtrLength,
            bWordCount,
            ppRxBuffer,
            pRxBitLength);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
        (*ppRxBuffer != NULL))
    {
        wByteLength = (*pRxBitLength & 0x07U) ? ((*pRxBitLength >> 3U) + 1U) : (*pRxBitLength >> 3U);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, wByteLength);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pRxBitLength_log, pRxBitLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_Write(
                                void * pDataParams,
                                uint8_t bOption,
                                uint8_t bMemBank,
                                uint8_t * pWordPtr,
                                uint8_t bWordPtrLength,
                                uint8_t * pData
                                )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_Write");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pWordPtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordPtrLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pWordPtr);
    PH_ASSERT_NULL (pData);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pWordPtr_log, pWordPtr, bWordPtrLength + 1U);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordPtrLength_log, &bWordPtrLength);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, 2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_Write(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bOption,
            bMemBank,
            pWordPtr,
            bWordPtrLength,
            pData);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_Kill(
                               void * pDataParams,
                               uint8_t bOption,
                               uint8_t * pPassword,
                               uint8_t bRecom
                               )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_Kill");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPassword);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRecom);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPassword);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPassword_log, pPassword, 4);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRecom_log, &bRecom);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_Kill(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bOption,
            pPassword,
            bRecom);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_Lock(
                               void * pDataParams,
                               uint8_t * pMask,
                               uint8_t * pAction
                               )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_Lock");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAction);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pAction);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAction_log, pAction, 2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_Lock(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            pMask,
            pAction);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_Access(
                                 void * pDataParams,
                                 uint8_t bOption,
                                 uint8_t * pPassword
                                 )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_Access");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPassword);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPassword);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPassword_log, pPassword, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_Access(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bOption,
            pPassword);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_BlockWrite(
                                     void * pDataParams,
                                     uint8_t bMemBank,
                                     uint8_t * pWordPtr,
                                     uint8_t bWordPtrLength,
                                     uint8_t bWordCount,
                                     uint8_t * pData
                                     )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_BlockWrite");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pWordPtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordPtrLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordCount);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pWordPtr);
    PH_ASSERT_NULL (pData);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pWordPtr_log, pWordPtr, bWordPtrLength + 1U);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordPtrLength_log, &bWordPtrLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordCount_log, &bWordCount);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, bWordCount << 1U);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_BlockWrite(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bMemBank,
            pWordPtr,
            bWordPtrLength,
            bWordCount,
            pData);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_BlockErase(
                                     void * pDataParams,
                                     uint8_t bMemBank,
                                     uint8_t * pWordPtr,
                                     uint8_t bWordPtrLength,
                                     uint8_t bWordCount
                                     )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_BlockErase");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pWordPtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordPtrLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWordCount);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pWordPtr);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pWordPtr_log, pWordPtr, bWordPtrLength + 1U);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordPtrLength_log, &bWordPtrLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWordCount_log, &bWordCount);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_BlockErase(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bMemBank,
            pWordPtr,
            bWordPtrLength,
            bWordCount);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_BlockPermaLock(
    void * pDataParams,
    uint8_t bRFU,
    uint8_t bReadLock,
    uint8_t bMemBank,
    uint8_t * pBlockPtr,
    uint8_t bBlockPtrLength,
    uint8_t bBlockRange,
    uint8_t * pMask,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxBitLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint16_t    PH_MEMLOC_REM wByteLength = 0;
#endif
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_BlockPermaLock");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRFU);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bReadLock);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBlockPtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockPtrLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockRange);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBitLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pBlockPtr);
    PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pRxBitLength);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRFU_log, &bRFU);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bReadLock_log, &bReadLock);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pBlockPtr_log, pBlockPtr, bBlockPtrLength + 1U);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockPtrLength_log, &bBlockPtrLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockRange_log, &bBlockRange);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bBlockRange << 1U);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_BlockPermaLock(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            bRFU,
            bReadLock,
            bMemBank,
            pBlockPtr,
            bBlockPtrLength,
            bBlockRange,
            pMask,
            ppRxBuffer,
            pRxBitLength);
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
        (*ppRxBuffer != NULL))
    {
        wByteLength = (*pRxBitLength & 0x07U) ? ((*pRxBitLength >> 3U) + 1U) : (*pRxBitLength >> 3U);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, wByteLength);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pRxBitLength_log, pRxBitLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI18000p3m3_SetHandle(
                                    void * pDataParams,
                                    uint8_t* pHandle
                                    )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI18000p3m3_SetHandle");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pHandle);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pHandle);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pHandle_log, pHandle, 2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
    case PHAL_I18000P3M3_SW_ID:
        status = phalI18000p3m3_Sw_SetHandle(
            (phalI18000p3m3_Sw_DataParams_t*)pDataParams,
            pHandle
            );
        break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHAL_I18000P3M3 */
