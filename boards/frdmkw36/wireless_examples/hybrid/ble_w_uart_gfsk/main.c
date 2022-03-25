/*! *********************************************************************************
 * \addtogroup Wireless UART GFSK Hybrid application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the main source file for the Wireless UART GFSK Hybrid application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/

/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "Panic.h"
#include "SerialManager.h"
#include "MWS.h"
#include "PWR_Interface.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
#include "gatt_db_handles.h"
#endif

/* Connection Manager */
#include "ble_conn_manager.h"


/* Application */
#include "ApplMain.h"
#include "board.h"
#include "wireless_uart.h"
#include "genfsk_adv.h"

/*************************************************************************************
**************************************************************************************
* Private macros
**************************************************************************************
*************************************************************************************/

#ifndef APP_DBG_LOG
#define APP_DBG_LOG(...)
#endif

/*!< Time to keep the device out of low power to allow application interaction */
#ifndef APP_WAKE_UP_TIMEOUT
#define APP_WAKE_UP_TIMEOUT 3000
#endif

/************************************************************************************
 *************************************************************************************
 * Private type definitions
 *************************************************************************************
 ************************************************************************************/

/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/
static uint8_t gAppSerMgrIf;
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
static tmrTimerID_t mAppWakeUpTimerId;
#endif /* # defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1) */

/************************************************************************************
 *************************************************************************************
 * Private functions prototypes
 *************************************************************************************
 ************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Initialize application
*
********************************************************************************** */
void App_Init(void)
{
#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    /* Get Serial Manager interface - Serial manager has been initialized in hardware_init.c file */
    gAppSerMgrIf = BOARD_GetSerialManagerInterface();
#else
    SerialManager_Init();

    /* Register Serial Manager interface */
    (void)Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    (void)Serial_SetBaudRate(gAppSerMgrIf, APP_SERIAL_INTERFACE_SPEED);
#endif /* (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES)) */

    (void)Serial_Print(gAppSerMgrIf, "\n\r=====================================", gAllowToBlock_d);
    (void)Serial_Print(gAppSerMgrIf, "\n\rHybrid Wireless Uart - GFSK Adv demo", gAllowToBlock_d);
    (void)Serial_Print(gAppSerMgrIf, "\n\r=====================================\n\r", gAllowToBlock_d);

    /* init GFSK demo app */
    GfskApp_Init(gAppSerMgrIf);

    /* init BLE serial interface */
    BleApp_InitSerialInterface(gAppSerMgrIf);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
    mAppWakeUpTimerId = TMR_AllocateTimer();
    if (mAppWakeUpTimerId == gTmrInvalidTimerID_c)
    {
        (void)Serial_Print(gAppSerMgrIf, "\n\rNot enough timers\n\r", gAllowToBlock_d);
        panic(0, 0, 0, 0);
    }
#endif /* # defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1) */

#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    PWR_SetNewAppState(PWR_APP_STATE_NO_ACTIVITY_RAM_RET);
    PWR_AllowDeviceToSleep();

    APP_DBG_LOG("");
#endif /* (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES)) */
}


/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    static bool_t isGfskTxStarted = FALSE;
    static bool_t isGfskRxStarted = FALSE;

    switch (events)
    {
        case gKBD_EventPressPB1_c:
            gAppCodedPhyInUse = FALSE;
            BleApp_Start(gGapPeripheral_c);
            break;

        case gKBD_EventLongPB1_c:
            BleApp_Start(gGapCentral_c);
            break;

        case gKBD_EventPressPB2_c:
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
            if (TMR_IsTimerActive(mAppWakeUpTimerId) == FALSE)
            {
                /* start a timer to keep the device out of low power to allow application actions */
                TMR_StartSingleShotTimer(mAppWakeUpTimerId, APP_WAKE_UP_TIMEOUT, NULL, NULL);
            }
            else
#endif /* # defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1) */
            {
                /* start/stop GFSK TX */
                if(!isGfskTxStarted)
                {
                    isGfskTxStarted = TRUE;
                    GfskApp_StartTx();
                }
                else
                {
                    isGfskTxStarted = FALSE;
                    GfskApp_StopTx();
                }
            }
            break;

        case gKBD_EventLongPB2_c:
            /* start/stop GFSK RX */
            if(!isGfskRxStarted)
            {
                isGfskRxStarted = TRUE;
                GfskApp_StartRx();
            }
            else
            {
                isGfskRxStarted = FALSE;
                GfskApp_StopRx();
            }
            break;
        case gKBD_EventVeryLongPB1_c:
            /* start extended advertising on coded phy */
            gAppCodedPhyInUse = TRUE;
            BleApp_Start(gGapPeripheral_c);
            break;

        default:
            ; /* No action required */
            break;
    }
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
