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


#ifndef APP_POWER_ON_COUNTER_H_
#define APP_POWER_ON_COUNTER_H_

/*!
\file       app_power_on_counter.h
\brief      ZLO Demo : Power On Counter  (Interface)
*/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define POWER_ON_COUNTER_DB_IN_MSEC 1750
#define POWER_ON_PRE_INIT_COUNTER_DB_IN_MSEC 250
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
    uint8 u8PowerOnCounter;
}tsPowerOnCounts;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vManagePowerOnCountLoadRecord(void);
PUBLIC void vManagePowerOnCountInit(void);
PUBLIC void vAPP_cbTimerPowerOnCount( void * pvParam);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/

/****************************************************************************/
/****************************************************************************/

#endif //APP_POWER_ON_COUNTER_H_
