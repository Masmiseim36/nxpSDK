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
* Generic Virtual Card Architecture Application Component of Reader Library Framework.
* $Author: nxa34640 $
* $Revision: 2160 $ (v06.01.00)
* $Date: 2016-09-22 14:26:29 +0530 (Thu, 22 Sep 2016) $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <ph_Status.h>
#include <phalVca.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_VCA_SW
#include "Sw/phalVca_Sw.h"
#endif /* NXPBUILD__PHAL_VCA_SW */



#ifdef NXPBUILD__PHAL_VCA

phStatus_t phalVca_VcSupport(
        void * pDataParams,
        uint8_t * pIid,
        uint16_t wKeyEncNumber,
        uint16_t wKeyEncVersion,
        uint16_t wKeyMacNumber,
        uint16_t wKeyMacVersion
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_VcSupport");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIid_log, pIid, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncNumber_log, &wKeyEncNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncVersion_log, &wKeyEncVersion);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacNumber_log, &wKeyMacNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacVersion_log, &wKeyMacVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pIid);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_VcSupport((phalVca_Sw_DataParams_t *)pDataParams,pIid ,wKeyEncNumber, wKeyEncVersion, wKeyMacNumber, wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM

phStatus_t phalVca_VcSupportLast(
        void * pDataParams,
        uint8_t * pIid,
        uint8_t bLenCap,
        uint8_t * pPcdCapabilities,
        uint16_t wKeyEncNumber,
        uint16_t wKeyEncVersion,
        uint16_t wKeyMacNumber,
        uint16_t wKeyMacVersion
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_VcSupportLast");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLenCap);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCapabilities);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIid_log, pIid, 16);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLenCap_log, &bLenCap);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCapabilities_log, pPcdCapabilities, bLenCap);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncNumber_log, &wKeyEncNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncVersion_log, &wKeyEncVersion);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacNumber_log, &wKeyMacNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacVersion_log, &wKeyMacVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pIid);
    if (0U != (bLenCap)) PH_ASSERT_NULL (pPcdCapabilities);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_VcSupportLast(
            (phalVca_Sw_DataParams_t *)pDataParams,
            pIid,
            bLenCap,
            pPcdCapabilities,
            wKeyEncNumber,
            wKeyEncVersion,
            wKeyMacNumber,
            wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalVca_StartCardSelection (
        void * pDataParams
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_StartCardSelection");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_StartCardSelection(
            (phalVca_Sw_DataParams_t *)pDataParams
            );
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_FinalizeCardSelection (
        void * pDataParams,
        uint16_t * pNumValidIids
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_FinalizeCardSelection");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumValidIids);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pNumValidIids);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_FinalizeCardSelection(
            (phalVca_Sw_DataParams_t *)pDataParams,
            pNumValidIids
            );
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pNumValidIids_log, pNumValidIids);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_GetIidInfo(
        void * pDataParams,
        uint16_t wValidIidIndex,
        uint16_t * pIidIndex,
        uint8_t * pVcUidSize,
        uint8_t * pVcUid,
        uint8_t * pInfo,
        uint8_t * pPdCapabilities
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_GetIidInfo");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValidIidIndex);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIidIndex);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVcUidSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVcUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPdCapabilities);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValidIidIndex_log, &wValidIidIndex);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pIidIndex);
    PH_ASSERT_NULL (pVcUidSize);
    PH_ASSERT_NULL (pVcUid);
    PH_ASSERT_NULL (pInfo);
    PH_ASSERT_NULL (pPdCapabilities);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_GetIidInfo(
            (phalVca_Sw_DataParams_t *)pDataParams,
            wValidIidIndex,
            pIidIndex,
            pVcUidSize,
            pVcUid,
            pInfo,
            pPdCapabilities
            );
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ( (status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pIidIndex_log, pIidIndex);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pVcUidSize_log, pVcUidSize);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVcUid_log, pVcUid, *pVcUidSize);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pInfo_log, pInfo);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPdCapabilities_log, pPdCapabilities, 2);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalVca_SelectVc(
        void * pDataParams,
        uint16_t wValidIidIndex,
        uint16_t wKeyNumber,
        uint16_t wKeyVersion
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_SelectVc");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValidIidIndex);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValidIidIndex_log, &wValidIidIndex);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_SelectVc((phalVca_Sw_DataParams_t *)pDataParams, wValidIidIndex, wKeyNumber,wKeyVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_IsoSelect(
        void *    pDataParams,
        uint8_t   bSelectionControl,
        uint8_t   bOption,
        uint8_t   bDFnameLen,
        uint8_t*  pDFname,
        uint8_t*  pDivInput,
        uint8_t   bDivInputLen,
        uint8_t*  pKeys,
        uint8_t*  pFCI,
        uint16_t* pwFCILen,
        uint8_t*  pResponse
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_IsoSelect");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSelectionControl);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDFname);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDFnameLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFCI);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pwFCILen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivInputLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKeys);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pResponse);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSelectionControl_log, &bSelectionControl);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDFnameLen_log, &bDFnameLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivInputLen_log, &bDivInputLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFname_log, pDFname, bDFnameLen);

    if(pKeys != NULL)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKeys_log, pKeys, 0x04);
    }
    if(pDivInput != NULL)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, (uint16_t) bDivInputLen);
    }

    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_IsoSelect((phalVca_Sw_DataParams_t *)pDataParams,
            bSelectionControl,
            bOption,
            bDFnameLen,
            pDFname,
            pFCI,
            pwFCILen
            );
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if (*pwFCILen != 0U)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFCI_log, pFCI, (*pwFCILen));
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pwFCILen_log, pwFCILen);
    if(pResponse != NULL)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pResponse_log, pResponse, 24);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    PH_UNUSED_VARIABLE(bDivInputLen);
    return status;
}

