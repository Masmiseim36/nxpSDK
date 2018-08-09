/*! *********************************************************************************
* \defgroup FSCI_HOST_BLE
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

#ifndef _FSCI_BLE_H
#define _FSCI_BLE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "MemManager.h"
#include "fsci_ble_interface.h"
#include "fsci_ble_types.h"

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

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/*! FSCI interface on which the monitored commands, events or statuses 
should be printed */
extern uint32_t fsciBleInterfaceId;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
* \brief  Allocates a FSCI packet.
*
* \param[in]    opCodeGroup     FSCI operation group.
* \param[in]    opCode          FSCI operation code.
* \param[in]    dataSize        Size of the payload.
*
* \return The allocated FSCI packet
*
********************************************************************************** */
clientPacketStructured_t* fsciBleAllocFsciPacket
(
    opGroup_t   opCodeGroup,
    uint8_t     opCode, 
    uint16_t    dataSize
);

/*! *********************************************************************************
* \brief  Creates a FSCI packet without payload and sends it over UART.
*
* \param[in]    opCodeGroup     FSCI operation group.
* \param[in]    opCode          FSCI operation code.
*
********************************************************************************** */
void fsciBleNoParamCmdOrEvtMonitor
(
    opGroup_t   opCodeGroup, 
    uint8_t     opCode
);

#ifdef __cplusplus
}
#endif 

#endif /* _FSCI_BLE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */