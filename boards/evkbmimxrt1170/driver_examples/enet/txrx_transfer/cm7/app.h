/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_phyrtl8201.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
extern phy_rtl8201_resource_t g_phy_resource;
#define EXAMPLE_ENET                  ENET
#define EXAMPLE_PHY_ADDRESS           0x03U
#define EXAMPLE_PHY_OPS               &phyrtl8201_ops
#define EXAMPLE_PHY_RESOURCE          &g_phy_resource
#define EXAMPLE_CLOCK_FREQ            CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#define EXAMPLE_PHY_LINK_INTR_SUPPORT (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
