/*
*                     Copyright 2009-2019, NXP
*
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
* Generic HAL Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
*/

#include <phhalHw.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_RC663
#include "Rc663/phhalHw_Rc663.h"
#include "../intfs/phhalHw_Rc663_Cmd.h"
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
#include "PN7462AU/phhalHw_PN7462AU.h"
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */


#ifdef NXPBUILD__PHHAL_HW_PN5180
#include "Pn5180/phhalHw_Pn5180.h"
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
#include "Pn5190/phhalHw_Pn5190.h"
#endif /* NXPBUILD__PHHAL_HW_PN5190 */



#ifdef NXPBUILD__PHHAL_HW



#ifndef _WIN32
phStatus_t phhalHw_Nfc_IC_Init(
                              phhalHw_Nfc_Ic_DataParams_t  * pDataParams,
                              uint16_t wSizeOfDataParams,
                              void * pBalDataParams,
                              void * pKeyStoreDataParams,
                              uint8_t * pTxBuffer,
                              uint16_t wTxBufSize,
                              uint8_t * pRxBuffer,
                              uint16_t wRxBufSize
                              )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Nfc_IC_Init");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);


#ifdef NXPBUILD__PHHAL_HW_RC663
    /* Initialize the Reader HAL (Hardware Abstraction Layer) component */
    status = phhalHw_Rc663_Init(
        &pDataParams->sHal,
        wSizeOfDataParams,
        pBalDataParams,
        pKeyStoreDataParams,
        pTxBuffer,
        wTxBufSize,
        pRxBuffer,
        wRxBufSize
        );
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    /* Initialize the Reader HAL (Hardware Abstraction Layer) component */
    status = phhalHw_PN7462AU_Init(
        (phhalHw_PN7462AU_DataParams_t*)&pDataParams->sHal,
        wSizeOfDataParams,
        pBalDataParams,
        pKeyStoreDataParams,
        pTxBuffer,
        wTxBufSize,
        pRxBuffer,
        wRxBufSize
        );
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    /* Initialize the Reader HAL (Hardware Abstraction Layer) component */
    status = phhalHw_Pn5180_Init(
        &pDataParams->sHal,
        wSizeOfDataParams,
        pBalDataParams,
        pKeyStoreDataParams,
        pTxBuffer,
        wTxBufSize,
        pRxBuffer,
        wRxBufSize
        );
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    /* Initialize the Reader HAL (Hardware Abstraction Layer) component */
    status = phhalHw_Pn5190_Init(
                                &pDataParams->sHal,
                                sizeof(phhalHw_Pn5190_DataParams_t),
                                pBalDataParams,
                                pKeyStoreDataParams,
                                pTxBuffer,
                                wTxBufSize,
                                pRxBuffer,
                                wRxBufSize
                                );
#endif /* NXPBUILD__PHHAL_HW_PN5190 */
    PH_UNUSED_VARIABLE(wSizeOfDataParams);

    return status;
}
#endif

