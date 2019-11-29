/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __PWR_LIB_PLATFORM_H__
#define __PWR_LIB_PLATFORM_H__

/*****************************************************************************
 *                             PUBLIC MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define NO_WAKE_UP    (0)
#define LPTMR_WAKE_UP (1)
#define UART_WAKE_UP  (2)
#define GPIO_WAKE_UP  (3)
#define XCVR_IRQ_WAKE_UP (4)
#define UNKNOWN_WAKE_UP  (5)
#define PWRP_LLWU_WAKE_UP_PIN   (BOARD_LLWU_PIN_ENABLE_BITMAP)

/*****************************************************************************
 *                        PUBLIC TYPE DEFINITIONS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/* Return codes from the custom platform power modes */
typedef enum pwrp_status_tag
{
    PWRP_OK = 0,
    PWRP_ERR,
    PWRP_MAX
}pwrp_status_t;

/*****************************************************************************
 *                               PUBLIC VARIABLES(External)                  *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have global      *
 * (project) scope.                                                          *
 * These variables / constants can be accessed outside this module.          *
 * These variables / constants shall be preceded by the 'extern' keyword in  *
 * the interface header.                                                     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *                            PUBLIC FUNCTIONS                               *
 *---------------------------------------------------------------------------*
 * Add to this section all the global functions prototype preceded (as a     *
 * good practice) by the keyword 'extern'                                    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
extern void PWRP_UpdateLPTMRCount(uint16_t u16TimeInMs);
extern void PWRP_Init(void);
extern pwrp_status_t PWRP_SetWakeUpSource(uint8_t u8WUSource);
extern pwrp_status_t Enter_Wait(void);
extern pwrp_status_t Enter_Stop(void);
extern pwrp_status_t Enter_RUN (void);
extern pwrp_status_t Enter_VLPR(void);
extern pwrp_status_t Enter_VLPW(void);
extern pwrp_status_t Enter_VLPS(void);
extern pwrp_status_t Enter_LLS (void);
extern pwrp_status_t Enter_VLLS(uint8_t u8SubMode);
extern uint8_t PWRP_GetWakeUpReason(void);

#endif /* __PWR_LIB_PLATFORM_H__ */

