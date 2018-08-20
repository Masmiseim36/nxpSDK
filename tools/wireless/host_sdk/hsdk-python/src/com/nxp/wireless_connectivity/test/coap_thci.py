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

from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciAckPolicy,\
    Protocol
from com.nxp.wireless_connectivity.commands.thread.enums import NWKU_CoapRegisterRequestSocketDomain,\
    THR_GetThreadIpAddrRequestAddressType, NWKU_CoapSendRequestRequestType,\
    NWKU_CoapSendRequestMessageType, NWKU_CoapCreateInstanceRequestSocketDomain
from com.nxp.wireless_connectivity.commands.thread.events import NWKU_CoapMsgReceivedIndicationObserver
from com.nxp.wireless_connectivity.commands.thread.sync_requests import NWKU_CoapRegister,\
    THR_GetThreadIpAddr, NWKU_CoapSend, NWKU_CoapCreateInstance
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.utils import print_event
import sys
from threading import Event
from time import sleep

from com.nxp.wireless_connectivity.test.get_neigh_ula import create_nwk


COAP_PORT = 5683
LEADER_RCV_EVENT = Event()
JOINER_RLOC, PAYLOAD = None, None


def cb_leader_coap_msg_recv(_, ind):
    global JOINER_RLOC, PAYLOAD
    print_event(leader, ind)

    JOINER_RLOC = ind.RemoteIpAddress
    PAYLOAD = ind.Payload
    print '\n[Leader] I have received the /calc request. Sending back the answer.\n'
    LEADER_RCV_EVENT.set()
    comm_leader.fsciFramer.removeAllObservers()


def cb_joiner_coap_msg_recv(_, ind):
    print_event(joiner, ind)
    print '\n[Joiner] Received answer', PAYLOAD, '=', ind.Payload, '\n'


if __name__ == '__main__':

    if len(sys.argv) < 4:
        print 'Usage: %s <leader_serial_port> <joiner_serial_port> <802.15.4 channel>' % sys.argv[0]
        sys.exit(1)

    assert int(sys.argv[3]) in range(11, 27), 'Channel must be in range 11-26.'
    leader, joiner = sys.argv[1], sys.argv[2]
    ans = raw_input(
        'Please confirm that you have two THCI enabled boards connected on %s (Leader) and %s (Joiner). [y/N] -> ' % (sys.argv[1], sys.argv[2]))
    if ans != 'y' and ans != 'Y':
        sys.exit(0)

    create_nwk(leader, joiner, int(sys.argv[3]))

    # add callbacks on CoAP message received indications
    comm_leader = Comm(leader, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm_joiner = Comm(joiner, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200)
    comm_leader.fsciFramer.addObserver(
        observer=NWKU_CoapMsgReceivedIndicationObserver('NWKU_CoapMsgReceivedIndication'),
        callback=cb_leader_coap_msg_recv
    )
    comm_joiner.fsciFramer.addObserver(
        observer=NWKU_CoapMsgReceivedIndicationObserver('NWKU_CoapMsgReceivedIndication'),
        callback=cb_joiner_coap_msg_recv
    )

    # Create CoAP instances
    confirm = NWKU_CoapCreateInstance(
        leader,
        UDPPort=COAP_PORT,
        SocketDomain=NWKU_CoapCreateInstanceRequestSocketDomain.AF_INET6
    )
    assert confirm.Status == 'Success'

    confirm = NWKU_CoapCreateInstance(
        joiner,
        UDPPort=COAP_PORT,
        SocketDomain=NWKU_CoapCreateInstanceRequestSocketDomain.AF_INET6
    )
    assert confirm.Status == 'Success'

    # Register /calc URI path
    confirm = NWKU_CoapRegister(
        leader,
        SocketDomain=NWKU_CoapRegisterRequestSocketDomain.AF_INET6,
        URIpath='/calc',  # MUST start with /
        Port=COAP_PORT
    )
    assert confirm.Status == 'Success'

    confirm = NWKU_CoapRegister(
        joiner,
        SocketDomain=NWKU_CoapRegisterRequestSocketDomain.AF_INET6,
        URIpath='/calc',  # MUST start with /
        Port=COAP_PORT
    )
    assert confirm.Status == 'Success'

    # Obtain Leader's RLOC
    leader_rloc = THR_GetThreadIpAddr(leader, AddressType=THR_GetThreadIpAddrRequestAddressType.RLOC).AddressList

    # Joiner: My Leader, please compute 42 * (42 ^ 42)!
    print '\n[Joiner] Leader, please compute 42 * (42 ^ 42)\n'
    confirm = NWKU_CoapSend(
        joiner,
        DestinationIpAddress=leader_rloc,
        RequestType=NWKU_CoapSendRequestRequestType.NON,
        MessageType=NWKU_CoapSendRequestMessageType.POST,
        URIpath='/calc',
        PayloadLength=len('42*42**42'),
        Payload='42*42**42',
    )
    assert confirm.Status == 'Success'

    LEADER_RCV_EVENT.wait()
    # Leader: Hey Joiner, 42 * (42 ^ 42) is not that hard when you know Python!
    confirm = NWKU_CoapSend(
        leader,
        DestinationIpAddress=JOINER_RLOC,
        RequestType=NWKU_CoapSendRequestRequestType.CON,
        MessageType=NWKU_CoapSendRequestMessageType.POST,
        URIpath='/calc',
        PayloadLength=len(str(eval(PAYLOAD))),
        Payload=str(eval(PAYLOAD))
    )
    assert confirm.Status == 'Success'
    sleep(1)  # allow for callback cb_joiner_coap_msg_recv to get executed
