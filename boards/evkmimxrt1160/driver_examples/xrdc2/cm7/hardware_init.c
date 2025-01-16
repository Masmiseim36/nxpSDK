/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_xrdc2.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void DEMO_AssignDomain(void)
{
    xrdc2_master_domain_assignment_t assignment;

    XRDC2_GetDefaultMasterDomainAssignment(&assignment);

    assignment.lock          = false;
    assignment.privilegeAttr = kXRDC2_MasterPrivilege;
    assignment.secureAttr    = kXRDC2_MasterSecure;
    assignment.domainId      = DEMO_CORE_DOMAIN;
    /* The XID input is not used for domain assignment hit. */
    assignment.mask  = 0x3FUL;
    assignment.match = 0UL;

    XRDC2_SetMasterDomainAssignment(DEMO_XRDC2, kXRDC2_Master_M7_AXI, 0, &assignment);

    /* The XID input is not used for domain assignment hit. */
    assignment.mask  = 0x3FUL;
    assignment.match = 0UL;
    XRDC2_SetMasterDomainAssignment(DEMO_XRDC2, kXRDC2_Master_M7_AHB, 0, &assignment);
}

void DEMO_SetAllMemAccessible(void)
{
    uint8_t domain;
    xrdc2_mem_access_config_t memAccessConfig;
    XRDC2_GetMemAccessDefaultConfig(&memAccessConfig);

    for (domain = 0; domain < FSL_FEATURE_XRDC2_DOMAIN_COUNT; domain++)
    {
        memAccessConfig.policy[domain] = kXRDC2_AccessPolicyAll;
    }

    /* CAAM */
    memAccessConfig.startAddr = 0x00280000U;
    memAccessConfig.endAddr   = 0x0028FFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_CAAM_Region0, &memAccessConfig);

    /* FLEXSPI1 */
    memAccessConfig.startAddr = 0x2F800000U;
    memAccessConfig.endAddr   = 0x3FFFFFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_FLEXSPI1_Region0, &memAccessConfig);

    /* FLEXSPI2 */
    memAccessConfig.startAddr = 0x60000000U;
    memAccessConfig.endAddr   = 0x7FFFFFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_FLEXSPI2_Region0, &memAccessConfig);

    /* M4 LMEM */
    memAccessConfig.startAddr = 0x20200000U;
    memAccessConfig.endAddr   = 0x2023ffffU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_M4LMEM_Region0, &memAccessConfig);

    /* M7 OCRAM */
    memAccessConfig.startAddr = 0x20360000U;
    memAccessConfig.endAddr   = 0x203fffffU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_M7OC_Region0, &memAccessConfig);

    /* SEMC */
    memAccessConfig.startAddr = 0x80000000U;
    memAccessConfig.endAddr   = 0xBFFFFFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_SEMC_Region0, &memAccessConfig);
}

void DEMO_TouchMemory(void)
{
    (*(volatile uint32_t *)DEMO_XRDC2_MEM_START_ADDR)++;
}

void DEMO_TouchPeriph(void)
{
    GPIO_PinRead(GPIO1, 0);
}

void DEMO_TouchMemSlot(void)
{
    uint32_t addrOffset;

    for (addrOffset = 0; addrOffset < DEMO_XRDC2_MEM_SLOT_ACCESS_SIZE; addrOffset += sizeof(uint32_t))
    {
        (void)(*(volatile uint32_t *)(DEMO_XRDC2_MEM_SLOT_START_ADDR + addrOffset));
    }
}

/*${function:end}*/
