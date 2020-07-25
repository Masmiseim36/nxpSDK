/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */
#include "infra_config.h"

#ifdef INFRA_REPORT

#include <stdio.h>
#include <string.h>
#include "infra_compat.h"
#include "infra_types.h"
#include "infra_defs.h"
#include "infra_string.h"
#include "infra_report.h"
#include "wrappers.h"

#ifdef INFRA_MEM_STATS
    #include "infra_mem_stats.h"
    #define SYS_REPORT_MALLOC(size) LITE_malloc(size, MEM_MAGIC, "sys.report")
    #define SYS_REPORT_FREE(ptr)    LITE_free(ptr)
#else
    #define SYS_REPORT_MALLOC(size) HAL_Malloc(size)
    #define SYS_REPORT_FREE(ptr)    HAL_Free(ptr)
#endif

#ifdef INFRA_LOG
    #include "infra_log.h"
    #define VERSION_DEBUG(...)  log_debug("version", __VA_ARGS__)
    #define VERSION_ERR(...)    log_err("version", __VA_ARGS__)
#else
    #define VERSION_DEBUG(...)  do{HAL_Printf(__VA_ARGS__);HAL_Printf("\r\n");}while(0)
    #define VERSION_ERR(...)    do{HAL_Printf(__VA_ARGS__);HAL_Printf("\r\n");}while(0)
#endif

static unsigned int g_report_id = 0;

int iotx_report_id(void)
{
    return g_report_id++;
}

static info_report_func_pt info_report_func = NULL;

void iotx_set_report_func(info_report_func_pt func)
{
    info_report_func = func;
}

const char *DEVICE_INFO_UPDATE_FMT = "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":["
                                     "{\"attrKey\":\"SYS_LP_SDK_VERSION\",\"attrValue\":\"%s\",\"domain\":\"SYSTEM\"},"
                                     "{\"attrKey\":\"SYS_SDK_LANGUAGE\",\"attrValue\":\"C\",\"domain\":\"SYSTEM\"}"
                                     "],\"method\":\"thing.deviceinfo.update\"}";

int iotx_report_devinfo(void *pclient)
{
    int ret = 0;
    char topic_name[IOTX_URI_MAX_LEN + 1] = {0};
    char product_key[IOTX_PRODUCT_KEY_LEN + 1] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN + 1] = {0};
    char *msg = NULL;
    int  msg_len = 0;


    if (info_report_func == NULL) {
        VERSION_ERR("report func not register!");
        return -1;
    }

    IOT_Ioctl(IOTX_IOCTL_GET_PRODUCT_KEY, product_key);
    IOT_Ioctl(IOTX_IOCTL_GET_DEVICE_NAME, device_name);
    VERSION_DEBUG("devinfo report");

    /* devinfo update topic name */
    ret = HAL_Snprintf(topic_name,
                       IOTX_URI_MAX_LEN,
                       "/sys/%s/%s/thing/deviceinfo/update",
                       product_key,
                       device_name);
    if (ret <= 0) {
        VERSION_ERR("topic generate err");
        return FAIL_RETURN;
    }
    VERSION_DEBUG("devinfo report topic: %s", topic_name);

    msg_len = strlen(DEVICE_INFO_UPDATE_FMT) + 10 + strlen(IOTX_SDK_VERSION) + 1;
    msg = (char *)SYS_REPORT_MALLOC(msg_len);
    if (msg == NULL) {
        VERSION_ERR("malloc err");
        return FAIL_RETURN;
    }
    memset(msg, 0, msg_len);

    /* devinfo update message */
    ret = HAL_Snprintf(msg,
                       msg_len,
                       DEVICE_INFO_UPDATE_FMT,
                       iotx_report_id(),
                       IOTX_SDK_VERSION
                      );
    if (ret <= 0) {
        VERSION_ERR("topic msg generate err");
        SYS_REPORT_FREE(msg);
        return FAIL_RETURN;
    }
    VERSION_DEBUG("devinfo report data: %s", msg);

    if (info_report_func != NULL) {
        ret = info_report_func(pclient, topic_name, 1, msg, strlen(msg));
        if (ret < 0) {
            VERSION_ERR("publish failed, ret = %d", ret);
            SYS_REPORT_FREE(msg);
            return FAIL_RETURN;
        }
    }

    SYS_REPORT_FREE(msg);
    VERSION_DEBUG("devinfo report succeed");

    return SUCCESS_RETURN;
}

/* report Firmware version */
int iotx_report_firmware_version(void *pclient)
{
    int ret;
    char topic_name[IOTX_URI_MAX_LEN + 1] = {0};
    char msg[FIRMWARE_VERSION_MSG_LEN] = {0};
    char version[IOTX_FIRMWARE_VERSION_LEN + 1] = {0};
    char product_key[IOTX_PRODUCT_KEY_LEN + 1] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN + 1] = {0};

    if (info_report_func == NULL) {
        VERSION_ERR("report func not register!");
        return -1;
    }

    IOT_Ioctl(IOTX_IOCTL_GET_PRODUCT_KEY, product_key);
    IOT_Ioctl(IOTX_IOCTL_GET_DEVICE_NAME, device_name);

    ret = HAL_GetFirmwareVersion(version);
    if (ret <= 0) {
        VERSION_ERR("firmware version does not implement");
        return FAIL_RETURN;
    }

    VERSION_DEBUG("firmware version report start in MQTT");

    /* firmware report topic name generate */
    ret = HAL_Snprintf(topic_name,
                       IOTX_URI_MAX_LEN,
                       "/ota/device/inform/%s/%s",
                       product_key,
                       device_name
                      );
    if (ret <= 0) {
        VERSION_ERR("firmware report topic generate err");
        return FAIL_RETURN;
    }
    VERSION_DEBUG("firmware report topic: %s", topic_name);

    /* firmware report message json data generate */
    ret = HAL_Snprintf(msg,
                       FIRMWARE_VERSION_MSG_LEN,
                       "{\"id\":\"%d\",\"params\":{\"version\":\"%s\"}}",
                       iotx_report_id(),
                       version
                      );
    if (ret <= 0) {
        VERSION_ERR("firmware report message json data generate err");
        return FAIL_RETURN;
    }
    VERSION_DEBUG("firmware report data: %s", msg);

    ret = info_report_func(pclient, topic_name, 1, msg, strlen(msg));

    if (ret < 0) {
        VERSION_ERR("publish failed, ret = %d", ret);
        return ret;
    }

    VERSION_DEBUG("firmware version report finished, iotx_publish() = %d", ret);
    return SUCCESS_RETURN;
}

/* report ModuleID */
int iotx_report_mid(void *pclient)
{
    return SUCCESS_RETURN;
}

#endif

