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

#include "fsci_ble.h"
#include "fsci_ble_l2cap.h"
#include "fsci_ble_gatt.h"
#include "fsci_ble_gatt_db_app.h"
#include "fsci_ble_gap.h"

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

uint32_t fsciBleInterfaceId = 0xFF;             /* Indicates the FSCI interface that 
                                                will be used for monitoring */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void fsciBleRegister(uint32_t fsciInterfaceId)
{
    /* Register L2CAP command handler */
    if(FSCI_RegisterOpGroup(gFsciBleL2capOpcodeGroup_c, 
                            gFsciMonitorMode_c, 
                            fsciBleL2capHandler, 
                            NULL,
                            fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }

    /* Register GATT command handler */
    if(FSCI_RegisterOpGroup(gFsciBleGattOpcodeGroup_c, 
                            gFsciMonitorMode_c, 
                            fsciBleGattHandler, 
                            NULL,
                            fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }

    /* Register GATT Database (application) command handler */
    if(FSCI_RegisterOpGroup(gFsciBleGattDbAppOpcodeGroup_c, 
                            gFsciMonitorMode_c, 
                            fsciBleGattDbAppHandler, 
                            NULL,
                            fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }

    /* Register GAP command handler */
    if(FSCI_RegisterOpGroup(gFsciBleGapOpcodeGroup_c, 
                            gFsciMonitorMode_c, 
                            fsciBleGapHandler, 
                            NULL,
                            fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }
   
    /* Save FSCI interface to be used for monitoring */
    fsciBleInterfaceId = fsciInterfaceId;
}


clientPacketStructured_t* fsciBleAllocFsciPacket(opGroup_t opCodeGroup, uint8_t opCode, uint16_t dataSize)
{
    /* Allocate buffer for the FSCI packet (header, data, and CRC) */
    clientPacketStructured_t* pClientPacket = (clientPacketStructured_t*)MEM_BufferAlloc(sizeof(clientPacketHdr_t) + 
                                                                                         dataSize + 
                                                                                         2 * sizeof(uint8_t));
    
    if(NULL == pClientPacket)
    {
        /* Buffer can not be allocated */
        FSCI_Error(gFsciOutOfMessages_c, fsciBleInterfaceId);
        return NULL;
    }
    
    /* Create FSCI packet header */
    pClientPacket->header.opGroup   = opCodeGroup;
    pClientPacket->header.opCode    = opCode;
    pClientPacket->header.len       = dataSize;
    
    /* Return the allocated FSCI packet */
    return pClientPacket;
}


void fsciBleNoParamCmdOrEvtMonitor(opGroup_t opCodeGroup, uint8_t opCode)
{
    clientPacketStructured_t* pClientPacket;
    
    /* Allocate the FSCI packet to be transmitted over UART (with FSCI header added) */
    pClientPacket = fsciBleAllocFsciPacket(opCodeGroup, opCode, 0);
    
    if(NULL == pClientPacket)
    {
        /* FSCI packet can not be allocated */
        return;
    }
    
    /* Transmit FSCI packet over UART */
    FSCI_transmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
