/*
* Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
* Copyright 2017 NXP. Not a Contribution
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
* @file subscribe_publish_sample.c
* @brief simple MQTT publish and subscribe on the same topic
*
* This example takes the parameters from the aws_iot_config.h file and establishes a connection to the AWS IoT MQTT
* Platform.
* It subscribes and publishes to the same topic - "sdkTest/sub"
*
* If all the certs are correct, you should see the messages received by the application in a loop.
*
*/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "iot_init.h"
#include "iot_mqtt_lib.h"
#include "iot_mqtt_agent.h"
#include "iot_secure_sockets.h"
#include "gcpJWTDemo.h"
#include "gcp_iot_config.h"
#include "gcp_default_root_certificates.h"
#include "jsmn.h"
#include "ledHandler.h"
#include "task.h"
#include "ex_sss.h"
#include "iot_mqtt_agent_config.h"
#include "iot_mqtt_agent_config_defaults.h"

#if AX_EMBEDDED
#include "board.h"
#include "clock_config.h"
#include "fsl_device_registers.h"
#include "ksdk_mbedtls.h"
#ifdef LPC_ENET
#include "lwip/netif.h"
#include "enet_ethernetif.h"
#include "lwip/dhcp.h"
#include "lwip/opt.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#endif

#include "pin_mux.h"
#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#endif
#if SSS_HAVE_ALT_SSS
#include "sss_mbedtls.h"
#endif
#include "a71_debug.h"
#include "sm_demo_utils.h"
#endif

#include "nxLog_App.h"

/*******************************************************************************
* Definitions
******************************************************************************/

#define ENABLE_TIME_PROFILING    0
/*******************************************************************************
* Prototypes
******************************************************************************/
int HandleReceivedMsg(char *sJsonString, uint16_t len);

static MQTTBool_t prvMQTTCallback(void *pvUserData, const MQTTPublishData_t *const pxCallbackParams);

/*******************************************************************************
* Variables
******************************************************************************/

#if ENABLE_TIME_PROFILING
extern volatile uint32_t gtimer_kinetis_msticks;
#endif
/**
* @brief This parameter will avoid infinite loop of publish and exit the program after certain number of publishes
*/
uint32_t publishCount = 4;

/*******************************************************************************
* Config
******************************************************************************/

static MQTTAgentHandle_t xMQTTHandle = NULL;

/*******************************************************************************
* Code
******************************************************************************/

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
    ledColor_t led = LED_INVALID;
    ledState_t state;
    jsmntok_t t[20]; /* We expect no more than 20 tokens */

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
        if (jsoneq(sJsonString, &t[i], "red") == 0) {
            led = RED;
        }
        else if (jsoneq(sJsonString, &t[i], "green") == 0) {
            led = GREEN;
        }
        else if (jsoneq(sJsonString, &t[i], "blue") == 0) {
            led = BLUE;
        }
        else if (jsoneq(sJsonString, &t[i], "user") == 0) {
            /* Pass */
        }
        else {
            //            printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
            //                    sJsonString + t[i].start);
            continue;
        }
        LOG_I("- LED Color: %.*s\n", t[i].end - t[i].start, sJsonString + t[i].start);
        i += 1;
        if (jsoneq(sJsonString, &t[i], "on") == 0) {
            state = LED_ON;
        }
        else if (jsoneq(sJsonString, &t[i], "off") == 0) {
            state = LED_OFF;
        }
        else if (jsoneq(sJsonString, &t[i], "toggle") == 0) {
            state = LED_TOGGLE;
        }
        else {
            //printf("Unexpected key: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start);
            continue;
        }
        LOG_I("- state: %.*s\n", t[i].end - t[i].start, sJsonString + t[i].start);

        if (LED_INVALID != led) {
            led_handler(led, state);
        }
    }
    return 0;
}

