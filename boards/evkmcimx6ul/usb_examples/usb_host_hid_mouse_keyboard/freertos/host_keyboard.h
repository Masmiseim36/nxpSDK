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

#ifndef _HOST_KEYBOARD_H_
#define _HOST_KEYBOARD_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief USB host keyboard instance structure */
typedef struct _usb_host_keyboard_instance
{
    usb_host_configuration_handle configHandle; /*!< the keybaord's configuration handle */
    usb_device_handle deviceHandle;             /*!< the keybaord's device handle */
    usb_host_class_handle classHandle;          /*!< the keybaord's class handle */
    usb_host_interface_handle interfaceHandle;  /*!< the keybaord's interface handle */
    uint8_t lastPressData[6];                   /*!< keyboard last keyboard data */
    uint16_t maxPacketSize;                     /*!< interrupt in max packet size */
    uint8_t deviceState;                        /*!< device attach/detach status */
    uint8_t prevState;                          /*!< device attach/detach previous status */
    uint8_t runState;                           /*!< keyboard application run status */
    uint8_t runWaitState; /*!< keyboard application wait status, go to next run status when the wait status success */
    uint8_t shiftPressed; /*!< set 1 when shift key is pressed */
    uint8_t keyboardBuffer[HID_BUFFER_SIZE]; /*!< use to receive report descriptor and data */
} usb_host_keyboard_instance_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief host keyboard task function.
 *
 * This function implements the host keyboard action, it is used to create task.
 *
 * @param param   the host keyboard instance pointer.
 */
extern void USB_HostHidKeyboardTask(void *param);

/*!
 * @brief host keyboard callback function.
 *
 * This function should be called in the host callback function.
 *
 * @param deviceHandle         device handle.
 * @param configurationHandle  attached device's configuration descriptor information.
 * @param eventCode            callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The configuration don't contain hid keyboard interface.
 */
extern usb_status_t USB_HostHidKeyboardEvent(usb_device_handle deviceHandle,
                                             usb_host_configuration_handle configurationHandle,
                                             uint32_t eventCode);

#endif /* _HOST_KEYBOARD_H_ */
