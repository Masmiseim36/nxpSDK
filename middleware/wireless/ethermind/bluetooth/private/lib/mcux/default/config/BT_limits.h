
/**
 *  \file BT_limits.h
 *
 *  This file lists all the Tunable constants used in
 *  EtherMind Bluetooth Stack modules.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_LIMITS_
#define _H_BT_LIMITS_


/* ----------------------------------------------------------------------- */
/* =============================  Common  ================================ */
/* ----------------------------------------------------------------------- */
/*
 *  Almost all the layers (RFCOMM, L2CAP etc.) defines Protocol entities based
 *  on the Remote Device - the Bluetooth Address (BD_ADDR) being the unique way
 *  to identify one concerned Protocol Entity. The following defines the
 *  Maximum number of supported Remote Devices in all the modules except HCI.
 */
#define BT_MAX_REMOTE_DEVICES                           16U

/*
 *  Static Data Size used in EtherMind Modules.
 *  This is a very "sensitive" global definition. Most of the major
 *  logics in most important situations depend on this logic. So, please
 *  be sure before changing this value.
 *
 *  Probably, it is safer to increase this value. But it is a general
 *  risk if this value is decremented anyway.
 *
 *  Some of the modules who heavily depend on it:
 *    - Write Task
 *        + Queue Data Size is of this value.
 *        + For Internal & API Queues, Write Task will free associated
 *          memory if it is more than this value.
 *        + For Data Queues, Write Task does not free any memory, HCI is
 *          supposed to free when it transfered the data.
 *
 *    - HCI
 *        + Entire Zero Copy implementation logic depends on this value.
 *
 *    - SDP, RFCOMM, BNEP, L2CAP
 *        + For Zero Copy Implementation.
 *
 *    - Timer Library
 *        + For user data storage in Timer Queues.
 *
 *  Minimum Value: 32  (Do NOT change it !)
 *  Maximum Value: anything
 */
#ifndef BT_NO_STATIC_DATA_SIZE
#define BT_STATIC_DATA_SIZE                             32U
#endif /* BT_NO_STATIC_DATA_SIZE */

/*
 *  Size of the Bluetooth Link Key in number of octets.
 *  This parameter is NOT configurable.
 *
 *  Minimum Value: 16
 *  Maximum Value: 16
 */
#define BT_LINK_KEY_SIZE                                16U

/*
 *  This parameter defines the upper limit for the size of Bluetooth
 *  PIN Code that will be supported by the concerned modules (such as,
 *  the Security Manager & HCI).
 *
 *  Minimum Value: 1
 *  Maximum Value: 16
 */
#define BT_PIN_CODE_SIZE                                8U

/*
 *  This parameter defines the upper limit for the size of Bluetooth
 *  device name that will be supported by the HCI module when application
 *  calls BT_bluetooth_on() or BT_hci_change_local_name().
 *
 *  Minimum Value: 1
 *  Maximum Value: 248
 */
#define BT_LOCAL_NAME_SIZE                              248U


/* ----------------------------------------------------------------------- */
/* =============================  Timer  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Number of simultaneous Timer Entities that Timer Library will support.
 *
 *  Minimum Value: 1
 *  Maximum Value: anything
 */
#define TIMER_MAX_ENTITIES                              20U


/* ----------------------------------------------------------------------- */
/* =============================  Write Task  ============================ */
/* ----------------------------------------------------------------------- */
/*
 *  The Write Task Queue Sizes for various types of Queues. Number of
 *  element in the User Data Queue must be more, if the application is
 *  expected to write data very frequently. Number of elements in the
 *  User API Queue is a matter of judgement - depends on the application
 *  or profile again - depends on how many simultaneous APIs may be issued
 */
#define WT_INTERNAL_QUEUE_SIZE                          50U
#define WT_USER_API_QUEUE_SIZE                          20U
#define WT_USER_DATA_QUEUE_SIZE                         50U
#define WT_USER_MEDIA_QUEUE_SIZE                        50U

/* ----------------------------------------------------------------------- */
/* =============================  HCI Transport ========================== */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies the maximum size of the HCI Transport's
 *  circular buffer to store incoming HCI packets before they are
 *  delivered to HCI. This value should be sufficiently large enough
 *  to store all incoming HCI packets under a given load condition, till
 *  Read Task (2-task model) or Write Task (1-task model) reads the packets.
 *
 *  This parameter must be define to be a value which is 2 raised
 *  to the power of 'some' N. Example: 16, 32, 64, 128, 256, 512, 1024 etc.
 *
 *  Minimum Value: depends on requirement.
 *  Maximum Value: depends on requirement.
 */
#define HCI_TRANSPORT_MAX_BUFFER_SIZE                   8192U

#if (((HCI_TRANSPORT_MAX_BUFFER_SIZE) == 0)        \
    ||(((HCI_TRANSPORT_MAX_BUFFER_SIZE) &          \
       ((HCI_TRANSPORT_MAX_BUFFER_SIZE) - 1)) != 0))
    #error "HCI_TRANSPORT_MAX_BUFFER_SIZE value is not in 2^n format"
#endif /* HCI_TRANSPORT_MAX_BUFFER_SIZE */

/*
 *  The HCI Transport Read Task (or, the Write Task) reads one complete
 *  HCI packet from the circular Transport buffer into another static
 *  buffer before HCI's callback for incoming data reception is invoked.
 *  This parameter specifies the size of this static data buffer which
 *  is used to deliver data to HCI. The size of this buffer should be
 *  chosen such a way so that it can accomodate any HCI packet under any
 *  condition.
 *
 *  For a Classic or DualMode controller with EDR, the maximum possible HCI
 *  payload can be 1021 bytes for ACL data packet (as the Max packet size
 *  over air using 3-DH5 packet is 1021 bytes). In addition, 4 bytes of
 *  HCI header and 1 byte of packet type will have to be accommodated.
 *  Hence the maximum packet size required to be supported would be
 *  (1021 + 4 + 1) = 1026 bytes
 *
 *  Minimum Value: depends on the host controller.
 *  Maximum Value: depends on the host controller.
 */
#define HCI_TRANSPORT_MAX_PACKET_SIZE                   1026U

/*
 *  Low & High Water Marks for the HCI Transport's circular buffer.
 *
 *  If the amount of free space in the circular buffer becomes less than
 *  or equal to 'HCI_TRANSPORT_THRESHOLD_LOW', HCI Transport Driver's
 *  'Throttle' routine is invoked to signal that incoming data reception
 *  should be throttled at this point.
 *
 *  If the amount of data queued in the circular buffer becomes less than
 *  or equal to 'HCI_TRANSPORT_THRESHOLD_HIGH', HCI Transport Driver's
 *  'Unthrottle' routine is invoked to signal that incoming data reception
 *  should be unthrottled at this point.
 *
 *  Minimum Value: depends on requirement.
 *  Maximum Value: depends on requirement.
 */
#define HCI_TRANSPORT_BUFFER_THRESHOLD_LOW              512U
#define HCI_TRANSPORT_BUFFER_THRESHOLD_HIGH             128U


/* ----------------------------------------------------------------------- */
/* =============================  UART  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Size of the buffer the HCI-UART module uses to recombine fragments of
 *  a HCI ACL Data or HCI Command packet before the same is written to the
 *  underlying driver for physical transport to the Bluetooth chip.
 *
 *  Minimum Value: 260 or (1+4+ Bluetooth Controller's Max ACL Buffer Size),
 *                 whichever is more.
 *  Maximum Value: depends on requirement.
 */
#define HCI_UART_WR_BUF_SIZE                            2048U


/* ----------------------------------------------------------------------- */
/* =============================  BCSP  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum BCSP Packet Payload Length. This value is mainly driven by the
 *  values returned by the Host Controller in response to Read Buffer Size
 *  command, for the ACL & SCO Buffer size. This parameter must have a
 *  value larger than the largest of the Host Controller reported buffer
 *  sizes for ACL & SCO packets.
 */
#define BCSP_MAX_PAYLOAD_LENGTH                         4096U

/*
 *  BCSP SLIP Layer transmits packet with SLIP byte stuffing algorithm.
 *  There are two approaches to do this:
 *    1. Each byte is SLIP encoded and written to UART separately.
 *       Result -> Lots of UART write. For every byte UART write API will
 *                 be called once.
 *    2. Each byte is encoded and put into a buffer. When this buffer is
 *       almost full, UART write is called to flush the buffer, and proceed
 *       with further encoding.
 *       Result -> Number of UART write is reduced drastically compared
 *                 to the previous scenario. Faster and efficient.
 *
 *  BCSP adopted the 2nd method, and this parameter defines the size of
 *  the buffer used to store the encoded bytes temporarily before flushing
 *  out on the UART.
 *
 *  Performance can be best optimized if the size of this buffer is twice
 *  the maximum size that SLIP encoding routine can be asked to write, which
 *  in turn depends on the Host Controller's buffer sizes.
 */
#define BCSP_SLIP_TX_SIZE                               1500U

/*
 *   BCSP Link Establishment Timeout values and counters. There are two
 *   timers involved in the BCSP Link establishment - Tshy & Tconf. And
 *   one counter - Number of times CONF message has been retransmitted.
 */
#define BCSP_T_SHY_TIMEOUT                              1U
#define BCSP_T_CONF_TIMEOUT                             1U
#define BCSP_MAX_CONF_TIMEOUT_LIMIT                     20U

/*
 *  BCSP Sequencing Layer provides reliable data transfer with the help
 *  packet sequencing and retransmission. To enable possible retransmission
 *  of a transmitted packet, BCSP needs to store them in its queue. This
 *  parameter defines the number of elements in this queue.
 *
 *  This number can be tuned on the basis of number of ACL or SCO
 *  buffers that the Host Controller has. Since HCI is bound to do
 *  flow control on the basis of the number of buffers that Host Controller
 *  has, BCSP need not maintain a very large queue here. A good value
 *  would be (number of HC buffers + 10), assuming HCI, in the meantime,
 *  will not send more than 10 commands.
 */
