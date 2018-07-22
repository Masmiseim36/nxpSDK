/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "event.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get event instance.
 * @param eventType The event type
 * @return The event instance's pointer.
 */
static volatile uint32_t *EVENT_GetInstance(event_t eventType);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Transfer complete event. */
static volatile uint32_t g_eventTransferComplete;
/*! @brief Card detect event. */
static volatile uint32_t g_eventCardDetect;

/*! @brief Time variable unites as milliseconds. */
static volatile uint32_t g_timeMilliseconds;

/*******************************************************************************
 * Code
 ******************************************************************************/

static volatile uint32_t *EVENT_GetInstance(event_t eventType)
{
    volatile uint32_t *event;

    switch (eventType)
    {
        case kEVENT_TransferComplete:
            event = &g_eventTransferComplete;
            break;
        case kEVENT_CardDetect:
            event = &g_eventCardDetect;
            break;
        default:
            event = NULL;
            break;
    }

    return event;
}

bool EVENT_Create(event_t eventType)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        *event = 0;
        return true;
    }
    else
    {
        return false;
    }
}

bool EVENT_Wait(event_t eventType, uint32_t timeoutMilliseconds)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        do
        {
        } while (*event == 0U);
        *event = 0U;

        return true;
    }
    else
    {
        return false;
    }
}

bool EVENT_Notify(event_t eventType)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        *event = 1U;
        return true;
    }
    else
    {
        return false;
    }
}

void EVENT_Delete(event_t eventType)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        *event = 0U;
    }
}
