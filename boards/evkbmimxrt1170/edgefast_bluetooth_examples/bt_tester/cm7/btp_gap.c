/* gap.c - Bluetooth GAP Tester */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "porting.h"

#include "fsl_component_serial_manager.h"
#include "fsl_debug_console.h"

#include "bt_pal_hci_core.h"

#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/ead.h>

#include "btp/btp.h"

#define CONTROLLER_NAME "btp_tester"

#define BT_LE_AD_DISCOV_MASK (BT_LE_AD_LIMITED | BT_LE_AD_GENERAL)
#define ADV_BUF_LEN (sizeof(struct btp_gap_device_found_ev) + 2 * 31)

struct bt_conn_set_security {
    struct bt_conn *conn;
    struct k_work_delayable set_security_work;
};
static struct bt_conn_set_security  bt_sc;

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
extern struct bt_conn *bt_conn_lookup_addr_br(const bt_addr_t *peer);
#endif

static atomic_t current_settings[1];
struct bt_conn_auth_cb cb;
static uint8_t oob_legacy_tk[16] = { 0 };

static bool filter_list_in_use;

static void reconnect_work_handler(struct k_work *work);
static uint8_t connect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len);

struct reconnect_work
{
    struct k_work work;
    uint8_t last_connect_cmd_line[BTP_MTU];
    uint32_t last_connect_cmd_line_len;
    bt_addr_le_t addr;
} reconnect;

#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
static struct bt_le_oob oob_sc_local = { 0 };
static struct bt_le_oob oob_sc_remote = { 0 };
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) ||
           ((defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))))) */

/* connection parameters for rejection test */
#define REJECT_INTERVAL_MIN 0x0C80
#define REJECT_INTERVAL_MAX 0x0C80
#define REJECT_LATENCY 0x0000
#define REJECT_SUPERVISION_TIMEOUT 0x0C80

#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
static struct {
	bt_addr_le_t addr;
	bool supported;
} cars[CONFIG_BT_MAX_PAIRED];

static uint8_t read_car_cb(struct bt_conn *conn, uint8_t err,
			  struct bt_gatt_read_params *params, const void *data,
			  uint16_t length);

static struct bt_gatt_read_params read_car_params = {
		.func = read_car_cb,
		.by_uuid.uuid = BT_UUID_CENTRAL_ADDR_RES,
		.by_uuid.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE,
		.by_uuid.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE,
};
#endif /* CONFIG_BT_PRIVACY */


#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
static uint8_t read_car_cb(struct bt_conn *conn, uint8_t err,
			  struct bt_gatt_read_params *params, const void *data,
			  uint16_t length)
{
	struct bt_conn_info info;
	bool supported = false;

    (void)memset(&info, 0x0, sizeof(struct bt_conn_info));
	if (!err && data && length == 1) {
		const uint8_t *tmp = data;

		/* only 0 or 1 are valid values */
		if (tmp[0] == 1) {
			supported = true;
		}
	}

    (void)bt_conn_get_info(conn, &info);

	for (int i = 0; i < CONFIG_BT_MAX_PAIRED; i++) {
        if ((info.le.dst != NULL) && bt_addr_le_eq(info.le.dst, &cars[i].addr)) {
			cars[i].supported = supported;
			break;
		}
	}

	return BT_GATT_ITER_STOP;
}
#endif /* CONFIG_BT_PRIVACY */

static bt_addr_le_t *bt_conn_get_peer_addr(struct bt_conn *conn)
{
	static bt_addr_le_t addr;
    struct bt_conn_info info = {(enum bt_conn_type)0};

    (void)bt_conn_get_info(conn, &info);

	memset(&addr, 0, sizeof(addr));

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (BT_CONN_TYPE_BR == info.type)
    {
        addr.type = BT_ADDR_LE_PUBLIC;
		if (info.br.dst != NULL)
		{
        	bt_addr_copy(&addr.a, info.br.dst);
		}
    }
    else
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
    {
		if (info.le.dst != NULL)
		{
        	bt_addr_le_copy(&addr, info.le.dst);
		}
    }
	return &addr;
}

static void le_connected(struct bt_conn *conn, uint8_t err)
{
	struct btp_gap_device_connected_ev ev = {0};
	struct bt_conn_info info;

    (void)memset(&info, 0x0, sizeof(struct bt_conn_info));
	if (err) {
		return;
	}

    (void)bt_conn_get_info(conn, &info);

#if 0
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (BT_CONN_TYPE_BR == info.type)
    {
        bt_conn_set_security(conn, BT_SECURITY_L2);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
#endif

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (BT_CONN_TYPE_BR == info.type)
    {
        bt_br_set_discoverable(false);
        bt_br_set_connectable(false);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

#if 0
    if (info.le.dst != NULL)
    {
        bt_addr_le_copy(&ev.address, info.le.dst);
    }
#endif
	bt_addr_le_copy(&ev.address, bt_conn_get_peer_addr(conn));

	ev.interval = sys_cpu_to_le16(info.le.interval);
	ev.latency = sys_cpu_to_le16(info.le.latency);
	ev.timeout = sys_cpu_to_le16(info.le.timeout);

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_DEVICE_CONNECTED, &ev, sizeof(ev));

	if (info.role == BT_CONN_ROLE_PERIPHERAL) {
		struct btp_gap_new_settings_ev sev;

		atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING);
		sev.current_settings = sys_cpu_to_le32(current_settings[0]);
		tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_NEW_SETTINGS, &sev, sizeof(sev));
	}
}

static void reconnect_work_handler(struct k_work *work)
{
    struct reconnect_work *rw = CONTAINER_OF(work, struct reconnect_work, work);
    struct btp_gap_device_disconnected_ev ev = {0};

    if (rw->last_connect_cmd_line_len > 0)
    {
        uint8_t ret = connect(rw->last_connect_cmd_line, rw->last_connect_cmd_line_len, NULL, 0);
        if (BTP_STATUS_SUCCESS == ret)
        {
            return;
        }
    }
    rw->last_connect_cmd_line_len = 0;
    bt_addr_le_copy(&ev.address, &rw->addr);
    tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_DEVICE_DISCONNECTED, &ev, sizeof(ev));
}

static void le_disconnected(struct bt_conn *conn, uint8_t reason)
{
    struct btp_gap_device_disconnected_ev ev = {0};
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);

    if (addr != NULL) {
        bt_addr_le_copy(&ev.address, addr);
    }

    if (BT_HCI_ERR_CONN_FAIL_TO_ESTAB == reason)
    {
        k_work_cancel(&reconnect.work);
        if (addr != NULL) {
            bt_addr_le_copy(&reconnect.addr, addr);
        }
        else
        {
            bt_addr_le_copy(&reconnect.addr, BT_ADDR_LE_ANY);
        }
        k_work_submit(&reconnect.work);
        return;
    }

    tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_DEVICE_DISCONNECTED, &ev, sizeof(ev));
}

static void le_identity_resolved(struct bt_conn *conn, const bt_addr_le_t *rpa,
				 const bt_addr_le_t *identity)
{
	struct btp_gap_identity_resolved_ev ev;

	bt_addr_le_copy(&ev.address, rpa);
	bt_addr_le_copy(&ev.identity_address, identity);

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_IDENTITY_RESOLVED, &ev, sizeof(ev));
}

static void le_param_updated(struct bt_conn *conn, uint16_t interval,
			     uint16_t latency, uint16_t timeout)
{
    struct btp_gap_conn_param_update_ev ev = {0};
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);

    if (addr != NULL)
    {
        bt_addr_le_copy(&ev.address, addr);
    }
	ev.interval = sys_cpu_to_le16(interval);
	ev.latency = sys_cpu_to_le16(latency);
	ev.timeout = sys_cpu_to_le16(timeout);

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_CONN_PARAM_UPDATE, &ev, sizeof(ev));
}

static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	/* reject update if all parameters match reject pattern */
	if ((param->interval_min == REJECT_INTERVAL_MIN) &&
			(param->interval_max == REJECT_INTERVAL_MAX) &&
			(param->latency == REJECT_LATENCY) &&
			(param->timeout == REJECT_SUPERVISION_TIMEOUT)) {
		return false;
	}

	return true;
}

