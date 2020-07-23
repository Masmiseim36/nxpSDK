/*! *********************************************************************************
* Copyright 2017 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the Mobile Wireless Standard Interface.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include "MWS.h"
#include "fsl_os_abstraction.h"
#include "GPIO_Adapter.h"
#include "fsl_common.h"

#if gMWS_UseCoexistence_d
#include "TimersManager.h"
#endif

#if gMWS_FsciEnabled_d
#include "FsciInterface.h"
#include "MemManager.h"
#endif

#ifdef CPU_QN908X
#include "controller_interface.h"
#include "fsl_iocon.h"
#endif
#if (gMWS_UseCoexistence_d) && (!gTimestamp_Enabled_d)
#warning The MWS Coexistence uses the Timestamp service. Please enable the TMR Timestamp (gTimestamp_Enabled_d).
#endif


/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
#if gMWS_Enabled_d || gMWS_UseCoexistence_d
/*! *********************************************************************************
* \brief  MWS helper function used for chaining the active protocols, ordered by priority
*
* \param[in]  protocol - the protocol
* \param[in]  priority - the priority to be set to above protocol
*
* \return  None
*
********************************************************************************** */
static void MWS_SetPriority (mwsProtocols_t protocol, uint8_t priority);
#endif

#if gMWS_UseCoexistence_d
/*! *********************************************************************************
* \brief  Interrupt Service Routine for handling the changes of the RF Deny pin
*
* \return  None
*
********************************************************************************** */
static void rf_deny_pin_changed(void);
#endif

#if gMWS_FsciEnabled_d
/*! *********************************************************************************
* \brief  Initialization function that registers MWS opcode group and the
*         corresponding message handler
*
* \return  None
*
********************************************************************************** */
void MWS_FsciInit (void);

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
void MWS_FsciMsgHandler (void* pData, void* param, uint32_t fsciInterface);
#endif

#if (gMWS_UseCoexistence_d == 1) 
/*! *********************************************************************************
* \brief  This function is used to enable the RF_DENY pin interrupt
*
* \param[in]  none
*
* \return  None
*
********************************************************************************** */
static void MWS_EnableRfDenyPinInterrupt(void);

/*! *********************************************************************************
* \brief  This function is used to disable the RF_DENY pin interrupt
*
* \return  None
*
********************************************************************************** */
static void MWS_DisableRfDenyPinInterrupt(void);
#endif


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if gMWS_Enabled_d || gMWS_UseCoexistence_d
/* Stores the id of the protocol with the Highest priority */
static mwsProtocols_t  mFirstMwsPrio = gMWS_None_c;
/* Stores the priority level for every protocol id */
static uint8_t mProtocolPriority[gMWS_None_c] =
{
    gMWS_BLEPriority_d,
    gMWS_802_15_4Priority_d,
    gMWS_ANTPriority_d,
    gMWS_GENFSKPriority_d
};
/* Stores the id of the protocol with the next priority */
static mwsProtocols_t mProtocolNextPrio[gMWS_None_c] =
{
    gMWS_None_c, gMWS_None_c, gMWS_None_c, gMWS_None_c
};
#endif

#if gMWS_Enabled_d
/* Stores the id of the protocol which uses the XCVR */
static mwsProtocols_t mActiveProtocol = gMWS_None_c;
/* Stores MWS callback functions for every protocol */
static pfMwsCallback mMwsCallbacks[gMWS_None_c] = {NULL, NULL, NULL, NULL};
static uint32_t mwsLockCount = 0;
#endif

#if gMWS_UseCoexistence_d
/* Assume that the Coexistence GPIO pins are controlled by hardware */
static gpioInputPinConfig_t  *rf_deny    = NULL;
static gpioOutputPinConfig_t *rf_active  = NULL;
static gpioOutputPinConfig_t *rf_status  = NULL;
static mwsRfState_t mXcvrRfState;
static uint8_t mCoexFlags;
static uint8_t mCoexEnabled;
/* Stores Coexistence callback functions for every protocol */
static pfMwsCallback mCoexCallbacks[gMWS_None_c] = {NULL, NULL, NULL, NULL};
static volatile bool_t RfRegActive = FALSE;

