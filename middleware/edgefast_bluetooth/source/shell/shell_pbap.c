/** @file
 * @brief Bluetooth PBAP shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>
#include <errno.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/pbap_pce.h>
#include <bluetooth/pbap_pse.h>
#include "bt_pal_pbap_internal.h"
#include <bluetooth/sdp.h>
#include "BT_sdp_api.h"
#include "db_gen.h"
#include "BT_common.h"
#include "BT_pbap_api.h"
#include "fsl_shell.h"
#include "shell_bt.h"

#if (defined(CONFIG_BT_PBAP_PCE) && ((CONFIG_BT_PBAP_PCE) > 0U)) || \
    (defined(CONFIG_BT_PBAP_PSE) && ((CONFIG_BT_PBAP_PSE) > 0U))

#define HELP_NONE "[none]"

#define PBAP_CLASS_OF_DEVICE (0x10020CU) /* Object Transfer, Phone, Smartphone */

struct pbap_hdr
{
    uint8_t *value;
    uint16_t length;
};
#if (defined(CONFIG_BT_PBAP_PCE) && ((CONFIG_BT_PBAP_PCE) > 0U))

#define CONFIG_BT_PRINTF_VALUE 1
#define BT_PBAP_PCE_NAME_LEN   16
#define BT_PBAP_PCE_PASSWD_LEN 16
typedef struct app_pbap_pce_
{
    struct bt_pbap_pce *pbap_pceHandle;
    struct bt_conn *conn;
    uint8_t peer_bd_addr[6];
    uint16_t goep_version;
    uint16_t pbap_version;
    uint32_t peer_feature;
    uint32_t loacal_feature;
    uint16_t psm;
    uint16_t scn;
    uint8_t supported_repositories;
    bool lcl_srmp_wait; /* local srmp */
    bool rem_srmp_wait; /* remote srmp */
    int8_t num_srmp_wait;
} app_pbap_pce_t;
app_pbap_pce_t g_PbapPce;

struct bt_pbap_auth g_PbapAuth;

static uint8_t pbap_initied;
static char name[45];
static char userid[BT_PBAP_PCE_NAME_LEN];
static char password[BT_PBAP_PCE_PASSWD_LEN];
static int local_srmp;
static bool psm_conn;

#define SDP_CLIENT_USER_BUF_LEN 512U
NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(pbap_appl_pool, CONFIG_BT_MAX_CONN, CONFIG_BT_PBAP_PCE_MAX_PKT_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static struct bt_sdp_attribute pbap_pce_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(BT_SDP_ATTR_SVCLASS_ID_LIST,
                /* ServiceClassIDList */
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 03
                BT_SDP_DATA_ELEM_LIST(
                    {
                        BT_SDP_TYPE_SIZE(BT_SDP_UUID16),         // 19
                        BT_SDP_ARRAY_16(BT_SDP_PBAP_PCE_SVCLASS) // 11 2E
                    }, )),
    BT_SDP_LIST(BT_SDP_ATTR_PROFILE_DESC_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),                        // 35 08
                BT_SDP_DATA_ELEM_LIST({BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), // 35 06
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),     // 19
                                               BT_SDP_ARRAY_16(BT_SDP_PBAP_SVCLASS) // 11 30
                                           },
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UINT16), // 09
                                               BT_SDP_ARRAY_16(0x0102U)         // 01 02
                                           }, )}, )),
    BT_SDP_SERVICE_NAME("Phonebook Access PCE"),
};

static struct bt_sdp_record pbap_pce_rec = BT_SDP_RECORD(pbap_pce_attrs);

static int stringTonum(char *str, uint64_t *value, uint8_t base)
{
    char *p           = str;
    uint64_t total    = 0;
    uint8_t chartoint = 0;
    if (base == 10)
    {
        while (p && *p != '\0')
        {
            if (*p >= '0' && *p <= '9')
            {
                total = total * 10 + (*p - '0');
            }
            else
            {
                return -EINVAL;
            }
            p++;
        }
    }
    else if (base == 16)
    {
        while (p && *p != '\0')
        {
            if (*p >= '0' && *p <= '9')
            {
                chartoint = *p - '0';
            }
            else if (*p >= 'a' && *p <= 'f')
            {
                chartoint = *p - 'a' + 10;
            }
            else if (*p >= 'A' && *p <= 'F')
            {
                chartoint = *p - 'A' + 10;
            }
            else
            {
                return -EINVAL;
            }
            total = 16 * total + chartoint;
            p++;
        }
    }
    *value = total;
    return 0;
}

static int strTouint_64(char *nptr, uint64_t *value)
{
    char *p    = nptr;
    int result = 0;
    if (p == NULL)
    {
        return -EINVAL;
    }
    if (*p == '0')
    {
        if (*(p + 1) == 'x' || *(p + 1) == 'X')
        {
            result = stringTonum(p + 2, value, 16);
        }
        else
        {
            result = stringTonum(p, value, 10);
        }
    }
    else
    {
        result = stringTonum(p, value, 10);
    }
    return result;
}

static void app_connected(struct bt_pbap_pce *pbap_pce)
{
    shell_print(ctx_shell, "PBAP connect successfully");
}

static void app_get_auth_info_cb(struct bt_pbap_pce *pbap_pce, struct bt_pbap_auth *pbap_auth_info)
{
    strcpy(userid, "PBAP");
    pbap_auth_info->user_id = userid;
    strcpy(password, "0000");
    pbap_auth_info->pin_code = password;
    shell_print(ctx_shell, "%s", pbap_auth_info->pin_code);
}

static void app_disconnected(struct bt_pbap_pce *pbap_pce, uint8_t result)
{
    shell_print(ctx_shell, "PBAP disconnct successfully : %x", result);
    if (result == BT_PBAP_FORBIDDEN_RSP)
    {
        shell_print(ctx_shell, "Possible reasons is Authentication failure");
    }
}

static void app_abort_cb(struct bt_pbap_pce *pbap_pce, uint8_t result)
{
    shell_print(ctx_shell, " pce abort - 0x%02X", result);
}

static void app_error_result(uint8_t result)
{
    switch (result)
    {
        case BT_PBAP_BAD_REQ_RSP:
            shell_error(ctx_shell, "Function not recognized or ill-formatted: %x", result);
            break;

        case BT_PBAP_NOT_IMPLEMENTED_RSP:
            shell_error(ctx_shell, "Function recognized but not supported: %x", result);
            break;

        case BT_PBAP_UNAUTH_RSP:
            shell_error(ctx_shell, "Access is not authorized: %x", result);
            break;

        case BT_PBAP_PRECOND_FAILED_RSP:
            shell_error(ctx_shell, "One of request parameter is wrong: %x", result);
            break;

        case BT_PBAP_NOT_FOUND_RSP:
            shell_error(ctx_shell, "object not found: %x", result);
            break;

        case BT_PBAP_NOT_ACCEPTABLE_RSP:
            shell_error(ctx_shell, "PSE can not meet one of request parameter: %x", result);
            break;

        case BT_PBAP_NO_SERVICE_RSP:
            shell_error(ctx_shell, "System condition prevents it: %x", result);
            break;

        case BT_PBAP_FORBIDDEN_RSP:
            shell_error(ctx_shell, "Temporarily barred: %x", result);
            break;

        default:
            break;
    }
    return;
}

static void app_print_body(struct net_buf *buf)
{
    struct pbap_hdr body;
    if (bt_pbap_pce_get_body(buf, &body.value, &body.length) == 0)
    {
        shell_error(ctx_shell, "============== BODY ==============");
        shell_error(ctx_shell, "%.*s", body.length, body.value);
        shell_error(ctx_shell, "============ END BODY ============");
    }
    else
    {
        shell_error(ctx_shell, "BODY not Found");
    }
}

