/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Copyright 2017-2018 NXP Semiconductors. Not a Contribution
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_jitr_task.c
 *
 * @brief simple MQTT publish and subscribe for NXP Secure Element Demo
 *
 * This example takes the parameters from the aws_iot_config.h file and establishes a connection to the AWS IoT MQTT
 * Platform.
 *
 * It subscribes to topic - "$aws/things/" clientcredentialIOT_THING_NAME "/shadow/update/accepted"
 *
 * If all the certs are correct, you should be get messages from cloud to this thing.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#include "iot_init.h"
#include "aws_iot_config.h"
#include "aws_clientcredential.h"
#include "iot_default_root_certificates.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sm_types.h"
#include "HLSEAPI.h"

#if AX_EMBEDDED
#include "board.h"
#include "ksdk_mbedtls.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "sm_demo_utils.h"
#endif

#include "jsmn.h"
#include "mbedtls/base64.h"
#include "ax_api.h"
#include "sm_printf.h"

#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#include <fsl_sscp_a71ch.h>
#endif
#if SSS_HAVE_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss.h"
#endif
#include "iot_mqtt_agent_config.h"
#include "iot_mqtt_agent_config_defaults.h"
#include "iot_mqtt_agent.h"
#include "iot_secure_sockets.h"

#if SSS_HAVE_SSS
#include <fsl_sss_sscp.h>
#include <fsl_sss_api.h>
#endif

#include "ledHandler.h"
#include "nxLog_App.h"

sss_sscp_session_t *sscp_session;
sscp_context_t sscp;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SUB_TOPIC "$aws/things/" clientcredentialIOT_THING_NAME "/shadow/update/accepted"
#define PUB_TOPIC "sdkTest/sub"

/* Used to avoid warnings in case of unused parameters in function pointers */
#define IOT_UNUSED(x) (void)(x)
/*******************************************************************************
 * types
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int HandleReceivedMsg(char *sJsonString, uint16_t len);
void awsPubSub(const U8 ax_uid[A71CH_MODULE_UNIQUE_ID_LEN]);

static MQTTBool_t prvMQTTCallback(void *pvUserData, const MQTTPublishData_t *const pxCallbackParams);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/**
 * @brief Default MQTT port is pulled from the aws_iot_config.h
 */
uint32_t port = clientcredentialMQTT_BROKER_PORT;

/**
 * @brief This parameter will avoid infinite loop of publish and exit the program after certain number of publishes
 */
uint32_t publishCount = 4;

/*******************************************************************************
 * AFR
 ******************************************************************************/

static MQTTAgentHandle_t xMQTTHandle = NULL;
#define echoCLIENT_ID ((const uint8_t *)AWS_IOT_MQTT_CLIENT_ID)
/*******************************************************************************
 * Code
 ******************************************************************************/

#if !AX_EMBEDDED
int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING) {
        if ((int)strlen(s) == tok->end - tok->start) {
            if (strncmp(json + tok->start, s, (size_t)(tok->end - tok->start)) == 0) {
                return 0;
            }
        }
    }
    return -1;
}
#endif

int HandleReceivedMsg(char *sJsonString, uint16_t len)
{
    int i;
    int r;
    jsmn_parser p;
    ledColor_t led;
    ledState_t state;
    jsmntok_t t[80]; /* Limit tokens to be parsed.*/

    jsmn_init(&p);
    r = jsmn_parse(&p, sJsonString, len, t, sizeof(t) / sizeof(t[0]));
    if (r < 0) {
        LOG_E("Failed to parse JSON: %d\n", r);
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        LOG_I("Object expected\n");
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(sJsonString, &t[i], "RED") == 0) {
            led = RED;
        }
        else if (jsoneq(sJsonString, &t[i], "GREEN") == 0) {
            led = GREEN;
        }
        else if (jsoneq(sJsonString, &t[i], "BLUE") == 0) {
            led = BLUE;
        }
        else {
            /*           LOG_I("Unexpected key: %.*s\n", t[i].end-t[i].start,
                    sJsonString + t[i].start); */
            continue;
        }
        LOG_I("- LED Color: %.*s\n", t[i].end - t[i].start, sJsonString + t[i].start);
        i += 2;
        if (jsoneq(sJsonString, &t[i], "ON") == 0) {
            state = LED_ON;
        }
        else if (jsoneq(sJsonString, &t[i], "OFF") == 0) {
            state = LED_OFF;
        }
        else if (jsoneq(sJsonString, &t[i], "toggle") == 0) {
            state = LED_TOGGLE;
        }
        else {
            /* LOG_I( "Unexpected key: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start); */
            continue;
        }
        LOG_I("- state: %.*s\n", t[i].end - t[i].start, sJsonString + t[i].start);

        led_handler(led, state);
        break;
    }
    return 0;
}

