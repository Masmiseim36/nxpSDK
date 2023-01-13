/*
 * Copyright 2021 NXP
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>

#include "porting.h"

#include <sys/byteorder.h>

#include <bluetooth/hci.h>
#include <bluetooth/addr.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_HCI_CORE)
#define LOG_MODULE_NAME bt_ssp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#include "sm_ssp_pl.h"
#include "BT_sm_api.h"

#include "bt_pal_keys.h"

#include "bt_pal_hci_core.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_ssp.h"

enum pairing_method {
	LEGACY,			/* Legacy (pre-SSP) pairing */
	JUST_WORKS,		/* JustWorks pairing */
	PASSKEY_INPUT,		/* Passkey Entry input */
	PASSKEY_DISPLAY,	/* Passkey Entry display */
	PASSKEY_CONFIRM,	/* Passkey confirm */
};

/* based on table 5.7, Core Spec 4.2, Vol.3 Part C, 5.2.2.6 */
static const uint8_t ssp_method[4 /* remote */][4 /* local */] = {
	      { JUST_WORKS, JUST_WORKS, PASSKEY_INPUT, JUST_WORKS },
	      { JUST_WORKS, PASSKEY_CONFIRM, PASSKEY_INPUT, JUST_WORKS },
	      { PASSKEY_DISPLAY, PASSKEY_DISPLAY, PASSKEY_INPUT, JUST_WORKS },
	      { JUST_WORKS, JUST_WORKS, JUST_WORKS, JUST_WORKS },
};

static int ssp_passkey_neg_reply(struct bt_conn *conn);
static int ssp_confirm_reply(struct bt_conn *conn);

static int pin_code_neg_reply(bt_addr_t *bdaddr)
{
#if 0
	struct bt_hci_cp_pin_code_neg_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_PIN_CODE_NEG_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, bdaddr);

	return bt_hci_cmd_send_sync(BT_HCI_OP_PIN_CODE_NEG_REPLY, buf, NULL);
#else
    API_RESULT retval = BT_sm_pin_code_request_reply (bdaddr->val, NULL, 0);

    if (API_SUCCESS == retval)
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
#endif
}

static int pin_code_reply(struct bt_conn *conn, const char *pin, uint8_t len)
{
#if 0
	struct bt_hci_cp_pin_code_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_PIN_CODE_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));

	bt_addr_copy(&cp->bdaddr, &conn->br.dst);
	cp->pin_len = len;
	strncpy((char *)cp->pin_code, pin, sizeof(cp->pin_code));

	return bt_hci_cmd_send_sync(BT_HCI_OP_PIN_CODE_REPLY, buf, NULL);
#else
	UCHAR pinCode[16];
	API_RESULT retval;

	memcpy(pinCode, pin, len);

	retval = BT_sm_pin_code_request_reply (conn->br.dst.val, (UCHAR *)pinCode, len);

	if (API_SUCCESS == retval)
	{
		return 0;
	}
	else
	{
		return -EIO;
	}
#endif
}

int bt_conn_auth_pincode_entry(struct bt_conn *conn, const char *pin)
{
	size_t len;

	if (!bt_auth) {
		return -EINVAL;
	}

	if (conn->type != BT_CONN_TYPE_BR) {
		return -EINVAL;
	}

	len = strlen(pin);
	if (len > 16) {
		return -EINVAL;
	}

	if (conn->required_sec_level == BT_SECURITY_L3 && len < 16) {
		BT_WARN("PIN code for %s is not 16 bytes wide",
			bt_addr_str(&conn->br.dst));
		return -EPERM;
	}

	/* Allow user send entered PIN to remote, then reset user state. */
	if (!atomic_test_and_clear_bit(conn->flags, BT_CONN_USER)) {
		return -EPERM;
	}

	if (len == 16) {
		atomic_set_bit(conn->flags, BT_CONN_BR_LEGACY_SECURE);
	}

	return pin_code_reply(conn, pin, len);
}

