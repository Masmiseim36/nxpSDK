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

import commands

from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.commands.thread.sync_requests import *
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.framing.fsci_framer import FsciFramer

from Queue import Empty
from binascii import hexlify
from ipaddress import ip_address


def create_nwk(br, joiner, channel=None):
    subscribe_to_async_thread_events_from(br)
    subscribe_to_async_thread_events_from(joiner)

    THR_FactoryReset(br)
    THR_FactoryReset(joiner)

    if channel is not None:
        ch_attr_id = THR_SetAttrRequestAttributeId.Channel
        confirm = THR_SetAttr(br, AttributeId=ch_attr_id, AttrSize=1, AttributeValue=channel)
        confirm = THR_SetAttr(joiner, AttributeId=ch_attr_id, AttrSize=1, AttributeValue=channel)

    confirm = THR_CreateNwk(br)
    assert confirm.Status == 'OK'
    sleep(7)

    confirm = MESHCOP_StartCommissioner(br)
    assert confirm.Status == 'Success'

    confirm = MESHCOP_AddExpectedJoiner(
        br,
        Selected=True,
        EuiType=MESHCOP_AddExpectedJoinerRequestEuiType.LongEUI,
        EUI=0xFFFFFFFFFFFFFFFF,
        PSKdSize=len('THREAD'),
        PSKd='THREAD'
    )
    assert confirm.Status == 'Success'

    confirm = MESHCOP_SyncSteeringData(br, EuiMask=MESHCOP_SyncSteeringDataRequestEuiMask.AllFFs)
    assert confirm.Status == 'Success'

    confirm = THR_Join(joiner)
    assert confirm.Status == 'OK'
    validate_join(joiner)


def validate_join(dev):
    print 79 * '=' + '\nWaiting for the Joiner to promote to Router (status RouterIdAssigned)\n' + 79 * '='

    while True:
        # we keep consuming async events until the Joiner confirmation occurs
        try:
            event = FsciFramer(dev, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200).async_queue.get(
                block=True, timeout=1)
        except Empty:
            continue
        except KeyboardInterrupt:
            print 'Exiting at user request..'
            sys.exit()
        if isinstance(event, THR_EventGeneralConfirm) and (event.EventStatus == 'RouterIdAssigned' or event.EventStatus == 'DeviceIsRouter'):
            break
    sleep(3)


def get_joiner_short_addr(dev):
    rsp = THR_GetNeighborTable(dev, NoOfElements=1)
    return rsp.NeighborEntries[0].ShortAddress


def get_mesh_prefix(dev):
    rsp = THR_GetThreadIpAddr(dev, AddressType=THR_GetThreadIpAddrRequestAddressType.RLOC)
    return rsp.AddressList[:8]


def get_br_linklocal(dev):
    rsp = NWKU_IfconfigAll(br)
    print 'BR LL64 ->', rsp.InterfaceList[0].Addresses[0]

    return rsp.InterfaceList[0].Addresses[0]


def get_joiner_rloc(dev):
    short = get_joiner_short_addr(dev)
    prefix = get_mesh_prefix(dev)

    rloc_list = prefix + [0x00, 0x00, 0x00, 0xff, 0xfe, 0x00] + [short >> 8, short & 0xff]
    rloc_hex = ['%02x' % b for b in rloc_list]
    rloc_str = ':'.join(rloc_hex[i] + rloc_hex[i + 1] for i in range(0, 16, 2))

    print 'Joiner RLOC (ML16) ->', rloc_str
    return rloc_list


def get_joiner_rloc_str(dev):
    short = get_joiner_short_addr(dev)
    prefix = get_mesh_prefix(dev)

    rloc_list = prefix + [0x00, 0x00, 0x00, 0xff, 0xfe, 0x00] + [short >> 8, short & 0xff]
    rloc_hex = ['%02x' % b for b in rloc_list]
    rloc_str = ':'.join(rloc_hex[i] + rloc_hex[i + 1] for i in range(0, 16, 2))

    return rloc_str


def cb_diag_get_rsp(devName, rsp):
    try:
        ula_list = rsp.TLVs[0].raw_value[48:64]
        ula_hex = ['%02x' % b for b in ula_list]
        ula_str = ':'.join(ula_hex[i] + ula_hex[i + 1] for i in range(0, 16, 2))
    except (IndexError, ValueError):
        return None

    print '\n\nJoiner\'s ULA (retrieved over THCI) ->', ula_str, '\n\n'


