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
