/*
 * Copyright 2023 NXP
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
#define EXAMPLE_PINT_BASE      PINT0
#define DEMO_PINT_BSLICE0_SRC  kPINT_PatternMatchInp0Src
#define DEMO_PINT_BSLICE1_SRC  kPINT_PatternMatchInp0Src
#define DEMO_PINT_BSLICE2_SRC  kPINT_PatternMatchInp1Src
#define DEMO_PINT_PIN_INT0_SRC kINPUTMUX_GpioPort0Pin9ToPintsel
#define DEMO_PINT_PIN_INT1_SRC kINPUTMUX_GpioPort1Pin3ToPintsel
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
