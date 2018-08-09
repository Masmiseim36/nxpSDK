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

# Heart Rate Service - Heart Rate Format
gHrs_8BitHeartRateFormat_c = 0x00
gHrs_16BitHeartRateFormat_c = 0x01

# Heart Rate Service - Sensor Contact detected status
gHrs_SensorContactNotDetected_c = 0x00
gHrs_SensorContactDetected_c = 0x02

# Heart Rate Service - Sensor Contact supported status
gHrs_SensorContactNotSupported_c = 0x00
gHrs_SensorContactSupported_c = 0x04

# Heart Rate Service - Energy expended status
gHrs_EnergyExpendedDisabled_c = 0x00
gHrs_EnergyExpendedEnabled_c = 0x08

# Heart Rate Service - RR Intervals status
gHrs_RrIntervalsDisabled_c = 0x00
gHrs_RrIntervalsEnabled_c = 0x10

# Heart Rate Service - Body Sensor Location
gHrs_BodySensorLocOther_c = 0x00
gHrs_BodySensorLocChest_c = 0x01
gHrs_BodySensorLocWrist_c = 0x02
gHrs_BodySensorLocFinger_c = 0x03
gHrs_BodySensorLocHand_c = 0x04
gHrs_BodySensorLocEarLobe_c = 0x05
gHrs_BodySensorLocFoot_c = 0x06


gFastConnMinAdvInterval_c = 32  # 20 ms
gFastConnMaxAdvInterval_c = 48  # 30 ms

# Advertising Type
gAdvConnectableUndirected_c = 0x00  # Answers to both connect and scan requests.
gAdvDirectedHighDutyCycle_c = 0x01  # Answers only to connect requests; smaller advertising interval for quicker connection.
gAdvScannable_c = 0x02  # Answers only to scan requests.
gAdvNonConnectable_c = 0x03  # Does not answer to connect nor scan requests.
gAdvDirectedLowDutyCycle_c = 0x04   # Answers only to connect requests; larger advertising interval.

# Bluetooth Device Address Types
gBleAddrTypePublic_c = 0x00  # Public Device Address - fixed into the Controller by the manufacturer.
gBleAddrTypeRandom_c = 0x01  # Random Device Address - set by the Host into the Controller for privacy reasons.

gAdvChanMapFlag37_c = 0x01  # Bit for channel 37.
gAdvChanMapFlag38_c = 0x02  # Bit for channel 38.
gAdvChanMapFlag39_c = 0x04  # Bit for channel 39.
gGapAdvertisingChannelMapDefault_c = gAdvChanMapFlag37_c | gAdvChanMapFlag38_c | gAdvChanMapFlag39_c

# Advertising Filter Policy values
gProcessAll_c = 0x00  # Default value: accept all connect and scan requests.
gProcessConnAllScanWL_c = 0x01  # Accept all connect requests, but scan requests only from devices in White List.
gProcessScanAllConnWL_c = 0x02  # Accept all scan requests, but connect requests only from devices in White List.
gProcessWhiteListOnly_c = 0x03  # Accept connect and scan requests only from devices in White List.
