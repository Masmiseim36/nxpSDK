/*
 *
 * Copyright 2018,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AWS_CREDENTIAL_KEYS_H
#define AWS_CREDENTIAL_KEYS_H

#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif
/*
 * Not Used for se_hostlib
 */

static const char keyCLIENT_CERTIFICATE_PEM[] = {0};

/*
 */
static const char *USED keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = "";

/*
 * Not Used for se_hostlib
 */
static const char keyCLIENT_PRIVATE_KEY_PEM[] = {0};

/*
 * MQTT Broker endpoint.
 */
static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "a29rg0ytflhg6y.iot.eu-central-1.amazonaws.com";

/* Use of a "define" and not a "static const" here to be able to
* use pre-compile concatenation on the string. */
#define clientcredentialIOT_THING_NAME "K64f_1"

/*
 * Port number the MQTT broker is using.
 */
#define clientcredentialMQTT_BROKER_PORT 8883

/*
 * Port number the Green Grass Discovery use for JSON retrieval from cloud is using.
 */
#define clientcredentialGREENGRASS_DISCOVERY_PORT 8443

/*
 * WIFI network to join.
 */
#define clientcredentialWIFI_SSID "Paste WiFi SSID here."

/*
 * Password needed to join WiFi network.
 */
#define clientcredentialWIFI_PASSWORD "Paste WiFi password here."

/**
 * @brief Security type
 * WPA2 Security, @see WIFISecurity_t
 * other values are - eWiFiSecurityOpen, eWiFiSecurityWEP, eWiFiSecurityWPA
 */
#define clientcredentialWIFI_SECURITY eWiFiSecurityWPA2

/*
 * Length of device certificate included from aws_clientcredential_keys.h .
 */
static const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH = sizeof(keyCLIENT_CERTIFICATE_PEM);

/*
 * Length of device private key included from aws_clientcredential_keys.h .
 */

static const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH = sizeof(keyCLIENT_PRIVATE_KEY_PEM);

#endif
