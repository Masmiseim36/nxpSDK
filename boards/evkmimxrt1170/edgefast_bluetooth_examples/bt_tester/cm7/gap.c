/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_component_serial_manager.h"
#include "fsl_debug_console.h"

#include "bt_tester.h"
#include "ble_utils.h"
#include <sys/atomic.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/att.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CONTROLLER_INDEX 0
#define CONTROLLER_NAME "btp_tester"

#define BT_LE_AD_DISCOV_MASK (BT_LE_AD_LIMITED | BT_LE_AD_GENERAL)
#define ADV_BUF_LEN (sizeof(struct gap_device_found_ev) + 2 * 31)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void tester_init_gap_cb(int err);

/* Events */
static void le_connected(struct bt_conn *conn, uint8_t err);
static void le_disconnected(struct bt_conn *conn, uint8_t reason);
static void le_identity_resolved(struct bt_conn *conn, const bt_addr_le_t *rpa,
                 const bt_addr_le_t *identity);
static void le_param_updated(struct bt_conn *conn, uint16_t interval,
                 uint16_t latency, uint16_t timeout);
static void le_security_changed(struct bt_conn *conn, bt_security_t level,
                enum bt_security_err err);
static void device_found
(
    const bt_addr_le_t *addr,
    int8_t rssi,
    uint8_t evtype,
    struct net_buf_simple *ad
);

/* Command handlers */
static void supported_commands(uint8_t *data, uint16_t len);
static void controller_index_list(uint8_t *data,  uint16_t len);
static void controller_info(uint8_t *data, uint16_t len);
static void set_connectable(uint8_t *data, uint16_t len);
static void set_discoverable(uint8_t *data, uint16_t len);
static void set_bondable(uint8_t *data, uint16_t len);
static void start_advertising(const uint8_t *data, uint16_t len);
static void stop_advertising(const uint8_t *data, uint16_t len);
static void start_discovery(const uint8_t *data, uint16_t len);
static void start_directed_advertising(const uint8_t *data, uint16_t len);
static void stop_discovery(const uint8_t *data, uint16_t len);
static void connect(const uint8_t *data, uint16_t len);
static void disconnect(const uint8_t *data, uint16_t len);
static void set_io_cap(const uint8_t *data, uint16_t len);
static void pair(const uint8_t *data, uint16_t len);
static void unpair(const uint8_t *data, uint16_t len);
static void passkey_entry(const uint8_t *data, uint16_t len);
static void passkey_confirm(const uint8_t *data, uint16_t len);
static void conn_param_update(const uint8_t *data, uint16_t len);
static void set_mitm(const uint8_t *data, uint16_t len);
static void set_oob_legacy_data(const uint8_t *data, uint16_t len);
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
static void get_oob_sc_local_data(void);
static void set_oob_sc_remote_data(const uint8_t *data, uint16_t len);
#endif
/* Helpers */
static void store_adv
(
    const bt_addr_le_t *addr,
    int8_t rssi,
    struct net_buf_simple *ad
);
static uint8_t get_ad_flags(struct net_buf_simple *ad);
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0U))
enum bt_security_err auth_pairing_accept(struct bt_conn *conn,
                     const struct bt_conn_pairing_feat *const feat);
#endif
#endif
static void auth_cancel(struct bt_conn *conn);
static void auth_passkey_entry(struct bt_conn *conn);
static void auth_passkey_confirm(struct bt_conn *conn, unsigned int passkey);
static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey);
static void auth_pairing_complete(struct bt_conn *conn, bool bonded);
void auth_pairing_failed(struct bt_conn *conn, enum bt_security_err reason);
static void oob_data_request(struct bt_conn *conn,
                 struct bt_conn_oob_info *oob_info);

#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
static uint8_t read_car_cb
(
    struct bt_conn *conn, uint8_t err,
    struct bt_gatt_read_params *params,
    const void *data,
    uint16_t length
);
#endif

extern bool bt_addr_le_is_bonded(uint8_t id, const bt_addr_le_t *addr);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static atomic_t current_settings;
struct bt_conn_auth_cb cb;
static uint8_t oob_legacy_tk[16] = { 0 };

#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
static struct bt_le_oob oob_sc_local = { 0 };
static struct bt_le_oob oob_sc_remote = { 0 };
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) ||
           ((defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))))) */

/* Advertising flags */
static uint8_t ad_flags = BT_LE_AD_NO_BREDR;
/* Advertising data */
static struct bt_data ad[10] = {
    BT_DATA(BT_DATA_FLAGS, &ad_flags, sizeof(ad_flags)),
};
/* Scanning data */
static struct bt_data sd[10];
static uint8_t discovery_flags;
static struct net_buf_simple *adv_buf = NET_BUF_SIMPLE(ADV_BUF_LEN);

#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
static struct bt_gatt_read_params read_car_params = {
        .func = read_car_cb,
        .by_uuid.uuid = BT_UUID_CENTRAL_ADDR_RES,
        .by_uuid.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE,
        .by_uuid.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE,
};

static struct {
    bt_addr_le_t addr;
    bool supported;
} cars[CONFIG_BT_MAX_PAIRED];
#endif

static struct bt_conn_cb conn_callbacks = {
    .connected = le_connected,
    .disconnected = le_disconnected,
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    .identity_resolved = le_identity_resolved,
#endif
    .le_param_updated = le_param_updated,
#if ((defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U)) || (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0U)))
    .security_changed = le_security_changed,
#endif
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * @brief   Tester App functionality
 */
uint8_t tester_init_gap(void)
{
    int err;

    (void)memset(&cb, 0, sizeof(cb));
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    bt_conn_auth_cb_register(NULL);
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0U))
    cb.pairing_accept = auth_pairing_accept;
#endif
    if (bt_conn_auth_cb_register(&cb))
    {
        return BTP_STATUS_FAILED;
    }
