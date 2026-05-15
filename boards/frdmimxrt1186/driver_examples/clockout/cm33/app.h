/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
typedef uint32_t clock_output_divider_t;

#define CLOCK_DIVIDER_NEED_ATCUAL_VALUE 1

#define APP_OUTPUT1_CLOCK_NAME_ARRAY                                \
    {                                                               \
        "OSC_RC_24M", "OSC_RC_400M", "PLL_480_DIV2", "PLL_1G_DIV2", \
    }

#define APP_OUTPUT1_CLOCK_NAME_ENUM_ARRAY                            \
    {                                                                \
        kCLOCK_CKO1OutputMuxOscRc24M, kCLOCK_CKO1OutputMuxOscRc400M, \
            kCLOCK_CKO1OutputMuxSysPll3Div2, kCLOCK_CKO1OutputMuxSysPll1Div2, \
    }

#define APP_OUTPUT2_CLOCK_NAME_ARRAY                           \
    {                                                          \
        "OSC_RC_24M", "OSC_RC_400M", "PLL_1G_DIV5", "PLL_ARM", \
    }

#define APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY                            \
    {                                                                \
        kCLOCK_CKO2OutputMuxOscRc24M, kCLOCK_CKO2OutputMuxOscRc400M, \
            kCLOCK_CKO2OutputMuxSysPll1Div5, kCLOCK_CKO2OutputMuxArmPllOut, \
    }

#define EXAMPLE_CLKO1_MAX_DIVIDER_VALUE 256U
#define EXAMPLE_CLKO2_MAX_DIVIDER_VALUE 256U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
