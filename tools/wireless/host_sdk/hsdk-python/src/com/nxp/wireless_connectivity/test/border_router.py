#!/usr/bin/env python
'''
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from com.nxp.wireless_connectivity.commands.thread.sync_requests import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.framing.fsci_framer import FsciFramer
from com.nxp.wireless_connectivity.commands.comm import Comm

from pprint import pprint
from Queue import Empty


def validate_join(dev):
    sleep(1)
    print 79 * '=' + '\nWaiting for the Joiner to promote to Router (status RouterIdAssigned)\n' + 79 * '='

    while True:
        # we keep consuming async events until the Joiner confirmation occurs
        try:
            event = FsciFramer(dev,
                               ack_policy=FsciAckPolicy.GLOBAL,
                               protocol=Protocol.Thread,
                               baudrate=Baudrate.BR115200).async_queue.get(block=True, timeout=1)
        except Empty:
            continue
        except KeyboardInterrupt:
            print 'Exiting at user request..'
            sys.exit()
        if isinstance(event, THR_EventGeneralConfirm) and event.EventStatus == 'RouterIdAssigned':
            break
    sleep(3)


def get_br_table(device, no_of_elements):
    table = THR_BrPrefixGetTable(
        device, InstanceId=0, StartIndex=0, NoOfElements=no_of_elements)
    print '\n' + 40 * '=' + ' BR TABLE ' + 40 * '='
    for entry in table.BrPrefixEntries:
        print entry.__dict__
    del table.BrPrefixEntries[:]
    print 90 * '=' + '\n'


def remove_router_id(leader_port, router_port):
    rsp = THR_GetAttr(
        router_port,
        InstanceId=0,
        AttributeId=THR_GetAttrRequestAttributeId.ShortAddress,
    )
    RouterShortAddr = rsp.AttributeValue

    THR_LeaderRemoveRouterId(leader_port, InstanceId=0, RouterShortAddr=RouterShortAddr)


def reset(*devices):

    # Subscribe to asynchronous events
    subscribe_to_async_thread_events_from(leader)
    subscribe_to_async_thread_events_from(joiner)

    for device in devices:
        confirm = THR_FactoryReset(device)
        assert confirm.Status == 'Success'


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

    if len(sys.argv) < 4:
        print 'Usage: # %s <LEADER_PORT> <JOINER_PORT> <802.15.4 CHANNEL>' % sys.argv[0]
        sys.exit(1)
    else:
        leader, joiner, channel = sys.argv[1], sys.argv[2], int(sys.argv[3])
        assert channel in range(11, 27), 'Channel must be in range 11-26.'

    reset(leader, joiner)

    # Get Channel and Extended Address attribute IDs
    ch_attr_id = THR_SetAttrRequestAttributeId.Channel
    ea_attr_id = THR_SetAttrRequestAttributeId.RandomExtendedAddr
    is_comm_id = THR_SetAttrRequestAttributeId.IsDevCommissioned

    # Set 802.15.4 channel
    confirm = THR_SetAttr(leader, AttributeId=ch_attr_id, AttrSize=1, AttributeValue=channel)
    confirm = THR_SetAttr(joiner, AttributeId=ch_attr_id, AttrSize=1, AttributeValue=channel)

    # Set extended addresses
    confirm = THR_SetAttr(leader, AttributeId=ea_attr_id, AttrSize=8, AttributeValue=0x166E0A0000000001)
    confirm = THR_SetAttr(joiner, AttributeId=ea_attr_id, AttrSize=8, AttributeValue=0x166E0A0000000002)

    # Don't use commissioning
    confirm = THR_SetAttr(leader, AttributeId=is_comm_id, AttrSize=1, AttributeValue=True)
    confirm = THR_SetAttr(joiner, AttributeId=is_comm_id, AttrSize=1, AttributeValue=True)

    # Start the leader
    confirm = THR_CreateNwk(leader, InstanceID=0)
    assert confirm.Status == 'OK'
    sleep(7)

    # Start the router
    confirm = THR_Join(joiner, InstanceID=0)
    assert confirm.Status == 'OK'
    validate_join(joiner)

    prefix1 = [0x20, 0x42] + [0] * 14  # DHCP
    prefix2 = [0x30, 0x55] + [0] * 14  # SLAAC

    PrefixFlags_DHCP = 0b00001001  # P_dhcp | P_on_mesh -> see Frames.THR_BrPrefixAddEntryRequest for more details
    PrefixFlags_Slaac = 0b00010001  # P_slaac | P_on_mesh -> see Frames.THR_BrPrefixAddEntryRequest for more details
    THR_BrPrefixAddEntry(
        leader,
        prefixLength=64,
        PrefixValue=prefix1,
        PrefixFlagsReserved=0,
        PrefixFlags=PrefixFlags_Slaac,
        prefixLifetime=0xFFFFFFFF,
        prefixAdvertised=True,
        ExternalRouteFlags=ExternalRouteFlagsR_preference.MediumDefault,
        ExternalRouteLifetime=0xFFFFFFFF,
        ExternalRouteAdvertised=True
    )
    get_br_table(leader, no_of_elements=5)

    THR_BrPrefixAddEntry(
        leader,
        prefixLength=64,
        PrefixValue=prefix2,
        PrefixFlagsReserved=0,
        PrefixFlags=PrefixFlags_DHCP,
        prefixLifetime=0xFFFFFFFF,
        prefixAdvertised=True,
        ExternalRouteFlags=ExternalRouteFlagsR_preference.MediumDefault,
        ExternalRouteLifetime=0xFFFFFFFF,
        ExternalRouteAdvertised=True
    )
    get_br_table(leader, no_of_elements=5)

    THR_BrPrefixRemoveEntry(
        leader,
        prefixLength=64,
        PrefixValue=prefix2,
    )
    get_br_table(leader, no_of_elements=5)

    THR_BrPrefixSync(leader, InstanceId=0)
    sleep(3)

    print "\n=== Leader's Addresses (expecting a 2042::/64 global) ==="
    response = NWKU_IfconfigAll(leader)
    for interface in response.InterfaceList:
        pprint(interface.__dict__)

    print "\n=== Joiner's Addresses (expecting a 2042::/64 global) ==="
    response = NWKU_IfconfigAll(joiner)
    for interface in response.InterfaceList:
        pprint(interface.__dict__)

    # get joiner Link_Local_64 address
    response = THR_GetThreadIpAddr(
        joiner,
        InstanceId=0,
        AddressType=THR_GetThreadIpAddrRequestAddressType.Link_Local_64
    )
    try:
        print 'LL64 -> %x' % list_to_int(response.AddressList, False)
    except ValueError:
        print 'LL64 -> None'

    if(response.AddressList is not None):
        # add THR_MgmtDiagnosticGetRspIndicationObserver
        # and send THR_MgmtDiagnosticGet to get SourceAddr, Ip6AddrList and LinkQuality
        comm = Comm(leader, ack_policy=FsciAckPolicy.GLOBAL,
                    protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
        comm.fsciFramer.addObserver(THR_MgmtDiagnosticGetRspIndicationObserver(
            'THR_MgmtDiagnosticGetRspIndication'), cb_diag_get_rsp)
        response = THR_MgmtDiagnosticGet(leader, 0, response.AddressList, 3, TlvIds=[
                                         THR_MgmtDiagnosticGetRequestTlvId.SourceAddr, THR_MgmtDiagnosticGetRequestTlvId.Ip6AddrList, THR_MgmtDiagnosticGetRequestTlvId.LinkQuality])

    remove_router_id(leader, joiner)
