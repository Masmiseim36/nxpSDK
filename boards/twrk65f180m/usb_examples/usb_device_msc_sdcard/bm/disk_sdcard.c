/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_msc.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "disk_sdcard.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#include "usb_phy.h"
#endif

#include "fsl_port.h"
#include "fsl_sdhc.h"
#include "fsl_card.h"
#include "fsl_gpio.h"
#include "event.h"

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcIrc48M
#define USB_FS_CLK_FREQ 48000000U
#define USB_HS_PHY_CLK_SRC kCLOCK_UsbPhySrcExt
#define USB_HS_PHY_CLK_FREQ BOARD_XTAL0_CLK_HZ
#define USB_HS_CLK_SRC kCLOCK_UsbSrcUnused
#define USB_HS_CLK_FREQ 0U
/*! @brief The maximum timeout time for the transfer complete event */
#define EVENT_TIMEOUT_TRANSFER_COMPLETE (1000U)
/*******************************************************************************
  * Prototypes
  ******************************************************************************/
void BOARD_InitHardware(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
USB_DATA_ALIGNMENT usb_device_inquiry_data_fromat_struct_t g_InquiryInfo = {
    (USB_DEVICE_MSC_UFI_PERIPHERAL_QUALIFIER << USB_DEVICE_MSC_UFI_PERIPHERAL_QUALIFIER_SHIFT) |
        USB_DEVICE_MSC_UFI_PERIPHERAL_DEVICE_TYPE,
    (uint8_t)(USB_DEVICE_MSC_UFI_REMOVABLE_MEDIUM_BIT << USB_DEVICE_MSC_UFI_REMOVABLE_MEDIUM_BIT_SHIFT),
    USB_DEVICE_MSC_UFI_VERSIONS,
    0x02,
    USB_DEVICE_MSC_UFI_ADDITIONAL_LENGTH,
    {0x00, 0x00, 0x00},
    {'N', 'X', 'P', ' ', 'S', 'E', 'M', 'I'},
    {'N', 'X', 'P', ' ', 'M', 'A', 'S', 'S', ' ', 'S', 'T', 'O', 'R', 'A', 'G', 'E'},
    {'0', '0', '0', '1'}};
USB_DATA_ALIGNMENT usb_device_mode_parameters_header_struct_t g_ModeParametersHeader = {
    /*refer to ufi spec mode parameter header*/
    0x0000, /*!< Mode Data Length*/
    0x00,   /*!<Default medium type (current mounted medium type)*/
    0x00,   /*!MODE SENSE command, a Write Protected bit of zero indicates the medium is write enabled*/
    {0x00, 0x00, 0x00, 0x00} /*!<This bit should be set to zero*/
};
/* Data structure of msc device, store the information ,such as class handle */
usb_msc_struct_t g_msc;

uint32_t g_mscReadRequestBuffer[USB_DEVICE_MSC_READ_BUFF_SIZE >> 2];

uint32_t g_mscWriteRequestBuffer[USB_DEVICE_MSC_WRITE_BUFF_SIZE >> 2];

/* Card detect flag. */
static volatile uint32_t g_sdInsertedFlag;

/* State in SDHC driver. */
uint32_t g_sdhcAdmaTable[USB_DEVICE_MSC_ADMA_TABLE_WORDS];
sdhc_handle_t g_sdhcHandle;
/* State in Card driver. */
sd_card_t g_sd;
volatile uint32_t g_sdhcTransferFailedFlag; /* SDHC transfer status. 0 means success, 1 means failed. */
sd_card_t *usbDeviceMscCard;
#if ((defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
     (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0)))
usb_msc_buffer_struct_t dataBuffer[USB_DEVICE_MSC_BUFFER_NUMBER];
usb_msc_buffer_struct_t *currentTrasfer;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

/* SDHC interrupt handler. */
void SDHC_IRQHandler(void)
{
    SDHC_TransferHandleIRQ(BOARD_SDHC_BASEADDR, &g_sdhcHandle);
}
/* Transfer complete callback function. */
void SDHC_TransferCompleteCallback(SDHC_Type *base, sdhc_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        g_sdhcTransferFailedFlag = 0;
    }
    else
    {
        g_sdhcTransferFailedFlag = 1;
    }

    EVENT_Notify(kEVENT_TransferComplete);
}
/* User defined transfer function. */
status_t USB_DeviceMscSdhcTransfer(SDHC_Type *base, sdhc_transfer_t *content)
{
    status_t error = kStatus_Success;

    do
    {
        error =
            SDHC_TransferNonBlocking(base, &g_sdhcHandle, g_sdhcAdmaTable, USB_DEVICE_MSC_ADMA_TABLE_WORDS, content);
    } while (error == kStatus_SDHC_BusyTransferring);

    if ((error != kStatus_Success) || (false == EVENT_Wait(kEVENT_TransferComplete, EVENT_TIMEOUT_TRANSFER_COMPLETE)) ||
        (g_sdhcTransferFailedFlag))
    {
        error = kStatus_Fail;
    }

    return error;
}
/*!
 * @brief device msc sdhc card detect function.
 *
 * This function check the card detect pin value.
 * @return g_sdInsertedFlag value.
 */
