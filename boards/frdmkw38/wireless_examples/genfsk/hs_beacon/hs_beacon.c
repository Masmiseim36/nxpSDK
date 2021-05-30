/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "MemManager.h"
#include "TimersManager.h"
#include "SerialManager.h"
#include "RNG_Interface.h"
#include "Messaging.h"
#include "SecLib.h"
#include "PWR_Interface.h"
#include "fsl_os_abstraction.h"

/* KSDK */
#include "board.h"

#ifdef FSL_RTOS_FREE_RTOS
#include "FreeRTOSConfig.h"
#endif

#include "genfsk_interface.h"
#include "hs_beacon.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

#ifdef FSL_RTOS_FREE_RTOS
#if (configUSE_IDLE_HOOK)
#define mAppIdleHook_c 1
#endif
#endif

#ifndef mAppIdleHook_c
#define mAppIdleHook_c 0
#endif

#define Reverse7Bit(i)  \
    ((((i)&0x01) << 6)| \
     (((i)&0x02) << 4)| \
     (((i)&0x04) << 2)| \
     (((i)&0x08)     )| \
     (((i)&0x10) >> 2)| \
     (((i)&0x20) >> 4)| \
     (((i)&0x40) >> 6))
      
/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

/* tx power */
#define gHsBeaconMaxTxPowerLevel_c        (32)
#define gHsBeaconMinTxPowerLevel_c        (0)
#define gHsBeaconDefaultTxPowerLevel_c    (32)

/* channel */
#define gHsBeaconMaxChannel_c             (0x7F)
#define gHsBeaconMinChannel_c             (0x00)
      
/* advertising address */
#define gHsBeaconAdvAdd_c                 (0x8E89BED6)

#define gGenfskDR1Mbps_c        0U
#define gGenfskDR2Mbps_c        3U
      
typedef enum
{
    gHsBeaconEvtTxDone_c       = 0x00000001U,
    gHsBeaconEvtTimerExpired_c = 0x00000002U,
    gHsBeaconEvtKBD_c          = 0x00000004U,
    gHsBeaconEvtUart_c         = 0x00000008U,
    gHsBeaconEvtEventsAll_c    = 0x0000000FU
} hs_beacon_event_t;

typedef struct
{ 
    uint8_t channel;
    uint8_t whiten_init;
} hs_beacon_chan_t;

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/* Application main */
static void App_Thread (uint32_t param);

/* Application event handler */
static void App_HandleEvents(osaEventFlags_t flags);

/* Application Init */
static void App_InitApp();

/* Generic FSK Notification callback */
static void App_GenFskEventNotificationCallback(genfskEvent_t event, genfskEventStatus_t eventStatus);

/* Timer callback */
static void App_TimerCallback(void* param);

/* Handles all keyboard events */
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
static void App_KeyboardCallBack(uint8_t events);
#endif

#if (cPWR_UsePowerDownMode)
#if (mAppIdleHook_c)
#define AppIdle_TaskInit()
#define App_Idle_Task()
#else
static osaStatus_t AppIdle_TaskInit(void);
static void App_Idle_Task(osaTaskParam_t argument);
#endif /* (mAppIdleHook_c) */
#endif /* (cPWR_UsePowerDownMode) */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* rate used */
genfskDataRate_t mAppRate = gGenfskDR2Mbps;

/* beacon transmission enabled */
bool_t mAppBeaconEnabled;

/* transmission is ongoing */
bool_t mAppTxRunning;

/* serial interface id */
uint8_t mAppSerId;

#if gHsBeaconAdvInterval_c != 0
/* timers manager app timer id */
uint8_t mAppTmrId;

/* timer has expired while transmission is still ongoing */
bool_t mAppStartTxAfterTimerExpiry;
#endif

/* initialization status */
static uint8_t platformInitialized = 0;

/* event used by the application thread */
static osaEventId_t mAppThreadEvt;

/* extern GENFSK instance id */
static uint8_t mAppGenfskId;

/* next transmission channel configuration table index */
static uint8_t mAppChannelIndex = 0;

