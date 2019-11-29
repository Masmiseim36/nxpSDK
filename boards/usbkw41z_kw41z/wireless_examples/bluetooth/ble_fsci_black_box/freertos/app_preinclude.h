/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 * 	HKB Definition
 ********************************************************************************** */
#ifndef gHkb_d
#define gHkb_d      0
#endif

/*! *********************************************************************************
 * 	Board Configuration
 ********************************************************************************** */
 /* Defines the number of available keys for the keyboard module */
#define gKBD_KeysCount_c        1

/* Specifies the number of physical LEDs on the target board */
#define gLEDsOnTargetBoardCnt_c 2

/*! *********************************************************************************
 * 	Framework Configuration
 ********************************************************************************** */
 /* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    0

/* Defines Rx Buffer Size for Serial Manager */
/*! *********************************************************************************
 * 	HKB Definition
 ********************************************************************************** */
#if gHkb_d
#define gSerialMgrRxBufSize_c   256
#else
#define gSerialMgrRxBufSize_c   320
#endif

/* Defines Tx Queue Size for Serial Manager */
#if gHkb_d
#define gSerialMgrTxQueueSize_c 10
#else
#define gSerialMgrTxQueueSize_c 5
#endif

/* Defines Size for Serial Manager Task*/
#if gHkb_d
#define gSerialTaskStackSize_c  1000
#else
#define gSerialTaskStackSize_c  700
#endif

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
#if gHkb_d
#define AppPoolsDetails_c \
         _block_size_   32  _number_of_blocks_    6 _eol_  \
         _block_size_   64  _number_of_blocks_    3 _eol_  \
         _block_size_  128  _number_of_blocks_    7 _eol_  \
         _block_size_  256  _number_of_blocks_    16 _eol_  \
         _block_size_  512 _number_of_blocks_     4 _eol_ \
         _block_size_  2048 _number_of_blocks_    1 _eol_ \
		 _block_size_ 4096 _number_of_blocks_     1 _eol_
#else
#define AppPoolsDetails_c \
         _block_size_   32  _number_of_blocks_    6 _eol_  \
         _block_size_   64  _number_of_blocks_    3 _eol_  \
         _block_size_  128  _number_of_blocks_    3 _eol_  \
         _block_size_  256 _number_of_blocks_     2 _eol_  \
		 _block_size_  512 _number_of_blocks_     4 _eol_  \
		 _block_size_ 1024 _number_of_blocks_     2 _eol_
#endif

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         4

/* Defines number of timers needed by the protocol stack */
#define gTmrStackTimers_c               5

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
                   
/* Enable/Disable FSCI Low Power Commands*/
#define gFSCI_IncludeLpmCommands_c      0        

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           0

/* Enable/Disable BLE Link Layer DSM */
#define cPWR_BLE_LL_Enable              1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              3

/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines the RTOS used */
#define FSL_RTOS_FREE_RTOS      1

/* Defines number of OS events used */
#if gHkb_d
#define osNumberOfEvents        6
#else
#define osNumberOfEvents        5
#endif
/* Defines main task stack size */
#define gMainThreadStackSize_c  1024
           
/* Defines total heap size used by the OS */
#define gTotalHeapSize_c        8000 
           
/*! *********************************************************************************
 * 	BLE Stack Configuration
 ********************************************************************************** */

#define gUseHciTransport_d          0

/* Enable/Disable Dynamic GattDb functionality */
#define gGattDbDynamic_d            1

/*! *********************************************************************************
 * 	NVM Module Configuration - gAppUseNvm_d shall be defined aboved as 1 or 0
 ********************************************************************************** */    
             
#if gAppUseNvm_d
    #define gNvmMemPoolId_c 1
    /* Defines NVM pools by block size and number of blocks. Must be aligned to 4 bytes.*/
    #define NvmPoolsDetails_c \
         _block_size_ 32   _number_of_blocks_    20 _pool_id_(1) _eol_ \
         _block_size_ 60   _number_of_blocks_    10 _pool_id_(1) _eol_ \
         _block_size_ 80   _number_of_blocks_    10 _pool_id_(1) _eol_ \
         _block_size_ 100  _number_of_blocks_    2 _pool_id_(1) _eol_
             
    /* configure NVM module */
    #define  gNvStorageIncluded_d                (1)
    #define  gNvFragmentation_Enabled_d          (1)
    #define  gUnmirroredFeatureSet_d             (1)
    #define  gNvRecordsCopiedBufferSize_c        (128)
#endif

/*! *********************************************************************************
 * 	Memory Pools Configuration
 ********************************************************************************** */ 
             
/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.
 * DO NOT MODIFY THIS DIRECTLY. CONFIGURE AppPoolsDetails_c
 * If gMaxBondedDevices_c increases, adjust NvmPoolsDetails_c
*/
#if gAppUseNvm_d
    #define PoolsDetails_c \
         AppPoolsDetails_c \
         NvmPoolsDetails_c
#else
    #define PoolsDetails_c \
         AppPoolsDetails_c
#endif

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
