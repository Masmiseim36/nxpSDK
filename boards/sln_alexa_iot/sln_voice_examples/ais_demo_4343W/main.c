/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <time.h>

/* Board includes */
#include "board.h"
#include "pin_mux.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

/* Crypto includes */
#include "ksdk_mbedtls.h"

/* AWS includes */
#include "iot_system_init.h"
#include "iot_logging_task.h"
#include "aws_clientcredential.h"
#include "aws_ota_check.h"
#include "aws_demo.h"
#include "iot_network_types.h"

/* AIS includes */
#include "aisv2.h"
#include "aisv2_app.h"
#include "ais_streamer.h"

/* Driver includes */
#include "fsl_dmamux.h"
#include "fsl_edma.h"

/* Device specific includes */
#include "device_utils.h"

/* Audio processing includes */
#include "pdm_to_pcm_task.h"
#include "audio_processing_task.h"
#include "sln_amplifier.h"

/* Network connection includes */
#include "udp_server.h"
#include "ais_authentication_task.h"
#include "network_connection.h"
#include "httpsclient.h"
#include "reconnection_task.h"
#include "mqtt_connection.h"

#include "ais_alerts.h"

/* Shell includes */
#include "sln_shell.h"

/* UX includes */
#include "sln_RT10xx_RGB_LED_driver.h"
#include "audio_samples.h"
#include "ux_attention_system.h"
#include "app_aws_shadow.h"

/* Button includes */
#include <switch.h>

/* Fault includes */
#include "fault_handlers.h"

/* Wake word includes*/
#include "amazon_wake_word.h"

#if PROVISIONING_METHOD == PROVISIONING_USE_BLE
/* Bluetooth includes */
#include "ble_connection.h"
#include "ble_server.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_INIT_NETWORK_CHECK_COUNT    30

#define LOGGING_STACK_SIZE 256
#define LOGGING_QUEUE_LENGTH 64

#define APP_VERSION_NUMBER (uint32_t)(((APP_MAJ_VER & 0xFFU ) << 24U) | ((APP_MIN_VER & 0xFFU) << 16U) | (APP_BLD_VER & 0xFFFFU ))

/* AIS Solution Default client ID matches given IoT Thing Name */
//#define alexaCLIENT_ID clientcredentialIOT_THING_NAME

#define alexaTOPIC_ROOT "$aws/alexa"

#ifndef alexaACCOUNT_ID
#define alexaACCOUNT_ID "793444176480"
#endif

#ifndef alexaAUTH_CLIENT_ID
#define alexaAUTH_CLIENT_ID "Paste Auth Client ID here."
#endif

#ifndef alexaAUTH_TOKEN
#define alexaAUTH_TOKEN "Paste Auth Token here."
#endif

#include "sln_flash_mgmt.h"
#include "sln_cfg_file.h"
#include "sln_file_table.h"

#define pdm_to_pcm_task_PRIORITY (configMAX_PRIORITIES - 2)
#define audio_processing_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void offline_audio_task(void *arg);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const uint64_t kEpochDayZero = 3752179200ULL; // 11/26/2018 0:00:00

volatile uint32_t aisAlertCount = 0U;
__attribute__((section(".ocram_non_cacheable_data"))) ais_alert_t appAlerts[AIS_APP_MAX_ALERT_COUNT] = {0};
__attribute__((section(".ocram_non_cacheable_data"))) char *alertsDelete[AIS_APP_MAX_ALERT_COUNT] = {0};

__attribute__((section(".ocram_non_cacheable_data"))) ais_handle_t aisHandle;
__attribute__((section(".ocram_non_cacheable_data"))) ais_config_t aisConfig;

streamer_handle_t streamerHandle;

ota_handle_t otaHandle;

TaskHandle_t appTaskHandle = NULL;
TaskHandle_t appReconnectHandle = NULL;
TaskHandle_t appInitTaskHandle = NULL;
TaskHandle_t appInitDummyNullHandle = NULL;
TaskHandle_t aisAuthTaskHandle = NULL;
TaskHandle_t xAudioProcessingTaskHandle = NULL;
TaskHandle_t xPdmToPcmTaskHandle = NULL;
TaskHandle_t xUXAttentionTaskHandle = NULL;
TaskHandle_t xReconnectionTaskHandle = NULL;
TaskHandle_t xOfflineAudioTaskHandle = NULL;

EventGroupHandle_t s_offlineAudioEventGroup = NULL;

extern EventGroupHandle_t g_button_pressed;
BaseType_t xHigherPriorityTaskWoken = pdFALSE;

/* Clock sync timer handle */
TimerHandle_t aisClockSyncHandle;

/* Clock sync directive received check timer handle */
TimerHandle_t aisClockSyncCheckHandle;

QueueHandle_t g_alertQueue;

/* Extern declaration of shadow demo function */
extern int RunShadowDemo(bool awsIotMqttMode,
                  const char *pIdentifier,
                  void *pNetworkServerInfo,
                  void *pNetworkCredentialInfo,
                  const IotNetworkInterface_t *pNetworkInterface);

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Callback for any streamer error to send app notification
 * This is here to ensure there isn't tight coupling in the streamer module */
void vStreamerErrorCallback()
{
    xTaskNotify(appTaskHandle, kExceptionEncountered, eSetBits);
}

void ais_app_clock_cb(TimerHandle_t xTimer)
{
    ais_app_data_t *appData = AIS_APP_GetAppData();
    appData->prevTime = appData->currTime;
    appData->currTime += AIS_APP_TIMER_INTERVAL_SEC;

    uint64_t timeSinceSync = appData->currTime - appData->lastRefresh;

    if ( (AIS_APP_TIME_SYNC_INTERVAL_SEC <= timeSinceSync) && (reconnection_task_get_state() == kStartState) )
    {
        xTaskNotify( appTaskHandle, kSyncClock, eSetBits );
    }

    if (kStartState != reconnection_task_get_state())
    {
        // Begin check for timers that should have triggered (Offline use case)
        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            uint64_t alertTime = AIS_Alerts_GetScheduledTime(idx);

            // Any timer before this date is guaranteed to be invalid
            if ((kEpochDayZero < alertTime) && ((alertTime != UINT64_MAX)))
            {
                if (alertTime < appData->currTime)
                {
                    // Trigger alerts that aren't already triggered
                    if (true == AIS_Alerts_GetIdleState(idx))
                    {
                        // Trigger alert in UX system
                        AIS_Alerts_Trigger(idx, true);

                        // Mark this as triggered
                        AIS_Alerts_MarkAsTriggeredOffline(idx);
                    }
                }
            }
        }
    }
}