/* advertising channels */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
/* If lowpower is enabled, reduce to 1 channel only to reduce active time */
const hs_beacon_chan_t mAppBeaconAdvChannel[]=
{
    {42,  Reverse7Bit(0x40+37)}, /* Freq = 2360MHz + 42*1MHz  = 2402MHz, BLE channel 37 */
};
#else
const hs_beacon_chan_t mAppBeaconAdvChannel[]=
{
    {42,  Reverse7Bit(0x40+37)}, /* Freq = 2360MHz + 42*1MHz  = 2402MHz, BLE channel 37 */
    {66,  Reverse7Bit(0x40+38)}, /* Freq = 2360MHz + 66*1MHz  = 2426MHz, BLE channel 38 */
    {120, Reverse7Bit(0x40+39)}  /* Freq = 2360MHz + 120*1MHz = 2480MHz, BLE channel 39 */
};
#endif

static const uint8_t mAppBeaconFrame[] = 
{
    (gHsBeaconAdvAdd_c)&0xff, (gHsBeaconAdvAdd_c>>8)&0xff, (gHsBeaconAdvAdd_c>>16)&0xff, (gHsBeaconAdvAdd_c>>24)&0xff,

    /* ADV_NONCONN_IND */
    0x00, 
    
    /* payload length */
    37, 
    
    /* advertiser address */
#ifdef TEST_GENFSK_ADV
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
#else
    0x01, 0x00, 0x00, 0x9f, 0x04, 0x00,
#endif /* TEST_GENFSK_ADV */
    
    /* AD element */
    0x02, 0x01, 0x06, 
    
    /* AD element */
    0x1b, 0xff, 0xff, 0x01, 0xbc, 0x8e, 0xf2, 0x22, 0x65, 0x3b, 0xef, 0x96, 0x81, 0x6d, 0x02, 0x2b, 0x96, 0x5f, 0xf6, 0xbc, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e
};

/* radio configuration */
static GENFSK_radio_config_t radioConfig = 
{
    .radioMode          = gGenfskGfskBt0p5h0p5,
#ifdef TEST_GENFSK_ADV
    .dataRate           = gGenfskDR1Mbps
#else
#if RADIO_IS_GEN_3P0
    .dataRate           = (genfskDataRate_t)gAppDefaultDataRate
#else
    .dataRate           = gGenfskDR1Mbps
#endif /* RADIO_IS_GEN_3P0 */
#endif /* TEST_GENFSK_ADV */
};

/* network / sync address configuration */
static GENFSK_nwk_addr_match_t ntwkAddr = 
{
    .nwkAddrSizeBytes   = 3, /* bytes = size + 1 */
    .nwkAddrThrBits     = 0,
    .nwkAddr            = gHsBeaconAdvAdd_c
};

/* packet configuration */
static GENFSK_packet_config_t pktConfig = 
{
    .preambleSizeBytes  = 0, /* 1 byte of preamble */
    .packetType         = gGenfskFormattedPacket,
    .lengthSizeBits     = 8,
    .lengthBitOrder     = gGenfskLengthBitLsbFirst,
    .syncAddrSizeBytes  = 3, /* bytes = size + 1*/
    .lengthAdjBytes     = 3, /* length field not including CRC so adjust by crc len */
    .h0SizeBits         = 8,
    .h1SizeBits         = 0,
    .h0Match            = 0,
    .h0Mask             = 0,
    .h1Match            = 0,
    .h1Mask             = 0
};

/* CRC configuration */
static GENFSK_crc_config_t crcConfig =
{
    .crcEnable          = gGenfskCrcEnable,
    .crcSize            = 3,
    .crcStartByte       = 4,
    .crcRefIn           = gGenfskCrcInputNoRef,
    .crcRefOut          = gGenfskCrcOutputNoRef,
    .crcByteOrder       = gGenfskCrcLSByteFirst,
    .crcSeed            = 0x00555555,
    .crcPoly            = 0x0000065B,
    .crcXorOut          = 0
};