void gcpPubSub(void *ctx)
{
    int i = 0;
    BaseType_t xResult;
    char cPayload[100];
    unsigned char b64token[JWT_PASSWORLD_BUFFER_LEN] = {0};
    size_t b64tokenLen = sizeof(b64token);
    char *gcpJWToken = (char *) &b64token[0];
    mbedtls_pk_context *pk = (mbedtls_pk_context *)ctx;
#if AX_EMBEDDED
    U8 AxUID[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
#endif
    int retryCount = 0;

#if ENABLE_TIME_PROFILING
    volatile uint32_t previousTime, currentTime;
#endif
    MQTTAgentConnectParams_t xConnectParameters = {
        .pcURL = GCP_IOT_MQTT_HOST,                                /* The URL of the MQTT broker to connect to. */
        .xFlags = mqttagentREQUIRE_TLS,                            /* Connection flags. */
        .xURLIsIPAddress = pdFALSE,                                /* Deprecated. */
        .usPort = GCP_IOT_MQTT_PORT,                               /* Port number on which the MQTT broker is listening. */
        .pucClientId = GCP_IOT_MQTT_CLIENT_ID,                     /* Client Identifier of the MQTT client. It should be unique per broker. */
        .usClientIdLength = 0,                                     /* The length of the client Id, filled in later as not const. */
        .xSecuredConnection = pdFALSE,                             /* Deprecated. */
        .pvUserData = NULL,                                        /* User data supplied to the callback. Can be NULL. */
        .pxCallback = NULL,                                        /* Callback used to report various events. Can be NULL. */
        .pcCertificate = (char *)tlsVERISIGN_ROOT_CERTIFICATE_PEM, /* Certificate used for secure connection. Can be NULL. */
        .ulCertificateSize = tlsVERISIGN_ROOT_CERTIFICATE_LENGTH,  /* Size of certificate used for secure connection. */
        .cUserName = "NA",                                         /* User name for MQTT connect. Filled later*/
        .uUsernamelength = sizeof("NA") - 1,                       /* Length of user name. Filled later*/
        .p_password = NULL,                                  /* Password for MQTT connect params*/
        .passwordlength = 0,                      /* Length of the password */
    };

    MQTTAgentReturnCode_t xReturned;
    MQTTAgentSubscribeParams_t xSubscribeParams;
    MQTTAgentPublishParams_t xPublishParameters_QOS0;
    MQTTAgentPublishParams_t xPublishParameters_QOS1;

#if AX_EMBEDDED
    BOARD_InitNetwork_MAC(AxUID);
#endif
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
        LED_RED_ON();
        goto exit;
    }

    xReturned = MQTT_AGENT_Create(&xMQTTHandle);

    unsigned long utc_epoch = gcpNTPGetEpoch();

    if (0 == utc_epoch) {
        LED_RED_ON();
        LED_BLUE_ON();
        utc_epoch = gcpNTPGetEpoch();
        LED_BLUE_OFF();
    }
    if (0 == utc_epoch) {
        LED_RED_ON();
        LED_BLUE_ON();
        utc_epoch = gcpNTPGetEpoch();
        LED_BLUE_OFF();
    }
    if (0 == utc_epoch) {
        LED_RED_ON();
        goto exit;
    }
    else {
        // The ratio of output bytes to input bytes is 4:3 (33% overhead). Specifically, given an input of n bytes, the output will be
        // 4*(n/3) bytes long, including padding characters.
        gcpCreateJWTTokenES256(b64token, &b64tokenLen, GCP_PROJECT_NAME, utc_epoch, 10 * 60 * 60, pk);
    }

    if (xReturned == eMQTTAgentSuccess) {
        xConnectParameters.usClientIdLength = (uint16_t)strlen((const char *)xConnectParameters.pucClientId);
        xConnectParameters.p_password = gcpJWToken;
        xConnectParameters.passwordlength = strlen(gcpJWToken);
        LOG_I("MQTT attempting to connect to\n  '%s'...", xConnectParameters.pucClientId);
#if ENABLE_TIME_PROFILING
        previousTime  =  gtimer_kinetis_msticks;
#endif

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

#if ENABLE_TIME_PROFILING
        currentTime =  gtimer_kinetis_msticks;
        LOG_I("MQTT Connection Time is \n  '%d'...", (currentTime -previousTime ));
#endif
        xSubscribeParams.pucTopic = (const uint8_t *)GCP_IOT_MQTT_SUB_TOPIC;
        xSubscribeParams.pvPublishCallbackContext = NULL;
        xSubscribeParams.pxPublishCallback = prvMQTTCallback;
        xSubscribeParams.usTopicLength = sizeof(GCP_IOT_MQTT_SUB_TOPIC) - 1;
        xSubscribeParams.xQoS = eMQTTQoS1;

#if ENABLE_TIME_PROFILING
        previousTime  =  gtimer_kinetis_msticks;
#endif
        xReturned = MQTT_AGENT_Subscribe(xMQTTHandle, &xSubscribeParams, pdMS_TO_TICKS(10000));

#if ENABLE_TIME_PROFILING
        currentTime =  gtimer_kinetis_msticks;
        LOG_I("MQTT Subscribe Time is \n  '%d'...", (currentTime -previousTime ));
#endif
        if (xReturned == eMQTTAgentSuccess) {
            LOG_I("MQTT Echo demo subscribed to %s\r\n", GCP_IOT_MQTT_SUB_TOPIC);
        }
        else {
            LOG_I("ERROR:  MQTT Echo demo could not subscribe to %s\r\n", GCP_IOT_MQTT_SUB_TOPIC);
            LED_RED_ON();
        }

        while ((publishCount > 0)) {
            LOG_I("-->sleep");
            vTaskDelay(pdMS_TO_TICKS(1000));

            memset(&(xPublishParameters_QOS0), 0x00, sizeof(xPublishParameters_QOS0));
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS0", (int)i++);
            xPublishParameters_QOS0.pucTopic = (const uint8_t *)GCP_IOT_MQTT_PUB_TOPIC;
            xPublishParameters_QOS0.pvData = cPayload;
            xPublishParameters_QOS0.usTopicLength = (uint16_t)strlen((const char *)xPublishParameters_QOS0.pucTopic);
            xPublishParameters_QOS0.ulDataLength = strlen(cPayload);
            xPublishParameters_QOS0.xQoS = eMQTTQoS0;

#if ENABLE_TIME_PROFILING
            previousTime  =  gtimer_kinetis_msticks;
#endif
            xReturned = MQTT_AGENT_Publish(xMQTTHandle, &xPublishParameters_QOS0, pdMS_TO_TICKS(10000));

#if ENABLE_TIME_PROFILING
            currentTime =  gtimer_kinetis_msticks;
            LOG_I("MQTT Publish Time is \n  '%d'...", (currentTime -previousTime ));
#endif
            if (publishCount > 0) {
                publishCount--;
            }
            if (xReturned == eMQTTAgentSuccess) {
                LOG_I("Echo successfully published \r\n");
            }
            else {
                LOG_I("ERROR:  Echo failed to publish \r\n");
                LED_RED_ON();
            }

            memset(&(xPublishParameters_QOS1), 0x00, sizeof(xPublishParameters_QOS1));
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS1", (int)i++);
            xPublishParameters_QOS1.pucTopic = (const uint8_t *)GCP_IOT_MQTT_PUB_TOPIC;
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
                LED_RED_ON();
            }
        }
    }
exit:
    LOG_I("Demo Example Finished");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
