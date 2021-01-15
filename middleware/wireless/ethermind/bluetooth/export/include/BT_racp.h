
/**
 *  \file BT_racp.h
 *
 *  \brief This file defines the RACP Application Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_RACP_
#define _H_BT_RACP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */
/* RACP Response Codes */
/**
 *  Success respose code is used when the procedure requested by the sensor
 *  is successfully completed.
 */
#define RACP_SUCCESS                              0x01

/**
 *  Opcode not supported response code is used when the handling of requested
 *  opcode by the sensor is not supported.
 */
#define RACP_OP_CODE_NOT_SUPPORTED                0x02

/**
 *  Invalid operator response code is used when the requested operator by the
 *  sensor is invalid or not defined by the specification.
 */
#define RACP_INVALID_OPERATOR                     0x03

/**
 *  Operator not supported response code is used when the handling of requested
 *  operator by the sensor is not supported.
 */
#define RACP_OPERATOR_NOT_SUPPORTED               0x04

/**
 *  Invalid operand response code is used when the requested operand filter
 *  type by the sensor is invalid or not defined by the specification.
 */
#define RACP_INVALID_OPERAND                      0x05

/**
 *  No records found response code is used when the sensor does not have any
 *  stored records that meet the specified criteria.
 */
#define RACP_NO_RECORDS_FOUND                     0x06

/**
 *  Abort unsuccessful response code is used when the abort operation
 *  requested by the sensor cannot be processed.
 */
#define RACP_ABORT_UNSUCCESSFUL                   0x07

/**
 *  Procedure incomplete response code is used when the procedure requested
 *  by the sensor was required to interrupt before completion for an
 *  unspecified reason.
 */
#define RACP_PROCEDURE_INCOMPLETE                 0x08

/**
 *  Operand not supported response code is used when the handling of requested
 *  operand filter type by the sensor is not supported.
 */
#define RACP_OPERAND_NOT_SUPPORTED                0x09


/* ----------------------------------------- Structures/Data Types */
typedef struct
{
    UCHAR           racp_opcode;

    UCHAR           racp_operator;

    UCHAR           racp_operand[20];

}RACP_REQ_INFO;

typedef API_RESULT(*RACP_REPORT_STORED_RECORDS_HANDLER)(RACP_REQ_INFO * req_info, UINT16 racp_length) DECL_REENTRANT;
typedef API_RESULT(*RACP_NUM_OF_RECORDS_HANDLER)(RACP_REQ_INFO * req_info, UINT16 racp_length) DECL_REENTRANT;
typedef API_RESULT(*RACP_DELETE_STORED_RECORD_HANDLER)(RACP_REQ_INFO * req_info, UINT16 racp_length) DECL_REENTRANT;
typedef API_RESULT(*RACP_ABORT_OPERATION_HANDLER)(RACP_REQ_INFO * req_info, UINT16 racp_length) DECL_REENTRANT;

/**
 *  The structure representing the RACP procedure hanlders. It stores all the callback
 *  functions. The upper layer uses this structure to register itself with RACP.
 */
typedef struct racp_module_t
{
    RACP_REPORT_STORED_RECORDS_HANDLER     report_stored_records_hndlr;

    RACP_NUM_OF_RECORDS_HANDLER            num_of_records_hndlr;

    RACP_DELETE_STORED_RECORD_HANDLER      delete_stored_records_hndlr;

    RACP_ABORT_OPERATION_HANDLER           abort_operation_hndlr;

}RACP_MODULE_T;


/* ----------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C"{
#endif

#ifdef BT_RACP

/** Initialization of EtherMind RACP Module */
void em_racp_init (void);
void racp_bt_init (void);

/**
 *  \brief Register function pointers table with RACP
 *
 *  \par Description:
 *       This routine registers function pointers table with RACP
 *
 *  \param [in] racp_module
 *         RACP callback handlers of the upper layer
 *
 *  \param [out] racp_id
 *         Registered module identifier returned by RACP
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_racp_register_module
           (
               /* IN */  DECL_CONST RACP_MODULE_T * racp_module,
               /* OUT */ UCHAR * racp_id
           );

/**
 *  \fn BT_racp_req_handler
 *
 *  \par Description
 *  This function is RACP request handler from the registed modules.
 *
 *  \param [in] racp_id
 *         RACP module identifier
 *
 *  \param [in] data
 *
 *  \param [in] data_length
 *
 *  \param [out] racp_req_inf
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_racp_req_handler
           (
                /* IN */   UCHAR              racp_id,
                /* IN */   UCHAR            * data,
                /* IN */   UINT16             data_length,
                /* OUT */  RACP_REQ_INFO    * racp_req_inf
           );

#else  /* BT_RACP */

#define em_racp_init()
#define racp_bt_init()
#define BT_racp_register_module(racp_module, racp_id)                    API_FAILURE
#define BT_racp_req_handler(racp_id, data, data_length, racp_req_inf)    API_FAILURE

#endif /* BT_RACP */

/** Shutdown of EtherMind RACP Module */
#define racp_bt_shutdown()

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_RACP_ */