#endif
    err = bt_enable(tester_init_gap_cb);
    if (err < 0)
    {
        PRINTF("Unable to enable Bluetooth: %d", err);
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
}

static void tester_init_gap_cb(int err)
{
    if (err)
    {
        tester_rsp(BTP_SERVICE_ID_CORE, CORE_REGISTER_SERVICE,
               BTP_INDEX_NONE, BTP_STATUS_FAILED);
        return;
    }

    atomic_clear(&current_settings);
    atomic_set_bit(&current_settings, GAP_SETTINGS_POWERED);
    atomic_set_bit(&current_settings, GAP_SETTINGS_CONNECTABLE);
    atomic_set_bit(&current_settings, GAP_SETTINGS_BONDABLE);
    atomic_set_bit(&current_settings, GAP_SETTINGS_LE);
#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
    atomic_set_bit(&current_settings, GAP_SETTINGS_PRIVACY);
#endif /* CONFIG_BT_PRIVACY */

    bt_conn_cb_register(&conn_callbacks);

    tester_rsp(BTP_SERVICE_ID_CORE, CORE_REGISTER_SERVICE, BTP_INDEX_NONE,
       BTP_STATUS_SUCCESS);
}

void tester_handle_gap
(
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    uint16_t len
)
{
    switch (opcode)
    {
        case GAP_READ_SUPPORTED_COMMANDS:
            if (index != BTP_INDEX_NONE)
            {
                tester_rsp(BTP_SERVICE_ID_GAP, opcode, index,
                       BTP_STATUS_FAILED);
            }
            else
            {
                supported_commands(data, len);
            }
        break;

        case GAP_READ_CONTROLLER_INDEX_LIST:
            if (index != BTP_INDEX_NONE)
            {
                tester_rsp(BTP_SERVICE_ID_GAP, opcode, index,
                       BTP_STATUS_FAILED);
            }
            else
            {
                controller_index_list(data, len);
            }
        break;

        case GAP_READ_CONTROLLER_INFO:
            controller_info(data, len);
        break;

        case GAP_SET_CONNECTABLE:
            set_connectable(data, len);
        break;

        case GAP_SET_DISCOVERABLE:
            set_discoverable(data, len);
        break;

        case GAP_SET_BONDABLE:
            set_bondable(data, len);
        break;

        case GAP_START_ADVERTISING:
            start_advertising(data, len);
        break;

        case GAP_START_DIRECTED_ADV:
            start_directed_advertising(data, len);
        break;

        case GAP_STOP_ADVERTISING:
            stop_advertising(data, len);
        break;

        case GAP_START_DISCOVERY:
            start_discovery(data, len);
        break;

        case GAP_STOP_DISCOVERY:
            stop_discovery(data, len);
        break;

        case GAP_CONNECT:
            connect(data, len);
        break;

        case GAP_DISCONNECT:
            disconnect(data, len);
        break;

        case GAP_SET_IO_CAP:
            set_io_cap(data, len);
        break;

        case GAP_PAIR:
            pair(data, len);
        break;

        case GAP_UNPAIR:
            unpair(data, len);
        break;

        case GAP_PASSKEY_ENTRY:
            passkey_entry(data, len);
        break;

        case GAP_PASSKEY_CONFIRM:
            passkey_confirm(data, len);
        break;

        case GAP_CONN_PARAM_UPDATE:
            conn_param_update(data, len);
        break;

        case GAP_SET_MITM:
            set_mitm(data, len);
        break;

        case GAP_OOB_LEGACY_SET_DATA:
            set_oob_legacy_data(data, len);
        break;

#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
        case GAP_OOB_SC_GET_LOCAL_DATA:
            get_oob_sc_local_data();
        break;

        case GAP_OOB_SC_SET_REMOTE_DATA:
            set_oob_sc_remote_data(data, len);
        break;

#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
           (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))) */
        default:
            tester_rsp(BTP_SERVICE_ID_GAP, opcode, index,
                   BTP_STATUS_UNKNOWN_CMD);
        break;
    }
}

uint8_t tester_unregister_gap(void)
{
    return BTP_STATUS_SUCCESS;
}

/*
 * @brief   Helpers
 */

/*
 * @brief   Read Supported Commands. Each bit in response is
 *          a flag indicating if command with opcode matching
 *          bit number is supported.
 */
static void supported_commands(uint8_t *data, uint16_t len)
{
    uint8_t cmds[4];
    gap_read_supported_commands_rp_t *rp = (void *) &cmds;

    (void)memset(cmds, 0, sizeof(cmds));

    tester_set_bit(cmds, GAP_READ_SUPPORTED_COMMANDS);
    tester_set_bit(cmds, GAP_READ_CONTROLLER_INDEX_LIST);
    tester_set_bit(cmds, GAP_READ_CONTROLLER_INFO);
    tester_set_bit(cmds, GAP_SET_CONNECTABLE);
    tester_set_bit(cmds, GAP_SET_DISCOVERABLE);
    tester_set_bit(cmds, GAP_SET_BONDABLE);
    tester_set_bit(cmds, GAP_START_ADVERTISING);
    tester_set_bit(cmds, GAP_START_DIRECTED_ADV);
    tester_set_bit(cmds, GAP_STOP_ADVERTISING);
    tester_set_bit(cmds, GAP_START_DISCOVERY);
    tester_set_bit(cmds, GAP_STOP_DISCOVERY);
    tester_set_bit(cmds, GAP_CONNECT);
    tester_set_bit(cmds, GAP_DISCONNECT);
    tester_set_bit(cmds, GAP_SET_IO_CAP);
    tester_set_bit(cmds, GAP_PAIR);
    tester_set_bit(cmds, GAP_PASSKEY_ENTRY);
    tester_set_bit(cmds, GAP_PASSKEY_CONFIRM);
    tester_set_bit(cmds, GAP_CONN_PARAM_UPDATE);
    tester_set_bit(cmds, GAP_SET_MITM);
    tester_set_bit(cmds, GAP_OOB_LEGACY_SET_DATA);
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
    tester_set_bit(cmds, GAP_OOB_SC_GET_LOCAL_DATA);
    tester_set_bit(cmds, GAP_OOB_SC_SET_REMOTE_DATA);
#endif

    tester_send(BTP_SERVICE_ID_GAP, GAP_READ_SUPPORTED_COMMANDS,
            CONTROLLER_INDEX, (uint8_t *) rp, sizeof(cmds));
}

