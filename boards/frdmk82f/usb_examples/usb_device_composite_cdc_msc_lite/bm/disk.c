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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_msc.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "disk.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>

#include "composite.h"
/*******************************************************************************
* Definitions
******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/
static usb_device_composite_struct_t *g_deviceComposite;

/*******************************************************************************
* Code
******************************************************************************/
usb_status_t USB_DeviceMscSend(usb_device_msc_struct_t *mscHandle)
{
    usb_status_t error = kStatus_USB_Success;
    usb_device_lba_app_struct_t lba;

    lba.offset = mscHandle->currentOffset;
    /*bulkInBufferSize is the application buffer size, USB_DEVICE_MSC_MAX_SEND_TRANSFER_LENGTH is the max transfer
       length by the hardware,
       lba.size is the data pending  for transfer ,select the minimum size to transfer ,the remaining will be transfer
       next time*/
    lba.size = (mscHandle->bulkInBufferSize > USB_DEVICE_MSC_MAX_SEND_TRANSFER_LENGTH) ?
                   USB_DEVICE_MSC_MAX_SEND_TRANSFER_LENGTH :
                   mscHandle->bulkInBufferSize;
    lba.size =
        (mscHandle->transferRemaining > lba.size) ? lba.size : mscHandle->transferRemaining; /* whichever is smaller */

    lba.buffer = g_deviceComposite->mscDisk.storageDisk + lba.offset;

    if (mscHandle->currentOffset < (mscHandle->totalLogicalBlockNumber * mscHandle->lengthOfEachLba))
    {
        error = USB_DeviceSendRequest(g_deviceComposite->deviceHandle, mscHandle->bulkInEndpoint, lba.buffer, lba.size);
    }
    else
    {
        mscHandle->needInStallFlag = 0;
        mscHandle->inEndpointStallFlag = 1;
        mscHandle->dataInFlag = 0;
        mscHandle->stallStatus = (uint8_t)USB_DEVICE_MSC_STALL_IN_DATA;
        USB_DeviceStallEndpoint(g_deviceComposite->deviceHandle, mscHandle->bulkInEndpoint);
    }
    return error;
}
/*!
 * @brief Receive data through a specified endpoint.
 *
 * The function is used to receive data through a specified endpoint.
 * The function calls USB_DeviceRecvRequest internally.
 *
 * @param handle The msc class handle got from usb_device_class_config_struct_t::classHandle.
 *
 * @return A USB error code or kStatus_USB_Success.
 *
 * @note The return value just means if the receiving request is successful or not; the transfer done is notified by
 * USB_DeviceMscBulkOut.
 * Currently, only one transfer request can be supported for one specific endpoint.
 * If there is a specific requirement to support multiple transfer requests for one specific endpoint, the application
 * should implement a queue in the application level.
 * The subsequent transfer could begin only when the previous transfer is done (get notification through the endpoint
 * callback).
 */
usb_status_t USB_DeviceMscRecv(usb_device_msc_struct_t *mscHandle)
{
    usb_status_t error = kStatus_USB_Success;
    usb_device_lba_app_struct_t lba;

    lba.offset = mscHandle->currentOffset;
    /*bulkOutBufferSize is the application buffer size, USB_DEVICE_MSC_MAX_RECV_TRANSFER_LENGTH is the max transfer
       length by the hardware,
       lba.size is the data pending  for transfer ,select the minimum size to transfer ,the remaining will be transfer
       next time*/
    lba.size = (mscHandle->bulkOutBufferSize > USB_DEVICE_MSC_MAX_RECV_TRANSFER_LENGTH) ?
                   USB_DEVICE_MSC_MAX_RECV_TRANSFER_LENGTH :
                   mscHandle->bulkOutBufferSize;
    lba.size =
        (mscHandle->transferRemaining > lba.size) ? lba.size : mscHandle->transferRemaining; /* whichever is smaller */

    lba.buffer = g_deviceComposite->mscDisk.storageDisk + lba.offset;

    if (mscHandle->currentOffset < (mscHandle->totalLogicalBlockNumber * mscHandle->lengthOfEachLba))
    {
        error =
            USB_DeviceRecvRequest(g_deviceComposite->deviceHandle, mscHandle->bulkOutEndpoint, lba.buffer, lba.size);
    }
    else
    {
        mscHandle->needOutStallFlag = 0;
        mscHandle->outEndpointStallFlag = 1;
        mscHandle->dataOutFlag = 0;
        mscHandle->stallStatus = (uint8_t)USB_DEVICE_MSC_STALL_IN_DATA;
        USB_DeviceStallEndpoint(g_deviceComposite->deviceHandle, mscHandle->bulkOutEndpoint);
    }
    return error;
}
/*!
 * @brief Recv Send data through a specified endpoint.
 *
 * The function is used when ufi process read/write command .
 * The function calls USB_DeviceMscRecv or  usb_deviceSendRecv as the direction internally.
 *
 * @param handle The msc class handle got from usb_device_class_config_struct_t::classHandle.
 * @param direction     Data direction: 0 = Data-Out from host to the device, 1 = Data-In from the device to the host.
 * @param buffer The memory address to hold the data need to be sent.
 * @return A USB error code or kStatus_USB_Success.
 *
 * @note The return value just means if the sending or reciving request is successful or not.
 */
