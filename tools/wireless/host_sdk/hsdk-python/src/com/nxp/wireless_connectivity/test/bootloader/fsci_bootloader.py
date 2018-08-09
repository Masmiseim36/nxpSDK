#!/usr/bin/env python
'''
* The Clear BSD License
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

from com.nxp.wireless_connectivity.hsdk import utils, config
config.MAX_SPEED_HZ = 2000000  # bootloader supports higher speeds than a stack

from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.commands.firmware.sync_requests import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.framing.fsci_command import FsciCommand
from com.nxp.wireless_connectivity.hsdk.utils import crc16xmodem

import os
from threading import Event
from time import sleep, time


SECTOR_SIZE = 2048
KW_NVM_SECTOR_START, KW_NVM_SECTOR_END = 0x0006F800 / SECTOR_SIZE, 0x0007F7FF / SECTOR_SIZE
CHUNK_LEN = 2048

new_request = Event()
new_request.set()
bytes_sent, seq_no = 0, 0


def usage():
    import argparse

    parser = argparse.ArgumentParser(
        description='Script to flash a binary file using the FSCI bootloader.')
    parser.add_argument(
        'dev', metavar='serial_port', type=str, help='Kinetis-W system device node.')
    parser.add_argument(
        'bin', metavar='binary_file', type=str, help='The binary file to be written.')
    parser.add_argument(
        '-s', '--chunk-size', help='Push chunks this large (in bytes). Defaults to 2048.',
        type=int, default=2048)
    parser.add_argument(
        '-d', '--disable-crc', help='Disable the CRC check on commit image.', action='store_true')
    parser.add_argument(
        '-e', '--erase-nvm', help='Erase the non-volatile memory. Useful if gEraseNVMLink_d=0 in Linker flags, '
        'otherwise the NVM is erased by default.', action='store_true')

    args = parser.parse_args()
    print args

    return args


def nvm_erase_bitmask(start_sector=KW_NVM_SECTOR_START, end_sector=KW_NVM_SECTOR_END):
    result = bytearray()
    index = 0

    NVM_ERASE_BITMASK = '0' * (start_sector - 1) + \
                        '1' * (end_sector - start_sector + 1) + \
                        '0' * (256 - end_sector)
    NVM_ERASE_BITMASK = int(NVM_ERASE_BITMASK, 2)

    while NVM_ERASE_BITMASK:
        result.append(NVM_ERASE_BITMASK & 0xFF)
        NVM_ERASE_BITMASK >>= 8
        index += 1

    return bytearray(32 - index) + result[::-1]


def read_image(path):
    if not os.path.isfile(path):
        sys.exit(path + ' does not exist or is inaccessible.')

    with open(path, 'rb') as fin:
        contents = fin.read()

    # remove useless 0xFFs at the end
    to_remove = bytearray([0xFF for _ in range(SECTOR_SIZE)])
    while contents.endswith(to_remove):
        contents = contents[:-SECTOR_SIZE]
    size = len(contents)

    return size, contents


def cb_push_image_cnf(devName, cnf):
    global bytes_sent, seq_no

    if cnf.Status == 'Success':
        bytes_sent = bytes_sent + CHUNK_LEN
        seq_no = (seq_no + 1) & 0xFF
        new_request.set()
    else:
        # repeat
        new_request.set()


def flash_image(args, size, contents):
    '''
    Flashes a binary image over FSCI.

    @param device: destination device
    @param size: size of the image in bytes
    @param contents: actual contents of the image as byte array
    '''

    FSCIEnterBootloader(args.dev)
    sleep(.5)

    confirm = FSCIFirmware_CancelProcess(args.dev)
    if confirm is None or confirm.Status != 'Success':
        sys.exit('Cannot communicate with the board, please try again. Exiting..')

    confirm = FSCIFirmware_StartImage(args.dev, ImageSize=size)
    assert confirm.Status == 'Success', 'Start Image Failed'

    # turn off FSCI-related prints to avoid flooding the console
    utils.VERBOSE = False

    # add a callback on push image request.
    comm = Comm(args.dev, ack_policy=FsciAckPolicy.GLOBAL,
                protocol=Protocol.Firmware, baudrate=Baudrate.BR115200)
    comm.fsciFramer.addObserver(FSCIFirmware_PushImageChunkConfirmObserver(
        'FSCIFirmware_PushImageChunkConfirm'), cb_push_image_cnf)

    # start transfer
    start = time()
    while new_request.wait():

        if bytes_sent < size:

            print '\rProgress: ', '{0:.2f}%'.format(100 * float(bytes_sent) / float(size)),
            comm.fsciFramer.send(FsciCommand(
                opGroup=0xA3,
                opCode=0x2A,
                payload=bytearray(chr(seq_no) + contents[bytes_sent:bytes_sent + CHUNK_LEN])
            ))
            new_request.clear()

        elif bytes_sent >= size:
            print '\rProgress: ', '100.00%',
            break
    end = time()
    print '\nAll chunks sent in', end - start, 'seconds.'

    # turn on FSCI-related prints
    utils.VERBOSE = True

    # measure commit image time
    start = time()
    CRCval, BitMask = bytearray(2), bytearray(32)

    if not args.disable_crc:
        CRCval = crc16xmodem(contents)
    if args.erase_nvm:
        BitMask = nvm_erase_bitmask()

    confirm = FSCIFirmware_CommitImage(args.dev, BitMask, not args.disable_crc, CRCval)
    end = time()
    print 'Commit image completed in', end - start, 'seconds.'

    if confirm.Status == 'CRCCheckError' and args.disable_crc:
        print '[FAILED] Bootloader has gFsciUseCRC_c = TRUE;',
        print 'please enable the CRC check by removing -d/--disable-crc.'
    elif confirm.Status == 'CRCCheckError' and not args.disable_crc:
        print '[FAILED] CRC validation failed.'
    elif confirm.Status != 'Success':
        print '[FAILED] Something went wrong. Commit image return status is', confirm.Status

    FSCICPUReset(args.dev)
    comm.fsciFramer.device.close()

if __name__ == '__main__':

    start = time()
    args = usage()

    # CHUNK_LEN is also used by cb_push_image_cnf, so make it global
    CHUNK_LEN = args.chunk_size
    flash_image(args, *read_image(args.bin))
    end = time()

    print 80 * '='
    print 'TOTAL EXECUTION TIME:', end - start, 'seconds.'
    print 80 * '='
