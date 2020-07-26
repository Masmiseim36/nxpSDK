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
* Generic ISO18000-3 Mode3 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 27. January 2010
*
*/

#include <ph_Status.h>
#include <phpalI18000p3m3.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
#include "Sw/phpalI18000p3m3_Sw.h"
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


#ifdef NXPBUILD__PHPAL_I18000P3M3

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phpalI18000p3m3_Exchange(
                                    void * pDataParams,
                                    uint16_t wOption,
                                    uint8_t * pTxBuffer,
                                    uint16_t wTxLength,
                                    uint8_t bTxLastBits,
                                    uint8_t ** ppRxBuffer,
                                    uint16_t * pRxLength,
                                    uint8_t * pRxLastBits
                                    )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_Exchange");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTxLastBits);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxLastBits);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    if (0U != (wTxLength)) PH_ASSERT_NULL (pTxBuffer);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTxLastBits_log, &bTxLastBits);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_Exchange(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            wOption,
            pTxBuffer,
            wTxLength,
            bTxLastBits,
            ppRxBuffer,
            pRxLength,
            pRxLastBits);
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
        (0U == ((wOption & PH_EXCHANGE_BUFFERED_BIT))) &&
        (ppRxBuffer != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pRxLastBits_log, pRxLastBits);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalI18000p3m3_Select(
                                  void * pDataParams,
                                  uint8_t bTarget,
                                  uint8_t bAction,
                                  uint8_t bMemBank,
                                  uint8_t * pPointer,
                                  uint8_t bPointerLength,
                                  uint8_t * pMask,
                                  uint8_t bMaskBitLength,
                                  uint8_t bTruncate
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t     PH_MEMLOC_REM bMaskByteLength;
#endif
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_Select");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTarget);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAction);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPointer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMaskBitLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTruncate);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPointer);
    if (bMaskBitLength != 0U) PH_ASSERT_NULL (pMask);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTarget_log, &bTarget);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAction_log, &bAction);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPointer_log, pPointer, bPointerLength);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMaskBitLength_log, &bMaskBitLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTruncate_log, &bTruncate);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_Select(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            bTarget,
            bAction,
            bMemBank,
            pPointer,
            bPointerLength,
            pMask,
            bMaskBitLength,
            bTruncate);
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
phStatus_t phpalI18000p3m3_BeginRound(
                                      void * pDataParams,
                                      uint8_t bDr,
                                      uint8_t bM,
                                      uint8_t bTRext,
                                      uint8_t bSel,
                                      uint8_t bSession,
                                      uint8_t bRfu,
                                      uint8_t bQ,
                                      uint8_t * pStoredCRC
                                      )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_BeginRound");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bM);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTRext);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSel);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSession);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRfu);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bQ);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pStoredCRC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pStoredCRC);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDr_log, &bDr);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bM_log, &bM);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTRext_log, &bTRext);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSel_log, &bSel);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSession_log, &bSession);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRfu_log, &bRfu);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bQ_log, &bQ);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_BeginRound(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            bDr,
            bM,
            bTRext,
            bSel,
            bSession,
            bRfu,
            bQ,
            pStoredCRC);
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pStoredCRC_log, pStoredCRC, 2);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalI18000p3m3_ResizeRound(
                                       void * pDataParams,
                                       uint8_t bUpDn,
                                       uint8_t * pStoredCRC
                                       )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_ResizeRound");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUpDn);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pStoredCRC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pStoredCRC);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUpDn_log, &bUpDn);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_ResizeRound(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            bUpDn,
            pStoredCRC);
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pStoredCRC_log, pStoredCRC, 2);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalI18000p3m3_NextSlot(
                                    void * pDataParams,
                                    uint8_t * pStoredCRC
                                    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_NextSlot");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pStoredCRC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pStoredCRC);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_NextSlot(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            pStoredCRC);
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pStoredCRC_log, pStoredCRC, 2);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalI18000p3m3_GetSerialNo(
                               void * pDataParams,
                               uint8_t * pRxBuffer,
                               uint8_t * pRxLength
                                       )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_GetSerialNo");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pRxBuffer);
    PH_ASSERT_NULL (pRxLength);


    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_GetSerialNo(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            pRxBuffer,
            pRxLength
            );
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pRxLength_log, pRxLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxBuffer_log, pRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

phStatus_t phpalI18000p3m3_CreateSelectCmd(
                                        void * pDataParams,
                                         uint8_t bTarget,
                                         uint8_t bAction,
                                         uint8_t bMemBank,
                                         uint8_t * pPointer,
                                         uint8_t bPointerLength,
                                         uint8_t * pMask,
                                         uint8_t bMaskBitLength,
                                         uint8_t bTruncate,
                                         uint8_t * pSelectCmd,
                                         uint8_t * bLen,
                                         uint8_t * bValidBits
                                        )
{

    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_Select");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTarget);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAction);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMemBank);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPointer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMaskBitLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTruncate);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPointer);
    PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pSelectCmd);
    PH_ASSERT_NULL (bLen);
    PH_ASSERT_NULL (bValidBits);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTarget_log, &bTarget);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAction_log, &bAction);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMemBank_log, &bMemBank);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMaskBitLength_log, &bMaskBitLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTruncate_log, &bTruncate);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pPointer_log, pPointer);


    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_CreateSelectCmd(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            bTarget,
            bAction,
            bMemBank,
            pPointer,
            bPointerLength,
            pMask,
            bMaskBitLength,
            bTruncate,
            pSelectCmd,
            bLen,
            bValidBits
            );
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;


}

