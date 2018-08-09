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

from ctypes import byref, c_uint32, POINTER

import com.nxp.wireless_connectivity.hsdk.singleton as Singleton

from com.nxp.wireless_connectivity.commands.fsci_frame_description import Protocol
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Availability, DeviceState
from com.nxp.wireless_connectivity.hsdk.device.physical_device import PhysicalDevice
from com.nxp.wireless_connectivity.hsdk.library_loader import LibraryLoader


@Singleton.singleton
class DeviceManager(object):

    '''
    Class that handles device discovery and creation of objects that map on devices.
    '''

    def __init__(self):
        '''
        Constructor method for DeviceManager that also loads the UART library.
        '''
        self.devices = []
        self.loader = LibraryLoader()
        self.CUartLibrary = self.loader.CUartLibrary
        self.CUartLibrary.InitializeDeviceManager()
        self.initDeviceList()

    def initDeviceList(self, detect_sniffers=False):
        '''
        Handles the initialization of the devices. Takes care of device detection based on
        the GetAllDevices function from the C library.
        '''
        self.devices = [PhysicalDevice(ds) for ds in PhysicalDevice.OPENED_DEVICES]

        ''' First retrieve the number of devices and store in pcount. '''
        GetAllDevices = self.CUartLibrary.GetAllDevices  # function pointer
        pcount = c_uint32()
        GetAllDevices(byref(pcount))
        ''' Now use pcount to retrieve all elements. '''
        GetAllDevices.restype = POINTER(DeviceState * pcount.value)
        dsp = GetAllDevices(byref(pcount))  # DeviceState*

        for i in range(pcount.value):
            if dsp.contents[i].isKinetisWDevice and (dsp.contents[i].availability != Availability.DeviceError):
                self.devices.append(PhysicalDevice(dsp.contents[i]))

        if detect_sniffers:

            from com.nxp.wireless_connectivity.commands.thread.sync_requests import Sniffer_Detect

            for i in range(len(self.devices)):

                # send a command to which only sniffers respond
                confirm = Sniffer_Detect(self.devices[i].name, timeout=1)

                # sniffer
                if confirm is not None:
                    self.devices[i].is_sniffer = True

                # not sniffer
                else:
                    # Sniffer_Detect opens the device, so we'll close it here
                    self.devices[i].close()
                    # and remove any singleton reference
                    for k in Singleton.known_instances.keys():
                        if self.devices[i].name in str(k):
                            obj = Singleton.known_instances.pop(k)
                            if 'FsciFramer' in obj.__class__.__name__:
                                obj.destroy()

    def getDevices(self):
        '''
        Getter for all present devices.
        '''
        return self.devices

    def getDevice(self, deviceName):
        '''
        Getter of a specific device based on its name.

        @param deviceName: the device OS identifier. e.g. /dev/ttyACMx on Linux.
        '''
        for device in self.devices:
            if device.name == deviceName:
                return device
        return None
