/*! *********************************************************************************
 * \addtogroup SHELL GAP
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the source file for the GAP Shell module
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
/* Framework / Drivers */
#include "TimersManager.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#include "shell.h"
#include "Panic.h"
#include "MemManager.h"
#include "board.h"

/* BLE Host Stack */
#include "ble_shell.h"
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_db_app_interface.h"
#include "gap_interface.h"
#include "l2ca_interface.h"

#include "shell_gattdb.h"
#include "ApplMain.h"
#include "shell_gap.h"

#include <string.h>
#include <stdlib.h>
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mShellGapCmdsCount_c                20
#define mShellGapMaxScannedDevicesCount_c   5
#define mShellGapMaxDeviceNameLength_c      20
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct gapCmds_tag
{
    char*       name;
    int8_t      (*cmd)(uint8_t argc, char * argv[]); 
}gapCmds_t;

typedef struct gapScannedDevices_tag
{
    bleAddressType_t    addrType;
    bleDeviceAddress_t  aAddress;
    uchar_t             name[10];
}gapScannedDevices_t;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Shell GAP Events Callback */
static void ShellGap_AdvertisingCallback
(
    gapAdvertisingEvent_t* pAdvertisingEvent
);

static void ShellGap_ScanningCallback
(
    gapScanningEvent_t* pScanningEvent
);

static void ShellGap_ConnectionCallback
(
    deviceId_t peerDeviceId,
    gapConnectionEvent_t* pConnectionEvent
);

/* Shell API Functions */
static int8_t ShellGap_DeviceAddress(uint8_t argc, char * argv[]);
static int8_t ShellGap_DeviceName(uint8_t argc, char * argv[]);

static int8_t ShellGap_StartAdvertising(uint8_t argc, char * argv[]);
static int8_t ShellGap_StopAdvertising(uint8_t argc, char * argv[]);
static int8_t ShellGap_SetAdvertisingParameters(uint8_t argc, char * argv[]);
static int8_t ShellGap_ChangeAdvertisingData(uint8_t argc, char * argv[]);

static int8_t ShellGap_StartScanning(uint8_t argc, char * argv[]);
static int8_t ShellGap_StopScanning(uint8_t argc, char * argv[]);
static int8_t ShellGap_SetScanParameters(uint8_t argc, char * argv[]);
static int8_t ShellGap_ChangeScanData(uint8_t argc, char * argv[]);

static int8_t ShellGap_Connect(uint8_t argc, char * argv[]);
static int8_t ShellGap_SetConnectionParameters(uint8_t argc, char * argv[]);
static int8_t ShellGap_Disconnect(uint8_t argc, char * argv[]);
static int8_t ShellGap_UpdateConnection(uint8_t argc, char * argv[]);

static int8_t ShellGap_Pair(uint8_t argc, char * argv[]);
static int8_t ShellGap_PairCfg(uint8_t argc, char * argv[]);
static int8_t ShellGap_EnterPasskey(uint8_t argc, char * argv[]);
static int8_t ShellGap_Bonds(uint8_t argc, char * argv[]);
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
const gapCmds_t mGapShellCmds[mShellGapCmdsCount_c] = 
{
    {"address",     ShellGap_DeviceAddress},
    {"devicename",  ShellGap_DeviceName},
    
    {"advstart",    ShellGap_StartAdvertising},
    {"advstop",     ShellGap_StopAdvertising},
    {"advcfg",      ShellGap_SetAdvertisingParameters},
    {"advdata",     ShellGap_ChangeAdvertisingData},

    {"scanstart",   ShellGap_StartScanning},
    {"scanstop",    ShellGap_StopScanning},
    {"scancfg",     ShellGap_SetScanParameters},
    {"scandata",    ShellGap_ChangeScanData},

    {"connect",     ShellGap_Connect},
    {"connectcfg",  ShellGap_SetConnectionParameters},
    {"disconnect",  ShellGap_Disconnect},
    {"connupdate",  ShellGap_UpdateConnection},

    {"pair",        ShellGap_Pair},
    {"paircfg",     ShellGap_PairCfg},
    {"enterpin",    ShellGap_EnterPasskey},
    {"bonds",       ShellGap_Bonds},
    
};

static bool_t mAdvOn = FALSE;
static bool_t mIsBonded = FALSE;
static bool_t mScanningOn = FALSE;

static bool_t mIsMaster = FALSE;

static gapScannedDevices_t     mScannedDevices[mShellGapMaxScannedDevicesCount_c];
static uint8_t                 mScannedDevicesCount;
static uint8_t                 mConnectToDeviceId = mShellGapMaxScannedDevicesCount_c;

