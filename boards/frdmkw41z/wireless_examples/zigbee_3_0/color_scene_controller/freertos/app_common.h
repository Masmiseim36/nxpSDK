/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_COMMON_H_
#define APP_COMMON_H_

#define ColorSceneController

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#ifdef ColorSceneController
    #include "App_ColorSceneController.h"
#endif

#ifdef DimmerSwitch
    #include "App_DimmerSwitch.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define APP_EXT_PAN_ID          0x1234567887654321ULL
#define NETWORK_RESTART_TIME    ZTIMER_TIME_MSEC(5000)
#define POLL_TIME               ZTIMER_TIME_MSEC(1000)
#define POLL_TIME_FAST          ZTIMER_TIME_MSEC(250)




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

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC void vTAM_MLME_RxInCca(bool_t bEnable);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COMMON_H_*/
