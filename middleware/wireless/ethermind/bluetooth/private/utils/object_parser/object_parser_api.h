
/**
 *  \file object_parser_api.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_OBJECT_PARSER_API_
#define _H_OBJECT_PARSER_API_

/* --------------------------------------------- Header File Inclusion */
#include "tokenizer.h"

/* --------------------------------------------- Global Definitions */

/* Definitions of Parser States */
#define OBJECT_STATE_START_OF_LINE               0x0001
#define OBJECT_STATE_GROUP_FOUND                 0x0002
#define OBJECT_STATE_START_OF_PROP_VAL           0x0004
#define OBJECT_STATE_START_OF_PROP_PARAM_NAME    0x0008
#define OBJECT_STATE_START_OF_PROP_PARAM_VAL     0x0010

/* Delimter states */
#define STATE_0             0x0002
#define STATE_1             0x0004
#define STATE_2             0x0008
#define STATE_3             0x0010

/* Parser Action definitions */
#define ACTION_PARSE_GROUP_NAME            0x0001
#define ACTION_PARSE_PROPERTY_NAME         0x0002
#define ACTION_PARSE_PROPERTY_PARAM_NAME   0x0004
#define ACTION_PARSE_PROPERTY_PARAM_VAL    0x0008
#define ACTION_PARSE_PROPERTY_VAL          0x0010
#define ACTION_PARSE_SKIP                  0x0020
#define ACTION_ERROR_DETECTED              0x0040

/* Parser Result code */
#define OBJECT_RESULT_SUCCESS                    0x0000
#define OBJECT_RESULT_PARTIAL_PARSED             0x0002
#define OBJECT_RESULT_UNKNOWN_TOK                0x0004
#define OBJECT_RESULT_ERROR                      0x0008

/*  Definition of Additional token ids Token IDs */
#define OBJECT_TOKEN_ID_STRING                   0x0001
#define OBJECT_TOKEN_ID_VAL                      0x0002
#define OBJECT_TOKEN_ID_ERROR                    0x0003

/* Definition of TOKEN types */
#define OBJECT_TOKEN_GRP_NAME                    0x0001
#define OBJECT_TOKEN_PROP_NAME                   0x0002
#define OBJECT_TOKEN_PROP_PARAM_NAME             0x0003
#define OBJECT_TOKEN_PROP_PARAM_VAL              0x0004
#define OBJECT_TOKEN_PROP_VAL                    0x0005
#define OBJECT_TOKEN_ERROR                       0x0006

/* Definition of delimiter ID */
#define CRLF_ID                 0x0A
#define DOT                     '.'
#define COLON                   ':'
#define SEMI_COLON              ';'
#define EQUAL                   '='


/* Below token definitions are generated using Jump Table scripts */

#define AT_START_TOKEN_ID       0x00FF

