/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

/**
 * @file fault_handlers.h
 * @brief This file handles saving and printing the fault logs
 */

#ifndef _FAULT_HANDLERS_H_
#define _FAULT_HANDLERS_H_

#include <stdint.h>

#include "fsl_common.h"

#define FAULT_STATUSREG_LOG_FILE_NAME "fault_statusreg_log.dat"
#define FAULT_STATUSREG_LOG_FILE_ADDR (0x1D40000)

/**
 * @brief This are the faults type
 */
enum _fault_type
{
    kFaultType_None      = 0,
    kFaultType_Hard      = 1,
    kFaultType_MemManage = 2,
    kFaultType_Bus       = 3,
    kFaultType_Usage     = 4
};

/**
 * @brief Processor registers
 */
typedef struct stack_registers
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;  /**< Link register. */
    uint32_t pc;  /**< Program counter. */
    uint32_t psr; /**< Program status register. */
} stack_registers_t;

/**
 * @brief Fault registers and status
 */
typedef struct fault_status
{
    uint32_t fault_type;
    uint32_t hfsr;         /**< HardFault Status Register (SCB->HFSR) */
    uint32_t cfsr;         /**< Configurable Fault Status Register (SCB->CFSR) */
    uint32_t mmfar;        /**< MemManage Fault Address Register (SCB->MMFAR) */
    uint32_t bfar;         /**< BusFault Address Register (SCB->BFAR) */
    stack_registers_t msr; /**< Main Stack Registers*/
    stack_registers_t psr; /**< Process Stack Registers */
} fault_status_t;

/**
 * @brief Reads fault log from flash memory
 *
 * @param fault_log[out]  Pointer to a pre-allocated fault_status_t structure to store the flash content
 * @return                0 on success, SLN_FLASH_MGMT error code otherwise
 */
status_t fault_log_flash_get(fault_status_t *fault_log);

/**
 * @brief Writes fault log in flash memory
 *
 * @param fault_log[in]   Pointer to a fault_status_t structure containing the data that will be written
 * @return                0 on success, SLN_FLASH_MGMT error code otherwise
 */
status_t fault_log_flash_set(fault_status_t *fault_log);

/**
 * @brief Erase fault log stored in flash memory
 *
 * @return                0 on success, SLN_FLASH_MGMT error code otherwise
 */
status_t fault_log_flash_erase(void);

/**
 * @brief Check fault status
 *
 * @param fault_log[in]   A fault_status_t structure containing the data that will be checked
 * @return                0 no fault, 1 fault detected
 */
status_t fault_check_status(fault_status_t s_fault_status);

#endif /* _FAULT_HANDLERS_H_ */
