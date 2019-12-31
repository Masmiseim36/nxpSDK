/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 *  Board Configuration
 ********************************************************************************** */
 /* Defines the number of available keys for the keyboard module */
#define gKBD_KeysCount_c        2

/* Specifies the number of physical LEDs on the target board */
#define gLEDsOnTargetBoardCnt_c 4

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1

/*! *********************************************************************************
 *  App Configuration
 ********************************************************************************** */

/*! Number of connections supported by the application */
#define gAppMaxConnections_c    8

/*! Number of credit-based channels supported */
#define gL2caMaxLeCbChannels_c         2

/*! *********************************************************************************
 *  Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    0

/* Defines Rx Buffer Size for Serial Manager */
/*! *********************************************************************************
 *  HKB Definition
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
    #define APP_SERIAL_INTERFACE_INSTANCE               0
#elif gSerialMgrUseIIC_c
    #define APP_SERIAL_INTERFACE_SPEED          gIIC_BaudRate_100000_c
    #define APP_SERIAL_INTERFACE_TYPE           gSerialMgrIICSlave_c
    #define APP_SERIAL_INTERFACE_INSTANCE               1
#endif

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_   32  _number_of_blocks_    6 _eol_  \
         _block_size_   80  _number_of_blocks_    8 _eol_  \
         _block_size_  280  _number_of_blocks_   20 _eol_  \
         _block_size_  312  _number_of_blocks_    2 _eol_  \
         _block_size_  392  _number_of_blocks_    2 _eol_  \
         _block_size_ 1024  _number_of_blocks_    1 _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         4

/* Defines number of timers needed by the protocol stack */
#if defined(gAppMaxConnections_c) && defined(gL2caMaxLeCbChannels_c)
    #define gTmrStackTimers_c (3 + (gAppMaxConnections_c * 2) + gL2caMaxLeCbChannels_c)
#else
    #define gTmrStackTimers_c (7)
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

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           1

/* Enable/Disable BLE Link Layer DSM */
#define cPWR_BLE_LL_Enable              1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              3

/* Enable/Disable MCU Sleep During BLE Events */
#define cMCU_SleepDuringBleEvents       1 

/*! *********************************************************************************
 *  RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        5

/*! *********************************************************************************
 *  BLE Stack Configuration
 ********************************************************************************** */

#define gUseHciTransport_d          0

/* Enable/Disable Dynamic GattDb functionality */
#define gGattDbDynamic_d            1

#define gFsciBleBBox_d              1

/*! *********************************************************************************
 *  NVM Module Configuration - gAppUseNvm_d shall be defined above as 1 or 0
 ********************************************************************************** */

#if gAppUseNvm_d
    /* configure NVM module */
    #define  gNvStorageIncluded_d                (1)
    #define  gNvFragmentation_Enabled_d          (1)
    #define  gNvUseFlexNVM_d                     (0)
    #if gNvUseFlexNVM_d
        #define  gUnmirroredFeatureSet_d         (0)
    #else
        #define  gUnmirroredFeatureSet_d         (1)
    #endif
    #define  gNvRecordsCopiedBufferSize_c        (128)
#endif

/*! *********************************************************************************
 *  Memory Pools Configuration
 ********************************************************************************** */

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.
 * DO NOT MODIFY THIS DIRECTLY. CONFIGURE AppPoolsDetails_c
 * If gMaxBondedDevices_c increases, adjust memory pools
*/
#define PoolsDetails_c \
     AppPoolsDetails_c


#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
