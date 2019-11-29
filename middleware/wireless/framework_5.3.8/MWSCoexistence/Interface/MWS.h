/*! *********************************************************************************
* Copyright 2017 NXP
* All rights reserved.
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
#define gMWS_Enabled_d        0
#endif

#ifndef gMWS_UsePrioPreemption_d
#define gMWS_UsePrioPreemption_d  1
#endif

#ifndef gMWS_UseCoexistence_d
#define gMWS_UseCoexistence_d  0
#endif

/* Default priority values of the protocols */
#ifndef gMWS_BLEPriority_d
#define gMWS_BLEPriority_d    0
#endif

#ifndef gMWS_802_15_4Priority_d
#define gMWS_802_15_4Priority_d    1
#endif

#ifndef gMWS_GENFSKPriority_d
#define gMWS_GENFSKPriority_d    2
#endif

#ifndef gMWS_ANTPriority_d
#define gMWS_ANTPriority_d    3
#endif

/* Check for same priority values */
#if (gMWS_BLEPriority_d == gMWS_802_15_4Priority_d)    || (gMWS_BLEPriority_d == gMWS_GENFSKPriority_d)   || \
    (gMWS_BLEPriority_d == gMWS_ANTPriority_d)         || (gMWS_802_15_4Priority_d == gMWS_GENFSKPriority_d) || \
    (gMWS_802_15_4Priority_d == gMWS_ANTPriority_d)    || (gMWS_GENFSKPriority_d == gMWS_ANTPriority_d)
#error MWS protocol priority values should not be the same!
#endif

/* Coexistence signals timings */
#ifndef gMWS_CoexPrioSignalTime_d
#define gMWS_CoexPrioSignalTime_d   (40) /* us */
#endif
        
#ifndef gMWS_CoexRfActiveAssertTime_d 
#define gMWS_CoexRfActiveAssertTime_d (100) /* us */
#endif

#ifndef gMWS_CoexConfirmWaitTime_d
#define gMWS_CoexConfirmWaitTime_d (90) /* us. Note: Should be lower than gMWS_CoexRfActiveAssertTime_d! */
#endif

/* Coexistence RF Deny pin polarity: 1 - for active on logic one; 0 - active on logic zero */
#ifndef gMWS_CoexRfDenyActiveState_d
#define gMWS_CoexRfDenyActiveState_d (1)
#endif

#ifndef gMWS_CoexRxPrio_d
#define gMWS_CoexRxPrio_d (gMWS_HighPriority)
#endif

#ifndef gMWS_CoexTxPrio_d
#define gMWS_CoexTxPrio_d (gMWS_HighPriority)
#endif

/* Supported coexistence models */
#define gMWS_Coex_Status_Prio_d    0
#define gMWS_Coex_Prio_Only_d      1

/* Coexistence model to be handled */
#ifndef gMWS_Coex_Model_d
#define gMWS_Coex_Model_d    gMWS_Coex_Prio_Only_d
#endif

/* Coexistence FSCI configuration */
#ifndef gMWS_FsciEnabled_d
#define gMWS_FsciEnabled_d           (0)
#endif

/* Default FSCI interface used */
#ifndef gMWS_FsciInterface_d
#define gMWS_FsciInterface_d         (0)
#endif

/* Operation Groups */
#define gMWS_FsciReqOG_d             (0xC0)
#define gMWS_FsciCnfOG_d             (0xC1)

/* Commands */
#define mFsciMsgCoexEnableReq_c      (0x01) /* Fsci-CoexEnable.Request.  */
#define mFsciMsgCoexDisableReq_c     (0x02) /* Fsci-CoexDisable.Request. */
#define mFsciMsgCoexRequestAccessReq_c  (0x03) /* Fsci-CoexRequestAccess.Request.  */
#define mFsciMsgCoexReleaseAccessReq_c  (0x04) /* Fsci-CoexReleaseAccess.Request.  */


#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
#define gMWS_RF_ACTIVE_PORT_d      gpioPort_C_c
#define gMWS_RF_ACTIVE_PIN_d       1
#define gMWS_RF_PRIORITY_PORT_d    gpioPort_C_c
#define gMWS_RF_PRIORITY_PIN_d     4
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
}mwsProtocols_t;

typedef enum
{
  gMWS_Success_c,
  gMWS_Denied_c,
  gMWS_InvalidParameter_c,
  gMWS_Error_c
}mwsStatus_t;

typedef enum
{
    gMWS_Init_c,
    gMWS_Idle_c,
    gMWS_Active_c,
    gMWS_Release_c,
    gMWS_Abort_c,
    gMWS_GetInactivityDuration_c
}mwsEvents_t;

typedef enum
{
    gMWS_IdleState_c,
    gMWS_RxState_c,
    gMWS_TxState_c
}mwsRfState_t;

typedef enum
{
    gMWS_LowPriority,
    gMWS_HighPriority
}mwsRfSeqPriority_t;


#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
typedef enum
{
  gMWS_PinInterruptFallingEdge_c,
  gMWS_PinInterruptRisingEdge_c,
  gMWS_PinInterruptEitherEdge_c  
} mwsPinInterruptMode_t;
#endif // gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d

