/*! *********************************************************************************
 * Copyright 2017,2025 NXP
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
#include "fsl_common.h"
#include "fsl_os_abstraction.h"

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
#if gMWS_Enabled_d
/*! *********************************************************************************
 * \brief  MWS helper function used for chaining the active protocols, ordered by priority
 *
 * \param[in]  protocol - the protocol
 * \param[in]  priority - the priority to be set to above protocol
 *
 * \return  None
 *
 ********************************************************************************** */
static void MWS_SetPriority(mwsProtocols_t protocol, uint8_t priority);
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if gMWS_Enabled_d
/* Stores the id of the protocol with the Highest priority */
static mwsProtocols_t mFirstMwsPrio = gMWS_None_c;
/* Stores the priority level for every protocol id */
static uint8_t mProtocolPriority[gMWS_None_c] = {gMWS_BLEPriority_d, gMWS_802_15_4Priority_d, gMWS_ANTPriority_d,
                                                 gMWS_GENFSKPriority_d};
/* Stores the id of the protocol with the next priority */
static mwsProtocols_t mProtocolNextPrio[gMWS_None_c] = {gMWS_None_c, gMWS_None_c, gMWS_None_c, gMWS_None_c};

/* Stores the id of the protocol which uses the XCVR */
static volatile mwsProtocols_t mActiveProtocol = gMWS_None_c;
/* Stores MWS callback functions for every protocol */
static pfMwsCallback     mMwsCallbacks[gMWS_None_c] = {NULL, NULL, NULL, NULL};
static volatile uint32_t mwsLockCount               = 0;
#endif

static uint8_t initialized = 0;

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
 * \brief  This function inits MWS module
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_Init(void)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d

    if (0U == initialized)
    {
        initialized     = 1;
        mActiveProtocol = gMWS_None_c;
        mFirstMwsPrio   = gMWS_None_c;
        mwsLockCount    = 0;
    }
