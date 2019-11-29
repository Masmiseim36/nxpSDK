#!/usr/bin/env python
'''
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.commands.thread.sync_requests import *  # @UnusedWildImport


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
        0,
        DestIpAddr=IN6ADDR_REALMLOCAL_ALLNODES_INIT,
        NumberOfTlvIds=2,
        TlvIds=[THR_MgmtDiagnosticGetRequestTlvId.ShortAddr,
                THR_MgmtDiagnosticGetRequestTlvId.Ip6AddrList],
    )
    assert cnf.Status == 'Success'

    # wait for all nodes to respond
    sleep(5)


def cb_diag_get_rsp(_, indication):
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

    subscribe_to_async_thread_events_from(sys.argv[1])
    
    THR_FactoryReset(sys.argv[1])

    THR_CreateNwk(sys.argv[1], InstanceID=0)
    sleep(7)

    display_local(args)

    # add a callback on diagnostic indication
    comm = Comm(args.dev, ack_policy=FsciAckPolicy.GLOBAL,
                protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm.fsciFramer.addObserver(THR_MgmtDiagnosticGetRspIndicationObserver(
        'THR_MgmtDiagnosticGetRspIndication'), cb_diag_get_rsp)

    display_remote(args)