static gapConnectionParameters_t mConnectionParams;

/* Pointer to ADV data */
static uint8_t  *mpAdvData = NULL;
static uint8_t  *mpScanRspData = NULL;
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern gapAdvertisingParameters_t   gAdvParams;
extern gapScanningParameters_t      gAppScanParams;
extern gapPairingParameters_t       gPairingParameters;
extern gapSmpKeys_t                 gSmpKeys;

deviceId_t   gPeerDeviceId = gInvalidDeviceId_c;
static char     mDeviceName[mMaxCharValueLength_d + 1]="";
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
int8_t ShellGap_Command(uint8_t argc, char * argv[])
{
    uint8_t i;
    
    if (argc < 2)
    {
        return CMD_RET_USAGE;
    }
    
    for (i=0; i< mShellGapCmdsCount_c; i++)
    {
        if(!strcmp((char*)argv[1], mGapShellCmds[i].name) )
        {
            return mGapShellCmds[i].cmd(argc-2, (char **)(&argv[2]));
        }
    }
    return CMD_RET_USAGE;
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

static int8_t ShellGap_DeviceAddress(uint8_t argc, char * argv[])
{
    switch(argc)
    {
        case 0:
        {
            Gap_ReadPublicDeviceAddress(); 
            return CMD_RET_ASYNC;
        }
        
        case 1:
        {
            
            return CMD_RET_SUCCESS;
        }
        default:
          return CMD_RET_USAGE;
    }
}

static int8_t ShellGap_DeviceName(uint8_t argc, char * argv[])
{
    uint16_t valueHandle = 0, serviceHandle = 0, length;
    uint8_t result;
    bleUuid_t   uuid;  
    
    
    uuid.uuid16 = gBleSig_GenericAccessProfile_d;
    GattDb_FindServiceHandle(1, gBleUuidType16_c, &uuid, &serviceHandle);
    
    uuid.uuid16 = gBleSig_GapDeviceName_d;
    GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c,
                                        &uuid, &valueHandle);

    if (argc == 0)
    {
        result =  (uint8_t) (0xFF & GattDb_ReadAttribute(valueHandle, mMaxCharValueLength_d, (uint8_t*)mDeviceName, &length)); 
        
        if (result != gGattDbSuccess_c)
        {
            shell_write("\n\r-->  GATTDB Event: Procedure Error ");
            SHELL_NEWLINE();      
        }
        else
        {
            shell_write("\n\r-->  GATTDB Event: Attribute Read ");
            shell_write("\n\r     Value: ");
            shell_write(mDeviceName);   
            SHELL_NEWLINE();
        }
       
        return CMD_RET_SUCCESS;
    }   
    else
    {
        length = strlen(argv[0]);
        
        if (length > mMaxCharValueLength_d)
        {
            shell_write("\n\r-->  Variable length exceeds maximum!");
            return CMD_RET_FAILURE;
        }    

        result =  (uint8_t) (0xFF & GattDb_WriteAttribute(valueHandle, length, (uint8_t*)argv[0])); 

        if (result != gGattDbSuccess_c)
        {
            shell_write("\n\r-->  GATTDB Event: Procedure Error ");
            SHELL_NEWLINE();      
        }
        else
        {
            shell_write("\n\r-->  GATTDB Event: Attribute Written ");
            SHELL_NEWLINE();      
        }    
    }
    
    FLib_MemCpy(mDeviceName, argv[0], length);
    mDeviceName[length] = '>';
    mDeviceName[length+1] = 0;
    
    shell_change_prompt(mDeviceName);
    return CMD_RET_SUCCESS;
}