phStatus_t phalVca_IsoExternalAuthenticate(
        void *     pDataParams,
        uint8_t*   pInData,
        uint16_t   wKeyNumber,
        uint16_t   wKeyVersion
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_IsoExternalAuthenticate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_IsoExternalAuthenticate((phalVca_Sw_DataParams_t *)pDataParams,
                            pInData,
                            wKeyNumber,
                            wKeyVersion
                            );
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */
phStatus_t phalVca_DeselectVc(
        void * pDataParams
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_DeselectVc");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_DeselectVc((phalVca_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_SetConfig(
        void * pDataParams,
        uint16_t wConfig,
        uint16_t wValue
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_SetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }
    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_SetConfig((phalVca_Sw_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalVca_GetConfig(
        void * pDataParams,
        uint16_t wConfig,
        uint16_t * pValue
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);
    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }
    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_GetConfig((phalVca_Sw_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
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
#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalVca_ProximityCheck(
        void * pDataParams,
        uint8_t bGenerateRndC,
        uint8_t * pRndC,
        uint8_t bPps1,
        uint8_t bNumSteps,
        uint16_t wKeyNumber,
        uint16_t wKeyVersion,
        uint8_t * pUsedRndC
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_ProximityCheck");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bGenerateRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPps1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumSteps);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUsedRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bGenerateRndC_log, &bGenerateRndC);
#ifdef NXPBUILD__PH_LOG
    if (0U == (bGenerateRndC))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRndC_log, pRndC, 7);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bPps1_log, &bPps1);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumSteps_log, &bNumSteps);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U == (bGenerateRndC)) PH_ASSERT_NULL (pRndC);
    PH_ASSERT_NULL (pUsedRndC);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_ProximityCheck((phalVca_Sw_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, wKeyNumber, wKeyVersion, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUsedRndC_log, pUsedRndC, 7);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_ProximityCheckNew(
        void    *pDataParams,
        uint8_t  bGenerateRndC,
        uint8_t *pPrndC,
        uint8_t  bNumSteps,
        uint16_t wKeyNumber,
        uint16_t wKeyVersion,
        uint8_t *pDivInput,
        uint8_t bDivInputLen,
        uint8_t *pOption,
        uint8_t *pPubRespTime,
        uint8_t *pPps1,
        uint8_t *pCumRndRC
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_ProximityCheckNew");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bGenerateRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPrndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPubRespTime);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPps1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumSteps);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivInputLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCumRndRC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bGenerateRndC_log, &bGenerateRndC);
#ifdef NXPBUILD__PH_LOG
    if (0U == (bGenerateRndC))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPrndC_log, pPrndC, 8);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOption_log, pOption, 1);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPubRespTime_log, pPubRespTime, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPps1_log, pPps1, 1);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumSteps_log, &bNumSteps);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivInputLen_log, &bDivInputLen);
    if(pDivInput != NULL)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, (uint16_t) bDivInputLen);
    }
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U == (bGenerateRndC)) PH_ASSERT_NULL (pPrndC);
    PH_ASSERT_NULL (pCumRndRC);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_ProximityCheckNew((phalVca_Sw_DataParams_t *)pDataParams, bGenerateRndC, pPrndC, bNumSteps, wKeyNumber, wKeyVersion, pOption, pPubRespTime, pPps1, pCumRndRC);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */




    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pCumRndRC_log, pCumRndRC, 16);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    PH_UNUSED_VARIABLE(bDivInputLen);
    return status;
}