static bool app_app_param_cb(struct bt_data *data, void *user_data)
{
    switch (data->type)
    {
        case BT_PBAP_TAG_ID_PHONE_BOOK_SIZE:
            shell_error(ctx_shell, "Phonebook Size - %d", sys_get_be16(data->data));
            break;

        case BT_PBAP_TAG_ID_NEW_MISSED_CALLS:
            shell_error(ctx_shell, "New Missed Calls -%d", data->data[0]);
            break;

        case BT_PBAP_TAG_ID_PRIMARY_FOLDER_VERSION:
            shell_error(ctx_shell, "Primary Floder Version - ");
            for (uint8_t index = 0; index < data->data_len; index++)
            {
                PRINTF("%02X", data->data[index]);
            }
            PRINTF("\r\n");
            break;

        case BT_PBAP_TAG_ID_SECONDARY_FOLDER_VERSION:
            shell_error(ctx_shell, "Secondary Floder Version - ");
            for (uint8_t index = 0; index < data->data_len; index++)
            {
                PRINTF("%02X", data->data[index]);
            }
            PRINTF("\r\n");
            break;

        case BT_PBAP_TAG_ID_DATABASE_IDENTIFIER:
            shell_error(ctx_shell, "Database Identifier - ");
            for (uint8_t index = 0; index < data->data_len; index++)
            {
                PRINTF("%02X", data->data[index]);
            }
            PRINTF("\r\n");
            break;

        default:
            break;
    }
    return true;
}

static void app_pull_phonebook_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
    int revert;
    if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
    {
        app_error_result(result);
        net_buf_unref(buf);
        return;
    }
    else
    {
        shell_error(ctx_shell, "Pull phonebook result - 0x%02X", result);
    }
    bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == PBAP_CONTINUE_RSP)
    {
        g_PbapPce.lcl_srmp_wait = (--g_PbapPce.num_srmp_wait) > 0 ? true : false;

        buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
        if (buf == NULL)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

        revert =
            bt_pbap_pce_pull_phonebook(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
        if (revert != 0)
        {
            net_buf_unref(buf);
            shell_error(ctx_shell, "pb_download callback  failed %x", result);
        }
    }
    return;
}

static void app_set_path_cb(struct bt_pbap_pce *pbap_pce, uint8_t result)
{
    switch (result)
    {
        case BT_PBAP_SUCCESS_RSP:
            shell_print(ctx_shell, "pbap pse path set success");
            break;

        default:
            app_error_result(result);
            break;
    }
}

static void app_pull_vcardlist_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
    int revert;
    if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
    {
        app_error_result(result);
        net_buf_unref(buf);
        return;
    }
    else
    {
        shell_error(ctx_shell, "pull vcard listing result - 0x%02X", result);
    }
    bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_PBAP_CONTINUE_RSP)
    {
        if (result == BT_PBAP_CONTINUE_RSP)
        {
            g_PbapPce.lcl_srmp_wait = --g_PbapPce.num_srmp_wait > 0 ? true : false;
            buf                     = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
            if (buf == NULL)
            {
                return;
            }
            net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

            revert = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait,
                                                    BT_OBEX_REQ_END);
            if (revert != 0)
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "pull_vcard_listing callback failed %x", result);
            }
        }
    }
    return;
}

static void app_pull_vcardentry_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
    int revert;
    if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
    {
        app_error_result(result);
        net_buf_unref(buf);
        return;
    }
    else
    {
        shell_error(ctx_shell, "pull vcard listing result - 0x%02X", result);
    }
    bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_PBAP_CONTINUE_RSP)
    {
        g_PbapPce.lcl_srmp_wait = --g_PbapPce.num_srmp_wait > 0 ? true : false;
        buf                     = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
        if (buf == NULL)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

        revert =
            bt_pbap_pce_pull_vcard_entry(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
        if (revert != 0)
        {
            net_buf_unref(buf);
            shell_error(ctx_shell, "pull_vcard_entry callback failed %x", result);
        }
    }
    return;
}

static struct bt_pbap_pce_cb pce_cb = {
    .connected          = app_connected,
    .disconnected       = app_disconnected,
    .get_auth_info      = app_get_auth_info_cb,
    .abort              = app_abort_cb,
    .pull_phonebook     = app_pull_phonebook_cb,
    .set_phonebook_path = app_set_path_cb,
    .pull_vcard_listing = app_pull_vcardlist_cb,
    .pull_vcard_entry   = app_pull_vcardentry_cb,
};
static uint8_t shell_pbap_init(void)
{
    if (pbap_initied == 0)
    {
        bt_sdp_register_service(&pbap_pce_rec);
        return bt_pbap_pce_register(&pce_cb);
    }
    return 0;
}
static uint8_t bt_pbap_pce_sdp_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint32_t peer_feature  = 0;
    uint16_t rfommchannel  = 0;
    uint16_t l2cappsm      = 0;
    uint16_t pbap_version  = 0;
    uint8_t supported_repo = 0;
    if ((result) && (result->resp_buf))
    {
        res = bt_sdp_get_profile_version(result->resp_buf, BT_SDP_PBAP_SVCLASS, &pbap_version);
        if (res < 0)
        {
            shell_error(ctx_shell, "pbap version is not found");
        }
        else
        {
            shell_print(ctx_shell, "pbap version is %x", pbap_version);
            g_PbapPce.pbap_version = pbap_version;
        }
        res = bt_sdp_get_supported_repositories(result->resp_buf, &supported_repo);
        if (res < 0)
        {
            shell_error(ctx_shell, "pbap pse supported repositories is not found");
        }
        else
        {
            shell_print(ctx_shell, "pbap pse supported repositories is %x", supported_repo);
            g_PbapPce.supported_repositories = supported_repo;
        }
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &peer_feature);
        if (res < 0)
        {
            shell_error(ctx_shell, "pse supported feature not found, use default feature",
                        BT_PBAP_SUPPORTED_FEATURES_V11);
            g_PbapPce.peer_feature = BT_PBAP_SUPPORTED_FEATURES_V11;
        }
        else
        {
            shell_print(ctx_shell, "supported feature = %x", peer_feature);
            g_PbapPce.peer_feature = peer_feature;
        }
        g_PbapPce.loacal_feature = CONFIG_BT_PBAP_PCE_SUPPORTED_FEATURE;
        if (psm_conn)
        {
            res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &l2cappsm);
            if (res >= 0)
            {
                shell_print(ctx_shell, "do L2CAP_PSM connect");
                g_PbapPce.psm = l2cappsm;
                res = bt_pbap_pce_psm_connect(conn, l2cappsm, &g_PbapAuth, peer_feature, &g_PbapPce.pbap_pceHandle);
                if (res < 0)
                {
                    shell_print(ctx_shell, "Send connect command failed");
                }
                g_PbapPce.goep_version = BT_GOEP_VERSION_2_0;
                return BT_SDP_DISCOVER_UUID_STOP;
            }
            shell_print(ctx_shell, "No L2CAP_PSM found");
            shell_print(ctx_shell, "Preparing for RFCOMM channel connection");
        }
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &rfommchannel);
        if (res < 0)
        {
            shell_print(ctx_shell, "No RFCOMM channel found");
            return BT_SDP_DISCOVER_UUID_STOP;
        }
        shell_print(ctx_shell, "Do RFCOMM CHANNEL");
        g_PbapPce.scn = rfommchannel;
        res = bt_pbap_pce_scn_connect(conn, rfommchannel, &g_PbapAuth, peer_feature, &g_PbapPce.pbap_pceHandle);
        if (res < 0)
        {
            shell_print(ctx_shell, "Send connect command failed");
        }
        g_PbapPce.goep_version = BT_GOEP_VERSION_1_1;
    }

    return BT_SDP_DISCOVER_UUID_STOP;
}

static struct bt_sdp_discover_params discov_pbap_pce = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_PBAP_PSE_SVCLASS),
    .func = bt_pbap_pce_sdp_user,
    .pool = &sdp_client_pool,
};

static int cmd_connect(const struct shell *sh, size_t argc, char *argv[])
{
    int retval = 0;
    char *p;
    psm_conn = 0;
    memset(userid, 0, BT_PBAP_PCE_NAME_LEN);
    memset(password, 0, BT_PBAP_PCE_PASSWD_LEN);
    g_PbapAuth.pin_code = NULL;
    g_PbapAuth.user_id  = NULL;
    for (uint8_t i = 0; i < argc; ++i)
    {
        p = argv[i];
        if (strcmp(p, "-uid") == 0)
        {
            p = argv[++i];
            memcpy(userid, p, (uint16_t)strlen(p));
            g_PbapAuth.user_id = userid;
        }
        else if (strcmp(p, "-pwd") == 0)
        {
            p = argv[++i];
            memcpy(password, p, (uint16_t)strlen(p));
            g_PbapAuth.pin_code = password;
        }
        else if (strcmp(p, "-psm") == 0)
        {
            psm_conn = 1;
        }
    }

    retval = bt_sdp_discover(default_conn, &discov_pbap_pce);
    if (retval)
    {
        shell_error(sh, "SDP discovery failed: %d", retval);
        return -EINVAL;
    }
    else
    {
        shell_print(sh, "SDP discovery started");
    }

    return 0;
}

