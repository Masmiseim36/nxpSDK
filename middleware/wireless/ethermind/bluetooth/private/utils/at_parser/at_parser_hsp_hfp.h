
/**
 *  \file at_parser_hsp_hfp.h
 *
 *  This Header file contains the Data Structures, Function Prototypes and
 *  Type Definitions used internally in the parser interface module for HFP
 *  and HSP profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_AT_PARSER_HSP_HFP_
#define _H_AT_PARSER_HSP_HFP_

/* -------------------------------------------------- Header File Inclusion */
#include "BT_at_parser_api.h"


/* -------------------------------------------------- Global Definitions */


/* -------------------------------------------------- Structures/Data Types */
/**
 *  Data Type to represent maximum & minimum values for Indicators mandated
 *  by the HFP-1.5 specification. More Indicators may be addded in future,
 *  as and when required.
 */
typedef struct
{
    /* AT_SERVICE */
    UCHAR    service_max_value;
    UCHAR    service_min_value;

    /* AT_CALL */
    UCHAR    call_max_value;
    UCHAR    call_min_value;

    /* AT_CALLSETUP */
    UCHAR    callsetup_max_value;
    UCHAR    callsetup_min_value;

    /* AT_CALL_SETUP */
    UCHAR    call_setup_max_value;
    UCHAR    call_setup_min_value;

    /* AT_SIGNAL */
    UCHAR    signal_max_value;
    UCHAR    signal_min_value;

    /* AT_ROAM */
    UCHAR    roam_max_value;
    UCHAR    roam_min_value;

    /* AT_BATTCHG */
    UCHAR    battchg_max_value;
    UCHAR    battchg_min_value;

    /* AT_CALLHELD */
    UCHAR    callheld_max_value;
    UCHAR    callheld_min_value;

} AT_PARSER_CIND_INDICATOR_VALUE;


/**
 *  The CIND Indicator Names are sent by the peer (HFP Audio Gateway)
 */
typedef enum
{
    AT_PARSER_IND_SERVICE        = AT_SERVICE,
    AT_PARSER_IND_CALL           = AT_CALL,
    AT_PARSER_IND_CALLSETUP      = AT_CALLSETUP,
    AT_PARSER_IND_CALL_SETUP     = AT_CALL_SETUP,
    AT_PARSER_IND_SIGNAL         = AT_SIGNAL,
    AT_PARSER_IND_ROAM           = AT_ROAM,
    AT_PARSER_IND_BATTCHG        = AT_BATTCHG,
    AT_PARSER_IND_CALL_HELD      = AT_CALLHELD,
    AT_PARSER_IND_UNKNOWN

} AT_PARSER_CIND_INDICATOR_NAME;


/**
 *  Data Type to represent the results obtained from peer for the
 *  BIND Test Command (AT+BIND=?)
 */
typedef struct
{
    /* Handsfree Indicator ID */
    UINT16   hf_ind_id;

    /* Handsfree Indicator Value */
    UCHAR    hf_ind_val;

}AT_PARSER_BIND_TEST_RESULT;

/**
 *  Data Type to represent the results obtained from peer for the
 *  BIND Read Command (AT+BIND?) and unsolited BIND rsp.
 */
#define AT_PARSER_BIND_READ_RESULT      AT_PARSER_BIND_TEST_RESULT


/**
 *  Data Type to represent the results obtained from peer for the
 *  CIND Test Command (AT+CIND=?)
 */
typedef struct
{
    /**
     *  Indicator Value - For Each indicator supported, the corresponding
     *  range of indicator values is stored in member indicator_value.
     */
    AT_PARSER_CIND_INDICATOR_VALUE    indicator_value;

    /**
     *  Indicator Array - It stores the position of the indicators in the
     *  CIND Test String/Response. It is used while CIND Read parsing.
     */
    AT_PARSER_CIND_INDICATOR_NAME     indicator_array
                                      [HFP_UNIT_MAX_INDICATOR_COUNT];

    /**
     *  Intermediate State - The intermediate state of CIND Test parsing is
     *  stored, so that parsing can be done if the response is received in
     *  multiple RFCOMM Reads.
     */
    UCHAR                            state;

} AT_PARSER_CIND_TEST_RESULT;