#define BCSP_MAX_SEQUENCE_TX_BUFFERS                    100U

/*
 *  BCSP Sequencing - Packet Retransmission Timeout and Retransmission Limit.
 */
#define BCSP_SEQUENCE_RTX_TIMEOUT                       1U
#define BCSP_SEQUENCE_RTX_LIMIT                         20U

/*
 *  BCSP Sequencing - Transmit Window Size
 */
#define BCSP_WINDOW_SIZE                                4U


/* ----------------------------------------------------------------------- */
/* =============================  HCI  =================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of connected Remote Devices maintained by HCI.
 */
#define HCI_MAX_REMOTE_DEVICES                          BT_MAX_REMOTE_DEVICES

/*
 *  HCI stores the outgoing commands to the Host Controller in a Global
 *  Command Queue. Transmission of the commands from this queue is
 *  depended upon the flow control of commands between the Host Controller
 *  and the Host (HCI). This parameter specifies the size of the HCI
 *  Command Queue.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define HCI_COMMAND_QUEUE_SIZE                          20U

/*
 *  HCI stores the outgoing ISO Data to the Host Controller in a Global
 *  Data Queue. Transmission of the ISO Data from this queue is
 *  depended upon the flow control of commands between the Host Controller
 *  and the Host (HCI). This parameter specifies the size of the HCI
 *  ISO Data Queue.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define HCI_ISO_DATA_QUEUE_SIZE                         40U

/*
 *  Length of the buffer packet used to enqueue HCI Events/Data received
 *  during the process of Private Address Resolution of a peer device
 *  address that has connected. Apart from the HCI payload, packet type
 *  and length are also stored.
 *
 *  This constant will be used only when HOST_RESOLVE_PVT_ADDR is defined
 *
 *  Minimum Value: (31+1+1)
 *  Maximum Value: can be anything >= 'Minimum Value'.
 */
#define HCI_RPA_BUFFER_LEN                              (31U + 1U + 1U)

/*
 *  Number of buffers of size HCI_RPA_BUFFER_LEN to be used as queue
 *  during the process of Address Resolution
 *
 *  This constant will be used only when HOST_RESOLVE_PVT_ADDR is defined
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define HCI_NUM_RPA_BUFFERS                             4U

/*
 *  Maximum number of supported SCO Channels per Remote Bluetooth Device.
 *  In HCI, each Remote Device Enity declares a number of Connection Entities,
 *  of which one is dedicated for ACL Channel, and the rest for SCO. The total
 *  number of such entities is derived by adding '1' (for ACL Channel) to this
 *  value.
 *
 *  Minimum: 1
 *  Maximum: 3
 */
#define HCI_MAX_SCO_CHANNELS                            3U

#define HCI_MAX_DEVICE_ATTRIBUTES                       HCI_MAX_REMOTE_DEVICES

#ifdef HCI_LE_EXT_ADV_SETS_LOCAL_ADDR_SUPPORT
/*
 *  Maximum number of Extended Advertising Sets supported by Host stack.This
 *  typically needs to match the value of Advertising Sets supported by the
 *  underlying Bluetooth LE controller.
 *
 *  Minimum: 1
 *  Maximum: can be anything.
 */
#define HCI_MAX_SUPPORTED_EXT_ADV_SETS                  4U
#endif /* HCI_LE_EXT_ADV_SETS_LOCAL_ADDR_SUPPORT */

#ifdef HCI_LE_CIG_CIS_SUPPORT
/*
 *  Maximum number of CIS Connections to be supported by local device.
 *  This is agreegate of all the CIS connections with multiple Remote Bluetooth Devices.
 *
 *  Minimum: 1
 *  Maximum: Any Number
 */
#define HCI_MAX_CIS_CONNECTIONS                         4U
#endif /* HCI_LE_CIG_CIS_SUPPORT */

#ifdef HCI_LE_BIG_BIS_SUPPORT
/*
 *  Maximum number of BIS Streams to be supported by local device.
 *  This can be a maximum of 31 as per the LL specification.
 *
 *  Minimum: 1
 *  Maximum: 31
 */
#define HCI_MAX_BIS_STREAMS                             4U
#endif /* HCI_LE_BIG_BIS_SUPPORT */

/* ----------------------------------------------------------------------- */
/* ========================  Device Queue  =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  This constant defines the number of devicess in the Device Queue can
 *  have at most. Since, Security Manager should at least support those
 *  devices which the stack itself connects to, it must at least be equal
 *  to BT_MAX_REMOTE_DEVICES.
 *
 *  Moreover, in general, size of the Device Queue is defined to be
 *  "BT_MAX_REMOTE_DEVICES + X". Of these, the "BT_MAX_REMOTE_DEVICES"
 *  number of entries must always be available to store incoming and
 *  outgoing ACL/LE connections. The rest, "X", can be used to store inactive
 *  (no active ACL/LE connection at present) devices which are paired,
 *  trusted etc.
 *
 *  Minimum Value: BT_MAX_REMOTE_DEVICES
 *  Maximum Value: depends on requirement.
 */
#define BT_MAX_DEVICE_QUEUE_SIZE                        (SM_MAX_DEVICES + 1U)


/* ----------------------------------------------------------------------- */
/* ==========================  FSM Engine  =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  This value specifies the maximum number of modules to be supported
 *  by the stack FSM engine.
 *
 *  Minimum Value: 1
 *  Maximum Value: depends on requirement.
 */
#define FSM_MAX_MODULES                                 5U


/* ----------------------------------------------------------------------- */
/* =============================  Security Manager  ====================== */
/* ----------------------------------------------------------------------- */
/*
 *  This constant defines the number of elements Security Manager's
 *  Service Database can have at most.
 *  If any of the services is based on RFCOMM, keep one additional service
 *  element for RFCOMM itself.
 *  If more than one services are based on RFCOMM, then also keep only one
 *  service element for RFCOMM.
 *
 *  Minimum Value: 1
 *  Maximum Value: depends on requirement.
 */
#define SM_MAX_SERVICES                                 10U

/*
 *  This constant defines the number of elements Security Manager's
 *  Device Database can have at most. Since, Security Manager should
 *  at least support those devices which the stack itself connects to,
 *  it must at least be equal to BT_MAX_REMOTE_DEVICES.
 *
 *  Moreover, in general, size of the SM's Device Database is defined
 *  to be "BT_MAX_REMOTE_DEVICES + X". Of these, the "BT_MAX_REMOTE_DEVICES"
 *  number of entries must always be available to store incoming and
 *  outgoing ACL connections. The rest, "X", can be used to store inactive
 *  (no active ACL connection at present) devices which are paired, trusted
 *  etc.
 *
 *  Minimum Value: BT_MAX_REMOTE_DEVICES
 *  Maximum Value: depends on requirement.
 */
#define SM_MAX_DEVICES                  (BT_MAX_REMOTE_DEVICES + 10U)

/*
 *  This constant defines the number of elements Security Manager's
 *  table for Security Access Contexts - created during Security
 *  Access Requests from L2CAP & RFCOMM.
 *
 *  Minimum Value: 1
 *  Maximum Value: depends on requirement.
 */
#define SM_MAX_CONTEXTS                                 10U

/*
 *  This constant defines the Default Security Mode that the Security
 *  Manager will be initialized with.
 *
 *  However, this may not be the mode in which Security Manager will
 *  continue to run. During Bluetooth-ON initialization Security
 *  Manager may read the Persistent Storage for default settings
 *  and finally use those if found.
 *
 *  Valid values:
 *  - SM_SECURITY_MODE_1
 *  - SM_SECURITY_MODE_2
 *  - SM_SECURITY_MODE_3
 */
#define SM_DEFAULT_SECURITY_MODE                        SM_SECURITY_MODE_1

/*
 *  This constant defines the Default Encryption Mode that the Security
 *  Manager will be initialized with.
 *
 *  However, this may not be the mode in which Security Manager will
 *  continue to run. During Bluetooth-ON initialization Security
 *  Manager may read the Persistent Storage for default settings
 *  and finally use those if found.
 *
 *  Valid values:
 *  - 0x00, 0x01
 */
#define SM_DEFAULT_ENCRYPTION_MODE                      0x00U

/*
 *  Default PIN Code for Security Manager Module.
 *  This PIN Code is used when SM is configured for Automatic Mode
 *  (no UI Notification Callback is registered), and PIN Code Request
 *  is received from the local LMP/Baseband.
 *
 *  Minimum Value: 0 (PIN Code would be "")
 *  Maximum Value: BT_PIN_CODE_SIZE
 */
#define SM_DEFAULT_PIN_CODE                             "1234"
#define SM_DEFAULT_PIN_CODE_LENGTH                      4U

#if (SM_DEFAULT_PIN_CODE_LENGTH > BT_PIN_CODE_SIZE)
    #error "SM_DEFAULT_PIN_CODE_LENGTH value Out-of-Range"
#endif /* SM_DEFAULT_PIN_CODE_LENGTH > BT_PIN_CODE_SIZE */

/*
 *  This parameter defines the upper limit for the size of Bluetooth
 *  device name that will be supported by the SM module when application
 *  calls BT_sm_set_device_name()/BT_sm_get_device_name().
 *
 *  Minimum Value: 1
 *  Maximum Value: 16
 */
#define SM_DEVICE_NAME_SIZE                             16U

/*
 *  This constant defines the symbolic name of the device used for
 *  persistent storage of Security Manager settings and list of
 *  trusted & paired devices.
 */
#define SM_STORAGE_MEDIA                                "btsm.db"

