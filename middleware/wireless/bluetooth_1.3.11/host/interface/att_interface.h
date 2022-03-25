/*! *********************************************************************************
 * \defgroup ATT ATT
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

/*!*************************************************************************************************
*\fn           bleResult_t Att_Init(void)
*\brief        Initializes maAttConnections[] table. Registers the ATT callback with the L2CAP layer
*
*\param [in]   void
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t Att_Init(void);

/*!*************************************************************************************************
*\fn           bleResult_t Att_NotifyConnection(deviceId_t deviceId)
*
*\brief        Called upon gHciLeConnectionCompleteEvent_c. Stores the deviceId and default MTU of the peer
*              in the maAttConnections[] table. Also allocates and stores the timer for GATT timed procedures.
*
*\param [in]   deviceId      Identifier of the connected peer
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t Att_NotifyConnection
(
    deviceId_t deviceId
);

/*!*************************************************************************************************
*\fn           bleResult_t Att_NotifyDisconnection(deviceId_t deviceId)
*
*\brief        Called upon gHciDisconnectionCompleteEvent_c and removes deviceId from 
*              maAttConnections[] table and frees any allocated timer for the specific connection.
*
*\param [in]   deviceId      Identifier of the connected peer
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t Att_NotifyDisconnection
(
    deviceId_t deviceId
);

/*!*************************************************************************************************
*\fn           bleResult_t Att_SetMtu(deviceId_t deviceId, uint16_t   mtu)
*
*\brief        Stores in maAttConnections[] table the MTU used for upcoming transactions with deviceId.
*              On the client side it is called upon an Exchange MTU Response, on the server
*              side it is called upon an Exchange MTU Request.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   mtu           The size of the MTU to be used for ATT PDU exchange
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t Att_SetMtu
(
    deviceId_t deviceId,
    uint16_t   mtu
);

/*!*************************************************************************************************
*\fn            bleResult_t Att_GetMtu(deviceId_t deviceId, uint16_t*  pOutMtu)
*
*\brief         Returns the stored MTU for the deviceID.
*
*\param [in]    deviceId      Identifier of the connected peer
*\param [out]   pOutMtu       Memory address where the MTU value will be written
*
*\return        bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t Att_GetMtu
(
    deviceId_t deviceId,
    uint16_t*  pOutMtu
);

/*!*************************************************************************************************
*\fn           bool_t Att_IsTimedRequestOpcode(attOpcode_t opcode)
*
*\brief        Check if the request operation provided has a timeout.
*
*\param [in]   opcode         Opcode of the request operation to be checked
*
*\return       bool_t         TRUE for timed request, FALSE otherwise
***************************************************************************************************/
bool_t Att_IsTimedRequestOpcode
(
    attOpcode_t opcode
);

