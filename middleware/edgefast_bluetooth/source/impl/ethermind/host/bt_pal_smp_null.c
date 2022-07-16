/**
 * @file smp_null.c
 * Security Manager Protocol stub
 */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <errno/errno.h>
#include <sys/atomic.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/buf.h>

#include "BT_common.h"
#include "BT_smp_api.h"
#include "BT_sm_api.h"
#include "smp_pl.h"

#if !(defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0) || (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_HCI_CORE)
#define LOG_MODULE_NAME bt_smp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include "bt_pal_hci_core.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_smp.h"

static struct bt_l2cap_le_chan bt_smp_pool[CONFIG_BT_MAX_CONN];

int bt_smp_sign_verify(struct bt_conn *conn, struct net_buf *buf)
{
	return -ENOTSUP;
}

int bt_smp_sign(struct bt_conn *conn, struct net_buf *buf)
{
	return -ENOTSUP;
}

#if 0 /* This function not used, but keep it would be better. */
static int bt_smp_recv(struct bt_l2cap_chan *chan, struct net_buf *req_buf)
{
	struct bt_conn *conn = chan->conn;
	struct bt_smp_pairing_fail *rsp;
	struct bt_smp_hdr *hdr;
	struct net_buf *buf;

	ARG_UNUSED(req_buf);

	/* If a device does not support pairing then it shall respond with
	 * a Pairing Failed command with the reason set to "Pairing Not
	 * Supported" when any command is received.
	 * Core Specification Vol. 3, Part H, 3.3
	 */

	buf = bt_l2cap_create_pdu(NULL, 0);
	/* NULL is not a possible return due to K_FOREVER */

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_SMP_CMD_PAIRING_FAIL;

	rsp = net_buf_add(buf, sizeof(*rsp));
	rsp->reason = BT_SMP_ERR_PAIRING_NOTSUPP;

	if (bt_l2cap_send(conn, BT_L2CAP_CID_SMP, buf)) {
		net_buf_unref(buf);
	}

	return 0;
}
#endif

static int bt_smp_accept(struct bt_conn *conn, struct bt_l2cap_chan **chan)
{
	int i;
	static const struct bt_l2cap_chan_ops ops = {
		.recv = NULL,
	};

	BT_DBG("conn %p handle %u", conn, conn->handle);

	for (i = 0; i < ARRAY_SIZE(bt_smp_pool); i++) {
		struct bt_l2cap_le_chan *smp = &bt_smp_pool[i];

		if (smp->chan.conn) {
			continue;
		}

		smp->chan.ops = &ops;

		*chan = &smp->chan;

		return 0;
	}

	BT_ERR("No available SMP context for conn %p", conn);

	return -ENOMEM;
}

BT_L2CAP_CHANNEL_DEFINE(smp_fixed_chan, BT_L2CAP_CID_SMP, bt_smp_accept, NULL);

#if LOG_ENABLE
static char *ethermind_bt_smp_event_get_name(UCHAR event)
{
    char *name = NULL;
    switch (event)
    {
    case SMP_AUTHENTICATION_COMPLETE:
      name = "SMP_AUTHENTICATION_COMPLETE";
      break;
    case SMP_AUTHENTICATION_REQUEST:
      name = "SMP_AUTHENTICATION_REQUEST";
      break;
    case SMP_PASSKEY_ENTRY_REQUEST:
      name = "SMP_PASSKEY_ENTRY_REQUEST";
      break;
    case SMP_PASSKEY_DISPLAY_REQUEST:
      name = "SMP_PASSKEY_DISPLAY_REQUEST";
      break;
    case SMP_LONG_TERM_KEY_REQUEST:
      name = "SMP_LONG_TERM_KEY_REQUEST";
      break;
    case SMP_KEY_EXCHANGE_INFO_REQUEST:
      name = "SMP_KEY_EXCHANGE_INFO_REQUEST";
      break;
    case SMP_KEY_EXCHANGE_INFO:
      name = "SMP_KEY_EXCHANGE_INFO";
      break;
    case SMP_RESOLVABLE_PVT_ADDR_CREATE_CNF:
      name = "SMP_RESOLVABLE_PVT_ADDR_CREATE_CNF";
      break;
    case SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF:
      name = "SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF";
      break;
    case SMP_DATA_SIGNING_COMPLETE:
      name = "SMP_DATA_SIGNING_COMPLETE";
      break;
    case SMP_SIGN_DATA_VERIFICATION_COMPLETE:
      name = "SMP_SIGN_DATA_VERIFICATION_COMPLETE";
      break;
    case SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST:
      name = "SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST";
      break;
    case SMP_KEY_PRESS_NOTIFICATION_EVENT:
      name = "SMP_KEY_PRESS_NOTIFICATION_EVENT";
      break;
    default:
      name = "UNKNOWN";
      break;
    }
    return name;
}
#endif