uint32_t USB_DeviceMscSdhcCardDetect(void)
{
    if (GPIO_ReadPinInput(BOARD_SDHC_CD_GPIO_BASE, BOARD_SDHC_CD_GPIO_PIN))
#if defined BOARD_SDHC_CD_LOGIC_RISING
    {
        g_sdInsertedFlag = 1U;
    }
    else
    {
        g_sdInsertedFlag = 0U;
    }
#else
    {
        g_sdInsertedFlag = 0U;
    }
    else
    {
        g_sdInsertedFlag = 1U;
    }
#endif
    return g_sdInsertedFlag;
}
/*!
 * @brief device msc sdhc init function.
 *
 * This function initialize the SDHC module and the card on a specific SDHC.
 * @return kStatus_USB_Success or error.
 */
uint8_t USB_DeviceMscSdhcInit(void)
{
    sdhc_transfer_callback_t g_sdhcCallback;
    sdhc_config_t *g_sdhcConfig;
    SDHC_Type *base;
    uint8_t *temptptr;
    int i;

    g_sdhcTransferFailedFlag = 0U;
    usb_status_t error = kStatus_USB_Success;
    base = BOARD_SDHC_BASEADDR;
    g_sdhcConfig = &(usbDeviceMscCard->host.config);

    NVIC_SetPriority(SDHC_IRQn, (USB_DEVICE_INTERRUPT_PRIORITY - 1U));
    /* Opens SDHC clock gate/NVIC. */
    /* Initializes SDHC. */

    temptptr = (uint8_t *)g_sdhcConfig;
    for (i = 0; i < sizeof(sdhc_config_t); i++)
    {
        temptptr[i] = 0;
    }
    g_sdhcConfig->cardDetectDat3 = false;
    g_sdhcConfig->endianMode = kSDHC_EndianModeLittle;
    g_sdhcConfig->dmaMode = kSDHC_DmaModeAdma2;
    g_sdhcConfig->readWatermarkLevel = 0x80U;
    g_sdhcConfig->writeWatermarkLevel = 0x80U;
    SDHC_Init(base, g_sdhcConfig);

    /* Sets callback in SDHC driver. */
    temptptr = (uint8_t *)&g_sdhcCallback;
    for (i = 0; i < sizeof(g_sdhcCallback); i++)
    {
        temptptr[i] = 0;
    }
    g_sdhcCallback.TransferComplete = SDHC_TransferCompleteCallback;
    /* Creates handle for SDHC driver */
    SDHC_TransferCreateHandle(BOARD_SDHC_BASEADDR, &g_sdhcHandle, &g_sdhcCallback, (void *)&g_sdhcTransferFailedFlag);

    /* Create transfer complete event. */
    if (false == EVENT_Create(kEVENT_TransferComplete))
    {
        return kStatus_USB_Error;
    }
    /* Fills state in card driver. */
    usbDeviceMscCard->host.base = base;
    usbDeviceMscCard->host.sourceClock_Hz = CLOCK_GetFreq(BOARD_SDHC_CLKSRC);
    usbDeviceMscCard->host.transfer = USB_DeviceMscSdhcTransfer;
    /* Init card. */
    if (SD_Init(usbDeviceMscCard))
    {
        PRINTF("\n SD card init failed \n");
        error = kStatus_USB_Error;
    }

    return error;
}
void USB_DeviceMscApp(void)
{
    /*TO DO*/
    /*add user code*/
    return;
}
#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))
/*!
 * @brief msc enter critical.
 *
 * This function is used to enter critical disable interrupt .
 *
 */
static void USB_BmEnterCritical(uint8_t *sr)
{
    *sr = __get_PRIMASK();
    __ASM("CPSID I");
}
/*!
 * @brief msc exit critical.
 *
 * This function is used to exit critical ,enable interrupt .
 *
 */
