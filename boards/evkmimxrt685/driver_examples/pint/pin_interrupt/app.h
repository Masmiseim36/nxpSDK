/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DEMO_PINT_PIN_INT0_SRC kINPUTMUX_GpioPort1Pin1ToPintsel  /* SW1 */
#define DEMO_PINT_PIN_INT1_SRC kINPUTMUX_GpioPort0Pin10ToPintsel /* SW2 */
#define DEMO_PINT_PIN_INT2_SRC kINPUTMUX_GpioPort0Pin3ToPintsel  /* J1-3 */
#define DEMO_PIN_NUM           3
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