usb_status_t USB_DeviceMscLbaTransfer(usb_device_msc_struct_t *mscHandle,
                                      uint8_t direction,
                                      usb_lba_transfer_information_struct_t *lbaInformation)
{
    usb_status_t error = kStatus_USB_Success;

    mscHandle->transferRemaining = lbaInformation->transferNumber * mscHandle->lengthOfEachLba;
    mscHandle->currentOffset = lbaInformation->startingLogicalBlockAddress * mscHandle->lengthOfEachLba;

    if (direction == USB_IN)
    {
        error = USB_DeviceMscSend(mscHandle);
    }
    else
    {
        error = USB_DeviceMscRecv(mscHandle);
    }
    return error;
}

/*!
 * @brief Process usb msc ufi command.
 *
 * This function analyse the cbw , get the command code.
 *
 * @param handle          The device msc class hanlde.
 *
 * @retval kStatus_USB_Success              Free device msc class hanlde successfully.
 */
usb_status_t USB_DeviceMscProcessUfiCommand(usb_device_msc_struct_t *mscHandle)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_msc_ufi_struct_t *ufi = NULL;

    ufi = &mscHandle->g_mscUfi;

    ufi->requestSense.senseKey = USB_DEVICE_MSC_UFI_NO_SENSE;
    ufi->requestSense.additionalSenseCode = USB_DEVICE_MSC_UFI_NO_SENSE;
    ufi->requestSense.additionalSenseQualifer = USB_DEVICE_MSC_UFI_NO_SENSE;
    ufi->thirteenCase.hostExpectedDataLength = mscHandle->g_mscCbw.dataTransferLength;
    ufi->thirteenCase.hostExpectedDirection =
        (uint8_t)(mscHandle->g_mscCbw.flags >> USB_DEVICE_MSC_CBW_DIRECTION_SHIFT);
    /*The first byte of all ufi command blocks shall contain an Operation Code, refer to ufi spec*/
    switch (mscHandle->g_mscCbw.cbwcb[0])
    {
        /* ufi commmand operation code*/
        case USB_DEVICE_MSC_INQUIRY_COMMAND: /*operation code : 0x12*/
            error = USB_DeviceMscUfiInquiryCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_READ_10_COMMAND: /*operation code : 0x28 */
        case USB_DEVICE_MSC_READ_12_COMMAND: /*operation code : 0xA8 */
            error = USB_DeviceMscUfiReadCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_REQUEST_SENSE_COMMAND: /*operation code : 0x03*/
            error = USB_DeviceMscUfiRequestSenseCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_TEST_UNIT_READY_COMMAND: /*operation code : 0x00 */
            error = USB_DeviceMscUfiTestUnitReadyCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_WRITE_10_COMMAND: /*operation code : 0x2A */
        case USB_DEVICE_MSC_WRITE_12_COMMAND: /*operation code : 0xAA */
            error = USB_DeviceMscUfiWriteCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_PREVENT_ALLOW_MEDIUM_REM_COMMAND: /*operation code :0x1E */
            error = USB_DeviceMscUfiPreventAllowMediumCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_FORMAT_UNIT_COMMAND: /*operation code : 0x04*/
            error = USB_DeviceMscUfiFormatUnitCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_READ_CAPACITY_10_COMMAND: /*operation code : 0x25*/
        case USB_DEVICE_MSC_READ_CAPACITY_16_COMMAND: /*operation code : 0x9E*/
            error = USB_DeviceMscUfiReadCapacityCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_MODE_SENSE_10_COMMAND: /* operation code :0x5A*/
        case USB_DEVICE_MSC_MODE_SENSE_6_COMMAND:  /* operation code : 0x1A */
            error = USB_DeviceMscUfiModeSenseCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_MODE_SELECT_10_COMMAND: /*operation code : 0x55 */
        case USB_DEVICE_MSC_MODE_SELECT_6_COMMAND:  /*operation code : 0x15 */
            error = USB_DeviceMscUfiModeSelectCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_READ_FORMAT_CAPACITIES_COMMAND: /*operation code : 0x23 */
            error = USB_DeviceMscUfiReadFormatCapacityCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_SEND_DIAGNOSTIC_COMMAND: /*operation code : 0x1D*/
            error = USB_DeviceMscUfiSendDiagnosticCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_VERIFY_COMMAND: /*operation code : 0x2F*/
            error = USB_DeviceMscUfiVerifyCommand(mscHandle);
            break;
        case USB_DEVICE_MSC_START_STOP_UNIT_COMMAND: /*operation code : 0x1B*/
            error = USB_DeviceMscUfiStartStopUnitCommand(mscHandle);
            break;
        default:
            error = USB_DeviceMscUfiUnsupportCommand(mscHandle);
            mscHandle->dataOutFlag = 0;
            mscHandle->dataInFlag = 0;
            mscHandle->outEndpointStallFlag = 0;
            mscHandle->inEndpointStallFlag = 0;
            mscHandle->needOutStallFlag = 0;
            mscHandle->needInStallFlag = 0;
            break;
    }
    return error;
}
/*!
 * @brief Bulk IN endpoint callback function.
 *
 * This callback function is used to notify uplayer the tranfser result of a transfer.
 * This callback pointer is passed when the Bulk IN pipe initialized.
 *
 * @param handle          The device handle. It equals the value returned from USB_DeviceInit.
 * @param event          The result of the Bulk IN pipe transfer.
 * @param arg             The paramter for this callback. It is same with
 * usb_device_endpoint_callback_struct_t::callbackParam. In the class, the value is the MSC class handle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceMscBulkIn(usb_device_handle deviceHandle,
                                 usb_device_endpoint_callback_message_struct_t *event,
                                 void *arg)
{
    usb_device_msc_csw_t *csw;
    usb_status_t error = kStatus_USB_Error;
    usb_device_msc_struct_t *mscHandle;

    mscHandle = &(g_deviceComposite->mscDisk.handle);
    if (event->length == USB_UNINITIALIZED_VAL_32)
    {
        return error;
    }
    if (mscHandle->transferRemaining >= event->length)
    {
        mscHandle->transferRemaining -= event->length;
    }

    if (mscHandle->needInStallFlag == 1)
    {
        mscHandle->needInStallFlag = 0;
        mscHandle->inEndpointStallFlag = 1;
        mscHandle->dataInFlag = 0;
        USB_DeviceStallEndpoint(deviceHandle, mscHandle->bulkInEndpoint);
        return error;
    }
    if ((!mscHandle->dataInFlag) && (event->length == USB_DEVICE_MSC_CSW_LENGTH))
    {
        csw = (usb_device_msc_csw_t *)(event->buffer);
    }

    if (mscHandle->dataInFlag)
    {
        if (mscHandle->transferRemaining)
        {
            mscHandle->currentOffset += event->length;
            error = USB_DeviceMscSend(mscHandle);
        }
        if (!mscHandle->transferRemaining)
        {
            mscHandle->dataInFlag = 0;

            /*data transfer has been done, send the csw to host */
            USB_DeviceSendRequest(deviceHandle, mscHandle->bulkInEndpoint, (uint8_t *)&mscHandle->g_mscCsw,
                                  USB_DEVICE_MSC_CSW_LENGTH);
        }
    }
    else if ((event->length == USB_DEVICE_MSC_CSW_LENGTH) && (csw->signature == USB_DEVICE_MSC_DCSWSIGNATURE))
    {
        mscHandle->cbwValidFlag = 1;

        (void)USB_DeviceRecvRequest(deviceHandle, mscHandle->bulkOutEndpoint, (uint8_t *)&mscHandle->g_mscCbw,
                                    USB_DEVICE_MSC_CBW_LENGTH);
        mscHandle->cbwPrimeFlag = 1;
    }
    else
    {
    }
    return error;
}
/*!
 * @brief Bulk OUT endpoint callback function.
 *
 * This callback function is used to notify uplayer the tranfser result of a transfer.
 * This callback pointer is passed when the Bulk OUT pipe initialized.
 *
 * @param handle          The device handle. It equals the value returned from USB_DeviceInit.
 * @param message         The result of the Bulk OUT pipe transfer.
 * @param callbackParam  The paramter for this callback. It is same with
 * usb_device_endpoint_callback_struct_t::callbackParam. In the class, the value is the MSC class handle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceMscBulkOut(usb_device_handle deviceHandle,
                                  usb_device_endpoint_callback_message_struct_t *event,
                                  void *arg)
{
    usb_status_t error = kStatus_USB_Success;
    usb_device_msc_struct_t *mscHandle;

    mscHandle = &(g_deviceComposite->mscDisk.handle);
    if (event->length == USB_UNINITIALIZED_VAL_32)
    {
        return error;
    }

    if (mscHandle->transferRemaining >= event->length)
    {
        mscHandle->transferRemaining -= event->length;
    }

    if (mscHandle->needOutStallFlag == 1)
    {
        mscHandle->needOutStallFlag = 0;
        mscHandle->outEndpointStallFlag = 1;
        mscHandle->dataOutFlag = 0;
        mscHandle->cbwPrimeFlag = 0;
        USB_DeviceStallEndpoint(deviceHandle, mscHandle->bulkOutEndpoint);
        return error;
    }

    if (mscHandle->dataOutFlag)
    {
        if (mscHandle->transferRemaining)
        {
            mscHandle->currentOffset += event->length;
            error = USB_DeviceMscRecv(mscHandle);
        }

        if (!mscHandle->transferRemaining)
        {
            mscHandle->dataOutFlag = 0;
            {
                USB_DeviceSendRequest(deviceHandle, mscHandle->bulkInEndpoint, (uint8_t *)&mscHandle->g_mscCsw,
                                      USB_DEVICE_MSC_CSW_LENGTH);
                mscHandle->cswPrimeFlag = 1;
            }
        }
    }
    else if ((mscHandle->cbwValidFlag) && (event->length == USB_DEVICE_MSC_CBW_LENGTH) &&
             (mscHandle->g_mscCbw.signature == USB_DEVICE_MSC_DCBWSIGNATURE) &&
             (!((mscHandle->g_mscCbw.logicalUnitNumber & 0xF0U) || (mscHandle->g_mscCbw.cbLength & 0xE0U))) &&
             (mscHandle->g_mscCbw.logicalUnitNumber < (mscHandle->logicalUnitNumber + 1)) &&
             ((mscHandle->g_mscCbw.cbLength >= 0x01) && (mscHandle->g_mscCbw.cbLength <= 0x10)))
    {
        mscHandle->cbwPrimeFlag = 0;
        mscHandle->transferRemaining = 0;

        mscHandle->g_mscCsw.signature = USB_DEVICE_MSC_DCSWSIGNATURE;
        mscHandle->g_mscCsw.dataResidue = 0;
        mscHandle->g_mscCsw.tag = mscHandle->g_mscCbw.tag;

        mscHandle->cbwValidFlag = 0;

        mscHandle->g_mscCbw.dataTransferLength = USB_LONG_TO_LITTLE_ENDIAN(mscHandle->g_mscCbw.dataTransferLength);

        mscHandle->dataOutFlag = (uint8_t)(((!(mscHandle->g_mscCbw.flags & USB_DEVICE_MSC_CBW_DIRECTION_BIT)) &&
                                            (mscHandle->g_mscCbw.dataTransferLength)) ?
                                               1 :
                                               0);

        mscHandle->dataInFlag = (uint8_t)(((mscHandle->g_mscCbw.flags & USB_DEVICE_MSC_CBW_DIRECTION_BIT) &&
                                           (mscHandle->g_mscCbw.dataTransferLength)) ?
                                              1 :
                                              0);

        if ((0 != mscHandle->dataInFlag) && (0 != mscHandle->inEndpointStallFlag))
        {
            error = kStatus_USB_Error;
            return error;
        }
        error = USB_DeviceMscProcessUfiCommand(mscHandle);
        if (error == kStatus_USB_InvalidRequest)
        {
            if (mscHandle->dataOutFlag == 1)
            {
                if (mscHandle->outEndpointStallFlag == 0)
                {
                    mscHandle->needOutStallFlag = 1;
                }
                mscHandle->dataOutFlag = 0;
            }
            else if (mscHandle->dataInFlag == 1)
            {
                if (mscHandle->inEndpointStallFlag == 0)
                {
                    mscHandle->needInStallFlag = 1;
                }
                mscHandle->dataInFlag = 0;
            }
            else
            {
            }
            mscHandle->stallStatus = (uint8_t)USB_DEVICE_MSC_STALL_IN_DATA;
        }

        if (!((mscHandle->dataOutFlag) || ((mscHandle->dataInFlag) || (mscHandle->needInStallFlag))))
        {
            USB_DeviceSendRequest(deviceHandle, mscHandle->bulkInEndpoint, (uint8_t *)&mscHandle->g_mscCsw,
                                  USB_DEVICE_MSC_CSW_LENGTH);
            mscHandle->cswPrimeFlag = 1;
        }
    }
    else
    {
        USB_DeviceStallEndpoint(deviceHandle, mscHandle->bulkOutEndpoint);
        USB_DeviceStallEndpoint(deviceHandle, mscHandle->bulkInEndpoint);
        mscHandle->cbwValidFlag = 0;
        mscHandle->outEndpointStallFlag = 1;
        mscHandle->inEndpointStallFlag = 1;
        mscHandle->stallStatus = (uint8_t)USB_DEVICE_MSC_STALL_IN_CBW;
        mscHandle->performResetRecover = 1;
    }
    return error;
}
/*!
 * @brief Initialize the endpoints of the msc class.
 *
 * This callback function is used to initialize the endpoints of the msc class.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceMscEndpointsInit(void)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_endpoint_init_struct_t epInitStruct;
    usb_device_endpoint_callback_struct_t endpointCallback;
    usb_device_msc_struct_t *mscHandle;

    mscHandle = &(g_deviceComposite->mscDisk.handle);
    endpointCallback.callbackFn = USB_DeviceMscBulkIn;
    endpointCallback.callbackParam = (void *)mscHandle;

    epInitStruct.zlt = 0;
    epInitStruct.transferType = USB_ENDPOINT_BULK;
    epInitStruct.endpointAddress =
        USB_MSC_DISK_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
    mscHandle->bulkInEndpoint = epInitStruct.endpointAddress;
    if (USB_SPEED_HIGH == g_deviceComposite->speed)
    {
        epInitStruct.maxPacketSize = HS_MSC_DISK_BULK_IN_PACKET_SIZE;
    }
    else
    {
        epInitStruct.maxPacketSize = FS_MSC_DISK_BULK_IN_PACKET_SIZE;
    }

    USB_DeviceInitEndpoint(g_deviceComposite->deviceHandle, &epInitStruct, &endpointCallback);

    endpointCallback.callbackFn = USB_DeviceMscBulkOut;
    endpointCallback.callbackParam = (void *)mscHandle;

    epInitStruct.zlt = 0;
    epInitStruct.transferType = USB_ENDPOINT_BULK;
    epInitStruct.endpointAddress =
        USB_MSC_DISK_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
    mscHandle->bulkOutEndpoint = epInitStruct.endpointAddress;
    if (USB_SPEED_HIGH == g_deviceComposite->speed)
    {
        epInitStruct.maxPacketSize = HS_MSC_DISK_BULK_OUT_PACKET_SIZE;
    }
    else
    {
        epInitStruct.maxPacketSize = FS_MSC_DISK_BULK_OUT_PACKET_SIZE;
    }

    USB_DeviceInitEndpoint(g_deviceComposite->deviceHandle, &epInitStruct, &endpointCallback);
    mscHandle->dataOutFlag = 0;
    mscHandle->dataInFlag = 0;
    mscHandle->outEndpointStallFlag = 0;
    mscHandle->inEndpointStallFlag = 0;
    mscHandle->needOutStallFlag = 0;
    mscHandle->needInStallFlag = 0;
    mscHandle->cbwValidFlag = 1;
    mscHandle->transferRemaining = 0;
    mscHandle->performResetRecover = 0;
    mscHandle->performResetDoneFlag = 0;
    mscHandle->stallStatus = 0;

    if (mscHandle->cbwPrimeFlag == 1)
    {
        USB_DeviceCancel(g_deviceComposite->deviceHandle, mscHandle->bulkOutEndpoint);
    }
    USB_DeviceRecvRequest(g_deviceComposite->deviceHandle, mscHandle->bulkOutEndpoint, (uint8_t *)&mscHandle->g_mscCbw,
                          USB_DEVICE_MSC_CBW_LENGTH);
    mscHandle->cbwPrimeFlag = 1;

    return error;
}
/*!
 * @brief De-initialize the endpoints of the msc class.
 *
 * This callback function is used to de-initialize the endpoints of the msc class.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceMscEndpointsDeinit(void)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_msc_struct_t *mscHandle;

    mscHandle = &(g_deviceComposite->mscDisk.handle);
    error = USB_DeviceDeinitEndpoint(g_deviceComposite->deviceHandle, mscHandle->bulkInEndpoint);
    error = USB_DeviceDeinitEndpoint(g_deviceComposite->deviceHandle, mscHandle->bulkOutEndpoint);
    return error;
}

usb_status_t USB_DeviceMscDiskConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_msc_struct_t *mscHandle;

    mscHandle = &(g_deviceComposite->mscDisk.handle);
    if (status)
    {
        if ((USB_MSC_DISK_BULK_IN_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80U))
        {
            if (mscHandle->inEndpointStallFlag == 0)
            {
                mscHandle->inEndpointStallFlag = 1;
                error = USB_DeviceStallEndpoint(handle, ep);
            }
        }
        else if ((USB_MSC_DISK_BULK_OUT_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80U))
        {
            if (mscHandle->outEndpointStallFlag == 0)
            {
                mscHandle->outEndpointStallFlag = 1;
                error = USB_DeviceStallEndpoint(handle, ep);
            }
        }
        else
        {
        }
    }
    else
    {
        if ((USB_MSC_DISK_BULK_IN_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80U))
        {
            if (mscHandle->inEndpointStallFlag == 1)
            {
                mscHandle->inEndpointStallFlag = 0;
                error = USB_DeviceUnstallEndpoint(handle, ep);
            }
        }
        else if ((USB_MSC_DISK_BULK_OUT_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80U))
        {
            if (mscHandle->outEndpointStallFlag == 1)
            {
                mscHandle->outEndpointStallFlag = 0;
                error = USB_DeviceUnstallEndpoint(handle, ep);
            }
        }
        else
        {
        }
    }
    if (((mscHandle->stallStatus == USB_DEVICE_MSC_STALL_IN_CSW) ||
         (mscHandle->stallStatus == USB_DEVICE_MSC_STALL_IN_DATA)) &&
        (mscHandle->performResetDoneFlag != 1))
    {
        if (mscHandle->cswPrimeFlag == 1)
        {
            USB_DeviceCancel(handle, mscHandle->bulkInEndpoint);
        }
        USB_DeviceSendRequest(handle, mscHandle->bulkInEndpoint, (uint8_t *)&mscHandle->g_mscCsw,
                              USB_DEVICE_MSC_CSW_LENGTH);
        mscHandle->cswPrimeFlag = 0;
        mscHandle->stallStatus = 0;
    }
    if ((mscHandle->performResetDoneFlag == 1) && (mscHandle->inEndpointStallFlag == 0) &&
        (mscHandle->outEndpointStallFlag == 0))
    {
        mscHandle->performResetDoneFlag = 0;
        if (mscHandle->cswPrimeFlag == 1)
        {
            USB_DeviceCancel(handle, mscHandle->bulkInEndpoint);
        }
        USB_DeviceRecvRequest(handle, mscHandle->bulkOutEndpoint, (uint8_t *)&mscHandle->g_mscCbw,
                              USB_DEVICE_MSC_CBW_LENGTH);
        mscHandle->cswPrimeFlag = 0;
        mscHandle->stallStatus = 0;
    }
    return error;
}

usb_status_t USB_DeviceMscDiskClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_msc_struct_t *mscHandle;

    mscHandle = &(g_deviceComposite->mscDisk.handle);
    if (setup->wIndex != USB_MSC_DISK_INTERFACE_INDEX)
    {
        return error;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_MSC_GET_MAX_LUN:
            if ((setup->wIndex == USB_MSC_DISK_INTERFACE_INDEX) && (!setup->wValue) && (setup->wLength <= 0x0001) &&
                ((setup->bmRequestType & USB_REQUSET_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_IN))
            {
                *buffer = (uint8_t *)&mscHandle->logicalUnitNumber;
                *length = setup->wLength;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case USB_DEVICE_MSC_BULK_ONLY_MASS_STORAGE_RESET:
            if ((setup->wIndex == mscHandle->interfaceNumber) && (!setup->wValue) && (!setup->wLength) &&
                ((setup->bmRequestType & USB_REQUSET_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_OUT))
            {
                error = USB_DeviceMscEndpointsDeinit();
                error = USB_DeviceMscEndpointsInit();
                mscHandle->performResetRecover = 0;
                mscHandle->performResetDoneFlag = 1;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        default:
            break;
    }
    return error;
}

usb_status_t USB_DeviceMscDiskSetConfigure(usb_device_handle handle, uint8_t configure)
{
    usb_status_t error = kStatus_USB_Error;

    g_deviceComposite->mscDisk.attach = 1;
    error = USB_DeviceMscEndpointsInit();
    return error;
}

usb_status_t USB_DeviceMscDiskInit(usb_device_composite_struct_t *deviceComposite)
{
    usb_device_msc_ufi_struct_t *ufi = NULL;
    usb_device_msc_struct_t *mscHandle;
    g_deviceComposite = deviceComposite;
    g_deviceComposite->mscDisk.storageDisk[0] = 0x01;

    mscHandle = &(g_deviceComposite->mscDisk.handle);

    mscHandle->handle = deviceComposite->deviceHandle;
    ufi = &mscHandle->g_mscUfi;
    mscHandle->totalLogicalBlockNumber = TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL;
    mscHandle->lengthOfEachLba = LENGTH_OF_EACH_LBA;
    mscHandle->logicalUnitNumber = LOGICAL_UNIT_SUPPORTED - 1;
    mscHandle->bulkInBufferSize = DISK_SIZE_NORMAL;
    mscHandle->bulkOutBufferSize = DISK_SIZE_NORMAL;
    mscHandle->implementingDiskDrive = USB_DEVICE_CONFIG_MSC_IMPLEMENTING_DISK_DRIVE;

    if (mscHandle->lengthOfEachLba * mscHandle->totalLogicalBlockNumber == 0)
    {
        return kStatus_USB_Error;
    }
    ufi->requestSense.validErrorCode = USB_DEVICE_MSC_UFI_REQ_SENSE_VALID_ERROR_CODE;
    ufi->requestSense.additionalSenseLength = USB_DEVICE_MSC_UFI_REQ_SENSE_ADDITIONAL_SENSE_LEN;
    ufi->requestSense.senseKey = USB_DEVICE_MSC_UFI_NO_SENSE;
    ufi->requestSense.additionalSenseCode = USB_DEVICE_MSC_UFI_NO_SENSE;
    ufi->requestSense.additionalSenseQualifer = USB_DEVICE_MSC_UFI_NO_SENSE;

    ufi->readCapacity.lastLogicalBlockAddress = USB_LONG_TO_BIG_ENDIAN(mscHandle->totalLogicalBlockNumber - 1);
    ufi->readCapacity.blockSize = USB_LONG_TO_BIG_ENDIAN((uint32_t)mscHandle->lengthOfEachLba);
    ufi->readCapacity16.lastLogicalBlockAddress1 = USB_LONG_TO_BIG_ENDIAN(mscHandle->totalLogicalBlockNumber - 1);
    ufi->readCapacity16.blockSize = USB_LONG_TO_BIG_ENDIAN((uint32_t)mscHandle->lengthOfEachLba);

    mscHandle->cbwPrimeFlag = 0;
    mscHandle->cswPrimeFlag = 0;
    return kStatus_USB_Success;
}
