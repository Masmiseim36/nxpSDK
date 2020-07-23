/*! *********************************************************************************
 * \defgroup CONTROLLER
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#include "EmbeddedTypes.h"
#include "Messaging.h"
#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"
#include "ble_controller_task_config.h"
#include "Panic.h"
#include "controller_interface.h"
#include "Flash_Adapter.h"
#include "SecLib.h"
#include "board.h"
#ifdef CPU_QN908X
    #include "nvds.h"
#endif

#if (defined(gMWS_Enabled_d) && (gMWS_Enabled_d)) || (defined(gMWS_UseCoexistence_d) && (gMWS_UseCoexistence_d))
    #include "MWS.h"
#if  !(defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || \
       defined(KW39A4_SERIES) || defined(CPU_MK64FN1M0VLL12))
    #include "fsl_xcvr.h"
#else
    #include "nxp2p4_xcvr.h"
    #include "nxp_xcvr_gfsk_bt_0p5_h_0p5_config.h"
    #include "nxp_xcvr_coding_config.h"
#endif /* !(defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || 
            defined(KW39A4_SERIES) || defined(CPU_MK64FN1M0VLL12)) */
    #include "gpio_pins.h"
#endif
/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#define mBD_ADDR_RandPartSize_c 3U
#define mBD_ADDR_OUIPartSize_c  3U

#define mBoardUidSize_c        16
#define mBoardMacAddrSize_c     5

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
osaEventId_t mControllerTaskEvent;

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void ControllerTask(osaTaskParam_t argument);

#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d)
    extern uint32_t MWS_BLE_Callback(mwsEvents_t event);
#endif

#if defined(gMWS_UseCoexistence_d) && (gMWS_UseCoexistence_d)
    extern uint32_t MWS_COEX_BLE_Callback(mwsEvents_t event);
#endif
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static osaTaskId_t  mControllerTaskId = NULL;

static OSA_TASK_DEFINE(ControllerTask, gControllerTaskPriority_c, 1, gControllerTaskStackSize_c, FALSE);

/* Radio system clock selection. */
#if defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ == 1)
    const uint8_t gRfSysClk26MHz_c = 1;  /* 26MHz radio clock. */
#else
    const uint8_t gRfSysClk26MHz_c = 0;  /* 32MHz radio clock. */
#endif

/* Organizationally Unique Identifier used in BD_ADDR. */
const uint8_t gBD_ADDR_OUI_c[mBD_ADDR_OUIPartSize_c] = {BD_ADDR_OUI};
#ifndef CPU_QN908X
    /* BD_ADDR referenced in the controller */
    uint8_t gBD_ADDR[gcBleDeviceAddressSize_c];
#endif
/* Time between the beginning of two consecutive advertising PDU's */
const uint8_t gAdvertisingPacketInterval = mcAdvertisingPacketInterval_c;
/* Advertising channels that are enabled for scanning operation. */
const uint8_t gScanChannelMap_c = mcScanChannelMap_c;
/* Advertising channels that are enabled for initiator scanning operation. */
const uint8_t gInitiatorChannelMap_c = mcInitiatorChannelMap_c;
/* Offset to the first instant register */
const uint16_t gOffsetToFirstInstant_c = mcOffsetToFirstInstant_c;
/* Scan FIFO lockup detection interval in milliseconds. */
const uint32_t gScanFifoLockupCheckIntervalMilliSeconds = mScanFifoLockupCheckIntervalMilliSeconds_c;
/* Configurable maximum distance in slots of 625us for anchor point placement between connections */
const uint16_t gAnchorPointDistance = mAnchorPointDistance_c;
/* Configurable autonomous feature exchange */
const uint8_t gAutonomousFeatureExchange = gL1AutonomousFeatureExchange_d;
/* Configurable long range advertising primary and secondary coding schemes.
For primary advertising, bit 0 is used to select the coding scheme: 0 = S8, 1 = S2
For secondary advertising, bit 1 is used to select the coding scheme: 0 = S8, 1 = S2
*/
const uint8_t gLongRangeAdvCodingScheme = mLongRangeAdvCodingScheme_c;
/* Default value for the DTM 2 wire serial connection. Can be changed also by using Controller_SetDTMBaudrate defined in "controller_interface.h". */
const dtmBaudrate_t gDefaultDTMBaudrate = gDTM_BaudRate_115200_c;
#ifdef gMWS_Coex_UseRFNotAllowed_d
const uint8_t  gMWS_Coex_UseRFNotAllowed = gMWS_Coex_UseRFNotAllowed_d;
#else
const uint8_t  gMWS_Coex_UseRFNotAllowed = 0;
#endif

#ifdef gMWS_Coex_AccessGrantedByReq_d
const uint8_t  gMWS_Coex_AccessGrantedByReq = gMWS_Coex_AccessGrantedByReq_d;
#else
const uint8_t  gMWS_Coex_AccessGrantedByReq = 1;
#endif

#ifdef gMWS_CoexRfDenyActiveState_d
const uint8_t  gMWS_CoexRfDenyActiveState = gMWS_CoexRfDenyActiveState_d;
#else
const uint8_t  gMWS_CoexRfDenyActiveState = 1;
#endif

