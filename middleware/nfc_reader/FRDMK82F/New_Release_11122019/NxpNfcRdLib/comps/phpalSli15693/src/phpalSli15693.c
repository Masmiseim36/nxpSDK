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
* Generic Sli15693 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 21. October 2009
*
*/

#include <phpalSli15693.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_SLI15693_SW
#include "Sw/phpalSli15693_Sw.h"
#endif /* NXPBUILD__PHPAL_SLI15693_SW */



#ifdef NXPBUILD__PHPAL_SLI15693

#ifndef  NXPRDLIB_REM_GEN_INTFS

phStatus_t phpalSli15693_Inventory(
                                   void * pDataParams,
                                   uint8_t bFlags,
                                   uint8_t bAfi,
                                   uint8_t * pMask,
                                   uint8_t bMaskBitLength,
                                   uint8_t * pDsfid,
                                   uint8_t * pUid
)
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_Inventory");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pDsfid);
    PH_ASSERT_NULL (pUid);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_Inventory(
            (phpalSli15693_Sw_DataParams_t*)pDataParams,
            bFlags,
            bAfi,
            pMask,
            bMaskBitLength,
            pDsfid,
            pUid);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 8);
    }
#endif /* NXPBUILD__PH_LOG */
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_SendEof(
                                 void * pDataParams,
                                 uint8_t bOption,
                                 uint8_t * pDsfid,
                                 uint8_t * pUid,
                                 uint8_t * pUidLength,
                                 uint8_t * pData,
                                 uint16_t * pDataLength
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_NextSlot");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pDsfid);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_SendEof(
            (phpalSli15693_Sw_DataParams_t*)pDataParams,
            bOption,
            pDsfid,
            pUid,
            pUidLength,
            pData,
            pDataLength);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */




    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        if (bOption==PHPAL_SLI15693_EOF_NEXT_SLOT)
        {
            PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, 8);
        }
        else
        {
            PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, 1);
        }
    }
