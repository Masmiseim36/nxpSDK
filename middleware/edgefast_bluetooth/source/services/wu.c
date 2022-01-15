/*
 * Copyright 2019, 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @file
 * @brief Wireless Uart Service Profile
 *
 */

#include "fsl_debug_console.h"

#include <stddef.h>
#include <string.h>
#include <errno/errno.h>
#include <porting.h>

#include "BT_att_api.h"
#include "BT_gatt_db_api.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <bluetooth/services/wu.h>


typedef struct _bt_gatt_wu_peer_state
{
    struct bt_conn *conn;
    uint8_t* wait4SendingDataBuffer;
    uint32_t wait4SendingDataLength;
    struct bt_uuid_128 serivceDiscoveryUuid;
    struct bt_gatt_exchange_params exchangeParams;
    struct bt_gatt_discover_params discoverParams;
    //struct bt_gatt_subscribe_params subscribeParams;
    struct bt_gatt_write_params writeParams;
    volatile uint8_t discoverWriteHandle;
    volatile uint8_t discoverPermission;
} bt_gatt_wu_peer_state_t;

typedef struct _bt_gatt_wu_state
{
    uint32_t mutexHandleBuffer[((OSA_MUTEX_HANDLE_SIZE - 1) >> 2) + 1];
    osa_mutex_handle_t mutex;
    bt_gatt_wu_config_t config;
    char* name;
    char* serialNo;
    bt_gatt_wu_peer_state_t peer[CONFIG_BT_MAX_CONN];
    volatile uint8_t readStreamCccCfg;
    volatile uint8_t connectCount;
} bt_gatt_wu_state_t;

static ssize_t bt_gatt_wu_read_stream(struct bt_conn *conn, const struct bt_gatt_attr *attr,
         void *buf, uint16_t len, uint16_t offset);

static void bt_gatt_wu_read_stream_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                 uint16_t value);