/*
 * @brief   Returns the list of controllers
 */
static void controller_index_list(uint8_t *data,  uint16_t len)
{
    gap_read_controller_index_list_rp_t *rp;
    uint8_t buf[sizeof(*rp) + 1];

    rp = (void *) buf;

    rp->num = 1U;
    rp->index = CONTROLLER_INDEX;

    tester_send(BTP_SERVICE_ID_GAP, GAP_READ_CONTROLLER_INDEX_LIST,
            BTP_INDEX_NONE, (uint8_t *) rp, sizeof(buf));
}

/*
 * @brief   Retrieve the current state and basic
 *          information of a controller.
 */
static void controller_info(uint8_t *data, uint16_t len)
{
    gap_read_controller_info_rp_t rp;
    uint32_t supported_settings;

    (void)memset(&rp, 0, sizeof(rp));

    struct bt_le_oob oob_local = { 0 };

    bt_le_oob_get_local(BT_ID_DEFAULT, &oob_local);
    memcpy(rp.address, &oob_local.addr.a, sizeof(bt_addr_t));

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    /* Re-use the oob data read here in get_oob_sc_local_data() */
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
    oob_sc_local = oob_local;
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
           (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))) */
#endif

    /* If privacy is used, the device uses random type address, otherwise
       static random or public type address is used. */
#if (!defined(CONFIG_BT_PRIVACY) || (CONFIG_BT_PRIVACY == 0U))
    if (oob_local.addr.type == BT_ADDR_LE_RANDOM) {
        atomic_set_bit(&current_settings, GAP_SETTINGS_STATIC_ADDRESS);
    }
#endif /* CONFIG_BT_PRIVACY */

    supported_settings = BIT(GAP_SETTINGS_POWERED);
    supported_settings |= BIT(GAP_SETTINGS_CONNECTABLE);
    supported_settings |= BIT(GAP_SETTINGS_BONDABLE);
    supported_settings |= BIT(GAP_SETTINGS_LE);
    supported_settings |= BIT(GAP_SETTINGS_ADVERTISING);

    rp.supported_settings = sys_cpu_to_le32(supported_settings);
    rp.current_settings = sys_cpu_to_le32(current_settings);

    memcpy(rp.name, CONTROLLER_NAME, sizeof(CONTROLLER_NAME));

    tester_send(BTP_SERVICE_ID_GAP, GAP_READ_CONTROLLER_INFO,
            CONTROLLER_INDEX, (uint8_t *) &rp, sizeof(rp));
}

/*
 * @brief   Set the controller to connectable.
 */
static void set_connectable(uint8_t *data, uint16_t len)
{
    const struct gap_set_connectable_cmd *cmd = (void *) data;
    gap_set_connectable_rp_t rp;

    if (cmd->connectable) {
        atomic_set_bit(&current_settings, GAP_SETTINGS_CONNECTABLE);
    }
    else
    {
        atomic_clear_bit(&current_settings, GAP_SETTINGS_CONNECTABLE);
    }

    rp.current_settings = sys_cpu_to_le32(current_settings);

    tester_send(BTP_SERVICE_ID_GAP, GAP_SET_CONNECTABLE, CONTROLLER_INDEX,
            (uint8_t *) &rp, sizeof(rp));
}


/*
 * @brief   Set the controller to discoverable.
 */
static void set_discoverable(uint8_t *data, uint16_t len)
{
    const struct gap_set_discoverable_cmd *cmd = (void *) data;
    gap_set_discoverable_rp_t rp;

    switch (cmd->discoverable)
    {
        case GAP_NON_DISCOVERABLE:
            ad_flags &= ~(BT_LE_AD_GENERAL | BT_LE_AD_LIMITED);
            atomic_clear_bit(&current_settings, GAP_SETTINGS_DISCOVERABLE);
        break;

        case GAP_GENERAL_DISCOVERABLE:
            ad_flags &= ~BT_LE_AD_LIMITED;
            ad_flags |= BT_LE_AD_GENERAL;
            atomic_set_bit(&current_settings, GAP_SETTINGS_DISCOVERABLE);
        break;

        case GAP_LIMITED_DISCOVERABLE:
            ad_flags &= ~BT_LE_AD_GENERAL;
            ad_flags |= BT_LE_AD_LIMITED;
            atomic_set_bit(&current_settings, GAP_SETTINGS_DISCOVERABLE);
        break;

        default:
            tester_rsp(BTP_SERVICE_ID_GAP, GAP_SET_DISCOVERABLE,
                   CONTROLLER_INDEX, BTP_STATUS_FAILED);
        break;
    }

    rp.current_settings = sys_cpu_to_le32(current_settings);

    tester_send(BTP_SERVICE_ID_GAP, GAP_SET_DISCOVERABLE, CONTROLLER_INDEX,
            (uint8_t *) &rp, sizeof(rp));
}

/*
 * @brief   Set the controller to bondable.
 */
static void set_bondable(uint8_t *data, uint16_t len)
{
    const struct gap_set_bondable_cmd *cmd = (void *) data;
    gap_set_bondable_rp_t rp;

    if (cmd->bondable)
    {
        atomic_set_bit(&current_settings, GAP_SETTINGS_BONDABLE);
    }
    else
    {
        atomic_clear_bit(&current_settings, GAP_SETTINGS_BONDABLE);
    }
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    bt_set_bondable(cmd->bondable);
#endif

    rp.current_settings = sys_cpu_to_le32(current_settings);

    tester_send(BTP_SERVICE_ID_GAP, GAP_SET_BONDABLE, CONTROLLER_INDEX,
            (uint8_t *) &rp, sizeof(rp));
}

