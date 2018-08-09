/*! *********************************************************************************
 * \addtogroup SHELL GATTDB
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the source file for the GATTDB Shell module
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
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_dynamic.h"
#include "gatt_database_dynamic.h"
#include "gap_interface.h"

#include "shell_gatt.h"
#include "shell_gattdb.h"
#include "ApplMain.h"

#include <stdlib.h>
#include <string.h>
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mShellGattDbCmdsCount_c             4

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct gattCmds_tag
{
    char*       name;
    int8_t      (*cmd)(uint8_t argc, char * argv[]); 
}gattCmds_t;


/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
/* Shell API Functions */
static int8_t   ShellGattDb_Read(uint8_t argc, char * argv[]);
static int8_t   ShellGattDb_Write(uint8_t argc, char * argv[]);
static int8_t   ShellGattDb_AddService(uint8_t argc, char * argv[]);
static int8_t   ShellGattDb_Erase(uint8_t argc, char * argv[]);

/* Local helper functions*/
static uint8_t  ShellGattDb_ParseHexValue(char* pInput);
static bleResult_t ShellGattDb_AddServiceInDatabase(serviceInfo_t* pServiceInfo);
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
const gattCmds_t mGattDbShellCmds[mShellGattDbCmdsCount_c] = 
{
    {"read",        ShellGattDb_Read},
    {"write",       ShellGattDb_Write},
    {"addservice",  ShellGattDb_AddService},
    {"erase",       ShellGattDb_Erase}
};

const char* mGattDbStatus[] = {
    "NoError",
    "InvalidHandle",
    "CharacteristicNotFound",
    "CccdNotFound",
    "ServiceNotFound",
    "DescriptorNotFound"
};

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t ShellGattDb_Init(void)
{
    bleResult_t result = gBleSuccess_c;

    result = GattDbDynamic_AddGattService(NULL);
    
    if( gBleSuccess_c == result )
    {
        result = GattDbDynamic_AddGapService(NULL);
    }
    
    return result;
}

int8_t ShellGattDb_Command(uint8_t argc, char * argv[])
{
    uint8_t i;
    
    if (argc < 2)
    {
        return CMD_RET_USAGE;
    }
    
    for (i=0; i< mShellGattDbCmdsCount_c; i++)
    {
        if(!strcmp((char*)argv[1], mGattDbShellCmds[i].name) )
        {
            return mGattDbShellCmds[i].cmd(argc-2, (char **)(&argv[2]));
        }
    }
    return CMD_RET_USAGE;
}

static int8_t ShellGattDb_Read(uint8_t argc, char * argv[])
{
    uint8_t *pValue = NULL;
    uint8_t result;
    uint16_t length, handle;
    
    if (argc != 1)
    {
        return CMD_RET_USAGE;
    }        
    
    pValue = MEM_BufferAlloc(mMaxCharValueLength_d);
    
    if (!pValue)
    {
        return CMD_RET_FAILURE;
    }
        
    handle = atoi(argv[0]);
    result =  (uint8_t) (0xFF & GattDb_ReadAttribute(handle, mMaxCharValueLength_d, pValue, &length)); 
    
    if (result != gGattDbSuccess_c)
    {
        shell_write("\n\r-->  GATTDB Event: Procedure Error ");
        shell_write((char*)mGattDbStatus[result]);
        SHELL_NEWLINE();      
    }
    else
    {
        shell_write("\n\r-->  GATTDB Event: Attribute Read ");
        shell_write("\n\r     Value: ");
        shell_writeHexLe(pValue, length);      
    }
    
    MEM_BufferFree(pValue);
    return CMD_RET_SUCCESS;
}

static int8_t ShellGattDb_Write(uint8_t argc, char * argv[])
{
    uint16_t length, handle;
    uint8_t result;
    
    if (argc != 2)
    {
        return CMD_RET_USAGE;
    }

    handle = atoi(argv[0]);
    
    length = ShellGattDb_ParseHexValue(argv[1]);
      
    if (length > mMaxCharValueLength_d)
    {
        shell_write("\n\r-->  Variable length exceeds maximum!");
        return CMD_RET_FAILURE;
    }    
    
    result =  (uint8_t) (0xFF & GattDb_WriteAttribute(handle, length, (uint8_t*)argv[1])); 
    
    if (result != gGattDbSuccess_c)
    {
        shell_write("\n\r-->  GATTDB Event: Procedure Error ");
        shell_write((char*)mGattDbStatus[result]);
        SHELL_NEWLINE();      
    }
    else
    {
        shell_write("\n\r-->  GATTDB Event: Attribute Written ");
    }    
    return CMD_RET_SUCCESS;
}