void ais_app_sync_clock_check_cb(TimerHandle_t xTimer)
{
    ais_app_data_t *appData = AIS_APP_GetAppData();
    /* reconnect if sync clock directive not received in specified timeout */
    if (!appData->syncClockReceived)
    {
        if (reconnection_task_set_event(kReconnectAISDisconnect))
        {
            configPRINTF(("Sync clock directive not received in specified timeout, triggering a reconnection\r\n"));
        }
    }
}

BaseType_t prvMQTTCallback( void * pvUserData,
                                   const MQTTAgentCallbackParams_t * const pxCallbackParams )
{
    BaseType_t xReturn = pdFALSE;

    if( pxCallbackParams->xMQTTEvent == eMQTTAgentPublish )
    {
        configPRINTF(
                ("MQTT Topic Received %s.\r\n", pxCallbackParams->u.xPublishData.pucTopic));
    }
    else if( pxCallbackParams->xMQTTEvent == eMQTTAgentDisconnect )
    {
        configPRINTF(
                ("MQTT Disconnected %s.\r\n", clientcredentialMQTT_BROKER_ENDPOINT));

        /* Trigger the reconnection process */
        reconnection_task_set_event(kReconnectMQTTDisconnect);
    }

    return xReturn;
}

#if USE_WIFI_CONNECTION
void APP_Wifi_UX_Callback(network_wifi_conn_state_t state)
{
    ux_attention_states_t uxState = uxError;

    switch (state)
    {
        case NETWORK_WIFI_CONN_WIFI_SETUP:
            uxState = uxWiFiSetup;
            break;
        case NETWORK_WIFI_CONN_INVALID_WIFI_CRED:
            uxState = uxInvalidWiFiCred;
            break;
        case NETWORK_WIFI_CONN_NO_ACCESS_POINT:
            if (uxNoAccessPoint != ux_attention_get_state())
            {
                uxState = uxNoAccessPoint;
            }
            break;
        case NETWORK_WIFI_CONN_ACCESS_POINT_FOUND:
            uxState = uxAccessPointFound;
            break;
        default:
            uxState = uxError;
            configPRINTF(("ERROR: Undefined wifi connection state, %d\r\n", state));
            break;
    }

    if (uxState != uxError)
    {
        ux_attention_set_state(uxState);
    }
}

#endif


void APP_connect_update_handler(bool connected)
{
    if (!connected)
    {
        if (get_connect_state())
        {
            /* Trigger the reconnection process */
            reconnection_task_set_event(kReconnectNetworkLoss);
        }
    }
}

