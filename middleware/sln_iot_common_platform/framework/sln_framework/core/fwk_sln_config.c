/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief solution configuration implementation.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"

#include "fwk_flash.h"
#include "fwk_sln_config.h"

#define FWK_MINOR_VERSION 0x01
#define FWK_MAJOR_VERSION 0x0
#define VERSION           (((FWK_MAJOR_VERSION << 16) & 0xFF00) | (FWK_MINOR_VERSION & 0xFF))

#define CONFIG_DIR "cfg"
#define METADATA_FILE_NAME \
    CONFIG_DIR             \
    "/"                    \
    "Metadata"
#define FWK_CONFIG_FILE_NAME \
    CONFIG_DIR               \
    "/"                      \
    "fwk_cfg"
#define APP_CONFIG_FILE_NAME \
    CONFIG_DIR               \
    "/"                      \
    "app_cfg"

typedef struct _fwk_metadata
{
    unsigned int fwkDataVersion;
    unsigned int fwkDataSize;
    unsigned int appDataVersion;
    unsigned int appDataSize;
} fwk_metadata_t;

typedef struct _fwk_config
{
    log_level_t logLevel;
    display_type_t displayType;
    display_output_t displayOutput;
} fwk_config_t;

typedef struct _app_config
{
    void *appData;
    unsigned int appDataVersion;
    unsigned int appDataSize;
} app_config_t;

static fwk_config_t s_FWKConfig = {
    .logLevel      = kLOGLevel_Debug,
    .displayType   = kDisplayType_RGB,
    .displayOutput = kDisplayOutput_Panel,
};

static app_config_t s_AppConfig = {
    .appData        = NULL,
    .appDataVersion = 0,
    .appDataSize    = 0,
};

static SemaphoreHandle_t s_FWKConfigLock;
static fwk_metadata_t s_Metadata;

static int32_t _FWK_ConfigSanityCheck(fwk_config_t *cfg)
{
    return 0;
}

static hal_config_status_t _FWK_ConfigLock()
{
    if ((s_FWKConfigLock == NULL) && (pdTRUE != xSemaphoreTake(s_FWKConfigLock, portMAX_DELAY)))
    {
        return kSLNConfigStatus_Error;
    }

    return kSLNConfigStatus_Success;
}

static void _FWK_ConfigUnlock()
{
    if (s_FWKConfigLock != NULL)
    {
        xSemaphoreGive(s_FWKConfigLock);
    }
}

static sln_flash_status_t _SLN_AppConfigSave()
{
    sln_flash_status_t status;
    status = FWK_Flash_Save(APP_CONFIG_FILE_NAME, s_AppConfig.appData, s_AppConfig.appDataSize);
    return status;
}

static sln_flash_status_t _SLN_AppConfigLoad()
{
    sln_flash_status_t status;
    memset(s_AppConfig.appData, 0, s_AppConfig.appDataSize);
    status = FWK_Flash_Read(APP_CONFIG_FILE_NAME, s_AppConfig.appData, s_AppConfig.appDataSize);
    return status;
}

static sln_flash_status_t _SLN_MetadataSave()
{
    sln_flash_status_t status;
    status = FWK_Flash_Save(METADATA_FILE_NAME, &s_Metadata, sizeof(fwk_metadata_t));
    return status;
}

static sln_flash_status_t _SLN_MetadataLoad()
{
    sln_flash_status_t status;
    memset(&s_Metadata, 0, sizeof(fwk_metadata_t));
    status = FWK_Flash_Read(METADATA_FILE_NAME, &s_Metadata, sizeof(fwk_metadata_t));
    return status;
}

static sln_flash_status_t _SLN_FwkConfigSave()
{
    sln_flash_status_t status;
    status = FWK_Flash_Save(FWK_CONFIG_FILE_NAME, &s_FWKConfig, sizeof(fwk_config_t));
    return status;
}

