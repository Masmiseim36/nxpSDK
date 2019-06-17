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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "gcp_iot_config.h"

#include "board.h"
#include "ksdk_mbedtls.h"

#include "gcpJWTDemo.h"

#include "nxLog_App.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#if defined(LPC_ENET)
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#endif
#include "jsmn.h"
#include "tst_utils_kinetis.h"
#include "aws_mqtt_agent.h"
#include "aws_bufferpool.h"
#include "aws_secure_sockets.h"
#include "ax_mbedtls.h"
#include "a71_debug.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * types
 ******************************************************************************/


typedef enum LED_COLOR {LED_INVALID, RED,GREEN, BLUE}ledColor_t;
typedef enum LED_STATE {LED_ON,LED_OFF, LED_TOGGLE}ledState_t;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int HandleReceivedMsg(char *sJsonString, uint16_t len);

static MQTTBool_t prvMQTTCallback( void * pvUserData,
                                   const MQTTPublishData_t * const pxCallbackParams );

/* Used to avoid warnings in case of unused parameters in function pointers */
#define IOT_UNUSED(x) (void)(x)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/**
 * @brief Default MQTT HOST URL is pulled from the aws_iot_config.h
 */
char HostAddress[] = GCP_IOT_MQTT_HOST;

/**
 * @brief Default MQTT port is pulled from the aws_iot_config.h
 */
uint32_t port = GCP_IOT_MQTT_PORT;

/**
 * @brief This parameter will avoid infinite loop of publish and exit the program after certain number of publishes
 */
uint32_t publishCount = 4;

/*******************************************************************************
 * Config
 ******************************************************************************/


static MQTTAgentHandle_t xMQTTHandle = NULL;
static const char clientcredentialMQTT_BROKER_ENDPOINT[] = GCP_IOT_MQTT_HOST;
#define clientcredentialMQTT_BROKER_PORT GCP_IOT_MQTT_PORT
#define echoCLIENT_ID          ( ( const uint8_t * ) GCP_IOT_MQTT_CLIENT_ID )


/*******************************************************************************
 * Code
 ******************************************************************************/

static MQTTBool_t prvMQTTCallback( void * pvUserData,
                                   const MQTTPublishData_t * const pxPublishParameters )
{
    IOT_UNUSED(pvUserData);
    LOG_I("Subscribe callback");
    LOG_I("%.*s\r\n\t%.*s", pxPublishParameters->usTopicLength, pxPublishParameters->pucTopic, (int)pxPublishParameters->ulDataLength, (char *)pxPublishParameters->pvData);

    if(0 != HandleReceivedMsg((char *)pxPublishParameters->pvData, (uint16_t)(pxPublishParameters->ulDataLength)))
    {
        LOG_I("Subscribe Msg not an agreed JSON Msg ");
    }

    return eMQTTFalse;
}


static void led_handler(
    ledColor_t led, ledState_t state)
{
    switch (led)
    {
    case RED:
        if (state == LED_ON)
        {
            LED_RED_ON();
        }
        else if (state == LED_OFF)
        {
            LED_RED_OFF();
        }
        else /*State is an enum so safely making use of else*/
        {
            LED_RED_TOGGLE();
        }
        break;
    case GREEN:
        if (state == LED_ON)
        {
            LED_GREEN_ON();
        }
        else if (state == LED_OFF)
        {
            LED_GREEN_OFF();
        }
        else /*State is an enum so safely making use of else*/
        {
            LED_GREEN_TOGGLE();
        }
        break;
    case BLUE:
        if (state == LED_ON)
        {
            LED_BLUE_ON();
        }
        else if (state == LED_OFF)
        {
            LED_BLUE_OFF();
        }
        else /*State is an enum so safely making use of else*/
        {
            LED_BLUE_TOGGLE();
        }
        break;
    default:
        printf("wrong LED \n");
    }
}

int HandleReceivedMsg(char *sJsonString, uint16_t len) {
    int i;
    int r;
    jsmn_parser p;
    ledColor_t led = LED_INVALID;
    ledState_t state;
    jsmntok_t t[20]; /* We expect no more than 20 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, sJsonString, len, t, sizeof(t)/sizeof(t[0]));
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(sJsonString, &t[i], "red") == 0) {
            led = RED;
        } else if (jsoneq(sJsonString, &t[i], "green") == 0) {
            led = GREEN;
        } else if (jsoneq(sJsonString, &t[i], "blue") == 0) {
            led = BLUE;
        } else if (jsoneq(sJsonString, &t[i], "user") == 0) {
            /* Pass */
        } else {
//            printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
//                    sJsonString + t[i].start);
            continue;
        }
        printf("- LED Color: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start);
        i+=1;
        if (jsoneq(sJsonString, &t[i], "on") == 0) {
            state = LED_ON;
        }else if (jsoneq(sJsonString, &t[i], "off") == 0) {
            state = LED_OFF;
        }else if (jsoneq(sJsonString, &t[i], "toggle") == 0) {
                state = LED_TOGGLE;
        } else{
            //printf("Unexpected key: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start);
            continue;
        }
        printf("- state: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start);

        if (LED_INVALID != led) {
            led_handler( led,  state);
        }

    }
    return 0;
}

