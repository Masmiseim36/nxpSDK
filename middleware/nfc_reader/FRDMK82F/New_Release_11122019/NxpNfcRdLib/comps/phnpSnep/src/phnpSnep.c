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
* Generic SNEP protocol management layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 05. Mar 2014
*
*/

#include <ph_Status.h>
#include <phnpSnep.h>

#ifdef NXPBUILD__PHNP_SNEP_SW
#include "Sw/phnpSnep_Sw.h"
#endif /* NXPBUILD__PHNP_SNEP_SW */

#ifdef NXPBUILD__PHNP_SNEP

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phnpSnep_ClientInit(
                               void * pDataParams,
                               phnpSnep_Server_type_t eServerType,
                               uint8_t *pNonDefaultUri,
                               uint8_t *pbSnepRxBuffer,
                               uint32_t dwSnepRxBufferSize
                               )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_ClientInit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pbSnepRxBuffer);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pbSnepRxBuffer_log, pbSnepRxBuffer, dwSnepRxBufferSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (dwSnepRxBufferSize)) PH_ASSERT_NULL (pbSnepRxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_ClientInit((phnpSnep_Sw_DataParams_t *) pDataParams, eServerType,
            pNonDefaultUri, pbSnepRxBuffer, dwSnepRxBufferSize);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phnpSnep_ServerInit(
                               void * pDataParams,
                               phnpSnep_Server_type_t eServerType,
                               uint8_t *pNonDefaultUri,
                               uint8_t *pbSnepRxBuffer,
                               uint32_t dwSnepRxBufferSize
                               )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_ServerInit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pbSnepRxBuffer);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pbSnepRxBuffer_log, pbSnepRxBuffer, dwSnepRxBufferSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (dwSnepRxBufferSize)) PH_ASSERT_NULL (pbSnepRxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_ServerInit((phnpSnep_Sw_DataParams_t *) pDataParams, eServerType,
            pNonDefaultUri, pbSnepRxBuffer, dwSnepRxBufferSize);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phnpSnep_Put(
                        void * pDataParams,
                        uint8_t * pTxBuffer,
                        uint32_t dwTxBufferSize
                        )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_Put");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, dwTxBufferSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (dwTxBufferSize)) PH_ASSERT_NULL (pTxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_Put((phnpSnep_Sw_DataParams_t *) pDataParams, pTxBuffer, dwTxBufferSize);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phnpSnep_Get(
                        void * pDataParams,
                        uint8_t *pNdefMsg,
                        uint32_t dwNdefMsgLen,
                        uint8_t *pRxBuffer,
                        uint32_t *pdwRxLen,
                        uint32_t dwAppBufSize
                        )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_Get");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNdefMsg);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pdwRxLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pNdefMsg_log, pNdefMsg, dwNdefMsgLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxBuffer_log, pRxBuffer, dwAppBufSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != (dwNdefMsgLen)) PH_ASSERT_NULL (pNdefMsg);
    if (0U != (dwAppBufSize)) PH_ASSERT_NULL (pRxBuffer);
    PH_ASSERT_NULL (pdwRxLen);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_Get((phnpSnep_Sw_DataParams_t *) pDataParams, pNdefMsg, dwNdefMsgLen,
            pRxBuffer, pdwRxLen, dwAppBufSize);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phnpSnep_ServerListen(
                                 void * pDataParams,
                                 uint32_t   dwGetReqBuffSize,
                                 uint8_t  * pGetReqData,
                                 uint32_t * dwGetReqLength,
                                 uint8_t  * pClientReq
                                 )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_ServerListen");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pClientReq);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pClientReq);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_ServerListen((phnpSnep_Sw_DataParams_t *) pDataParams, dwGetReqBuffSize,
            pGetReqData, dwGetReqLength, pClientReq);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status ;
}

phStatus_t phnpSnep_ServerSendResponse(
                                       void * pDataParams,
                                       uint8_t bClientReq,
                                       uint8_t *pRespDataForGet,
                                       uint32_t dwRespDataLen,
                                       uint32_t dwPutBuffSize,
                                       uint8_t *pPutData,
                                       uint32_t *pdwPutDataLen
                                       )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_ServerSendResponse");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bClientReq);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bClientReq_log, &bClientReq);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_ServerSendResponse((phnpSnep_Sw_DataParams_t *) pDataParams, bClientReq,
            pRespDataForGet, dwRespDataLen, dwPutBuffSize, pPutData, pdwPutDataLen);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status ;
}

phStatus_t phnpSnep_ClientDeInit(
                                 void * pDataParams
                                 )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_ClientDeInit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_ClientDeInit((phnpSnep_Sw_DataParams_t *) pDataParams);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status ;
}

phStatus_t phnpSnep_ServerDeInit(
                                 void * pDataParams
                                 )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phnpSnep_ServerDeInit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_NP_SNEP)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHNP_SNEP_SW
    case PHNP_SNEP_SW_ID:
        status = phnpSnep_Sw_ServerDeInit((phnpSnep_Sw_DataParams_t *) pDataParams);
        break;
#endif /* NXPBUILD__PHNP_SNEP_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_NP_SNEP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status ;
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHNP_SNEP */
