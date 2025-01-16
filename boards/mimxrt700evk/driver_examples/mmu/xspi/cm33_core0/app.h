/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_power.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_MMU_INSTANCE   (MMU1)
#define DEMO_MMU_BASE_ADDR   (0x0800UL)
#define DEMO_MMU_MAX_ADDR    (0x0FFFUL)
#define DEMO_MMU_UNITE_SIZE   (kMMU_UnitSize8KB)
#define DEMO_UNIT_BYTE_SIZE           (0x2000UL)
#define DEMO_IRQN              (MMU1_IRQn)
#define DEMO_MMU_IRQ_HANDLER   MMU1_IRQHandler

#define DEMO_MMU_ADDR_ARRAY_SIZE  (4U)
#define DEMO_MMU_PHY_ADDR_ARRAY {0x8000000UL, 0x8000000UL, 0x8000000UL, 0x8000000UL}
#define DEMO_MMU_PHY_ADDR_STR   "{0x8000000UL, 0x8000000UL, 0x8000000UL, 0x8000000UL}"
#define DEMO_MMU_VIRTUAL_ADDR_ARRAY {0x8000000UL, 0x8004000UL, 0x8006000UL, 0x8008000UL}
#define DEMO_MMU_VIRTUAL_ADDR_STR "{0x8000000UL, 0x8004000UL, 0x8006000UL, 0x8008000UL}"

#define DEMO_TEST_BUFFER_LEN   (256UL)
#define DEMO_SIMPLE_MAP_VIRT_ADDR (0x8002000UL)
#define DEMO_SIMPLE_MAP_PHYS_ADDR (0x8000000UL)
      
#define DEMO_REGION_MAP_VIRT_ADDR (0x8100000UL)
#define DEMO_REGION_MAP_PHYS_ADDR (0x8000000UL)
#define DEMO_REGION_SIZE          (0x100000UL)  /* Region size is 1MB */

#define DEMO_CLEAN_INVALIDA_CACHE   do {\
      CACHE64_CTRL1->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK| \
        CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK; \
      while ((CACHE64_CTRL1->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)  {} \
      CACHE64_CTRL1->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK| \
                CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);  \
     } while(0)
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/


/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
