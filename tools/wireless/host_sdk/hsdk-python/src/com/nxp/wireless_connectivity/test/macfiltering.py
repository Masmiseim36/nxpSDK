#!/usr/bin/env python
'''
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from com.nxp.wireless_connectivity.commands.thread.sync_requests import MAC_MacFilteringEnable,\
    MAC_MacFilteringAddEntry, MAC_MacFilteringGetTable,\
    MAC_MacFilteringRemoveEntry, THR_FactoryReset
from com.nxp.wireless_connectivity.commands.thread.enums import MAC_MacFilteringEnableRequestMacFiltering


leader = 'COM17'

filter_addr = 0x1122334455667788

if __name__ == '__main__':

    THR_FactoryReset(leader)

    confirm = MAC_MacFilteringEnable(
        leader,
        InstanceId=0,
        MacFiltering=MAC_MacFilteringEnableRequestMacFiltering.EnableReject)
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
