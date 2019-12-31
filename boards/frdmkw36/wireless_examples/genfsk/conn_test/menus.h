/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "EmbeddedTypes.h"          /*Include special data types*/    
#include "SerialManager.h"
#include "gen_fsk_tests.h"
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern char * const cu8Logo[];
extern char * const cu8AppError[];
extern char * const cu8MainMenu[]; 
extern char * const cu8ShortCutsBar[];
extern char * const cu8ContinuousTestMenu[]; 
extern char * const cu8PerTxTestMenu[]; 
extern char * const cu8PerRxTestMenu[];
extern char * const cu8RangeTxTestMenu[];
extern char * const cu8RangeRxTestMenu[];
extern char * const cu8ContinuousTestTags[]; 
extern char * const cu8SelectTags[];
extern char * const cu8TxModTestTags[];
extern char * const cu8PerRxMessages[];
extern char * const cu8PerTxMessages[];
extern char * const cu8RangeRxMessages[];
extern char * const cu8RangeTxMessages[];
extern char * const cu8ContMessages[];
/************************************************************************************
*************************************************************************************
* Module macros
*************************************************************************************
************************************************************************************/

#define isAsciiHex(Data) (((Data >= '0') && (Data <= '9')) \
                       || ((Data >= 'A') && (Data <= 'F')) \
                       || ((Data >= 'a') && (Data <= 'f'))) 
               
/************************************************************************************
*************************************************************************************
* Interface functions prototypes
*************************************************************************************
************************************************************************************/
extern void PrintMenu(char * const pu8Menu[], uint8_t port);

extern void PrintTestParams(ct_config_params_t* params, 
                            bool_t bEraseLine, 
                            uint8_t port); 

extern void PrintPerTxPackets(uint16_t u16SentPackets,
                              bool_t bIsValidPacket,
                              uint8_t port);

extern void PrintPerRxInfo(uint16_t u16PacketsIndex, 
                           uint16_t u16ReceivedPackets, 
                           ct_rx_indication_t* pIndicationInfo, 
                           uint8_t port);

extern void PrintPerRxFinal(uint16_t u16Received,
                            uint16_t u16TotalPackets,
                            int8_t i8AvgRssi,
                            uint8_t port);

extern void PrintRangeTxDroppedPackets(uint16_t u16PacketsDropped, 
                                       uint8_t port);
							
extern void PrintAvgRssi(int8_t i8AverageRssi,
                         uint8_t port);

extern void PrintCrtRssi(int8_t i8CrtRssi,
                         uint8_t port);