/*
 *  This constant defines the minimum encryption key size requirement
 *  for an encrypted link, the authentication requirement of which is
 *  less than level 4. For an authentication requirement of Level 4
 *  or when the device is in SecureConnection Only mode, this value
 *  is by default set to 16.
 *
 *  Minimum Value: As required
 *  Maximum Value: 16
 */
#define SM_MIN_ENCRYPTION_KEY_SIZE                      7U

/* ----------------------------------------------------------------------- */
/* =======================  Attribute Protocol  ========================== */
/* ----------------------------------------------------------------------- */

/**
 * ATT_RESPONSE_TIMEOUT
 *
 * ATT Response-Response TimeOut Value. Currently the value of the timer is set
 * to 30 sec in the specification. Can be changed for testing purposes only.
 *
 * Minimum Value: 30 Seconds.
 * Maximum Value: 30 Seconds.
 */
#define ATT_RESPONSE_TIMEOUT                             30U

/**
 * ATT_MAX_CONNECTION_INSTANCES
 *
 * Maximum Instances supported by ATT. This is the maximum number of
 * simultanoeus Transactions that the module can have.
 *
 * Minimum Value:   1.
 * Maximum Value: 255.
 */
/* #define ATT_MAX_CONNECTION_INSTANCES                      (1U + 5U) */
#define ATT_MAX_CONNECTION_INSTANCES                      16U

/**
 * ATT_MAX_MTU
 *
 * Maximum MTU supported by ATT on an Instance. ATT Specification mandates
 * the minimum MTU as 23. Hence the default value used would be 23.
 *
 * This size is inclusive of the 1 byte ATT Opcode, 2 bytes ATT Handle,
 * and the value following it.
 *
 * Minimum Value:  23
 * Maximum Value: L2CAP_MAX_MTU for Classic Link. As per requirement for LE Link
 */
#define ATT_MAX_MTU                                    1024U

/**
 * ATT_L2CA_RECV_CREDIT_LWM
 *
 * Low Watermark value for the ATT credit given to peer device.
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define ATT_L2CA_RECV_CREDIT_LWM                       1

/* ----------------------------------------------------------------------- */
/* ===================  Generic Attribute Protocol  ====================== */
/* ----------------------------------------------------------------------- */

/**
 * GATT_DB_DYN_MAX_SERVICE_COUNT
 *
 * Maximum Number of GATT Services.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_MAX_SERVICE_COUNT                  20U

/**
 * GATT_DB_DYN_CHARACTERISTIC_COUNT
 *
 * Maximum Number of GATT Characteristics.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_CHARACTERISTIC_COUNT               40U

/**
 * GATT_DB_DYN_MAX_ATTRIBUTES
 *
 * Maximum Number of GATT Attributes.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_MAX_ATTRIBUTES                     80U

/**
 * GATT_DB_DYN_MAX_TYPE_COUNT
 *
 * Maximum Number of GATT attribute types or UUIDs.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_MAX_TYPE_COUNT                     80U

/**
 * GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE
 *
 * Maximum Peer specific characteristic or characteristic descriptor
 * attribute value array size for a single instance of peer device.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE              160U

/**
 * GATT_DB_DYN_VALUE_ARRAY_SIZE
 *
 * Maximum Characteristic attribute value array size, which are not
 * peer specific, but fixed/constant.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_VALUE_ARRAY_SIZE                   512U

/**
 * GATT_DB_DYN_UUID_ARRAY_SIZE
 *
 * Maximum Flattened list size of UUID values (both 16 and 128 bit).
 *
 * Minimum Value: 2
 * Maximum Value: anything
 */
#define GATT_DB_DYN_UUID_ARRAY_SIZE                    512U

/**
 * GATT_DB_DYN_MAX_PEER_CONFIGURATION
 *
 * 'config_GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE' times maximum number of
 *  supported peer devices 'BT_MAX_DEVICE_QUEUE_SIZE'.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_DB_DYN_MAX_PEER_CONFIGURATION        \
        (GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

#ifdef GATT_DB_DYNAMIC
/**
 * GATT_VALUE_ARRAY_SIZE
 *
 * Maximum Characteristic attribute value array size, which are not
 * peer specific, but can be changed.
 *
 * Minimum Value: 1
 * Maximum Value: anything
 */
#define GATT_VALUE_ARRAY_SIZE                          256U
#endif /* GATT_DB_DYNAMIC */


/* ----------------------------------------------------------------------- */
/* ====================  Security Manager Protocol  ====================== */
/* ----------------------------------------------------------------------- */

/*
 *  This constant defines the number of elements Security Manager's
 *  Device Database can have at most. Since, Security Manager should
 *  at least support those devices which the stack itself connects to,
 *  it must at least be equal to BT_MAX_REMOTE_DEVICES.
 *
 *  Moreover, in general, size of the SM's Device Database is defined
 *  to be "BT_MAX_REMOTE_DEVICES + X". Of these, the "BT_MAX_REMOTE_DEVICES"
 *  number of entries must always be available to store incoming and
 *  outgoing ACL connections. The rest, "X", can be used to store inactive
 *  (no active ACL connection at present) devices which are paired, trusted
 *  etc.
 *
 *  Minimum Value: BT_MAX_REMOTE_DEVICES + 1
 *  Maximum Value: depends on requirement.
 */
#define SMP_MAX_DEVICES                                (BT_MAX_REMOTE_DEVICES + 1U)

/*
 *  This constant defines the number of elements Security Manager's
 *  table for Security Access Contexts.
 *
 *  Minimum Value: 1
 *  Maximum Value: depends on requirement.
 */
#define SMP_MAX_CONTEXTS                                2U

/*
 *  This constant defines the Security Manager pairing procedure timeout
 *  values in seconds.
 *
 *  Minimum Value: depends on requirement
 *  Maximum Value: 30
 */
#define SMP_MAX_AUTHENTICATION_TIMEOUT                  30U


/* ----------------------------------------------------------------------- */
/* =============================  L2CAP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of higher layer protocols using L2CAP's services. PSM
 *  stands for Protocol Service Multiplexer.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define L2CAP_MAX_PSM                                   10U

/*
 *  Maximum number of connection-oriented logical channels that L2CAP needs
 *  to support. Normally, for each PSM there might be one L2CAP channel. But
 *  there could also be more than one L2CAP channel for a PSM & a BD_ADDR.
 *
 *  Minimum Value: 1
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_CE                                    10U

/*
 *  Maximum number of connection-less channels (groups) that L2CAP needs
 *  to support.
 *
 *  Minimum Value: 1
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_GROUPS_CE                             1U

/*
 *  In L2CAP, pre-allocated static buffers are used for transmission of
 *  L2CAP Command packets. The following defines the size of that buffer.
 *
 *  Minimum Value: 52
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_PACKET_LENGTH                         52U

/*
 *  There could be multiple outstanding commands for an L2CAP Connection
 *  Entity for a Remote Bluetooth Device, for which Responses are yet to
 *  be received. L2CAP Command Elements are maintained for this purpose
 *  to match received Response with the transmitted Command.
 *
 *  Minimum Value: 2
 *  Maximum Value: 255
 */
#define L2CAP_MAX_COMMANDS                              8U

/*
 *  Maximum MTU size that L2CAP will allow advertising to the peer.
 *
 *  Minimum Value: 48
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_MTU                                   1800U

/*
 *  The timeout period, in seconds, within which configuration for a
 *  connection-oriented L2CAP channel must conclude.
 *
 *  Minimum Value: 30
 *  Maximum Value: 120
 */
#define L2CAP_DEFAULT_CONFIG_TIMEOUT                    60U

/*
 *  Pool of Flow Specifications used for non-default settings
 *  of the same during Configuration of Data channels
 *
 *  Minimum Value: 3
 *  Maximum Value: 254
 */
#define L2CAP_MAX_FLOW_SPEC_BLOCKS                      10U

/*
 *  Maximum number of L2CAP Buffers. This is maximum number of upper
 *  layer data write allowed at any point of time over L2CAP.
 *
 *  This constant is meaningful only if new L2CAP/HCI (1.2 Stack)
 *  is being used.
 *
 *  Minimum Value: 1
 *  Maximum Value: 254
 */
#define L2CAP_MAX_BUFFER_ELEMENTS                       50U

/*
 *  L2CAP employs a Flow Control mechanism in which it is possible to
 *  provide feedback to application when number of available buffers in
 *  L2CAP's ACL Transmission Queue goes too low, or, too high. L2CAP
 *  provides this feedback using the Global ACL Transmission Queue Flow
 *  Control Callback, that can be registered by application after
 *  Bluetooth-ON.
 *
 *  When it is too low (ie, the number of free buffers reaches or goes below
 *  the Low Water Mark, LWM), the application can take necessary steps to stop
 *  making "data transmission requests" using various Stack/Profile APIs,
 *  for some time. L2CAP calls the registered callback again when number
 *  of available buffers reaches a High Water Mark (HWM) - at this point,
 *  application can take necessary steps to restart data transmission.
 *
 *  Typically, the LWM is set to ~10% and ~90% of the total number of
 *  buffers (ie, L2CAP_MAX_BUFFER_ELEMENTS), respectively. However, this
 *  should be fine tuned for the platform/system for various data throughput
 *  tests & observations.
 *
 *  Minimum Value: 1 (LWM), 2 (HWM)
 *  Maximum Value: 252 (LWM), 253 (HWM)
 */
#define L2CAP_NUM_BUFFER_ELEMENTS_LWM                   5U
#define L2CAP_NUM_BUFFER_ELEMENTS_HWM                   45U

