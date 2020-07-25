#include <stdio.h>
#include <string.h>
#include "infra_state.h"
#include "infra_defs.h"
#include "infra_config.h"
#include "infra_sha256.h"

#include "dev_sign_api.h"
#include "wrappers.h"

/* all secure mode define */
#define MODE_TLS_GUIDER             "-1"
#define MODE_TLS_DIRECT             "2"
#define MODE_TCP_DIRECT_PLAIN       "3"
#define MODE_ITLS_DNS_ID2           "8"

#if defined(MQTT_PRE_AUTH)
    #define SECURE_MODE             MODE_TLS_GUIDER
#elif defined(SUPPORT_TLS)
    #define SECURE_MODE             MODE_TLS_DIRECT
#else
    #define SECURE_MODE             MODE_TCP_DIRECT_PLAIN
#endif

/* use fixed timestamp */
#define TIMESTAMP_VALUE             "2524608000000"

/* clientid key value pair define */
const char *clientid_kv[][2] = {
    {
        "timestamp", TIMESTAMP_VALUE
    },
    {
        "_v", "sdk-c-"IOTX_SDK_VERSION
    },
    {
        "securemode", SECURE_MODE
    },
    {
        "signmethod", "hmacsha256"
    },
    {
        "lan", "C"
    },
#ifdef MQTT_AUTO_SUBSCRIBE
    {
        "_ss", "1"
    },
#endif
#if 0
    {
        "v", IOTX_ALINK_VERSION
    },
#else
    {
        "gw", "0"
    },
    {
        "ext", "0"
    },
#endif
};

static void _hex2str(uint8_t *input, uint16_t input_len, char *output)
{
    char *zEncode = "0123456789ABCDEF";
    int i = 0, j = 0;

    for (i = 0; i < input_len; i++) {
        output[j++] = zEncode[(input[i] >> 4) & 0xf];
        output[j++] = zEncode[(input[i]) & 0xf];
    }
}

int _sign_get_clientid(char *clientid_string, const char *device_id, const char *custom_kv, uint8_t enable_itls)
{
    uint8_t i;

    if (clientid_string == NULL || device_id == NULL) {
        return STATE_USER_INPUT_INVALID;
    }

    memset(clientid_string, 0, DEV_SIGN_CLIENT_ID_MAXLEN);
    memcpy(clientid_string, device_id, strlen(device_id));
    memcpy(clientid_string + strlen(clientid_string), "|", 2);

    if (enable_itls > 0) {
        clientid_kv[2][1] = MODE_ITLS_DNS_ID2;
    } else {
        clientid_kv[2][1] = SECURE_MODE;
    }

    for (i = 0; i < (sizeof(clientid_kv) / (sizeof(clientid_kv[0]))); i++) {
        if ((strlen(clientid_string) + strlen(clientid_kv[i][0]) + strlen(clientid_kv[i][1]) + 2) >=
            DEV_SIGN_CLIENT_ID_MAXLEN) {
            return STATE_MQTT_SIGN_CLIENTID_BUF_SHORT;
        }

        memcpy(clientid_string + strlen(clientid_string), clientid_kv[i][0], strlen(clientid_kv[i][0]));
        memcpy(clientid_string + strlen(clientid_string), "=", 1);
        memcpy(clientid_string + strlen(clientid_string), clientid_kv[i][1], strlen(clientid_kv[i][1]));
        memcpy(clientid_string + strlen(clientid_string), ",", 1);
    }

    if (custom_kv != NULL) {
        if ((strlen(clientid_string) + strlen(custom_kv) + 1) >= DEV_SIGN_CLIENT_ID_MAXLEN) {
            return STATE_MQTT_SIGN_CLIENTID_BUF_SHORT;
        }
        memcpy(clientid_string + strlen(clientid_string), custom_kv, strlen(custom_kv));
        memcpy(clientid_string + strlen(clientid_string), ",", 1);
    }

    memcpy(clientid_string + strlen(clientid_string) - 1, "|", 1);

    return STATE_SUCCESS;
}

