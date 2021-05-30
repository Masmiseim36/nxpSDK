/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*
* Copyright 2019-2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 * 	KSDK Configuration
 ********************************************************************************** */
#define FSL_RTOS_FREE_RTOS              1

/*! *********************************************************************************
 *     App Configuration
 ***********************************************************************************/
/* Configure Flags to ease power measurements */
//#define gAppPowerMeasurement            1

/* Enables the console in the application.
   With the console, the application can process RX packet sent over UART, so its configuration can change (Data rate for example).
   If disabled, the application will start beaconing at startup with a default data rate, see gAppDefaultDataRate.
   Also, to reduce active time, less information will be printed.
   If cPWR_UsePowerDownMode is set to 1, the console is disabled. */
#define gAppEnableConsole               1

/* Defines the default data rate, useful when console is disabled to force a config
   Use gGenfskDR1Mbps_c or gGenfskDR2Mbps_c */
#define gAppDefaultDataRate             gGenfskDR2Mbps_c

/* advertising interval in ms. 0 in case of continuous transmission */
#define gHsBeaconAdvInterval_c          (250)

/* If enabled, device will go VLLS0/1 (RAMOFF) between 2 advertise events
   cPWR_UsePowerDownMode to 1 is required
   ADV can still be disabled by pressing SW2 when device is active.
   If this flag is not enabled, and cPWR_UsePowerDownMode is enabled, the device will go to STOP mode
   between ADV events. */
#define gAppGenfskRamoff                0

#define gDebugConsoleEnable_d           0

/*! *********************************************************************************
 * 	Drivers Configuration
 ********************************************************************************** */
 /* Defines the number of available keys for the keyboard module for this application */
#define gKBD_KeysCount_c        1
 
 /* Defines the number of available tsi electrodes for this application */
#define gKBD_TsiElectdCount_c   0

/* Specifies the number of physical LEDs on the target board */
#define gLEDsOnTargetBoardCnt_c 1

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1

/* Enable Generic FSK protocol in XCVR */
#define XCVR_GENFSK_ENABLED     1

/*! *********************************************************************************
 * 	Framework Configuration
 ********************************************************************************** */
/* Use the MemManager light in order to reach VLLS2 16K for this application on Bare metal*/
#define gMemManagerLight                1

/* Selects the allocation scheme that will be used by the MemManagerLight
    0: Allocates the first free block available in the heap, no matter its size
    1: Allocates the first free block whose size is at most the double of the requested size
    2: Allocates the first free block whose size is at most the 4/3 of the requested size     */
#define cMemManagerLightReuseFreeBlocks 1

/* Defines Num of Serial Manager interfaces */
#define gSerialManagerMaxInterfaces_c   1

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c  384

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define PoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6 _eol_  \
         _block_size_  64  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_    4 _eol_  \
         _block_size_ 512  _number_of_blocks_    4 _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         1

/* Defines number of timers needed by the protocol stack */
#define gTmrStackTimers_c               3

 /*! *********************************************************************************
 *     Lowpower Configuration
 ********************************************************************************** */
/* Enable/Disable PowerDown functionality in PwrLib
   For this application, you can press SW2 while the device is active to stop ADV,
   the device will go to no activity state (VLLS0/1/PSWITCH)
   The device can be woken up with SW3, and will start ADV again */
#define cPWR_UsePowerDownMode           0

/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers       1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              6

/* Default deep sleep duration in ms */ 
#define cPWR_DeepSleepDurationMs        30000

/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1

/* Default DCDC Mode used by the application */           
#define APP_DCDC_MODE                   gDCDC_Buck_c

/* Default DCDC Battery Level Monitor interval */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  600000

/* Enable XCVR calibration storage in Flash
    Store the XCVR calibration in HW parameters to allow faster wakeup from lowpower
    The Radio calibration can be cleared by setting the xcvrCal field in gHardwareParameters to 0xFF.*/
#define gGenfskPreserveXcvrDacTrimValue_d       1

/* On wakeup from POR or lowpower modes where the 32KHz oscilator is disabled (Pswitch, VLLS0, eventually VLLS1), the
       32KHz crystal takes roughly 200-300 ms to set up. In this case, there are some issues when going to lowpower
       while the Link layer / Radio has already been initialized, the power consumption of the chip remains high.
       In addition to this, the Linklayer/Radio will not wakeup correctly in case 32KHz clock is not stable enough.
    By setting this Macro, the lowpower module will go to lowpower (LLS) immediatly after wakeup from
       POR, Pswitch mode, VLLS0 or VLLS1 (if 32K was disabled) until the 32KHz oscillator is running correctly.
       The first Advertisment event is delayed, but it ensures the device does not consume too high and
       Linklayer/Radio does not encounter any issue can because of this
    Note: In VLLS1, the 32khz is disabled when no LPTMR are running   */
#define PWR_WaitFor32kOscRestart        1

/* Use Pswitch Mode when going to mode 4 (DCDC switched OFF) or RAM off mode
       instead of VLLS1 (DCDC must be in buck mode)
       In this case, DCDC should be set to manual startup (J38 in position 1-2)
       In manual mode, DCDC needs to be started with PSWITCH button (SW4)
       and you can only wake up the device with this button */
//#define gPWR_UsePswitchMode_d          1

/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines total heap size used by the OS */
#define gTotalHeapSize_c                6500

/* Defines number of OS events used */
#define osNumberOfEvents        5

/* Defines number of OS semaphores used */
#define osNumberOfSemaphores    1
             
/*! *********************************************************************************
 * 	Auto Configuration
 ********************************************************************************** */
#if defined(gAppPowerMeasurement) && (gAppPowerMeasurement == 1)
#undef cPWR_UsePowerDownMode
#undef gHsBeaconAdvInterval_c
#undef gDebugConsoleEnable_d
#undef gSerialManagerMaxInterfaces_c
#undef APP_DCDC_MODE

#define cPWR_UsePowerDownMode           1
#define gHsBeaconAdvInterval_c          (5000)
#define gDebugConsoleEnable_d           0
#define gSerialManagerMaxInterfaces_c   0                 /* Disable Serial Manager to reduce active time */
//#define APP_DCDC_MODE                   gDCDC_Bypass_c
#define APP_DCDC_MODE                   gDCDC_Buck_c
#endif

/* Disable LEDs when enabling low power */           
#if cPWR_UsePowerDownMode || gMWS_UseCoexistence_d
#define gLEDSupported_d 0           
#endif

#if cPWR_UsePowerDownMode
/* Disable console if lowpower is enabled */
#undef gAppEnableConsole
#define gAppEnableConsole       0
#endif
           
#if gMWS_UseCoexistence_d   
#undef gKBD_KeysCount_c
#define gKBD_KeysCount_c        1
#endif

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
