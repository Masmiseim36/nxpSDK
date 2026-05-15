/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCUX_CONFIG_H_
#define _MCUX_CONFIG_H_

#define CONFIG_CPU_CORTEX_M 1
#define CONFIG_FLASH_BASE_ADDRESS 0x04000000
#define CONFIG_FLASH_DEFAULT_APP_OFFSET 0x800000
#define CONFIG_HAS_FLASH_LOAD_OFFSET 1
#define CONFIG_FLASH_LOAD_OFFSET 0x800000
#define CONFIG_UART_RETRY_TIMES 0
// #define CONFIG_FLASH_DRIVER_EXECUTES_FROM_RAM 0
// #define CONFIG_ENABLE_QUICKACCESS_SECTION_IN_XSPI_DRIVER 0
// #define CONFIG_STREAM_FLASH 0
#define LWIP_OPT_H_CODE_LINE_1 void sys_lock_tcpip_core(void);
#define LWIP_OPT_H_CODE_LINE_2 void sys_unlock_tcpip_core(void);
#define LWIP_OPT_H_CODE_LINE_3 void sys_check_core_locking(void);
#define LWIP_OPT_H_CODE_LINE_4 void sys_mark_tcpip_thread(void);
#define LWIP_OPT_H_CODE_LINE_5 
#define LWIP_DEFINE_LWIP_RAND_IN_CC_H 1
// #define LIB_JPEG_USE_HW_ACCEL 0
// #define USE_PNGDEC_DRIVER 0
// #define CONFIG_BOOT_CUSTOM_DEVICE_SETUP 0
// #define CONFIG_FREERTOS_USE_CUSTOM_CONFIG_FRAGMENT 0
// #define MPU_SUPPORT 0
// #define USE_PERCEPIO_TRACELYZER 0

#endif /* _MCUX_CONFIG_H_ */
