#!/usr/bin/env python
'''
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from __future__ import division

from test_dongle_mode import *  # @UnusedWildImport


# Events to change OTA server state
query_img_event = Event()
chunk_event = Event()

# Global variables to save information from callbacks
last_offset = 0
block_len = 150


def cb_standalone_query_image_ind(_, event):
    print_event(dev, event)
    query_img_event.set()


def cb_standalone_chunk_ind(_, event):
    global last_offset, block_len

    print_event(dev, event)
    last_offset = event.ImageOffset
    block_len = event.BlockLength
    chunk_event.set()

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

    # Expect these async events
    comm = Comm(dev, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm.fsciFramer.addObserver(FSCIFirmware_QueryImageIndicationObserver('FSCIFirmware_QueryImageIndication'), cb_standalone_query_image_ind)
    comm.fsciFramer.addObserver(FSCIFirmware_ChunkIndicationObserver('FSCIFirmware_ChunkIndication'), cb_standalone_chunk_ind)

    # Create the network
    start_leader_as_commissioner(dev)
    raw_input(79 * '=' + '\nPress ENTER after joining all the OTA clients to the network...\n' + 79 * '=' + '\n')

    FSCIFirmware_CancelProcess(dev)
    cnf = FSCIFirmware_StartImage(dev, header.totalImageSize)
    if not cnf.ExternalMemorySupported:
        sys.exit('The OTA server does not have enough memory to store the image -> standalone mode will not work. Please use another form factor or use the Dongle Mode.')

    FSCIFirmware_SetOperationMode(dev, OperationMode=FSCIFirmware_SetOperationModeRequestOperationMode.Standalone)

    while not query_img_event.is_set():
        sleep(.1)

    FSCIFirmware_QueryImageRsp(
        dev,
        OtaImageStatus=FSCIFirmware_QueryImageRspRequestOtaImageStatus.Success,
        DeviceId=0xFFFF,
        ManufacturerCode=header.manufacturerCode,
        ImageType=header.imageType,
        FileVersion=header.fileVersion,
        ImageSize=header.totalImageSize,
    )

    # Turn off console prints
    utils.VERBOSE = False

    while last_offset < image_size:

        # Simple stat
        print '\r' + '{percent:.2%}'.format(percent=last_offset / image_size),

        # Wait for a chunk request
        while not chunk_event.is_set():
            sleep(.01)
        chunk_event.clear()

        # If last chunk make the loop break
        if image_size - last_offset < block_len:
            FSCIFirmware_PushImageChunk(dev, DataImageBlock=raw[last_offset:])
            last_offset = image_size
            print '\r' + '{percent:.2%}'.format(percent=last_offset / image_size),

        # Not last chunk
        else:
            FSCIFirmware_PushImageChunk(dev, DataImageBlock=raw[last_offset:last_offset + block_len])

    print 'New firmware has been transferred to the OTA server successfully.'
    print 'The update procedure continues w/o host intervention, please observe OTA traffic.'
