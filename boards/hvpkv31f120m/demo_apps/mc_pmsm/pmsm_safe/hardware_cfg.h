/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_CFG_H_
#define _HARDWARE_CFG_H_

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "fsl_device_registers.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Interrupts.
 ******************************************************************************/
/* The startup interrupt vector handler. */
#define VECTOR_001             IRQHNDLR_fsStart

/* The non-maskable interrupt vector handler. */
#define VECTOR_002             IRQHNDLR_fsNMI

/* The hard-fault interrupt vector handler. */
#define VECTOR_003             IRQHNDLR_fsHardFault

/* ADC16 IRQ for fast-control loop timing and execution. */
#define VECTOR_055             IRQHNDLR_fsADC0
#define IRQ_FAST_LOOP_PRIO     (1U)         /* Fast control loop priority. */
#define IRQ_FAST_LOOP_FREQ     (10000UL)    /* Fast control loop frequency in Hz. */

/* PDB0 IRQ for exception handling. */
#define VECTOR_068             IRQHNDLR_fsPDB
#define IRQ_PDB0_PRIO          (2U)

/* FTM2 IRQ for slow-control loop timing and execution. */
#define VECTOR_060             IRQHNDLR_fsFTM2
#define IRQ_SLOW_LOOP_PRIO     (3U)        /* Slow control loop priority. */
#define IRQ_SLOW_LOOP_FREQ     (1000U)     /* Slow control loop frequency in Hz. */

/*******************************************************************************
 * Clock configuration.
 ******************************************************************************/
/* The IRC48M internal oscillator frequency. */
#define CLOCK_IRC48MCLK_FREQ      (48000000UL)

/* Configure the PLL divider and multiplier factor.  */
#define CLOCK_PLL_MULT            (30U)
#define CLOCK_PLL_DIV             (12U)

/* The PLL output frequency in Hz.
   -By default set to 120 MHz. */
#define CLOCK_MCGOUTCLK_FREQ       ((CLOCK_IRC48MCLK_FREQ * CLOCK_PLL_MULT) / CLOCK_PLL_DIV)

/* The core- and system-clock frequency in Hz.
   -By default set to 120 MHz. */
#define CLOCK_SYSTEM_OUTDIV        (1UL)
#define CLOCK_SYSTEM_FREQ          (CLOCK_MCGOUTCLK_FREQ / CLOCK_SYSTEM_OUTDIV)

/* The bus-clock frequency in Hz.
   -By default set to 60 MHz. */
#define CLOCK_BUS_OUTDIV           (2UL)
#define CLOCK_BUS_FREQ             (CLOCK_MCGOUTCLK_FREQ / CLOCK_BUS_OUTDIV)

/* The FlexBus clock frequency in Hz.
   -By default set to 30 MHz.*/
#define CLOCK_FLEXBUS_OUTDIV       (4UL)
#define CLOCK_FLEXBUS_FREQ         (CLOCK_MCGOUTCLK_FREQ / CLOCK_FLEXBUS_OUTDIV)

/* The flash clock frequency in Hz.
   -By default set to 24 MHz.*/
#define CLOCK_FLASH_OUTDIV         (5UL)
#define CLOCK_FLASH_FREQ           (CLOCK_MCGOUTCLK_FREQ / CLOCK_FLASH_OUTDIV)

/* The MCGIRCLK frequency.
   -By default set to 1000 kHz. */
#define CLOCK_MCGIRCLK_DIV         (2U)
#define CLOCK_MCGIRCLK_FREQ        (4000000UL / (1UL << CLOCK_MCGIRCLK_DIV))

/* The LPO frequency in Hz. */
#define CLOCK_LPO_FREQ             (1000U)

/* The systick timer overflow frequency in Hz. */
#define CLOCK_SYSTICK_FREQ         (200U)

/* The ADC frequency in Hz for ADC16 calibration process (should be in 2-4 MHz
   range).
   -The bus clock is used as input to ADC0 and ADC1.
   -By default set to 3.75 MHz. */
#define CLOCK_ADC_CAL_BUS_OUTDIV   (4UL)
#define CLOCK_ADC_CAL_ADC16_DIV    (3UL)
#define CLOCK_ADC_CAL_FREQ         (CLOCK_MCGOUTCLK_FREQ / (CLOCK_BUS_ADC_CAL_OUTDIV *\
                                    (1UL << CLOCK_ADC_CAL_ADC16_DIV)))