/* Place holder for storing allocated memory address in the link layer */
#define _block_size_         0 *
#define _number_of_blocks_ + 1 *
#define _eol_              + 0 +
#define _pool_id_(a)
const uint16_t gLeMemBufListSize = AppPoolsDetails_c 0;
void          *gLeMemBufList[AppPoolsDetails_c 0];


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#ifndef __WEAK_FUNC
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined( __CC_ARM )
#define __WEAK_FUNC __weak
#endif
#endif /* __WEAK_FUNC */

__WEAK_FUNC void Controller_InterruptHandlerWrapper(void)
{
    Controller_InterruptHandler();
}

static void ControllerTask(osaTaskParam_t argument)
{
    (void)Controller_TaskHandler((void *) NULL);
}

#ifndef CPU_QN908X
static void ControllerSetBD_ADDR(void)
{
    uint8_t sha256Output[SHA256_HASH_SIZE] = {0};
    uint8_t uid[mBoardUidSize_c] = {0};
    uint8_t macAddr[mBoardMacAddrSize_c] = {0};
    uint8_t len = 0;
    uint8_t i = 0;
    uint8_t all_00 = 1;
    uint8_t all_ff = 1;

    (void)NV_ReadHWParameters(&gHardwareParameters);

    /* Check BD_ADDR in HW Params */
    while ((i < gcBleDeviceAddressSize_c) && (gHardwareParameters.bluetooth_address[i] == 0xFFU)) { i++; }

    if (i == gcBleDeviceAddressSize_c)
    {
        /* Check MAC ADDRESS register */
        BOARD_GetMACAddr(macAddr);
        i = 0;

        while ((i < mBD_ADDR_RandPartSize_c) && ((all_00 != 0U) || (all_ff != 0U)))
        {
            if ((all_00 != 0U) && (macAddr[i] != 0U))
            {
                all_00 = 0U;
            }

            if ((all_ff != 0U) && (macAddr[i] != 0xFFU))
            {
                all_ff = 0U;
            }

            i++;
        }

        if ((all_00 == 0U) && (all_ff == 0U))
        {
            FLib_MemCpy(gHardwareParameters.bluetooth_address, macAddr, mBD_ADDR_RandPartSize_c);
        }
        else
        {
            /* Get MCUUID and create a SHA256 output */
            BOARD_GetMCUUid(uid, &len);
            SHA256_Hash(uid, len, sha256Output);
            FLib_MemCpy(gHardwareParameters.bluetooth_address, sha256Output, mBD_ADDR_RandPartSize_c);
        }

        FLib_MemCpy(&gHardwareParameters.bluetooth_address[mBD_ADDR_RandPartSize_c],
                    (const void *)gBD_ADDR_OUI_c, mBD_ADDR_OUIPartSize_c);
        (void)NV_WriteHWParameters(&gHardwareParameters);
    }

    FLib_MemCpy(gBD_ADDR, gHardwareParameters.bluetooth_address, gcBleDeviceAddressSize_c);
}
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
/**
 * \fn      Controller_TaskInit
 * \brief   This Function
 * \return  osa_status_t
 */
osaStatus_t Controller_TaskInit(void)
{
    /* Initialization of task related objects */
    if (NULL != mControllerTaskId)
    {
        return osaStatus_Error;
    }

    mControllerTaskEvent = OSA_EventCreate(TRUE);

    if (NULL == mControllerTaskEvent)
    {
        return osaStatus_Error;
    }

    /* Task creation */
    mControllerTaskId = OSA_TaskCreate(OSA_TASK(ControllerTask), NULL);

    if (NULL == mControllerTaskId)
    {
        panic(0, 0, 0, 0);
        return osaStatus_Error;
    }

#if (defined(CPU_MKW20Z160VHT4) || defined(CPU_MKW30Z160VHM4) || defined(CPU_MKW40Z160VHT4))
    OSA_InstallIntHandler((uint32_t)BTLL_RSIM_IRQn, &Controller_InterruptHandlerWrapper);

    NVIC_ClearPendingIRQ(BTLL_RSIM_IRQn);
    NVIC_EnableIRQ(BTLL_RSIM_IRQn);
    NVIC_SetPriority(BTLL_RSIM_IRQn, 0x80U >> (8U - (uint8_t)__NVIC_PRIO_BITS));

#elif (defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
    defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
    defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4))

    /* Select BLE protocol on RADIO0_IRQ */
    XCVR_MISC->XCVR_CTRL = (uint32_t)((XCVR_MISC->XCVR_CTRL & (uint32_t)~(uint32_t)(
                                           XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK
                                       )) | (uint32_t)(
                                          (0UL << XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_SHIFT)
                                      ));

    OSA_InstallIntHandler((uint32_t)Radio_0_IRQn, &Controller_InterruptHandlerWrapper);

    NVIC_ClearPendingIRQ(Radio_0_IRQn);
    NVIC_EnableIRQ(Radio_0_IRQn);
    NVIC_SetPriority(Radio_0_IRQn, 0x80U >> (8U - (uint8_t)__NVIC_PRIO_BITS));

