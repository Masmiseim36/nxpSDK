/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 *     Lowpower Configuration
 ********************************************************************************** */

/*! Enable/Disable PowerDown functionality in PwrLib - In case of troubles with lowpower,
      turn it off for debug purpose to ensure your application is fine without lowpower  */
#define cPWR_UsePowerDownMode           0

/*! Enable/Disable BLE Link Layer DSM - Need to be kept enabled if BLE is used
    Shall be disabled if BLE is not used, the BLE LL will not be woken up on every device wakeup (thus saving power ) */
#define cPWR_BLE_LL_Enable              1

/*! Maximum deep sleep duration in ms - Increase this value to sleep longer when there is
      no Link layer activity - 300 seconds default*/
#define cPWR_DeepSleepDurationMs        300000

/*! Enable / Disable the DCDC SW component
     Required in Buck mode to have optimim DCDC settings by Software during active and lowpower modes
     Should be disabled in DCDC bypass mode                                              */
#define gDCDC_Enabled_d                 1

/*! Default DCDC Mode used by the application gDCDC_Buck_c or gDCDC_Bypass_c
      Warning : requires HW changes on the board when changed to gDCDC_Bypass_c   */
#define APP_DCDC_MODE                   gDCDC_Buck_c

/*! Use Pswitch Mode when going to mode 4 (DCDC switched OFF) or RAM off mode
       instead of VLLS1 (DCDC must be in buck mode)
       In this case, DCDC should be set to manual startup (J38 in position 1-2)
       In manual mode, DCDC needs to be started with PSWITCH button (SW4)
       and you can only wake up the device with this button */
//#define gPWR_UsePswitchMode_d          1

/*! Enable/Disable Low Power Timers - When disabled, no lowpower timer will run in lowpower
      The chip will wakeup on Link layer counter                                 */
#define gTMR_EnableLowPowerTimers_d     1

/*! Default DCDC Battery Level Monitor interval in Ms - Set the flag to higher value, you are unlikely requested
    having Vbat monitor every 6 seconds. typical is 60 seconds. */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  6000

/*! Store RNG seed to allow faster wakeup time from RAM OFF
    Seed location value is stored in HW parameters and then retrieved from their on each wakeup from
      POR, Pswitch, VLLS0, VLLS1
     Warning : The user may want to clear the Seed from time to time for security reason. In this case,
       an erase of the rngSeed field in gHardwareParameters is required so the SW could generate a new Seed on next
       reboot (will increase the wakeup time)*/
#define gRngSeedHwParamStorage_d        1

/*! Enable XCVR calibration storage in Flash
    Store the XCVR calibration in HW parameters to allow faster wakeup from lowpower VLLS2/3
    Not used on wakeup from POR, Pswitch, VLLS0, VLLS1
    Clear the Radio calibration by erasing the xcvrCal field in gHardwareParameters.*/
#define gControllerPreserveXcvrDacTrimValue_d     1

/*! These flags are not required on this application
      Application only uses one shoot TX / RX so GenFSK does not require to go to DSM       */
//#define cPWR_EnableDeepSleepMode_7      1
//#define cPWR_GENFSK_LL_Enable           1

/*! Disable VLLS2/3 feature if set to 0 - Use LLS3 instead
       This application has not been tested with VLLS mode yet                              */
#define cPWR_EnableDeepSleepMode_8      0
#define cPWR_EnableDeepSleepMode_5      0

/*! Number of cycles of 16KHz to wait for LL to exit DSM
 * Oscillator stabilization/startup delay is reduced when accuracy is not critical. Applied while advertising only in mode 8
 * This is in X.Y for-mat where X is in terms of number of BT slots (625 us) and Y is in terms of number of clock periods of 16KHz clock input, required for RF oscillator to stabilize the clock output to the controller on its output pin, after os-cillator is turned ON. In this period the clock is assumed to be unstable, and so the controller does not turn on the clock to internal logic till this period is over. This means, the wake up from deep sleep mode must ac-count for this delay before the wakeup instant.
 *      Osc_startup_delay[7:5] is number of slots(625us)
 *      Osc_startup_delay[4:0 is number of clock periods of 16KHz clock
 *   (Warning: Max. value of Osc_startup_delay [4:0] sup-ported is 9. Therefore do not program value greater than 9)
 *  This setting is not used on this application since VLLS has not been tested
 */
//#define cPWR_BLE_LL_OscStartupDelayVllsAdv    (0x8U)


/*! On Pin wakeup from lowpower, Bypass the keyboard module and keyboard timer
      and call directly the application call back BleApp_HandleKeys() with parameter
      gKBD_EventPressPB2_c . This saves a subtantial amount of power as the device does not
      remain active during 400-500ms for debouncing
    This application requires to have this setting disabled  */
#define cPWR_BleAppHandleKeyDirectCall_d  0

