/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the private header file for the MAC FSCI cmmands.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _FSCI_ASP_COMMANDS_H_
#define _FSCI_ASP_COMMANDS_H_


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "FsciInterface.h"
#include "AspInterface.h"

#if gFsciIncluded_c && gFsciHost_802_15_4_c
#include "fsl_os_abstraction.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef struct FsciHost_AspInterface_tag
{   
    instanceId_t          upperLayerId;
    uint8_t               fsciInterfaceId;
}FsciHostAspInterface_t;

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
/* OpGroup codes used with FSCI */
#define gFSCI_AspAppOpcodeGroup_c        0x94    /* ASP_APP_SapHandler           */
#define gFSCI_AppAspOpcodeGroup_c        0x95    /* APP_ASP_SapHandler           */


/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void AspSapMonitor(void *pData, void* param, uint32_t interfaceId);
void fsciRegisterAsp(instanceId_t aspInterface, uint32_t fsciInterfaceId);
void fsciRegisterAspToHost(instanceId_t aspInterface, uint32_t fsciInterfaceId);
uint32_t fsciGetAspInterfaceId(instanceId_t phyInstance);

#endif /* _FSCI_ASP_COMMANDS_H_ */