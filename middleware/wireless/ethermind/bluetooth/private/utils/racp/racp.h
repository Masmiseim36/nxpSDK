/**
 *  \file racp.h
 *
 *  Header File for Record Access Control Point module.
 */

/*
 *  Copyright (C) 2017. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_RACP_
#define _H_RACP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_racp.h"

/* ----------------------------------------- Global Definitions */

#define RACP_TRC(...)     BT_debug_trace(BT_MODULE_ID_RACP,__VA_ARGS__)
#define RACP_INF(...)     BT_debug_info(BT_MODULE_ID_RACP,__VA_ARGS__)
#define RACP_ERR(...)     BT_debug_error(BT_MODULE_ID_RACP, __VA_ARGS__)

/* RACP Response packet size */
/* #define RACP_RESPONSE_DATA_SIZE                 4 */

/* RACP Response response timeout */
#define RACP_RESPONSE_TIME_OUT                   2

/* RACP Opcodes */
/* Report Stored Records Request Opcode */
#define RACP_REPORT_STORED_RECORD               0x01

/* Delete Stored Records Request Opcode */
#define RACP_DELETE_STORED_RECORD               0x02

/* Abort Operation Request Opcode */
#define RACP_ABORT_OPERATION                    0x03

/* Report Number of Stored Records Request Opcode */
#define RACP_REPORT_NUMBER_OF_STORED_RECORD     0x04

/* Number of Stored Records Response Opcode */
#define RACP_NUMBER_OF_STORED_RECORD_RESPONSE   0x05

/* RACP Operators */
/* Null Operator */
#define RACP_NULL_OPERATOR                      0x00

/* All Record Operator */
#define RACP_ALL_RECORD_OPERATOR                0x01

/* Less than or Equal to Operator */
#define RACP_LESS_THAN_EQUAL_TO_OPERATOR        0x02

/* Greater than or Equal to Operator */
#define RACP_GREATER_THAN_EQUAL_TO_OPERATOR     0x03

/* Within Range of (inclusive) Operator */
#define RACP_WITHIN_RANGE_OF_OPERATOR           0x04

/* First(Oldest Record) Operator */
#define RACP_FIRST_RECORD_OPERATOR              0x05

/*  Last(Latest Record) Operator */
#define RACP_LAST_RECORD_OPERATOR               0x06

#define RACP_GET_OPCODE(dest,src)\
        BT_UNPACK_LE_1_BYTE\
        (\
            (dest), \
            &(src[0])\
        );

#define RACP_GET_OPERATOR(dest,src)\
        BT_UNPACK_LE_1_BYTE\
        (\
            (dest), \
            &(src[1])\
        );

#define RACP_GET_OPERAND(dest,src, len)\
        BT_mem_copy\
        (\
            (dest), \
            &(src[2]), \
            (len) - 2 \
        );

#define racp_validate_opcode(opcode)\
        if (! (( RACP_REPORT_NUMBER_OF_STORED_RECORD >= (opcode)) && \
              ( RACP_REPORT_STORED_RECORD <= (opcode))) ) \
        {\
            RACP_ERR("[**ERR**] RACP opcode not supported\n"); \
            return (RACP_ERR_ID | RACP_OP_CODE_NOT_SUPPORTED); \
        }

/* Validate Operator, an unsigned value */
#define racp_validate_operator(oprtr)\
        if (RACP_LAST_RECORD_OPERATOR < (oprtr)) \
        {\
            RACP_ERR("[**ERR**] RACP operator not supported\n"); \
            return (RACP_ERR_ID | RACP_OPERATOR_NOT_SUPPORTED); \
        }

#define RACP_OPERAND_SUPPORTED_OPERATOR(op)\
        (((RACP_WITHIN_RANGE_OF_OPERATOR >= (op)) && \
        (RACP_LESS_THAN_EQUAL_TO_OPERATOR <= (op))) ? BT_TRUE : BT_FALSE)

#define RACP_START_RSP_TIMER(data,retval)\
        (retval) = BT_start_timer\
                   (\
                       &racp_rsp_timer_handle, \
                       RACP_RESPONSE_TIME_OUT, \
                       racp_rsp_timer_expiry_handler, \
                       (data), \
                       sizeof (RACP_EVENT_INFO)\
                   );

/* ----------------------------------------- Structures/Data Types */

typedef struct
{
    /* Data */
    UCHAR           * data;

    /* Data Length */
    UINT16          data_len;

}RACP_EVENT_INFO;


/*---------------------------------------- Function Prototypes */
API_RESULT racp_validate_request
           (
               /* IN */ UCHAR    racp_opcode,
               /* IN */ UCHAR    racp_operator,
               /* IN */ UINT16   data_len
           );

#ifdef BT_RACP
void racp_post_request
     (
        /* IN */ UCHAR               racp_id,
        /* IN */ RACP_REQ_INFO     * racp_req_info,
        /* IN */ UINT16              racp_length
     );
#endif /* BT_RACP */

#endif /* _H_RACP_ */

