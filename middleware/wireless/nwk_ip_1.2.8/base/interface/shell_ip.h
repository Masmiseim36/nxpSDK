/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _SHELL_IP_H
#define _SHELL_IP_H
/*!=================================================================================================
\file       shell_ip.h
\brief      This is a header file for the shell application for the IP stack.
==================================================================================================*/
#if THREAD_USE_SHELL
/*==================================================================================================
Include Files
==================================================================================================*/
#include "network_utils.h"
#include "shell_config.h"
#include "shell.h"
#include "ip.h"
#include "ip6.h"
#include "thread_types.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#if defined(__IAR_SYSTEMS_ICC__)
#pragma section="SHELL_COMMANDS"
#endif

#if defined(__GNUC__)
extern uint32_t __start_SHELL_COMMANDS;
extern uint32_t __stop_SHELL_COMMANDS;
  #define gSHELL_COMMANDS_startAddr_d ((cmd_tbl_t*)__start_SHELL_COMMANDS)
  #define gSHELL_COMMANDS_endAddr_d   ((cmd_tbl_t*)__stop_SHELL_COMMANDS)
#elif defined(__IAR_SYSTEMS_ICC__)
  #define gSHELL_COMMANDS_startAddr_d ((cmd_tbl_t*)__section_begin("SHELL_COMMANDS"))
  #define gSHELL_COMMANDS_endAddr_d   ((cmd_tbl_t*)__section_end("SHELL_COMMANDS"))
#else
  #define gSHELL_COMMANDS_startAddr_d ((cmd_tbl_t*)0)
  #define gSHELL_COMMANDS_endAddr_d   ((cmd_tbl_t*)0)
  #warning shell command will not be stored!
#endif

#define gSHELL_COMMANDS_entries_d  ( ((uint32_t)gSHELL_COMMANDS_endAddr_d - \
                                        (uint32_t)gSHELL_COMMANDS_startAddr_d)/ \
                                        sizeof(cmd_tbl_t) )

#define SET_HANDLER_STRUCT_NAME_CONCAT(structName, moduleName, line) g##structName##_##moduleName##_##line
#define SET_HANDLER_STRUCT_NAME(structName, moduleName, line) SET_HANDLER_STRUCT_NAME_CONCAT(structName, moduleName, line)

#if defined(__IAR_SYSTEMS_ICC__)
#define ShellComm_RegisterStatic(module, ...) \
    _Pragma("location=\"SHELL_COMMANDS\"") __root \
    const cmd_tbl_t SET_HANDLER_STRUCT_NAME(ShellCommEntry, module, __LINE__) = {__VA_ARGS__};

#elif defined(__GNUC__)

#define ShellComm_RegisterStatic(module, name, maxargs, repeatable, cmd, usage, help, complete) \
    const cmd_tbl_t SET_HANDLER_STRUCT_NAME(ShellCommEntry, module, __LINE__) __attribute__ ((section ("SHELL_COMMANDS"), used)) = \
    {name, maxargs, repeatable, cmd, usage, help, complete}

#else
    #warning shell command will not be stored!
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    gAllIpAddr_c,
    gLinkLocalAddr_c,
    gMeshLocalAddr_c,
    gGlobalAddr_c
}threadAddrTypes_t;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void SHELLComm_Init(taskMsgQueue_t *pMsgQueue)
\brief  This function is used to initialize the SHELL commands module.

\param  [in]    pMsgQueue    Pointer to the message queue
***************************************************************************************************/
void SHELLComm_Init(taskMsgQueue_t *pMsgQueue);

/*!*************************************************************************************************
\fn     void SHELL_PrintIpAddr(threadAddrTypes_t addrType)
\brief  This function is used for printing in shell terminal the IP addresses of a certain type.

\param  [in]    threadAddrTypes_t    the type of the IP addresses to be printed
***************************************************************************************************/
void SHELL_PrintIpAddr(threadAddrTypes_t addrType);
#if THR_ENABLE_EVENT_MONITORING
/*!*************************************************************************************************
\fn     bool_t SHELL_EvMonitorInit(instanceId_t instanceId)
\brief  This function is used to initialize the Shell event monitor.

\param  [in]   instanceId    Instance ID
***************************************************************************************************/
void SHELL_EvMonitorInit(instanceId_t instanceId);

/*!*************************************************************************************************
\fn     bool_t SHELL_EvMonitor(void *param)
\brief  This function is used to handle the events received from the Event monitor.

\param  [in]   param    Pointer to monitor event
***************************************************************************************************/
void SHELL_EvMonitor(void* param);
#endif /* THR_ENABLE_EVENT_MONITORING */

/*!*************************************************************************************************
\fn     void SHELL_NwkScanPrint(thrNwkScanResults_t *pScanResults)
\brief  This function is used to network scan information.

\param  [in]    pScanResults   The scan results
***************************************************************************************************/
void SHELL_NwkScanPrint(thrNwkScanResults_t *pScanResults);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_USE_SHELL */
/*================================================================================================*/
#endif  /* _SHELL_IP_H */


