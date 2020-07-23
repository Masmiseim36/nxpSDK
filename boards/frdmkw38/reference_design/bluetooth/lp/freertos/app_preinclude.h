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
* This file is the configuration file for the lowpower slave reference design application
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

/*! Configure Flags to ease power measurements */
//#define gAppPowerMeasurement            1

 /*! Defines the number of required keys for the keyboard module */
#define gKBD_KeysCount_c                2

/*! Enable LED Module */
#define gLEDSupported_d                 0

/*! Enable Debug Console (PRINTF) */
#define gDebugConsoleEnable_d           0

/*! *********************************************************************************
 *     App Configuration
 ***********************************************************************************/

/*! Set maximum number of connection supported by the application
 *  Set by default to 1 in app_preinclude_common.h
 *  Overriden to 2 connection for this application here
 *  Connection number over 2 is not tested.
 *  You may face issues when setting gAppAdvUseLowPowerTimers to 1.
 *  Best configuration to test 2 connections is:
 *  - using lp_master and a phone.
 *  - bond both devices seperately.
 *  - set gAppDisconnectOnTimeoutOnly_s to 1.
 *  - increase gAppConnectionTimeoutInSecs_c to 30sec. */
#define gAppMaxConnections_c           (2U)

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d                1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d                1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d                0

#define gPasskeyValue_c                 999999

/*! Enable BLE5.0 features such as Extended Advertising and Extended Scanning.
    If enabled, the AE Host lib is required     */
//#define gAppExtAdvEnable_d              1

/*! Set the default Tx power in dBm - Range is between -127 and 5*/
#define mAdvertisingDefaultTxPower_c    0
#define mConnectionDefaultTxPower_c     0

/*! ADV interval in Slots : 0x0030 matches to 30ms, 0x320 matches to 500ms
    If gAppAdvertisingInterval is above 0x4000, then LowPower mode 9 will be used
       rather than Link layer HW counter - see  gAppAdvOnWakeUp  */
#define gAppAdvertisingInterval         0x320

/*! ADV interval while connected if ADV is restarted for multiconnection
    WARNING : It is only used if gAppAdvOnWakeUp is set to 1 */
#define gAppAdvertisingIntervalConnected 0x500

/*! ADV expiration timeout - Stop ADV when timers expires (in seconds) */
#define gAppAdvTimeout_c                30

/*! ADV empty payload - Empty the ADV payload
    Will reduce the payload to the Header (2 bytes) and the advertiser device address (6 bytes).
    This option reduces the power consumption when there is no information to be transmitted in
       advertising message (ADV for connection only or if Data is contained in the
       Scan Rsp payload (see gAppUseScanRsp)
    Save power as the TX ADV time duration is smaller */
#define gAppAdvNoPayload_d              0

/*! Put ADV message in the Scan Response
    Useful when gAppAdvNoPayload_d is 1 to save power: data will be transmitted only as scan response
      Warning : Not supported by the lp master app - requires app update
      Warning : Meaningless if gAppConnectableAdv_d is 0
    Warning, Size of the ScanRSP payload in addition to ADV payload shall not exceed 46 bytes
      If it exceeds 46 bytes, then the TX interslot interval shall be increased to 1.825ms (3 slots)
      impacting significantly the active time in ADV and power consumption */
#define gAppUseScanRsp                  0

/*! ADV connectable - Set to 0 if ADV is not connectable,
    Save some power by removing the RX scan after the TX  and reducing the TX interslot interval */
#define gAppConnectableAdv_d            1

/*! Disable VLLS0/1 after connection and restart ADV - used for stress tests
    Application still goes into VLLS0/1 on button press */
#define gAppRestartAdvAfterConnect      0

/*! Connection timeout value after connection establishement
   If gAppDisconnectOnTimeoutOnly_s is kept to 0, the connection ends when the Connection timeout
      expires. However, in case the temperature request is not received (could be caused by unsuccesful
      pairing or bonding or any other reason), The connection ends on timeout expiration,
      timeout value given by this define.
   Warning: when bonding/pairing is enabled, it can take up to 8 seconds to connect and
      get temperature request. The connection timeout shall be big enough to allow this.        */
#define gAppConnectionTimeoutInSecs_c   10 /* 10 s*/

/*! Disconnect when timeout expires only, do not disconnect when temperature is sent.
    When set to 0, the disconnect happens immediatly after the temperature is sent
    When set to 1, the disconnect happens only on Connection timeout expiration ( value
       given by gAppConnectionTimeoutInSecs_c )
    Set the flag to 1 when carrying power measurement to allow more time in connected mode     */
