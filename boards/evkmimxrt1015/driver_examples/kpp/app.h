/*
 * Copyright 2018-2019 NXP
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
#define EXAMPLE_KPP_ACTIVE_COLUMROWS  (0xE)
#define EXAMPLE_KPP_MATRX_NUM         (3)
#define EXAMPLE_KPP_COLROW_START_INDX (1)
#define EXAMPLE_KPP                   KPP
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
char *keyMap[EXAMPLE_KPP_MATRX_NUM][EXAMPLE_KPP_MATRX_NUM] = {
    /* COL0    COL1    COL2   */
    {"SW1", "SW2", "SW3"}, /* ROW0 */
    {"SW4", "SW5", "SW6"}, /* ROW1 */
    {"SW7", "SW8", "SW9"}  /* ROW2 */
};
/*${variable:end}*/

#endif /* _APP_H_ */