static void USB_BmExitCritical(uint8_t sr)
{
    __set_PRIMASK(sr);
}
/*!
 * @brief device msc add a buffer to the tail queue function.
 *
 * This function add a buffer which contains data from the host pc to the head queue when receive write response
 * kUSB_DeviceMscEventWriteResponse
 */
void USB_DeviceMscAddBufferToTail(usb_msc_buffer_struct_t *bufferinfo)
{
    usb_msc_buffer_struct_t *temp;
    usb_msc_buffer_struct_t *pre;

    temp = g_msc.taillist;
    pre = g_msc.taillist;
    while (temp)
    {
        pre = temp;
        if (temp == bufferinfo)
        {
            return;
        }
        temp = temp->next;
    }
    if (pre)
    {
        pre->next = bufferinfo;
        bufferinfo->next = NULL;
    }
    else
    {
        g_msc.taillist = bufferinfo;
        bufferinfo->next = NULL;
    }
}
/*!
 * @brief device msc get buffer from tail queue function.
 *
 * This function get a buffer which contains data from the host pc . The write task need get those data in order from
 * the tail queue, and then write those data to sdcard.
 */

void USB_DeviceMscGetBufferFromTail(usb_msc_buffer_struct_t **bufferinfo)
{
    if (g_msc.taillist != NULL)
    {
        *bufferinfo = g_msc.taillist;
        g_msc.taillist = g_msc.taillist->next;
    }
    else
    {
        *bufferinfo = NULL;
    }
}
/*!
 * @brief device msc add a buffer to the head function.
 *
 * This function add a empty buffer to the head queue, head queue maintain the empty buffer, when usb stack will add
 * the
 *  buffer to head queue when the data in this buffer has already write into the sdcard.
 */
void USB_DeviceMscAddBufferToHead(usb_msc_buffer_struct_t *bufferinfo)
{
    usb_msc_buffer_struct_t *p;

    p = g_msc.headlist;
    while (p)
    {
        if (p == bufferinfo)
        {
            return;
        }
        p = p->next;
    }
    if (g_msc.headlist == NULL)
    {
        g_msc.headlist = bufferinfo;
    }
    else
    {
        bufferinfo->next = g_msc.headlist;
        g_msc.headlist = bufferinfo;
    }
}
/*!
 * @brief device msc get buffer from head function.
 *
 * This function get a empty buffer from the head queue. The stack need get a
 * empty buffer from head queue when receive a write request
 */
void USB_DeviceMscGetBufferFromHead(usb_msc_buffer_struct_t **bufferinfo)
{
    if (g_msc.headlist != NULL)
    {
        *bufferinfo = g_msc.headlist;
        g_msc.headlist = g_msc.headlist->next;
    }
    else
    {
        *bufferinfo = NULL;
    }
}
/*!
 * @brief device msc write task function.
 *
 * This function write data to the sdcard.
 */
void USB_DeviceMscWriteTask(void)
{
    usb_msc_buffer_struct_t *temp;
    temp = g_msc.taillist;
    status_t errorCode;
    usb_status_t error = kStatus_USB_Success;
    uint8_t usbOsaCurrentSr;
    USB_BmEnterCritical(&usbOsaCurrentSr);
    USB_DeviceMscGetBufferFromTail(&temp);
    USB_BmExitCritical(usbOsaCurrentSr);
    if (NULL != temp)
    {
        errorCode = SD_WriteBlocks(usbDeviceMscCard, temp->buffer, temp->offset,
                                   temp->size >> USB_DEVICE_SDCARD_BLOCK_SIZE_POWER);
        USB_BmEnterCritical(&usbOsaCurrentSr);
        USB_DeviceMscAddBufferToHead(temp);
        USB_BmExitCritical(usbOsaCurrentSr);
        if (kStatus_Success != errorCode)
        {
            g_msc.read_write_error = 1;
            usb_echo(
                "Write error, error = 0xx%x \t Please check write request buffer size(must be less than 128 "
                "sectors)\r\n",
                error);
            error = kStatus_USB_Error;
        }
    }
}
void USB_DeviceMscInitQueue(void)
{
    uint8_t i;
    g_msc.headlist = dataBuffer;
    usb_msc_buffer_struct_t *pre;
    usb_msc_buffer_struct_t *temp;
    pre = temp = g_msc.headlist;
    for (i = 1; i < USB_DEVICE_MSC_BUFFER_NUMBER; i++)
    {
        temp++;
        pre->next = temp;
        pre++;
    }
    pre->next = NULL;
    g_msc.taillist = NULL;
    g_msc.transferlist = NULL;
}
#endif
/*!
 * @brief device msc callback function.
 *
 * This function handle the disk class specified event.
 * @param handle          The USB class  handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the class specific event.
 * @return kStatus_USB_Success or error.
 */
