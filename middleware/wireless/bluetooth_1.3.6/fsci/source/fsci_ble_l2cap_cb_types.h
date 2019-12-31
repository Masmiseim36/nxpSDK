/*! *********************************************************************************
 * \defgroup FSCI_BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a header file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef FSCI_BLE_L2CAP_CB_TYPES_H
#define FSCI_BLE_L2CAP_CB_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble.h"
#include "l2ca_cb_interface.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#define fsciBleL2capCbGetLeCbConnectionRequestBufferSize(pLeCbConnectionRequest)    \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbConnectionRequest)->deviceId) +        \
        sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) +  sizeof(uint16_t))

#define fsciBleL2capCbGetLeCbConnectionCompleteBufferSize(pLeCbConnectionComplete)  \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbConnectionComplete)->deviceId) +       \
        sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + \
        sizeof(l2caLeCbConnectionRequestResult_t))

#define fsciBleL2capCbGetLeCbDisconnectionBufferSize(pLeCbDisconnection)    \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbDisconnection)->deviceId) +    \
        sizeof(uint16_t))

#define fsciBleL2capCbGetLeCbNoPeerCreditsBufferSize(pLeCbNoPeerCredits)    \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbNoPeerCredits)->deviceId) +    \
        sizeof(uint16_t))

#define fsciBleL2capCbGetLeCbLocalCreditsNotificationBufferSize(pLeCbLocalCreditsNotification)  \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbLocalCreditsNotification)->deviceId) +             \
        sizeof(uint16_t) + sizeof(uint16_t))

#define fsciBleL2capCbGetLeCbErrorBufferSize(pLeCbError)         \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbError)->deviceId) + \
        sizeof(bleResult_t))

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void fsciBleL2capCbGetLeCbConnReqFromBuffer
(
    l2caLeCbConnectionRequest_t*    pLeCbConnectionRequest,
    uint8_t**                       ppBuffer
);

void fsciBleL2capCbGetBuffFromLeCbConnRequest
(
    l2caLeCbConnectionRequest_t*    pLeCbConnectionRequest,
    uint8_t**                       ppBuffer
);

void fsciBleL2capCbGetLeCbConnectionCompleteFromBuffer
(
    l2caLeCbConnectionComplete_t*   pLeCbConnectionComplete,
    uint8_t**                       ppBuffer
);

void fsciBleL2capCbGetBufferFromLeCbConnectionComplete
(
    l2caLeCbConnectionComplete_t*   pLeCbConnectionComplete,
    uint8_t**                       ppBuffer
);

void fsciBleL2capCbGetLeCbDisconnectionFromBuffer
(
    l2caLeCbDisconnection_t*    pLeCbDisconnection,
    uint8_t**                   ppBuffer
);

void fsciBleL2capCbGetBuffFromLeCbDisconnection
(
    l2caLeCbDisconnection_t*    pLeCbDisconnection,
    uint8_t**                   ppBuffer
);

void fsciBleL2capCbGetLeCbNoPeerCreditsFromBuffer
(
    l2caLeCbNoPeerCredits_t*    pLeCbNoPeerCredits,
    uint8_t**                   ppBuffer
);

void fsciBleL2capCbGetBuffFromLeCbNoPeerCredits
(
    l2caLeCbNoPeerCredits_t*    pLeCbNoPeerCredits,
    uint8_t**                   ppBuffer
);

void fsciBleL2capCbGetLeCbLocalCreditsNotificationFromBuffer
(
    l2caLeCbLocalCreditsNotification_t* pLeCbLocalCreditsNotification,
    uint8_t**                           ppBuffer
);

void fsciBleL2capCbGetBuffFromLeCbLocalCreditsNotification
(
    l2caLeCbLocalCreditsNotification_t* pLeCbLocalCreditsNotification,
    uint8_t**                           ppBuffer
);

void fsciBleL2capCbGetLeCbErrorFromBuffer
(
    l2caLeCbError_t* pLeCbError,
    uint8_t**        ppBuffer
);

void fsciBleL2capCbGetBuffFromLeCbError
(
    l2caLeCbError_t* pLeCbError,
    uint8_t**        ppBuffer
);

#ifdef __cplusplus
}
#endif

#endif /* FSCI_BLE_L2CAP_CB_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
