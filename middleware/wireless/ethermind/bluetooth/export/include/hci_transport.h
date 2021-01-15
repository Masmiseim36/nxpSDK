
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

#else /* Error */

    #error "Forgot to define HCI Transport Flag in the Makefile ?"

#endif
#endif /* HCI_TX_RUN_TIME_SELECTION */


/* -----------------------------------------------  Type Definitions */


/* -----------------------------------------------  Structures */
/* HCI Packet parsing utility structure */
typedef struct _HT_PARSE
{
    /* Packet state */
    UCHAR packet_state;

    /* Packet Expected Length */
    UINT16 packet_expected_len;

    /* Packet Pointer */
    UCHAR * packet;

} HT_PARSE;

/* -----------------------------------------------  API Declarations */
/* Init & Shutdown */
void em_hci_transport_init (void);
void hci_transport_bt_init (void);
void hci_transport_bt_shutdown (void);

void ht_read_task_create_pl (BT_THREAD_START_ROUTINE routine);
UCHAR ht_read_task_shutdown_pl ( void );

/* To enqueue incoming HCI packet to Transport Buffer */
#ifndef HT_ENQUEUE_WITH_RETURN
void hci_transport_enqueue (UCHAR *packet, UINT16 packet_len);
#else /* HT_ENQUEUE_WITH_RETURN */
API_RESULT hci_transport_enqueue(UCHAR *packet, UINT16 packet_len, UINT16 *enqueued_len);
#endif /* HT_ENQUEUE_WITH_RETURN */

/* APIs to packet parsing utility */
void ht_parse_packet_init (HT_PARSE * ht);
API_RESULT ht_parse_packet (HT_PARSE * ht);

#endif /* _H_HCI_TRANSPORT_ */