/*
 *  Maximum number of credit based logical channels that L2CAP needs
 *  to support. Normally, for each PSM there might be one L2CAP channel. But
 *  there could also be more than one L2CAP channel for a PSM.
 *
 *  The number of Channels here is not inclusive of 3 fixed channels ATT,
 *  L2CAP and SMP.
 *  So, if the use case requires all 3 fixed channels i.e. ATT, L2CAP & SMP
 *  and 2 L2CAP CBFC channels and 3 L2CAP ECBFC channels in addition to those then the value of
 *  L2CAP_MAX_CHANNELS should be  5 (=2+3).
 *
 *  Minimum Value: 1
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_CHANNELS                              10U

/*
 *  Maximum number of credit based Flow Control PSM that L2CAP needs
 *  to support.
 *
 *  Minimum Value: 1
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_CBFC_PSM                              2U

/*
 *  After receiving L2CAP data/payload from peer device for a CBFC Channel,
 *  the available credits are calculated. If the credit count goes below this
 *  (low water mark) L2CAP_CBFC_RX_CREDITS_LWM, the application is notified of
 *  of this condition through the callback registered, so that if application
 *  wants can send more credits to the peer device.
 *
 *  Minimum Value: 0
 *  Maximum Value: Less than allowed maximum value L2CAP_MAX_CREDITS (0xFFFF)
 */
#define L2CAP_CBFC_RX_CREDITS_LWM                       0x0001U

/*
 *  Maximum number of enhanced credit based Flow Control PSM that L2CAP needs
 *  to support.
 *
 *  Minimum Value: 1
 *  Maximum Value: as required.
 */
#define L2CAP_MAX_ECBFC_PSM                             2U

 /*
 *  After receiving L2CAP data/payload from peer device for a ECBFC Channel,
 *  the available credits are calculated. If the credit count goes below this
 *  (low water mark) L2CAP_ECBFC_RX_CREDITS_LWM, the application is notified of
 *  of this condition through the callback registered, so that if application
 *  wants can send more credits to the peer device.
 *
 *  Minimum Value: 0
 *  Maximum Value: Less than allowed maximum value L2CAP_MAX_CREDITS (0xFFFF)
 */
#define L2CAP_ECBFC_RX_CREDITS_LWM                      0x0001U

/*
 *  A L2CAP Channel in 1.2 Stack can either be configured in Basic (1.1)
 *  mode, or, one of the two defined FEC modes (retransmission or flow
 *  control). In either case, FEC related parameters are stored in a
 *  different table, other than L2CAP Connection Entity (L2CAP_CE). The
 *  L2CAP_CE now contains a index which points to the FEC Block, if the
 *  concerned channel is configured for one of the FEC mode.
 *
 *  This parameter configures the size of FEC Blocks (table). If it is
 *  desirable that all L2CAP CEs can be configured to FEC mode, value
 *  of this parameter should be set to Maximum number of L2CAP channels.
 *
 *  Minimum Value: 1
 *  Maximum Value: 254 or L2CAP_MAX_CE (whichever is less)
 */
#define L2CAP_MAX_FEC_ENTITIES                          L2CAP_MAX_CE

/**
 *  Timeout (in seconds) for the RTX Timer which is run at the time of
 *  transmitting L2CAP_InfoREQ when ACL Connection is up for a remote
 *  Bluetooth device (BT-1.2 only).
 *
 *  Minimum Value: 1
 *  Maximum Value: 60
 */
#define L2CAP_FEATURE_INFO_REQ_TIMEOUT                  1U

/**
 *  Max Retransmission Count for the L2CAP_InfoREQ which is transmitted
 *  when ACL Connection is up for a remote Bluetooth device (BT-1.2 only).
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define L2CAP_FEATURE_INFO_REQ_RTX_MAX                  1U

/**
 *  Maximum number of I-frames to store in Enhanced L2CAP.
 *
 *  Minimum Value: as required
 *  Maximum Value: 254
 */
#define L2CAP_MAX_I_FRAMES_RX                           50U

/*
 *  Maximum MPS size that L2CAP will allow advertising to the peer,
 *  when configuring a channel in ERTM or SM. Since MPS also cannot
 *  be more than configured MTU, it becomes logical that maximum value
 *  of MPS would also be limited by maximum value of MTU.
 *
 *  Minimum Value: 1
 *  Maximum Value: 65529
 */
#define L2CAP_MAX_MPS                                   L2CAP_MAX_MTU

/*
 *  Default value of the Retransmission Timer in Milliseconds for an L2CAP
 *  channel in Enhanced Retransmission Mode, and when Flush TO is not
 *  configured for the ACL link.
 *
 *  Minimum Value: 2000
 *  Maximum Value: as required
 */
#define L2CAP_FEC_DEFAULT_RETX_TO                       2000U

/*
 *  Default value of the Monitor Timer in Milliseconds for an L2CAP
 *  channel in Enhanced Retransmission Mode, and when Flush TO is not
 *  configured for the ACL link.
 *
 *  Minimum Value: 12000
 *  Maximum Value: as required
 */
#define L2CAP_FEC_DEFAULT_MONITOR_TO                    12000U


/* ----------------------------------------------------------------------- */
/* =============================  RFCOMM  ================================ */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of Remote Devices (= RFCOMM Sessions) supported.
 *
 *  Minimum Value: 1
 *  Maximum Value: BT_MAX_REMOTE_DEVICES
 */
#define RFCOMM_MAX_REMOTE_DEVICES                       BT_MAX_REMOTE_DEVICES

/*
 *  Maximum L2CAP InMTU size for RFCOMM.
 *
 *  Minimum Value: 48
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define RFCOMM_L2CAP_DEFAULT_MTU                        672U

#if ((RFCOMM_L2CAP_DEFAULT_MTU < 48) \
    || (RFCOMM_L2CAP_DEFAULT_MTU > L2CAP_MAX_MTU))
    #error "RFCOMM_L2CAP_DEFAULT_MTU value Out-of-Range"
#endif /* RFCOMM_L2CAP_DEFAULT_MTU */

/*
 *  Maximum number of RFCOMM User Contexts. One RFCOMM User Context is
 *  for every RFCOMM API (that requires a BH to be enqueued) that Application
 *  calls.
 *
 *  Minimum Value: 2
 *  Maximum Value: 255
 */
#ifdef SPP_ENABLE_MAX_CONN_RANGE
#define RFCOMM_MAX_USER_CONTEXTS                        30U
#else
#define RFCOMM_MAX_USER_CONTEXTS                        10U
#endif
/*
 *  The following value specifies the supported number of simultaneous
 *  RFCOMM connections. These connections can be between two devices or
 *  multiple devices. It is the aggregate of all the RFCOMM connections,
 *  not connection per device.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#ifdef SPP_ENABLE_MAX_CONN_RANGE
#define RFCOMM_MAX_CONNECTION                           30U
#else
#define RFCOMM_MAX_CONNECTION                           10U
#endif
/*
 *  When Credit Based Flow Control is in use, PN Frame is used to convey that
 *  Local RFCOMM supports the same, along with initial value for Local
 *  Credits. This Parameter defines the Number of Credit that this RFCOMM
 *  will advertise to its Peer.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define RFCOMM_NUM_CREDITS_PER_DLCI                     15U

/*
 *  Low Water Mark decider for transmission of Local Credit to Peer.
 *  For a DLCI, RFCOMM will trigger transmission of more Local Credit
 *  to the peer RFCOMM entity, once Local Credit less than or equal to
 *  this value.
 *
 *  Minimum Value: 2
 *  Maximum Value: RFCOMM_NUM_CREDITS_PER_DLCI - 2
 */
#define RFCOMM_CBFC_LOW_WATER_MARK \
        (RFCOMM_NUM_CREDITS_PER_DLCI - 5U)

/*
 *  Buffer Pointers from User in RFCOMM Write API are stored in the
 *  RFCOMM Transmit Queue. This parameter defines the size of RFCOMM
 *  Transmit Queue.
 *
 *  Minimum Value: (Number of DLCIs being Supported)
 *  Maximum Value: 65534 (0xFFFE)
 */
#define RFCOMM_TRANSMIT_QUEUE_SIZE                      30U

/*
 *  All the RFCOMM connections may not configure the RPN values other than
 *  Default values. RFCOMM maintain a list of RFCOMM PORT PARAMETERS to
 *  store the RPN values which mismatch with the Default values. This
 *  parameter configure the maximum number of PORT PARAMS Entities.
 *
 *  Minimum Value: 1
 *  Maximum Value: Number of supported DLCIs (RFCOMM_MAX_CONNECTION)
 */
#define RFCOMM_NUM_PORT_PARAMS                          2U

/*
 *  The TxWindow value to use when RFCOMM is enabled to use the Enhanced
 *  Retransmission Mode of L2CAP. It should be made as large as possible
 *  to maximize channel utilization.
 *
 *  Minimum Value: 1
 *  Maximum Value: 63
 */
#define RFCOMM_ERTM_TX_WINDOW                           63U

/*
 *  The MaxTransmit value to use when RFCOMM is enabled to use the Enhanced
 *  Retransmission Mode of L2CAP. Lower values might be appropriate for
 *  services requiring low latency. Higher values will be suitable for a
 *  link requiring robust operation.
 *
 *  A value of 1 means that no retransmissions will be made but also means
 *  that the channel will be disconnected as soon as a packet is lost.
 *
 *  A value of zero means infinite retransmissions.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255 or 0 (0 = infinite retransmissions)
 */
#define RFCOMM_ERTM_MAX_TRANSMIT                        10U

/*
 *  The maximum L2CAP PDU size to use when RFCOMM is enabled to use the
 *  Enhanced Retransmission Mode of L2CAP.
 *
 *  Minimum Value: 1
 *  Maximum Value: RFCOMM_L2CAP_DEFAULT_MTU
 */
#define RFCOMM_ERTM_MPS \
        RFCOMM_L2CAP_DEFAULT_MTU