/*! On wakeup from POR or lowpower modes where the 32KHz oscilator is disabled (Pswitch, VLLS0, eventually VLLS1), the
       32KHz crystal takes roughly 200-300 ms to set up. In this case, there are some issues when going to lowpower
       while the Link layer / Radio has already been initialized, the power consumption of the chip remains high.
       In addition to this, the Linklayer/Radio will not wakeup correctly in case 32KHz clock is not stable enough.
    By setting this Macro, the lowpower module will go to lowpower (LLS) immediatly after wakeup from
       POR, Pswitch mode, VLLS0 or VLLS1 (if 32K was disabled) until the 32KHz oscillator is running correctly.
       The first Advertisment event is delayed, but it ensures the device does not consume too high and
       Linklayer/Radio does not encounter any issue can because of this
    Note: In VLLS1, the 32khz is disabled when no LPTMR are running   */
#define PWR_WaitFor32kOscRestart          1


/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
 /* Defines the number of required keys for the keyboard module */
#define gKBD_KeysCount_c        2

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
/* Disable LEDs when lowpower is enabled */
#define gLEDSupported_d         0

#else
/* Specifies the number of required LEDs for the LED module */
#define gLEDsOnTargetBoardCnt_c 4

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1
#endif

/*! Enable Debug Console (PRINTF) */
#define gDebugConsoleEnable_d   1


/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */
/*! Maximum number of connections supported for this application */
#define gAppMaxConnections_c    5

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d        1

/*! Enable/disable use of bonding procedure */
#define gAppUseBonding_d        0

#define gPasskeyValue_c         999999

/* The GenFSK coexistence is controlled by the application */
#define gGENFSK_MwsControl_c    gGENFSK_AppMwsControl_c

/* Enable/disable the use of extended advertising */
#define gAppUseExtendedAdvertising 1

/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* Do not modify. Not used for this application */
#define gAppUseNvm_d                    1

/* Defines Num of Serial Manager interfaces */
#define gSerialManagerMaxInterfaces_c   1

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    10 _eol_  \
         _block_size_  64  _number_of_blocks_    6 _eol_  \
         _block_size_ 128  _number_of_blocks_    6 _eol_  \
         _block_size_ 512  _number_of_blocks_    6 _eol_


/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         5

/* Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            0

#define gMWS_Enabled_d                  1

/* Additional flags if lowpower is enabled */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
/*! Use the MemManager light in order to reach VLLS2 16K for this application on Bare metal*/
#define gMemManagerLight                1

/*! Selects the allocation scheme that will be used by the MemManagerLight
    0: Allocates the first free block available in the heap, no matter its size
    1: Allocates the first free block whose size is at most the double of the requested size
    2: Allocates the first free block whose size is at most the 4/3 of the requested size     */
#define cMemManagerLightReuseFreeBlocks 1

/* Prevent the MCU from entering lowpower during serial data transmission
   Meaningfull only when gSerialManagerMaxInterfaces_c is set to 1    */
#define gSerialMgr_DisallowMcuSleep_d   1

/* Keep the device active (in STOP mode) when receiving a byte from lowpower. This allows
   more bytes to be received before the device enters back to lowpower.
   Set to 0 if you don t expect any additional bytes
   When connected to a hostdevice on the same board, set this value to its minimum to save power.
     Current set to 600ms for human reaction when using an external terminal for demonstration
   Meaningfull only when gSerialManagerMaxInterfaces_c is set to 1    */
#define gAppSerialManagerKeepActiveMs_c 600
#endif

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */

/*! Defines number of OS events used */
#define osNumberOfEvents                6

/* specific Flasg definition when FreeRTOs is used */
#if defined(FSL_RTOS_FREE_RTOS)
#if !defined(gDebugConsoleEnable_d) || (gDebugConsoleEnable_d == 0)

/* Defines total heap size used by the OS */
#define gTotalHeapSize_c                9950

/*! Defines Size for Timer Task */
#define gTmrTaskStackSize_c             384
//#define gTmrTaskStackSize_c             650

/*! Defines Size for Idle Task  (minimum 572 is required) */
#define gAppIdleTaskStackSize_c         600

/*! Defines Size for Serial Manager Task (use 1500 in DEBUG with PRINTF)*/
#define gSerialTaskStackSize_c          700

/* Defines main task stack size */
#define gMainThreadStackSize_c          900

#else
/* When debug console is enabled, increase the stack sizes of the tasks to allow PRINTF,
       (PRINTF requires an additional buffer in the stack)
   Stack size increased to 1500Bytes - safe value, not optimized                    */
#define gTotalHeapSize_c                15000

#define gTmrTaskStackSize_c             1500
#define gAppIdleTaskStackSize_c         1500
#define gSerialTaskStackSize_c          1500
#define gMainThreadStackSize_c          1500
#endif

#endif

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */


/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */
#define gAppExtAdvEnable_d                      1
#define gLlScanPeriodicAdvertiserListSize_c     8
/*
 * Specific configuration of LL pools by block size and number of blocks for this application.
 * Optimized using the MEM_OPTIMIZE_BUFFER_POOL feature in MemManager,
 * we find that the most optimized combination for LL buffers.
 *
 * If LlPoolsDetails_c is not defined, default LL buffer configuration in app_preinclude_common.h
 * will be applied.
 */

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
