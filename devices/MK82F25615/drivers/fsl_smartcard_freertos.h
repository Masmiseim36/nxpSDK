/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#ifndef _FSL_SMARTCARD_RTOS_H_
#define _FSL_SMARTCARD_RTOS_H_

#include "fsl_smartcard.h"
#if (defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
#include "fsl_smartcard_emvsim.h"
#else
#include "fsl_smartcard_uart.h"
#endif
#if defined(USING_PHY_EMVSIM)
#include "fsl_smartcard_phy_emvsim.h"
#endif
#if defined(USING_PHY_NCN8025)
#include "fsl_smartcard_phy_ncn8025.h"
#endif
#if defined(USING_PHY_GPIO)
#include "fsl_smartcard_phy_gpio.h"
#endif
#include <FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>

/*!
 * @addtogroup smartcard_freertos_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief smart card RTOS transfer complete flag */
#define RTOS_SMARTCARD_COMPLETE 0x1u

/*! @brief smart card RTOS transfer time-out flag */
#define RTOS_SMARTCARD_TIMEOUT 0x2u

/*! @brief Common smart card driver API defines */
#if (defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
#define SMARTCARD_Control(base, context, control, param) \
    SMARTCARD_EMVSIM_Control(base, context, control, 0) /*!< Common smart card API macro */
#define SMARTCARD_Transfer(base, context, xfer) \
    SMARTCARD_EMVSIM_TransferNonBlocking(base, context, xfer) /*!< Common smart card API macro */
#define SMARTCARD_Init(base, context, sourceClockHz) \
    SMARTCARD_EMVSIM_Init(base, context, sourceClockHz)      /*!< Common smart card API macro */
#define SMARTCARD_Deinit(base) SMARTCARD_EMVSIM_Deinit(base) /*!< Common smart card API macro */
#define SMARTCARD_GetTransferRemainingBytes(base, context) \
    SMARTCARD_EMVSIM_GetTransferRemainingBytes(base, context) /*!< Common smart card API macro */
#define SMARTCARD_GetDefaultConfig(cardParams) \
    SMARTCARD_EMVSIM_GetDefaultConfig(cardParams) /*!< Common smart card API macro */
#else                                             /* smart card module is UART */
#define SMARTCARD_Control(base, context, control, param) \
    SMARTCARD_UART_Control(base, context, control, 0) /*!< Common smart card API macro */
#define SMARTCARD_Transfer(base, context, xfer) \
    SMARTCARD_UART_TransferNonBlocking(base, context, xfer) /*!< Common smart card API macro */
#define SMARTCARD_Init(base, context, sourceClockHz) \
    SMARTCARD_UART_Init(base, context, sourceClockHz)      /*!< Common smart card API macro */
#define SMARTCARD_Deinit(base) SMARTCARD_UART_Deinit(base) /*!< Common smart card API macro */
#define SMARTCARD_GetTransferRemainingBytes(base, context) \
    SMARTCARD_UART_GetTransferRemainingBytes(base, context) /*!< Common smart card API macro */
#define SMARTCARD_GetDefaultConfig(cardParams) \
    SMARTCARD_UART_GetDefaultConfig(cardParams) /*!< Common smart card API macro */
#endif /* (defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)) */

#if defined(USING_PHY_NCN8025)
#define SMARTCARD_PHY_Activate(base, context, resetType) \
    SMARTCARD_PHY_NCN8025_Activate(base, context, resetType) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Deactivate(base, context) \
    SMARTCARD_PHY_NCN8025_Deactivate(base, context) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Control(base, context, control, param) \
    SMARTCARD_PHY_NCN8025_Control(base, context, control, param) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Init(base, config, sourceClockHz) \
    SMARTCARD_PHY_NCN8025_Init(base, config, sourceClockHz) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Deinit(base, config)                                     \
    SMARTCARD_PHY_NCN8025_Deinit(base, config) /*!< Common smart card API macro \
                                                  */
#define SMARTCARD_PHY_GetDefaultConfig(config) \
    SMARTCARD_PHY_NCN8025_GetDefaultConfig(config) /*!< Common smart card API macro */
#endif                                             /* defined(USING_PHY_NCN8025) */

#if defined(USING_PHY_EMVSIM)
#define SMARTCARD_PHY_Activate(base, context, resetType) \
    SMARTCARD_PHY_EMVSIM_Activate(base, context, resetType) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Deactivate(base, context) \
    SMARTCARD_PHY_EMVSIM_Deactivate(base, context) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Control(base, context, control, param) \
    SMARTCARD_PHY_EMVSIM_Control(base, context, control, param) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Init(base, config, sourceClockHz) \
    SMARTCARD_PHY_EMVSIM_Init(base, config, sourceClockHz) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Deinit(base, config)                                    \
    SMARTCARD_PHY_EMVSIM_Deinit(base, config) /*!< Common smart card API macro \
                                                 */
