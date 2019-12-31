/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_init.h"
#include "ble_general.h"
#include "ble_host_task_config.h"
#include "ble_controller_task_config.h"
#include "board.h"

#if (defined(gUseHciTransportDownward_d) && (gUseHciTransportDownward_d == 1)) || \
    (defined(gUseHciTransportUpward_d) && (gUseHciTransportUpward_d == 1))
#include "hci_transport.h"
#endif

#if !gUseHciTransportDownward_d
#include "controller_interface.h"
#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
#if (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_gfsk_bt_0p5_h_0p5_config.h"
#include "nxp_xcvr_coding_config.h"
#else
#include "fsl_xcvr.h"
#endif
#endif /* CPU_QN908X */
#include "Flash_Adapter.h"
#endif /* gUseHciTransportDownward_d */

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#ifndef cMCU_SleepDuringBleEvents
    #define cMCU_SleepDuringBleEvents    0
#endif

/* This enables/disables the HCI Reset command sent by the Host at init sequence
   Default value is disabled, only for gUseHciTransportDownward_d is required */
#ifndef gHostInitResetController_c
    #if gUseHciTransportDownward_d
        #define gHostInitResetController_c TRUE
    #else
        #define gHostInitResetController_c FALSE
    #endif
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if !gUseHciTransportUpward_d
bool_t gHostInitResetController = gHostInitResetController_c;
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
extern bool_t Ble_CheckMemoryStorage(void);
#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
static xcvrStatus_t Ble_RadioInit(void);
#endif /* CPU_QN908X */
#if  (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || \
      defined(KW39A4_SERIES) || defined(CPU_MK64FN1M0VLL12))
void Controller_ParamInit(void);
#endif
#if defined(gXcvrDacTrimValueSorageAddr_d)
static uint32_t SaveXcvrDcocDacTrimToFlash(xcvr_DcocDacTrim_t *xcvrDacTrim);
static uint32_t RestoreXcvrDcocDacTrimFromFlash(xcvr_DcocDacTrim_t *xcvrDacTrim);
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#ifndef CPU_QN908X
extern bool_t gEnableSingleAdvertisement;
extern bool_t gMCUSleepDuringBleEvents;
#endif /* CPU_QN908X */

#if defined(gPreserveXcvrDacTrimValue_d) && gPreserveXcvrDacTrimValue_d
/*  This variable must be preserved between CPU reset.
    Place the XCVR DAC trim value in RAM retention region or in Flash. */
static xcvr_DcocDacTrim_t mXcvrDacTrim;
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Ble_Initialize
(
    gapGenericCallback_t gapGenericCallback
)
{

#if (gUseHciTransportDownward_d == 1)

    /* Configure HCI Transport */
    hcitConfigStruct_t hcitConfigStruct =
    {
        .interfaceType = gHcitInterfaceType_d,
        .interfaceChannel = gHcitInterfaceNumber_d,
        .interfaceBaudrate = gHcitInterfaceSpeed_d,
        .transportInterface =  Ble_HciRecv
    };

    /* HCI Transport Init */
    if (gHciSuccess_c != Hcit_Init(&hcitConfigStruct))
    {
        return gHciTransportError_c;
    }

    /* Check for available memory storage */
    if (!Ble_CheckMemoryStorage())
    {
        return gBleOutOfMemory_c;
    }

    /* BLE Host Tasks Init */
    if (osaStatus_Success != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }

    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback, Hcit_SendPacket);

#elif (gUseHciTransportUpward_d == 1)

#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
    /* BLE Radio Init */
    if ( gXcvrSuccess_c != Ble_RadioInit())
    {
        return gBleUnexpectedError_c;
    }
#endif /* CPU_QN908X */

    if (osaStatus_Success != Controller_TaskInit())
    {
        return gBleOsError_c;
    }

#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
    gMCUSleepDuringBleEvents = (bool_t)cMCU_SleepDuringBleEvents;
#endif /* CPU_QN908X */

    /* BLE Controller Init */
    if (osaStatus_Success != Controller_Init(Hcit_SendPacket))
    {
        return gBleOsError_c;
    }   

#if  (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || \
      defined(KW38Z4_SERIES) || defined(KW39A4_SERIES) || defined(CPU_MK64FN1M0VLL12))
    /* Configures default TX power */
    Controller_ParamInit();
#endif

    /* Configure HCI Transport */
    hcitConfigStruct_t hcitConfigStruct =
    {
        .interfaceType = gHcitInterfaceType_d,
        .interfaceChannel = gHcitInterfaceNumber_d,
        .interfaceBaudrate = gHcitInterfaceSpeed_d,
        .transportInterface =  (hciTransportInterface_t)Hci_SendPacketToController
    };

    return Hcit_Init(&hcitConfigStruct);

#else

#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
    /* BLE Radio Init */
    if ( gXcvrSuccess_c != Ble_RadioInit())
    {
        return gBleUnexpectedError_c;
    }
#endif /* CPU_QN908X */

    /* BLE Controller Task Init */
    if (osaStatus_Success != Controller_TaskInit())
    {
        return gBleOsError_c;
    }

