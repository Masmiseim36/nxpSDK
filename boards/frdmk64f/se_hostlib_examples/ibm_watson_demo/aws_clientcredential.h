/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SE_HOSTLIB_AWS_CREDENTIALS
#define SE_HOSTLIB_AWS_CREDENTIALS

#include "wifi_config.h"
#include "aws_clientcredential_keys.h"


/*
 * MQTT Broker endpoint.
 */
static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "a29rg0ytflhg6y.iot.eu-central-1.amazonaws.com";


/* Use of a "define" and not a "static const" here to be able to
*  */
#define clientcredentialIOT_THING_NAME "K64F_1"

/*
 * Port number the MQTT broker is using.
 */
#define clientcredentialMQTT_BROKER_PORT 8883

/*
 * Port number the Green Grass Discovery use for JSON retrieval from cloud is using.
 */
#define clientcredentialGREENGRASS_DISCOVERY_PORT 8443


/*
 * Not used
 */
static const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH = sizeof( clientcredentialCLIENT_CERTIFICATE_PEM );

/*
 * Length of device private key included from aws_clientcredential_keys.h .
 */

static const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH = sizeof( clientcredentialCLIENT_PRIVATE_KEY_PEM );


#ifdef AWS_IOT
#   include "aws_default_root_certificates.h"
#	include "aws_iot_config.h"
#endif

#ifdef GCP_IOT
#   include "gcp_default_root_certificates.h"
#	include "gcp_iot_config.h"
#endif

#ifdef WATSONP
#   include "ibm_watson_default_root_certificate.h"
#	include "watson_iot_config.h"
#endif

#endif
