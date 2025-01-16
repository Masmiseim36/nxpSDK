/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_CLOCK_OUT_SELECT_ARRAY                                                                                             \
    {                                                                                                                          \
        kCOMMON_VDD2_BASE_to_VDD2_CLKOUT, kMAIN_PLL_PFD0_to_VDD2_CLKOUT, kFRO0_DIV1_to_VDD2_CLKOUT, kFRO1_DIV1_to_VDD2_CLKOUT, \
    }
#define APP_CLOCK_OUT_NAME_ARRAY                                                             \
    {                                                                                        \
        "Common VDD2 Base Clock", "MAIN PLL PFD0 Clock", "FRO0 Max Clock", "FRO1 Max Clock", \
    }

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