#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
    gEnableSingleAdvertisement = FALSE;
    gMCUSleepDuringBleEvents = (bool_t)cMCU_SleepDuringBleEvents;
#endif /* CPU_QN908X */

    /* BLE Controller Init */
    if (osaStatus_Success != Controller_Init(Ble_HciRecv))
    {
        return gBleOsError_c;
    }  

#if  (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || \
      defined(KW38Z4_SERIES) || defined(KW39A4_SERIES) || defined(CPU_MK64FN1M0VLL12))
    /* Configures default TX power */
    Controller_ParamInit();
#endif

    /* Check for available memory storage */
    if (!Ble_CheckMemoryStorage())
    {
        return gBleOutOfMemory_c;
    }

    /* BLE Host Tasks Init */
    if (osaStatus_Success != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }

    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback,
                (hciHostToControllerInterface_t) Hci_SendPacketToController);

#endif
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#if defined(gXcvrDacTrimValueSorageAddr_d)
static uint32_t  SaveXcvrDcocDacTrimToFlash(xcvr_DcocDacTrim_t *xcvrDacTrim)
{
    uint32_t status;
    
    if (FLib_MemCmpToVal((void const *)gXcvrDacTrimValueSorageAddr_d, 0xFF, sizeof(xcvr_DcocDacTrim_t)))
    {
        status = NV_FlashProgramUnaligned(gXcvrDacTrimValueSorageAddr_d,
                                          sizeof(xcvr_DcocDacTrim_t),
                                          (uint8_t*)xcvrDacTrim);
    }
    else
    {
        status = 1;
    }
    
    return status;
}

static uint32_t RestoreXcvrDcocDacTrimFromFlash(xcvr_DcocDacTrim_t *xcvrDacTrim)
{
    uint32_t status;
    
    if (FLib_MemCmpToVal((void const *)gXcvrDacTrimValueSorageAddr_d, 0xFF, sizeof(xcvr_DcocDacTrim_t)))
    {
        status = 1;
    }
    else
    {
        status = 0;
        FLib_MemCpy(xcvrDacTrim, (void const *)gXcvrDacTrimValueSorageAddr_d, sizeof(xcvr_DcocDacTrim_t));
    }
    
    return status;
}
#endif

/*! *********************************************************************************
* \brief   Initializez the XCVR module
*
* \return gXcvrSuccess_c or an error status.
*
********************************************************************************** */
#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
static xcvrStatus_t Ble_RadioInit(void)
{
    xcvrStatus_t status;
    uint32_t count = 0;
#if (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    const xcvr_config_t *xcvrConfig         = &xcvr_gfsk_bt_0p5_h_0p5_1mbps_full_config;
    const xcvr_coding_config_t *rbmeConfig  = &xcvr_ble_coded_s8_config;
#endif /* (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES)) */
#if defined(gPreserveXcvrDacTrimValue_d) && gPreserveXcvrDacTrimValue_d
#if (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    status = XCVR_InitNoDacTrim(&xcvrConfig, &rbmeConfig);
#else
    status = XCVR_InitNoDacTrim(BLE_MODE, DR_1MBPS);
#endif /* (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES)) */
    if ( gXcvrSuccess_c == status )
    {
#if defined(gXcvrDacTrimValueSorageAddr_d)
        (void)RestoreXcvrDcocDacTrimFromFlash(&mXcvrDacTrim);
#endif
        /* Check if trim value is valid */
        if (FLib_MemCmpToVal(&mXcvrDacTrim, 0x00, sizeof(mXcvrDacTrim)) == FALSE &&
            FLib_MemCmpToVal(&mXcvrDacTrim, 0xFF, sizeof(mXcvrDacTrim)) == FALSE)
        {
            /* Trim value is valid. Restore it. */
            status = XCVR_SetDcocDacTrims(&mXcvrDacTrim);
        }
        else
        {
            /* Trim value is not valid. Calculate now. */
            do {
                count++;
                status = XCVR_CalculateDcocDacTrims(&mXcvrDacTrim);
            } while ((gXcvrSuccess_c != status) && (count < gBleXcvrInitRetryCount_c));
#if defined(gXcvrDacTrimValueSorageAddr_d)
            (void)SaveXcvrDcocDacTrimToFlash(&mXcvrDacTrim);
#endif
        }
    }
#else
    do {
        count++;
#if (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
        status = XCVR_Init(&xcvrConfig, &rbmeConfig);
#else
        status = XCVR_Init(BLE_MODE, DR_1MBPS);
#endif
    } while ((gXcvrSuccess_c != status) && (count < gBleXcvrInitRetryCount_c));
#endif /* defined(gPreserveXcvrDacTrimValue_d) && gPreserveXcvrDacTrimValue_d */

#if !(defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    /* XtalTrim is not functionnal for KW37 series */
    (void)XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );
#endif
    return status;
}
#endif /* CPU_QN908X */

/*! *********************************************************************************
* @}
********************************************************************************** */
