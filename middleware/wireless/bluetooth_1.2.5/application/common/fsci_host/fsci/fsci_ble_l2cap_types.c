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
* This is a source file for FSCI Host BLE management.
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble_l2cap_types.h"


/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void fsciBleL2capGetLeCbConnectionRequestFromBuffer(l2caLeCbConnectionRequest_t* pLeCbConnectionRequest, uint8_t** ppBuffer)
{
    /* Read l2caLeCbConnectionRequest_t structure fields from buffer */
    fsciBleGetDeviceIdFromBuffer(&pLeCbConnectionRequest->deviceId, ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionRequest->lePsm, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionRequest->peerMtu, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionRequest->peerMps, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionRequest->initialCredits, *ppBuffer);
}


void fsciBleL2capGetBufferFromLeCbConnectionRequest(l2caLeCbConnectionRequest_t* pLeCbConnectionRequest, uint8_t** ppBuffer)
{
    /* Write l2caLeCbConnectionRequest_t structure fields in buffer */
    fsciBleGetBufferFromDeviceId(&pLeCbConnectionRequest->deviceId, ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionRequest->lePsm, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionRequest->peerMtu, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionRequest->peerMps, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionRequest->initialCredits, *ppBuffer);
}


void fsciBleL2capGetLeCbConnectionCompleteFromBuffer(l2caLeCbConnectionComplete_t* pLeCbConnectionComplete, uint8_t** ppBuffer)
{
    /* Read l2caLeCbConnectionComplete_t structure fields from buffer */
    fsciBleGetDeviceIdFromBuffer(&pLeCbConnectionComplete->deviceId, ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionComplete->cId, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionComplete->peerMtu, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionComplete->peerMps, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbConnectionComplete->initialCredits, *ppBuffer);
    fsciBleGetEnumValueFromBuffer(pLeCbConnectionComplete->result, *ppBuffer, l2caLeCbConnectionRequestResult_t);
}


void fsciBleL2capGetBufferFromLeCbConnectionComplete(l2caLeCbConnectionComplete_t* pLeCbConnectionComplete, uint8_t** ppBuffer)
{
    /* Write l2caLeCbConnectionComplete_t structure fields in buffer */
    fsciBleGetBufferFromDeviceId(&pLeCbConnectionComplete->deviceId, ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionComplete->cId, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionComplete->peerMtu, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionComplete->peerMps, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbConnectionComplete->initialCredits, *ppBuffer);
    fsciBleGetBufferFromEnumValue(pLeCbConnectionComplete->result, *ppBuffer, l2caLeCbConnectionRequestResult_t);
}


void fsciBleL2capGetLeCbDisconnectionFromBuffer(l2caLeCbDisconnection_t* pLeCbDisconnection, uint8_t** ppBuffer)
{
    /* Read l2caLeCbDisconnection_t structure fields from buffer */
    fsciBleGetDeviceIdFromBuffer(&pLeCbDisconnection->deviceId, ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbDisconnection->cId, *ppBuffer);
}


void fsciBleL2capGetBufferFromLeCbDisconnection(l2caLeCbDisconnection_t* pLeCbDisconnection, uint8_t** ppBuffer)
{
    /* Write l2caLeCbDisconnection_t structure fields in buffer */
    fsciBleGetBufferFromDeviceId(&pLeCbDisconnection->deviceId, ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbDisconnection->cId, *ppBuffer);
}


void fsciBleL2capGetLeCbNoPeerCreditsFromBuffer(l2caLeCbNoPeerCredits_t* pLeCbNoPeerCredits, uint8_t** ppBuffer)
{
    /* Read l2caLeCbNoPeerCredits_t structure fields from buffer */
    fsciBleGetDeviceIdFromBuffer(&pLeCbNoPeerCredits->deviceId, ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbNoPeerCredits->cId, *ppBuffer);
}


void fsciBleL2capGetBufferFromLeCbNoPeerCredits(l2caLeCbNoPeerCredits_t* pLeCbNoPeerCredits, uint8_t** ppBuffer)
{
    fsciBleGetBufferFromDeviceId(&pLeCbNoPeerCredits->deviceId, ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbNoPeerCredits->cId, *ppBuffer);
}


void fsciBleL2capGetLeCbLocalCreditsNotificationFromBuffer(l2caLeCbLocalCreditsNotification_t* pLeCbLocalCreditsNotification, uint8_t** ppBuffer)
{
    /* Read l2caLeCbLocalCreditsNotification_t structure fields from buffer */
    fsciBleGetDeviceIdFromBuffer(&pLeCbLocalCreditsNotification->deviceId, ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbLocalCreditsNotification->cId, *ppBuffer);
    fsciBleGetUint16ValueFromBuffer(pLeCbLocalCreditsNotification->localCredits, *ppBuffer);
}


void fsciBleL2capGetBufferFromLeCbLocalCreditsNotification(l2caLeCbLocalCreditsNotification_t* pLeCbLocalCreditsNotification, uint8_t** ppBuffer)
{
    /* Write l2caLeCbLocalCreditsNotification_t structure fields in buffer */
    fsciBleGetBufferFromDeviceId(&pLeCbLocalCreditsNotification->deviceId, ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbLocalCreditsNotification->cId, *ppBuffer);
    fsciBleGetBufferFromUint16Value(pLeCbLocalCreditsNotification->localCredits, *ppBuffer);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