static int cmd_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
    API_RESULT retval = 0;
    retval            = bt_pbap_pce_disconnect(g_PbapPce.pbap_pceHandle);

    if (API_SUCCESS != retval)
    {
        shell_error(sh, "disconnect failed Reason 0x%02X", retval);
        return -EINVAL;
    }
    return 0;
}

static int cmd_abort(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_pbap_pce_abort(g_PbapPce.pbap_pceHandle);

    if (err == -EINPROGRESS)
    {
        shell_print(sh, "abort is pending");
    }
    else if (err < 0)
    {
        shell_error(sh, "abort failed Reason %d", err);
        return -EINVAL;
    }
    return 0;
}

static int bt_pull_phonebook_command_analy(struct net_buf *buf, size_t argc, char **argv)
{
    uint16_t index   = 0;
    uint64_t value   = 0;
    uint8_t cs_flag  = 0;
    uint8_t cso_flag = 0;
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-ps") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, " Property Selsector  invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_PROPERTY_SELECTOR(buf, value);
        }
        else if (strcmp(argv[index], "-f") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, " Format invaild");
                return -EINVAL;
            }
            if (value != 1 && value != 0)
            {
                shell_error(ctx_shell, "Format invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_FORMAT(buf, value);
        }
        else if (strcmp(argv[index], "-mlc") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "MaxListCount invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, value);
        }
        else if (strcmp(argv[index], "-lso") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "ListStartOffset invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_PHONE_BOOK_SIZE(buf, value);
        }
        else if (strcmp(argv[index], "-rnmc") == 0)
        {
            if ((g_PbapPce.peer_feature & BT_PBAP_FEATURE_ENHANCED_MISSED_CALLS) &&
                (g_PbapPce.loacal_feature & BT_PBAP_FEATURE_ENHANCED_MISSED_CALLS))
            {
                if (strTouint_64(argv[index + 1], &value) != 0)
                {
                    shell_error(ctx_shell, "ResetNewMissedCalls invaild");
                    return -EINVAL;
                }
                BT_PBAP_ADD_PARAMS_RESET_NEW_MISSED_CALLS(buf, value);
            }
            else
            {
                shell_error(ctx_shell, "pse or pce do not supported enhanced missed calls feature, params ResetNewMissedCalls add failed");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-cs") == 0)
        {
            if (g_PbapPce.peer_feature & BT_PBAP_FEATURE_VCARD_SELECTING &&
                g_PbapPce.loacal_feature & BT_PBAP_FEATURE_VCARD_SELECTING)
            {
                if (strTouint_64(argv[index + 1], &value) != 0)
                {
                    shell_error(ctx_shell, "vCardSelector invaild");
                    return -EINVAL;
                }
                cs_flag = 1;
                BT_PBAP_ADD_PARAMS_VCARD_SELECTOR(buf, value);
            }
            else
            {
                shell_error(ctx_shell, "pse or pce do not supported vCard Selecting feature, params vCardSelector add failed");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-cso") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "vCardSelectorOperator invaild");
                return -EINVAL;
            }
            cso_flag = 1;
            BT_PBAP_ADD_PARAMS_VCARD_SELECTOR_OPERATOR(buf, value);
        }
        else if (strcmp(argv[index], "-name") == 0)
        {
            if (strstr(argv[index + 1], ".vcf") == NULL)
            {
                shell_error(ctx_shell, "file name invaild");
                return -EINVAL;
            }
            memcpy(name, argv[index + 1], (uint16_t)strlen(argv[index + 1]));
        }
        else if (strcmp(argv[index], "-srmp") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "SRMP invaild");
                return -EINVAL;
            }
            local_srmp = value;
        }
        else
        {
            /* no action */
        }
    }
    if (cso_flag == 1 && cs_flag == 0)
    {
        shell_error(ctx_shell, "params vCardSelectorOperator is added but params vCardSelector is not be added");
        return -EINVAL;
    }
    return 0;
}

static int cmd_pull_pb(const struct shell *sh, size_t argc, char *argv[])
{
    API_RESULT retval = 0;
    struct net_buf *buf;
    memset(name, 0, sizeof(name));

    buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
    if (!buf)
    {
        return -EINVAL;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));

    BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, 0xFFFF);

    retval = bt_pull_phonebook_command_analy(buf, argc, argv);
    if (retval)
    {
        net_buf_unref(buf);
        shell_error(sh, "params wrong");
        return -EINVAL;
    }

    g_PbapPce.num_srmp_wait = local_srmp;
    g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

    retval = bt_pbap_pce_pull_phonebook(g_PbapPce.pbap_pceHandle, buf, name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        shell_error(sh, "command send fail");
        return -EINVAL;
    }
    return 0;
}

static int cmd_set_path(const struct shell *sh, size_t argc, char *argv[])
{
    API_RESULT retval = 0;
    struct net_buf *buf;

    memset(name, 0, sizeof(name));
    buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
    if (!buf)
    {
        return -EINVAL;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_SET_PATH(g_PbapPce.pbap_pceHandle));

    memcpy(name, argv[1], (uint16_t)strlen(argv[1]));
    retval = bt_pbap_pce_set_phonebook_path(g_PbapPce.pbap_pceHandle, buf, name);
    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        shell_error(sh, "command send fail");
        return -EINVAL;
    }
    return 0;
}

static int bt_pull_vcardlist_command_analy(struct net_buf *buf, size_t argc, char **argv)
{
    uint16_t index   = 1;
    uint64_t value   = 0;
    uint8_t sp_flag  = 0;
    uint8_t sv_flag  = 0;
    uint8_t cs_flag  = 0;
    uint8_t cso_flag = 0;
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-o") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "Order invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_ORDER(buf, value);
        }
        else if (strcmp(argv[index], "-sp") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "Search Property invaild");
                return -EINVAL;
            }
            if (value != 0 && value != 1 && value != 2)
            {
                shell_error(ctx_shell, "Search Property invaild");
                return -EINVAL;
            }
            sp_flag = 1;
            BT_PBAP_ADD_PARAMS_SEARCH_PROPERTY(buf, value);
        }
        else if (strcmp(argv[index], "-sv") == 0)
        {
            sv_flag = 1;
            BT_PBAP_ADD_PARAMS_SEARCH_VALUE(buf, (uint8_t *)argv[index + 1], (uint8_t)strlen(argv[index + 1]));
        }
        else if (strcmp(argv[index], "-mlc") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "MaxListCount invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, value);
        }
        else if (strcmp(argv[index], "-lso") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "ListStartOffset invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_PHONE_BOOK_SIZE(buf, value);
        }
        else if (strcmp(argv[index], "-rnmc") == 0)
        {
            if ((g_PbapPce.peer_feature & BT_PBAP_FEATURE_ENHANCED_MISSED_CALLS) && (g_PbapPce.loacal_feature & BT_PBAP_FEATURE_ENHANCED_MISSED_CALLS))
            {
                if (strTouint_64(argv[index + 1], &value) != 0)
                {
                    shell_error(ctx_shell, "ResetNewMissedCalls invaild");
                    return -EINVAL;
                }
                BT_PBAP_ADD_PARAMS_RESET_NEW_MISSED_CALLS(buf, value);
            }
            else
            {
                shell_error(ctx_shell, "pse or pce do not supported enhanced missed calls feature, params ResetNewMissedCalls add failed");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-cs") == 0)
        {
            if (g_PbapPce.peer_feature & BT_PBAP_FEATURE_VCARD_SELECTING && g_PbapPce.loacal_feature & BT_PBAP_FEATURE_VCARD_SELECTING)
            {
                if (strTouint_64(argv[index + 1], &value) != 0)
                {
                    shell_error(ctx_shell, "vCardSelector invaild");
                    return -EINVAL;
                }
                cs_flag = 1;
                BT_PBAP_ADD_PARAMS_VCARD_SELECTOR(buf, value);
            }
            else
            {
                shell_error(ctx_shell, "pse or pce do not supported vCard Selecting feature, params vCardSelector add failed");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-cso") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "vCardSelectorOperator invaild");
                return -EINVAL;
            }
            cso_flag = 1;
            BT_PBAP_ADD_PARAMS_VCARD_SELECTOR_OPERATOR(buf, value);
        }
        else if (strcmp(argv[index], "-name") == 0)
        {
            memcpy(name, argv[index + 1], (uint16_t)strlen(argv[index + 1]));
        }
        else if (strcmp(argv[index], "-srmp") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, "SRMP invaild");
                return -EINVAL;
            }
            local_srmp = value;
        }
        else
        {
            /* no action */
        }
    }
    if (sv_flag == 1 && sp_flag == 0)
    {
        shell_error(ctx_shell, "params SearchValue is added but params SearchProperty is not be added");
        return -EINVAL;
    }
    if (cso_flag == 1 && cs_flag == 0)
    {
        shell_error(ctx_shell, "params vCardSelectorOperator is added but params vCardSelector is not be added");
        return -EINVAL;
    }
    return 0;
}

