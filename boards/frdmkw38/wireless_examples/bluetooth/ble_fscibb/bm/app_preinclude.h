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

/*!
 *  Application specific configuration file only
 *  Board Specific Configuration shall be added to board.h file directly such as :
 *  - Number of button on the board,
 *  - Number of LEDs,
 *  - etc...
 */
/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
 /* Defines the number of required keys for the keyboard module */
#define gKBD_KeysCount_c        2

/* Specifies the number of required LEDs for the LED module */
#define gLEDsOnTargetBoardCnt_c 4

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */
/*! Maximum number of connections supported for this application */
#define gAppMaxConnections_c    8

/*! Repeated Attempts - Mitigation for pairing attacks */
#define gRepeatedAttempts_d             0
/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
 /* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    1

/* Defines Rx Buffer Size for Serial Manager */
/*! *********************************************************************************
 *     HKB Definition
 ********************************************************************************** */
#define gSerialMgrRxBufSize_c   320

/* Defines Tx Queue Size for Serial Manager */
#define gSerialMgrTxQueueSize_c 5

/* Defines Size for Serial Manager Task*/
#define gSerialTaskStackSize_c  700

/* Enable/Disable UART ussage */
#define gSerialMgrUseUart_c     1

/* Enable/Disable SPI ussage */
#define gSerialMgrUseSPI_c      0

/* Enable/Disable IIC ussage */
#define gSerialMgrUseIIC_c      0

#if gSerialMgrUseUart_c
     #define APP_SERIAL_INTERFACE_SPEED         gUARTBaudRate115200_c
     #define APP_SERIAL_INTERFACE_TYPE          gSerialMgrLpuart_c
     #define APP_SERIAL_INTERFACE_INSTANCE              0
#elif gSerialMgrUseSPI_c
    #define APP_SERIAL_INTERFACE_SPEED          gSPI_BaudRate_1000000_c
    #define APP_SERIAL_INTERFACE_TYPE           gSerialMgrSPISlave_c
    #define APP_SERIAL_INTERFACE_INSTANCE               1
#elif gSerialMgrUseIIC_c
    #define APP_SERIAL_INTERFACE_SPEED          gIIC_BaudRate_100000_c
    #define APP_SERIAL_INTERFACE_TYPE           gSerialMgrIICSlave_c
    #define APP_SERIAL_INTERFACE_INSTANCE               1
#endif

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_   32  _number_of_blocks_   30 _eol_  \
         _block_size_   80  _number_of_blocks_   20 _eol_  \
         _block_size_  280  _number_of_blocks_   20 _eol_  \
         _block_size_  312  _number_of_blocks_    2 _eol_  \
         _block_size_  392  _number_of_blocks_    2 _eol_  \
         _block_size_ 1024  _number_of_blocks_    2 _eol_

/* Defines number of timers needed by the application */
#if gRepeatedAttempts_d
#define gTmrApplicationTimers_c         5
#else
#define gTmrApplicationTimers_c         4
#endif

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

/* Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            0

/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers       0

/* Enable/Disable FSCI */
#define gFsciIncluded_c                 1

/* Defines FSCI length - set this to FALSE is FSCI length has 1 byte */
#define gFsciLenHas2Bytes_c             1

/* Defines FSCI maximum payload length */
#define gFsciMaxPayloadLen_c            600

/* Enable/Disable Ack transmission */
#define gFsciTxAck_c                    0

/* Enable/Disable Ack reception */
#define gFsciRxAck_c                    0

/* Enable FSCI Rx restart with timeout */
#define gFsciRxTimeout_c                1
#define mFsciRxTimeoutUsePolling_c      1

/* Enable/Disable FSCI Low Power Commands*/
#define gFSCI_IncludeLpmCommands_c      1

/* Use Misra Compliant version of FSCI module */
#define gFsciUseDedicatedTask_c         1

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           1

/* Enable/Disable BLE Link Layer DSM */
#define cPWR_BLE_LL_Enable              1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              3

/* Enable/Disable MCU Sleep During BLE Events */
#define cMCU_SleepDuringBleEvents       0

/*! Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1

/*! Default DCDC Mode used by the application gDCDC_Buck_c or gDCDC_Bypass_c */
#define APP_DCDC_MODE                   gDCDC_Buck_c

/* Disable unused LowPower modes */
#define cPWR_EnableDeepSleepMode_1      1
#define cPWR_EnableDeepSleepMode_2      0
#define cPWR_EnableDeepSleepMode_3      1
#define cPWR_EnableDeepSleepMode_4      0
#define cPWR_EnableDeepSleepMode_5      0
#define cPWR_EnableDeepSleepMode_7      0
#define cPWR_EnableDeepSleepMode_8      0

/*! Enable the SWD pins to be managed into low-power */
#define gBoard_ManageSwdPinsInLowPower_d    1

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        6

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */

#define gUseHciTransport_d          0

/* Enable/Disable Dynamic GattDb functionality */
#define gGattDbDynamic_d            1

#define gFsciBleBBox_d              1

/* Enable BLE 5.0 */
#define gBLE50_d                    1

#define gGapSimultaneousEAChainedReports_c     2
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
