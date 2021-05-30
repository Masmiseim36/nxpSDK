
/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BOARD_EXTPA_H__
#define __BOARD_EXTPA_H__

#include "GPIO_Adapter.h"
#include "gpio_pins.h"

typedef enum {
    BOARD_ExtPaSuccess,
    BOARD_ExtPaInvalidParamter,
    BOARD_ExtPaError,
} BOARD_ExtPaStatus_t;

typedef enum {
    CSD = 0,
    CPS,
#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
    CHL,
#endif
ANT_SEL,
} BOARD_ExtPaCtrlPins_t;

/* Initializa ExtPA pins - pin configuration that is passed by argument pinCfg
    shall be in the order given by BOARD_ExtPaCtrlPins_t structure
   Note that two Other pins CTX and CRX are directly configured by this function
    to PTAC2 and PTAC3 pads as these are not mapped to GPIO module but TSM XCVR */
BOARD_ExtPaStatus_t  BOARD_ExtPaInit(const gpioOutputPinConfig_t  *pinCfg);

/* Select the corresponding PA table according to the PA configuration. Used only by BLE */
void                 BOARD_ExtPaUpdatePaTable(void);

/* Configure the XCVR for PA control */
BOARD_ExtPaStatus_t  BOARD_ExtPaXcvrInit(bool_t init_all);

/* Set the external PA to sleep mode */
void  BOARD_ExtPaSleep(void);

/* Set the external PA to active mode. The PA will go to sleep if no RX/TX ongoing */
void  BOARD_ExtPaActive(void);

/* Set CPS pin to high */
void  BOARD_ExtPaBypassEnable(void);

/* Set CPS pin to low */
void  BOARD_ExtPaBypassDisable(void);

#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
/* Select transmit linear mode. Receive will use LNA mode. */
void  BOARD_ExtPaSelTxModeLinear(void);
/* Select transmit high-efficency mode. Receive will use LNA mode. */
void  BOARD_ExtPaSelTxModeHighEfficiency(void);
#endif

/* Use bypass mode for TX. RX mode will be set to bypass as well. */
#define BOARD_ExtPaSelTxModeBypass()    BOARD_ExtPaBypassEnable()

/* Use bypass mode for RX . TX mode will be set to bypass as well. */
#define BOARD_ExtPaSelRxModeBypass()    BOARD_ExtPaBypassEnable()

/* Disable bypass mode for RX. TX mode may be linear or high-efficiency. */
#define BOARD_ExtPaSelRxModeLna()       BOARD_ExtPaBypassDisable()

/* Select antenna 1 */
void  BOARD_ExtPaSelAnt1(void);

/* Select antenna 2 */
void  BOARD_ExtPaSelAnt2(void);

void  BOARD_ExtPaGetConvTable(const int8_t **ConvPaTablePtr);


#endif
