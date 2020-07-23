/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "rsim_ctrl.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RF_OSCILLATOR_STAYS_ON     (false) /* Control whether RF_OSC can be left on all the time. */
#define RF_OSCILLATOR_READY        ((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) != 0x0UL)

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void RSIM_rf_osc_startup(void)
{
    RSIM->CONTROL |= RSIM_CONTROL_RF_OSC_EN_MASK;
    while (!RF_OSCILLATOR_READY)
    {
        /* Wait for RF_OSC_READY to be asserted before continuing */
    }
}

void RSIM_rf_osc_shutdown(void)
{
    /* RF_OSCILLATOR_STAYS_ON flag controls whether XCVR can shut off RF OSC or not */
#if (!RF_OSCILLATOR_STAYS_ON)
    RSIM->CONTROL &= ~RSIM_CONTROL_RF_OSC_EN_MASK;
#endif
}

void RSIM_radio_reset(void)
{
    /* Assert and then clear radio reset */
    RSIM->CONTROL |= RSIM_CONTROL_BLOCK_SOC_RESETS_MASK; /* RADIO_RESET_BIT is conditional on this bit being asserted */
    RSIM->CONTROL |= RSIM_CONTROL_RADIO_RESET_BIT_MASK;
    RSIM->CONTROL &= ~(RSIM_CONTROL_RADIO_RESET_BIT_MASK); /* Ref. Manual states that reset must be cleared twice */
    RSIM->CONTROL &= ~(RSIM_CONTROL_RADIO_RESET_BIT_MASK);
    RSIM->CONTROL &= ~(RSIM_CONTROL_BLOCK_SOC_RESETS_MASK); /* restore reset control to the SOC */
}


#if (RSIM_POWERUP_SUPPORTED)
void RSIM_radio_powerup(void)
{
    /* RSIM doesn't control the radio power state */
}

void RSIM_radio_powerdown(void)
{
    /* RSIM doesn't control the radio power state */
}
#endif /* RSIM_POWERUP_SUPPORTED */

