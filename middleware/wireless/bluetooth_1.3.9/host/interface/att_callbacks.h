/*! *********************************************************************************
 * \addtogroup ATT
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ATT_CALLBACKS_H
#define ATT_CALLBACKS_H

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef union
{
    void *                                        pGenericParam;
    attErrorResponseParams_t*                     pErrorResponseParams;
    attExchangeMtuRequestParams_t*                pExchangeMtuRequestParams;
    attExchangeMtuResponseParams_t*               pExchangeMtuResponseParams;
    attFindInformationRequestParams_t*            pFindInformationRequestParams;
    attFindInformationResponseParams_t*           pFindInformationResponseParams;
    attFindByTypeValueRequestParams_t*            pFindByTypeValueRequestParams;
    attFindByTypeValueResponseParams_t*           pFindByTypeValueResponseParams;
    attReadByTypeRequestParams_t*                 pReadByTypeRequestParams;
    attReadByTypeResponseParams_t*                pReadByTypeResponseParams;
    attReadRequestParams_t*                       pReadRequestParams;
    attReadResponseParams_t*                      pReadResponseParams;
    attReadBlobRequestParams_t*                   pReadBlobRequestParams;
    attReadBlobResponseParams_t*                  pReadBlobResponseParams;
    attReadMultipleRequestParams_t*               pReadMultipleRequestParams;
    attReadMultipleResponseParams_t*              pReadMultipleResponseParams;
    attReadByGroupTypeRequestParams_t*            pReadByGroupTypeRequestParams;
    attReadByGroupTypeResponseParams_t*           pReadByGroupTypeResponseParams;
    attWriteRequestAndCommandParams_t*            pWriteRequestAndCommandParams;
    attSignedWriteCommandParams_t*                pSignedWriteCommandParams;
    attPrepareWriteRequestResponseParams_t*       pPrepareWriteRequestResponseParams;
    attExecuteWriteRequestParams_t*               pExecuteWriteRequestParams;
    attHandleValueNotificationIndicationParams_t* pHandleValueNotificationIndicationParams;
    attInvalidPduIndicationParams_t*              pInvalidPduIndicationParams;
}attCbParams_t;

typedef void (*pfAttToUpperLayerCallback_t)(deviceId_t deviceId, attOpcode_t opcode, void* pParam);

/************************************************************************************
 *************************************************************************************
 * Public prototypes
 *************************************************************************************
 ************************************************************************************/

/*! *********************************************************************************
 * \brief Register ATT's upper layer callback function
 *
 * \return gBleSuccess_c
 *
 ********************************************************************************** */
bleResult_t Att_RegisterCallback
(
    pfAttToUpperLayerCallback_t        callback
);

#endif /* ATT_CALLBACKS_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
