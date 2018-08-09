#!/usr/bin/env python
'''
* The Clear BSD License
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

from com.nxp.wireless_connectivity.hsdk import utils
from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.commands.firmware.spec import Spec as _Spec
from com.nxp.wireless_connectivity.commands.firmware.sync_requests import *
from com.nxp.wireless_connectivity.commands.thread.sync_requests import *

from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.ota_server import OTAFileHeader

from math import floor
from threading import Event
from time import time

FwSpec = _Spec()

# Constants for commissioning
JOINER_EXT_ADDR = 0xFFFFFFFFFFFFFFFF
JOINER_PSKD = 'THREAD'

# How many devices shall be served?
remaining_devices = 0
# Last device served
last_device_id = None
# Dict with floored progress values / device
progress = {}

query_img_event = Event()

##############################################################################
# Thread management methods
##############################################################################


def factory_reset(device):
    confirm = THR_FactoryReset(device)
    if confirm.Status != 'Success':
        print '[FAIL] Factory Reset'
        return False
    return True


def set_channel(device, channel):
    confirm = THR_SetAttr(
        device,
        AttributeId=THR_SetAttrRequestAttributeId.Channel,
        AttrSize=1,
        AttributeValue=int(channel)
    )
    if confirm.Status != 'Success':
        print '[FAIL] THR_SetAttr Channel'
        return False
    return True


def configure(device, channel):
    subscribe_to_async_thread_events_from(device)
    assert factory_reset(device), 'Factory Reset failed'
    if channel is not None:
        assert set_channel(device, channel), 'Set Channel failed'


def start_commissioner(device):
    confirm = THR_CreateNwk(device, InstanceID=0, timeout=8)
    if confirm.Status != 'OK':
        return False
    # expect async events: watch console
    sleep(7)

    confirm = MESHCOP_StartCommissioner(device, InstanceId=0)
    if confirm.Status != 'Success':
        return False

    confirm = MESHCOP_RemoveAllExpectedJoiners(device, InstanceId=0)
    if confirm.Status != 'Success':
        return False

    return True


def allow_joiner(device, joiner_ext_addr, joiner_pskd):
    confirm = MESHCOP_AddExpectedJoiner(
        device,
        Selected=True,
        EuiType=MESHCOP_AddExpectedJoinerRequestEuiType.LongEUI,
        EUI=joiner_ext_addr,
        PSKdSize=len(joiner_pskd),
        PSKd=joiner_pskd
    )
    if confirm.Status != 'Success':
        return False

    if joiner_ext_addr == 0xFFFFFFFFFFFFFFFF:
        EuiMask = MESHCOP_SyncSteeringDataRequestEuiMask.AllFFs
    else:
        EuiMask = MESHCOP_SyncSteeringDataRequestEuiMask.ExpectedJoiners

    confirm = MESHCOP_SyncSteeringData(
        device,
        InstanceId=0,
        EuiMask=EuiMask
    )
    if confirm.Status != 'Success':
        return False

    return True


def start_leader_as_commissioner(device, channel=None):
    configure(device, channel)
    assert start_commissioner(device), 'Start Commissioner failed'
    assert allow_joiner(device, JOINER_EXT_ADDR, JOINER_PSKD), 'Allow Joiner into Network failed'


##############################################################################
# Callbacks for OTA processing
##############################################################################


def cb_query_image_ind(devName, event):
    global remaining_devices

    query_img_event.set()
    remaining_devices += 1
    progress[event.DeviceId] = 0
    print_event(dev, event)

    comm.send(
        FwSpec.FSCIFirmware_QueryImageRspRequestFrame,
        FSCIFirmware_QueryImageRspRequest(
            OtaImageStatus=FSCIFirmware_QueryImageRspRequestOtaImageStatus.Success,
            DeviceId=event.DeviceId,
            ManufacturerCode=header.manufacturerCode,
            ImageType=header.imageType,
            FileVersion=header.fileVersion,
            ImageSize=header.totalImageSize,
        )
    )


def cb_chunk_ind(devName, event):
    global remaining_devices, last_device_id

    # new line if different device this time, so that \r does not delete current progress
    if event.DeviceId != last_device_id:
        last_device_id = event.DeviceId

    comm.send(
        FwSpec.FSCIFirmware_PushImageChunkRequestFrame,
        FSCIFirmware_PushImageChunkRequest(DataImageBlock=raw[event.ImageOffset:event.ImageOffset + event.BlockLength])
    )

    # basic stats
    if event.ImageOffset + event.BlockLength >= image_size:
        print '\rDevice ID', event.DeviceId, '-> 100% COMPLETED!',
        remaining_devices -= 1
    else:
        fraction = (float)(event.ImageOffset + event.BlockLength) / image_size
        if progress[event.DeviceId] != floor(fraction * 100):
            progress[event.DeviceId] = floor(fraction * 100)
            print '\rDevice ID', event.DeviceId, '-> {:.2%}'.format(fraction),


if __name__ == '__main__':

    if len(sys.argv) < 3:
        print 'Usage # %s <server tty> <.thread ota_client file>' % sys.argv[0]
        sys.exit(1)

    dev = sys.argv[1]

    # Read formatted image
    with open(sys.argv[2], 'rb') as fin:
        raw = fin.read()

    header = OTAFileHeader(raw)
    image_size = list_to_int([ord(x) for x in header.totalImageSize])

    # Expect these async events, to be processed by previously defined callbacks.
    comm = Comm(dev, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm.fsciFramer.addObserver(
        observer=FSCIFirmware_QueryImageIndicationObserver('FSCIFirmware_QueryImageIndication'),
        callback=cb_query_image_ind
    )
    comm.fsciFramer.addObserver(
        observer=FSCIFirmware_ChunkIndicationObserver('FSCIFirmware_ChunkIndication'),
        callback=cb_chunk_ind
    )

    # Create the network
    start_leader_as_commissioner(dev)
    raw_input(79 * '=' + '\nPress ENTER after joining all the OTA clients to the network...\n' + 79 * '=' + '\n')

    FSCIFirmware_CancelProcess(dev)
    FSCIFirmware_StartImage(dev, header.totalImageSize)
    FSCIFirmware_SetOperationMode(dev, OperationMode=FSCIFirmware_SetOperationModeRequestOperationMode.Dongle)
    FSCIFirmware_ImageNotify(
        dev,
        DeviceId=0xFFFF,
        ManufacturerCode=header.manufacturerCode,
        ImageType=header.imageType,
        ImageSize=header.imageSize,
        FileSize=header.totalImageSize,
        FileVersion=header.fileVersion
    )

    print 'Waiting for an Image Query from an OTA client..'
    while not query_img_event.is_set():
        sleep(.1)

    start = time()
    if remaining_devices == 0:
        print 'OTA clients not present in the network. Exiting..'

    # turn off FSCI-related prints to avoid flooding the console
    utils.VERBOSE = False

    # serve forever on threads started in callbacks
    while remaining_devices > 0:
        try:
            sleep(1)
        except KeyboardInterrupt:
            print 'Exiting at user input..'
            sys.exit(1)
    end = time()

    print '\nCompleted in', end - start, 'seconds!'