static int8_t ShellGap_StartAdvertising(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }
    
    mIsMaster = FALSE;    
    App_StartAdvertising(ShellGap_AdvertisingCallback,
            ShellGap_ConnectionCallback);  
    
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_StopAdvertising(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }
    
    Gap_StopAdvertising();
    
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_SetAdvertisingParameters(uint8_t argc, char * argv[])
{
    uint8_t i;
    bool_t bValidCmd = FALSE;
    
    if (argc == 0)
    {
        shell_write("\n\r-->  Advertising Parameters:");
        shell_write("\n\r    -->  Advertising Interval: ");
        shell_writeDec(gAdvParams.maxInterval * 625 / 1000);
        shell_write(" ms");
        shell_write("\n\r    -->  Advertising Type: ");
        
        switch(gAdvParams.advertisingType)
        {
            case gAdvConnectableUndirected_c:
            {
                shell_write("ADV_IND");
            }
            break;
            case gAdvDirectedHighDutyCycle_c:
            {
                shell_write("ADV_IND");
            }
            break;
            case gAdvNonConnectable_c:
            {
                shell_write("ADV_NONCON_IND");
            }
            break;
            case gAdvScannable_c:
            {
                shell_write("ADV_SCAN_IND");
            }
            break;
            case gAdvDirectedLowDutyCycle_c:
            {
                shell_write("ADV_DIRECT");
            }
            break;
            default:
            break;
        }
        SHELL_NEWLINE();
        return CMD_RET_SUCCESS;
    }
    
    for (i=0; i<argc; i+=2)
    {
        if(!strcmp((char*)argv[i], "-interval") && ((i+1) < argc))
        {
            uint16_t interval = atoi(argv[i+1])*1000/625;
            gAdvParams.maxInterval = interval;
            gAdvParams.minInterval = interval;
            bValidCmd = TRUE;
        }

        if(!strcmp((char*)argv[i], "-type") && ((i+1) < argc))
        {
            uint8_t advType = atoi(argv[i+1]);
            gAdvParams.advertisingType = (bleAdvertisingType_t)advType;
            bValidCmd = TRUE;
        }
    }
    
    if (bValidCmd)
    {
        Gap_SetAdvertisingParameters(&gAdvParams);
        return CMD_RET_ASYNC;
    }
    else
    {
        return CMD_RET_USAGE;
    }
}

static void ShellGap_AppendAdvData(gapAdvertisingData_t *pAdvData, gapAdType_t type, char * pData, bool_t fIsScanRsp)
{
    uint8_t i, advIdx = 0;
    uint8_t advCursor = 0; 
    uint8_t length;
    int32_t number;
    uint8_t *pAdvArrayData;
    
    pAdvArrayData = (fIsScanRsp) ? mpScanRspData : mpAdvData;
        
    if (pAdvArrayData == NULL)
    {
        /* No existing ADV data. Allocate memory */        
        pAdvArrayData = MEM_BufferAlloc(gcGapMaxAdvertisingDataLength_c);
        
        (fIsScanRsp)?(mpScanRspData = pAdvArrayData):(mpAdvData = pAdvArrayData);
        
    }
      
    for (advIdx=0; advIdx<pAdvData->cNumAdStructures; advIdx++)
    {
        if (pAdvData->aAdStructures[advIdx].adType == type)
        {
            return;
        }
        
        advCursor += pAdvData->aAdStructures[advIdx].length-1;
    }
    
    length = strlen(pData);
    
    /* Copy ADV payload*/
    pAdvData->aAdStructures[advIdx].aData = pAdvArrayData + advCursor;
    
    switch(type)
    {
        case gAdShortenedLocalName_c:
        case gAdCompleteLocalName_c:
        {   
            /* Check if we got space. Take into account length and type octets */
            if ((advCursor + length + 2 * (pAdvData->cNumAdStructures)) > gcGapMaxAdvertisingDataLength_c )
            {
                return;
            }
            
            FLib_MemCpy(pAdvArrayData + advCursor, pData, length);
        }
        break;
        
        default:
        {
            /* Halfen length because we print */
            length = (length % 2) ? (length/2) + 1 : (length/2);
            
            /* Check if we got space. Take into account length and type octets */
            if ((advCursor + length + 2 * (pAdvData->cNumAdStructures)) > gcGapMaxAdvertisingDataLength_c )
            {
                return;
            }
            
            for (i=0; i<strlen(pData); i+=4)
            {
                 number = strtoul(pData + i, NULL, 16);
                 FLib_MemCpy(pAdvArrayData + advCursor + i, &number, sizeof(uint32_t));
            }
        }
        break;
    }
    
    pAdvData->aAdStructures[advIdx].adType = type;
    pAdvData->aAdStructures[advIdx].length = length + 1;
    
    pAdvData->cNumAdStructures += 1;
}

static int8_t ShellGap_ChangeAdvertisingData(uint8_t argc, char * argv[])
{
    switch(argc)
    {
        case 0:
        {
            shell_write("\n\r\n\r-->  Advertising Data:");
            return CMD_RET_SUCCESS;
        }
        break;
        
        case 1:
        {
            if(!strcmp((char*)argv[0], "-clear"))
            {
                gAppAdvertisingData.cNumAdStructures = 0;
                if (mpAdvData)
                {
                    MEM_BufferFree(mpAdvData);
                }
                shell_write("\n\r\n\r-->  Advertising Data Erased.");
                return CMD_RET_SUCCESS;
            }            
        }
        break;
        
        default:
        {
            gapAdType_t advType = (gapAdType_t)atoi(argv[0]);
            ShellGap_AppendAdvData(&gAppAdvertisingData, advType, argv[1], FALSE);
            Gap_SetAdvertisingData(&gAppAdvertisingData, NULL);
            return CMD_RET_ASYNC;
        }
        break;      
    }
    
    return CMD_RET_USAGE;
}

