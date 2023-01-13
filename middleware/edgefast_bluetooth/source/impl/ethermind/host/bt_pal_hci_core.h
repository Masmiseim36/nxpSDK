/* bt_pal_hci_core.h - Bluetooth HCI core access */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __HCI_CORE_H__
#define __HCI_CORE_H__

/* LL connection parameters */
#define LE_CONN_LATENCY		0x0000
#define LE_CONN_TIMEOUT		0x002a

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#define LMP_FEAT_PAGES_COUNT	3
#else
#define LMP_FEAT_PAGES_COUNT	1
#endif

#define BT_INIT_TIMEOUT	BT_MSEC(5000)

/* SCO  settings */
#define BT_VOICE_CVSD_16BIT     0x0060

/* k_poll event tags */
enum {
	BT_EVENT_CMD_TX,
	BT_EVENT_CONN_TX_QUEUE,
};

/* bt_dev flags: the flags defined here represent BT controller state */
enum {
	BT_DEV_ENABLE,
	BT_DEV_DISABLE,
	BT_DEV_READY,
	BT_DEV_PRESET_ID,
	BT_DEV_HAS_PUB_KEY,
	BT_DEV_PUB_KEY_BUSY,

	BT_DEV_SCANNING,
	BT_DEV_EXPLICIT_SCAN,
	BT_DEV_ACTIVE_SCAN,
	BT_DEV_SCAN_FILTER_DUP,
	BT_DEV_SCAN_FILTERED,
	BT_DEV_SCAN_LIMITED,
	BT_DEV_INITIATING,

	BT_DEV_RPA_VALID,
	BT_DEV_RPA_TIMEOUT_CHANGED,

	BT_DEV_ID_PENDING,
	BT_DEV_STORE_ID,

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
	BT_DEV_ISCAN,
	BT_DEV_PSCAN,
	BT_DEV_INQUIRY,
#endif /* CONFIG_BT_BREDR */

	/* Total number of flags - must be at the end of the enum */
	BT_DEV_NUM_FLAGS,
};

/* Flags which should not be cleared upon HCI_Reset */
#define BT_DEV_PERSISTENT_FLAGS (BIT(BT_DEV_ENABLE) | \
				 BIT(BT_DEV_PRESET_ID))

#if (defined(CONFIG_BT_EXT_ADV_LEGACY_SUPPORT) && (CONFIG_BT_EXT_ADV_LEGACY_SUPPORT > 0U))
/* Check the feature bit for extended or legacy advertising commands */
#define BT_DEV_FEAT_LE_EXT_ADV(feat) BT_FEAT_LE_EXT_ADV(feat)
#else
/* Always use extended advertising commands. */
#define BT_DEV_FEAT_LE_EXT_ADV(feat)  1
#endif

enum {
	/* Advertising set has been created in the host. */
	BT_ADV_CREATED,
	/* Advertising parameters has been set in the controller.
	 * This implies that the advertising set has been created in the
	 * controller.
	 */
	BT_ADV_PARAMS_SET,
	/* Advertising data has been set in the controller. */
	BT_ADV_DATA_SET,
	/* Advertising random address pending to be set in the controller. */
	BT_ADV_RANDOM_ADDR_PENDING,
	/* The private random address of the advertiser is valid for this cycle
	 * of the RPA timeout.
	 */
	BT_ADV_RPA_VALID,
	/* The advertiser set is limited by a timeout, or number of advertising
	 * events, or both.
	 */
	BT_ADV_LIMITED,
	/* Advertiser set is currently advertising in the controller. */
	BT_ADV_ENABLED,
	/* Advertiser should include name in advertising data */
	BT_ADV_INCLUDE_NAME_AD,
	/* Advertiser should include name in scan response data */
	BT_ADV_INCLUDE_NAME_SD,
	/* Advertiser set is connectable */
	BT_ADV_CONNECTABLE,
	/* Advertiser set is scannable */
	BT_ADV_SCANNABLE,
	/* Advertiser set is using extended advertising */
	BT_ADV_EXT_ADV,
	/* Advertiser set has disabled the use of private addresses and is using
	 * the identity address instead.
	 */
	BT_ADV_USE_IDENTITY,
	/* Advertiser has been configured to keep advertising after a connection
	 * has been established as long as there are connections available.
	 */
	BT_ADV_PERSIST,
	/* Advertiser has been temporarily disabled. */
	BT_ADV_PAUSED,
	/* Periodic Advertising has been enabled in the controller. */
	BT_PER_ADV_ENABLED,
	/* Periodic Advertising parameters has been set in the controller. */
	BT_PER_ADV_PARAMS_SET,
	/* Periodic Advertising to include AdvDataInfo (ADI) */
	BT_PER_ADV_INCLUDE_ADI,
	/* Constant Tone Extension parameters for Periodic Advertising
	 * has been set in the controller.
	 */
	BT_PER_ADV_CTE_PARAMS_SET,
	/* Constant Tone Extension for Periodic Advertising has been enabled
	 * in the controller.
	 */
	BT_PER_ADV_CTE_ENABLED,