static int cmd_pull_vcardlist(const struct shell *sh, size_t argc, char *argv[])
{
    API_RESULT retval = 0;
    struct net_buf *buf;

    memset(name, 0, sizeof(name));
    buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
    if (!buf)
    {
        return -EINVAL;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));

    BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, 0xFFFF);
    retval = bt_pull_vcardlist_command_analy(buf, argc, argv);
    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        shell_error(sh, "params wrong");
        return -EINVAL;
    }

    g_PbapPce.num_srmp_wait = local_srmp;
    g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;
    if (strlen(name) > 0)
    {
        retval = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    }
    else
    {
        retval = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    }

    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        shell_error(sh, "command send fail");
        return -EINVAL;
    }
    return 0;
}

static int bt_pull_vcardentry_command_analy(struct net_buf *buf, size_t argc, char **argv)
{
    uint16_t index = 1;
    uint64_t value = 0;
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-ps") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, " Property Selsector  invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_PROPERTY_SELECTOR(buf, value);
        }
        else if (strcmp(argv[index], "-f") == 0)
        {
            if (strTouint_64(argv[index + 1], &value) != 0)
            {
                shell_error(ctx_shell, " Format invaild");
                return -EINVAL;
            }
            if (value != 1 && value != 0)
            {
                shell_error(ctx_shell, "Format invaild");
                return -EINVAL;
            }
            BT_PBAP_ADD_PARAMS_FORMAT(buf, value);
        }
        else if (strcmp(argv[index], "-name") == 0)
        {
            if (strstr(argv[index + 1], ".vcf") == NULL && strstr(argv[index + 1], "X-BT-UID:") == NULL)
            {
                shell_error(ctx_shell, "filename invaild");
                return -EINVAL;
            }
            memcpy(name, argv[index + 1], (uint16_t)strlen(argv[index + 1]));
        }
        else
        {
            /* no action */
        }
    }
    return 0;
}

static int cmd_pull_vcardentry(const struct shell *sh, size_t argc, char *argv[])
{
    API_RESULT retval = 0;
    struct net_buf *buf;

    memset(name, 0, sizeof(name));
    buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
    if (!buf)
    {
        return -EINVAL;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));
    retval = bt_pull_vcardentry_command_analy(buf, argc, argv);
    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        shell_error(sh, "params wrong");
        return -EINVAL;
    }

    g_PbapPce.num_srmp_wait = local_srmp;
    g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;
    retval  = bt_pbap_pce_pull_vcard_entry(g_PbapPce.pbap_pceHandle, buf, name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        shell_error(sh, "command send fail");
        return -EINVAL;
    }
    return 0;
}

static int cmd_register(const struct shell *sh, size_t argc, char *argv[])
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err = 0;
    buf     = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(PBAP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
        shell_error(sh, "write class of device failed, Reason : %d", err);
    }
    shell_pbap_init();
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(pce_cmds,
                               SHELL_CMD_ARG(register, NULL, HELP_NONE, cmd_register, 1, 0),
                SHELL_CMD_ARG(connect, NULL, "SDP first, then connect.\r\n\
                            -psm(optional).\r\n\
                            obex auth params(optional)\r\n\
                            -uid   : [userid].\r\n\
                            -pwd   : [password].\r\n", cmd_connect, 1, 5),
                               SHELL_CMD_ARG(disconnect, NULL, HELP_NONE, cmd_disconnect, 1, 0),
                               SHELL_CMD_ARG(abort, NULL, HELP_NONE, cmd_abort, 1, 0),
                SHELL_CMD_ARG(pull_phonebook, NULL,"\r\n\
                           -name(mandatory) : [name].\r\n \
                           -srmp(optional)  : [Single Response Mode Param(>=0)].\r\n\
                           input application parameters(optional). \r\n\
                           1: -ps  : [Property Selector (64-bit)].\r\n\
                           2: -f   : [Format(0: vcard 2.1 | 1 : vcard 3.0)].\r\n\
                           3: -mlc : [MaxListCount (0 - 0xFFFF)].\r\n\
                           4: -lso : [ListStartOffset (0 - 0xFFFF)].\r\n\
                           5: -rnmc: [RestNewMissedCalls(0/1)].\r\n\
                           6: -cs  : [vCardSelector(64-bit)].\r\n\
                           7: -cso : [vCardSelecorOperator(0 : or | 1 : and)]\r\n ", cmd_pull_pb, 3, 18),
                               SHELL_CMD_ARG(set_path, NULL, "[path_name]\r\n", cmd_set_path, 2, 0),
                SHELL_CMD_ARG(pull_vcardlist, NULL, "\r\n \
                           -name(optional) : [name].\r\n \
                           -srmp(optional)  : [Single Response Mode Param(>=0)].\r\n\
                           input application parameters(optional). \r\n\
                           1: -o   : [order(0 : Indexed | 1 : Alphanumeric | 2 : Phonetical)].\r\n\
                           2: -sp  : [SearchProperty(0 : name | 1 : number | 2 : sound)].\r\n\
                           3: -sv  : [SearchValue(string)].\r\n\
                           4: -mlc : [MaxListCount (0 - 0xFFFF)].\r\n\
                           5: -lso : [ListStartOffset (0 - 0xFFFF)].\r\n\
                           6: -rnmc: [ResetNewMissedCalls(0/1)].\r\n\
                           7: -cs  : [vCardSelector (64-bit)].\r\n\
                           8: -cso : [vCardSelecorOperator(0 : or | 1 : and)].\r\n ", cmd_pull_vcardlist, 1, 18),
                SHELL_CMD_ARG(pull_vcardentry, NULL, "\r\n \
                           -name(mandatory) : [name].\r\n \
                           -srmp(optional)  : [Single Response Mode Param(>=0)].\r\n\
                           input application parameters(optional).\r\n\
                           1: -ps  : [Property Selector (64-bit)].\r\n\
                           2: -f   : [Format(0: vcard 2.1 | 1 : vcard 3.0)].\r\n", cmd_pull_vcardentry, 3, 6),
                               SHELL_SUBCMD_SET_END);
#endif /* CONFIG_BT_PBAP_PCE */

#if (defined(CONFIG_BT_PBAP_PSE) && ((CONFIG_BT_PBAP_PSE) > 0U))

struct appl_params
{
    /**
     * Attribute Mask
     */
    uint64_t property_selector;

    /**
     * Maximum entries in the list
     */
    uint16_t max_list_count;

    /**
     * Offset of the first entry
     */
    uint16_t list_start_offset;

    /**
     * Number of indexes in phonebook
     */
    uint16_t phonebook_size;

    /**
     * Application Parameter Bit field
     */

    /**
     *  16 Bit Flag used as below
     *
     *  Usage:
     *       15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
     *      [  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ]
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |  |  |____Order
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |  |_______Search Value
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |__________Search Attribute
     *        |  |  |  |  |  | |  |   |  |  |  |  |_____________Max List Count
     *        |  |  |  |  |  | |  |   |  |  |  |________________List Start Offset
     *        |  |  |  |  |  | |  |   |  |  |___________________Property Selector
     *        |  |  |  |  |  | |  |   |  |______________________Format
     *        |  |  |  |  |  | |  |   |_________________________Phone book size
     *        |  |  |  |  |  | |  |_____________________________New Missed Calls
     *        |  |  |  |  |  | |________________________________PrimaryVersionCounter
     *        |  |  |  |  |  |__________________________________SecondaryVersionCounter
     *        |  |  |  |  |_____________________________________vCardSelector
     *        |  |  |  |________________________________________DatabaseIdentifier
     *        |  |  |___________________________________________vCardSelectorOperator
     *        |  |______________________________________________ResetNewMissedCalls
     *        |_________________________________________________PbapSupportedFeatures
     *
     *
     */
    uint16_t appl_param_flag;