static int8_t ShellGap_StartScanning(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }
    
    mIsMaster = TRUE;    
    Gap_StartScanning(&gAppScanParams, ShellGap_ScanningCallback, TRUE);
    
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_StopScanning(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }
    
    Gap_StopScanning();
    
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_SetScanParameters(uint8_t argc, char * argv[])
{
    uint8_t i;
    bool_t bValidCmd;
    
    if (argc == 0)
    {
        shell_write("\n\r-->  Scan Parameters:");
        shell_write("\n\r    -->  Scan Interval: ");
        shell_writeDec(gAppScanParams.interval * 625 / 1000);
        shell_write(" ms");
        shell_write("\n\r    -->  Scan Window: ");
        shell_writeDec(gAppScanParams.window * 625 / 1000);
        shell_write(" ms");
        shell_write("\n\r    -->  Scan Type: ");
        (gAppScanParams.type) ? shell_write("ACTIVE") : shell_write("PASSIVE");
        SHELL_NEWLINE();
        return CMD_RET_SUCCESS;
    }
    
    for (i=0; i<argc; i+=2)
    {
        if(!strcmp((char*)argv[i], "-interval") && ((i+1) < argc))
        {
            uint16_t interval = atoi(argv[i+1])*1000/625;
            gAppScanParams.interval = interval;
            gConnReqParams.scanInterval = interval;
            bValidCmd = TRUE;
        }

        if(!strcmp((char*)argv[i], "-window") && ((i+1) < argc))
        {
            uint16_t window = atoi(argv[i+1])*1000/625;
            gAppScanParams.window = window;
            gConnReqParams.scanWindow = window;
            bValidCmd = TRUE;
        }

        if(!strcmp((char*)argv[i], "-type") && ((i+1) < argc))
        {
            uint8_t scanType = atoi(argv[i+1]);
            gAppScanParams.type = (bleScanType_t)scanType;
            bValidCmd = TRUE;
        }
    }    
    
    if (bValidCmd)
    {   
        return CMD_RET_SUCCESS;
    }
    else
    {
        return CMD_RET_USAGE;
    }
}

static int8_t ShellGap_ChangeScanData(uint8_t argc, char * argv[])
{
    switch(argc)
    {
        case 0:
        {
            shell_write("\n\r\n\r-->  Scan Response Data:");
            return CMD_RET_SUCCESS;
        }
        break;
        
        case 1:
        {
            if(!strcmp((char*)argv[0], "-clear"))
            {
                gAppScanRspData.cNumAdStructures = 0;
                if (mpAdvData)
                {
                    MEM_BufferFree(mpAdvData);
                }
                shell_write("\n\r\n\r-->  Scan Response Erased.");
                return CMD_RET_SUCCESS;
            }            
        }
        break;
        
        default:
        {
            gapAdType_t advType = (gapAdType_t)atoi(argv[0]);
            ShellGap_AppendAdvData(&gAppScanRspData, advType, argv[1], TRUE);
            Gap_SetAdvertisingData(NULL, &gAppScanRspData);
            return CMD_RET_ASYNC;
        }
        break;      
    }
    
    return CMD_RET_USAGE;
}

