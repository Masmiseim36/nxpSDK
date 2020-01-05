/**
 * @file ibm_watson_demo.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017,2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * IBM watson demo file
 */

/*******************************************************************
* INCLUDE FILES
*******************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "ibm_watson_iot_config.h"
#include "sm_types.h"

#if AX_EMBEDDED
#include "ksdk_mbedtls.h"
#include "app_boot.h"
#include "board.h"
#endif

#include "HLSETypes.h"
#include "mbedtls/platform.h"
#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#include <fsl_sscp_a71ch.h>
#endif
#if SSS_HAVE_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss.h"
#endif
#include "ax_api.h"
#include "aws_clientcredential.h"
#include "HLSEComm.h"
#include "HLSEAPI.h"

#include "iot_init.h"
#include "iot_mqtt_agent_config.h"
#include "iot_mqtt_agent_config_defaults.h"
#include "iot_mqtt_agent.h"
//#include "aws_bufferpool.h"
#include "iot_secure_sockets.h"
#include "mbedtls/debug.h"
#include "jsmn.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include "mbedtls/pk.h"

#if SSS_HAVE_SSS
#include <fsl_sss_api.h>
#include <fsl_sss_sscp.h>
#endif

#include "ax_api.h"
#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#include <fsl_sscp_a71ch.h>
#endif
#if SSS_HAVE_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss.h"
#endif

#include "mbedtls/platform.h"
#include "sm_demo_utils.h"

#include "ledHandler.h"

#include <nxLog_App.h>

#if AX_EMBEDDED
#include <ex_sss.h>
#include <ex_sss_boot.h>

static ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;
ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx = &gex_sss_demo_boot_ctx;

static ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx;
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_demo_boot_ctx)
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_DO_ERASE 0

#include <ex_sss_main_inc.h>
#endif

/*******************************************************************
* MACROS
*******************************************************************/
#define WATSONIOT_TASK_PRIORITY (tskIDLE_PRIORITY)
#define WATSONIOT_TASK_STACK_SIZE 7000
#define CERT_DER_SIZE 900

/*******************************************************************
* FUNCTION DECLARATION
*******************************************************************/
void WatsonIoTDemo_task_lwip(void *);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static MQTTAgentHandle_t xMQTTHandle = NULL;
mbedtls_ssl_config conf;
uint32_t publishCount = 4;

/*******************************************************************
* FUNCTIONS
*******************************************************************/
/*!
 * @brief Main function.
 */
#undef SDK_OS_FREE_RTOS
#undef FSL_RTOS_FREE_RTOS

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
            /*        LOG_I( "Unexpected key: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start); */
            continue;
        }
        LOG_I("- state: %.*s\n", t[i].end - t[i].start, sJsonString + t[i].start);

        led_handler(led, state);
        break;
    }
    return 0;
}

/*For subscribing to commands */

/*Subscribe and publish events*/
static MQTTBool_t prvMQTTCallback(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters)
{
    //IOT_UNUSED(pvUserData);
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

#if AX_EMBEDDED
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    WatsonIoTDemo_task_lwip((void *)pCtx);

    /* Should not reach this statement */
    for (;;)
        ;
}
#endif

