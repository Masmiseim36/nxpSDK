#ifndef PIN_MUX_H
#define PIN_MUX_H


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

#define BOARD_INITBUTTONS_GPIOC_2_DIRECTION              kPIN_MUX_DirectionInput   /*!< Direction of GPIOC_2 signal */
#define BOARD_INITBUTTONS_GPIOB_18_DIRECTION             kPIN_MUX_DirectionInput   /*!< Direction of GPIOB_18 signal */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtons(void);

#define BOARD_INITLEDS_GPIOB_3_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of GPIOB_3 signal */
#define BOARD_INITLEDS_GPIOB_2_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of GPIOB_2 signal */
#define BOARD_INITLEDS_GPIOC_1_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of GPIOC_1 signal */
#define BOARD_INITLEDS_GPIOA_16_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction of GPIOA_16 signal */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

#define BOARD_INITRGB_TPM0_CH2_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of TPM0_CH2 signal */
#define BOARD_INITRGB_TPM0_CH0_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of TPM0_CH0 signal */
#define BOARD_INITRGB_TPM1_CH0_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of TPM1_CH0 signal */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitRGB(void);

#define BOARD_INITSPI_SPI0_SCK_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction of SPI0_SCK signal */
#define BOARD_INITSPI_SPI0_PCS0_SS_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction of SPI0_PCS0_SS signal */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSPI(void);

#define BOARD_INITLPUART_LPUART0_TX_DIRECTION           kPIN_MUX_DirectionOutput   /*!< Direction of LPUART0_TX signal */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPUART(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* PIN_MUX_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