int appInit(bool awsIotMqttMode,
             const char * pIdentifier,
             void * pNetworkServerInfo,
             void * pNetworkCredentialInfo,
             const IotNetworkInterface_t * pNetworkInterface )
{
    status_t ret;
    BaseType_t xReturned;
    ais_reg_config *fileData = NULL;
    volatile bool registrationNeeded = true;
    uint8_t waitCount = 0;
    fault_status_t s_fault_log;
    status_t status = 0;
    cJSON_Hooks hooks;
    ais_app_data_t *appData = AIS_APP_GetAppData();
    uint32_t len = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;
    bool restoreMicMute = false;

    char *alexaCLIENT_ID;

    /* Check for empty flash */
    uint8_t validByteCount = 0U;

    configPRINTF(("*** Starting Alexa Test application ***\r\n"));

#if USE_ETHERNET_CONNECTION
    APP_NETWORK_Init(true); // start Ethernet with DHCP
#endif
    /* Initialize Amazon libraries */
    if (SYSTEM_Init() != pdPASS)
    {
        configPRINTF(("SYSTEM_Init failed\r\n"));
        goto error;
    }

    /* Check if the device was recovered from fault state */
    status = fault_log_flash_get(&s_fault_log);
    if (0 == status)
    {
        fault_check_status(s_fault_log);
    }
    /* At first boot SLN_FLASH_MGMT_ENOENTRY2 is returned because there is no log saved */
    else if (SLN_FLASH_MGMT_ENOENTRY2 != status)
    {
        configPRINTF(("[ERR] Failed to get fault log from flash\r\n"));
    }

    /* make the handle of the current task available in shell task */
    sln_shell_set_app_init_task_handle(&appInitTaskHandle);

    /* Initialize cJSON library to use FreeRTOS heap memory management. */
    hooks.malloc_fn = pvPortMalloc;
    hooks.free_fn = vPortFree;
    cJSON_InitHooks(&hooks);

    /* from this point on, if the appInit task gets suspended, the shell task should not resume it */
    sln_shell_set_app_init_task_handle(&appInitDummyNullHandle);

    /* Try read sharedKey */
    fileData = (ais_reg_config *)pvPortMalloc(sizeof(ais_reg_config));
    if (NULL != fileData)
    {
        //if (kStatus_Success == SLN_Read_Flash_At_Address(SECRET_ADDRESS, fileData, AIS_SECRET_LENGTH))
        uint32_t secLen = sizeof(ais_reg_config);
        if (kStatus_Success == SLN_FLASH_MGMT_Read(AIS_REGISTRATION_INFO_FILE_NAME, (uint8_t *)fileData, &secLen))
        {
            for (uint32_t idx = 0; idx < AIS_SECRET_LENGTH; idx++)
            {
                if ((fileData->sharedSecret[idx] != 0x00) && (fileData->sharedSecret[idx] != 0xFF))
                {
                    validByteCount++;
                }
            }
        }
    }
    else
    {
        configPRINTF(("ERROR: Out of Memory!!\r\n"));
        goto error;
    }

    if (validByteCount) {
        registrationNeeded = false;
    }

    /* start registration task if the device was not registered */
    if (registrationNeeded)
    {
        configPRINTF(("Device not registered\r\n"));
        /* Create ais authentication task, responsible for obtaining
         * the ais access tokens from the ais tokens endpoint */
        ais_authentication_set_app_init_task_handle(&appInitTaskHandle);
        xReturned = xTaskCreate(ais_authentication_task, "AIS_Auth_Task", 2048, &aisAuthTaskHandle, configMAX_PRIORITIES - 1, &aisAuthTaskHandle);
        assert(xReturned == pdPASS);

        /* Suspend until the tokens are obtained (will be awaken from the auth task) */
        vTaskSuspend(NULL);
    }
    else
    {
        configPRINTF(("Device already registered\r\n"));
    }

    /* Indicate to user we are now connecting */
    ux_attention_set_state(uxReconnecting);

    /* Amazon doesn't use special characters, need to remove "=" character */
    APP_GetUniqueID(&alexaCLIENT_ID, true);

    if (eMQTTAgentSuccess == APP_MQTT_Connect(prvMQTTCallback))
    {
        companion_app_status_set(MQTT_SUBSCRIBED);
    }
    else
    {
        companion_app_status_set(CONN_ERROR);
        goto error;
    }

    /* Initlialize the shadow */
    RunShadowDemo (
            true,
            alexaCLIENT_ID,
            NULL,
            NULL,
            NULL );

    STREAMER_Init();

    /* Setup the exception handler callback */
    streamerHandle.pvExceptionCallback = &vStreamerErrorCallback;

    ret = STREAMER_Create(&streamerHandle, AIS_DECODER_OPUS);
    if (ret != kStatus_Success)
    {
        configPRINTF(("STREAMER_Create failed\r\n"));
        goto error;
    }

    /* Setup task for playing offline audio */
    xTaskCreate(offline_audio_task, "Offline_Audio_Task", 256, NULL, configMAX_PRIORITIES - 2, NULL);

    ret = AIS_Init(&aisHandle, (void *)&streamerHandle);
    if (ret != kStatus_Success)
    {
        configPRINTF(("AIS_Init failed\r\n"));
        goto error;
    }

    /* Configure the client information. */
    if (!registrationNeeded)
    {
        strcpy(aisConfig.registrationConfig.awsPartnerRoot, fileData->awsPartnerRoot);
    }
    else
    {
        strcpy(aisConfig.registrationConfig.awsPartnerRoot, alexaTOPIC_ROOT);
    }

    strcpy(aisConfig.awsClientId, alexaCLIENT_ID);
    strcpy(aisConfig.awsAccountId, alexaACCOUNT_ID);
    strcpy(aisConfig.awsEndpoint, clientcredentialMQTT_BROKER_ENDPOINT);
    if (!registrationNeeded)
    {
        strcpy(aisConfig.registrationConfig.awsPartnerRoot, fileData->awsPartnerRoot);
    }
    if (registrationNeeded)
    {
        strcpy(aisConfig.awsAuthClientId, get_lwa_auth_data()->client_id);
    }
    strcpy(aisConfig.awsAuthToken, alexaAUTH_TOKEN);
    sprintf(aisConfig.firmwareVersion, "%d", APP_VERSION_NUMBER);

    SLN_AMAZON_WAKE_GetModelLocale((uint8_t *)aisConfig.locale);

    aisConfig.maxAlertCount = AIS_APP_MAX_ALERT_COUNT;
    aisConfig.speakerDecoder = AIS_SPEAKER_DECODER_OPUS;
    aisConfig.speakerChannels = 1;
    aisConfig.speakerBitrate = 64000;
    aisConfig.speakerBitrateType = AIS_SPEAKER_BITRATE_CONSTANT;

    AIS_SetConfig(&aisHandle, &aisConfig);

    /* Set shared secret */
    if (!registrationNeeded)
    {
        AIS_SetSecret(&aisHandle, fileData->sharedSecret);
        registrationNeeded = false;
    }
    else
    {
        /* invalid sharedKey secret size */
        configPRINTF(("Invalid sharedKey secret\r\n"));
    }

    /* Free the file data obtained from the flash read */
    vPortFree(fileData);
    fileData = NULL;

    /* Register with AIS
     * Only necessary to be performed once per device. */
    if (registrationNeeded)
    {
        char *auth_token = refresh_token_get();
        strcpy(aisConfig.awsAuthToken, auth_token);
        refresh_token_free();

        /* send status to the companion app */
        companion_app_status_set(REGISTERING_REQ);

        ret = AIS_Register(&aisHandle);
        if (ret != kStatus_Success)
        {
            configPRINTF(("AIS_Register failed\r\n"));
            companion_app_status_set(CONN_ERROR);
            goto error;
        }
        else
        {
            companion_app_status_set(REGISTERING_RSP);
        }
    }

    /* Initialize AIS Alerts module */
    ret = AIS_Alerts_Init();

    if (kStatus_Success != ret)
    {
        configPRINTF(("Alerts Management failed to initialize!\r\n"));
        goto error;
    }

    /* Set alert volume from NVM */
    ret = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);

    if (kStatus_Success != ret)
    {
        configPRINTF(("Warning, no saved Alert Volume!\r\n"));

        /* We are setting volume to an invalid value. Customers can change this
         * if they want to set there own default volume
         */
        appData->volume = -1;
        ret = kStatus_Success; // Just a warning, continue as needed
    }
    else
    {
        /* Set the volume to the value from flash */
        appData->volume = cfg.streamVolume;
        appData->alertVolume = cfg.alertVolume;

        /* set restore mic mute accordingly if cfg file reading succeeded */
        if (kMicMuteModeOn == cfg.mic_mute_mode)
        {
            restoreMicMute = true;
        }
    }

    /* Set UX */
    ux_attention_set_state(uxConnected);

    configPRINTF(("Wait 1 second\r\n"));
    osa_time_delay(1000);

    ret = AIS_Connect(&aisHandle);
    if (ret != kStatus_Success) {
        configPRINTF(("AIS_Connect failed\r\n"));
        companion_app_status_set(CONN_ERROR);
        goto error;
    }

    /* Gather any alerts requires for deletion */
    alertTokenList_t alertsList;
    uint32_t alertsCount = 0;
#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
    AIS_Alerts_GetAlertsList(&alertsList, &alertsCount);
#else
    AIS_Alerts_GetDeletedList(&alertsList, &alertsCount);
