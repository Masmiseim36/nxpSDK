/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "common_connection_handler.h"
#include "comms_connection_handler_cfg.h"
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTA_TCP
#include "tcp_connection_handler_private.h"
#endif
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTW_UART
#include "uart_connection_handler_private.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

sln_common_connection_message_status SLN_CommonConnectionHandlerInit(sln_common_connection_desc *desc)
{
    sln_common_connection_message_status status = kCommon_Success;

    if (NULL == desc->recv_cb)
    {
        status = kCommon_Failed;
    }

    if (kCommon_Success == status)
    {
        switch (desc->connType)
        {
            case kCommonInterfaceTcp:
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTA_TCP
                status = SLN_TCP_COMMS_Init(desc);
#else
                status = kCommon_Failed;
#endif
                break;
            case kCommonInterfaceUart:
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTW_UART
                /* Configure the UART connection */
                desc->context.sUartContext.portbase         = FWUPDATE_OTW_UART_BASEADDR;
                desc->context.sUartContext.speed_bps        = FWUPDATE_OTW_UART_BAUDRATE;
                desc->context.sUartContext.clksrc_hz        = FWUPDATE_OTW_UART_SRCFREQ;
                desc->context.sUartContext.max_rx_buff_size = OTW_MAX_BUFFER_SIZE;

                status = SLN_UART_COMMS_Init(desc);
#else
                status = kCommon_Failed;
#endif
                break;
            default:
                status = kCommon_Failed;
        }
    }

    return status;
}

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
