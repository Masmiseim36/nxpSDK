/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2019-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the configuration file for the lowpower master reference design application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*!
 *  Application specific configuration file only
 *  Board Specific Configuration shall be added to board.h file directly such as :
 *  - Number of button on the board,
 *  - Number of LEDs,
 *  - etc...
 */
/*! *********************************************************************************
 *     Board Configuration
 ***********************************************************************************/

 /*! Defines the number of required keys for the keyboard module */
#define gKBD_KeysCount_c                2

/*! Enable LED Module */
#define gLEDSupported_d                 0

/*! Enable Debug Console (PRINTF) */
#define gDebugConsoleEnable_d           0

/*! *********************************************************************************
 *     App Configuration
 ***********************************************************************************/

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d                1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d                1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d                0

#define gPasskeyValue_c                 999999

#define gMaxServicesCount_d             6

/*! Enable BLE5.0 features such as Extended Advertising and Extended Scanning.
If enabled, the AE Host lib is required.
the scanning PHY parameter can be whether 1M or Coded S=8 (LR) */
//#define gAppExtAdvEnable_d                1

/*! Change the PHY mode during connection event for RX and TX
      0 or not defined : No change - default to 1Mb
      1: Coded Phy
      2: 2Mb Phy                         */
//#define gAppConnectionPhyMode             2      /*  2Mb PHY   */

/*! Configure scan interval
    Default is gGapScanIntervalDefault_d (0x0010U)
    Max is gGapScanIntervalMax_d (0x4000U)
    Min is gGapScanIntervalMin_d (0x0004U) */
#define gAppScanInterval_d              0x0010U

/*! Configure scan window
    Default is gGapScanWindowDefault_d (0x0010U)
    Max is gGapScanWindowMax_d (0x4000U)
    Min is gGapScanWindowMin_d (0x0004U) */
#define gAppScanWindow_d                0x0010U

/*! Disable VLLS0/1 after connection and restart Scan
    Application still goes into VLLS0/1/Pswitch on button press during scanning */
#define gAppRestartScanAfterConnect     0

/*! Scanning period timeout in seconds
    The device stops scanning when the timer expires */
#define gAppScanningTimeout_c           30

/*! Application Disconnect Timeout in seconds
    The device will automaticaly disconnect when the timer expires
    Note the slave may disconnect before the timer expires
        Check flag gAppDisconnectOnTimeoutOnly_s on lp slave app_preinclude.h file */
#define gAppDisconnectTimeout_c         5

/*! When set to 1, the device will go to lowest lowpower mode with RAM retention (PWR_APP_STATE_NO_ACTIVITY_RAM_RET)
    when there is no RF activity :
      - PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE  if cPWR_EnableDeepSleepMode_8 is set (default)
      - PWR_LOWPOWER_LLS2_3_LL_DSM_MODE    if cPWR_EnableDeepSleepMode_8 unset
    When set to 0, the device will go to lowpower mode without RAM retention (PWR_APP_STATE_NO_ACTIVITY) :
        VLLS1, VLLS1, or Pswicth (PWR_LOWPOWER_VLLS0_1_PSWITCH_MODE) depending on DCDC configuration  */
#define gAppStateNoActivityRamRetention_d   0

/*! If defined, the application will start a lowpower timer when going into no activity state.
    When the timer expires, the device will wake up and start scanning again.
    If gAppStateNoActivityRamRetention_d is set to 1, this option is useful to be able to restart Scan
    on wake up from timer expiration. */
//#define gAppWakeUpTimerAfterNoActivityMs  60000


/*! *********************************************************************************
 *     Framework Configuration
 ***********************************************************************************/

/*! Use the MemManager light in order to reach VLLS2 16K for this application*/
#define gMemManagerLight                1

/*! Selects the allocation scheme that will be used by the MemManagerLight
    0: Allocates the first free block available in the heap, no matter its size
    1: Allocates the first free block whose size is at most the double of the requested size
    2: Allocates the first free block whose size is at most the 4/3 of the requested size     */
#define cMemManagerLightReuseFreeBlocks 1

/*! Enable NVM to be used as non volatile storage management by the host stack
    This is mandatory if one of these gAppUseBonding_d, gRngSeedHwParamStorage_d
      or gControllerPreserveXcvrDacTrimValue_d are set */
#define gAppUseNvm_d                    1

/*! Enable Serial Console with external device or host using the LPUART
    Used by SHELL
    Define the Number of Serial Manager interfaces - typical value is 0 or 1
    Need to be set in app_preinclude.h so board.c will enable the Serial peripheral
        restore on VLLS wakeup                                               */
#define gSerialManagerMaxInterfaces_c   1

/* Prevent the MCU to enter Sleep during serial data transmission */
#define gSerialMgr_DisallowMcuSleep_d   1

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6 _eol_  \
         _block_size_  64  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_   10 _eol_  \
         _block_size_ 512  _number_of_blocks_   10 _eol_

/*! Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         4

/*! Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            0


 /*! *********************************************************************************
 *     Lowpower Configuration
 ********************************************************************************** */

/*! Enable/Disable Low Power Timers - When disabled, no lowpower timer will run in lowpower
      The chip will wakeup on Link layer counter                                 */
#define gTMR_EnableLowPowerTimers_d     1