static int8_t ShellGap_Connect(uint8_t argc, char * argv[])
{
    uint8_t deviceId;
    
    if (argc != 1)
    {
        return CMD_RET_USAGE;
    }
    
    deviceId = (uint8_t)atoi(argv[0]);
    
    if (deviceId > mScannedDevicesCount)
    {
        return CMD_RET_USAGE;
    }
    
    mConnectToDeviceId =  deviceId;
    mIsMaster = TRUE;
    
    gConnReqParams.scanInterval = gAppScanParams.interval;
    gConnReqParams.scanWindow = gAppScanParams.window;
    gConnReqParams.peerAddressType = mScannedDevices[deviceId].addrType;
    FLib_MemCpy(gConnReqParams.peerAddress, 
                mScannedDevices[deviceId].aAddress, 
                sizeof(bleDeviceAddress_t));
    
    /* Stop scanning if it's on*/
    if (mScanningOn)
    {
        Gap_StopScanning();
    }
    
    App_Connect(&gConnReqParams, ShellGap_ConnectionCallback);
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_SetConnectionParameters(uint8_t argc, char * argv[])
{
    uint8_t i;
    bool_t bValidCmd = FALSE;
    
    if (argc == 0)
    {
        bValidCmd = TRUE;
    }
    
    for (i=0; i<argc; i+=2)
    {
        if(!strcmp((char*)argv[i], "-interval") && ((i+1) < argc))
        {
            uint16_t interval = atoi(argv[i+1])*8/10;
            gConnReqParams.connIntervalMin = interval;
            gConnReqParams.connIntervalMax = interval;
            bValidCmd = TRUE;
        }

        if(!strcmp((char*)argv[i], "-latency") && ((i+1) < argc))
        {
            uint16_t latency = atoi(argv[i+1]);
            gConnReqParams.connLatency = latency;
            bValidCmd = TRUE;
        }

        if(!strcmp((char*)argv[i], "-timeout") && ((i+1) < argc))
        {
            uint16_t timeout = atoi(argv[i+1])/10;
            gConnReqParams.supervisionTimeout = timeout;
            bValidCmd = TRUE;
        }
    }    
    
    if (bValidCmd)
    {   
        shell_write("\n\r-->  Connection Parameters:");
        shell_write("\n\r    -->  Connection Interval: ");
        shell_writeDec(gConnReqParams.connIntervalMax * 10 / 8);
        shell_write(" ms");
        
        shell_write("\n\r    -->  Connection Latency: ");
        shell_writeDec(gConnReqParams.connLatency);

        shell_write("\n\r    -->  Supervision Timeout: ");
        shell_writeDec(gConnReqParams.supervisionTimeout * 10);
        shell_write(" ms\n\r");

        SHELL_NEWLINE();
        return CMD_RET_SUCCESS;
    }
    else
    {
        return CMD_RET_USAGE;
    }
}

static int8_t ShellGap_Disconnect(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }

    Gap_Disconnect(gPeerDeviceId);
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_UpdateConnection(uint8_t argc, char * argv[])
{
    uint16_t minInterval, maxInterval, latency, timeout;
    
    if (argc != 4)
    {
        return CMD_RET_USAGE;
    }
            
    minInterval = atoi(argv[0])*8/10;
    maxInterval = atoi(argv[1])*8/10;
    latency = atoi(argv[2]);
    timeout = atoi(argv[3])/10;

    Gap_UpdateConnectionParameters(gPeerDeviceId, minInterval, maxInterval, latency, timeout,
                                    gGapConnEventLengthMin_d, gGapConnEventLengthMax_d);
    
    return CMD_RET_SUCCESS;
}

static int8_t ShellGap_Pair(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }
        
    if (gPeerDeviceId == gInvalidDeviceId_c)
    {
        shell_write("\n\r-->  Please connect the node first...");
        return CMD_RET_FAILURE;
    }    
    
    shell_write("\n\r-->  Pairing...\n\r");

    if (mIsMaster)
    {
        Gap_Pair(gPeerDeviceId, &gPairingParameters);
    }
    else
    {
    
        Gap_SendSlaveSecurityRequest(gPeerDeviceId, 
                                 gPairingParameters.withBonding,
                                 gPairingParameters.securityModeAndLevel);
    }
    
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_PairCfg(uint8_t argc, char * argv[])
{
    uint8_t i;
    bool_t bValidCmd = FALSE;
    
    if (argc == 0)
    {
        bValidCmd = TRUE;
    }
    
    for (i=0; i<argc; i+=2)
    {
        if(!strcmp((char*)argv[i], "-usebonding") && ((i+1) < argc))
        {
            uint8_t usebonding = atoi(argv[i+1]);
            gPairingParameters.withBonding = usebonding?TRUE:FALSE;
            bValidCmd = TRUE;
        }

        if(!strcmp((char*)argv[i], "-seclevel") && ((i+1) < argc))
        {
            uint8_t level = strtoul(argv[i+1], NULL, 16);
            if ((level & 0x0F) <= gSecurityLevel_WithMitmProtection_c &&
                (level & 0xF0) <= gSecurityMode_2_c)
            {
                gPairingParameters.securityModeAndLevel = (gapSecurityModeAndLevel_t)level;
                bValidCmd = TRUE;
            }
        }

        if(!strcmp((char*)argv[i], "-keyflags") && ((i+1) < argc))
        {
            uint8_t flags = strtoul(argv[i+1], NULL, 16);
            gPairingParameters.centralKeys = (gapSmpKeyFlags_t)(flags & 0x07);
            gPairingParameters.peripheralKeys = (gapSmpKeyFlags_t)(flags & 0x07);
            bValidCmd = TRUE;
        }
    }    
    
    if (bValidCmd)
    {   
        shell_write("\n\r-->  Pairing Configuration:");
        shell_write("\n\r    -->  Use Bonding: ");
        shell_writeBool(gPairingParameters.withBonding);
        
        shell_write("\n\r    -->  SecurityLevel: ");
        shell_writeHex((uint8_t*)&gPairingParameters.securityModeAndLevel, 1);

        shell_write("\n\r    -->  Flags: ");
        shell_writeHex((uint8_t*)&gPairingParameters.centralKeys, 1);

        SHELL_NEWLINE();
        return CMD_RET_SUCCESS;
    }
    else
    {
        return CMD_RET_USAGE;
    }
}

static int8_t ShellGap_EnterPasskey(uint8_t argc, char * argv[])
{   
    uint32_t passKey;
      
    if (argc != 1)
    {
        return CMD_RET_USAGE;
    }

    passKey = atoi(argv[0]);
    
    Gap_EnterPasskey(gPeerDeviceId, passKey);
    return CMD_RET_ASYNC;
}

static int8_t ShellGap_Bonds(uint8_t argc, char * argv[])
{
    uint8_t i = 0, count = 0;

    switch(argc)
    {
        case 0:
        {
            /* Get number of bonded devices */
            Gap_GetBondedDevicesCount(&count);
            
            if (!count)
            {
                shell_write("\n\r-->  No bonds found on the device! \n\r");
                return CMD_RET_SUCCESS;
            }
            else
            {
                void* pBuffer = NULL;
                
                /* Allocate buffer for name */
                pBuffer = MEM_BufferAlloc(mShellGapMaxDeviceNameLength_c);
                
                if (!pBuffer)
                {
                    return CMD_RET_FAILURE;
                }
                
                for (i=0; i<count;i++)
                {
                    shell_write("\n\r-->  ");                    
                    shell_writeDec(i);
                    shell_write(". ");
                    Gap_GetBondedDeviceName(i, pBuffer, mShellGapMaxDeviceNameLength_c);
                    
                    if (strlen(pBuffer) > 0)
                    {
                        shell_write(pBuffer);
                    }
                    else
                    {
                        shell_write("[No saved name]");
                    }
                }
                SHELL_NEWLINE();
                return CMD_RET_SUCCESS;
            }
        }
        break;
        
        case 1:
        {
            if(!strcmp((char*)argv[0], "-erase"))
            {              
                if (gPeerDeviceId != gInvalidDeviceId_c)
                {
                    shell_write("\n\r-->  Please disconnect the node first...");
                    return CMD_RET_FAILURE;
                }
                
                if (Gap_RemoveAllBonds() == gBleSuccess_c)
                {
                    shell_write("\n\r-->  Bonds removed!\n\r");
                    return CMD_RET_SUCCESS;
                }
                else
                {
                    shell_write("\n\r-->  Operation failed!\n\r");
                    return CMD_RET_FAILURE;
                }
            }            
        }
        break;
        
        case 2:
        {
            if(!strcmp((char*)argv[0], "-remove"))
            {
                uint8_t index = atoi(argv[1]);
                
                if (gPeerDeviceId != gInvalidDeviceId_c)
                {
                    shell_write("\n\r-->  Please disconnect the node first...\n\r");
                    return CMD_RET_FAILURE;
                }
                
                /* Get number of bonded devices */
                Gap_GetBondedDevicesCount(&count);
                
                if ((count > 0) && (index < count) &&
                    (Gap_RemoveBond(index) == gBleSuccess_c))
                {
                    shell_write("\n\r-->  Bond removed!\n\r");
                    return CMD_RET_SUCCESS;                  
                }
                else
                {
                    shell_write("\n\r-->  Operation failed!\n\r");
                    return CMD_RET_FAILURE;               
                }
            }            
        }
        break;
        
        default:
            break;      
    }
    
    return CMD_RET_USAGE;
}

static void ShellGap_ParseScannedDevice(gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    uint8_t nameLength;
    
    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;
        
        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1];
        adElement.aData = &pData->data[index + 2];
        
        if ((adElement.adType == gAdShortenedLocalName_c) ||
          (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length-1, mShellGapMaxDeviceNameLength_c);
            FLib_MemCpy(mScannedDevices[mScannedDevicesCount].name, adElement.aData, nameLength);
            
            shell_write((char*)mScannedDevices[mScannedDevicesCount].name);
            shell_write(" "); 
        }
        
        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }

    shell_writeHexLe(pData->aAddress, sizeof(bleDeviceAddress_t));
    shell_write(" ");   
    
    /* Temporary store scanned data to use for connection */
    mScannedDevices[mScannedDevicesCount].addrType = pData->addressType;
    FLib_MemCpy(mScannedDevices[mScannedDevicesCount].aAddress, 
                pData->aAddress, 
                sizeof(bleDeviceAddress_t));
    
    shell_writeSignedDec(pData->rssi);
    shell_write(" dBm");

    mScannedDevicesCount++;
}