/* whitener configuration */
static GENFSK_whitener_config_t whitenConfig = 
{
    .whitenEnable       = gGenfskWhitenEnable,
    .whitenStart        = gWhitenStartWhiteningAtH0,
    .whitenEnd          = gWhitenEndAtEndOfCrc,
    .whitenB4Crc        = gCrcB4Whiten,
    .whitenPolyType     = gGaloisPolyType,
    .whitenRefIn        = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit= gGenfskWhitenNoPayloadReinit,
    .whitenSize         = 7,
#ifdef TEST_GENFSK_ADV
    .whitenInit         = 0x53,
#else
    .whitenInit         = 0,
#endif /* TEST_GENFSK_ADV */
    .whitenPoly         = 0x11, /*x^7 + x^4 + 1! x^7 is never set*/
    .whitenSizeThr      = 0,
    .manchesterEn       = gGenfskManchesterDisable,
    .manchesterStart    = gGenfskManchesterStartAtPayload,
    .manchesterInv      = gGenfskManchesterNoInv
};

const char* u8Logo = 
#if defined(gAppEnableConsole) && (gAppEnableConsole == 1)
  "\n\r\n\r"
  " ####         ######      ##### ##########\n\r"
  " ######      # ######    ##### #############\n\r"
  " #######     ## ######  ##### ###############\n\r"  
  " ########    ### ########### ####       #####\n\r"  
  " #### #####  #### ######### #####       #####\n\r"   
  " ####  ##### #### ######### #################\n\r"  
  " ####   ######## ########### ###############\n\r" 
  " ####     ##### ######  ##### ############\n\r"
  " ####      ### ######    ##### ##\n\r"
  " ####       # ######      ##### #\n\r\n\r"
  " High Speed Beacon Demo\n\r\n\r"
  " To enable or disable beacon\n\r"
  " - Press [Enter] key in the terminal or\n\r"
  " - Press [SW2] button on the board\n\r"
  "\n\r"
  " To switch between 2 MBps and 1 MBps rate\n\r"
  " - Press [s] key in the terminal\n\r"
  "By default application uses 2 MBps rate.\n\r"
  "\n\r"
  " The LED D2 indicates the beacon status:\n\r"
  " - Off:      beacon is disabled\n\r"
  " - Blinking: beacon is enabled, LED toggles at each advertising event\n\r"
  "\n\r";
#else
  "\n\r\n\r"
  " High Speed Beacon Demo\n\r\n\r"
  " To enable or disable beacon\n\r"
  " - Press [SW2] button on the board\n\r"
  "\n\r";
#endif /* gAppDisableConsole */

#if (cPWR_UsePowerDownMode)
#if (!mAppIdleHook_c)
OSA_TASK_DEFINE( App_Idle_Task, gAppIdleTaskPriority_c, 1, gAppIdleTaskStackSize_c, FALSE );
osaTaskId_t gAppIdleTaskId = 0;
#endif
#endif  /* cPWR_UsePowerDownMode */


/************************************************************************************
*************************************************************************************
* Functions
*************************************************************************************
************************************************************************************/

#if gHsBeaconAdvInterval_c != 0
/* periodic timer callback */
static void App_TimerCallback(void* param)
{
    OSA_EventSet(mAppThreadEvt, gHsBeaconEvtTimerExpired_c);
}
#endif

#if defined(gAppEnableConsole) && (gAppEnableConsole == 1)
static void App_SerialCallback(void *param)
{
    OSA_EventSet(mAppThreadEvt, gHsBeaconEvtUart_c);
}
#endif /* gAppDisableConsole */

