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

#ifndef BOARD_LPC1769PN5180_H
#define BOARD_LPC1769PN5180_H

#include <board.h>

#define PORT0               0          /**< Port0. */
#define PORT1               1          /**< Port1. */
#define PORT2               2          /**< Port2. */
#define PORT3               3          /**< Port3. */
#define PORT4               4          /**< Port4. */

/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/
#define PHDRIVER_PIN_RESET         ((PORT2<<8) | 5)    /**< Reset pin, Port 2, pin5. */
#define PHDRIVER_PIN_IRQ           ((PORT2<<8) | 12)   /**< Interrupt pin from Frontend to Host, Port2, pin12 */
#define PHDRIVER_PIN_BUSY          ((PORT2<<8) | 11)   /**< Frontend's Busy Status, Port2, pin11 */
#define PHDRIVER_PIN_DWL           ((PORT0<<8) | 21)   /**< Download mode pin of Frontend, Port0, pin21 */

/* These pins are used for EMVCo Interoperability test status indication,
 * not for the generic Reader Library implementation.
 */
#define PHDRIVER_PIN_RLED          ((PORT2<<8) | 0)    /**< RED LED, Port2, pin0, Pin function 0 */
#define PHDRIVER_PIN_GLED          ((PORT3<<8) | 25)   /**< GREEN LED, Port3, pin25, Pin function 0 */
#define PHDRIVER_PIN_SUCCESS       ((PORT0<<8) | 2)    /**< GPIO, Port0, pin2, Pin function 0 */
#define PHDRIVER_PIN_FAIL          ((PORT0<<8) | 3)    /**< GPIO, Port0, pin3, Pin function 0 */

/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_BUSY_PULL_CFG     PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_DWL_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_NSS_PULL_CFG      PH_DRIVER_PULL_UP

/******************************************************************
 * IRQ PIN NVIC settings
 ******************************************************************/
#define PIN_IRQ_TRIGGER_TYPE    PH_DRIVER_INTERRUPT_RISINGEDGE  /**< IRQ pin RISING edge interrupt */
#define EINT_PRIORITY           5                /**< Interrupt priority. */
#define EINT_IRQn               EINT3_IRQn       /**< NVIC IRQ */

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL PH_DRIVER_SET_LOW
#define RESET_POWERUP_LEVEL   PH_DRIVER_SET_HIGH


/*****************************************************************
 * SPI Configuration
 ****************************************************************/
#define LPC_SSP             LPC_SSP0   /**< SPI Module */
#define SSP_CLOCKRATE       4000000    /**< SPI clock rate. Allowed clock rate: 6 or 4 or 3 or 2.4MHz etc. */

 /******************************************************************/
/* Pin configuration format : Its a 32 bit format where 1st 3 bytes represents each field as shown below.
 * | Byte3 | Byte2 | Byte1 | Byte0 |
 * | ---   | FUNC  | PORT  | PIN   |
 * */
#define PHDRIVER_PIN_MOSI     ((IOCON_FUNC2<<16) | (PORT0<<8) | 18)  /**< MOSI pin, Port0, pin18, Pin function 2 */
#define PHDRIVER_PIN_MISO     ((IOCON_FUNC2<<16) | (PORT0<<8) | 17)  /**< MISO pin, Port0, pin17, Pin function 2 */
#define PHDRIVER_PIN_SCK      ((IOCON_FUNC2<<16) | (PORT0<<8) | 15)  /**< Clock pin, Port0, pin15, Pin function 2 */
#define PHDRIVER_PIN_SSEL     ((IOCON_FUNC0<<16) | (PORT0<<8) | 16)  /**< Slave select, Port0, pin16, Pin function 0 */


/*****************************************************************
 * Timer Configuration
 ****************************************************************/
#define PH_DRIVER_LPC_TIMER                    LPC_TIMER0           /**< Use LPC timer0 */
#define PH_DRIVER_LPC_TIMER_CLK                SYSCTL_CLOCK_TIMER0  /**< Timer 0 clock source */
#define PH_DRIVER_LPC_TIMER_MATCH_REGISTER     0x01  /* use match register 1 always. */
#define PH_DRIVER_LPC_TIMER_IRQ                TIMER0_IRQn          /**< NVIC Timer0 Irq */
#define PH_DRIVER_LPC_TIMER_IRQ_HANDLER        TIMER0_IRQHandler    /**< Timer0 Irq Handler */
#define PH_DRIVER_LPC_TIMER_IRQ_PRIORITY       5                    /**< NVIC Timer0 Irq priority */

#endif /* BOARD_LPC1769PN5180_H */
