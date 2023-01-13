
/**
 *  \file sm.h
 *
 *  This file contains declaration of APIs that are exported for
 *  the other EtherMind modules interfacing with Security Manager
 *  for Security Access Requests and various other procedures.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SM_
#define _H_SM_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"

/* ----------------------------------------- Global Definitions */
/** Events for Security Access Contest State Machine */
#define SM_PIN_CODE_REQUEST                     0x01U
#define SM_AUTHORIZE_REQUEST                    0x02U
#define SM_CONNECTION_REQUEST                   0x03U
#define SM_L2CAP_REQUEST                        0x04U
#define SM_RFCOMM_REQUEST                       0x05U
#define SM_AUTHORIZATION_COMPLETE               0x06U
#define SM_AUTHENTICATION_COMPLETE              0x07U
#define SM_ENCRYPTION_COMPLETE                  0x08U
#define SM_DISCONNECTION_COMPLETE               0x09U


/* ----------------------------------------- Internal Functions */
#if 0
/** Security Manager Access Requests */
API_RESULT sm_access_request
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UINT16     psm,
               /* IN */  UCHAR      server_ch,
               /* IN */  UINT32     proto_data
           );
#endif /* 0 */

API_RESULT sm_access_request_ex
           (
               /* IN */  DEVICE_HANDLE    *  handle,
               /* IN */  UINT16           psm,
               /* IN */  UCHAR            server_ch,
               /* IN */  UINT32           proto_data
           );

#ifndef BT_SSP
#ifdef SM_HAVE_MODE_2
API_RESULT sm_access_request_mode_1_3
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UINT16     psm,
               /* IN */  UCHAR      server_ch,
               /* IN */  UINT32     proto_data
           );
#endif /* SM_HAVE_MODE_2 */
#endif /* BT_SSP */


/* ----------------------------------------- API Declarations */
#ifdef CLASSIC_SEC_MANAGER
/** Initialization & Shutdown of Security Manager */
void em_sm_init (void);
#ifdef BT_HAVE_SHUTDOWN
void em_sm_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */
void sm_bt_init (void);
void sm_bt_shutdown (void);

/** Security Access Request from L2CAP */
#define sm_l2cap_access_request(dhandle, psm, l2cap_data) \
        sm_access_request_ex((dhandle), (psm), 0x0, (l2cap_data))

/** Security Access equest from RFCOMM */
#define sm_rfcomm_access_request(dhandle, server_ch, l2cap_data) \
        sm_access_request_ex((dhandle), RFCOMM_PSM, (server_ch), (rfcomm_data))

/** HCI Callback for ACL Connection Request */
API_RESULT sm_acl_connection_request
           (
               /* IN */  UCHAR *   bd_addr,
               /* IN */  UINT32    cod
           );

/** HCI Callback for ACL Create Connection */
API_RESULT sm_acl_create_connection
           (
               /* IN */  UCHAR *   bd_addr
           );

/** HCI Callback for ACL Connection Complete */
API_RESULT sm_acl_connection_complete
           (
               /* IN */  UCHAR *   bd_addr,
               /* IN */  UCHAR     status,
               /* IN */  UINT16    acl_handle,
               /* IN */  UCHAR     encrypt
           );

/** HCI Callback for Link Key & PIN Code Request */
API_RESULT sm_link_key_pin_code_request
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UCHAR      event_type
           );

/** HCI Callback for Link Key Notification */
API_RESULT sm_link_key_notification
           (
               /* IN */  UCHAR *   bd_addr,
               /* IN */  UCHAR *   link_key,
               /* IN */  UCHAR     link_key_type
           );

/** HCI Callback for Command Complete Event */
API_RESULT sm_command_complete_event
           (
               /* IN */  UCHAR *   packet
           );

/**
 *  HCI Callback for Completion of Authentication, Encryption
 *  ACL Channel Disconnection
 */
API_RESULT sm_hci_proc_complete
           (
               /* IN */  UCHAR     event,
               /* IN */  UINT16    acl_handle,
               /* IN */  UCHAR     status,
               /* IN */  UCHAR     enable
           );

/** HCI Callback for Error Indication */
API_RESULT sm_hci_error_indication
           (
               /* IN */  UINT16    opcode,
               /* IN */  UINT16    status
           );

#ifdef BT_SSP
/** HCI Callback for IO Capability Request */
API_RESULT sm_io_capability_event
           (
               /* IN */  UCHAR      type,
               /* IN */  UCHAR *    packet
           );
#endif /* BT_SSP */

#ifdef BT_SSP_UC
/** HCI Callback for User Confirmation Request */
API_RESULT sm_user_conf_request
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UCHAR *    numeric_value
           );
#endif /* BT_SSP_UC */

#ifdef BT_SSP_PE
/** HCI Callback for User Passkey Request */
API_RESULT sm_user_passkey_request
           (
               /* IN */  UCHAR *    bd_addr
           );

/** HCI Callback for User Passkey Notification */
API_RESULT sm_user_passkey_notification
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UCHAR *    passkey
           );
#endif /* BT_SSP_PE */

#ifdef BT_SSP_OOB
/** HCI Callback for Remote OOB Data Request */
API_RESULT sm_remote_oob_data_request
           (
               /* IN */  UCHAR *    bd_addr
           );
#endif /* BT_SSP_OOB */

#ifdef SM_TEST_MODE
API_RESULT sm_dump_device_status (UCHAR *bd_addr);
#endif /* SM_TEST_MODE */
#else

#define em_sm_init()
#define sm_bt_init()
#define sm_bt_shutdown()

#define sm_l2cap_access_request(dhandle, psm, l2cap_data) API_SUCCESS
#define sm_rfcomm_access_request(dhandle, server_ch, l2cap_data) API_SUCCESS
#define sm_acl_connection_request(bd_addr, cod) API_SUCCESS
#define sm_acl_create_connection(bd_addr) API_SUCCESS
#define sm_acl_connection_complete(bd_addr, status, acl_handle, encrypt) API_SUCCESS
#define sm_link_key_pin_code_request(bd_addr, event_type) API_SUCCESS
#define sm_link_key_notification(bd_addr, link_key, link_key_type) API_SUCCESS
#define sm_command_complete_event(packet) API_SUCCESS
#define sm_hci_proc_complete(event, acl_handle, status, enable) API_SUCCESS
#define sm_hci_error_indication(opcode, status) API_SUCCESS

#endif /* CLASSIC_SEC_MANAGER */

#endif /* _H_SM_ */


