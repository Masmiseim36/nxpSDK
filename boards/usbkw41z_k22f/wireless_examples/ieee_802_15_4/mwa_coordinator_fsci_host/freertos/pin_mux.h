/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTB2 (coord E7), SW1 */
#define BOARD_INITBUTTONS_SW1_GPIO                                         GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITBUTTONS_SW1_PORT                                         PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITBUTTONS_SW1_GPIO_PIN                                        2U   /*!< PORTB pin index: 2 */
#define BOARD_INITBUTTONS_SW1_PIN_NAME                                      PTB2   /*!< Pin name */
#define BOARD_INITBUTTONS_SW1_LABEL                                        "SW1"   /*!< Label */
#define BOARD_INITBUTTONS_SW1_NAME                                         "SW1"   /*!< Identifier name */
#define BOARD_INITBUTTONS_SW1_DIRECTION                  kPIN_MUX_DirectionInput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtons(void);

/* PORTC6 (coord A7), LED */
#define BOARD_INITLEDS_LED_GPIO                                            GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLEDS_LED_PORT                                            PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLEDS_LED_GPIO_PIN                                           6U   /*!< PORTC pin index: 6 */
#define BOARD_INITLEDS_LED_PIN_NAME                                         PTC6   /*!< Pin name */
#define BOARD_INITLEDS_LED_LABEL                                           "LED"   /*!< Label */
#define BOARD_INITLEDS_LED_NAME                                            "LED"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_DIRECTION                    kPIN_MUX_DirectionOutput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitRGB(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSPI(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUART(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPUART(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
