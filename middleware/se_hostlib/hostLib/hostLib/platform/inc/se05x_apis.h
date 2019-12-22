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

#ifndef _SE05X_API_H
#define _SE05X_API_H

/*
 * Define Reset logic for reset pin on SE 
 * Active high for SE050
 */
#define SE_RESET_LOGIC 1

void se05x_ic_reset(void);

#endif // _SE05X_API_H