void pin_code_req(struct bt_conn *conn)
{
	conn->br.pairing_method = LEGACY;
	if (bt_auth && bt_auth->pincode_entry) {
		bool secure = false;

		if (conn->required_sec_level == BT_SECURITY_L3) {
			secure = true;
		}

		atomic_set_bit(conn->flags, BT_CONN_USER);
		atomic_set_bit(conn->flags, BT_CONN_BR_PAIRING);
		bt_auth->pincode_entry(conn, secure);
	} else {
		pin_code_neg_reply(&conn->br.dst);
	}
}

void user_confirm_req(struct bt_conn *conn, unsigned int passkey)
{
	conn->br.pairing_method = PASSKEY_CONFIRM;
	if (bt_auth && bt_auth->passkey_confirm) {
		atomic_set_bit(conn->flags, BT_CONN_USER);
		bt_auth->passkey_confirm(conn, passkey);
	} else {
		ssp_confirm_reply(conn);
	}
}

void user_passkey_req(struct bt_conn *conn)
{
	conn->br.pairing_method = PASSKEY_INPUT;
	if (bt_auth && bt_auth->passkey_entry) {
		atomic_set_bit(conn->flags, BT_CONN_USER);
		bt_auth->passkey_entry(conn);
	} else {
		ssp_passkey_neg_reply(conn);
	}
}

void user_passkey_ntf(struct bt_conn *conn, unsigned int passkey)
{
	conn->br.pairing_method = PASSKEY_DISPLAY;
	if (bt_auth && bt_auth->passkey_display) {
		atomic_set_bit(conn->flags, BT_CONN_USER);
		bt_auth->passkey_display(conn, passkey);
	}
}


static uint8_t get_io_capa(void)
{
	if (!bt_auth) {
		return BT_IO_NO_INPUT_OUTPUT;
	}

	if (bt_auth->passkey_confirm && bt_auth->passkey_display) {
		return BT_IO_DISPLAY_YESNO;
	}

	if (bt_auth->passkey_entry) {
		return BT_IO_KEYBOARD_ONLY;
	}

	if (bt_auth->passkey_display) {
		return BT_IO_DISPLAY_ONLY;
	}

	return BT_IO_NO_INPUT_OUTPUT;
}

static uint8_t ssp_pair_method(const struct bt_conn *conn)
{
	return ssp_method[conn->br.remote_io_capa][get_io_capa()];
}

static uint8_t ssp_get_auth(const struct bt_conn *conn)
{
	/* Validate no bond auth request, and if valid use it. */
	if ((conn->br.remote_auth == BT_HCI_NO_BONDING) ||
	    ((conn->br.remote_auth == BT_HCI_NO_BONDING_MITM) &&
	     (ssp_pair_method(conn) > JUST_WORKS))) {
		return conn->br.remote_auth;
	}

	/* Local & remote have enough IO capabilities to get MITM protection. */
	if (ssp_pair_method(conn) > JUST_WORKS) {
		return conn->br.remote_auth | BT_MITM;
	}

	/* No MITM protection possible so ignore remote MITM requirement. */
	return (conn->br.remote_auth & ~BT_MITM);
}

static int ssp_confirm_reply(struct bt_conn *conn)
{
#if 0
	struct bt_hci_cp_user_confirm_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_USER_CONFIRM_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, &conn->br.dst);

	return bt_hci_cmd_send_sync(BT_HCI_OP_USER_CONFIRM_REPLY, buf, NULL);
#else
	API_RESULT retval;

	retval = BT_sm_user_conf_request_reply (conn->br.dst.val, 0x01);

    if (API_SUCCESS == retval)
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
#endif
}

static int ssp_confirm_neg_reply(struct bt_conn *conn)
{
#if 0
	struct bt_hci_cp_user_confirm_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_USER_CONFIRM_NEG_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, &conn->br.dst);

	return bt_hci_cmd_send_sync(BT_HCI_OP_USER_CONFIRM_NEG_REPLY, buf,
				    NULL);
