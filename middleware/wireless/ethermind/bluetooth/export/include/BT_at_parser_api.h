
/**
 *  \file BT_at_parser_api.h
 *
 *  This header file describes various definitions from
 *  AT Parser module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_AT_PARSER_API_
#define _H_BT_AT_PARSER_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
/**
 * \defgroup at_parser_module  AT Parser
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  AT Parser module to the Application and other upper layers of the stack.
 *
 */
/**
 * \defgroup  at_parser_defines Defines
 * \{
 * Describes defines for AT Parser module.
 */
/**
 * \defgroup at_parser_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* Maximum number of paramters */
/**
 *  @name AT Maximum number of parameters
 *
 *   Constant Definition for maximum number of parameters
 */
#define AT_MAX_NUM_PARAM  15

/* AT string type */
/**
 *  @name AT string type
 *
 *   Constant Definitions for AT string types
 */
#define AT_INVALID_TYPE           0x00
#define AT_COMMAND_TYPE           0x01
#define AT_RESPONSE_TYPE          0x02

/* Invalid keyword */
/**
 *  @name AT Invalid keyword
 *
 *   Constant Definitions for AT Invalid keyword
 */
#define AT_INVALID_KEYWORD        0xFF

/* HFP Unit Token Defines - Generated */
/**
 *   Constant Definitions for generated HFP Unit Token Defines
 */
#define AT_U_START_TOKEN_ID    0x0080

#define AT_CALL                 (AT_U_START_TOKEN_ID + 0x00)
#define ATERROR                 (AT_U_START_TOKEN_ID + 0x01)
#define ATOK                    (AT_U_START_TOKEN_ID + 0x02)
#define ATNO_CARRIER            (AT_U_START_TOKEN_ID + 0x03)
#define ATBUSY                  (AT_U_START_TOKEN_ID + 0x04)
#define ATNO_ANSWER             (AT_U_START_TOKEN_ID + 0x05)
#define ATDELAYED               (AT_U_START_TOKEN_ID + 0x06)
#define ATBLACKLISTED           (AT_U_START_TOKEN_ID + 0x07)
#define ATRING                  (AT_U_START_TOKEN_ID + 0x08)
#define AT_PL_CCWA_CL_          (AT_U_START_TOKEN_ID + 0x09)
#define AT_PL_CIND_CL_          (AT_U_START_TOKEN_ID + 0x0A)
#define AT_PL_CLCC_CL_          (AT_U_START_TOKEN_ID + 0x0B)
#define AT_PL_CHLD_RSP_CL_      (AT_U_START_TOKEN_ID + 0x0C)
#define AT_PL_COPS_CL_          (AT_U_START_TOKEN_ID + 0x0D)
#define AT_PL_CME_ERROR_CL_     (AT_U_START_TOKEN_ID + 0x0E)
#define AT_PL_CLIP_CL_          (AT_U_START_TOKEN_ID + 0x0F)
#define AT_PL_CIEV_CL_          (AT_U_START_TOKEN_ID + 0x10)
#define AT_PL_CNUM_CL_          (AT_U_START_TOKEN_ID + 0x11)
#define AT_PL_BINP_CL_          (AT_U_START_TOKEN_ID + 0x12)
#define AT_PL_BVRA_CL_          (AT_U_START_TOKEN_ID + 0x13)
#define AT_PL_BRSF_CL_          (AT_U_START_TOKEN_ID + 0x14)
#define AT_PL_VGM_CL_           (AT_U_START_TOKEN_ID + 0x15)
#define AT_PL_VGS_CL_           (AT_U_START_TOKEN_ID + 0x16)
#define AT_PL_VGM_EQ            (AT_U_START_TOKEN_ID + 0x17)
#define AT_PL_VGS_EQ            (AT_U_START_TOKEN_ID + 0x18)
#define AT_PL_BSIR_CL_          (AT_U_START_TOKEN_ID + 0x19)
#define AT_PL_BTRH_CL_          (AT_U_START_TOKEN_ID + 0x1A)
#define AT_PL_BCS_CL_           (AT_U_START_TOKEN_ID + 0x1B)
#define AT_PL_BIND_CL_          (AT_U_START_TOKEN_ID + 0x1C)
#define AT_CALLSETUP            (AT_U_START_TOKEN_ID + 0x1D)
#define AT_CALL_SETUP           (AT_U_START_TOKEN_ID + 0x1E)
#define AT_SERVICE              (AT_U_START_TOKEN_ID + 0x1F)
#define AT_SIGNAL               (AT_U_START_TOKEN_ID + 0x20)
#define AT_ROAM                 (AT_U_START_TOKEN_ID + 0x21)
#define AT_BATTCHG              (AT_U_START_TOKEN_ID + 0x22)
#define AT_CALLHELD             (AT_U_START_TOKEN_ID + 0x23)