static void le_conn_set_security_delayed_work(struct k_work *work)
{
    struct k_work_delayable *dwork = k_work_delayable_from_work(work);
    struct bt_conn_set_security *bt_con_sc = CONTAINER_OF(dwork, struct bt_conn_set_security,
                                          set_security_work);
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    (void)bt_conn_set_security(bt_con_sc->conn, (bt_security_t)(BT_SECURITY_L2 | BT_SECURITY_FORCE_PAIR));
#endif /* CONFIG_BT_SMP */
}

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
static uint8_t br_smp_authenticate(const void *cmd, uint16_t cmd_len,
    void *rsp, uint16_t *rsp_len)
{
    const struct btp_sm_auth_cmd *cp = cmd;
    struct bt_conn *conn;
    uint8_t status;
    int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }

    bt_smp_set_ct2(conn, cp->useh7 ? 1:0);
    bt_smp_br_send_pairing_req(conn);
    return 0;
}
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
static void le_security_changed(struct bt_conn *conn, bt_security_t level,
				enum bt_security_err err)
{
#if 0
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);
    struct btp_gap_sec_level_changed_ev sec_ev = {0};
    struct btp_gap_bond_lost_ev bond_ev = {0};
    struct bt_conn_info info;
    (void)memset(&info, 0x0, sizeof(struct bt_conn_info));

	switch (err) {
	case BT_SECURITY_ERR_SUCCESS:
            if (addr != NULL)
            {
                bt_addr_le_copy(&sec_ev.address, addr);
            }
            /* BTP spec values for Security Level:
              Possible values for the Security Level parameter map to those
              defined in Core Specification in LE security mode 1:
                                      1 = Unauthenticated pairing with encryption
                                      2 = Authenticated pairing with encryption
                                      3 = Authenticated LE Secure Connections pairing
                                      with encryption using a 128-bit strength encryption key
              We will send 0 for anything else.
            */
            switch (level)
            {
                case BT_SECURITY_L2:
                    sec_ev.sec_level = 1;
                break;

                case BT_SECURITY_L3:
                    sec_ev.sec_level = 2;
                break;

                case BT_SECURITY_L4:
                    sec_ev.sec_level = 3;
                break;

                default:
                case BT_SECURITY_L0:
                case BT_SECURITY_L1:
                    sec_ev.sec_level = 0;
                break;
            }

		tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_SEC_LEVEL_CHANGED,
			     &sec_ev, sizeof(sec_ev));
		break;
	case BT_SECURITY_ERR_PIN_OR_KEY_MISSING:
		/* for central role this means that peer have no LTK when we
		 * started encryption procedure
		 *
		 * This means bond is lost and we restart pairing to re-bond
		 */
		if (bt_conn_get_info(conn, &info) == 0 &&
		    info.role == BT_CONN_ROLE_CENTRAL) {
                if (addr != NULL)
                {
                    bt_addr_le_copy(&bond_ev.address, addr);
                }

			tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_BOND_LOST,
				     &bond_ev, sizeof(bond_ev));
            bt_sc.conn = conn;
            k_work_reschedule(&bt_sc.set_security_work, BT_SECONDS(10));

		}
		break;
	default:
		break;
	}
}

static struct bt_conn_cb conn_callbacks = {
	.connected = le_connected,
	.disconnected = le_disconnected,
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
	.identity_resolved = le_identity_resolved,
#endif /* CONFIG_BT_SMP */
	.le_param_updated = le_param_updated,
	.le_param_req = le_param_req,
#if ((defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U)) || (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U)))
	.security_changed = le_security_changed,
#endif /* CONFIG_BT_SMP || CONFIG_BT_CLASSIC */
};

static uint8_t supported_commands(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	struct btp_gap_read_supported_commands_rp *rp = rsp;

	/* octet 0 */
	tester_set_bit(rp->data, BTP_GAP_READ_SUPPORTED_COMMANDS);
	tester_set_bit(rp->data, BTP_GAP_READ_CONTROLLER_INDEX_LIST);
	tester_set_bit(rp->data, BTP_GAP_READ_CONTROLLER_INFO);
	tester_set_bit(rp->data, BTP_GAP_SET_CONNECTABLE);

	/* octet 1 */
	tester_set_bit(rp->data, BTP_GAP_SET_DISCOVERABLE);
	tester_set_bit(rp->data, BTP_GAP_SET_BONDABLE);
	tester_set_bit(rp->data, BTP_GAP_START_ADVERTISING);
	tester_set_bit(rp->data, BTP_GAP_STOP_ADVERTISING);
	tester_set_bit(rp->data, BTP_GAP_START_DISCOVERY);
	tester_set_bit(rp->data, BTP_GAP_STOP_DISCOVERY);
	tester_set_bit(rp->data, BTP_GAP_CONNECT);
	tester_set_bit(rp->data, BTP_GAP_DISCONNECT);

	/* octet 2 */
	tester_set_bit(rp->data, BTP_GAP_SET_IO_CAP);
	tester_set_bit(rp->data, BTP_GAP_PAIR);
	tester_set_bit(rp->data, BTP_GAP_PASSKEY_ENTRY);
	tester_set_bit(rp->data, BTP_GAP_PASSKEY_CONFIRM);
	tester_set_bit(rp->data, BTP_GAP_START_DIRECTED_ADV);
	tester_set_bit(rp->data, BTP_GAP_CONN_PARAM_UPDATE);

	/* octet 3 */
	tester_set_bit(rp->data, BTP_GAP_OOB_LEGACY_SET_DATA);
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
	tester_set_bit(rp->data, BTP_GAP_OOB_SC_GET_LOCAL_DATA);
	tester_set_bit(rp->data, BTP_GAP_OOB_SC_SET_REMOTE_DATA);
#endif /* !defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) */
	tester_set_bit(rp->data, BTP_GAP_SET_MITM);
	tester_set_bit(rp->data, BTP_GAP_SET_FILTER_LIST);
#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0U))
	tester_set_bit(rp->data, BTP_GAP_SET_EXTENDED_ADVERTISING);
#endif /* CONFIG_BT_EXT_ADV */

	*rsp_len = sizeof(*rp) + 4;

	return BTP_STATUS_SUCCESS;
}

static uint8_t controller_index_list(const void *cmd, uint16_t cmd_len,
				     void *rsp, uint16_t *rsp_len)
{
	struct btp_gap_read_controller_index_list_rp *rp = rsp;

	rp->num = 1U;
	rp->index[0] = BTP_INDEX;

	*rsp_len = sizeof(*rp) + 1;

	return BTP_STATUS_SUCCESS;
}

static uint8_t controller_info(const void *cmd, uint16_t cmd_len,
			       void *rsp, uint16_t *rsp_len)
{
	struct btp_gap_read_controller_info_rp *rp = rsp;
	uint32_t supported_settings;
	struct bt_le_oob oob_local = { 0 };

	bt_le_oob_get_local(BT_ID_DEFAULT, &oob_local);

	bt_addr_copy(&rp->address, &oob_local.addr.a);

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
	/*
	 * Re-use the oob data read here in get_oob_sc_local_data()
	 */
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
	oob_sc_local = oob_local;
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
           (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))) */
#endif /* CONFIG_BT_SMP */
	/*
	 * If privacy is used, the device uses random type address, otherwise
	 * static random or public type address is used.
	 */
#if (!defined(CONFIG_BT_PRIVACY) || (CONFIG_BT_PRIVACY == 0U))
	if (oob_local.addr.type == BT_ADDR_LE_RANDOM) {
        atomic_set_bit(current_settings, BTP_GAP_SETTINGS_STATIC_ADDRESS);
	}
#endif /* CONFIG_BT_PRIVACY */

	supported_settings = BIT(BTP_GAP_SETTINGS_POWERED);
	supported_settings |= BIT(BTP_GAP_SETTINGS_CONNECTABLE);
	supported_settings |= BIT(BTP_GAP_SETTINGS_BONDABLE);
	supported_settings |= BIT(BTP_GAP_SETTINGS_LE);
	supported_settings |= BIT(BTP_GAP_SETTINGS_ADVERTISING);
	supported_settings |= BIT(BTP_GAP_SETTINGS_EXTENDED_ADVERTISING);

	rp->supported_settings = sys_cpu_to_le32(supported_settings);
    rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	memcpy(rp->name, CONTROLLER_NAME, sizeof(CONTROLLER_NAME));

	*rsp_len = sizeof(*rp);

	return BTP_STATUS_SUCCESS;
}

#if 0
/* used for log debug in zephyr, not used */
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
static const char *oob_config_str(int oob_config)
{
	switch (oob_config) {
	case BT_CONN_OOB_LOCAL_ONLY:
		return "Local";
	case BT_CONN_OOB_REMOTE_ONLY:
		return "Remote";
	case BT_CONN_OOB_BOTH_PEERS:
		return "Local and Remote";
	case BT_CONN_OOB_NO_DATA:
	default:
		return "no";
	}
}
#endif /* !defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) */
#endif

static void oob_data_request(struct bt_conn *conn,
			     struct bt_conn_oob_info *oob_info)
{
	struct bt_conn_info info;
	int err = bt_conn_get_info(conn, &info);

	if (err) {
		return;
	}

	char addr[BT_ADDR_LE_STR_LEN];

    (void)bt_addr_le_to_str(info.le.dst, addr, sizeof(addr));

	switch (oob_info->type) {
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
	case BT_CONN_OOB_LE_SC:
	{
#if 0
		LOG_DBG("Set %s OOB SC data for %s, ",
			oob_config_str(oob_info->lesc.oob_config),
			addr);
#endif

		struct bt_le_oob_sc_data *oobd_local =
			oob_info->lesc.oob_config != BT_CONN_OOB_REMOTE_ONLY ?
				      &oob_sc_local.le_sc_data :
				      NULL;

		struct bt_le_oob_sc_data *oobd_remote =
			oob_info->lesc.oob_config != BT_CONN_OOB_LOCAL_ONLY ?
				      &oob_sc_remote.le_sc_data :
				      NULL;

		if (oobd_remote) {
			/* Assume that oob_sc_remote
			 * corresponds to the currently connected peer
			 */
			bt_addr_le_copy(&oob_sc_remote.addr, info.le.remote);
		}

		if (oobd_local &&
		    !bt_addr_le_eq(info.le.local, &oob_sc_local.addr)) {
			bt_addr_le_to_str(info.le.local, addr, sizeof(addr));
#if 0
			LOG_DBG("No OOB data available for local %s",
				addr);
#endif
			bt_conn_auth_cancel(conn);
			return;
		}

        if (oobd_remote != NULL) {
            err = bt_le_oob_set_sc_data(conn, oobd_local, oobd_remote);
	    	if (err) {
			    LOG_DBG("bt_le_oob_set_sc_data failed with: %d", err);
		    }
        }

		break;
	}
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
           (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))) */

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
#if ((!defined(CONFIG_BT_SMP_SC_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_SC_PAIR_ONLY) && (CONFIG_BT_SMP_SC_PAIR_ONLY == 0U)))
	case BT_CONN_OOB_LE_LEGACY:

		err = bt_le_oob_set_legacy_tk(conn, oob_legacy_tk);
		if (err < 0) {
			LOG_ERR("Failed to set OOB TK: %d", err);
		}

		break;
