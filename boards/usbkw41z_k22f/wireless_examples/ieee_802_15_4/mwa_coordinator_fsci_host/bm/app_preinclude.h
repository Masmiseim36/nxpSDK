/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/* 
 * KSDK configuration 
 */
#define CPU_MK22FN512VMP12              1
#define USB_STACK_BM                    1


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