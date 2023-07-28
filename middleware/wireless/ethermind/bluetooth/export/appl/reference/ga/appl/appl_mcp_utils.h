/**
 *  \file appl_mcp_utils.h
 *
 *  \brief This file defines the GA MCP profile util methods
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_MCP_UTILS_
#define _H_APPL_MCP_UTILS_

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_mcp_ce_api.h"
#include "GA_mcp_se_api.h"

/* --------------------------------------------- Global Definitions */

/*
 * Application Error Code for Long Read request.
 * To be used when value changes during Long Read Request in Progress.
 */
#define MCP_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG 0x80

/* OTS Application error codes */
/**
 * Write Request Rejected - An attempt was made to write a value that
 * is invalid or not supported by this Server for a reason other than
 * the attribute permissions
 */
#define OTP_ERR_CODE_WRITE_REQUEST_REJECTED                          0x80U

/**
 * Object Not Selected - An attempt was made to read or write to an
 * Object Metadata characteristic while the Current Object
 * was an Invalid Object
 */
#define OTP_ERR_CODE_OBJECT_NOT_SELECTED                             0x81U

/**
 * Concurrency Limit Exceeded - The Server is unable to service the
 * Read Request or Write Request because it exceeds the concurrency
 * limit of the service
 */
#define OTP_ERR_CODE_CONCURRENCY_LIMIT_EXCEEDED                      0x82U

/**
 * Object Name Already Exists - The requested object name was rejected
 * because the name was already in use by an existing object on the Server
 */
#define OTP_ERR_CODE_OBJECT_NAME_ALREADY_EXISTS                      0x83U

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/* Object Access Control Point Commands */
#define MCP_OTS_OACP_CREATE_OPCODE                    0x01
#define MCP_OTS_OACP_DELETE_OPCODE                    0x02
#define MCP_OTS_OACP_CAL_CHECK_SUM_OPCODE             0x03
#define MCP_OTS_OACP_EXECUTE_OPCODE                   0x04
#define MCP_OTS_OACP_READ_OPCODE                      0x05
#define MCP_OTS_OACP_WRITE_OPCODE                     0x06
#define MCP_OTS_OACP_ABORT_OPCODE                     0x07
#define MCP_OTS_OACP_RSP_CODE_OPCODE                  0x60

/* Object Action Control Point Response Value */
#define MCP_OTS_OACP_SUCCESS                          0x01
#define MCP_OTS_OACP_OPCODE_NOT_SUPPORTED             0x02
#define MCP_OTS_OACP_INVALID_PARAMETER                0x03
#define MCP_OTS_OACP_INSUFF_RESOURCES                 0x04
#define MCP_OTS_OACP_INVALID_OBJECT                   0x05
#define MCP_OTS_OACP_CHANNEL_UNAVAILABLE              0x06
#define MCP_OTS_OACP_UNSUPP_TYPE                      0x07
#define MCP_OTS_OACP_PROCEDURE_NOT_PERMITTED          0x08
#define MCP_OTS_OACP_OBJECT_LOCKED                    0x09
#define MCP_OTS_OACP_OPERATION_FAILED                 0x0A

/* Object List Control Point Commands */
#define MCP_OTS_OLCP_FIRST_OPCODE                     0x01
#define MCP_OTS_OLCP_LAST_OPCODE                      0x02
#define MCP_OTS_OLCP_PREVIOUS_OPCODE                  0x03
#define MCP_OTS_OLCP_NEXT_OPCODE                      0x04
#define MCP_OTS_OLCP_GO_TO_OPCODE                     0x05
#define MCP_OTS_OLCP_ORDER_OPCODE                     0x06
#define MCP_OTS_OLCP_REQ_NUM_OF_OBJ_OPCODE            0x07
#define MCP_OTS_OLCP_CLEAR_MARKING_OPCODE             0x08
#define MCP_OTS_OLCP_RSP_CODE_OPCODE                  0x70

/* Object List Control Point Response Value */
#define MCP_OTS_OLCP_SUCCESS                          0x01
#define MCP_OTS_OLCP_OPCODE_NOT_SUPPORTED             0x02
#define MCP_OTS_OLCP_INVALID_PARAMETER                0x03
#define MCP_OTS_OLCP_OPERATION_FAILED                 0x04
#define MCP_OTS_OLCP_OUT_OF_BOUNDS                    0x05
#define MCP_OTS_OLCP_TOO_MANY_OBJECTS                 0x06
#define MCP_OTS_OLCP_NO_OBJECT                        0x07
#define MCP_OTS_OLCP_OBJ_ID_NOT_FOUND                 0x08