/*
 * @brief   SThis command is used to start advertising.
 */
static void start_advertising(const uint8_t *data, uint16_t len)
{
    const struct gap_start_advertising_cmd *cmd = (void *) data;
    gap_start_advertising_rp_t rp;
    uint8_t adv_len, sd_len;
    bool adv_conn;
    int i;
    bool early_return = false;

    for (i = 0, adv_len = 1U; i < cmd->adv_data_len; adv_len++)
    {
        if (adv_len >= ARRAY_SIZE(ad))
        {
            PRINTF("ad[] Out of memory");
            early_return = true;
            tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_ADVERTISING, CONTROLLER_INDEX,
                       BTP_STATUS_FAILED);
            break;
        }

        ad[adv_len].type = cmd->adv_sr_data[i++];
        ad[adv_len].data_len = cmd->adv_sr_data[i++];
        ad[adv_len].data = &cmd->adv_sr_data[i];
        i += ad[adv_len].data_len;
    }

    if (early_return == false)
    {
        for (sd_len = 0U; i < cmd->adv_data_len+cmd->scan_rsp_len; sd_len++)
        {
            if (sd_len >= ARRAY_SIZE(sd))
            {
                PRINTF("sd[] Out of memory");
                early_return = true;
                tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_ADVERTISING, CONTROLLER_INDEX,
                           BTP_STATUS_FAILED);
                break;
            }

            sd[sd_len].type = cmd->adv_sr_data[i++];
            sd[sd_len].data_len = cmd->adv_sr_data[i++];
            sd[sd_len].data = &cmd->adv_sr_data[i];
            i += sd[sd_len].data_len;
        }

        if (early_return == false)
        {
            adv_conn = atomic_test_bit(&current_settings, GAP_SETTINGS_CONNECTABLE);

            /* BTP API don't allow to set empty scan response data. */
            if (bt_le_adv_start(adv_conn ? BT_LE_ADV_CONN : BT_LE_ADV_NCONN,
                        ad, adv_len, sd_len ? sd : NULL, sd_len) < 0)
            {
                early_return = true;
                tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_ADVERTISING, CONTROLLER_INDEX,
                           BTP_STATUS_FAILED);
            }

            if (early_return == false)
            {
                atomic_set_bit(&current_settings, GAP_SETTINGS_ADVERTISING);
                rp.current_settings = sys_cpu_to_le32(current_settings);

                tester_send(BTP_SERVICE_ID_GAP, GAP_START_ADVERTISING, CONTROLLER_INDEX,
                        (uint8_t *) &rp, sizeof(rp));
            }
        }
    }
}

/*
 * @brief   This command is used to stop advertising.
 */
static void stop_advertising(const uint8_t *data, uint16_t len)
{
    gap_stop_advertising_rp_t rp;
    int err;
    bool early_return = false;

    err = bt_le_adv_stop();
    if (err < 0) {
        tester_rsp(BTP_SERVICE_ID_GAP, GAP_STOP_ADVERTISING,
               CONTROLLER_INDEX, BTP_STATUS_FAILED);
        PRINTF("Failed to stop advertising: %d", err);
        early_return = true;
    }

    if (early_return == false)
    {
        atomic_clear_bit(&current_settings, GAP_SETTINGS_ADVERTISING);
        rp.current_settings = sys_cpu_to_le32(current_settings);

        tester_send(BTP_SERVICE_ID_GAP, GAP_STOP_ADVERTISING, CONTROLLER_INDEX,
                (uint8_t *) &rp, sizeof(rp));
    }
}

/*
 * @brief   This command is used to start discovery.
 */
static void start_discovery(const uint8_t *data, uint16_t len)
{
    const gap_start_discovery_cmd_t *cmd = (void *) data;
    uint8_t status;

    /* only LE scan is supported */
    if (cmd->flags & GAP_DISCOVERY_FLAG_BREDR)
    {
        status = BTP_STATUS_FAILED;

        tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_DISCOVERY, CONTROLLER_INDEX,
                   status);
    }
    else
    {
        if (bt_le_scan_start(cmd->flags & GAP_DISCOVERY_FLAG_LE_ACTIVE_SCAN ?
                     BT_LE_SCAN_ACTIVE : BT_LE_SCAN_PASSIVE,
                     device_found) < 0)
        {
            status = BTP_STATUS_FAILED;
            PRINTF("Failed to start scanning");
            tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_DISCOVERY, CONTROLLER_INDEX,
                       status);
        }
        else
        {
            net_buf_simple_init(adv_buf, 0);
            discovery_flags = cmd->flags;
            status = BTP_STATUS_SUCCESS;
            tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_DISCOVERY, CONTROLLER_INDEX,
                       status);
        }
    }
}

/*
 * @brief   This command is used to start directed advertising.
 */
