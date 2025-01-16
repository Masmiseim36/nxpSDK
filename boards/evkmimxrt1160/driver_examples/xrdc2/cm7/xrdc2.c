/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "fsl_xrdc2.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum _demo_state
{
    kDEMO_StateInit = 0,
    kDEMO_StateMem,
    kDEMO_StatePeriph,
    kDEMO_StateMemSlot,
} demo_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile static bool s_faultFlag         = false;
volatile static demo_state_t s_demoState = kDEMO_StateInit;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void Fault_Handler(void)
{
    s_faultFlag = true;

    if (s_demoState == kDEMO_StateMem)
    {
        XRDC2_SetMemDomainAccessPolicy(DEMO_XRDC2, DEMO_XRDC2_MEM, DEMO_CORE_DOMAIN, kXRDC2_AccessPolicyAll);
    }
    else if (s_demoState == kDEMO_StatePeriph)
    {
        XRDC2_SetPeriphDomainAccessPolicy(DEMO_XRDC2, DEMO_XRDC2_PERIPH, DEMO_CORE_DOMAIN, kXRDC2_AccessPolicyAll);
    }
    else if (s_demoState == kDEMO_StateMemSlot)
    {
        XRDC2_SetMemSlotDomainAccessPolicy(DEMO_XRDC2, DEMO_XRDC2_MEM_SLOT, DEMO_CORE_DOMAIN, kXRDC2_AccessPolicyAll);
    }
    else
    {
        PRINTF("ERROR: Should not reach here.\r\n");
        while (1)
            ;
    }

    SDK_ISR_EXIT_BARRIER;
}

void HardFault_Handler(void)
{
    Fault_Handler();
}

void BusFault_Handler(void)
{
    Fault_Handler();
}

void DEMO_SetMemoryUnaccessible(void)
{
    xrdc2_mem_access_config_t memAccessConfig;

    XRDC2_GetMemAccessDefaultConfig(&memAccessConfig);

    memAccessConfig.startAddr                = DEMO_XRDC2_MEM_START_ADDR;
    memAccessConfig.endAddr                  = DEMO_XRDC2_MEM_END_ADDR;
    memAccessConfig.policy[DEMO_CORE_DOMAIN] = kXRDC2_AccessPolicyNone;

    XRDC2_SetMemAccessConfig(DEMO_XRDC2, DEMO_XRDC2_MEM, &memAccessConfig);
}

void DEMO_Mem(void)
{
    s_demoState = kDEMO_StateMem;

    /* Set the unaccessible memory region. */
    PRINTF("Set the memory not accessible\r\n");
    DEMO_SetMemoryUnaccessible();

    /*
     * Invalidate the cache, so new read will read from memory directly,
     * to make sure trigger read error.
     */
    DCACHE_InvalidateByRange(DEMO_XRDC2_MEM_START_ADDR, DEMO_XRDC2_MEM_END_ADDR - DEMO_XRDC2_MEM_START_ADDR + 1);

    /* Touch the memory, there will be hardfault. */
    s_faultFlag = false;

    DEMO_TouchMemory();

    /*
     * Flush the cache, so the modified data is written to memory,
     * to make sure trigger write error.
     */
    DCACHE_CleanInvalidateByRange(DEMO_XRDC2_MEM_START_ADDR, DEMO_XRDC2_MEM_END_ADDR - DEMO_XRDC2_MEM_START_ADDR + 1);
    __DSB();

    /* Wait for the hardfault occurs. */
    while (!s_faultFlag)
    {
    }

    PRINTF("The memory is accessible now\r\n");
}

void DEMO_SetPeriphUnaccessible(void)
{
    xrdc2_periph_access_config_t periphConfig;

    XRDC2_GetPeriphAccessDefaultConfig(&periphConfig);

    periphConfig.lockMode                 = kXRDC2_AccessConfigLockDisabled;
    periphConfig.policy[DEMO_CORE_DOMAIN] = kXRDC2_AccessPolicyNone;

    XRDC2_SetPeriphAccessConfig(DEMO_XRDC2, DEMO_XRDC2_PERIPH, &periphConfig);
}

void DEMO_Periph(void)
{
    s_demoState = kDEMO_StatePeriph;

    /* Set unaccessible. */
    PRINTF("Set the peripheral not accessible\r\n");
    DEMO_SetPeriphUnaccessible();

    /* Touch the memory, there will be hardfault. */
    s_faultFlag = false;

    DEMO_TouchPeriph();

    /* Wait for the hardfault occurs. */
    while (!s_faultFlag)
    {
    }

    PRINTF("The peripheral is accessible now\r\n");
}

void DEMO_SetMemSlotUnaccessible(void)
{
    xrdc2_mem_slot_access_config_t memSlotConfig;

    XRDC2_GetMemSlotAccessDefaultConfig(&memSlotConfig);

    memSlotConfig.lockMode                 = kXRDC2_AccessConfigLockDisabled;
    memSlotConfig.policy[DEMO_CORE_DOMAIN] = kXRDC2_AccessPolicyNone;

    XRDC2_SetMemSlotAccessConfig(DEMO_XRDC2, DEMO_XRDC2_MEM_SLOT, &memSlotConfig);
}

void DEMO_MemSlot(void)
{
    s_demoState = kDEMO_StateMemSlot;

    /* Set unaccessible. */
    PRINTF("Set the memory slot not accessible\r\n");
    DEMO_SetMemSlotUnaccessible();

    /*
     * Invalidate the cache, so new read will read from memory directly,
     * to make sure trigger read error.
     */
    DCACHE_InvalidateByRange(DEMO_XRDC2_MEM_SLOT_START_ADDR, DEMO_XRDC2_MEM_SLOT_ACCESS_SIZE);

    /* Touch the memory, there will be hardfault. */
    s_faultFlag = false;

    DEMO_TouchMemSlot();

    /*
     * Flush the cache, so the modified data is written to memory,
     * to make sure trigger write error.
     */
    DCACHE_CleanInvalidateByRange(DEMO_XRDC2_MEM_SLOT_START_ADDR, DEMO_XRDC2_MEM_SLOT_ACCESS_SIZE);
    __DSB();

    /* Wait for the hardfault occurs. */
    while (!s_faultFlag)
    {
    }

    PRINTF("The memory slot is accessible now\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitHardware();

    PRINTF("XRDC2 example started\r\n");

    XRDC2_Init(DEMO_XRDC2);

    DEMO_AssignDomain();

    DEMO_SetAllMemAccessible();

    XRDC2_SetGlobalValid(DEMO_XRDC2, true);

    if (XRDC2_GetCurrentMasterDomainId(DEMO_XRDC2) != DEMO_CORE_DOMAIN)
    {
        PRINTF("ERROR: Domain set failed\r\n");
        while (1)
            ;
    }

    DEMO_Mem();

    DEMO_Periph();

    DEMO_MemSlot();

    PRINTF("XRDC2 example Success\r\n");

    XRDC2_SetGlobalValid(DEMO_XRDC2, false);

    XRDC2_Deinit(DEMO_XRDC2);

    while (1)
    {
    }
}
