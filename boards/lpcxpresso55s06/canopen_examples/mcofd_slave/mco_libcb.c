/**************************************************************************
MODULE:    MCO_LIBCB
CONTAINS:  MicroCANopen Lib Helper Functions
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2019
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.01, EmSA 02-APR-20
           $LastChangedDate: 2020-04-02 17:30:41 +0200 (Thu, 02 Apr 2020) $
           $LastChangedRevision: 4909 $
***************************************************************************/

#include "mcop_inc.h"
#include "fsl_gpio.h"

/**************************************************************************
DEFINITIONS
***************************************************************************/
// Ports for LEDs
#if defined(CPU_LPC54628) || defined(CPU_LPC54608) || defined(CPU_LPC54618) || defined(CPU_LPC54S018)

#define LED_NR_MAX 2

#define LED_PORT_MAX 3

#define LED_PORT(nr) (nr == 0 ? 3 : nr == 1 ? 3 : nr == 2 ? 2 : 3)

#define LED_PIN(nr) (nr == 0 ? 14 : nr == 1 ? 3 : nr == 2 ? 2 : 14)

#elif defined(LPC55S16_SERIES) || defined(LPC55S06_SERIES)

#define LED_NR_MAX 2

#define LED_PORT_MAX 1

#define LED_PORT(nr) (1)

#define LED_PIN(nr) (nr == 0 ? 7 : nr == 1 ? 4 : nr == 2 ? 6 : 7)

#endif

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
    int i;
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };
    uint8_t ports[LED_PORT_MAX + 1];

    // Find all used ports, only initialize them once
    for (i = 0; i <= LED_PORT_MAX; i++)
    {
        ports[i] = FALSE;
    }
    for (i = 0; i <= LED_NR_MAX; i++)
    {
        ports[LED_PORT(i)] = TRUE;
    }
    for (i = 0; i <= LED_PORT_MAX; i++)
    {
        if (ports[i])
        {
            /* Init output LED port. */
            GPIO_PortInit(GPIO, LED_PORT(i));
        }
    }

    for (i = 0; i <= LED_NR_MAX; i++)
    {
        /* Init output LED pin. */
        GPIO_PinInit(GPIO, LED_PORT(i), LED_PIN(i), &led_config);
        LIBCB_SwitchLeds(i, COLED_ON); // turn on LED
    }

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
    if (LED_NR <= LED_NR_MAX)
    {
        GPIO_PinWrite(GPIO, LED_PORT(LED_NR), LED_PIN(LED_NR), 1 - state);
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