/*! *********************************************************************************
 * \brief        Handles BLE generic callback.
 *
 * \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
 ********************************************************************************** */
void ShellGap_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    shell_write("\n\r-->  GAP Event: ");
    
    switch(pGenericEvent->eventType)
    {
        case gPublicAddressRead_c:
        {
            shell_write("Public Address Read:");
            shell_writeHex(pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
            break;
        }

        case gAdvertisingDataSetupComplete_c:
        {
            shell_write("Advertising data successfully set.");
            break;
        }
        
        case gAdvertisingParametersSetupComplete_c:
        {
            shell_write("Advertising parameters successfully set.");
            break;
        }

        case gAdvertisingSetupFailed_c:
        {
            shell_write("Advertising setup failed.");
            break;
        }

        case gInternalError_c:
        {
          
        }
        break;

        default:
            break;
    }
    SHELL_NEWLINE();
    shell_cmd_finished();
}

/*! *********************************************************************************
 * \brief        Handles BLE Advertising callback from host stack.
 *
 * \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
 ********************************************************************************** */
static void ShellGap_AdvertisingCallback
(
    gapAdvertisingEvent_t* pAdvertisingEvent
)
{
    shell_write("\n\r-->  GAP Event: Advertising ");
                
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
        {
            mAdvOn = !mAdvOn;
    
            if (mAdvOn)
            {
                shell_write("started.\n\r");
            }
            else
            {
                shell_write("stopped.\n\r");
            }
            break;
        }

        case gAdvertisingCommandFailed_c:
        {
            shell_write("state could not be changed.\n\r");
            break;
        }

    default:
        break;
    }
    
    shell_cmd_finished();
}

