/*
 * Copyright 2018, 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file bootloader.h
 * @brief Bootloader Main
 */

#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

/* Includes required by the FreeRTOS timers structure. */
#include "FreeRTOS.h"
#include "timers.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/**
 * @brief ReRun Bootloader function.
 *
 * Restarts the Bootloader, soft reset.
 */
void ReRunBootloader();

/**
 * @brief Bootloader Main function.
 *
 * Execute the primary Bootloader main functions. Cycles LEDs to alert user it is running,
 * detects if the user is pressing a button for MSD, to start Factory or Main Application OTA, or
 * do a factory reset.
 */
void BootloaderMain(void *args);

/* _BOOTLOADER_H_ */
#endif
