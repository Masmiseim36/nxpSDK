/**************************************************************************
MODULE:    MCOHW_LED
CONTAINS:  Macro definition for LED switching
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _LED_H
#define _LED_H

/**************************************************************************
DEFINITIONS
***************************************************************************/

// LED numbers and states
#define COLED_NUM_RUN 0u
#define COLED_NUM_ERR 1u
#define COLED_OFF     0u
#define COLED_ON      1u

#if USE_LEDS

#ifdef __SIMULATION__

#define LED_RUN_ON  SimDriver_UpdateLEDState(SIMDRV_RUNLED, 1)
#define LED_RUN_OFF SimDriver_UpdateLEDState(SIMDRV_RUNLED, 0)
#define LED_ERR_ON  SimDriver_UpdateLEDState(SIMDRV_ERRLED, 1)
#define LED_ERR_OFF SimDriver_UpdateLEDState(SIMDRV_ERRLED, 0)

#else

#define LED_RUN_ON  LIBCB_SwitchLeds(COLED_NUM_RUN, COLED_ON)
#define LED_RUN_OFF LIBCB_SwitchLeds(COLED_NUM_RUN, COLED_OFF)
#define LED_ERR_ON  LIBCB_SwitchLeds(COLED_NUM_ERR, COLED_ON)
#define LED_ERR_OFF LIBCB_SwitchLeds(COLED_NUM_ERR, COLED_OFF)

#endif // __SIMULATION__

/**************************************************************************
PUBLIC FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Initialize I/O for LEDs.
RETURNS: nothing
**************************************************************************/
void LIBCB_InitLeds(void);

/**************************************************************************
DOES:    This function turns a numbered LED on or off.
RETURNS: nothing
**************************************************************************/
void LIBCB_SwitchLeds(uint8_t LED_NR, // LED number
                      uint8_t state   // LED state
);

#endif // USE_LEDS

#endif // _LED_H
