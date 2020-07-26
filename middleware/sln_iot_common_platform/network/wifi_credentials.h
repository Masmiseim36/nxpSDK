/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef WIFI_CREDENTIALS_H_
#define WIFI_CREDENTIALS_H_

#include "network_connection.h"

#if USE_WIFI_CONNECTION

#include "flexspi_hyper_flash_ops.h"
#include "fsl_common.h"
#include "sln_flash.h"
#include "stdint.h"

#define WIFI_CRED_FILE_NAME "wifi.dat"
#define WIFI_CRED_FILE_ADDR (0x1C80000)

/**
 * @brief Reads wifi credentials from flash memory
 *
 * @param cred[out]       Pointer to a pre-allocated wifi_cred_t structure where the credentials will be stored
 * @return                0 on success, 1 otherwise
 */
status_t wifi_credentials_flash_get(wifi_cred_t *cred);

/**
 * @brief Writes wifi credentials in flash memory
 *
 * @param cred[in]        Pointer to a wifi_cred_t structure containing the data that will be written
 * @return                0 on success, 1 otherwise
 */
status_t wifi_credentials_flash_set(wifi_cred_t *cred);

/**
 * @brief Resets wifi credentials stored in flash memory
 *
 * @return                0 on success, 1 otherwise
 */
status_t wifi_credentials_flash_reset(void);

uint8_t check_valid_credentials(wifi_cred_t *wifi_cred);

#endif /* USE_WIFI_CONNECTION */

#endif /* WIFI_CREDENTIALS_H_ */
