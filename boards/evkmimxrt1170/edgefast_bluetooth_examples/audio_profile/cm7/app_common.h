/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

enum _bt_app_control_code
{
    APP_CONTROL_CONNECT = 0,
    APP_CONTROL_INQUIRY,
    APP_CONTROL_STOP_INQUIRY,
    APP_CONTROL_DISCONNECT,
    APP_CONTROL_DELETE,
    APP_CONTROL_AWS_READY,
    APP_CONTROL_BT_READY,
};

void app_bt_connect_addr(char *addrStr);

void app_bt_inquiry(void);

int app_bt_connect_index(uint8_t index);

void app_bt_disconnect(void);

void app_bt_delete_device(void);

void app_bt_init_task(void);

void app_bt_aws_ready(void);

void app_bt_stop_inquiry(void);

#endif /* __APP_COMMON_H__ */

