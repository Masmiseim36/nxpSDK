/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef SLN_OTA_H_
#define SLN_OTA_H_

/* Board includes */
#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

/* Crypto includes */
#include "ksdk_mbedtls.h"
#include "mbedtls/base64.h"
#include "sln_auth.h"
#include "nor_encrypt_bee.h"

#include "aws_demo.h"
#include "iot_network_types.h"

/* AWS includes */
#include AWS_SYS_INIT_INCLUDE
#include AWS_LOGGING_INCLUDE
#include AWS_MQTT_AGENT_INCLUDE
#include "aws_clientcredential.h"
#include "aws_iot_ota_agent.h"
#include "aws_application_version.h"
#include "aws_iot_ota_pal.h"
#include "aws_demo.h"
#include "iot_network_types.h"

/* USB includes */
#include "serial_manager.h"
#include "usb_device_config.h"
#include "usb_phy.h"
#include "usb.h"

/* Networking includes */
#include "network_connection.h"
#if USE_WIFI_CONNECTION
#include "wifi_credentials.h"
#endif

/* Flash includes */
#include "sln_flash.h"
#include "sln_flash_mgmt.h"
#include "flash_ica_driver.h"

#include "bootloader.h"
#include "limits.h"

/*!
 * @brief Task which initializes OTA agent
 *
 * @param arg optional argument to be given to this FreeRTOS task
 */
int otaAppInitTask(bool awsIotMqttMode,
                   const char *pIdentifier,
                   void *pNetworkServerInfo,
                   void *pNetworkCredentialInfo,
                   const IotNetworkInterface_t *pNetworkInterface);

/*!
 * @brief Shares otaDoneTask handle with OTA PAL layer, for inter-task communication
 *
 * @param handle FreeRTOS task handle of otaDoneTask
 */
void otaPalOtaDoneTaskSet(TaskHandle_t handle);

/*!
 * @brief For negative OTA cases, this function resets NAP bit
 *        and restores PRDB context (if eXIP is used)
 */
void selfTestCleanup(void);

void Run_OTA_Task(void);

#endif /* SLN_OTA_H_ */