static ssize_t bt_gatt_wu_write_stream(struct bt_conn *conn, const struct bt_gatt_attr *attr,
        const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

static ssize_t bt_gatt_wu_info(struct bt_conn *conn, const struct bt_gatt_attr *attr,
         void *buf, uint16_t len, uint16_t offset);

static ssize_t bt_gatt_wu_serial_no(struct bt_conn *conn, const struct bt_gatt_attr *attr,
         void *buf, uint16_t len, uint16_t offset);

static uint8_t bt_gatt_wu_discovery_service(struct bt_conn *conn,
                 const struct bt_gatt_attr *attr,
                 struct bt_gatt_discover_params *params);

static bt_gatt_wu_state_t s_WuState;

/* Wireless Uart Service Declaration */
BT_GATT_SERVICE_DEFINE(wirelessUart,
    BT_GATT_PRIMARY_SERVICE(WIRELESS_UART_SERIVCE),

    BT_GATT_CHARACTERISTIC(WIRELESS_UART_WRITE_STREAM, BT_GATT_CHRC_WRITE_WITHOUT_RESP, BT_GATT_PERM_WRITE, NULL, bt_gatt_wu_write_stream, NULL),

    BT_GATT_CHARACTERISTIC(WIRELESS_UART_RAED_STREAM, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, bt_gatt_wu_read_stream, NULL, NULL),
    BT_GATT_CCC(bt_gatt_wu_read_stream_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    BT_GATT_CHARACTERISTIC(WIRELESS_UART_INFO, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, bt_gatt_wu_info, NULL, NULL),

    BT_GATT_CHARACTERISTIC(WIRELESS_UART_SERIAL_NO, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, bt_gatt_wu_serial_no, NULL, NULL),
);

void bt_gatt_wu_init(char* name, char* serial_no, bt_gatt_wu_config_t* config)
{
    osa_status_t ret;


    assert((name) && (serial_no) && (config) && (config->data_received) && (config->read));

    s_WuState.mutex = (osa_mutex_handle_t)&s_WuState.mutexHandleBuffer[0];
    ret = OSA_MutexCreate(s_WuState.mutex);
    assert(KOSA_StatusSuccess == ret);

    s_WuState.config = *config;
    s_WuState.name = name;
    s_WuState.serialNo = serial_no;
    s_WuState.readStreamCccCfg = 0;
    (void)ret;
}

static void bt_gatt_wu_write_func(struct bt_conn *conn, uint8_t err,
               struct bt_gatt_write_params *params)
{
}

static void bt_gatt_wu_notify_internal(uint32_t i)
{
    int ret;

    if (s_WuState.readStreamCccCfg)
    {
        OSA_MutexLock(s_WuState.mutex, osaWaitForever_c);
        if ((0 == s_WuState.peer[i].wait4SendingDataLength))
        {
            s_WuState.config.read(s_WuState.peer[i].conn, &s_WuState.peer[i].wait4SendingDataBuffer, (ssize_t*)&s_WuState.peer[i].wait4SendingDataLength);
        }
        if ((s_WuState.peer[i].wait4SendingDataLength))
        {
            ret = bt_gatt_notify(NULL, &wirelessUart.attrs[1], &s_WuState.peer[i].wait4SendingDataBuffer[0], s_WuState.peer[i].wait4SendingDataLength);
            if (ret)
            {
#if PRINTF_ADVANCED_ENABLE
                PRINTF("bt_gatt_notify error %d\r\n", ret);
#else
                char* singnedString = "-";
                if (ret >= 0)
                {
                    singnedString = NULL;
                }
                PRINTF("bt_gatt_notify error %s%d\r\n", singnedString, ret);
#endif
            }
            s_WuState.peer[i].wait4SendingDataLength = 0;
        }
        OSA_MutexUnlock(s_WuState.mutex);
    }
    else if (s_WuState.peer[i].discoverWriteHandle)
    {
    	OSA_MutexLock(s_WuState.mutex, osaWaitForever_c);
		if ((0 == s_WuState.peer[i].wait4SendingDataLength))
		{
			s_WuState.config.read(s_WuState.peer[i].conn, &s_WuState.peer[i].wait4SendingDataBuffer, (ssize_t*)&s_WuState.peer[i].wait4SendingDataLength);
		}
		if ((s_WuState.peer[i].wait4SendingDataLength))
		{
			ret = 0;
	    	if (s_WuState.peer[i].discoverPermission & BT_GATT_CHRC_WRITE_WITHOUT_RESP)
	    	{
	    		ret = bt_gatt_write_without_response(s_WuState.peer[i].conn, s_WuState.peer[i].discoverWriteHandle, &s_WuState.peer[i].wait4SendingDataBuffer[0], s_WuState.peer[i].wait4SendingDataLength, 0);
	    	}
	    	else if (s_WuState.peer[i].discoverPermission & BT_GATT_CHRC_WRITE)
	    	{
                s_WuState.peer[i].writeParams.data = (const void *)s_WuState.peer[i].wait4SendingDataBuffer;
                s_WuState.peer[i].writeParams.length = s_WuState.peer[i].wait4SendingDataLength;
                s_WuState.peer[i].writeParams.handle = s_WuState.peer[i].discoverWriteHandle;
                s_WuState.peer[i].writeParams.offset = 0;
                s_WuState.peer[i].writeParams.func = bt_gatt_wu_write_func;
                ret = bt_gatt_write(s_WuState.peer[i].conn, &s_WuState.peer[i].writeParams);
	    	}
	    	else
	    	{
	    	}
			if (ret)
			{
#if PRINTF_ADVANCED_ENABLE
				PRINTF("bt_gatt_notify error %d\r\n", ret);
#else
				char* singnedString = "-";
				if (ret >= 0)
				{
					singnedString = NULL;
				}
				PRINTF("bt_gatt_notify error %s%d\r\n", singnedString, ret);
#endif
			}
			s_WuState.peer[i].wait4SendingDataLength = 0;
		}
		OSA_MutexUnlock(s_WuState.mutex);
    }
    else
    {
    }
}

void bt_gatt_wu_notify(struct bt_conn *conn)
{
    uint32_t i;

    for (i = 0;i < CONFIG_BT_MAX_CONN;i++)
    {
        if ((NULL == conn) || (conn == s_WuState.peer[i].conn))
        {
            bt_gatt_wu_notify_internal(i);
        }
    }
}

static void bt_gatt_wu_start_service_discovery(struct bt_conn *conn)
{
    int err;
    uint8_t i;
    for (i = 0;i < CONFIG_BT_MAX_CONN;i++)
    {
        if (conn == s_WuState.peer[i].conn)
        {
            break;
        }
    }
    if (i >= CONFIG_BT_MAX_CONN)
    {
        return;
    }
    memcpy(&s_WuState.peer[i].serivceDiscoveryUuid, WIRELESS_UART_SERIVCE, sizeof(s_WuState.peer[i].serivceDiscoveryUuid));
    s_WuState.peer[i].discoverParams.uuid = &s_WuState.peer[i].serivceDiscoveryUuid.uuid;
    s_WuState.peer[i].discoverParams.func = bt_gatt_wu_discovery_service;
    s_WuState.peer[i].discoverParams.start_handle = 0x0001;
    s_WuState.peer[i].discoverParams.end_handle = 0xffff;
    s_WuState.peer[i].discoverParams.type = BT_GATT_DISCOVER_PRIMARY;

    err = bt_gatt_discover(conn, &s_WuState.peer[i].discoverParams);
    if (err) {
        PRINTF("Discover failed(err %d)\n", err);
        return;
    }
}

static void bt_gatt_wu_get_mtu_callback(struct bt_conn *conn, uint8_t err,
                             struct bt_gatt_exchange_params *params)
{
    if (err)
    {
        PRINTF("GATT MTU exchange response failed (err %u)\n", err);
    }
    else
    {
        PRINTF("GATT MTU exchanged: %d\n", bt_gatt_get_mtu(conn));
    }

    bt_gatt_wu_start_service_discovery(conn);
}

void bt_gatt_wu_connected(struct bt_conn *conn)
{
    int err;
    uint32_t i;
    int index = -1;

    for (i = 0;i < CONFIG_BT_MAX_CONN;i++)
    {
        if (conn == s_WuState.peer[i].conn)
        {
            PRINTF("the connection 0x08X is attached\r\n", conn);
            return;
        }
        else if (NULL == s_WuState.peer[i].conn)
        {
            if (-1 == index)
            {
                index = (int)i;
            }
        }
        else
        {
        }
    }

    if (-1 == index)
    {
        PRINTF("All connection slots are used\r\n");
        return;
    }
    i = (uint32_t)index;

    s_WuState.peer[i].conn = conn;
    s_WuState.connectCount++;

    s_WuState.peer[i].exchangeParams.func = bt_gatt_wu_get_mtu_callback;

    err = bt_gatt_exchange_mtu(conn, &s_WuState.peer[i].exchangeParams);
    if (err)
    {
        bt_gatt_wu_start_service_discovery(conn);
        PRINTF("Exchange GATT mtu failed (err %d)\n", err);
    }
    else
    {
    }
}

void bt_gatt_wu_disconnected(struct bt_conn *conn)
{
    uint32_t i;

    for (i = 0;i < CONFIG_BT_MAX_CONN;i++)
    {
        if (conn == s_WuState.peer[i].conn)
        {
            break;
        }
        else
        {
        }
    }
    if (i >= CONFIG_BT_MAX_CONN)
    {
        PRINTF("The connection 0x%08X is not found.\r\n", conn);
        return;
    }
    OSA_MutexLock(s_WuState.mutex, osaWaitForever_c);
    s_WuState.peer[i].wait4SendingDataLength = 0;
    OSA_MutexUnlock(s_WuState.mutex);

    s_WuState.peer[i].wait4SendingDataBuffer = NULL;
    s_WuState.peer[i].discoverWriteHandle = 0;
    s_WuState.peer[i].discoverPermission = 0;
    s_WuState.peer[i].conn = NULL;
    s_WuState.connectCount--;

    if (0 == s_WuState.connectCount)
    {
        s_WuState.readStreamCccCfg = 0;
    }
}

static ssize_t bt_gatt_wu_read_stream(struct bt_conn *conn, const struct bt_gatt_attr *attr,
         void *buf, uint16_t len, uint16_t offset)
{
    ssize_t length = 0;
    uint32_t i;

    for (i = 0;i < CONFIG_BT_MAX_CONN;i++)
    {
        if (conn == s_WuState.peer[i].conn)
        {
            break;
        }
        else
        {
        }
    }
    if (i >= CONFIG_BT_MAX_CONN)
    {
        return 0;
    }
    OSA_MutexLock(s_WuState.mutex, osaWaitForever_c);
    if ((0 == s_WuState.peer[i].wait4SendingDataLength))
    {
        s_WuState.config.read(s_WuState.peer[i].conn, &s_WuState.peer[i].wait4SendingDataBuffer, (ssize_t*)&s_WuState.peer[i].wait4SendingDataLength);
    }
    if (s_WuState.peer[i].wait4SendingDataLength)
    {
        length = bt_gatt_attr_read(conn, attr, buf, len, offset, &s_WuState.peer[i].wait4SendingDataBuffer[0],
                s_WuState.peer[i].wait4SendingDataLength);
        s_WuState.peer[i].wait4SendingDataLength = 0;
    }
    OSA_MutexUnlock(s_WuState.mutex);

    return length;
}

static void bt_gatt_wu_read_stream_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                 uint16_t value)
{
    s_WuState.readStreamCccCfg = (value & BT_GATT_CCC_NOTIFY);
}

static ssize_t bt_gatt_wu_write_stream(struct bt_conn *conn, const struct bt_gatt_attr *attr,
        const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
#if 0
    PRINTF("Data received (%d) 0x: ", len);
    for (int i = 0;i < len; i++)
    {
        PRINTF("%02X ", ((uint8_t*)buf)[i]);
    }
    PRINTF("\r\n");
#endif
    s_WuState.config.data_received(conn, (uint8_t*)buf, len);

    return 0;
}

static ssize_t bt_gatt_wu_info(struct bt_conn *conn, const struct bt_gatt_attr *attr,
         void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, s_WuState.name,
                 strlen(s_WuState.name));
}

