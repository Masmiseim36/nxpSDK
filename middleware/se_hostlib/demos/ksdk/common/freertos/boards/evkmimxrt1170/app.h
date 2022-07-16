/* Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* For MCU Integration */


#define BOARD_InitHardware() \
    BOARD_ConfigMPU(); \
    BOARD_InitBootPins();    \
    BOARD_BootClockRUN(); \
    BOARD_InitDebugConsole()