static API_RESULT ethermind_bt_smp_cb
           (
               /* IN */ SMP_BD_HANDLE   * bd_handle,
               /* IN */ UCHAR      event,
               /* IN */ API_RESULT status,
               /* IN */ void     * eventdata,
               /* IN */ UINT16     data_len
           )
{
    API_RESULT retval;

    UCHAR    ltk_null;

    SMP_AUTH_INFO * auth;
    SMP_BD_ADDR bdaddr;

    struct bt_conn *conn;

    UCHAR * bd_addr;
    UCHAR   bd_addr_type;

    UCHAR * event_data = (UCHAR *)eventdata;

    /* Get the BD Address from handle */
    BT_smp_get_bd_addr (bd_handle, &bdaddr);

    bd_addr = bdaddr.addr;
    bd_addr_type = bdaddr.type;

    (void)bd_addr;
    (void)bd_addr_type;

    BT_DBG("SMP event =  %s, status %d", ethermind_bt_smp_event_get_name(event), status);

    conn = bt_conn_lookup_device_id(*bd_handle);
	assert(NULL != conn);

    if ((NULL != conn) && (BT_CONN_TYPE_LE != conn->type))
    {
        bt_conn_unref(conn);
        return SMP_INVALID_PARAMETERS;
    }

    retval = API_SUCCESS;

    (void)retval;

    switch(event)
    {
    case SMP_AUTHENTICATION_COMPLETE:
        break;

    case SMP_AUTHENTICATION_REQUEST:
        BT_DBG("Recvd SMP_AUTHENTICATION_REQUEST");
        BT_DBG("BD Address : %02X %02X %02X %02X %02X %02X",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        BT_DBG("BD addr type : %s",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        auth = (SMP_AUTH_INFO *)event_data;

        BT_DBG("Authentication type : %s",
        (SMP_SEC_LEVEL_2 == (auth->security & 0x0F))?  "With MITM":
        "Encryption Only (without MITM)");

        BT_DBG("Bonding type : %s",
        (auth->bonding)? "Bonding": "Non-Bonding");

        auth->param = BT_SMP_ERR_PAIRING_NOTSUPP;
        retval = BT_smp_authentication_request_reply
                 (
                     bd_handle,
                     auth
                 );
        break;

    case SMP_PASSKEY_ENTRY_REQUEST:
        BT_DBG("Event   : SMP_PASSKEY_ENTRY_REQUEST");
        BT_DBG("BD Address : %02X %02X %02X %02X %02X %02X",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        BT_DBG("BD addr type : %s",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        retval = BT_smp_passkey_entry_request_reply
        (
            bd_handle,
            NULL,
            SMP_FALSE
        );
        break;

    case SMP_PASSKEY_DISPLAY_REQUEST:
        break;

    case SMP_KEY_EXCHANGE_INFO_REQUEST:
        break;

    case SMP_LONG_TERM_KEY_REQUEST:
        BT_DBG("Sending -ve LTK request reply.");
        ltk_null = 0;
        retval = BT_smp_long_term_key_request_reply
                    (
                        bd_handle,
                        &ltk_null,
                        SMP_FALSE
                    );
        break;

    case SMP_RESOLVABLE_PVT_ADDR_CREATE_CNF:
        break;

    case SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF:
        break;

    case SMP_DATA_SIGNING_COMPLETE:
        break;

    case SMP_SIGN_DATA_VERIFICATION_COMPLETE:
        break;

    case SMP_KEY_EXCHANGE_INFO:
        break;

    case SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST:
        break;

    case SMP_KEY_PRESS_NOTIFICATION_EVENT:
        break;

    default:
        BT_DBG("ERROR!!! Received unknown event. event = %02X", event);
    }

    BT_DBG("ret = %d", retval);
    bt_conn_unref(conn);
    return API_SUCCESS;
}

static int ethermind_bt_smp_init(void)
{
    SMP_BD_ADDR localAddr;
    API_RESULT retVal;

    retVal = BT_smp_set_io_cap_pl(SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    if (API_SUCCESS == retVal)
    {
        /* Setting the local Public Address as Identity Address */
        retVal = BT_hci_get_local_bd_addr
        (
            localAddr.addr
        );
    }

    if (API_SUCCESS == retVal)
    {
        localAddr.type = BT_BD_PUBLIC_ADDRESS_TYPE;
        retVal = BT_smp_set_local_identity_addr(&localAddr);
    }
#if 1
    if (API_SUCCESS == retVal)
    {
        retVal = BT_smp_register_user_interface (ethermind_bt_smp_cb);
    }
#endif
    if (API_SUCCESS == retVal)
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
}

int bt_smp_init(void)
{
    int ret;

	ret = ethermind_bt_smp_init();
	assert (0 == ret);
    return ret;
}

#endif
