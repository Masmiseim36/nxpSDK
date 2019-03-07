/*
 * Copyright 2018 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* PTA17 (number AD19), I2C0_SDA */
#define BOARD_INITPINS_I2C0_SDA_PERIPHERAL                                 FXIO0   /*!< Device name: FXIO0 */
#define BOARD_INITPINS_I2C0_SDA_SIGNAL                                    fxio_d   /*!< FXIO0 signal: fxio_d */
#define BOARD_INITPINS_I2C0_SDA_CHANNEL                                        1   /*!< FXIO0 fxio_d channel: 1 */
#define BOARD_INITPINS_I2C0_SDA_PIN_NAME                                   PTA17   /*!< Pin name */
#define BOARD_INITPINS_I2C0_SDA_PIN_FUNCTION_ID            IOMUXC_PTA17_FXIO0_D1   /*!< Pin function id */
#define BOARD_INITPINS_I2C0_SDA_LABEL                                 "I2C0_SDA"   /*!< Label */
#define BOARD_INITPINS_I2C0_SDA_NAME                                  "I2C0_SDA"   /*!< Identifier name */

/* PTA16 (number AG18), I2C0_SCL */
#define BOARD_INITPINS_I2C0_SCL_PERIPHERAL                                 FXIO0   /*!< Device name: FXIO0 */
#define BOARD_INITPINS_I2C0_SCL_SIGNAL                                    fxio_d   /*!< FXIO0 signal: fxio_d */
#define BOARD_INITPINS_I2C0_SCL_CHANNEL                                        0   /*!< FXIO0 fxio_d channel: 0 */
#define BOARD_INITPINS_I2C0_SCL_PIN_NAME                                   PTA16   /*!< Pin name */
#define BOARD_INITPINS_I2C0_SCL_PIN_FUNCTION_ID            IOMUXC_PTA16_FXIO0_D0   /*!< Pin function id */
#define BOARD_INITPINS_I2C0_SCL_LABEL                                 "I2C0_SCL"   /*!< Label */
#define BOARD_INITPINS_I2C0_SCL_NAME                                  "I2C0_SCL"   /*!< Identifier name */

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

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
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: Cortex-M4[cm4] */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
