/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//${macro:start}
#define DEMO_ACMP_BASEADDR    CMP1
#define DEMO_ACMP_MINUS_INPUT 2U
#define DEMO_ACMP_PLUS_INPUT  7U    /*  Internal 8bit DAC output. */
#define DEMO_CMP_USE_VIN1     false /* The Vin1 is connect to GND, so use VIN2 in this example. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//${prototype:start}
void BOARD_InitHardware(void);
//${prototype:end}