#endif /* !defined(CONFIG_BT_SMP_SC_PAIR_ONLY) */
#endif /* CONFIG_BT_SMP */
	default:
        PRINTF("Unhandled OOB type %d", oob_info->type);
		break;
	}
}

#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
static uint8_t get_oob_sc_local_data(const void *cmd, uint16_t cmd_len,
				     void *rsp, uint16_t *rsp_len)
{
	struct btp_gap_oob_sc_get_local_data_rp *rp = rsp;

	cb.oob_data_request = oob_data_request;

	memcpy(rp->conf, &oob_sc_local.le_sc_data.c[0], sizeof(rp->conf));
	memcpy(rp->rand, &oob_sc_local.le_sc_data.r[0], sizeof(rp->rand));

	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

static uint8_t set_oob_sc_remote_data(const void *cmd, uint16_t cmd_len,
				      void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_oob_sc_set_remote_data_cmd *cp = cmd;

	cb.oob_data_request = oob_data_request;
	bt_le_oob_set_sc_flag(true);

	/* Note that the .addr field
	 * will be set by the oob_data_request callback
	 */
	memcpy(&oob_sc_remote.le_sc_data.r[0], cp->rand,
	       sizeof(oob_sc_remote.le_sc_data.r));
	memcpy(&oob_sc_remote.le_sc_data.c[0], cp->conf,
	       sizeof(oob_sc_remote.le_sc_data.c));

	return BTP_STATUS_SUCCESS;
}
#endif /* !defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) */

static uint8_t set_connectable(const void *cmd, uint16_t cmd_len,
			       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_set_connectable_cmd *cp = cmd;
	struct btp_gap_set_connectable_rp *rp = rsp;

	if (cp->connectable) {
		atomic_set_bit(current_settings, BTP_GAP_SETTINGS_CONNECTABLE);
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
        (void)bt_br_set_connectable(true);
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
	} else {
		atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_CONNECTABLE);
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
        (void)bt_br_set_connectable(false);
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
	}

    rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);

	return BTP_STATUS_SUCCESS;
}

#if ((defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U)))
static uint8_t ad_flags = 0;
#else
static uint8_t ad_flags = BT_LE_AD_NO_BREDR;
#endif /* CONFIG_BT_CLASSIC */

static struct bt_data ad[10] = {
	BT_DATA(BT_DATA_FLAGS, &ad_flags, sizeof(ad_flags)),
};
static struct bt_data sd[10];

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0U))
static struct bt_le_ext_adv *ext_adv;

struct bt_le_ext_adv *tester_gap_ext_adv_get(void)
{
	return ext_adv;
}

int tester_gap_start_ext_adv(void)
{
	int err;

	err = bt_le_ext_adv_start(ext_adv, BT_LE_EXT_ADV_START_DEFAULT);
	if (err != 0) {

		return -EINVAL;
	}

	atomic_set_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING);

	return 0;
}

int tester_gap_stop_ext_adv(void)
{
	int err;

	err = bt_le_ext_adv_stop(ext_adv);
	if (err != 0) {
		return -EINVAL;
	}

	atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING);

	return 0;
}
#endif /* defined(CONFIG_BT_EXT_ADV) */

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
struct k_work_delayable limited_discoverable_work;

void limited_discoverable_work_handler(struct k_work *work)
{
    struct btp_gap_set_discoverable_rp rsp;

    int err = bt_br_set_discoverable(false);

    if (err == 0)
    {
        err = bt_br_set_connectable(false);
    }

    if (err == 0)
    {
        ad_flags &= ~(BT_LE_AD_GENERAL | BT_LE_AD_LIMITED);
        atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_DISCOVERABLE);

        rsp.current_settings = sys_cpu_to_le32(current_settings[0]);

        tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_NEW_SETTINGS, &rsp, sizeof(rsp));
    }
}
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

static uint8_t set_discoverable(const void *cmd, uint16_t cmd_len,
			       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_set_discoverable_cmd *cp = cmd;
	struct btp_gap_set_discoverable_rp *rp = rsp;

	switch (cp->discoverable) {
	case BTP_GAP_NON_DISCOVERABLE:
		ad_flags &= ~(BT_LE_AD_GENERAL | BT_LE_AD_LIMITED);
		atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_DISCOVERABLE);
		break;
	case BTP_GAP_GENERAL_DISCOVERABLE:
		ad_flags &= ~BT_LE_AD_LIMITED;
		ad_flags |= BT_LE_AD_GENERAL;
		atomic_set_bit(current_settings, BTP_GAP_SETTINGS_DISCOVERABLE);
		break;
	case BTP_GAP_LIMITED_DISCOVERABLE:
		ad_flags &= ~BT_LE_AD_GENERAL;
		ad_flags |= BT_LE_AD_LIMITED;
		atomic_set_bit(current_settings, BTP_GAP_SETTINGS_DISCOVERABLE);
		break;
	default:
		return BTP_STATUS_FAILED;
	}

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
	if ((BTP_GAP_GENERAL_DISCOVERABLE == cp->discoverable) || (BTP_GAP_LIMITED_DISCOVERABLE == cp->discoverable))
	{
		int err;

		struct net_buf *buf = NULL;
		struct bt_hci_cp_write_class_of_device *cod;
		struct bt_hci_cp_write_current_iac_lap *iac;
		struct bt_hci_cp_write_extended_inquiry_response *eir_response;
		uint32_t cod_value = 0x000000U;
		uint32_t num_iacs = 1U;

		if (BTP_GAP_LIMITED_DISCOVERABLE == cp->discoverable)
		{
			cod_value = 0x002000U;
			num_iacs = 2U;
		}

#if (defined(CONFIG_BT_AUDIO) && (CONFIG_BT_AUDIO > 0))
		cod_value += BIT(14);
#endif /* CONFIG_BT_AUDIO */

		buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cod));
		if (buf != NULL)
		{
			cod = net_buf_add(buf, sizeof(*cod));
			sys_put_le24(cod_value, &cod->class_of_device[0]);
			err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
		}
		else
		{
			err = -ENOBUFS;
		}

		if (err == 0)
		{
			buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CURRENT_IAC_LAP, sizeof(*iac) + 3 * num_iacs);
			if (buf != NULL)
			{
				iac = net_buf_add(buf, sizeof(*iac) + 3 * num_iacs);
				iac->num_current_iac = num_iacs;
				sys_put_le24(0x9e8b33U, &iac->lap[0].iac[0]);
				if (num_iacs > 1U)
				{
					sys_put_le24(0x9e8b00U, &iac->lap[1].iac[0]);
				}

				err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
			}
			else
			{
				err = -ENOBUFS;
			}
		}

		bt_br_set_discoverable(false);
		bt_br_set_connectable(false);

		if (err == 0)
		{
			buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_EXTENDED_INQUIRY_RESPONSE, sizeof(*eir_response));
			if (buf != NULL)
			{
				eir_response = net_buf_add(buf, sizeof(*eir_response));
				eir_response->fec_required = 0x00U;
				memset(&eir_response->Extended_Inquiry_Response[0], 0, sizeof(eir_response->Extended_Inquiry_Response));
				eir_response->Extended_Inquiry_Response[0] = 1 + strlen(CONFIG_BT_DEVICE_NAME);
				eir_response->Extended_Inquiry_Response[1] = BT_DATA_NAME_COMPLETE;
				memcpy(&eir_response->Extended_Inquiry_Response[2], CONFIG_BT_DEVICE_NAME, strlen(CONFIG_BT_DEVICE_NAME));

				err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_EXTENDED_INQUIRY_RESPONSE, buf, NULL);
			}
			else
			{
				err = -ENOBUFS;
			}
		}

		if ((err == 0))
		{
			err = bt_br_set_connectable(true);
		}

		if ((err == 0))
		{
			err = bt_br_set_discoverable(true);
		}

		if ((err != 0))
		{
			ad_flags &= ~(BT_LE_AD_GENERAL | BT_LE_AD_LIMITED);
			atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_DISCOVERABLE);
			bt_br_set_discoverable(false);
			bt_br_set_connectable(false);
		}

		if ((err == 0))
		{
			static int init = 0;
			if (init == 0)
			{
				k_work_init_delayable(&limited_discoverable_work, limited_discoverable_work_handler);
				init = 1;
			}
			else
			{
				k_work_cancel_delayable(&limited_discoverable_work);
			}

			if (0 != k_work_reschedule(&limited_discoverable_work, BT_SECONDS(31)))
			{
				ad_flags &= ~(BT_LE_AD_GENERAL | BT_LE_AD_LIMITED);
				atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_DISCOVERABLE);
				bt_br_set_discoverable(false);
				bt_br_set_connectable(false);
				err = -EIO;
			}
		}

		if ((err != 0))
		{
			return BTP_STATUS_FAILED;
		}
	}
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

static uint8_t set_bondable(const void *cmd, uint16_t cmd_len,
			    void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_set_bondable_cmd *cp = cmd;
	struct btp_gap_set_bondable_rp *rp = rsp;

	if (cp->bondable) {
        atomic_set_bit(current_settings, BTP_GAP_SETTINGS_BONDABLE);
	} else {
        atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_BONDABLE);
	}
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
	bt_set_bondable(cp->bondable);
#endif /* CONFIG_BT_SMP */

    rp->current_settings = sys_cpu_to_le32(current_settings[0]);
	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

