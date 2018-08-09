/*! *********************************************************************************
* \addtogroup FSCI_HOST_BLE
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This is a header file for FSCI Host BLE management.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FSCI_BLE_L2CAP_TYPES_H
#define _FSCI_BLE_L2CAP_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble.h"
#include "l2ca_interface.h"
#include "l2ca_cb_interface.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#define fsciBleL2capGetLeCbConnectionRequestBufferSize(pLeCbConnectionRequest)  \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbConnectionRequest)->deviceId) +    \
        sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) +  sizeof(uint16_t))

#define fsciBleL2capGetLeCbConnectionCompleteBufferSize(pLeCbConnectionComplete)    \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbConnectionComplete)->deviceId) +       \
		sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + \
        sizeof(l2caLeCbConnectionRequestResult_t))

#define fsciBleL2capGetLeCbDisconnectionBufferSize(pLeCbDisconnection)      \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbDisconnection)->deviceId) +    \
        sizeof(uint16_t))

#define fsciBleL2capGetLeCbNoPeerCreditsBufferSize(pLeCbNoPeerCredits)      \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbNoPeerCredits)->deviceId) +    \
        sizeof(uint16_t))

#define fsciBleL2capGetLeCbLocalCreditsNotificationBufferSize(pLeCbLocalCreditsNotification)    \
        (fsciBleGetDeviceIdBufferSize(&(pLeCbLocalCreditsNotification)->deviceId) +             \
        sizeof(uint16_t) + sizeof(uint16_t))
          
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

void fsciBleL2capGetLeCbConnectionRequestFromBuffer
(
    l2caLeCbConnectionRequest_t*    pLeCbConnectionRequest, 
    uint8_t**                       ppBuffer
);

void fsciBleL2capGetBufferFromLeCbConnectionRequest
(
    l2caLeCbConnectionRequest_t*    pLeCbConnectionRequest, 
    uint8_t**                       ppBuffer
);

void fsciBleL2capGetLeCbConnectionCompleteFromBuffer
(
    l2caLeCbConnectionComplete_t*   pLeCbConnectionComplete, 
    uint8_t**                       ppBuffer
);

void fsciBleL2capGetBufferFromLeCbConnectionComplete
(
    l2caLeCbConnectionComplete_t*   pLeCbConnectionComplete, 
    uint8_t**                       ppBuffer
);

void fsciBleL2capGetLeCbDisconnectionFromBuffer
(
    l2caLeCbDisconnection_t*    pLeCbDisconnection, 
    uint8_t**                   ppBuffer
);

void fsciBleL2capGetBufferFromLeCbDisconnection
(
    l2caLeCbDisconnection_t*    pLeCbDisconnection, 
    uint8_t**                   ppBuffer
);

void fsciBleL2capGetLeCbNoPeerCreditsFromBuffer
(
    l2caLeCbNoPeerCredits_t*    pLeCbNoPeerCredits, 
    uint8_t**                   ppBuffer
);

void fsciBleL2capGetBufferFromLeCbNoPeerCredits
(
    l2caLeCbNoPeerCredits_t*    pLeCbNoPeerCredits, 
    uint8_t**                   ppBuffer
);

void fsciBleL2capGetLeCbLocalCreditsNotificationFromBuffer
(
    l2caLeCbLocalCreditsNotification_t* pLeCbLocalCreditsNotification, 
    uint8_t**                           ppBuffer
);

void fsciBleL2capGetBufferFromLeCbLocalCreditsNotification
(
    l2caLeCbLocalCreditsNotification_t* pLeCbLocalCreditsNotification, 
    uint8_t**                           ppBuffer
);

#ifdef __cplusplus
}
#endif 

#endif /* _FSCI_BLE_L2CAP_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */