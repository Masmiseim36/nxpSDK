/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <porting.h>
#include <bluetooth/gatt.h>
#include <bluetooth/audio/tbs.h>
#include <bluetooth/audio/csip.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <sys/check.h>
#include <sys/util_macro.h>

#include "cap_internal.h"

#define BT_DBG_ENABLED IS_ENABLED(CONFIG_BT_DEBUG_CAP)
#define LOG_MODULE_NAME bt_cap
#include "fsl_component_log.h"

#if (defined(CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER) && (CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER > 0))

static struct bt_gatt_attr svc_attrs[] = {
	BT_GATT_PRIMARY_SERVICE(BT_UUID_CAS),
	BT_GATT_INCLUDE_SERVICE(NULL) /* To be overwritten */
};

int bt_cap_acceptor_register(const struct bt_csip_set_member_register_param *param,
			     struct bt_csip_set_member_svc_inst **svc_inst)
{
	static struct bt_gatt_service cas;
	int err;

	CHECKIF(param->set_size == 0U) {
		LOG_DBG("param->set_size shall be non-zero");
		return -EINVAL;
	}

	CHECKIF(param->rank == 0U) {
		LOG_DBG("param->rank shall be non-zero");
		return -EINVAL;
	}

	err = bt_csip_set_member_register(param, svc_inst);
	if (err != 0) {
		LOG_DBG("Failed to register CSIP");
		return err;
	}

	cas = (struct bt_gatt_service)BT_GATT_SERVICE(svc_attrs);

	/* Overwrite the include definition with the CSIP */
	cas.attrs[1].user_data = bt_csip_set_member_svc_decl_get(*svc_inst);

	err = bt_gatt_service_register(&cas);
	if (err) {
		const int csip_err = bt_csip_set_member_unregister(*svc_inst);

		if (csip_err) {
			LOG_ERR("Failed to unregister CSIS: %d", csip_err);
		}

		cas.attrs[1].user_data = NULL;
		LOG_DBG("Failed to register CAS");

		return err;
	}

	return 0;
}

#else /* CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER */

BT_GATT_SERVICE_DEFINE(cas_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_CAS)
	/* TODO: Add support for included CSIP */
);

#endif /* CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER */

bool bt_cap_acceptor_ccid_exist(const struct bt_conn *conn, uint8_t ccid)
{
	if (IS_ENABLED(CONFIG_BT_TBS_CLIENT_CCID) &&
	    bt_tbs_client_get_by_ccid(conn, ccid) != NULL) {
		return true;
	}

	/* TODO: check mcs */

	return false;
}

#endif /* BT_CAP_ACCEPTOR */
