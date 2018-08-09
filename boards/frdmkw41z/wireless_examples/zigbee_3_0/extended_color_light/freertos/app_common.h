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

#ifndef APP_COMMON_H_
#define APP_COMMON_H_

/*!
\file       app_common.h
\brief      ZLO Application common includes selector
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "app_reporting.h"

#ifdef DimmableLight
    #include "App_DimmableLight.h"
#endif

#ifdef ExtendedColorLight
    #include "App_ExtendedColorLight.h"
#endif

#ifdef ColorTemperatureLight
    #include "App_ColorTemperatureLight.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define APP_EXT_PAN_ID          0x1234567887654321ULL
#define NETWORK_RESTART_TIME    ZTIMER_TIME_MSEC(5000)
#define POLL_TIME               ZTIMER_TIME_MSEC(1000)
#define POLL_TIME_FAST          ZTIMER_TIME_MSEC(250)
#define GP_ZCL_TICK_TIME        ZTIMER_TIME_MSEC(1)



/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
    uint8 u8Effect;
    uint8 u8Tick;
    uint8 u8Red;
    uint8 u8Green;
    uint8 u8Blue;
    uint8 u8Level;
    uint8 u8Count;
    bool_t  bDirection;
    bool_t  bFinish;
} tsIdentifyColour;

typedef struct {
    uint8 u8Effect;
    uint8 u8Tick;
    uint8 u8Level;
    uint8 u8Count;
    bool_t  bDirection;
    bool_t  bFinish;
} tsIdentifyWhite;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC uint8 app_u8GetDeviceEndpoint(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC void vTAM_MLME_RxInCca(bool_t bEnable);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COMMON_H_*/
