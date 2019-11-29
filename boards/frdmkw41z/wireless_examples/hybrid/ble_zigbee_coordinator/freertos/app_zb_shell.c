/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include <jendefs.h>
#include "EmbeddedTypes.h"
#include "app_zb_shell.h"
#include "app_zb_utils.h"
#include "shell.h"
#include "app_zcl_cfg.h"
#include "zps_apl.h"
#include "zps_apl_aib.h"
#include "bdb_api.h"
#include "bdb_nf.h"
#include "Identify.h"
#include "OnOff.h"
#include "ZTimer.h"
#include "app_zps_cfg.h"
#include "bdb_nf.h"

#ifdef APP_ALLOW_ZPS_SUSPEND
#include "app_coordinator.h"
#include "app_main.h"
#endif
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
extern void APP_vFactoryResetRecords(void);

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void vPrintAPSTable(void);
static int8_t SHELL_Reboot(uint8_t argc, char *argv[]);
static int8_t SHELL_FactoryResetReq(uint8_t argc, char *argv[]);
static int8_t SHELL_ChannelReq(uint8_t argc, char *argv[]);
static int8_t SHELL_PanIdReq(uint8_t argc, char *argv[]);
static int8_t SHELL_EpidReq(uint8_t argc, char *argv[]);
static int8_t SHELL_ShortAddrReq(uint8_t argc, char *argv[]);
static int8_t SHELL_ExtendedAddrReq(uint8_t argc, char *argv[]);
static int8_t SHELL_PrintAPSTableReq(uint8_t argc, char *argv[]);
#ifdef APP_ALLOW_ZPS_SUSPEND
static int8_t SHELL_ZpsStop(uint8_t argc, char *argv[]);
static int8_t SHELL_ZpsStart(uint8_t argc, char *argv[]);
#endif
#ifndef ZBPRO_DEVICE_TYPE_ZED
static int8_t SHELL_FormReq(uint8_t argc, char *argv[]);
#endif

#if ((defined BDB_SUPPORT_FIND_AND_BIND_INITIATOR) || (defined BDB_SUPPORT_FIND_AND_BIND_TARGET))
static int8_t SHELL_FindReq(uint8_t argc, char *argv[]);
#endif

#ifdef BDB_SUPPORT_NWK_STEERING
static int8_t SHELL_SteerReq(uint8_t argc, char *argv[]);
#endif

#ifndef ZPS_COORDINATOR
static int8_t SHELL_JoinReq(uint8_t argc, char *argv[]);
#endif

#ifdef ZBPRO_DEVICE_TYPE_ZED
static int8_t SHELL_PollIntervalReq(uint8_t argc, char *argv[]);
#endif

#ifdef IDENTIFY_CLIENT
static int8_t SHELL_IdentifyReq(uint8_t argc, char *argv[]);
#endif

#ifdef ONOFF_CLIENT
static int8_t SHELL_ToggleReq(uint8_t argc, char *argv[]);
#endif

#ifdef ZPS_COORDINATOR
static int8_t SHELL_InstallCodeReq(uint8_t argc, char *argv[]);
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
extern uint8_t  gShellSerMgrIf;