#else
    API_RESULT retval = BT_sm_user_conf_request_reply (conn->br.dst.val, 0x00);

    if (API_SUCCESS == retval)
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
#endif
}

static void ssp_pairing_complete(struct bt_conn *conn, uint8_t status)
{
	if (!status) {
		bool bond = !atomic_test_bit(conn->flags, BT_CONN_BR_NOBOND);
		struct bt_conn_auth_info_cb *listener, *next;

		SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&bt_auth_info_cbs, listener,
						  next, node, struct bt_conn_auth_info_cb) {
			if (listener->pairing_complete) {
				listener->pairing_complete(conn, bond);
			}
		}
	} else {
		struct bt_conn_auth_info_cb *listener, *next;

		SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&bt_auth_info_cbs, listener,
						  next, node, struct bt_conn_auth_info_cb) {
			if (listener->pairing_complete) {
				listener->pairing_complete(conn, status);
			}
		}
	}
}

#if 0
static void ssp_auth(struct bt_conn *conn, uint32_t passkey)
{
	conn->br.pairing_method = ssp_pair_method(conn);

	/*
	 * If local required security is HIGH then MITM is mandatory.
	 * MITM protection is no achievable when SSP 'justworks' is applied.
	 */
	if (conn->required_sec_level > BT_SECURITY_L2 &&
	    conn->br.pairing_method == JUST_WORKS) {
		BT_DBG("MITM protection infeasible for required security");
		ssp_confirm_neg_reply(conn);
		return;
	}

	switch (conn->br.pairing_method) {
	case PASSKEY_CONFIRM:
		atomic_set_bit(conn->flags, BT_CONN_USER);
		bt_auth->passkey_confirm(conn, passkey);
		break;
	case PASSKEY_DISPLAY:
		atomic_set_bit(conn->flags, BT_CONN_USER);
		bt_auth->passkey_display(conn, passkey);
		break;
	case PASSKEY_INPUT:
		atomic_set_bit(conn->flags, BT_CONN_USER);
		bt_auth->passkey_entry(conn);
		break;
	case JUST_WORKS:
		/*
		 * When local host works as pairing acceptor and 'justworks'
		 * model is applied then notify user about such pairing request.
		 * [BT Core 4.2 table 5.7, Vol 3, Part C, 5.2.2.6]
		 */
		if (bt_auth && bt_auth->pairing_confirm &&
		    !atomic_test_bit(conn->flags,
				     BT_CONN_BR_PAIRING_INITIATOR)) {
			atomic_set_bit(conn->flags, BT_CONN_USER);
			bt_auth->pairing_confirm(conn);
			break;
		}
		ssp_confirm_reply(conn);
		break;
	default:
        /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
		break;
	}
}
#endif

static int ssp_passkey_reply(struct bt_conn *conn, unsigned int passkey)
{
#if 0
	struct bt_hci_cp_user_passkey_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_USER_PASSKEY_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, &conn->br.dst);
	cp->passkey = sys_cpu_to_le32(passkey);

	return bt_hci_cmd_send_sync(BT_HCI_OP_USER_PASSKEY_REPLY, buf, NULL);
#else
    API_RESULT retval = BT_sm_user_passkey_request_reply (conn->br.dst.val, (UCHAR *)&passkey);

    if (API_SUCCESS == retval)
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
#endif
}

static int ssp_passkey_neg_reply(struct bt_conn *conn)
{
#if 0
	struct bt_hci_cp_user_passkey_neg_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_USER_PASSKEY_NEG_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, &conn->br.dst);

	return bt_hci_cmd_send_sync(BT_HCI_OP_USER_PASSKEY_NEG_REPLY, buf,
				    NULL);
#else
    API_RESULT retval = BT_sm_user_passkey_request_reply (conn->br.dst.val, NULL);

    if (API_SUCCESS == retval)
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
#endif
}