#define SMARTCARD_PHY_GetDefaultConfig(config) \
    SMARTCARD_PHY_EMVSIM_GetDefaultConfig(config) /*!< Common smart card API macro */
#endif

#if defined(USING_PHY_GPIO)
#define SMARTCARD_PHY_Activate(base, context, resetType) \
    SMARTCARD_PHY_GPIO_Activate(base, context, resetType) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Deactivate(base, context) \
    SMARTCARD_PHY_GPIO_Deactivate(base, context) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Control(base, context, control, param) \
    SMARTCARD_PHY_GPIO_Control(base, context, control, param) /*!< Common smart card API macro */
#define SMARTCARD_PHY_Init(base, config, sourceClockHz) \
    SMARTCARD_PHY_GPIO_Init(base, config, sourceClockHz)                           /*!< Common smart card API macro */
#define SMARTCARD_PHY_Deinit(base, config) SMARTCARD_PHY_GPIO_Deinit(base, config) /*!< Common smart card API macro */
#define SMARTCARD_PHY_GetDefaultConfig(config) \
    SMARTCARD_PHY_GPIO_GetDefaultConfig(config) /*!< Common smart card API macro */
#endif

/*! @brief Runtime RTOS smart card driver context.*/
typedef struct rtos_smartcard_context
{
    SemaphoreHandle_t x_sem;       /*!< RTOS unique access assurance object */
    EventGroupHandle_t x_event;    /*!< RTOS synchronization object */
    smartcard_context_t x_context; /*!< transactional layer state */
} rtos_smartcard_context_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes a smart card (EMVSIM/UART) peripheral for smart card/ISO-7816 operation.
 * Also initialize smart card PHY interface .
 *
 * This function ungates the smart card clock, initializes the module to EMV default settings,
 * configures the IRQ state structure, and enables the module-level interrupt to the core.
 * Initialize RTOS synchronization objects and context.
 *
 * @param base The smart card peripheral base address.
 * @param ctx The smart card RTOS structure.
 * @param sourceClockHz smart card clock generation module source clock.
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_Init(void *base, rtos_smartcard_context_t *ctx, uint32_t sourceClockHz);

/*!
 * @brief
 *
 * This function disables the smart card (EMVSIM/UART) interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs) and gates smart card clock in SIM. Deactivates also smart card PHY
 * interface,
 * stops smart card clocks.
 * Free all synchronization objects allocated in RTOS smart card context.
 *
 * @param ctx The smart card RTOS state.
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_Deinit(rtos_smartcard_context_t *ctx);

/*!
 * @brief Transfers data using interrupts.
 *
 * A blocking (also known as synchronous) function means that the function returns
 * after the transfer is done. User can cancel this transfer by calling function AbortTransfer.
 *
 * @param ctx A pointer to the RTOS smart card driver context.
 * @param xfer smart card transfer structure.
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_Transfer(rtos_smartcard_context_t *ctx, smartcard_xfer_t *xfer);

/*!
 * @brief Waits until the transfer is finished.
 *
 * Task waits on a transfer finish event. Don't initialize the transfer. Instead, wait for transfer callback.
 * Can be used while waiting on initial TS character.
 *
 * @param ctx A pointer to the RTOS smart card driver context.
 *
 * @return A zero in Success or error code.
 */
int SMARTCARD_RTOS_WaitForXevent(rtos_smartcard_context_t *ctx);
/*!
 * @brief Controls the smart card module as per different user request.
 *
 * @param ctx The smart card RTOS context pointer.
 * @param control Control type
 * @param param Integer value of specific to control command.
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_Control(rtos_smartcard_context_t *ctx, smartcard_control_t control, uint32_t param);

/*!
 * @brief Controls the smart card module as per different user request.
 *
 * @param ctx The smart card RTOS context pointer.
 * @param control Control type
 * @param param Integer value of specific to control command.
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_PHY_Control(rtos_smartcard_context_t *ctx, smartcard_interface_control_t control, uint32_t param);

/*!
 * @brief Activates the smart card interface.
 *
 * @param ctx The smart card RTOS driver context structure.
 * @param resetType type of reset to be performed, possible values
 *                       = kSmartcardColdReset, kSmartcardWarmReset
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_PHY_Activate(rtos_smartcard_context_t *ctx, smartcard_reset_type_t resetType);

/*!
 * @brief Deactivates the smart card interface.
 *
 * @param ctx The smart card RTOS driver context structure.
 *
 * @return An zero in Success or error code.
 */
int SMARTCARD_RTOS_PHY_Deactivate(rtos_smartcard_context_t *ctx);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* _FSL_SMARTCARD_RTOS_H_*/
