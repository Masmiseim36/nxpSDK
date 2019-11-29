'''
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
    EndDevice = 0x02

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

class DiagnosticRequestAddressMode(GenericEnum):

    # Short Address Mode
    TargetShort = 0x02
    # IEEE Address Mode
    TargetIEEE = 0x03
    # Broadcast Address Mode
    TargetBroadcast = 0x04

class DiagnosticRequestCommandID(GenericEnum):

    # Delay Request
    DelayRequest = 0x00
    # Delay Request
    DelayResults = 0x01

class DiagnosticResponseStatus(GenericEnum):

    # Delay Response
    DelayResponse = 0x01

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

class RouterDiscoveryConfirmStatus(GenericEnum):

    # Route Discovery success.
    Success = 0x00
    # Route Discovery failed.
    Failed = 0xD0

class RouterDiscoveryConfirmNwkStatus(GenericEnum):

    # Route Discovery success.
    SUCCESS = 0x00
    # Counter Error
    COUNTER_ERROR = 0xDB
    # Improper Key Type
    IMPROPER_KEY_TYPE = 0xDC
    # Improper Security Level
    IMPROPER_SECURITY_LEVEL = 0xDD
    # Unsupported legacy
    UNSUPPORTED_LEGACY = 0xDE
    # Beacon loss
    BEACON_LOSS = 0xE0
    # Channel access failure
    CHANNEL_ACCESS_FAILURE = 0xE1
    # Denied
    DENIED = 0xE2
    # Disable TRX failure
    DISABLE_TRX_FAILURE = 0xE3
    # Failed security check
    FAILED_SECURITY_CHECK = 0xE4
    # Frame too long
    FRAME_TOO_LONG = 0xE5
    # Invalid GTS
    INVALID_GTS = 0xE6
    # Invalid handle
    INVALID_HANDLE = 0xE7
    # Invalid parameter
    INVALID_PARAMETER = 0xE8
    # No ACK
    NO_ACK = 0xE9
    # No beacon
    NO_BEACON = 0xEA
    # No data
    NO_DATA = 0xEB
    # No short address
    NO_SHORT_ADDRESS = 0xEC
    # Out of CAP
    OUT_OF_CAP = 0xED
    # Pan ID conflict
    PAN_ID_CONFLICT = 0xEE
    # Realignment
    REALIGNMENT = 0xEF
    # Transaction expired
    TRANSACTION_EXPIRED = 0xF0
    # Transaction overflow
    TRANSACTION_OVERFLOW = 0xF1
    # TX active
    TX_ACTIVE = 0xF2
    # Unavailable key
    UNAVAILABLE_KEY = 0xF3
    # Unsupported attribute
    UNSUPPORTED_ATTRIBUTE = 0xF4
    # Scan in progress
    SCAN_IN_PROGRESS = 0xF5

class APSDataConfirmFailDestAddressingMode(GenericEnum):

    # PAN identifier and address fields are not present.
    NO_PAN = 0x00
    # Reserved.
    Reserved = 0x01
    # Address field contains a 16_bit short address.
    Address_16b = 0x02
    # Address field contains a 64_bit extended address.
    Address_64b = 0x03
