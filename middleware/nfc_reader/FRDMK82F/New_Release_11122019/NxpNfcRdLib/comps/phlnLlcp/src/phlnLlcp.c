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
* Generic LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 18. Feb 2014
*
*/
#include <ph_Status.h>
#include <phlnLlcp.h>

#ifdef NXPBUILD__PHLN_LLCP_SW
#include "Sw/phlnLlcp_Sw.h"
#endif/* NXPBUILD__PHLN_LLCP_SW */

#ifdef NXPBUILD__PHLN_LLCP

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phlnLlcp_Activate(
                             void     * pDataParams,
                             uint8_t  * pRemoteGnrlBytes,
                             uint8_t    bRemoteGnrBytesLen,
                             uint8_t    bDevType
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Activate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Activate((phlnLlcp_Sw_DataParams_t *)pDataParams, pRemoteGnrlBytes, bRemoteGnrBytesLen, bDevType);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Deactivate(
                               void * pDataParams
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Deactivate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Deactivate((phlnLlcp_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_WaitForActivation(
                                      void * pDataParams
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_WaitForActivation");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_WaitForActivation((phlnLlcp_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_Init(
                                          void   *pDataParams,
                                          phlnLlcp_Transport_Socket_t* psSocket,
                                          phlnLlcp_Transport_Socket_Type_t eSocketType,
                                          uint8_t* pSocketRxBuffer,
                                          uint32_t dwSocketRxBufferSize
                                          )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Init");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);


    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Init((phlnLlcp_Sw_DataParams_t *)pDataParams, psSocket, eSocketType, pSocketRxBuffer, dwSocketRxBufferSize);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_Register(
                                              void  *pDataParams,
                                              phlnLlcp_Transport_Socket_t* pSocket,
                                              phlnLlcp_Transport_Socket_Type_t eSocketType,
                                              uint8_t bLsap,
                                              uint8_t* pUri)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Register");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);


    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Register((phlnLlcp_Sw_DataParams_t *)pDataParams, pSocket, eSocketType, bLsap, pUri);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_WaitForConnection(
                                                       void  *pDataParams,
                                                       phlnLlcp_Transport_Socket_t* pServerSocket,
                                                       phlnLlcp_Transport_Socket_t* pConnServerSocket
                                                       )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_WaitForConnection");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_WaitForConnection((phlnLlcp_Sw_DataParams_t *)pDataParams, pServerSocket, pConnServerSocket);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_Connect(
                                             void  *pDataParams,
                                             phlnLlcp_Transport_Socket_t* pClientSocket,
                                             uint8_t bDsap
                                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Connect");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Connect((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket, bDsap);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_ConnectViaUri(
                                                   void * pDataParams,
                                                   phlnLlcp_Transport_Socket_t * pClientSocket,
                                                   uint8_t * pUri
                                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Connect");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_ConnectViaUri((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket, pUri);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_Receive(
                                             void  *pDataParams,
                                             phlnLlcp_Transport_Socket_t* psSocket)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Receive");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Receive((phlnLlcp_Sw_DataParams_t *)pDataParams, psSocket);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_Send(
                                          void * pDataParams,
                                          phlnLlcp_Transport_Socket_t * pClientSocket,
                                          uint8_t * pTxBuffer,
                                          uint32_t dwTxBufferSize,
                                          uint16_t wFrameOpt
                                          )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Send");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Send((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket, pTxBuffer, dwTxBufferSize, wFrameOpt);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_DiscoverServices(
                                                      void * pDataParams,
                                                      phlnLlcp_Transport_Socket_t * pSocket,
                                                      uint8_t  bListSize,
                                                      uint8_t  bSNBuffSize,
                                                      uint8_t *  psServiceNameList,
                                                      uint8_t *  pSapList
                                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_DiscoverServices");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_DiscoverServices((phlnLlcp_Sw_DataParams_t *)pDataParams, pSocket, bListSize, bSNBuffSize, psServiceNameList, pSapList);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_SendTo(
                                            void * pDataParams,
                                            phlnLlcp_Transport_Socket_t * pClientSocket,
                                            uint8_t   bDsap,
                                            uint8_t * pTxBuffer,
                                            uint32_t dwTxBufferSize,
                                            uint16_t wFrameOpt
                                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_SendTo");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_SendTo((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket, bDsap, pTxBuffer, dwTxBufferSize, wFrameOpt);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_Transport_Socket_Disconnect(
                                                void* pDataParams,
                                                phlnLlcp_Transport_Socket_t * pClientSocket
                                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Disconnect");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Disconnect((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phlnLlcp_Transport_Socket_Unregister(
                                                void * pDataParams,
                                                phlnLlcp_Transport_Socket_t * pSocket
                                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_Transport_Socket_Unregister");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_Transport_Socket_Unregister((phlnLlcp_Sw_DataParams_t *)pDataParams, pSocket);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phlnLlcp_DeInit(
                           void * pDataParams
                           )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phlnLlcp_DeInit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_LN_LLCP)
    {
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHLN_LLCP_SW
    case PHLN_LLCP_SW_ID:
        status = phlnLlcp_Sw_DeInit((phlnLlcp_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHLN_LLCP_SW */

    default:
        status = (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_LN_LLCP);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHLN_LLCP */
