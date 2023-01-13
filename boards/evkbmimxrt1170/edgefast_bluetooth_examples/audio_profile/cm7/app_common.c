/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <porting.h>
#include "app_common.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/sdp.h>
#include "app_a2dp.h"
#include "fsl_debug_console.h"
#include "cmd.h"

#define APP_INQUIRY_LENGTH        (10) /* 10 * 1.28 Sec */
#define APP_INQUIRY_NUM_RESPONSES (10)

static struct bt_br_discovery_result br_discovery_results[APP_INQUIRY_NUM_RESPONSES];

static QueueHandle_t app_control_queue = NULL;

#define SDP_CLIENT_USER_BUF_LEN		512U
NET_BUF_POOL_FIXED_DEFINE(app_sdp_client_pool, CONFIG_BT_MAX_CONN,
			  SDP_CLIENT_USER_BUF_LEN, NULL);

static uint8_t app_sdp_a2sink_user(struct bt_conn *conn, struct bt_sdp_client_result *result);
static struct bt_sdp_discover_params discov_a2dp_sink =
{
    .uuid = BT_UUID_DECLARE_16(BT_SDP_AUDIO_SINK_SVCLASS),
    .func = app_sdp_a2sink_user,
    .pool = &app_sdp_client_pool,
};

static struct bt_sdp_attribute a2dp_source_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
            BT_SDP_ARRAY_16(BT_SDP_AUDIO_SOURCE_SVCLASS)
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0X0100u)
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0x0103U)
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("A2DPSink"),
    BT_SDP_SUPPORTED_FEATURES(0x0001U),
};

static struct bt_sdp_record a2dp_source_rec = BT_SDP_RECORD(a2dp_source_attrs);

static void app_bt_set_task_msg(uint8_t controlCode)
{
    if (0U != __get_IPSR())
    {
        portBASE_TYPE task_to_wake = pdFALSE;
        if (xQueueSendToBackFromISR(app_control_queue, &controlCode, &task_to_wake) == pdTRUE)
        {
            portYIELD_FROM_ISR((task_to_wake));
        }
    }
    else
    {
        if (xQueueSendToBack(app_control_queue, &controlCode, portMAX_DELAY) != pdPASS)
        {
            configPRINTF(("Fail to send message to app_music_control_queue.\r\n"));
        }
    }
}

void app_bt_connect_addr(char *addrStr)
{
    char data;
    char tmp;
    for (uint8_t index = 0; index < 6U; index++)
    {
        data = 0;
        tmp = addrStr[index * 3];
        if ((tmp >= '0') && (tmp <= '9'))
        {
            data = ((tmp - '0') << 4U);
        }
        else if ((tmp >= 'A') && (tmp <= 'F'))
        {
            data = (((tmp - 'A') + 10) << 4U);
        }
        else if ((tmp >= 'a') && (tmp <= 'f'))
        {
            data = (((tmp - 'a') + 10) << 4U);
        }
        else
        {
        }

        tmp = addrStr[index * 3 + 1];
        if ((tmp >= '0') && (tmp <= '9'))
        {
            data |= ((tmp - '0'));
        }
        else if ((tmp >= 'A') && (tmp <= 'F'))
        {
            data |= (((tmp - 'A') + 10));
        }
        else if ((tmp >= 'a') && (tmp <= 'f'))
        {
            data |= (((tmp - 'a') + 10));
        }
        else
        {
        }
        app_a2dp.peer_bd_addr[index] = data;
    }

    app_bt_set_task_msg(APP_CONTROL_CONNECT);
}

int app_bt_connect_index(uint8_t index)
{
    if (index >= APP_INQUIRY_NUM_RESPONSES)
    {
        return -1;
    }
    memcpy(app_a2dp.peer_bd_addr, &br_discovery_results[index].addr.val[0], 6U);
    app_bt_set_task_msg(APP_CONTROL_CONNECT);
    return 0;
}

void app_bt_disconnect(void)
{
    app_bt_set_task_msg(APP_CONTROL_DISCONNECT);
}

void app_bt_delete_device(void)
{
    app_bt_set_task_msg(APP_CONTROL_DELETE);
}

void app_bt_inquiry(void)
{
    app_bt_set_task_msg(APP_CONTROL_INQUIRY);
}

void app_bt_stop_inquiry(void)
{
    app_bt_set_task_msg(APP_CONTROL_STOP_INQUIRY);
}

