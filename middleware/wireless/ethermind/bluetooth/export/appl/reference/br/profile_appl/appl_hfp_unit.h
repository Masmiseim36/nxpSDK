
/**
 *  \file appl_hfp_unit.h
 *
 *  Header file for HFP Unit Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HFP_UNIT_
#define _H_APPL_HFP_UNIT_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hfp_unit_api.h"
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_sdp_api.h"

#include "appl_hci.h"
#include "db_gen.h"

/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Macros */
#define APPL_HFU_INITIAL_GAIN           "7"

#define MAX_CLCC_PARAMS                 8U
#define MAX_CCWA_PARAMS                 4U
#define MAX_COPS_PARAMS                 3U
#define INVALID_VALUE                   0xFFU

/* HFP v1.8 realated */
#define APPL_HFP_UNIT_BRSF_CMD_FEATURE_BITMASK_ENH_VR_STATUS   0x0400U
#define APPL_HFP_UNIT_BRSF_CMD_FEATURE_BITMASK_VR_TEXT         0x0800U

#define APPL_HFP_UNIT_BRSF_RSP_FEATURE_BITMASK_ENH_VR_STATUS   0x1000U
#define APPL_HFP_UNIT_BRSF_RSP_FEATURE_BITMASK_VR_TEXT         0x2000U

/*
 *  The Maximum digits required to represent any Codec Id.
 *
 *  '3', because Codec Id is an 8-bit value and hence the maximum posssible
 *  value of Codec Id is 255.
 */
#define APPL_HFU_MAX_DIGITS_CODEC_ID                    (3U)

/*
 *  The Maximum possible length of the string which lists the available Codecs.
 *
 *  HFP_UNIT_MAX_NUM_AVL_CODECS*(APPL_HFU_MAX_DIGITS_CODEC_ID+1), because
 *  maximum number of codecs in the list will be limited to
 *  HFP_UNIT_MAX_NUM_AVL_CODECS; APPL_HFU_MAX_DIGITS_CODEC_ID digits to
 *  represent each Codec Id, and one space for the comma.
 */
#define APPL_HFU_MAX_AVL_CODEC_ID_LIST_LEN              \
                (HFP_UNIT_MAX_NUM_AVL_CODECS*(APPL_HFU_MAX_DIGITS_CODEC_ID+1))

/*
 *  The Maximum digits required to represent any HF Indicator.
 *
 *  '5', because HF Indicator Id is an 16-bit value and hence the maximum posssible
 *  value of HF Indicator Id is 65535.
 */
#define APPl_HFP_UNIT_MAX_DIGITS_HF_INDICATOR_ID        (5U)

/*
 *  The Maximum possible length of the string which lists the available
 *  HF Indicators.
 *
 *  HFP_UNIT_MAX_HF_INDICATOR_COUNT*(APPl_HFP_UNIT_MAX_DIGITS_HF_INDICATOR_ID+1), because
 *  maximum number of indicators in the list will be limited to
 *  HFP_UNIT_MAX_HF_INDICATOR_COUNT; APPl_HFP_UNIT_MAX_DIGITS_HF_INDICATOR_ID digits to
 *  represent each Indicator Id, and one space for the comma.
 */
#define APPL_HFP_UNIT_HF_INDICATOR_LIST_LEN              \
                (HFP_UNIT_MAX_HF_INDICATOR_COUNT*(APPl_HFP_UNIT_MAX_DIGITS_HF_INDICATOR_ID+1))

/*
 *  Default Voice Settings to be used for Wideband Speech Synchronous Connection:
 *  Input Coding: A-Law
 *  Input Format: 2's Compliment
 *  Input Sample Size: 16-bit
 *  Linear PCM Bit Position: 3
 *  Air Coding Format: TRANSPARENT
 */
#define APPL_WIDEBAND_VOICE_SETTING_DEFAULT \
        (LMP_VOICE_INPUT_CODING_LINEAR|\
         LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT|\
         LMP_VOICE_INPUT_SAMPLE_16_BIT|\
         LMP_VOICE_LINEAR_BIT_POS_0|\
         LMP_VOICE_AIR_CODING_TRANSPARENT)

/*Maximum supporting feature value: 4095
 *
 */
#define HFP_UNIT_MAX_SUPP_FEATURE_RANGE                  4095U


/*Minimum supporting feature value: 1
 *
 */
#define HFP_UNIT_MIN_SUPP_FEATURE_RANGE                  1U

