/*
 *
 * Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* This file contains configuration settings for the demos. */

#ifndef _COMMON_CLOUD_CONFIG_H_
#define _COMMON_CLOUD_CONFIG_H_

/**
 * @brief The maximum number of retries for connecting to server.
 */
#define CONNECTION_RETRY_MAX_ATTEMPTS                ( 5U )

/**
 * @brief The maximum back-off delay (in milliseconds) for retrying connection to server.
 */
#define CONNECTION_RETRY_MAX_BACKOFF_DELAY_MS        ( 5000U )

/**
 * @brief The base back-off delay (in milliseconds) to use for connection retry attempts.
 */
#define CONNECTION_RETRY_BACKOFF_BASE_MS             ( 500U )

/**
 * @brief The maximum number of retries for subscribing to topic filter when broker rejects an attempt.
 */
#define SUBSCRIBE_RETRY_MAX_ATTEMPTS                 ( 4U )

/**
 * @brief The maximum back-off delay (in milliseconds) for retrying subscription to topic.
 */
#define SUBSCRIBE_RETRY_MAX_BACKOFF_DELAY_MS         ( 2500U )

/**
 * @brief The base back-off delay (in milliseconds) to use for subscription retry attempts.
 */
#define SUBSCRIBE_RETRY_BACKOFF_BASE_MS              ( 500U )

/**
 * @brief Timeout for receiving CONNACK packet in milliseconds.
 */
#define mqttexampleCONNACK_RECV_TIMEOUT_MS           ( 1000U )

/**
 * @brief The number of topic filters to subscribe.
 */
#define mqttexampleTOPIC_COUNT                       ( 1 )

/**
 * @brief Time to wait between each cycle of the demo implemented by prvMQTTDemoTask().
 */
#define mqttexampleDELAY_BETWEEN_DEMO_ITERATIONS     ( pdMS_TO_TICKS( 5000U ) )

/**
 * @brief Timeout for MQTT_ProcessLoop in milliseconds.
 */
#define mqttexamplePROCESS_LOOP_TIMEOUT_MS           ( 500U )

/**
 * @brief The maximum number of times to call MQTT_ProcessLoop() when polling
 * for a specific packet from the broker.
 */
#define MQTT_PROCESS_LOOP_PACKET_WAIT_COUNT_MAX      ( 30U )

/**
 * @brief Keep alive time reported to the broker while establishing an MQTT connection.
 *
 * It is the responsibility of the Client to ensure that the interval between
 * Control Packets being sent does not exceed the this Keep Alive value. In the
 * absence of sending any other Control Packets, the Client MUST send a
 * PINGREQ Packet.
 */
#define mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS        ( 60U )

/**
 * @brief Delay between MQTT publishes. Note that the process loop also has a
 * timeout, so the total time between publishes is the sum of the two delays.
 */
#define mqttexampleDELAY_BETWEEN_PUBLISHES           ( pdMS_TO_TICKS( 500U ) )

/**
 * @brief Transport timeout in milliseconds for transport send and receive.
 */
#define mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS    ( 750U )

/**
 * @brief Maximum number of outgoing publishes maintained in the application
 * until an ack is received from the broker.
 */
#define MAX_OUTGOING_PUBLISHES                       ( 1U )

/**
 * @brief Milliseconds per second.
 */
#define MILLISECONDS_PER_SECOND                      ( 1000U )

/**
 * @brief Milliseconds per FreeRTOS tick.
 */
#define MILLISECONDS_PER_TICK                        ( MILLISECONDS_PER_SECOND / configTICK_RATE_HZ )

/**
 * @brief Network buffer size.
 */
#define democonfigNETWORK_BUFFER_SIZE    ( 1024U )

/**
 * @brief Length (in byte) of unique identifier contained in secure module
 */
#define MODULE_UNIQUE_ID_LEN    ( 18U )

#endif /* _COMMON_CLOUD_CONFIG_H_ */