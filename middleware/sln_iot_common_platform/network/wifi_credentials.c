/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "wifi_credentials.h"

#include "sln_flash_mgmt.h"

#if USE_WIFI_CONNECTION

status_t wifi_credentials_flash_get(wifi_cred_t *wifi_cred)
{
    if (!wifi_cred)
    {
        return 1;
    }

    uint32_t credLen = sizeof(wifi_cred_t);
    SLN_FLASH_MGMT_Read(WIFI_CRED_FILE_NAME, (uint8_t *)wifi_cred, &credLen);

    return 0;
}

status_t wifi_credentials_flash_set(wifi_cred_t *wifi_cred)
{
    int32_t status = 0;

    status = SLN_FLASH_MGMT_Save(WIFI_CRED_FILE_NAME, (uint8_t *)wifi_cred, sizeof(wifi_cred_t));

    if (status == SLN_FLASH_MGMT_EOVERFLOW || status == SLN_FLASH_MGMT_EOVERFLOW2)
    {
        status = SLN_FLASH_MGMT_Erase(WIFI_CRED_FILE_NAME);
        status = SLN_FLASH_MGMT_Save(WIFI_CRED_FILE_NAME, (uint8_t *)wifi_cred, sizeof(wifi_cred_t));
    }

    return status;
}

status_t wifi_credentials_flash_reset()
{
    status_t status = 0;

    uint8_t buf[sizeof(wifi_cred_t)] = {0};

    status = SLN_FLASH_MGMT_Erase(WIFI_CRED_FILE_NAME);

    status = SLN_FLASH_MGMT_Save(WIFI_CRED_FILE_NAME, buf, sizeof(wifi_cred_t));

    return status;
}

uint8_t check_valid_credentials(wifi_cred_t *wifi_cred)
{
    if (!wifi_cred)
    {
        return 1;
    }

    if (wifi_cred->ssid.length == 0)
    {
        return 1;
    }

    for (uint8_t idx = 0; idx < wifi_cred->ssid.length; idx++)
    {
        if ((wifi_cred->ssid.value[idx] == 0x00) || (wifi_cred->ssid.value[idx] == 0xFF))
        {
            return 1;
        }
    }

    if (strlen((char *)(wifi_cred->ssid.value)) == 0)
    {
        return 1;
    }

    return 0;
}

#endif /* USE_WIFI_CONNECTION */
