/*
 * Copyright 2018 NXP
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
#define DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_PowerOffSDCARD(void);
void BOARD_PowerOnSDCARD(void);
void BOARD_USDHC_Switch_VoltageTo1V8(void);
void BOARD_USDHC_Switch_VoltageTo3V3(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
