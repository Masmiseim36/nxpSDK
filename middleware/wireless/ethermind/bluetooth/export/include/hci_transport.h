
/**
 *  \file hci_transport.h
 *
 *  Module Header File for HCI Transport module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_HCI_TRANSPORT_
#define _H_HCI_TRANSPORT_

/* -----------------------------------------------  Header File Inclusion */
#include "BT_common.h"

#ifndef HCI_TX_RUN_TIME_SELECTION

#if defined BT_UART && !defined BT_BCSP

    #include "hci_uart.h"

#elif defined BT_BCSP

    #include "hci_uart.h"
    #include "bcsp.h"

#elif defined BT_USB

    #include "hci_usb.h"

#elif defined BT_SW_TXP

    #include "hci_sw_txp.h"

#elif defined BT_SOCKET

    #include "hci_socket.h"

#elif defined BT_PLATFORM

    #include "hci_platform.h"

#else /* Error */

    #error "Forgot to define HCI Transport Flag in the Makefile ?"

#endif
#endif /* HCI_TX_RUN_TIME_SELECTION */


/* -----------------------------------------------  Type Definitions */


/* -----------------------------------------------  Structures */
#ifdef HT_ENABLE_PARSE_UTIL
/* HCI Packet parsing utility structure */
typedef struct _HT_PARSE
{
    /* Packet state */
    UCHAR * packet;

    /* Packet Expected Length */
    UINT16 packet_expected_len;

    /* Packet Pointer */
    UCHAR packet_state;

} HT_PARSE;
#endif /* HT_ENABLE_PARSE_UTIL */

/* -----------------------------------------------  API Declarations */
/* Init & Shutdown */
void em_hci_transport_init (void);
#ifdef BT_HAVE_SHUTDOWN
/** De-init routine */
void em_hci_transport_shutdown (void);
#endif /* BT_HAVE_SHUTDOWN */

void hci_transport_bt_init (void);
void hci_transport_bt_shutdown (void);

void ht_read_task_create_pl (BT_THREAD_START_ROUTINE routine);

#ifdef BT_HAVE_SHUTDOWN
API_RESULT ht_read_task_shutdown_pl ( void );
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \fn ht_read_task_exit_pl
 *
 *  \par Description
 *  This function is called by the Read Task Start Routine
 *  to know if the Task needs to break out of its core loop
 *  and return (which effectively means exit of the task).
 *
 *  \param None
 *
 *  \return UCHAR
 *          0 - No exit, continue to execute.
 *          1 - Break from core loop & return.
 */
#define ht_read_task_exit_pl()  1

/* To enqueue incoming HCI packet to Transport Buffer */
#ifndef HT_ENQUEUE_WITH_RETURN
void hci_transport_enqueue (UCHAR *packet, UINT16 packet_len);
#else /* HT_ENQUEUE_WITH_RETURN */
API_RESULT hci_transport_enqueue(UCHAR *packet, UINT16 packet_len, UINT16 *enqueued_len);
#endif /* HT_ENQUEUE_WITH_RETURN */

#ifdef HT_ENABLE_PARSE_UTIL
/* APIs to packet parsing utility */
void ht_parse_packet_init (HT_PARSE * ht);
API_RESULT ht_parse_packet (HT_PARSE * ht);
#endif /* HT_ENABLE_PARSE_UTIL */

#endif /* _H_HCI_TRANSPORT_ */

