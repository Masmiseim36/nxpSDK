/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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

#include "EmbeddedTypes.h"          /*Include special data types*/    
#include "SerialManager.h"
#include "gen_fsk_tests.h"
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern char * const cu8Logo[];
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