usb_status_t USB_DeviceMscCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    status_t errorCode = kStatus_Success;
    usb_device_lba_information_struct_t *lbaInformation;
    usb_device_lba_app_struct_t *lba;
    usb_device_ufi_app_struct_t *ufi;

#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))
    usb_msc_buffer_struct_t *tempbuffer;
#endif
    switch (event)
    {
        case kUSB_DeviceMscEventReadResponse:
            lba = (usb_device_lba_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventWriteResponse:
            lba = (usb_device_lba_app_struct_t *)param;
#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))
            if (NULL != currentTrasfer)
            {
                currentTrasfer->offset = lba->offset;
                currentTrasfer->size = lba->size;
                if (0 == lba->size)
                {
                    USB_DeviceMscAddBufferToHead(currentTrasfer);
                }
                else
                {
                    USB_DeviceMscAddBufferToTail(currentTrasfer);
                }
            }
#else
            /*write the data to sd card*/
            if (0 != lba->size)
            {
                errorCode = SD_WriteBlocks(usbDeviceMscCard, lba->buffer, lba->offset,
                                           lba->size >> USB_DEVICE_SDCARD_BLOCK_SIZE_POWER);
                if (kStatus_Success != errorCode)
                {
                    g_msc.read_write_error = 1;
                    usb_echo(
                        "Write error, error = 0xx%x \t Please check write request buffer size(must be less than 128 "
                        "sectors)\r\n",
                        error);
                    error = kStatus_USB_Error;
                }
            }
#endif
            break;
        case kUSB_DeviceMscEventWriteRequest:
            lba = (usb_device_lba_app_struct_t *)param;
/*get a buffer to store the data from host*/
#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))

            USB_DeviceMscGetBufferFromHead(&tempbuffer);
            while (NULL == tempbuffer)
            {
                usb_echo("No buffer available");
                USB_DeviceMscWriteTask();
                USB_DeviceMscGetBufferFromHead(&tempbuffer);
            }
            lba->buffer = tempbuffer->buffer;
            currentTrasfer = tempbuffer;
#else
            lba->buffer = (uint8_t *)&g_mscWriteRequestBuffer[0];
#endif
            break;
        case kUSB_DeviceMscEventReadRequest:
            lba = (usb_device_lba_app_struct_t *)param;
            lba->buffer = (uint8_t *)&g_mscReadRequestBuffer[0];

            /*read the data from sd card, then store these data to the read buffer*/
            errorCode = SD_ReadBlocks(usbDeviceMscCard, lba->buffer, lba->offset,
                                      lba->size >> USB_DEVICE_SDCARD_BLOCK_SIZE_POWER);

            if (kStatus_Success != errorCode)
            {
                g_msc.read_write_error = 1;
                usb_echo(
                    "Read error, error = 0xx%x \t Please check read request buffer size(must be less than 128 "
                    "sectors)\r\n",
                    error);
                error = kStatus_USB_Error;
            }
            break;
        case kUSB_DeviceMscEventGetLbaInformation:
            lbaInformation = (usb_device_lba_information_struct_t *)param;
            lbaInformation->lengthOfEachLba = usbDeviceMscCard->blockSize;
            lbaInformation->totalLbaNumberSupports = usbDeviceMscCard->blockCount;
            lbaInformation->logicalUnitNumberSupported = LOGICAL_UNIT_SUPPORTED;
            lbaInformation->bulkInBufferSize = USB_DEVICE_MSC_READ_BUFF_SIZE;
            lbaInformation->bulkOutBufferSize = USB_DEVICE_MSC_WRITE_BUFF_SIZE;
            break;
        case kUSB_DeviceMscEventTestUnitReady:
            /*change the test unit ready command's sense data if need, be careful to modify*/
            ufi = (usb_device_ufi_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventInquiry:
            ufi = (usb_device_ufi_app_struct_t *)param;
            ufi->size = sizeof(usb_device_inquiry_data_fromat_struct_t);
            ufi->buffer = (uint8_t *)&g_InquiryInfo;
            break;
        case kUSB_DeviceMscEventModeSense:
            ufi = (usb_device_ufi_app_struct_t *)param;
            ufi->size = sizeof(usb_device_mode_parameters_header_struct_t);
            ufi->buffer = (uint8_t *)&g_ModeParametersHeader;
            break;
        case kUSB_DeviceMscEventModeSelect:
            break;
        case kUSB_DeviceMscEventModeSelectResponse:
            ufi = (usb_device_ufi_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventFormatComplete:
            break;
        case kUSB_DeviceMscEventRemovalRequest:
            break;
        default:
            break;
    }
    return error;
}
/*!
 * @brief device callback function.
 *
 * This function handle the usb standard event. more information, please refer to usb spec chapter 9.
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 * @return  A USB error code or kStatus_USB_Success..
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t *temp16 = (uint16_t *)param;
    uint8_t *temp8 = (uint8_t *)param;
    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            g_msc.attach = 0;
            error = kStatus_USB_Success;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_msc.speed))
            {
                USB_DeviceSetSpeed(handle, g_msc.speed);
            }
#endif
        }
#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))
            /*re-init the queue every time device is reset*/
            USB_DeviceMscInitQueue();
            currentTrasfer = NULL;
#endif
            break;
        case kUSB_DeviceEventSetConfiguration:
            if (param)
            {
                g_msc.attach = 1;
                g_msc.currentConfiguration = *temp8;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (g_msc.attach)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (interface < USB_MSC_INTERFACE_COUNT)
                {
                    g_msc.currentInterfaceAlternateSetting[interface] = alternateSetting;
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *temp8 = g_msc.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | g_msc.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            break;
    }
    return error;
}
/* USB device class information */
usb_device_class_config_struct_t msc_config[1] = {{
    USB_DeviceMscCallback, 0, &g_UsbDeviceMscConfig,
}};
/* USB device class configuration information */
usb_device_class_config_list_struct_t msc_config_list = {
    msc_config, USB_DeviceCallback, 1,
};
/*!
 * @brief USB Interrupt service routine.
 *
 * This function serves as the USB interrupt service routine.
 *
 * @return None.
 */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_msc.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_msc.deviceHandle);
}
#endif

