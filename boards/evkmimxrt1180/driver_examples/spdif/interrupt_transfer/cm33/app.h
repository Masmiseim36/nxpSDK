/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPDIF          SPDIF
#define DEMO_SPDIF_CLOCK_FREQ  CLOCK_GetPllFreq(kCLOCK_Pll_AudioPll)
#define DEMO_SPDIF_SAMPLE_RATE 48000
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
