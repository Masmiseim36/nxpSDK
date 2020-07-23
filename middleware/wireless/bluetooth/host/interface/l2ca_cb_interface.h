/*! *********************************************************************************
* \addtogroup L2CA
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef L2CA_CB_INTERFACE_H
#define L2CA_CB_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"

#include "l2ca_types.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
/* Result values for LE Credit Based Connection Response */
typedef enum
{
    gSuccessful_c                    = 0x0000U,
    gLePsmNotSupported_c             = 0x0002U,
    gNoResourcesAvailable_c          = 0x0004U,
    gInsufficientAuthentication_c    = 0x0005U,
    gInsufficientAuthorization_c     = 0x0006U,
    gInsufficientEncryptionKeySize_c = 0x0007U,
    gInsufficientEncryption_c        = 0x0008U,
    gInvalidSourceCid_c              = 0x0009U,
    gSourceCidAreadyAllocated_c      = 0x000AU,
    gInvalidParameters_c             = 0x000BU,
    gCommandRejected_c               = 0x0100U,
    gResponseTimeout_c               = 0xFFFEU
} l2caLeCbConnectionRequestResult_t;

typedef enum
{
    gL2ca_CancelConnection_c = 0x0000U,
    gL2ca_SendLeFlowControlCredit_c  = 0x00001U,
    gL2ca_DisconnectLePsm_c = 0x0002U,
    gL2ca_HandleSendLeCbData_c = 0x0003U,
    gL2ca_HandleRecvLeCbData_c = 0x0004U,
    gL2ca_HandleLeFlowControlCredit_c = 0x00005U
} l2caErrorSource_t;

typedef struct
{
    deviceId_t  deviceId;
    uint16_t    lePsm;
    uint16_t    peerMtu;
    uint16_t    peerMps;
    uint16_t    initialCredits;
} l2caLeCbConnectionRequest_t;

typedef struct
{
    deviceId_t                          deviceId;
    uint16_t                            cId;
    uint16_t                            peerMtu;
    uint16_t                            peerMps;
    uint16_t                            initialCredits;
    l2caLeCbConnectionRequestResult_t   result;
} l2caLeCbConnectionComplete_t;

typedef struct
{
    deviceId_t  deviceId;
    uint16_t    cId;
} l2caLeCbDisconnection_t;

typedef struct
{
    deviceId_t  deviceId;
    uint16_t    cId;
} l2caLeCbNoPeerCredits_t;

typedef struct
{
    deviceId_t  deviceId;
    uint16_t    cId;
    uint16_t    localCredits;
} l2caLeCbLocalCreditsNotification_t;

typedef struct
{
    deviceId_t  deviceId;
    bleResult_t result;
    l2caErrorSource_t errorSource;
} l2caLeCbError_t;

typedef struct
{
    l2capControlMessageType_t messageType;
    union
    {
        l2caLeCbConnectionRequest_t        connectionRequest;
        l2caLeCbConnectionComplete_t       connectionComplete;
        l2caLeCbDisconnection_t            disconnection;
        l2caLeCbNoPeerCredits_t            noPeerCredits;
        l2caLeCbLocalCreditsNotification_t localCreditsNotification;
        l2caLeCbError_t error;
    } messageData;
} l2capControlMessage_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Interface callback type definitions
*************************************************************************************
************************************************************************************/
typedef void(*l2caLeCbDataCallback_t)
                (deviceId_t deviceId,
                 uint16_t   channelId,
                 uint8_t* pPacket,
                 uint16_t packetLength);

typedef void(*l2caControlCallback_t)
                (l2capControlMessage_t* pMessage);

typedef l2caControlCallback_t l2caLeCbControlCallback_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*
* L2CAP LE Credit Based Interface Primitives
*/

