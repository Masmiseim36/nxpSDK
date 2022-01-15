/** @file
 *  @brief Bluetooth SPP handling
 */

/*
 * Copyright (c) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SPP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SPP_H_

/**
 * @brief Serial Port Profile (SPP)
 * @defgroup bt_spp Serial Port Profile (SPP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/conn.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief SPP Generic Data Element Value. */
struct bt_spp;

/** @brief SPP Role Value.
 */
typedef enum bt_spp_role {
 BT_SPP_ROLE_SERVER,
 BT_SPP_ROLE_CLIENT
} bt_spp_role_t;

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
#define SPP_NEGOTIATE_PORT_SETTING 0x01
#define SPP_REQUEST_PORT_SETTING   0x02
/*! @brief spp port setting */
struct bt_spp_port
{
    uint8_t dlci;        /** SPP Port Setting - DLCI */
    uint8_t baud_rate;   /** SPP Port Setting - Baud Rate, refer to SPP Port Parameters - Baud Rates for configured value */
    uint8_t data_bits;   /** SPP Port Setting - Data Bits, refer to SPP Port Parameters - Data Bits for configured value */
    uint8_t stop_bit;    /** SPP Port Setting - Stop Bit, refer to SPP Port Parameters - Stop Bit for configured value */
    uint8_t parity;      /** SPP Port Setting - Parity, refer to SPP Port Parameters - Parity for configured value */
    uint8_t parity_type; /** SPP Port Setting - Parity Type, refer to SPP Port Parameters - Parity Type for configured value */
    uint8_t flc;         /** SPP Port Setting - Flow Control */
    uint8_t x_on;        /** SPP Port Setting - XON Character */
    uint8_t x_off;       /** SPP Port Setting - XOFF Character */
};

/**
 * @name  SPP Port Parameters - Baud Rates
 *
 * Constant Definitions for SPP Port Parameters - Baud Rates
 */
#define SPP_PORT_BAUD_RATE_2400   0x00U
#define SPP_PORT_BAUD_RATE_4800   0x01U
#define SPP_PORT_BAUD_RATE_7200   0x02U
#define SPP_PORT_BAUD_RATE_9600   0x03U
#define SPP_PORT_BAUD_RATE_19200  0x04U
#define SPP_PORT_BAUD_RATE_38400  0x05U
#define SPP_PORT_BAUD_RATE_57600  0x06U
#define SPP_PORT_BAUD_RATE_115200 0x07U
#define SPP_PORT_BAUD_RATE_230400 0x08U

/** SPP Port Parameters - Data Bits */
/**
 * @name  SPP Port Parameters - Data Bits
 *
 * Constant Definitions for SPP Port Parameters - Data Bits
 */
#define SPP_PORT_DATA_BITS_5                 0x00U
#define SPP_PORT_DATA_BITS_6                 0x02U
#define SPP_PORT_DATA_BITS_7                 0x01U
#define SPP_PORT_DATA_BITS_8                 0x03U

/** SPP Port Parameters - Stop Bit */
/**
 * @name  SPP Port Parameters - Stop Bit
 *
 * Constant Definitions for SPP Port Parameters - Stop Bit
 */
#define SPP_PORT_STOP_BIT_1                  0x00U
#define SPP_PORT_STOP_BIT_1_5                0x01U

/** SPP Port Parameters - Parity */
/**
 * @name  SPP Port Parameters - Parity
 *
 * Constant Definitions for SPP Port Parameters - Parity
 */
#define SPP_PORT_PARITY_NONE                 0x00U
#define SPP_PORT_PARITY_SET                  0x01U

/** SPP Port Parameters - Parity Type */
/**
 * @name  SPP Port Parameters - Parity Type
 *
 * Constant Definitions for SPP Port Parameters - Parity Type
 */
#define SPP_PORT_PARITY_TYPE_ODD             0x00U
#define SPP_PORT_PARITY_TYPE_EVEN            0x02U
#define SPP_PORT_PARITY_TYPE_MARK            0x01U
#define SPP_PORT_PARITY_TYPE_SPACE           0x03U

#define SPP_GET_LOCAL_PN     0x10
#define SPP_SEND_PN          0x11
STRUCT_PACKED_PRE
struct bt_spp_pn
{
    uint8_t  dlci;      /** SPP Parameter Negotiation - DLCI */
	uint8_t  flow_ctrl; /** SPP Parameter Negotiation - Flow Control */
	uint8_t  priority;  /** SPP Parameter Negotiation - Priority */
	uint16_t mtu;       /** SPP Parameter Negotiation - Maximum Frame Size */
	uint8_t  credits;   /** SPP Parameter Negotiation - Initial Count of Credits */
}STRUCT_PACKED_POST;

