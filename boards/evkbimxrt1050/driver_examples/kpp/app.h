/*
 * Copyright 2017 NXP
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
#define EXAMPLE_KPP_ACTIVE_COLUMROWS  (0xF0)
#define EXAMPLE_KPP_MATRX_NUM         (4)
#define EXAMPLE_KPP_COLROW_START_INDX (4)
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
    /* COL0    COL1    COL2    COL3   */
    {"SW1", "SW2", "SW3", "SW4"},     /* ROW0 */
    {"SW5", "SW6", "SW7", "SW8"},     /* ROW1 */
    {"SW9", "SW10", "SW11", "SW12"},  /* ROW2 */
    {"SW13", "SW14", "SW15", "SW16"}, /* ROW3 */
};
/*${variable:end}*/

#endif /* _APP_H_ */