/*! *********************************************************************************
 * \brief        Handles BLE Connection callback from host stack.
 *
 * \param[in]    peerDeviceId        Peer device ID.
 * \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
 ********************************************************************************** */
static void ShellGap_ConnectionCallback
(
    deviceId_t peerDeviceId,
    gapConnectionEvent_t* pConnectionEvent
)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            gPeerDeviceId = peerDeviceId;
            shell_write("\n\r-->  GAP Event: Connected\n\r");
            shell_cmd_finished();  
            
            mAdvOn = FALSE;
            mIsBonded = FALSE;
            
            Gap_CheckIfBonded(peerDeviceId, &mIsBonded);
            
            if ((mIsBonded) &&
            (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId, NULL, 0 ,0)))
            {              
                /* Restored custom connection information. Encrypt link */
                Gap_EncryptLink(peerDeviceId);
            }
            
            /* Save connection parameters */
            FLib_MemCpy(&mConnectionParams,
                        &pConnectionEvent->eventData.connectedEvent.connParameters,
                        sizeof(gapConnectionParameters_t));
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            gPeerDeviceId = gInvalidDeviceId_c;
            shell_write("\n\r-->  GAP Event: Disconnected\n\r");
            shell_cmd_finished();
        }
        break;    
        
        case gConnEvtPairingRequest_c:
        {
            Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
        }
        break;

        case gConnEvtKeyExchangeRequest_c:
        {
            gapSmpKeys_t sentSmpKeys = gSmpKeys;
            
            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gLtk_c))
            {
                sentSmpKeys.aLtk = NULL;
                /* When the LTK is NULL EDIV and Rand are not sent and will be ignored. */
            }
            
            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gIrk_c))
            {
                sentSmpKeys.aIrk = NULL;
                /* When the IRK is NULL the Address and Address Type are not sent and will be ignored. */
            }
            
            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gCsrk_c))
            {
                sentSmpKeys.aCsrk = NULL;
            }
            
            Gap_SendSmpKeys(peerDeviceId, &sentSmpKeys);
        }
        break;
        
        case gConnEvtPasskeyRequest_c:
        {
            shell_write("\n\r\n\r-->  GAP Event: PIN required\n\r");
            shell_cmd_finished();
        }
        break;
        
        case gConnEvtPasskeyDisplay_c:
        {
            shell_write("\n\r-->  GAP Event: Passkey is ");
            shell_writeDec(pConnectionEvent->eventData.passkeyForDisplay);
            SHELL_NEWLINE();
            shell_cmd_finished();
        }
        break;
        
        case gConnEvtLongTermKeyRequest_c:
        {
            if (pConnectionEvent->eventData.longTermKeyRequestEvent.ediv == gSmpKeys.ediv &&
                pConnectionEvent->eventData.longTermKeyRequestEvent.randSize == gSmpKeys.cRandSize)
            {
                Gap_LoadEncryptionInformation(peerDeviceId, gSmpKeys.aLtk, &gSmpKeys.cLtkSize);
                /* EDIV and RAND both matched */
                Gap_ProvideLongTermKey(peerDeviceId, gSmpKeys.aLtk, gSmpKeys.cLtkSize);
            }
            else
            /* EDIV or RAND size did not match */
            {
                Gap_DenyLongTermKey(peerDeviceId);
            }
            break;
        }
       
        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                shell_write("\n\r-->  GAP Event: Device Paired.\n\r");
                
                /* Save Device Name in NVM*/
                Gap_SaveDeviceName(peerDeviceId, mScannedDevices[mConnectToDeviceId].name, strlen((char*)mScannedDevices[mConnectToDeviceId].name));
            }
            else
            {
                shell_write("\n\r-->  GAP Event: Pairing Unsuccessful.\n\r");
            }
            shell_cmd_finished();
        }
        break;
        
        case gConnEvtLeScDisplayNumericValue_c:
        {
            (void) pConnectionEvent->eventData.numericValueForDisplay;
            /* Display on a screen for user confirmation then validate/invalidate based on value. */
            Gap_LeScValidateNumericValue(peerDeviceId, TRUE);
        }
        break;
        
        case gConnEvtEncryptionChanged_c:
        {
            if (pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState)
            {
                shell_write("\n\r-->  GAP Event: Link Encrypted.\n\r");
            }
            else
            {
                shell_write("\n\r-->  GAP Event: Link Not Encrypted.\n\r");
            }
            shell_cmd_finished();
        }
        break;
        
        case gConnEvtParameterUpdateRequest_c:
        {
            Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
        }   
        break;
        
        case gConnEvtParameterUpdateComplete_c:
        {
            if (pConnectionEvent->eventData.connectionUpdateComplete.status == gBleSuccess_c)
            {
                mConnectionParams.connInterval = pConnectionEvent->eventData.connectionUpdateComplete.connInterval;
                mConnectionParams.connLatency = pConnectionEvent->eventData.connectionUpdateComplete.connLatency;
                mConnectionParams.supervisionTimeout = pConnectionEvent->eventData.connectionUpdateComplete.supervisionTimeout;
                
                shell_write("\n\r-->  GAP Event: Connection Parameters Changed.");
                shell_write("\n\r   -->Connection Interval: ");
                shell_writeDec(mConnectionParams.connInterval * 10/8);
                shell_write(" ms");
                shell_write("\n\r   -->Connection Latency: ");
                shell_writeDec(mConnectionParams.connLatency);
                shell_write("\n\r   -->Supervision Timeout: ");
                shell_writeDec(mConnectionParams.supervisionTimeout * 10);
                shell_write(" ms");                
                shell_cmd_finished();
            }
            else
            {
                shell_write("\n\r-->  GAP Event: Connection Parameters Did Not Change.");
            }
        }
        break;
        
        default:
        break;
    }
}

