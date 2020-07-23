/*! *********************************************************************************
* \addtogroup FSCI_BLE
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a source file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"

#if gFsciIncluded_c

#include "fsci_ble.h"

#if gFsciBleL2capCbLayerEnabled_d
    #include "fsci_ble_l2cap_cb.h"
#endif 

#if gFsciBleGattLayerEnabled_d
    #include "fsci_ble_gatt.h"
#endif 

#if gFsciBleGattDbAppLayerEnabled_d
    #include "fsci_ble_gatt_db_app.h"
#endif 

#if gFsciBleGapLayerEnabled_d
    #include "fsci_ble_gap.h"
#endif 

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

#if gFsciBleL2capCbLayerEnabled_d
    /* Register L2CAP CB command handler */
    if(fsciBleRegisterOpGroup(gFsciBleL2capCbOpcodeGroup_c, 
                              fsciBleL2capCbHandler, 
                              fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }
#endif /* gFsciBleL2capCbLayerEnabled_d */      

#if gFsciBleGattLayerEnabled_d
    /* Register GATT command handler */
    if(fsciBleRegisterOpGroup(gFsciBleGattOpcodeGroup_c, 
                              fsciBleGattHandler, 
                              fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }
#endif /* gFsciBleGattLayerEnabled_d */    

#if gFsciBleGattDbAppLayerEnabled_d
    /* Register GATT Database (application) command handler */
    if(fsciBleRegisterOpGroup(gFsciBleGattDbAppOpcodeGroup_c, 
                              fsciBleGattDbAppHandler, 
                              fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }
#endif /* gFsciBleGattDbAppLayerEnabled_d */       

#if gFsciBleGapLayerEnabled_d
    /* Register GAP command handler */
    if(fsciBleRegisterOpGroup(gFsciBleGapOpcodeGroup_c, 
                              fsciBleGapHandler, 
                              fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciBleRegister, 0, 0);
    }
#endif /* gFsciBleGapLayerEnabled_d */  
   
    /* Save FSCI interface to be used for monitoring */
    fsciBleInterfaceId = fsciInterfaceId;
}


#if gFsciBleBBox_d || gFsciBleTest_d
void fsciBleStatusMonitor(opGroup_t opCodeGroup, uint8_t opCode, bleResult_t result)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

    
    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleAllocFsciPacket(opCodeGroup, 
                                           opCode,
                                           sizeof(bleResult_t));
    
    if(NULL == pClientPacket)
    {
        return;
    }
    
    pBuffer = &pClientPacket->payload[0];
    
    /* Set status in the buffer */
    fsciBleGetBufferFromEnumValue(result, pBuffer, bleResult_t);
    
    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}
#endif /* gFsciBleBBox_d || gFsciBleTest_d */


clientPacketStructured_t* fsciBleAllocFsciPacket(opGroup_t opCodeGroup, uint8_t opCode, uint16_t dataSize)
{
    /* Allocate buffer for the FSCI packet (header, data, and CRC) */
    clientPacketStructured_t* pClientPacket = (clientPacketStructured_t*)MEM_BufferAlloc(sizeof(clientPacketHdr_t) + 
                                                                                         (uint32_t)dataSize + 
                                                                                         2U * sizeof(uint8_t));
    
    if(NULL == pClientPacket)
    {
        /* Buffer can not be allocated */
        fsciBleError(gFsciOutOfMessages_c, (uint8_t)fsciBleInterfaceId);
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
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#endif /* gFsciIncluded_c */

/*! *********************************************************************************
* @}
********************************************************************************** */
