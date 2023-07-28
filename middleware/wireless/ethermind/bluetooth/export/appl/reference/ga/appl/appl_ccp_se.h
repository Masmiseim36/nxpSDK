

/**
 *  \file appl_ccp_se.h
 *
 *  \brief This file is the Call Control Profile Client reference application header
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CCP_SE_
#define _H_APPL_CCP_SE_

/* --------------------------------------------- Header File Inclusion */
#include "appl_ccp.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

typedef struct _APPL_CCP_SE_CP_NTF_INFO
{
    /* Requested Opcode */
    UINT8 cp_opcode;

    /* Call Index */
    UINT8 call_index;

    /* Result Code */
    UINT8 result_code;

    /* Datalen */
    UINT8 datalen;
}APPL_CCP_SE_CP_NTF_INFO;

/** Notification Info */
typedef struct _APPL_CCP_SE_NTF_INFO
{
    /* Service Handle */
    TBS_HANDLE tbs_handle;

    /** Char UUID */
    UINT16 char_uuid;

    /** service instance */
    UCHAR srvc_inst;

    /** Peer device */
    GA_ENDPOINT device;

    /* CP Notification Info */
    APPL_CCP_SE_CP_NTF_INFO cp_ntf_info;

}APPL_CCP_SE_NTF_INFO;

/* --------------------------------------------- Macros */

/*
 * A characteristic value has changed while a Read Long Value Characteristic
 * sub-procedure is in progress.
 */
#define TBS_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG 0x80U

/* Invalid Call ID */
#define APPL_TBS_SE_INVALID_CALL_ID    0xFFU

#define  APPL_TBS_SE_ENITITES          2U
#define  APPL_TBS_SE_BRR_CALL_LIST     10U

#define  APPL_TBS_BRR_AIRTEL            "AirTel"
#define  APPL_TBS_BRR_BSNL              "BSNL"
#define  APPL_TBS_BRR_JIO               "Jio"

#define  APPL_TBS_TRGT_BRR_URI          "tel:+0123456789"

/* Sample Call Friendly Name */
#define  APPL_TBS_FRNDLY_NAME           "Mindtree"

#define  APPL_TBS_CALLER_URI            "tel:+9876543210"

/* For PTS Test */
#define TBS_URI_ID_LONG_VALUE_V1           "tel,mailto,news,chrome,bitcoin,aim"
#define TBS_URI_ID_LONG_VALUE_V2           "chrome,mailto,tel,news,bitcoin,aim"

/* --------------------------------------------- APIs */
GA_RESULT appl_ccp_se_callback
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void        * event_data,
              /* IN */ UINT16        event_datalen
          );

void main_ccp_se_menu_operations(void);

GA_RESULT appl_ccp_se_pack_tbs_chr_value
          (
              /* IN  */  UINT16 chr_uuid,
              /* IN  */  TBS_PARAMS *tbs_params,
              /* OUT */  UCHAR  *chr_val,
              /* OUT */  UINT16 *chr_val_len
          );

GA_RESULT  appl_ccp_se_handle_cp_write
           (
               /* IN */ GA_ENDPOINT  * device,
               /* IN */ CCP_EVENT_INFO  *appl_event_info
           );

void appl_ccp_se_init_gtbs_info(/* IN */ TBS_PARAMS * appl_gtbs_params);
void appl_ccp_se_init_tbs_info(/* IN */ TBS_PARAMS * appl_tbs_params, UCHAR indx);
void appl_ccp_se_init_timer_handle(void);


void appl_tbs_notify_incall_details(UCHAR tbs_srvc_index, UCHAR index);
void appl_tbs_display_cur_call_state(void);
void appl_gtbs_display_cur_call_state(void);

void appl_gtbs_reset_all_call_details(UCHAR  tbs_indx);

void appl_set_active_tbs_service(void);
void appl_tbs_handle_accept
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     );
void appl_tbs_handle_terminate
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     );
void appl_tbs_handle_local_hold
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     );
void appl_tbs_handle_local_retrieve
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     );
void appl_tbs_handle_local_originate
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     );
void appl_tbs_handle_local_join
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     );

void appl_gtbs_set_active(UCHAR  srvc, UCHAR  call_index);
void appl_gtbs_set_locate_held(UCHAR  srvc, UCHAR  call_index);
void appl_gtbs_set_remotely_held(UCHAR  srvc, UCHAR  call_index);
void appl_gtbs_set_locally_and_remotely_held(UCHAR  srvc, UCHAR  call_index);
void appl_gtbs_set_call_alerting(UCHAR  srvc, UCHAR  call_index);
GA_RESULT appl_tbs_setup_incall(UCHAR  tbs_srvc_index, UCHAR * index_in_list);

GA_RESULT appl_get_free_call_index
          (
              /* IN */ UCHAR   tbs_indx,
              /* IN */ UCHAR * index
          );
GA_RESULT appl_tbs_get_call_index_from_call_id
          (
              /* IN */ UCHAR   tbs_srvc_index,
              /* IN */ UCHAR   call_id,
              /* IN */ UCHAR * index
          );

/* For PTS test */
void appl_set_large_call_state_info
     (
         UCHAR  * call_state_info,
         UINT16 * call_state_info_len
     );

void appl_tbs_set_bearer_name(void);
void appl_tbs_set_bearer_uci(void);
void appl_tbs_set_bearer_technology(void);
void appl_tbs_set_bearer_supp_uri_schemes(void);
void appl_tbs_set_bearer_signal_strength(void);
void appl_tbs_set_bearer_signal_strength_reporting_interval(void);
void appl_tbs_set_bearer_current_call_list(void);
void appl_tbs_set_bearer_content_control_id(void);
void appl_tbs_set_incomming_caller_target_info(void);
void appl_tbs_set_status_flags(void);
void appl_tbs_set_call_state(void);
void appl_tbs_set_optional_cp_opcodes(void);
void appl_tbs_set_incoming_caller_info(void);
void appl_tbs_set_caller_friendly_name(void);

void appl_tbs_set_char_value(void);

void appl_tbs_notify_bearer_provider_name(void);
void appl_tbs_notify_bearer_technology(void);
void appl_tbs_notify_bearer_supp_uri_schemes(void);
void appl_tbs_notify_bearer_signal_strength(void);
void appl_tbs_notify_curr_call_list(void);
void appl_tbs_notify_status_flags(void);
void appl_tbs_notify_incoming_call_target_bearer_uri(void);
void appl_tbs_notify_call_state(void);
void appl_tbs_notify_cp(void);
void appl_tbs_notify_termination_reason(void);
void appl_tbs_notify_incoming_call(void);
void appl_tbs_notify_call_friendly_name(void);

void appl_tbs_notify_char_value(void);

void appl_set_tbs_sdp_record_status(UCHAR flag);

void appl_ccp_se_display_response_data(CCP_RSP_INF * tbs_rsp_info, UINT16 offset_rcvd);

void appl_ccp_se_call_state_timer_handler(void * data, UINT16 datalen);
void appl_ccp_se_call_cp_timer_handler(void * data, UINT16 datalen);
void appl_ccp_se_terminate_reason_timer_handler(void * data, UINT16 datalen);

#endif /* _H_APPL_CCP_SE_ */
