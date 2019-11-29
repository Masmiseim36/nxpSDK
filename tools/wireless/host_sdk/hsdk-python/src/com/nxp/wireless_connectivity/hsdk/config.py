'''
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

# Whether or not the Framer will be put in monitor mode.
# That means every RX event will get printed instead of observed.
MONITOR_MODE = False

# Print summary of sent/received frames. When available, use colorama
# to differentiate commands from events, by enabling COLORED_OUTPUT.
VERBOSE = True

# Print more information, e.g. the actual bytes the host sends or
# the system time of received events.
DEBUG = False

# Use FSCI_TX_ACK to validate ACKs from Python instead of C
# IMPORTANT: Before enabling, set FsciTxAck=0 in /usr/local/etc/hsdk/hsdk.conf
FSCI_TX_ACK = False

# The speed (Hz) used for SPI communication.
MAX_SPEED_HZ = 1000000  # 1Mhz

# Distinguish commands by events through colors in console
# Uses Python module colorama
COLORED_OUTPUT = True

# Use Python's logging module.
USE_LOGGER = False

# Whether or not parameter endianness is relevant only for integers in BLE tests.
# List-type parameters will not account for endianness in both TX and RX packets.
# Status parameters in events will be integers.
BLE_PTS_TEST = False

# Whether the current package is being bundled in the Thread Harness
THREAD_HARNESS = False
