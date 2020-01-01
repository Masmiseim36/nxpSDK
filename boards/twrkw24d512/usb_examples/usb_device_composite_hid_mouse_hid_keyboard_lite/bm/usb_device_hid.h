/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#ifndef __USB_DEVICE_HID_H__
#define __USB_DEVICE_HID_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The class code of the HID class */
#define USB_DEVICE_CONFIG_HID_CLASS_CODE (0x03U)

/*! @brief Request code to get report of HID class. */
#define USB_DEVICE_HID_REQUEST_GET_REPORT (0x01U)
#define USB_DEVICE_HID_REQUEST_GET_REPORT_TYPE_INPUT (0x01U)
#define USB_DEVICE_HID_REQUEST_GET_REPORT_TYPE_OUPUT (0x02U)
#define USB_DEVICE_HID_REQUEST_GET_REPORT_TYPE_FEATURE (0x03U)
/*! @brief Request code to get idle of HID class. */
#define USB_DEVICE_HID_REQUEST_GET_IDLE (0x02U)
/*! @brief Request code to get protocol of HID class. */
#define USB_DEVICE_HID_REQUEST_GET_PROTOCOL (0x03U)
/*! @brief Request code to set report of HID class. */
#define USB_DEVICE_HID_REQUEST_SET_REPORT (0x09U)
/*! @brief Request code to set idle of HID class. */
#define USB_DEVICE_HID_REQUEST_SET_IDLE (0x0AU)
/*! @brief Request code to set protocol of HID class. */
#define USB_DEVICE_HID_REQUEST_SET_PROTOCOL (0x0BU)

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* __USB_DEVICE_HID_H__ */
