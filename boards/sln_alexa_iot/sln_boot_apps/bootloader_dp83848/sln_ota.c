/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */
#include "sln_ota.h"
#include "sln_RT10xx_RGB_LED_driver.h"

#include "device_utils.h"
#include "mqtt_connection.h"

#define OTA_AGENT_SHUTDOWN_RETRY  60
#define APP_NETWORK_RE_LINK_RETRY 10

/* Declare the firmware version structure for all to see. */
AppVersion32_t xAppFirmwareVersion = {
    .u.x.ucMajor = 0,
    .u.x.ucMinor = 0,
    .u.x.usBuild = 0,
};

char *alexaCLIENT_ID           = NULL;
TaskHandle_t otaDoneTaskHandle = NULL;
static EventGroupHandle_t s_reconnectionEventGroup;
void SLN_OTA_CompleteCallback(OTA_JobEvent_t eEvent);

typedef enum __reconnect_state
{
    kReconnectIdle = 0,
    kReconnectReady,
    kReconnectBusy
} reconnectState_t;

typedef enum __reconnect_event
{
    kReconnectNetworkLoss = (1U << 0U),
    kReconnectNetworkUp   = (1U << 1U)
} reconnectEvent_t;

static reconnectState_t s_reconnectState = kReconnectIdle;

/* Remove the trailing '=' sign from the thing name */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void remove_equal_sign(char *my_string)
{
    const char equal = '='; // character to search
    char *found;
    found = strchr(my_string, equal);
    if (found)
    {
        /* replace '=' with null character '\0' */
        *found = '\0';
    }
}
#pragma GCC diagnostic pop
#endif

