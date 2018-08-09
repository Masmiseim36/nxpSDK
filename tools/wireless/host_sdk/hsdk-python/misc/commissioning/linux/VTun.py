#!/usr/bin/env python
'''
* The Clear BSD License
* Copyright 2014-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
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

from binascii import hexlify
from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.commands.thread.enums import SerialTun_LinkIndicationRequestState
from com.nxp.wireless_connectivity.commands.thread.events import SerialTun_IPPacketReceivedConfirmObserver
from com.nxp.wireless_connectivity.commands.thread.frames import SerialTun_IPPacketSendRequest
from com.nxp.wireless_connectivity.commands.thread.spec import Spec
from com.nxp.wireless_connectivity.commands.thread.sync_requests import SerialTun_LinkIndication
from com.nxp.wireless_connectivity.hsdk.device.device_manager import DeviceManager

import commands
import threading

from pytun import TunTapDevice, IFF_TUN, IFF_TAP, IFF_NO_PI


class VTunInterface(threading.Thread):
    '''
    Handles the sending/receiving of data over the TUN/TAP interface.
    '''

    def __init__(self, port, tun_mode=True, debug=False):
        threading.Thread.__init__(self)

        self.port = port
        self.debug = debug

        # enable forwarding on host
        print commands.getoutput('sysctl -w net.ipv6.conf.all.forwarding=1')

        # create a TUN/TAP device and get an object pointing to it
        if tun_mode:
            print commands.getoutput('ip -6 tuntap add mode tun threadtun0')
            print commands.getoutput('ip -6 addr add fd01::2 dev threadtun0')
            print commands.getoutput('ip -6 route add fd01::1 dev threadtun0')
            print commands.getoutput('ip -6 route add fd01:0000:0000:3ead::/64 dev threadtun0')
            print commands.getoutput('ip link set threadtun0 up')

            self.tun = TunTapDevice(name='threadtun0', flags=IFF_TUN | IFF_NO_PI)
        else:
            print commands.getoutput('ip -6 tuntap add mode tap threadtap0')
            print commands.getoutput('ip -6 addr add fd01::2 dev threadtap0')
            print commands.getoutput('ip -6 route add fd01::1 dev threadtap0')
            print commands.getoutput('ip -6 route add fd01:0000:0000:3ead::/64 via fe80::260:37ff:fe00:fa5c dev threadtap0')
            print commands.getoutput('ip link set threadtap0 up')

            self.tun = TunTapDevice(name='threadtap0', flags=IFF_TAP | IFF_NO_PI)
        self.tun.up()

        # open FSL device
        self.comm = Comm(self.port)

        observer = SerialTun_IPPacketReceivedConfirmObserver('RX')
        self.comm.fsciFramer.addObserver(observer, self.event_callback)

    def event_callback(self, devName, event):
        '''
        Gets executed by libframer.so whenever the board sends VTun packets back.
        @param event: VTUNIPPacketReceivedConfirm object, IPpayload of interest
        '''
        # concatenate bytes into a string
        response = ''.join([chr(x) for x in event.IPpayload])
        # send the response on the tun interface
        if self.debug:
            print 'RX: ' + hexlify(response) + '\n'
        self.tun.write(response)

    def run(self):

        SerialTun_LinkIndication(self.port, SerialTun_LinkIndicationRequestState.Up)

        while 1:
            try:
                # read data from the TUN interface
                buf = self.tun.read(self.tun.mtu)
                if self.debug:
                    print 'TX: ' + hexlify(buf)

                # send it to the Kinetis-W device, FSCI encapsulated
                buf_fsci = SerialTun_IPPacketSendRequest(
                    Size=bytearray([len(buf) & 0x00FF, (len(buf) & 0xFF00) >> 8]),
                    Data=bytearray(buf)
                )
                self.comm.send(Spec().SerialTun_IPPacketSendRequestFrame, buf_fsci, printCmd=False)

            except KeyboardInterrupt:
                self.tun.close()
                break

if __name__ == '__main__':
    vtun_thread = VTunInterface('/dev/ttyACM0')
    vtun_thread.start()
