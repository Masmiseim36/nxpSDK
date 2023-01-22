/*
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef _KVSTORE_CONFIG_H
#define _KVSTORE_CONFIG_H

#include "demo_config.h"
#include "core_pkcs11_config.h"
#include "aws_clientcredential.h"

typedef enum KVStoreKey
{
    KVS_CORE_THING_NAME = 0,
    KVS_CORE_MQTT_ENDPOINT,
    KVS_CORE_MQTT_PORT,
    KVS_DEVICE_PRIVKEY_ID,
    KVS_DEVICE_PUBKEY_ID,
    KVS_DEVICE_CERT_ID,
    KVS_PRIV_ROOTCA_ID,
    KVS_AWS_ROOTCA_ID,
    KVS_CODEVERIFY_PUBKEY_ID,
    KVS_NUM_KEYS
} KVStoreKey_t;

#define KVSTORE_KEY_MAX_LEN 32
#define KVSTORE_VAL_MAX_LEN 256

#define KVSTORE_FILE_PATH ("/kvstore")

/* Array to map between strings and KVStoreKey_t IDs */
#define KVSTORE_KEYS                                                                      \
    {                                                                                     \
        [KVS_CORE_THING_NAME] = "thing_name", [KVS_CORE_MQTT_ENDPOINT] = "mqtt_endpoint", \
        [KVS_CORE_MQTT_PORT] = "mqtt_port", [KVS_DEVICE_PRIVKEY_ID] = "priv_key_id",      \
        [KVS_DEVICE_PUBKEY_ID] = "pub_key_id", [KVS_DEVICE_CERT_ID] = "cert_id",          \
        [KVS_PRIV_ROOTCA_ID] = "priv_root_ca_id", [KVS_AWS_ROOTCA_ID] = "aws_root_ca_id", \
        [KVS_CODEVERIFY_PUBKEY_ID] = "codeverify_key_id"                                  \
    }

#define KV_STORE_DEFAULTS                                                                                     \
    {                                                                                                         \
        KV_DFLT(KV_TYPE_STRING, clientcredentialIOT_THING_NAME),                   /* THINGNAME */            \
            KV_DFLT(KV_TYPE_STRING, clientcredentialMQTT_BROKER_ENDPOINT),         /* MQTT_ENDPOINT */        \
            KV_DFLT(KV_TYPE_UINT32, clientcredentialMQTT_BROKER_PORT),             /* MQTT_PORT */            \
            KV_DFLT(KV_TYPE_STRING, pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS), /* DEVICE_PRIVKEY_ID */    \
            KV_DFLT(KV_TYPE_STRING, pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS),  /* DEVICE_PUBKEY_ID */     \
            KV_DFLT(KV_TYPE_STRING, pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS), /* DEVICE_CERT_ID */       \
            KV_DFLT(KV_TYPE_STRING, pkcs11configLABEL_JITP_CERTIFICATE),           /* PRIV_ROOTCA_ID */       \
            KV_DFLT(KV_TYPE_STRING, pkcs11configLABEL_ROOT_CERTIFICATE),           /* AWS_ROOTCA_ID */        \
            KV_DFLT(KV_TYPE_STRING, pkcs11configLABEL_CODE_VERIFICATION_KEY),      /* CODEVERIFY_PUBKEY_ID */ \
    }

#endif /* _KVSTORE_CONFIG_H */