void USB_DeviceMscAppTask(void)
{
    if (g_msc.read_write_error)
    {
        return;
    }
    USB_DeviceMscApp();
}
/*!
 * @brief device application init function.
 *
 * This function init the usb stack and sdhc driver.
 *
 * @return None.
 */

void USB_DeviceApplicationInit(void)
{
    uint8_t irq_no;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
    uint8_t ehci_irq[] = USBHS_IRQS;
    irq_no = ehci_irq[CONTROLLER_ID - kUSB_ControllerEhci0];

    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
    uint8_t khci_irq[] = USB_IRQS;
    irq_no = khci_irq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */

    usb_echo("Please insert SD card\r\n");

    usbDeviceMscCard = &g_sd;
    g_sdInsertedFlag = 0;

    while (!USB_DeviceMscSdhcCardDetect())
    {
        ;
    }
    if (kStatus_USB_Success != USB_DeviceMscSdhcInit())
    {
        usb_echo("SDHC card init failed\r\n");
        return;
    }
#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))
    USB_DeviceMscInitQueue();
#endif

    g_msc.speed = USB_SPEED_FULL;
    g_msc.attach = 0;
    g_msc.mscHandle = (class_handle_t)NULL;
    g_msc.deviceHandle = NULL;
    if (kStatus_USB_Success != USB_DeviceClassInit(CONTROLLER_ID, &msc_config_list, &g_msc.deviceHandle))
    {
        usb_echo("USB device init failed\r\n");
    }
    else
    {
        usb_echo("USB device mass storage demo\r\n");
        g_msc.mscHandle = msc_config_list.config->classHandle;
    }

    NVIC_SetPriority((IRQn_Type)irq_no, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irq_no);

    USB_DeviceRun(g_msc.deviceHandle);
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    USB_DeviceApplicationInit();

    while (1)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
        USB_DeviceEhciTaskFunction(g_msc.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
        USB_DeviceKhciTaskFunction(g_msc.deviceHandle);
#endif
#endif
        USB_DeviceMscAppTask();
#if (defined(USB_DEVICE_CONFIG_USE_TASK) && (USB_DEVICE_CONFIG_USE_TASK > 0)) && \
    (defined(USB_DEVICE_MSC_USE_WRITE_TASK) && (USB_DEVICE_MSC_USE_WRITE_TASK > 0))
        USB_DeviceMscWriteTask();
#endif
    }
}