    /**
     * Sorting Order
     */
    uint8_t order;

    /**
     * Attribute to be searched
     */
    uint8_t search_attr;

    /**
     * vCard Format 2.1 or 3.0
     */
    uint8_t format;

    /**
     * Number of new missed calls
     */
    uint8_t new_missed_calls;
    /**
     * PSE primary folder version
     */
    uint8_t primary_folder_ver[16];

    /**
     * PSE secodary folder version
     */
    uint8_t secondary_folder_ver[16];

    /**
     * PSE database identifier
     */
    uint8_t database_identifier[16];

    /**
     * vCard Selector, Attribute mask
     */
    uint64_t vcard_selector;

    /**
     * vCard selector operator when multiple
     * attributes are slector
     */
    uint8_t vcard_selector_operator;

    /**
     * To reset newmissed calls
     */
    uint8_t reset_new_missed_calls;

    /**
     * PBAP supported features
     */
    uint32_t supported_features;

    /**
     * Value of the Search attribute
     */
    struct pbap_hdr search_value;
};
#define CURRENT_PATH_MAX_LEN 30
typedef struct app_pbap_pse_
{
    struct bt_pbap_pse *pbap_pseHandle;
    struct bt_conn *conn;
    uint8_t peer_bd_addr[6];
    char currentpath[CURRENT_PATH_MAX_LEN];
    uint32_t remaining_rsp;
    struct appl_params req_appl_params;
    char name[42];
    uint32_t send_rsp;
    uint32_t lcl_supported_features;
    uint32_t rem_supported_features;
} app_pbap_pse_t;

app_pbap_pse_t g_PbapPse;

#define PBAP_PSE_MAX_PKT_SIZE OBEX_MAX_PACKET_LENGTH

