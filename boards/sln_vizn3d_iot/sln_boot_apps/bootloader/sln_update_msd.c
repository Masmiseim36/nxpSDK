/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_update.h"

#if BOOTLOADER_MSD_ENABLED
#include "disk.h"
#endif /* BOOTLOADER_MSD_ENABLED */

bool SLN_CheckForUpdate(void)
{
    bool update = false;

#if BOOTLOADER_MSD_ENABLED
    update = CheckForMSDMode();
#endif /* BOOTLOADER_MSD_ENABLED */

    return update;
}
