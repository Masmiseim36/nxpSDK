
/**
 *  \file appl_hci.h
 *
 *  Header File for HCI Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HCI_LE_
#define _H_APPL_HCI_LE_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#ifdef APPL_FSM_SUPPORT
#include "appl_fsm.h"
#endif /* APPL_FSM_SUPPORT */

#ifdef AES_CMAC_UTIL
#include "aes_cmac_pl.h"
#endif /* AES_CMAC_UTIL */

/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Structures/Data Types */
typedef struct
{
    UINT16    opcode;
    char *    command_name;
} APPL_HCI_LE_OPCODE_NAME;

/* ----------------------------------------- Macros */

/* ----------------------------------------- Function Declarations */
/* HCI Application Event Indication Callback */
API_RESULT appl_hci_le_event_indication_callback
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           );


/* HCI API Handlers */
void appl_hci_read_transmit_power_level(void);

void appl_le_send_data(void);

char * appl_hci_le_get_command_name (UINT16 opcode);
void main_hci_le_operations (void);

#ifdef BT_4_0
/* Send HCI_LE_SET_EVENT_MASK */
void appl_hci_le_set_event_mask(void);

/* Send HCI_LE_READ_BUFFER_SIZE */
void appl_hci_le_read_buffer_size(void);

/* Send HCI_LE_READ_LOCAL_SUPPORTED_FEATURES */
void appl_hci_le_read_local_supported_features(void);

/* Send HCI_LE_SET_RANDOM_ADDRESS */
void appl_hci_le_set_random_address(void);

/* Send HCI_LE_SET_ADVERTISING_PARAMETERS */
void appl_hci_le_set_advertising_parameters(void);

/* Send HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER */
void appl_hci_le_read_advertising_channel_tx_power(void);

/* Send HCI_LE_SET_ADVERTISING_DATA */
void appl_hci_le_set_advertising_data(void);

/* Send HCI_LE_SET_SCAN_RESPONSE_DATA */
void appl_hci_le_set_scan_response_data(void);

/* Send HCI_LE_SET_ADVERTISING_ENABLE */
void appl_hci_le_set_advertising_enable(void);

/* Send HCI_LE_SET_SCAN_PARAMETERS */
void appl_hci_le_set_scan_parameters(void);

/* Send HCI_LE_SET_SCAN_ENABLE */
void appl_hci_le_set_scan_enable(void);

/* Send HCI_LE_CREATE_CONNECTION */
void appl_hci_le_create_connection(void);

/* Send HCI_LE_CREATE_CONNECTION_CANCEL */
void appl_hci_le_create_connection_cancel(void);

/* Send HCI_LE_READ_WHITE_LIST_SIZE */
void appl_hci_le_read_white_list_size(void);

/* Send HCI_LE_CLEAR_WHITE_LIST */
void appl_hci_le_clear_white_list(void);

/* Send HCI_LE_ADD_DEVICE_TO_WHITE_LIST */
void appl_hci_le_add_device_to_white_list(void);

/* Send HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST */
void appl_hci_le_remove_device_from_white_list(void);

/* Send HCI_LE_CONNECTION_UPDATE */
void appl_hci_le_connection_update(void);

/* Send HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION */
void appl_hci_le_set_host_channel_classification(void);

/* Send HCI_LE_READ_CHANNEL_CLASSIFICATION */
void appl_hci_le_read_channel_map(void);

/* Send HCI_LE_READ_REMOTE_USED_FEATURES */
void appl_hci_le_read_remote_used_features(void);

/* Send HCI_LE_ENCRYPT */
void appl_hci_le_encrypt(void);

/* Send HCI_LE_RAND */
void appl_hci_le_rand(void);

/* Send HCI_LE_START_ENCRYPTION */
void appl_hci_le_start_encryption(void);

/* Send HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY */
void appl_hci_le_long_term_key_requested_reply(void);

/* Send HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY */
void appl_hci_le_long_term_key_requested_negative_reply(void);

/* Send HCI_LE_READ_SUPPORTED_STATES */
void appl_hci_le_read_supported_states(void);

/* Send HCI_LE_RECEIVER_TEST_COMMAND */
void appl_hci_le_receiver_test_command(void);

/* Send HCI_LE_TRANSMITTER_TEST_COMMAND */
void appl_hci_le_transmitter_test_command(void);

/* Send HCI_LE_TEST_END */
void appl_hci_le_test_end(void);
#endif /* BT_4_0 */

