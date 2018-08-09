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

import Queue
import abc
import time

from com.nxp.wireless_connectivity.commands.comm import Comm
from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciAckPolicy, Protocol
from com.nxp.wireless_connectivity.hsdk import config
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.utils import print_command


class FsciOperation(object):

    '''
    A FsciOperation encapsulates both the sending of a request and the receiving of triggered events.
    '''

    __metaclass__ = abc.ABCMeta

    def __init__(self, deviceName, request=None, callbacks=[], ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.Thread, baudrate=Baudrate.BR115200, sync_request=False):
        '''
        Constructor for a FsciOperation.

        @param deviceName: The OS name for a NXP Kinetis-W device. e.g. /dev/ttyACMx on Linux
        @param request: The request object to be sent to the board.
        @param callbacks: The callbacks that are to be executed on the received events.
        @param ack_policy: The policy for FSCI ACK synchronization.
        @param protocol: The protocol used in the operation. Defaults to Thread.
        @param baudrate: The baudrate used to open the UART device. Defaults to 115200.
        @param sync_request: Whether or not this operation should return the event on the begin() method.
        '''
        self.comm = Comm(deviceName, ack_policy=ack_policy, protocol=protocol, baudrate=baudrate)
        self.request = request
        self.callbacks = callbacks
        self.protocol = protocol
        self.sync_request = sync_request

    def sendRequest(self):
        '''
        Sends the request downwards to the Communication module to be transmitted on wire.
        '''
        virtualInterface = 1 if self.protocol == Protocol.Hybrid else 0
        self.comm.send(self.spec, self.request, virtualInterface)

    @abc.abstractmethod
    def subscribeToEvents(self):
        '''
        Abstract method that will be implemented by children classes. It subscribes to specific events
        with specific event observers.
        Reason for abstract method: self.observers will be set from the child class.
        '''
        if self.callbacks == []:
            for observer in self.observers:
                self.comm.fsciFramer.addObserver(observer, sync_request=self.sync_request)
        else:
            if len(self.callbacks) != len(self.observers):
                print 'Please add one callback for every observer, even if it\'s None.'
            else:
                for observer, callback in zip(self.observers, self.callbacks):
                    self.comm.fsciFramer.addObserver(observer, callback)

    def unsubscribeFromEvents(self):
        '''
        Removes the added observers.
        '''
        self.comm.fsciFramer.removeObservers(self.observers)

    def begin(self, timeout=0.2):
        '''
        Method called when we want to start an operation.

        @param timeout: seconds to pass before unsubscribing from events
        '''
        # Do not allow other requests until this operation completes
        with self.comm.lock:

            self.subscribeToEvents()
            self.sendRequest()

            if self.sync_request:
                event = None

                # Some sync requests do not have observers. e.g. CPU Reset.
                if self.observers != []:
                    try:
                        while event.__class__.__name__ not in [obs.name for obs in self.observers]:
                            start = time.clock()
                            event = self.comm.fsciFramer.event_queue.get(block=True, timeout=timeout)
                            self.comm.fsciFramer.event_queue.task_done()
                            end = time.clock()
                            timeout -= (end - start)

                    except (Queue.Empty, ValueError):  # ValueError if negative timeout
                        print 'No response for the previous command', self.request.__class__.__name__
                        event = None
                self.unsubscribeFromEvents()

                return event

            else:
                time.sleep(timeout)
                self.unsubscribeFromEvents()

    def end(self):
        '''
        Method called when we want to end an operation.
        '''
        self.unsubscribeFromEvents()
