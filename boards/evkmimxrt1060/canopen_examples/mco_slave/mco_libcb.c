/**************************************************************************
MODULE:    MCO_LIBCB
CONTAINS:  MicroCANopen Lib Helper Functions
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#include "mcop_inc.h"
#include "fsl_gpio.h"

/**************************************************************************
DEFINITIONS
***************************************************************************/
// Ports for LEDs
#define LED_NR_MAX 1

/**************************************************************************
GLOBAL FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Call-back function to return the size of the process image
RETURNS: Size of the process image
**************************************************************************/
int LIBCB_GetProcImgSize(void)
{
    return PROCIMG_SIZE;
}

/**************************************************************************
DOES:    Initialize I/O for LEDs.
RETURNS: nothing
**************************************************************************/
void LIBCB_InitLeds(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config);
    USER_LED_INIT(0);

    return;
}

/**************************************************************************
DOES:    This function turns a numbered LED on or off.
RETURNS: nothing
**************************************************************************/
void LIBCB_SwitchLeds(uint8_t LED_NR, // LED number
                      uint8_t state   // LED state
)
{
    if (LED_NR == 0)
    {
        if (state)
            USER_LED_ON();
        else
            USER_LED_OFF();
    }
    return;
}

/**************************************************************************
DOES:    Call-back function to return the size of non-volatile
         memory for stack use.
RETURNS: Size of the non-volatile memory for stack use
**************************************************************************/
int LIBCB_GetNvolSize(void)
{
    return NVOL_STORE_SIZE;
}

/**************************************************************************
DOES:    Call-back function to return the start/offset of non-volatile
         memory for stack use.
RETURNS: Start/offset of non-volatile memory for stack use
**************************************************************************/
int LIBCB_GetNvolStart(void)
{
    return NVOL_STORE_START;
}

/**************************************************************************
END-OF-FILE
***************************************************************************/