void awsPubSub_task(void *pvParameters)
{
    U8 AxUID[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
    U16 AxUIDLen = A71CH_MODULE_UNIQUE_ID_LEN;
    HLSE_OBJECT_INDEX index = 2;
    int ret;
#endif

    /*keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM Not used variable to avoid warning set it to NULL*/
    // keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

#if SSS_HAVE_ALT_A71CH
    HLSE_CONNECTION_PARAMS params;
    HLSE_RET_CODE hlseret;
    HLSE_COMMUNICATION_STATE commState;

    hlseret = HLSE_Connect(&params, &commState);
    if (hlseret != HLSE_SW_OK) {
        LOG_E("Failed to establish connection to Secure Module");
        while (1)
            ;
    }
#endif

#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
    A71_GetUniqueID(AxUID, &AxUIDLen);
#endif

    LOG_I("AWS subscribe publish example\n\n");

#if AX_EMBEDDED
    BOARD_InitNetwork_MAC(AxUID);

#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
    ret = GetHandle_GPstorage(index);
    if (ret == 1) {
        LOG_E("GP storage access failed\n");
        while (1)
            ;
    }
#endif
#endif

    awsPubSub(AxUID);
}

static MQTTBool_t prvMQTTCallback(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters)
{
    IOT_UNUSED(pvUserData);
    LOG_I("Subscribe callback");
    LOG_I("%.*s\r\n\t%.*s",
        pxPublishParameters->usTopicLength,
        pxPublishParameters->pucTopic,
        (int)pxPublishParameters->ulDataLength,
        (char *)pxPublishParameters->pvData);

    if (0 != HandleReceivedMsg((char *)pxPublishParameters->pvData, (uint16_t)(pxPublishParameters->ulDataLength))) {
        LOG_I("Subscribe Msg not an agreed JSON Msg ");
    }

    return eMQTTFalse;
}

void awsPubSub(const U8 ax_uid[A71CH_MODULE_UNIQUE_ID_LEN])
{
    BaseType_t xResult;
    char cPayload[100];
    int i = 0;
    int retryCount = 0;

    MQTTAgentConnectParams_t xConnectParameters = {
		.pcURL = clientcredentialMQTT_BROKER_ENDPOINT,            /* The URL of the MQTT broker to connect to. */
		.xFlags = mqttagentREQUIRE_TLS,                           /* Connection flags. */
		.xURLIsIPAddress = pdFALSE,                               /* Deprecated. */
		.usPort = clientcredentialMQTT_BROKER_PORT,               /* Port number on which the MQTT broker is listening. */
		.pucClientId = echoCLIENT_ID,                             /* Client Identifier of the MQTT client. It should be unique per broker. */
		.usClientIdLength = 0,                                    /* The length of the client Id, filled in later as not const. */
		.xSecuredConnection = pdFALSE,                            /* Deprecated. */
		.pvUserData = NULL,                                       /* User data supplied to the callback. Can be NULL. */
		.pxCallback = NULL,                                       /* Callback used to report various events. Can be NULL. */
		.pcCertificate = (char *)tlsCombi_ROOT_CERTIFICATE_PEM,/* Certificate used for secure connection. Can be NULL. */
		.ulCertificateSize = tlsCombi_ROOT_CERTIFICATE_LENGTH, /* Size of certificate used for secure connection. */
		.cUserName = NULL,                                        /* User name for MQTT connect. Filled later*/
		.uUsernamelength = 0,                                     /* Length of user name. Filled later*/
		.p_password = NULL,                                       /* Password for MQTT connect params*/
		.passwordlength = 0,                                      /* Length of the password */
    };

    MQTTAgentReturnCode_t xReturned;
    MQTTAgentSubscribeParams_t xSubscribeParams;
    MQTTAgentPublishParams_t xPublishParameters_QOS0;
    MQTTAgentPublishParams_t xPublishParameters_QOS1;
    xResult = IotSdk_Init();
    if (xResult != pdPASS)
    {
         //PRINTF("ERROR:  IotSdk_Init() failed \r\n");
         while(1)
                ;
    }
    xResult = MQTT_AGENT_Init();
    if (xResult == pdPASS) {
        xResult = SOCKETS_Init();
    }

    if (xResult != pdPASS) {
        /*Init failure */
        LOG_E("\nFailure at MQTT Init\n");
        LED_GREEN_OFF();
        LED_RED_ON();
        goto exit;
    }

    xReturned = MQTT_AGENT_Create(&xMQTTHandle);
    if (xReturned == eMQTTAgentSuccess) {
        xConnectParameters.usClientIdLength = (uint16_t)strlen((const char *)echoCLIENT_ID);
        LOG_I("MQTT attempting to connect...");
        do {
            LOG_I("MQTT attempting to connect to '%s'... \n\n", xConnectParameters.pucClientId);
            xReturned = MQTT_AGENT_Connect(xMQTTHandle, &xConnectParameters, (mqttconfigKEEP_ALIVE_ACTUAL_INTERVAL_TICKS * (1 + 2*retryCount)));
            if (xReturned == eMQTTAgentSuccess) {
                break;
                }
            else
            {
                LOG_E(("Connect failed: Retrying \r\n"));
                retryCount ++;
            }
        } while (retryCount <= 5);

        if (xReturned != eMQTTAgentSuccess) {
            LOG_E(("Connect failed: Exiting\r\n"));
                (void)MQTT_AGENT_Delete(xMQTTHandle);
                LED_RED_ON();
                goto exit;
            }

        xSubscribeParams.pucTopic = (const uint8_t *)SUB_TOPIC;
        xSubscribeParams.pvPublishCallbackContext = NULL;
        xSubscribeParams.pxPublishCallback = prvMQTTCallback;
        xSubscribeParams.usTopicLength = sizeof(SUB_TOPIC) - 1;
        xSubscribeParams.xQoS = eMQTTQoS1;

        xReturned = MQTT_AGENT_Subscribe(xMQTTHandle, &xSubscribeParams, pdMS_TO_TICKS(10000));
        if (xReturned == eMQTTAgentSuccess) {
            LOG_I("MQTT Echo demo subscribed to %s\r\n", SUB_TOPIC);
        }
        else {
            LOG_I("ERROR:  MQTT Echo demo could not subscribe to %s\r\n", SUB_TOPIC);
            LED_RED_ON();
        }

        while ((publishCount > 0)) {
            LOG_I("-->sleep");
            vTaskDelay(pdMS_TO_TICKS(1000));

            memset(&(xPublishParameters_QOS0), 0x00, sizeof(xPublishParameters_QOS0));
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS0", (int)i++);
            xPublishParameters_QOS0.pucTopic = (const uint8_t *)PUB_TOPIC;
            xPublishParameters_QOS0.pvData = cPayload;
            xPublishParameters_QOS0.usTopicLength = (uint16_t)strlen((const char *)xPublishParameters_QOS0.pucTopic);
            xPublishParameters_QOS0.ulDataLength = strlen(cPayload);
            xPublishParameters_QOS0.xQoS = eMQTTQoS0;
            xReturned = MQTT_AGENT_Publish(xMQTTHandle, &xPublishParameters_QOS0, pdMS_TO_TICKS(10000));
            if (publishCount > 0) {
                publishCount--;
            }
            if (xReturned == eMQTTAgentSuccess) {
                LOG_I("Echo successfully published \r\n");
            }
            else {
                LOG_E("ERROR:  Echo failed to publish \r\n");
                LED_RED_ON();
            }

            memset(&(xPublishParameters_QOS1), 0x00, sizeof(xPublishParameters_QOS1));
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS1", (int)i++);
            xPublishParameters_QOS1.pucTopic = (const uint8_t *)PUB_TOPIC;
            xPublishParameters_QOS1.pvData = cPayload;
            xPublishParameters_QOS1.usTopicLength = (uint16_t)strlen((const char *)xPublishParameters_QOS1.pucTopic);
            xPublishParameters_QOS1.ulDataLength = strlen(cPayload);
            xPublishParameters_QOS1.xQoS = eMQTTQoS1;
            xReturned = MQTT_AGENT_Publish(xMQTTHandle, &xPublishParameters_QOS1, pdMS_TO_TICKS(10000));
            if (publishCount > 0) {
                publishCount--;
            }
            if (xReturned == eMQTTAgentSuccess) {
                LOG_I("Echo successfully published \r\n");
            }
            else {
                LOG_I("ERROR:  Echo failed to publish \r\n");
            }
        }
    }
exit:
    LOG_I("Demo Example Finished");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