NET_BUF_POOL_FIXED_DEFINE(pbap_appl_pse_pool, CONFIG_BT_MAX_CONN, PBAP_PSE_MAX_PKT_SIZE, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

#define PBAP_CLASS_OF_DEVICE (0x10020CU) /* Object Transfer, Phone, Smartphone */

static uint8_t sample_primay_folder_version[16] =
       {
          0x05U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U
       };

static uint8_t sample_secondary_folder_version[16] =
       {
          0x06U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U
       };

static uint8_t sample_database_identifier[16] =
       {
          0x07U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x03U
       };

static struct bt_sdp_attribute pbap_pse_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
                BT_SDP_DATA_ELEM_LIST(
                    {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_PBAP_PSE_SVCLASS) //11 2F
        },
        )
    ),
    /* ProtocolDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                                               BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
                                           },
                                           {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_PBAP_PSE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                                               BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
    /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PBAP_SVCLASS) //11 30
                                           },
                                           {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0102U) //01 02
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Phonebook Access PSE"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
   {
       BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
       BT_SDP_ARRAY_16(BT_BR_PSM_PBAP_PSE)
   },

    /* SupportedRepositories */
    BT_SDP_ATTR_SUPPORTED_REPOSITORIES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8),
        BT_SDP_ARRAY_8(CONFIG_BT_PBAP_PSE_SUPPORTED_REPOSITORIES)
    },

    /* PBAP_PSE SupportedFeatures */
    BT_SDP_ATTR_PBAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32),
        BT_SDP_ARRAY_32(CONFIG_BT_PBAP_PSE_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record pbap_pse_rec = BT_SDP_RECORD(pbap_pse_attrs);

static const char pbap_pse_phonebook_example[] =
    "BEGIN:VCARD\n\
VERSION:2.1\n\
FN;CHARSET=UTF-8:descvs\n\
N;CHARSET=UTF-8:descvs\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;cc;;;\n\
FN:cc\n\
TEL;CELL:154555845\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;qwe;;;\n\
FN:qwe\n\
X-ANDROID-CUSTOM:vnd.android.cursor.item/nickname;147;\n\
TEL;CELL:151865216\n\
TEL;CELL:153464856\n\
EMAIL;HOME:wudhxjsjd@qq.com\n\
ADR;HOME:;;123456789;;;;\n\
NOTE:old\n\
BDAY:1904-05-24\n\
X-AIM:@qq.com\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
FN;CHARSET=UTF-8:descvs\n\
N;CHARSET=UTF-8:descvs\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;cc;;;\n\
FN:cc\n\
TEL;CELL:154555845\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;qwe;;;\n\
FN:qwe\n\
X-ANDROID-CUSTOM:vnd.android.cursor.item/nickname;147;\n\
TEL;CELL:151865216\n\
TEL;CELL:153464856\n\
EMAIL;HOME:wudhxjsjd@qq.com\n\
ADR;HOME:;;123456789;;;;\n\
NOTE:old\n\
BDAY:1904-05-24\n\
X-AIM:@qq.com\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
FN;CHARSET=UTF-8:descvs\n\
N;CHARSET=UTF-8:descvs\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;cc;;;\n\
FN:cc\n\
TEL;CELL:154555845\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;qwe;;;\n\
FN:qwe\n\
X-ANDROID-CUSTOM:vnd.android.cursor.item/nickname;147;\n\
TEL;CELL:151865216\n\
TEL;CELL:153464856\n\
EMAIL;HOME:wudhxjsjd@qq.com\n\
ADR;HOME:;;123456789;;;;\n\
NOTE:old\n\
BDAY:1904-05-24\n\
X-AIM:@qq.com\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
FN;CHARSET=UTF-8:descvs\n\
N;CHARSET=UTF-8:descvs\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;cc;;;\n\
FN:cc\n\
TEL;CELL:154555845\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;qwe;;;\n\
FN:qwe\n\
X-ANDROID-CUSTOM:vnd.android.cursor.item/nickname;147;\n\
TEL;CELL:151865216\n\
TEL;CELL:153464856\n\
EMAIL;HOME:wudhxjsjd@qq.com\n\
ADR;HOME:;;123456789;;;;\n\
NOTE:old\n\
BDAY:1904-05-24\n\
X-AIM:@qq.com\n\
END:VCARD";

static const char pbap_pse_vcard_listing[] =
    "<?xml version=\"1.0\"?><!DOCTYPE vcard-listing SYSTEM \"vcard-listing.dtd\"><vCard-listing version=\"1.0\">\
<card handle=\"1.vcf\" name=\"qwe\"/><card handle=\"2.vcf\" name=\"qwe\"/><card handle=\"3.vcf\" name=\"qwe\"/>\
<card handle=\"4.vcf\" name=\"1155\"/><card handle=\"5.vcf\" name=\"051295205593\"/><card handle=\"6.vcf\" name=\"130\"/>/vCard-listing>";

static const char pbap_pse_vcard_entry[] =
    "BEGIN:VCARD\n\
VERSION:2.1\n\
FN:\n\
N:\n\
TEL;X-0:1155\n\
X-IRMC-CALL-DATETIME;DIALED:20220913T110607\n\
END:VCARD";

char *child_floader_name[8] = {"telecom", "pb", "ich", "och", "mch", "cch", "spd", "fav"};

static int startwith(char *str, char *prefix)
{
    uint8_t str_len    = strlen(str);
    uint8_t prefix_len = strlen(prefix);
    if (prefix_len > str_len)
    {
        return 0;
    }
    return strncmp(str, prefix, prefix_len) == 0;
}
static int endwith(char *str, char *suffix)
{
    uint8_t str_len    = strlen(str);
    uint8_t suffix_len = strlen(suffix);
    if (str_len < suffix_len)
    {
        return 0;
    }
    char *endpart = str + str_len - suffix_len;
    return strcmp(endpart, suffix) == 0;
}

static bool app_app_pse_param_cb(struct bt_data *data, void *user_data)
{
    switch (data->type)
    {
        case BT_PBAP_TAG_ID_ORDER:
            g_PbapPse.req_appl_params.order = data->data[0];
            shell_print(ctx_shell, "appl params order : %d", g_PbapPse.req_appl_params.order);
            break;
        case BT_PBAP_TAG_ID_SEARCH_VALUE:
            g_PbapPse.req_appl_params.search_value.value  = (uint8_t *)data->data;
            g_PbapPse.req_appl_params.search_value.length = data->data_len;
            shell_print(ctx_shell, "appl params search value : %.*s", data->data_len, data->data);
            break;
        case BT_PBAP_TAG_ID_SEARCH_PROPERTY:
            g_PbapPse.req_appl_params.search_attr = data->data[0];
            shell_print(ctx_shell, "appl params search property : %d", g_PbapPse.req_appl_params.search_attr);
            break;
        case BT_PBAP_TAG_ID_MAX_LIST_COUNT:
            g_PbapPse.req_appl_params.max_list_count = sys_get_be16(data->data);
            shell_print(ctx_shell, "appl params max list count : %d", g_PbapPse.req_appl_params.max_list_count);
            break;
        case BT_PBAP_TAG_ID_LIST_START_OFFSET:
            g_PbapPse.req_appl_params.list_start_offset = sys_get_be16(data->data);
            shell_print(ctx_shell, "appl params list start offset : %d", g_PbapPse.req_appl_params.list_start_offset);
            break;
        case BT_PBAP_TAG_ID_PROPERTY_SELECTOR:
            g_PbapPse.req_appl_params.property_selector = sys_get_be64(data->data);
            shell_print(ctx_shell, "appl params property selector : %llu", g_PbapPse.req_appl_params.property_selector);
            break;
        case BT_PBAP_TAG_ID_FORMAT:
            g_PbapPse.req_appl_params.format = data->data[0];
            shell_print(ctx_shell, "appl params format : %d", g_PbapPse.req_appl_params.format);
            break;
        case BT_PBAP_TAG_ID_PHONE_BOOK_SIZE:
            g_PbapPse.req_appl_params.phonebook_size = sys_get_be16(data->data);
            shell_print(ctx_shell, "appl params phonebook size : %d", g_PbapPse.req_appl_params.phonebook_size);
            break;
        case BT_PBAP_TAG_ID_NEW_MISSED_CALLS:
            g_PbapPse.req_appl_params.new_missed_calls = data->data[0];
            shell_print(ctx_shell, "appl params new mossed calls : %d", g_PbapPse.req_appl_params.new_missed_calls);
            break;
        case BT_PBAP_TAG_ID_VCARD_SELECTOR:
            g_PbapPse.req_appl_params.vcard_selector = sys_get_be64(data->data);
            shell_print(ctx_shell, "appl params vcard selector : %llu", g_PbapPse.req_appl_params.vcard_selector);
            break;
        case BT_PBAP_TAG_ID_VCARD_SELECTOR_OPERATOR:
            g_PbapPse.req_appl_params.vcard_selector_operator = data->data[0];
            shell_print(ctx_shell, "appl params vcard selector operator : %d", g_PbapPse.req_appl_params.vcard_selector_operator);
            break;
        case BT_PBAP_TAG_ID_RESET_NEW_MISSED_CALLS:
            g_PbapPse.req_appl_params.reset_new_missed_calls = data->data[0];
            shell_print(ctx_shell, "appl params reset new missed calls : %d", g_PbapPse.req_appl_params.reset_new_missed_calls);
            break;
        case BT_PBAP_TAG_ID_PBAP_SUPPORTED_FEATURES:
            g_PbapPse.req_appl_params.supported_features = sys_get_be32(data->data);
            shell_print(ctx_shell, "appl params supported features  : %d", g_PbapPse.req_appl_params.supported_features);
            break;
        default:
            break;
    }
    return 1;
}

static void app_pse_connected(struct bt_pbap_pse *pbap_pse)
{
    shell_print(ctx_shell, "PBAP connect successfully");
    memcpy(g_PbapPse.currentpath, "root", 4);
    g_PbapPse.pbap_pseHandle         = pbap_pse;
    g_PbapPse.lcl_supported_features = CONFIG_BT_PBAP_PSE_SUPPORTED_FEATURES;
    bt_pbap_pse_get_peer_supported_features(pbap_pse, &g_PbapPse.rem_supported_features);
    g_PbapPse.send_rsp      = 0;
    g_PbapPse.remaining_rsp = 0;
}

static void app_pse_get_auth_info_cb(struct bt_pbap_pse *pbap_pse, struct bt_pbap_auth *pbap_auth_info, bool *active_auth)
{
    strcpy(userid, "PBAP");
    pbap_auth_info->user_id = userid;
    strcpy(password, "0000");
    pbap_auth_info->pin_code = password;
    *active_auth             = 0;
    shell_print(ctx_shell, "%s", pbap_auth_info->pin_code);
}

static void app_pse_disconnected(struct bt_pbap_pse *pbap_pse, uint8_t result)
{
    shell_print(ctx_shell, "PBAP disconnect successfully : %x", result);
    if (result == BT_PBAP_FORBIDDEN_RSP)
    {
        shell_print(ctx_shell, "Possible reasons is Authentication failure");
    }
}

static int app_check_pull_phonebook_path(char *name)
{
    uint8_t length = strlen(name);
    uint8_t index  = 0;
    if (name == NULL || length < strlen("pb.vcf"))
    {
        return -EINVAL;
    }
    if (endwith((char *)name, ".vcf") == 0)
    {
        shell_error(ctx_shell, "name invaild");
        return -EINVAL;
    }
    /* Check if requested phonebook file has 'SIMI' */
    char *pb_ptr = (char *)BT_str_str(name, "SIM1");
    if (NULL == pb_ptr)
    {
        /* pb_file_name does not have 'SIM1', check for 'telecom' */
        pb_ptr = (char *)BT_str_str(name, "telecom");
        if (NULL != pb_ptr)
        {
            memcpy(g_PbapPse.currentpath, "root/telecom", strlen("root/telecom"));
        }
        else
        {
            /* Requested pb_file_name is not valid, Send Error */
            shell_error(ctx_shell, "name is invailed");
            return -EINVAL;
        }
    }
    else
    {
        index += 5U;
        pb_ptr = (char *)BT_str_str(&name[index], (char *)"telecom");
        if (NULL != pb_ptr)
        {
            memcpy(g_PbapPse.currentpath, "root/SIM1/telecom", strlen("root/SIM1/telecom"));
        }
        else
        {
            /* Requested pb_file_name is not valid, Send Error */
            shell_error(ctx_shell, "name is invailed");
            return -EINVAL;
        }
    }
    return 0;
}

static void app_pse_pull_phonebook_cb(struct bt_pbap_pse *pbap_pse,
                                      struct net_buf *buf,
                                      char *name,
                                      enum bt_obex_req_flags flag)
{
    struct pbap_hdr body;
    int revert           = 0;
    uint8_t result       = BT_PBAP_SUCCESS_RSP;
    uint16_t max_pkt_len = 0;
    if (pbap_pse == NULL || buf == NULL)
    {
        return;
    }
    bt_pbap_pse_app_param_parse(buf, app_app_pse_param_cb, NULL);
    net_buf_unref(buf);
    if (name != NULL)
    {
        if ((char *)BT_str_str(name, "telecom") == NULL)
        {
            memcpy(g_PbapPse.name, g_PbapPse.currentpath, strlen(g_PbapPse.currentpath));
            strcat(g_PbapPse.name, name);
        }
        else
        {
            memcpy(g_PbapPse.name, name, strlen(name) + 1);
        }
    }
    if (!(flag & BT_OBEX_REQ_UNSEG)) /* stack do not support now */
    {
        g_PbapPse.remaining_rsp = 0;
        result                  = BT_PBAP_NOT_IMPLEMENTED_RSP;
    }
    else
    {
        if (g_PbapPse.remaining_rsp == 0)
        {
            if (app_check_pull_phonebook_path(g_PbapPse.name) == 0)
            {
                memcpy(g_PbapPse.currentpath, "root\0", strlen("root\0"));
                g_PbapPse.remaining_rsp = strlen(pbap_pse_phonebook_example);
                if (g_PbapPse.remaining_rsp == 0)
                {
                    result = BT_PBAP_NOT_FOUND_RSP;
                }
                else
                {
                    g_PbapPse.send_rsp = 0;
                }
            }
            else
            {
                buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
                if (!buf)
                {
                    return;
                }
                net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
                result                  = BT_PBAP_NOT_FOUND_RSP;
                g_PbapPse.remaining_rsp = 0;
            }
        }
    }
    if (g_PbapPse.remaining_rsp != 0)
    {
        buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
        if (!buf)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
        if ((g_PbapPse.lcl_supported_features & BT_PBAP_FEATURE_FOLDER_VERSION_COUNTERS) &&
            (g_PbapPse.rem_supported_features & BT_PBAP_FEATURE_FOLDER_VERSION_COUNTERS))
        {
            BT_PBAP_ADD_PARAMS_PRIMARY_FOLDER_VERSION(buf, sample_primay_folder_version);
            BT_PBAP_ADD_PARAMS_SECONDARY_FOLDER_VERSION(buf, sample_secondary_folder_version);
        }
        if ((g_PbapPse.lcl_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER) &&
            (g_PbapPse.rem_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER))
        {
            BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, sample_database_identifier);
        }
        bt_pbap_pse_get_max_pkt_len(pbap_pse, &max_pkt_len);
        max_pkt_len -= BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse);
        max_pkt_len -= buf->len;
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes);
        if (g_PbapPse.remaining_rsp <= max_pkt_len)
        {
            body.value  = (uint8_t *)(uint8_t *)&pbap_pse_phonebook_example[g_PbapPse.send_rsp];
            body.length = g_PbapPse.remaining_rsp;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp  = 0;
            result                   = BT_PBAP_SUCCESS_RSP;
            g_PbapPse.currentpath[4] = 0;
        }
        else
        {
            body.value  = (uint8_t *)&pbap_pse_phonebook_example[g_PbapPse.send_rsp];
            body.length = max_pkt_len;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp -= body.length;
            g_PbapPse.send_rsp += body.length;
            result = BT_PBAP_CONTINUE_RSP;
        }
    }
    shell_print(ctx_shell, "send response : %x", result);
    revert = bt_pbap_pse_pull_phonebook_response(pbap_pse, result, buf, 0);
    if (revert != 0)
    {
        net_buf_unref(buf);
        shell_error(ctx_shell, "Send Response failed %x", revert);
    }
}