/*******************************************************************************
 * PWM pin configuration.
 ******************************************************************************/
/* PWM signal pin assignment. */
#define PIN_PWM0_PORT     (PORTC)
#define PIN_PWM0_PIN      (1U)
#define PIN_PWM0_MUX      (4U)
#define PIN_PWM1_PORT     (PORTC)
#define PIN_PWM1_PIN      (2U)
#define PIN_PWM1_MUX      (4U)
#define PIN_PWM2_PORT     (PORTC)
#define PIN_PWM2_PIN      (5U)
#define PIN_PWM2_MUX      (7U)
#define PIN_PWM3_PORT     (PORTC)
#define PIN_PWM3_PIN      (4U)
#define PIN_PWM3_MUX      (4U)
#define PIN_PWM4_PORT     (PORTD)
#define PIN_PWM4_PIN      (4U)
#define PIN_PWM4_MUX      (4U)
#define PIN_PWM5_PORT     (PORTD)
#define PIN_PWM5_PIN      (5U)
#define PIN_PWM5_MUX      (4U)

/* Over-current fault detection FTM periphery input. */
#define PIN_FAULT_1_PORT  (PORTA)   /*< Port periphery. */
#define PIN_FAULT_1_PIN   (19U)     /*< Pin number. */
#define PIN_FAULT_1_MUX   (2U)      /*< Initial GPIO pin state. */


/*******************************************************************************
 * GPIO pin initialization and control.
 ******************************************************************************/

/* Braking resistor control macros. */
#define PIN_MCU_BRAKE_GPIO     (GPIOC)    /*< GPIO periphery. */
#define PIN_MCU_BRAKE_PIN      (0U)      /*< Pin number. */

/* In-rush relay control macros. */
#define PIN_RELAY_GPIO         (GPIOC)    /*< GPIO periphery. */
#define PIN_RELAY_PIN          (13U)      /*< Pin number. */

/* USER_LED control macro. */
#define PIN_USER_LED_GPIO      (GPIOB)   /*< GPIO periphery. */
#define PIN_USER_LED_PIN       (19U)      /*< Pin number. */
#define BOARD_USER_LED_ON()    (PIN_USER_LED_GPIO->PSOR = (1UL << PIN_USER_LED_PIN))
#define BOARD_USER_LED_OFF()   (PIN_USER_LED_GPIO->PCOR = (1UL << PIN_USER_LED_PIN))

/* USER_LED_2 control macro. */
#define PIN_USER_LED_2_GPIO    (GPIOA)   /*< GPIO periphery. */
#define PIN_USER_LED_2_PIN     (5U)      /*< Pin number. */
#define BOARD_USER_LED_2_ON()  (PIN_USER_LED_2_GPIO->PSOR = (1UL << PIN_USER_LED_2_PIN))
#define BOARD_USER_LED_2_OFF() (PIN_USER_LED_2_GPIO->PCOR = (1UL << PIN_USER_LED_2_PIN))
#define BOARD_USER_LED_2_TGL() (PIN_USER_LED_2_GPIO->PTOR = (1UL << PIN_USER_LED_2_PIN))

/* MB_TP_26 control macro. */
#define PIN_MB_TP_26_GPIO      (GPIOC)    /*< GPIO periphery. */
#define PIN_MB_TP_26_PIN       (7U)       /*< Pin number. */
#define BOARD_MB_TP_26_HI()    (PIN_MB_TP_26_GPIO->PSOR = (1UL << PIN_MB_TP_26_PIN))
#define BOARD_MB_TP_26_LO()    (PIN_MB_TP_26_GPIO->PCOR = (1UL << PIN_MB_TP_26_PIN))


/*                   GPIO output pin initialization table.                    */
/*
 * The GPIO table entry macro, which converts the GPIOA..GPIOE address to
 * index 0..4.
 * -gpio: The GPIO periphery address
 */
#define PIN_GPIO2IDX(gpio) ((((uint32_t)(gpio)) >> 6U) & 0x7UL)

/*
 * This table lists the GPIO pins, which should be configured as outputs.
 * |     GPIO Periphery Address        |     Pin Number    | Output Value |
 */
