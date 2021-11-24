/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_push_buttons_driver.h"

__attribute__((weak)) status_t PUSH_BUTTONS_Init(void)
{
    /* Initialize device's push buttons */
    return kStatus_Success;
}

__attribute__((weak)) bool PUSH_BUTTONS_MSDPressed(void)
{
    /* Check if the button that triggers the MSD is pressed */
    return false;
}

__attribute__((weak)) bool PUSH_BUTTONS_OTWPressed(void)
{
    /* Check if the button that triggers the FWUPDATE OTW is pressed */
    return false;
}