/* HFP Unit Sub-Token Defines - Not generated */

/*
 * NOTE: Update this w.r.to the last token ID above
 */
/**
 *
 *   Constant Definitions for generated HFP Unit Tokens
 */
#define AT_END_TOKEN_DEFINE     (AT_U_START_TOKEN_ID + 0x24)

/* CIND Sub-Tokens */
/**
 *   \name  CIND Sub-Tokens
 *
 *   Constant Definitions for CIND Sub-Tokens
 */
#define AT_PL_CIND_TEST_        (AT_END_TOKEN_DEFINE + 0x00)
#define AT_PL_CIND_READ_        (AT_END_TOKEN_DEFINE + 0x01)
/* AT command/response terminator */
/**
 *   \name  AT command/response terminator
 *
 *   Constant Definitions for AT command/response terminator
 */
#define AT_CRLF                 (AT_END_TOKEN_DEFINE + 0x0A)
/* BIND Sub-Tokens */
/**
 *   \name  AT command/response terminator
 *
 *   Constant Definitions for BIND Sub-Token
 */
#define AT_PL_BIND_TEST_        (AT_END_TOKEN_DEFINE + 0x0B)
#define AT_PL_BIND_READ_        (AT_END_TOKEN_DEFINE + 0x0C)

/* HFP AG Token Defines - Generated */
/**
 *   \name  HFP AG Token Defines - Generated
 *
 *   Constant Definitions for HFP AG Token Defines (Generated)
 */
#define AT_G_START_TOKEN_ID    0x0043

#define ATA                     (AT_G_START_TOKEN_ID + 0x00)
#define ATDM                    (AT_G_START_TOKEN_ID + 0x01)
#define AT_CCWA                 (AT_G_START_TOKEN_ID + 0x02)
#define AT_CHLD_TEST            (AT_G_START_TOKEN_ID + 0x03)
#define AT_CHUP                 (AT_G_START_TOKEN_ID + 0x04)
#define AT_CIND_TEST            (AT_G_START_TOKEN_ID + 0x05)
#define AT_CIND_READ            (AT_G_START_TOKEN_ID + 0x06)
#define AT_CLCC                 (AT_G_START_TOKEN_ID + 0x07)
#define AT_COPS                 (AT_G_START_TOKEN_ID + 0x08)
#define AT_COPS_READ            (AT_G_START_TOKEN_ID + 0x09)
#define AT_CMEE                 (AT_G_START_TOKEN_ID + 0x0A)
#define AT_CLIP                 (AT_G_START_TOKEN_ID + 0x0B)
#define AT_CMER                 (AT_G_START_TOKEN_ID + 0x0C)
#define AT_VTS                  (AT_G_START_TOKEN_ID + 0x0D)
#define AT_CNUM_TEST            (AT_G_START_TOKEN_ID + 0x0E)
#define AT_BIA                  (AT_G_START_TOKEN_ID + 0x0F)
#define AT_BINP                 (AT_G_START_TOKEN_ID + 0x10)
#define AT_BLDN                 (AT_G_START_TOKEN_ID + 0x11)
#define AT_BVRA                 (AT_G_START_TOKEN_ID + 0x12)
#define AT_BRSF                 (AT_G_START_TOKEN_ID + 0x13)
#define AT_NREC                 (AT_G_START_TOKEN_ID + 0x14)
#define AT_VGM                  (AT_G_START_TOKEN_ID + 0x15)
#define AT_VGS                  (AT_G_START_TOKEN_ID + 0x16)
#define AT_BTRH                 (AT_G_START_TOKEN_ID + 0x17)
#define AT_BTRH_READ            (AT_G_START_TOKEN_ID + 0x18)
#define AT_BCC                  (AT_G_START_TOKEN_ID + 0x19)
#define AT_BCS                  (AT_G_START_TOKEN_ID + 0x1A)
#define AT_BAC                  (AT_G_START_TOKEN_ID + 0x1B)
#define AT_BIND_TEST            (AT_G_START_TOKEN_ID + 0x1C)
#define AT_BIND_READ            (AT_G_START_TOKEN_ID + 0x1D)
#define AT_BIEV                 (AT_G_START_TOKEN_ID + 0x1E)
#define ATD                     (AT_G_START_TOKEN_ID + 0x1F)
#define AT_CHLD                 (AT_G_START_TOKEN_ID + 0x20)
#define AT_CNUM                 (AT_G_START_TOKEN_ID + 0x21)
#define AT_BIND                 (AT_G_START_TOKEN_ID + 0x22)
/** \} */
/** \} */
/** \} */

