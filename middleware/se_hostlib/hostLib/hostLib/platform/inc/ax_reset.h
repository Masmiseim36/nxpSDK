/* Copyright 2018-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _AX_RESET_H
#define _AX_RESET_H

#include "sm_types.h"

/* 
 * Where applicable, Configure the PINs on the Host
 *
 */
void axReset_HostConfigure(void);

/* 
 * Where applicable, PowerCycle the SE
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_ResetPluseDUT(void);

/* 
 * Where applicable, put SE in low power/standby mode
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_PowerDown(void);

/* 
 * Where applicable, put SE in powered/active mode
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_PowerUp(void);

#endif // _AX_RESET_H