static void start_directed_advertising(const uint8_t *data, uint16_t len)
{
    const struct gap_start_directed_adv_cmd *cmd = (void *)data;
    gap_start_directed_adv_rp_t rp;
    struct bt_le_adv_param adv_param;
    uint16_t options = sys_le16_to_cpu(cmd->options);
    const bt_addr_le_t *peer = (bt_addr_le_t *)data;

    adv_param = *BT_LE_ADV_CONN_DIR(peer);

    if (!(options & GAP_START_DIRECTED_ADV_HD))
    {
        adv_param.options |= BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY;
        adv_param.interval_max = BT_GAP_ADV_FAST_INT_MAX_2;
        adv_param.interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
    }

    if (options & GAP_START_DIRECTED_ADV_PEER_RPA) {

#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
        /* check if peer supports Central Address Resolution */
        for (int i = 0; i < CONFIG_BT_MAX_PAIRED; i++)
        {
            if (bt_addr_le_cmp(peer, &cars[i].addr) == 0) {
                if (cars[i].supported)
                {
                    adv_param.options |= BT_LE_ADV_OPT_DIR_ADDR_RPA;
                }
            }
        }
#endif
    }

    if (bt_le_adv_start(&adv_param, NULL, 0, NULL, 0) < 0)
    {
        PRINTF("Failed to start advertising");
        goto fail;
    }

    atomic_set_bit(&current_settings, GAP_SETTINGS_ADVERTISING);
    rp.current_settings = sys_cpu_to_le32(current_settings);

    tester_send(BTP_SERVICE_ID_GAP, GAP_START_DIRECTED_ADV,
                CONTROLLER_INDEX, (uint8_t *)&rp, sizeof(rp));
    return;
fail:
    tester_rsp(BTP_SERVICE_ID_GAP, GAP_START_DIRECTED_ADV, CONTROLLER_INDEX,
               BTP_STATUS_FAILED);
}

/*
 * @brief   This command is used to start directed advertising.
 */
static void stop_discovery(const uint8_t *data, uint16_t len)
{
    uint8_t status = BTP_STATUS_SUCCESS;
    int err;

    err = bt_le_scan_stop();
    if (err < 0)
    {
        PRINTF("Failed to stop scanning: %d", err);
        status = BTP_STATUS_FAILED;
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_STOP_DISCOVERY, CONTROLLER_INDEX,
               status);
}

/*
 * @brief   This command is used to create a Link Layer connection with
 *          the remote device.
 */
static void connect(const uint8_t *data, uint16_t len)
{
    struct bt_conn *conn;
    uint8_t status;
    int err;

    err = bt_conn_le_create((bt_addr_le_t *) data, BT_CONN_LE_CREATE_CONN,
                            BT_LE_CONN_PARAM_DEFAULT, &conn);
    if (err)
    {
        PRINTF("Failed to create connection (%d)", err);
        status = BTP_STATUS_FAILED;
    }
    else
    {
        bt_conn_unref(conn);
        status = BTP_STATUS_SUCCESS;
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_CONNECT, CONTROLLER_INDEX, status);
}

/*
 * @brief   This command is used to terminate an existing connection or
 *          to cancel pending connection attempt.
 */
static void disconnect(const uint8_t *data, uint16_t len)
{
    struct bt_conn *conn;
    uint8_t status;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (!conn)
    {
        status = BTP_STATUS_FAILED;
        PRINTF("Unknown connection");
    }
    else
    {
        if (bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN))
        {
            PRINTF("Failed to disconnect");
            status = BTP_STATUS_FAILED;
        }
        else
        {
            status = BTP_STATUS_SUCCESS;
            bt_conn_unref(conn);
        }
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_DISCONNECT, CONTROLLER_INDEX,
               status);
}

/*
 * @brief   This command is used to set I/O capabilities
 */
static void set_io_cap(const uint8_t *data, uint16_t len)
{
    const struct gap_set_io_cap_cmd *cmd = (void *) data;
    uint8_t status = BTP_STATUS_SUCCESS;

    /* Reset io cap requirements */
    (void)memset(&cb, 0, sizeof(cb));
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    bt_conn_auth_cb_register(NULL);
#endif
    switch (cmd->io_cap)
    {
        case GAP_IO_CAP_DISPLAY_ONLY:
            cb.cancel = auth_cancel;
            cb.passkey_display = auth_passkey_display;
        break;

        case GAP_IO_CAP_KEYBOARD_DISPLAY:
            cb.cancel = auth_cancel;
            cb.passkey_display = auth_passkey_display;
            cb.passkey_entry = auth_passkey_entry;
            cb.passkey_confirm = auth_passkey_confirm;
        break;

        case GAP_IO_CAP_NO_INPUT_OUTPUT:
            cb.cancel = auth_cancel;
        break;

        case GAP_IO_CAP_KEYBOARD_ONLY:
            cb.cancel = auth_cancel;
            cb.passkey_entry = auth_passkey_entry;
        break;

        case GAP_IO_CAP_DISPLAY_YESNO:
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
#endif
        cb.pairing_failed = auth_pairing_failed;
        cb.pairing_complete = auth_pairing_complete;

        if (bt_conn_auth_cb_register(&cb))
        {
            status = BTP_STATUS_FAILED;
        }
#endif
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_SET_IO_CAP, CONTROLLER_INDEX,
               status);
}

/*
 * @brief   This command is used to initiate security with remote. If
 *          peer is already paired IUT is expected to enable security
 *          (encryption) with peer. If peer is not paired IUT shall
 *          start pairing process.
 */
static void pair(const uint8_t *data, uint16_t len)
{
    struct bt_conn *conn;
    uint8_t status;
    int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
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
            status = BTP_STATUS_FAILED;
            bt_conn_unref(conn);
        }
        else
        {
            bt_conn_unref(conn);
            status = BTP_STATUS_SUCCESS;
        }
#endif
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_PAIR, CONTROLLER_INDEX, status);
}

/*
 * @brief   This command is used to unpair with remote.
 */
static void unpair(const uint8_t *data, uint16_t len)
{
    struct gap_unpair_cmd *cmd = (void *) data;
    struct bt_conn *conn;
    bt_addr_le_t addr;
    uint8_t status;
    int err;

    addr.type = cmd->address_type;
    memcpy(addr.a.val, cmd->address, sizeof(addr.a.val));

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &addr);
    if (!conn)
    {
        err = bt_unpair(BT_ID_DEFAULT, &addr);
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
            err = bt_unpair(BT_ID_DEFAULT, &addr);
            status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
        }
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_UNPAIR, CONTROLLER_INDEX, status);
}

