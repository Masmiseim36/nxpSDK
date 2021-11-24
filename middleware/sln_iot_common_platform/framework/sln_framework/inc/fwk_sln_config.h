/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief solution configuration declaration.
 */

#ifndef _FWK_SLN_CONFIG_H_
#define _FWK_SLN_CONFIG_H_

#include "fwk_common.h"

typedef enum _hal_config_status
{
    kSLNConfigStatus_Success     = 0,
    kSLNConfigStatus_Error       = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Config, 1),
    kSLNConfigStatus_VersionSame = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Config, 2),
} hal_config_status_t;

#if defined(__cplusplus)
extern "C" {
#endif

hal_config_status_t FWK_ConfigInit();
hal_config_status_t FWK_ConfigDeinit();

hal_config_status_t FWK_ConfigSetLogLevel(log_level_t logLevel);
log_level_t FWK_ConfigGetLogLevel();

hal_config_status_t FWK_ConfigSetDisplayType(display_type_t displayType);
display_type_t FWK_ConfigGetDisplayType();

display_output_t FWK_ConfigGetDisplayOutput();
hal_config_status_t FWK_ConfigSetDisplayOutput(display_output_t displayOutput);

hal_config_status_t FWK_ConfigSetAppData(void *appData, unsigned int appDataSize, unsigned int appDataVersion);
unsigned int FWK_ConfigGetAppDataVersion();
unsigned int FWK_ConfigGetAppDataSize();
void *FWK_ConfigLockAppData();
void FWK_ConfigUnlockAppData(uint8_t save);

#if defined(__cplusplus)
}
#endif

#endif /* _FWK_SLN_CONFIG_H_ */