#define BEGIN                   (AT_START_TOKEN_ID + 0x00)
#define VCARD                   (AT_START_TOKEN_ID + 0x01)
#define END                     (AT_START_TOKEN_ID + 0x02)
#define ADR                     (AT_START_TOKEN_ID + 0x03)
#define ORG                     (AT_START_TOKEN_ID + 0x04)
#define AGENT                   (AT_START_TOKEN_ID + 0x05)
#define LOGO                    (AT_START_TOKEN_ID + 0x06)
#define PHOTO                   (AT_START_TOKEN_ID + 0x07)
#define LABEL                   (AT_START_TOKEN_ID + 0x08)
#define FN                      (AT_START_TOKEN_ID + 0x09)
#define TITLE                   (AT_START_TOKEN_ID + 0x0A)
#define SOUND                   (AT_START_TOKEN_ID + 0x0B)
#define VERSION                 (AT_START_TOKEN_ID + 0x0C)
#define TEL                     (AT_START_TOKEN_ID + 0x0D)
#define EMAIL                   (AT_START_TOKEN_ID + 0x0E)
#define TZ                      (AT_START_TOKEN_ID + 0x0F)
#define GEO                     (AT_START_TOKEN_ID + 0x10)
#define NOTE                    (AT_START_TOKEN_ID + 0x11)
#define URL                     (AT_START_TOKEN_ID + 0x12)
#define BDAY                    (AT_START_TOKEN_ID + 0x13)
#define ROLE                    (AT_START_TOKEN_ID + 0x14)
#define REV                     (AT_START_TOKEN_ID + 0x15)
#define UID                     (AT_START_TOKEN_ID + 0x16)
#define KEY                     (AT_START_TOKEN_ID + 0x17)
#define MAILER                  (AT_START_TOKEN_ID + 0x18)
#define X_IRMC_CALL_DATETIME    (AT_START_TOKEN_ID + 0x19)
#define MISSED                  (AT_START_TOKEN_ID + 0x1A)
#define RECEIVED                (AT_START_TOKEN_ID + 0x1B)
#define DIALED                  (AT_START_TOKEN_ID + 0x1C)
#define TYPE                    (AT_START_TOKEN_ID + 0x1D)
#define VALUE                   (AT_START_TOKEN_ID + 0x1E)
#define ENCODING                (AT_START_TOKEN_ID + 0x1F)
#define CHARSET                 (AT_START_TOKEN_ID + 0x20)
#define LANGUAGE                (AT_START_TOKEN_ID + 0x21)
#define IN_LINE                 (AT_START_TOKEN_ID + 0x22)
#define CONTENT_ID              (AT_START_TOKEN_ID + 0x23)
#define CID                     (AT_START_TOKEN_ID + 0x24)
#define BIT_7                   (AT_START_TOKEN_ID + 0x25)
#define BIT_8                   (AT_START_TOKEN_ID + 0x26)
#define QUOTED_PRINTABLE        (AT_START_TOKEN_ID + 0x27)
#define BASE64                  (AT_START_TOKEN_ID + 0x28)
#define DOM                     (AT_START_TOKEN_ID + 0x29)
#define INTL                    (AT_START_TOKEN_ID + 0x2A)
#define POSTAL                  (AT_START_TOKEN_ID + 0x2B)
#define PARCEL                  (AT_START_TOKEN_ID + 0x2C)
#define HOME                    (AT_START_TOKEN_ID + 0x2D)
#define WORK                    (AT_START_TOKEN_ID + 0x2E)
#define BBS                     (AT_START_TOKEN_ID + 0x2F)
#define MODEM                   (AT_START_TOKEN_ID + 0x30)
#define CAR                     (AT_START_TOKEN_ID + 0x31)
#define ISDN                    (AT_START_TOKEN_ID + 0x32)
#define VIDEO                   (AT_START_TOKEN_ID + 0x33)
#define GIF                     (AT_START_TOKEN_ID + 0x34)
#define BMP                     (AT_START_TOKEN_ID + 0x35)
#define TIFF                    (AT_START_TOKEN_ID + 0x36)
#define PDF                     (AT_START_TOKEN_ID + 0x37)
#define JPEG                    (AT_START_TOKEN_ID + 0x38)
#define PS                      (AT_START_TOKEN_ID + 0x39)
#define MPEG2                   (AT_START_TOKEN_ID + 0x3A)
#define AVI                     (AT_START_TOKEN_ID + 0x3B)
#define WAVE                    (AT_START_TOKEN_ID + 0x3C)
#define PCM                     (AT_START_TOKEN_ID + 0x3D)
#define X509                    (AT_START_TOKEN_ID + 0x3E)
#define PGP                     (AT_START_TOKEN_ID + 0x3F)
#define LENGTH                  (AT_START_TOKEN_ID + 0x40)
#define DATE                    (AT_START_TOKEN_ID + 0x41)
#define SUBJECT                 (AT_START_TOKEN_ID + 0x42)
#define FROM                    (AT_START_TOKEN_ID + 0x43)
#define TO                      (AT_START_TOKEN_ID + 0x44)
#define SMS_GSM                 (AT_START_TOKEN_ID + 0x45)
#define SMS_CDMA                (AT_START_TOKEN_ID + 0x46)
#define MMS                     (AT_START_TOKEN_ID + 0x47)
#define MSG                     (AT_START_TOKEN_ID + 0x48)
#define BMSG                    (AT_START_TOKEN_ID + 0x49)
#define BENV                    (AT_START_TOKEN_ID + 0x4A)
#define READ                    (AT_START_TOKEN_ID + 0x4B)
#define UNREAD                  (AT_START_TOKEN_ID + 0x4C)
#define STATUS                  (AT_START_TOKEN_ID + 0x4D)
#define FOLDER                  (AT_START_TOKEN_ID + 0x4E)
#define X_BT_SPEEDDIALKEY       (AT_START_TOKEN_ID + 0x4F)
#define X_BT_UCI                (AT_START_TOKEN_ID + 0x50)
#define X_BT_UID                (AT_START_TOKEN_ID + 0x51)
#define N                       (AT_START_TOKEN_ID + 0x52)
#define MPEG                    (AT_START_TOKEN_ID + 0x53)

