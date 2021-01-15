
/**
 *  \file hci_uart_internal.h
 *
 *  This files containes the definitins used by the HCI UART Transport Layer
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_HCI_UART_INTERNAL_
#define _H_HCI_UART_INTERNAL_

/* ---------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "hci_transport.h"
#include "hci_uart.h"

#ifdef BT_BCSP
    #include "bcsp.h"
#endif /* BT_BCSP */


/* ---------------------------------- HCI UART Debug */
#ifdef HCI_UART_DEBUG

#define HCI_UART_TRC(...)               BT_debug_trace(0, __VA_ARGS__)
#define HCI_UART_INF(...)               BT_debug_info(0, __VA_ARGS__)

#else /* HCI_UART_DEBUG */

#define HCI_UART_TRC                    BT_debug_null
#define HCI_UART_INF                    BT_debug_null

#endif /* HCI_UART_DEBUG */

#ifndef HCI_UART_NO_DEBUG
#define HCI_UART_ERR(...)               BT_debug_error(0, __VA_ARGS__)
#else /* HCI_UART_NO_DEBUG */
#define HCI_UART_ERR                    BT_debug_null
#endif /* HCI_UART_NO_DEBUG */

/* ---------------------------------- Global Definitions */
#define hci_uart_lock(mutex)            BT_thread_mutex_lock(&mutex)
#define hci_uart_unlock(mutex)          BT_thread_mutex_unlock(&mutex)

#define hci_uart_signal(cond)           BT_thread_cond_signal(&cond)
#define hci_uart_wait(cond, mutex)      BT_thread_cond_wait(&cond,&mutex)


/* ---------------------------------- Internal Functions */


#endif /* _H_HCI_UART_INTERNAL_ */