phStatus_t phalVca_PrepareProximityCheckNew(
        void * pDataParams,
        uint8_t *bOption,
        uint8_t *pPubRespTime,
        uint8_t *pPPS
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_PrepareProximityCheckNew");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPubRespTime);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPPS);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bOption_log, bOption, 8);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPubRespTime_log, pPubRespTime, 2);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPPS_log, pPPS, 8);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_Cmd_PrepareProximityCheckNew((phalVca_Sw_DataParams_t *)pDataParams,
                            bOption,
                            pPubRespTime,
                            pPPS);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalVca_ExecuteProximityCheckNew(
        void * pDataParams,
        uint8_t bGenerateRndC,
        uint8_t * pPrndC,
        uint8_t bNumSteps,
        uint8_t * pPubRespTime,
        uint8_t * pCumRndRC
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_ExecuteProximityCheckNew");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bGenerateRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPrndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumSteps);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPubRespTime);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCumRndRC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bGenerateRndC_log, &bGenerateRndC);
#ifdef NXPBUILD__PH_LOG
    if (0U == (bGenerateRndC))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPrndC_log, pPrndC, 8);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumSteps_log, &bNumSteps);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPubRespTime_log, pPubRespTime, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pCumRndRC_log, pCumRndRC, 8);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U == (bGenerateRndC)) PH_ASSERT_NULL_PARAM (pPrndC, PH_COMP_AL_VCA);
    PH_ASSERT_NULL_PARAM (pCumRndRC, PH_COMP_AL_VCA);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_Cmd_ProximityCheckNew((phalVca_Sw_DataParams_t *)pDataParams, bGenerateRndC, pPrndC, bNumSteps, pPubRespTime, pCumRndRC);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pCumRndRC_log, pCumRndRC, 7);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalVca_VerifyProximityCheckNew(
        void *      pDataParams,
        uint8_t     bOption,
        uint8_t *   pPubRespTime,
        uint8_t     bPps1,
        uint16_t    wKeyNumber,
        uint16_t    wKeyVersion,
        uint8_t *   pRndCmdResp
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_VerifyProximityCheckNew");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPubRespTime);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPps1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRndCmdResp);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPubRespTime_log, pPubRespTime, 8);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRndCmdResp_log, pRndCmdResp, 8);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bPps1_log, &bPps1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_Cmd_VerifyProximityCheckNew((phalVca_Sw_DataParams_t *)pDataParams, bOption, pPubRespTime, bPps1, wKeyNumber, wKeyVersion, pRndCmdResp);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */
phStatus_t phalVca_VerifyProximityCheckUtility(
        void *    pDataParams,
        uint8_t *   pCmdMac,
        uint8_t *   pCmdResp
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_VerifyProximityCheckUtility");

    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCmdMac);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pCmdResp);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pCmdMac_log, pCmdMac, 8);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pCmdMac);
    PH_ASSERT_NULL (pCmdResp);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_Cmd_VerifyProximityCheckUtility((phalVca_Sw_DataParams_t *)pDataParams, pCmdMac, pCmdResp);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pCmdResp_log, pCmdResp, 8);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalVca_SetSessionKeyUtility(
        void *    pDataParams,
        uint8_t * pSessionKey,
        uint8_t   bAuthMode
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_SetSessionKeyUtility");

    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSessionKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAuthMode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAuthMode_log, &bAuthMode);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSessionKey_log, pSessionKey, 16);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pSessionKey);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_SetSessionKeyUtility((phalVca_Sw_DataParams_t *)pDataParams, pSessionKey, bAuthMode);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSessionKey_log, pSessionKey, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}
#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalVca_DecryptResponse(
        void *    pDataParams,
        uint16_t  wKeyNo,
        uint16_t  wKeyVersion,
        uint8_t * pInData,
        uint8_t * pRandChal,
        uint8_t * pVCData
    )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_DecryptResponse");

    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pInData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRandChal);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVCData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pInData_log, pInData, 32);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pInData);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_DecryptResponse((phalVca_Sw_DataParams_t *)pDataParams, wKeyNo, wKeyVersion, pInData, pRandChal, pVCData);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRandChal_log, pRandChal, 16);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVCData_log, pVCData, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */
phStatus_t phalVca_SetApplicationType(
                           void * pDataParams,
                           void * pAlDataParams
                           )
{
    phStatus_t PH_MEMLOC_REM status;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_SetApplicationType");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAlDataParams);
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_SetApplicationType((phalVca_Sw_DataParams_t *)pDataParams, pAlDataParams);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

#endif /* NXPBUILD__PHAL_VCA */

