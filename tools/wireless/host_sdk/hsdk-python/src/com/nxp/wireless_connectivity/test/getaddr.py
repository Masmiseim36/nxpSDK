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

import sys

from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.commands.thread.enums import \
    THR_GetThreadIpAddrRequestAddressType, THR_MgmtDiagnosticGetRequestTlvId
from com.nxp.wireless_connectivity.commands.thread.sync_requests import *
from com.nxp.wireless_connectivity.hsdk.utils import list_to_int
from time import sleep


IN6ADDR_REALMLOCAL_ALLNODES_INIT = [
    0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
]


def usage():
    '''
    Define the command-line interface.
    '''
    import argparse

    parser = argparse.ArgumentParser(
        description='Demo script to obtain IPv6 addresses of all nodes')
    parser.add_argument('dev', metavar='<serial_port host_controlled_device>',
                        type=str, help='Kinetis-W system device node.')
    args = parser.parse_args()
    print args

    return args


def display_local(args):
    print 30 * '=' + ' LOCAL ADDRESSES ' + 30 * '='

    response = NWKU_IfconfigAll(args.dev)
    for interface in response.InterfaceList:
        print interface.__dict__

    response = THR_GetThreadIpAddr(
        args.dev,
        InstanceId=0,
        AddressType=THR_GetThreadIpAddrRequestAddressType.Link_Local_64
    )
    try:
        print 'LL64 -> %x' % list_to_int(response.AddressList, False)
    except ValueError:
        print 'LL64 -> None'

    response = THR_GetThreadIpAddr(
        args.dev,
        InstanceId=0,
        AddressType=THR_GetThreadIpAddrRequestAddressType.MLEID
    )
    try:
        print 'MLEID -> %x' % list_to_int(response.AddressList, False)
    except ValueError:
        print 'MLEID -> None'

    response = THR_GetThreadIpAddr(
        args.dev,
        InstanceId=0,
        AddressType=THR_GetThreadIpAddrRequestAddressType.RLOC
    )
    try:
        print 'RLOC -> %x' % list_to_int(response.AddressList, False)
    except ValueError:
        print 'RLOC -> None'

    response = THR_GetThreadIpAddr(
        args.dev,
        InstanceId=0,
        AddressType=THR_GetThreadIpAddrRequestAddressType.Global,
        StartIndex=0,
        NoOfElements=1
    )
    try:
        print 'Global -> %x' % list_to_int(response.AddressList, False)
    except ValueError:
        print 'Global -> None'

    print


def display_remote(args):
    print 30 * '=' + ' REMOTE ADDRESSES ' + 30 * '='

    cnf = THR_MgmtDiagnosticGet(
        args.dev,
        DestIpAddr=IN6ADDR_REALMLOCAL_ALLNODES_INIT,
        NumberOfTlvIds=2,
        TlvIds=[THR_MgmtDiagnosticGetRequestTlvId.ShortAddr,
                THR_MgmtDiagnosticGetRequestTlvId.Ip6AddrList],
    )
    assert cnf.Status == 'Success'

    # wait for all nodes to respond
    sleep(5)


def cb_diag_get_rsp(devName, indication):
    for tlv in indication.TLVs:
        if tlv.type == 'Ip6AddrList':
            print tlv.type, '->',
            for i in range(0, len(tlv.raw_value), 16):
                rawstr = ''.join('%02x' % x for x in tlv.raw_value[i:i+16])
                ip6str = ':'.join(rawstr[j:j+4]
                                  for j in range(0, len(rawstr), 4))
                print ip6str,
            print
        else:
            print tlv.type, '->', tlv.value


if __name__ == '__main__':

    args = usage()

    display_local(args)

    # add a callback on diagnostic indication
    comm = Comm(args.dev, ack_policy=FsciAckPolicy.GLOBAL,
                protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm.fsciFramer.addObserver(THR_MgmtDiagnosticGetRspIndicationObserver(
        'THR_MgmtDiagnosticGetRspIndication'), cb_diag_get_rsp)

    display_remote(args)