void gcpPubSub(const char * gcpJWToken)
{
    BaseType_t xResult;
    PRINTF("GAE subscribe publish example\n\n");
    int i = 0;
    char cPayload[100];


    MQTTAgentConnectParams_t xConnectParameters =
    {
        clientcredentialMQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
        mqttagentREQUIRE_TLS,                 /* Connection flags. */
        pdFALSE,                              /* Deprecated. */
        clientcredentialMQTT_BROKER_PORT,     /* Port number on which the MQTT broker is listening. */
        echoCLIENT_ID,                        /* Client Identifier of the MQTT client. It should be unique per broker. */
        0,                                    /* The length of the client Id, filled in later as not const. */
        pdFALSE,                              /* Deprecated. */
        NULL,                                 /* User data supplied to the callback. Can be NULL. */
        NULL,                                 /* Callback used to report various events. Can be NULL. */
        NULL,                                 /* Certificate used for secure connection. Can be NULL. */
        0,                                    /* Size of certificate used for secure connection. */
        NULL,                                 /* User name for MQTT connect. Filled later*/
        0,                                    /* Length of user name. Filled later*/
        gcpJWToken,                           /* Password for MQTT connect params*/
        strlen(gcpJWToken),                   /* Length of the password */
    };

    MQTTAgentReturnCode_t xReturned;
    MQTTAgentSubscribeParams_t xSubscribeParams;
    MQTTAgentPublishParams_t xPublishParameters_QOS0;
    MQTTAgentPublishParams_t xPublishParameters_QOS1;

    xResult = MQTT_AGENT_Init();
    if (xResult == pdPASS)
    {
         xResult = BUFFERPOOL_Init();
         if (xResult == pdPASS)
         {
             xResult = SOCKETS_Init();
         }
    }

    if (xResult != pdPASS)
    {
        /*Init failure */
        LOG_I("\nFailure at MQTT Init\n");
        //LED_GREEN_OFF();
        LED_RED_ON();
        while(1);
    }

    xReturned = MQTT_AGENT_Create( &xMQTTHandle );

    if( xReturned == eMQTTAgentSuccess )
    {
        xConnectParameters.usClientIdLength = ( uint16_t ) strlen( ( const char * ) echoCLIENT_ID );
        LOG_I("MQTT attempting to connect to '%s'...",xConnectParameters.pucClientId);
        xReturned = MQTT_AGENT_Connect( xMQTTHandle,
                                        &xConnectParameters,
                                        pdMS_TO_TICKS(10000) );
        if( xReturned != eMQTTAgentSuccess )
        {
            LOG_I( ( "Connect failed \r\n") );
            /* Could not connect, so delete the MQTT client. */
            ( void ) MQTT_AGENT_Delete( xMQTTHandle );
            //LED_GREEN_OFF();
            LED_RED_ON();
            while(1);
        }

        xSubscribeParams.pucTopic = (const uint8_t *)GCP_IOT_MQTT_SUB_TOPIC;
        xSubscribeParams.pvPublishCallbackContext = NULL;
        xSubscribeParams.pxPublishCallback = prvMQTTCallback;
        xSubscribeParams.usTopicLength = sizeof(GCP_IOT_MQTT_SUB_TOPIC) - 1;
        xSubscribeParams.xQoS = eMQTTQoS1;

        xReturned = MQTT_AGENT_Subscribe(xMQTTHandle, &xSubscribeParams, pdMS_TO_TICKS(10000));
        if (xReturned == eMQTTAgentSuccess)
        {
            LOG_I("MQTT Echo demo subscribed to %s\r\n", GCP_IOT_MQTT_SUB_TOPIC);
        }
        else
        {
            LOG_I("ERROR:  MQTT Echo demo could not subscribe to %s\r\n", GCP_IOT_MQTT_SUB_TOPIC );
            LED_RED_ON();
        }

        while ((publishCount > 0))
        {
            LOG_I("-->sleep");
            vTaskDelay(pdMS_TO_TICKS(1000));

            memset( &( xPublishParameters_QOS0 ), 0x00, sizeof( xPublishParameters_QOS0 ) );
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS0", (int)i++);
            xPublishParameters_QOS0.pucTopic = (const uint8_t *)GCP_IOT_MQTT_PUB_TOPIC;
            xPublishParameters_QOS0.pvData = cPayload;
            xPublishParameters_QOS0.usTopicLength = strlen((const char *)xPublishParameters_QOS0.pucTopic);
            xPublishParameters_QOS0.ulDataLength = strlen(cPayload);
            xPublishParameters_QOS0.xQoS = eMQTTQoS0;
            xReturned = MQTT_AGENT_Publish(xMQTTHandle, &xPublishParameters_QOS0, pdMS_TO_TICKS(10000));
            if (publishCount > 0)
            {
                publishCount--;
            }
            if( xReturned == eMQTTAgentSuccess )
            {
                LOG_I("Echo successfully published \r\n");
            }
            else
            {
                LOG_I("ERROR:  Echo failed to publish \r\n");
                LED_RED_ON();
            }

            memset( &( xPublishParameters_QOS1 ), 0x00, sizeof( xPublishParameters_QOS1 ) );
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS1", (int)i++);
            xPublishParameters_QOS1.pucTopic = (const uint8_t *)GCP_IOT_MQTT_PUB_TOPIC;
            xPublishParameters_QOS1.pvData = cPayload;
            xPublishParameters_QOS1.usTopicLength = strlen((const char *)xPublishParameters_QOS1.pucTopic);
            xPublishParameters_QOS1.ulDataLength = strlen(cPayload);
            xPublishParameters_QOS1.xQoS = eMQTTQoS1;
            xReturned = MQTT_AGENT_Publish(xMQTTHandle, &xPublishParameters_QOS1, pdMS_TO_TICKS(10000));
            if (publishCount > 0)
            {
                publishCount--;
            }
            if( xReturned == eMQTTAgentSuccess )
            {
                LOG_I("Echo successfully published \r\n");
            }
            else
            {
                LOG_I("ERROR:  Echo failed to publish \r\n");
                LED_RED_ON();
            }
        }
        while(1)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