int tester_gap_create_adv_instance(struct bt_le_adv_param *param, uint8_t own_addr_type,
				   const struct bt_data *ad, size_t ad_len,
				   const struct bt_data *sd, size_t sd_len,
				   uint32_t *settings)
{
	int err = 0;

	if (settings != NULL) {
		atomic_set(current_settings, *settings);
	}

	if (atomic_test_bit(current_settings, BTP_GAP_SETTINGS_CONNECTABLE)) {
		param->options |= BT_LE_ADV_OPT_CONNECTABLE;

		if (filter_list_in_use) {
			param->options |= BT_LE_ADV_OPT_FILTER_CONN;
		}
	}

	if (filter_list_in_use) {
		param->options |= BT_LE_ADV_OPT_FILTER_SCAN_REQ;
	}

	switch (own_addr_type) {
	case BTP_GAP_ADDR_TYPE_IDENTITY:
		param->options |= BT_LE_ADV_OPT_USE_IDENTITY;
		break;
#if defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0)
	case BTP_GAP_ADDR_TYPE_RESOLVABLE_PRIVATE:
		/* RPA usage is is controlled via privacy settings */
		if (!atomic_test_bit(current_settings, BTP_GAP_SETTINGS_PRIVACY)) {
			return -EINVAL;
		}
		break;
	case BTP_GAP_ADDR_TYPE_NON_RESOLVABLE_PRIVATE:
		/* NRPA is used only for non-connectable advertising */
		if (atomic_test_bit(current_settings, BTP_GAP_SETTINGS_CONNECTABLE)) {
			return -EINVAL;
		}
		break;
#endif
	default:
		return -EINVAL;
	}

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0U))
	if (IS_ENABLED(CONFIG_BT_EXT_ADV) && atomic_test_bit(current_settings,
	    BTP_GAP_SETTINGS_EXTENDED_ADVERTISING)) {
		param->options |= BT_LE_ADV_OPT_EXT_ADV;
		if (ext_adv != NULL) {
			err = bt_le_ext_adv_stop(ext_adv);
			if (err != 0) {
				return err;
			}

			err = bt_le_ext_adv_delete(ext_adv);
			if (err != 0) {
				return err;
			}

			ext_adv = NULL;
		}

		err = bt_le_ext_adv_create(param, NULL, &ext_adv);
		if (err != 0) {
			return BTP_STATUS_FAILED;
		}

		err = bt_le_ext_adv_set_data(ext_adv, ad, ad_len, sd_len ? sd : NULL, sd_len);
	}
#endif

	return err;
}

static uint8_t start_advertising(const void *cmd, uint16_t cmd_len,
				 void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_start_advertising_cmd *cp = cmd;
	struct btp_gap_start_advertising_rp *rp = rsp;
	struct bt_le_adv_param param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_ONE_TIME,
							    BT_GAP_ADV_FAST_INT_MIN_2,
							    BT_GAP_ADV_FAST_INT_MAX_2,
							    NULL);
	uint8_t own_addr_type;
	uint32_t duration;
	uint8_t adv_len;
	uint8_t sd_len;
	int err;
	int i;

	/* This command is very unfortunate since after variable data there is
	 * additional 5 bytes (4 bytes for duration, 1 byte for own address
	 * type.
	 */
	if ((cmd_len < sizeof(*cp)) ||
	    (cmd_len != sizeof(*cp) + cp->adv_data_len + cp->scan_rsp_len +
			    sizeof(duration) + sizeof(own_addr_type))) {
		return BTP_STATUS_FAILED;
	}

	/* currently ignored */
	duration = sys_get_le32(cp->adv_sr_data + cp->adv_data_len + cp->scan_rsp_len);
	(void)duration;
	own_addr_type = cp->adv_sr_data[cp->adv_data_len + cp->scan_rsp_len + sizeof(duration)];


	for (i = 0, adv_len = 1U; i < cp->adv_data_len; adv_len++) {
		if (adv_len >= ARRAY_SIZE(ad)) {
			PRINTF("ad[] Out of memory");
			return BTP_STATUS_FAILED;
		}

        ad[adv_len].type = cp->adv_sr_data[i++];
        ad[adv_len].data_len = cp->adv_sr_data[i++];
        ad[adv_len].data = &cp->adv_sr_data[i];
        i += ad[adv_len].data_len;
	}

	for (sd_len = 0U; i < cp->adv_data_len + cp->scan_rsp_len; sd_len++) {
		if (sd_len >= ARRAY_SIZE(sd)) {
			PRINTF("sd[] Out of memory");
			return BTP_STATUS_FAILED;
		}

		sd[sd_len].type = cp->adv_sr_data[i++];
		sd[sd_len].data_len = cp->adv_sr_data[i++];
		sd[sd_len].data = &cp->adv_sr_data[i];
		i += sd[sd_len].data_len;
	}

#if 0
    uint8_t plaintext_data[] = {0x0F, 0x09, 0x53, 0x68, 0x6F, 0x72, 0x74, 0x20, 0x4D, 0x69, 0x6E, 0x69, 0x2D, 0x42, 0x75, 0x73, 0x03, 0x19, 0x0A, 0x8C};
    uint8_t randomizer[] = {0x18, 0xE1, 0x57, 0xCA, 0xDE};
    uint8_t iv[] = {0x9E, 0x7A, 0x00, 0xEF, 0xB1, 0x7A, 0xE7, 0x46};
    uint8_t session_key[] = {0x57, 0xA9, 0xDA, 0x12, 0xD1, 0x2E, 0x6E, 0x13, 0x1E, 0x20, 0x61, 0x2A, 0xD1, 0x0A, 0x6A, 0x19};
    uint8_t output[sizeof(randomizer) + sizeof(plaintext_data) + 4 + 4] = {0};
    uint8_t sample_data[] = {0x1E, 0x31, 0x18, 0xE1, 0x57, 0xCA, 0xDE, 0x74, 0xE4, 0xDC, 0xAF, 0xDC, 0x51, 0xC7, 0x28, 0x28, 0x10, 0xC2, 0x21, 0x7F, 0x0E, 0x4C, 0xEF, 0x43, 0x43, 0x18, 0x1F, 0xBA, 0x00, 0x69, 0xCC};

    err = bt_test_ead_encrypt(session_key, iv, randomizer, plaintext_data, sizeof(plaintext_data), output);
    if (0 == err)
    {
        if (0 != memcmp(output, &sample_data[2], sizeof(sample_data) - 2))
        {
            PRINTF("Fail to encrypt adv data");
            return BTP_STATUS_FAILED;
        }
    }

    if (0 == err)
    {
        err = bt_ead_encrypt(session_key, iv, &cp->adv_sr_data[cp->adv_data_len], cp->scan_rsp_len, output);
    }

    if (0 != err)
    {
        PRINTF("Fail to encrypt adv data");
        return BTP_STATUS_FAILED;
    }
#if 0
    if (0 != err)
    {
        __BKPT(0);
    }
    if (0 != memcmp(output, &sample_data[2], sizeof(sample_data) - 2))
    {
        __BKPT(0);
    }
#endif

    sd[sd_len].type = BT_DATA_ENCRYPTED_AD_DATA;
    sd[sd_len].data_len = sizeof(randomizer) + cp->scan_rsp_len + 4;
    sd[sd_len].data = output;
    sd_len++;
#endif

	err = tester_gap_create_adv_instance(&param, own_addr_type, ad, adv_len, sd, sd_len, NULL);
	if (err != 0) {
		return BTP_STATUS_FAILED;
	}

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
	if (atomic_test_bit(current_settings, BTP_GAP_SETTINGS_EXTENDED_ADVERTISING)) {
		err = bt_le_ext_adv_start(ext_adv, BT_LE_EXT_ADV_START_DEFAULT);
#else
	if (0) {
#endif
	} else {
		err = bt_le_adv_start(&param, ad, adv_len, sd_len ? sd : NULL, sd_len);
	}

	/* BTP API don't allow to set empty scan response data. */
	if (err < 0) {
		PRINTF("Failed to start advertising");
		return BTP_STATUS_FAILED;
	}

	atomic_set_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING);
	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

static uint8_t start_directed_advertising(const void *cmd, uint16_t cmd_len,
					  void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_start_directed_adv_cmd *cp = cmd;
	struct btp_gap_start_directed_adv_rp *rp = rsp;
	struct bt_le_adv_param adv_param;
	uint16_t options = sys_le16_to_cpu(cp->options);

	adv_param = *BT_LE_ADV_CONN_DIR(&cp->address);

	if (!(options & BTP_GAP_START_DIRECTED_ADV_HD)) {
		adv_param.options |= BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY;
		adv_param.interval_max = BT_GAP_ADV_FAST_INT_MAX_2;
		adv_param.interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
	}

	if (options & BTP_GAP_START_DIRECTED_ADV_PEER_RPA) {
#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
		/* check if peer supports Central Address Resolution */
		for (int i = 0; i < CONFIG_BT_MAX_PAIRED; i++) {
			if (bt_addr_le_eq(&cp->address, &cars[i].addr)) {
				if (cars[i].supported) {
					adv_param.options |= BT_LE_ADV_OPT_DIR_ADDR_RPA;
				}
			}
		}
#endif
	}

	if (bt_le_adv_start(&adv_param, NULL, 0, NULL, 0) < 0) {
		PRINTF("Failed to start advertising");
		return BTP_STATUS_FAILED;
	}

	atomic_set_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING);
	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