/* perform beacon transmission */
static void App_DoTx(void)
{
    genfskStatus_t status;
    
    assert(!mAppTxRunning);

    /* set whitener config */
    whitenConfig.whitenInit = mAppBeaconAdvChannel[mAppChannelIndex].whiten_init;
    GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
    
    /* set channel */
    GENFSK_SetChannelNumber(mAppGenfskId, mAppBeaconAdvChannel[mAppChannelIndex].channel);
    
    /* do tx */
    status = GENFSK_StartTx(mAppGenfskId, mAppBeaconFrame, sizeof(mAppBeaconFrame), 0);
    assert(status==gGenfskSuccess_c);
    
    /* remove not used warning in case of assert defined as ((void)0) */
    (void)status;
    
#if !(cPWR_UsePowerDownMode)
    if(mAppChannelIndex==0)
    {
        /* start flashing */
        Led1Toggle();
    }
#endif
    
    mAppChannelIndex++;
    if( mAppChannelIndex >= sizeof(mAppBeaconAdvChannel)/sizeof(mAppBeaconAdvChannel[0]) )
        mAppChannelIndex = 0;
    
    mAppTxRunning = TRUE;
    
#if gHsBeaconAdvInterval_c != 0
    if(mAppStartTxAfterTimerExpiry)
        mAppStartTxAfterTimerExpiry = FALSE;
#endif
}

/* beacon enabling */
static void App_StartBeacon(void)
{
    Serial_Print(mAppSerId, "\n\r Beacon transmission enabled.", gAllowToBlock_d);

    assert(!mAppBeaconEnabled);
    assert(!mAppTxRunning);

    mAppChannelIndex = 0;
    App_DoTx();

#if gHsBeaconAdvInterval_c != 0
    TMR_StartLowPowerTimer(mAppTmrId, gTmrLowPowerIntervalMillisTimer_c, gHsBeaconAdvInterval_c, App_TimerCallback, NULL);
#endif

    mAppBeaconEnabled = TRUE;
}

/* beacon disabling */
static void App_StopBeacon(void)
{
    Serial_Print(mAppSerId, "\n\r Beacon transmission disabled.", gAllowToBlock_d);

    assert(mAppBeaconEnabled);

#if !(cPWR_UsePowerDownMode)
    /* stop led */
    Led1Off();
#endif

#if gHsBeaconAdvInterval_c != 0
    TMR_StopTimer(mAppTmrId);
#endif

    if(mAppTxRunning)
    {
        GENFSK_AbortAll();
        mAppTxRunning = FALSE;
    }

    mAppBeaconEnabled = FALSE;

#if (cPWR_UsePowerDownMode)
    /* Change App State to no activity to go VLLS0/1
       Wake up by SW3 button */
    PWR_SetNewAppState(PWR_APP_STATE_NO_ACTIVITY);
#endif
}

/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param)
{
    if (!platformInitialized)
    {
        platformInitialized = 1;

        /* Framework init */
        MEM_Init();
        TMR_Init();
        LED_Init();

#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
        KBD_Init(App_KeyboardCallBack);
#endif

        GENFSK_Init();

        /* GENFSK LL Init with default register config */
        GENFSK_AllocInstance(&mAppGenfskId, NULL, NULL, NULL);

        /*create app thread event*/
        mAppThreadEvt = OSA_EventCreate(TRUE);

#if (cPWR_UsePowerDownMode)
#if (!mAppIdleHook_c)
        AppIdle_TaskInit();
#endif
        PWR_Init();
        //PWR_DisallowDeviceToSleep();
        /* allow only stop or sleep mode */
        PWR_DisallowXcvrToSleep();
#endif

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
        /* Serial Manager has been initialized in hardware_init
           the application has to retrieve the interface id*/
        mAppSerId = BOARD_GetSerialManagerInterface();
#endif

#if defined(gAppEnableConsole) && (gAppEnableConsole == 1)
        /*set Serial Manager receive callback*/
        Serial_SetRxCallBack(mAppSerId, App_SerialCallback, NULL);
#endif /* gAppDisableConsole */
        
#if gHsBeaconAdvInterval_c != 0
        /*allocate a timer*/
        mAppTmrId = TMR_AllocateTimer();
#endif

        /* initalize the app */
        App_InitApp();

        Serial_Print(mAppSerId, u8Logo, gAllowToBlock_d);

#if !defined(gAppEnableConsole) || (gAppEnableConsole == 0)

#if defined(gAppDefaultDataRate) && (gAppDefaultDataRate == gGenfskDR2Mbps_c)
        Serial_Print(mAppSerId, "\n\r Data rate is 2Mbps \n\r", gAllowToBlock_d);
#elif defined(gAppDefaultDataRate) && (gAppDefaultDataRate == gGenfskDR1Mbps_c)
        Serial_Print(mAppSerId, "\n\r Data rate is 1Mbps \n\r", gAllowToBlock_d);
#endif /* gAppDefaultDataRate */

        /* No console to Start Beacon, so we can start it now */
        App_StartBeacon();
#endif
        
    }

    /* Call application task */
    App_Thread( param );
}

