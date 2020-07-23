/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "autobaud/autobaud.h"
#include "microseconds.h"
#include "bootloader_common.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

enum _autobaud_counts
{
    //! the number of falling edge transitions being counted
    //! for 0x5A
    kFirstByteRequiredFallingEdges = 4u,
    //! the number of falling edge transitions being counted
    //! for 0xA6
    kSecondByteRequiredFallingEdges = 3u,
    //! the number of bits being measured for the baud rate
    //! for 0x5A we have the start bit + 7 bits to the last falling edge = 8 bits
    kNumberOfBitsForFirstByteMeasured = 8u,
    //! for 0xA6 we have the start bit + 6 bits to the last falling edge = 7 bits
    kNumberOfBitsForSecondByteMeasured = 7u,
    //! Time in microseconds that we will wait in between toggles before restarting detection
    //! Make this value 8 bits at 100 baud worth of time = 80000 microseconds
    kMaximumTimeBetweenFallingEdges = 80000u,
    //! Autobaud baud step size that our calculation will be rounded to, this is to ensure
    //! that we can use a valid multiplier in the UART configuration which runs into problems
    //! at higher baud rates with slightly off baud rates (e.g. if we measure 115458 vs 115200)
    //! a calculation of UartClock at 48MHz gives an SBR calculation of 48000000/ (16 * 115458) = 25
    //! giving a baud rate calculation of 48000000/(16 * 25) = 120000 baud
    //! which is out of spec and fails
    kAutobaudStepSize = 1200u
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void instance_transition_callback(uint32_t instance);
// This function is used to filter invalid noises on LPUART RX pins.
static bool is_falling_edge_interval_valid(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static uint32_t s_transitionCount;
static uint64_t s_ticksAtFirstFallingEdge;
static uint64_t s_firstByteTotalTicks;
static uint64_t s_secondByteTotalTicks;
static uint64_t s_lastToggleTicks;
static uint64_t s_ticksBetweenFailure;
static uint32_t s_instanceMeasured;
static uint32_t s_ticksFallingEdges[kFirstByteRequiredFallingEdges + kSecondByteRequiredFallingEdges];
static uint32_t s_intervalFallingEdgesFirstByte[kFirstByteRequiredFallingEdges - 1u];
static uint32_t s_intervalFallingEdgesSecondByte[kSecondByteRequiredFallingEdges - 1u];

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void autobaud_init(uint32_t instance)
{
    s_transitionCount = 0;
    s_firstByteTotalTicks = 0;
    s_secondByteTotalTicks = 0;
    s_lastToggleTicks = 0;
    s_instanceMeasured = 0;
    s_ticksBetweenFailure = microseconds_convert_to_ticks((uint32_t)kMaximumTimeBetweenFallingEdges);
    enable_autobaud_pin_irq(instance, instance_transition_callback);
}

void autobaud_deinit(uint32_t instance)
{
    disable_autobaud_pin_irq(instance);
}

/*
 *   This function filters the invalid noises on LPUART rx pins by following characters:
 *   For 0x5A, the interval between falling edges meets 3:3:2
 *   For 0xA5, the interval between falling edges meets: 4:3
 *      |              0x5A                 |                 0xA6              |
 *
 *   ___       ___     _____     ___     ______       _____       ___     ________
 *      | S 0 | 1 | 0 | 1 1 | 0 | 1 | 0 | STOP | S 0 | 1 1 | 0 0 | 1 | 0 | 1  STOP
 *       -----     ---       ---     ---        -----       -----     ---
 *   ---------------------------------------------------------------------------
 *      |    1st  |   2nd   |  3rd  |          |    1st    |    2nd  |
 *   ---------------------------------------------------------------------------
 *      |     3   :     3   :   2   |          |     4     :     3   |
 *
 *  NOTE:
 *       There are still some noises which cannot be filtered by this function due to algorithm limitation.
 *       1st byte: 0x5B, 0x53, 0x52 can pass this check, and will be treated as 0x5A.
 *       2nd byte: 0xA4,0xA5,0xA7, 0xB4,0xB5,0xB6,0xB7 can pass this check. and will be treated as 0xA6.
 *       This function is able to filter up to 99.95% noises.
 */
static bool is_falling_edge_interval_valid(void)
{
    bool retValue = true;

    // Calculate intervals between falling edges for 0x5A
    for (uint32_t i = 0u; i < (uint32_t)kFirstByteRequiredFallingEdges - 1u; i++)
    {
        s_intervalFallingEdgesFirstByte[i] = s_ticksFallingEdges[i + 1u] - s_ticksFallingEdges[i];
    }

    // Calculate intervals between falling edges for 0xA6
    for (uint32_t i = 0u; i < (uint32_t)kSecondByteRequiredFallingEdges - 1u; i++)
    {
        s_intervalFallingEdgesSecondByte[i] = s_ticksFallingEdges[(uint32_t)kFirstByteRequiredFallingEdges + 1u + i] -
                                              s_ticksFallingEdges[(uint32_t)kFirstByteRequiredFallingEdges + i];
    }

    // Ensure the intervals for 0x5A meet above characters.
    // 3rd Interval : 1st Interval = 2:3
    uint32_t tmp = s_intervalFallingEdgesFirstByte[2] * 100u;
    uint32_t div = tmp / s_intervalFallingEdgesFirstByte[0];
    // For PL2303, there is a large jitter at the first falling edge.
    // So expand the check range. No such issue for the left edges.
    if ((div < 60u) || (div > 73u))
    {
        retValue = false;
    }
    else
    {
        // 3rd interval : 2nd Interval = 2:3
        div = tmp / s_intervalFallingEdgesFirstByte[1];
        // baud rate tolerance should be -+3%
        // 2 / 3 * 100 * 0.97 = 64.667
        // 2 / 3 * 100 * 1.03 = 68.667
        if ((div < 64u) || (div > 69u))
        {
            retValue = false;
        }
        else
        {
            // Ensure the intervals for 0xA6 meet above characters.
            // 1st Interval : 2nd Interval = 4:3
            tmp = s_intervalFallingEdgesSecondByte[0] * 100u;
            div = tmp / s_intervalFallingEdgesSecondByte[1];
            // baud rate tolerance should be -+3%
            // 4 / 3 * 100 * 0.97 = 129.33
            // 2 / 3 * 100 * 1.03 = 137.33
            if ((div < 129u) || (div > 138u))
            {
                retValue = false;
            }
            else
            {
                // 1st interval for 0xA6 : 1st interval for 0x5a = 4:3
                // For PL2303, there is a large jitter at the first falling edge.
                // So uses s_intervalFallingEdgesFirstByte[1] instead of
                // s_intervalFallingEdgesFirstByte[0]
                div = tmp / s_intervalFallingEdgesFirstByte[1];
                // baud rate tolerance should be -+3%
                // 4 / 3 * 100 * 0.97 = 129.33
                // 2 / 3 * 100 * 1.03 = 137.33
                if ((div < 129u) || (div > 138u))
                {
                    retValue = false;
                }
            }
        }
    }

    return retValue;
}

status_t autobaud_get_rate(uint32_t instance, uint32_t *rate)
{
    status_t retStatus = (int32_t)kStatus_Success;
    if ((s_transitionCount == ((uint32_t)kFirstByteRequiredFallingEdges + (uint32_t)kSecondByteRequiredFallingEdges)) &&
        (instance == s_instanceMeasured))
    {
        // Invalid bytes received.
        if (!is_falling_edge_interval_valid())
        {
            // Restart auto buad detection
            autobaud_init(instance);
            retStatus = (int32_t)kStatus_Fail;
        }
        else
        {
            uint32_t calculatedBaud =
                (microseconds_get_clock() * ((uint32_t)kNumberOfBitsForFirstByteMeasured + (uint32_t)kNumberOfBitsForSecondByteMeasured)) /
                (uint32_t)(s_firstByteTotalTicks + s_secondByteTotalTicks);

            // Round the rate to the nearest step size
            // rounded = stepSize * (value/stepSize + .5)
            // multiplying by 10 since we can't work with floats
            *rate = (((((uint32_t)calculatedBaud * 10u) / (uint32_t)kAutobaudStepSize) + 5u) / 10u) * (uint32_t)kAutobaudStepSize;
        }
    }
    else
    {
        // no baud rate yet/inactive
        retStatus = (int32_t)kStatus_Fail;
    }

    return retStatus;
}

void instance_transition_callback(uint32_t instance)
{
    uint64_t ticks = microseconds_get_ticks();
    s_transitionCount++;

    uint64_t delta = ticks - s_lastToggleTicks;

    // The last toggle was longer than we allow so treat this as the first one
    if (delta > s_ticksBetweenFailure)
    {
        s_transitionCount = 1;
    }

    if (s_transitionCount == 1u)
    {
        // This is our first falling edge, store the initial ticks temporarily in firstByteTicks
        // and save the instance that we are measuring
        s_ticksAtFirstFallingEdge = ticks;
        s_firstByteTotalTicks = ticks;
        s_instanceMeasured = instance;
    }
    else if (s_transitionCount == (uint32_t)kFirstByteRequiredFallingEdges)
    {
        // We reached the end of our measurable first byte, subtract the current ticks from the initial
        // first byte ticks
        s_firstByteTotalTicks = ticks - s_firstByteTotalTicks;
    }
    else if (s_transitionCount == ((uint32_t)kFirstByteRequiredFallingEdges + 1u))
    {
        // We hit our first falling edge of the second byte, store the initial ticks temporarily in secondByteTicks
        s_secondByteTotalTicks = ticks;
    }
    else if (s_transitionCount == ((uint32_t)kFirstByteRequiredFallingEdges + (uint32_t)kSecondByteRequiredFallingEdges))
    {
        // We reached the end of our measurable second byte, subtract the current ticks from the initial
        // second byte ticks
        s_secondByteTotalTicks = ticks - s_secondByteTotalTicks;
        disable_autobaud_pin_irq(instance);
    }
    else
    {
        // by default, there is no process here.
    }

    s_ticksFallingEdges[s_transitionCount - 1u] = (uint32_t)(ticks - s_ticksAtFirstFallingEdge);

    s_lastToggleTicks = ticks;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