#endif
    configPRINTF(("Found %d alerts ready to delete.\r\n", alertsCount));

    /* Send SynchronizeState to update on our status. */
    char *alertTokens[AIS_APP_MAX_ALERT_COUNT] = {0};
    for (uint32_t idx = 0; idx < alertsCount; idx++)
    {
        alertTokens[idx] = (char *)(&alertsList[idx]);
        configPRINTF(("Alert ready for delete: %s\r\n", alertTokens[idx]));
    }


    AIS_EventSynchronizeState(&aisHandle, appData->volume, (const char **)alertTokens, alertsCount);

    companion_app_status_set(CONN_COMPLETE);

    aisClockSyncHandle = xTimerCreate( "AIS_Clock_Sync", AIS_APP_TIMER_INTERVAL_MSEC, pdTRUE, (void *)0, ais_app_clock_cb );

    xTimerStart( aisClockSyncHandle, 0);

    aisClockSyncCheckHandle = xTimerCreate( "AIS_Clock_Sync_Check", AIS_APP_TIMER_SYNC_CLOCK_TIMEOUT_MSEC, pdFALSE, (void *)0, ais_app_sync_clock_check_cb );

    OTA_Init(&otaHandle, (const char *)alexaCLIENT_ID);
    ret = OTA_Connect(&otaHandle);
    if (ret != kStatus_Success)
    {
        configPRINTF(("OTA_Connect failed\r\n"));
        goto error;
    }

    reconnection_task_init(&xReconnectionTaskHandle);

    /* Restore mic mute mode from flash */
    if (restoreMicMute)
    {
        configPRINTF(("Restoring microphone mute mode ... \r\n"));
        /* set mute mode in a non persistent way, we already have it written in flash;
         * ignore error code, the function will simply perform variable copy */
        audio_processing_set_mic_mute(kMicMuteModeOn, false);
        ux_attention_set_state(uxMicOntoOff);
    }

    vTaskResume(appTaskHandle);

    configPRINTF(("Exiting App Init\r\n"));
    vTaskDelete(NULL);

    return 0;

    error:

    configPRINTF(("ERROR: Unable to initialize. Attempting to re-establish connection...\r\n"));

    do
    {
        configPRINTF(("...attempt %d...\r\n", waitCount++));
        vTaskDelay(portTICK_PERIOD_MS * 1000);

        if (get_connect_state())
        {
            configPRINTF(("SUCCESS: Connection Available!\r\n"));
            break;
        }
    }
    while (APP_INIT_NETWORK_CHECK_COUNT > waitCount);

    if (APP_INIT_NETWORK_CHECK_COUNT == waitCount)
    {
        configPRINTF(("ERROR: Could not establish network connection! \r\nPlease check network connection...\r\n"));

        while (!get_connect_state())
        {
            vTaskDelay(portTICK_PERIOD_MS * 1000);
        }
    }

    configPRINTF(("Network connection available. Restarting device... \r\n"));

    ux_attention_sys_fault();

    vTaskDelay(portTICK_PERIOD_MS * 5000);

    NVIC_SystemReset();

}