#ifdef BT_4_1
/* Remote Connection Parameter Request Reply */
void appl_hci_le_remote_conn_param_req_reply(void);

/* Remote Connection Parameter Request Negative Reply */
void appl_hci_le_remote_conn_param_req_negtive_reply(void);
#endif /* BT_4_1 */

void appl_hci_read_authenticated_payload_timeout(void);

void appl_hci_write_authenticated_payload_timeout(void);

void appl_send_data( void );

#ifdef BT_4_2

void appl_hci_le_set_data_length(void);

void appl_hci_le_read_suggested_default_data_length(void);

void appl_hci_le_write_suggested_default_data_length(void);

void appl_hci_le_read_local_p256_public_key(void);

void appl_hci_le_generate_dhkey(void);

void appl_hci_le_add_device_to_resolving_list(void);

void appl_hci_le_remove_device_from_resolving_list(void);

void appl_hci_le_clear_resolving_list(void);

void appl_hci_le_read_resolving_list_size(void);

void appl_hci_le_read_peer_resolvable_address(void);

void appl_hci_le_read_local_resolvable_address(void);

void appl_hci_le_set_address_resolution_enable(void);

void appl_hci_le_set_resolvable_private_address_timeout(void);

void appl_hci_le_read_maximum_data_length(void);

#endif /* BT_4_2 */

#ifdef BT_5_0
void appl_hci_le_read_phy(void);
void appl_hci_le_set_default_phy(void);
void appl_hci_le_set_phy(void);
void appl_hci_le_set_extended_advertising_params(void);
void appl_hci_le_set_periodic_adv_params(void);
void appl_hci_le_set_extended_advertising_data(void);
void appl_hci_le_set_extended_scan_response_data(void);
void appl_hci_le_set_extended_advertise_enable(void);
void appl_hci_le_read_max_advertising_data_length(void);
void appl_hci_le_read_num_supported_advertising_sets(void);
void appl_hci_le_read_transmit_power(void);
void appl_hci_le_remove_advertising_set(void);
void appl_hci_le_clear_advertising_sets(void);
void appl_hci_le_set_extended_scan_parameters(void);
void appl_hci_le_set_extended_scan_enable(void);
void appl_hci_le_periodic_adv_create_sync(void);
void appl_hci_le_add_dev_to_periodic_advertiser_list(void);
void appl_hci_le_rm_dev_from_periodic_advertiser_list(void);
void appl_hci_le_clear_periodic_advertiser_list(void);
void appl_hci_le_read_periodic_advertiser_list_size(void);
void appl_hci_le_write_rf_path_compensation(void);
void appl_hci_le_read_rf_path_compensation(void);
void appl_hci_le_set_advertising_random_address(void);
void appl_hci_le_set_periodic_advertising_data(void);
void appl_hci_le_set_periodic_advertising_enable(void);
void appl_hci_le_periodic_advertising_create_sync_cancel(void);
void appl_hci_le_periodic_adv_terminate_sync(void);
void appl_hci_le_extended_create_connection(void);
void appl_hci_le_set_privacy_mode(void);
#endif /* BT_5_0 */

#ifdef BT_5_1
void appl_hci_le_receiver_test_v3(void);
void appl_hci_le_transmitter_test_v3(void);
void appl_hci_le_set_connectionless_cte_transmit_parameters(void);
void appl_hci_le_set_connectionless_cte_transmit_enable(void);
void appl_hci_le_set_connectionless_iq_sampling_enable(void);
void appl_hci_le_set_connection_cte_receive_parameters(void);
void appl_hci_le_set_connection_cte_transmit_parameters(void);
void appl_hci_le_set_connection_cte_request_enable(void);
void appl_hci_le_set_connection_cte_response_enable(void);
void appl_hci_le_read_antenna_information(void);
void appl_hci_le_set_periodic_advertising_receive_enable(void);
void appl_hci_le_periodic_advertising_sync_transfer(void);
void appl_hci_le_periodic_advertising_set_info_transfer(void);
void appl_hci_le_set_periodic_advertising_sync_transfer_parameters(void);
void appl_hci_le_set_default_periodic_advertising_sync_transfer_parameters(void);
void appl_hci_le_generate_dhkey_v2(void);
void appl_hci_le_modify_sleep_clock_accuracy(void);
#endif /* BT_5_1 */

#endif /* _H_APPL_HCI_LE_ */

