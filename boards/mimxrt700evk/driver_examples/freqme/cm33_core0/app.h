/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CLOCK_SOURCE_COUNT 3U
#define DEMO_CLOCK_SOURCE_NAME                  \
    {                                           \
        "OSC_CLK", "FRO1_DIV8", "LPOSC"         \
    }

#define DEMO_REFERENCE_CLOCK_SOURCE_SIGNAL                                                   \
    {                                                                                        \
        kINPUTMUX_OscClkToFreqmeasRef, kINPUTMUX_Fro1Div8ToFreqmeasRef, kINPUTMUX_LposcToFreqmeasRef \
    }

#define DEMO_TARGET_CLOCK_SOURCE_SIGNAL                                                            \
    {                                                                                              \
        kINPUTMUX_OscClkToFreqmeasTar, kINPUTMUX_Fro1Div8ToFreqmeasTar, kINPUTMUX_LposcToFreqmeasTar \
    }

#define DEMO_REF_CLK_SOURCE    kINPUTMUX_Fro1ToFreqmeasRef
#define DEMO_TARGET_CLK_SOURCE kINPUTMUX_Fro1ToFreqmeasTar
#define DEMO_FREQME            FREQME
#define FREQME_IRQHANDLER      Freqme_IRQHandler
#define DEMO_MAXEXPECTVALUE    (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE    (0xFUL)
#define INPUTMUX               INPUTMUX0
#define DEMO_REF_CLK_FREQ      CLOCK_GetFroClkFreq(1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