void appTask(void *arg)
{
    ais_mic_open_t micOpen;
    ais_app_data_t *appData = AIS_APP_GetAppData();

    //osa_time_delay(100);
    configPRINTF(("One second delay...\r\n"));
    osa_time_delay(1000);

    /* Queue needs to be initialized before AIS connect as this will cause a fault if a set or delete alert occurs */
    g_alertQueue = xQueueCreate( AIS_APP_MAX_ALERT_COUNT, AIS_MAX_ALERT_TOKEN_LEN_BYTES );

    vTaskSuspend(NULL);

    // Provide the audio_processing_task module with APP Task handle and AIS Task Handle
    audio_processing_set_app_task_handle(&appTaskHandle);

    int16_t *micBuf = pdm_to_pcm_get_pcm_output();
    audio_processing_set_mic_input_buffer(&micBuf);

    int16_t *ampBuf = pdm_to_pcm_get_amp_output();
    audio_processing_set_amp_input_buffer(&ampBuf);

    audio_processing_set_task_handle(&xAudioProcessingTaskHandle);

    // Create audio processing task
    if (xTaskCreate(audio_processing_task, "Audio_proc_task", 1536U, NULL, audio_processing_task_PRIORITY, &xAudioProcessingTaskHandle) != pdPASS)
    {
        configPRINTF(("Task creation failed!.\r\n"));
        while (1);
    }

    // Set loopback event bit for AMP
    SLN_AMP_SetLoopBackEventBits(pdm_to_pcm_get_amp_loopback_event());

    // Set PDM to PCM config
    pcm_pcm_task_config_t config;
    config.thisTask = &xPdmToPcmTaskHandle;
    config.processingTask = &xAudioProcessingTaskHandle;
    config.feedbackInit = SLN_AMP_Read;
    config.feedbackBuffer = (int16_t *)SLN_AMP_GetLoopBackBuffer();

    pcm_to_pcm_set_config(&config);

    // Create pdm to pcm task
    if (xTaskCreate(pdm_to_pcm_task, "pdm_to_pcm_task", 512U, NULL, pdm_to_pcm_task_PRIORITY, &xPdmToPcmTaskHandle) != pdPASS)
    {
        configPRINTF(("Task creation failed!.\r\n"));
        while (1);
    }

    // Pass loopback event group to AMP
    EventGroupHandle_t ampLoopBackEventGroup = NULL;
    while(NULL == ampLoopBackEventGroup)
    {
        ampLoopBackEventGroup = pdm_to_pcm_get_event_group();
        vTaskDelay(10);
    }
    SLN_AMP_SetLoopBackEventGroup(&ampLoopBackEventGroup);

    SLN_AMP_WriteDefault();

    uint32_t taskNotification = 0;

    micOpen.asr_profile = AIS_ASR_FAR_FIELD;
    micOpen.initiator = AIS_INITIATOR_WAKEWORD;
    /* Initiator opaque type/token and wakeword indices not used for TAP. */
    micOpen.init_type = NULL;
    micOpen.init_token = NULL;
    micOpen.wwStart = 0;
    micOpen.wwEnd = 0;

    configPRINTF(("[App Task] Starting Available Heap: %d\r\n", xPortGetFreeHeapSize()));

    /* Give alerts the handlefor UX system*/
    AIS_Alerts_SetUxHandle(&xUXAttentionTaskHandle);
    AIS_Alerts_SetUxTimerBit(uxTimer);
    AIS_Alerts_SetUxAlarmBit(uxAlarm);
    AIS_Alerts_SetUxReminderBit(uxReminder);

    // Wait for a valid time
    while(0 == appData->currTime)
    {
        vTaskDelay(10);
    }

    // Re-sync alerts with service
    int32_t updateStatus = -1;

    updateStatus = AIS_Alerts_UpdateState(appData->currTime);

    if (1 == updateStatus)
    {
        /* Gather any alerts requires for deletion */
        alertTokenList_t alertsList;
        uint32_t alertsCount = 0;
#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
        AIS_Alerts_GetAlertsList(&alertsList, &alertsCount);
#else
        AIS_Alerts_GetDeletedList(&alertsList, &alertsCount);
#endif

        /* Send SynchronizeState to update on our status. */
        char *alertTokens[AIS_APP_MAX_ALERT_COUNT] = {0};

        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            alertTokens[idx] = (char *)(&alertsList[idx]);
        }

        AIS_EventSynchronizeState(&aisHandle, appData->volume, (const char **)alertTokens, alertsCount);
    }

    while(1)
    {

        xTaskNotifyWait( ULONG_MAX, ULONG_MAX, &taskNotification, portMAX_DELAY );

        if (kIdle & taskNotification)
        {
            // Set UX attention state
            ux_attention_set_state(uxIdle);

            taskNotification &= ~kIdle;
        }

        if (kWakeWordDetected & taskNotification)
        {
            if (reconnection_task_get_state() == kStartState)
            {
                if (!AIS_CheckState(&aisHandle, AIS_TASK_STATE_MICROPHONE))
                {
                    // Set UX attention state
                    ux_attention_set_state(uxListeningStart);

                    if (AIS_CheckState(&aisHandle, AIS_TASK_STATE_SPEAKER))
                    {
                        appData->bargein = true;

                        /* Force speaker state update here to close and send event */
                        AIS_StateSpeaker(&aisHandle);

                        /* Need to make sure there is a 50ms gap between close speaker and mic open for spec compliance */
                        vTaskDelay(portTICK_PERIOD_MS * 50);
                    }

                    // Begin sending speech
                    micOpen.wwStart = aisHandle.micStream.audio.audioData.offset + 16000;
                    micOpen.wwEnd = aisHandle.micStream.audio.audioData.offset + audio_processing_get_wake_word_end();
                    micOpen.initiator = AIS_INITIATOR_WAKEWORD;
                    AIS_EventMicrophoneOpened(&aisHandle, &micOpen);

                    // We are now recoding
                    audio_processing_set_state(kWakeWordDetected);
                }
            }
            else
            {
                ux_attention_set_state(uxDisconnected);
                audio_processing_set_state(kReconnect);
            }

            taskNotification &= ~kWakeWordDetected;
        }

        if (kMicKeepOpen & taskNotification)
        {
            if (reconnection_task_get_state() == kStartState)
            {
                if (!AIS_CheckState(&aisHandle, AIS_TASK_STATE_MICROPHONE))
                {
                    // Set UX attention state
                    ux_attention_set_state(uxListeningStart);

                    // Being Sending speech
                    micOpen.initiator = AIS_INITIATOR_TAP;
                    AIS_EventMicrophoneOpened(&aisHandle, &micOpen);
                }
            }
            else
            {
                ux_attention_set_state(uxDisconnected);
                audio_processing_set_state(kReconnect);
            }

            taskNotification &= ~kMicKeepOpen;
        }

        if (kMicRecording & taskNotification)
        {
            // Set UX attention state and check if mute is active
            // so we don't confuse the user
            if (audio_processing_get_mic_mute())
            {
                ux_attention_set_state(uxMicOntoOff);
            }
            else
            {
                ux_attention_set_state(uxListeningActive);
            }

            taskNotification &= ~kMicRecording;
        }

        if (kMicStop & taskNotification)
        {
            // Set UX attention state
            ux_attention_set_state(uxListeningEnd);
            taskNotification &= ~kMicStop;
        }

        if (kNewAlertSet & taskNotification)
        {
            char token[AIS_MAX_ALERT_TOKEN_LEN_BYTES] = {0};
            if( xQueueReceive( g_alertQueue, token, ( TickType_t ) 5 ) )
            {
                AIS_EventSetAlertSucceeded(&aisHandle, token);
            }

            taskNotification &= ~kNewAlertSet;
        }

        if (kNewAlertFail & taskNotification)
        {
            char token[AIS_MAX_ALERT_TOKEN_LEN_BYTES] = {0};
            if( xQueueReceive( g_alertQueue, token, ( TickType_t ) 5 ) )
            {
                AIS_EventSetAlertFailed(&aisHandle, token);
            }

            taskNotification &= ~kNewAlertFail;
        }

        if (kAlertDelete & taskNotification)
        {
            char token[AIS_MAX_ALERT_TOKEN_LEN_BYTES] = {0};
            if( xQueueReceive( g_alertQueue, token, ( TickType_t ) 5 ) )
            {
                AIS_EventDeleteAlertSucceeded(&aisHandle, token);
            }

            taskNotification &= ~kAlertDelete;
        }

        if (kFailDelete & taskNotification)
        {
            char token[AIS_MAX_ALERT_TOKEN_LEN_BYTES] = {0};
            if( xQueueReceive( g_alertQueue, token, ( TickType_t ) 5 ) )
            {
                AIS_EventDeleteAlertFailed(&aisHandle, token);
            }

            taskNotification &= ~kFailDelete;
        }

        if (kAlertOnlineTrigger & taskNotification)
        {
            char token[AIS_MAX_ALERT_TOKEN_LEN_BYTES] = {0};
            if( xQueueReceive( g_alertQueue, token, ( TickType_t ) 5 ) )
            {
                AIS_EventSetAlertSucceeded(&aisHandle, token);
            }

            taskNotification &= ~kAlertOnlineTrigger;
        }

        if (kVolChanged & taskNotification)
        {
            // Sent VolumeChanged back to service, don't need to send offset
            AIS_EventVolumeChanged(&aisHandle, appData->volume, 0);

            taskNotification &= ~kVolChanged;
        }

        if (kAlertVolChanged & taskNotification)
        {
            // Sent VolumeChanged back to service
            AIS_EventAlertVolumeChanged(&aisHandle, appData->alertVolume);

            taskNotification &= ~kAlertVolChanged;
        }

        if (kExceptionEncountered & taskNotification)
        {
            /* TODO: Create a queue for any exceptions that occur */
            const char * error = AIS_EXCEPTION_DESCRIPTION_STREAMER_MSG;
            AIS_EventExceptionEncountered(&aisHandle, error);

            taskNotification &= ~kExceptionEncountered;
        }

        if (kSyncClock & taskNotification)
        {
            /* reset directive received check timer */
            appData->syncClockReceived = false;
            if (xTimerReset(aisClockSyncCheckHandle, 0) != pdPASS)
            {
                configPRINTF(("xTimerReset failed\r\n"));
            }

            if (kStatus_Success != AIS_EventSynchronizeClock(&aisHandle))
            {
                configPRINTF(("Stopping SynchronizeClock timeout timer...\r\n"));

                if (xTimerStop(aisClockSyncCheckHandle, 0))
                {
                    configPRINTF(("xTimerStop failed\r\n"));
                }
            }

            taskNotification &= ~kSyncClock;
        }

        taskNotification = 0;
    }

    configPRINTF(("Error occurred: exiting\r\n"));
    configPRINTF(("Exiting demo\r\n"));
    vTaskDelete(NULL);
}

