/*! *********************************************************************************
 * Copyright 2017-2021,2025 NXP
 *
 * \file
 *
 * This is the header file for the Mobile Wireless Standard interface.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _MWS_H_
#define _MWS_H_

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

#ifndef gMWS_Enabled_d
#define gMWS_Enabled_d 1
#endif

#ifndef gMWS_UsePrioPreemption_d
#define gMWS_UsePrioPreemption_d 1
#endif

/* Default priority values of the protocols */
#ifndef gMWS_BLEPriority_d
#define gMWS_BLEPriority_d 0
#endif

#ifndef gMWS_802_15_4Priority_d
#define gMWS_802_15_4Priority_d 1
#endif

#ifndef gMWS_GENFSKPriority_d
#define gMWS_GENFSKPriority_d 2
#endif

#ifndef gMWS_ANTPriority_d
#define gMWS_ANTPriority_d 3
#endif

/* Check for same priority values */
#if (gMWS_BLEPriority_d == gMWS_802_15_4Priority_d) || (gMWS_BLEPriority_d == gMWS_GENFSKPriority_d) || \
    (gMWS_BLEPriority_d == gMWS_ANTPriority_d) || (gMWS_802_15_4Priority_d == gMWS_GENFSKPriority_d) || \
    (gMWS_802_15_4Priority_d == gMWS_ANTPriority_d) || (gMWS_GENFSKPriority_d == gMWS_ANTPriority_d)
#error MWS protocol priority values should not be the same!
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
    gMWS_BLE_c,
    gMWS_802_15_4_c,
    gMWS_ANT_c,
    gMWS_GENFSK_c,
    gMWS_None_c /*! must be the last item */
} mwsProtocols_t;

typedef enum
{
    gMWS_Success_c,
    gMWS_Denied_c,
    gMWS_InvalidParameter_c,
    gMWS_Error_c
} mwsStatus_t;

typedef enum
{
    gMWS_Init_c,
    gMWS_Idle_c,
    gMWS_Active_c,
    gMWS_Release_c,
    gMWS_Abort_c,
    gMWS_GetInactivityDuration_c
} mwsEvents_t;

typedef enum
{
    gMWS_IdleState_c,
    gMWS_RxState_c,
    gMWS_TxState_c
} mwsRfState_t;

typedef enum
{
    gMWS_LowPriority,
    gMWS_HighPriority
} mwsRfSeqPriority_t;

typedef enum
{
    gMWS_PinInterruptFallingEdge_c,
    gMWS_PinInterruptRisingEdge_c,
    gMWS_PinInterruptEitherEdge_c
} mwsPinInterruptMode_t;

typedef uint32_t (*pfMwsCallback)(mwsEvents_t event);

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
/*! *********************************************************************************
 * \brief  This function inits MWS module
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_Init(void);

/*! *********************************************************************************
 * \brief  This function will register a protocol stack into MWS
 *
 * \param[in]  protocol - One of the supported MWS protocols
 * \param[in]  cb       - The callback function used by the MWS to signal events to
 *                        the protocol stack
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_Register(mwsProtocols_t protocol, pfMwsCallback cb);

/*! *********************************************************************************
 * \brief  This function will return the registered MWS callback for a protocol
 *
 * \param[in]  protocol - One of the supported MWS protocols
 *
 * \return  pointer to the registered callback function. NULL if no callback is registered
 *
 ********************************************************************************** */
pfMwsCallback MWS_GetRegisteredCallback(mwsProtocols_t protocol);

/*! *********************************************************************************
 * \brief  This function try to acquire access to the XCVR for the specified protocol
 *
 * \param[in]  protocol - One of the supported MWS protocols
 * \param[in]  force    - If set, the active protocol will be preempted
 *
 * \return  If access to the XCVR is not granted, gMWS_Denied_c is returned.
 *
 ********************************************************************************** */
mwsStatus_t MWS_Acquire(mwsProtocols_t protocol, uint8_t force);

/*! *********************************************************************************
 * \brief  This function will release access to the XCVR, and will notify other
 *         protocols that the resource is idle.
 *
 * \param[in]  protocol - One of the supported MWS protocols
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_Release(mwsProtocols_t protocol);

/*! *********************************************************************************
 * \brief  This function will notify other protocols that the specified protocol is
 *         Idle and the XCVR is unused.
 *
 * \param[in]  protocol - One of the supported MWS protocols
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_SignalIdle(mwsProtocols_t protocol);

/*! *********************************************************************************
 * \brief  Force the active protocol to Idle state.
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_Abort(void);

/*! *********************************************************************************
 * \brief  This function will poll all other protocols for their inactivity period,
 *         and will return the minimum time until the first protocol needs to be serviced.
 *
 * \param[in]  currentProtocol - One of the supported MWS protocols
 *
 * \return  the minimum inactivity duration (in microseconds)
 *
 ********************************************************************************** */
uint32_t MWS_GetInactivityDuration(mwsProtocols_t currentProtocol);

/*! *********************************************************************************
 * \brief  Returns the protocol that is currently using the XCVR
 *
 * \return  One of the supported MWS protocols
 *
 ********************************************************************************** */
mwsProtocols_t MWS_GetActiveProtocol(void);

#endif /* _MWS_H_ */
