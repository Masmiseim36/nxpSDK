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

from com.nxp.wireless_connectivity.commands.thread.sync_requests import MAC_MacFilteringEnable,\
    MAC_MacFilteringAddEntry, MAC_MacFilteringGetTable,\
    MAC_MacFilteringRemoveEntry, THR_FactoryReset
from com.nxp.wireless_connectivity.commands.thread.enums import MAC_MacFilteringEnableRequestMacFiltering
from com.nxp.wireless_connectivity.hsdk.utils import list_to_int


leader = '/dev/ttyACM0'

filter_addr = 0x1122334455667788

if __name__ == '__main__':

    THR_FactoryReset(leader)

    confirm = MAC_MacFilteringEnable(
        leader,
        InstanceId=0,
        MacFiltering=MAC_MacFilteringEnableRequestMacFiltering.EnableDefaultPolicyReject)
    assert confirm.Status == 'Success'

    confirm = MAC_MacFilteringAddEntry(
        leader,
        InstanceId=0,
        ExtendedAddress=filter_addr,
        ShortAddress=0xAABB,
        LinkIndicator=42,
        BlockNeighbor=True
    )
    assert confirm.Status == 'Success'

    table = MAC_MacFilteringGetTable(
        leader,
        InstanceId=0,
        StartIndex=0,
        NoOfElements=1
    )
    for entry in table.MacFilteringEntries:
        print entry.__dict__
        assert entry.ExtendedAddress == filter_addr

    confirm = MAC_MacFilteringRemoveEntry(
        leader,
        InstanceId=0,
        ExtendedAddress=filter_addr
    )
    assert confirm.Status == 'Success'

    table = MAC_MacFilteringGetTable(
        leader,
        InstanceId=0,
        StartIndex=0,
        NoOfElements=42
    )
    assert table.NoOfElements == 0