#define SPP_SEND_LINE_STATUS 0x20
#define SPP_REC_LINE_STATUS  0x21
enum bt_spp_rls_error_status
{
	BT_SPP_RLS_NO_ERROR,
	BT_SPP_RLS_OVERRUN_ERROR,
	BT_SPP_RLS_PARITY_ERROR,
	BT_SPP_RLS_FRAMEING_ERROR,
};
typedef enum bt_spp_rls_error_status bt_spp_rls_status_t;
struct bt_spp_rls
{
    uint8_t dlci;
    bt_spp_rls_status_t rls;
};

#define SPP_SEND_MSC     0x30
#define SPP_REC_MSC      0x31
struct bt_spp_msc {
    uint8_t dlci;         /** The DLCI */
    uint8_t fc;           /** Modem Status Parameter - Flow Control */
    uint8_t rtc;          /** Modem Status Parameter - Ready to Communicate */
    uint8_t rtr;          /** Modem Status Parameter - Ready to Receive */
    uint8_t ic;           /** Modem Status Parameter - Incoming Call */
    uint8_t dv;           /** Modem Status Parameter - Data valid */
    uint8_t flag;         /** Flag to indicate if Break Signal is present */
    uint8_t break_present;/** To indicate that the Octet encodes a valid Break Signal */
    uint8_t break_length; /** Length of Break in units of 200 ms */
};

struct bt_spp_control
{
    struct bt_spp  *spp;

    struct bt_conn *conn;
    uint8_t         channel;
    bt_spp_role_t   role;

    /* control type */
    uint8_t type;

    union
    {
        struct bt_spp_port port;
        struct bt_spp_pn   pn;
        struct bt_spp_rls rls;
        struct bt_spp_msc msc;
    }control_data;
};
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/*! @brief spp application callback function */
typedef struct _bt_spp_callback
{
    void (*connected)(struct bt_spp *spp, int error);
    void (*disconnected)(struct bt_spp *spp, int error);
    void (*data_received)(struct bt_spp *spp, uint8_t *data, uint16_t len);
    void (*data_sent)(struct bt_spp *spp, uint8_t *data, uint16_t len);
#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
    void (*control)(struct bt_spp_control *control, int error);
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */
}bt_spp_callback;

/*! @brief spp sdp discover callback function */
typedef int (*bt_spp_discover_callback)(struct bt_conn *conn, uint8_t count, uint16_t *channel);

/*! @brief bt_spp_discover callback parameter */
typedef struct
{
    bt_spp_discover_callback cb;
    struct bt_conn *conn;
    void *param;
    void *next;
} discover_cb_t;

/* SPP API */

/** @brief Register a SPP server
 *
 *  Register a SPP server channel, wait for spp connection from SPP client.
 *  Once it's connected by spp client, will notify application by calling cb->connected.
 *
 *  @param channel Registered server channel.
 *
 *  @param cb      Application callback.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_server_register(uint8_t channel, bt_spp_callback *cb);


/** @brief Discover SPP server channel
 *
 *  Discover peer SPP server channel after basic BR connection is created.
 *  Will notify application discover results by calling cb->cb.
 *
 *  @param conn BR connection handle.
 *
 *  @param cb   Discover callback.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_discover(struct bt_conn *conn, discover_cb_t *cb);

/** @brief Connect SPP server channel
 *
 *  Create SPP connection with remote SPP server channel.
 *  Once connection is created successfully, will notify application by calling cb->connected.
 *
 *  @param conn    Conn handle created with remote device.
 *
 *  @param channel Remote server channel to be connected, if it's 0, will connect remote BT_RFCOMM_CHAN_SPP channel.
 *
 *  @param cb      Application callback.
 *
 *  @param spp     SPP handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_client_connect(struct bt_conn *conn, uint8_t channel, bt_spp_callback *cb, struct bt_spp **spp);

/** @brief Send data to peer SPP device
 *
 *  Send data to connected peer spp.
 *  Once data is sent, will notify application by calling cb->data_sent, which is provided by bt_spp_server_register or bt_spp_client_connect.
 *  If peer spp receives data, will notify application by calling cb->data_received.
 *
 *  @param spp  SPP handle.
 *  @param data Data buffer.
 *  @param len  Data length.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_data_send(struct bt_spp *spp, uint8_t *data, uint16_t len);

/** @brief Disconnect SPP connection
 *
 *  Disconnect SPP connection.
 *  Once connection is disconnected, will notify application by calling cb->disconnected, which is provided by bt_spp_server_register or bt_spp_client_connect.
 *
 *  @param spp SPP handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_disconnect(struct bt_spp *spp);

/** @brief Get channel of SPP handle
 *
 *  @param spp      SPP handle.
 *  @param channel  Pointer to channel of spp handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_get_channel(struct bt_spp *spp, uint8_t * channel);

/** @brief Get role of SPP handle
 *
 *  @param spp   SPP handle.
 *  @param role  Pointer to role of spp handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_get_role(struct bt_spp *spp, bt_spp_role_t * role);

/** @brief Get conn handle of SPP handle
 *
 *  @param spp   SPP handle.
 *  @param conn  Pointer to conn handle of spp handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_get_conn(struct bt_spp *spp, struct bt_conn ** conn);

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
/** @brief Request SPP Port Setting
 *
 *  This API enables the User to retrieve peer SPP entity's port settings.
 *
 *  @param conn    BR conn handle.
 *  @param channel Server channel.
 *  @param role    SPP role.
 *  @param cb      Callback.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Notice: In one spp connection, port setting in server and client
 *          is always the same whether spp connection is created or not.
 *          This API can be called before or after a spp connection is created.
 *          When it's called before spp connection is created, it will get the port
 *          setting of spp connection which will be initialized by current decive(client),
 *          in this case, conn, channel and cb needs to be provided.
 *          When it's called after spp connection is created, conn, channel and role needs
 *          to be provided.
 */
