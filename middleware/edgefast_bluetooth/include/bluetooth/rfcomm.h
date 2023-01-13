/** @file
 *  @brief Bluetooth RFCOMM handling
 */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_RFCOMM_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_RFCOMM_H_

/**
 * @brief RFCOMM
 * @defgroup bt_rfcomm RFCOMM
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/buf.h>
#include <bluetooth/conn.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RFCOMM channels (1-30): pre-allocated for profiles to avoid conflicts */
enum {
	BT_RFCOMM_CHAN_HFP_HF = 1,
	BT_RFCOMM_CHAN_HFP_AG,
	BT_RFCOMM_CHAN_HSP_AG,
	BT_RFCOMM_CHAN_HSP_HS,
	BT_RFCOMM_CHAN_SPP,
};

struct bt_rfcomm_dlc;

/** @brief RFCOMM DLC operations structure. */
struct bt_rfcomm_dlc_ops {
	/** DLC connected callback
	 *
	 *  If this callback is provided it will be called whenever the
	 *  connection completes.
	 *
	 *  @param dlc The dlc that has been connected
	 */
	void (*connected)(struct bt_rfcomm_dlc *dlc);

	/** DLC disconnected callback
	 *
	 *  If this callback is provided it will be called whenever the
	 *  dlc is disconnected, including when a connection gets
	 *  rejected or cancelled (both incoming and outgoing)
	 *
	 *  @param dlc The dlc that has been Disconnected
	 */
	void (*disconnected)(struct bt_rfcomm_dlc *dlc);

	/** DLC recv callback
	 *
	 *  @param dlc The dlc receiving data.
	 *  @param buf Buffer containing incoming data.
	 */
	void (*recv)(struct bt_rfcomm_dlc *dlc, struct net_buf *buf);

    /** DLC sent callback
     *
     *  @param dlc The dlc receiving data.
     *  @param buf Buffer containing sending data.
     */
    void (*sent)(struct bt_rfcomm_dlc *dlc, struct net_buf *buf);
};

/** @brief Role of RFCOMM session and dlc. Used only by internal APIs
 */
ENUM_PACKED_PRE
enum bt_rfcomm_role {
	BT_RFCOMM_ROLE_ACCEPTOR,
	BT_RFCOMM_ROLE_INITIATOR
} ENUM_PACKED_POST;
typedef enum bt_rfcomm_role bt_rfcomm_role_t;

/** @brief RFCOMM DLC structure. */
struct bt_rfcomm_dlc {
	/* Response Timeout eXpired (RTX) timer */
	struct k_work_delayable    rtx_work;

	/* Queue for outgoing data */
	struct k_fifo              tx_queue;

	/* TX credits, Reuse as a binary sem for MSC FC if CFC is not enabled */
	struct k_sem               tx_credits;

	struct bt_rfcomm_session  *session;
	struct bt_rfcomm_dlc_ops  *ops;
	struct bt_rfcomm_dlc      *_next;

	bt_security_t              required_sec_level;
	bt_rfcomm_role_t           role;

	uint16_t                      mtu;
	uint8_t                       dlci;
	uint8_t                       state;
	uint8_t                       rx_credit;

	/* Stack & kernel data for TX thread */
	struct k_thread            tx_thread;
	K_KERNEL_STACK_MEMBER(stack, 256);
};

struct bt_rfcomm_server {
	/** Server Channel */
	uint8_t channel;

	/** Server accept callback
	 *
	 *  This callback is called whenever a new incoming connection requires
	 *  authorization.
	 *
	 *  @param conn The connection that is requesting authorization
	 *  @param dlc Pointer to received the allocated dlc
	 *
	 *  @return 0 in case of success or negative value in case of error.
	 */
	int (*accept)(struct bt_conn *conn, struct bt_rfcomm_dlc **dlc);

	struct bt_rfcomm_server	*_next;
};

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
#define BT_RFCOMM_TEST          0x01

#define BT_RFCOMM_FCON          0x11
#define BT_RFCOMM_FCOFF         0x12

#define BT_RFCOMM_RPN_COMMAND   0x21
#define BT_RFCOMM_RPN_REQUEST   0x22

/**
 * @name  RFCOMM RPN Parameters - Baud Rates
 *
 * Constant Definitions for RFCOMM RPN Parameters - Baud Rates
 */
