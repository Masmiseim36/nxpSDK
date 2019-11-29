/*! *********************************************************************************
* \defgroup HCI
* @{
********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the interface file for the HCI Transport module
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _HCI_TRANSPORT_H_
#define _HCI_TRANSPORT_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "hci_types.h"

#include "SerialManager.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#ifndef gHcitMaxPayloadLen_c
#define gHcitMaxPayloadLen_c    (gHcLeAclDataPacketLengthDefault_c + gHciAclDataPacketHeaderLength_c)
#endif

/* Enables Upward HCI Transport. 
   The controller sends HCI packets to be transported through the serial interface */
#ifndef gUseHciTransportUpward_d
#define gUseHciTransportUpward_d      0
#endif

/* Enables Downward HCI Transport. 
   The Host sends HCI packets to be transported through the serial interface */
#ifndef gUseHciTransportDownward_d
#define gUseHciTransportDownward_d    0
#endif

#if (gUseHciTransportDownward_d) && (gUseHciTransportUpward_d)
#error "Select maximum one HCI transport method!"
#endif

/* Interface configuration */
#ifndef gHcitInterfaceType_d
#define gHcitInterfaceType_d        (APP_SERIAL_INTERFACE_TYPE)
#endif

#ifndef gHcitInterfaceNumber_d
#define gHcitInterfaceNumber_d      (APP_SERIAL_INTERFACE_INSTANCE)
#endif

#ifndef gHcitInterfaceSpeed_d
#define gHcitInterfaceSpeed_d        (gUARTBaudRate115200_c)
#endif

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef bleResult_t (* hciTransportInterface_t)
(
    hciPacketType_t packetType,     /*!< HCI Packet Type. */
    void* pPacket,                  /*!< Pointer to packet payload. */
    uint16_t packetSize             /*!< Packet payload size. */  
);

typedef struct hcitConfigStruct_tag
{
    serialInterfaceType_t   interfaceType;
    uint8_t                 interfaceChannel;
    uint32_t                interfaceBaudrate;
    hciTransportInterface_t transportInterface;
}hcitConfigStruct_t;

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
* \param[in]    
*
* \param[out]   
*
* \return  
*
* \pre 
*
* \remarks 
*
********************************************************************************** */
bleResult_t Hcit_Init(hcitConfigStruct_t* hcitConfigStruct);

/*! *********************************************************************************
* \brief  
*
* \param[in]    pPacket
* \param[in]    packetSize
*
* \param[out]   
*
* \return  
*
* \pre 
*
* \remarks 
*
********************************************************************************** */
bleResult_t Hcit_SendPacket(hciPacketType_t packetType, void* pPacket, uint16_t packetSize);

/*! *********************************************************************************
* \brief  
*
* \param[in]    
*
* \param[out]   pPacket
* \param[out]   packetSize
*
* \return  
*
* \pre 
*
* \remarks 
*
********************************************************************************** */
bleResult_t Hcit_RecvPacket(void* pPacket, uint16_t packetSize);

#ifdef __cplusplus
    }
#endif 

#endif /* _HCI_TRANSPORT_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
