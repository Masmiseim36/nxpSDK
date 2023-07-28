
/**
 *  \file appl_hfp_ag.h
 *
 *  Internal header file of HFP AG application.
 *  This file contains the data structures, type definitions
 *  and function declarations that HFP AG application uses
 *  internally.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HFP_AG_
#define _H_APPL_HFP_AG_

/* ---------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_hfp_ag_api.h"

#include "db_gen.h"


/* Flag to Enable only Enhaced Voice Recognition feature of HFPv1.8 */
/* #define  APPL_HAVE_ENH_VREC */

/* ---------------------------------------- Global Definitions */
/* Response to SLC AT Commands */
#define HFAG_OK_STR         (CHAR *)"\r\nOK\r\n"
#define HFAG_ERROR_STR      (CHAR *)"\r\nERROR\r\n"
#define HFAG_BRSF_STR       (CHAR *)"\r\n+BRSF: "
#define HFAG_BCS_STR        (CHAR *)"\r\n+BCS: "
#define HFAG_BIND_TEST_STR  (CHAR *)"\r\n+BIND: (1,2)\r\n"
#define HFAG_BIND_READ_STR  (CHAR *)"\r\n+BIND: "
#define HFAG_BVRA_STR       (CHAR *)"\r\n+BVRA: "
#define HFAG_BINP_STR       (CHAR *)"\r\n+BINP: "
#define HFAG_BSIR_STR       (CHAR *)"\r\n+BSIR: "
#define HFAG_VGS_STR        (CHAR *)"\r\n+VGS: "
#define HFAG_VGM_STR        (CHAR *)"\r\n+VGM: "

#define HFP_CR_LF           (CHAR *)"\r\n"

#define  HFP_AG_ENABLE_VOICE_REG            (CHAR *)"\r\n+BVRA: 1\r\n"
#define  HFP_AG_DISABLE_VOICE_REG           (CHAR *)"\r\n+BVRA: 0\r\n"

/* + BVRA: 1, 1 */
#define  HFP_AG_ENH_VR_MSG                  (CHAR *)"\r\n+BVRA: 1,"

/* + BVRA: 1, 1, 12AB, 0, 1, “Message to Melissa” */
#define  HFP_AG_ENH_VR_AND_VR_TXT_MSG       (CHAR *)"\r\n+BVRA: 1,5,12AB,0,1,\"Message to Melissa\"\r\n"
#define  HFAG_ENHVR_TXTREP                  (CHAR *)"12AB,0,1,\"Message to Melissa\""

#define HFAG_OK         0U
#define HFAG_ERROR      1U
#define HFAG_BRSF       2U
#define HFAG_BCS        3U
#define HFAG_BINP       4U
#define HFAG_VGS        5U
#define HFAG_VGM        6U
#define HFAG_BSIR       7U
#define HFAG_BIND_TEST  8U
#define HFAG_BIND_READ  9U
#define HFAG_BVRA       10U

/* Other definitions */
#define APPL_HF_STATE_DISCONNECTED     0x00U
#define APPL_HF_STATE_CONNECTED        0x01U

/*
 *  Local device capabilities for Audio Gateway
 *  AG_CCWA     : Three way calling.
 *  AG_ECNR     : EC ond/or NR Function
 *  AG_BVRA     : Voice Recognition function.
 *  AG_BSIR     : In-band ring tone capability
 *  AG_BINP     : Attach a Phone Number to Voice Tag.
 *  AG_GSM_NW   : GSM like Network.
 */
#define   SET_AG_CCWA                  0x01U
#define   SET_AG_ECNR                  0x02U
#define   SET_AG_BVRA                  0x04U
#define   SET_AG_BSIR                  0x08U
#define   SET_AG_BINP                  0x10U
#define   SET_AG_GSM_NW                0x20U
#define   SET_ALL_FEATURES             0x1FU

#define   RESET_AG_CCWA                0xFEU
#define   RESET_AG_ECNR                0xFDU
#define   RESET_AG_BVRA                0xFBU
#define   RESET_AG_BSIR                0xF7U
#define   RESET_AG_BINP                0xEFU
#define   RESET_AG_GSM_NW              0xDFU

