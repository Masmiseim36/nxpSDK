/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef MQTT_CONNECTION_H_
#define MQTT_CONNECTION_H_

#include "task.h"

/**
 * @brief This function obtains the MQTT v2 connection handler
 *
 * @return MQTTAgentHandle_t the MQTT v2 handler
 */
MQTTAgentHandle_t APP_MQTT_Getv2Handle();

/**
 * @brief This function returns the global MQTT handler
 *
 * @return MQTTAgentHandle_t the MQTT handler
 */
MQTTAgentHandle_t APP_MQTT_GetHandle();

/**
 * @brief This function initializes the MQTT handler and connects to the broker
 *
 * @param callback Handler to treat various connection events
 *
 * @return A MQTTAgentReturnCode_t status indicating the if the operation was successful
 */
MQTTAgentReturnCode_t APP_MQTT_Connect(MQTTAgentCallback_t callback);

/**
 * @brief This function gracefully disconnects from the MQTT broker and clears the handler
 *
 * @param sendDisconnect Flag for sending the MQTT disconnect message to broker
 */
void APP_MQTT_Disconnect(bool sendDisconnect);

#endif /* MQTT_CONNECTION_H_ */