void button_callback(int button_nr, int state)
{
    if(button_nr == 1)
    {
        if(state == PRESSED)
        {
            xEventGroupSetBitsFromISR(g_button_pressed, BIT_PRESS_1, &xHigherPriorityTaskWoken);
        }
        else
        {
            xEventGroupSetBitsFromISR(g_button_pressed, BIT_RELEASE_1, &xHigherPriorityTaskWoken);
        }
    }
    else if (button_nr == 2)
    {
        if(state == PRESSED)
        {
            xEventGroupSetBitsFromISR(g_button_pressed, BIT_PRESS_2, &xHigherPriorityTaskWoken);
        }
        else
        {
            xEventGroupSetBitsFromISR(g_button_pressed, BIT_RELEASE_2, &xHigherPriorityTaskWoken);
        }
    }
}

void vProcessAlexaActionButton(ais_app_data_t *appData)
{
    /* Make sure we are offline */
    reconnectState_t currState = reconnection_task_get_state();
    bool alertsToDelete = false;

    /* If the device is offline, process offline alerts */
    if ((kStartState != currState) && (kLinkUp != currState))
    {
        int32_t ret = 0;

        /* Mark all that are not idle as deleted */
        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            if (false == AIS_Alerts_GetIdleState(idx))
            {
                /* Make a note that we should notify the alert task. */
                alertsToDelete = true;

                ret = AIS_Alerts_MarkForDeleteOffline(idx);
                if (0 != ret)
                {
                    configPRINTF(("Unable to mark alert for delete: %d\r\n", ret));
                }
            }
        }

        if (alertsToDelete)
        {
            /* Send request to alert task to delete marked alerts */
            TaskHandle_t alertTask = AIS_Alerts_GetThisHandle();

            if (NULL != alertTask)
            {
                xTaskNotify(alertTask, kAlertDelete, eSetBits);
            }
            else
            {
                configPRINTF(("Alert task handle is NULL.\r\n"));
            }
        }

        ux_attention_states_t currUxState = ux_attention_get_state();

        switch(currUxState)
        {
            case uxTimer:
                ux_attention_set_state(uxTimerEnd);
                break;
            case uxAlarm:
                ux_attention_set_state(uxAlarmEnd);
                break;
            case uxReminder:
                ux_attention_set_state(uxReminderEnd);
                break;
            default:
                break;
        };

        if (NULL != s_offlineAudioEventGroup)
        {
            xEventGroupSetBits(s_offlineAudioEventGroup, OFFLINE_AUDIO_ABORT);
        }
    }
    /* Device is online */
    else
    {
        if (AIS_CheckState(&aisHandle, AIS_TASK_STATE_MICROPHONE))
        {
            AIS_AppCallback_CloseMicrophone(&aisHandle);
            AIS_EventMicrophoneClosed(&aisHandle, aisHandle.micStream.audio.audioData.offset);
        }
        else
        {
            configPRINTF(("Alexa Action Button\r\n"));
            /* If the device is in idle, act as a wakeup/barge-in
             * Need to ensure that the device is not in mute mode.
             * Echo dot doesn't wakeup when in mute mode
             * If its IDLE, DND or NOTIFICATION and NOT MUTE, open Mic
             */
            if (((appData->state == AIS_STATE_IDLE) ||
                 (appData->state == AIS_STATE_DO_NOT_DISTURB) ||
                 (appData->state == AIS_STATE_NOTIFICATION)) &&
                 (audio_processing_get_mic_mute() == kMicMuteModeOff))
            {
                configPRINTF(("Push to talk\r\n"));
                /* Boost CPU now for best performance */
                BOARD_BoostClock();
                /* Send a wakeup but without cloud based verifier */
                if (AIS_CheckState(&aisHandle, AIS_TASK_STATE_SPEAKER))
                {
                    appData->bargein = true;

                    /* Force speaker state update here to close and send event */
                    AIS_StateSpeaker(&aisHandle);
                }
                audio_processing_set_state(kMicKeepOpen);
            }
            /* Else treat it like a stop button which the service will handle */
            else
            {
                /* Anything else we should send button stop */
                AIS_EventButtonCommandIssued(&aisHandle, AIS_BUTTON_CMD_STOP);
            }
        }
    }
}

int32_t SaveVolumeToFlash(ais_app_data_t *appData)
{
    uint32_t len = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;

    int32_t status = SLN_FLASH_MGMT_OK;
    /* When the button one is held and button two is released, this is a volume down change */
    status = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);

    if (SLN_FLASH_MGMT_OK == status)
    {
        /* Check if the volume is different. If it's not, skip and return success */
        if (appData->volume != cfg.streamVolume)
        {
            cfg.streamVolume = appData->volume;
            status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));

            if ((SLN_FLASH_MGMT_EOVERFLOW == status) || (SLN_FLASH_MGMT_EOVERFLOW2 == status))
            {
                SLN_FLASH_MGMT_Erase(DEVICE_CONFIG_FILE_NAME);
                /* Volume Down */
                status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
            }
        }
    }
    else if ((SLN_FLASH_MGMT_ENOENTRY2 == status) || (SLN_FLASH_MGMT_ENOENTRY == status))
    {
        // We should have an empty file so we can save a new one
        cfg.streamVolume = appData->volume;
        status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
    }

    if (SLN_FLASH_MGMT_OK != status)
    {
        configPRINTF(("[AIS App] ERROR: Unable to store stream volume %d to NVM\r\n", appData->volume));
        configPRINTF(("[AIS App] ERROR: NVM operation returned with %d\r\n", status));
    }

    return status;
}