static uint8_t stop_advertising(const void *cmd, uint16_t cmd_len,
				void *rsp, uint16_t *rsp_len)
{
	struct btp_gap_stop_advertising_rp *rp = rsp;
	int err;

	err = bt_le_adv_stop();
	if (err < 0) {
		tester_rsp(BTP_SERVICE_ID_GAP, BTP_GAP_STOP_ADVERTISING, BTP_STATUS_FAILED);
		PRINTF("Failed to stop advertising: %d", err);
		return BTP_STATUS_FAILED;
	}

	atomic_clear_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING);
	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

static uint8_t get_ad_flags(struct net_buf_simple *buf_ad)
{
	uint8_t len, i;

	/* Parse advertisement to get flags */
	for (i = 0U; i < buf_ad->len; i += len - 1) {
		len = buf_ad->data[i++];
		if (!len) {
			break;
		}

		/* Check if field length is correct */
		if (len > (buf_ad->len - i) || (buf_ad->len - i) < 1) {
			break;
		}

		switch (buf_ad->data[i++]) {
		case BT_DATA_FLAGS:
			return buf_ad->data[i];
		default:
			break;
		}
	}

	return 0;
}

static uint8_t discovery_flags;
static struct net_buf_simple *adv_buf = NET_BUF_SIMPLE(ADV_BUF_LEN);
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
static struct net_buf_simple *adv_buf_br = NET_BUF_SIMPLE(sizeof(struct bt_br_discovery_result));
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

static void store_adv(const bt_addr_le_t *addr, int8_t rssi,
		      struct net_buf_simple *buf_ad)
{
	struct btp_gap_device_found_ev *ev;

	/* cleanup */
	net_buf_simple_init(adv_buf, 0);

	ev = net_buf_simple_add(adv_buf, sizeof(*ev));

	bt_addr_le_copy(&ev->address, addr);
	ev->rssi = rssi;
	ev->flags = BTP_GAP_DEVICE_FOUND_FLAG_AD | BTP_GAP_DEVICE_FOUND_FLAG_RSSI;
	ev->eir_data_len = buf_ad->len;
	memcpy(net_buf_simple_add(adv_buf, buf_ad->len), buf_ad->data, buf_ad->len);
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t evtype,
			 struct net_buf_simple *buf_ad)
{
	/* if General/Limited Discovery - parse Advertising data to get flags */
	if (!(discovery_flags & BTP_GAP_DISCOVERY_FLAG_LE_OBSERVE) &&
	    (evtype != BT_GAP_ADV_TYPE_SCAN_RSP)) {
		uint8_t flags = get_ad_flags(buf_ad);

		/* ignore non-discoverable devices */
		if (!(flags & BT_LE_AD_DISCOV_MASK)) {
			return;
		}

		/* if Limited Discovery - ignore general discoverable devices */
		if ((discovery_flags & BTP_GAP_DISCOVERY_FLAG_LIMITED) &&
		    !(flags & BT_LE_AD_LIMITED)) {
			return;
		}
	}

	/* attach Scan Response data */
	if (evtype == BT_GAP_ADV_TYPE_SCAN_RSP) {
		struct btp_gap_device_found_ev *ev;
		bt_addr_le_t a;

		/* skip if there is no pending advertisement */
		if (!adv_buf->len) {
			return;
		}

		ev = (void *) adv_buf->data;

		bt_addr_le_copy(&a, &ev->address);

		/*
		 * in general, the Scan Response comes right after the
		 * Advertisement, but if not if send stored event and ignore
		 * this one
		 */
		if (!bt_addr_le_eq(addr, &a)) {
			goto done;
		}

		ev->eir_data_len += buf_ad->len;
		ev->flags |= BTP_GAP_DEVICE_FOUND_FLAG_SD;

		memcpy(net_buf_simple_add(adv_buf, buf_ad->len), buf_ad->data, buf_ad->len);

		goto done;
	}

	/*
	 * if there is another pending advertisement, send it and store the
	 * current one
	 */
	if (adv_buf->len) {
		tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_DEVICE_FOUND,
			    adv_buf->data, adv_buf->len);
		net_buf_simple_reset(adv_buf);
	}

	store_adv(addr, rssi, buf_ad);

	/* if Active Scan and scannable event - wait for Scan Response */
	if ((discovery_flags & BTP_GAP_DISCOVERY_FLAG_LE_ACTIVE_SCAN) &&
	    (evtype == BT_GAP_ADV_TYPE_ADV_IND ||
	     evtype == BT_GAP_ADV_TYPE_ADV_SCAN_IND)) {
		return;
	}
done:
	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_DEVICE_FOUND,
		    adv_buf->data, adv_buf->len);
	net_buf_simple_reset(adv_buf);
}

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
static void br_discovery_complete(struct bt_br_discovery_result *results,
				  size_t count)
{
    size_t index;
    static uint32_t br_discover_result_count;
    struct btp_gap_device_found_ev *ev;
    bt_addr_le_t a;

    br_discover_result_count = count;
    PRINTF("BR/EDR discovery complete\r\n");

    (void)br_discover_result_count;
    for (index = 0; index < count; index ++)
    {
        /* cleanup */
        net_buf_simple_init(adv_buf_br, 0);

        ev = (struct btp_gap_device_found_ev *)net_buf_simple_add(adv_buf_br, sizeof(*ev));

        bt_addr_copy(&ev->address.a, &results[index].addr);
        ev->address.type = BT_ADDR_LE_PUBLIC;
        ev->flags = BTP_GAP_DEVICE_FOUND_FLAG_AD | BTP_GAP_DEVICE_FOUND_FLAG_RSSI;
        ev->rssi = results[index].rssi;
	memcpy(ev->cod, results[index].cod, sizeof(ev->cod));
        ev->eir_data_len = 0U;
        for (uint32_t i = 0U; i < sizeof(results[index].eir);)
        {
            if (results[index].eir[i] != 0)
            {
                ev->eir_data_len += results[index].eir[i] + 1;
                i = ev->eir_data_len;
            }
            else
            {
                break;
            }
        }
        memcpy(net_buf_simple_add(adv_buf_br, ev->eir_data_len), results[index].eir, ev->eir_data_len);
        tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_DEVICE_FOUND,
                adv_buf_br->data, adv_buf_br->len);
    }
}
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

static uint8_t start_discovery(const void *cmd, uint16_t cmd_len,
			       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_start_discovery_cmd *cp = cmd;

	/* only LE scan is supported */
	if (cp->flags & BTP_GAP_DISCOVERY_FLAG_BREDR) {
		LOG_WRN("BR/EDR not supported");
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
        int err;
        struct bt_br_discovery_param param;
        #define APP_INQUIRY_NUM_RESPONSES (20)

        static struct bt_br_discovery_result br_discovery_results[APP_INQUIRY_NUM_RESPONSES];

        param.length = 10;
        if (cp->flags & BTP_GAP_DISCOVERY_FLAG_LIMITED)
        {
            param.limited = 1U;
        }
        else
        {
            param.limited = 0U;
        }

        memset(&br_discovery_results[0], 0, sizeof(br_discovery_results));
        err = bt_br_discovery_start(&param, br_discovery_results,
                                APP_INQUIRY_NUM_RESPONSES,
                                br_discovery_complete);
        if (err < 0)
        {
            PRINTF("Failed to start scanning");
            return BTP_STATUS_FAILED;
        }

        net_buf_simple_init(adv_buf_br, 0);
        discovery_flags = cp->flags;
        return BTP_STATUS_SUCCESS;
#else
		return BTP_STATUS_FAILED;
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
	}

	if (bt_le_scan_start(cp->flags & BTP_GAP_DISCOVERY_FLAG_LE_ACTIVE_SCAN ?
			     BT_LE_SCAN_ACTIVE : BT_LE_SCAN_PASSIVE,
			     device_found) < 0) {
		PRINTF("Failed to start scanning");
		return BTP_STATUS_FAILED;
	}

	net_buf_simple_init(adv_buf, 0);
	discovery_flags = cp->flags;

	return BTP_STATUS_SUCCESS;
}

static uint8_t stop_discovery(const void *cmd, uint16_t cmd_len,
			      void *rsp, uint16_t *rsp_len)
{
	int err;
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (discovery_flags & BTP_GAP_DISCOVERY_FLAG_BREDR)
    {
        err = bt_br_discovery_stop();
        if (err < 0) {
            PRINTF("Failed to stop discovery: %d", err);
            return BTP_STATUS_FAILED;
        }
        return BTP_STATUS_SUCCESS;
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
	err = bt_le_scan_stop();
	if (err < 0) {
        PRINTF("Failed to stop scanning: %d", err);
		return BTP_STATUS_FAILED;
	}

	return BTP_STATUS_SUCCESS;
}

static uint8_t connect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_connect_cmd *cp = cmd;
	int err;

	if (!bt_addr_le_eq(&cp->address, BT_ADDR_LE_ANY)) {
		struct bt_conn *conn;

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
        if (BTP_GAP_CONNECT_BREDR == cp->transport)
        {
            conn = bt_conn_create_br(&cp->address.a, BT_BR_CONN_PARAM_DEFAULT);
            if (NULL == conn) {
                PRINTF("Failed to create connection");
                return BTP_STATUS_FAILED;
            }
            bt_conn_unref(conn);
        }
        else
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */
        {
                err = bt_conn_le_create(&cp->address, BT_CONN_LE_CREATE_CONN,
                            BT_LE_CONN_PARAM_DEFAULT, &conn);
                if (err) {
                    PRINTF("Failed to create connection (%d)", err);
                    return BTP_STATUS_FAILED;
                }

                bt_conn_unref(conn);
        }
	} else {
		err = bt_conn_le_create_auto(BT_CONN_LE_CREATE_CONN,
					     BT_LE_CONN_PARAM_DEFAULT);
		if (err) {
            PRINTF("Failed to create auto connection (%d)", err);
			return BTP_STATUS_FAILED;
		}
	}

    memcpy(reconnect.last_connect_cmd_line, cmd, cmd_len);
    reconnect.last_connect_cmd_line_len = cmd_len;

	return BTP_STATUS_SUCCESS;
}