/*@{*/
#define RFCOMM_RPN_BAUD_RATE_2400              0x00U
#define RFCOMM_RPN_BAUD_RATE_4800              0x01U
#define RFCOMM_RPN_BAUD_RATE_7200              0x02U
#define RFCOMM_RPN_BAUD_RATE_9600              0x03U
#define RFCOMM_RPN_BAUD_RATE_19200             0x04U
#define RFCOMM_RPN_BAUD_RATE_38400             0x05U
#define RFCOMM_RPN_BAUD_RATE_57600             0x06U
#define RFCOMM_RPN_BAUD_RATE_115200            0x07U
#define RFCOMM_RPN_BAUD_RATE_230400            0x08U

/*@}*/
/** RFCOMM RPN Parameters - Data Bits */
/**
 * @name  RFCOMM RPN Parameters - Data Bits
 *
 * Constant Definitions for RFCOMM RPN Parameters - Data Bits
 */
/*@{*/
#define RFCOMM_RPN_DATA_BITS_5                 0x00U
#define RFCOMM_RPN_DATA_BITS_6                 0x02U
#define RFCOMM_RPN_DATA_BITS_7                 0x01U
#define RFCOMM_RPN_DATA_BITS_8                 0x03U

/*@}*/
/** RFCOMM RPN Parameters - Stop Bit */
/**
 * @name  RFCOMM RPN Parameters - Stop Bit
 *
 * Constant Definitions for RFCOMM RPN Parameters - Stop Bit
 */
/*@{*/
#define RFCOMM_RPN_STOP_BIT_1                  0x00U
#define RFCOMM_RPN_STOP_BIT_1_5                0x01U

/** RFCOMM RPN Parameters - Parity */
/**
 * @name  RFCOMM RPN Parameters - Parity
 *
 * Constant Definitions for RFCOMM RPN Parameters - Parity
 */
#define RFCOMM_RPN_PARITY_NONE                 0x00U
#define RFCOMM_RPN_PARITY_SET                  0x01U

/** RFCOMM RPN Parameters - Parity Type */
/**
 * @name  RFCOMM RPN Parameters - Parity Type
 *
 * Constant Definitions for RFCOMM RPN Parameters - Parity Type
 */
#define RFCOMM_RPN_PARITY_TYPE_ODD             0x00U
#define RFCOMM_RPN_PARITY_TYPE_EVEN            0x02U
#define RFCOMM_RPN_PARITY_TYPE_MARK            0x01U
#define RFCOMM_RPN_PARITY_TYPE_SPACE           0x03U

/**
 * struct bt_rfcomm_rpn
 * Data structure used to send rfcomm remote port negotiation command.
 */
STRUCT_PACKED_PRE
struct bt_rfcomm_rpn {
    /** The Data Link Connection Identifier */
	uint8_t  dlci;
    /** Baud Rate, default value is RFCOMM_RPN_BAUD_RATE_9600.
      * For other values that can be set, refer to  RFCOMM RPN Parameters - Baud Rates.
      */
	uint8_t  baud_rate;
    /** Data Bit, default value is RFCOMM_RPN_DATA_BITS_8.
      * For other values that can be set, refer to RFCOMM RPN Parameters - Data Bits.
      */
    uint8_t  data_bits;
    /** Stop Bit, default value is RFCOMM_RPN_STOP_BIT_1.
     *  For other values that can be set, refer to RFCOMM RPN Parameters - Stop Bit.
     */
    uint8_t  stop_bit;
    /** Parity: default value is RFCOMM_RPN_PARITY_NONE.
     *  For other values that can be set, refer to RFCOMM RPN Parameters - Parity.
     */
    uint8_t  parity;
    /** Parity Type
     *  For values that can be set, refer to RFCOMM RPN Parameters - Parity Type.
     */
    uint8_t  parity_type;
    /** Flow Control: default value is 0x00(no flow control)
     *  bit 1: XON/XOFF on input,
     *  bit 2: XON/XOFF on output,
     *  bit 3: RTR on input,
     *  bit 4: RTR on output,
     *  bit 5: RTC on input,
     *  bit 6: RTC on output
     */
    uint8_t  flow_control;
    /** XON Character: default value is DC1 */
    uint8_t  xon_char;
    /** XOFF Character: default value is DC3 */
    uint8_t  xoff_char;
    /**
     *  Parameter Masks: used to indicate which parameters in
     *  the RPN Frame are negotiated.
     *
     *  For a RPN "Command":
     *      0 -> No Change
     *      1 -> Change
     *
     *  For a RPN "Response":
     *      0 -> Not accepted proposal
     *      1 -> Accepted proposal, and new values are used
     *
     *  Bitmask for the 1st octet of Parameter Mask:
     *      Bit 0 -> Bit/Baud Rate
     *      Bit 1 -> Data Bits
     *      Bit 2 -> Stop Bit
     *      Bit 3 -> Parity
     *      Bit 4 -> Parity Type
     *      Bit 5 -> XON Character
     *      Bit 6 -> XOFF Character
     *      Bit 7 -> (Reserved)
     *
     *  Bitmask for the 2nd octet of Parameter Mask:
     *      Bit 0 -> XON/XOFF in input
     *      Bit 1 -> XON/XOFF in output
     *      Bit 2 -> RTR on input
     *      Bit 3 -> RTR on output
     *      Bit 4 -> RTC on input
     *      Bit 5 -> RTC on output
     *      Bit 6 -> (Reserved)
     *      Bit 7 -> (Reserved)
     */
	uint16_t param_mask;
} STRUCT_PACKED_POST;

