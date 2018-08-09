/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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
#include "MemManager.h"
#include "SerialManager.h"
#include "LED.h"                    /*Include all LED functionality*/ 
#include "TimersManager.h"
#include "SMAC_Interface.h"         /*@CMA Conn Test*/
#include "FunctionLib.h"
#include "AspInterface.h"
#include "fsl_os_abstraction.h"
#include "board.h"

#include "connectivity_test_menus.h"
/************************************************************************************
*************************************************************************************
* Currently supported feature-sets. 
* DO NOT CHANGE as they MIGHT be platform dependent
*************************************************************************************
************************************************************************************/
/*This feature is only for sub-ghz phy's*/
#ifndef CT_Feature_Bitrate_Select
#define CT_Feature_Bitrate_Select  (0)
#endif

/*This feature is only for platforms that have BER Test Connectors
*(currently only KW01)*/
#ifndef CT_Feature_BER_Test
#define CT_Feature_BER_Test        (0)
#endif

#ifndef CT_Feature_Direct_Registers
#define CT_Feature_Direct_Registers (0)
#endif

#ifndef CT_Feature_Indirect_Registers
#define CT_Feature_Indirect_Registers (0)
#endif

/*This feature is currently supported only on KW01 platforms*/
#ifndef CT_Feature_Calibration
#define CT_Feature_Calibration	   (0)
#endif

/*This feature is only for sub-ghz platforms*/
#ifndef CT_Feature_Custom_CCA_Dur
#define CT_Feature_Custom_CCA_Dur  (0)
#endif

/*This feature is currently supported on KW01. Disabled by default*/
#ifndef CT_Feature_Afc
#define CT_Feature_Afc             (0)
#endif

#ifndef CT_Feature_RSSI_Has_Sign
#define CT_Feature_RSSI_Has_Sign   (1)
#endif

/* This feature is currently supported on MKW41/MKW21. Disabled by default */
#ifndef CT_Feature_Xtal_Trim
 #if gAspCapability_d
 #define CT_Feature_Xtal_Trim      (1)
 #else
 #define CT_Feature_Xtal_Trim      (0)
 #endif
#endif

#if CT_Feature_Calibration
#include "Flash_Adapter.h"
#endif

/************************************************************************************
*************************************************************************************
* Macros
*************************************************************************************
************************************************************************************/
#define gMaxOutputPower_c	       ( 0x20 ) 
#define gMinOutputPower_c              ( 0x00 )
#define gDefaultOutputPower_c          ( 0x05 )

#if CT_Feature_Calibration
#define gMinAdditionalRFOffset_c       ( -1000)
#define gMaxAdditionalRFOffset_c       ( 1000 )
#endif
             
#define gDefaultChannelNumber_c         gChannel11_c                                    
#define gMaxCCAThreshold_c              0x6EU
#define gMinCCAThreshold_c              0x00U
#define gDefaultCCAThreshold_c          0x50U

#if CT_Feature_Xtal_Trim
#define gMaxTrimValue_c                 0x7FU
#define gMinTrimValue_c                 0x00U
#endif

/*register size in bytes and ASCII characters macros*/

#define gRegisterSize_c	     (4)
#define gRegisterSizeASCII_c (2*gRegisterSize_c)

/*register address size in ASCII*/
#define gRegisterAddress_c	(2)
#define gRegisterAddressASCII_c (2*gRegisterAddress_c)

/************************************************************************************
*************************************************************************************
* Memory Type Definitions
*************************************************************************************
************************************************************************************/

/* these type definitions must be changed depending on register size and address range*/
typedef uint32_t registerSize_t;
typedef uint16_t registerAddressSize_t;

/*this structure defines the upper and lower bound for dump registers feature*/
typedef struct registerLimits_tag
{
  registerAddressSize_t regStart;
  registerAddressSize_t regEnd;
  bool_t  bIsRegisterDirect;
}registerLimits_t;

typedef enum operationModes_tag
{
  mTxOperation_c,
  mRxOperation_c
}operationModes_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern const registerLimits_t registerIntervals[];

extern uint8_t crtBitrate;

extern bool_t evTestParameters;
extern bool_t evDataFromUART;
extern bool_t bEdDone;
extern bool_t shortCutsEnabled;

extern uint8_t testPower;
extern uint8_t mAppSer;
extern uint8_t gu8UartData;
extern uint8_t au8ScanResults[];
extern uint8_t ccaThresh;
extern uint8_t testPayloadLen;
extern channels_t testChannel;

extern osaEventId_t gTaskEvent;
extern operationModes_t testOpMode;

#if CT_Feature_Xtal_Trim
extern uint8_t xtalTrimValue;
#endif

/************************************************************************************
*************************************************************************************
* Public functions declarations
*************************************************************************************
************************************************************************************/

extern void PrintMenu(char * const pu8Menu[], uint8_t port);
/*common functions declarations which have platform dependent behavior*/
extern void PrintTestParameters(bool_t bEraseLine);
extern void ShortCutsParser( uint8_t u8UartData );
extern void InitApp_custom();
extern void InitProject_custom();