static ssize_t bt_gatt_wu_serial_no(struct bt_conn *conn, const struct bt_gatt_attr *attr,
         void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, s_WuState.serialNo,
                 strlen(s_WuState.serialNo));
}


static uint8_t bt_gatt_wu_discovery_service(struct bt_conn *conn,
                 const struct bt_gatt_attr *attr,
                 struct bt_gatt_discover_params *params)
{
    int err;
    uint32_t i;

    for (i = 0;i < CONFIG_BT_MAX_CONN;i++)
    {
        if (conn == s_WuState.peer[i].conn)
        {
            break;
        }
        else
        {
        }
    }
    if (i >= CONFIG_BT_MAX_CONN)
    {
        memset(params, 0, sizeof(*params));
        return BT_GATT_ITER_STOP;
    }

    if (!attr) {
        PRINTF("Discover complete\n");
        memset(params, 0, sizeof(*params));
        return BT_GATT_ITER_STOP;
    }

    PRINTF("[ATTRIBUTE] handle %u\n", attr->handle);

    if (!bt_uuid_cmp(s_WuState.peer[i].discoverParams.uuid, WIRELESS_UART_SERIVCE))
    {
        memcpy(&s_WuState.peer[i].serivceDiscoveryUuid, WIRELESS_UART_WRITE_STREAM, sizeof(s_WuState.peer[i].serivceDiscoveryUuid));
        s_WuState.peer[i].discoverParams.uuid = &s_WuState.peer[i].serivceDiscoveryUuid.uuid;
        s_WuState.peer[i].discoverParams.start_handle = attr->handle + 1;
        s_WuState.peer[i].discoverParams.type = BT_GATT_DISCOVER_CHARACTERISTIC;

        err = bt_gatt_discover(conn, &s_WuState.peer[i].discoverParams);
        if (err)
        {
            PRINTF("Discover stream write service failed (err %d)\n", err);
        }
    }
    else if (!bt_uuid_cmp(s_WuState.peer[i].discoverParams.uuid,
            WIRELESS_UART_WRITE_STREAM))
    {
        struct bt_gatt_chrc* gattChrc = (struct bt_gatt_chrc*)attr->user_data;

        s_WuState.peer[i].discoverPermission = gattChrc->properties;
        s_WuState.peer[i].discoverWriteHandle = attr->handle + 1;
    }
    else
    {
    }

    return BT_GATT_ITER_STOP;
}