/*! *********************************************************************************
 * \brief        Handles BLE Scanning callback from host stack.
 *
 * \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
 ********************************************************************************** */
static void ShellGap_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    switch (pScanningEvent->eventType)
    {
        case gDeviceScanned_c:
        {
            if (mScannedDevicesCount >= mShellGapMaxScannedDevicesCount_c)
            {
                break;
            }
            
            shell_write("\n\r-->  GAP Event: Found device ");
            shell_writeDec(mScannedDevicesCount);
            shell_write(" : ");            
            ShellGap_ParseScannedDevice(&pScanningEvent->eventData.scannedDevice);              
            break;
        }

        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            
            shell_write("\n\r->  GAP Event: Scan ");
            if (mScanningOn)
            {
                mScannedDevicesCount = 0; 
                shell_write("started.\n\r");
            }
            else
            {
                shell_write("stopped.\n\r");
            }
            shell_cmd_finished();
            break;
        }

    case gScanCommandFailed_c:
    {
        shell_write("\n\r-->  GAP Event: Scan state could not be changed.");
        shell_cmd_finished();
        break;
    }
    default:
        break;
    }
}

void ShellGap_L2caControlCallback
(
    l2capControlMessageType_t  messageType,
    void* pMessage
)
{
    switch (messageType)
    {

        
    default:
      break;
    }
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
