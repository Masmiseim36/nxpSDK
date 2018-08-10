/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#ifndef _FSL_FT6X06_H_
#define _FSL_FT6X06_H_

#include "fsl_common.h"
#include "Driver_I2C.h"

/*!
 * @addtogroup ft6x06
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief FT6X06 I2C address. */
#define FT6X06_I2C_ADDRESS (0x38)

/*! @brief FT6X06 maximum number of simultaneously detected touches. */
#define FT6X06_MAX_TOUCHES (2U)

/*! @brief FT6X06 register address where touch data begin. */
#define F6X06_TOUCH_DATA_SUBADDR (1)

/*! @brief FT6X06 raw touch data length. */
#define FT6X06_TOUCH_DATA_LEN (2 + (FT6X06_MAX_TOUCHES) * 6)

typedef enum _touch_event
{
    kTouch_Down = 0,    /*!< The state changed to touched. */
    kTouch_Up = 1,      /*!< The state changed to not touched. */
    kTouch_Contact = 2, /*!< There is a continuous touch being detected. */
    kTouch_Reserved = 3 /*!< No touch information available. */
} touch_event_t;

typedef struct _touch_point
{
    touch_event_t TOUCH_EVENT; /*!< Indicates the state or event of the touch point. */
    uint8_t TOUCH_ID; /*!< Id of the touch point. This numeric value stays constant between down and up event. */
    uint16_t TOUCH_X; /*!< X coordinate of the touch point */
    uint16_t TOUCH_Y; /*!< Y coordinate of the touch point */
} touch_point_t;

typedef struct _ft6x06_handle
{
    ARM_DRIVER_I2C *i2c_driver;
    volatile uint32_t i2c_event;
    volatile bool i2c_event_received;
    uint8_t touch_buf[FT6X06_TOUCH_DATA_LEN];
} ft6x06_handle_t;

status_t FT6X06_Init(ft6x06_handle_t *handle, ARM_DRIVER_I2C *i2c_driver);

status_t FT6X06_Denit(ft6x06_handle_t *handle);

void FT6X06_EventHandler(ft6x06_handle_t *handle, uint32_t i2c_event);

status_t FT6X06_GetSingleTouch(ft6x06_handle_t *handle, touch_event_t *touch_event, int *touch_x, int *touch_y);

status_t FT6X06_GetMultiTouch(ft6x06_handle_t *handle, int *touch_count, touch_point_t touch_array[FT6X06_MAX_TOUCHES]);

#endif