#if gMWS_FsciEnabled_d
    /* NEEDS CITATION: AP needs to send beacons every 100ms, therefore the slot time cannot be
       greater than 1ms: if Wi-Fi duty cycle is set to the minimum 1%, then Thread has 99ms and AP
       is still able to send a buffered beacon in the 100th millisecond. */
    static const uint8_t    mCoexRequestAccessSlotTimeMs = 1;  // milliseconds
    static uint64_t         mCoexRequestAccessTimestamp;
    static uint8_t          mCoexRequestAccessSeconds;
    static uint8_t          mCoexRequestAccessDutyCycle;
    static tmrTimerID_t     mCoexRequestAccessTimerID = gTmrInvalidTimerID_c;
#endif

#endif

/*** Callback example:

uint32_t protocolCallback ( mwsEvents_t event )
{
    uint32_t status = 0;

    switch(event)
    {
    case gMWS_Init_c:
        status = protocolInittFunction();
        break;
    case gMWS_Active_c:
        status = protocolSetActiveFunction();
        break;
    case gMWS_Abort_c:
        status = protocolAbortFunction();
        break;
    case gMWS_Idle_c:
        status = protocolNotifyIdleFunction();
        break;
    case gMWS_GetInactivityDuration_c:
        status = protocolGetInactiveDurationFunction();
        break;
    default:
        status = gMWS_InvalidParameter_c;
        break;
    }
    return status;
}
*/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

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
mwsStatus_t MWS_Register (mwsProtocols_t protocol, pfMwsCallback cb)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d
    static uint8_t initialized = 0;

    if( 0U == initialized )
    {
        mActiveProtocol = gMWS_None_c;
        mFirstMwsPrio = gMWS_None_c;
        mwsLockCount = 0;
        initialized = 1;
    }

    if( (protocol >= gMWS_None_c) || (NULL == cb) )
    {
        status = gMWS_InvalidParameter_c;
    }
    else if( NULL == mMwsCallbacks[protocol] )
    {
        mMwsCallbacks[protocol] = cb;
        MWS_SetPriority(protocol, mProtocolPriority[protocol]);
        (void)cb( gMWS_Init_c ); /* Signal the protocol */
    }
    else
    {
        /* Already registered! Only update callback */
        mMwsCallbacks[protocol] = cb;
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief  This function will return the registered callback for a protocol
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \return  pointer to the registered callback function. NULL if no callback is registered
*
********************************************************************************** */
pfMwsCallback MWS_GetRegisteredCallback(mwsProtocols_t protocol)
{
    pfMwsCallback cb = NULL;
#if gMWS_Enabled_d
    if (protocol < gMWS_None_c)
    {
        cb = mMwsCallbacks[protocol];
    }
#endif
    return cb;
}

/*! *********************************************************************************
* \brief  This function will poll all other protocols for their inactivity period,
*         and will return the minimum time until the first protocol needs to be serviced.
*
* \param[in]  currentProtocol - One of the supported MWS protocols
*
* \return  the minimum inactivity duration (in microseconds)
*
********************************************************************************** */
uint32_t MWS_GetInactivityDuration (mwsProtocols_t currentProtocol)
{
    uint32_t duration = 0xFFFFFFFFU;
#if gMWS_Enabled_d
    uint32_t i, temp;

    /* Get the minimum inactivity duration from all protocols */
    for (i=0; i<NumberOfElements(mMwsCallbacks); i++)
    {
        if( (i != (uint32_t)currentProtocol) && (NULL != mMwsCallbacks[i]) )
        {
            temp = mMwsCallbacks[i](gMWS_GetInactivityDuration_c);
            if( temp < duration )
            {
                duration = temp;
            }
        }
    }
#endif
    return duration;
}

/*! *********************************************************************************
* \brief  This function try to acquire access to the XCVR for the specified protocol
*
* \param[in]  protocol - One of the supported MWS protocols
* \param[in]  force    - If set, the active protocol will be preempted
*
* \return  If access to the XCVR is not granted, gMWS_Denied_c is returned.
*
********************************************************************************** */
mwsStatus_t MWS_Acquire (mwsProtocols_t protocol, uint8_t force)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d

    OSA_InterruptDisable();

    if (gMWS_None_c == mActiveProtocol)
    {
        mActiveProtocol = protocol;
        mwsLockCount = 1;
        (void)mMwsCallbacks[mActiveProtocol](gMWS_Active_c);
    }
    else if( mActiveProtocol == protocol )
    {
        mwsLockCount++;
    }
    else
    {
        /* Lower value means higher priority */
        if( (0U != force)
#if gMWS_UsePrioPreemption_d
           || (mProtocolPriority[mActiveProtocol] > mProtocolPriority[protocol])
#endif
          )
        {
            status = (mwsStatus_t)mMwsCallbacks[mActiveProtocol](gMWS_Abort_c);
            mActiveProtocol = protocol;
            mwsLockCount = 1;
            (void)mMwsCallbacks[mActiveProtocol](gMWS_Active_c);
        }
        else
        {
            status = gMWS_Denied_c;
        }
    }

    OSA_InterruptEnable();
#endif
    return status;
}