void reconnectTask(void *arg)
{
    EventBits_t reconnectionEventBits;
    uint8_t retries                    = 0;
    s_reconnectionEventGroup           = xEventGroupCreate();
    MQTTAgentReturnCode_t mqttReturned = eMQTTAgentSuccess;
    OTA_State_t ota_ret                = eOTA_AgentState_NoTransition;
    status_t wifi_ret                  = kStatus_Success;

    if (s_reconnectionEventGroup == NULL)
    {
        configPRINTF(("xEventGroupCreate failed\r\n"));
    }

    s_reconnectState = kReconnectReady;

    while (s_reconnectionEventGroup)
    {
        reconnectionEventBits =
            xEventGroupWaitBits(s_reconnectionEventGroup, kReconnectNetworkLoss, pdTRUE, pdFALSE, portMAX_DELAY);

        if (kReconnectNetworkLoss & reconnectionEventBits)
        {
            s_reconnectState = kReconnectBusy;

            /* Turn off OTA agent now */
            OTA_AgentShutdown(1000);
            for (retries = 0; retries < OTA_AGENT_SHUTDOWN_RETRY; retries++)
            {
                /* If the agent is in a blocking state, like eOTA_AgentState_RequestingJob,
                 * it will not be able to process the shutdown command until the timeout
                 * for the respective state will trigger.
                 * For example, for eOTA_AgentState_RequestingJob timeout is 30 seconds.
                 */
                ota_ret = OTA_GetAgentState();
                if (ota_ret == eOTA_AgentState_Stopped)
                {
                    break;
                }
                else
                {
                    configPRINTF(("OTA_Agent is not stopped. Current state: %d. Retries: %d. Checking again.\r\n",
                                  ota_ret, retries));
                    vTaskDelay(1000);
                }
            }
            if (ota_ret != eOTA_AgentState_Stopped)
            {
                configPRINTF(("OTA_AgentShutdown failed, resetting the board.\r\n"));
                vTaskDelay(2000);
                NVIC_SystemReset();
            }

            /* MQTT connection clean-up */
            APP_MQTT_Disconnect(false);

            for (retries = 0; retries < APP_NETWORK_RE_LINK_RETRY; retries++)
            {
                configPRINTF(("Waiting for link to re-establish...\r\n"));

                wifi_ret = APP_NETWORK_Re_Link();
                if (wifi_ret == kStatus_Success)
                {
                    configPRINTF(("...link re-established!\r\n"));
                    break;
                }
                else
                {
                    configPRINTF(("...link re-establishing failed!\r\n"));
                }
            }
            if (wifi_ret != kStatus_Success)
            {
                configPRINTF(("APP_NETWORK_Re_Link failed, resetting the board.\r\n"));
                vTaskDelay(2000);
                NVIC_SystemReset();
            }

            vTaskDelay(2000);

            /* Connect to MQTT broker */
            mqttReturned = APP_MQTT_Connect(NULL);

            if (eMQTTAgentFailure == mqttReturned)
            {
                configPRINTF(("APP_MQTT_Connect failed, resetting the board.\r\n"));
                vTaskDelay(2000);
                NVIC_SystemReset();
            }
            else if (eMQTTAgentTimeout == mqttReturned)
            {
                /* link loss, stop current iteration here, wait for a new connection */
                configPRINTF(("APP_MQTT_Connect failed, link loss during connection.\r\n"));

                /* The reconnection process is in kReconnectBusy state so WiFi link loss event
                 * was probably triggered but ignored.
                 * Force a new reconnection process.
                 */
                xEventGroupSetBits(s_reconnectionEventGroup, kReconnectNetworkLoss);
                continue;
            }
            else
            {
                /* mqtt connect succeeded */
            }

            /* OTA agent initialization */
            ota_ret = OTA_AgentInit(APP_MQTT_GetOtaHandle(), (const uint8_t *)alexaCLIENT_ID, SLN_OTA_CompleteCallback,
                                    (TickType_t)~0);

            if (ota_ret != eOTA_AgentState_Ready)
            {
                if (get_connect_state())
                {
                    configPRINTF(("OTA_AgentInit failure, resetting the board.\r\n"));
                    vTaskDelay(2000);
                    NVIC_SystemReset();
                }
                else
                {
                    /* link loss during OTA_AgentInit */
                    configPRINTF(("OTA_AgentInit failed, link loss during connection.\r\n"));

                    /* The reconnection process is in kReconnectBusy state so WiFi link loss event
                     * was probably triggered but ignored.
                     * Force a new reconnection process.
                     */
                    xEventGroupSetBits(s_reconnectionEventGroup, kReconnectNetworkLoss);
                    continue;
                }
            }

            if (get_connect_state() == false)
            {
                configPRINTF(("Link lost during reconnection, trying again.\r\n"));

                /* The reconnection process is in kReconnectBusy state so WiFi link loss event
                 * was probably triggered but ignored.
                 * Force a new reconnection process.
                 */
                xEventGroupSetBits(s_reconnectionEventGroup, kReconnectNetworkLoss);
                continue;
            }

            s_reconnectState = kReconnectReady;
        }
    }

    vTaskDelete(NULL);
}

void otaDoneTask(void *arg)
{
    /* suspend the task here, wait for the OTA PAL to wake it */
    ulTaskNotifyTake(true, ULONG_MAX);

    /* extra delay, precaution measure, give time to OTA agent
     * to send status message to cloud */
    vTaskDelay(2000);

    /* wait for max 10 seconds to have the agent in ready state */
    int retry = 100;
    while (retry--)
    {
        if (OTA_GetAgentState() != eOTA_AgentState_Ready)
        {
            vTaskDelay(100);
        }
        else
        {
            break;
        }
    }

    prvPAL_ResetDevice();

    /* prvPAL_ResetDevice should reset the board, but, hey, this is still a FreeRTOS task */
    vTaskDelete(NULL);
}