/*! *********************************************************************************
* \brief  This function represents the Application task.
*         This task reuses the stack alocated for the MainThread.
*         This function is called to process all events for the task. Events
*         include timers, messages and any other user defined events.
* \param[in]  argument
*
********************************************************************************** */
static void App_Thread (uint32_t param)
{
    osaEventFlags_t mAppThreadEvtFlags = 0;
    
    while(1)
    {
        OSA_EventWait(mAppThreadEvt, gHsBeaconEvtEventsAll_c, FALSE, osaWaitForever_c ,&mAppThreadEvtFlags);
        if(mAppThreadEvtFlags)
        {
            App_HandleEvents(mAppThreadEvtFlags);/*handle app events*/
        }
        if(gUseRtos_c == 0) /* if bare-metal break while */
        {
            break;
        }
    }
}

#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
static void App_KeyboardCallBack(uint8_t events)
{
    switch (events)
    {
        case gKBD_EventPB1_c:
            OSA_EventSet(mAppThreadEvt, gHsBeaconEvtKBD_c);
            break;
        default:
            break;
    }
}
#endif

/*! *********************************************************************************
* \brief  The application event handler
*         This function is called each time there is an OS event for the AppThread
* \param[in]  flags The OS event flags specific to the Connectivity Test App.
*
********************************************************************************** */
static void App_HandleEvents(osaEventFlags_t flags)
{
#if gHsBeaconAdvInterval_c != 0
    if(flags & gHsBeaconEvtTimerExpired_c)
    {
        mAppStartTxAfterTimerExpiry = TRUE;
        if( !mAppTxRunning )
        {
            App_DoTx();
        }
    }
#endif

    if(flags & gHsBeaconEvtTxDone_c)
    {
        mAppTxRunning = FALSE;
        if(mAppBeaconEnabled)
        {
#if gHsBeaconAdvInterval_c == 0
            /* continuous transmission */
            App_DoTx();
#else
            /* periodic transmission */
            if( mAppChannelIndex != 0 || mAppStartTxAfterTimerExpiry )
            {
                App_DoTx();
            }
#if (cPWR_UsePowerDownMode)
            else
            {
                /* All channels have been covered, allow device to sleep */
                //PWR_AllowDeviceToSleep();
                /* allow deep sleep modes */
                PWR_AllowXcvrToSleep();
#if defined(gAppGenfskRamoff) && (gAppGenfskRamoff == 1)
                /* Change App State to no activity to go VLLS0/1
                   Device will be woken up at LPTMR expiration and will restart ADV */
                PWR_SetNewAppState(PWR_APP_STATE_NO_ACTIVITY);
#endif
            }
#endif

#endif
        }
    }
    
#if defined(gAppEnableConsole) && (gAppEnableConsole == 1)
    if(flags & gHsBeaconEvtUart_c)
    {
        bool_t beacon = mAppBeaconEnabled;
        genfskDataRate_t rate = mAppRate;

        uint16_t u16SerBytesCount = 0;
        uint8_t data;
        while(1)
        {
            Serial_RxBufferByteCount(mAppSerId, &u16SerBytesCount);
            if(u16SerBytesCount==0)
                break;
            if(gSerial_Success_c == Serial_Read(mAppSerId, &data, 1, &u16SerBytesCount))
            {
                if(data=='\r')
                {
                    beacon = !beacon;
                }
                else if (data=='s')
                {
                    if (rate == gGenfskDR1Mbps)
                    {
                        rate = gGenfskDR2Mbps;
                    }
                    else
                    {
                        rate = gGenfskDR1Mbps;
                    }
                }
            } 
        }
        
        if( beacon != mAppBeaconEnabled )
        {
            if (mAppBeaconEnabled)
                App_StopBeacon();
            else
                App_StartBeacon();
        }
        
        if (rate != mAppRate)
        {
            if (rate == gGenfskDR1Mbps)
            {
                Serial_Print(mAppSerId, "\n\r Rate switched to 1 MBps.", gAllowToBlock_d);
            }
            else
            {
                Serial_Print(mAppSerId, "\n\r Rate switched to 2 MBps.", gAllowToBlock_d);              
            }
            radioConfig.dataRate = rate;
            if (mAppBeaconEnabled)
            {
                /* If ongoing advertising stop before changing configuration and then re-enable it. */
                App_StopBeacon();
                GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
                App_StartBeacon();
            }
            else
            {
                GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
            }
            mAppRate = rate;
        }
    }
#endif /* gAppDisableConsole */

    if(flags & gHsBeaconEvtKBD_c)
    {
        if (mAppBeaconEnabled)
            App_StopBeacon();
        else
            App_StartBeacon();
    }
}

