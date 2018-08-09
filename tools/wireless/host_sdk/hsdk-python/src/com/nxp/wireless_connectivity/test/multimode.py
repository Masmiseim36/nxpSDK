#!/usr/bin/env python
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

from Queue import Empty
from threading import Thread, Event

from com.nxp.wireless_connectivity.commands.thread.sync_requests import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.framing.fsci_framer import FsciFramer
from com.nxp.wireless_connectivity.test.hrs import main as hrs_main


network_created = Event()


def create_nwk(leader, joiner, channel):
    subscribe_to_async_thread_events_from(leader)
    subscribe_to_async_thread_events_from(joiner)

    THR_FactoryReset(leader)
    THR_FactoryReset(joiner)

    # Set 802.15.4 channel
    confirm = THR_SetAttr(hybrid_dev, AttributeId=THR_SetAttrRequestAttributeId.Channel, AttrSize=1, AttributeValue=channel)
    assert confirm.Status == 'Success'
    confirm = THR_SetAttr(thread_dev, AttributeId=THR_SetAttrRequestAttributeId.Channel, AttrSize=1, AttributeValue=channel)
    assert confirm.Status == 'Success'

    confirm = THR_CreateNwk(leader)
    assert confirm.Status == 'OK'
    sleep(10)

    confirm = MESHCOP_StartCommissioner(leader)
    assert confirm.Status == 'Success'

    confirm = MESHCOP_AddExpectedJoiner(
        leader,
        Selected=True,
        EuiType=MESHCOP_AddExpectedJoinerRequestEuiType.LongEUI,
        EUI=0xFFFFFFFFFFFFFFFF,
        PSKdSize=len('THREAD'),
        PSKd='THREAD'
    )
    assert confirm.Status == 'Success'

    confirm = MESHCOP_SyncSteeringData(leader, EuiMask=MESHCOP_SyncSteeringDataRequestEuiMask.AllFFs)
    assert confirm.Status == 'Success'

    confirm = THR_Join(joiner)
    assert confirm.Status == 'OK'
    validate_join(joiner)

    leader_ll64 = THR_GetThreadIpAddr(leader, AddressType=THR_GetThreadIpAddrRequestAddressType.Link_Local_64, NoOfElements=1).AddressList
    joiner_ll64 = THR_GetThreadIpAddr(joiner, AddressType=THR_GetThreadIpAddrRequestAddressType.Link_Local_64, NoOfElements=1).AddressList

    network_created.set()
    # now we start the BLE stack
    sleep(10)

    print 'ICMP-ECHO.request', leader_ll64, '->', joiner_ll64
    continuous_ping(leader, leader_ll64, joiner_ll64)


def validate_join(joiner):

    while True:
        # we keep consuming async events until the Joiner confirmation occurs
        try:
            event = FsciFramer(joiner, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200).async_queue.get(
                block=True, timeout=1)
        except Empty:
            continue
        except KeyboardInterrupt:
            print 'Exiting at user request..'
            sys.exit()
        if isinstance(event, THR_EventGeneralConfirm) and event.EventStatus == 'Connected':
            break

    sleep(1)
    print 79 * '=' + '\nWaiting for the Joiner to promote to Router (status DeviceIsRouter)\n' + 79 * '='

    while True:
        # we keep consuming async events until the Joiner confirmation occurs
        try:
            event = FsciFramer(joiner, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200).async_queue.get(
                block=True, timeout=1)
        except Empty:
            continue
        except KeyboardInterrupt:
            print 'Exiting at user request..'
            sys.exit()
        if isinstance(event, THR_EventGeneralConfirm) and event.EventStatus == 'DeviceIsRouter':
            break

    sleep(1)


def continuous_ping(initiator_dev, src, dst):
    while 1:
        NWKU_Ping(
            initiator_dev,
            DestinationIpAddress=dst,
            SourceIpAddress=src,
            Payloadlength=16,
            Timeout=1000,  # ms
            Secured=True
        )
        sleep(1)


if __name__ == '__main__':

    if len(sys.argv) < 4:
        sys.exit('Usage: # %s <port ble_thread_host_controlled_device> '
                 '<port host_controlled_device> <802.15.4 channel [11-26]>' % sys.argv[0])

    hybrid_dev = sys.argv[1]
    thread_dev = sys.argv[2]
    channel = int(sys.argv[3])
    assert channel in range(11, 27), 'Channel must be in range 11-26.'

    # create Thread network w/ continuous ping6
    Thread_thread = Thread(target=create_nwk, args=(hybrid_dev, thread_dev, channel, ))
    # enable a Heart Rate Sensor
    BLE_thread = Thread(target=hrs_main, args=(type('mock', (object,), {'dev': hybrid_dev, 'pair': False, 'protocol': Protocol.Hybrid, 'reset': False}), ))

    Thread_thread.daemon, BLE_thread.daemon = True, True

    Thread_thread.start()
    network_created.wait()
    # start BLE after Thread network has been created
    BLE_thread.start()

    # serve forever on threads
    while 1:
        try:
            sleep(1)
        except KeyboardInterrupt:
            sys.exit('Exiting..')