/* ----------------------------------------------------------------------- */
/* =============================  SDP Common  ============================ */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum L2CAP MTU size for the SDP. This value determines the
 *  Memory allocated for the SDP PDU packets.
 *
 *  Minimum Value: 40  (Sufficient for Attribute serach of Protocol list)
 *  Maximum Value: Depends upon the Memory constraints
 *
 *  The Maximum Dynamic memory allocated for the SDP packet =
 *  SDP_L2CAP_DEFAULT_MTU + L2CAP_HDR_LEN (4) + 17 (Continaution State Info)
 */
#define SDP_L2CAP_DEFAULT_MTU                           672U

#if ((SDP_L2CAP_DEFAULT_MTU < 48) || (SDP_L2CAP_DEFAULT_MTU > L2CAP_MAX_MTU))
    #error "SDP_L2CAP_DEFAULT_MTU value Out-of-Range"
#endif /* SDP_L2CAP_DEFAULT_MTU */


/* ----------------------------------------------------------------------- */
/* =============================  SDP Client  ============================ */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of allowed Attribute IDS and Attribute Ranges in a
 *  Single Request
 *
 *  Minimum Value: 0
 *  Maximum Value:  (SDP_MAX_ATTR_ID * 3) +
 *                  (SDP_MAX_ATTR_RANGE * 5) < (SDP_DEFAULT_L2CAP_MTU - 33)
 *
 */
#define SDP_MAX_ATTR_ID                                 10U
#define SDP_MAX_ATTR_RANGE                              5U

/*
 *  The maximum number of simultaneously allowed client connections
 *
 *  Minimum Value: 1
 *  Maximum Value: BT_MAX_REMOTE_DEVICES
 */
#define SDP_MAX_NUM_APIS                                BT_MAX_REMOTE_DEVICES

/*
 *  Maximum Number of UUIDS SDP Client can request in the service
 *  serach or service search attribute transactions.If the client
 *  requested more than the MAX_CLIENT_NUMBER_OF_UUIDS client returns
 *  failure.
 *
 *  Minimum Value: 1
 *  Maximum Value: 12 (As per Bluetooth Specification v1.1)
 */
#define SDP_CLIENT_MAX_NUM_UUIDS                        12U


/* ----------------------------------------------------------------------- */
/* =============================  SDP Server  ============================ */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of Record Handles SDP Server can get even if the client
 *  ask for more. SDP Request Handler maintains a local variable of array
 *  of MAX_REC_HANDLES * 4 to get the possible number of Handles from the
 *  database.
 *  This define is not used if 'SDP_SERVER_HAVE_NO_MAX_REC_DEPENDENCY'
 *  feature flag is defined.
 *
 *  Minimum Value: The number of Record Handles the Server device has
 *  Maximum Value (Default): Depends on stack memory
 */
#define SDP_MAX_REC_HANDLES                             8U

/*
 *  Maximum Number of UUIDS SDP server can extract from a single request
 *  to process. If the client requested more than the MAX_NUMBER_OF_UUIDS
 *  Server return with an error indicates INSUFFICIENT RESOURCES. SDP
 *  Request Handler has a local variable of array of MAX_NUM_UUIDS * 16
 *  To extract all UUIDS from the request
 *
 *  Minimum Value: 1
 *  Maximum Value: 12 (As per Bluetooth Specification v1.1)
 */
#define SDP_SERVER_MAX_NUM_UUIDS                        12U

/*
 *  The maximum number of active client connections the server will
 *  support simultaneously
 *
 *  Minimum Value: 1
 *  Maximum Value: 254 (Recommented value is BT_MAX_REMOTE_DEVICES)
 */
#define SDP_MAX_SERVER_CONNECTIONS                      BT_MAX_REMOTE_DEVICES


/* ----------------------------------------------------------------------- */
/* =============================  Database  ============================== */
/* ----------------------------------------------------------------------- */
/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of UUID can be used used to search
 *  a database record.
 *
 *  Minimum Value:  1
 *  Maximum Value:  N/A
 */
#define DB_MAX_UUID_SERVICE_SEARCH                      (12U)

