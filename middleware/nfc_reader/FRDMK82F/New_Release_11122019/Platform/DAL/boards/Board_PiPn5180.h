/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  RS: Generated 24. Jan 2017
*
*/

#ifndef BOARD_PIPN5180_H
#define BOARD_PIPN5180_H

/*******************************************************************
 *  Enable User/Kernel space SPI.
 ******************************************************************/
#define PHDRIVER_LINUX_USER_SPI
/* #define PHDRIVER_LINUX_KERNEL_SPI */

/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/
#define PHDRIVER_PIN_RESET         7   /**< "/sys/class/gpio/gpio7/" */
#define PHDRIVER_PIN_IRQ           23  /**< "/sys/class/gpio/gpio23/" */
#define PHDRIVER_PIN_BUSY          25  /**< "/sys/class/gpio/gpio25/" */
#define PHDRIVER_PIN_DWL           18  /**< "/sys/class/gpio/gpio18/" */

/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_BUSY_PULL_CFG     PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_DWL_PULL_CFG      PH_DRIVER_PULL_UP

/******************************************************************
 * IRQ & BUSY PIN TRIGGER settings
 ******************************************************************/
#define PIN_IRQ_TRIGGER_TYPE         PH_DRIVER_INTERRUPT_RISINGEDGE
#define PIN_BUSY_TRIGGER_TYPE        PH_DRIVER_INTERRUPT_FALLINGEDGE

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL       PH_DRIVER_SET_LOW
#define RESET_POWERUP_LEVEL         PH_DRIVER_SET_HIGH

/*****************************************************************
 * SPI Configuration
 ****************************************************************/
#ifdef PHDRIVER_LINUX_USER_SPI
#    define PHDRIVER_USER_SPI_BUS                    0   /**< "/dev/spidev0.0" */
#    define PHDRIVER_USER_SPI_CS                     0   /**< "/dev/spidev0.0" */
#    define PHDRIVER_USER_SPI_FREQ                   5000000 /**< 5 MHz. */
#    define PHDRIVER_USER_SPI_CFG_DIR                "/dev/spidev"
#    define PHDRIVER_USER_SPI_CFG_MODE               SPI_MODE_0
#    define PHDRIVER_USER_SPI_CFG_BITS_PER_WORD      8
#endif

#ifdef PHDRIVER_LINUX_KERNEL_SPI
#    define PHDRIVER_KERNEL_SPI_ID                   0x11U       /**< ID for Linux Kernel Spi BAL component */
#    define PHDRIVER_KERNEL_SPI_CFG_DIR              "/dev/bal"
#endif

/*****************************************************************
 * Dummy entries
 * No functionality. To suppress build error in HAL. No pin functionality in SPI Linux BAL.
 *****************************************************************/
#define PHDRIVER_PIN_SSEL                            0xFFFF
#define PHDRIVER_PIN_NSS_PULL_CFG                    PH_DRIVER_PULL_UP

/*****************************************************************
 * STATUS LED Configuration
 ****************************************************************/
#define PHDRIVER_LED_SUCCESS_DELAY      2

#define PHDRIVER_LED_FAILURE_DELAY_MS   250
#define PHDRIVER_LED_FAILURE_FLICKER    4

#endif /* BOARD_PIPN5180_H */