/*
 * @brief   This command is used to response with passkey for pairing
 *          request.
 */
static void passkey_entry(const uint8_t *data, uint16_t len)
{
    const struct gap_passkey_entry_cmd *cmd = (void *) data;
    struct bt_conn *conn;
    uint8_t status;
    int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (!conn)
    {
        status = BTP_STATUS_FAILED;
    }
    else
    {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
        err = bt_conn_auth_passkey_entry(conn, sys_le32_to_cpu(cmd->passkey));
        bt_conn_unref(conn);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
#endif
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_PASSKEY_ENTRY, CONTROLLER_INDEX,
               status);
}


/*
 * @brief   This command is used to response for pairing request with
 *          confirmation in accordance with initiator and responder passkey
 */
static void passkey_confirm(const uint8_t *data, uint16_t len)
{
    const struct gap_passkey_confirm_cmd *cmd = (void *) data;
    struct bt_conn *conn;
    uint8_t status;
    int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (!conn)
    {
        status = BTP_STATUS_FAILED;
    }
    else
    {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
        if (cmd->match)
        {
            err = bt_conn_auth_passkey_confirm(conn);
        }
        else
        {
            err = bt_conn_auth_cancel(conn);
        }
#endif
        bt_conn_unref(conn);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_PASSKEY_CONFIRM, CONTROLLER_INDEX,
               status);
}

/*
 * @brief   This command is used to response for pairing request with
 *          confirmation in accordance with initiator and responder passkey
 */
static void conn_param_update(const uint8_t *data, uint16_t len)
{
    const struct gap_conn_param_update_cmd *cmd = (void *) data;
    struct bt_le_conn_param param = {
        .interval_min = sys_le16_to_cpu(cmd->interval_min),
        .interval_max = sys_le16_to_cpu(cmd->interval_max),
        .latency = sys_le16_to_cpu(cmd->latency),
        .timeout = sys_le16_to_cpu(cmd->timeout),
    };
    struct bt_conn *conn;
    uint8_t status = BTP_STATUS_FAILED;
    int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (conn)
    {
        err = bt_conn_le_param_update(conn, &param);
        bt_conn_unref(conn);
        status = err < 0 ? BTP_STATUS_FAILED : BTP_STATUS_SUCCESS;
    }

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_CONN_PARAM_UPDATE, CONTROLLER_INDEX,
               status);
}

/*
 * @brief   This command is used to set MITM setting.
 */
static void set_mitm(const uint8_t *data, uint16_t len)
{
    tester_rsp(BTP_SERVICE_ID_GAP, GAP_SET_MITM, CONTROLLER_INDEX,
               BTP_STATUS_SUCCESS);
}

/*
 * @brief   This command is used to set legacy OOB data
 */
static void set_oob_legacy_data(const uint8_t *data, uint16_t len)
{
    const struct gap_oob_legacy_set_data_cmd *cmd = (void *) data;

    memcpy(oob_legacy_tk, cmd->oob_data, 16);
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
    bt_set_oob_data_flag(true);
#endif
    cb.oob_data_request = oob_data_request;

    tester_rsp(BTP_SERVICE_ID_GAP, GAP_OOB_LEGACY_SET_DATA,
               CONTROLLER_INDEX, BTP_STATUS_SUCCESS);
}

/*
 *    Events
 */

/*
 * @brief   This event indicates that a device was connected.
 */
static void le_connected(struct bt_conn *conn, uint8_t err)
{
    struct gap_device_connected_ev ev;
    struct bt_conn_info info;

    if (!err)
    {
        bt_conn_get_info(conn, &info);

        memcpy(ev.address, info.le.dst->a.val, sizeof(ev.address));
        ev.address_type = info.le.dst->type;
        ev.interval = sys_cpu_to_le16(info.le.interval);
        ev.latency = sys_cpu_to_le16(info.le.latency);
        ev.timeout = sys_cpu_to_le16(info.le.timeout);

        tester_send(BTP_SERVICE_ID_GAP, GAP_EV_DEVICE_CONNECTED,
                    CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
    }
}

/*
 * @brief   This event indicates that a device was disconnected.
 */
static void le_disconnected(struct bt_conn *conn, uint8_t reason)
{
    struct gap_device_disconnected_ev ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_DEVICE_DISCONNECTED,
                CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
}

/*
 * @brief   This event indicates that the remote Identity Address has been
 *          resolved.
 */
static void le_identity_resolved(struct bt_conn *conn, const bt_addr_le_t *rpa,
                 const bt_addr_le_t *identity)
{
    struct gap_identity_resolved_ev ev;

    ev.address_type = rpa->type;
    memcpy(ev.address, rpa->a.val, sizeof(ev.address));

    ev.identity_address_type = identity->type;
    memcpy(ev.identity_address, identity->a.val,
           sizeof(ev.identity_address));

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_IDENTITY_RESOLVED,
                CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
}

/*
 * @brief   This event can be sent when the connection parameters have changed
 */
static void le_param_updated(struct bt_conn *conn, uint16_t interval,
                 uint16_t latency, uint16_t timeout)
{
    struct gap_conn_param_update_ev ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;
    ev.interval = sys_cpu_to_le16(interval);
    ev.latency = sys_cpu_to_le16(latency);
    ev.timeout = sys_cpu_to_le16(timeout);

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_CONN_PARAM_UPDATE,
                CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
}

/*
 * @brief   This event can be sent when the Security Level has changed
 */
static void le_security_changed(struct bt_conn *conn, bt_security_t level,
                enum bt_security_err err)
{
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);
    struct gap_sec_level_changed_ev sec_ev;
    struct gap_bond_lost_ev bond_ev;
    struct bt_conn_info info;

    switch (err)
    {
        case BT_SECURITY_ERR_SUCCESS:
            memcpy(sec_ev.address, addr->a.val, sizeof(sec_ev.address));
            sec_ev.address_type = addr->type;
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

            tester_send(BTP_SERVICE_ID_GAP, GAP_EV_SEC_LEVEL_CHANGED,
                        CONTROLLER_INDEX, (uint8_t *) &sec_ev, sizeof(sec_ev));
        break;

        case BT_SECURITY_ERR_PIN_OR_KEY_MISSING:
            /* for central role this means that peer have no LTK when we
             * started encryption procedure
             *
             * This means bond is lost and we restart pairing to re-bond
             */
            if (bt_conn_get_info(conn, &info) == 0 &&
                info.role == BT_CONN_ROLE_CENTRAL)
            {
                (void)memcpy(bond_ev.address, addr->a.val, sizeof(bond_ev.address));
                bond_ev.address_type = addr->type;

                tester_send(BTP_SERVICE_ID_GAP, GAP_EV_BOND_LOST,
                            CONTROLLER_INDEX, (uint8_t *)&bond_ev, sizeof(bond_ev));
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
                (void)bt_conn_set_security(conn, (bt_security_t)(BT_SECURITY_L2 | BT_SECURITY_FORCE_PAIR));
#endif
            }
        break;

        default:
        break;
    }
}