/* Error Redirection */
#ifndef OBJECT_PARSER_NO_DEBUG
#define OBJECT_PARSER_ERR(...)  BT_debug_error(BT_MODULE_ID_OBJECT_PARSER, __VA_ARGS__)
#else  /* OBJECT_PARSER_NO_DEBUG */
#define OBJECT_PARSER_ERR       BT_debug_null
#endif /* OBJECT_PARSER_NO_DEBUG */

/* --------------------------------------------- Structures/Data Types */

typedef UINT16 PARSER_STATE;

typedef struct _PARSER_CONTEXT_STRUCT_   * POBJECT_PARSER_CONTEXT;

typedef struct _PARSER_CALLBACK_STRUCT_
{
    /* Parser Result */
    UINT16    parser_result;

    /**
     *  Token/Keyword Type
     *  A token can either be 1: Property name, 2: Property parameter,
     *  3: Property value sequence, 4: Group,
     */
    UCHAR    keyword_type;

    /* Token/Keyword ID */

    UINT16   keyword_id;

    /**
     *  Pointer to start of the buffer containing the value of property
     *  The pointer to the value points to the input buffer of data stream
     *  provided by the user to the parser at a offset where the value of the
     *  property is found.
     */
    UCHAR    * value;

    /**
     *  Property value length - this may not always be the total length of the
     *  value but actually is the lenght of the value avilable in the input
     *  buffer
     */
    UINT16     value_len;

} OBJECT_PARSER_CB_PARAM;

/* Parser Application callback typedef */
typedef void (* OBJECT_PARSER_CB)
             (
                 OBJECT_PARSER_CB_PARAM cb_param,
                 POBJECT_PARSER_CONTEXT context
             );

/* Type definition of the parser context */
typedef struct _PARSER_CONTEXT_STRUCT_
{
    /**
     * The following members of the context structure are not to be used or
     * interpreted by the user, these members are used by the parser to
     * maintain the state information for parsing between the consicutive calls
     * of the parser with the same context.
     */

    /* Current state of the parser */
    PARSER_STATE    parser_state;

    /* Current state of the delimiter find state machine */
    UINT16 delim_state;

    /* Current Parsed length of the buffer */
    UINT16 cur_parsed_length;

    /* Actual parsed till value */
    UINT16 value_till;

    /* Total parsed length */
    UINT16 total_parsed_length;

    /**
     *  The following members of the context structure are to be used by the
     *  caller of the parser to feed input to the parser, configure the parser
     *  for specific requirents, as well as to monitorthe result of parasing
     *  after the call to the parser returns.
     */

    /* Input data stream */
    UCHAR        * buffer;

    /* Input data stream length */
    UINT16         buffer_len;

    /**
     *  Next Parser action - using this parameter the application can
     *  instruct the parser what should it be doing next
     */
    UCHAR         parser_action;

    /* Application callback */
    OBJECT_PARSER_CB    callback;

} OBJECT_PARSER_CONTEXT;

/* --------------------------------------------- Macros */
#define object_parser_get_token_id token_match

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/**
 *  \fn object_parser_init
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param parser_context
 *
 *  \return
 */

UINT16 object_parser_init
       (
           OBJECT_PARSER_CONTEXT * parser_context,
           OBJECT_PARSER_CB        callback
       );

/**
 *  \fn object_parser_parse
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param parser_context
 *
 *  \return
 */

UINT16 object_parser_parse (OBJECT_PARSER_CONTEXT * parser_context);

#if 0
/**
 *  \fn object_parser_unfold
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param in_buffer
 *  \param in_buffer_len
 *  \param out_buffer
 *  \param out_buffer_len
 *
 *  \return
 */
UINT16 object_parser_unfold
       (
           UCHAR  * in_buffer,
           UINT16   in_buffer_len,
           UCHAR  * out_buffer,
           UINT16 * out_buffer_len
       );
#endif /* 0 */

UCHAR get_next_delimiter
      (
          UCHAR   * buf,
          UINT16    buffer_len,
          UINT16  * parsed_len,
          UINT16  * value_till,
          UINT16  * line_fold_flag,
          UINT16  * state
      );

UINT16 get_next_action (UCHAR delim, UINT16  * state);

#endif /* _H_OBJECT_PARSER_API_ */