static uint8_t disconnect(const void *cmd, uint16_t cmd_len,
			  void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_disconnect_cmd *cp = cmd;
	struct bt_conn *conn;
	uint8_t status;

	conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

	if (!conn) {
        PRINTF("Unknown connection");
        status = BTP_STATUS_FAILED;
	} else {
        if (bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN)) {
            PRINTF("Failed to disconnect");
            status = BTP_STATUS_FAILED;
        } else {
            status = BTP_STATUS_SUCCESS;
        }

		bt_conn_unref(conn);
    }

	return status;
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    struct btp_gap_passkey_display_ev ev = {0};
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);

	bt_addr_le_copy(&ev.address, addr);
	ev.passkey = sys_cpu_to_le32(passkey);

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_PASSKEY_DISPLAY, &ev, sizeof(ev));
}

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U))
static void auth_pincode_entry(struct bt_conn *conn, bool highsec)
{
    char *pincode;
    int err;

    if (true == highsec)
    {
        pincode = (char *)"0000000000000000";
    }
    else
    {
        pincode = (char *)"0000";
    }
    err = bt_conn_auth_pincode_entry(conn, (char const *)pincode);
    if (err < 0)
    {
        (void)bt_conn_auth_cancel(conn);
    }
}
#endif /* (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U)) */

static void auth_passkey_entry(struct bt_conn *conn)
{
    struct btp_gap_passkey_entry_req_ev ev = {0};
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);

	bt_addr_le_copy(&ev.address, addr);

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_PASSKEY_ENTRY_REQ, &ev, sizeof(ev));
}

static void auth_passkey_confirm(struct bt_conn *conn, unsigned int passkey)
{
	struct btp_gap_passkey_confirm_req_ev ev;
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);

	bt_addr_le_copy(&ev.address, addr);
	ev.passkey = sys_cpu_to_le32(passkey);

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_PASSKEY_CONFIRM_REQ, &ev, sizeof(ev));
}

static void auth_cancel(struct bt_conn *conn)
{
	/* TODO */
}

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0U))
extern bool bt_addr_le_is_bonded(uint8_t id, const bt_addr_le_t *addr);

enum bt_security_err auth_pairing_accept(struct bt_conn *conn,
					 const struct bt_conn_pairing_feat *const feat)
{
    struct btp_gap_bond_lost_ev ev = {0};
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);

    if ((addr != NULL) && (!bt_addr_le_is_bonded(BT_ID_DEFAULT, addr))) {
		return BT_SECURITY_ERR_SUCCESS;
	}

	/* If a peer is already bonded and tries to pair again then it means that
	 * the it has lost its bond information.
	 */
    if (addr != NULL) {
        bt_addr_le_copy(&ev.address, addr);
    }

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_BOND_LOST, &ev, sizeof(ev));

	return BT_SECURITY_ERR_SUCCESS;
}
#endif /* CONFIG_BT_SMP_APP_PAIRING_ACCEPT */
#endif /* CONFIG_BT_SMP */

void auth_pairing_failed(struct bt_conn *conn, enum bt_security_err reason)
{
    struct btp_gap_bond_pairing_failed_ev ev = {0};
#if 0
	const bt_addr_le_t *addr = bt_conn_get_dst(conn);
#endif
	const bt_addr_le_t *addr = bt_conn_get_peer_addr(conn);
    if (addr != NULL) {
	    bt_addr_le_copy(&ev.address, addr);
    }
	ev.reason = reason;

	tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_PAIRING_FAILED, &ev, sizeof(ev));
}

static void auth_pairing_complete(struct bt_conn *conn, bool bonded)
{
#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
	/* Read peer's Central Address Resolution if bonded */
	if (bonded) {
        (void)bt_gatt_read(conn, &read_car_params);
	}
#endif /* CONFIG_BT_PRIVACY */
}

static struct bt_conn_auth_info_cb auth_info_cb = {
	.pairing_failed = auth_pairing_failed,
	.pairing_complete = auth_pairing_complete,
};

static uint8_t set_io_cap(const void *cmd, uint16_t cmd_len,
			  void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_set_io_cap_cmd *cp = cmd;
    uint8_t status = BTP_STATUS_SUCCESS;

	/* Reset io cap requirements */
	(void)memset(&cb, 0, sizeof(cb));
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
	bt_conn_auth_cb_register(NULL);
#endif /* CONFIG_BT_SMP */

	switch (cp->io_cap) {
	case BTP_GAP_IO_CAP_DISPLAY_ONLY:
		cb.cancel = auth_cancel;
		cb.passkey_display = auth_passkey_display;
		break;
	case BTP_GAP_IO_CAP_KEYBOARD_DISPLAY:
		cb.cancel = auth_cancel;
		cb.passkey_display = auth_passkey_display;
		cb.passkey_entry = auth_passkey_entry;
		cb.passkey_confirm = auth_passkey_confirm;
#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U))
        cb.pincode_entry = auth_pincode_entry;
#endif /* (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U)) */
		break;
	case BTP_GAP_IO_CAP_NO_INPUT_OUTPUT:
		cb.cancel = auth_cancel;
		break;
	case BTP_GAP_IO_CAP_KEYBOARD_ONLY:
		cb.cancel = auth_cancel;
		cb.passkey_entry = auth_passkey_entry;
#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U))
        cb.pincode_entry = auth_pincode_entry;
#endif /* (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U)) */
		break;
	case BTP_GAP_IO_CAP_DISPLAY_YESNO:
		cb.cancel = auth_cancel;
		cb.passkey_display = auth_passkey_display;
		cb.passkey_confirm = auth_passkey_confirm;
		break;
	default:
		status = BTP_STATUS_FAILED;
		break;
	}

    if (status != BTP_STATUS_FAILED)
    {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0U))
        cb.pairing_accept = auth_pairing_accept;
#endif /* CONFIG_BT_SMP_APP_PAIRING_ACCEPT */
        if (bt_conn_auth_cb_register(&cb)) {
            status = BTP_STATUS_FAILED;
        }
#endif /* CONFIG_BT_SMP */
    }

    return status;
}

static uint8_t pair(const void *cmd, uint16_t cmd_len,
		    void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_pair_cmd *cp = cmd;
	struct bt_conn *conn;
    uint8_t status;
	int err;

	conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

    if (!conn)
    {
        status = BTP_STATUS_FAILED;
    }
    else
    {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
        err = bt_conn_set_security(conn, BT_SECURITY_L2);
        if (err < 0)
        {
            bt_conn_unref(conn);
            status = BTP_STATUS_FAILED;
        }
        else
        {
            bt_conn_unref(conn);
            status = BTP_STATUS_SUCCESS;
        }
#endif /* CONFIG_BT_SMP */
    }

    return status;
}

static uint8_t unpair(const void *cmd, uint16_t cmd_len,
		      void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_unpair_cmd *cp = cmd;
	struct bt_conn *conn;
    uint8_t status;
	int err;

	conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

    if (!conn)
    {
        err = bt_unpair(BT_ID_DEFAULT, &cp->address);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
    }
    else
    {
        err = bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
        bt_conn_unref(conn);

        if (err < 0)
        {
            status = BTP_STATUS_FAILED;
        }
        else
        {
            err = bt_unpair(BT_ID_DEFAULT, &cp->address);
            status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
        }
    }

    return status;
}

static uint8_t passkey_entry(const void *cmd, uint16_t cmd_len,
			     void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_passkey_entry_cmd *cp = cmd;
	struct bt_conn *conn;
    uint8_t status;
	int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

    if (!conn)
    {
        status = BTP_STATUS_FAILED;
    }
    else
    {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
        err = bt_conn_auth_passkey_entry(conn, sys_le32_to_cpu(cp->passkey));
        bt_conn_unref(conn);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
#endif /* CONFIG_BT_SMP */
    }

    return status;
}

static uint8_t passkey_confirm(const void *cmd, uint16_t cmd_len,
			       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_passkey_confirm_cmd *cp = cmd;
	struct bt_conn *conn;
    uint8_t status;
	int err;

	conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

    if (!conn)
    {
        status = BTP_STATUS_FAILED;
    }
    else
    {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
        if (cp->match)
        {
            err = bt_conn_auth_passkey_confirm(conn);
        }
        else
        {
            err = bt_conn_auth_cancel(conn);
        }
#endif /* CONFIG_BT_SMP */
        bt_conn_unref(conn);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
    }


    return status;
}

static uint8_t conn_param_update(const void *cmd, uint16_t cmd_len,
				 void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_conn_param_update_cmd *cp = cmd;
	struct bt_le_conn_param param = {
		.interval_min = sys_le16_to_cpu(cp->interval_min),
		.interval_max = sys_le16_to_cpu(cp->interval_max),
		.latency = sys_le16_to_cpu(cp->latency),
		.timeout = sys_le16_to_cpu(cp->timeout),
	};
	struct bt_conn *conn;
    uint8_t status = BTP_STATUS_FAILED;
	int err;

	conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &cp->address);

#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    if (NULL == conn)
    {
        conn = bt_conn_lookup_addr_br(&cp->address.a);
    }
#endif /* defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U) */

    if (conn)
    {
        err = bt_conn_le_param_update(conn, &param);
        bt_conn_unref(conn);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
    }

    return status;
}

