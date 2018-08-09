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

mGattDbDynamic_GattServiceChangedInitValue = [0x00, 0x00, 0x00, 0x00]

mGattDbDynamic_GapServiceDeviceNameInitValue = "Kinetis BLE\0"
mGattDbDynamic_GapServiceAppearanceInitValue = [0x00, 0x00]
mGattDbDynamic_GapServicePpcpInitValue = [0x0A, 0x00, 0x10, 0x00, 0x64, 0x00, 0xE2, 0x04]

mGattDbDynamic_HRServiceHrMeasurementInitValue = [0x00, 0xB4]
mGattDbDynamic_HRServiceBodySensorLocationInitValue = [0x01]
mGattDbDynamic_HRServiceHrControlPointInitValue = [0x00]

mGattDbDynamic_BServiceLevelInitValue = [0x5A]
mGattDbDynamic_BServiceCharPresFormatDescriptorInitValue = [
    0x04, 0x00, 0xAD, 0x27, 0x01, 0x00, 0x00]

mGattDbDynamic_DIServiceManufNameInitValue = "NXP\0"
mGattDbDynamic_DIServiceModelNbInitValue = "Kinetis BLE\0"
mGattDbDynamic_DIServiceSerialNoInitValue = "BLESN01\0"
mGattDbDynamic_DIServiceHwRevInitValue = "USB-KW41Z-K22F\0"
mGattDbDynamic_DIServiceFwRevInitValue = "1.1.1\0"
mGattDbDynamic_DIServiceSwRevInitValue = "1.1.4\0"
mGattDbDynamic_DIServiceSysIdInitValue = [0x00, 0x00, 0x00, 0xFE, 0xFF, 0x9F, 0x04, 0x00]
mGattDbDynamic_DIServiceIeeeRcdlInitValue = [0x00, 0x00, 0x00, 0x00]
