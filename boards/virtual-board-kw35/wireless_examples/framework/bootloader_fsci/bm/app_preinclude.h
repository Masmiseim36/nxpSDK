/*! *********************************************************************************
* Copyright 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/**************************************************/
/** USER SETTINGS OF KINETIS MCU */
/**  Kinetis ARM Cortex-M0+ model */
#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/*  In the case of using USB VIRTUAL SERIAL LINK you must activate No break TRIM CHECKBOX in the master AN2295 PC Application 
 *  the break impulse is replaced by using only 9 bits zero impulse 
 *  BREAK IMPULSE       |START| |0| |0| |0| |0| |0| |0| |0| |0| |0| |0| STOP| 
 *  ZERO IMPULSE        |START| |0| |0| |0| |0| |0| |0| |0| |0| |0| |STOP|
 */
#define BOOTLOADER_SHORT_TRIM            (1)

/** Kinetis RAM */
#define KINETIS_RAM_START                (0x1FFFC000)
#define KINETIS_RAM_END                  (0x2000BFFF)

/* Disable NMI pin function */
#define gFlashConfigOptions_d            (0xFFFFFBFE)

/* Serial Interfaces used */
#define gBoot_UseUart_d                  (1)
#define gBoot_UseSpiSlave_d              (1)


/**************************************************/
/* UART module configuration                      */
/**************************************************/
#define USE_LPUART                       (1)

#define BOOT_UART_CLOCK                  (32000000U)
#define BOOT_UART_BAUD_RATE              (115200U)

#define BOOT_UART_IRQ                    (LPUART0_IRQn)

#define BOOT_UART_BASE                   (LPUART0)
#define BOOT_UART_SIM_SCG_REG            (SIM->SCGC5)
#define BOOT_UART_SIM_SCG_MASK           (SIM_SCGC5_LPUART0_MASK)
/** GPIO & UART pins initialization */
#define BOOT_UART_GPIO_PORT_RX           (PORTC)
#define BOOT_UART_GPIO_PORT_TX           (PORTC)
#define BOOT_UART_GPIO_PORT_SIM_SCG_REG  (SIM->SCGC5)
#define BOOT_UART_GPIO_PORT_SIM_SCG_MASK (SIM_SCGC5_PORTC_MASK)
/*  setting of multiplexer for UART alternative of pin */
#define BOOT_PIN_UART_ALTERNATIVE        (4U)
#define BOOT_UART_GPIO_PIN_RX            (6U)
#define BOOT_UART_GPIO_PIN_TX            (7U)

/*  setting of multiplexer for GPIO alternative of pin */
#define BOOT_PIN_GPIO_ALTERNATIVE        (1U)

/**************************************************/
/* SPI module configuration                       */
/**************************************************/
#define USE_DSPI                         (1)

/* SPI Master */
#define BOOT_SPI_BasePtr                 (SPI0)
#define BOOT_SPI_PORT_SIM_SCG            (SIM->SCGC5)
#define cSPI_PORT_SIM_SCG_Config_c       (SIM_SCGC5_PORTC_MASK)
#define BOOT_SPI_SIM_SCG                 (SIM->SCGC6)
#define cSPI_SIM_SCG_Config_c            (SIM_SCGC6_SPI0_MASK)

#define BOOT_SPI_ALTERNATIVE             (2)
#define BOOT_SPI_SCLK_PCR                (PORTC->PCR[16])
#define BOOT_SPI_MOSI_PCR                (PORTC->PCR[17])
#define BOOT_SPI_MISO_PCR                (PORTC->PCR[18])
#define BOOT_SPI_SSEL_PCR                (PORTC->PCR[19])

#define BOOT_SPI_SSEL_PCOR               (GPIOC->PCOR)
#define BOOT_SPI_SSEL_PSOR               (GPIOC->PSOR)
#define BOOT_SPI_SSEL_PDDR               (GPIOC->PDDR)
#define BOOT_SPI_SSEL_PIN_Number_c       (19)

/* SPI Slave */
#define BOOT_SPI_Slave_IRQ               (SPI1_IRQn)
#define BOOT_SPI_Slave_BasePtr           (SPI1)
#define BOOT_SPI_Slave_PORT_SIM_SCG      (SIM->SCGC5)
#define cSPI_Slave_PORT_SIM_SCG_Config_c (SIM_SCGC5_PORTA_MASK)
#define BOOT_SPI_Slave_SIM_SCG           (SIM->SCGC6)
#define cSPI_Slave_SIM_SCG_Config_c      (SIM_SCGC6_SPI1_MASK)

#define BOOT_SPI_Slave_SCLK_PCR          (PORTA->PCR[18])
#define BOOT_SPI_Slave_MOSI_PCR          (PORTA->PCR[17])
#define BOOT_SPI_Slave_MISO_PCR          (PORTA->PCR[16])
#define BOOT_SPI_Slave_SSEL_PCR          (PORTA->PCR[19])
#define BOOT_SPI_Slave_ALTERNATIVE       (2)

#define BOOT_SPI_Slave_DAP_SCG_REG       (SIM->SCGC5)
#define BOOT_SPI_Slave_DAP_SCG_MASK      (SIM_SCGC5_PORTC_MASK)
#define BOOT_SPI_Slave_DAP_PCR           (PORTC->PCR[1])
#define BOOT_SPI_Slave_DAP_PDDR          (GPIOC->PDDR)
#define BOOT_SPI_Slave_DAP_PSOR          (GPIOC->PSOR)
#define BOOT_SPI_Slave_DAP_PCOR          (GPIOC->PCOR)
#define BOOT_SPI_Slave_DAP_pin           (1)

/**************************************************/
/* FSCI Boot trigger pin config                   */
/**************************************************/
#ifndef BOOT_PIN_ENABLE_SIM_SCG_REG
#define BOOT_PIN_ENABLE_SIM_SCG_REG      (SIM->SCGC5)
#endif

#ifndef BOOT_PIN_ENABLE_SIM_SCG_MASK
#define BOOT_PIN_ENABLE_SIM_SCG_MASK     (SIM_SCGC5_PORTC_MASK)
#endif

#ifndef BOOT_PIN_ENABLE_PORT_BASE
#define BOOT_PIN_ENABLE_PORT_BASE        (PORTC)
#endif

#ifndef BOOT_PIN_ENABLE_GPIO_BASE
#define BOOT_PIN_ENABLE_GPIO_BASE        (GPIOC)
#endif

#ifndef BOOT_PIN_ENABLE_NUM
#define BOOT_PIN_ENABLE_NUM              (2)
#endif

/**************************************************/
/* NMI Pin settings                               */
/**************************************************/
#define NMI_PIN_SIM_SCG_REG              (SIM->SCGC5)
#define NMI_PIN_SIM_SCG_MASK             (SIM_SCGC5_PORTB_MASK)
#define NMI_PIN_PORT_BASE                (PORTB)
#define NMI_PIN_NUM                      (18)
/* Unused Pin ALT set for disabling the NMI PIN */
#define NMI_PIN_ALT                      (9)

/**************************************************/
/** CALIBRATION OF BOOTLOADER TRIM SETTINGS */
#define BOOT_CALIBRATION_GPIO_BASE       (PTC_BASE_PTR)

#endif/*__APP_PREINCLUDE_H__ */