/*!*************************************************************************************************
*\fn           bool_t Att_IsTimedResponseOpcode(attOpcode_t opcode)
*
*\brief        Returns TRUE if the response operation provided has a timeout. The responses
*              which fit into this category correspond to the requests enumerated above.
*
*\param [in]   opcode         Opcode of the response operation to be checked.
*
*\return       bool_t         TRUE for timed response, FALSE otherwise
***************************************************************************************************/
bool_t Att_IsTimedResponseOpcode
(
    attOpcode_t opcode
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendErrorResponse(deviceId_t deviceId,
*                                                      const attErrorResponseParams_t* pParams)
*
*\brief        Server sends an Error Response whenever a request cannot be performed
*
*\param [in]   deviceId       Identifier of the connected peer
*\param [in]   pParams        Pointer to Error Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendErrorResponse
(
    deviceId_t                  deviceId,
    const attErrorResponseParams_t*   pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendExchangeMtuRequest(deviceId_t deviceId,
*                                                           const attExchangeMtuRequestParams_t* pParams)
*
*\brief        Client initiates Exchange MTU Request to server and starts a timer waiting for response
*
*\param [in]   deviceId       Identifier of the connected peer
*\param [in]   pParams        Pointer to MTU exchange parameters to be sent to ATT server
*
*\return       bleResult_t    As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendExchangeMtuRequest
(
    deviceId_t                            deviceId,
    const attExchangeMtuRequestParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendExchangeMtuResponse(deviceId_t deviceId,
*                                                            const attExchangeMtuResponseParams_t* pParams)
*
*\brief        Server responds with Exchange MTU Response to an Exchange MTU Request.
*              Server sends its maximum MTU and stores the one received from the client.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Exchange MTU Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendExchangeMtuResponse
(
    deviceId_t                            deviceId,
    const attExchangeMtuResponseParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendFindInformationRequest(deviceId_t deviceId,
*                                                               const attFindInformationRequestParams_t* pParams)
*
*\brief        Client initiates Find Information Request to obtain the mapping of attribute
*              handles with their associated types. A timer is started waiting for response.
*
*\param [in]   deviceId       Identifier of the connected peer
*\param [in]   pParams        Pointer to Find Information Request parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendFindInformationRequest
(
    deviceId_t                                deviceId,
    const attFindInformationRequestParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendFindInformationResponse(deviceId_t deviceId,
*                                                                const attFindInformationResponseParams_t* pParams)
*
*\brief        Server responds with Find Information Response to a Find Information Request.
*              The response contains the (handle, UUID) pairs.
*
*\param [in]   deviceId       Identifier of the connected peer
*\param [in]   pParams        Pointer to Find Information Response parameters to be sent to ATT client
*
*\return       bleResult_t    As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendFindInformationResponse
(
    deviceId_t                                deviceId,
    const attFindInformationResponseParams_t* pParams
);

/*!*************************************************************************************************
*\fn    bleResult_t AttClient_SendFindByTypeValueRequest(deviceId_t deviceId, const attFindByTypeValueRequestParams_t*  pParams)
*
*\brief        Client initiates Find by Value Request to obtain the handles of attributes that have a 16-bit UUID
*              attribute type and attribute value. A timer is started waiting for a Find by Value Response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Find by Type Value Request parameters to be sent to ATT server.
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendFindByTypeValueRequest
(
    deviceId_t                                deviceId,
    const attFindByTypeValueRequestParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendFindByTypeValueResponse(deviceId_t deviceId,
*                                                                const attFindByTypeValueResponseParams_t* pParams)
*
*\brief        Server responds with Find by Value Response to a Find by Value Request.
*              Response contains the start and end handles of the attributes requested by client.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Find by Type Value Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendFindByTypeValueResponse
(
    deviceId_t                                deviceId,
    const attFindByTypeValueResponseParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendReadByTypeRequest(deviceId_t deviceId,
*                                                          const attReadByTypeRequestParams_t* pParams)
*
*\brief        Client initiates Read by type Request to obtain the values of attributes where
*              the attribute is known, but the handle is not known  A timer is started for a Read by Type Response
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read by Type Request parameters to be sent to ATT server.
*\param [out]  bleResult_t   Error codes are defined in "ble_general.h"
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendReadByTypeRequest
(
    deviceId_t                            deviceId,
    const attReadByTypeRequestParams_t*   pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendReadByTypeResponse(deviceId_t deviceId,
*                                                           const attReadByTypeResponseParams_t* pParams)
*
*\brief        Server responds with Read by Type Response to a Read by Type Request.
*              Response contains a list of attribute values and their corresponding handles.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read by Type Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendReadByTypeResponse
(
    deviceId_t                            deviceId,
    const attReadByTypeResponseParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendReadRequest(deviceId_t deviceId,
*                                                    const attReadByTypeRequestParams_t* pParams)
*
*\brief        Client initiates Read Request to read the value of an attribute
*              A timer is started for a Read Response from Server.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read Request parameters to be sent to ATT server.
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendReadRequest
(
    deviceId_t                    deviceId,
    const attReadRequestParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendReadResponse(deviceId_t deviceId,
*                                                     const attReadResponseParams_t* pParams)
*
*\brief        Server responds with Read Response to Read Request.
*              Response contains the value of the attribute from the Request.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendReadResponse
(
    deviceId_t                        deviceId,
    const attReadResponseParams_t*    pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendReadBlobRequest(deviceId_t deviceId,
*                                                        const attReadBlobRequestParams_t* pParams)
*
*\brief        Client initiates Read Blob Request to read part of the value of an attribute
*              at a given offset  A timer is started for a Read Blob Response.
*
*\param [in]   deviceId       Identifier of the connected peer
*\param [in]   pParams        Pointer to Read Blob Request parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendReadBlobRequest
(
    deviceId_t                        deviceId,
    const attReadBlobRequestParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendReadBlobResponse(deviceId_t deviceId,
*                                                         const attReadBlobResponseParams_t* pParams)
*
*\brief        Server responds with Read Blob Response to a Read Blob Request.
*              Response contains part of the value of the attribute requested.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read Blob Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendReadBlobResponse
(
    deviceId_t                            deviceId,
    const attReadBlobResponseParams_t*    pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendReadMultipleRequest(deviceId_t deviceId,
*                                                            const attReadMultipleRequestParams_t* pParams)
*
*\brief        Client initiates Read Multiple Request when reading two or more values
*              of a set of attributes. A timer is started for a Read Multiple Response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read Multiple Request parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendReadMultipleRequest
(
    deviceId_t                            deviceId,
    const attReadMultipleRequestParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendReadMultipleResponse(deviceId_t deviceId,
*                                                             const attReadMultipleResponseParams_t* pParams)
*
*\brief        Server sends Read Multiple Response in response to a Read Multiple Request.
*              Response contains the values of the attributes requested by Client
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read Multiple Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h
***************************************************************************************************/
bleResult_t AttServer_SendReadMultipleResponse
(
    deviceId_t                                deviceId,
    const attReadMultipleResponseParams_t*    pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendReadByGroupTypeRequest(deviceId_t deviceId,
*                                                               const attReadByGroupTypeRequestParams_t* pParams)
*
*\brief        Client initiates Read by Group Request to obtain the values of attributes where the
*              attribute type is known, but the handle is not known. A timer is started for a Read by Group Response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       pointer to Read by Group Type Request parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendReadByGroupTypeRequest
(
    deviceId_t                                deviceId,
    const attReadByGroupTypeRequestParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendReadByGroupTypeResponse(deviceId_t deviceId,
*                                                                const attReadByGroupTypeResponseParams_t* pParams)
*
*\brief        Server sends a Read by Group Type Response in response to a Read by Group Type Request.
*              Response contains the handles and the values of the attributes in the request.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Read by Group Type Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendReadByGroupTypeResponse
(
    deviceId_t                                deviceId,
    const attReadByGroupTypeResponseParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendWriteRequest(deviceId_t deviceId,
*                                                     const attVarWriteRequestAndCommandParams_t* pParams)
*
*\brief        Client initiates Write Request to write the value of an attribute on the server.
*              A timer is started for a  Write Response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Write Request/Command parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendWriteRequest
(
    deviceId_t                                   deviceId,
    const attVarWriteRequestAndCommandParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendWriteResponse(deviceId_t deviceId,
*                                                      const attReadByGroupTypeResponseParams_t* pParams)
*
*\brief        Server sends a Write Response in response to a Write Request.
*              Response confirms that the attributes sent in the Write Request were successfully written.
*              This message does not contain any data.
*
*\param [in]   deviceId      Identifier of the connected peer
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendWriteResponse
(
    deviceId_t deviceId
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendWriteCommand(deviceId_t deviceId,
*                                                     const attVarWriteRequestAndCommandParams_t* pParams)
*
*\brief        Client initiates Write Command to write the value of an attribute on the server.
*              the client does not wait for a response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Write Request/Command parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendWriteCommand
(
    deviceId_t                                   deviceId,
    const attVarWriteRequestAndCommandParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendSignedWriteCommand(deviceId_t deviceId,
*                                                           const attSignedWriteCommandParams_t*  pParams)
*
*\brief        Client initiates a Signed Write Command to write the value of an attribute with an
* b            authentication signature. the client does not wait for a response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Write Signed Command parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendSignedWriteCommand
(
    deviceId_t                            deviceId,
    const attSignedWriteCommandParams_t*  pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendPrepareWriteRequest(deviceId_t deviceId,
*                                                     const attPrepareWriteRequestResponseParams_t* pParams)
*
*\brief        Client initiates a Prepare Write Request to write the value of a long attribute.
*              the client may initiate multiple Prepare Write Requests.
*              A timer is started for a Prepare Write Response.
*              the server will queue all the attributes received in Prepare Write Requests.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Prepare Write Request parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendPrepareWriteRequest
(
    deviceId_t                                    deviceId,
    const attPrepareWriteRequestResponseParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendPrepareWriteResponse(deviceId_t deviceId,
*                                                             const attPrepareWriteRequestResponseParams_t* pParams)
*
*\brief        Server sends a Prepare Write Response in response to a Prepare Write Request.
*              Response confirms that the attributes sent in Prepare Write Request were
*              successfully placed in the writing queue. It contains the handles and the values sent
*              by client in request.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Prepare Write Response parameters to be sent to ATT client
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendPrepareWriteResponse
(
    deviceId_t                                    deviceId,
    const attPrepareWriteRequestResponseParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendExecuteWriteRequest(deviceId_t deviceId,
*                                                            const attExecuteWriteRequestParams_t* pParams)
*
*\brief        Client initiates Execute Write Request to request the server to write or cancel the
*              attributes held in the prepare queue. A timer is started for an Execute Write Response.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Execute Write Request parameters to be sent to ATT server
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttClient_SendExecuteWriteRequest
(
    deviceId_t                            deviceId,
    const attExecuteWriteRequestParams_t* pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendExecuteWriteResponse(deviceId_t deviceId)
*
*\brief        Server sends an Execute Write Response in response to an Execute Write Request.
*              Response acknowledges that the attributes sent in Execute Write Request were
*              successfully written. This message does not contain any data.
*
*\param [in]   deviceId      Identifier of the connected peer
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendExecuteWriteResponse
(
    deviceId_t deviceId
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendHandleValueNotification(deviceId_t deviceId,
*                                                                const attVarHandleValueNotificationIndicationParams_t* pParams)
*
*\brief        Server initiates a Handle Value Notification message containing the attribute handle and its value
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Handle Value Notification parameters to be sent to ATT client.
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendHandleValueNotification
(
    deviceId_t                                               deviceId,
    const attVarHandleValueNotificationIndicationParams_t*   pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttServer_SendHandleValueIndication(deviceId_t deviceId,
*                                                              const attVarHandleValueNotificationIndicationParams_t*   pParams)
*
*\brief        Server initiates a Handle Value Indication message containing the attribute handle
               and its value. A timer is started for a Handle Value Confirmation.
*
*\param [in]   deviceId      Identifier of the connected peer
*\param [in]   pParams       Pointer to Handle Value Indication parameters to be sent to ATT client"
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
bleResult_t AttServer_SendHandleValueIndication
(
    deviceId_t                                               deviceId,
    const attVarHandleValueNotificationIndicationParams_t*   pParams
);

/*!*************************************************************************************************
*\fn           bleResult_t AttClient_SendHandleValueConfirmation(deviceId_t deviceId)
*
*\brief        Client sends Handle Value Confirmation as a response to server's Handle Value Indication
*
*\param [in]   deviceId      Identifier of the connected peer
*
*\return       bleResult_t   As defined in "ble_internal.h"
***************************************************************************************************/
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