	BT_ADV_NUM_FLAGS,
};

struct bt_le_ext_adv {
	/* ID Address used for advertising */
	uint8_t                 id;

	/* Advertising handle */
	uint8_t                 handle;

	/* Current local Random Address */
	bt_addr_le_t            random_addr;

	/* Current target address */
	bt_addr_le_t            target_addr;

	ATOMIC_DEFINE(flags, BT_ADV_NUM_FLAGS);

#if (defined(CONFIG_BT_EXT_ADV) && ((CONFIG_BT_EXT_ADV) > 0U))
	const struct bt_le_ext_adv_cb *cb;

	/* TX Power in use by the controller */
	int8_t                    tx_power;
#endif /* defined(CONFIG_BT_EXT_ADV) */

	struct k_work_delayable	lim_adv_timeout_work;
};

enum {
	/** Periodic Advertising Sync has been created in the host. */
	BT_PER_ADV_SYNC_CREATED,

	/** Periodic Advertising Sync is established and can be terminated */
	BT_PER_ADV_SYNC_SYNCED,

	/** Periodic Advertising Sync is attempting to create sync */
	BT_PER_ADV_SYNC_SYNCING,

	/** Periodic Advertising Sync is attempting to create sync using
	 *  Advertiser List
	 */
	BT_PER_ADV_SYNC_SYNCING_USE_LIST,

	/** Periodic Advertising Sync established with reporting disabled */
	BT_PER_ADV_SYNC_RECV_DISABLED,

	/** Constant Tone Extension for Periodic Advertising has been enabled
	 * in the Controller.
	 */
	BT_PER_ADV_SYNC_CTE_ENABLED,

	BT_PER_ADV_SYNC_NUM_FLAGS,
};

struct bt_le_per_adv_sync {
	/** Periodic Advertiser Address */
	bt_addr_le_t addr;

	/** Advertiser SID */
	uint8_t sid;

	/** Sync handle */
	uint16_t handle;

	/** Periodic advertising interval (N * 1.25 ms) */
	uint16_t interval;

	/** Periodic advertising advertiser clock accuracy (ppm) */
	uint16_t clock_accuracy;

	/** Advertiser PHY */
	uint8_t phy;

#if (defined(CONFIG_BT_DF_CONNECTIONLESS_CTE_RX) && (CONFIG_BT_DF_CONNECTIONLESS_CTE_RX > 0U))
	/**
	 * @brief Bitfield with allowed CTE types.
	 *
	 *  Allowed values are defined by @ref bt_df_cte_type, except BT_DF_CTE_TYPE_NONE.
	 */
	uint8_t cte_types;
#endif /* CONFIG_BT_DF_CONNECTIONLESS_CTE_RX */

#if CONFIG_BT_PER_ADV_SYNC_BUF_SIZE > 0
	/** Reassembly buffer for advertising reports */
	struct net_buf_simple reassembly;

	/** Storage for the reassembly buffer */
	uint8_t reassembly_data[CONFIG_BT_PER_ADV_SYNC_BUF_SIZE];
#endif /* CONFIG_BT_PER_ADV_SYNC_BUF_SIZE > 0 */

	/** True if the following periodic adv reports up to and
	 * including the next complete one should be dropped
	 */
	bool report_truncated;

	/** Flags */
	ATOMIC_DEFINE(flags, BT_PER_ADV_SYNC_NUM_FLAGS);
};

