
/**
 *  \file appl_hci.h
 *
 *  Header File for HCI Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HCI_
#define _H_APPL_HCI_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Structures/Data Types */
typedef struct
{
    UINT16    opcode;
    char *    command_name;
} APPL_HCI_OPCODE_NAME;

/* The HCI Profile Event Indication Callback Type */
typedef struct
{
    /* Pointer to HCI callback function  */
    API_RESULT (* hci_event_ind_cb) (UINT8, UCHAR *, UINT8);

} APPL_HCI_CB_PTR;

/* ----------------------------------------- Macros */
#define appl_hci_print_bd_addr(bd)      appl_dump_bytes((bd), 6);

/*
 *  Maximum number of HCI Profile Event Indication Callbacks that Profiles
 *  and Applications on top of HCI (EtherMind Stack) can register
 *  with HCI Application.
 *
 *  Minimum Value: 1
 *  Maximum Value: 254
 */
#define APPL_MAX_NUM_HCI_CB     2

/**
 *  To Lock APPL HCI Global Mutex.
 *  Since in the sample applications are called from the main task context
 *  this is just a place holder, and implement only if required.
 */
#define APPL_HCI_MUTEX_LOCK()

/**
 *  To Unlock APPL HCI Global Mutex.
 *  Since in the sample applications are called from the main task context
 *  this is just a place holder, and implement only if required.
 */
#define APPL_HCI_MUTEX_UNLOCK()


/* ----------------------------------------- Function Declarations */
/* HCI Application Event Indication Callback */
API_RESULT appl_hci_event_indication_callback
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           );

/* HCI Error Indication Callback */
API_RESULT appl_hci_error_indication_callback
           (
               UINT16  opcode,
               UINT16  error_code
           );

/* To show HCI Inquiry Results */
void appl_hci_show_inq_result
     (UCHAR inq_resp_recvd, UCHAR *inq_bytes);
void appl_hci_show_inq_result_with_rssi
     (UCHAR inq_resp_recvd, UCHAR *inq_bytes);

#ifdef HCI_SCO
void appl_hci_esco_audio_config(void);

void appl_hci_sco_audio_mic_record_callback
     (
         /* IN */ UCHAR     *mic_data,
         /* IN */ UINT16     mic_datalen
     );

void appl_hci_sco_audio_spkr_playack_callback
     (
         /* IN */ UCHAR     *data,
         /* IN */ UINT16     datalen
     );

API_RESULT appl_hci_esco_msbc_data_handler
           (
               /* IN */ UCHAR    * packet,
               /* IN */ UINT16     packet_len
           );
#endif /* HCI_SCO */


/* HCI API Handlers */
void appl_hci_inquiry (void);
void appl_hci_create_connection (void);
void appl_hci_add_sco_connection (void);
void appl_hci_setup_synchronous_connection (void);
void appl_hci_setup_synchronous_connection_response (void);

#ifdef HCI_ENH_SCO
void appl_hci_read_local_supported_codecs (void);
void appl_hci_enhanced_setup_synchronous_connection (void);
void appl_hci_enhanced_setup_synchronous_connection_response (void);
#endif /* HCI_ENH_SCO */

void appl_hci_disconnect (void);
void appl_hci_remote_name_request (void);
void appl_hci_role_discovery (void);
void appl_hci_switch_role (void);
void appl_hci_sniff_mode(void);
void appl_hci_park_mode(void);
void appl_hci_hold_mode(void);
void appl_hci_exit_sniff_mode(void);
void appl_hci_exit_park_mode(void);
void appl_hci_write_link_policy(void);

void appl_hci_read_connection_accept_timeout (void);
void appl_hci_write_connection_accept_timeout (void);
void appl_hci_read_page_timeout (void);
void appl_hci_write_page_timeout (void);
void appl_hci_read_page_scan_activity (void);
void appl_hci_write_page_scan_activity (void);
void appl_hci_read_inquiry_scan_activity (void);
void appl_hci_write_inquiry_scan_activity (void);
void appl_hci_read_class_of_device (void);
void appl_hci_write_class_of_device (void);
void appl_hci_read_scan_enable (void);
void appl_hci_write_scan_enable (void);
void appl_hci_read_local_name (void);
void appl_hci_change_local_name (void);
void appl_hci_read_inquiry_mode (void);
void appl_hci_write_inquiry_mode (void);
void appl_hci_read_inquiry_scan_type (void);
void appl_hci_write_inquiry_scan_type (void);
void appl_hci_read_page_scan_type (void);
void appl_hci_write_page_scan_type (void);
void appl_hci_read_current_iaclap(void);
void appl_hci_write_current_iaclap(void);
void appl_hci_get_connection_details (void);

#ifdef BT_3_0

#ifdef HCI_FLOW_CONTROL_COMMANDS
void appl_hci_read_flow_control_mode (void);
void appl_hci_write_flow_control_mode (void);
#endif /* HCI_FLOW_CONTROL_COMMANDS */

void appl_hci_read_enhanced_transmit_power_level (void);

#ifndef HCI_LITE
void appl_hci_read_encryption_key_size (void);
#endif /* HCI_LITE */

#endif /* BT_3_0 */

#ifdef BT_BRSC
void appl_hci_read_secure_connections_host_support (void);
void appl_hci_write_secure_connections_host_support (void);
#endif /* BT_BRSC */

#ifdef BT_BRSC_TEST
void appl_hci_write_secure_connections_test_mode (void);
#endif /* BT_BRSC_TEST */

char * appl_hci_get_command_name (UINT16 opcode);
void main_hci_operations ( void );

/* Application Interface for Profiles to Register their HCI Callbacks */
API_RESULT appl_hci_register_callback
           (
               API_RESULT (* callback_ptr)
                          (
                              UINT8    event_code,
                              UINT8 *  event_data,
                              UINT8    event_datalen
                          )
           );

API_RESULT appl_hci_unregister_callback
           (
               API_RESULT (* callback_ptr)
                          (
                              UINT8    event_code,
                              UINT8 *  event_data,
                              UINT8    event_datalen
                          )
           );

#ifndef HCI_NO_ESCO_AUTO_ACCEPT
#ifdef BT_HCI_1_2
API_RESULT appl_hci_set_esco_channel_parameters
           (
               /* IN */ UCHAR               set_sco_param,
               /* IN */ HCI_SCO_IN_PARAMS * sco_params
           );
#endif /* BT_HCI_1_2 */

#else
/* Empty Macro */
#define appl_hci_set_esco_channel_parameters(set_sco_param, sco_params)
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */

#ifdef BT_EIR

void appl_print_extended_inquiry_response
     (
         UCHAR *eir_data,
         UINT16 eir_datalen
     );

void appl_hci_write_extended_inquiry_response(void);

#endif /* BT_EIR */

#endif /* _H_APPL_HCI_ */