#define BT_RFCOMM_GET_LOCAL_PN    0x31
#define BT_RFCOMM_SEND_PN         0x32

/**
 * struct bt_rfcomm_pn
 * Data structure used to send rfcomm parameter negotiation command.
 */
STRUCT_PACKED_PRE
struct bt_rfcomm_pn {
    /** The Data Link Connection Identifier */
    uint8_t  dlci;
    /** Flow Control */
    uint8_t  flow_ctrl;
    /** Priority */
    uint8_t  priority;
    /** Maximum Frame Size */
    uint16_t mtu;
    /** Initial Amount of Credits */
    uint8_t  credits;
} STRUCT_PACKED_POST;

#define BT_RFCOMM_SEND_RLS      0x41
#define BT_RFCOMM_RECVD_RLS     0x42

/** RFCOMM RLS Parameters - Line Status */
/**
 * @name  RFCOMM RLS Parameters - Line Status
 *
 * Constant Definitions for RFCOMM RLS Parameters - Line Status
 */
#define RFCOMM_RLS_NO_ERROR       0x00
#define RFCOMM_RLS_OVERRUN_ERROR  0x03
#define RFCOMM_RLS_PARITY_ERROR   0x05
#define RFCOMM_RLS_FRAMEING_ERROR 0x09

/**
 * struct bt_rfcomm_rls
 * Data structure used to send rfcomm remote line status command.
 */
STRUCT_PACKED_PRE
struct bt_rfcomm_rls {
    /** The Data Link Connection Identifier */
    uint8_t  dlci;
    /** Line Status
     *  For detailed values that can be set, refer to RFCOMM RLS Parameters - Line Status.
     */
    uint8_t  line_status;
}STRUCT_PACKED_POST;

#define BT_RFCOMM_SEND_MSC      0x51
#define BT_RFCOMM_RECVD_MSC     0x52

/**
 * struct bt_rfcomm_msc
 * Data structure used to send rfcomm modum status command.
 */
STRUCT_PACKED_PRE
struct bt_rfcomm_msc {
	/** The Data Link Connection Identifier */
	uint8_t dlci;
    /** Flow Control, set this field as 0x01 when device is unable to accept frames. */
    uint8_t fc;
    /** Ready to Communicate, set this field as 0x01 when device is ready to communicate. */
    uint8_t rtc;
    /** Ready to Receive, set this field as 0x01 when device is ready to receive data. */
    uint8_t rtr;
    /** Incoming Call, set this field as 0x01 to indicate an incoming call. */
    uint8_t ic;
    /** Data valid, set this field as 0x01 to indicate valid data is being sent. */
    uint8_t dv;
    /** Boolean Flag to indicate if Break Signal is present. */
    uint8_t flag;
    /** To indicate that the Octet encodes a valid Break Signal. */
    uint8_t break_present;
    /** Length of Break in units of 200 ms */
	uint8_t break_length;
} STRUCT_PACKED_POST;

struct bt_rfcomm_control
{
    /** if dlc isn't connected, set dlc as null */
    struct bt_rfcomm_dlc *dlc;

    /** if dlc is null, please provide conn, channel and role */
    struct bt_conn   *conn;
    uint8_t          channel;
    bt_rfcomm_role_t role;

    /** rfcomm control type
     *  BT_RFCOMM_TEST,
     *  BT_RFCOMM_FCON, BT_RFCOMM_FCOFF,
     *  BT_RFCOMM_RPN_COMMAND, BT_RFCOMM_RPN_REQUEST,
     *  BT_RFCOMM_SEND_PN, BT_RFCOMM_GET_LOCAL_PN,
     *  BT_RFCOMM_SEND_RLS, BT_RFCOMM_RECVD_RLS,
     *  BT_RFCOMM_SEND_MSC, BT_RFCOMM_RECVD_MSC.
     */
    uint8_t type;

