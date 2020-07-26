/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* Platform includes */
#include "platform_imxrt.h"

/* Bluetooth includes */
#include "ble_connection.h"
#include "ble_server.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "event_groups.h"

/* Json includes */
#include "cJSON.h"

/* Network connection includes */
#include "network_connection.h"

/* Device specific includes */
#include "device_utils.h"

/* Wifi includes */
#include "wifi_credentials.h"

#if USE_WIFI_CONNECTION

extern EventGroupHandle_t ble_cred_received_event;

void BleProvisioning(void)
{
    cJSON *parsed_message   = NULL;
    const cJSON *json_item1 = NULL;
    const cJSON *json_item2 = NULL;

    char wifi_characteristic_str[COMMISSIONING_CHAR_WIFI_VAL_SIZE_B + 1];
    status_t credsave_status = 1;
    wifi_cred_t wifi_cred    = {0};

    char *serialNumber = NULL;

    /* Serial number on the box has the padding "=" character" */
    APP_GetUniqueID(&serialNumber, false);

    configPRINTF(("BLE: Provisioning job started (ble device name '%s')\r\n", serialNumber));
    ble_pwr_on();
    ble_server_start(serialNumber);

    while (1)
    {
        configPRINTF(("BLE: Wait for wifi characteristic\r\n"));
        xEventGroupWaitBits(ble_cred_received_event, 1, pdTRUE, pdFALSE, portMAX_DELAY);

        ble_server_get_char_wifi_str(wifi_characteristic_str);
        configPRINTF(("BLE: Wifi characteristic value received\r\n", wifi_characteristic_str));

        if (parsed_message != NULL)
        {
            cJSON_Delete(parsed_message);
        }
        parsed_message = cJSON_Parse(wifi_characteristic_str);
        if (parsed_message == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                configPRINTF(("BLE: Error at cJSON_Parse: %s\n", error_ptr));
            }
            continue;
        }

        /* Check if wifi characteristic was received */
        if (cJSON_GetArraySize(parsed_message) == 2)
        {
            /* Expected Json {"SSID":"<ssid>","Password":"<password>"} */
            json_item1 = cJSON_GetObjectItemCaseSensitive(parsed_message, "SSID");
            json_item2 = cJSON_GetObjectItemCaseSensitive(parsed_message, "Password");
            if (cJSON_IsString(json_item1) && (json_item1->valuestring != NULL) && cJSON_IsString(json_item2) &&
                (json_item2->valuestring != NULL))
            {
                configPRINTF(("BLE: Wifi credentials received: SSID '%s', Password '%s'\r\n", json_item1->valuestring,
                              json_item2->valuestring));

                wifi_cred.ssid.length     = strlen(json_item1->valuestring);
                wifi_cred.password.length = strlen(json_item2->valuestring);

                if (wifi_cred.ssid.length <= sizeof(wifi_cred.ssid.value))
                {
                    memcpy(wifi_cred.ssid.value, json_item1->valuestring, wifi_cred.ssid.length);
                }
                else
                {
                    configPRINTF(("BLE: Cannot get wifi SSID\r\n"));
                    continue;
                }

                if (wifi_cred.password.length <= sizeof(wifi_cred.password.value))
                {
                    memcpy(wifi_cred.password.value, json_item2->valuestring, wifi_cred.password.length);
                }
                else
                {
                    configPRINTF(("BLE: Cannot get wifi Password\r\n"));
                    continue;
                }

                credsave_status = wifi_credentials_flash_set(&wifi_cred);
                if (!credsave_status)
                {
                    configPRINTF(("BLE: Wifi credentials saved in flash\r\n"));
                    break;
                }
                else
                {
                    configPRINTF(
                        ("BLE: Failed to write wifi credentials in flash, error code %d\r\n", credsave_status));
                    continue;
                }
            }
            else
            {
                configPRINTF(("BLE: Wrong wifi characteristic Json size"));
            }
        }
        else
        {
            configPRINTF(("BLE: Wrong wifi characteristic Json parameters"));
        }
    }

    if (parsed_message != NULL)
    {
        cJSON_Delete(parsed_message);
    }
    ble_server_stop();
    vPortFree(serialNumber);
    ble_pwr_off();

    configPRINTF(("BLE: Provisioning job stopped\r\n"));
}

#endif