static uint8_t set_mitm(const void *cmd, uint16_t cmd_len,
			void *rsp, uint16_t *rsp_len)
{
	/* TODO verify if can be done in runtime */
	LOG_WRN("Use CONFIG_BT_SMP_ENFORCE_MITM instead");

	return BTP_STATUS_SUCCESS;
}

static uint8_t set_oob_legacy_data(const void *cmd, uint16_t cmd_len,
				   void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_oob_legacy_set_data_cmd *cp = cmd;

	memcpy(oob_legacy_tk, cp->oob_data, 16);
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
	bt_le_oob_set_legacy_flag(true);
#endif /* CONFIG_BT_SMP */
	cb.oob_data_request = oob_data_request;

	return BTP_STATUS_SUCCESS;
}

static uint8_t set_filter_list(const void *cmd, uint16_t cmd_len,
			       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_set_filter_list *cp = cmd;
    uint8_t status = BTP_STATUS_SUCCESS;
	int err;

	if ((cmd_len < sizeof(*cp)) ||
	    (cmd_len != sizeof(*cp) + (cp->cnt * sizeof(cp->addr[0])))) {
        status = BTP_STATUS_FAILED;
    }
    else
    {
        (void)bt_le_filter_accept_list_clear();

        for (int i = 0; i < cp->cnt; i++) {
	    	err = bt_le_filter_accept_list_add(&cp->addr[i]);
            if (err < 0) {
                status = BTP_STATUS_FAILED;
            }
        }
    }

	if (BTP_STATUS_SUCCESS == status) {
	    filter_list_in_use = cp->cnt != 0;
	}

	return status;
}

static uint8_t set_extended_advertising(const void *cmd, uint16_t cmd_len,
					void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_set_extended_advertising_cmd *cp = cmd;
	struct btp_gap_set_extended_advertising_rp *rp = rsp;

	LOG_DBG("ext adv settings: %u", cp->settings);

	if (cp->settings != 0) {
		atomic_set_bit(current_settings,
			       BTP_GAP_SETTINGS_EXTENDED_ADVERTISING);
	} else {
		atomic_clear_bit(current_settings,
				 BTP_GAP_SETTINGS_EXTENDED_ADVERTISING);
	}

	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

#if defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0U)
static struct bt_data padv[10];
static struct bt_le_per_adv_sync *pa_sync;

struct bt_le_per_adv_sync *tester_gap_padv_get(void)
{
	return pa_sync;
}

static void pa_sync_synced_cb(struct bt_le_per_adv_sync *sync,
			      struct bt_le_per_adv_sync_synced_info *info)
{
	LOG_DBG("");

	if (sync == pa_sync) {
		struct btp_gap_ev_periodic_sync_established_ev ev;

		bt_addr_le_copy(&ev.address, info->addr);
		ev.sync_handle = sys_cpu_to_le16(sync->handle);
		ev.status = 0;

		tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_PERIODIC_SYNC_ESTABLISHED,
			     &ev, sizeof(ev));
	}
}

static void pa_sync_terminated_cb(struct bt_le_per_adv_sync *sync,
				  const struct bt_le_per_adv_sync_term_info *info)
{
	LOG_DBG("");

	if (sync == pa_sync) {
		struct btp_gap_ev_periodic_sync_lost_ev ev;

		LOG_DBG("PA sync lost with reason %u", info->reason);
		pa_sync = NULL;

		ev.sync_handle = sys_cpu_to_le16(sync->handle);
		ev.reason = info->reason;

		tester_event(BTP_SERVICE_ID_GAP, BTP_GAP_EV_PERIODIC_SYNC_LOST,
			     &ev, sizeof(ev));
	}
}

static struct bt_le_per_adv_sync_cb pa_sync_cb = {
	.synced = pa_sync_synced_cb,
	.term = pa_sync_terminated_cb,
};

int tester_gap_padv_configure(const struct bt_le_per_adv_param *param)
{
	int err;
	struct bt_le_adv_param ext_adv_param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_ONE_TIME,
								    param->interval_min,
								    param->interval_max,
								    NULL);

	if (ext_adv == NULL) {
		current_settings[0] = BIT(BTP_GAP_SETTINGS_DISCOVERABLE) |
				   BIT(BTP_GAP_SETTINGS_EXTENDED_ADVERTISING);
		err = tester_gap_create_adv_instance(&ext_adv_param, BTP_GAP_ADDR_TYPE_IDENTITY, ad,
						     1, NULL, 0, NULL);
		if (err != 0) {
			return -EINVAL;
		}
	}

	/* Set periodic advertising parameters and the required
	 * bit in AD Flags of extended advertising.
	 */
	err = bt_le_per_adv_set_param(ext_adv, param);
	if (err != 0) {
	}

	return err;
}

static uint8_t padv_configure(const void *cmd, uint16_t cmd_len,
			      void *rsp, uint16_t *rsp_len)
{
	int err;
	uint32_t options = BT_LE_PER_ADV_OPT_NONE;
	const struct btp_gap_padv_configure_cmd *cp = cmd;
	struct btp_gap_padv_configure_rp *rp = rsp;

	if (cp->flags & BTP_GAP_PADV_INCLUDE_TX_POWER) {
		options |= BT_LE_PER_ADV_OPT_USE_TX_POWER;
	}

	err = tester_gap_padv_configure(BT_LE_PER_ADV_PARAM(sys_le16_to_cpu(cp->interval_min),
							    sys_le16_to_cpu(cp->interval_max),
							    options));
	if (err) {
		return BTP_STATUS_FAILED;
	}

	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);

	return BTP_STATUS_SUCCESS;
}

int tester_gap_padv_start(void)
{
	int err;

	if (ext_adv == NULL) {
		return -EINVAL;
	}

	if (!atomic_test_bit(current_settings, BTP_GAP_SETTINGS_ADVERTISING)) {
		err = tester_gap_start_ext_adv();
		if (err != 0) {
			return -EINVAL;
		}
	}

	/* Enable Periodic Advertising */
	err = bt_le_per_adv_start(ext_adv);
	if (err != 0) {
	}

	return err;
}

static uint8_t padv_start(const void *cmd, uint16_t cmd_len,
			  void *rsp, uint16_t *rsp_len)
{
	int err;
	struct btp_gap_padv_start_rp *rp = rsp;

	err = tester_gap_padv_start();

	if (err) {
		return BTP_STATUS_FAILED;
	}

	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);

	return BTP_STATUS_SUCCESS;
}

int tester_gap_padv_stop(void)
{
	int err;

	if (ext_adv == NULL) {
		return -EINVAL;
	}

	/* Enable Periodic Advertising */
	err = bt_le_per_adv_stop(ext_adv);
	if (err != 0) {
	}

	return err;
}

static uint8_t padv_stop(const void *cmd, uint16_t cmd_len,
			 void *rsp, uint16_t *rsp_len)
{
	int err;
	struct btp_gap_padv_stop_rp *rp = rsp;

	err = tester_gap_padv_stop();

	if (err) {
		return BTP_STATUS_FAILED;
	}

	rp->current_settings = sys_cpu_to_le32(current_settings[0]);

	*rsp_len = sizeof(*rp);

	return BTP_STATUS_SUCCESS;
}

int tester_gap_padv_set_data(struct bt_data *per_ad, uint8_t ad_len)
{
	int err;

	if (ext_adv == NULL) {
		return -EINVAL;
	}

	/* Set Periodic Advertising data */
	err = bt_le_per_adv_set_data(ext_adv, per_ad, ad_len);
	if (err != 0) {
	}

	return err;
}

static uint8_t padv_set_data(const void *cmd, uint16_t cmd_len,
			     void *rsp, uint16_t *rsp_len)
{
	int err;
	uint8_t padv_len = 0U;
	const struct btp_gap_padv_set_data_cmd *cp = cmd;

	for (uint8_t i = 0; i < cp->data_len; padv_len++) {
		if (padv_len >= ARRAY_SIZE(padv)) {
			return BTP_STATUS_FAILED;
		}

		padv[padv_len].data_len = cp->data[i++] - 1;
		padv[padv_len].type = cp->data[i++];
		padv[padv_len].data = &cp->data[i];
		i += padv[padv_len].data_len;
	}

	err = tester_gap_padv_set_data(padv, padv_len);
	if (err != 0) {
		return BTP_STATUS_FAILED;
	}

	return BTP_STATUS_SUCCESS;
}

int tester_gap_padv_create_sync(struct bt_le_per_adv_sync_param *create_params)
{
	int err;

	if (pa_sync != NULL) {
		return -EBUSY;
	}

	err = bt_le_per_adv_sync_create(create_params, &pa_sync);

	if (err != 0) {
		LOG_DBG("Unable to sync to PA: %d", err);
	}

	return err;
}

int tester_gap_padv_stop_sync(void)
{
	int err;

	if (pa_sync == NULL) {
		return -EALREADY;
	}

	err = bt_le_per_adv_sync_delete(pa_sync);
	if (err != 0) {
		LOG_DBG("Unable to stop sync to PA: %d", err);
	}

	return err;
}

static uint8_t padv_create_sync(const void *cmd, uint16_t cmd_len,
				void *rsp, uint16_t *rsp_len)
{
	int err;
	const struct btp_gap_padv_create_sync_cmd *cp = cmd;
	struct bt_le_per_adv_sync_param create_params = {0};

	bt_addr_le_copy(&create_params.addr, &cp->address);
	create_params.options = BT_LE_PER_ADV_SYNC_OPT_NONE;
	create_params.sid = cp->advertiser_sid;
	create_params.skip = sys_le16_to_cpu(cp->skip);
	create_params.timeout = sys_le16_to_cpu(cp->sync_timeout);

	if (cp->flags & BTP_GAP_PADV_CREATE_SYNC_FLAG_REPORTS_DISABLED) {
		create_params.options |= BT_LE_PER_ADV_SYNC_OPT_REPORTING_INITIALLY_DISABLED;
	}

	if (cp->flags & BTP_GAP_PADV_CREATE_SYNC_FLAG_FILTER_DUPLICATES) {
		create_params.options |= BT_LE_PER_ADV_SYNC_OPT_FILTER_DUPLICATE;
	}

	err = tester_gap_padv_create_sync(&create_params);
	if (err != 0) {
		return BTP_STATUS_FAILED;
	}

	return BTP_STATUS_SUCCESS;
}

