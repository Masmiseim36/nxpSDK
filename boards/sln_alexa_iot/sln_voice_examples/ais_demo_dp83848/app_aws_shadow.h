/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/** @file app_aws_shadow.h
 *  @brief This file handles the alerts
 *
 *  This file handles the shadow connection and interaction
 */

#ifndef _APP_AWS_SHADOW_H_
#define _APP_AWS_SHADOW_H_

#include <limits.h>
#include <stdint.h>

#include "cJSON.h"
#include "iot_mqtt_agent.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/*!
 * @brief Updates the shadow document to ensure the cloud is in sync with the current state
 *        This function takes care of the shadow wrapping adding the state and reported json structure
 *        Note: This function deletes the cJSON payload passed in
 *
 * @params payload The payload of the shadow document
 *
 * @returns Status of initialization
 */
BaseType_t APP_AWS_ShadowUpdate(cJSON *payload);

/*!
 * @brief Process the updated shadow document; call by UpdateCallback
 *
 * @params pcThingName The name of the AWS IoT device "Thing" name
 * @params pcUpdateDocument The shadow document that have been updated
 * @params ulDocumentLength The length of the shadow document
 *
 * @returns Returns the status of the shadow update
 */
BaseType_t ProcessUpdatedShadowDocument(const char *const pcThingName,
                                        const char *const pcUpdateDocument,
                                        uint32_t ulDocumentLength);

#if defined(__cplusplus)
}
#endif

#endif /* _APP_AWS_SHADOW_H_ */
