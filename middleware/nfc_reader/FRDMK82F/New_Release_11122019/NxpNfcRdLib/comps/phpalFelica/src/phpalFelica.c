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
* Generic FeliCa Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 26. October 2010
*
*/

#include <phpalFelica.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_FELICA_SW
#include "Sw/phpalFelica_Sw.h"
#endif /* NXPBUILD__PHPAL_FELICA_SW */



#ifdef NXPBUILD__PHPAL_FELICA

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phpalFelica_ReqC(
                            void* pDataParams,
                            uint8_t* pSystemCode,
                            uint8_t bTimeSlot,
                            uint8_t ** ppRxBuffer,
                            uint16_t * pRxLength
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_ReqC");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSystemCode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTimeSlot);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pSystemCode_log, pSystemCode, 2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bTimeSlot_log, &bTimeSlot);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pSystemCode);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_ReqC((phpalFelica_Sw_DataParams_t *)pDataParams, pSystemCode, bTimeSlot, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxBuffer_log, ppRxBuffer, PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalFelica_ActivateCard(
                                    void* pDataParams,
                                    uint8_t * pIDmPMm,
                                    uint8_t bIDmPMmLength,
                                    uint8_t* pSystemCode,
                                    uint8_t bTimeSlot,
                                    uint8_t* pRxBuffer,
                                    uint8_t* pRxLength,
                                    uint8_t* pMoreCardsAvailable
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_ActivateCard");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIDmPMm);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIDmPMmLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSystemCode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTimeSlot);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMoreCardsAvailable);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pSystemCode_log, pSystemCode, 2);
    if (0U != (bIDmPMmLength)) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pIDmPMm_log, pIDmPMm, bIDmPMmLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bIDmPMmLength_log, &bIDmPMmLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bTimeSlot_log, &bTimeSlot);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (bIDmPMmLength)) PH_ASSERT_NULL (pIDmPMm);
    PH_ASSERT_NULL (pSystemCode);
    PH_ASSERT_NULL (pRxBuffer);
    PH_ASSERT_NULL (pRxLength);
    PH_ASSERT_NULL (pMoreCardsAvailable);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_ActivateCard( (phpalFelica_Sw_DataParams_t *)pDataParams, pIDmPMm, bIDmPMmLength, pSystemCode, bTimeSlot, pRxBuffer, pRxLength, pMoreCardsAvailable );
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

/* PAL as DUT*/
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pRxBuffer_log, pRxBuffer, *pRxLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, pMoreCardsAvailable_log, pMoreCardsAvailable);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalFelica_Exchange(
                                void* pDataParams,
                                uint16_t wOption,
                                uint16_t wN,
                                uint8_t* pTxBuffer,
                                uint16_t wTxLength,
                                uint8_t** ppRxBuffer,
                                uint16_t* pRxLength
                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_Exchange");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wN);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wN_log, &wN);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (wTxLength)) PH_ASSERT_NULL (pTxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_Exchange( (phpalFelica_Sw_DataParams_t *)pDataParams, wOption, wN, pTxBuffer, wTxLength, ppRxBuffer, pRxLength );
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

/* PAL as DUT*/
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
        (0U == ((wOption & PH_EXCHANGE_BUFFERED_BIT))) &&
        (ppRxBuffer != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalFelica_SetConfig(
                                 void * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t wValue
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_SetConfig");
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
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_SetConfig((phpalFelica_Sw_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalFelica_GetConfig(
                                 void * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t * pValue
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_GetConfig");
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
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_GetConfig((phpalFelica_Sw_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalFelica_GetSerialNo(
                                   void* pDataParams,
                                   uint8_t* pIDmPMmOut,
                                   uint8_t* pLenIDmPMmOut
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_GetSerialNo");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIDmPMmOut);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pIDmPMmOut);
    PH_ASSERT_NULL (pLenIDmPMmOut);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_GetSerialNo( (phpalFelica_Sw_DataParams_t *)pDataParams, pIDmPMmOut, pLenIDmPMmOut );
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

/*PAL as DUT*/
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pIDmPMmOut_log, pIDmPMmOut, *pLenIDmPMmOut);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phpalFelica_SetSerialNo(
                                   void* pDataParams,
                                   uint8_t* pIDmPMm
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_SetSerialNo");
        PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pIDmPMm);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_SetSerialNo( (phpalFelica_Sw_DataParams_t *)pDataParams, pIDmPMm );
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t  phpalFelica_GetFrameInfo(
                                     void * pDataParams,
                                     uint8_t bFrameNum,
                                     uint8_t * pResponseBuffer,
                                     uint16_t *pwStatus,
                                     uint8_t** ppID,
                                     uint8_t * pLen
                                     )

{

    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phpalFelica_GetFrameInfo");

    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFrameNum);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pResponseBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pwStatus);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppID);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pResponseBuffer);
    PH_ASSERT_NULL (pwStatus);
    PH_ASSERT_NULL (ppID);
    PH_ASSERT_NULL (pLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, pResponseBuffer_log, pResponseBuffer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bFrameNum_log, &bFrameNum);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);


    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_PAL_FELICA)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHPAL_FELICA_SW
    case PHPAL_FELICA_SW_ID:
        status = phpalFelica_Sw_GetFrameInfo((phpalFelica_Sw_DataParams_t*) pDataParams, bFrameNum, pResponseBuffer, pwStatus, ppID, pLen);
        break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_FELICA);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pwStatus_log, pwStatus);
        if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS) &&
            (ppID != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppID_log, *ppID, *pLen);
    }
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, pLen_log, pLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;

}
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_FELICA */