/*! *********************************************************************************
* \brief        Registers callbacks for credit based data and control events on L2CAP.
*
* \param[in]    pCallback           Callback function for data plane messages
* \param[in]    pCtrlCallback       Callback function for control plane messages
*
* \return       Result of the operation
*
********************************************************************************** */
bleResult_t L2ca_RegisterLeCbCallbacks
(
    l2caLeCbDataCallback_t      pCallback,
    l2caLeCbControlCallback_t   pCtrlCallback
);

/*! *********************************************************************************
* \brief        Registers the LE_PSM from the L2CAP.
*
* \param[in]    lePsm               Bluetooth SIG or Vendor LE_PSM
* \param[in]    lePsmMtu            MTU of the registered PSM
*
* \return       Result of the operation
*
********************************************************************************** */
bleResult_t L2ca_RegisterLePsm
    (
        uint16_t                    lePsm,
        uint16_t                    lePsmMtu
    );

/*! *********************************************************************************
* \brief        Unregisters the LE_PSM from the L2CAP.
*
* \param[in]    lePsm               Bluetooth SIG or Vendor LE_PSM
*
* \return       Result of the operation
*
* \pre          A LE_PSM must be registered a priori
*
********************************************************************************** */
bleResult_t L2ca_DeregisterLePsm
    (
        uint16_t    lePsm
    );

/*! *********************************************************************************
* \brief        Initiates a connection with a peer device for a registered LE_PSM.
*
* \param[in]    lePsm               Bluetooth SIG or Vendor LE_PSM
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    initialCredits      Initial credits
*
* \return       Result of the operation
*
* \pre          A LE_PSM must be registered a priori
*
********************************************************************************** */
bleResult_t L2ca_ConnectLePsm
    (
        uint16_t    lePsm,
        deviceId_t  deviceId,
        uint16_t    initialCredits
    );

/*! *********************************************************************************
* \brief        Disconnects a peer device for a registered LE_PSM.
*
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    channelId           The L2CAP Channel Id assigned on the initiator
*
* \return       Result of the operation
*
* \pre          A connection must have already been created
*
* \remarks      Once this command is issued, all incoming data in transit for this
*               device shall be discarded and any new additional outgoing data shall
*               be discarded.
*
********************************************************************************** */
bleResult_t L2ca_DisconnectLeCbChannel
    (
        deviceId_t  deviceId,
        uint16_t    channelId
    );

/*! *********************************************************************************
* \brief        Terminates an L2CAP channel.
*
* \param[in]    lePsm               Bluetooth SIG or Vendor LE_PSM
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    refuseReason        Reason to refuse the channel creation
*
* \return       Result of the operation
*
* \remarks      This interface can be used for a connection pending creation.
*
********************************************************************************** */
bleResult_t L2ca_CancelConnection
    (
        uint16_t    lePsm,
        deviceId_t  deviceId,
        l2caLeCbConnectionRequestResult_t refuseReason
    );

/*! *********************************************************************************
* \brief        Sends a data packet through a Credit-Based Channel.
*
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    channelId           The L2CAP Channel Id assigned on the initiator
* \param[in]    pPacket             Data buffer to be transmitted
* \param[in]    packetLength        Length of the data buffer
*
* \return       Result of the operation
*
* \pre          An L2CAP Credit Based connection must be in place
*
********************************************************************************** */
bleResult_t L2ca_SendLeCbData
    (
        deviceId_t        deviceId,
        uint16_t          channelId,
        const uint8_t*    pPacket,
        uint16_t          packetLength
    );

/*! *********************************************************************************
* \brief        Sends credits to a device when capable of receiving additional LE-frames
*
* \param[in]    deviceId            The DeviceID to which credits are given
* \param[in]    channelId           The L2CAP Channel Id assigned on the initiator
* \param[in]    credits             Number of credits to be given
*
* \return       Result of the operation
*
* \pre          An L2CAP Credit Based connection must be in place
*
********************************************************************************** */
bleResult_t L2ca_SendLeCredit
    (
        deviceId_t  deviceId,
        uint16_t    channelId,
        uint16_t    credits
    );

#ifdef __cplusplus
}
#endif

#endif /* L2CA_CB_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
