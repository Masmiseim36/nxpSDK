
/**
 *  \file BT_racp.h
 *
 *  \brief This file defines the Record Access Control Point(RACP)
 *  Application Interface - includes Data Structures and Methods.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_RACP_
#define _H_BT_RACP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"

/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup racp_module RACP (Remote Access Control Point)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  RACP(Record Access Control Point) module to the Application and other
 *  upper layers of the stack.
 *
 */
/**
 * \defgroup racp_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup racp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* ----------------------------------------- Global Definitions */
/**
 * \cond ignore_this Ignore this block while generating doxygen document
 */
/**
 * This value determines the length of the RACP Operand Array
 */
#define RACP_MAX_OPERAND_LEN                      20U
/**
 * \endcond
 */

/* RACP Response Codes */
/**
 *  Success respose code is used when the procedure requested by the sensor
 *  is successfully completed.
 */
#define RACP_SUCCESS                              0x01U

/**
 *  Opcode not supported response code is used when the handling of requested
 *  opcode by the sensor is not supported.
 */
#define RACP_OP_CODE_NOT_SUPPORTED                0x02U

/**
 *  Invalid operator response code is used when the requested operator by the
 *  sensor is invalid or not defined by the specification.
 */
#define RACP_INVALID_OPERATOR                     0x03U

/**
 *  Operator not supported response code is used when the handling of requested
 *  operator by the sensor is not supported.
 */
#define RACP_OPERATOR_NOT_SUPPORTED               0x04U

/**
 *  Invalid operand response code is used when the requested operand filter
 *  type by the sensor is invalid or not defined by the specification.
 */
#define RACP_INVALID_OPERAND                      0x05U

/**
 *  No records found response code is used when the sensor does not have any
 *  stored records that meet the specified criteria.
 */
#define RACP_NO_RECORDS_FOUND                     0x06U

/**
 *  Abort unsuccessful response code is used when the abort operation
 *  requested by the sensor cannot be processed.
 */
#define RACP_ABORT_UNSUCCESSFUL                   0x07U

/**
 *  Procedure incomplete response code is used when the procedure requested
 *  by the sensor was required to interrupt before completion for an
 *  unspecified reason.
 */
#define RACP_PROCEDURE_INCOMPLETE                 0x08U

/**
 *  Operand not supported response code is used when the handling of requested
 *  operand filter type by the sensor is not supported.
 */
#define RACP_OPERAND_NOT_SUPPORTED                0x09U
/** \} */
/** \} */

/* ----------------------------------------- Structures/Data Types */

/**
 * \addtogroup racp_defines Defines
 * \{
 */

/**
 * \defgroup racp_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/** RACP Request Information */
typedef struct _RACP_REQ_INFO
{
    /** RACP Opcode to be handled */
    UCHAR racp_opcode;

    /** RACP Operator to be handled */
    UCHAR racp_operator;

    /** RACP Operand to be handled */
    UCHAR racp_operand[RACP_MAX_OPERAND_LEN];

} RACP_REQ_INFO;
/** \} */
/** \} */

/**
 * \defgroup racp_callback Application Callback
 * \{
 * This section defines the callback through which RACP provides
 * asyncrhonous notifications about the RACP related Operations to
 * application/higher layers.
 */

/**
 * RACP report stored records handler.
 * RACP calls the registered callback to indicate Report Store Records
 * Operation.
 *
 * \param [in] req_info    Pointer to RACP request info as in \ref RACP_REQ_INFO
 * \param [in] racp_length Size of the RACP info event.
 */
typedef API_RESULT (* RACP_REPORT_STORED_RECORDS_HANDLER)
                   (
                       /* IN */ RACP_REQ_INFO * req_info,
                       /* IN */ UINT16        racp_length
                   ) DECL_REENTRANT;

/**
 * RACP report number of stored records handler.
 * RACP calls the registered callback to indicate Report Number of Records
 * Operation.
 *
 * \param [in] req_info    Pointer to RACP request info as in \ref RACP_REQ_INFO
 * \param [in] racp_length Size of the RACP info event.
 */
typedef API_RESULT (* RACP_NUM_OF_RECORDS_HANDLER)
                   (
                       /* IN */ RACP_REQ_INFO * req_info,
                       /* IN */ UINT16        racp_length
                   ) DECL_REENTRANT;

/**
 * RACP delete stored records handler.
 * RACP calls the registered callback to indicate Delete Stored Records
 * Operation.
 *
 * \param [in] req_info    Pointer to RACP request info as in \ref RACP_REQ_INFO
 * \param [in] racp_length Size of the RACP info event.
 */