typedef uint32_t ( *pfMwsCallback ) ( mwsEvents_t event );

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

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
mwsStatus_t MWS_Register    (mwsProtocols_t protocol, pfMwsCallback cb);

/*! *********************************************************************************
* \brief  This function try to acquire access to the XCVR for the specified protocol
*
* \param[in]  protocol - One of the supported MWS protocols
* \param[in]  force    - If set, the active protocol will be preempted
*
* \return  If access to the XCVR is not granted, gMWS_Denied_c is returned.
*
********************************************************************************** */
mwsStatus_t MWS_Acquire     (mwsProtocols_t protocol, uint8_t force);

/*! *********************************************************************************
* \brief  This function will release access to the XCVR, and will notify other 
*         protocols that the resource is idle.
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_Release     (mwsProtocols_t protocol);

/*! *********************************************************************************
* \brief  This function will notify other protocols that the specified protocol is 
*         Idle and the XCVR is unused.
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_SignalIdle  (mwsProtocols_t protocol);

/*! *********************************************************************************
* \brief  Force the active protocol to Idle state.
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_Abort       (void);

/*! *********************************************************************************
* \brief  This function will poll all other protocols for their inactivity period,
*         and will return the minimum time until the first protocol needs to be serviced.
*
* \param[in]  currentProtocol - One of the supported MWS protocols
*
* \return  the minimum inactivity duration (in microseconds)
*
********************************************************************************** */
uint32_t MWS_GetInactivityDuration (mwsProtocols_t currentProtocol);

/*! *********************************************************************************
* \brief  Returns the protocol that is currently using the XCVR
*
* \return  One of the supported MWS protocols
*
********************************************************************************** */
mwsProtocols_t MWS_GetActiveProtocol (void);


/*! *********************************************************************************
* \brief  Initialize the MWS External Coexistence driver
*
* \param[in]  rfDenyPin   - Pointer to the GPIO input pin used to signal RF access 
*                           granted/denied. Set to NULL if controlled by hardware.
* \param[in]  rfActivePin - Pointer to the GPIO output pin used to signal RF activity
*                           Set to NULL if controlled by hardware.
* \param[in]  rfStatusPin - Pointer to the GPIO output pin to signal the RF activyty type
*                           Set to NULL if controlled by hardware.
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_CoexistenceInit(void *rfDenyPin, void *rfActivePin, void *rfStatusPin);

/*! *********************************************************************************
* \brief  This function will register a protocol stack into MWS Coexistence module
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \param[in]  cb       - The callback function used by the MWS Coexistence to signal 
*                        events to the protocol stack
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_CoexistenceRegister (mwsProtocols_t protocol, pfMwsCallback cb);

/*! *********************************************************************************
* \brief  This function returns the state of RF Deny pin.
*
* \return  uint8_t !gMWS_CoexRfDenyActiveState_d - RF Deny Inactive,
*                   gMWS_CoexRfDenyActiveState_d - RF Deny Active
*
********************************************************************************** */
uint8_t MWS_CoexistenceDenyState(void);

/*! *********************************************************************************
* \brief  This function will register a protocol stack into MWS Coexistence module
*
* \param[in]  rxPrio - The priority of the RX sequence
* \param[in]  txPrio - The priority of the TX sequence
*
********************************************************************************** */
void MWS_CoexistenceSetPriority(mwsRfSeqPriority_t rxPrio, mwsRfSeqPriority_t txPrio);

/*! *********************************************************************************
* \brief  Request for permission to access the medium for the specified RF sequence
*
* \param[in]  newState - The RF sequence type
*
* \return  If RF access is not granted, gMWS_Denied_c is returned.
*
********************************************************************************** */
mwsStatus_t MWS_CoexistenceRequestAccess(mwsRfState_t newState);

/*! *********************************************************************************
* \brief  This function will signal the change of the RF state, and request for permission
*
* \param[in]  newState - The new state in which the XCVR will transition
*
* \return  If RF access is not granted, gMWS_Denied_c is returned.
*
********************************************************************************** */
mwsStatus_t MWS_CoexistenceChangeAccess(mwsRfState_t newState);

/*! *********************************************************************************
* \brief  Signal externally that the Radio is not using the medium anymore.
*
********************************************************************************** */
void MWS_CoexistenceReleaseAccess(void);

/*! *********************************************************************************
* \brief  Enable Coexistence signals.
*
********************************************************************************** */
void MWS_CoexistenceEnable (void);

/*! *********************************************************************************
* \brief  Disable Coexistence signals.
*
********************************************************************************** */
void MWS_CoexistenceDisable (void);

/*! *********************************************************************************
* \brief  MWS FSCI message handler
*
* \param[in]  pData - pointer to data message 
* \param[in]  param - pointer to additional parameters (if any)
* \param[in]  fsciInterface - FSCI interface used
*
* \return  None
*
********************************************************************************** */
#if gMWS_FsciEnabled_d
void MWS_FsciMsgHandler( void* pData, void* param, uint32_t fsciInterface );
/*! *********************************************************************************
* \brief  Initialization function that registers MWS opcode group and the 
*         corresponding message handler
*
* \return  None
*
********************************************************************************** */

void MWS_FsciInit( void );
#endif

#endif /* _MWS_H_ */