void vProcessVolume(ais_app_data_t *appData, ais_app_volume_direction_t direction)
{
    int32_t status = SLN_FLASH_MGMT_OK;
    reconnectState_t currState = reconnection_task_get_state();

    if (kInitState != currState)
    {
        /* Guard against a current invalid volume which could be after startup */
        if (appData->volume < 0)
        {
            appData->volume = 0;
        }

        appData->volume += direction * 10;
        appData->volume = (appData->volume > 100 ? 100 : appData->volume);
        appData->volume = (appData->volume < 0 ? 0 : appData->volume);

        status = SaveVolumeToFlash(appData);

        if (SLN_FLASH_MGMT_OK == status)
        {
            STREAMER_SetVolume(appData->volume);
            ux_attention_set_state(uxDeviceChange);

            configPRINTF(("Change volume to %d\r\n", appData->volume));

            if ((kStartState == currState) || (kLinkUp == currState))
            {
                AIS_EventVolumeChanged(&aisHandle, appData->volume, appData->speakerOffsetWritten);
            }
        }
        else
        {
            configPRINTF(("Failed to save to flash restoring to previous volume\r\n"));
            appData->volume -= direction * 10;
        }
    }
}

/*
 **
 **      Mute/Unmute:                    SW1 Held and released for 0 - 3 seconds
 **      WiFi Credentials reset:         SW1 Held and released for 5 - 10 seconds
 **      Factory Reset:                  SW1 Held and released more than 10 seconds
 **      Alexa Action:                   SW2 Held and released for 0 - 3 seconds
 **      Volume Up:                      SW2 Held while SW1 is pressed and release (Multi press and release of SW supported)
 **      Volume Down:                    SW1 Held while SW2 is pressed and release (Multi press and release of SW supported)
 */

void button_task(void *arg)
{
    EventBits_t bits;
    ais_app_data_t *appData = AIS_APP_GetAppData();

    volatile uint32_t sec_1 = 0;
    OsaTimeval pressed_time_1 = {0};
    OsaTimeval released_time_1 = {0};

    volatile uint32_t sec_2 = 0;
    OsaTimeval pressed_time_2 = {0};
    OsaTimeval released_time_2 = {0};

    bool buttonHeld1 = FALSE;
    bool buttonHeld2 = FALSE;
    bool buttonProcessed1 = FALSE;
    bool buttonProcessed2 = FALSE;

    while(1)
    {
        bits = xEventGroupWaitBits(g_button_pressed, BIT_PRESS_1 | BIT_RELEASE_1 | BIT_PRESS_2 | BIT_RELEASE_2 , pdTRUE, pdFALSE, portMAX_DELAY);

        if( bits & BIT_PRESS_1 )
        {
            osa_time_get(&pressed_time_1);
            buttonHeld1 = TRUE;
        }

        if( bits & BIT_RELEASE_1 )
        {
            /* If an event was already processed on another button then reset and don't do anything
             * We don't want to process an overloaded function if volume was changed*/
            if (buttonProcessed2)
            {
                buttonHeld2 = FALSE;
                buttonHeld1 = FALSE;
                buttonProcessed1 = FALSE;
                buttonProcessed2 = FALSE;
                continue;
            }
            /* When the button two is held and button one is released, this is a volume up change */
            if (buttonHeld2 == TRUE)
            {
                vProcessVolume(appData, kVolumeUp);
                buttonProcessed1 = TRUE;
                continue;
            }

            osa_time_get(&released_time_1);
            sec_1 = (released_time_1.tv_usec - pressed_time_1.tv_usec) / 1000000;

            if(sec_1 >= 10)
            {
                configPRINTF(("Starting factory reset ...\r\n"));

                RGB_LED_SetColor(LED_COLOR_ORANGE);

                /* Factory reset stuff */
                SLN_FLASH_MGMT_Erase(AIS_REGISTRATION_INFO_FILE_NAME);
                SLN_FLASH_MGMT_Erase(AIS_ALERT_FILE_NAME);
#if USE_WIFI_CONNECTION
                SLN_FLASH_MGMT_Erase(WIFI_CRED_FILE_NAME);
#endif

                /* a bit of delay to let everyone admire the beautiful orange LED */
                osa_time_delay(2000);

                NVIC_SystemReset();
            }
            if((sec_1 <= 3) && (sec_1 >= 0))
            {
                if (kMicMuteModeOff == audio_processing_get_mic_mute())
                {
                    configPRINTF(("Switch to microphone mute mode ...\r\n"));
                    /* ignore error code on purpose, logs printed in the function */
                    audio_processing_set_mic_mute(kMicMuteModeOn, true);
                    ux_attention_set_state(uxMicOntoOff);
                }
                else
                {
                    configPRINTF(("Finish microphone mute mode ...\r\n"));
                    /* ignore error code on purpose, logs printed in the function */
                    audio_processing_set_mic_mute(kMicMuteModeOff, true);
                    /* Set the UX state to the current one */
                    ux_attention_resume_state(appData->state);
                }
            }
#if USE_WIFI_CONNECTION
            /* Perform a wifi credentials reset if SW_1 is pressed between 5 to 10 secs */
            else if((sec_1 >= 5) && (sec_1 < 10))
            {
                configPRINTF(("Reset the wifi credentials and the board\r\n"));
                wifi_credentials_flash_reset();
                vTaskDelay(portTICK_PERIOD_MS * 500);
                NVIC_SystemReset();
            }
            buttonHeld1 = FALSE;
#endif
        }

        if( bits & BIT_PRESS_2 )
        {
            osa_time_get(&pressed_time_2);
            buttonHeld2 = TRUE;
        }

        if( bits & BIT_RELEASE_2 )
        {
            /* If an event was already processed on another button then reset and don't do anything
             * We don't want to process an overloaded function if volume was changed*/
            if (buttonProcessed1)
            {
                buttonHeld2 = FALSE;
                buttonHeld1 = FALSE;
                buttonProcessed1 = FALSE;
                buttonProcessed2 = FALSE;
                continue;
            }
            if (buttonHeld1 == TRUE)
            {
                /* When the button one is held and button two is released, this is a volume Down change */
                vProcessVolume(appData, kVolumeDown);
                buttonProcessed2 = TRUE;
                continue;
            }

            osa_time_get(&released_time_2);
            sec_2 = (released_time_2.tv_usec - pressed_time_2.tv_usec) / 1000000;

            /* This is Alexa Action Button */
            if((sec_2 <= 3) && (sec_2 >= 0))
            {
                vProcessAlexaActionButton(appData);
            }

            buttonHeld2 = FALSE;
        }
    }

    vTaskDelete(NULL);
}