static int _SLN_FwkConfigLoad()
{
    sln_flash_status_t status;
    memset(&s_FWKConfig, 0, sizeof(s_FWKConfig));
    status = FWK_Flash_Read(FWK_CONFIG_FILE_NAME, &s_FWKConfig, sizeof(fwk_config_t));
    return status;
}

hal_config_status_t _FWK_ConfigInit()
{
    /* First time it boots check if there is this file  */
    sln_flash_status_t status;
    status = FWK_Flash_Mkdir(CONFIG_DIR);
    if (status == kStatus_HAL_FlashDirExist)
    {
        status = _SLN_MetadataLoad();
        if (status == kStatus_HAL_FlashSuccess)
        {
            if (s_Metadata.fwkDataVersion != VERSION)
            {
                /* Different version erase  */
                /* Todo Decision should be taken */

                status = _SLN_FwkConfigSave();
                if (status == kStatus_HAL_FlashSuccess)
                {
                    s_Metadata.fwkDataVersion = VERSION;
                    s_Metadata.fwkDataSize    = sizeof(fwk_config_t);
                    status                    = _SLN_MetadataSave();
                }
            }
            else
            {
                /* Same version */
                status = _SLN_FwkConfigLoad();
            }

            if (s_Metadata.appDataVersion == 0)
            {
                /* App config has not been updated do nothing*/
            }
            else
            {
                /* Load appData in RAM */
                s_AppConfig.appData = (void *)SLN_MALLOC(s_Metadata.appDataSize);
                if (NULL == s_AppConfig.appData)
                {
                    LOGE("Could not allocate memory for app data ");
                    status = kSLNConfigStatus_Error;
                }
                else
                {
                    s_AppConfig.appDataSize    = s_Metadata.appDataSize;
                    s_AppConfig.appDataVersion = s_Metadata.appDataVersion;
                    status                     = _SLN_AppConfigLoad();
                }
            }
        }
        else
        {
            LOGE("Could not read metadata ");
        }
    }
    else if (status == kStatus_HAL_FlashSuccess)
    {
        /* First time write default, No appData to be written */
        status = _SLN_FwkConfigSave();
        if (status == kStatus_HAL_FlashSuccess)
        {
            s_Metadata.appDataSize    = 0;
            s_Metadata.appDataVersion = 0;
            s_Metadata.fwkDataVersion = VERSION;
            s_Metadata.fwkDataSize    = sizeof(fwk_config_t);
            status                    = _SLN_MetadataSave();
        }
    }
    else
    {
        LOGE("Failed to init the config file");
    }

    return status;
}

hal_config_status_t FWK_ConfigInit()
{
    hal_config_status_t ret = kSLNConfigStatus_Success;

    if (NULL == s_FWKConfigLock)
    {
        s_FWKConfigLock = xSemaphoreCreateMutex();

        if (NULL == s_FWKConfigLock)
        {
            LOGE("Create SLN Config lock semaphore");
            return kSLNConfigStatus_Error;
        }

        ret = _FWK_ConfigLock();
        if (ret == kSLNConfigStatus_Success)
        {
            ret = _FWK_ConfigInit();
            _FWK_ConfigUnlock();
        }
    }

    return ret;
}

hal_config_status_t FWK_ConfigDeinit()
{
    hal_config_status_t ret = kSLNConfigStatus_Success;

    return ret;
}

log_level_t FWK_ConfigGetLogLevel()
{
    return s_FWKConfig.logLevel;
}

hal_config_status_t FWK_ConfigSetLogLevel(log_level_t logLevel)
{
    hal_config_status_t ret = kSLNConfigStatus_Error;

    if ((logLevel >= kLOGLevel_None) && (logLevel < kLOGLevel_Invalid))
    {
        ret = _FWK_ConfigLock();

        if (ret == kSLNConfigStatus_Success)
        {
            s_FWKConfig.logLevel = logLevel;
            ret                  = _SLN_FwkConfigSave();
            _FWK_ConfigUnlock();
        }
    }
    else
    {
        LOGE("Invalid log level");
    }

    return ret;
}