static int conn_auth(struct bt_conn *conn)
{
#if 0
    struct bt_hci_cp_auth_requested *auth;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_AUTH_REQUESTED, sizeof(*auth));
	if (!buf) {
		return -ENOBUFS;
	}

	auth = net_buf_add(buf, sizeof(*auth));
	auth->handle = sys_cpu_to_le16(conn->handle);

	atomic_set_bit(conn->flags, BT_CONN_BR_PAIRING_INITIATOR);

	return bt_hci_cmd_send_sync(BT_HCI_OP_AUTH_REQUESTED, buf, NULL);
#else
	API_RESULT retval = BT_sm_authentication_request (conn->br.dst.val);
	if (API_SUCCESS == retval)
	{
		return 0;
	}
	else
	{
		return -EIO;
	}
#endif
}

int bt_ssp_start_security(struct bt_conn *conn)
{
	if (atomic_test_bit(conn->flags, BT_CONN_BR_PAIRING)) {
		return -EBUSY;
	}

	if (conn->required_sec_level > BT_SECURITY_L3) {
		return -ENOTSUP;
	}

	if (get_io_capa() == BT_IO_NO_INPUT_OUTPUT &&
	    conn->required_sec_level > BT_SECURITY_L2) {
		return -EINVAL;
	}

	return conn_auth(conn);
}

int bt_ssp_auth_passkey_entry(struct bt_conn *conn, unsigned int passkey)
{
	/* User entered passkey, reset user state. */
	if (!atomic_test_and_clear_bit(conn->flags, BT_CONN_USER)) {
		return -EPERM;
	}

	if (conn->br.pairing_method == PASSKEY_INPUT) {
		return ssp_passkey_reply(conn, passkey);
	}

	return -EINVAL;
}

int bt_ssp_auth_passkey_confirm(struct bt_conn *conn)
{
	/* Allow user confirm passkey value, then reset user state. */
	if (!atomic_test_and_clear_bit(conn->flags, BT_CONN_USER)) {
		return -EPERM;
	}

	return ssp_confirm_reply(conn);
}

int bt_ssp_auth_pairing_confirm(struct bt_conn *conn)
{
	return ssp_confirm_reply(conn);
}

int bt_ssp_auth_cancel(struct bt_conn *conn)
{
	/* Allow user cancel authentication, then reset user state. */
	if (!atomic_test_and_clear_bit(conn->flags, BT_CONN_USER)) {
		return -EPERM;
	}

	switch (conn->br.pairing_method) {
	case JUST_WORKS:
	case PASSKEY_CONFIRM:
		return ssp_confirm_neg_reply(conn);
	case PASSKEY_INPUT:
		return ssp_passkey_neg_reply(conn);
	case PASSKEY_DISPLAY:
		return bt_conn_disconnect(conn,
					  BT_HCI_ERR_AUTH_FAIL);
	case LEGACY:
		return pin_code_neg_reply(&conn->br.dst);
	default:
	  /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
		break;
	}

	return -EINVAL;
}

void bt_hci_pin_code_req(struct net_buf *buf)
{
#if 0
	struct bt_hci_evt_pin_code_req *evt = (void *)buf->data;
	struct bt_conn *conn;

	BT_DBG("");

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	pin_code_req(conn);
	bt_conn_unref(conn);
#endif
}