def get_joiner_ula(dev):

    # we first find the joiner RLOC, from ML Prefix + Fixed Part + Short Address
    joiner_rloc = get_joiner_rloc(br)

    # add a callback on diagnostic indication
    comm = Comm(dev, ack_policy=FsciAckPolicy.GLOBAL,
                protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm.fsciFramer.addObserver(THR_MgmtDiagnosticGetRspIndicationObserver(
        'THR_MgmtDiagnosticGetRspIndication'), cb_diag_get_rsp)

    # and use it to get all addresses of the Joiner
    cnf = THR_MgmtDiagnosticGet(
        dev,
        DestIpAddr=joiner_rloc,
        NumberOfTlvIds=1,
        TlvIds=[THR_MgmtDiagnosticGetRequestTlvId.Ip6AddrList]
    )

    assert cnf.Status == 'Success'


##############################################################################
# CoAP operations over RNDIS, using txThings
##############################################################################

class Agent():

    def __init__(self, protocol, dst):
        self.protocol = protocol
        self.dst = dst
        reactor.callLater(1, self.requestResource)

    def requestResource(self):

        # IPv6 address list Diagnostic TLV as defined by the Thread spec, chapter 10.8
        payload = '\x12\x01\x08'
        request = coap.Message(code=coap.GET, payload=payload)
        request.opt.uri_path = ('d', 'dg', )
        request.remote = (ip_address(self.dst), 61631)
        request.opt.content_format = coap.media_types_rev['application/octet-stream']

        d = protocol.request(request)
        d.addCallback(self.printResponse)
        d.addErrback(self.noResponse)

    def printResponse(self, response):
        assert ord(response.payload[0]) == 0x08
        curr = 2

        print 'Joiner Addresses:',
        while curr < ord(response.payload[1]):
            print hexlify(response.payload[curr:curr + 16]),
            curr += 16

        ula_str = hexlify(response.payload[2 + 48:2 + 64])
        ula_str = ':'.join(ula_str[4 * i:4 * (i + 1)] for i in range(8))
        print '\n\nJoiner ULA (retrieved over CoAP) ->', ula_str, '\n\n'
        reactor.stop()

    def noResponse(self, failure):
        print 'Failed to fetch resource:'
        print failure
        reactor.stop()


if __name__ == '__main__':

    if len(sys.argv) < 3:
        print 'Usage: %s <br_serial_port> <joiner_serial_port>' % sys.argv[0]
        sys.exit(1)

    br, joiner = sys.argv[1], sys.argv[2]
    ans = raw_input('Please confirm that you have two THCI enabled boards connected on %s (BR) and %s (Joiner). [y/N] -> ' % (sys.argv[1], sys.argv[2]))
    if ans != 'y' and ans != 'Y':
        sys.exit(0)

    ans = raw_input('Create Thread network (select N if already created)? [y/N] -> ')
    if ans == 'y' or ans == 'Y':
        create_nwk(br, joiner)

    # get Joiner ULA over THCI, using only the serial port of the BR
    try:
        ula = get_joiner_ula(br)
    except IndexError:
        sys.exit('An issue occurred, please try again..')

    try:
        import twisted
        from twisted.internet import reactor
        from twisted.python import log
        import txthings.coap as coap
        import txthings.resource as resource
    except ImportError:
        sys.exit('In order to retrieve Joiner\'s ULA over CoaP, please install txThings: # pip install txThings --upgrade')

    if twisted.__version__.startswith('17'):
        sys.exit("Please uninstall twisted %s and install version 16.6.0: # pip install twisted==16.6.0" % twisted.__version__)

    ans = raw_input('Please confirm that the BR board connected on %s also acts as a router on RNDIS/Ethernet for the PC. [y/N] -> ' % sys.argv[1])
    if ans != 'y' and ans != 'Y':
        sys.exit(0)
    if not sys.platform.startswith('linux'):
        sys.exit('The CoAP part is only available on Linux for now. Exiting..')

    # get Joiner ULA over CoAP, yet we still need Joiner's RLOC first
    # if the BR does not have THCI, the RLOC can be analogously obtained from the shell interface
    # Joiner_RLOC = ML Prefix + Fixed Part + Neighbor Short Address
    intf = None
    routes = commands.getoutput('ip -6 route show')
    for line in routes.split('\n'):
        if line.startswith('fd01:0:0:') and '3ead' not in line:
            intf = line.split(' ')[2]
            break
    if intf is None:
        sys.exit('Could not retrieve the RNDIS/Ethernet interface name. Exiting..')

    log.startLogging(sys.stdout)

    endpoint = resource.Endpoint(None)
    protocol = coap.Coap(endpoint)

    joiner_rloc = get_joiner_rloc_str(br)
    br_ll64 = get_br_linklocal(br)
    print commands.getoutput('ip -6 route add ' + joiner_rloc + ' dev ' + intf + ' via ' + br_ll64)
    client = Agent(protocol, joiner_rloc)

    reactor.listenUDP(61631, protocol, interface='::0')
    reactor.run()

    print commands.getoutput('ip -6 route del ' + joiner_rloc + ' dev ' + intf + ' via ' + br_ll64)