/*! Enable/Disable PowerDown functionality in PwrLib - In case of troubles with lowpower,
      turn it off for debug purpose to ensure your application is fine without lowpower  */
#define cPWR_UsePowerDownMode           1

/*! Enable/Disable BLE Link Layer DSM - Need to be kept enabled with BLE */
#define cPWR_BLE_LL_Enable              1

/*! Maximum deep sleep duration in ms - Increase this value to sleep longer when there is
      no Link layer activity */
#define cPWR_DeepSleepDurationMs        300000

/*! Number of slots(625us) that the hardware/software required to exit from LLSdeep sleep mode.
    Lowpower module will add one additional slot for Link layer restore when waking up from VLLS
    On FreeRTOS connected mode from a Master device, we need to add one more slot
      -> increase from 3 (default) to 4 slots                                                */
#if defined(FSL_RTOS_FREE_RTOS)
#define cPWR_BLE_LL_OffsetToWakeupInstant 4
#endif

/*! Enable / Disable the DCDC SW component
      Shall be enabled in buck mode to have optimized DCDC settings during active and lowpower modes
      Not required in DCDC bypass mode                                                   */
#define gDCDC_Enabled_d                 1

/*! Default DCDC Mode used by the application gDCDC_Buck_c or gDCDC_Bypass_c
      Warning :
     - in Buck mode , DCDC module shall be enabled (gDCDC_Enabled_d= 1)
     - in bypass mode , It is advised to disable DCDC module  (not needed)   */
#define APP_DCDC_MODE                   gDCDC_Buck_c

/*! Use Pswitch Mode when going to mode 4 (DCDC switched OFF) or RAM off mode
       instead of VLLS1 (DCDC must be in buck mode)
       In this case, DCDC should be set to manual startup (J38 in position 1-2)
       In manual mode, DCDC needs to be started with PSWITCH button (SW4)
       and you can only wake up the device with this button */
//#define gPWR_UsePswitchMode_d          1

/*! Default DCDC Battery Level Monitor interval in Ms - Set the flag to higher value, you are unlikely requested
       having Vbat monitor every 6 seconds. typical is 60 seconds.
    Vbat Monitoring is done in DCDC buck mode only on every wakeup. However, if the device remains active (such as in scan mode),
       then the battery measurement is done on periodic timer whose Interval value is given by this define     */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  60000

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

/*! Disable VLLS2/3 feature - Use LLS3 instead if uncommented    */
#define cPWR_EnableDeepSleepMode_8      0

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
 *     RTOS Configuration
 ***********************************************************************************/

/* specific Flag definition when FreeRTOs is used */
#if defined(FSL_RTOS_FREE_RTOS)
#if !defined(gDebugConsoleEnable_d) || (gDebugConsoleEnable_d == 0)

/*! Defines total heap size used by the OS */
#define gTotalHeapSize_c                9500

/*! Defines number of OS events used */
#define osNumberOfEvents                5

/*! Defines Size for Timer Task */
#define gTmrTaskStackSize_c             384

/*! Defines Size for Idle Task  (minimum 572 is required)*/
#define gAppIdleTaskStackSize_c         600

/*! Defines Size for Serial Manager Task */
#define gSerialTaskStackSize_c          700

/* Defines main task stack size */
//#define gMainThreadStackSize_c          1024

#else
/* When debug console is enabled, increase the stack sizes of the tasks to allow PRINTF,
       (PRINTF requires an additional buffer in the stack)
   Stack size increased to 1500Bytes - safe value, not optimized                    */
#define gTotalHeapSize_c                15000
#define osNumberOfEvents                5

#define gTmrTaskStackSize_c             1500
#define gAppIdleTaskStackSize_c         1500
#define gSerialTaskStackSize_c          1500
#define gMainThreadStackSize_c          1500
#endif

#endif

/*! *********************************************************************************
 *     BLE Stack Configuration
 ***********************************************************************************/



/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/

/*  Phy setting depending on gAppConnectionPhyMode previsouly set */
#if defined(gAppConnectionPhyMode ) &&  (gAppConnectionPhyMode == 2)     /* 2Mb Phy */
#define gConnInitiatePhyUpdateRequest_c         (1U)                     /* This flag needs to be set to update the PHY settings */
#define gConnDefaultTxPhySettings_c             (gLePhy2MFlag_c)
#define gConnDefaultRxPhySettings_c             (gLePhy2MFlag_c)
#define gConnPhyUpdateReqTxPhySettings_c        (gLePhy2MFlag_c)
#define gConnPhyUpdateReqRxPhySettings_c        (gLePhy2MFlag_c)

#elif defined(gAppConnectionPhyMode ) &&  (gAppConnectionPhyMode == 1)   /* Coded Phy */
#define gConnInitiatePhyUpdateRequest_c         (1U)                     /* This flag needs to be set to update the PHY settings */
#define gConnDefaultTxPhySettings_c             (gLePhyCodedFlag_c)
#define gConnDefaultRxPhySettings_c             (gLePhyCodedFlag_c)
#define gConnPhyUpdateReqTxPhySettings_c        (gLePhyCodedFlag_c)
#define gConnPhyUpdateReqRxPhySettings_c        (gLePhyCodedFlag_c)
#else
/*  1Mb Phy default : no change */
#endif

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"
#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