static uint32_t mainAppVersionGet()
{
    bool commflag    = false;
    int32_t imgtype  = FICA_IMG_TYPE_APP_A;
    uint32_t appaddr = FICA_IMG_APP_A_ADDR;

    /* Just in case ... */
    FICA_initialize();

    /* Determine if we are in self test mode. We can do that by verifying ICA_COMM_AIS_NAP_BIT */
    if (FICA_get_comm_flag(FICA_COMM_AIS_NAP_BIT, &commflag) != SLN_FLASH_NO_ERROR)
        return SLN_FLASH_ERROR;

    if (commflag)
    {
        /* self test mode, get the address of the pending commit main app */
        if (FICA_get_new_app_img_type(&imgtype) != SLN_FLASH_NO_ERROR)
            return SLN_FLASH_ERROR;

        configPRINTF(("Reading version of pending commit app, type %d ... \r\n", imgtype));
    }
    else
    {
        /* not self test mode, get the address of the currently running main app */
        if (FICA_GetCurAppStartType(&imgtype) != SLN_FLASH_NO_ERROR)
            return SLN_FLASH_ERROR;

        configPRINTF(("Reading version of main app, type %d ... \r\n", imgtype));
    }

    if (imgtype == 0 || imgtype > FICA_NUM_IMG_TYPES)
        return SLN_FLASH_ERROR;

    if (FICA_get_app_img_start_addr(imgtype, &appaddr) != SLN_FLASH_NO_ERROR)
        return SLN_FLASH_ERROR;

    xAppFirmwareVersion.u.ulVersion32 = (uint32_t) * ((uint32_t *)(SLN_Flash_Get_Read_Address(appaddr + 0x3C0)));
    configPRINTF(("App version is %d.%d.%d\r\n", xAppFirmwareVersion.u.x.ucMajor, xAppFirmwareVersion.u.x.ucMinor,
                  xAppFirmwareVersion.u.x.usBuild));

    return 0;
}

void APP_connect_update_handler(bool connected)
{
    if ((!connected) && (get_connect_state()))
    {
        /* signal reconnection task about the event */
        if ((NULL != s_reconnectionEventGroup) && (s_reconnectState == kReconnectReady))
        {
            xEventGroupSetBits(s_reconnectionEventGroup, kReconnectNetworkLoss);
        }
    }
}