/*
 * @brief   This event indicates that a device was found during device
 *          discovery.
 */
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t evtype,
             struct net_buf_simple *ad)
{
    /* if General/Limited Discovery - parse Advertising data to get flags */
    if (!(discovery_flags & GAP_DISCOVERY_FLAG_LE_OBSERVE) &&
        (evtype != BT_GAP_ADV_TYPE_SCAN_RSP))
    {
        uint8_t flags = get_ad_flags(ad);

        /* ignore non-discoverable devices */
        if (!(flags & BT_LE_AD_DISCOV_MASK)) {
            return;
        }

        /* if Limited Discovery - ignore general discoverable devices */
        if ((discovery_flags & GAP_DISCOVERY_FLAG_LIMITED) &&
            !(flags & BT_LE_AD_LIMITED)) {
            return;
        }
    }

    /* attach Scan Response data */
    if (evtype == BT_GAP_ADV_TYPE_SCAN_RSP)
    {
        struct gap_device_found_ev *ev;
        bt_addr_le_t a;

        /* skip if there is no pending advertisement */
        if (!adv_buf->len)
        {
            return;
        }

        ev = (void *) adv_buf->data;
        a.type = ev->address_type;
        memcpy(a.a.val, ev->address, sizeof(a.a.val));

        /*
         * in general, the Scan Response comes right after the
         * Advertisement, but if not if send stored event and ignore
         * this one
         */
        if (bt_addr_le_cmp(addr, &a))
        {
            goto done;
        }

        ev->eir_data_len += ad->len;
        ev->flags |= GAP_DEVICE_FOUND_FLAG_SD;

        memcpy(net_buf_simple_add(adv_buf, ad->len), ad->data, ad->len);

        goto done;
    }

    /*
     * if there is another pending advertisement, send it and store the
     * current one
     */
    if (adv_buf->len)
    {
        tester_send(BTP_SERVICE_ID_GAP, GAP_EV_DEVICE_FOUND,
                CONTROLLER_INDEX, adv_buf->data, adv_buf->len);
        net_buf_simple_reset(adv_buf);
    }

    store_adv(addr, rssi, ad);

    /* if Active Scan and scannable event - wait for Scan Response */
    if ((discovery_flags & GAP_DISCOVERY_FLAG_LE_ACTIVE_SCAN) &&
        (evtype == BT_GAP_ADV_TYPE_ADV_IND ||
         evtype == BT_GAP_ADV_TYPE_ADV_SCAN_IND))
    {
        return;
    }
done:
    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_DEVICE_FOUND,
            CONTROLLER_INDEX, adv_buf->data, adv_buf->len);
    net_buf_simple_reset(adv_buf);
}
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
/*
 *    Helpers
 */
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0U))
enum bt_security_err auth_pairing_accept(struct bt_conn *conn,
                     const struct bt_conn_pairing_feat *const feat)
{
    gap_bond_lost_ev_t ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    if (!bt_addr_le_is_bonded(BT_ID_DEFAULT, addr))
    {
        return BT_SECURITY_ERR_SUCCESS;
    }

    /* If a peer is already bonded and tries to pair again then it means that
     * the it has lost its bond information.
     */
    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_BOND_LOST, CONTROLLER_INDEX, (uint8_t *)&ev,
            sizeof(ev));

    return BT_SECURITY_ERR_SUCCESS;
}
#endif
#endif

#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
static uint8_t read_car_cb
(
    struct bt_conn *conn, uint8_t err,
    struct bt_gatt_read_params *params,
    const void *data,
    uint16_t length
)
{
    struct bt_conn_info info;
    bool supported = false;

    if (!err && data && length == 1) {
        const uint8_t *tmp = data;

        /* only 0 or 1 are valid values */
        if (tmp[0] == 1) {
            supported = true;
        }
    }

    bt_conn_get_info(conn, &info);

    for (int i = 0; i < CONFIG_BT_MAX_PAIRED; i++) {
        if (bt_addr_le_cmp(info.le.dst, &cars[i].addr) == 0) {
            cars[i].supported = supported;
            break;
        }
    }

    return BT_GATT_ITER_STOP;
}
#endif

void auth_pairing_failed(struct bt_conn *conn, enum bt_security_err reason)
{
    gap_bond_pairing_failed_ev_t ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;
    ev.reason = reason;

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_PAIRING_FAILED, CONTROLLER_INDEX,
            (uint8_t *)&ev, sizeof(ev));
}



