/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file switch.h
 * @brief This file is used for the initialization of the buttons
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "FreeRTOS.h"
#include "board.h"
#include "event_groups.h"
#include "lwip/timeouts.h"
#include "osa_time.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Buttons related GPIOs */
#define SW_COUNT         2
#define GPIO_IRQHandler  BOARD_USER_BUTTON_IRQ_HANDLER
#define PRESSED          0
#define RELEASED         1
#define BIT_PRESS_1      (1 << 0)
#define BIT_RELEASE_1    (1 << 1)
#define BIT_PRESS_2      (1 << 2)
#define BIT_RELEASE_2    (1 << 3)
#define DEBOUNCE_TIME_US 50000

/*******************************************************************************
 * Prototype
 ******************************************************************************/
/*!
 * @brief Initializes the GPIO and intrerupts for the buttons
 */
void switchInit(void);

#endif /* SWITCH_H_ */