void WatsonIoTDemo_task_lwip(void *ctx)
{
#if ( AX_EMBEDDED && (SSS_HAVE_A71CH || SSS_HAVE_A71CL || SSS_HAVE_SE050_EAR_CH || SSS_HAVE_SE050_EAR_CL || SSS_HAVE_SE05X) )
    U8 AxUID[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
#endif

#if (SSS_HAVE_ALT_A71CH && AX_EMBEDDED)
    U16 AxUIDLen = A71CH_MODULE_UNIQUE_ID_LEN;
#endif

#if SSS_HAVE_ALT_A71CH
    uint16_t connectStatus;
    SmCommState_t commState;
    /* Connect to A71CH */
    U8 Atr[64];
    U16 AtrLen = sizeof(Atr);

#endif

    BaseType_t xResult;
    char cPayload[100];
    int i = 0;
    unsigned char *p_cert_der_buf;
    U16 length_der;
    U8 exitFlag = 0x0;
    U16 Temp = 0x0;
    U8 InfiniteLoopFlag = 0x01;
    int retryCount = 0;

    /*keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM Not used variable to avoid warning set it to NULL*/
    // keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

#if SSS_HAVE_ALT_A71CH
    commState.skip_select_applet = 0;
    connectStatus = SM_Connect(&commState, Atr, &AtrLen);
    if (connectStatus != SW_OK) {
        LOG_E("Failed to establish connection to Secure Module");
        LED_BLUE_OFF();
        LED_RED_ON();
        while (1)
            ;
    }
#endif

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(3);
#endif /* MBEDTLS_DEBUG_C */

#if (SSS_HAVE_ALT_A71CH && AX_EMBEDDED)
    A71_GetUniqueID(AxUID, &AxUIDLen);
#endif

#if ( AX_EMBEDDED && (SSS_HAVE_A71CH || SSS_HAVE_A71CL || SSS_HAVE_SE050_EAR_CH || SSS_HAVE_SE050_EAR_CL || SSS_HAVE_SE05X) )
    BOARD_InitNetwork_MAC(AxUID);
#endif

    do {
        LOG_I("\tDemo App for Secure Element with Watson IoT\r\n");
        /* Update watson root CA certificates which is to be used during SSL init*/

        p_cert_der_buf = tlsVERISIGN_ROOT_CERT_WATSON_PEM;
        length_der = tlsVERISIGN_ROOT_CERT_WATSON_LENGTH;

        MQTTAgentConnectParams_t xConnectParameters = {
            .pcURL = WATSONIOT_MQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
            .xFlags = mqttagentREQUIRE_TLS,          /* Connection flags. */
            .xURLIsIPAddress = pdFALSE,              /* Deprecated. */
            .usPort = WATSONIOT_MQTT_BROKER_PORT,    /* Port number on which the MQTT broker is listening. */
            .pucClientId =
                WATSONIOT_A71CH_CLIENT_ID, /* Client Identifier of the MQTT client. It should be unique per broker. */
            .usClientIdLength = 0,         /* The length of the client Id, filled in later as not const. */
            .xSecuredConnection = pdFALSE, /* Deprecated. */
            .pvUserData = NULL,            /* User data supplied to the callback. Can be NULL. */
            .pxCallback = NULL,            /* Callback used to report various events. Can be NULL. */
            .pcCertificate = (char *)(p_cert_der_buf), /* Certificate used for secure connection. Can be NULL. */
            .ulCertificateSize = length_der,           /* Size of certificate used for secure connection. */
            .cUserName = CUSTOM_MQTT_USER_NAME,        /* Custom user Name which is used during MQTT connect*/
            .uUsernamelength = 0,                      /* Length of User name. To be updated later */
            .p_password = NULL,                        /* Password used for connection */
            .passwordlength = 0,                       /* Length of password */
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

        xReturned = MQTT_AGENT_Create(&xMQTTHandle);

        xConnectParameters.usClientIdLength = (uint16_t)strlen((const char *)WATSONIOT_A71CH_CLIENT_ID);
        if (xConnectParameters.cUserName != NULL) {
            xConnectParameters.uUsernamelength = (uint16_t)strlen((const char *)CUSTOM_MQTT_USER_NAME);
        }

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
            goto done;
        }

        /*
	 * Subscribe and publish events *
	 */

        xSubscribeParams.pucTopic = (const uint8_t *)WATSON_SUB_TOPIC;
        xSubscribeParams.pvPublishCallbackContext = NULL;
        xSubscribeParams.pxPublishCallback = prvMQTTCallback;
        xSubscribeParams.usTopicLength = sizeof(WATSON_SUB_TOPIC) - 1;
        xSubscribeParams.xQoS = eMQTTQoS1;

        xReturned = MQTT_AGENT_Subscribe(xMQTTHandle, &xSubscribeParams, pdMS_TO_TICKS(10000));
        if (xReturned == eMQTTAgentSuccess) {
            LOG_I("MQTT Echo demo subscribed to %s\r\n", WATSON_SUB_TOPIC);
        }
        else {
            LOG_I("ERROR:  MQTT Echo demo could not subscribe to %s\r\n", WATSON_SUB_TOPIC);
            LED_RED_ON();
            exitFlag = 0x01;
            goto done;
        }

        while ((publishCount > 0)) {
            LOG_I("-->sleep\n");
            vTaskDelay(pdMS_TO_TICKS(1000));

            memset(&(xPublishParameters_QOS0), 0x00, sizeof(xPublishParameters_QOS0));
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS0", (int)i++);
            xPublishParameters_QOS0.pucTopic = (const uint8_t *)WATSON_PUB_TOPIC;
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
                LOG_I("ERROR:  Echo failed to publish \r\n");
                LED_RED_ON();
                exitFlag = 0x01;
                goto done;
            }

            memset(&(xPublishParameters_QOS1), 0x00, sizeof(xPublishParameters_QOS1));
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS1", (int)i++);
            xPublishParameters_QOS1.pucTopic = (const uint8_t *)WATSON_PUB_TOPIC;
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
                //Dont set the exitFlag and Temp variable because as we want the MQTT connection to be alive for Subscribe
                /*This while loop ensures that the thread is kept alive for subscribe and this condition is reached if every thing is successful*/
                exitFlag = 0x00;
                Temp++;
                InfiniteLoopFlag = 0x01;
                goto done;
            }
            else {
                LOG_I("ERROR:  Echo failed to publish \r\n");
                exitFlag = 0x01;
                goto done;
            }
        }

    done:
        if (exitFlag) {
            MQTT_AGENT_Disconnect(xMQTTHandle, pdMS_TO_TICKS(10000));
            MQTT_AGENT_Delete(xMQTTHandle);
        }
        else {
            LOG_I("Demo Example Finished");
            while (InfiniteLoopFlag) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }
        }

    } while (Temp <= 1);
    LOG_I("Demo Example Finished");
}