struct bt_dev_le {
	/* LE features */
	uint8_t			features[8];
	/* LE states */
	uint64_t			states;

#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	/* Controller buffer information */
	uint16_t		mtu;
	osa_semaphore_handle_t		pkts;
	OSA_SEMAPHORE_HANDLE_DEFINE(pkts_handle);
	uint16_t		acl_mtu;
	osa_semaphore_handle_t		acl_pkts;
	OSA_SEMAPHORE_HANDLE_DEFINE(acl_pkts_handle);
#endif /* CONFIG_BT_CONN */
#if (defined(CONFIG_BT_ISO) && ((CONFIG_BT_ISO) > 0U))
	uint16_t		iso_mtu;
	osa_semaphore_handle_t		iso_pkts;
	OSA_SEMAPHORE_HANDLE_DEFINE(iso_pkts_handle);
#endif /* CONFIG_BT_ISO */

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	/* Size of the the controller resolving list */
	uint8_t                    rl_size;
	/* Number of entries in the resolving list. rl_entries > rl_size
	 * means that host-side resolving is used.
	 */
	uint8_t                    rl_entries;
#endif /* CONFIG_BT_SMP */
};

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
struct bt_dev_br {
	/* Max controller's acceptable ACL packet length */
	uint16_t         mtu;
	osa_semaphore_handle_t  pkts;
	OSA_SEMAPHORE_HANDLE_DEFINE(pkts_handle);
	uint16_t         esco_pkt_type;
};
#endif

/* The theoretical max for these is 8 and 64, but there's no point
 * in allocating the full memory if we only support a small subset.
 * These values must be updated whenever the host implementation is
 * extended beyond the current values.
 */
#define BT_DEV_VS_FEAT_MAX  1
#define BT_DEV_VS_CMDS_MAX  2

#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
#if (defined(CONFIG_BT_ISO) && ((CONFIG_BT_ISO) > 0U))
/* command FIFO + conn_change signal + MAX_CONN + CONFIG_BT_ISO_MAX_CHAN */
#define EV_COUNT (2 + CONFIG_BT_MAX_CONN + CONFIG_BT_ISO_MAX_CHAN * CONFIG_BT_ISO_TX_BUF_COUNT)
#else
/* command FIFO + conn_change signal + MAX_CONN */
#define EV_COUNT (2 + CONFIG_BT_MAX_CONN)
#endif /* CONFIG_BT_ISO */
#else
/* command FIFO */
#define EV_COUNT 1
#endif /* CONFIG_BT_CONN */

/* State tracking for the local Bluetooth controller */
struct _bt_dev {
	/* Local Identity Address(es) */
	bt_addr_le_t            id_addr[CONFIG_BT_ID_MAX];
	uint8_t                    id_count;

	struct bt_conn_le_create_param create_param;

#if !(defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0U))
	/* Legacy advertiser */
	struct bt_le_ext_adv    adv;
#else
	/* Pointer to reserved advertising set */
	struct bt_le_ext_adv    *adv;
#if (CONFIG_BT_ID_MAX > 1) && (CONFIG_BT_EXT_ADV_MAX_ADV_SET > 1)
	/* When supporting multiple concurrent connectable advertising sets
	 * with multiple identities, we need to know the identity of
	 * the terminating advertising set to identify the connection object.
	 * The identity of the advertising set is determined by its
	 * advertising handle, which is part of the
	 * LE Set Advertising Set Terminated event which is always sent
	 * _after_ the LE Enhanced Connection complete event.
	 * Therefore we need cache this event until its identity is known.
	 */
	struct {
		bool valid;
		struct bt_hci_evt_le_enh_conn_complete evt;
	} cached_conn_complete[MIN(CONFIG_BT_MAX_CONN,
				CONFIG_BT_EXT_ADV_MAX_ADV_SET)];
#endif
#endif
	/* Current local Random Address */
	bt_addr_le_t            random_addr;
	uint8_t                    adv_conn_id;

	/* Controller version & manufacturer information */
	uint8_t			hci_version;
	uint8_t			lmp_version;
	uint16_t			hci_revision;
	uint16_t			lmp_subversion;
	uint16_t			manufacturer;

	/* LMP features (pages 0, 1, 2) */
	uint8_t			features[LMP_FEAT_PAGES_COUNT][8];

	/* Supported commands */
	uint8_t			supported_commands[64];

#if (defined(CONFIG_BT_HCI_VS_EXT) && ((CONFIG_BT_HCI_VS_EXT) > 0U))
	/* Vendor HCI support */
	uint8_t                    vs_features[BT_DEV_VS_FEAT_MAX];
	uint8_t                    vs_commands[BT_DEV_VS_CMDS_MAX];
#endif

	struct k_work           init;

	ATOMIC_DEFINE(flags, BT_DEV_NUM_FLAGS);