static void app_pse_set_phonebook_path_cb(struct bt_pbap_pse *pbap_pse, char *name)
{
    int revert      = 0;
    uint8_t result  = 0;
    uint8_t index   = 0;
    char *path_name = NULL;
    if (name == NULL)
    {
        return;
    }
    shell_print(ctx_shell, "pse current path is %s", g_PbapPse.currentpath);
    if (strcmp(name, "/") == 0)
    {
        shell_print(ctx_shell, "set path to root");
        memset(g_PbapPse.currentpath, 0, sizeof(g_PbapPse.currentpath));
        memcpy(g_PbapPse.currentpath, "root\0", 5);
        result = BT_PBAP_SUCCESS_RSP;
    }
    else if (strcmp(name, "..") == 0)
    {
        if (strcmp(g_PbapPse.currentpath, "root") == 0)
        {
            shell_print(ctx_shell, "current path is root");
            result = BT_PBAP_NOT_FOUND_RSP;
        }
        else
        {
            for (int8_t index = strlen(g_PbapPse.currentpath); index >= 0; index--)
            {
                if (g_PbapPse.currentpath[index] == '/')
                {
                    g_PbapPse.currentpath[index] = 0;
                    break;
                }
            }
            shell_print(ctx_shell, "set path to parent");
            result = BT_PBAP_SUCCESS_RSP;
        }
    }
    else if (name[0] == '.' && name[1] == '/' && strlen(name) > 2)
    {
        path_name = &name[2];
        shell_print(ctx_shell, "set path to child %s", path_name);

        if (strcmp(path_name, "SIM1") == 0)
        {
            if (strcmp(g_PbapPse.currentpath, "root") == 0)
            {
                strcat(g_PbapPse.currentpath, "/");
                strcat(g_PbapPse.currentpath, path_name);
                result = BT_PBAP_SUCCESS_RSP;
            }
            else
            {
                result = BT_PBAP_NOT_FOUND_RSP;
            }
        }
        for (index = 0; index < 8; index++)
        {
            if (strcmp(path_name, child_floader_name[index]) == 0)
            {
                if (index == 0)
                {
                    if (endwith(g_PbapPse.currentpath, "root") || endwith(g_PbapPse.currentpath, "SIM1"))
                    {
                        strcat(g_PbapPse.currentpath, "/");
                        strcat(g_PbapPse.currentpath, path_name);
                        result = BT_PBAP_SUCCESS_RSP;
                        break;
                    }
                    else
                    {
                        shell_error(ctx_shell, "path incorrent");
                        result = BT_PBAP_NOT_FOUND_RSP;
                        break;
                    }
                }
                else
                {
                    if (endwith(g_PbapPse.currentpath, "telecom"))
                    {
                        strcat((char *)g_PbapPse.currentpath, "/");
                        strcat((char *)g_PbapPse.currentpath, path_name);
                        result = BT_PBAP_SUCCESS_RSP;
                        break;
                    }
                    else
                    {
                        shell_error(ctx_shell, "path incorrent");
                        result = BT_PBAP_NOT_FOUND_RSP;
                        break;
                    }
                }
            }
        }
        if (index >= 8)
        {
            shell_error(ctx_shell, "path incorrent");
            result = BT_PBAP_NOT_FOUND_RSP;
        }
    }
    if (result == BT_PBAP_SUCCESS_RSP)
    {
        shell_print(ctx_shell, "pse set current path is %s", g_PbapPse.currentpath);
    }
    revert = bt_pbap_pse_set_phonebook_path_response(pbap_pse, result);
    if (API_SUCCESS != revert)
    {
        shell_error(ctx_shell, "Send Response failed %x", revert);
    }
}

static int app_check_pull_vcard_listing_path(char *name)
{
    uint8_t index = 0;
    if (name == NULL || strlen(name) == 0)
    {
        for(index = 1; index < 8; index++)
        {
            if (endwith(g_PbapPse.currentpath, child_floader_name[index]) == 1)
            {
                return 0;
            }
        }
        return -EINVAL;
    }
    for (index = 0; index < 8; index++)
    {
        if (strcmp(name, child_floader_name[index]) == 0)
        {
            break;
        }
    }
    if (index == 8)
    {
        return -EINVAL;
    }
    if (endwith(g_PbapPse.currentpath, "telecom"))
    {
        return 0;
    }
    else
    {
        return -EINVAL;
    }
}

static void app_pse_pull_vcard_listing_cb(struct bt_pbap_pse *pbap_pse,
                                          struct net_buf *buf,
                                          char *name,
                                          enum bt_obex_req_flags flag)
{
    struct pbap_hdr body;
    int revert           = 0;
    uint8_t result       = BT_PBAP_SUCCESS_RSP;
    uint16_t max_pkt_len = 0;
    if (pbap_pse == NULL || buf == NULL)
    {
        return;
    }
    bt_pbap_pse_app_param_parse(buf, app_app_pse_param_cb, NULL);
    net_buf_unref(buf);
    g_PbapPse.name[0] = 0;
    if (name != NULL)
    {
        memcpy(g_PbapPse.name, name, strlen(name) + 1);
    }
    if (!(flag & BT_OBEX_REQ_UNSEG)) /* stack do not support now */
    {
        g_PbapPse.remaining_rsp = 0;
        result                  = BT_PBAP_NOT_IMPLEMENTED_RSP;
    }
    else
    {
        if (g_PbapPse.remaining_rsp == 0)
        {
            if (app_check_pull_vcard_listing_path(g_PbapPse.name) == 0)
            {
                g_PbapPse.remaining_rsp = strlen(pbap_pse_vcard_listing);
                if (g_PbapPse.remaining_rsp == 0)
                {
                    result = BT_PBAP_NOT_FOUND_RSP;
                }
                else
                {
                    g_PbapPse.send_rsp = 0;
                }
            }
            else
            {
                buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
                if (!buf)
                {
                    return;
                }
                net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
                result                  = BT_PBAP_NOT_FOUND_RSP;
                g_PbapPse.remaining_rsp = 0;
            }
        }
    }
    if (g_PbapPse.remaining_rsp != 0)
    {
        buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
        if (!buf)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
        if ((g_PbapPse.lcl_supported_features & BT_PBAP_FEATURE_FOLDER_VERSION_COUNTERS) &&
            (g_PbapPse.rem_supported_features & BT_PBAP_FEATURE_FOLDER_VERSION_COUNTERS))
        {
            BT_PBAP_ADD_PARAMS_PRIMARY_FOLDER_VERSION(buf, sample_primay_folder_version);
            BT_PBAP_ADD_PARAMS_SECONDARY_FOLDER_VERSION(buf, sample_secondary_folder_version);
        }
        if ((g_PbapPse.lcl_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER) &&
            (g_PbapPse.rem_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER))
        {
            BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, sample_database_identifier);
        }
        bt_pbap_pse_get_max_pkt_len(pbap_pse, &max_pkt_len);
        max_pkt_len -= BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse);
        max_pkt_len -= buf->len;
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes);
        if (g_PbapPse.remaining_rsp <= max_pkt_len)
        {
            body.value  = (uint8_t *)(uint8_t *)&pbap_pse_vcard_listing[g_PbapPse.send_rsp];
            body.length = g_PbapPse.remaining_rsp;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp = 0;
            result                  = BT_PBAP_SUCCESS_RSP;
        }
        else
        {
            body.value  = (uint8_t *)&pbap_pse_vcard_listing[g_PbapPse.send_rsp];
            body.length = max_pkt_len;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp -= body.length;
            g_PbapPse.send_rsp += body.length;
            result = BT_PBAP_CONTINUE_RSP;
        }
    }
    shell_print(ctx_shell, "send response : %x", result);
    revert = bt_pbap_pse_pull_vcard_listing_response(pbap_pse, result, buf, 0);
    if (revert != 0)
    {
        net_buf_unref(buf);
        shell_error(ctx_shell, "Send Response failed %x", revert);
    }
}

