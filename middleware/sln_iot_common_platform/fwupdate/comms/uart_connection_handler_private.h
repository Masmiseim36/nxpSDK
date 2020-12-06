/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file uart_connection_handler_private.h
 * @brief This file declares the private functions
 */

#ifndef UART_CONNECTION_HANDLER_PRIVATE_H_
#define UART_CONNECTION_HANDLER_PRIVATE_H_

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

#include "FreeRTOS.h"
#include "common_connection_handler.h"

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
sln_common_connection_message_status SLN_UART_COMMS_Init(sln_common_connection_desc *descriptor);

#endif /* UART_CONNECTION_HANDLER_PRIVATE_H_ */