#elif (defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
    defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))

    /* Select BLE protocol on RADIO0_IRQ */
    XCVR_MISC->XCVR_CTRL = (uint32_t)((XCVR_MISC->XCVR_CTRL & (uint32_t)~(uint32_t)(
                                           XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK
                                       )) | (uint32_t)(
                                          (0UL << XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_SHIFT)
                                      ));

    OSA_InstallIntHandler((uint32_t)Radio_0_IRQn, &Controller_InterruptHandlerWrapper);

    NVIC_ClearPendingIRQ(Radio_0_IRQn);
    NVIC_EnableIRQ(Radio_0_IRQn);
    NVIC_SetPriority(Radio_0_IRQn, 0x80U >> (8U - (uint8_t)__NVIC_PRIO_BITS));

#elif (defined(CPU_K32W032S1M2VPJ_cm0plus))
    OSA_InstallIntHandler((uint32_t)RF0_0_IRQn, &Controller_InterruptHandlerWrapper);

    NVIC_ClearPendingIRQ(RF0_0_IRQn);
    NVIC_EnableIRQ(RF0_0_IRQn);
    NVIC_SetPriority(RF0_0_IRQn, 0x80U >> (8U - (uint8_t)__NVIC_PRIO_BITS));

#elif (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || \
       defined (KW38Z4_SERIES) || defined(KW39A4_SERIES) )
    /* Ungate GENFSK+XCVR+BTELL */
    SIM->SCGC5 |= SIM_SCGC5_BTLL(1) | SIM_SCGC5_PHYDIG(1) | SIM_SCGC5_GEN_FSK(1) |
        SIM_SCGC5_RSIM(1) | SIM_SCGC5_LTC(1);
    RADIO_CTRL->LL_CTRL= 0x00000000;
  
    /* Select BLE protocol on RADIO0_IRQ */
    XCVR_MISC->XCVR_CTRL = (uint32_t)((XCVR_MISC->XCVR_CTRL & (uint32_t)~(uint32_t)(
                               XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL_MASK
                              )) | (
                               ((uint32_t)(0U) << XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL_SHIFT)
                              ));

    OSA_InstallIntHandler((uint32_t)Radio_0_IRQn, &Controller_InterruptHandlerWrapper);
    NVIC_ClearPendingIRQ(Radio_0_IRQn);
    
    /*Interrupts shall not be enabled here but later in pfd_finalize_init(). The reason is that
    at this step after a SW reset, the LL FW is not yet initilized properly.*/
#elif (defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
#else
#warning "No valid CPU defined!"
#endif

#ifndef CPU_QN908X
#if  !(defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || \
      defined(KW39A4_SERIES) || defined(CPU_MK64FN1M0VLL12))
    /* done in Controller_ParamInit for KW37 series */
    /* Set Default Tx Power Level */
    (void)Controller_SetTxPowerLevel(mAdvertisingDefaultTxPower_c, gAdvTxChannel_c);
    (void)Controller_SetTxPowerLevel(mConnectionDefaultTxPower_c, gConnTxChannel_c);
#endif
#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d)
    MWS_Register(gMWS_BLE_c, MWS_BLE_Callback);
#endif

#if defined(gMWS_UseCoexistence_d) && (gMWS_UseCoexistence_d)
    XCVR_CoexistenceInit();
  #if (gMWS_Coex_UseRFNotAllowed_d == 0)
    MWS_CoexistenceInit(&gCoexistence_RfDeny, NULL, NULL);
  #endif
  #if ((gMWS_Coex_AccessGrantedByReq_d == 0) && (gMWS_Coex_UseRFNotAllowed_d == 0))
    MWS_CoexistenceRegister(gMWS_BLE_c, MWS_COEX_BLE_Callback);
  #else
    MWS_COEX_BLE_Callback(gMWS_Init_c);
  #endif

#endif

    /* Configure BD_ADDR before calling Controller_Init */
    ControllerSetBD_ADDR();
#endif /* CPU_QN908X */

    return osaStatus_Success;
}

#if  (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || \
      defined(KW39A4_SERIES))
void Controller_ParamInit(void)
{
    /* Set Default Tx Power Level */
    (void)Controller_SetTxPowerLevelDbm(mAdvertisingDefaultTxPower_c, gAdvTxChannel_c);
    (void)Controller_SetTxPowerLevelDbm(mConnectionDefaultTxPower_c, gConnTxChannel_c);
}
#endif /* KW37A4_SERIES, KW37Z4_SERIES, KW38A4_SERIES, KW38Z4_SERIES, KW39A4_SERIES */

#ifdef CPU_QN908X
volatile uint8_t controllerEventFlag = 0;

void BLE_Semaphore_Give(void)
{
    if (0 == controllerEventFlag)
    {
        controllerEventFlag = 1;
        OSA_EventSet(mControllerTaskEvent, 1);
    }
}
#endif /* CPU_QN908X */

/*! *********************************************************************************
* @}
********************************************************************************** */