#define gAppDisconnectOnTimeoutOnly_s   0

/*! When set to 1, the device will go to lowest lowpower mode with RAM retention (PWR_APP_STATE_NO_ACTIVITY_RAM_RET)
    when there is no RF activity :
      - PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE  if cPWR_EnableDeepSleepMode_8 is set (default)
      - PWR_LOWPOWER_LLS2_3_LL_DSM_MODE    if cPWR_EnableDeepSleepMode_8 unset
    When set to 0, the device will go to lowpower mode without RAM retention (PWR_APP_STATE_NO_ACTIVITY) :
        VLLS1, VLLS1, or Pswicth (PWR_LOWPOWER_VLLS0_1_PSWITCH_MODE) depending on DCDC configuration  */
#define gAppStateNoActivityRamRetention_d   0

/*! Use VLLS mode in connected mode.
    There is no really good reason to use VLLS in connected mode because the VLLS mode takes one more slot to
        wakeup (Link layer restore) so it is more power efficient to stick on LLS mode  */
//#define cPWR_UseVllsInConnectedMode      1

/*! If defined, the application will start a lowpower timer when going into no activity state.
    When the timer expires, the device will wake up and start advertising again.
    If gAppStateNoActivityRamRetention_d is set to 1, this option is useful to be able to restart ADV
    on wake up from timer expiration. */
//#define gAppWakeUpTimerAfterNoActivityMs  60000

/*! *********************************************************************************
 *     Framework Configuration
 ***********************************************************************************/

/*! Use the MemManager light in order to reach VLLS2 16K for this application on Bare metal*/
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
    Define the Number of Serial Manager interfaces - typical value is 0 or 1
      Lowpower does not support more than 1 serial interface
    Warning   : Memory usage is increased  when set to 0 , roughly :
      1KB of RAM, several KB of Flash (depend of the string size)     */
#define gSerialManagerMaxInterfaces_c   1

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

/*! Defines pools by block size and number of blocks. Must be aligned to 4 bytes.
    Warning : This is no longer used when using the Memory ManagerLight (see gMemManagerLight)
      Keep it only in case gMemManagerLight is disabled */
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6 _eol_  \
         _block_size_  64  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_   10 _eol_  \
         _block_size_ 512  _number_of_blocks_    8 _eol_

/*! Defines the number of timers needed by the application. The application requires the following timers:
      - ADV timeout  : ADV is stopped on timeout. refer to gAppAdvTimeout_c
      - Connection timeout  : one timer for each devices to disconnect on timeout , refer to gAppConnectionTimeoutInSecs_c
      - Serial Manager timeout : refer to gAppSerialManagerKeepActiveMs_c - required if gSerialManagerMaxInterfaces_c is set
      - RAM off in ADV with Pswitch : timer to disable lowpower until 32K is running on connection establishment
                      required if gPWR_UsePswitchMode_d and gPWR_RamOffDuringAdv_d are set        */
#if defined(gAppMaxConnections_c)
#define gTmrApplicationTimers_c         (3U + gAppMaxConnections_c)
#else
#define gTmrApplicationTimers_c         (4U)
#endif

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
      no Link layer activity - 300 seconds default*/
#define cPWR_DeepSleepDurationMs        300000

/*! Number of cycles of 16KHz to wait for LL to exit DSM
 * Oscillator stabilization/startup delay is reduced when accuracy is not critical. Applied while advertising only in mode 8
 * This is in X.Y for-mat where X is in terms of number of BT slots (625 us) and Y is in terms of number of clock periods of 16KHz clock input, required for RF oscillator to stabilize the clock output to the controller on its output pin, after os-cillator is turned ON. In this period the clock is assumed to be unstable, and so the controller does not turn on the clock to internal logic till this period is over. This means, the wake up from deep sleep mode must ac-count for this delay before the wakeup instant.
 *      Osc_startup_delay[7:5] is number of slots(625us)
 *      Osc_startup_delay[4:0 is number of clock periods of 16KHz clock
 *   (Warning: Max. value of Osc_startup_delay [4:0] sup-ported is 9. Therefore do not program value greater than 9)
 */
#define cPWR_BLE_LL_OscStartupDelayVllsAdv    (0x8U)

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
    Vbat Monitoring is done in DCDC buck mode only on every wakeup. However, if the device remains active,
       then the battery measurement is done on periodic timer whose Interval value is given by this define     */
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