static uint8_t padv_sync_transfer_set_info(const void *cmd, uint16_t cmd_len,
					   void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_padv_sync_transfer_set_info_cmd *cp = cmd;
	(void)cp;

	/* TODO */

	return BTP_STATUS_FAILED;
}

static uint8_t padv_sync_transfer_start(const void *cmd, uint16_t cmd_len,
					void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_padv_sync_transfer_start_cmd *cp = cmd;
	(void)cp;

	/* TODO */

	return BTP_STATUS_FAILED;
}

static uint8_t padv_sync_transfer_recv(const void *cmd, uint16_t cmd_len,
				       void *rsp, uint16_t *rsp_len)
{
	const struct btp_gap_padv_sync_transfer_recv_cmd *cp = cmd;
	(void)cp;

	/* TODO */

	return BTP_STATUS_FAILED;
}
#endif /* defined(CONFIG_BT_PER_ADV) */

static const struct btp_handler handlers[] = {
	{
		.opcode = BTP_GAP_READ_SUPPORTED_COMMANDS,
		.index = BTP_INDEX_NONE,
		.expect_len = 0,
		.func = supported_commands,
	},
	{
		.opcode = BTP_GAP_READ_CONTROLLER_INDEX_LIST,
		.index = BTP_INDEX_NONE,
		.expect_len = 0,
		.func = controller_index_list,
	},
	{
		.opcode = BTP_GAP_READ_CONTROLLER_INFO,
		.expect_len = 0,
		.func = controller_info,
	},
	{
		.opcode = BTP_GAP_SET_CONNECTABLE,
		.expect_len = sizeof(struct btp_gap_set_connectable_cmd),
		.func = set_connectable,
	},
	{
		.opcode = BTP_GAP_SET_DISCOVERABLE,
		.expect_len = sizeof(struct btp_gap_set_discoverable_cmd),
		.func = set_discoverable,
	},
	{
		.opcode = BTP_GAP_SET_BONDABLE,
		.expect_len = sizeof(struct btp_gap_set_bondable_cmd),
		.func = set_bondable,
	},
	{
		.opcode = BTP_GAP_START_ADVERTISING,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = start_advertising,
	},
	{
		.opcode = BTP_GAP_START_DIRECTED_ADV,
		.expect_len = sizeof(struct btp_gap_start_directed_adv_cmd),
		.func = start_directed_advertising,
	},
	{
		.opcode = BTP_GAP_STOP_ADVERTISING,
		.expect_len = 0,
		.func = stop_advertising,
	},
	{
		.opcode = BTP_GAP_START_DISCOVERY,
		.expect_len = sizeof(struct btp_gap_start_discovery_cmd),
		.func = start_discovery,
	},
	{
		.opcode = BTP_GAP_STOP_DISCOVERY,
		.expect_len = 0,
		.func = stop_discovery,
	},
	{
		.opcode = BTP_GAP_CONNECT,
		.expect_len = sizeof(struct btp_gap_connect_cmd),
		.func = connect,
	},
	{
		.opcode = BTP_GAP_DISCONNECT,
		.expect_len = sizeof(struct btp_gap_disconnect_cmd),
		.func = disconnect,
	},
	{
		.opcode = BTP_GAP_SET_IO_CAP,
		.expect_len = sizeof(struct btp_gap_set_io_cap_cmd),
		.func = set_io_cap,
	},
	{
		.opcode = BTP_GAP_PAIR,
		.expect_len = sizeof(struct btp_gap_pair_cmd),
		.func = pair,
	},
	{
		.opcode = BTP_GAP_UNPAIR,
		.expect_len = sizeof(struct btp_gap_unpair_cmd),
		.func = unpair,
	},
	{
		.opcode = BTP_GAP_PASSKEY_ENTRY,
		.expect_len = sizeof(struct btp_gap_passkey_entry_cmd),
		.func = passkey_entry,
	},
	{
		.opcode = BTP_GAP_PASSKEY_CONFIRM,
		.expect_len = sizeof(struct btp_gap_passkey_confirm_cmd),
		.func = passkey_confirm,
	},
	{
		.opcode = BTP_GAP_CONN_PARAM_UPDATE,
		.expect_len = sizeof(struct btp_gap_conn_param_update_cmd),
		.func = conn_param_update,
	},
	{
		.opcode = BTP_GAP_SET_MITM,
		.expect_len = sizeof(struct btp_gap_set_mitm),
		.func = set_mitm,
	},
	{
		.opcode = BTP_GAP_OOB_LEGACY_SET_DATA,
		.expect_len = sizeof(struct btp_gap_oob_legacy_set_data_cmd),
		.func = set_oob_legacy_data,
	},
#if defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U)
    {
        .opcode = BTP_SM_AUTH,
        .expect_len = sizeof(struct btp_sm_auth_cmd),
        .func = br_smp_authenticate,
    },
#endif
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
	{
		.opcode = BTP_GAP_OOB_SC_GET_LOCAL_DATA,
		.expect_len = 0,
		.func = get_oob_sc_local_data,
	},
	{
		.opcode = BTP_GAP_OOB_SC_SET_REMOTE_DATA,
		.expect_len = sizeof(struct btp_gap_oob_sc_set_remote_data_cmd),
		.func = set_oob_sc_remote_data,
	},
#endif /* !defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) */
	{
		.opcode = BTP_GAP_SET_FILTER_LIST,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = set_filter_list,
	},
#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0U))
	{
		.opcode = BTP_GAP_SET_EXTENDED_ADVERTISING,
		.expect_len = sizeof(struct btp_gap_set_extended_advertising_cmd),
		.func = set_extended_advertising,
	},
#if (defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0U))
	{
		.opcode = BTP_GAP_PADV_CONFIGURE,
		.expect_len = sizeof(struct btp_gap_padv_configure_cmd),
		.func = padv_configure,
	},
	{
		.opcode = BTP_GAP_PADV_START,
		.expect_len = sizeof(struct btp_gap_padv_start_cmd),
		.func = padv_start,
	},
	{
		.opcode = BTP_GAP_PADV_STOP,
#if 1
		.expect_len = 0,
#else
		.expect_len = sizeof(struct btp_gap_padv_stop_cmd),
#endif
		.func = padv_stop,
	},
	{
		.opcode = BTP_GAP_PADV_SET_DATA,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = padv_set_data,
	},
	{
		.opcode = BTP_GAP_PADV_CREATE_SYNC,
		.expect_len = sizeof(struct btp_gap_padv_create_sync_cmd),
		.func = padv_create_sync,
	},
	{
		.opcode = BTP_GAP_PADV_SYNC_TRANSFER_SET_INFO,
		.expect_len = sizeof(struct btp_gap_padv_sync_transfer_set_info_cmd),
		.func = padv_sync_transfer_set_info,
	},
	{
		.opcode = BTP_GAP_PADV_SYNC_TRANSFER_START,
		.expect_len = sizeof(struct btp_gap_padv_sync_transfer_start_cmd),
		.func = padv_sync_transfer_start,
	},
	{
		.opcode = BTP_GAP_PADV_SYNC_TRANSFER_RECV,
		.expect_len = sizeof(struct btp_gap_padv_sync_transfer_recv_cmd),
		.func = padv_sync_transfer_recv,
	},
#endif /* defined(CONFIG_BT_PER_ADV) */
#endif /* defined(CONFIG_BT_EXT_ADV) */
};

uint8_t tester_init_gap(void)
{
	int err;

	err = bt_enable(NULL);
	if (err < 0) {
		PRINTF("Unable to enable Bluetooth: %d", err);
		return BTP_STATUS_FAILED;
	}

	(void)memset(&cb, 0, sizeof(cb));
	bt_conn_auth_cb_register(NULL);
	cb.pairing_accept = auth_pairing_accept;
	if (bt_conn_auth_cb_register(&cb)) {
		return BTP_STATUS_FAILED;
	}

    k_work_init(&reconnect.work, reconnect_work_handler);

    atomic_clear(current_settings);
    atomic_set_bit(current_settings, BTP_GAP_SETTINGS_POWERED);
    atomic_set_bit(current_settings, BTP_GAP_SETTINGS_CONNECTABLE);
    atomic_set_bit(current_settings, BTP_GAP_SETTINGS_BONDABLE);
    atomic_set_bit(current_settings, BTP_GAP_SETTINGS_LE);
#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
    atomic_set_bit(current_settings, BTP_GAP_SETTINGS_PRIVACY);
#endif /* CONFIG_BT_PRIVACY */

	bt_conn_cb_register(&conn_callbacks);
	bt_conn_auth_info_cb_register(&auth_info_cb);

#if defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0)
	bt_le_per_adv_sync_cb_register(&pa_sync_cb);
#endif /* defined(CONFIG_BT_PER_ADV) */

	tester_register_command_handlers(BTP_SERVICE_ID_GAP, handlers,
					 ARRAY_SIZE(handlers));
    k_work_init_delayable(&bt_sc.set_security_work, le_conn_set_security_delayed_work);
	return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_gap(void)
{
	return BTP_STATUS_SUCCESS;
}