void bt_hci_link_key_notify(struct net_buf *buf)
{
	struct bt_hci_evt_link_key_notify *evt = (struct bt_hci_evt_link_key_notify *)buf->data;
	struct bt_conn *conn;
    SM_IO_CAPS peer_iocaps;

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

    if (API_SUCCESS == BT_sm_get_remote_iocaps_pl(&evt->bdaddr.val[0], &peer_iocaps))
    {
        conn->br.remote_io_capa = (uint8_t)peer_iocaps.io_cap;
        conn->br.remote_auth = (uint8_t)peer_iocaps.auth_reqs;
    }

	BT_DBG("%s, link type 0x%02x", bt_addr_str(&evt->bdaddr), evt->key_type);

	if (!conn->br.link_key) {
		conn->br.link_key = bt_keys_get_link_key(&evt->bdaddr);
	}
	if (!conn->br.link_key) {
		BT_ERR("Can't update keys for %s", bt_addr_str(&evt->bdaddr));
		bt_conn_unref(conn);
		return;
	}

	/* clear any old Link Key flags */
	conn->br.link_key->flags = 0U;

	switch (evt->key_type) {
	case BT_LK_COMBINATION:
		/*
		 * Setting Combination Link Key as AUTHENTICATED means it was
		 * successfully generated by 16 digits wide PIN code.
		 */
		if (atomic_test_and_clear_bit(conn->flags,
					      BT_CONN_BR_LEGACY_SECURE)) {
			conn->br.link_key->flags |= BT_LINK_KEY_AUTHENTICATED;
		}
		memcpy(conn->br.link_key->val, evt->link_key, 16);
		break;
	case BT_LK_AUTH_COMBINATION_P192:
		conn->br.link_key->flags |= BT_LINK_KEY_AUTHENTICATED;
		__fallthrough;
	case BT_LK_UNAUTH_COMBINATION_P192:
		/* Mark no-bond so that link-key is removed on disconnection */
		if (ssp_get_auth(conn) < BT_HCI_DEDICATED_BONDING) {
			atomic_set_bit(conn->flags, BT_CONN_BR_NOBOND);
		}

		memcpy(conn->br.link_key->val, evt->link_key, 16);
		break;
	case BT_LK_AUTH_COMBINATION_P256:
		conn->br.link_key->flags |= BT_LINK_KEY_AUTHENTICATED;
		__fallthrough;
	case BT_LK_UNAUTH_COMBINATION_P256:
		conn->br.link_key->flags |= BT_LINK_KEY_SC;

		/* Mark no-bond so that link-key is removed on disconnection */
		if (ssp_get_auth(conn) < BT_HCI_DEDICATED_BONDING) {
			atomic_set_bit(conn->flags, BT_CONN_BR_NOBOND);
		}

		memcpy(conn->br.link_key->val, evt->link_key, 16);
		break;
	default:
		BT_WARN("Unsupported Link Key type %u", evt->key_type);
		(void)memset(conn->br.link_key->val, 0,
			     sizeof(conn->br.link_key->val));
		break;
	}

	if (IS_ENABLED(CONFIG_BT_SETTINGS) &&
	    !atomic_test_bit(conn->flags, BT_CONN_BR_NOBOND)) {
		bt_keys_link_key_store(conn->br.link_key);
	}

	bt_conn_unref(conn);
}

static void link_key_neg_reply(bt_addr_t *bdaddr)
{
#if 0
	struct bt_hci_cp_link_key_neg_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_LINK_KEY_NEG_REPLY, sizeof(*cp));
	if (!buf) {
		BT_ERR("Out of command buffers");
		return;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, bdaddr);
	bt_hci_cmd_send_sync(BT_HCI_OP_LINK_KEY_NEG_REPLY, buf, NULL);
#else
    (void)BT_sm_link_key_request_reply (bdaddr->val, NULL, 0);
#endif
}

static void link_key_reply(bt_addr_t *bdaddr, uint8_t *lk)
{
#if 0
	struct bt_hci_cp_link_key_reply *cp;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_LINK_KEY_REPLY, sizeof(*cp));
	if (!buf) {
		BT_ERR("Out of command buffers");
		return;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, bdaddr);
	memcpy(cp->link_key, lk, 16);
	bt_hci_cmd_send_sync(BT_HCI_OP_LINK_KEY_REPLY, buf, NULL);
#else
    (void)BT_sm_link_key_request_reply (bdaddr->val, lk, 1);
#endif
}