/*! Disable VLLS2/3 feature if set to 0 - Use LLS3 instead
    cPWR_EnableDeepSleepMode_8 (power gated mode) is enabled by default in lowpower module. If the application requires to stick
      on LLS3 mode (clock gated only , set this flag to 0                          */
//#define cPWR_EnableDeepSleepMode_8      0

/*! On Pin wakeup from lowpower, Bypass the keyboard module and keyboard timer
      and call directly the application call back BleApp_HandleKeys() with parameter
      gKBD_EventPressPB2_c . This saves a subtantial amount of power as the device does not
      remain active during 400-500ms for debouncing
    However, if a differentiation between several wakeup pins is required, or a debounce
      is required on the pin, change this define to 0  */
#define cPWR_BleAppHandleKeyDirectCall_d  1

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

/*! Enable Fast Advertising on any wakeup: lowpower timers, IO wakeup, Serial, etc..
       The device will Advertise on each wakeup in ADV mode (PWR_SetNewAppState() called with PWR_APP_STATE_ADV)
       lowpower mode9 is required for proper functionning
       This mode is disabled in connected state or any other state than ADV
    Benefit :
       The Link layer counter is disabled so expect 0.1uA gain in VLLS2/3 (versus 2uA)
       Wakeup time is reduced by roughly 100us                                                       */
//#define gAppAdvOnWakeUp                   1

/*! Enable RAM OFF lowpower mode during Advertising.
      Allows to reduce significantly the power consumption in lowpower. However, The Application context will be lost.
      After first advertising event, before the device goes to RAM OFF lowpower mode (VLLS1 in buck mode or VLLS0 in bypass mode),
          The SW will save an image of the RAM in Flash and switch to RAM OFF lowpower mode. On each wakeup from lowpower mode,
          or from power on reset, the RAM will be restored and an advertising event will be performed.
      This feature feature uses the mode 9 lowpower mode - gAppAdvOnWakeUp is set when this flag is set. However, PWR_WaitFor32kOscRestart
          is not required in this mode.
      When RAM OFF lowpower mode during Advertising is enabled :
         - you shall define the link script option gRamImageSizeInFlash_c the variable to the RAM image size that will be stored
              in Flash. For baremetal application, 0x4000 (16KB) could be enough as in this example. For freertos, 0x8000 (32KB) is at least
              required
         - you may set gPWR_UsePswitchMode_d to 1 so the device will go to pswitch mode in lowpower mode between ADV events. However,
              an external IO event from external componenet is required to wakeup the device. This allows very low power
              consumption in this mode (less than 60nA)
      For more details and information on limitation when using this feature, please refer to the lowpower reference design user guide document
          included in the package.
      IMPORTANT WARNING : Flash shall be fully erased before reflashing a new firmware
                          Bonding is not supported in this mode, only the pairing            */
//#define gPWR_RamOffDuringAdv_d             1

/*! *********************************************************************************
 *     RTOS Configuration
 ***********************************************************************************/

/* specific Flasg definition when FreeRTOs is used */
#if defined(FSL_RTOS_FREE_RTOS)

/*! Enable systicks for FreeRTOS in lowpower
      only required for FreeRTOS operation such as OSA API OSA_EventWait() with a timeout
      See App_Thread task function in ApplMain.c and mAppTaskWaitTime_c flag  */
//#define PWR_EnableRtosSysticks          1

/*! Remove one task in the system by removing the App Idle task and using the FreeRtos Idle task instead
      This flag is set when PWR_EnableRtosSysticks is enabled */
//#define configUSE_IDLE_HOOK             1

/*! Defines number of OS events used */
#define osNumberOfEvents                5


#if !defined(gDebugConsoleEnable_d) || (gDebugConsoleEnable_d == 0)

/*! Defines total heap size used by the OS */
#define gTotalHeapSize_c                (8500)

/*! Defines Size for Timer Task
       Not used if PWR_EnableRtosSysticks set to 1, use configMINIMAL_STACK_SIZE instead*/
#define gTmrTaskStackSize_c             384

/*! Defines Size for Idle Task  (minimum 572 is required) */
#define gAppIdleTaskStackSize_c         600

/*! Defines Size for Serial Manager Task (use 1500 in DEBUG with PRINTF)*/
#define gSerialTaskStackSize_c          700

/* Defines main task stack size */
//#define gMainThreadStackSize_c          1024

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
 ***********************************************************************************/

/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/

/* Offset to the first instant register. */
#define mcOffsetToFirstInstant_c    0x00      /* 625 usec */

/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */

