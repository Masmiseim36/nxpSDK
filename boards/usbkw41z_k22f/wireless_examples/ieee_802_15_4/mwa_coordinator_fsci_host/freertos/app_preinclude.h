/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/* 
 * KSDK configuration 
 */
#define CPU_MK22FN512VMP12              1
#define FSL_RTOS_FREE_RTOS              1
#define gTotalHeapSize_c                11000
#define USB_STACK_FREERTOS              1
#define USB_DEVICE_CONFIG_USE_TASK      1


/* 
 * 802.15.4 PHY configuration 
 */
#define gPhyDefaultTxPowerLevel_d       5


/* 
 * 802.15.4 MAC configuration 
 */
#define gMacFeatureSet_d gMacFeatureSet_06M4_d


/* 
 * Connectivity Framework configuration 
 */
#define gFsciIncluded_c                 1
#define gFsciMaxInterfaces_c            1
#define gFsciHostSupport_c              1
#define gFsciHost_802_15_4_c            1
#define gFSCI_IncludeMacCommands_c      1
#define gFSCI_IncludeLpmCommands_c      0
#define gSerialManagerMaxInterfaces_c   2
#define gSerialMgrUseUSB_c              1
/* Select the serial type used for inter-processor communication */
#define gSerialMgrUseUart_c             1
#define gSerialMgrUseSPI_c              0
#define gSerialMgrUseIIC_c              0

#define gKBD_KeysCount_c                1
#define gLEDSupported_d                 0

/* Application Configuration */
#if gSerialMgrUseUart_c
    #define gAppFSCIHostInterfaceBaud_d     gUARTBaudRate115200_c
    #define gAppFSCIHostInterfaceType_d     gSerialMgrUart_c
    #define gAppFSCIHostInterfaceId_d       1
#elif gSerialMgrUseSPI_c
    #define gAppFSCIHostInterfaceBaud_d     gSPI_BaudRate_100000_c
    #define gAppFSCIHostInterfaceType_d     gSerialMgrSPIMaster_c
    #define gAppFSCIHostInterfaceId_d       0
#elif gSerialMgrUseIIC_c
    #define gAppFSCIHostInterfaceBaud_d     gIIC_BaudRate_100000_c
    #define gAppFSCIHostInterfaceType_d     gSerialMgrIICMaster_c
    #define gAppFSCIHostInterfaceId_d       1
#endif

#endif /* __APP_PREINCLUDE_H__ */