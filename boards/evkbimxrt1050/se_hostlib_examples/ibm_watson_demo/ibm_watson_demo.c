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
#include "board.h"
#include "ksdk_mbedtls.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "nxLog_App.h"

#include "mbedtls/platform.h"
#include "ax_mbedtls.h"
#include "ax_api.h"
#include "aws_clientcredential.h"
#include "HLSEComm.h"
#include "HLSEAPI.h"
#include "tst_utils_kinetis.h"
#include "aws_mqtt_agent.h"
#include "aws_bufferpool.h"
#include "aws_secure_sockets.h"
#include "mbedtls/debug.h"
#include "jsmn.h"
#ifdef IMX_RT
#include "pin_mux.h"
#include "fsl_trng.h"
#include "fsl_dcp.h"
#include "sm_printf.h"
#include "fsl_iomuxc.h"


#endif

#ifdef CPU_LPC54018JET180
#   include "fsl_power.h"
#endif


/*******************************************************************
* MACROS
*******************************************************************/
#ifdef CPU_MIMXRT1052DVL6B
#define TRNG0 TRNG
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define WATSONIOT_TASK_PRIORITY (tskIDLE_PRIORITY)
#define WATSONIOT_TASK_STACK_SIZE 7000
#define CERT_DER_SIZE                           900



static MQTTAgentHandle_t xMQTTHandle = NULL;

uint32_t publishCount = 4;


typedef enum LED_COLOR {RED,GREEN, BLUE}ledColor_t;
typedef enum LED_STATE {LED_ON,LED_OFF, LED_TOGGLE}ledState_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void WatsonIoTDemo_task(void *);
void awsPubSub(const U8 ax_uid[18]);
void BOARD_InitNetwork(const unsigned char ax_uid[18]);

/*******************************************************************************
 * Variables
 ******************************************************************************/

mbedtls_ssl_config conf;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined (IMX_RT)
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}
#endif

/*!
 * @brief Main function.
 */
#undef SDK_OS_FREE_RTOS
#undef FSL_RTOS_FREE_RTOS

#if defined(MBEDTLS_DEBUG_C)
 void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
     ((void)level);
     LOG_I("\r\n%s, at line %d in file %s\n", str, line, file);
}
#endif
 /*For subscribing to commands */
 static void led_handler(ledColor_t led, ledState_t state)
  {
     switch (led) {

     case RED:
         if (state == LED_ON) {
             LED_RED_ON();
         }

         else if (state == LED_OFF) {
             LED_RED_OFF();
         } else /*State is an enum so safely making use of else*/
         {
             LED_RED_TOGGLE();
         }

         break;
     case GREEN:
         if (state == LED_ON) {
             LED_GREEN_ON();
         }

         else if (state == LED_OFF) {
             LED_GREEN_OFF();
         } else /*State is an enum so safely making use of else*/
         {
             LED_GREEN_TOGGLE();
         }
         break;
     case BLUE:
         if (state == LED_ON) {
             LED_BLUE_ON();
         }

         else if (state == LED_OFF) {
             LED_BLUE_OFF();
         } else /*State is an enum so safely making use of else*/
         {
             LED_BLUE_TOGGLE();
         }
         break;
     default:
         LOG_I("wrong LED \n");

     }
 }

 int HandleReceivedMsg(char *sJsonString, uint16_t len) {
     int i;
     int r;
     jsmn_parser p;
     ledColor_t led;
     ledState_t state;
     jsmntok_t t[80]; /* Limit tokens to be parsed.*/

     jsmn_init(&p);
     r = jsmn_parse(&p, sJsonString, len, t, sizeof(t)/sizeof(t[0]));
     if (r < 0) {
         LOG_I("Failed to parse JSON: %d\n", r);
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
         } else if (jsoneq(sJsonString, &t[i], "GREEN") == 0) {
             led = GREEN;
         } else if (jsoneq(sJsonString, &t[i], "BLUE") == 0) {
             led = BLUE;
         } else {
 /*           LOG_I("Unexpected key: %.*s\n", t[i].end-t[i].start,
                     sJsonString + t[i].start); */
             continue;
         }
         LOG_I("- LED Color: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start);
         i+=2;
         if (jsoneq(sJsonString, &t[i], "ON") == 0) {
             state = LED_ON;
         }else if (jsoneq(sJsonString, &t[i], "OFF") == 0) {
             state = LED_OFF;
         }else if (jsoneq(sJsonString, &t[i], "toggle") == 0) {
                 state = LED_TOGGLE;
         } else{
     /*        LOG_I("Unexpected key: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start); */
             continue;
         }
         LOG_I("- state: %.*s\n", t[i].end-t[i].start, sJsonString + t[i].start);

         led_handler( led,  state);
         break;
     }
     return 0;
 }

 /*For subscribing to commands */