static uint8_t get_ad_flags(struct net_buf_simple *ad)
{
    uint8_t len, i;

    /* Parse advertisement to get flags */
    for (i = 0U; i < ad->len; i += len - 1)
    {
        len = ad->data[i++];
        if (!len)
        {
            break;
        }

        /* Check if field length is correct */
        if (len > (ad->len - i) || (ad->len - i) < 1)
        {
            break;
        }

        if (ad->data[i++] == BT_DATA_FLAGS)
        {
            return ad->data[i];
        }
    }

    return 0;
}

static void store_adv
(
    const bt_addr_le_t *addr,
    int8_t rssi,
    struct net_buf_simple *ad
)
{
    struct gap_device_found_ev *ev;

    /* cleanup */
    net_buf_simple_init(adv_buf, 0);

    ev = net_buf_simple_add(adv_buf, sizeof(*ev));

    memcpy(ev->address, addr->a.val, sizeof(ev->address));
    ev->address_type = addr->type;
    ev->rssi = rssi;
    ev->flags = GAP_DEVICE_FOUND_FLAG_AD | GAP_DEVICE_FOUND_FLAG_RSSI;
    ev->eir_data_len = ad->len;
    memcpy(net_buf_simple_add(adv_buf, ad->len), ad->data, ad->len);
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    struct gap_passkey_display_ev ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;
    ev.passkey = sys_cpu_to_le32(passkey);

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_PASSKEY_DISPLAY,
                CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
}

static void auth_passkey_entry(struct bt_conn *conn)
{
    struct gap_passkey_entry_req_ev ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_PASSKEY_ENTRY_REQ,
                CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
}

static void auth_passkey_confirm(struct bt_conn *conn, unsigned int passkey)
{
    struct gap_passkey_confirm_req_ev ev;
    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    memcpy(ev.address, addr->a.val, sizeof(ev.address));
    ev.address_type = addr->type;
    ev.passkey = sys_cpu_to_le32(passkey);

    tester_send(BTP_SERVICE_ID_GAP, GAP_EV_PASSKEY_CONFIRM_REQ,
                CONTROLLER_INDEX, (uint8_t *) &ev, sizeof(ev));
}

static void auth_pairing_complete(struct bt_conn *conn, bool bonded)
{
#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
    /* Read peer's Central Address Resolution if bonded */
    if (bonded)
    {
        bt_gatt_read(conn, &read_car_params);
    }
#endif
}

static void auth_cancel(struct bt_conn *conn)
{
    /* TODO */
}

static void oob_data_request(struct bt_conn *conn,
                 struct bt_conn_oob_info *oob_info)
{
    struct bt_conn_info info;
    int err = bt_conn_get_info(conn, &info);

    if (err)
    {
        return;
    }

    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(info.le.dst, addr, sizeof(addr));

    switch (oob_info->type)
    {
#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
    case BT_CONN_OOB_LE_SC:
    {
        struct bt_le_oob_sc_data *oobd_local =
            oob_info->lesc.oob_config != BT_CONN_OOB_REMOTE_ONLY ?
                      &oob_sc_local.le_sc_data :
                      NULL;

        struct bt_le_oob_sc_data *oobd_remote =
            oob_info->lesc.oob_config != BT_CONN_OOB_LOCAL_ONLY ?
                      &oob_sc_remote.le_sc_data :
                      NULL;

        if (oobd_remote)
        {
            /* Assume that oob_sc_remote
             * corresponds to the currently connected peer
             */
            bt_addr_le_copy(&oob_sc_remote.addr, info.le.remote);
        }

        if (oobd_local &&
            bt_addr_le_cmp(info.le.local, &oob_sc_local.addr)) {
            bt_addr_le_to_str(info.le.local, addr, sizeof(addr));
            bt_conn_auth_cancel(conn);
            return;
        }
#if !(defined(CONFIG_BT_SMP_SC_PAIR_ONLY) && (CONFIG_BT_SMP_SC_PAIR_ONLY > 0U))
        bt_le_oob_set_sc_data(conn, oobd_local, oobd_remote);
#endif
        break;
    }
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
           (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))) */

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
#if !defined(CONFIG_BT_SMP_SC_PAIR_ONLY)
    case BT_CONN_OOB_LE_LEGACY:
        bt_le_oob_set_legacy_tk(conn, oob_legacy_tk);
        break;
#endif /* !defined(CONFIG_BT_SMP_SC_PAIR_ONLY) */
#endif
    default:
        PRINTF("Unhandled OOB type %d", oob_info->type);
        break;
    }
}

#if ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
     (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U)))
static void get_oob_sc_local_data(void)
{
	cb.oob_data_request = oob_data_request;
	struct gap_oob_sc_get_local_data_rp rp = { 0 };

	memcpy(&rp.conf[0], &oob_sc_local.le_sc_data.c[0], sizeof(rp.conf));
	memcpy(&rp.rand[0], &oob_sc_local.le_sc_data.r[0], sizeof(rp.rand));
	tester_send(BTP_SERVICE_ID_GAP, GAP_OOB_SC_GET_LOCAL_DATA,
		    CONTROLLER_INDEX, (uint8_t *)&rp, sizeof(rp));
}

static void set_oob_sc_remote_data(const uint8_t *data, uint16_t len)
{
	cb.oob_data_request = oob_data_request;
	bt_set_oob_data_flag(true);

	const struct gap_oob_sc_set_remote_data_cmd *cmd = (void *)data;

	/* Note that the .addr field
	 * will be set by the oob_data_request callback
	 */
	memcpy(&oob_sc_remote.le_sc_data.r[0], &cmd->rand[0],
	       sizeof(oob_sc_remote.le_sc_data.r));
	memcpy(&oob_sc_remote.le_sc_data.c[0], &cmd->conf[0],
	       sizeof(oob_sc_remote.le_sc_data.c));

	tester_rsp(BTP_SERVICE_ID_GAP, GAP_OOB_SC_SET_REMOTE_DATA,
		   CONTROLLER_INDEX, BTP_STATUS_SUCCESS);
}
#endif /* ((!defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) || \
           (defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY == 0U))) */
