/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_XBAR_H_
#define _FSL_XBAR_H_

#include "fsl_common.h"

/*!
 * @addtogroup xbar
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FSL_XBAR_DRIVER_VERSION (MAKE_VERSION(2, 0, 2)) /*!< Version 2.0.2. */

#define XBAR_SELx(base, output)           \
    (*(volatile uint16_t *)((uintptr_t) & \
                            (base->SEL0) + ((output) / 2U) * 2U)) /*!< Macros for entire XBAR_SELx register. */

#define XBAR_WR_SELx_SELx(base, input, output)                                                   \
    (XBAR_SELx((base), (output)) =                                                               \
         ((XBAR_SELx((base), (output)) & ~(0xFFU << (XBAR_SEL0_SEL1_SHIFT * ((output) % 2U)))) | \
          ((input) << (XBAR_SEL0_SEL1_SHIFT * ((output) % 2U))))) /*!< Set the XBAR_SELx_SELx field to a new value. */

/*!
 * @brief XBAR active edge for detection
 */
typedef enum _xbar_active_edge
{
    kXBAR_EdgeNone = 0U,            /*!< Edge detection status bit never asserts. */
    kXBAR_EdgeRising = 1U,          /*!< Edge detection status bit asserts on rising edges. */
    kXBAR_EdgeFalling = 2U,         /*!< Edge detection status bit asserts on falling edges. */
    kXBAR_EdgeRisingAndFalling = 3U /*!< Edge detection status bit asserts on rising and falling edges. */

} xbar_active_edge_t;

/*!
 * @brief Defines the XBAR DMA and interrupt configurations.
 */
typedef enum _xbar_request
{
    kXBAR_RequestDisable = 0U,        /*!< Interrupt and DMA are disabled. */
    kXBAR_RequestDMAEnable = 1U,      /*!< DMA enabled, interrupt disabled. */
    kXBAR_RequestInterruptEnalbe = 2U /*!< Interrupt enabled, DMA disabled. */
} xbar_request_t;

/*!
 * @brief XBAR status flags.
 *
 * This provides constants for the XBAR status flags for use in the XBAR
 * functions.
 */
typedef enum _xbar_status_flag_t
{
    kXBAR_EdgeDetectionOut0 =
        (XBAR_CTRL0_STS0_MASK), /*!< XBAR_OUT0 active edge interrupt flag, sets when active edge detected. */
    kXBAR_EdgeDetectionOut1 =
        (XBAR_CTRL0_STS1_MASK), /*!< XBAR_OUT1 active edge interrupt flag, sets when active edge detected. */
    kXBAR_EdgeDetectionOut2 =
        (XBAR_CTRL1_STS2_MASK << 16U), /*!< XBAR_OUT2 active edge interrupt flag, sets when active edge detected. */
    kXBAR_EdgeDetectionOut3 =
        (XBAR_CTRL1_STS3_MASK << 16U), /*!< XBAR_OUT3 active edge interrupt flag, sets when active edge detected. */
} xbar_status_flag_t;

/*!
 * @brief Defines the configuration structure of the XBAR control register.
 *
 * This structure keeps the configuration of XBAR control register for one output.
 * Control registers are available only for a few outputs. Not every XBAR module has
 * control registers.
 */
typedef struct _xbar_control_config
{
    xbar_active_edge_t activeEdge; /*!< Active edge to be detected. */
    xbar_request_t requestType;    /*!< Selects DMA/Interrupt request. */
} xbar_control_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name XBAR functional Operation
 * @{
 */

/*!
 * @brief Initializes the XBAR modules.
 *
 * This function un-gates the XBAR clock.
 *
 * @param base XBAR peripheral address.
 */
void XBAR_Init(XBAR_Type *base);

/*!
 * @brief Shutdown the XBAR modules.
 *
 * This function disables XBAR clock.
 *
 * @param base XBAR peripheral address.
 */
void XBAR_Deinit(XBAR_Type *base);

/*!
 * @brief Set connection between the selected XBAR_IN[*] input and the XBAR_OUT[*] output signal.
 *
 * This function connects the XBAR input to the selected XBAR output.
 * If more than one XBAR module is available, only the inputs and outputs from the same module
 * can be connected.
 *
 * Example:
   @code
   XBAR_SetSignalsConnection(XBAR, kXBAR_InputTMR_CH0_Output, kXBAR_OutputXB_DMA_INT2);
   @endcode
 *
 * @param base XBAR peripheral address
 * @param input XBAR input signal.
 * @param output XBAR output signal.
 */
void XBAR_SetSignalsConnection(XBAR_Type *base, xbar_input_signal_t input, xbar_output_signal_t output);

/*!
 * @brief Clears the the edge detection status flags of relative mask.
 *
 * @param base XBAR peripheral address
 * @param mask the status flags to clear.
 */
void XBAR_ClearStatusFlags(XBAR_Type *base, uint32_t mask);

/*!
 * @brief Gets the active edge detection status.
 *
 * This function gets the active edge detect status of all XBAR_OUTs. If the
 * active edge occurs, the return value is asserted. When the interrupt or the DMA
 * functionality is enabled for the XBAR_OUTx, this field is 1 when the interrupt
 * or DMA request is asserted and 0 when the interrupt or DMA request has been
 * cleared.
 *
 * Example:
   @code
   uint32_t status;

   status = XBAR_GetStatusFlags(XBAR);
   @endcode
 *
 * @param base XBAR peripheral address.
 * @return the mask of these status flag bits.
 */
uint32_t XBAR_GetStatusFlags(XBAR_Type *base);

/*!
 * @brief Configures the XBAR control register.
 *
 * This function configures an XBAR control register. The active edge detection
 * and the DMA/IRQ function on the corresponding XBAR output can be set.
 *
 * Example:
   @code
   xbar_control_config_t userConfig;
   userConfig.activeEdge = kXBAR_EdgeRising;
   userConfig.requestType = kXBAR_RequestInterruptEnalbe;
   XBAR_SetOutputSignalConfig(XBAR, kXBAR_OutputXB_DMA_INT0, &userConfig);
   @endcode
 *
 * @param base XBAR peripheral address
 * @param output XBAR output number.
 * @param controlConfig Pointer to structure that keeps configuration of control register.
 */
void XBAR_SetOutputSignalConfig(XBAR_Type *base,
                                xbar_output_signal_t output,
                                const xbar_control_config_t *controlConfig);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @}*/

/*! @}*/

#endif /* _FSL_XBAR_H_ */
