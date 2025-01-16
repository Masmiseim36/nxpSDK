/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define BOARD_SW_GPIO        BOARD_SW6_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_SW6_GPIO_PIN
#define BOARD_SW_NAME        "SW6"
#define BOARD_SW_IRQ         PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER PIN_INT0_IRQHandler

#define PINT                   PINT1
#define INPUTMUX               INPUTMUX1
#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort8Pin5ToPintsel

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER_CPU1      /*!< Tickless timer base address. */
#define TICKLESS_OSTIMER_IRQn     OS_EVENT_IRQn /*!< Tickless timer IRQ number. */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern OSTIMER_Type *vPortGetOstimerBase(void);
extern IRQn_Type vPortGetOstimerIrqn(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