void SLN_OTA_CompleteCallback(OTA_JobEvent_t eEvent)
{
    DEFINE_OTA_METHOD_NAME("prvDefaultOTACompleteCallback");

    OTA_Err_t xErr = kOTA_Err_Uninitialized;

    if (eEvent == eOTA_JobEvent_Activate)
    {
        OTA_LOG_L1("[%s] Received eOTA_JobEvent_Activate callback from OTA Agent.\r\n", OTA_METHOD_NAME);
        (void)OTA_ActivateNewImage();
    }
    else if (eEvent == eOTA_JobEvent_Fail)
    {
        OTA_LOG_L1("[%s] Received eOTA_JobEvent_Fail callback from OTA Agent.\r\n", OTA_METHOD_NAME);
        /* Nothing special to do. The OTA agent handles it. */
    }
    else if (eEvent == eOTA_JobEvent_StartTest)
    {
        int32_t imgType = FICA_IMG_TYPE_NONE;
        int32_t status  = SLN_FLASH_NO_ERROR;
        bool savedCert  = false;
        /* Accept the image since it was a good transfer
         * and networking and services are all working.
         */
        OTA_LOG_L1("[%s] Received eOTA_JobEvent_StartTest callback from OTA Agent.\r\n", OTA_METHOD_NAME);

        // Get new image type
        status = FICA_get_new_app_img_type(&imgType);

        OTA_LOG_L1("[%s] New image type %d.\r\n", OTA_METHOD_NAME, imgType);

        if (SLN_FLASH_NO_ERROR == status)
        {
            if (SLN_FLASH_NO_ERROR == status)
            {
                // Verify Certificate
                status = FICA_Verify_Certificate(imgType);
            }

            if (SLN_FLASH_NO_ERROR == status)
            {
                // Save Certificate
                status = FICA_Save_Certificate(imgType);

                if (SLN_FLASH_MGMT_EOVERFLOW == status || SLN_FLASH_MGMT_EOVERFLOW2 == status)
                {
                    // Handle file header overflow
                    status = FICA_Erase_Certificate(imgType);

                    if (SLN_FLASH_NO_ERROR == status)
                    {
                        // Save Certificate again
                        status = FICA_Save_Certificate(imgType);
                    }
                }
            }

            if (SLN_FLASH_NO_ERROR == status)
            {
                savedCert = true;
            }

            if (SLN_FLASH_NO_ERROR == status)
            {
                // Verify Image Signature
                status = FICA_Verify_Signature(imgType);
            }

            OTA_LOG_L1("[%s] Signature verification status: %d.\r\n", OTA_METHOD_NAME, status);
        }

        if (SLN_FLASH_NO_ERROR == status)
        {
            /* Check if there is a mismatch between intended update bank
             * and the actual entry point address */
            status = FICA_Verify_OTA_Image_Entry_Point(imgType);

            if (SLN_FLASH_NO_ERROR != status)
            {
                OTA_LOG_L1("[%s] Image entry point check failed, firmware not built for the specified bank.\r\n",
                           OTA_METHOD_NAME);
            }
        }

        /* OTA_SetImageState call below will reach PAL layer, eventually calling
         * prvPAL_SetPlatformImageState. NAP bit will be cleared there. For
         * app rejected flow and in case eXIP is used, PRDB context will be
         * restored there as well. */
        if (SLN_FLASH_NO_ERROR == status)
        {
            xErr = OTA_SetImageState(eOTA_ImageState_Accepted);
        }
        else
        {
            xErr = OTA_SetImageState(eOTA_ImageState_Rejected);
        }

        if (xErr != kOTA_Err_None)
        {
            OTA_LOG_L1("[%s] Failed to set image state, error %d.\r\n", OTA_METHOD_NAME, xErr);
        }

        if (xErr != kOTA_Err_None || SLN_FLASH_NO_ERROR != status)
        {
            /* Invalidate certificate */
            if (savedCert)
            {
                status = FICA_Invalidate_Certificate(imgType);

                if (SLN_FLASH_MGMT_EOVERFLOW == status || SLN_FLASH_MGMT_EOVERFLOW2 == status)
                {
                    /* Handle file header overflow */
                    status = FICA_Erase_Certificate(imgType);

                    if (SLN_FLASH_NO_ERROR == status)
                    {
                        /* Invalidate certificate again */
                        status = FICA_Invalidate_Certificate(imgType);
                    }
                }

                if (SLN_FLASH_NO_ERROR != status)
                {
                    OTA_LOG_L1("[%s] Failed to invalidate certificate, error %d.\r\n", OTA_METHOD_NAME, status);
                }
            }
        }
    }
    else
    {
        OTA_LOG_L1("[%s] Received invalid job event %d from OTA Agent.\r\n", OTA_METHOD_NAME, eEvent);
    }
}

void selfTestCleanup(void)
{
    DEFINE_OTA_METHOD_NAME("selfTestCleanup");

    bool self_test   = false;
    int32_t fica_ret = 0;
    int32_t auth_ret = 0;

    /* Determine if we are in self test mode. We can do that by verifying ICA_COMM_AIS_NAP_BIT */
    fica_ret = FICA_get_comm_flag(FICA_COMM_AIS_NAP_BIT, &self_test);
    if (SLN_FLASH_NO_ERROR != fica_ret)
    {
        OTA_LOG_L1("[%s] Failed to read FICA_COMM_AIS_NAP_BIT, error %d\r\n", OTA_METHOD_NAME, fica_ret);
    }

    if ((SLN_FLASH_NO_ERROR == fica_ret) && self_test)
    {
        OTA_LOG_L1("[%s] Clearing NAP bit ...\r\n", OTA_METHOD_NAME);

        /* Must reset NAP bit, otherwise we'll read wrong version at next OTA */
        fica_ret = FICA_clr_comm_flag(FICA_COMM_AIS_NAP_BIT);
        if (SLN_FLASH_NO_ERROR != fica_ret)
        {
            OTA_LOG_L1("[%s] Failed to reset ICA_COMM_AIS_NAP_BIT, error %d\r\n", OTA_METHOD_NAME, fica_ret);
        }

        if (bl_nor_encrypt_is_enabled())
        {
            OTA_LOG_L1("[%s] Swapping PRDB context ...\r\n", OTA_METHOD_NAME);

            /* Swap crypto context */
            auth_ret = SLN_AUTH_swap_context(SLN_CRYPTO_CTX_1);
            if (SLN_AUTH_OK != auth_ret)
            {
                OTA_LOG_L1("[%s] Failed to swap PRDB context, error %d\r\n", OTA_METHOD_NAME, auth_ret);
            }
        }
    }
}