static int app_check_pull_vcard_entry_path(char *name)
{
    uint8_t index = 1;
    if (name == NULL)
    {
        return -EINVAL;
    }
    if (endwith(name, ".vcf") == 0)
    {
        if (startwith(name, "X-BT-UID") == 0)
        {
            return -EINVAL;
        }
    }
    for (index = 1; index < 8; index++)
    {
        if (endwith(g_PbapPse.currentpath, child_floader_name[index]))
        {
            break;
        }
    }
    if (index >= 8)
    {
        return -EINVAL;
    }
    return 0;
}

static void app_pse_pull_vcard_entry_cb(struct bt_pbap_pse *pbap_pse,
                                        struct net_buf *buf,
                                        char *name,
                                        enum bt_obex_req_flags flag)
{
    struct pbap_hdr body;
    int revert           = 0;
    uint8_t result       = BT_PBAP_SUCCESS_RSP;
    uint16_t max_pkt_len = 0;
    if (pbap_pse == NULL || buf == NULL)
    {
        return;
    }
    bt_pbap_pse_app_param_parse(buf, app_app_pse_param_cb, NULL);
    net_buf_unref(buf);
    if (name != NULL)
    {
        memcpy(g_PbapPse.name, name, strlen(name) + 1);
    }
    if (!(flag & BT_OBEX_REQ_UNSEG)) /* stack do not support now */
    {
        g_PbapPse.remaining_rsp = 0;
        result                  = BT_PBAP_NOT_IMPLEMENTED_RSP;
    }
    else
    {
        if (g_PbapPse.remaining_rsp == 0)
        {
            if (app_check_pull_vcard_entry_path(g_PbapPse.name) == 0)
            {
                g_PbapPse.remaining_rsp = strlen(pbap_pse_vcard_entry);
                if (g_PbapPse.remaining_rsp == 0)
                {
                    result = BT_PBAP_NOT_FOUND_RSP;
                }
                else
                {
                    g_PbapPse.send_rsp = 0;
                }
            }
            else
            {
                buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
                if (!buf)
                {
                    return;
                }
                net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
                result                  = BT_PBAP_NOT_FOUND_RSP;
                g_PbapPse.remaining_rsp = 0;
            }
        }
    }
    if (g_PbapPse.remaining_rsp != 0)
    {
        buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
        if (!buf)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
        if ((g_PbapPse.lcl_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER) &&
            (g_PbapPse.rem_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER))
        {
            BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, sample_database_identifier);
        }
        bt_pbap_pse_get_max_pkt_len(pbap_pse, &max_pkt_len);
        max_pkt_len -= BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse);
        max_pkt_len -= buf->len;
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes);
        if (g_PbapPse.remaining_rsp <= max_pkt_len)
        {
            body.value  = (uint8_t *)(uint8_t *)&pbap_pse_vcard_entry[g_PbapPse.send_rsp];
            body.length = g_PbapPse.remaining_rsp;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp = 0;
            result                  = BT_PBAP_SUCCESS_RSP;
        }
        else
        {
            body.value  = (uint8_t *)&pbap_pse_vcard_entry[g_PbapPse.send_rsp];
            body.length = max_pkt_len;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp -= body.length;
            g_PbapPse.send_rsp += body.length;
            result = BT_PBAP_CONTINUE_RSP;
        }
    }

    shell_print(ctx_shell, "send response : %x", result);
    revert = bt_pbap_pse_pull_vcard_entry_response(pbap_pse, result, buf, 0);
    if (revert != 0)
    {
        net_buf_unref(buf);
        shell_error(ctx_shell, "Send Response failed %x", revert);
    }
}

static struct bt_pbap_pse_cb pse_cb = {
    .connected          = app_pse_connected,
    .disconnected       = app_pse_disconnected,
    .get_auth_info      = app_pse_get_auth_info_cb,
    .pull_phonebook     = app_pse_pull_phonebook_cb,
    .set_phonebook_path = app_pse_set_phonebook_path_cb,
    .pull_vcard_listing = app_pse_pull_vcard_listing_cb,
    .pull_vcard_entry   = app_pse_pull_vcard_entry_cb,
};

static int shell_pbap_pse_init()
{
    int err;
    bt_sdp_register_service(&pbap_pse_rec);
    err = bt_pbap_pse_register(&pse_cb);
    if (err != 0)
    {
        shell_error(ctx_shell, "PBAP register failed");
        return -EINVAL;
    }
    return 0;
}

static int cmd_pse_register(const struct shell *sh, size_t argc, char *argv[])
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err = 0;
    buf     = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(PBAP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
        shell_error(sh, "write class of device failed, Reason : %d", err);
    }
    if (err < 0)
    {
        return -EINVAL;
    }
    return shell_pbap_pse_init();
}

static int cmd_pse_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
    API_RESULT retval = 0;
    retval            = bt_pbap_pse_disconnect(g_PbapPse.pbap_pseHandle);

    if (API_SUCCESS != retval)
    {
        shell_error(sh, "disconnect failed Reason 0x%02X", retval);
        return -EINVAL;
    }
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(pse_cmds,
                               SHELL_CMD_ARG(register, NULL, HELP_NONE, cmd_pse_register, 1, 0),
                               SHELL_CMD_ARG(disconnect, NULL, HELP_NONE, cmd_pse_disconnect, 1, 0),
                               SHELL_SUBCMD_SET_END);
#endif /*CONFIG_BT_PBAP_PSE */

static int cmd_pce_pse(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1)
    {
        shell_help(sh);
        /* shell returns 1 when help is printed */
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

#if ((defined(CONFIG_BT_PBAP_PCE) && ((CONFIG_BT_PBAP_PCE) > 0U)) || \
     (defined(CONFIG_BT_PBAP_PSE) && ((CONFIG_BT_PBAP_PSE) > 0U)))
SHELL_STATIC_SUBCMD_SET_CREATE(pbap_cmds,
#if (defined(CONFIG_BT_PBAP_PCE) && ((CONFIG_BT_PBAP_PCE) > 0U))
                               SHELL_CMD_ARG(pce, pce_cmds, HELP_NONE, cmd_pce_pse, 1, 0),
#endif
#if (defined(CONFIG_BT_PBAP_PSE) && ((CONFIG_BT_PBAP_PSE) > 0U))
                               SHELL_CMD_ARG(pse, pse_cmds, HELP_NONE, cmd_pce_pse, 1, 0),
#endif
                               SHELL_SUBCMD_SET_END);
#endif

static int cmd_pbap(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1)
    {
        shell_help(sh);
        /* shell returns 1 when help is printed */
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

SHELL_CMD_ARG_REGISTER(pbap, pbap_cmds, "Bluetooth pbap shell commands", cmd_pbap, 1, 1);

void bt_ShellPbapInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandpbap))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandpbap.pcCommand);
    }
}

#endif
