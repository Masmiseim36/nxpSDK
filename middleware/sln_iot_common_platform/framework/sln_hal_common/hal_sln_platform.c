/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief framework platform dependency implementation.
 */

#include "sln_time.h"

/*
 * Get the current time in us.
 */
unsigned int sln_current_time_us()
{
    unsigned int timeValue = Time_Current();
    unsigned int timeUnit  = Time_Unit();
    return (timeValue * timeUnit);
}
