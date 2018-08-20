'''
* The Clear BSD License
* Copyright 2017-2018 NXP
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

class GenericEnum(object):

    @classmethod
    def getEnumString(cls, value):
        for k, v in cls.__dict__.items():
            if v == value:
                return k
        return value


class SetDeviceTypeDeviceType(GenericEnum):

    Coordinator = 0x00
    Router = 0x01
    LegacyRouter = 0x02


class BindDestAddressingMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03


class UnbindDestAddressingMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03


class NetworkAddressRequestRequestType(GenericEnum):

    # Single.Request
    SingleRequest = 0x00
    # Extended.Request
    ExtendedRequest = 0x01


class IEEEAddressRequestRequestType(GenericEnum):

    # Single.Request
    SingleRequest = 0x00
    # Extended.Request
    ExtendedRequest = 0x01


class ManagementLeaveRequestRejoin(GenericEnum):

    DoNotRejoin = 0x00
    Rejoin = 0x01


class ManagementLeaveRequestRemoveChildren(GenericEnum):

    LeaveRemovingChildren = 0x00
    LeaveDoNotRemoveChildren = 0x01


class PermitJoiningRequestTCSignificance(GenericEnum):

    NoChangeInAuthentication = 0x00
    AuthenticationPolicyAsSpec = 0x01


class ReadAttributeRequestDirection(GenericEnum):

    FromServerToClient = 0x00
    FromClientToServer = 0x01


class ReadAttributeRequestManufacturerSpecific(GenericEnum):

    No = 0x00
    Yes = 0x01


class WriteAttributeRequestDirection(GenericEnum):

    FromServerToClient = 0x00
    FromClientToServer = 0x01


class WriteAttributeRequestManufacturerSpecific(GenericEnum):

    No = 0x00
    Yes = 0x01


class AttributeDiscoveryRequestDirection(GenericEnum):

    FromServerToClient = 0x01
    FromClientToServer = 0x00


class AttributeDiscoveryRequestManufacturerSpecific(GenericEnum):

    No = 0x00
    Yes = 0x01


class EnablePermissionsControlledJoinsEnableDisable(GenericEnum):

    Disable = 0x00
    Enable = 0x01


class OnOffWithNoEffectsCommandID(GenericEnum):

    Off = 0x00
    On = 0x01
    Toggle = 0x02


class OnOffTimedSendOnOff(GenericEnum):

    Off = 0x00
    On = 0x01


class LockUnlockDoorLockUnlock(GenericEnum):

    Lock = 0x00
    Unlock = 0x01


class StatusStatus(GenericEnum):

    Success = 0x00
    IncorrectParameters = 0x01
    UnhandledCommand = 0x02
    CommandFailed = 0x03
    # Node is carrying out a lengthy operation and is currently unable to handle the incoming command
    Busy = 0x04
    # Stack already started (no new configuration accepted)
    StackAlreadyStarted = 0x05
    # Failed (ZigBee event codes)
    Failed = 0x80
    # Failed (ZigBee event codes)
    Reserved = 0xF5


class LogMessageLogLevel(GenericEnum):

    Emergency = 0x00
    Alert = 0x01
    Critical = 0x02
    Error = 0x03
    Warning = 0x04
    Notice = 0x05
    Information = 0x06
    Debug = 0x07


class DataIndicationSourceAddressMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03


class DataIndicationDestAddressingMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03


class NetworkJoinedFormedStatus(GenericEnum):

    JoinedExistingNetwork = 0x00
    FormedNewNetwork = 0x01
    # Failed (ZigBee event codes).
    Failed = 0x80


class PermitJoinStatusResponseStatus(GenericEnum):

    Off = 0x00
    On = 0x01


class NonFactoryNewRestartStatus(GenericEnum):

    STARTUP = 0x00
    NFN_START = 0x02
    RUNNING = 0x06


class FactoryNewRestartStatus(GenericEnum):

    STARTUP = 0x00
    NFN_START = 0x02
    RUNNING = 0x06


class ListOfEntries_BitMapOfAttributesDeviceType(GenericEnum):

    Coordinator = 0x00
    Router = 0x01
    EndDevice = 0x02


class ListOfEntries_BitMapOfAttributesPermitJoinStatus(GenericEnum):

    Off = 0x00
    On = 0x01


class ListOfEntries_BitMapOfAttributesRelationship(GenericEnum):

    Parent = 0x00
    Child = 0x01
    Sibling = 0x02


class ListOfEntries_BitMapOfAttributesRxOnWhenIdleStatus(GenericEnum):

    Off = 0x00
    On = 0x01


class AttributeDiscoveryResponseComplete(GenericEnum):

    MoreAttributesToFollow = 0x00
    ThisWasTheLastAttribute = 0x01


class TouchlinkStatusStatus(GenericEnum):

    Success = 0x00
    Failure = 0x01


class ZoneStatusChangeNotificationSourceAddressMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03


class LoadRecordResponseStatus(GenericEnum):

    NoRecordFound = 0x00
    RecordRecovered = 0x01


class APSDataConfirmFailDestAddressingMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03
