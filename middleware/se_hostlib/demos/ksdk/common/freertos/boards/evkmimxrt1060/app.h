/* Copyright 2020 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* For MCU Integration */


#define BOARD_InitHardware() \
    BOARD_InitPins();    \
    BOARD_BootClockRUN(); \
    BOARD_InitDebugConsole()