void app_bt_init_task(void)
{
    app_control_queue = xQueueCreate(8, sizeof(uint8_t));
    if (app_control_queue == NULL)
    {
        configPRINTF(("Failed to create app_control_queue queue.\r\n"));
        vTaskDelete(NULL);
    }
}

void app_bt_aws_ready(void)
{
    app_bt_set_task_msg(APP_CONTROL_AWS_READY);
}

static uint8_t app_sdp_a2sink_user(struct bt_conn *conn,
			   struct bt_sdp_client_result *result)
{
    uint16_t param;
    int res;

    if ((result) && (result->resp_buf))
    {
        PRINTF("sdp success callback\r\n");
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_L2CAP, &param);
        if (res < 0)
        {
            PRINTF("PSM is not found\r\n");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        if (param == BT_UUID_AVDTP_VAL)
        {
            PRINTF ("A2DP Service found. Connecting ...\n");
            app_a2dp.a2dp_handle = bt_a2dp_connect(app_a2dp.conn);
            if (NULL == app_a2dp.a2dp_handle)
            {
                PRINTF ("fail to connect a2dp\r\n");
            }
            return BT_SDP_DISCOVER_UUID_STOP;
        }
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
    else
    {
        PRINTF("sdp fail callback\r\n");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static void br_device_found(size_t index, const bt_addr_t *addr, int8_t rssi,
                            const uint8_t cod[3], const uint8_t eir[240])
{
    char br_addr[BT_ADDR_STR_LEN];
    char name[239];
    int len = 240;

    (void)memset(name, 0, sizeof(name));
    
    while (len)
    {
        if (len < 2)
        {
            break;
        }

        /* Look for early termination */
        if (!eir[0])
        {
            break;
        }
        
        /* check if field length is correct */
        if (eir[0] > len - 1)
        {
            break;
        }
        
        switch (eir[1])
        {
        case BT_DATA_NAME_SHORTENED:
        case BT_DATA_NAME_COMPLETE:
            memcpy(name, &eir[2], (eir[0] - 1) > (sizeof(name) - 1) ? (sizeof(name) - 1) : (eir[0] - 1));
            break;
        default:
            break;
        }

        /* Parse next AD Structure */
        len -= (eir[0] + 1);
        eir += (eir[0] + 1);
    }

    bt_addr_to_str(addr, br_addr, sizeof(br_addr));
    PRINTF("[%d]: %s, RSSI %i %s\r\n", index + 1, br_addr, rssi, name);
}

static void br_discovery_complete(struct bt_br_discovery_result *results,
				  size_t count)
{
    size_t index;

    PRINTF("BR/EDR discovery complete\r\n");
    for (index = 0; index < count; ++index)
    {
        br_device_found(index, &results[index].addr, results[index].rssi, results[index].cod, results[index].eir);
    }
}

void app_search_devices(void)
{
    int err;
    struct bt_br_discovery_param param;

    param.length = APP_INQUIRY_LENGTH;
    param.limited = 0U;
    err = bt_br_discovery_start(&param, br_discovery_results,
                                APP_INQUIRY_NUM_RESPONSES,
                                br_discovery_complete);
    if (err != 0)
    {
        PRINTF("Failed to start discovery\r\n");
    }
    else
    {
        PRINTF("Discovery started. Please wait ...\r\n");
    }
}

void app_stop_search_devices(void)
{
    if (bt_br_discovery_stop())
    {
        PRINTF("Failed to stop discovery\r\n");
        return;
    }
    PRINTF("Discovery stopped\r\n");
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    int res;

    if (err)
    {
        if (app_a2dp.conn != NULL)
        {
            app_a2dp.conn = NULL;
        }
        PRINTF("Connection failed (err 0x%02x)\n", err);
    }
    else
    {
        struct bt_conn_info info;

        app_a2dp.conn = bt_conn_ref(conn);
        bt_conn_get_info(conn, &info);
        if (info.type == BT_CONN_TYPE_LE)
        {
            return;
        }

        /*
         * Do an SDP Query on Successful ACL connection complete with the
         * required device
         */
        if (1U == app_a2dp.acl_initiated)
        {
            app_a2dp.acl_initiated = 0U;

            if (0 == memcmp(info.br.dst, app_a2dp.peer_bd_addr, 6U))
            {
                res = bt_sdp_discover(app_a2dp.conn, &discov_a2dp_sink);
                if (res)
                {
                    PRINTF("SDP discovery failed: result\r\n");
                }
                else
                {
                    PRINTF("SDP discovery started\r\n");
                }
            }
        }
        else
        {
            memcpy(app_a2dp.peer_bd_addr, info.br.dst, 6U);
        }

        PRINTF("Connected\n");
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{

    PRINTF("Disconnected (reason 0x%02x)\n", reason);

    if (app_a2dp.conn != conn)
    {
        return;
    }

    if (app_a2dp.conn)
    {
        bt_conn_unref(app_a2dp.conn);
        app_a2dp.conn = NULL;
    }
    else
    {
        return;
    }

    if (app_a2dp.peer_key_missed)
    {
        app_a2dp.peer_key_missed = 0U;
        app_bt_set_task_msg(APP_CONTROL_CONNECT);
    }
}

static void security_changed(struct bt_conn *conn, bt_security_t level,
				 enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (!err)
    {
        PRINTF("Security changed: %s level %u\n", addr, level);
    }
    else
    {
        PRINTF("Security failed: %s level %u err %d\n", addr, level, err);
        if (err == BT_SECURITY_ERR_PIN_OR_KEY_MISSING)
        {
            int error;
            bt_addr_le_t addr;

            app_a2dp.peer_key_missed = 1U;
            addr.type = BT_ADDR_LE_PUBLIC;
            memcpy(&addr.a.val[0], app_a2dp.peer_bd_addr, 6U);
            error = bt_unpair(BT_ID_DEFAULT, &addr);
            if (0 == error)
            {
                PRINTF("Delete binding information, try connecting again.\n");
            }
            else
            {
                PRINTF("Failed to delete binding information, please try shell command  \"bt deletedevice\", and try connecting again.\n");
            }
        }
    }
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
    .security_changed = security_changed,
};

static void bt_ready(int err)
{
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    PRINTF("Bluetooth initialized\n");

    bt_sdp_register_service(&a2dp_source_rec);
    bt_conn_cb_register(&conn_callbacks);
    app_edgefast_a2dp_init();
    /* Make self discoverable and connectable over BR/EDR */
    err = bt_br_set_connectable(true);
    if (err)
    {
        PRINTF("BR/EDR set/rest connectable failed (err %d)\n", err);
    }
    err = bt_br_set_discoverable(true);
    if (err)
    {
        PRINTF("BR/EDR set discoverable failed (err %d)\n", err);
    }
    app_bt_set_task_msg(APP_CONTROL_BT_READY);
}

int app_main (int argc, char **argv)
{
    uint8_t bt_control;
    int err = 0;
    (void)err;

    for (;;)
    {
        if (xQueueReceive(app_control_queue, &bt_control, portMAX_DELAY) == pdTRUE)
        {
            if (bt_control == APP_CONTROL_AWS_READY)
            {
                break;
            }
        }
    }

    /* Initializate BT Host stack */
    err = bt_enable(bt_ready);
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        while (1)
        {
            vTaskDelay(2000);
        }
    }

    app_a2dp.acl_initiated = 0U;
 
    for (;;)
    {
        /* process delta shadow JSON received in prvDeltaCallback() */
        if (xQueueReceive(app_control_queue, &bt_control, portMAX_DELAY) == pdTRUE)
        {
            switch (bt_control)
            {
            case APP_CONTROL_BT_READY:
                shell_task_init();
                break;

            case APP_CONTROL_CONNECT:
            {
                bt_addr_t addr;
                memcpy(&addr, app_a2dp.peer_bd_addr, 6U);
                app_a2dp.conn = bt_conn_create_br(&addr, BT_BR_CONN_PARAM_DEFAULT);
                if (!app_a2dp.conn)
                {
                    PRINTF("Connection failed\r\n");
                }
                else
                {
                    app_a2dp.acl_initiated = 1U;
                    /* unref connection obj in advance as app user */
                    bt_conn_unref(app_a2dp.conn);
                    PRINTF("Connection pending\r\n");
                }
                break;
            }

            case APP_CONTROL_DISCONNECT:
                err = bt_conn_disconnect(app_a2dp.conn, 0x13U);
                if (err)
                {
                    PRINTF("Disconnection failed (err %d)", err);
                }
                break;

            case APP_CONTROL_DELETE:
            {
                err = bt_unpair(BT_ID_DEFAULT, NULL);
                if (err != 0)
                {
                    printf("FAILED ! Reason = 0x%04X\n", err);
                }
                else
                {
                    printf("OK\n");
                }
                break;
            }

            case APP_CONTROL_INQUIRY:
                app_search_devices();
                break;

            case APP_CONTROL_STOP_INQUIRY:
                break;

            default:
                break;
            }
        }
    }
}