void bt_hci_link_key_req(struct net_buf *buf)
{
	struct bt_hci_evt_link_key_req *evt = (struct bt_hci_evt_link_key_req *)buf->data;
	struct bt_conn *conn;

	BT_DBG("%s", bt_addr_str(&evt->bdaddr));

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		link_key_neg_reply(&evt->bdaddr);
		return;
	}

	if (!conn->br.link_key) {
		conn->br.link_key = bt_keys_find_link_key(&evt->bdaddr);
	}

	if (!conn->br.link_key) {
		link_key_neg_reply(&evt->bdaddr);
		bt_conn_unref(conn);
		return;
	}

	/*
	 * Enforce regenerate by controller stronger link key since found one
	 * in database not covers requested security level.
	 */
	if (!(conn->br.link_key->flags & BT_LINK_KEY_AUTHENTICATED) &&
	    conn->required_sec_level > BT_SECURITY_L2) {
		link_key_neg_reply(&evt->bdaddr);
		bt_conn_unref(conn);
		return;
	}

	link_key_reply(&evt->bdaddr, conn->br.link_key->val);
	bt_conn_unref(conn);
}

#if 0
static void io_capa_neg_reply(const bt_addr_t *bdaddr, const uint8_t reason)
{
	struct bt_hci_cp_io_capability_neg_reply *cp;
	struct net_buf *resp_buf;

	resp_buf = bt_hci_cmd_create(BT_HCI_OP_IO_CAPABILITY_NEG_REPLY,
				     sizeof(*cp));
	if (!resp_buf) {
		BT_ERR("Out of command buffers");
		return;
	}

	cp = net_buf_add(resp_buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, bdaddr);
	cp->reason = reason;
	bt_hci_cmd_send_sync(BT_HCI_OP_IO_CAPABILITY_NEG_REPLY, resp_buf, NULL);
}
#endif

void bt_hci_io_capa_resp(struct net_buf *buf)
{
#if 0
	struct bt_hci_evt_io_capa_resp *evt = (void *)buf->data;
	struct bt_conn *conn;

	BT_DBG("remote %s, IOcapa 0x%02x, auth 0x%02x",
	       bt_addr_str(&evt->bdaddr), evt->capability, evt->authentication);

	if (evt->authentication > BT_HCI_GENERAL_BONDING_MITM) {
		BT_ERR("Invalid remote authentication requirements");
		io_capa_neg_reply(&evt->bdaddr,
				  BT_HCI_ERR_UNSUPP_FEATURE_PARAM_VAL);
		return;
	}

	if (evt->capability > BT_IO_NO_INPUT_OUTPUT) {
		BT_ERR("Invalid remote io capability requirements");
		io_capa_neg_reply(&evt->bdaddr,
				  BT_HCI_ERR_UNSUPP_FEATURE_PARAM_VAL);
		return;
	}

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Unable to find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	conn->br.remote_io_capa = evt->capability;
	conn->br.remote_auth = evt->authentication;
	atomic_set_bit(conn->flags, BT_CONN_BR_PAIRING);
	bt_conn_unref(conn);
#endif
}

void bt_hci_io_capa_req(struct net_buf *buf)
{
#if 0
	struct bt_hci_evt_io_capa_req *evt = (void *)buf->data;
	struct net_buf *resp_buf;
	struct bt_conn *conn;
	struct bt_hci_cp_io_capability_reply *cp;
	uint8_t auth;

	BT_DBG("");

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	resp_buf = bt_hci_cmd_create(BT_HCI_OP_IO_CAPABILITY_REPLY,
				     sizeof(*cp));
	if (!resp_buf) {
		BT_ERR("Out of command buffers");
		bt_conn_unref(conn);
		return;
	}

	/*
	 * Set authentication requirements when acting as pairing initiator to
	 * 'dedicated bond' with MITM protection set if local IO capa
	 * potentially allows it, and for acceptor, based on local IO capa and
	 * remote's authentication set.
	 */
	if (atomic_test_bit(conn->flags, BT_CONN_BR_PAIRING_INITIATOR)) {
		if (get_io_capa() != BT_IO_NO_INPUT_OUTPUT) {
			auth = BT_HCI_DEDICATED_BONDING_MITM;
		} else {
			auth = BT_HCI_DEDICATED_BONDING;
		}
	} else {
		auth = ssp_get_auth(conn);
	}

	cp = net_buf_add(resp_buf, sizeof(*cp));
	bt_addr_copy(&cp->bdaddr, &evt->bdaddr);
	cp->capability = get_io_capa();
	cp->authentication = auth;
	cp->oob_data = 0U;
	bt_hci_cmd_send_sync(BT_HCI_OP_IO_CAPABILITY_REPLY, resp_buf, NULL);
	bt_conn_unref(conn);
#endif
}

