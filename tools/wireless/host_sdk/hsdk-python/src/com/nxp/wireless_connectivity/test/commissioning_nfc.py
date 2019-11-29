#!/usr/bin/env python
'''
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from com.nxp.wireless_connectivity.commands.thread.sync_requests import *  # @UnusedWildImport
import subprocess


# values for color printing
HEADER = '\033[95m'
OKBLUE = '\033[94m'
OKGREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
ENDC = '\033[0m'
BOLD = '\033[1m'
UNDERLINE = '\033[4m'


def extractEUIandPSKd():
    p = subprocess.Popen(['/usr/bin/stdbuf', '-o0', '/usr/sbin/nfcDemoApp', 'poll'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    with p.stdout:
        for line in iter(p.stdout.readline, b''):
            print line,
            stripped_line = line.replace(' ', '').replace('\n', '').replace('\t', '').replace("'", '')
            if stripped_line.startswith('Text'):
                PSKd = stripped_line.split('&cc=')[1].split('&')[0]
                EUI = stripped_line.split('eui=')[1]
                p.stdin.write('\n')
    p.wait()  # wait for the subprocess to exit

    print OKGREEN + 'PSKd:', PSKd + ENDC
    print OKGREEN + 'EUI:', EUI + ENDC

    return int(EUI, 16), PSKd


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


def set_eui_addr(device, eui):
    confirm = THR_SetAttr(
        device,
        AttributeId=THR_SetAttrRequestAttributeId.IeeeExtendedAddr,
        AttrSize=8,
        AttributeValue=eui
    )
    if confirm.Status != 'Success':
        print '[FAIL] THR_SetAttr RandomExtendedAddr'
        return False
    return True


def setProvURL(device, provUrl):
    confirm = THR_SetAttr(
        device,
        InstanceId=0,
        AttributeId=THR_SetAttrRequestAttributeId.ProvisioningURL,
        AttrSize=len(provUrl),
        AttributeValue=provUrl
    )
    if confirm.Status != 'Success':
        return False
    return True


def configure(device, channel, eui):
    subscribe_to_async_thread_events_from(device)
    assert factory_reset(device), 'Factory Reset failed'
    assert set_channel(device, channel), 'Set Channel failed'
    assert set_eui_addr(device, eui), 'Set Extended Address failed'
    assert setProvURL(device, 'nxp.com/thread'), 'Set Provisioning URL failed'


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


def usage():
    import argparse

    ans = raw_input('Please confirm that you are running this script on the Jabil i.MX6UL gateway running FSL Community BSP powered Yocto. [y/N] -> ')
    if ans != 'y' and ans != 'Y':
        sys.exit(0)

    parser = argparse.ArgumentParser(
        description='Starts a Thread Border Router with Serial TUN capabilities, capable of allowing external commissioners.'
    )
    parser.add_argument(
        'dev',
        metavar='serial_port',
        type=str,
        help='Kinetis-W system device node.',
        nargs='?',
        default='/dev/ttymxc0')
    parser.add_argument(
        '-c', '--channel',
        metavar='802.15.4 RF channel',
        type=int,
        choices=range(11, 27),
        help='RF channel for the Thread network',
        default=11
    )
    parser.add_argument(
        '-e', '--eui',
        metavar='EUI-64',
        type=int,
        help='The EUI-64 address for the Leader.',
        default=0x146E0A000000000B
    )

    args = parser.parse_args()
    print args

    return args


if __name__ == '__main__':

    args = usage()

    configure(args.dev, args.channel, args.eui)

    assert start_commissioner(args.dev), 'Start Commissioner failed'

    while 1:
        EUI, PSKd = extractEUIandPSKd()

        assert allow_joiner(args.dev, EUI, PSKd), 'Allow Joiner into Network failed'
        print OKBLUE + '\nExpecting Joiner with EUI', EUI, 'and PSKd', PSKd + ENDC + '\n'