#define SIGN_FMT_LEN  50  /*  "clientId%sdeviceName%sproductKey%stimestamp%s";*/
int _iotx_generate_sign_string(const char *device_id, const char *device_name, const char *product_key,
                               const char *device_secret, char *sign_string)
{
    char signsource[DEV_SIGN_SOURCE_MAXLEN] = {0};
    uint16_t signsource_len = 0;
    uint8_t sign_hex[32] = {0};

    signsource_len = SIGN_FMT_LEN + strlen(device_id) + strlen(device_name) + strlen(product_key) + strlen(
                                 TIMESTAMP_VALUE);
    if (signsource_len >= DEV_SIGN_SOURCE_MAXLEN) {
        return STATE_MQTT_SIGN_SOURCE_BUF_SHORT;
    }

    memset(signsource, 0, DEV_SIGN_SOURCE_MAXLEN);
    memcpy(signsource, "clientId", strlen("clientId"));
    memcpy(signsource + strlen(signsource), device_id, strlen(device_id));
    memcpy(signsource + strlen(signsource), "deviceName", strlen("deviceName"));
    memcpy(signsource + strlen(signsource), device_name, strlen(device_name));
    memcpy(signsource + strlen(signsource), "productKey", strlen("productKey"));
    memcpy(signsource + strlen(signsource), product_key, strlen(product_key));
    memcpy(signsource + strlen(signsource), "timestamp", strlen("timestamp"));
    memcpy(signsource + strlen(signsource), TIMESTAMP_VALUE, strlen(TIMESTAMP_VALUE));

    utils_hmac_sha256((uint8_t *)signsource, strlen(signsource), (uint8_t *)device_secret,
                      strlen(device_secret), sign_hex);

    _hex2str(sign_hex, 32, sign_string);

    return STATE_SUCCESS;
}

int32_t IOT_Sign_MQTT(iotx_mqtt_region_types_t region, iotx_dev_meta_info_t *meta, iotx_sign_mqtt_t *signout)
{
    uint16_t length = 0;
    char device_id[IOTX_PRODUCT_KEY_LEN + IOTX_DEVICE_NAME_LEN + 1] = {0};
    int res;

    if (region >= IOTX_MQTT_DOMAIN_NUMBER || meta == NULL) {
        return STATE_USER_INPUT_INVALID;
    }

    memset(signout, 0, sizeof(iotx_sign_mqtt_t));

    memcpy(device_id, meta->product_key, strlen(meta->product_key));
    memcpy(device_id + strlen(device_id), ".", strlen("."));
    memcpy(device_id + strlen(device_id), meta->device_name, strlen(meta->device_name));

    /* setup clientid */
    if (_sign_get_clientid(signout->clientid, device_id, NULL, 0) < STATE_SUCCESS) {
        return STATE_MQTT_SIGN_CLIENTID_BUF_SHORT;
    }

    /* setup password */
    memset(signout->password, 0, DEV_SIGN_PASSWORD_MAXLEN);
    res = _iotx_generate_sign_string(device_id, meta->device_name, meta->product_key, meta->device_secret,
                                     signout->password);
    if (res < STATE_SUCCESS) {
        return res;
    }

    /* setup hostname */
    if (IOTX_CLOUD_REGION_CUSTOM == region) {
        if (g_infra_mqtt_domain[region] == NULL) {
            return STATE_USER_INPUT_MQTT_DOMAIN;
        }

        length = strlen(g_infra_mqtt_domain[region]) + 1;
        if (length >= DEV_SIGN_HOSTNAME_MAXLEN) {
            return STATE_MQTT_SIGN_HOSTNAME_BUF_SHORT;
        }

        memset(signout->hostname, 0, DEV_SIGN_HOSTNAME_MAXLEN);
        memcpy(signout->hostname, g_infra_mqtt_domain[region], strlen(g_infra_mqtt_domain[region]));
    } else {
        length = strlen(meta->product_key) + strlen(g_infra_mqtt_domain[region]) + 2;
        if (length >= DEV_SIGN_HOSTNAME_MAXLEN) {
            return STATE_MQTT_SIGN_HOSTNAME_BUF_SHORT;
        }
        memset(signout->hostname, 0, DEV_SIGN_HOSTNAME_MAXLEN);
        memcpy(signout->hostname, meta->product_key, strlen(meta->product_key));
        memcpy(signout->hostname + strlen(signout->hostname), ".", strlen("."));
        memcpy(signout->hostname + strlen(signout->hostname), g_infra_mqtt_domain[region],
               strlen(g_infra_mqtt_domain[region]));
    }

    /* setup username */
    length = strlen(meta->device_name) + strlen(meta->product_key) + 2;
    if (length >= DEV_SIGN_USERNAME_MAXLEN) {
        return STATE_MQTT_SIGN_USERNAME_BUF_SHORT;
    }
    memset(signout->username, 0, DEV_SIGN_USERNAME_MAXLEN);
    memcpy(signout->username, meta->device_name, strlen(meta->device_name));
    memcpy(signout->username + strlen(signout->username), "&", strlen("&"));
    memcpy(signout->username + strlen(signout->username), meta->product_key, strlen(meta->product_key));

    /* setup port */
    signout->port = 443;
    return STATE_SUCCESS;
}