void offline_audio_task(void *arg)
{
    ais_app_data_t *appData = AIS_APP_GetAppData();
    // Play offline alert audio if needed

    EventBits_t offlineAudioEventBits;

    s_offlineAudioEventGroup = xEventGroupCreate();

    if( s_offlineAudioEventGroup != NULL )
    {
        while(1)
        {
            offlineAudioEventBits = xEventGroupWaitBits(s_offlineAudioEventGroup, OFFLINE_AUDIO_EVENT_MASK, pdTRUE, pdFALSE, portMAX_DELAY );

            // Choose what audio to play
            if (OFFLINE_AUDIO_TIMER & offlineAudioEventBits)
            {
                configPRINTF(("Starting offline timer audio...\r\n"));
                /* Need to set the alerting state, put individual assignments as this may change depending on the type of alert
                ** This needs to be set to ensure that the resume UX shows alerting when offline
                */
                appData->state = AIS_STATE_ALERTING;
                SLN_AMP_SetVolume(appData->alertVolume);
                SLN_AMP_WriteLoop((uint8_t *)med_system_alerts_melodic_02_mono_wav, MED_SYSTEM_ALERTS_MELODIC_02_MONO_WAV_LEN);
            }
            else if (OFFLINE_AUDIO_ALARM & offlineAudioEventBits)
            {
                configPRINTF(("Starting offline alarm audio...\r\n"));
                /* Need to set the alerting state, put individual assignments as this may change depending on the type of alert
                ** This needs to be set to ensure that the resume UX shows alerting when offline
                */
                appData->state = AIS_STATE_ALERTING;
                SLN_AMP_SetVolume(appData->alertVolume);
                SLN_AMP_WriteLoop((uint8_t *)med_system_alerts_melodic_01_mono_wav, MED_SYSTEM_ALERTS_MELODIC_01_MONO_WAV_LEN);
            }
            else if (OFFLINE_AUDIO_RMNDR & offlineAudioEventBits)
            {
                configPRINTF(("Starting offline reminder audio...\r\n"));
                /* Need to set the alerting state, put individual assignments as this may change depending on the type of alert
                ** This needs to be set to ensure that the resume UX shows alerting when offline
                */
                appData->state = AIS_STATE_ALERTING;
                SLN_AMP_SetVolume(appData->alertVolume);
                SLN_AMP_WriteLoop((uint8_t *)med_alerts_notification_01_mono_wav, MED_ALERTS_NOTIFICATION_01_MONO_WAV_LEN);
            }
            else if (OFFLINE_AUDIO_ABORT & offlineAudioEventBits)
            {
                configPRINTF(("...offline audio stopped!\r\n"));
                /* Set the state back to idle */
                appData->state = AIS_STATE_IDLE;
                SLN_AMP_SetVolume(0);
                SLN_AMP_AbortWrite();
            }
        }
    }
}

void vApplicationDaemonTaskStartupHook(void)
{

    //start attention task
    ux_attention_task_Init(&xUXAttentionTaskHandle);

    // start blue and cyan boot up
    ux_attention_set_state(uxBootUp);

    /* set wifi/ethernet connect / disconnect handler from here */
    APP_Connect_Update_Handler_Set(&APP_connect_update_handler);

    static demoContext_t mqttDemoContext = {.networkTypes                = DEMO_NETWORK_TYPE,
                                            .demoFunction                = appInit,
                                            .networkConnectedCallback    = NULL,
                                            .networkDisconnectedCallback = NULL};

    xTaskCreate(runDemoTask, "RUN_DEMO_TASK", 2048, &mqttDemoContext, configMAX_PRIORITIES - 1, &appInitTaskHandle);

}

void main(void)
{
    /* Enable additional fault handlers */
    SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | /*SCB_SHCSR_USGFAULTENA_Msk |*/ SCB_SHCSR_MEMFAULTENA_Msk);

    /* Relocate Vector Table */
#if RELOCATE_VECTOR_TABLE
    BOARD_RelocateVectorTableToRam();
#endif

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    //BOARD_ClockConfig_FLASH();

    CRYPTO_InitHardware();

    /* Initialize Flash to allow writing */
    SLN_Flash_Init();

    /* Initialize flash management */
    SLN_FLASH_MGMT_Init((sln_flash_entry_t *)g_fileTable , false);

    // XXX: For Test
    //SLN_FLASH_MGMT_Erase(AIS_ALERT_FILE_NAME);

    /*
     * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
     *                              = 24 * (32 + 77/100)
     *                              = 786.48 MHz
     */
    const clock_audio_pll_config_t audioPllConfig = {
            .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
            .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
            .numerator = 77,    /* 30 bit numerator of fractional loop divider. */
            .denominator = 100, /* 30 bit denominator of fractional loop divider */
    };

    CLOCK_InitAudioPll(&audioPllConfig);

    CLOCK_SetMux(kCLOCK_Sai1Mux, BOARD_PDM_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, BOARD_PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, BOARD_PDM_SAI_CLOCK_SOURCE_DIVIDER);
    CLOCK_EnableClock(kCLOCK_Sai1);

    CLOCK_SetMux(kCLOCK_Sai2Mux, BOARD_PDM_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai2PreDiv, BOARD_PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai2Div, BOARD_PDM_SAI_CLOCK_SOURCE_DIVIDER);
    CLOCK_EnableClock(kCLOCK_Sai2);

    edma_config_t dmaConfig = {0};

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DMA0, &dmaConfig);

    DMAMUX_Init(DMAMUX);

    RGB_LED_Init();

    switchInit();

    sln_shell_init();

    xLoggingTaskInitialize(LOGGING_STACK_SIZE, tskIDLE_PRIORITY + 1, LOGGING_QUEUE_LENGTH);

    xTaskCreate(appTask, "APP_Task", 1024, NULL, configTIMER_TASK_PRIORITY - 1, &appTaskHandle);

    xTaskCreate(sln_shell_task, "Shell_Task", 512, NULL, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(button_task, "Button_Task", 1024, NULL, configMAX_PRIORITIES - 1, NULL);

    xTaskCreate(ota_task, "OTA_Task", 384, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Run RTOS */
    vTaskStartScheduler();

    /* Exit */
    ux_attention_sys_fault();
    AIS_Deinit(&aisHandle);
    APP_MQTT_Disconnect(true);
    APP_NETWORK_Uninit();

    /* Should not reach this statement */
    while (1)
    {
    }
}