/*Subscribe and publish events*/
 static MQTTBool_t prvMQTTCallback( void * pvUserData,
                                    const MQTTPublishData_t * const pxPublishParameters )
 {
     //IOT_UNUSED(pvUserData);
     LOG_I("Subscribe callback");
     LOG_I("%.*s\r\n\t%.*s", pxPublishParameters->usTopicLength, pxPublishParameters->pucTopic, (int)pxPublishParameters->ulDataLength, (char *)pxPublishParameters->pvData);

    if(0 != HandleReceivedMsg((char *)pxPublishParameters->pvData, (uint16_t)(pxPublishParameters->ulDataLength)))
     {
         LOG_I("Subscribe Msg not an agreed JSON Msg ");
     }
     return eMQTTFalse;
 }

int main(void)
{

#if defined(IMX_RT)
    dcp_config_t dcpConfig;
    trng_config_t trngConfig;

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);

    SCB_DisableDCache();
    CRYPTO_InitHardware();


    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
    DCP_Init(DCP, &dcpConfig);

    /* Initialize TRNG */
    TRNG_GetDefaultConfig(&trngConfig);
    /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
    * It is optional.*/
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    /* Initialize TRNG */
    TRNG_Init(TRNG0, &trngConfig);
#elif defined(CPU_LPC54018JET180)
   MPU_Type *base = MPU;

   CLOCK_EnableClock(kCLOCK_InputMux);
   /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
   CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

   /* Enable Clock for RIT */
   CLOCK_EnableClock(kCLOCK_Gpio3);

   /* attach 12 MHz clock to FLEXCOMM2 (I2C master) */
   CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

   /* reset FLEXCOMM for I2C */
   RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);

#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kSHA_RST_SHIFT_RSTn);
#endif

   BOARD_InitBootPins();
   BOARD_BootClockFROHF96M();
   BOARD_InitDebugConsole();
   /* Disable MPU. */
   base->CTRL &= ~(0x1U);
#else
    SYSMPU_Type *base = SYSMPU;
    /*Initialize to bring up the board */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

        /* Disable SYSMPU. */
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;

