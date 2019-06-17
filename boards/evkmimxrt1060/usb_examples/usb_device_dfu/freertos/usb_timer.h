/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_TIMER_H_
#define _USB_TIMER_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef void (*usb_timer_callback_t)(void);

/*******************************************************************************
 * API
 ******************************************************************************/
void USB_TimerInit(uint8_t instance, uint32_t interval, uint32_t clock, usb_timer_callback_t callback);
void USB_TimerInt(uint8_t instance, uint8_t enable);
uint32_t USB_TimerGetUsTickCount(uint8_t instance);

#endif /* _USB_TIMER_H_ */