typedef API_RESULT(* RACP_DELETE_STORED_RECORD_HANDLER)
                   (
                       /* IN */ RACP_REQ_INFO * req_info,
                       /* IN */ UINT16        racp_length
                   ) DECL_REENTRANT;
/**
 * RACP abort handler.
 * RACP calls the registered callback to indicate Abort
 * Operation.
 *
 * \param [in] req_info    Pointer to RACP request info as in \ref RACP_REQ_INFO
 * \param [in] racp_length Size of the RACP info event.
 */
typedef API_RESULT(* RACP_ABORT_OPERATION_HANDLER)
                   (
                       /* IN */ RACP_REQ_INFO * req_info,
                       /* IN */ UINT16        racp_length
                   ) DECL_REENTRANT;
/** \} */

/**
 * \addtogroup racp_defines Defines
 * \{
 */
/**
 * \addtogroup racp_structures Structures
 * \{
 */
/**
 *  The structure representing the RACP procedure handlers.
 *  It stores all the callback functions references performing specific
 *  RACP operations.
 *  The upper layer uses this structure to register dedicated callbacks
 *  with RACP to get notified of various RACP specific events.
 */
typedef struct _RACP_MODULE_T
{
    /** RACP report stored records handler */
    RACP_REPORT_STORED_RECORDS_HANDLER report_stored_records_hndlr;

    /** RACP report number of records handler */
    RACP_NUM_OF_RECORDS_HANDLER        num_of_records_hndlr;

    /** RACP delete stored records handler */
    RACP_DELETE_STORED_RECORD_HANDLER  delete_stored_records_hndlr;

    /** RACP abort handler */
    RACP_ABORT_OPERATION_HANDLER       abort_operation_hndlr;

} RACP_MODULE_T;

/** \} */
/** \} */

/* ----------------------------------------- Function Declarations */
/**
 * \defgroup racp_api API definitions
 * \{
 * Describes API definitions of RACP module.
 */

#ifdef __cplusplus
extern "C"{
#endif

#ifdef BT_RACP

/** Initialization of EtherMind RACP Module */
/**
 *  \brief To do power on initialization of EtherMind RACP module
 *
 *  \par Description:
 *       This function is the EtherMind-Init handler for the RACP module
 *       and performs power-on initialization.
 *
 *  \note This function must be called only once.
 */
void em_racp_init (void);

#ifdef BT_HAVE_SHUTDOWN
/** De-initialization of EtherMind RACP Module */
/**
 *  \brief To do power off deinitialization of EtherMind RACP module
 *
 *  \par Description:
 *       This function is the EtherMind-Sutdown handler for the RACP module
 *       and performs power-off deinitialization.
 *
 *  \note This function must be called only once.
 */
void em_racp_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \brief To perform Bluetooth specific initializations for EtherMind RACP module
 *
 *  \par Description:
 *       This function is the Bluetooth-ON handler for RACP module, and it
 *       performs bluetooth specific initializations for the RACP module.
 */
void racp_bt_init (void);

/**
 *  \brief Register function pointers table with RACP
 *
 *  \par Description:
 *       This routine registers specific Callback pointer table with RACP
 *
 *  \param [in] racp_module
 *         reference to RACP callback handlers of the upper layer
 *
 *  \param [out] racp_id
 *         Registered module identifier returned by RACP
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_racp_register_module
           (
               /* IN */  DECL_CONST RACP_MODULE_T * racp_module,
               /* OUT */ UCHAR                    * racp_id
           );

/**
 *  \brief To handle RACP request
 *
 *  \par Description
 *  This function is RACP request handler from the registed modules.
 *
 *  \param [in] racp_id
 *         RACP module identifier
 *
 *  \param [in] data
 *         RACP message
 *
 *  \param [in] data_length
 *         RACP message length
 *
 *  \param [out] racp_req_inf
 *         Pointer to RACP request info structure (\ref RACP_REQ_INFO)
 *         where the RACP message is parsed and filled.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_racp_req_handler
           (
                /* IN */   UCHAR           racp_id,
                /* IN */   UCHAR         * data,
                /* IN */   UINT16          data_length,
                /* OUT */  RACP_REQ_INFO * racp_req_inf
           );

#else  /* BT_RACP */

#define em_racp_init()
#define em_racp_shutdown()
#define racp_bt_init()
#define BT_racp_register_module(racp_module, racp_id)                    API_FAILURE
#define BT_racp_req_handler(racp_id, data, data_length, racp_req_inf)    API_FAILURE

#endif /* BT_RACP */

/** Shutdown of EtherMind RACP Module */
#define racp_bt_shutdown()

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_RACP_ */