/* Object List Filter Value */
/* No Filter (everything passes) */
#define MCP_OTS_OBJ_LIST_FILTER_NO_FILTER                       0x00
/* Name Starts With */
#define MCP_OTS_OBJ_LIST_FILTER_NAME_STARTS_WITH                0x01
/* Name Ends With */
#define MCP_OTS_OBJ_LIST_FILTER_NAME_ENDS_WITH                  0x02
/* Name Contains */
#define MCP_OTS_OBJ_LIST_FILTER_NAME_CONTAINS                   0x03
/* Name is Exactly */
#define MCP_OTS_OBJ_LIST_FILTER_NAME_IS_EXACTLY                 0x04
/* Object Type (UUID) */
#define MCP_OTS_OBJ_LIST_FILTER_OBJ_TYPE                        0x05
/* Created between (inclusive): timestamp1 <= t <= timestamp2 */
#define MCP_OTS_OBJ_LIST_FILTER_CREAT_BTWN                      0x06
/* Modified between (inclusive): timestamp1 <= t <= timestamp2 */
#define MCP_OTS_OBJ_LIST_FILTER_MODIFY_BTWN                     0x07
/* Current Size between (inclusive): size1 <= s <= size2 */
#define MCP_OTS_OBJ_LIST_FILTER_CURR_SIZE_BTWN                  0x08
/* Allocated Size between (inclusive): size1 <= s <= size2 */
#define MCP_OTS_OBJ_LIST_FILTER_ALLOC_SIZE_BTWN                 0x09
/* Marked Objects */
#define MCP_OTS_OBJ_LIST_FILTER_MARKED_OBJS                     0x0A

/* OACP Features */
/* OACP Create Op Code Supported */
#define MCP_OTS_OACP_CREATE_OPC_SUPPORTED                       (0x00000001 << 0U)
/* OACP Delete Op Code Supported */
#define MCP_OTS_OACP_DELETE_OPC_SUPPORTED                       (0x00000001 << 1U)
/* OACP Calculate Checksum Op Code Supported */
#define MCP_OTS_OACP_CAL_CHECKSUM_OPC_SUPPORTED                 (0x00000001 << 2U)
/* OACP Execute Op Code Supported */
#define MCP_OTS_OACP_EXECUTE_OPC_SUPPORTED                      (0x00000001 << 3U)
/* OACP Read Op Code Supported */
#define MCP_OTS_OACP_READ_OPC_SUPPORTED                         (0x00000001 << 4U)
/* OACP Write Op Code Supported */
#define MCP_OTS_OACP_WRITE_OPC_SUPPORTED                        (0x00000001 << 5U)
/* Appending Additional Data to Objects Supported */
#define MCP_OTS_OACP_APPEND_ADDNL_DATA_TO_OBJS_SUPPORTED        (0x00000001 << 6U)
/* Truncation of Objects Supported */
#define MCP_OTS_OACP_TRUNC_OF_OBJS_SUPPORTED                    (0x00000001 << 7U)
/* Patching of Objects Supported */
#define MCP_OTS_OACP_PATCH_OF_OBJS_SUPPORTED                    (0x00000001 << 8U)
/* OACP Abort Op Code Supported */
#define MCP_OTS_OACP_ABORT_OPC_SUPPORTED                        (0x00000001 << 9U)

/* OLCP Features */
/* OLCP Go To Op Code Supported */
#define MCP_OTS_OLCP_GO_TO_OPC_SUPPORTED                        (0x00000001 << 0U)
/* OLCP Order Op Code Supported */
#define MCP_OTS_OLCP_ORDER_OPC_SUPPORTED                        (0x00000001 << 1U)
/* OLCP Request Number of Objects Op Code Supported */
#define MCP_OTS_OLCP_REQ_NUM_OF_OBJS_OPC_SUPPORTED              (0x00000001 << 2U)
/* OLCP Clear Marking Op Code Supported */
#define MCP_OTS_OLCP_CLEAR_MARKING_OPC_SUPPORTED                (0x00000001 << 3U)