int bt_spp_request_port_setting(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb);

/** @brief Negotiate Remote Port Setting
 *
 *  This API enables the User to initiate Port Negotiation with remote SPP entity with respect to those port settings.
 *
 *  @param conn    Conn handle.
 *  @param channel Channel.
 *  @param role    SPP role.
 *  @param cb      Callback.
 *  @param port    Port setting.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Notice: In one spp connection, port setting in server and client
 *          is always the same whether spp connection is created or not.
 *          This API can be called before or after a spp connection is created.
 *          When it's called before spp connection is created, it will negotiate the port
 *          setting of spp connection which will be initialized by current decive(client),
 *          in this case, conn, channel, cb and port needs to be provided.
 *          When it's called after spp connection is created, conn, channel, role and port
 *          needs to be provided.
 */
int bt_spp_negotiate_port_setting(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb, struct bt_spp_port *port);

/** @brief Send Parameter Negotiation(PN)
 *
 *  This API enables the User to initiate Parameter Negotiation with the remote
 *  SPP Entity. The only useful parameter in the Parameter Negotiation(struct bt_spp_pn)
 *  block is mtu.
 *
 *  @param conn    Conn handle.
 *  @param channel Channel.
 *  @param role    SPP role.
 *  @param cb      Callback.
 *  @param pn      Parameter data handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Notice: This API can be called before or after a spp connection is created.
 *          When it's called before spp connection is created, it will set the port
 *          setting of spp connection which will be initialized by current decive(client),
 *          in this case, conn, channel, cb and pn needs to be provided.
 *          When it's called after spp connection is created, conn, channel, role and pn needs
 *          to be provided.
 */
int bt_spp_send_pn(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb, struct bt_spp_pn *pn);

/** @brief Get Local Parameter
 *
 *  This API enables the User to get local parameter data. Only mtu, dlci and priority parameter in the Parameter Negotiation(struct bt_rfcomm_pn)
 *  block can be gotten.
 *
 *  @param conn    Conn handle.
 *  @param channel Channel.
 *  @param role    SPP role.
 *  @param cb      Callback.
 *  @param pn      Parameter data handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Notice: This API can be called before or after a spp connection is created.
 *          When it's called before spp connection is created, the pn must be set by
 *          bt_spp_send_pn(), in this case, conn, channel, and cb needs to be provided.
 *          When it's called after spp connection is created, conn, channel, and role needs
 *          to be provided.
 */
int bt_spp_get_local_pn(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, struct bt_spp_pn *pn);

/** @brief Send Line Status
 *
 *  This API enables the User to send line status.
 *
 *  @param spp  SPP handle.
 *  @param rls  RLS data handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_send_rls(struct bt_spp *spp, struct bt_spp_rls *rls);

/** @brief Send Modum Status
 *
 *  This API enables the User to send modum status.
 *
 *  @param spp  SPP handle.
 *  @param msc  MSC data handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_send_msc(struct bt_spp *spp, struct bt_spp_msc *msc);
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SPP_H_ */