#ifdef _WIN32
phStatus_t phhalHw_WriteRegister(
                                 void * pDataParams,
                                 uint8_t bAddress,
                                 uint8_t bValue
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_WriteRegister");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddress);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddress_log, &bAddress);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bValue_log, &bValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_WriteRegister((phhalHw_Rc663_DataParams_t *)pDataParams, bAddress, bValue);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#if 0



#endif
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_ReadRegister(
                                void * pDataParams,
                                uint8_t bAddress,
                                uint8_t * pValue
                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_ReadRegister");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddress);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddress_log, &bAddress);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_ReadRegister((phhalHw_Rc663_DataParams_t *)pDataParams, bAddress, pValue);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#if 0

#endif

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* ifdef _WIN32 */

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phhalHw_Exchange(
                            void * pDataParams,
                            uint16_t wOption,
                            uint8_t * pTxBuffer,
                            uint16_t wTxLength,
                            uint8_t ** ppRxBuffer,
                            uint16_t * pRxLength
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Exchange");
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
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_Exchange((phhalHw_Rc663_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_Exchange((phhalHw_PN7462AU_DataParams_t *)pDataParams,
                                            wOption,
                                            pTxBuffer,
                                            wTxLength,
                                            ppRxBuffer,
                                            pRxLength
                                            );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */


#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_Exchange((phhalHw_Pn5180_DataParams_t*)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength);
        break;
#endif  /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_Exchange((phhalHw_Pn5190_DataParams_t*)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength);
        break;
#endif  /* NXPBUILD__PHHAL_HW_PN5190 */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
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

phStatus_t phhalHw_ApplyProtocolSettings(
    void * pDataParams,
    uint8_t bMode
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_ApplyProtocolSettings");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMode_log, &bMode);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_ApplyProtocolSettings((phhalHw_Rc663_DataParams_t *)pDataParams, bMode);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_ApplyProtocolSettings((phhalHw_PN7462AU_DataParams_t *) pDataParams,
                                                        bMode
                                                        );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */
#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_ApplyProtocolSettings((phhalHw_Pn5180_DataParams_t *)pDataParams, bMode);
        break;
#endif  /* NXPBUILD__PHHAL_HW_PN5180 */
#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_ApplyProtocolSettings((phhalHw_Pn5190_DataParams_t *)pDataParams, bMode);
        break;
#endif  /* NXPBUILD__PHHAL_HW_PN5190 */




    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phhalHw_SetConfigMultiple(
    void * pDataParams,
    uint16_t wNumOfConfigs,
    uint16_t (*arr2dConfig)[PHHAL_HW_ARRY_COLUMNS_SIZE]
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_SetConfigMultiple");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNumOfConfigs);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(arrConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNumOfConfigs_log, &wNumOfConfigs);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (arr2dConfig);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_SetConfigMultiple((phhalHw_Rc663_DataParams_t *)pDataParams,
                                                    wNumOfConfigs, arr2dConfig);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}



phStatus_t phhalHw_SetConfig(
                             void * pDataParams,
                             uint16_t wConfig,
                             uint16_t wValue
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_SetConfig");
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
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_SetConfig((phhalHw_Rc663_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_SetConfig((phhalHw_PN7462AU_DataParams_t *) pDataParams,
                                             wConfig,
                                             wValue
                                             );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_SetConfig((phhalHw_Pn5180_DataParams_t*)pDataParams, wConfig, wValue);
        break;
#endif  /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_SetConfig((phhalHw_Pn5190_DataParams_t*)pDataParams, wConfig, wValue);
        break;
#endif  /* NXPBUILD__PHHAL_HW_PN5190 */




    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_MfcAuthenticateKeyNo(
                                        void * pDataParams,
                                        uint8_t bBlockNo,
                                        uint8_t bKeyType,
                                        uint16_t wKeyNo,
                                        uint16_t wKeyVersion,
                                        uint8_t * pUid
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_MfcAuthenticateKeyNo");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyType_log, &bKeyType);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pUid);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_MfcAuthenticateKeyNo(
            (phhalHw_Rc663_DataParams_t *)pDataParams,
            bBlockNo,
            bKeyType,
            wKeyNo,
            wKeyVersion,
            pUid);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN5180
        case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_MfcAuthenticateKeyNo(
            (phhalHw_Pn5180_DataParams_t *)pDataParams,
            bBlockNo,
            bKeyType,
            wKeyNo,
            wKeyVersion,
            pUid);
         break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
        case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_MfcAuthenticateKeyNo(
            (phhalHw_Pn5190_DataParams_t *)pDataParams,
            bBlockNo,
            bKeyType,
            wKeyNo,
            wKeyVersion,
            pUid);
         break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_MfcAuthenticateKeyNo(
                                            (phhalHw_PN7462AU_DataParams_t *)pDataParams,
                                            bBlockNo,
                                            bKeyType,
                                            wKeyNo,
                                            wKeyVersion,
                                            pUid);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_MfcAuthenticate(
                                   void * pDataParams,
                                   uint8_t bBlockNo,
                                   uint8_t bKeyType,
                                   uint8_t * pKey,
                                   uint8_t * pUid
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_MfcAuthenticate");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyType_log, &bKeyType);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKey_log, pKey, 6);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pKey);
    PH_ASSERT_NULL (pUid);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_MfcAuthenticate(
            (phhalHw_Rc663_DataParams_t *)pDataParams,
            bBlockNo,
            bKeyType,
            pKey,
            pUid);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_MfcAuthenticate((phhalHw_PN7462AU_DataParams_t *)pDataParams,
                                                   bBlockNo,
                                                   bKeyType,
                                                   pKey,
                                                   pUid
                                                   );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_MfcAuthenticate((phhalHw_Pn5180_DataParams_t *)pDataParams,
                bBlockNo, bKeyType, pKey, pUid);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_MfcAuthenticate((phhalHw_Pn5190_DataParams_t *)pDataParams,
                bBlockNo, bKeyType, pKey, pUid);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_GetConfig(
                             void * pDataParams,
                             uint16_t wConfig,
                             uint16_t * pValue
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_GetConfig");
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
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
        case PHHAL_HW_RC663_ID:
            status = phhalHw_Rc663_GetConfig((phhalHw_Rc663_DataParams_t *)pDataParams, wConfig, pValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_GetConfig((phhalHw_PN7462AU_DataParams_t *) pDataParams,
                                            wConfig,
                                            pValue
                                            );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
        case PHHAL_HW_PN5180_ID:
            status = phhalHw_Pn5180_GetConfig((phhalHw_Pn5180_DataParams_t *)pDataParams, wConfig, pValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
        case PHHAL_HW_PN5190_ID:
            status = phhalHw_Pn5190_GetConfig((phhalHw_Pn5190_DataParams_t *)pDataParams, wConfig, pValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */




        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_FieldOn(
                           void * pDataParams
                           )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_FieldOn");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_FieldOn((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_FieldOn((phhalHw_PN7462AU_DataParams_t *) pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_FieldOn((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_FieldOn((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */




    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_FieldOff(
                            void * pDataParams
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_FieldOff");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_FieldOff((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_FieldOff((phhalHw_PN7462AU_DataParams_t *) pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_FieldOff((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_FieldOff((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */




    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_FieldReset(
                              void * pDataParams
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_FieldReset");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_FieldReset((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */










#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_FieldReset((phhalHw_PN7462AU_DataParams_t *) pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_FieldReset((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_FieldReset((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_Wait(
                        void * pDataParams,
                        uint8_t bUnit,
                        uint16_t wTimeout
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Wait");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUnit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wTimeout);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUnit_log, &bUnit);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wTimeout_log, &wTimeout);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_Wait((phhalHw_Rc663_DataParams_t *)pDataParams, bUnit, wTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */











#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_Wait((phhalHw_PN7462AU_DataParams_t *) pDataParams,
                                        bUnit,
                                        wTimeout
                                        );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */


#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_Wait((phhalHw_Pn5180_DataParams_t *) pDataParams, bUnit, wTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_Wait((phhalHw_Pn5190_DataParams_t *) pDataParams, bUnit, wTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_Transmit(
                           void * pDataParams,
                           uint16_t wOption,
                           uint8_t * pTxBuffer,
                           uint16_t wTxLength
                           )
{

    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Transmit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (wTxLength)) PH_ASSERT_NULL (pTxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_Transmit((phhalHw_Rc663_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */



#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_Transmit((phhalHw_PN7462AU_DataParams_t *)pDataParams,
                                            wOption,
                                            pTxBuffer,
                                            wTxLength
                                            );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_Transmit((phhalHw_Pn5180_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_Transmit((phhalHw_Pn5190_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_Receive(
                           void * pDataParams,
                           uint16_t wOption,
                           uint8_t ** ppRxBuffer,
                           uint16_t * pRxLength
                           )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Receive");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_Receive((phhalHw_Rc663_DataParams_t *)pDataParams, wOption, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_Receive((phhalHw_PN7462AU_DataParams_t *)pDataParams,
                                           wOption,
                                           ppRxBuffer,
                                           pRxLength
                                           );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_Receive((phhalHw_Pn5180_DataParams_t *)pDataParams, wOption, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_Receive((phhalHw_Pn5190_DataParams_t *)pDataParams, wOption, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
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

phStatus_t phhalHw_Autocoll(
                            void * pDataParams,
                            uint16_t wMode,
                            uint8_t ** ppRxBuffer,
                            uint16_t * pRxLength,
                            uint16_t * pProtParams
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Autocoll");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wMode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wMode_log, &wMode);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        /* RC663 doesn't support Target Mode */
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_HAL);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_Autocoll((phhalHw_PN7462AU_DataParams_t *)pDataParams,
                                            wMode,
                                            ppRxBuffer,
                                            pRxLength,
                                            pProtParams
                                            );
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_Autocoll((phhalHw_Pn5180_DataParams_t *)pDataParams, wMode, ppRxBuffer, pRxLength, pProtParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_Autocoll((phhalHw_Pn5190_DataParams_t *)pDataParams, wMode, ppRxBuffer, pRxLength, pProtParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS &&
        (ppRxBuffer != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phhalHw_Lpcd(
                        void * pDataParams
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Lpcd");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

     /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_Cmd_Lpcd((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_Lpcd((phhalHw_PN7462AU_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU*/

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_Lpcd((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_Lpcd((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_AsyncAbort(
                              void * pDataParams
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_AsyncAbort");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_AsyncAbort((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_AsyncAbort((phhalHw_PN7462AU_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_AsyncAbort((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_AsyncAbort((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_I18000p3m3Inventory(
                                       void * pDataParams,
                                       uint8_t * pSelCmd,
                                       uint8_t bSelCmdLen,
                                       uint8_t bNumValidBitsinLastByte,
                                       uint8_t * pBeginRndCmd,
                                       uint8_t bTSprocessing,
                                       uint8_t ** ppRxBuffer,
                                       uint16_t * wRxBufferLen
                                       )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_I18000p3m3Inventory");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSelCmd);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumValidBitsinLastByte);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTSprocessing);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pSelCmd);
    PH_ASSERT_NULL (pBeginRndCmd);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (wRxBufferLen);

    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSelCmd_log, pSelCmd, bSelCmdLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumValidBitsinLastByte_log, &bNumValidBitsinLastByte);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTSprocessing_log, &bTSprocessing);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_I18000p3m3Inventory(
            (phhalHw_Rc663_DataParams_t *)pDataParams,
            pSelCmd,
            bSelCmdLen,
            bNumValidBitsinLastByte,
            pBeginRndCmd,
            bTSprocessing,
            ppRxBuffer,
            wRxBufferLen);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */
#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_I18000p3m3Inventory((phhalHw_PN7462AU_DataParams_t  *) pDataParams,
                                                        pSelCmd,
                                                        bSelCmdLen,
                                                        bNumValidBitsinLastByte,
                                                        pBeginRndCmd,
                                                        bTSprocessing,
                                                        ppRxBuffer,
                                                        wRxBufferLen);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */
    /* TBD Add PN518 Inventory HAL function */
    /* case PHHAL_HW_PN518_ID: */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_I18000p3m3Inventory((phhalHw_Pn5180_DataParams_t *)pDataParams,
            pSelCmd,
            bSelCmdLen,
            bNumValidBitsinLastByte,
            pBeginRndCmd,
            bTSprocessing,
            ppRxBuffer,
            wRxBufferLen);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_I18000p3m3Inventory((phhalHw_Pn5190_DataParams_t *)pDataParams,
            pSelCmd,
            bSelCmdLen,
            bNumValidBitsinLastByte,
            pBeginRndCmd,
            bTSprocessing,
            ppRxBuffer,
            wRxBufferLen);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    return status;
}

phStatus_t phhalHw_I18000p3m3ResumeInventory(
                                             void * pDataParams,
                                             uint8_t ** ppRxBuffer,
                                             uint16_t * wRxBufferLen
                                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_I18000p3m3ResumeInventory");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (wRxBufferLen);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_I18000p3m3ResumeInventory(
            (phhalHw_Rc663_DataParams_t *)pDataParams,
            ppRxBuffer,
            wRxBufferLen);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#ifdef NXPBUILD__PHHAL_HW_PN5180
         case PHHAL_HW_PN5180_ID:
             status = phhalHw_Pn5180_18000p3m3ResumeInventory((phhalHw_Pn5180_DataParams_t *)pDataParams,
                 ppRxBuffer,
                 wRxBufferLen);

        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
         case PHHAL_HW_PN5190_ID:
             status = phhalHw_Pn5190_18000p3m3ResumeInventory((phhalHw_Pn5190_DataParams_t *)pDataParams,
                 ppRxBuffer,
                 wRxBufferLen);

        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_I18000p3m3ResumeInventory((phhalHw_PN7462AU_DataParams_t  *) pDataParams,
                                                            ppRxBuffer,
                                                            wRxBufferLen);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    return status;
}

phStatus_t phhalHw_EventWait(
                            void * pDataParams,
                            uint32_t dwEventTimeout
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_EventWait");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_EventWait((phhalHw_Rc663_DataParams_t *)pDataParams, dwEventTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_EventWait((phhalHw_PN7462AU_DataParams_t *)pDataParams, dwEventTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_EventWait((phhalHw_Pn5180_DataParams_t *)pDataParams, dwEventTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_EventWait((phhalHw_Pn5190_DataParams_t *)pDataParams, dwEventTimeout);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_EventConsume(
                                void * pDataParams
                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_EventConsume");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_EventConsume((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_EventConsume((phhalHw_PN7462AU_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_EventConsume((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_EventConsume((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phhalHw_DeInit(
                          void * pDataParams
                          )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_DeInit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHHAL_HW_RC663_ID:
        status = phhalHw_Rc663_DeInit((phhalHw_Rc663_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */


#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    case PHHAL_HW_PN7462AU_ID:
        status = phhalHw_PN7462AU_DeInit((phhalHw_PN7462AU_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHHAL_HW_PN5180_ID:
        status = phhalHw_Pn5180_DeInit((phhalHw_Pn5180_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
    case PHHAL_HW_PN5190_ID:
        status = phhalHw_Pn5190_DeInit((phhalHw_Pn5190_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHHAL_HW */