#ifdef SDP_DYNAMIC_DB
/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of records in the SDP database.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_RECORDS                                  (32U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of attributes in any of the SDP database record.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_REC_ATTR                                 (25U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of UUIDs in any of the SDP database record.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_REC_UUID                                 (10U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of UUIDs in the SDP database.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_DYN_UUIDS                                (100U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum length of any attribure in the SDP database.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_ATTR_LEN                                 (400U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of indicies of Service Class UUIDs in the SDP database.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_SERVICE_CLASS_UUID_INDICES               (100U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of elements in a protocol description list
 *  in any of the SDP database record.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_LIST_ELEMS                               (5U)

/*
 *  The following value is used in the sdp database interface.
 *  This is maximum number of parameters in any elements in a protocol description list
 *  in any of the SDP database record.
 *
 *  Minimum Value:  1
 *  Maximum Value:  depends on application requirement
 */
#define DB_MAX_PROTOCOL_PARAMS                          (5U)
#endif /* SDP_DYNAMIC_DB */


/* ----------------------------------------------------------------------- */
/* =============================  BNEP  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of simultaneous BNEP connections.
 *
 *  Minimum Value:  1
 *  Maximum Value:  254
 */
#define BNEP_MAX_ENTITIES                               3U


/*
 *  Maximum number of Network Type & Multicast Address filters that is
 *  supported for all BNEP connections. At a minimum, BNEP must support
 *  at least one filter per connection.
 *
 *  Minimum Value:  BNEP_MAX_ENTITIES
 *  Maximum Value:  As required
 */
#define BNEP_MAX_NW_TYPE_FILTERS                        10U
#define BNEP_MAX_MC_ADDR_FILTERS                        10U


/* ----------------------------------------------------------------------- */
/* =============================  AVDTP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of Remote Devices (= Signalling Channels) supported.
 *
 *  Minimum Value: 1
 *  Maximum Value: BT_MAX_REMOTE_DEVICES
 */
#define AVDTP_MAX_REMOTE_DEVICES                        BT_MAX_REMOTE_DEVICES

/*
 *  Maximum number of Stream End Point that can be registered to AVDTP.
 *  This is also the maximum number of simultaneous Stream Context that
 *  can be created in AVDTP.
 *
 *  Minimum Value: 1
 *  Maximum Value: 62
 */
#define AVDTP_MAX_STREAM_CONTEXTS                       3U

/*
 *  Maximum number of Signaling Commands that can be outstanding at any
 *  point of time for all Signaling channels to all remote AVDTP entities.
 *
 *  Minimum Value: 1
 *  Maximum Value: 16
 */
#define AVDTP_MAX_USER_CONTEXTS                         5U

/*
 *  Maximum number of Logical L2CAP channels supported by AVDTP at any point
 *  time (including those for all signaling and transport channels).
 *
 *  Minimum Value: 2
 *  Maximum Value: AVDTP_MAX_STREAM_CONTEXTS * 4
 */
#define AVDTP_MAX_LOGICAL_CHANNELS      AVDTP_MAX_STREAM_CONTEXTS * 4U

/*
 *  Maximum number of Transport Sessions supported by AVDTP at any point
 *  time.
 *
 *  Minimum Value: 1
 *  Maximum Value: AVDTP_MAX_STREAM_CONTEXTS * 3
 */
#define AVDTP_MAX_TRANSPORT_SESSIONS    AVDTP_MAX_STREAM_CONTEXTS * 3U

/*
 *  Maximum number of bytes in packed Codec capability information
 *  element.
 *
 *  Note:
 *  For SBC or MPEG-1,2 (MP3), Codec IE length of 4 octet is required.
 *  For MPEG-2,4 (AAC), Codec IE length should be configured as 6.
 *
 *  For Vendor Specific Codec Type, IE length should be configured as per Codec Info.
 *  and it will always be greater than 6 bytes.
 *  For aptX codec, IE length should be configured as 8.
 *
 *  For any other codec, please refer to the Codec specific documentation.
 *
 *  Minimum Value: 4
 *  Maximum Value: depends on type of codec
 */
#define AVDTP_MAX_CODEC_IE_LEN                          7U

/*
 *  Maximum number of bytes in packed Content Protection capability
 *  information element.
 *
 *  Minimum Value: 4
 *  Maximum Value: depends on type of content protection
 */
#define AVDTP_MAX_CP_IE_LEN                             4U

/*
 *  Maximum number of Content Protection methods supported by AVDTP.
 *
 *  Minimum Value: 1
 *  Maximum Value: depends on type of content protection
 */
#define AVDTP_MAX_CP_IE                                 1U
/*
 *  Maximum number of Media Transport Sessions (TSIDs) that can be
 *  multiplexed on a Transport Channel (TCID).
 *
 *  Minimum Value: 2
 *  Maximum Value: 3
 */
#define AVDTP_MAX_NUM_MEDIA_MUX                         2U

/*
 *  Maximum number of Reporting Transport Sessions (TSIDs) that can be
 *  multiplexed on a Transport Channel (TCID).
 *
 *  Minimum Value: 2
 *  Maximum Value: 3
 */
#define AVDTP_MAX_NUM_REPORTING_MUX                     2U

/*
 *  Maximum number of Recovery Transport Sessions (TSIDs) that can be
 *  multiplexed on a Transport Channel (TCID).
 *
 *  Minimum Value: 2
 *  Maximum Value: 3
 */
#define AVDTP_MAX_NUM_RECOVERY_MUX                      2U

/*
 *  Maximum L2CAP InMTU size for AVDTP Signalling Channel.
 *
 *  Though a value of 48 octets would suffice, but some Bluetooth Stereo
 *  Headset products are known to expect L2CAP InMTU for AVDTP Signalling
 *  Channel as 672.
 *
 *  The A2DP/VDP specifies a minimum of 335 for L2CAP MTU.
 *
 *  Minimum Value: 335
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define AVDTP_DEFAULT_SIG_L2CAP_MTU                     672U

#if ((AVDTP_DEFAULT_SIG_L2CAP_MTU < 335) \
    || (AVDTP_DEFAULT_SIG_L2CAP_MTU > L2CAP_MAX_MTU))
    #error "AVDTP_DEFAULT_SIG_L2CAP_MTU value Out-of-Range"
#endif /* AVDTP_DEFAULT_SIG_L2CAP_MTU */

/*
 *  Maximum L2CAP InMTU size for AVDTP Media Transport Channel.
 *  This constant will be used if the AVDTP SEP is a Source of
 *  Media Stream. Since Source do not expect data on the incoming
 *  channel, it should be set to minimum L2CAP MTU.
 *
 *  The A2DP/VDP specifies a minimum of 335 for L2CAP MTU.
 *
 *  Minimum Value: 335
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SOURCE        335U

#if ((AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SOURCE < 335) \
    || (AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SOURCE > L2CAP_MAX_MTU))
    #error "AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SOURCE value Out-of-Range"
#endif /* AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SOURCE */

/*
 *  Maximum L2CAP InMTU size for AVDTP Media Transport Channel.
 *  This constant will be used if the AVDTP SEP is a Sink of
 *  Media Stream.
 *
 *  Though a value of 339 octets would suffice, but some Bluetooth Stereo
 *  Headset products are known to expect L2CAP InMTU for AVDTP Transport
 *  Channel as 672.
 *
 *  The A2DP/VDP specifies a minimum of 335 for L2CAP MTU.
 *
 *  Minimum Value: 335
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SINK          620U

#if ((AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SINK < 335) \
    || (AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SINK > L2CAP_MAX_MTU))
    #error "AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SINK value Out-of-Range"
#endif /* AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_SINK */

/*
 *  Maximum L2CAP InMTU size for AVDTP Reporting Transport Channel.
 *
 *  The A2DP/VDP specifies a minimum of 335 for L2CAP MTU.
 *
 *  Minimum Value: 335
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_REPORTING     335U

#if ((AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_REPORTING < 335) \
    || (AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_REPORTING > L2CAP_MAX_MTU))
    #error "AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_REPORTING value Out-of-Range"
#endif /* AVDTP_DEFAULT_TRANSPORT_L2CAP_MTU_REPORTING */

/*
 *  Discover, Get Capabilities and Get Configuration need
 *  User Area Length of minimum 2 bytes.
 *
 *  Minimum Value: 2
 *  Maximum Value: As required
 */
#define AVDTP_MIN_UAREA_LEN                             2U


/* ----------------------------------------------------------------------- */
/* =============================  AVCTP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Value of Maximum Number of Profiles running over AVCTP
 *
 *  Minimum Value: 1
 *  Maximum Value: It depends upon the maximum number of profiles using AVCTP.
 *  With support for AVRCP as only profile, the maximum number is also 1.
 */
#define AVCTP_MAX_PROFILES                              1U

/*
 *  Maximum Number of Remote AVCTP Entities supported.
 *
 *  Minimum Value: 1
 *  Maximum Value: BT_MAX_REMOTE_DEVICES
 */
#define AVCTP_MAX_REMOTE_DEVICES                        BT_MAX_REMOTE_DEVICES

/*
 *  Maximum Number of Remote AVCTP Signalling channels.
 *
 *  Minimum Value: 1  -> For only Control Channel
 *                 2  -> For Both Control and Browsing Channels Support
 *
 *  Maximum Value:
 *         (AVCTP_MAX_REMOTE_DEVICES * 1) -> For only Control Channel
 *         (AVCTP_MAX_REMOTE_DEVICES * 2) -> For Both Control and Browsing
 *                                           Channels Support
 *
 */
#define AVCTP_MAX_SIG_CHANNELS                     (AVCTP_MAX_REMOTE_DEVICES * 2U)

/*
 *  Maximum L2CAP InMTU size for AVCTP Signaling Channel.
 *
 *  Minimum Value: 48
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define AVCTP_DEFAULT_L2CAP_MTU                         48U

#if ((AVCTP_DEFAULT_L2CAP_MTU < 48) \
    || (AVCTP_DEFAULT_L2CAP_MTU > L2CAP_MAX_MTU))
    #error "AVCTP_DEFAULT_L2CAP_MTU value Out-of-Range"
#endif /* AVCTP_DEFAULT_L2CAP_MTU */

/*
 *  Maximum L2CAP InMTU size for AVCTP Browsing Channel.
 *
 *  Minimum Value: 335
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define AVCTP_DEFAULT_BROW_CH_L2CAP_MTU                  335U

#if ((AVCTP_DEFAULT_BROW_CH_L2CAP_MTU < 335) \
    || (AVCTP_DEFAULT_BROW_CH_L2CAP_MTU > L2CAP_MAX_MTU))
    #error "AVCTP_DEFAULT_BROW_CH_L2CAP_MTU value Out-of-Range"
#endif /* AVCTP_DEFAULT_BROW_CH_L2CAP_MTU */

/*
 *  Default value of the Max Transmit for AVCTP Browsing Channel in
 *  Enhanced Retransmission Mode.
 *
 *  From AVRCP 1.4 Specification:
 *  The value for MaxTransmit should be set to be infinite.
 *  This ensures consistency between different profiles that are running,
 *  by using the baseband timeout to determine if the link is insufficiently
 *  reliable.
 *
 *  Minimum Value: 1
 *  Maximum Value: 0xFF and Infinite (0)
 */
#define AVCTP_BROWSING_CH_DEFAULT_MAX_TRANSMIT          0xFFU

/*
 *  Default value of the Retransmission Timer in Milliseconds for AVCTP
 *  Browsing Channel in Enhanced Retransmission Mode, and when Flush TO is not
 *  configured for the ACL link.
 *
 *  From AVRCP 1.4 Specification:
 *  Retransmission time-out should be set to a value in the range of
 *  300 - 2000 ms for typical devices. It is desirable that this timer be
 *  set to a low value to reduce latency, but not so low that unnecessary
 *  retransmissions occur.
 *
 *  Minimum Value: 300
 *  Maximum Value: 2000
 */
#define AVCTP_BROWSING_CH_DEFAULT_RETX_TO               2000U

/*
 *  Default value of the Monitor Timer in Milliseconds for AVCTP Browsing
 *  channel in Enhanced Retransmission Mode, and when Flush TO is not
 *  configured for the ACL link.
 *
 *  From AVRCP 1.4 Specification:
 *  Monitor time-out should be set to a value in the range of 300 - 2000 ms
 *  for typical devices. It is desirable that this timer be set to a low
 *  value to reduce latency, but not so low that unnecessary retransmissions
 *  occur.
 *
 *  Minimum Value: 300
 *  Maximum Value: 2000
 */
#define AVCTP_BROWSING_CH_DEFAULT_MONITOR_TO            2000U

/*
 *  Default value of the Tx Window for AVCTP Browsing Channel in
 *  Enhanced Retransmission Mode.
 *
 *  Minimum Value: 1
 *  Maximum Value: 63
 */
#define AVCTP_BROWSING_CH_DEFAULT_TX_WINDOW             30U


/* ----------------------------------------------------------------------- */
/* =============================  OBEX  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of OBEX instances. This value defines the number of
 *  OBEX Sessions that OBEX can support at a particular time.
 *
 *  Minimum Value: 1
 *  Maximum Value: 10
 */
#define OBEX_MAX_INSTANCES                              4U

/*
 *  This is the maximum L2CAP payload that OBEX can receive when
 *  run directly over L2CAP as transport
 *
 *  Minimum Value: 255
 *  Maximum Value: L2CAP_MAX_MTU
 */
#define OBEX_L2CAP_DEFAULT_MTU                          L2CAP_MAX_MTU

/*
 *  Transmission window for Enhanced Retransmission Mode
 *  operation of L2CAP. This mode is mandatory when OBEX is running
 *  over L2CAP transport
 *
 *  Minimum Value: 1
 *  Maximum Value:
 */
#define OBEX_ERTM_TX_WINDOW                             63U

/*
 *  Maximum transmit parameter for Enhanced Retransmission Mode
 *  when OBEX is run over L2CAP transport
 *
 *  Minimum Value: 1
 *  Maximum Value:
 */
#define OBEX_ERTM_MAX_TRANSMIT                          32U

/*
 *  Maximum Payload size that can be received over L2CAP transport
 *  operating in Enhanced Retransmission Mode
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_L2CAP_DEFAULT_MTU
 */
#define OBEX_ERTM_MPS                                   OBEX_L2CAP_DEFAULT_MTU

/*
 *  Maximum size of OBEX Packet during Transmission & Reception.
 *
 *  Minimum Value: 255
 *  Maximum Value: (OBEX_L2CAP_DEFAULT_MTU - 10)
 *
 *  Note:
 *  Reduced by '10' octets to account for RFCOMM Protocol Header which resides
 *  between L2CAP & OBEX Layers.
 */
#define OBEX_MAX_PACKET_LENGTH                          (OBEX_L2CAP_DEFAULT_MTU - 10U) /* 1536 */

#if (OBEX_MAX_PACKET_LENGTH > OBEX_L2CAP_DEFAULT_MTU)
    #error "OBEX_MAX_PACKET_LENGTH value Out-of-Range"
#endif /* OBEX_DEFAULT_PACKET_LENGTH */

/*
 *  Default size of OBEX Packet if the connect operation doesn't
 *  happen before a PUT/GET operation, or the paket size wasn't set during the
 *  connect operation.
 *
 *  Minimum Value: 255
 *  Maximum Value: 4096
 */
#define OBEX_DEFAULT_PACKET_LENGTH                      512U

#if ((OBEX_DEFAULT_PACKET_LENGTH < 255) \
    || (OBEX_DEFAULT_PACKET_LENGTH > OBEX_MAX_PACKET_LENGTH))
    #error "OBEX_DEFAULT_PACKET_LENGTH value Out-of-Range"
#endif /* OBEX_DEFAULT_PACKET_LENGTH */

/*
 *  Maximum size of OBEX Objects used for a single packet
 *
 *  Minimum Value: 1
 *  Maximum Value: Depends on the OBEX_MAX_PACKET_LENGTH
 */
#define OBEX_MAX_OBJECTS                                12U

/*
 *  Size of Nonce Length to be used for local device
 *
 *  Minimum Value: 4
 *  Maximum Value: 16
 */
#define OBEX_RELIABLE_SSN_NONCE_LEN                     4U


/* ----------------------------------------------------------------------- */
/* =============================  MCAP  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of profile instances running on MCAP.
 *
 *  Minimum value: 1
 *  Maximum value: 255
 */
#define MCAP_MAX_MEP_INSTANCES                          2U

/*
 *  Maximum number of control channels MCAP can manage.
 *
 *  Minimum value: 1
 *  Maximum value: 65535
 */
#define MCAP_MAX_CONTROL_CHANNELS                       2U

/*
 *  Maximum number of data channels MCAP can manage.
 *
 *  Minimum value: 1
 *  Maximum value: Sum of MCAP_MAX_CONTROL_CHANNELS & MCAP_MAX_DATA_CHANNELS
 *                 must be less than 65535
 */
#define MCAP_MAX_DATA_CHANNELS                          2U

/*
 * MCAP_RESPONSE_TIMEOUT
 *
 * MCAP Response - Response TimeOut Value.
 *
 * Minimum Value : 30 Seconds.
 * Maximum Value : 30 Seconds.
 */
#define MCAP_RESPONSE_TIMEOUT                           30U


/* ----------------------------------------------------------------------- */
/* =============================  OPP  =================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of OPP Clients. This Value should not be greater
 *  than the number of OBEX_MAX_INSTANCES
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define OPP_NUM_CLIENT_ENTITIES                                 1U


/*
 *  Maximum number of OPP Server.
 *
 *  Minimum Value: 1
 *  Maximum Value: ???
 */
#define OPP_NUM_SERVER_ENTITIES                                 3U


/* ----------------------------------------------------------------------- */
/* =============================  FTP  =================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of FTP Clients. This Value should not be greater
 *  than the number of OBEX_MAX_INSTANCES
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define FTP_NUM_CLIENT_ENTITIES                             1U

/*
 *  Maximum number of FTP Server.
 *
 *  Minimum Value: 1
 *  Maximum Value: ???
 */
#define FTP_NUM_SERVER_ENTITIES                             1U


/* ----------------------------------------------------------------------- */
/* =============================  BIP  =================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of BIP Initiator Primary Entities.
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define BIP_NUM_INITIATOR_PRI_ENTITIES                      1U

/*
 *  Maximum Number of BIP Initiator Secondary Entities.
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define BIP_NUM_INITIATOR_SEC_ENTITIES                      1U

/*
 *  Maximum Number of BIP Responder Primary Entities
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define BIP_NUM_RESPONDER_PRI_ENTITIES                      1U

/*
 *  Maximum Number of BIP Responder Secondary Entities
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define BIP_NUM_RESPONDER_SEC_ENTITIES                      1U


/* ----------------------------------------------------------------------- */
/* =============================  BPP  =================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of BPP Printer  Entities.
 *
 *  Minimum Value: 1
 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define BPP_NUM_PRINTER_ENTITIES                      1U

/*
 *  Maximum Number of BIP Sender Entities.
 *
 *  Minimum Value: 1

 *  Maximum Value: OBEX_MAX_INSTANCES
 */
#define BPP_NUM_SENDER_ENTITIES                      1U

/* ----------------------------------------------------------------------- */
/* =============================  XML Parser Encoder  ==================== */
/* ----------------------------------------------------------------------- */
/*
 *  The maximum size of document that the xml parser will parse.
 *  This limits the text size that could be sent to the parser for decoding
 *  The current max value has been set based on the current requirements of
 *  profiles such as ftp and bpp.
 */
#define XML_MAX_DOC_SIZE                                2000U

/*
 *  This  is the maximum no. of attributes any element of the xml string
 *  can have. Based on this value an aarray of structures is declared to
 *  hold the attribute name and its value
 */
#define XML_MAX_NO_OF_ATTRIBUTES                        5U

/*
 *  This is the maximum number of levels of element nesting allowed
 *  within each element of the xml string sent to the parser
 */
#define XML_MAX_SUB_ELEMENTS                            200U

/*
 *  This the Maximum length of each xml token that can be parsed by the
 *  parser. This also has been defined based on current requirements and
 *  can be tuned to lower values after proper analysis and testing of the
 *  profiles using the parser
 */
#define XML_MAX_TOKEN_SIZE                              500U

/*
 *  This is the maximum number of commands that can be embedded in an
 *  xml document to be parsed. These commands are returned to the appl
 *  calling the parser as an array of this size
 */
#define XML_MAX_COMMANDS                                2U


/* ----------------------------------------------------------------------- */
/* =============================  HCRP  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum size of the buffer used by the HCRP for data reception; This same
 *  value will be used as the  Maximum credit limit.
 *  Minimum Value: 1200 (HCRP_DEFAULT_MTU_SIZE)
 *  maximum Value: Depends on the Memory constraints
 */
#define HCRP_MAX_BUFFER_SIZE                            3000U

/*
 *  Maximum number of allowed connections per device
 *  Minimum Value: 1
 *  Maximum Value: 8
 */
#define HCRP_MAX_REMOTE_CONNECTION                      2U


/* ----------------------------------------------------------------------- */
/* =============================  HID Host  ============================== */
/* ----------------------------------------------------------------------- */
#define BT_HID_HOST_INQUIRY_PAUSE_LENGTH                30U
#define BT_HID_HOST_INQUIRY_LENGTH                      10U
#define BT_HID_HOST_NUM_INQ_RESPS                       10U

/**
 *  Maximum number of devices that the HID host supports simultaneously.
 *  Minimum Value: 1
 *  Maximum Value: Depends on the Memory constraints
 */
#define BT_HID_HOST_MAX_DEVS                            5U

/*
 *  This is maximum length of report accepted by the HID Host.
 */
#define BT_HID_HOST_MAX_REPORT_LEN                      25U


/* ----------------------------------------------------------------------- */
/* =============================  A2DP  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of Codecs that can be registered to A2DP.
 *
 *  Minimum Value: 1
 *  Maximum Value: 62
 */
#define A2DP_MAX_CODECS                                 3U

/*
 *  Maximum buffer size required to store AVDTP Discover Response in each
 *  A2DP Codec Entity.
 *
 *  Minimum Value: 2
 *  Maximum Value: 124
 */
#define A2DP_MAX_DISCOVER_RESPONSE_LEN                  40U

/*
 *  Maximum buffer size required to store AVDTP Get Capabilities Response.
 *
 *  Minimum Value: 10
 *  Maximum Value: As required.
 */
#define A2DP_GET_CAPABILITIES_RESPONSE_LEN              40U


/* ----------------------------------------------------------------------- */
/* =============================  AVRCP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Value of Maximum Number of Profiles Instances Running for AVRCP
 *
 *  Minimum Value: 1
 *  Maximum Value: AVCTP_MAX_REMOTE_DEVICES.
 */
#define AVRCP_MAX_PROFILES                              AVCTP_MAX_REMOTE_DEVICES

/*
 *  Maximum number of AVRCP Control Commands that can be outstanding at any
 *  point of time for all remote devices.
 *
 *  Minimum Value: 1
 *  Maximum Value: 16
 */
#define AVRCP_MAX_USER_CONTEXTS                         3U

/*
 *  AVRCP Command Timeout in number of seconds
 *
 *  Minimum Value: 1
 *  Maximum Value: as desired
 */
#define AVRCP_COMMAND_TIMEOUT_VALUE                     10U


/* ----------------------------------------------------------------------- */
/* =============================  AVRCP Cover Art ======================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum Number of Cover Art Initiator Instances
 *
 *  Minimum Value: 1
 *  Maximum Value: AVRCP_MAX_PROFILES.
 */
#define AVRCP_CAI_NUM_ENTITIES                               1U

/*
 *  Maximum Number of Cover Art Responder Instances
 *
 *  Minimum Value: 1
 *  Maximum Value: AVRCP_MAX_PROFILES.
 */
#define AVRCP_CAR_NUM_ENTITIES                               1U


/* ----------------------------------------------------------------------- */
/* =============================  HFP Unit  ============================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of simultaneous connections supported by HFP_UNIT.
 *
 *  Minimum Value: 1
 *  Maximum Value: RFCOMM_MAX_CONNECTION
 */
#define HFP_UNIT_MAX_CONNECTIONS                        1U

/*
 *  Maximum size of AT Command.
 *  Note: DTMF data is copied from location 9 onwards.
 *
 *  Minimum Value: 32
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_COMMAND_LENGTH                     48

/*
 *  Default timeout values in seconds for protecting HFP Unit
 *  implementation against non-responsive peer for BRSF Command.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required.
 */
#define HFP_UNIT_BRSF_RSP_TIMEOUT                       1U

/*
 *  Default timeout values in seconds for protecting HFP Unit
 *  implementation against non-responsive peer for Commands sent
 *  during SLC establishment, other than BRSF.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required.
 */
#define HFP_UNIT_SLC_PRC_TIMEOUT                        5U

/*
 *  Maximum number of octets used to represent the supported features.
 *
 *  Minimum Value: 4
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_SUPP_FEATURE_LEN                   6U

/*
 *  Maximum number of CIND indicators that can be stored locally, upon
 *  receiving the corresponding response from HFP Audio Gateway.
 *
 *  Minimum Value: 3
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_INDICATOR_COUNT                    20U


/*
 *  Maximum number of HF indicators that can be stored locally, upon
 *  receiving the corresponding response from HFP Audio Gateway.
 *
 *  Minimum Value: 1
 *  Maximum Value: 20.
 */
#define HFP_UNIT_MAX_HF_INDICATOR_COUNT                 2U

/*
 *  Size of array to store phone number digits sent or received
 *  from HFP Audio Gateway.
 *
 *  Minimum Value: 13
 *  Maximum Value: As required.
 */
#define HFP_UNIT_DIGIT_ARRAY_SIZE                       32U

/*
 *  Size of the Operator name that can be stored by the HFP Unit,
 *  upon receiving the same from HFP Audio Gateway in AT+COPS response.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_OPERATOR_NAME_LEN                  16U

/*
 *  The Maximum number of Call hold Types supported by HFP Audio Gateway.
 *
 *  Minimum Value: 20
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_HOLD_TYPES                         20U

/*
 *  The Maximum number of available Codecs.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_NUM_AVL_CODECS                     10U

/*
 *  The Maximum length of string in Voice Recognition Text Feature.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required.
 */
#define HFP_UNIT_MAX_ENH_VREC_TEXT_LEN                   64U


/* ----------------------------------------------------------------------- */
/* ===========================  HSP Unit ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of supported HSP Unit Entities
 *
 *  Minimum Value: 1
 *  Maximum Value: RFCOMM_MAX_CONNECTION
 */
#define HSP_UNIT_MAX_ENTITY                             1U


/* ----------------------------------------------------------------------- */
/* =============================  SPP ==================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of supported SPP Entities
 *
 *  Minimum Value: 1
 *  Maximum Value: RFCOMM_MAX_CONNECTION
 */
#ifdef SPP_ENABLE_MAX_CONN_RANGE
#define SPP_MAX_ENTITY                                  21U
#else
#define SPP_MAX_ENTITY                                  2U
#endif

/* ----------------------------------------------------------------------- */
/* =============================  DUNP DT ================================ */
/* ----------------------------------------------------------------------- */
#define DUNP_DT_MAX_AT_COMMAND_LENGTH                   20U
#define DUNP_DT_MAX_MODEM_RSP_LEN                       20U
#define DUNP_DT_MAX_SDP_DATA_LEN                        50U
#define DUNP_DT_MAX_RFCOMM_DATA                         10U


/* ----------------------------------------------------------------------- */
/* =============================  DUNP GW ================================ */
/* ----------------------------------------------------------------------- */

/*
 *  Value of the maximum number of DUNP DTs that can connect
 *  to the DUNP GW simultaneously
 *
 *  Minimum Value: 1
 *  Maximum Value: as required (to be tuned based on requirements)
 */
#define DUNP_GW_MAX_DT_CONNECTED                        1U


/* ----------------------------------------------------------------------- */
/* ==============================  BPP  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of BPP Sender entities
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define BPP_SENDER_MAX_ENTITY                           1U

/*
 *  Maximum number of BPP Printer entities
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define BPP_PRINTER_MAX_ENTITY                          1U

/*
 *  Number of channels in BPP
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define BPP_MAX_CHANNELS                                4U


/* ----------------------------------------------------------------------- */
/* ==============================  PBAP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of PBAP PCE entities
 *
 *  Minimum Value: 1
 *  Maximum Value: BT_MAX_REMOTE_DEVICES
 */
#define PBAP_PCE_MAX_ENTITY                             1U

/*
 *  Maximum number of PBAP PSE entities
 *
 *  Minimum Value: 1
 *  Maximum Value: BT_MAX_REMOTE_DEVICES
 */
#define PBAP_PSE_MAX_ENTITY                             1U

/*
 *  Maximum Idle State Time Out Value in Seconds
 *
 *  Minimum Value: 1
 *  Maximum Value: 65535
 */
#define PBAP_IDLE_TIMEOUT                               120U


/* ----------------------------------------------------------------------- */
/* ===============================  SAP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of SAP Parameters
 *
 *  Minimum Value: 1
 *  Maximum Value: As required.
 */
#define SAP_MAX_NO_OF_PARAMS                            2U

/*
 *  Maximum L2CAP InMTU size for SAP.
 *
 *  Minimum Value: 48
 *  Maximum Value: As required.
 */
#define SAP_MAX_MTU                                     512U


/* ----------------------------------------------------------------------- */
/* ===============================  MAP  ================================= */
/* ----------------------------------------------------------------------- */
/*
 * Maximum number of MAP Client Entities. Defines the number of distinct MAP
 * server's that can be connected.
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define MAP_MCE_NUM_ENTITIES                            2U

/*
 * Maximum number of MAP Server Entities. Defines the number of distinct MAP
 * clients that can be connected
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define MAP_MSE_NUM_ENTITIES                            2U

/*
 * Maximum number of Message Access Service (MAS) Instances per MAP Client/Server
 * entity.
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define MAP_NUM_MAS_INSTANCE                            2U

/*
 * Number of Message Access Service (MAS) Entities in Pool.
 *
 * Minimum Value: Larger of (MAP_MCE_NUM_ENTITIES * MAP_NUM_MAS_INSTANCE)/
 *                (MAP_MSE_NUM_ENTITIES * MAP_NUM_MAS_INSTANCE)
 * Maximum Value: As required
 */
#define MAP_MAS_NUM_ENTITIES                            2U

/*
 * Number of Message Notification Service (MNS) Entities in Pool
 *
 * Minimum Value: Larger of MAP_MCE_NUM_ENTITIES/MAP_MSE_NUM_ENTITIES
 * Maximum Value: As required
 */
#define MAP_MNS_NUM_ENTITIES                            1U


/* ----------------------------------------------------------------------- */
/* ==============================  GNSS  ================================= */
/* ----------------------------------------------------------------------- */
/*
 * Maximum number of GNSS Client Entities. Defines the number of distinct GNSS
 * server's that can be connected
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define GNSS_CLIENT_MAX_CONNECTION_ENTITIES             1U

/*
 * Maximum number of GNSS Server Entities. Defines the number of distinct GNSS
 * clients that can be connected.
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define GNSS_SERVER_MAX_CONNECTION_ENTITIES             1U


/* ----------------------------------------------------------------------- */
/* ===============================  CTN  ================================= */
/* ----------------------------------------------------------------------- */
/*
 * Maximum number of CTN Client Entities. Defines the number of distinct CTN
 * server's that can be connected.
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define CTN_CCE_NUM_ENTITIES                            1U

/*
 * Maximum number of CTN Server Entities. Defines the number of distinct CTN
 * clients that can be connected
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define CTN_CSE_NUM_ENTITIES                            2U

/*
 * Maximum number of CTN Access Service (CAS) Instances per CTN Client/Server
 * entity.
 *
 * Minimum Value: 1
 * Maximum Value: As required
 */
#define CTN_NUM_CAS_INSTANCE                            2U

/*
 * Number of CTN Access Service (CAS) Entities in Pool.
 *
 * Minimum Value: Larger of (CTN_CCE_NUM_ENTITIES * CTN_NUM_CAS_INSTANCE)/
 *                (CTN_CSE_NUM_ENTITIES * CTN_NUM_CAS_INSTANCE)
 * Maximum Value: As required
 */
#define CTN_CAS_NUM_ENTITIES                            2U

/*
 * Number of CTN Notification Service (CNS) Entities in Pool
 *
 * Minimum Value: Larger of CTN_CCE_NUM_ENTITIES/CTN_CSE_NUM_ENTITIES
 * Maximum Value: As required
 */
#define CTN_CNS_NUM_ENTITIES                            1U


/* ----------------------------------------------------------------------- */
/* ===========================  AT Parser  =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  Size of the internal buffer to store & parse the commands/responses
 *  received from HSP/HFP Audio Gateway.
 *
 *  Minimum Value: 60
 *  Maximum Value: As required.
 */
#define AT_PARSER_PARTIAL_RSP_BUF_SIZE                  60U


/* ----------------------------------------------------------------------- */
/* ==============================  JPL  ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Amount of PCM data, in number of Octets to be provided to DAC, each
 *  time it asks for new data.
 *
 *  This parameter must be integral multiple of Block Size calculated.
 *
 *  Minimum Value: Block Size
 *  Maximum Value: As required
 */
#define JPL_PCM_BLOCK_SIZE                              8192U

/*
 *  Number of PCM data blocks to be used for PCM frame buffering.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define JPL_PCM_NUM_BLOCKS                              6U

/*
 *  Maximum amount of data, in number of octets, for any SBC frame that
 *  JPL is expected to handle & buffer.
 *
 *  Minimum Value: Does not matter
 *  Maximum Value: 255
 */
#define JPL_SBC_FRAME_SIZE                              120U

/*
 *  Number of SBC frame blocks to be used for SBC frame buffering.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define JPL_SBC_NUM_FRAMES                              100U

/*
 *  Initial Number of Data Read IND Callback to call to load the DAC
 *  with sufficient data.
 *
 *  Minimum Value: 1
 *  Maximum Value: (JPL_PCM_NUM_BLOCKS - 1)
 */
#define JPL_INITIAL_NUM_DATA_READ_IND                   5U


/* ----------------------------------------------------------------------- */
/* ===========================  RACP ===================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Maximum number of modules which can register with RACP.
 *
 *  Minimum Value: 1
 *  Maximum Value: 255
 */
#define RACP_MAX_MODULES                                5U

#endif /* _H_BT_LIMITS_ */

