/*
* The Clear BSD License
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


#ifndef APP_MAIN_H
#define APP_MAIN_H

/*!
\file       app_main.h
\brief      app main header file.
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "ZQueue.h"
#ifdef CPU_MKW41Z512VHT4
#include "mac_sap.h"
#else
#include "tsv_pub.h"
#endif
#include "bdb_api.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitResources(void);
PUBLIC void APP_vSetUpHardware(void);
PUBLIC void APP_vMainLoop(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern PUBLIC uint8 u8TimerButtonScan;
extern PUBLIC uint8 u8TimerRadioRecal;
extern PUBLIC uint8 u8TimerTick;
extern PUBLIC uint8 u8TimerPowerOn;
#ifdef CLD_GREENPOWER
extern PUBLIC uint8 u8GPTimerTick;
#endif
#ifdef APP_NTAG
extern PUBLIC uint8 u8TimerNtag;
#endif


extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

extern PUBLIC tszQueue APP_msgBdbEvents;
extern PUBLIC tszQueue APP_msgAppEvents;
#ifdef CLD_GREENPOWER
extern PUBLIC tszQueue APP_msgGPZCLTimerEvents;
extern uint8 au8GPZCLEvent[];
extern uint8 u8GPZCLTimerEvent;
#endif

#ifdef OTA_CLD_ATTR_REQUEST_DELAY
    extern PUBLIC uint8 u8TimerZclMsTick;
#endif

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H */