/* Object Properties */
/* Delete: Deletion of this object is permitted */
#define MCP_OTS_OBJ_PROP_DELETE                                 (0x00000001 << 0U)
/* Execute: Execution of this object is permitted */
#define MCP_OTS_OBJ_PROP_EXECUTE                                (0x00000001 << 1U)
/* Read: Reading this object is permitted */
#define MCP_OTS_OBJ_PROP_READ                                   (0x00000001 << 2U)
/* Write: Writing data to this object is permitted */
#define MCP_OTS_OBJ_PROP_WRITE                                  (0x00000001 << 3U)
/*
 * Append: Appending data to this object that increases its
 * Allocated Size is permitted
 */
#define MCP_OTS_OBJ_PROP_APPEND                                 (0x00000001 << 4U)
/* Truncate: Truncation of this object is permitted */
#define MCP_OTS_OBJ_PROP_TRUNCATE                               (0x00000001 << 5U)
/*
 * Patch: Patching this object by overwriting some of the
 * object's existing contents is permitted
 */
#define MCP_OTS_OBJ_PROP_PATCH                                  (0x00000001 << 6U)
/* Mark: This object is a marked object */
#define MCP_OTS_OBJ_PROP_MARK                                   (0x00000001 << 7U)

/* Object Changed characteristics - Flag Field values */
/*
 * Source of Change
 * 0: Server
 * 1: Client
 */
#define MCP_OTS_OBJ_CHGD_FLAG_SOURCE_OF_CHNG                    (0x00000001 << 0U)
/* Change occurred to the object contents */
#define MCP_OTS_OBJ_CHGD_FLAG_OBJ_CONTENT_CHNG                  (0x00000001 << 1U)
/* Change occurred to the object metadata */
#define MCP_OTS_OBJ_CHGD_FLAG_OBJ_METADATA_CHNG                 (0x00000001 << 2U)
/* Object Creation */
#define MCP_OTS_OBJ_CHGD_FLAG_OBJ_CREATION                      (0x00000001 << 3U)
/* Object Deletion */
#define MCP_OTS_OBJ_CHGD_FLAG_OBJ_DELETION                      (0x00000001 << 4U)

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/* --------------------------------------------- Functions */
/* Parser for printing Playing order */
void appl_mcp_utils_parse_and_display_playing_order(UINT8 playing_order);

/* Parser for printing Playing orders supported */
void appl_mcp_utils_parse_and_display_playing_orders_supp(UINT16 playing_order_supp);

/* Parser for printing Media state */
void appl_mcp_utils_parse_and_display_media_state(UINT8 media_state);

/* Parser for printing Media Control Point opcode supported */
void appl_mcp_utils_parse_and_display_mcp_opc_supp(UINT32 mcp_opc_supp);

/* Parser for printing MCP GA Result */
void appl_mcp_utils_display_ga_result(GA_RESULT result, UINT16 svc_type);

/* Parser for printing OTS related Error codes */
void appl_mcp_utils_parse_and_display_ots_err_codes(GA_RESULT result);

/* Parser for printing MCS related Error codes */
void appl_mcp_utils_parse_and_display_mcs_err_codes(GA_RESULT result);

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* Parser for printing Object type */
void appl_mcp_utils_parse_and_display_object_type(void * object_type);

/* Parser for printing Object Property */
void appl_mcp_utils_parse_and_display_obj_prop(void * data, UINT16 datalen);

/* Parser for printing List Filter Values */
void appl_mcp_utils_parse_and_display_list_filter(UINT8 * data, UINT16 datalen);

/* Parser for printing Object Changed Flags */
void appl_mcp_utils_parse_and_display_obj_chgd_flag_bit(UINT8 flag);

/* Parser for printing OACP features */
void appl_mcp_par_and_display_oacp_feature(UINT32 oacp_feature);

/* Parser for printing OLCP features */
void appl_mcp_par_and_display_olcp_feature(UINT32 olcp_feature);

/* Parser for printing OACP */
void appl_mcp_utils_parse_and_display_oacp(UINT8 * data);

/* Parser for printing OACP Result code */
void appl_mcp_utils_parse_and_display_oacp_rsp_result(UINT8 result_code);

/* Parser for printing OLCP */
void appl_mcp_utils_parse_and_display_olcp(UINT8 * data, UINT16 datalen);

/* Parser for printing OLCP Result code */
void appl_mcp_utils_parse_and_display_olcp_rsp_result(UINT8 result_code);

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

#endif /* _H_APPL_MCP_UTILS_ */
