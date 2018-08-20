#!/usr/bin/env python
'''
* The Clear BSD License
* Copyright 2017 NXP
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

from com.nxp.wireless_connectivity.hsdk import config
config.MONITOR_MODE = True

from com.nxp.wireless_connectivity.hsdk.utils import to_bytes
from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from time import sleep

from com.nxp.wireless_connectivity.commands.zigbee.sync_requests import *  # @UnusedWildImport


class ZigBeeDevice(object):

    def __init__(self, serial_port, channel):
        self.serial_port = serial_port
        self.channel = channel

        self.comm = Comm(self.serial_port,
                         FsciAckPolicy.NONE,
                         Protocol.ZigBee,
                         baudrate=Baudrate.BR115200)

        self.comm.fsciFramer.addObserver(
            observer=NetworkJoinedFormedObserver('NetworkJoinedFormed'),
            callback=self.cb_save_short_addr)

        self.short_addr = None

    def cb_save_short_addr(self, devName, event):
        print_event(devName, event)
        self.short_addr = event.ShortAddr

    def factory_reset(self):
        self.comm.send(Spec.ErasePersistentDataFrame, Frames.ErasePersistentData())
        sleep(3)

    def set_channel_mask(self):
        self.comm.send(Spec.SetChannelMaskFrame,
                       Frames.SetChannelMask(ChannelMask=to_bytes(1 << self.channel, 4, False)))

    def start_network(self):
        self.comm.send(Spec.StartNetworkMessageFrame, Frames.StartNetworkMessage())
        sleep(3)
        self.comm.send(Spec.PermitJoiningRequestFrame,
                       Frames.PermitJoiningRequest(
                           TargetShortAddress=[0xFF, 0xFC],
                           Interval=0xFF,  # enable permanently
                           TCSignificance=PermitJoiningRequestTCSignificance.NoChangeInAuthentication
                       ))
        sleep(1)

    def join_network(self):
        self.comm.send(Spec.SetDeviceTypeFrame, Frames.SetDeviceType(SetDeviceTypeDeviceType.Router))
        self.comm.send(Spec.StartNetworkScanFrame, Frames.StartNetworkScan())
        sleep(3)

    def disallow_join(self):
        self.comm.send(Spec.PermitJoiningRequestFrame,
                       Frames.PermitJoiningRequest(
                           TargetShortAddress=[0xFF, 0xFC],
                           Interval=0x00,  # disable permanently
                           TCSignificance=PermitJoiningRequestTCSignificance.NoChangeInAuthentication
                       ))
        sleep(1)

    def active_ep_req(self, target_short_addr):
        assert ActiveEndpoint(self.serial_port, target_short_addr) is not None, 'Active Endpoint Request Failed!'
        sleep(.1)


if __name__ == '__main__':

    if len(sys.argv) < 4:
        print 'Usage: %s <coord_serial_port> <router_serial_port> <802.15.4 channel>' % sys.argv[0]
        sys.exit(1)

    coord, router, channel = sys.argv[1], sys.argv[2], int(sys.argv[3])

    assert 11 <= channel and channel <= 26, 'Incorrect channel value, must be in [11-26].'
    ZC = ZigBeeDevice(coord, channel)
    ZR = ZigBeeDevice(router, channel)

    print '\n' + 30 * '=' + ' Factory Reset ' + 30 * '='
    ZC.factory_reset()
    ZR.factory_reset()

    print '\n' + 30 * '=' + ' Set 802.15.4 Channel ' + 30 * '='
    ZC.set_channel_mask()
    ZR.set_channel_mask()

    print '\n' + 30 * '=' + ' Create Network and Permit Join for All Devices ' + 30 * '='
    ZC.start_network()

    print '\n' + 30 * '=' + ' Join Network ' + 30 * '='
    ZR.join_network()

    assert ZR.short_addr is not None, 'Router failed to join the network!'

    print '\n' + 30 * '=' + ' Test Connectivity with an Active Endpoint Request ' + 30 * '='
    ZC.active_ep_req(ZR.short_addr)

    print '\n' + 30 * '=' + ' Disallow Join ' + 30 * '='
    ZC.disallow_join()

    # wait for last confirmations to be printed out
    sleep(1)
