/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 *
 * Created by: NXP IoT Solutions Team.
 */

/*
 * @brief  board type define for all needed macros, please all place here before compiling.
 *
 */

#ifndef BOARD_DEFINE_H_
#define BOARD_DEFINE_H_

#define AT_CACHEABLE_SECTION_ALIGN_OCRAM(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_OCRAM_CACHED,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_OCRAM_NCACHED,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

/*********************************
  BOOTLOADER-SPECIFIC DEFINES
**********************************/

#ifndef BOOTLOADER_MSD_ENABLED
#define BOOTLOADER_MSD_ENABLED 0
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*
 * Debug console definition
 */
#define DEBUG_CONSOLE_UART_INDEX 12
#define ENABLE_CSI_3DCAMERA      1
#define ENABLE_MIPI_3DCAMERA     0

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