#if !defined(gAppExtAdvEnable_d) || (gAppExtAdvEnable_d == 0)
#define gLlExtAdvWithLegacyAdv_d    1
#define gLlMaxUsedAdvSet_c          1
#define gLlMaxExtAdvDataLength_c    31
#define gLlUsePeriodicAdvertising_d 0
#else
#define gLlMaxUsedAdvSet_c          2 //Value ranging from 1 to 2
#define gLlMaxExtAdvDataLength_c    255 //Value ranging from 31 to 1650
#define gLlUsePeriodicAdvertising_d 0
#endif

/*! *********************************************************************************
 *     Flag dependencies
 ***********************************************************************************/

#if defined(gAppExtAdvEnable_d) && (gAppExtAdvEnable_d == 1)
/* Allow lp slave to support LePhy 1M, 2M and Coded */
#define gConnDefaultTxPhySettings_c             (gLePhy1MFlag_c | gLePhy2MFlag_c | gLePhyCodedFlag_c)
#define gConnDefaultRxPhySettings_c             (gLePhy1MFlag_c | gLePhy2MFlag_c | gLePhyCodedFlag_c)
#endif

/* Time between the beginning of two consecutive advertising PDU's        */
#if ( gAppExtAdvEnable_d || !gAppConnectableAdv_d )
/* If Extended ADV is set or device is not connectable, and LR feature is not used,
    TX interslot can be reduced to 0.625ms only.
   If both Primary PHY and Secondary PHY are set to CODED (s=8), Tx interslot
    needs to be 1.875ms  */
#define mcAdvertisingPacketInterval_c   0x01      /* 0.625 msec */
#else
/* If payload size is below 3 bytes, set value to 0x02 (See gAppScanRspData)
   If above 3 bytes => set to 0x03 (1.875ms) to allow scan req/scan to happen
   between 2 ADV TX                                                       */
#define mcAdvertisingPacketInterval_c   0x02      /* 1.25 msec */
#endif

#if defined(gAppPowerMeasurement) && (gAppPowerMeasurement == 1)
#undef gDebugConsoleEnable_d
#undef gAppAdvertisingInterval
#undef gAppConnectableAdv_d
//#undef gAppAdvNoPayload_d
#undef gSerialManagerMaxInterfaces_c
#undef APP_DCDC_MODE

#define gDebugConsoleEnable_d           0
#define gAppAdvertisingInterval         0x1F40            /* set 5 seconds ADV interval for improving current averaging during lowpower */
#define gAppConnectableAdv_d            0                 /* Prevent any device from Scan Req / Scan Resp impacting the active current */
//#define gAppAdvNoPayload_d              1                 /* Decrease TX duration / Increase interslot for STOP mode current measurement */
#define gSerialManagerMaxInterfaces_c   0                 /* No need to connect a RX/ TX pins on serial UART for power measurement */
//#define APP_DCDC_MODE                   gDCDC_Bypass_c
#define APP_DCDC_MODE                   gDCDC_Buck_c
#endif

/*! Enable Advertising on wake up from low power
    1 - When ADV interval is higher than 10.24sec
          Because the Link layer advertising interval is limited to 0x4000 (10.24sec),
          if the desired Advertising interval is higher, the LowPower module will use
          Lowpower mode 9, which is the same as mode 8 (VLLS2/3) but advertises at wake up.
    2 - RamOff lowpower mode in ADV                                           */
#if (gAppAdvertisingInterval > 0x4000) || (defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1))
/* RAM Off feature requires ADV on Wakeup to work in ADV */
#undef gAppAdvOnWakeUp
#define gAppAdvOnWakeUp                    1
#endif

/*! Enable lowpower mode9 and disable Wait For 32 KHz Restart when Adv On Wakeup set:
      Required when gAppAdvOnWakeUp is set to 1 in order to fast ADV on wake up.
      gAppAdvOnWakeUp is required for long ADV interval > 10.24 sec              */
#if defined(gAppAdvOnWakeUp) && (gAppAdvOnWakeUp == 1)
#define cPWR_EnableDeepSleepMode_9         1
#else
#define cPWR_EnableDeepSleepMode_9         0
#endif

/*! when RTOS systicks are used , Idle hook and freertos tickless mode shall be enabled for lowpower */
#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
#define configUSE_IDLE_HOOK              1
#define configUSE_TICKLESS_IDLE          1
#endif

#if defined(configUSE_IDLE_HOOK) && (configUSE_IDLE_HOOK == 1)
/* Freertos task stack shall be increased when App Idle task is not used (in ApplMain.c)
      Require 600 bytes  . In words                                          */
#define configMINIMAL_STACK_SIZE         ((unsigned short)150)
#endif

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
