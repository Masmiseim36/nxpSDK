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

from com.nxp.wireless_connectivity.commands.thread.sync_requests import *
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.framing.fsci_framer import FsciFramer

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

    remove_router_id(leader, joiner)