void vApplicationDaemonTaskStartupHook(void)
{
    /* Nothing to do here */
}

void Run_OTA_Task(void)
{
    static demoContext_t mqttDemoContext = {.networkTypes                = DEMO_NETWORK_TYPE,
                                            .demoFunction                = otaAppInitTask,
                                            .networkConnectedCallback    = NULL,
                                            .networkDisconnectedCallback = NULL};

    APP_Connect_Update_Handler_Set(&APP_connect_update_handler);

    xTaskCreate(runDemoTask, "AWS_OTA_Task", 2048, &mqttDemoContext, configMAX_PRIORITIES - 1, NULL);
}

int otaAppInitTask(bool awsIotMqttMode,
                   const char *pIdentifier,
                   void *pNetworkServerInfo,
                   void *pNetworkCredentialInfo,
                   const IotNetworkInterface_t *pNetworkInterface)
{
    /* give time for usb logging init */
    vTaskDelay(2000);

    /* Get the main app version */
    if (mainAppVersionGet())
    {
        configPRINTF(("mainAppVersionGet failed\r\n"));
        goto exit;
    }

#if USE_ETHERNET_CONNECTION
    APP_NETWORK_Init(true); // start Ethernet with DHCP
#endif

    xTaskCreate(reconnectTask, "Reconnect_task", 1400, NULL, configMAX_PRIORITIES - 1, NULL);

    /* Create a task for getting signaled when OTA finishes and a reset is needed */
    xTaskCreate(otaDoneTask, "ota_done_task", 1024, NULL, tskIDLE_PRIORITY + 1, &otaDoneTaskHandle);

    /* Inform the AWS OTA PAL about the task handle */
    otaPalOtaDoneTaskSet(otaDoneTaskHandle);

    APP_GetUniqueID(&alexaCLIENT_ID, true);

    /* Connect to MQTT broker */
    MQTTAgentReturnCode_t ret = APP_MQTT_Connect(NULL);

    if (eMQTTAgentFailure == ret)
    {
        configPRINTF(("APP_MQTT_Connect failed\r\n"));
        goto exit;
    }
    else if (eMQTTAgentTimeout == ret)
    {
        /* link loss, stop current iteration here, wait for a new connection */
        configPRINTF(("APP_MQTT_Connect failed, link loss during connection.\r\n"));
        goto exit;
    }
    else
    {
        /* mqtt connect succeeded */
    }

    OTA_State_t eState = eOTA_AgentState_NoTransition;

    /* OTA agent initialization */
    eState = OTA_AgentInit(APP_MQTT_GetOtaHandle(), (const uint8_t *)alexaCLIENT_ID, SLN_OTA_CompleteCallback,
                           (TickType_t)~0);
    for (;;)
    {
        while ((eState = OTA_GetAgentState()) != eOTA_AgentState_Init)
        {
            /* Wait forever for OTA traffic but allow other tasks to run
               and output statistics from time to time. */

            vTaskDelay(5000);
            configPRINTF(("State: %d  Received: %u   Queued: %u   Processed: %u   Dropped: %u\r\n", OTA_GetAgentState(),
                          OTA_GetPacketsReceived(), OTA_GetPacketsQueued(), OTA_GetPacketsProcessed(),
                          OTA_GetPacketsDropped()));
        }

        /* ... Handle MQTT disconnect per your application needs ... */
        vTaskDelay(5000);
    }

exit:
    /* If reached here for self test phase, treat this like self test failed:
     * - reset NAP bit
     * - swap PRDB context when eXIP is used */
    selfTestCleanup();

    configPRINTF(("Resetting ... .\r\n"));
    vTaskDelay(2000);
    NVIC_SystemReset();
    vTaskDelete(NULL);
}