/* HF AG SDP definitons */
#define HFP_AG_SUPP_FEATURES_ATTR_ID   0x0311U

/* Call states */
#define APPL_HFAG_CALL_IDLE            0x00U
#define APPL_HFAG_CALL_INCOMING        0x01U
#define APPL_HFAG_CALL_OUTGOING        0x02U
#define APPL_HFAG_CALL_ACTIVE          0x04U
#define APPL_HFAG_CALL_HELD            0x08U

#define APPL_HFAG_SEND_QUEUE_SIZE      8U

/* Maximum supporting feature value: 16383 */
#define HFP_AG_MAX_SUPP_FEATURE_RANGE  16383U

/* Minimum supporting feature value: 1 */
#define HFP_AG_MIN_SUPP_FEATURE_RANGE  1U



/* ---------------------------------------- Structures/Data Types */
typedef struct _APPL_HFAG_SENDELT
{
    UCHAR * buf;

    UINT16 len;

}APPL_HFAG_SENDELT;

/* ---------------------------------------- Macros */


/* ---------------------------------------- Function Declarations */
void main_hfp_ag_operations(void);

API_RESULT hfp_ag_appl_callback
           (
               /* IN */ HFP_AG_EVENTS hfp_ag_event,
               /* IN */ API_RESULT result,
               /* IN */ void * data,
               /* IN */ UINT16 data_length
           );

void appl_handle_retval_from_hfag( /* IN */ UINT16 retval);

void register_features( /* IN */ UCHAR supported_features);

void hfp_ag_register_supp_feat( /* IN */ UCHAR supported_features);

void appl_start_hfp_ag(void);

/* SDP Callback for HFP */
void appl_hfag_sdp_cb
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void appl_hfp_ag_send_data(void);

#if 0
void appl_hfp_ag_send_call_indicator(void);

void appl_hfp_ag_send_callsetup_indicator(void);

void appl_hfp_ag_send_service_indicator(void);

void appl_hfp_ag_send_signal_indicator(void);

void appl_hfp_ag_send_roam_indicator(void);

void appl_hfp_ag_send_battchrg_indicator(void);

void appl_hfp_ag_send_callheld_indicator(void);

void appl_hfp_ag_send_callring(void);

void appl_hfp_ag_send_select_codec (void);

void appl_hfp_ag_send_result_codes (void);

void appl_hfp_ag_send_call_list (void);

void appl_hfp_ag_send_call_req_stage_1 (void);

void appl_hfp_ag_send_call_req_stage_2 (void);

void appl_hfp_ag_send_2nd_call_req (void);

void appl_hfp_ag_set_call_satus_before_slc(void);

void appl_hfp_ag_send_twc_callsetup0_heldcall_1 (void);

void appl_hfp_ag_accept_incoming_call_at_remote_termimal (void);

void appl_hfp_ag_send_held_incoming_call_reject (void);

void appl_hfp_ag_send_accept_held_call (void);
#endif /* 0 */

void appl_hfp_ag_send_enable_voice_recognition (void);

void appl_hfp_ag_send_disable_voice_recognition (void);

#ifdef HFP_AG_1_8
void appl_hfp_ag_send_enh_voice_recognition (void);
#endif /* HFP_AG_1_8 */

void appl_hfpag_create_eSCO_connection(void);
void appl_hfpag_change_esco_config(void);
void appl_hfag_open_voice_channel(UCHAR codec);
void appl_hfag_close_voice_channel(void);
#if 0
void appl_hfag_setup_incoming_call(UCHAR inband);
void appl_hfag_handle_in_call(UCHAR option);
void appl_hfag_handle_twc(UCHAR option);
void appl_hfag_setup_outgoing_call(UCHAR voice);
void appl_hfag_handle_outgoing_call(UCHAR option, UCHAR hold);
API_RESULT appl_hfag_handle_btrh(UCHAR option);
#endif /* 0 */

#endif /* _H_APPL_HFP_AG_ */