void bt_hci_ssp_complete(struct net_buf *buf)
{
	struct bt_hci_evt_ssp_complete *evt = (struct bt_hci_evt_ssp_complete *)buf->data;
	struct bt_conn *conn;

	BT_DBG("status 0x%02x", evt->status);

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	ssp_pairing_complete(conn, bt_security_err_get(evt->status));
	if (evt->status) {
		bt_conn_disconnect(conn, BT_HCI_ERR_AUTH_FAIL);
	}

	bt_conn_unref(conn);
}

void bt_hci_user_confirm_req(struct net_buf *buf)
{
#if 0
	struct bt_hci_evt_user_confirm_req *evt = (void *)buf->data;
	struct bt_conn *conn;

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	ssp_auth(conn, sys_le32_to_cpu(evt->passkey));
	bt_conn_unref(conn);
#endif
}

void bt_hci_user_passkey_notify(struct net_buf *buf)
{
#if 0
	struct bt_hci_evt_user_passkey_notify *evt = (void *)buf->data;
	struct bt_conn *conn;

	BT_DBG("");

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	ssp_auth(conn, sys_le32_to_cpu(evt->passkey));
	bt_conn_unref(conn);
#endif
}

void bt_hci_user_passkey_req(struct net_buf *buf)
{
#if 0
	struct bt_hci_evt_user_passkey_req *evt = (void *)buf->data;
	struct bt_conn *conn;

	conn = bt_conn_lookup_addr_br(&evt->bdaddr);
	if (!conn) {
		BT_ERR("Can't find conn for %s", bt_addr_str(&evt->bdaddr));
		return;
	}

	ssp_auth(conn, 0);
	bt_conn_unref(conn);
#endif
}
#if 0
static void link_encr(const uint16_t handle)
{
#if 0
	struct bt_hci_cp_set_conn_encrypt *encr;
	struct net_buf *buf;

	BT_DBG("");

	buf = bt_hci_cmd_create(BT_HCI_OP_SET_CONN_ENCRYPT, sizeof(*encr));
	if (!buf) {
		BT_ERR("Out of command buffers");
		return;
	}

	encr = net_buf_add(buf, sizeof(*encr));
	encr->handle = sys_cpu_to_le16(handle);
	encr->encrypt = 0x01;

	bt_hci_cmd_send_sync(BT_HCI_OP_SET_CONN_ENCRYPT, buf, NULL);
#endif
}
#endif

void bt_hci_auth_complete(struct net_buf *buf)
{
	struct bt_hci_evt_auth_complete *evt = (struct bt_hci_evt_auth_complete *)buf->data;
	struct bt_conn *conn;
	uint16_t handle = sys_le16_to_cpu(evt->handle);

	BT_DBG("status 0x%02x, handle %u", evt->status, handle);

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Can't find conn for handle %u", handle);
		return;
	}
	if (evt->status) {
		/*
		 * Inform layers above HCI about non-zero authentication
		 * status to make them able cleanup pending jobs.
		 */
		bt_conn_security_changed(conn, evt->status,
					 bt_security_err_get(evt->status));
	} else {
#if 0
		link_encr(handle);
#endif
	}
	bt_conn_unref(conn);
}
#endif /* #if CONFIG_BT_BREDR */