/*! *********************************************************************************
* \brief  Application initialization. It installs the main menu callbacks and
*         calls the Connectivity Test for Generic FSK init.
*
********************************************************************************** */
static void App_InitApp()
{
   /*register callbacks for the generic fsk LL */
   GENFSK_RegisterCallbacks(mAppGenfskId,
                            NULL,
                            App_GenFskEventNotificationCallback);

    /*set bitrate*/
    GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
    /*set packet config*/
    GENFSK_SetPacketConfig(mAppGenfskId, &pktConfig);
    /*set crc config*/
    GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);

    /*set network address at location 0 and enable it*/
    GENFSK_SetNetworkAddress(mAppGenfskId, 0, &ntwkAddr);
    GENFSK_EnableNetworkAddress(mAppGenfskId, 0);

    /*set tx power level*/
    GENFSK_SetTxPowerLevel(mAppGenfskId, gHsBeaconDefaultTxPowerLevel_c);

}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK event notification callback.
*         This function is called each time the Generic FSK Link Layer has
*         a notification for the upper layer
* \param[in]  event The event that generated the notification
* \param[in]  eventStatus status of the event
*
********************************************************************************** */
static void App_GenFskEventNotificationCallback(genfskEvent_t event, genfskEventStatus_t eventStatus)
{
    assert(event == gGenfskTxEvent);
    assert(eventStatus == gGenfskSuccess);
       
    /*send event done*/
    OSA_EventSet(mAppThreadEvt, gHsBeaconEvtTxDone_c);
}

/*! *********************************************************************************
* \brief  Prints the test parameters: mode, channel, TX power and payload length.
*         Some or all of the above parameters are used throughout the tests.
********************************************************************************** */

#if (cPWR_UsePowerDownMode)
static void App_Idle(void)
{
    if( PWR_CheckIfDeviceCanGoToSleep() )
    {
        /* Enter Low Power */
        (void)PWR_EnterLowPower();
    }
    else
    {
        /* Enter MCU Sleep */
        PWR_EnterSleep();
    }
}
#endif /* cPWR_UsePowerDownMode */

#if (mAppIdleHook_c)
void vApplicationIdleHook(void)
{
#if (cPWR_UsePowerDownMode)
    App_Idle();
#endif
}
#else /* mAppIdleHook_c */
#if (cPWR_UsePowerDownMode)
static void App_Idle_Task(osaTaskParam_t argument)
{
    while(1)
    {
        App_Idle();

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

static osaStatus_t AppIdle_TaskInit(void)
{
    if(gAppIdleTaskId)
    {
        return osaStatus_Error;
    }

    /* Task creation */
    gAppIdleTaskId = OSA_TaskCreate(OSA_TASK(App_Idle_Task), NULL);

    if( NULL == gAppIdleTaskId )
    {
        assert(0);
        return osaStatus_Error;
    }

    return osaStatus_Success;
}
#endif /* cPWR_UsePowerDownMode */
#endif /* mAppIdleHook_c */

