/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Freertos ---------------------------------------------*/
#include "FreeRTOS.h"
#include "queue.h"

/* ----------------------- Variables ----------------------------------------*/
static QueueHandle_t QueueHandler;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    QueueHandler = xQueueCreate( 1, sizeof( eMBEventType ) );
    return QueueHandler != NULL ? 1 : 0;
}
 
BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    portBASE_TYPE xEventSent = pdFALSE;

    xEventSent = xQueueSendFromISR( QueueHandler, &eEvent, &xEventSent );
    return xEventSent == pdTRUE ? 1 : 0;
}
 
BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    BOOL xEventHappened = 0;

    if( xQueueReceive( QueueHandler, eEvent, portMAX_DELAY ) == pdTRUE ) {
        xEventHappened = 1;
    }
    return xEventHappened;
}