/* ----------------------------------------- Structures/ Data Types */
/* Data Structures to display received responses from AG */
typedef struct
{
    UCHAR   number[HFP_UNIT_DIGIT_ARRAY_SIZE];
    UCHAR   type;
    UCHAR   voice_class;
    UCHAR   alpha[HFP_UNIT_MAX_OPERATOR_NAME_LEN];

} PARSER_CCWA_RESULT;

typedef struct
{
    UCHAR   mode;
    UCHAR   format;
    UCHAR   op_name[HFP_UNIT_MAX_OPERATOR_NAME_LEN];

} PARSER_COPS_RESULT;

typedef struct
{
    UCHAR   number[HFP_UNIT_DIGIT_ARRAY_SIZE];
    UCHAR   type;
    UCHAR   service;

} PARSER_CNUM_RESULT;

typedef struct
{
    UCHAR   id;
    UCHAR   dir;
    UCHAR   status;
    UCHAR   mode;
    UCHAR   mprty;
    UCHAR   number[HFP_UNIT_DIGIT_ARRAY_SIZE];
    UCHAR   type;
    UCHAR   alpha[HFP_UNIT_MAX_OPERATOR_NAME_LEN];
    UCHAR   dummy;

} PARSER_CLCC_RESULT;

typedef struct
{
    UCHAR  digits[HFP_UNIT_DIGIT_ARRAY_SIZE];
    UCHAR  name[HFP_UNIT_DIGIT_ARRAY_SIZE];

} PARSER_CLI_RESULT;

#ifdef HFP_UNIT_1_8
typedef struct _PARSER_BVRA_TEXT
{
    UINT32  text_id;

    UCHAR   text_type;

    UCHAR   text_op;

    UCHAR   str[HFP_UNIT_MAX_ENH_VREC_TEXT_LEN];
} PARSER_BVRA_TEXT;

typedef struct
{
    UCHAR   vrect;

    UCHAR   vrecstate;

    PARSER_BVRA_TEXT  text_rep;
} PARSER_BVRA_RESULT;
#endif /* HFP_UNIT_1_8 */

typedef UINT8 APPL_PARSER_TOKEN_TYPE;

typedef union
{
    PARSER_CCWA_RESULT   ccwa_result;
    PARSER_COPS_RESULT   cops_resp_result;
    PARSER_CNUM_RESULT   cnum_resp_result;
    PARSER_CLCC_RESULT   clcc_resp_result;
    PARSER_CLI_RESULT    cli_info;
#ifdef HFP_UNIT_1_8
    PARSER_BVRA_RESULT   bvra_result;
#endif /* HFP_UNIT_1_8 */
    UCHAR                uchar_result;
    UCHAR                digits[ HFP_UNIT_DIGIT_ARRAY_SIZE ];
} APPL_PARSER_RESPONSE;

typedef struct
{
    APPL_PARSER_TOKEN_TYPE   token;
    APPL_PARSER_RESPONSE     result_param;

} APPL_PARSER_RESULT;


/* ----------------------------------------- Function Declarations */
void main_hfp_unit_operations (void);
void appl_hfpunit_create_eSCO_connection(void);
void appl_hfpunit_change_esco_config(void);
API_RESULT appl_callback_registered_with_hfu
           (
               HFP_UNIT_HANDLE   handle,   /* Connection Instance */
               HFP_UNIT_EVENTS   event,    /* HFP Events          */
               API_RESULT        result,   /* Result of operation */
               void *            data,     /* Data for each event */
               UINT16            data_len  /* Length of Data      */
           );

void appl_hfu_sdp_cb
     (
         UCHAR    command,
         UCHAR  * data,
         UINT16   length,
         UINT16   status
     );

/* Utility function to extract values from parser results */
API_RESULT appl_extract_result
           (
               /* IN */  AT_PARSER_RESPONSE  * parser_response,
               /* IN */  UCHAR               * buffer,
               /* OUT */ APPL_PARSER_RESULT  * parse_result
           );

#ifndef HFP_UNIT_1_6
/* Start HFP Unit */
API_RESULT appl_start_hfu
           (
               UCHAR  * hfp_unit_local_supported_features,
               UINT16   data_length
           );
#else /* HFP_UNIT_1_6 */
/* Start HFP Unit */
API_RESULT appl_start_hfu
           (
               /* IN */  HFP_UNIT_APPL_CONFIG_PARAMS *p_hfp_unit_appl_conf_params
           );
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_5
void appl_hfp_unit_1_5_choice_hdlr( void );
#endif /* HFP_UNIT_1_5 */

#ifdef HFP_UNIT_1_6
void appl_hfp_unit_1_6_choice_hdlr( void );
#endif /* HFP_UNIT_1_6 */

#endif /* _H_APPL_HFP_UNIT_ */