/**
 *  Data Type to represent the value of Indicators obtained from peer as
 *  response to CIND Read Command sent by HSP/HFP Unit (AT+CIND?).
 *  More Indicators may be addded in future, as and when required.
 *
 *  Each element holds the corresponding value received in CIND Read response.
 *  For Example: If 'service' indicator is supported and its value is
 *  1 in CIND Read Response, then 1 will be stored in member 'service' of
 *  this structure.
 */
typedef struct
{
    UCHAR    service;
    UCHAR    call;
    UCHAR    callsetup;
    UCHAR    call_setup;
    UCHAR    signal;
    UCHAR    roam;
    UCHAR    battchg;
    UCHAR    callheld;

} AT_PARSER_CIND_READ_RESULT;


/**
 *  Data Type to represent parameters exchanged with AT Command/Response
 *  Parser module for HFP and HSP Unit
 */
typedef struct
{
    /*
     * Parser State - It represents the current state of parsing.
     * Refer to state description section.
     */
    UCHAR    state;

    /*
     * Number/Count of Indicators received/parsed.
     * It is populated when CIND Test response is Parsed Successfully.
     * It is used while CIND Read parsing, to keep track of max. indicators.
     */
    UCHAR    indicator_count;

    /* Index of Data used up from Input Buffer */
    UINT16   consumed;

    /* Size of Buffer to be parsed */
    UINT16   parse_len;

    /* Start Index of the Buffer to be parsed */
    UINT16   start_indx;

    /* Place holder for CIND Read results */
    AT_PARSER_CIND_READ_RESULT    cind_read_result;

    /* Data Buffer to store partial responses */
    UCHAR    local_buf [AT_PARSER_PARTIAL_RSP_BUF_SIZE];

    /* Initial Size of Buffer */
    UINT16   local_buf_init_len;

    /* Flag to track status whether invalid kw is received or not */
    UCHAR    flag;

    /* Place holder for CIND Test results */
    AT_PARSER_CIND_TEST_RESULT    cind_test_result;

    /*
     * Retains the intermediate value of indicators parsed completely
     * during CIND Test or CIND Read Parsing. This value will be
     * retained till the Parsing is Complete.
     */
    UCHAR    indicator_index;

#ifdef HFP_UNIT_1_7

    /**
     * No. of HF indicator supported by the peer
     */
    UCHAR    hf_ind_count;

    /**
     * Place holder for AT+BIND=? command response result
     */
    AT_PARSER_BIND_TEST_RESULT       peer_hf_ind_list[HFP_UNIT_MAX_HF_INDICATOR_COUNT];

    /**
     *  Place holder for AT+BIND? command & unsolicited +BIND rsp result
     */
    AT_PARSER_BIND_READ_RESULT       peer_hf_ind_val;
#endif /* HFP_UNIT_1_7 */

} AT_PARSER_INTERFACE_PARAM;


/**
 *  Data Type to hold parsed results after parsing is complete
 */
typedef struct
{
    /* Buffer on which the AT Parser Response must be applied */
    UCHAR *             buffer;

    /* Size of Buffer on which the AT Parser Response must be applied */
    UINT16              buffer_len;

    /* Token Type */
    UCHAR               token;

    /* Pointer to Invalid string */
    UCHAR *             data_buf;

    /* Length of invalid kw string in data_buf */
    UINT16              invalid_kw_length;

    /* AT Parser Response */
    AT_PARSER_RESPONSE  parser_response;

} AT_PARSER_RESULT;


/* -------------------------------------------------- Macros */

/* -------------------------------------------------- Function Declarations */

#ifdef __cplusplus
extern "C" {
#endif

/** To parse incoming AT Response from Audio Gateway */
API_RESULT BT_at_parse_hsp_hfp_data
           (
               /* INOUT */ AT_PARSER_INTERFACE_PARAM *    params,
               /* IN */    UCHAR *                        input_buf,
               /* IN */    UINT16                         input_buf_len,
               /* OUT */   AT_PARSER_RESULT *             result
           );

#ifdef __cplusplus
};
#endif

#endif /* _H_AT_PARSER_HSP_HFP_ */


