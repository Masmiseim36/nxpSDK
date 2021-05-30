/* Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.

 * */

/** @file
 *
 * gcp_iot_config.h:  <The purpose and scope of this file>
 *
 * $Date: 14-Jun-2018 $
 * $Author: ing05193 $
 * $Revision$
 */

#ifndef GCP_IOT_CONFIG_H_
#define GCP_IOT_CONFIG_H_

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* doc:start:gcp-config */
#define GCP_PROJECT_NAME "pgh-cloud-iot"
#define GCP_LOCATION_NAME "us-central1"
#define GCP_REGISTRY_NAME "nxp-se-demo-reg"

#if (SSS_HAVE_SE050_C || SSS_HAVE_SE050_A)
#define GCP_DEVICE_NAME "nxp-ecc-dev-01"
#elif SSS_HAVE_SE050_B
#define GCP_DEVICE_NAME "nxp-rsa-dev-01"
#else
#define GCP_DEVICE_NAME "a71ch-dev-04"
#endif
/* doc:end:gcp-config */

#define GCP_IOT_MQTT_HOST "mqtt.googleapis.com" ///< Customer specific MQTT HOST. The same will be used for Thing Shadow
#define GCP_IOT_MQTT_PORT 8883                  ///< default port for MQTT/S
#define GCP_IOT_MQTT_CLIENT_ID                                                                               \
    (uint8_t *)"projects/" GCP_PROJECT_NAME "/locations/" GCP_LOCATION_NAME "/registries/" GCP_REGISTRY_NAME \
               "/devices/" GCP_DEVICE_NAME ///< MQTT client ID should be unique for every device
#define GCP_IOT_MQTT_PUB_TOPIC "/devices/" GCP_DEVICE_NAME "/events"
#define GCP_IOT_MQTT_SUB_TOPIC "/devices/" GCP_DEVICE_NAME "/config"

/* doc:start:gcp-keyids */
#define SSS_KEYPAIR_INDEX_CLIENT_PRIVATE 0x20181001
#define SSS_CERTIFICATE_INDEX 0x20181002
/* doc:end:gcp-keyids */

// =================================================

#endif /* SRC_SHADOW_IOT_SHADOW_CONFIG_H_ */