#endif
    return status;
}

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
mwsStatus_t MWS_Register(mwsProtocols_t protocol, pfMwsCallback cb)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d
    if (0U == initialized)
    {
        status = MWS_Init();
    }
    if (status == gMWS_Success_c)
    {
        if ((protocol >= gMWS_None_c) || (NULL == cb))
        {
            status = gMWS_InvalidParameter_c;
        }
        else if (NULL == mMwsCallbacks[protocol])
        {
            mMwsCallbacks[protocol] = cb;
            MWS_SetPriority(protocol, mProtocolPriority[protocol]);
            (void)cb(gMWS_Init_c); /* Signal the protocol */
        }
        else
        {
            /* Already registered! Only update callback */
            mMwsCallbacks[protocol] = cb;
        }
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
uint32_t MWS_GetInactivityDuration(mwsProtocols_t currentProtocol)
{
    uint32_t duration = 0xFFFFFFFFU;
#if gMWS_Enabled_d
    uint32_t i, temp;

    /* Get the minimum inactivity duration from all protocols */
    for (i = 0; i < NumberOfElements(mMwsCallbacks); i++)
    {
        if ((i != (uint32_t)currentProtocol) && (NULL != mMwsCallbacks[i]))
        {
            temp = mMwsCallbacks[i](gMWS_GetInactivityDuration_c);
            if (temp < duration)
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
mwsStatus_t MWS_Acquire(mwsProtocols_t protocol, uint8_t force)
{
    mwsStatus_t status = gMWS_Success_c;

#if gMWS_Enabled_d
    /* remote protocol is not using the XCVR
     * So now we can check local protocols */
    OSA_InterruptDisable();

    if (gMWS_None_c == mActiveProtocol)
    {
        mActiveProtocol = protocol;
        mwsLockCount    = 1;
        (void)mMwsCallbacks[mActiveProtocol](gMWS_Active_c);
    }
    else if (mActiveProtocol == protocol)
    {
        mwsLockCount++;
    }
    else
    {
        mwsProtocols_t active_protocol = mActiveProtocol;
        /* Lower value means higher priority */
        if ((0U != force)
#if gMWS_UsePrioPreemption_d
            || (mProtocolPriority[active_protocol] > mProtocolPriority[protocol])
#endif
        )
        {
            status = MWS_Abort();
            if (status == gMWS_Success_c)
            {
                mActiveProtocol = protocol;
                mwsLockCount    = 1;
                (void)mMwsCallbacks[protocol](gMWS_Active_c);
            }
            else
            {
                status = gMWS_Denied_c;
            }
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
mwsStatus_t MWS_Release(mwsProtocols_t protocol)
{
    mwsStatus_t status = gMWS_Success_c;

#if gMWS_Enabled_d
    OSA_InterruptDisable();

    if (mActiveProtocol != gMWS_None_c)
    {
        if (protocol == mActiveProtocol)
        {
            mwsLockCount--;

            if (0U == mwsLockCount)
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

    OSA_InterruptEnable();
#endif
    return status;
}

/*! *********************************************************************************
 * \brief  Force the active protocol to Idle state.
 *
 * \return  mwsStatus_t
 *
 ********************************************************************************** */
mwsStatus_t MWS_Abort(void)
{
    mwsStatus_t status = gMWS_Success_c;

#if gMWS_Enabled_d
    if (mActiveProtocol != gMWS_None_c)
    {
        /* if the active protocol is local, send abort event to callback */
        if (0U != mMwsCallbacks[mActiveProtocol](gMWS_Abort_c))
        {
            status = gMWS_Error_c;
        }
        else
        {
            mActiveProtocol = gMWS_None_c;
            mwsLockCount    = 0;
        }
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
mwsProtocols_t MWS_GetActiveProtocol(void)
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
mwsStatus_t MWS_SignalIdle(mwsProtocols_t protocol)
{
    mwsStatus_t status = gMWS_Success_c;
#if gMWS_Enabled_d
    uint32_t i = (uint32_t)mFirstMwsPrio;

    while ((uint32_t)gMWS_None_c != i)
    {
        if (mActiveProtocol != gMWS_None_c)
        {
            break;
        }

        if ((uint32_t)protocol != i)
        {
            assert(mMwsCallbacks[i]);
            if (0U != mMwsCallbacks[i](gMWS_Idle_c))
            {
                status = gMWS_Error_c;
            }
        }
        i = (uint32_t)mProtocolNextPrio[i];
    }
#endif
    return status;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  MWS helper function used for chaining the active protocols, ordered by priority
 *
 * \param[in]  protocol - the protocol
 * \param[in]  priority - the priority to be set to the above protocol
 *
 * \return  None
 *
 ********************************************************************************** */
#if gMWS_Enabled_d
static void MWS_SetPriority(mwsProtocols_t protocol, uint8_t priority)
{
    mwsProtocols_t i;

    if ((mFirstMwsPrio == gMWS_None_c) || (priority <= mProtocolPriority[mFirstMwsPrio]))
    {
        /* Insert at the begining of the list */
        mProtocolNextPrio[protocol] = mFirstMwsPrio;
        mFirstMwsPrio               = protocol;
    }
    else
    {
        i = mFirstMwsPrio;

        while (i != gMWS_None_c)
        {
            if (mProtocolNextPrio[i] == gMWS_None_c)
            {
                /* Insert at the end of the list */
                mProtocolNextPrio[protocol] = gMWS_None_c;
                mProtocolNextPrio[i]        = protocol;
                i                           = gMWS_None_c;
            }
            else if (priority <= mProtocolPriority[mProtocolNextPrio[i]])
            {
                mProtocolNextPrio[protocol] = mProtocolNextPrio[i];
                mProtocolNextPrio[i]        = protocol;
                i                           = gMWS_None_c;
            }
            else
            {
                i = mProtocolNextPrio[i];
            }
        }
    }
}
#endif
