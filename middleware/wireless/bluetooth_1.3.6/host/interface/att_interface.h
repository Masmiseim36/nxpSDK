/*! *********************************************************************************
 * \defgroup ATT ATT
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ATT_INTERFACE_H
#define ATT_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "att_types.h"
#include "att_params.h"
#include "att_callbacks.h"

/************************************************************************************
 *************************************************************************************
 * Public prototypes
 *************************************************************************************
 ************************************************************************************/

 #ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t Att_Init(void);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t Att_NotifyConnection
(
    deviceId_t deviceId
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t Att_NotifyDisconnection
(
    deviceId_t deviceId
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t Att_SetMtu
(
    deviceId_t deviceId,
    uint16_t   mtu
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t Att_GetMtu
(
    deviceId_t deviceId,
    uint16_t*  pOutMtu
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bool_t Att_IsTimedRequestOpcode
(
    attOpcode_t opcode
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bool_t Att_IsTimedResponseOpcode
(
    attOpcode_t opcode
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendErrorResponse
(
    deviceId_t                  deviceId,
    const attErrorResponseParams_t*   pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendExchangeMtuRequest
(
    deviceId_t                      deviceId,
    const attExchangeMtuRequestParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendExchangeMtuResponse
(
    deviceId_t                      deviceId,
    const attExchangeMtuResponseParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendFindInformationRequest
(
    deviceId_t                          deviceId,
    const attFindInformationRequestParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendFindInformationResponse
(
    deviceId_t                                deviceId,
    const attFindInformationResponseParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendFindByTypeValueRequest
(
    deviceId_t                          deviceId,
    const attFindByTypeValueRequestParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendFindByTypeValueResponse
(
    deviceId_t                                deviceId,
    const attFindByTypeValueResponseParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendReadByTypeRequest
(
    deviceId_t                            deviceId,
    const attReadByTypeRequestParams_t*   pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendReadByTypeResponse
(
    deviceId_t                            deviceId,
    const attReadByTypeResponseParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendReadRequest
(
    deviceId_t                  deviceId,
    const attReadRequestParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendReadResponse
(
    deviceId_t                        deviceId,
    const attReadResponseParams_t*    pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendReadBlobRequest
(
    deviceId_t                        deviceId,
    const attReadBlobRequestParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendReadBlobResponse
(
    deviceId_t                            deviceId,
    const attReadBlobResponseParams_t*    pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendReadMultipleRequest
(
    deviceId_t                      deviceId,
    const attReadMultipleRequestParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendReadMultipleResponse
(
    deviceId_t                                deviceId,
    const attReadMultipleResponseParams_t*    pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendReadByGroupTypeRequest
(
    deviceId_t                          deviceId,
    const attReadByGroupTypeRequestParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendReadByGroupTypeResponse
(
    deviceId_t                                deviceId,
    const attReadByGroupTypeResponseParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendWriteRequest
(
    deviceId_t                                deviceId,
    const attVarWriteRequestAndCommandParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendWriteResponse
(
    deviceId_t deviceId
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendWriteCommand
(
    deviceId_t                                deviceId,
    const attVarWriteRequestAndCommandParams_t*  pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendSignedWriteCommand
(
    deviceId_t                      deviceId,
    const attSignedWriteCommandParams_t*  pParams
);

/*! *********************************************************************************
 * \brief]
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendPrepareWriteRequest
(
    deviceId_t                              deviceId,
    const attPrepareWriteRequestResponseParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendPrepareWriteResponse
(
    deviceId_t                                    deviceId,
    const attPrepareWriteRequestResponseParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendExecuteWriteRequest
(
    deviceId_t                      deviceId,
    const attExecuteWriteRequestParams_t* pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendExecuteWriteResponse
(
    deviceId_t deviceId
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendHandleValueNotification
(
    deviceId_t                                            deviceId,
    const attVarHandleValueNotificationIndicationParams_t*   pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttServer_SendHandleValueIndication
(
    deviceId_t                                            deviceId,
    const attVarHandleValueNotificationIndicationParams_t*   pParams
);

/*! *********************************************************************************
 * \brief
 *
 * \return
 *
 * \pre
 *
 * \remarks
 *
 ********************************************************************************** */
bleResult_t AttClient_SendHandleValueConfirmation
(
    deviceId_t deviceId
);


#ifdef __cplusplus
}
#endif

#endif /* ATT_INTERFACE_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
