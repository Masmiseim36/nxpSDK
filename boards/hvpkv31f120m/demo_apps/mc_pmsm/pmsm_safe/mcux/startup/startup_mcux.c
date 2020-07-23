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
/* The initialized part of non-safety RW data section SEC_RWRAM. */
extern uint32_t m_sec_rwram_load_start;
extern uint32_t m_sec_rwram_load_end;
extern uint32_t m_sec_rwram_start;

/* The zero-initialized part of non-safety RW data section SEC_RWRAM. */
extern uint32_t m_sec_bss_start;
extern uint32_t m_sec_bss_end;

/* The safety-related RW data section SEC_BSS. */
extern uint32_t m_sec_fs_ram_load_start;
extern uint32_t m_sec_fs_ram_load_end;
extern uint32_t m_sec_fs_ram_start ;

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

    /* Get the addresses for the SEC_RWRAM section (initialized data section). */
    ui32SrcAddr  = (uint32_t)&m_sec_rwram_load_start;
    ui32EndAddr  = (uint32_t)&m_sec_rwram_load_end;
    ui32DestAddr = (uint32_t)&m_sec_rwram_start;

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

    /* Get the addresses for the SEC_FS_RAM section (initialized .safety_ram
       section). */
    ui32SrcAddr  = (uint32_t)&m_sec_fs_ram_load_start;
    ui32EndAddr  = (uint32_t)&m_sec_fs_ram_load_end;
    ui32DestAddr = (uint32_t)&m_sec_fs_ram_start;

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

    /* Get the addresses for the SEC_BSS section (zero-initialized data). */
    ui32DestAddr = (uint32_t)&m_sec_bss_start;
    ui32EndAddr  = (uint32_t)&m_sec_bss_end;

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