/*! *********************************************************************************
* \brief  This function will release access to the XCVR, and will notify other
*         protocols that the resource is idle.
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_Release (mwsProtocols_t protocol)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d

    if( mActiveProtocol != gMWS_None_c )
    {
        if (protocol == mActiveProtocol)
        {
            mwsLockCount--;

            if( 0U == mwsLockCount )
            {
                (void)mMwsCallbacks[mActiveProtocol](gMWS_Release_c);
                mActiveProtocol = gMWS_None_c;

                /* Notify other protocols that XCVR is Idle, based on the priority */
                status = MWS_SignalIdle(protocol);
            }
        }
        else
        {
            /* Another protocol is using the XCVR */
            status = gMWS_Denied_c;
        }
    }
    else
    {
        status = MWS_SignalIdle(protocol);
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief  Force the active protocol to Idle state.
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_Abort (void)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d

    if( mActiveProtocol != gMWS_None_c )
    {
        if( 0U != mMwsCallbacks[mActiveProtocol](gMWS_Abort_c) )
        {
            status = gMWS_Error_c;
        }
        mActiveProtocol = gMWS_None_c;
        mwsLockCount = 0;
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief  Returns the protocol that is currently using the XCVR
*
* \return  One of the supported MWS protocols
*
********************************************************************************** */
mwsProtocols_t MWS_GetActiveProtocol (void)
{
#if gMWS_Enabled_d
    return mActiveProtocol;
#else
    return gMWS_None_c;
#endif
}

/*! *********************************************************************************
* \brief  This function will notify other protocols that the specified protocol is
*         Idle and the XCVR is unused.
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \return  mwsStatus_t
*
********************************************************************************** */
mwsStatus_t MWS_SignalIdle  (mwsProtocols_t protocol)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d
    uint32_t i = (uint32_t)mFirstMwsPrio;

    while( (uint32_t)gMWS_None_c != i )
    {
        if( mActiveProtocol != gMWS_None_c )
        {
            break;
        }

        if( (uint32_t)protocol != i )
        {
            assert(mMwsCallbacks[i]);
            if( 0U != mMwsCallbacks[i](gMWS_Idle_c) )
            {
                status = gMWS_Error_c;
            }
        }
        i = (uint32_t)mProtocolNextPrio[i];
    }
#endif
    return status;
}

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
mwsStatus_t MWS_CoexistenceInit(void *rfDenyPin, void *rfActivePin, void *rfStatusPin)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_UseCoexistence_d
    static uint8_t initialized = 0;

    rf_active = (gpioOutputPinConfig_t*)rfActivePin;
    rf_status = (gpioOutputPinConfig_t*)rfStatusPin;
    rf_deny   = (gpioInputPinConfig_t*)rfDenyPin;

    if( rf_active )
    {
        GpioOutputPinInit(rf_active, 1);
    }

    if( rf_status )
    {
        GpioOutputPinInit(rf_status, 1);
    }

    if( rf_deny )
    {
        GpioInputPinInit(rf_deny, 1);
    }

    if( !initialized )
    {
        initialized = 1;

        MWS_CoexistenceSetPriority(gMWS_CoexRxPrio_d, gMWS_CoexTxPrio_d);
        MWS_CoexistenceReleaseAccess();
        MWS_CoexistenceEnable();
        TMR_TimeStampInit();

        /* Check if the RF Confirm signal must be handled by Software */
        if( rf_deny )
        {
          MWS_DisableRfDenyPinInterrupt();
#if (gMWS_Coex_UseRFNotAllowed_d == 0)          
          if( gpio_success != GpioInstallIsr(rf_deny_pin_changed, gGpioIsrPrioNormal_c, gGpioDefaultNvicPrio_c >> 1, rf_deny) )
          {
              status = gMWS_InvalidParameter_c;
          }
#endif          
        }

#if gMWS_FsciEnabled_d
        MWS_FsciInit();
#endif
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief  Enable Coexistence signals.
*
********************************************************************************** */
void MWS_CoexistenceEnable (void)
{
#ifdef CPU_QN908X
    IOCON_PinMuxSet(IOCON, 0, 10, IOCON_FUNC6); //BLE_TX indicator, active high
    IOCON_PinMuxSet(IOCON, 0, 11, IOCON_FUNC6); //BLE_RX indicator, active high
    
    /* BLE Core slot timing reference - optional
       Can be routed to PA18 and PA26 */
    // IOCON_PinMuxSet(IOCON, 0, 18, IOCON_FUNC6); //BLE_SYNC, pluse
    // IOCON_PinMuxSet(IOCON, 0, 26, IOCON_FUNC6); //BLE_SYNC, pluse
    
    /* BLE Core is processing an event - optional
       Can be routed to PA19 and PA27. !!! CAREFULL with PA19 is it also used by SW2 button !!!  */
    // IOCON_PinMuxSet(IOCON, 0, 19, IOCON_FUNC6); //BLE_IN_PROC, active high.
    // IOCON_PinMuxSet(IOCON, 0, 27, IOCON_FUNC6); //BLE_IN_PROC, active high
    
    /* Priority pins - optional */
    // IOCON_PinMuxSet(IOCON, 0, 6, IOCON_FUNC5); //BLE_PTI[0]
    // IOCON_PinMuxSet(IOCON, 0, 7, IOCON_FUNC5); //BLE_PTI[1]
    // IOCON_PinMuxSet(IOCON, 0, 8, IOCON_FUNC5); //BLE_PTI[2]
    // IOCON_PinMuxSet(IOCON, 0, 9, IOCON_FUNC5); //BLE_PTI[3]
    
    /* WLAN signaling input pins - optional */
    // IOCON_PinMuxSet(IOCON, 0, 0, IOCON_FUNC6); //WLAN Tx, active high, input
    // IOCON_PinMuxSet(IOCON, 0, 1, IOCON_FUNC6); //WLAN_rx, active high, input
    
    BLE_EnableWlanCoex();
#else /* CPU_QN908X */
#if gMWS_UseCoexistence_d
    OSA_InterruptDisable();
    mCoexEnabled = 1;
    OSA_InterruptEnable();
#endif
#endif /* CPU_QN908X */
}

/*! *********************************************************************************
* \brief  Disable Coexistence signals.
*
********************************************************************************** */
void MWS_CoexistenceDisable (void)
{
#if gMWS_UseCoexistence_d
    OSA_InterruptDisable();
    MWS_CoexistenceReleaseAccess();
    mCoexEnabled = 0;
    OSA_InterruptEnable();
#endif
}

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
mwsStatus_t MWS_CoexistenceRegister (mwsProtocols_t protocol, pfMwsCallback cb)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_UseCoexistence_d

    if( (protocol >= gMWS_None_c) || (NULL == cb) )
    {
        status = gMWS_InvalidParameter_c;
    }
    else if( NULL == mCoexCallbacks[protocol] )
    {
        mCoexCallbacks[protocol] = cb;
        MWS_SetPriority(protocol, mProtocolPriority[protocol]);
        cb( gMWS_Init_c ); /* Signal the protocol */

        if( rf_deny )
        {
            if( GpioReadPinInput(rf_deny) == gMWS_CoexRfDenyActiveState_d )
            {
                cb(gMWS_Abort_c);
            }
            else
            {
                cb(gMWS_Idle_c);
            }
        }
    }
    else
    {
        /* Already registered! Only update callback */
        mCoexCallbacks[protocol] = cb;
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief  This function will return the registered MWS Coexistence callback for a protocol
*
* \param[in]  protocol - One of the supported MWS protocols
*
* \return  pointer to the registered callback function. NULL if no callback is registered
*
********************************************************************************** */
pfMwsCallback MWS_CoexistenceGetRegisteredCallback(mwsProtocols_t protocol)
{
    pfMwsCallback cb = NULL;
#if gMWS_UseCoexistence_d
    if (protocol < gMWS_None_c)
    {
        cb = mCoexCallbacks[protocol];
    }
#endif
    return cb;
}

/*! *********************************************************************************
* \brief  This function returns the state of RF Deny pin.
*
* \return  uint8_t !gMWS_CoexRfDenyActiveState_d - RF Deny Inactive,
*                   gMWS_CoexRfDenyActiveState_d - RF Deny Active
*
********************************************************************************** */
uint8_t MWS_CoexistenceDenyState(void)
{
    uint8_t retVal = (uint8_t)(!((bool_t)gMWS_CoexRfDenyActiveState_d));
#if gMWS_UseCoexistence_d
    if( rf_deny )
    {
        if( GpioReadPinInput(rf_deny) == gMWS_CoexRfDenyActiveState_d )
        {
            retVal = gMWS_CoexRfDenyActiveState_d;
        }
    }
#endif
    return retVal;
}

/*! *********************************************************************************
* \brief  This function will register a protocol stack into MWS Coexistence module
*
* \param[in]  rxPrio - The priority of the RX sequence
* \param[in]  txPrio - The priority of the TX sequence
*
********************************************************************************** */
void MWS_CoexistenceSetPriority(mwsRfSeqPriority_t rxPrio, mwsRfSeqPriority_t txPrio)
{
#if gMWS_UseCoexistence_d
    OSA_InterruptDisable();

    if( rxPrio == gMWS_HighPriority )
    {
        mCoexFlags |= (1 << gMWS_RxState_c);
    }
    else
    {
        mCoexFlags &= ~(1 << gMWS_RxState_c);
    }

    if( txPrio == gMWS_HighPriority )
    {
        mCoexFlags |= 1 << gMWS_TxState_c;
    }
    else
    {
        mCoexFlags &= ~(1 << gMWS_TxState_c);
    }

    OSA_InterruptEnable();
#endif
}

/*! *********************************************************************************
* \brief  Request for permission to access the medium for the specified RF sequence
*
* \param[in]  newState - The RF sequence type
*
* \return  If RF access is not granted, gMWS_Denied_c is returned.
*
********************************************************************************** */
mwsStatus_t MWS_CoexistenceRequestAccess(mwsRfState_t newState)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_UseCoexistence_d
    uint64_t timestamp;

    if( mCoexEnabled )
    {
        OSA_InterruptDisable();

        timestamp = TMR_GetTimestamp();

        /* Set Priority signal */
        if( (rf_status) && (mCoexFlags & (1 << newState)) )
        {
            GpioSetPinOutput(rf_status);
        }

        /* Signal that protocol is about to become active */
        if( rf_active )
        {
            GpioSetPinOutput(rf_active);
        }

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        /* wait 50us */
        while( (TMR_GetTimestamp() - timestamp) < gMWS_CoexPrioSignalTime_d ) {;};

        /* Set RF sequence type: RX/TX */
        if( rf_status )
        {
            if( newState == gMWS_RxState_c )
            {
                /* Set status line LOW to signal RX sequence */
                GpioClearPinOutput(rf_status);
            }
            else
            {
                /* Set status line HIGH to signal TX sequence */
                GpioSetPinOutput(rf_status);
            }
        }
#endif // gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d

        OSA_InterruptEnable();

        /* Wait for confirm signal */
        if( rf_deny )
        {
            status = gMWS_Denied_c; /* assume access is denied */

            do
            {
                if(GpioReadPinInput(rf_deny) != gMWS_CoexRfDenyActiveState_d)
                {
                  /* PTA granted the access */
                  status = gMWS_Success_c;
                  RfRegActive = TRUE;
                  mXcvrRfState = newState;
#if (gMWS_Coex_UseRFNotAllowed_d == 0)                  
                  MWS_EnableRfDenyPinInterrupt();
#endif
                    break;
                }
            }
            while( (TMR_GetTimestamp() - timestamp) < gMWS_CoexConfirmWaitTime_d );

            if( status != gMWS_Success_c )
            {
                MWS_CoexistenceReleaseAccess();
            }
        }
    }
#endif // gMWS_UseCoexistence_d
    return status;
}

/*! *********************************************************************************
* \brief  This function will signal the change of the RF state, and request for permission
*
* \param[in]  newState - The new state in which the XCVR will transition
*
* \return  If RF access is not granted, gMWS_Denied_c is returned.
*
********************************************************************************** */
mwsStatus_t MWS_CoexistenceChangeAccess(mwsRfState_t newState)
{
    mwsStatus_t status = gMWS_Success_c;

#if gMWS_UseCoexistence_d
    if( mCoexEnabled )
    {
        if( mXcvrRfState == gMWS_IdleState_c )
        {
            status = gMWS_Denied_c;
        }
        else
        {
            mXcvrRfState = newState;
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
            if( rf_status )
            {
                if( newState == gMWS_RxState_c )
                {
                    /* Set status line LOW to signal RX sequence */
                    GpioClearPinOutput(rf_status);
                }
                else
                {
                    /* Set status line HIGH to signal TX sequence */
                    GpioSetPinOutput(rf_status);
                }
            }
#endif
        }
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief  Signal externally that the Radio is not using the medium anymore.
*
********************************************************************************** */
void MWS_CoexistenceReleaseAccess(void)
{
#if gMWS_UseCoexistence_d
    if( mCoexEnabled && RfRegActive)
    {
    /* Disable the RF_DENY pin interrupt */
#if (gMWS_Coex_UseRFNotAllowed_d == 0)      
    MWS_DisableRfDenyPinInterrupt();
#endif
        OSA_InterruptDisable();
        mXcvrRfState = gMWS_IdleState_c;

        if( rf_active )
        {
            GpioClearPinOutput(rf_active);
        }

        if( rf_status )
        {
            GpioClearPinOutput(rf_status);
        }

        RfRegActive = FALSE;
        OSA_InterruptEnable();
    }
#endif
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function is used to enable the RF_DENY pin interrupt
*
* \param[in]  mode - pin interrupt mode to be set (rising edge, falling edge, etc)
*
* \return  None
*
********************************************************************************** */
#if (gMWS_UseCoexistence_d == 1) 
static void MWS_EnableRfDenyPinInterrupt()
{
  if(rf_deny)
  {
    GpioSetPinInterrupt(rf_deny->gpioPort, rf_deny->gpioPin, rf_deny->interruptSelect);
  }
}

/*! *********************************************************************************
* \brief  This function is used to disable the RF_DENY pin interrupt
*
* \return  None
*
********************************************************************************** */
static void MWS_DisableRfDenyPinInterrupt(void)
{
  if(rf_deny)
  {
    GpioSetPinInterrupt(rf_deny->gpioPort, rf_deny->gpioPin, pinInt_Disabled_c);
  }
}
#endif

/*! *********************************************************************************
* \brief  MWS helper function used for chaining the active protocols, ordered by priority
*
* \param[in]  protocol - the protocol
* \param[in]  priority - the priority to be set to the above protocol
*
* \return  None
*
********************************************************************************** */
#if gMWS_Enabled_d || gMWS_UseCoexistence_d
static void MWS_SetPriority (mwsProtocols_t protocol, uint8_t priority)
{
    mwsProtocols_t i;

    if( (mFirstMwsPrio == gMWS_None_c) || (priority <= mProtocolPriority[mFirstMwsPrio]) )
    {
        /* Insert at the begining of the list */
        mProtocolNextPrio[protocol] = mFirstMwsPrio;
        mFirstMwsPrio = protocol;
    }
    else
    {
        i = mFirstMwsPrio;

        while( i != gMWS_None_c )
        {
            if( mProtocolNextPrio[i] == gMWS_None_c )
            {
                /* Insert at the end of the list */
                mProtocolNextPrio[protocol] = gMWS_None_c;
                mProtocolNextPrio[i] = protocol;
                i = gMWS_None_c;
            }
            else if (priority <= mProtocolPriority[mProtocolNextPrio[i]])
            {
                mProtocolNextPrio[protocol] = mProtocolNextPrio[i];
                mProtocolNextPrio[i] = protocol;
                i = gMWS_None_c;
            }
            else
            {
                i = mProtocolNextPrio[i];
            }
        }
    }
}
#endif

/*! *********************************************************************************
* \brief  Interrupt Service Routine for handling the changes of the RF Deny pin
*
* \return  None
*
********************************************************************************** */
#if gMWS_UseCoexistence_d
static void rf_deny_pin_changed(void)
{
    uint32_t i;

    GpioClearPinIntFlag(rf_deny);

    if( mCoexEnabled )
    {
        if( GpioReadPinInput(rf_deny) == gMWS_CoexRfDenyActiveState_d )
        {
            /* Abort all protocols */
            for( i=0; i<gMWS_None_c; i++ )
            {
                if( mCoexCallbacks[i] )
                {
                    mCoexCallbacks[i](gMWS_Abort_c);
                }
            }
        }
        else
        {
            i = mFirstMwsPrio;

            while( i != gMWS_None_c )
            {
                if( NULL != mCoexCallbacks[i] )
                {
                    mCoexCallbacks[i](gMWS_Idle_c);
                }
                i = mProtocolNextPrio[i];
            }
        }
    }
}
#endif /* gMWS_UseCoexistence_d */

/*! *********************************************************************************
* \brief  Initialization function that registers MWS opcode group and the
*         corresponding message handler
*
* \return  None
*
********************************************************************************** */
#if gMWS_FsciEnabled_d
void MWS_FsciInit( void )
{
    FSCI_RegisterOpGroup(gMWS_FsciReqOG_d,
                         gFsciMonitorMode_c,
                         MWS_FsciMsgHandler,
                         NULL,
                         gMWS_FsciInterface_d);
}
#endif

/*! *********************************************************************************
* \brief  Callback function that implements the Wi-Fi duty cycle.
*
* \return  None
*
********************************************************************************** */
#if gMWS_FsciEnabled_d
static void MWS_FsciCoexistenceRequestAccessHandler
(
    void *param
)
{
    uint64_t timestamp = TMR_GetTimestamp();

    /* release access */
    MWS_CoexistenceReleaseAccess();

    /* if seconds == 0 -> run indefinitely */
    if (mCoexRequestAccessSeconds != 0)
    {
        /* stop timer if we got out of time */
        if ((timestamp - mCoexRequestAccessTimestamp) > TmrSecondsToMicroseconds(mCoexRequestAccessSeconds))
        {
            TMR_FreeTimer(mCoexRequestAccessTimerID);
            mCoexRequestAccessTimerID = gTmrInvalidTimerID_c;
            return;
        }
    }

    /* wait for Wi-Fi duty_cycle% * slot time milliseconds */
    OSA_TimeDelay(mCoexRequestAccessDutyCycle * mCoexRequestAccessSlotTimeMs);

    /* request access */
    if (MWS_CoexistenceRequestAccess(gMWS_RxState_c) != gMWS_Success_c)
    {
        /* inform host over FSCI if error */
        clientPacket_t packet = { 0 };
        packet.structured.header.opGroup = gMWS_FsciCnfOG_d;
        packet.structured.header.opCode = mFsciMsgCoexRequestAccessReq_c;
        packet.structured.header.len = 1;
        packet.structured.payload[0] = gFsciError_c;
        FSCI_transmitFormatedPacket(&packet, gMWS_FsciInterface_d);
    }
}
#endif /* gMWS_FsciEnabled_d */

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
#if gFsciIncluded_c && gMWS_FsciEnabled_d
void MWS_FsciMsgHandler( void* pData, void* param, uint32_t fsciInterface )
{
    gFsciStatus_t status = gFsciSuccess_c;

    switch(((clientPacket_t*)pData)->structured.header.opCode)
    {
    case mFsciMsgCoexEnableReq_c:
        MWS_CoexistenceEnable();
        break;

    case mFsciMsgCoexDisableReq_c:
        MWS_CoexistenceDisable();
        break;

    case mFsciMsgCoexRequestAccessReq_c:

        /* CoEX request will be handled on timer */
        if (mCoexRequestAccessTimerID == gTmrInvalidTimerID_c)
        {
            mCoexRequestAccessTimerID = TMR_AllocateTimer();
        }
        else
        {
            TMR_StopTimer(mCoexRequestAccessTimerID);
        }

        mCoexRequestAccessTimestamp = TMR_GetTimestamp();
        mCoexRequestAccessSeconds = ((clientPacket_t *)pData)->structured.payload[0];
        mCoexRequestAccessDutyCycle = ((clientPacket_t *)pData)->structured.payload[1];

        if (mCoexRequestAccessDutyCycle >= 100)
        {
            status = gFsciTooBig_c;
        }
        else
        {
            /* start interval timer each 100 * slots */
            TMR_StartIntervalTimer(
                mCoexRequestAccessTimerID,
                mCoexRequestAccessSlotTimeMs * 100,
                MWS_FsciCoexistenceRequestAccessHandler,
                NULL
            );
        }

        break;

    case mFsciMsgCoexReleaseAccessReq_c:
        if (TMR_IsTimerActive(mCoexRequestAccessTimerID))
        {
            TMR_FreeTimer(mCoexRequestAccessTimerID);
            mCoexRequestAccessTimerID = gTmrInvalidTimerID_c;
        }

        MWS_CoexistenceReleaseAccess();
        break;

    default:
        status = gFsciUnknownOpcode_c;
        FSCI_Error( gFsciUnknownOpcode_c, fsciInterface );
        break;
    }
    
    if( gFsciUnknownOpcode_c == status )
    {
        MEM_BufferFree(pData);
    }
    else
    {
        /* Reuse the received message */
        ((clientPacket_t*)pData)->structured.header.opGroup = gMWS_FsciCnfOG_d;
        ((clientPacket_t*)pData)->structured.header.len = 1;
        ((clientPacket_t*)pData)->structured.payload[0] = status;
        FSCI_transmitFormatedPacket((clientPacket_t*)pData, fsciInterface);
    }
}
#endif