display_type_t FWK_ConfigGetDisplayType()
{
    display_type_t displayType = kDisplayType_Invalid;

    hal_config_status_t ret = _FWK_ConfigLock();

    if (ret == kSLNConfigStatus_Success)
    {
        displayType = s_FWKConfig.displayType;
        _FWK_ConfigUnlock();
    }

    return displayType;
}

hal_config_status_t FWK_ConfigSetDisplayType(display_type_t displayType)
{
    hal_config_status_t ret = kSLNConfigStatus_Error;

    if ((displayType >= kDisplayType_RGB) && (displayType < kDisplayType_Invalid))
    {
        hal_config_status_t ret = _FWK_ConfigLock();

        if (ret == kSLNConfigStatus_Success)
        {
            s_FWKConfig.displayType = displayType;
            ret                     = _SLN_FwkConfigSave();
            _FWK_ConfigUnlock();
        }
    }
    else
    {
        LOGE("Invalid display type");
    }

    return ret;
}

display_output_t FWK_ConfigGetDisplayOutput()
{
    display_output_t displayOutput = kDisplayOutput_Invalid;

    hal_config_status_t ret = _FWK_ConfigLock();

    if (ret == kSLNConfigStatus_Success)
    {
        displayOutput = s_FWKConfig.displayOutput;
        _FWK_ConfigUnlock();
    }

    return displayOutput;
}

hal_config_status_t FWK_ConfigSetDisplayOutput(display_output_t displayOutput)
{
    hal_config_status_t ret = kSLNConfigStatus_Error;

    if ((displayOutput >= kDisplayOutput_Panel) && (displayOutput < kDisplayOutput_Invalid))
    {
        ret = _FWK_ConfigLock();

        if (ret == kSLNConfigStatus_Success)
        {
            s_FWKConfig.displayOutput = displayOutput;
            ret                       = _SLN_FwkConfigSave();
            _FWK_ConfigUnlock();
        }
    }
    else
    {
        LOGE("Invalid display output");
    }

    return ret;
}

hal_config_status_t FWK_ConfigSetAppData(void *appData, unsigned int appDataSize, unsigned int appDataVersion)
{
    hal_config_status_t ret = _FWK_ConfigLock();

    if (ret == kSLNConfigStatus_Success)
    {
        if (appDataSize != s_AppConfig.appDataSize)
        {
            if (s_AppConfig.appData)
            {
                SLN_FREE(s_AppConfig.appData);
            }

            s_AppConfig.appData = (void *)SLN_MALLOC(appDataSize);
        }

        if (NULL == s_AppConfig.appData)
        {
            LOGE("Could not allocate memory for app data");
            ret = kSLNConfigStatus_Error;
        }
        else
        {
            s_AppConfig.appDataVersion = appDataVersion;
            s_AppConfig.appDataSize    = appDataSize;
            memcpy(s_AppConfig.appData, appData, appDataSize);
            ret = _SLN_AppConfigSave();
        }

        if (ret == kSLNConfigStatus_Success)
        {
            s_Metadata.appDataVersion = appDataVersion;
            s_Metadata.appDataSize    = appDataSize;

            /* Update metadata  */
            ret = _SLN_MetadataSave();
        }
    }

    _FWK_ConfigUnlock();
    return ret;
}

unsigned int FWK_ConfigGetAppDataVersion()
{
    return s_AppConfig.appDataVersion;
}

unsigned int FWK_ConfigGetAppDataSize()
{
    return s_AppConfig.appDataSize;
}

void *FWK_ConfigLockAppData()
{
    void *appData = NULL;

    hal_config_status_t ret = _FWK_ConfigLock();

    if (ret == kSLNConfigStatus_Success)
    {
        appData = s_AppConfig.appData;
    }

    return appData;
}

void FWK_ConfigUnlockAppData(uint8_t save)
{
    if (save)
    {
        _SLN_AppConfigSave();
    }

    _FWK_ConfigUnlock();
}
