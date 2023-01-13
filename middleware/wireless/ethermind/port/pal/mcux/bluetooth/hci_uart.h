
/**
 *  \file hci_uart.h
 *
 *  This files containes the definitins used by the HCI UART Transport Layer
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_HCI_UART_
#define _H_HCI_UART_

/** --------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ---------------------------------- Global Definitions */

#ifdef BT_BCSP
    #define hci_transport_write_data    hci_bcsp_send_data
#endif /* BT_BCSP */

#ifndef HCI_UART_TX_NONBLOCKING
#define HCI_UART_TX_NONBLOCKING 0u
#endif /* HCI_UART_TX_NONBLOCKING */

/* ---------------------------------- Function Declarations */
/** HCI UART enable interface */
/* void hci_uart_enable(void); */

/** HCI UART Initialization & Shutdown */
void hci_uart_init (void);
#ifdef BT_HAVE_SHUTDOWN
void hci_uart_shutdown (void);
#endif /* BT_HAVE_SHUTDOWN */
void hci_uart_bt_init (void);
void hci_uart_bt_shutdown (void);

/* HCI UART Send Data */
API_RESULT hci_uart_send_data (UCHAR type, UCHAR *buf, UINT16 length, UCHAR flag);
API_RESULT hci_transport_write_data (UCHAR type, UCHAR * buf, UINT16 length, UCHAR flag);
void hci_uart_write_data(UCHAR *data, UINT16 datalen);


void hci_uart_set_serial_settings (CHAR *device, UINT32 baud);

#endif /* _H_HCI_UART_ */

