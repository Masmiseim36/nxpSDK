/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef BLE_INIT_H
#define BLE_INIT_H

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "board.h"

#if (defined(gUseHciTransportDownward_d) && (gUseHciTransportDownward_d == 1)) || \
    (defined(gUseHciTransportUpward_d) && (gUseHciTransportUpward_d == 1))
#include "hci_transport.h"
#endif

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
#include "controller_interface.h"
#if  !(defined(CPU_QN908X) || defined(CPU_MK64FN1M0VLL12))
#if (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
#include "nxp2p4_xcvr.h"
#else
#include "fsl_xcvr.h"
#endif
#endif /* CPU_QN908X */
#endif /* gUseHciTransportDownward_d */

/************************************************************************************
*************************************************************************************
* Public macros - Do not modify directly! Override in app_preinclude.h if needed.
*************************************************************************************
************************************************************************************/
#ifndef gBleXcvrInitRetryCount_c
#define gBleXcvrInitRetryCount_c (10U)
#endif

#ifndef CPU_QN908X
#if (!defined(KW37A4_SERIES) && !defined(KW37Z4_SERIES) && !defined(KW38A4_SERIES) && !defined(KW38Z4_SERIES) && !defined(KW39A4_SERIES))
extern bool_t gEnableSingleAdvertisement;
extern bool_t gMCUSleepDuringBleEvents;
#endif
#endif /* CPU_QN908X */

/* This enables/disables the HCI Reset command sent by the Host at init sequence
   Default value is disabled, only for gUseHciTransportDownward_d is required */
#ifndef gHostInitResetController_c
    #if defined(gUseHciTransportDownward_d) && gUseHciTransportDownward_d
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
#if !defined(gUseHciTransportUpward_d) || (!gUseHciTransportUpward_d)
extern bool_t gHostInitResetController;
#endif
#if (defined(KW37A4_SERIES) ||defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
extern uint16_t gRfRadioVer_c;
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  Performs full initialization of the BLE stack.
*
* \param[in] genericCallback  Callback used by the Host Stack to propagate GAP generic
* events to the application.
*
* \return  gBleSuccess_c or error.
*
* \remarks The gInitializationComplete_c generic GAP event is triggered on completion.
*
********************************************************************************** */
bleResult_t Ble_Initialize
(
    gapGenericCallback_t gapGenericCallback
);

#endif /* BLE_INIT_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
