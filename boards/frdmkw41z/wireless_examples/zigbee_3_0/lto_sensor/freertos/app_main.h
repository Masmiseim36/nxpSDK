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


#ifndef APP_MAIN_H_
#define APP_MAIN_H_

/*!
\file       app_main.h
\brief      ZLO Main Handler Interface
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "ZQueue.h"
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

extern PUBLIC uint8 u8TimerPoll;
extern PUBLIC uint8 u8TimerPowerOnCount;
extern PUBLIC uint8 u8TimerLightTempSensorSample;
extern PUBLIC uint8 u8TimerTick;
extern PUBLIC uint8 u8TimerBlink;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
extern PUBLIC uint8 u8TimerNtag;
#endif

extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

extern PUBLIC tszQueue APP_msgZpsEvents;
extern PUBLIC tszQueue APP_msgZclEvents;
extern PUBLIC tszQueue APP_msgAppEvents;
extern PUBLIC tszQueue APP_msgBdbEvents;

extern PUBLIC bool_t APP_bPersistantPolling;
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H_ */