const cmd_tbl_t gZigBee_commands[] = 
{
    {
        "reboot", 1, 0, SHELL_Reboot
#if SHELL_USE_HELP
        , "MCU Reset",
        "MCU Reset"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "factoryreset", 1, 0, SHELL_FactoryResetReq
#if SHELL_USE_HELP
        ,"FactoryReset Command",
        "Factory reset the device, erasing persistent data"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },

#ifndef ZBPRO_DEVICE_TYPE_ZED
    {
        "form", 1, 0, SHELL_FormReq
#if SHELL_USE_HELP
        ,"Form ZigBee network",
        "Triggers network formation for a device not on a network"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
    
#ifdef ZBPRO_DEVICE_TYPE_ZED
    {
        "polltime", 2, 0, SHELL_PollIntervalReq
#if SHELL_USE_HELP
        ,"Set Poll interval",
        "Set End Device Polling interval in seconds.\n\r"
        " polltime [HEX_VALUE]"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif

#if ((defined BDB_SUPPORT_FIND_AND_BIND_INITIATOR) || (defined BDB_SUPPORT_FIND_AND_BIND_TARGET))
    {
        "find", 1, 0, SHELL_FindReq
#if SHELL_USE_HELP
        ,"Find and Bind",
#if defined(BDB_SUPPORT_FIND_AND_BIND_INITIATOR)
        "Triggers Find and Bind as an Initiator"
#elif defined(BDB_SUPPORT_FIND_AND_BIND_TARGET)
        "Triggers Find and Bind as a Target"
#endif
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif

#ifdef BDB_SUPPORT_NWK_STEERING
    {
        "steer", 1, 0, SHELL_SteerReq
#if SHELL_USE_HELP
        ,"Network Steering",
        "Triggers Network Steering for a device on the network"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif

#if defined(BDB_SUPPORT_NWK_STEERING) && !defined(ZPS_COORDINATOR)
    {
        "join", 1, 0, SHELL_JoinReq
#if SHELL_USE_HELP
        ,"Network Join",
        "Join a device on the network"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
    
#ifdef IDENTIFY_CLIENT
    {
        "identify", 5, 0, SHELL_IdentifyReq
#if SHELL_USE_HELP
        ,"Identify device",
        "Identify a device from a network\r\n"
        " identify <addr> <src_endpoint> <dst_endpoint> <timeout>"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif /* IDENTIFY_CLIENT */
    
#ifdef ONOFF_CLIENT
    {
        "toggle", 1, 0, SHELL_ToggleReq
#if SHELL_USE_HELP
        ,"Toggle the bound devices",
        "Sends an On Of Togle Command to the bound devices\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif /* ONOFF_CLIENT */
    
    {
        "print", 1, 0, SHELL_PrintAPSTableReq
#if SHELL_USE_HELP
        ,"Print APS Table",
        "Print APS Key Table"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
#ifdef ZPS_COORDINATOR
    {
        "code", 3, 0, SHELL_InstallCodeReq
#if SHELL_USE_HELP
        ,"Add device install code",
        "Provisions an install code into the APS Key Table\r\n"
        " code <addr> <install_code>"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif /* ZPS_COORDINATOR */
    
    {
        "channel", 2, 0, SHELL_ChannelReq
#if SHELL_USE_HELP
        ,"Get/Set Nwk channel",
        " channel [DEC_VALUE]"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "panid", 2, 0, SHELL_PanIdReq
#if SHELL_USE_HELP
        ,"Get/Set Nwk PAN Id",
        " panid [HEX_VALUE]"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "shortaddr", 2, 0, SHELL_ShortAddrReq
#if SHELL_USE_HELP
        ,"Get/Set Device Short Address",
        " shortaddr [HEX_VALUE]"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "epid", 1, 0, SHELL_EpidReq
#if SHELL_USE_HELP
        ,"Get Device Extended PAN Id",
        "epid"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "extendedaddr", 2, 0, SHELL_ExtendedAddrReq
#if SHELL_USE_HELP
        ,"Get Device Extended Address",
        "extendedaddr"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#ifdef APP_ALLOW_ZPS_SUSPEND
    {
        "ztop", 1, 0, SHELL_ZpsStop
#if SHELL_USE_HELP
        ,"Stop Zigbee functionality",
        "ztop"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "ztart", 1, 0, SHELL_ZpsStart
#if SHELL_USE_HELP
        ,"Resume Zigbee functionality",
        "ztart"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
};

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*!*************************************************************************************************
\fn     void app_zb_shell_init(void
\brief  Initialize ZigBee Shell.
***************************************************************************************************/
void app_zb_shell_init(void)
{
    static const char * prompt = "$ ";
    
    shell_init((char*)prompt);
    shell_register_function_array((cmd_tbl_t*)gZigBee_commands, NumberOfElements(gZigBee_commands));
    
    APP_SerialId = gShellSerMgrIf;
}

/*!*************************************************************************************************
\fn     void app_zb_shell_task(void)
\brief  Process characters received from the serial interface.
***************************************************************************************************/
void app_zb_shell_task(void)
{
#if SHELL_USE_ALT_TASK
    shell_task();
#endif
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_Reboot(uint8_t argc, char *argv[])
\brief  This function is used to reset the device.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Reboot
(
    uint8_t argc,
    char *argv[]
)
{
    ResetMCU();
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     int8_t SHELL_FactoryResetReq(uint8_t argc, char *argv[])
\brief  This function is used to reset the device and restore the factory defaults.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_FactoryResetReq
(
    uint8_t argc,
    char *argv[]
)
{
    shell_write("\n\rReset ZigBee stack to factory defaults!\n\r");
    if (ZPS_E_SUCCESS !=  ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE)) 
    {
        APP_vFactoryResetRecords();
        ResetMCU();
    }
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_FormReq(uint8_t argc, char *argv[])
\brief  Start network formation.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#ifndef ZBPRO_DEVICE_TYPE_ZED
static int8_t SHELL_FormReq(uint8_t argc, char *argv[])
{
    BDB_teStatus eStatus;
    /* Create Network */
    if (FALSE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
    {
#ifdef APP_ALLOW_ZPS_SUSPEND
        /* Turn rx on. */
        ZPS_vMacPibSetRxOnWhenIdle(TRUE, FALSE);
        /* Start the ZCL timer. The ZCL timer is stopped when Zigbee is not
        running to allow the device to enter low power mode. */
        (void)ZTIMER_eStart(u8TimerZCL, 100);
#endif
        eStatus = BDB_eNfStartNwkFormation();
        APP_vPrintf("Nwk Formation %02x\r\n", eStatus);
        if (eStatus != 0 && eStatus != 7)
        {
            APP_vPrintf("APP_EVENT: Formation Failed %02x\r\n", eStatus);
        }
        else
        {
            APP_vPrintf("APP-ZDO: Network started Channel = %d\r\n", ZPS_u8AplZdoGetRadioChannel());
        } 
        
    }
    return CMD_RET_SUCCESS;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_PollIntervalReq(uint8_t argc, char *argv[])
\brief  Set Polling interval

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#ifdef ZBPRO_DEVICE_TYPE_ZED
static int8_t SHELL_PollIntervalReq(uint8_t argc, char *argv[])
{
    int8_t status = CMD_RET_SUCCESS;
    extern uint8 u8TimerPoll;
    uint32 u32TimerSeconds;

    if (argc != 2)
    {
        status = CMD_RET_USAGE;
    }
    else
    {
        u32TimerSeconds = (uint8_t)hex_str_to_uint(argv[1]);
        ZTIMER_eStop(u8TimerPoll);
        ZTIMER_eStart(u8TimerPoll, u32TimerSeconds);
    }

    return status;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_FindReq(uint8_t argc, char *argv[])
\brief  Start Find and Bind procedure.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#if ((defined BDB_SUPPORT_FIND_AND_BIND_INITIATOR) || (defined BDB_SUPPORT_FIND_AND_BIND_TARGET))
static int8_t SHELL_FindReq(uint8_t argc, char *argv[])
{
    BDB_teStatus eStatus;
    uint8_t src_endpoint = 1;

    /* Find & Bind */
#ifdef USE_GROUPS
    sBDB.sAttrib.u16bdbCommissioningGroupID = GROUP_ID;
#endif

#if defined(BDB_SUPPORT_FIND_AND_BIND_INITIATOR)    
    eStatus = BDB_eFbTriggerAsInitiator(src_endpoint);
#elif defined(BDB_SUPPORT_FIND_AND_BIND_TARGET)
    eStatus = BDB_eFbTriggerAsTarget(src_endpoint);
#endif
    APP_vPrintf("Find and Bind status %02x\r\n", eStatus);

    return CMD_RET_SUCCESS;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_SteerReq(uint8_t argc, char *argv[])
\brief  Start network steering procedure.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#ifdef BDB_SUPPORT_NWK_STEERING
static int8_t SHELL_SteerReq(uint8_t argc, char *argv[])
{
    BDB_teStatus eStatus;
    /* Permit Join */
    if (TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
    {
        eStatus = BDB_eNsStartNwkSteering();
        APP_vPrintf("Nwk Steering %02x\r\n", eStatus);
    }
    return CMD_RET_SUCCESS;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_JoinReq(uint8_t argc, char *argv[])
\brief  Start network join procedure.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#if defined(BDB_SUPPORT_NWK_STEERING) && !defined(ZPS_COORDINATOR)
static int8_t SHELL_JoinReq(uint8_t argc, char *argv[])
{
    BDB_teStatus eStatus;
    /* Join */
    if (FALSE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
    {
        eStatus = BDB_eNsStartNwkSteering();
        APP_vPrintf("Nwk Join %02x\r\n", eStatus);
    }
    return CMD_RET_SUCCESS;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_IdentifyReq(uint8_t argc, char *argv[])
\brief  Start device identify procedure.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#ifdef IDENTIFY_CLIENT
static int8_t SHELL_IdentifyReq(uint8_t argc, char *argv[])
{
    tsCLD_Identify_IdentifyRequestPayload    sCommand;
    uint8                  u8SeqNum = 0;
    teZCL_Status           eStatus;
    tsZCL_Address          sAddress;
    int8_t                 status = CMD_RET_SUCCESS;

    if (argc != 5)
    {
        status = CMD_RET_USAGE;
    }
    else
    {
        sAddress.uAddress.u64DestinationAddress = hex_str_to_uint(argv[1]);
        if (sAddress.uAddress.u64DestinationAddress > 0xFFFF)
        {
            sAddress.eAddressMode = E_ZCL_AM_IEEE;
        }
        else
        {
            sAddress.eAddressMode = E_ZCL_AM_SHORT;
        }
        sCommand.u16IdentifyTime =  hex_str_to_uint(argv[4]);
        eStatus = eCLD_IdentifyCommandIdentifyRequestSend (hex_str_to_uint(argv[2]),
                                                           hex_str_to_uint(argv[3]),
                                                           &sAddress,
                                                           &u8SeqNum,
                                                           &sCommand);
        APP_vPrintf("Identify %02x\r\n", eStatus);
    }
    
    return status;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_ToggleReq(uint8_t argc, char *argv[])
\brief  Sends an On Of Togle Command to the bound devices

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#ifdef ONOFF_CLIENT
static int8_t SHELL_ToggleReq(uint8_t argc, char *argv[])
{
    tsZCL_Address   sDestinationAddress;
    uint8 u8seqNo;
    teZCL_Status eStatus;

    sDestinationAddress.eAddressMode = E_ZCL_AM_BOUND_NON_BLOCKING;
    
    eStatus = eCLD_OnOffCommandSend(1,      /* Src Endpoint */
                                    0,      /* Dest Endpoint (bound so do not care) */
                                    &sDestinationAddress,
                                    &u8seqNo,
                                    E_CLD_ONOFF_CMD_TOGGLE);
    
    if (eStatus != E_ZCL_SUCCESS)
    {
    	APP_vPrintf("Send Toggle Failed x%02x Last error %02x\r\n",
                    eStatus, eZCL_GetLastZpsError());
    }
    return CMD_RET_SUCCESS;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_PrintAPSTableReq(uint8_t argc, char *argv[])
\brief  Print APS table.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_PrintAPSTableReq(uint8_t argc, char *argv[])
{
    vPrintAPSTable();
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_InstallCodeReq(uint8_t argc, char *argv[])
\brief  Set device install code.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
#ifdef ZPS_COORDINATOR
static int8_t SHELL_InstallCodeReq(uint8_t argc, char *argv[])
{
    int8_t   status = CMD_RET_SUCCESS;
    uint64_t u64Addr = hex_str_to_uint(argv[1]);
    uint8_t  Key[16];
    
    if (argc != 3)
    {
        status = CMD_RET_USAGE;
    }
    else if (!u64Addr)
    {
        APP_vPrintf("Address validation failed, use form 0123456789abcdef\r\n");
    }
    else
    {
        if (hex_str_to_array(argv[2], Key, 16) != 16)
        {
            APP_vPrintf("Key validation failed, use form 00112233445566778899aabbccddeeff\r\n");
        }
        else
        {
            ZPS_teStatus eStatus;
            eStatus = ZPS_eAplZdoAddReplaceInstallCodes( u64Addr, Key, 16, ZPS_APS_UNIQUE_LINK_KEY);
            APP_vPrintf("Key Added for %016llx, Status %02x\r\n", u64Addr, eStatus);
            vPrintAPSTable();
        }
    }
    
    return status;
}
#endif

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_ChannelReq(uint8_t argc, char *argv[])
\brief  Get/Set network channel (11-26).

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_ChannelReq(uint8_t argc, char *argv[])
{
    uint32_t value;
    uint32_t i = 11;
    
    if (argc == 2)
    {
        value = (uint8_t)dec_str_to_uint(argv[1]);

        if ((value >= 11) && (value <=26))
        {
            sBDB.sAttrib.u32bdbPrimaryChannelSet   = 1 << value;
            sBDB.sAttrib.u32bdbSecondaryChannelSet =  0;
            ZPS_vNwkNibSetChannel(ZPS_pvAplZdoGetNwkHandle(), value);
        }
    }
    
    value = sBDB.sAttrib.u32bdbPrimaryChannelSet;
    APP_vPrintf(" > ");    
    while(i<=26)
    {
      if (value & (1<<i)) 
      {
        APP_vPrintf("%d ",i); 
        
      }      
      i++; 
    }
    APP_vPrintf("\r\n");
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_PanIdReq(uint8_t argc, char *argv[])
\brief  Get/Set device PAN Id.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_PanIdReq(uint8_t argc, char *argv[])
{
    uint16_t value;
    int8_t status = CMD_RET_SUCCESS;

    if (argc == 2)
    {
        value = (uint16_t)hex_str_to_uint(argv[1]);
        
        if (value != 0xFFFF)
        {
            BDB_vNfSetPanID(value);
        }
        else
        {
            APP_vPrintf("Invalid PAN ID value\r\n");
            status = CMD_RET_FAILURE;
        }
    }

    if (status == CMD_RET_SUCCESS)
    {
        value = ZPS_u16NwkNibGetMacPanId(ZPS_pvAplZdoGetNwkHandle());
        APP_vPrintf(" > 0x%lx\r\n", value);
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_EpidReq(uint8_t argc, char *argv[])
\brief  Get/Set device extended PAN Id.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_EpidReq(uint8_t argc, char *argv[])
{
    uint64_t value;

    value = ZPS_u64NwkNibGetEpid(ZPS_pvAplZdoGetNwkHandle());
    APP_vPrintf(" > 0x%llx\r\n", value);
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_ExtendedAddrReq(uint8_t argc, char *argv[])
\brief  Get/Set device /IEEE address.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_ShortAddrReq(uint8_t argc, char *argv[])
{
    uint16_t value;
    
    if (argc == 2)
    {
        value = (uint16_t)hex_str_to_uint(argv[1]);
        BDB_vNfSetNwkAddress(value);
    }
    
    value = ZPS_u16NwkNibGetNwkAddr(ZPS_pvAplZdoGetNwkHandle());
    APP_vPrintf(" > 0x%lx\r\n", value);
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_ExtendedAddrReq(uint8_t argc, char *argv[])
\brief  Print device IEEE address.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_ExtendedAddrReq(uint8_t argc, char *argv[])
{
    uint64_t value;
    
    value = ZPS_u64NwkNibGetExtAddr(ZPS_pvAplZdoGetNwkHandle());
    APP_vPrintf(" > 0x%llx\r\n", value);
    return CMD_RET_SUCCESS;
}
#ifdef APP_ALLOW_ZPS_SUSPEND
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_ZpsStop(uint8_t argc, char *argv[])
\brief  Stop the Zigbee stack.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_ZpsStop(uint8_t argc, char *argv[])
{
    APP_vZpsStop();
    
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_ZpsStart(uint8_t argc, char *argv[])
\brief  Restore the Zigbee stack functionality.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_ZpsStart(uint8_t argc, char *argv[])
{
    APP_vZpsStart();
    return CMD_RET_SUCCESS;
}
#endif

/*!*************************************************************************************************
\private
\fn     static void vPrintAPSTable(void)
\brief  Prints the content of APS table
***************************************************************************************************/
static void vPrintAPSTable(void)
{
    uint8 i;
    uint8 j;
    ZPS_tsAplAib * psAplAib;
    uint64 u64Addr;
    
    psAplAib = ZPS_psAplAibGetAib();
    
    for ( i = 0 ; i < (psAplAib->psAplDeviceKeyPairTable->u16SizeOfKeyDescriptorTable + 3) ; i++ )
    {
        u64Addr = ZPS_u64NwkNibGetMappedIeeeAddr(ZPS_pvAplZdoGetNwkHandle(), psAplAib->psAplDeviceKeyPairTable->psAplApsKeyDescriptorEntry[i].u16ExtAddrLkup);
    	APP_vPrintf("%d MAC: 0x%x %x Key: ", i, (uint32)(u64Addr>>32), (uint32)(u64Addr&0xffffffff) );
        for(j=0; j<16;j++)
        {
            APP_vPrintf("%02x ", psAplAib->psAplDeviceKeyPairTable->psAplApsKeyDescriptorEntry[i].au8LinkKey[j]);
        }
        APP_vPrintf("\r\n");
    }
}