phStatus_t phpalI18000p3m3_CreateBeginRoundCmd(
                                    void * pDataParams,
                                    uint8_t bDr,
                                    uint8_t bM,
                                    uint8_t bTRext,
                                    uint8_t bSel,
                                    uint8_t bSession,
                                    uint8_t bRfu,
                                    uint8_t bQ,
                                    uint8_t * pBeginRnd
                                    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_BeginRound");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bM);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTRext);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSel);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSession);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRfu);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bQ);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBeginRnd);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pBeginRnd);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDr_log, &bDr);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bM_log, &bM);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTRext_log, &bTRext);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSel_log, &bSel);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSession_log, &bSession);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRfu_log, &bRfu);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bQ_log, &bQ);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pBeginRnd_log, pBeginRnd);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
     if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
     {
         status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

         PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
         PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
         PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

         return status;
     }

        /* perform operation on active layer */
        switch (PH_GET_COMPID(pDataParams))
        {
    #ifdef NXPBUILD__PHPAL_I18000P3M3_SW
        case PHPAL_I18000P3M3_SW_ID:
            status = phpalI18000p3m3_Sw_CreateBeginRoundCmd(
                (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
                bDr,
                bM,
                bTRext,
                bSel,
                bSession,
                bRfu,
                bQ,
                pBeginRnd);
            break;
    #endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
            break;
        }
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        return status;
}




phStatus_t phpalI18000p3m3_Ack(
                               void * pDataParams,
                               uint8_t bOption,
                               uint8_t * pHandle,
                               uint8_t ** ppRxBuffer,
                               uint16_t * pRxBitLength
                               )
{
    phStatus_t  PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint16_t    PH_MEMLOC_REM wBufferByteLength = 0;
#endif
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_Ack");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pHandle);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBitLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    if (bOption == PHPAL_I18000P3M3_ACK_USE_HANDLE) PH_ASSERT_NULL (pHandle);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pRxBitLength);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
#ifdef NXPBUILD__PH_LOG
    if (bOption == PHPAL_I18000P3M3_ACK_USE_HANDLE)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pHandle_log, pHandle, 2);
    }
#endif
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_Ack(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            bOption,
            pHandle,
            ppRxBuffer,
            pRxBitLength
            );
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        wBufferByteLength = *pRxBitLength >> 3U;
        /* check for incomplete bytes */
        if (0U != (*pRxBitLength & 0x07U))
        {
            ++wBufferByteLength;
        }
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pRxBitLength_log, pRxBitLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, wBufferByteLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalI18000p3m3_Nak(
                               void * pDataParams
                               )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_Nak");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_Nak((phpalI18000p3m3_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalI18000p3m3_ActivateCard(
        void * pDataParams,
        uint8_t * pSelCmd,
        uint8_t bSelCmdLen,
        uint8_t bNumValidBitsinLastByte,
        uint8_t * pBeginRndCmd,
        uint8_t bTSprocessing,
        uint8_t ** ppRxBuffer,
        uint16_t * pRxBufferLen,
        uint8_t * pMoreCardsAvailable
        )
{
    phStatus_t  PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_ActivateCard");

    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSelCmd);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumValidBitsinLastByte);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTSprocessing);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_ASSERT_NULL (pDataParams);
    if (bSelCmdLen != 0U) PH_ASSERT_NULL (pSelCmd);
    PH_ASSERT_NULL (pBeginRndCmd);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pRxBufferLen);
    PH_ASSERT_NULL (pMoreCardsAvailable);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSelCmd_log, pSelCmd, bSelCmdLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumValidBitsinLastByte_log, &bNumValidBitsinLastByte);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxBufferLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTSprocessing_log, &bTSprocessing);

      /* perform operation on active layer */
        switch (PH_GET_COMPID(pDataParams))
        {
    #ifdef NXPBUILD__PHPAL_I18000P3M3_SW
        case PHPAL_I18000P3M3_SW_ID:

            status = phpalI18000p3m3_Sw_ActivateCard(
                  (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
                  pSelCmd,
                  bSelCmdLen,
                  bNumValidBitsinLastByte,
                  pBeginRndCmd,
                  bTSprocessing,
                  ppRxBuffer,
                  pRxBufferLen,
                  pMoreCardsAvailable);

            break;
    #endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
            break;
        }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    return status;
}

phStatus_t phpalI18000p3m3_SetConfig(
                                     void * pDataParams,
                                     uint16_t wConfig,
                                     uint16_t wValue
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalI18000p3m3_SetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_ASSERT_NULL (pDataParams);


    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_I18000P3M3)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    case PHPAL_I18000P3M3_SW_ID:
        status = phpalI18000p3m3_Sw_SetConfig(
            (phpalI18000p3m3_Sw_DataParams_t *)pDataParams,
            wConfig,
            wValue
            );
        break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18000P3M3);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_I18000P3M3 */
