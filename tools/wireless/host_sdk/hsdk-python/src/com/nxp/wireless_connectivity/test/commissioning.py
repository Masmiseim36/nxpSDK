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


LEADER_EXT_ADDR = 0x146E0A0000000001
JOINER_EXT_ADDR = 0x146E0A0000000002
JOINER_PSKD = 'THREAD'


def factory_reset(device):
    confirm = THR_FactoryReset(device)
    if confirm.Status != 'Success':
        print '[FAIL] Factory Reset'
        return False
    return True


def set_channel(device, channel):
    confirm = THR_SetAttr(
        device,
        AttributeId=THR_SetAttrRequestAttributeId.Channel,
        AttrSize=1,
        AttributeValue=int(channel)
    )
    if confirm.Status != 'Success':
        print '[FAIL] THR_SetAttr Channel'
        return False
    return True


def set_extended_addr(device, ext_addr):
    confirm = THR_SetAttr(
        device,
        AttributeId=THR_SetAttrRequestAttributeId.IeeeExtendedAddr,
        AttrSize=8,
        AttributeValue=ext_addr
    )
    if confirm.Status != 'Success':
        print '[FAIL] THR_SetAttr RandomExtendedAddr'
        return False
    return True


def configure(device, ext_addr, channel):
    subscribe_to_async_thread_events_from(device)
    assert factory_reset(device), 'Factory Reset failed'
    assert set_extended_addr(device, ext_addr), 'Set Extended Address failed'
    assert set_channel(device, channel), 'Set Channel failed'


def start_commissioner(device):
    confirm = THR_CreateNwk(device, InstanceID=0)
    if confirm.Status != 'OK':
        return False
    # expect async events: watch console
    sleep(7)

    confirm = MESHCOP_StartCommissioner(device, InstanceId=0)
    if confirm.Status != 'Success':
        return False

    confirm = MESHCOP_RemoveAllExpectedJoiners(device, InstanceId=0)
    if confirm.Status != 'Success':
        return False

    return True


def start_joiner(device, PSKd):
    confirm = THR_SetAttr(
        device,
        AttributeId=THR_SetAttrRequestAttributeId.Security_PSKd,
        AttrSize=len(PSKd),
        AttributeValue=PSKd
    )
    if confirm.Status != 'Success':
        return False

    confirm = THR_Join(device, InstanceID=0)
    if confirm.Status != 'OK':
        return False

    # expect async events: watch console
    sleep(20)

    return True


def allow_joiner(device, joiner_ext_addr, joiner_pskd):
    confirm = MESHCOP_AddExpectedJoiner(
        device,
        Selected=True,
        EuiType=MESHCOP_AddExpectedJoinerRequestEuiType.LongEUI,
        EUI=joiner_ext_addr,
        PSKdSize=len(joiner_pskd),
        PSKd=joiner_pskd
    )
    if confirm.Status != 'Success':
        return False

    if joiner_ext_addr == 0xFFFFFFFFFFFFFFFF:
        EuiMask = MESHCOP_SyncSteeringDataRequestEuiMask.AllFFs
    else:
        EuiMask = MESHCOP_SyncSteeringDataRequestEuiMask.ExpectedJoiners

    confirm = MESHCOP_SyncSteeringData(
        device,
        InstanceId=0,
        EuiMask=EuiMask
    )
    if confirm.Status != 'Success':
        return False

    return True


def stop_commissioner(device):
    confirm = MESHCOP_StopCommissioner(device, InstanceId=0)
    if confirm.Status != 'Success':
        return False
    return True

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: # %s LEADER_TTY JOINER_TTY' % sys.argv[0]
        sys.exit(1)

    leader, joiner_router = sys.argv[1:3]

    configure(leader, LEADER_EXT_ADDR, 12)
    configure(joiner_router, JOINER_EXT_ADDR, 12)

    assert start_commissioner(leader), 'Start Commissioner failed'
    assert allow_joiner(leader, JOINER_EXT_ADDR, JOINER_PSKD), 'Allow Joiner into Network failed'
    assert start_joiner(joiner_router, JOINER_PSKD), 'Start Joiner failed'
    assert stop_commissioner(leader), 'Stop Commissioner failed'
