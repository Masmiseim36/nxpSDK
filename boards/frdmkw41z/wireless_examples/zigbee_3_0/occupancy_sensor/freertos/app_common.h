/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
#define POLL_TIME_FAST          ZTIMER_TIME_MSEC(500)
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