#endif

    /*Initialize the Tri-color LED */
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);
    LED_BLUE_INIT(1);

    CRYPTO_InitHardware();



    /*Create WatsonIoT task */
    if (xTaskCreate(&WatsonIoTDemo_task, "WatsonIoTDemo_task", WATSONIOT_TASK_STACK_SIZE, NULL, WATSONIOT_TASK_PRIORITY, NULL) != pdPASS)
    {
        LOG_I("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    __disable_irq();
    __WFI();  /* Never exit */

    /* Should not reach this statement */
    for (;;)
        ;

}

void WatsonIoTDemo_task(void * ctx) {

    mbedtls_pk_context pk;
    int ret;
    /* Connect to A71CH */
    uint16_t connectStatus;
    SmCommState_t commState;
    U8 Atr[64];
    U16 AtrLen = sizeof(Atr);
    BaseType_t xResult;
    char cPayload[100];
    int i = 0;
    U8 AxUID[A71CH_MODULE_UNIQUE_ID_LEN];
    U16 AxUIDLen = A71CH_MODULE_UNIQUE_ID_LEN;
    unsigned char * p_cert_der_buf;
    U16 length_der;
    U8 exitFlag=0x0;
    U16 Temp = 0x0;
    U8 InfiniteLoopFlag=0x01;

    BOARD_InitNetwork(AxUID);


    connectStatus = SM_Connect(&commState, Atr, &AtrLen);
    if (connectStatus != SW_OK)
    {
       LOG_I("Failed to establish connection to Secure Module");
       LED_BLUE_OFF();
       LED_RED_ON();
       while(1);
    }




    A71_GetUniqueID(AxUID, &AxUIDLen);

    mbedtls_pk_init( &pk );
    ret = ax_mbedtls_associate_keypair(WATSON_IOT_KEYS_INDEX_SM, &pk);
    if (ret != 0)
    {
        LOG_I("Failed to Associating the key pair ");
    }

#if defined(MBEDTLS_DEBUG_C)
     mbedtls_debug_set_threshold(3);
#endif /* MBEDTLS_DEBUG_C */



     do
     {
     LOG_I("\tDemo App for A71CH with Watson IoT\r\n");
    /* Update watson root CA certificates which is to be used during SSL init*/

    p_cert_der_buf = tlsVERISIGN_ROOT_CERT_WATSON_PEM;
    length_der = tlsVERISIGN_ROOT_CERT_WATSON_LENGTH;


   MQTTAgentConnectParams_t xConnectParameters =
   {
        WATSONIOT_MQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
        mqttagentREQUIRE_TLS,           /* Connection flags. */
        pdFALSE,                        /* Deprecated. */
        WATSONIOT_MQTT_BROKER_PORT,     /* Port number on which the MQTT broker is listening. */
        WATSONIOT_A71CH_CLIENT_ID,      /* Client Identifier of the MQTT client. It should be unique per broker. */
        0,                              /* The length of the client Id, filled in later as not const. */
        pdFALSE,                        /* Deprecated. */
        NULL,                           /* User data supplied to the callback. Can be NULL. */
        NULL,                           /* Callback used to report various events. Can be NULL. */
        (char *)(p_cert_der_buf),       /* Certificate used for secure connection. Can be NULL. */
        length_der,                     /* Size of certificate used for secure connection. */
        CUSTOM_MQTT_USER_NAME,          /* Custom user Name which is used during MQTT connect*/
        0,                               /* Length of User name. To be updated later */
        NULL,                           /* Password used for connection */
        0,                              /* Length of password */
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

    xReturned = MQTT_AGENT_Create( &xMQTTHandle );

    xConnectParameters.usClientIdLength = ( uint16_t ) strlen( ( const char * ) WATSONIOT_A71CH_CLIENT_ID );
    if(xConnectParameters.cUserName != NULL)
    {
        xConnectParameters.uUsernamelength  = ( uint16_t ) strlen( ( const char * ) CUSTOM_MQTT_USER_NAME );
    }

    LOG_I("MQTT attempting to connect...");
    xReturned = MQTT_AGENT_Connect( xMQTTHandle,
                                    &xConnectParameters,
                                    pdMS_TO_TICKS(10000) );
    if( xReturned != eMQTTAgentSuccess )
    {
        LOG_I("\tConnect failed\r\n");
        if ((xReturned = MQTT_AGENT_Delete(xMQTTHandle)) == eMQTTAgentSuccess)
        {
            LOG_I("\tAgent deleted successfully \r\n");
        }
        Temp++;
        InfiniteLoopFlag = 0x0;
        LOG_I("-->sleep\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
        goto done;
    }
    else
    {
        LOG_I("MQTT attempting to connect... Suceeded\n");
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
    if (xReturned == eMQTTAgentSuccess)
    {
        PRINTF("MQTT Echo demo subscribed to %s\r\n", WATSON_SUB_TOPIC);
    }
    else
    {
        LOG_I("ERROR:  MQTT Echo demo could not subscribe to %s\r\n", WATSON_SUB_TOPIC );
        LED_RED_ON();
        exitFlag = 0x01;
        goto done;
    }


    while ((publishCount > 0))
    {
            LOG_I("-->sleep\n");
            vTaskDelay(pdMS_TO_TICKS(1000));

            memset( &( xPublishParameters_QOS0 ), 0x00, sizeof( xPublishParameters_QOS0 ) );
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS0", (int)i++);
            xPublishParameters_QOS0.pucTopic = (const uint8_t *)WATSON_PUB_TOPIC;
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
                exitFlag = 0x01;
                goto done;
            }

            memset( &( xPublishParameters_QOS1 ), 0x00, sizeof( xPublishParameters_QOS1 ) );
            sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS1", (int)i++);
            xPublishParameters_QOS1.pucTopic = (const uint8_t *)WATSON_PUB_TOPIC;
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
                //Dont set the exitFlag and Temp variable because as we want the MQTT connection to be alive for Subscribe
                /*This while loop ensures that the thread is kept alive for subscribe and this condition is reached if every thing is successful*/
                exitFlag = 0x00;
                Temp++;
                InfiniteLoopFlag = 0x01;
                goto done;

            }
            else
            {
                LOG_I("ERROR:  Echo failed to publish \r\n");
                exitFlag = 0x01;
                goto done;
            }
    }

    done:
        if(exitFlag)
        {
            MQTT_AGENT_Disconnect(xMQTTHandle,pdMS_TO_TICKS(10000));
            MQTT_AGENT_Delete(xMQTTHandle);
        }
        else
        {
            while(InfiniteLoopFlag)
            {
                vTaskDelay(pdMS_TO_TICKS(1));
            }
        }

    } while(Temp <= 1);



}