    /** rfcomm control data */
    union
    {
        /** control parameter for test command */
        struct net_buf *test;

        /** control parameter for flow control command */
        uint8_t fc_on;

        /** control parameter for rpn command */
        struct bt_rfcomm_rpn rpn;

        /** control parameter for pn command */
        struct bt_rfcomm_pn  pn;

        /** control parameter for rls command */
        struct bt_rfcomm_rls rls;

        /** control parameter for msc command */
        struct bt_rfcomm_msc msc;
    }control_data;

    sys_snode_t node;
};

/**
 *  Data Type to represent a RFCOMM Control Callback
 */
struct bt_rfcomm_control_cb
{
    struct bt_conn  *conn;
    uint8_t          channel;
    bt_rfcomm_role_t role;

    void (*cb)(struct bt_rfcomm_control *control, int err);

    sys_snode_t node;
};
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

/** @brief Register RFCOMM server
 *
 *  Register RFCOMM server for a channel, each new connection is authorized
 *  using the accept() callback which in case of success shall allocate the dlc
 *  structure to be used by the new connection.
 *
 *  @param server Server structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_rfcomm_server_register(struct bt_rfcomm_server *server);

/** @brief Connect RFCOMM channel
 *
 *  Connect RFCOMM dlc by channel, once the connection is completed dlc
 *  connected() callback will be called. If the connection is rejected
 *  disconnected() callback is called instead.
 *
 *  @param conn Connection object.
 *  @param dlc Dlc object.
 *  @param channel Server channel to connect to.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_rfcomm_dlc_connect(struct bt_conn *conn, struct bt_rfcomm_dlc *dlc,
			  uint8_t channel);

/** @brief Send data to RFCOMM
 *
 *  Send data from buffer to the dlc. Length should be less than or equal to
 *  mtu.
 *
 *  @param dlc Dlc object.
 *  @param buf Data buffer.
 *
 *  @return Bytes sent in case of success or negative value in case of error.
 */
int bt_rfcomm_dlc_send(struct bt_rfcomm_dlc *dlc, struct net_buf *buf);

/** @brief Disconnect RFCOMM dlc
 *
 *  Disconnect RFCOMM dlc, if the connection is pending it will be
 *  canceled and as a result the dlc disconnected() callback is called.
 *
 *  @param dlc Dlc object.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_rfcomm_dlc_disconnect(struct bt_rfcomm_dlc *dlc);

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
/** @brief Register rfcomm control callback
 *
 *  Register RFCOMM control callback.
 *
 *  @param cb Callback handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_rfcomm_register_control_callback(struct bt_rfcomm_control_cb *cb);

/** @brief Unregister rfcomm control callback
 *
 *  Unregister RFCOMM control callback.
 *
 *  @param cb Callback handle, which is registered by bt_rfcomm_register_control_callback().
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_rfcomm_unregister_control_callback(struct bt_rfcomm_control_cb *cb);

/** @brief Send rfcomm control command
 *
 *  Send RFCOMM control command.
 *
 *  @param control Control handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Notice:
 *  Sending RFCOMM Session Test command: control->type is BT_RFCOMM_TEST,
 *  control->control_data.test is test data handle.
 *  Sending RFCOMM Session Flow Control On command: control->type is BT_RFCOMM_FCON,
 *  control->control_data.fc_on is 1U.
 *  Sending RFCOMM Session Flow Control Off command: control->type is BT_RFCOMM_FCOFF,
 *  control->control_data.fc_on is 0U.
 *  Sending RFCOMM RPN request: control->control_data.type is BT_RFCOMM_RPN_REQUEST.
 *  Sending RFCOMM RPN command: control->control_data.type is BT_RFCOMM_RPN_COMMAND,
 *  control->control_data.rpn is rpn data.
 *  Sending RFCOMM PN command: control->control_data.type is BT_RFCOMM_SEND_PN,
 *  control->control_data.pn is pn data.
 *  Sending RFCOMM RLS command: control->control_data.type is BT_RFCOMM_SEND_RLS,
 *  control->control_data.rls is rls data.
 *  Sending RFCOMM MSC command: control->control_data.type is BT_RFCOMM_SEND_MSC,
 *  control->control_data.msc is msc data.
*/
int bt_rfcomm_send_control(struct bt_rfcomm_control *control);
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

/** @brief Allocate the buffer from pool after reserving head room for RFCOMM,
 *  L2CAP and ACL headers.
 *
 *  @param pool Which pool to take the buffer from.
 *
 *  @return New buffer.
 */
struct net_buf *bt_rfcomm_create_pdu(struct net_buf_pool *pool);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_RFCOMM_H_ */