/**
 *  \addtogroup at_parser_defines Defines
 *  \{
 */

/**
 * \addtogroup at_parser_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/** AT Parser Parameters */
typedef struct
{
    /** Starting index of parameter in the AT string buffer */
    UINT16  start_of_value_index;

    /** Length of parameter */
    UINT16 value_length;

    /** Parameter type, mandatory or optional */
    UCHAR  param_type;

    /** Nesting level in case of optional parameter */
    UCHAR  optional_nesting_level;
}AT_PARAM;

/** AT Parser Response */
typedef struct
{
    /** AT string buffer */
    UCHAR *global_at_str;

    /** Length up to which the AT string is parsed */
    UINT16    length_parsed;

    /** Index to the string buffer */
    UINT16 global_current_index;

    /** AT string length */
    UINT16 global_at_str_len;

    /** Length of AT string parsed */
    UINT16 global_length_parsed;

    /**
     * AT Buffer size:
     * In case of circular buffer it is the buffer size.
     * Otherwise it should be the length of passed AT string
     */
    UINT16 global_buffer_size;

    /** Identify AT_COMMAND_TYPE or AT_RESPONSE_TYPE */
    UCHAR     at_cmd_rsp;

    /** AT command keyword type */
    UCHAR     keyword_type;

    /** Total Number of Params found */
    UCHAR     number_of_params;

    /** Parameter count */
    UCHAR global_param_count;

    /** List of parameters */
    AT_PARAM  param[AT_MAX_NUM_PARAM];

}AT_PARSER_RESPONSE;
/** \} */
/** \} */
/* ------------------------------------------Function Declarations */

/**
 * \defgroup at_parser_api_defs API Definitions
 * \{
 * This section describes the EtherMind AT Parser APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
/** Interface Function - to parse AT String */
API_RESULT BT_at_parser
           (
               /* IN */     UCHAR              *at_string,
               /* IN */     UINT16             start_index,
               /* IN */     UINT16             buffer_size,
               /* INOUT */  UINT16             *length,
               /* OUT */    AT_PARSER_RESPONSE *parser_response
           );

/** Interface Function - to identify a keyword */
API_RESULT BT_at_find_at_keyword_type
           (
               AT_PARSER_RESPONSE *parser_response
           );

/** Interface Function - to parse AT String */
API_RESULT BT_ag_at_parser
           (
               /* IN */     UCHAR              *at_string,
               /* IN */     UINT16             start_index,
               /* IN */     UINT16             buffer_size,
               /* INOUT */  UINT16             *length,
               /* OUT */    AT_PARSER_RESPONSE *parser_response
           );

/** Interface Function - to identify a keyword */
API_RESULT BT_ag_at_find_at_keyword_type
           (
               AT_PARSER_RESPONSE *parser_response
           );

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */
#endif /* _H_BT_AT_PARSER_API_ */

