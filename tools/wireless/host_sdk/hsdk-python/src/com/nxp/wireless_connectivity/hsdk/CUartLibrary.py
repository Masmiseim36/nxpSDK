'''
* The Clear BSD License
* Copyright 2014-2015 Freescale Semiconductor, Inc.
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

from ctypes import Structure, c_char_p, c_int, c_uint8
import sys


class DeviceState(Structure):

    '''
    ctypes Structure that maps over the DeviceState C structure.
    '''

    _fields_ = [
        ('availability', c_int),
        ('friendlyName', c_char_p),
        ('deviceName', c_char_p),
        ('isKinetisWDevice', c_uint8),
        ('vid', c_char_p),
        ('pid', c_char_p)
    ]

    def __str__(self):
        s = ''
        # get Availability human-readable name
        for k, v in Availability.__dict__.items():
            if v == self.availability:
                s += k + ' '
        s += str(self.friendlyName) + ' '
        s += str(self.deviceName) + ' '
        s += str(bool(self.isKinetisWDevice)) + ' '
        s += str(self.vid) + ' '
        s += str(self.pid)
        return s


class DeviceType(object):
    UART = 0
    USB = 1
    PCAP = 2
    SPI = 3
    BT = 4


class Baudrate(object):
    BR110 = 0
    BR300 = 1
    BR600 = 2
    BR1200 = 3
    BR2400 = 4
    BR4800 = 5
    BR9600 = 6
    BR19200 = 7
    BR38400 = 8
    BR57600 = 9
    BR115200 = 10
    BR921600 = 11


class Availability(object):
    NoInfo = 0
    Available = 1
    NotPluggedIn = 2
    InUse = 3
    DeviceError = 4
