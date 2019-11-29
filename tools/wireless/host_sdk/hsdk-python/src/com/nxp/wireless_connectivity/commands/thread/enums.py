'''
* Copyright 2014-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
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


class MAC_MacFilteringEnableRequestMacFiltering(GenericEnum):

    Disable = 0x00
    # Enable - Default Policy: Reject
    EnableReject = 0x01
    # Enable - Default Policy: Accept
    EnableAccept = 0x02

class THR_NwkScanRequestScanType(GenericEnum):

    EnergyDetect = 0x01
    ActiveScan = 0x02
    EnergyDetectAndActiveScan = 0x03


class THR_JoinRequestdiscoveryMethod(GenericEnum):

    gUseMACBeacon_c = 0x00
    gUseThreadDiscovery_c = 0x01


class THR_PromoteAsRouterRequestReason(GenericEnum):

    # Too Few Routers
    TooFewRouters = 0x02
    # Have Child Id Request
    HaveChildIdRequest = 0x03
    # Parent Partition Change
    ParentPartitionChange = 0x04


class THR_MgmtDiagnosticGetRequestTlvId(GenericEnum):

    # Source Address TLV (EUI64)
    SourceAddr = 0x00
    # Short Address TLV
    ShortAddr = 0x01
    # Mode TLV
    Mode = 0x02
    # Timeout - Sleepy polling rate
    Timeout = 0x03
    # Link Quality
    LinkQuality = 0x04
    # Routing table Tlv
    RoutingTable = 0x05
    # Leader Data
    LeaderData = 0x06
    # Network Data
    NwkData = 0x07
    # List of all Ipv6 addresses registered by the device
    Ip6AddrList = 0x08
    # Mac Counters
    MacCounters = 0x09
    # Battery Level
    BatteryLevel = 0x0E
    # Supply Voltage
    SupplyVoltage = 0x0F
    # Structure containing information on all children
    ChildTable = 0x10
    # Supported IEEE 802.15.4 Channel Pages
    ChannelPages = 0x11
    # MAC, 6lowpan (Mac Filtering, 6LowPan) NVM data save count
    Fsl_Mac6lowPanNvmDataCount = 0xA0
    # Network (IP, DHCP, Leader Id Assignment, Trickle, MPL, ND) NVM data save count
    Fsl_NetworkNvmDataCount_c = 0xA1
    # Security NVM data save count
    Fsl_SecurityNvmDataCount_c = 0xA2
    # Attributes, events NVM data save count
    Fsl_FunctionalNvmDataCount_c = 0xA3
    # Board name
    Fsl_BoardName_c = 0xA4
    # Unique MCU identifier
    Fsl_UniqueMcuId_c = 0xA5
    # Attribute, stack version
    Fsl_StackVersion_c = 0xA6
    # Attribute, software version
    Fsl_SoftwareVersion_c = 0xA7
    # IEEE Address
    IeeeAddress = 0xA8

class THR_DiagTestGetRequestTlvId(GenericEnum):

    # Factory reset the node
    ColdFactoryReset = 0xB0
    # CPU reset the node
    WarmCPUReset = 0xB1
    # Large Network Data
    Data = 0xB2
    # Large Network Results
    Results = 0xB3

class THR_MgmtDiagnosticResetRequestTlvId(GenericEnum):

    # Source Address TLV (EUI64)
    SourceAddr = 0x00
    # Short Address TLV
    ShortAddr = 0x01
    # Mode TLV
    Mode = 0x02
    # Timeout - Sleepy polling rate
    Timeout = 0x03
    # Link Quality
    LinkQuality = 0x04
    # Routing table Tlv
    RoutingTable = 0x05
    # Leader Data
    LeaderData = 0x06
    # Network Data
    NwkData = 0x07
    # List of all IPv6 addresses registered by the device
    Ip6AddrList = 0x08
    # Mac Counters
    MacCounters = 0x09
    # Battery Level
    BatteryLevel = 0x0E
    # Supply Voltage
    SupplyVoltage = 0x0F
    # Structure containing information on all children
    ChildTable = 0x10
    # Supported IEEE 802.15.4 Channel Pages
    ChannelPages = 0x11
    # MAC, 6lowpan (Mac Filtering, 6LowPan) NVM data save count
    Fsl_Mac6lowPanNvmDataCount = 0xA0
    # Network (IP, DHCP, Leader Id Assignment, Trickle, MPL, ND) NVM data save count
    Fsl_NetworkNvmDataCount_c = 0xA1
    # Security NVM data save count
    Fsl_SecurityNvmDataCount_c = 0xA2
    # Attributes, events NVM data save count
    Fsl_FunctionalNvmDataCount_c = 0xA3
    # Board name
    Fsl_BoardName_c = 0xA4
    # Unique MCU identifier
    Fsl_UniqueMcuId_c = 0xA5
    # Attribute, stack version
    Fsl_StackVersion_c = 0xA6
    # Attribute, software version
    Fsl_SoftwareVersion_c = 0xA7

class THR_SetParentPriorityRequestPriority(GenericEnum):

    # Default Priority
    MediumPriority = 0x00
    # High Priority
    HighPriority = 0x01
    # Low Priority
    LowPriority = 0x03
    # Reset to dynamically compute the parent priority.
    Reset = 0xFF

class THR_SetThresholdRequestThresholdType(GenericEnum):

    # The number of active routers on the Thread network below which a REED
    # may device to become a Router
    RouterUpgradeThreshold = 0x00
    # The number of active Routers on the Thread Network above which an active
    # Router may decide to become a Child
    RouterDowngradeThreshold = 0x01
    # The minimum number of neighbours with link quality 2 or better that a
    # Router must have to downgrade to a REED
    MinDowngradeNeighbors = 0x02
    # The maximum number of Routers that a Thread Network may contain
    MaxAllowedRouters = 0x03

class THR_SetThreshold32RequestThresholdType(GenericEnum):

    # The maximum time a context can be used for decompression after the Prefix has been removed.
    ContextReuseDelay = 0x04


class THR_GetAttrRequestAttributeId(GenericEnum):

    # Random MAC address used for communication inside the Thread nwk
    RandomExtendedAddr = 0x00
    # Short Address
    ShortAddress = 0x01
    # Scan channel mask
    ScanChannelMask = 0x02
    # Scan duration
    ScanDuration = 0x03
    # 802.15.4 channel
    Channel = 0x04
    # Short Pan Id
    ShortPanId = 0x05
    # Extended Pan Id
    ExtendedPanId = 0x06
    # Permit Join
    PermitJoin = 0x07
    # Rx on idle status
    RxOnIdle = 0x08
    # The polling interval for the sleepy end device (SED)[milliseconds]
    SedPollInterval = 0x09
    # Unique Extended Address Enabled or disabled
    UniqueExtendedAddress = 0x0A
    # Vendor Name
    VendorName = 0x0B
    # Model Name
    ModelName = 0x0C
    # Software version
    SwVersion = 0x0D
    # Stack version
    StackVersion = 0x0E
    # Network Capabilities
    NwkCapabilities = 0x0F
    # Network Name
    NwkName = 0x10
    # Device type
    DeviceType = 0x11
    # Is Device Connected?
    IsDevConnected = 0x12
    # Is Device Commissioned?
    IsDevCommissioned = 0x13
    # Partition Identifier
    PartitionId = 0x14
    # Device role
    DeviceRole = 0x15
    # Network Master Key
    Security_NwkMasterKey = 0x16
    # Network Key Sequence
    Security_NwkKeySeq = 0x17
    # PSKc
    Security_PSKc = 0x18
    # PSKd
    Security_PSKd = 0x19
    # Vendor Data
    VendorData = 0x1A
    # Mesh local prefix
    MLPrefix = 0x1C
    # Mac Filtering Entry
    MacFilteringEntry = 0x1D
    # KeyRotationInterval
    Security_KeyRotationInterval = 0x20
    # Child address mask
    ChildAddrMask = 0x21
    # The timeout period included in the Child ID Request sent to the parent
    ChildSEDTimeout = 0x22
    # The timeout period included in the Child ID Request sent to the parent
    ChildEDTimeout = 0x1B
    # If it is set TRUE The child End device should request the Full network data
    EndDevice_ChildEDReqFullNwkData = 0x23
    # IsFastPollEnabled
    EndDevice_IsFastPollEnabled = 0x24
    # FastPollInterval
    SleepyEndDevice_FastPollInterval = 0x25
    JoinLqiThreshold = 0x26
    # A URL for the Joiner to communicate to the user which Commissioning
    # application is best to use to properly provision it to the appropriate
    # service
    ProvisioningURL = 0x27
    # The energy channel threshold to select the best channel when more
    # channels are scan to form the network
    SelectBestChannelEDThreshold = 0x28
    # Select Commissioner mode
    CommissionerMode = 0x29
    # Border Router prefix entry
    BorderRouter_BrPrefixEntry = 0x30
    # Steering data
    SteeringData = 0x31
    # The thread Key switch guard time to prevent inadvertent key switching
    Security_KeySwitchGuardTime = 0x33
    # Hold time on parent device in seconds
    ParentHoldTime = 0x34
    # SecurityPolicy, O and N bits without the rotation times
    Security_Policy = 0x35
    # Stack starts automatically with NVM restore after reset
    NVM_RestoreAutoStart = 0x36
    # Restore from NVM
    NVM_Restore = 0x37
    # Slaac IID generation policy
    SlaacPolicy = 0x38
    # IEEE extended mac address
    IeeeExtendedAddr = 0x39
    # Leader Weight
    LeaderWeight = 0x3A
    # SHA256 generated MAC address used during commissioning phase
    HashIeeeAddr = 0x40
    # Global /64 on-Mesh Prefix on Border Router
    BorderRouter_BrGlobalOnMeshPrefix = 0x50
    # Default Route of the /64 on-mesh prefix
    BorderRouter_BrDefaultRouteOnMeshPrefix = 0x51
    # Global /64 external interface prefix
    BorderRouter_BrExternalIfPrefix = 0x52
    # Active timestamp
    ActiveTimestamp = 0x60
    # Pending Channel
    PendingChannel = 0x61
    # Pending Channel Mask
    PendingChannelMask = 0x62
    # Pending Extended PanId
    PendingXpanId = 0x63
    # Pending MeshLocal prefix
    PendingMLprefix = 0x64
    # Pending Master Key
    PendingNwkMasterKey = 0x65
    # Pending Network Name
    PendingNwkName = 0x66
    # Pending Pan ID
    PendingPanId = 0x67
    # Pending PSKc
    PendingPSK = 0x68
    # Pending Security Policy bits
    PendingSecurityPolicy = 0x69
    # Pending Key Rotation Interval [sec]
    PendingNwkKeyRotationInterval = 0x6A
    # Pending Delay Timer [msec]
    PendingDelayTimer = 0x6B
    # Pending Active Timestamp
    PendingActiveTimestamp = 0x6C
    # Pending Timestamp
    PendingTimestamp = 0x6D
    # Commissioner string
    CommissionerId = 0x6E
    # Joiner UDP Port
    JoinerPort = 0x6F
    # Commissioner UDP Port
    CommissionerUdpPort = 0x70
    # The default discovery request Mac Tx options
    DiscoveryReqMacTxOptions = 0x71


class THR_SetAttrRequestAttributeId(GenericEnum):

    # Random MAC address used for communication inside the Thread nwk
    RandomExtendedAddr = 0x00
    # Scan channel mask
    ScanChannelMask = 0x02
    # Scan duration
    ScanDuration = 0x03
    # 802.15.4 channel
    Channel = 0x04
    # Short Pan Id
    ShortPanId = 0x05
    # Extended Pan Id
    ExtendedPanId = 0x06
    # Rx on idle status
    RxOnIdle = 0x08
    # The polling interval for the sleepy end device (SED)[milliseconds]
    SedPollInterval = 0x09
    # Unique Extended Address Enabled or disabled
    UniqueExtendedAddress = 0x0A
    # Vendor Name
    VendorName = 0x0B
    # Model Name
    ModelName = 0x0C
    # Software version
    SwVersion = 0x0D
    # Network Capabilities
    NwkCapabilities = 0x0F
    # Network Name
    NwkName = 0x10
    # Is Device Connected?
    IsDevConnected = 0x12
    # Is Device Commissioned?
    IsDevCommissioned = 0x13
    # Partition Identifier
    PartitionId = 0x14
    # Device role
    DeviceRole = 0x15
    # Network Master Key
    Security_NwkMasterKey = 0x16
    # Network Key Sequence
    Security_NwkKeySeq = 0x17
    # PSKc - network password
    Security_PSKc = 0x18
    # PSKd - device password
    Security_PSKd = 0x19
    # Vendor Data
    VendorData = 0x1A
    # Mesh local prefix
    MLPrefix = 0x1C
    # Key rotation interval
    Security_KeyRotationInterval = 0x20
    # The timeout period included in the Child ID Request sent to the parent
    ChildSEDTimeout = 0x22
    # If it is set TRUE The child End device should request the Full network data
    EndDevice_ChildEDReqFullNwkData = 0x23
    # Is Fast Poll Interval?
    EndDevice_IsFastPollEnabled = 0x24
    # Fast Poll Interval
    SleepyEndDevice_FastPollInterval = 0x25
    # Join Lqi threshold
    JoinLqiThreshold = 0x26
    # A URL for the Joiner to communicate to the user which Commissioning
    # application is best to use
    # to properly provision it to the appropriate service
    ProvisioningURL = 0x27
    # The energy channel threshold to select the best channel when
    # more channels are scan to form the network
    SelectBestChannelEDThreshold = 0x28
    # Border Router prefix entry
    BorderRouter_BrPrefixEntry = 0x30
    # the thread Key switch guard time to prevent inadvertent key switching
    Security_KeySwitchGuardTime = 0x33
    # The hoold time period in seconds used by the parent to hold the packets for SED devices
    ParentHoldTime = 0x34
    # Security Policy, O and N bits without the rotation time
    Security_Policy = 0x35
    # Stack starts automatically with NVM restore after reset
    NVM_RestoreAutoStart = 0x36
    # Restore from NVM
    NVM_Restore = 0x37
    # Specifies the policy for generating the IID of an address configured using SLAAC
    SlaacPolicy = 0x38
    # IEEE extended mac address
    IeeeExtendedAddr = 0x39
    # Leader Weight
    LeaderWeight = 0x3A
    # Avoid generation of a random partition ID
    DoNotGeneratePartitionId = 0x41
    # Global /64 on-Mesh Prefix on Border Router
    BorderRouter_BrGlobalOnMeshPrefix = 0x50
    # Default Route of the /64 on-mesh prefix
    BorderRouter_BrDefaultRouteOnMeshPrefix = 0x51
    # Global /64 external interface prefix
    BorderRouter_BrExternalIfPrefix = 0x52
    # Active timestamp
    ActiveTimestamp = 0x60
    # Joiner Port
    JoinerPort = 0x6F


class DeviceTypeValue(GenericEnum):

    EndNode = 0x00
    # Combo (Leader/Router/EndDevice/SleepyEndDevice)
    Combo = 0x01


class DeviceRoleValue(GenericEnum):

    Disconnected = 0x00
    SleepyEndDevice = 0x01
    MinimalEndDevice = 0x02
    FullEndDevice = 0x03
    RouterEligibleEndDevice = 0x04
    Router = 0x05
    Leader = 0x06


class CommissionerModeValue(GenericEnum):

    # Disable Commissioner on this device
    Disabled = 0x00
    # Enable Collapsed Commissioner
    Collapsed = 0x01
    # Enable Native Commissioner
    Native = 0x02
    # Enable Ethernet Commissioner
    Ethernet = 0x04
    # Enable OnMesh Commissioner
    OnMesh = 0x08


class PrefixFlagsP_preference(GenericEnum):

    # Medium(Default)
    MediumDefault = 0x00
    High = 0x01
    # Reserved - MUST NOT be used
    Reserved = 0x02
    Low = 0x03


class ExternalRouteFlagsR_preference(GenericEnum):

    # Medium(Default)
    MediumDefault = 0x00
    High = 0x01
    # Reserved - MUST NOT be used
    Reserved = 0x02
    Low = 0x03


class SlaacPolicyValue(GenericEnum):

    # Slaac IID is constructed random
    SlaacRandom = 0x00
    # Slaac IID is constructed by the application
    SlaacManual = 0x01
    # Slaac IID is constructed using the ML-EID IID
    SlaacMlIid = 0x02


class THR_GetThreadIpAddrRequestAddressType(GenericEnum):

    Link_Local_64 = 0x00
    MLEID = 0x01
    RLOC = 0x02
    Global = 0x03
    Anycast = 0x04


class SocketCreateRequestSocketDomain(GenericEnum):

    AF_INET6 = 0x0A
    AF_INET = 0x02


class SocketCreateRequestSocketType(GenericEnum):

    Datagram = 0x00
    Stream = 0x01


class SocketCreateRequestSocketProtocol(GenericEnum):

    UDP = 0x11
    TCP = 0x06


class SocketBindRequestSocketDomain(GenericEnum):

    AF_INET6 = 0x0A
    AF_INET = 0x02


class SocketSendRequestFlags(GenericEnum):

    MSG_DONTWAIT = 0x40


class SocketSendToRequestFlags(GenericEnum):

    MSG_DONTWAIT = 0x40


class SocketReceiveRequestFlags(GenericEnum):

    MSG_DONTWAIT = 0x40


class SocketReceiveFromRequestFlags(GenericEnum):

    MSG_DONTWAIT = 0x40


class SocketConnectRequestSocketDomain(GenericEnum):

    AF_INET6 = 0x0A
    AF_INET = 0x02


class SocketSetOptionRequestSocketLevel(GenericEnum):

    SOL_SOCKET = 0x00
    SOL_IP = 0x01
    SOL_UDP = 0x02
    SOL_TCP = 0x03


class SocketSetOptionRequestSocketOption(GenericEnum):

    SO_TYPE = 0x0000
    SO_BINDTODEVICE = 0x0019
    SO_REUSEADDR = 0x0004
    IPV6_MULTICAST_HOPS = 0x0012
    IPV6_UNICAST_HOPS = 0x0010
    IPV6_JOIN_GROUP = 0x0014
    IP_MULTICAST_TTL = 0x0021
    IP_ADD_MEMBERSHIP = 0x0023
    IP_DROP_MEMBERSHIP = 0x0024
    SO_SOCKETBLOCK = 0x1003

class SocketGetOptionRequestSocketLevel(GenericEnum):

    SOL_SOCKET = 0x00
    SOL_IP = 0x01
    SOL_UDP = 0x02
    SOL_TCP = 0x03


class SocketGetOptionRequestSocketOption(GenericEnum):

    SO_TYPE = 0x0000
    SO_RCVBUF = 0x1002
    SO_SOCKETBLOCK = 0x1003

class MESHCOP_AddExpectedJoinerRequestEuiType(GenericEnum):

    # Add a short EUI
    ShortEUI = 0x00
    # Add a long EUI
    LongEUI = 0x01


class MESHCOP_GetExpectedJoinerRequestEuiType(GenericEnum):

    # Add a short EUI
    ShortEUI = 0x00
    # Add a long EUI
    LongEUI = 0x01


class MESHCOP_RemoveExpectedJoinerRequestEuiType(GenericEnum):

    # Add a short EUI
    ShortEUI = 0x00
    # Add a long EUI
    LongEUI = 0x01


class MESHCOP_SyncSteeringDataRequestEuiMask(GenericEnum):

    # All zeroes(joining is disabled)
    AllZeroes = 0x00
    # Allow all devices are allowed
    AllFFs = 0x01
    # Allow only expected Joiners
    ExpectedJoiners = 0x02


class MESHCOP_MgmtGetRequestTlvId(GenericEnum):

    # Network PSKc
    PSKc = 0x04
    # Network Channel
    Channel = 0x00
    # Network Pan ID
    PanId = 0x01
    # Network Extended Pan ID
    XpanId = 0x02
    # Network Name
    NetworkName = 0x03
    # Network Master Key
    MasterKey = 0x05
    # Network Key Sequence
    KeySequence = 0x06
    # Network Mesh Local ULA prefix
    MeshLocalUla = 0x07
    # Network Steering Data
    SteeringData = 0x08
    # Network Border Router Locator
    BorderRouterLocator = 0x09
    # Network Commissioner ID
    CommissionerID = 0x0A
    # Network Commissioner Session ID
    CommissionerSessionID = 0x0B
    # Network Security Policy
    SecurityPolicy = 0x0C
    # Network Commissioner Dataset Timestamp
    ActiveTimestamp = 0x0E


class MESHCOP_MgmtCommissionerGetRequestTlvId(GenericEnum):

    # Border Router Locator
    BorderRouterLocator = 0x09
    # Commissioner Session ID
    CommissionerSessionId = 0x0B
    # Steering Data
    SteeringData = 0x08
    # Network Channel
    Channel = 0x00
    # Network Channel Mask
    ChannelMask = 0x35
    # Network Extended Pan ID
    XpanId = 0x02
    # Network Mesh Local ULA prefix
    MeshLocalUla = 0x07
    # Network Master Key
    MasterKey = 0x05
    # Network Name
    NetworkName = 0x03
    # Network Pan ID
    PanId = 0x01
    # Network PSKc
    PSKc = 0x04
    # Network Security Policy
    SecurityPolicy = 0x0C
    # Active Commissioner Dataset Timestamp
    ActiveTimestamp = 0x0E


class MESHCOP_MgmtActiveGetRequestTlvId(GenericEnum):

    # Network Channel
    Channel = 0x00
    # Network Channel Mask
    ChannelMask = 0x35
    # Network Extended Pan ID
    XpanId = 0x02
    # Network Mesh Local ULA prefix
    MeshLocalUla = 0x07
    # Network Master Key
    MasterKey = 0x05
    # Network Name
    NetworkName = 0x03
    # Network Pan ID
    PanId = 0x01
    # Network PSKc
    PSKc = 0x04
    # Network Security Policy
    SecurityPolicy = 0x0C
    # Active Commissioner Dataset Timestamp
    ActiveTimestamp = 0x0E
    # Scan Duration (not allowed - for certification purposes)
    ScanDuration = 0x38
    # Energy List (not allowed - for certification purposes)
    EnergyList = 0x39


class MESHCOP_MgmtPendingGetRequestTlvId(GenericEnum):

    # Network Channel
    Channel = 0x00
    # Network Channel Mask
    ChannelMask = 0x35
    # Network Extended Pan ID
    XpanId = 0x02
    # Network Mesh Local ULA prefix
    MeshLocalUla = 0x07
    # Network Master Key
    MasterKey = 0x05
    # Network Name
    NetworkName = 0x03
    # Network Pan ID
    PanId = 0x01
    # Network PSKc
    PSKc = 0x04
    # Network Security Policy
    SecurityPolicy = 0x0C
    # Delay Timer
    DelayTimer = 0x34
    # Active Commissioner Dataset Timestamp
    ActiveTimestamp = 0x0E
    # Pending Commissioner Dataset Timestamp
    PendingTimestamp = 0x33


class NWKU_CoapCreateInstanceRequestSocketDomain(GenericEnum):

    AF_INET6 = 0x0A
    AF_INET = 0x02


class NWKU_CoapSendRequestRequestType(GenericEnum):

    CON = 0x00
    NON = 0x01


class NWKU_CoapSendRequestMessageType(GenericEnum):

    GET = 0x01
    POST = 0x02
    PUT = 0x03
    DELETE = 0x04


class NWKU_CoapRegisterRequestSocketDomain(GenericEnum):

    AF_INET6 = 0x0A
    AF_INET = 0x02


class NWKU_McastGroupManageRequestAction(GenericEnum):

    JoinGroup = 0x01
    LeaveGroup = 0x02
    Commissioner = 0x03


class NWKU_FirewallRuleAddRequestRule(GenericEnum):

    Drop = 0x00
    Accept = 0x01


class NWKU_RoutesManageRequestAction(GenericEnum):

    Add = 0x01
    Remove = 0x02


class SerialTun_LinkIndicationRequestState(GenericEnum):

    Down = 0x00
    Up = 0x01


class Sniffer_MacSetPIBAttributeRequestPIBAttribute(GenericEnum):

    # The channel to use
    macLogicalChannel = 0x21
    # MAC_PromiscuousRx.Indication (receive all) mode enabled or not
    macMAC_PromiscuousRxIndicationMode = 0x51
    # The receiver is enabled during IDLE periods
    macRxOnWhenIdle = 0x52


class FSCIACKStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketShutdownConfirmStatus(GenericEnum):

    OK = 0x00
    ERRORSocketisnoTCP = 0xFF


class SocketBindConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketSendConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketSendToConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketReceiveFromConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketConnectConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketListenConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketAcceptConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketSetOptionConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketGetOptionConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class SocketCloseConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class MAC_MacFilteringAddEntryConfirmStatus(GenericEnum):

    Success = 0x00
    Notpermitted = 0x04
    Nomemory = 0x06


class MAC_MacFilteringRemoveEntryConfirmStatus(GenericEnum):

    Success = 0x00
    Notpermitted = 0x04


class MAC_MacFilteringEnableConfirmStatus(GenericEnum):

    Success = 0x00
    Notpermitted = 0x04


class THR_SetDeviceConfigConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    Invalidparameter = 0x03
    Notpermitted = 0x04
    UnsupportedAttribute = 0x07
    EmptyEntry = 0x08
    InvalidValue = 0x09


class THR_NwkScanConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    InvalidParam = 0x03
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_CreateNwkConfirmStatus(GenericEnum):

    OK = 0x00
    AlreadyConnected = 0x0A
    AlreadyCreated = 0x0B
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_JoinConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_FactoryResetConfirmStatus(GenericEnum):

    Success = 0x00
    Theselectedconfigurationisnotvalid = 0xFF


class THR_CpuResetConfirmStatus(GenericEnum):

    Success = 0x00
    Theselectedconfigurationisnotvalid = 0xFF


class THR_CpuResetIndicationStatus(GenericEnum):

    ResetCpuSuccess = 0x00
    ResetCpuPending = 0x01


class StackVersionStructStackVendorOUI(GenericEnum):

    NXP = 0x006037


class THR_DisconnectConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Operationnotpermiteddevicealreadydisconnected = 0x04
    Theselectedconfigurationisnotvalid = 0xFF


class THR_AttachConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_PromoteAsRouterConfirmStatus(GenericEnum):

    OK = 0x00
    InvalidInstance = 0x02
    NotPermitted = 0x04


class THR_EventNwkScanConfirmEventStatus(GenericEnum):

    ScanResult = 0x0001


class THR_EventNwkScanConfirmScanType(GenericEnum):

    EnergyDetect = 0x01
    ActiveScan = 0x02
    EnergyDetectAndActiveScan = 0x03


class THR_EventNwkCreateConfirmEventStatus(GenericEnum):

    Success = 0x0001
    Failed = 0x0002
    SelectBestChannel = 0x0003
    GeneratePSKc = 0x0004


class THR_EventNwkJoinConfirmEventStatus(GenericEnum):

    Attaching = 0x0001
    JoinSuccess = 0x0002
    JoinFailed = 0x0003


class THR_EventNwkJoinSelectParentsConfirmEventStatus(GenericEnum):

    ScanStarted = 0x0001
    ReceivedBeacon = 0x0002
    ScanEnded = 0x0003


class THR_EventGeneralConfirmEventStatus(GenericEnum):

    Disconnected = 0x0001
    Connected = 0x0002
    ResetToFactoryDefault = 0x0003
    InstanceRestoreStarted = 0x0004
    RouterSynced = 0x0005
    EndDeviceSynced = 0x0006
    ConnectingStarted = 0x0007
    ConnectingFailed = 0x0008
    ConnectingDeffered = 0x0009
    DeviceIsLeader = 0x000A
    DeviceIsRouter = 0x000B
    DeviceIsREED = 0x000C
    DeviceIsFullEndDevice = 0x000D
    DeviceIsSleepyEndDevice = 0x000E
    RequestingGlobalAddress = 0x000F
    GlobalAddressAssigned = 0x0010
    RequestingRouterId = 0x0011
    RouterIdAssigned = 0x0012
    RouterIdAssignFailed = 0x0013
    AllowDeviceToSleep = 0x0014
    DisallowDeviceToSleep = 0x0015
    ChildIdAssigned = 0x0016
    DeviceIsMinimalEndDevice = 0x0017
    ChildRemoved = 0x0018
    AllChildrenRemoved = 0x0019
    RouterRemoved = 0x001A
    RoutingUpdates = 0x001B


class THR_EventNwkCommissioningIndicationEventStatus(GenericEnum):

    # Joiner has started discovery
    JoinerDiscoveryStarted = 0x0001
    # No Thread networks/routers found
    JoinerDiscoveryFailedNoBeacon = 0x0002
    # Joiner Routers found, but device is filtered
    JoinerDiscoveryFailedFiltered = 0x0003
    # Network selected
    JoinerDiscoverySuccess = 0x0004
    # Started DTLS session to Commissioner (sent Hello)
    JoinerDtlsSessionStarted = 0x0005
    # DTLS session error - all DTLS errors, e.g.: incorrect PSKd
    JoinerDtlsError = 0x0006
    # All other non-DTLS errors (e.g.: Joiner Router failed to send credentials)
    JoinerError = 0x0007
    # Joiner has received credentials
    JoinerAccepted = 0x0008
    # Petitioning has started
    CommissionerPetitionStarted = 0x0009
    # Petition success
    CommissionerPetitionAccepted = 0x000A
    # Petition rejected
    CommissionerPetitionRejected = 0x000B
    # Other errors in petitioning (did not get PET response)
    CommissionerPetitionError = 0x000C
    # Keep Alive was sent
    CommissionerKeepAliveSent = 0x000D
    # Errors during generating Keep ALive or other errors on the Commissioner session
    CommissionerError = 0x000E
    # A Joiner sent Hello
    CommissionerJoinerDtlsSessionStarted = 0x000F
    # DTLS session error - all DTLS errors, e.g.: incorrect PSKd
    CommissionerJoinerDtlsError = 0x0010
    # Joiner accepted
    CommissionerJoinerAccepted = 0x0011
    # The Commissioner changed the Nwk data
    CommissionerNwkDataSynced = 0x0012
    # Started DTLS session to BR (sent Hello)
    CommissionerBrDtlsSessionStarted = 0x0013
    # DTLS session error - all DTLS errors, e.g.: incorrect PSKc
    CommissionerBrDtlsError = 0x0014
    # All Other errors non-DTLS errors when communicating with the BR
    CommissionerBrError = 0x0015
    # BR session established
    CommissionerBrAccepted = 0x0016
    # Commissioner sent Hello
    BrCommissionerDtlsSessionStarted = 0x0017
    # DTLS session error - all DTLS errors, e.g.: incorrect PSKc
    BrCommissionerDtlsError = 0x0018
    # BR session established
    BrCommissionerAccepted = 0x0019
    # Relay sent from BR to Thread
    BrCommissionerDataRelayedInbound = 0x001A
    # Relay sent to BR from Thread
    BrCommissionerDataRelayedOutbound = 0x001B
    # Relay sent from Joiner to Commissioner
    JoinerrouterJoinerDataRelayedInbound = 0x001C
    # Relay sent to Joiner from Commissioner
    JoinerrouterJoinerDataRelayedOutbound = 0x001D
    # Providing the security material to the Joiner
    JoinerrouterJoinerAccepted = 0x001E
    # Start Vendor Provisioning
    StartVendorProvisioning = 0x001F


class THR_CommissioningDiagnosticIndicationDirection(GenericEnum):

    # Sent packet
    OUT = 0x00
    # Received packet
    IN = 0x01


class THR_CommissioningDiagnosticIndicationType(GenericEnum):

    # JOIN_FIN.REQ
    JOIN_FIN_REQ = 0x00
    # JOIN_FIN.RSP
    JOIN_FIN_RSP = 0x01
    # JOIN_ENT.REQ
    JOIN_ENT_REQ = 0x02
    # JOIN_ENT.RSP
    JOIN_ENT_RSP = 0x03
    DTLS_CLOSE_NOTIFY = 0x04


class THR_MgmtDiagnosticGetConfirmStatus(GenericEnum):

    Success = 0x00
    InvalidParameter = 0x03
    NotPermitted = 0x04
    NoMemory = 0x06
    Error = 0xFF


class THR_DiagTestGetConfirmStatus(GenericEnum):

    Success = 0x00
    InvalidParameter = 0x03
    NotPermitted = 0x04
    Error = 0xFF


class THR_MgmtDiagnosticResetConfirmStatus(GenericEnum):

    Success = 0x00
    InvalidParameter = 0x03
    NotPermitted = 0x04
    Error = 0xFF


class THR_MgmtWriteMemoryConfirmStatus(GenericEnum):

    Success = 0x00


class THR_SetManualSlaacIIDConfirmStatus(GenericEnum):

    OK = 0x00

class THR_SetParentPriorityConfirmStatus(GenericEnum):

    OK = 0x00
    Error = 0xFF


class THR_SendProactiveAddrNotifConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    InvalidInstance = 0x02
    NotStarted = 0x05


class THR_NwkDiscoveryConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_NwkDiscoveryStopConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_SearchNwkWithAnounceConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_MgmtDiagnosticGetRspIndicationStatus(GenericEnum):

    Success = 0x00
    FailedNotsupported = 0x01

class THR_MgmtDiagnosticGetRspIndicationTlvId(GenericEnum):

    # Source Address TLV (EUI64)
    SourceAddr = 0x00
    # Short Address TLV
    ShortAddr = 0x01
    # Mode TLV
    Mode = 0x02
    # Timeout - Sleepy polling rate
    Timeout = 0x03
    # Link Quality
    LinkQuality = 0x04
    # Routing table Tlv
    RoutingTable = 0x05
    # Leader Data
    LeaderData = 0x06
    # Network Data
    NwkData = 0x07
    # List of all Ipv6 addresses registered by the device
    Ip6AddrList = 0x08
    # Mac Counters
    MacCounters = 0x09
    # Battery Level
    BatteryLevel = 0x0E
    # Supply Voltage
    SupplyVoltage = 0x0F
    # Structure containing information on all children
    ChildTable = 0x10
    # Supported IEEE 802.15.4 24 Channel Pages
    ChannelPages = 0x11
    # MAC, 6lowpan (Mac Filtering, 6LowPan) NVM data save count
    Fsl_Mac6lowPanNvmDataCount = 0xA0
    # Network (IP, DHCP, Leader Id Assignment, Trickle, MPL, ND) NVM data save count
    Fsl_NetworkNvmDataCount_c = 0xA1
    # Security NVM data save count
    Fsl_SecurityNvmDataCount_c = 0xA2
    # Attributes, events NVM data save count
    Fsl_FunctionalNvmDataCount_c = 0xA3
    # Board name
    Fsl_BoardName_c = 0xA4
    # Unique MCU identifier
    Fsl_UniqueMcuId_c = 0xA5
    # Attribute, stack version
    Fsl_StackVersion_c = 0xA6
    # Attribute, software version
    Fsl_SoftwareVersion_c = 0xA7
    # IEEE Address
    IeeeAddress = 0xA8

class Fsl_Mac6lowPanNvmDataCountNvmDataSetId(GenericEnum):

    NvmId_SlwpStruct = 0x0000
    NvmId_ContextTable = 0x0001
    NvmId_macFilteringTable = 0x000B


class Fsl_SecurityNvmDataCount_cNvmDataSetId(GenericEnum):

    NvmId_MleSecInfo = 0x0012
    NvmId_MleActiveKeyIndex = 0x0013
    NvmId_MacOutSecFrameCounter = 0x0014


class Fsl_FunctionalNvmDataCount_cNvmDataSetId(GenericEnum):

    NvmId_ThrAttr = 0x0010
    NvmId_EventsTbl = 0x0011
    NvmId_ThrStringAttr = 0x0018
    NvmId_BrPrefixSetAttr = 0x0019


class Fsl_StackVersion_cStackVendorOUI(GenericEnum):

    NXP = 0x006037


class THR_DiagTestGetRspIndicationStatus(GenericEnum):

    Success = 0x00
    FailedNotsupported = 0x01


class THR_DiagTestGetRspIndicationPayload_TlvId(GenericEnum):

    # Factory reset the node
    ColdFactoryReset = 0xB0
    # CPU reset the node
    WarmCPUReset = 0xB1
    # Large Network Data
    Data = 0xB2
    # Large Network Results
    Results = 0xB3


class THR_MgmtDiagnosticResetRspIndicationStatus(GenericEnum):

    Success = 0x00
    FailedNotallowed = 0x01


class THR_SetNwkIdTimeoutConfirmStatus(GenericEnum):

    Success = 0x00
    Notpermitted = 0x04


class THR_SetRouterPromoteMaxJitterConfirmStatus(GenericEnum):

    Success = 0x00
    Notpermitted = 0x04


class THR_SetThresholdConfirmStatus(GenericEnum):

    Success = 0x00
    InvalidParameter = 0x03

class THR_SetThreshold32ConfirmStatus(GenericEnum):

    Success = 0x00
    InvalidParameter = 0x03

class THR_GetNeighborInfoConfirmStatus(GenericEnum):

    Success = 0x00
    # Neighbor Not Found
    NeighborNotFound = 0x01


class THR_GetAttrConfirmAttributeId(GenericEnum):

    # Random MAC address used for communication inside the Thread nwk
    RandomExtendedAddr = 0x00
    # Short Address
    ShortAddress = 0x01
    # Scan channel mask
    ScanChannelMask = 0x02
    # Scan duration
    ScanDuration = 0x03
    # 802.15.4 channel
    Channel = 0x04
    # Short Pan Id
    ShortPanId = 0x05
    # Extended Pan Id
    ExtendedPanId = 0x06
    # Permit Join
    PermitJoin = 0x07
    # Rx on idle status
    RxOnIdle = 0x08
    # The polling interval for SED (sleepy end device) [miliseconds]
    SedPollInterval = 0x09
    # Unique Extended Address Enabled or disabled
    UniqueExtendedAddress = 0x0A
    # Vendor Name
    VendorName = 0x0B
    # Model Name
    ModelName = 0x0C
    # Software version
    SwVersion = 0x0D
    # Stack version
    StackVersion = 0x0E
    # Network Capabilities
    NwkCapabilities = 0x0F
    # Network Name
    NwkName = 0x10
    # Device type
    DeviceType = 0x11
    # Is Device Connected?
    IsDevConnected = 0x12
    # Is Device Commissioned?
    IsDevCommissioned = 0x13
    # Partition Identifier
    PartitionId = 0x14
    # Device role
    DeviceRole = 0x15
    # Network Master Key
    Security_NwkMasterKey = 0x16
    # Network Key Sequence
    Security_NwkKeySeq = 0x17
    # PSKc
    Security_PSKc = 0x18
    # PSKd
    Security_PSKd = 0x19
    # Vendor Data
    VendorData = 0x1A
    # Mesh local prefix
    MLPrefix = 0x1C
    # Mac Filtering Entry
    MacFilteringEntry = 0x1D
    # KeyRotationInterval
    Security_KeyRotationInterval = 0x20
    # Child address mask
    ChildAddrMask = 0x21
    # The timeout period included in the Child ID Request sent to the parent
    ChildSEDTimeout = 0x22
    # The timeout period included in the Child ID Request sent to the parent
    ChildEDTimeout = 0x1B
    # If it is set TRUE The child End device should request the Full network data
    EndDevice_ChildEDReqFullNwkData = 0x23
    # IsFastPollEnabled
    EndDevice_IsFastPollEnabled = 0x24
    # FastPollInterval
    SleepyEndDevice_FastPollInterval = 0x25
    JoinLqiThreshold = 0x26
    # A URL for the Joiner to communicate to the user which Commissioning
    # application is best to use to properly provision it to the appropriate
    # service
    ProvisioningURL = 0x27
    # The energy channel threshold to select the best channel when more
    # channels are scan to form the network
    SelectBestChannelEDThreshold = 0x28
    # Select Commissioner mode
    CommissionerMode = 0x29
    # Border Router prefix entry
    BorderRouter_BrPrefixEntry = 0x30
    # Steering data
    SteeringData = 0x31
    # The thread Key switch guard time to prevent inadvertent key switching
    Security_KeySwitchGuardTime = 0x33
    # Hold time on parent device in seconds
    ParentHoldTime = 0x34
    # SecurityPolicy, O and N bits without the rotation times
    Security_Policy = 0x35
    # Stack starts automatically with NVM restore after reset
    NVM_RestoreAutoStart = 0x36
    # Restore from NVM
    NVM_Restore = 0x37
    # Slaac IID generation policy
    SlaacPolicy = 0x38
    # IEEE extended mac address
    IeeeExtendedAddr = 0x39
    # Leader Weight
    LeaderWeight = 0x3A
    # SHA256 generated MAC address used during commissioning phase
    HashIeeeAddr = 0x40
    # Global /64 on-Mesh Prefix on Border Router
    BorderRouter_BrGlobalOnMeshPrefix = 0x50
    # Default Route of the /64 on-mesh prefix
    BorderRouter_BrDefaultRouteOnMeshPrefix = 0x51
    # Global /64 external interface prefix
    BorderRouter_BrExternalIfPrefix = 0x52
    # Active timestamp
    ActiveTimestamp = 0x60
    # Pending Channel
    PendingChannel = 0x61
    # Pending Channel Mask
    PendingChannelMask = 0x62
    # Pending Extended PanId
    PendingXpanId = 0x63
    # Pending MeshLocal prefix
    PendingMLprefix = 0x64
    # Pending Master Key
    PendingNwkMasterKey = 0x65
    # Pending Network Name
    PendingNwkName = 0x66
    # Pending Pan ID
    PendingPanId = 0x67
    # Pending PSKc
    PendingPSK = 0x68
    # Pending Security Policy bits
    PendingSecurityPolicy = 0x69
    # Pending Key Rotation Interval [sec]
    PendingNwkKeyRotationInterval = 0x6A
    # Pending Delay Timer [msec]
    PendingDelayTimer = 0x6B
    # Pending Active Timestamp
    PendingActiveTimestamp = 0x6C
    # Pending Timestamp
    PendingTimestamp = 0x6D
    # Commissioner string
    CommissionerId = 0x6E
    # Joiner UDP Port
    JoinerPort = 0x6F
    # Commissioner UDP Port
    CommissionerUdpPort = 0x70
    # The default discovery request Mac Tx options
    DiscoveryReqMacTxOptions = 0x71


class THR_GetAttrConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    Invalidparameter = 0x03
    Notpermitted = 0x04
    UnsupportedAttribute = 0x07
    EmptyEntry = 0x08


class THR_SetAttrConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    Invalidparameter = 0x03
    Notpermitted = 0x04
    UnsupportedAttribute = 0x07
    EmptyEntry = 0x08
    InvalidValue = 0x09


class THR_GetThreadIpAddrConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    InvalidInstance = 0x02
    Error = 0xFF


class THR_GetParentConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    InvalidInstance = 0x02
    Error = 0xFF


class THR_ChildUpdateToParentConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    NotPermitted = 0x04


class THR_LeaderRemoveRouterIdConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_GenerateAllKeysConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_SwitchKeyKeyConfirmStatus(GenericEnum):

    OK = 0x00
    Invalidinstance = 0x02
    Theselectedconfigurationisnotvalid = 0xFF


class THR_BrPrefixAddEntryConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    Notpermitted = 0x04
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_BrPrefixRemoveEntryConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    InvalidParameter = 0x03
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_BrServiceRemoveEntryConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    InvalidParameter = 0x03
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_BrServiceAddEntryConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    InvalidParameter = 0x03
    NotPermitted = 0x04
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_BrPrefixSyncConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    InvalidParameter = 0x03
    Notpermitted = 0x04
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_BrPrefixRemoveAllConfirmStatus(GenericEnum):

    Success = 0x00
    Invalidinstance = 0x02
    InvalidParameter = 0x03
    Notpermitted = 0x04
    Nomemory = 0x06
    Theselectedconfigurationisnotvalid = 0xFF


class THR_IdentifyConfirmStatus(GenericEnum):

    Success = 0x00


class NWKU_IfconfigBindConfirmStatus(GenericEnum):

    OK = 0x00
    Addressesmaximumlimitreached = 0x01
    ERROR = 0xFF


class NWKU_PingConfirmStatus(GenericEnum):

    OK = 0x00
    RequestTimeout = 0x02
    Wrongdestinationaddress = 0x03
    ERROR = 0xFF


class NWKU_EchoUDPConfirmStatus(GenericEnum):

    OK = 0x00
    RequestTimeout = 0x01
    Wrongdestinationaddress = 0x03
    ERROR = 0xFF


class NWKU_CoapMsgReceivedIndicationStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Duplicate = 0x02


class NWKU_CoapMsgReceivedIndicationRequestType(GenericEnum):

    CON = 0x00
    NON = 0x01
    ACK = 0x02


class NWKU_CoapMsgReceivedIndicationMessageType(GenericEnum):

    GET = 0x01
    POST = 0x02
    PUT = 0x03
    DELETE = 0x04
    Success = 0x05


class NWKU_CoapRegisterConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_CoapCreateInstanceConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_CoapSendConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_DnsSendRequestConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_McastGroupManageConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_FirewallEnableConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_FirewallRuleAddConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_FirewallRuleRemoveConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class NWKU_RoutesManageConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class MESHCOP_StartCommissionerConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_StartNativeCommissionerConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_StopCommissionerConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_AddExpectedJoinerConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_GetExpectedJoinerConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_GetExpectedJoinerConfirmSelected(GenericEnum):

    FALSE = 0x00
    TRUE = 0x01


class MESHCOP_RemoveExpectedJoinerConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_RemoveAllExpectedJoinersConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_SyncSteeringDataConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtSetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtGetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtGetConfirmType(GenericEnum):

    # Network Channel
    Channel = 0x00
    # Network Channel Mask
    ChannelMask = 0x35
    # Network Pan ID
    PanId = 0x01
    # Network Extended Pan ID
    XpanId = 0x02
    # Network Name
    NetworkName = 0x03
    # Network PSKc
    PSKc = 0x04
    # Network Master Key
    MasterKey = 0x05
    # Network Key Sequence
    KeySequence = 0x06
    # Network Mesh Local ULA prefix
    MeshLocalUla = 0x07
    # Network Steering Data
    SteeringData = 0x08
    # Network Border Router Locator
    BorderRouterLocator = 0x09
    # Network Commissioner ID
    CommissionerID = 0x0A
    # Network Commissioner Session ID
    CommissionerSessionID = 0x0B
    # Network Security Policy
    SecurityPolicy = 0x0C
    # Delay Timer
    DelayTimer = 0x34
    # Active Commissioner Dataset Timestamp
    ActiveTimestamp = 0x0E
    # Pending Commissioner Dataset Timestamp
    PendingTimestamp = 0x33


class MESHCOP_SetCommissionerCredentialConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmNwkFormConfirmStatus(GenericEnum):

    Success = 0x00
    Notpermitted = 0x04


class MESHCOP_MgmtCommissionerGetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtActiveGetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtPendingGetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtCommissionerSetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtActiveSetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtPendingSetConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtSendPanIdQueryConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtPanIdConflictConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtSendEdScanConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtEdReportConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class MESHCOP_MgmtSendAnnounceBeginConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Error = 0xFF


class DTLSCloseConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class DTLSClosePeerConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class DTLSConnectConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class DTLSClientConnectedConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class DTLSSendConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class Sniffer_MacSetPIBAttributeConfirmStatus(GenericEnum):

    # The PhySetPIBAttribute.Request operation was successful
    gSuccess_c = 0x00
    # Unknown PIB attribute
    UNSUPORTED_ATTRIBUTE = 0xF4
    # Unknown PIB attribute
    gInvalidParameter_c = 0xE8
    # Read Only Attribute
    gReadOnly_c = 0xFB


class Sniffer_MacSetPIBAttributeConfirmPIBAttribute(GenericEnum):

    # The channel to use
    macLogicalChannel = 0x21
    # MAC_PromiscuousRx.Indication (receive all) mode enabled or not
    macMAC_PromiscuousRxIndicationMode = 0x51
    # The receiver is enabled during IDLE periods
    macRxOnWhenIdle = 0x52


class SerialTun_LinkIndicationConfirmStatus(GenericEnum):

    OK = 0x00
    ERROR = 0xFF


class AspSetPowerLevelConfirmStatus(GenericEnum):

    # Request successfully performed
    SUCCESS = 0x00
    # A parameter is invalid or the primitive is not allowed at the moment
    INVALID_PARAMETER = 0xE8


class Sniffer_DetectConfirmSnifferType(GenericEnum):

    USBKW24D = 0x50
    Navajo = 0x51
    NavajoCRC2 = 0x52
    NavajoCRC4 = 0x53
    Apache1 = 0x54
    Apache2 = 0x55
    Apache3 = 0x56
    USBKW41Z = 0x57
