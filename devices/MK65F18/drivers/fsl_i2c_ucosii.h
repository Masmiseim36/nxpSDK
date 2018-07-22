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
#ifndef __FSL_I2C_UCOSII_H__
#define __FSL_I2C_UCOSII_H__

#include <ucos_ii.h>

#include "fsl_i2c.h"

/*!
 * @addtogroup i2c_ucosii_driver I2C µCO/S-II driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @cond RTOS_PRIVATE
 * @brief Transfer event complete flag (i2c_rtos_handle_t).
 */
#define RTOS_I2C_COMPLETE 0x1

/*! @brief I2C FreeRTOS handle */
typedef struct _i2c_rtos_handle
{
    I2C_Type *base;                 /*!< I2C base address */
    i2c_master_handle_t drv_handle; /*!< A handle of the underlying driver, treated as opaque by the RTOS layer */
    status_t async_status;          /*!< Transactional state of the underlying driver */
    OS_EVENT *mutex;                /*!< A mutex to lock the handle during a trasfer */
    OS_FLAG_GRP *event;             /*!< A semaphore to notify and unblock a task when the transfer ends */
} i2c_rtos_handle_t;
/*! \endcond */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name I2C RTOS Operation
 * @{
 */

/*!
 * @brief Initializes the I2C.
 *
 * This function initializes the I2C module and the related RTOS context.
 *
 * @param handle The RTOS I2C handle; the pointer to an allocated space for RTOS context.
 * @param base The pointer base address of the I2C instance to initialize.
 * @param masterConfig A configuration structure to set-up the I2C in master mode.
 * @param srcClock_Hz A frequency of the input clock of the I2C module.
 * @return status of the operation.
 */
status_t I2C_RTOS_Init(i2c_rtos_handle_t *handle,
                       I2C_Type *base,
                       const i2c_master_config_t *masterConfig,
                       uint32_t srcClock_Hz);

/*!
 * @brief Deinitializes the I2C.
 *
 * This function deinitializes the I2C module and the related RTOS context.
 *
 * @param handle The RTOS I2C handle.
 */
status_t I2C_RTOS_Deinit(i2c_rtos_handle_t *handle);

/*!
 * @brief Performs the I2C transfer.
 *
 * This function performs the  I2C transfer according to the data given in the transfer structure.
 *
 * @param handle The RTOS I2C handle.
 * @param transfer A structure specifying the transfer parameters.
 * @return status of the operation.
 */
status_t I2C_RTOS_Transfer(i2c_rtos_handle_t *handle, i2c_master_transfer_t *transfer);

/*!
 * @}
 */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* __FSL_I2C_UCOSII_H__ */
