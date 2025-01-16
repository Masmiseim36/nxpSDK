/*
 * FreeRTOS V202111.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 */
#ifndef CORE_MQTT_CONFIG_H
#define CORE_MQTT_CONFIG_H

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros depending on
 * the logging configuration for MQTT.
 * 3. Include the header file "logging_stack.h", if logging is enabled for MQTT.
 */

#include "logging_levels.h"

/* Logging configuration for the MQTT library. */
#ifndef LIBRARY_LOG_NAME
#define LIBRARY_LOG_NAME "MQTT"
#endif

#ifndef LIBRARY_LOG_LEVEL
#define LIBRARY_LOG_LEVEL LOG_ERROR
#endif

#include "logging.h"
/************ End of logging configuration ****************/

/**
 * @brief The maximum number of MQTT PUBLISH messages that may be pending
 * acknowledgement at any time.
 *
 * QoS 1 and 2 MQTT PUBLISHes require acknowledgment from the server before
 * they can be completed. While they are awaiting the acknowledgment, the
 * client must maintain information about their state. The value of this
 * macro sets the limit on how many simultaneous PUBLISH states an MQTT
 * context maintains.
 */
#define MQTT_STATE_ARRAY_MAX_COUNT 10U

#define MQTT_AGENT_COMMAND_QUEUE_LENGTH (25)

#define MQTT_COMMAND_CONTEXTS_POOL_SIZE (10)

/**
 * @brief The maximum number of subscriptions to track for a single connection.
 *
 * @note The MQTT agent keeps a record of all existing MQTT subscriptions.
 * MQTT_AGENT_MAX_SIMULTANEOUS_SUBSCRIPTIONS sets the maximum number of
 * subscriptions records that can be maintained at one time.  The higher this
 * number is the greater the agent's RAM consumption will be.
 */
#define MQTT_AGENT_MAX_SIMULTANEOUS_SUBSCRIPTIONS (10)

/**
 * @brief Size of statically allocated buffers for holding subscription filters.
 *
 * @note Subscription filters are strings such as "/my/topicname/#".  These
 * strings are limited to a maximum of MQTT_AGENT_MAX_SUBSCRIPTION_FILTER_LENGTH
 * characters. The higher this number is the greater the agent's RAM consumption
 * will be.
 */
#define MQTT_AGENT_MAX_SUBSCRIPTION_FILTER_LENGTH (100)

/**
 * @brief Dimensions the buffer used to serialize and deserialize MQTT packets.
 * @note Specified in bytes.  Must be large enough to hold the maximum
 * anticipated MQTT payload.
 */
#define MQTT_AGENT_NETWORK_BUFFER_SIZE (5000)

/**
 * @brief This is the timeout for the duration on which no data is received
 * from the transport interface, after at least a single byte of the MQTT packet has
 * been read.
 *
 * Since the transport interface is set to non-blocking, this is set to a value large enough
 * of a delay for a chunk of packet to be sent over the connection.
 */
#define MQTT_RECV_POLLING_TIMEOUT_MS (500)

/**
 * @brief Maximum wait time in milliseconds for MQTT agent on the input queue.
 * If there is no events(commands) on the queue for the period, then MQTT agent
 * switches to receive packet from network.
 *
 * Since the transport interface is non blocking, the queue wait time is set to
 * a interval such that agent does not polls busy on the socket.
 *
 */
#define MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME (50)

#endif /* ifndef CORE_MQTT_CONFIG_H */