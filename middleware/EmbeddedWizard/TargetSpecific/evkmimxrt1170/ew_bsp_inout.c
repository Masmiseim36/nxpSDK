/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   This template provides access to some LEDs and buttons of the board.
*
*******************************************************************************/

#include "board.h"
#include "fsl_gpio.h"

#include "ewconfig.h"
#include "ew_bsp_inout.h"

#define EwBspGGpioIrqHandler BOARD_USER_BUTTON_IRQ_HANDLER

static TButtonCallback        ButtonCallback = NULL;


/*******************************************************************************
* FUNCTION:
*   EwBspGGpioIrqHandler
*
* DESCRIPTION:
*   The GPIO interrupt callback function.
*
* ARGUMENTS:
*   GPIO_Pin - the gpio pin.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
  uint32_t  portState;
void EwBspGGpioIrqHandler(void)
{
  int       pinState = 1;

  /* clear the interrupt status */
  GPIO_PortClearInterruptFlags( BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN );

  if ( GPIO_PinRead( BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN ) )
    pinState = 0;

  if ( ButtonCallback )
    ButtonCallback( pinState );

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
    __DSB();
#endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspInOutInitButton
*
* DESCRIPTION:
*   Configures one hardware button of the board used for demo applications.
*
* ARGUMENTS:
*   aButtonCallback - The button callback.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspInOutInitButton( TButtonCallback aButtonCallback )
{
  /* Define the init structure for the input switch pin */
  gpio_pin_config_t sw_config = { kGPIO_DigitalInput, 0, kGPIO_IntRisingOrFallingEdge };

  /* Init input switch GPIO. */
  EnableIRQ( BOARD_USER_BUTTON_IRQ );
  GPIO_PinInit( BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &sw_config );

  /* Enable GPIO pin interrupt */
  GPIO_PortEnableInterrupts( BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN );

  ButtonCallback = aButtonCallback;
}


/*******************************************************************************
* FUNCTION:
*   EwBspInOutInitLed
*
* DESCRIPTION:
*   Configures one LED of the board used for demo applications.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspInOutInitLed( void )
{
  /* Define the init structure for the output LED pin */
  gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

  /* Init output LED GPIO. */
  GPIO_PinInit( BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config );
}


/*******************************************************************************
* FUNCTION:
*   EwBspInOutLedOn
*
* DESCRIPTION:
*   Switch LED on (used for demo applications).
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspInOutLedOn( void )
{
  GPIO_PinWrite( BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 1 );
}


/*******************************************************************************
* FUNCTION:
*   EwBspInOutLedOff
*
* DESCRIPTION:
*   Switch LED off (used for demo applications).
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspInOutLedOff( void )
{
  GPIO_PinWrite( BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 0 );
}


/* mli */