	/* LE controller specific features */
	struct bt_dev_le	le;

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
	/* BR/EDR controller specific features */
	struct bt_dev_br	br;
#endif
#if 0
	/* Number of commands controller can accept */
	osa_semaphore_handle_t		ncmd_sem;
	OSA_SEMAPHORE_HANDLE_DEFINE(ncmd_sem_handle);
#endif
	/* Initialization done */
	osa_semaphore_handle_t		init_done;
	OSA_SEMAPHORE_HANDLE_DEFINE(init_done_handle);

	/* Last sent HCI command */
	struct net_buf		*sent_cmd;

#if !(defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
	/* Queue for incoming HCI events & ACL data */
	osa_msgq_handle_t rx_queue;
    OSA_MSGQ_HANDLE_DEFINE(rx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
#endif

	/* Queue for outgoing HCI commands */
	osa_msgq_handle_t		cmd_tx_queue;
	OSA_MSGQ_HANDLE_DEFINE(cmd_tx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
#if 0
	/* Registered HCI driver */
	const struct bt_hci_driver *drv;
#endif
	/* New msg availiable */
#define BT_DEV_SEND_COMMAND BIT(0)
#define BT_DEV_SEND_ISO     BIT(1)
#define BT_DEV_CONN_CHANGED BIT(2)

	osa_event_handle_t		new_event;
	OSA_EVENT_HANDLE_DEFINE(new_event_handle);
	osa_msgq_handle_t		conn_changed;
	OSA_MSGQ_HANDLE_DEFINE(conn_changed_handle, EV_COUNT, sizeof(void *));
#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
	osa_msgq_handle_t		iso_conn;
	OSA_MSGQ_HANDLE_DEFINE(iso_conn_handle, EV_COUNT, sizeof(void *));
#endif /* CONFIG_BT_ISO */

#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
	/* Local Identity Resolving Key */
	uint8_t			irk[CONFIG_BT_ID_MAX][16];

	/* Work used for RPA rotation */
	struct k_work_delayable rpa_update;

	/* The RPA timeout value. */
	uint16_t rpa_timeout;
#endif

	/* Local Name */
#if (defined(CONFIG_BT_DEVICE_NAME_DYNAMIC) && ((CONFIG_BT_DEVICE_NAME_DYNAMIC) > 0U))
	char			name[CONFIG_BT_DEVICE_NAME_MAX + 1];
#endif
#if (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC > 0))
	/* Appearance Value */
	uint16_t		appearance;
#endif
};

extern struct _bt_dev bt_dev;
#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
extern const struct bt_conn_auth_cb *bt_auth;
extern sys_slist_t bt_auth_info_cbs;
enum bt_security_err bt_security_err_get(uint8_t hci_err);
#endif /* CONFIG_BT_SMP || CONFIG_BT_BREDR */

/* Data type to store state related with command to be updated
 * when command completes successfully.
 */
struct bt_hci_cmd_state_set {
	/* Target memory to be updated */
	atomic_t *target;
	/* Bit number to be updated in target memory */
	int bit;
	/* Value to determine if enable or disable bit */
	bool val;
};

/* Set command state related with the command buffer */
void bt_hci_cmd_state_set_init(struct net_buf *buf,
			       struct bt_hci_cmd_state_set *state,
			       atomic_t *target, int bit, bool val);

int bt_hci_disconnect(uint16_t handle, uint8_t reason);

void bt_set_conn_changed(struct bt_conn *conn);

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
void bt_set_send_iso(struct bt_conn *conn);
#endif /* CONFIG_BT_ISO */

bool bt_le_conn_params_valid(const struct bt_le_conn_param *param);
int bt_le_set_data_len(struct bt_conn *conn, uint16_t tx_octets, uint16_t tx_time);
int bt_le_set_phy(struct bt_conn *conn, uint8_t all_phys,
		  uint8_t pref_tx_phy, uint8_t pref_rx_phy, uint8_t phy_opts);
uint8_t bt_get_phy(uint8_t hci_phy);

int bt_le_scan_update(bool fast_scan);

int bt_le_create_conn(const struct bt_conn *conn);
int bt_le_create_conn_cancel(void);

void bt_br_acl_link_connect_req(bt_addr_t *peer, uint32_t cod);

bool bt_addr_le_is_bonded(uint8_t id, const bt_addr_le_t *addr);
const bt_addr_le_t *bt_lookup_id_addr(uint8_t id, const bt_addr_le_t *addr);

int bt_send(struct net_buf *buf);
int bt_recv(struct net_buf *buf);

uint16_t ethermind_hci_event_callback(uint8_t event_type,
                        uint8_t *event_data, uint8_t  event_datalen);

void bt_conn_unpair(uint8_t id, const bt_addr_le_t *addr);

/* Don't require everyone to include bt_pal_keys.h */
struct bt_keys;
void bt_id_add(struct bt_keys *keys);
void bt_id_del(struct bt_keys *keys);

int bt_setup_random_id_addr(void);
int bt_setup_public_id_addr(void);

void bt_finalize_init(void);

void bt_hci_host_num_completed_packets(struct net_buf *buf);

int bt_le_adv_start_internal(const struct bt_le_adv_param *param,
			     const struct bt_data *ad, size_t ad_len,
			     const struct bt_data *sd, size_t sd_len,
			     const bt_addr_le_t *peer);

void bt_le_adv_resume(void);
bool bt_le_scan_random_addr_check(void);

/* HCI event handlers */
void bt_hci_pin_code_req(struct net_buf *buf);
void bt_hci_link_key_notify(struct net_buf *buf);
void bt_hci_link_key_req(struct net_buf *buf);
void bt_hci_io_capa_resp(struct net_buf *buf);
void bt_hci_io_capa_req(struct net_buf *buf);
void bt_hci_ssp_complete(struct net_buf *buf);
void bt_hci_user_confirm_req(struct net_buf *buf);
void bt_hci_user_passkey_notify(struct net_buf *buf);
void bt_hci_user_passkey_req(struct net_buf *buf);
void bt_hci_auth_complete(struct net_buf *buf);

/* ECC HCI event handlers */
void bt_hci_evt_le_pkey_complete(struct net_buf *buf);
void bt_hci_evt_le_dhkey_complete(struct net_buf *buf);

/* Common HCI event handlers */
void bt_hci_le_enh_conn_complete(struct bt_hci_evt_le_enh_conn_complete *evt);

/* Scan HCI event handlers */
void bt_hci_le_adv_report(struct net_buf *buf);
void bt_hci_le_scan_timeout(struct net_buf *buf);
void bt_hci_le_adv_ext_report(struct net_buf *buf);
void bt_hci_le_per_adv_sync_established(struct net_buf *buf);
void bt_hci_le_per_adv_report(struct net_buf *buf);
void bt_hci_le_per_adv_sync_lost(struct net_buf *buf);
void bt_hci_le_biginfo_adv_report(struct net_buf *buf);
void bt_hci_le_df_connectionless_iq_report(struct net_buf *buf);
void bt_hci_le_vs_df_connectionless_iq_report(struct net_buf *buf);
void bt_hci_le_past_received(struct net_buf *buf);

/* Adv HCI event handlers */
void bt_hci_le_adv_set_terminated(struct net_buf *buf);
void bt_hci_le_scan_req_received(struct net_buf *buf);

/* BR/EDR HCI event handlers */
void bt_hci_conn_req(struct net_buf *buf);
void bt_hci_conn_complete(struct net_buf *buf);


void bt_hci_inquiry_complete(struct net_buf *buf);
void bt_hci_inquiry_result_with_rssi(struct net_buf *buf);
void bt_hci_extended_inquiry_result(struct net_buf *buf);
void bt_hci_remote_name_request_complete(struct net_buf *buf);

void bt_hci_read_remote_features_complete(struct net_buf *buf);
void bt_hci_read_remote_ext_features_complete(struct net_buf *buf);
void bt_hci_role_change(struct net_buf *buf);
void bt_hci_synchronous_conn_complete(struct net_buf *buf);

void bt_hci_le_df_connection_iq_report(struct net_buf *buf);
void bt_hci_le_vs_df_connection_iq_report(struct net_buf *buf);
void bt_hci_le_df_cte_req_failed(struct net_buf *buf);
/* HCI command complete response of le encrypt handlers */
struct bt_hci_cmd_le_encrypt_rp_cb
{
	void (*cb)(struct net_buf *buf, void *user_data);
	void *user_data;

	sys_snode_t node;
};

int hci_cmd_le_encrypt_rp_cb_register(struct bt_hci_cmd_le_encrypt_rp_cb *cb);
int hci_cmd_le_encrypt_rp_cb_unregister(struct bt_hci_cmd_le_encrypt_rp_cb *cb);

struct bt_hci_acl_hdr_simulation
{
    void (*handler)(struct net_buf *buf);
    uint16_t len;
};

bool update_sec_level(struct bt_conn *conn);

#endif /* __HCI_CORE_H__ */
