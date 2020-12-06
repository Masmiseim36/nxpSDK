/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

#ifndef __SLN_SHELL_H_
#define __SLN_SHELL_H_

#include "fsl_common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "fsl_shell.h"
#include "fsl_str.h"

#include "clock_config.h"
#include "pin_mux.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

#include "serial_manager.h"
#include "serial_port_usb.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"
#include "usb_phy.h"

#include "event_groups.h"
#include "fsl_debug_console.h"
#include "usb_device_config.h"

#define CONTROLLER_ID kSerialManager_UsbControllerEhci0

/* Offset to the 30th MB + (1 sector) of Flash, having 256KB sectors */
#define USB_LOGGING_FLAG_FLASH_OFFSET 0xF200
#define USB_LOGGING_FLAG_ADDRESS      (USB_LOGGING_FLAG_FLASH_OFFSET * FLASH_PAGE_SIZE)

/* Offset to the 30th MB + (2 sectors) of Flash, having 256KB sectors */
#define HARDFAULT_LOGS_HISTORY_FLASH_OFFSET 0xF400
#define HARDFAULT_LOGS_HISTORY_ADDRESS      (HARDFAULT_LOGS_HISTORY_FLASH_OFFSET * FLASH_PAGE_SIZE)

/* Offset to the 30th MB + (3 sectors) of Flash, having 256KB sectors */
#define HARDFAULT_LOGS_HISTORY_SIZE_FLASH_OFFSET 0xF600
#define HARDFAULT_LOGS_HISTORY_SIZE_ADDRESS      (HARDFAULT_LOGS_HISTORY_SIZE_FLASH_OFFSET * FLASH_PAGE_SIZE)

#define MAX_LOG_HISTORY 20

typedef enum __shell_event
{
#if USE_WIFI_CONNECTION
    PRINT_EVENT = (1 << 0U),
    ERASE_EVENT = (1 << 1U),
    SETUP_EVENT = (1 << 2U),
#endif
    RESET_EVENT        = (1 << 3U),
    EXIT_EVENT         = (1 << 4U),
    MEM_VIEW_EVENT     = (1 << 5U),
    ENA_HEAP_TRACE_EVT = (1 << 6U),
    DIS_HEAP_TRACE_EVT = (1 << 7U),
    ENA_USB_LOG_EVT    = (1 << 8U),
    DIS_USB_LOG_EVT    = (1 << 9U),
    LOGS_EVENT         = (1 << 10U),
    HARDFAULT_LOGS_EVT = (1 << 11U),
    SERIAL_NUMBER_EVT  = (1 << 12U)
} shell_event_t;

typedef struct __shell_heap_trace
{
    uint8_t enable : 1;
    uint16_t threshold;
} shell_heap_trace_t;

typedef struct log_history_node
{
    void *log;
    void *next;
} log_history_node_t;

/**
 * @brief Main function called from shellTask. Initializes USB, SerialManager and SHELL.
 *
 * @return                        0 on success execution, 1 otherwise
 */
int sln_shell_init(void);

/**
 * @brief The task implements a basic SHELL function, supporting a limited number of commands
 *        and also having a history feature.
 *
 * @param arg[in]                 Pointer to an argument, in accordance with TaskFunction_t
 * @return                        Void.
 */
void sln_shell_task(void *arg);

/**
 * @brief Setter function for app_init_task_handle, needed for waking that task.
 *
 * @param handle[in]              Pointer to task handle.
 * @return                        Void.
 */
void sln_shell_set_app_init_task_handle(TaskHandle_t *handle);

int log_shell_printf(const char *formatString, ...);
status_t log_history_flash_save(void);

/*
 * @brief Shell callback for FreeRTOS heap trace on pvPortMalloc
 *
 * @param ptr Heap address that is allocated
 * @param size Size of allocation in bytes
 */
void sln_shell_trace_malloc(void *ptr, size_t size);

/*
 * @brief Shell callback for FreeRTOS heap trace on vPortFree
 *
 * @param ptr Heap address that is de-allocated
 * @param size Size of de-allocation in bytes
 */
void sln_shell_trace_free(void *ptr, size_t size);

void APP_GetUniqueID(char **uniqueID);

#endif /* __SLN_SHELL_H */