#define PIN_GPIO_INIT_TABLE {\
    {PIN_GPIO2IDX(PIN_MCU_BRAKE_GPIO),  PIN_MCU_BRAKE_PIN,       0U       },\
    {PIN_GPIO2IDX(PIN_RELAY_GPIO),      PIN_RELAY_PIN,           0U       },\
    {PIN_GPIO2IDX(PIN_USER_LED_GPIO),   PIN_USER_LED_PIN,        0U       },\
    {PIN_GPIO2IDX(PIN_USER_LED_2_GPIO), PIN_USER_LED_2_PIN,      0U       },\
    {PIN_GPIO2IDX(PIN_MB_TP_26_GPIO),   PIN_MB_TP_26_PIN,        0U       },\
    {PIN_GPIO2IDX(GPIOA),               1U,                      0U       },\
    {PIN_GPIO2IDX(GPIOA),               14U,                     0U       },\
    {PIN_GPIO2IDX(GPIOA),               15U,                     0U       },\
    {PIN_GPIO2IDX(GPIOA),               16U,                     0U       },\
    {PIN_GPIO2IDX(GPIOA),               17U,                     0U       },\
    {PIN_GPIO2IDX(GPIOA),               18U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               3U,                      0U       },\
    {PIN_GPIO2IDX(GPIOB),               9U,                      0U       },\
    {PIN_GPIO2IDX(GPIOB),               10U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               11U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               18U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               20U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               21U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               22U,                     0U       },\
    {PIN_GPIO2IDX(GPIOB),               23U,                     0U       },\
    {PIN_GPIO2IDX(GPIOC),               6U,                      0U       },\
    {PIN_GPIO2IDX(GPIOC),               14U,                     0U       },\
    {PIN_GPIO2IDX(GPIOC),               15U,                     0U       },\
    {PIN_GPIO2IDX(GPIOC),               16U,                     0U       },\
    {PIN_GPIO2IDX(GPIOC),               17U,                     0U       },\
    {PIN_GPIO2IDX(GPIOC),               18U,                     0U       },\
    {PIN_GPIO2IDX(GPIOD),               0U,                      0U       },\
    {PIN_GPIO2IDX(GPIOD),               1U,                      0U       },\
    {PIN_GPIO2IDX(GPIOD),               2U,                      0U       },\
    {PIN_GPIO2IDX(GPIOD),               3U,                      0U       },\
    {PIN_GPIO2IDX(GPIOD),               7U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               0U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               2U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               3U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               4U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               5U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               6U,                      0U       },\
    {PIN_GPIO2IDX(GPIOE),               16U,                     0U       },\
    {PIN_GPIO2IDX(GPIOE),               18U,                     0U       },\
    {PIN_GPIO2IDX(GPIOE),               19U,                     0U       },\
    {PIN_GPIO2IDX(GPIOE),               24U,                     0U       },\
    {PIN_GPIO2IDX(GPIOE),               25U,                     0U       },\
    {PIN_GPIO2IDX(GPIOE),               26U,                     0U       }\
}


/*******************************************************************************
 * FreeMASTER interface configuration
 ******************************************************************************/
/* FreeMASTER serial port and baud rate. */
#define BOARD_FMSTR_UART_PORT      (UART0)
#define BOARD_FMSTR_UART_BAUDRATE  (19200U)

/* UART port configuration. */
#define PIN_UART_RXD_PORT          (PORTB)
#define PIN_UART_RXD_PIN           (16U)
#define PIN_UART_RXD_MUX           (3U)
#define PIN_UART_TXD_PORT          (PORTB)
#define PIN_UART_TXD_PIN           (17U)
#define PIN_UART_TXD_MUX           (3U)

/*******************************************************************************
 * API
 ******************************************************************************/
/* List of IRQ handlers. See main.c for their definition. */

/* The default IRQ handler is executed for all unassigned vectors. */
extern void IRQHNDLR_fsDefault(void);

/* The startup IRQ handler. */
extern void IRQHNDLR_fsStart(void);

/* The non-maskable IRQ handler. */
extern void IRQHNDLR_fsNMI(void);

/* The hard-fault IRQ handler. */
extern void IRQHNDLR_fsHardFault(void);

/* The ADC0 periphery IRQ handler. */
extern void IRQHNDLR_fsADC0(void);

/* The PDB periphery IRQ handler. */
extern void IRQHNDLR_fsPDB(void);

/* The FTM2 periphery IRQ handler. */
extern void IRQHNDLR_fsFTM2(void);

#endif  /* _HARDWARE_CFG_H_ */