#endif /* NXPBUILD__PH_LOG */

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phpalSli15693_StayQuiet(
                                   void * pDataParams
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_StayQuiet");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_StayQuiet((phpalSli15693_Sw_DataParams_t*)pDataParams);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phpalSli15693_Select(
                                void * pDataParams
                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_Select");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_Select((phpalSli15693_Sw_DataParams_t*)pDataParams );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phpalSli15693_ResetToReady(
                                      void * pDataParams
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_ResetToReady");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_ResetToReady((phpalSli15693_Sw_DataParams_t*)pDataParams );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_Exchange(
                                  void * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxLength
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_Exchange");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (wTxLength)) PH_ASSERT_NULL (pTxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_Exchange((phpalSli15693_Sw_DataParams_t*)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS) &&
        (0U == ((wOption & PH_EXCHANGE_BUFFERED_BIT))) &&
        (ppRxBuffer != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_ActivateCard(
                                      void *   pDataParams,
                                      uint8_t  bOption,
                                      uint8_t  bFlags,
                                      uint8_t  bAfi,
                                      uint8_t * pMask,
                                      uint8_t  bMaskBitLength,
                                      uint8_t * pDsfid,
                                      uint8_t * pUid,
                                      uint8_t * pMoreCardsAvailable
                                   )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_ActivateCard");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMoreCardsAvailable);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pDsfid);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pMoreCardsAvailable);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_ActivateCard(
            (phpalSli15693_Sw_DataParams_t*)pDataParams,
            bOption,
            bFlags,
            bAfi,
            pMask,
            bMaskBitLength,
            pDsfid,
            pUid,
            pMoreCardsAvailable);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 8);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pMoreCardsAvailable_log, pMoreCardsAvailable);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_GetSerialNo(
                                     void * pDataParams,
                                     uint8_t * pUid,
                                     uint8_t *pUidLength
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_GetSerialNo");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_GetSerialNo((phpalSli15693_Sw_DataParams_t*)pDataParams, pUid, pUidLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 8);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_SetSerialNo(
                                     void * pDataParams,
                                     uint8_t * pUid,
                                     uint8_t bUidLength
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_SetSerialNo");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUidLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 8);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUidLength_log, &bUidLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pUid);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_SetSerialNo((phpalSli15693_Sw_DataParams_t*)pDataParams, pUid, bUidLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_SetConfig(
                                   void * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t wValue
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_SetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_SetConfig((phpalSli15693_Sw_DataParams_t*)pDataParams, wConfig, wValue );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phpalSli15693_GetConfig(
                                   void * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_GetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_GetConfig((phpalSli15693_Sw_DataParams_t*)pDataParams, wConfig, pValue );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
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

phStatus_t phpalSli15693_InventoryRead(
                                       void * pDataParams,
                                       uint8_t bFlags,
                                       uint8_t bAfi,
                                       uint8_t * pMask,
                                       uint8_t bMaskBitLength,
                                       uint8_t bBlockNo,
                                       uint16_t wNoOfBlocks,
                                       uint8_t * pUid,
                                       uint8_t * pUidLength,
                                       uint8_t * pData,
                                       uint16_t * pDataLength
                                       )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_InventoryRead");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNoOfBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNoOfBlocks_log, &wNoOfBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_InventoryRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pUid, pUidLength, pData, pDataLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, *pUidLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, *pDataLength);
    }
#endif /* NXPBUILD__PH_LOG */
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_InventoryReadExtended(
                                           void * pDataParams,
                                           uint8_t bFlags,
                                           uint8_t bAfi,
                                           uint8_t * pMask,
                                           uint8_t bMaskBitLength,
                                           uint8_t bExtendedOptions,
                                           uint8_t * pCID,
                                           uint8_t bBlockNo,
                                           uint16_t wNoOfBlocks,
                                           uint8_t * pCIDOut,
                                           uint8_t * pUid,
                                           uint8_t * pUidLength,
                                           uint8_t * pData,
                                           uint16_t * pDataLength
                                           )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_InventoryReadExtended");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bExtendedOptions);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCID);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNoOfBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCIDOut);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bExtendedOptions_log, &bExtendedOptions);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNoOfBlocks_log, &wNoOfBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pCIDOut);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_InventoryReadExtended((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi,pMask, bMaskBitLength, bExtendedOptions, pCID, bBlockNo, wNoOfBlocks, pCIDOut, pUid, pUidLength, pData, pDataLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, *pUidLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, *pDataLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_FastInventoryRead(
    void * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bBlockNo,
    uint16_t wNoOfBlocks,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * pDataLength
    )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_FastInventoryRead");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNoOfBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNoOfBlocks_log, &wNoOfBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_FastInventoryRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pUid, pUidLength, pData, pDataLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, *pUidLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, *pDataLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_FastInventoryReadExtended(
                                           void * pDataParams,        /**< [In] Pointer to this layers parameter structure. */
                                           uint8_t bFlags,            /**< [In] Request flags byte. */
                                           uint8_t bAfi,              /**< [In] Application Family Identifier. */
                                           uint8_t * pMask,           /**< [In] UID mask, holding known UID bits. */
                                           uint8_t bMaskBitLength,    /**< [In] Number of UID bits within pMask. */
                                           uint8_t bExtendedOptions,  /**< [In] Extended options */
                                           uint8_t * pCID,            /**< [In] Two byte CID -> if marked in extended options */
                                           uint8_t bBlockNo,          /**< [In] Block Number from where start reading */
                                           uint16_t wNumOfBlocks,     /**< [In] Number of blocks to read */
                                           uint8_t * pCIDOut,        /**< [Out] Received UID. */
                                           uint8_t * pUid,            /**< [Out] Received UID. */
                                           uint8_t * pUidLength,      /**< [Out] Received UID Length. */
                                           uint8_t * pData,           /**< [Out] Received Data. */
                                           uint16_t * pDataLength     /**< [Out] Received Data Length. */
                                           )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_FastInventoryReadExtended");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bExtendedOptions);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCID);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNumOfBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCIDOut);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bExtendedOptions_log, &bExtendedOptions);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNumOfBlocks_log, &wNumOfBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pCIDOut);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_FastInventoryReadExtended(
            (phpalSli15693_Sw_DataParams_t*)pDataParams,
            bFlags,
            bAfi,
            pMask,
            bMaskBitLength,
            bExtendedOptions,
            pCID,
            bBlockNo,
            wNumOfBlocks,
            pCIDOut,
            pUid,
            pUidLength,
            pData,
            pDataLength);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, *pUidLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, *pDataLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phpalSli15693_InventoryPageRead(
    void * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bPageNo,
    uint16_t wNoOfPages,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * pDataLength
    )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_FastInventoryRead");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPageNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNoOfPages);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bPageNo_log, &bPageNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNoOfPages_log, &wNoOfPages);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_InventoryPageRead(
            (phpalSli15693_Sw_DataParams_t*)pDataParams,
            bFlags,
            bAfi,
            pMask,
            bMaskBitLength,
            bPageNo,
            wNoOfPages,
            pUid,
            pUidLength,
            pData,
            pDataLength);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, *pUidLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, *pDataLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_FastInventoryPageRead(
    void * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bPageNo,
    uint16_t wNoOfPages,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * pDataLength
    )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint8_t    PH_MEMLOC_REM bMaskByteLength;
#endif

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_FastInventoryRead");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFlags);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMask);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPageNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNoOfPages);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFlags_log, &bFlags);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAfi_log, &bAfi);
#ifdef NXPBUILD__PH_LOG
    bMaskByteLength = bMaskBitLength >> 3U;
    /* check for incomplete bytes */
    if (0U != (bMaskBitLength & 0x07U))
    {
        ++bMaskByteLength;
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMask_log, pMask, bMaskByteLength);
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bPageNo_log, &bPageNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNoOfPages_log, &wNoOfPages);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bMaskBitLength)) PH_ASSERT_NULL (pMask);
    PH_ASSERT_NULL (pUid);
    PH_ASSERT_NULL (pUidLength);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_FastInventoryPageRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bPageNo, wNoOfPages, pUid, pUidLength, pData, pDataLength );
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, *pUidLength);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, *pDataLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalSli15693_StayQuietPersistent(
                                             void * pDataParams
                                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalSli15693_StayQuietPersistent");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_SLI15693)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_SLI15693_SW
    case PHPAL_SLI15693_SW_ID:
        status = phpalSli15693_Sw_StayQuietPersistent((phpalSli15693_Sw_DataParams_t*)pDataParams);
        break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_SLI15693 */
