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