static int8_t ShellGattDb_AddService(uint8_t argc, char * argv[])
{
    serviceInfo_t* pServiceInfo = NULL;
    uint16_t       serviceUuid = 0, length;
    
    if (argc != 1)
    {
        return CMD_RET_USAGE;
    }
    
    length = ShellGattDb_ParseHexValue(argv[0]);
    
    if (length == 2)
    {
        FLib_MemCpy(&serviceUuid, argv[0], sizeof(uint16_t));
    }
    
    switch (serviceUuid)
    {
        case gBleSig_GenericAccessProfile_d:
        case gBleSig_GenericAttributeProfile_d:
        {
            shell_write("\n\r-->  GATTDB Event: Service already exists! ");
            SHELL_NEWLINE();            
        }
        break;

        case gBleSig_HeartRateService_d:
        {
            pServiceInfo = GattDbDynamic_GetHeartRateService();
        }
        break;

        case gBleSig_BatteryService_d:
        {
            pServiceInfo = GattDbDynamic_GetBatteryService();
        }
        break;

        case gBleSig_DeviceInformationService_d:
        {
            pServiceInfo = GattDbDynamic_GetDeviceInformationService();
        }
        break;

        case gBleSig_IpsService_d:
        {
            pServiceInfo = GattDbDynamic_GetIpssService();
        }
        break;
        
        default:
        {
            shell_write("\n\r-->  GATTDB Event: Service not supported ! ");
            SHELL_NEWLINE();  
        }
    }
    
    if ((pServiceInfo != gBleSuccess_c) &&
        (gBleSuccess_c == ShellGattDb_AddServiceInDatabase(pServiceInfo)))
    {
        shell_write("\n\r-->  GATTDB Event: Service Added in database. ");
        SHELL_NEWLINE();            
    }
    else
    {
        shell_write("\n\r-->  GATTDB Event: Insufficient memory. ");
        SHELL_NEWLINE();            
    }
    MEM_BufferFree(pServiceInfo);
    return CMD_RET_SUCCESS;
}

static int8_t ShellGattDb_Erase(uint8_t argc, char * argv[])
{
    if (argc != 0)
    {
        return CMD_RET_USAGE;
    }
        
    GattDbDynamic_ReleaseDatabase();
    
    GattDbDynamic_Init();
    ShellGattDb_Init();

    return CMD_RET_SUCCESS;
}


static uint8_t ShellGattDb_ParseHexValue(char* pInput)
{
    uint8_t i, length = strlen(pInput);
    uint32_t value;
    
    /* If the hex misses a 0, return error */
    if (length % 2)
    {
        return 0;
    }
    
    if(!strncmp(pInput, "0x", 2))
    {
        length -= 2;
        /* Save as little endian hex value */
        value = strtoul(pInput + 2, NULL, 16);
        
        FLib_MemCpy(pInput, &value, sizeof(uint32_t));
        
        return (length/2);
    }
    else
    {        
        char octet[2];        
        
        /* Save as big endian hex */
        for(i=0;i < length / 2; i++)
        {
            FLib_MemCpy(octet, &pInput[i*2], 2);
            
            pInput[i] = strtoul(octet, NULL, 16);
        }
        return length/2;        
    }

}

static bleResult_t ShellGattDb_AddServiceInDatabase(serviceInfo_t* pServiceInfo)
{
    bleResult_t result;
    uint16_t    handle;
    
    /* Add service */
    result = GattDbDynamic_AddPrimaryServiceDeclaration(gBleUuidType16_c, 
                                                        (bleUuid_t*)&pServiceInfo->uuid16, 
                                                        &handle);      
    if (result != gBleSuccess_c)
    {
        return result;
    }
    
    shell_write("\n\r  --> ");
    shell_write(ShellGatt_GetServiceName(pServiceInfo->uuid16));
    
    /* Add characteristics */
    for (uint8_t i=0; i < pServiceInfo->nbOfCharacteristics; i++)
    {
        characteristicInfo_t* pCharacteristicInfo = &pServiceInfo->pCharacteristicInfo[i];
             
        result = GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c,
                                            (bleUuid_t*)&pCharacteristicInfo->uuid16,
                                            pCharacteristicInfo->properties,
                                            pCharacteristicInfo->maxValueLength,
                                            pCharacteristicInfo->valueLength,
                                            pCharacteristicInfo->pValue,                                                                                 
                                            pCharacteristicInfo->accessPermissions,
                                            &handle);
        if (result != gBleSuccess_c)
        {
            return result;
        }
               
        shell_write("\n\r     - ");
        shell_write(ShellGatt_GetCharacteristicName(pCharacteristicInfo->uuid16));

        shell_write("  Value Handle: ");
        shell_writeDec(handle+1);      
        
        /* Add descriptors */
        for (uint8_t j=0; j < pCharacteristicInfo->nbOfDescriptors; j++)
        {
            descriptorInfo_t* pDescriptorInfo = &pCharacteristicInfo->pDescriptorInfo[j];
            
            result = GattDbDynamic_AddCharacteristicDescriptor(gBleUuidType16_c,
                                                   (bleUuid_t*)&pDescriptorInfo->uuid16,
                                                   pDescriptorInfo->valueLength,
                                                   pDescriptorInfo->pValue,
                                                   pDescriptorInfo->accessPermissions,
                                                   &handle);
            if (result != gBleSuccess_c)
            {
                return result;
            }
            
            shell_write("\n\r       - ");
            shell_write(ShellGatt_GetDescriptorName(pDescriptorInfo->uuid16));
            shell_write("  Descriptor Handle: ");
            shell_writeDec(handle);            
        }
        
        /* Add CCCD */
        if (pCharacteristicInfo->bAddCccd)
        {
             result = GattDbDynamic_AddCccd(&handle);

            if (result != gBleSuccess_c)
            {
                return result;
            }
                        
            shell_write("\n\r       - CCCD Handle: ");
            shell_writeDec(handle);
        }
    }    
    return result;
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
