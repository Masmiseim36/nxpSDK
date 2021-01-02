/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_cfg.h"
#define  _STARTUP_
#include "safety_flow_check.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The initialized part of non-safety RW data section ER_RWRAM. */
extern uint32_t Load$$ER_RWRAM$$RW$$Base;
extern uint32_t Load$$ER_RWRAM$$RW$$Limit;
extern uint32_t Image$$ER_RWRAM$$RW$$Base;

/* The zero-initialized part of non-safety RW data section ER_RWRAM. */
extern uint32_t Image$$ER_RWRAM$$ZI$$Base;
extern uint32_t Image$$ER_RWRAM$$ZI$$Limit;

/* The safety-related RW data section ER_FSRAM. */
extern uint32_t Load$$ER_FS_RAM$$Base;
extern uint32_t Load$$ER_FS_RAM$$Limit;
extern uint32_t Image$$ER_FS_RAM$$Base;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief  Initialization of RAM sections.
 *
 * @param  void
 *
 * @return none
 */
void fsCommonStartup(void);

/*******************************************************************************
* Code
******************************************************************************/
/*!
* @brief Initialization of RAM sections.
*
* @param void
*
* @return None
*/
void fsCommonStartup(void)
{
    uint32_t  ui32SrcAddr;   /* Address of the source memory. */
    uint32_t  ui32EndAddr;   /* End of copied memory. */
    uint32_t  ui32DestAddr;  /* Address of the destination memory. */
    uint32_t  ui32BckFlowCheck = g_sfsFlowChkAR.ui32Signature; /* Backup the flow-check status variable. */

#if defined(__FPU_PRESENT)
    /* Set full co-processor access for CP10 and CP11 in System Control Block. */
    SCB->CPACR = (uint32_t)(0xFUL << 20U);
#endif /* defined(__FPU_PRESENT) */

    /* Get the addresses for the ER_RWRAM section (initialized data section). */
    ui32SrcAddr  = (uint32_t)&Load$$ER_RWRAM$$RW$$Base;
    ui32EndAddr  = (uint32_t)&Load$$ER_RWRAM$$RW$$Limit;
    ui32DestAddr = (uint32_t)&Image$$ER_RWRAM$$RW$$Base;

    /* Copy initialized data from ROM to RAM. */
    while(ui32SrcAddr < ui32EndAddr)
    {
        /* Copy one byte. */
        *((uint8_t*)ui32DestAddr) = *((uint8_t*)ui32SrcAddr);

        /* Increment the destination and source pointers. */
        ui32DestAddr++;
        ui32SrcAddr++;

        /* Refresh watchdog. */
        FS_WDOG_REFRESH();
    }

    /* Get the addresses for the ER_FS_RAM section (initialized .safety_ram
       section). */
    ui32SrcAddr  = (uint32_t)&Load$$ER_FS_RAM$$Base;
    ui32EndAddr  = (uint32_t)&Load$$ER_FS_RAM$$Limit;
    ui32DestAddr = (uint32_t)&Image$$ER_FS_RAM$$Base;

    /* Copy initialized data from ROM to RAM. */
    while(ui32SrcAddr < ui32EndAddr)
    {
        /* Copy one byte. */
        *((uint8_t*)ui32DestAddr) = *((uint8_t*)ui32SrcAddr);

        /* Increment the destination and source pointers. */
        ui32DestAddr++;
        ui32SrcAddr++;

        /* Refresh watchdog. */
        FS_WDOG_REFRESH();
    }

    /* Get the addresses for the .bss section (zero-initialized data). */
    ui32DestAddr = (uint32_t)&Image$$ER_RWRAM$$ZI$$Base;
    ui32EndAddr  = (uint32_t)&Image$$ER_RWRAM$$ZI$$Limit;

    /* Reset the .bss section RAM. */
    while(ui32DestAddr < ui32EndAddr)
    {
        /* Clear one byte. */
        *((uint8_t*)ui32DestAddr) = 0U;

        /* Increment the destination pointer. */
        ui32DestAddr++;

        /* Refresh watchdog. */
        FS_WDOG_REFRESH();
    }

    /* Restore the signature value. */
    g_sfsFlowChkAR.ui32Signature = ui32BckFlowCheck;

    /* Common startup node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_STARTUP);
}
