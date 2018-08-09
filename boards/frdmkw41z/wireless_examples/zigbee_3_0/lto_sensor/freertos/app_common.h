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
\brief      Application Device Type Includes
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "ZTimer.h"

/* Device specific includes
 * As a new light or controller is added it needs an entry here
 *
 */
#ifdef DimmerSwitch
    #include "App_DimmerSwitch.h"
#endif

#ifdef ColorDimmerSwitch
    #include "App_ColorDimmerSwitch.h"
#endif

#ifdef DimmableLight
    #include "App_DimmableLight.h"
#endif

#ifdef OTAColorDimmableLightWithOccupancy
    #include "App_OTAColorDimmableLightWithOccupancy.h"
#endif

#ifdef ColorTempTunableWhiteLight
    #include "App_ColorTempTunableWhiteLight.h"
#endif

#ifdef ColorDimmableLight
    #include "App_ColorDimmableLight.h"
#endif

#ifdef OccupancySensor
    #include "App_OccupancySensor.h"
#endif

#ifdef LightSensor
    #include "App_LightSensor.h"
#endif

#ifdef RemoteControl
    #include "App_RemoteControl.h"
#endif

#include "zcl_options.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define POLL_TIME               ZTIMER_TIME_MSEC(1000)
#define POLL_TIME_FAST          ZTIMER_TIME_MSEC(100)
#define TEN_HZ_TICK_TIME        ZTIMER_TIME_MSEC(100)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_STARTUP,
    E_JOINING_NETWORK,
    E_RUNNING
} teNODE_STATES;

typedef struct
{
    teNODE_STATES   eNodeState;
    teNODE_STATES   eNodePrevState;
#ifdef CLD_OTA
    bool bValid;
    uint64 u64IeeeAddrOfServer;
    uint16 u16NwkAddrOfServer;
    uint8 u8OTAserverEP;
#endif
}tsDeviceDesc;

extern tsDeviceDesc sDeviceDesc;
#define IIC_MASK    ((1<<SDA_PIN) | (1<<SCL_PIN))

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC uint8 app_u8GetDeviceEndpoint(void);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COMMON_H_*/
