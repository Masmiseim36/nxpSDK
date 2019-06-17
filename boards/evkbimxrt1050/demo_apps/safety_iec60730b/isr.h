/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/

#ifndef _ISR_H_
#define _ISR_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/  
#undef  VECTOR_015
#define VECTOR_015 SYSTICK_Isr

/*******************************************************************************
 * API
 ******************************************************************************/
extern void SYSTICK_Isr(void);

#endif  /* _ISR_H_ */
