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

#define BOARD_SW_GPIO        BOARD_SW5_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_SW5_GPIO_PIN
#define BOARD_SW_IRQ         PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER PIN_INT0_IRQHandler
#define BOARD_SW_NAME        BOARD_SW5_NAME

#define PINT                   PINT0
#define INPUTMUX               INPUTMUX0
#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin9ToPintsel

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER_CPU0      /*!< Tickless timer base address. */
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
