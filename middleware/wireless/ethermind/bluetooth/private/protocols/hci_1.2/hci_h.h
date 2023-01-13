
/**
 *  \file hci_h.h
 *
 *  This Header File describes the Upper Layer to HCI Interface.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_HCI_H_
#define _H_HCI_H_

/* ------------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_hci_api.h"

/* ------------------------------------------------- Global Definitions */

/** Packet Boundary Flag in the ACL Data Header */
#define HCI_START_OF_L2CAP_PACKET                   0x02U
#define HCI_CONTINUATION_OF_L2CAP_PACKET            0x01U

/**
 * Non - Automatically - Flushable Packet Boundary Flag
 * introduced with 2.1+EDR Specification
 */
#define HCI_START_OF_NON_FLUSHABLE_L2CAP_PACKET     0x00U
#define HCI_START_OF_FLUSHABLE_L2CAP_PACKET         0x02U

#ifdef HCI_ISO_DATA
/** Packet Boundary Flag in the ISO Data Header */
#define HCI_START_OF_ISO_PACKET                     0x00U
#define HCI_CONTINUATION_OF_ISO_PACKET              0x01U
#define HCI_COMPLETE_ISO_PACKET                     0x02U
#define HCI_END_OF_ISO_PACKET                       0x03U
#endif /* HCI_ISO_DATA */

/* Broadcast flag in the ACL Data Header */
#define HCI_NO_BROADCAST                            0x00U
#define HCI_ACTIVE_BROADCAST                        0x01U
#define HCI_PICONET_BROADCAST                       0x02U

/* ------------------------------------------------- Function Declarations */

/* ------------------------------------------------ API Declarations */
/** Initialisation & shutdown routines of the HCI Layer */
void em_hci_init( void );
#ifdef BT_HAVE_SHUTDOWN
void em_hci_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */
API_RESULT hci_bt_init ( void );
void hci_bt_shutdown ( void );

/* Routine to set Local Name */
API_RESULT hci_set_local_name ( CHAR *name );

/** For Interaction with the HCI Transport Layer */
API_RESULT hci_transport_read_data
           (
               UCHAR    type,
               UCHAR *  packet,
               UINT16   length
           );

/** Function to handle invalid packet type received at the HCI Transport */
#ifdef HT_ENABLE_INVALID_RX_IND
API_RESULT hci_transport_invalid_read_ind
           (
               /* IN */ UCHAR rx_octet
           );
#endif /* HT_ENABLE_INVALID_RX_IND */

/** Function which handles a Sync Loss at the HCI Transport */
void hci_transport_sync_lost_ind (void);

#ifdef HCI_CHECK_TARNSPORT_READINESS
/** Interfaces to inform HCI of the transport readiness */
void hci_transport_set_ready (UCHAR state);
#endif /* HCI_CHECK_TARNSPORT_READINESS */

/* APIs for EtherMind Security Manager ------------------------------------- */
API_RESULT hci_sm_connection_request_reply
           (UCHAR *bd_addr, UCHAR accept, UCHAR reason);

API_RESULT hci_sm_auth_encr_change_link_key
           (
               UINT16    opcode,
               UINT16    handle,
               UCHAR     enable
           );

#define hci_sm_authentication_request(hdl) \
        hci_sm_auth_encr_change_link_key \
        (HCI_AUTHENTICATION_REQUESTED_OPCODE, (hdl), 0x00)

#define hci_sm_change_connection_link_key_request(hdl) \
        hci_sm_auth_encr_change_link_key \
        (HCI_CHANGE_CONNECTION_LINK_KEY_OPCODE, (hdl), 0x00)

#define hci_sm_encryption_request(hdl, en) \
        hci_sm_auth_encr_change_link_key \
        (HCI_SET_CONNECTION_ENCRYPTION_OPCODE, (hdl), (en))

#ifdef BT_SSP
API_RESULT hci_sm_io_capability_request_reply
           (
               UCHAR  * bd_addr,
               UCHAR  io_caps,
               UCHAR  oob_data_present,
               UCHAR  auth_requirements
           );

API_RESULT hci_sm_io_capability_request_negative_reply
           (
               UCHAR  * bd_addr,
               UCHAR  reason
           );
#endif /* BT_SSP */

#ifdef BT_SSP_OOB
API_RESULT hci_sm_remote_oob_data_request_reply
           (
               UCHAR  * bd_addr,
               UCHAR    oob_present,
               UCHAR  * ssp_c,
               UCHAR  * ssp_r
           );
#endif /* BT_SSP_OOB */

#endif /* _H_HCI_H_ */

