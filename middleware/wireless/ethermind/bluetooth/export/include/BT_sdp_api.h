
/**
 *  \file BT_sdp_api.h
 *
 *  This File contains the declaration of the Mindtree Bluetooth
 *  Service Discovery Protocol Application Program Interface functions.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_SDP_API_
#define _H_BT_SDP_API_

/* -------------------------------- Header File Inclusion */
#include "BT_common.h"

/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  sdp_module  SDP (Service Discovery Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Service Discovery Protocol module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup  sdp_defines Defines
 * \{
 * Describes defines for SDP module.
 */
/**
 * \defgroup sdp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* -------------------------------- Global Definitions */
/* ----------------- SDP API/PDU Identifiers */
/** PDU ID for SDP Error response */
#define SDP_ErrorResponse                  0x01U

/** PDU ID for Service Search Request */
#define SDP_ServiceSearchRequest           0x02U

/** PDU ID for Service Search Response */
#define SDP_ServiceSearchResponse          0x03U

/** PDU ID for Service Attribute Request */
#define SDP_ServiceAttributeRequest        0x04U

/** PDU ID for Service Attribute Response */
#define SDP_ServiceAttributeResponse       0x05U

/** PDU ID for Service Search Attribute Request */
#define SDP_ServiceSearchAttributeRequest  0x06U

/** PDU ID for Service Search Attribute Response */
#define SDP_ServiceSearchAttributeResponse 0x07U

/* ----------------- Other APIs */
/** Identifer for SDP open operation */
#define SDP_Open                           0x08U

/** Identifier for SDP Close operation */
#define SDP_Close                          0x09U

/* ---------------- SDP Response Error code Values */
/** SDP error response code : Reserved */
#define SDP_INVALID_ERR                    0x0000U

/** SDP error response code : Invalid version */
#define SDP_INVALID_VERSION                0x0001U

/** SDP error response code : Invalid record handle */
#define SDP_INVALID_RECORD_HANDLE          0x0002U

/** SDP error response code : Invalid request syntax */
#define SDP_INVALID_REQUEST_SYNTAX         0x0003U

/** SDP error response : invalid  PDU size */
#define SDP_INVALID_PDU_SIZE               0x0004U

/** SDP error response : Invalid continuation state */
#define SDP_INVALID_CONT_STATE             0x0005U

/** SDP error response : Insufficient resources */
#define SDP_INSUFFICIENT_RESOURCE          0x0006U

/** SDP Server Events */
#define SDP_SERVER_CONNECT_IND             0x11U
#define SDP_SERVER_DISCONNECT_IND          0x12U

/** \} */
/** \} */

/* ---------------- Parameter Types */



/* -------------------------------- Data Type/Structures/Typedefs */
/**
 *  \addtogroup sdp_defines Defines
 *  \{
 */
/**
 * \addtogroup sdp_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 *  Structure for SDP Profile Descriptor list.
 *  This contains two fields, UUID value followed by a Version number.
 *  Utility API for extracting profile descriptor list uses this structure.
 */
typedef struct profile_desc_list
{
     /** UUID */
     S_UUID uuid;

     /** Version number */
     UINT16 version;

} PROFILE_DESC_STRUCT;
/** \} */
/** \} */

/**
 *  \defgroup sdp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/* Registering Callbacks to SDP */
#ifdef SDP_HAVE_HANDLE_WITH_CB
/* Forward declaration */
struct sdp_handle;

/** SDP Application Callback */
typedef void (* SDP_CB)
             (
                 struct sdp_handle * handle,
                 UCHAR               command,
                 UCHAR *             data,
                 UINT16              length,
                 UINT16              status
             );
#else  /* SDP_HAVE_HANDLE_WITH_CB */
typedef void (* SDP_CB)
             (
                 UCHAR      command,
                 UCHAR *    data,
                 UINT16     length,
                 UINT16     status
             );
#endif /* SDP_HAVE_HANDLE_WITH_CB */
/** \} */

/**
 *  \addtogroup sdp_defines Defines
 *  \{
 */

/**
 *  \addtogroup sdp_structures Structures
 *  \{
 */

/**
 *  SDP Handle.
 *
 *  For each SDP connection, SDP assigns a handle to be used while calling
 *  the SDP Request APIs later on.
 */
typedef struct _SDP_HANDLE
{
    /** Application callback */
    SDP_CB fn_ptr;

    /** Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

    /** Handle ID */
    UCHAR id;

} SDP_HANDLE;
/** \} */
/** \} */

/**
 *  \defgroup sdp_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */
/** Macros for Setting and Resetting the SDP handles */
#define SDP_SET_HANDLE(handle, bd, callback) \
        BT_COPY_BD_ADDR ((handle).bd_addr, (bd)); \
        (handle).fn_ptr = (callback)

#define SDP_RESET_HANDLE(handle) \
        BT_mem_set (&(handle), 0U, sizeof (SDP_HANDLE))

/** Macros for UUID Conversion API */
#define BT_uuid_16_to_128       BT_uuid_32_to_128
/** \} */


/* -------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 * \defgroup sdp_api API Definitions
 * \{
 * This section describes the EtherMind Service Discovery Profile APIs.
 */
/* ---------------- Request APIs */
/**
 *  \brief To establish a SDP connection with a Bluetooth device.
 *
 *  \par Description:
 *       This API establishes the SDP connection with a remote Bluetooth device,
 *       identified by its Bluetooth Device Address (BD_ADDR) as specified in
 *       the supplied SDP Handle. The L2CAP channel configuration is done
 *       automatically, once the connection is established. The application is
 *       expected to establish the ACL connection with the remote Bluetooth
 *       device prior to calling this API.
 *
 *  \param [in] handle
 *         The SDP Handle for the SDP connection.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The Application can use SDP_SET_HANDLE() to populate the SDP Handle
 *        prior to calling this API.
 *        The SDP Response Callback is called on connection establishment or
 *        error with 'type' set to SDP_Open.
 */
API_RESULT BT_sdp_open
           (
               /* IN */   SDP_HANDLE *  handle
           );

/**
 *  \brief To close the SDP connection with a Bluetooth device.
 *
 *  \par Description:
 *       This API closes the SDP connection with a remote Bluetooth device, as
 *       identified by the supplied SDP Handle.
 *
 *  \param [in] handle
 *         The SDP Handle for the SDP connection.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The SDP Response Callback is called on connection establishment or
 *        error with 'type' set to SDP_Close.
 */
API_RESULT BT_sdp_close
           (
               /* IN */    SDP_HANDLE * handle
           );


/**
 *  \brief To perform a SDP Service Search Request on another Bluetooth device
 *         to find the services provided by that device.
 *
 *  \par Description:
 *       This API is used to search the SDP Database of a remote Bluetooth device
 *       for the available services.  This API returns the SDP Service Record
 *       Handles of the services that are provided by the remote Bluetooth device.
 *       The retrieved SDP Service Record Handles then can be used to determine
 *       the service characteristics, and various parameters.
 *
 *  \param [in] handle
 *         The SDP Handle for the SDP connection.
 *
 *  \param [in] uuids
 *         The set of UUIDs to be used to limit the search for services offered
 *         by the remote Bluetooth device. This parameter must be an array of
 *         S_UUID variable, of size num_uuids, containing the UUIDs.
 *
 *  \param [in] num_uuids
 *         Number of UUIDs present in the uuids parameter.
 *
 *  \param [out] rec_handles
 *         Buffer allocated by the application for storing the record handles.
 *         The allocated buffer size should be sufficient to store num_rec_handles
 *         handles.
 *
 *  \param [in,out] num_rec_handles
 *         This parameter specifies the maximum number of SDP Record Handles
 *         that the remote Bluetooth device is required to return when sending
 *         its response for the Service Search Request.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The buffer for SDP Record Handles must be allocated and passed as
 *        rec_handles parameter.
 *        The allocated buffer must be resident, and the application must not
 *        free the buffer before SDP Response Callback is called indicating
 *        completion of the Service Search Request. The SDP Response Callback
 *        uses the same buffer as response parameter to indicate the received
 *        record handles.
 *        Once the SDP Service Search process completes successfully, the buffer
 *        containing the SDP Record Handles, rec_handles, can be parsed using the
 *        API BT_sdp_get_RecordHandle() to extract the Record Handles.
 */
API_RESULT BT_sdp_servicesearchrequest
           (
               /* IN */    SDP_HANDLE *    handle,
               /* IN */    S_UUID *        uuids,
               /* IN */    UINT16          num_uuids,
               /* OUT */   UINT32 *        rec_handles,
               /* INOUT */ UINT16 *        num_rec_handles
           );

/**
 *  \brief To perform a SDP Service Attribute Request on another Bluetooth
 *         device to determine the characteristics of a service.
 *
 *  \par Description:
 *       This API initiates Service Attribute transaction with the peer SDP
 *       server with which the L2CAP channel is already established.
 *       This API follows the service search transaction, which provides the
 *       service record handle that is required for this API for getting
 *       the attributes of the desired services provided by the peer.
 *       It utilizes the "common API handler" to service this transaction.
 *       Blocking mode blocks on the user context until the API handler returns with
 *       the response for the Service Attribute or with the Error Code, whereas in
 *       Non-Blocking mode calls the registered callback function with the out
 *       parameters passed in this API, hence the caller must preserver the memory
 *       for these parameters until it gets the callback.
 *
 *  \param [in] handle
 *         SDP Handle for the connection.
 *
 *  \param [in] rec_handle
 *         Desired Service Record Handle from which the attribute ID values
 *         are to be retrieved. This handle is obtained via a previous
 *         SDP_servicesearch transaction.
 *
 *  \param [in] attribue_ids
 *         This is an attribute ID list for which the attribute values are
 *         to be retrieved from the Service Record. This value must be in
 *         ascending order.
 *
 *  \param [in] num_attribute_ids
 *         Number of attribute IDs in the list.
 *         If this value is greater than SDP_MAX_ATTR_ID, SDP returns failure.
 *
 *  \param [in] attribute_id_range
 *         List of attribute ID range for which the values to be retrieved
 *         and  must be in the ascending order.
 *
 *  \param [in] num_attribute_id_range
 *         Number of attribute ID range in the  attribute Id range list.
 *         If this is greater than SDP_MAX_ATTR_RANGE, SDP returns failure.
 *
 *  \param [out] attribute_data
 *         This contains the list of attribute IDs and attribute values
 *         returned for this request.
 *
 *  \param [in,out] len_attribute_data
 *         The length of attribute data . This specifies the maximum number
 *         of bytes to be returned. This value should be greater than 0.
 *         sdp client never sends next request after receiving
 *         the partial response.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_serviceattributerequest
           (
               /* IN */    SDP_HANDLE *    handle,
               /* IN */    UINT32          rec_handle,
               /* IN */    UINT16 *        attribute_ids,
               /* IN */    UINT16          num_attribute_ids,
               /* IN */    UINT32 *        attribute_id_range,
               /* IN */    UINT16          num_attribute_id_range,
               /* OUT */   UCHAR *         attribute_data,
               /* INOUT */ UINT16 *        len_attribute_data
           );

/**
 * \brief To perform a SDP Service Search Attribute Request to find the
 * services available and the characteristics of the available service.
 *
 *  \par Description:
 *       This API initiates Service Search Attribute transaction with the peer SDP
 *       server with which the L2CAP channel is already established.
 *       This transaction combines the capabilities of  SDP_ServiceSearch request
 *       and  SDP_ServiceAttribute request. It utilizes the "common API handler"
 *       to service the transaction.
 *       Blocking mode blocks on the user context until the API handler returns with
 *       the response for the Service Attribute or with the Error Code, whereas in
 *       Non-Blocking mode calls the registered callback function with the out
 *       parameters passed in this API, hence the caller must preserver the memory
 *       for these parameters until it gets the callback.
 *
 *  \param [in] handle
 *         SDP Handle for the connection.
 *
 *  \param [in] uuids
 *         Pointer to list of UUIDs. If the number of UUIDs exceeds
 *         the maximum value (SDP_CLIENT_MAX_NUM_UUIDS), SDP returns failure.
 *         The service search pattern for the request is framed using
 *         this UUID list.
 *
 *  \param [in] num_uuids
 *         Indicates the number of UUIDS in the list. If this value exceeds
 *         the maximum permitted value (SDP_CLIENT_MAX_NUM_UUIDS)
 *         sdp truncates the additional UUIDS from the list.
 *
 *  \param [in] attribue_ids
 *         List of attribute IDs for which the values to be retrieved from
 *         the service record. This value must be in ascendingd order.
 *
 *  \param [in] num_attribute_ids
 *         Number of attribute IDs in the list.
 *
 *  \param [in] attribute_id_range
 *         This is a list of attribute ID range for which the values to be
 *         retrieved. This also must be in the ascending order.
 *
 *  \param [in] num_attribute_id_range
 *         Number of attribute ID range in the attribute Id range list.
 *
 *  \param [out] attribute_data
 *         This contains the list of attribute IDs and attribute values
 *         returned for this request.
 *
 *  \param [in,out] len_attribute_data
 *         As input this specifies the maximum number of bytes of attribute
 *         data to be  returned in the response to this request.
 *         Output indicates the length of attribute data sequence.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_servicesearchattributerequest
           (
               /* IN */    SDP_HANDLE *    handle,
               /* IN */    S_UUID *        uuids,
               /* IN */    UINT16          num_uuids,
               /* IN */    UINT16 *        attribute_ids,
               /* IN */    UINT16          num_attribute_ids,
               /* IN */    UINT32 *        attribute_id_range,
               /* IN */    UINT16          num_attribute_id_range,
               /* OUT */   UCHAR *         attribute_data,
               /* INOUT */ UINT16 *        len_attribute_data
           );


/* -------------- UTILITY APIS */
/**
 *  \brief To extract Record Handles.
 *
 *  \par Description:
 *       This API can be used to extract any Record Handles from the buffer
 *       returned by the SDP Service Search Request API.
 *
 *  \param [in] buffer
 *         The buffer containing the SDP Record Handles, as returned by the SDP
 *         Service Search Request API.
 *
 *  \param [in] buffer_length
 *         Size of the buffer containing the SDP Record Handles.
 *
 *  \param [out] rec_handles
 *         Caller allocated UINT32 Array of size num_handles, which will contain
 *         the extracted Record Handles on return.
 *
 *  \param [in,out] num_handles
 *         This is a IN-OUT parameter. The Application must specify a pointer to
 *         a UINT16 variable, containing the allocated size of record_handles array.
 *         On return, this parameter contains the actual number of Record Handles
 *         extracted and available in the record_handles parameter.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_RecordHandle
           (
               /* IN */    UCHAR *     buffer,
               /* IN */    UINT16      length,
               /* OUT */   UINT32 *    rec_handles,
               /* INOUT */ UINT16 *    num_handles
           );

/**
 *  \brief To extract the Service Record Handle Attribute.
 *
 *  \par Description:
 *       This API can be used to extract the Service Record Handle attribute
 *       value from the Attribute data returned by the SDP Service Attribute
 *       Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] handle
 *         Application allocated UINT32 Array, where the list of Record Handles
 *         will be populated.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceRecordHandle
           (
               /* IN */  UCHAR * attribute_data,
               /* OUT */ UINT32 * handle
           );

/**
 *  \brief To extract Service Class ID List Attributes value.
 *
 *  \par Description:
 *       This API can be used to extract the Service Class ID List attribute
 *       value from the attribute data returned by the SDP Service Attribute
 *       Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] id_list
 *         Application allocated S_UUID array, of size id_list_length,
 *         where the extracted UUIDs will be copied on return.
 *
 *  \param [in,out] id_list_length
 *         This is a IN-OUT parameter. The Application must specify a pointer to
 *         a UINT16 variable, containing the allocated size of service_id_list.
 *         On return, this parameter contains the actual number of UUIDs extracted
 *         and available in the service_id_list parameter.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceClassIDList
           (
               /* IN */    UCHAR *  attribute_data,
               /* OUT */   S_UUID * id_list,
               /* INOUT */ UINT16 * id_list_length
           );

/**
 *  \brief To extract Service record state attribute.
 *
 *  \par Description:
 *       This API can be used to extract the Service Record State attribute value
 *       from the attribute data returned the SDP Service Attribute Request and/or
 *       SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *          Application allocated UCHAR buffer containing the Service Attribute
 *          data, as received as a result of the SDP Service Attribute Request
 *          and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] record_state
 *         Pointer to an application allocated UINT32 variable containing the
 *         Service Record State attribute value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceRecordState
           (
               /* IN */  UCHAR *  attribute_data,
               /* OUT */ UINT32 * record_state
           );

/**
 *  \brief To extract the Language based Attrubute ID list attribute value
 *
 *  \par Description:
 *       This API can be used to extract the Language Base Attribute ID List
 *       attribute value from the attribute data returned by the SDP Service
 *       Attribute Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] lang_base_id_list
 *         Application allocated UINT16 array of size 'lang_count * 3', which
 *         will contain the Language ID List providing the Language Identifier,
 *         Character Encoding Identifier, Base Attribute ID.
 *
 *  \param [in,out] lang_count
 *         Number of natural languages supported. The Application must specify a
 *         pointer to a UINT16 variable, containing the allocated size of
 *         language_base_id_list. On return, this parameter contains the actual
 *         number of Language ID Lists extracted and available in the
 *         language_base_id_list parameter
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_LanguageBaseAttributeIDList
           (
               /* IN */    UCHAR *  attribute_data,
               /* OUT */   UINT16 * lang_base_id_list,
               /* INOUT */ UINT16 * lang_count
           );

/**
 *  \brief To extract Service ID attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Service ID attribute value from the
 *       attribute data returned by the SDP Service Attribute Request and/or SDP
 *       Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] service_id
 *         Pointer to a caller allocated S_UUID variable, which will contain the
 *         UUID value, extracted from the given attribute data, that universally
 *         and uniquely identifies the Service Instance described by the Service
 *         Record.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceID
           (
               /* IN */  UCHAR *  attribute_data,
               /* OUT */ S_UUID * service_id
           );

/**
 *  \brief To extract Browse Group List attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Browse Group List attribute value
 *       from the attribute data returned by the SDP Service Attribute Request
 *       and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] browse_list
 *         Application allocated S_UUID array of size, browse_list_length,
 *         which will contain the UUIDs from the Browse Group List.
 *
 *  \param [in,out] browse_list_length
 *          The Application must specify a pointer to a UINT16 variable,
 *          containing the allocated size of browse_group_list.
 *          On return, this parameter contains the actual number of UUIDs extracted and
 *          available in the browse_group_list parameter
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_BrowseGroupList
           (
               /* IN */    UCHAR *  attribute_data,
               /* OUT */   S_UUID * browse_list,
               /* INOUT */ UINT16 * browse_list_length
           );

/**
 *  \brief To extract Service Information Time To Live attribute
 *
 *  \par Description:
 *       This API can be used to extract the Service Information Time To Live
 *       attribute value from the attribute data returned by the SDP Service
 *       Attribute Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] ttl
 *         Pointer to a caller allocated UINT32 variable, which will contain the
 *         Time to Live extracted attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceInfoTimeToLive
           (
               /* IN */    UCHAR *  attribute_data,
               /* OUT */   UINT32 * ttl
           );

/**
 *  \brief To extract Service Availability attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Service Availability attribute value
 *       from the attribute data returned by the SDP Service Attribute Request
 *       and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] service_availability
 *         Pointer to a caller allocated UCHAR variable, which will contain the
 *         Service Availability attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceAvailability
           (
               /* IN */    UCHAR * attribute_data,
               /* OUT */   UCHAR * service_availability
           );

/**
 *  \brief To extract Bluetooth Profile Descriptor List attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Bluetooth Profile Descriptor List
 *       attribute value from the attribute data returned by the SDP Service
 *       Attribute Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] prof_list
 *         Caller allocated PROFILE_DESC_LIST Array of size profile_count, which
 *         will contain the Bluetooth Profile Descriptor List attribute value on
 *         return.
 *
 *  \param [in,out] profile_count
 *         The Application must specify a pointer to a UINT16 variable,
 *         containing the allocated size of profile_list.
 *         On return, this parameter contains the actual number of Profile Descriptor
 *         Lists extracted and available in the profile_list parameter.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_BluetoothProfileDescriptorList
           (
               /* IN */    UCHAR *               attribute_data,
               /* OUT */   PROFILE_DESC_STRUCT * prof_list,
               /* INOUT */ UINT16 *              profile_count
           );

/**
 *  \brief To extract Documentation URL attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Documentation URL attribute value
 *       from the attribute data returned by the SDP Service Attribute Request
 *       and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] url
 *         Caller allocated UCHAR Array of size url_length, which will contain
 *         the Document URL attribute value on return.
 *
 *  \param [in] length
 *         This parameter provides the size of the UCHAR buffer allocated for url,
 *         to store the Document URL attribute value.
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_DocumentationURL
           (
               /* IN */  UCHAR * attribute_data,
               /* OUT */ UCHAR * url,
               /* IN */  UINT16  length
           );

/**
 *  \brief To extract Client Executable URL attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Client Executable URL attribute
 *       value from the attribute data returned by the SDP Service Attribute
 *       Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] url
 *         Caller allocated UCHAR Array of size url_length, which will contain
 *         the Client Executable URL attribute value on return.
 *
 *  \param [in] url_length
 *         This parameter provides the size of the UCHAR buffer allocated for
 *         url, to store the Client Executable URL attribute value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ClientExecutableURL
           (
               /* IN */  UCHAR * attribute_data,
               /* OUT */ UCHAR * url,
               /* IN */  UINT16  length
           );

/**
 *  \brief To extract Icon URL attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Icon URL attribute value from the
 *       attribute data returned by the SDP Service Attribute Request and/or
 *       SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] url
 *         Caller allocated UCHAR Array of size url_length, which will contain
 *         the Icon URL attribute value on return.
 *
 *  \param [in] length
 *         This parameter provides the size of the UCHAR buffer allocated for url,
 *         to store the Icon URL attribute value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_IconURL
           (
               /* IN */  UCHAR * attribute_data,
               /* OUT */ UCHAR * url,
               /* IN */  UINT16  length
           );

/**
 *  \brief To extract Service Name attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Service Name attribute value from
 *       the attribute data returned by the SDP Service Attribute Request and/or
 *       SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] lang_base_attr_id
 *         Language Base Attribute ID for the language in which the name is specified.
 *
 *  \param [out] string
 *         Caller allocated UCHAR Array of size name_length, where the Service
 *         Name will be available on return.
 *
 *  \param [in] str_buf_len
 *         This parameter provides the size of the UCHAR Array, service_name,
 *         allocated by the caller, to store the Service Name attribute value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceName
           (
               /* IN */  UCHAR * attribute_data,
               /* IN */  UINT16  lang_base_attr_id,
               /* OUT */ UCHAR * string,
               /* IN */  UINT16  str_buf_len
           );

/**
 *  \brief To extract the Service Description Attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Service Description attribute value
 *       from the attribute data returned by the SDP Service Attribute Request
 *       and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] language_base_attr_id
 *         Language Base Attribute ID for the language in which the description
 *         is specified.
 *
 *  \param [out] string
 *         Caller allocated UCHAR Array of size desc_length, where the Service
 *         Description will be available on return.
 *
 *  \param [in] str_buf_len
 *         This parameter provides the size of the UCHAR Array, service_desc,
 *         allocated by the caller, to store the Service Description attribute value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceDescription
           (
               /* IN */  UCHAR * attribute_data,
               /* IN */  UINT16  lang_base_attr_id,
               /* OUT */ UCHAR * string,
               /* IN */  UINT16  str_buf_len
           );

/**
 *  \brief To extract Provider Name attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Provider Name attribute value from
 *       the attribute data returned by the SDP Service Attribute Request and/or
 *       SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] lang_base_attr_id
 *         Language Base Attribute ID for the language in which the name is specified.
 *
 *  \param [out] string
 *         Caller allocated UCHAR Array of size name_length, where the Provider
 *         Name will be available on return.
 *
 *  \param [in] str_buf_len
 *         This parameter provides the size of the UCHAR Array, provider_name,
 *         allocated by the caller, to store the Provider Name attribute value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ProviderName
           (
               /* IN */  UCHAR * attribute_data,
               /* IN */  UINT16  lang_base_attr_id,
               /* OUT */ UCHAR * string,
               /* IN */  UINT16  str_buf_len
           );

/**
 *  \brief To extract the Version Number attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Version Number attribute value from
 *       the attribute data returned by the SDP Service Attribute Request and/or
 *       SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] version_number
 *         Pointer to a caller allocated UINT16 variable, which will contain the
 *         Version Number attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_VersionNumberList
           (
               /* IN */  UCHAR *  attribute_data,
               /* OUT */ UINT16 * version_number
           );

/**
 *  \brief To extract Service Database State attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Service Database State attribute
 *       value from the attribute data returned by the SDP Service Attribute
 *       Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] database_state
 *         Pointer to a caller allocated UINT32 variable, which will contain the
 *         Service Database State attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_ServiceDatabaseState
           (
               /* IN */  UCHAR *  attribute_data,
               /* OUT */ UINT32 * database_state
           );

/**
 *  \brief To extract Group ID attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the Group ID attribute value from the
 *       attribute data returned by the SDP Service Attribute Request and/or
 *       SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] group_id
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.Application allocated
 *         S_UUID array, of size id_list_length, where the extracted Group UUIDs
 *         will be copied on return.
 *
 *  \param [in,out] group_id_length
 *         The Application must specify a pointer to a UINT16 variable, containing
 *         the allocated size of group_id_list. On return, this parameter contains
 *         the actual number of UUIDs extracted and available in the
 *         group_id_list parameter.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_GroupID
           (
               /* IN */    UCHAR *  attribute_data,
               /* OUT */   S_UUID * group_id,
               /* INOUT */ UINT16 * group_id_length
           );

/**
 *  \brief To extract RFCOMM Server Channel attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the RFCOMM Server Channel attribute
 *       value from the attribute data (Protocol Descriptor List) returned by
 *       the SDP Service Attribute Request and/or SDP Service Search Attribute
 *       Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] channel_number
 *         Pointer to a caller allocated UCHAR variable, which will contain the
 *         RFCOMM Server Channel attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_channel_number
           (
               /* IN */  UCHAR * attribute_data,
               /* OUT */ UCHAR * channel_number
           );

/**
 *  \brief To extract additional RFCOMM Server Channel attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the RFCOMM Server Channel attribute
 *       value from the attribute data (Additional Protocol Descriptor List)
 *       returned by the SDP Service Attribute Request and/or SDP Service
 *       Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] channel_number
 *         Pointer to a caller allocated UCHAR variable, which will contain the
 *         RFCOMM Server Channel attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_additional_channel
           (
               /* IN */  UCHAR * attribute_data,
               /* OUT */ UCHAR * channel_number
           );

/**
 *  \brief To extract L2CAP PSM attribute value.
 *
 *  \par Description:
 *       This API can be used to extract the L2CAP PSM attribute value from the
 *       attribute data (Protocol Descriptor List) returned by the SDP Service
 *       Attribute Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] psm
 *         Pointer to a caller allocated UINT16 variable, which will contain the
 *         L2CAP PSM attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_psm
           (
               /* IN */  UCHAR  * attribute_data,
               /* OUT */ UINT16 * psm
           );

/**
 *  \brief To get L2CAP PSM value from the additional protocol descriptor list.
 *
 *  \par Description
 *  This routine extract the L2CAP psm value from the additional protocol
 *  descriptor list in the attribute data of the response.
 *
 *  \param  [in] attribute_data
 *          Attribute data used to extract the L2CAP psm.
 *
 *  \param  [out] psm
 *          Extracted L2CAP PSM value.
 *
 *  \param  [in] psm_index
 *          Index of psm in additional protocol list.
 *
 *  \result
 *     API_RESULT : API_SUCCESS or Error code
 *
 * Note:
 * Few SDP records(Example AVRCP v1.6) may contains multiple L2CAP psm's in the
 * additional protocal descriptor list, so psm_index should used to get corresponding
 * psm value.
 *
 * Example:
 * To extract first L2CAP psm, set psm_index = 0.
 * To extract 2nd L2CAP psm, set psm_index = 1.
 */
API_RESULT BT_sdp_get_additional_psm_with_index
           (
               /* IN */  UCHAR    *  attribute_data,
               /* OUT */ UINT16   *  psm,
               /* IN */  UCHAR       psm_index
           );

/**
 * To Get First L2CAP psm from additional protocol descriptor list.
 */
#define BT_sdp_get_additional_psm(attr_data, psm)        \
        BT_sdp_get_additional_psm_with_index((attr_data), (psm), 0x00U)

/**
 *  \brief To extract Start and End handle attribute values of a GATT Service.
 *
 *  \par Description:
 *       This API can be used to extract the start and end handle attribute values
 *       for a GATT Service from the attribute data (Protocol Descriptor List)
 *       returned by the SDP Service Attribute Request and/or SDP Service Search
 *       Attribute Request APIs.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] start_handle
 *         Pointer to a caller allocated UINT16 variable, which will contain the
 *         Start Handle attribute value on return.
 *
 *  \param [out] end_handle
 *         Pointer to a caller allocated UINT16 variable, which will contain the
 *         End Handle attribute value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_gatt_handle_range
           (
               /* IN */  UCHAR  *  attribute_data,
               /* OUT */ UINT16 *  start_handle,
               /* OUT */ UINT16 *  end_handle
           );

/**
 *  \brief To extract attribute value for a given attribute ID.
 *
 *  \par Description:
 *       This API can be used to extract the attribute value corresponding to a
 *       given attribute ID from the attribute data returned by the SDP Service
 *       Attribute Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] attrib_id
 *         Attribute ID for which the attribute value to be extracted.
 *
 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] attribute_value
 *         Caller allocated UCHAR Array, where the attribute value will be
 *         available on return.
 *
 *  \param [in,out] attribute_len
 *         This parameter provides the size of the UCHAR Array, allocated by
 *         the caller, to store the attribute value.
 *         The actual attribute length will be available on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_DataSequence_attr_val
           (
               /* IN */    UINT16   attrib_id,
               /* IN */    UCHAR *  attribute_data,
               /* OUT */   UCHAR *  attribute_value,
               /* INOUT */ UINT16 * attribute_len
           );

/**
 *  \brief To extract Data Element.
 *
 *  \par Description:
 *       This API can be used to extract the attribute value corresponding to a
 *       given attribute ID from the attribute data returned by the SDP Service
 *       Attribute Request and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [in] pdu
 *         Attribute value from which the Data Element to be extracted.
 *
 *  \param [in] pdu_length
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Attribute Request
 *         and/or SDP Service Search Attribute Request APIs.
 *
 *  \param [out] value_offset
 *         Caller provided UCHAR pointer, where the start of Data Element value
 *         offset will be available on return.
 *
 *  \param [out] value_length
 *         Caller provided UINT32 pointer, where the length of Data Element
 *         will be available on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sdp_get_data_element
           (
               /* IN */  UCHAR   * pdu,
               /* IN */  UINT32    pdu_length,
               /* OUT */ UCHAR   * value_offset,
               /* OUT */ UINT32  * value_length
           );

/**
 *  \brief To extract attribute value for a given attribute ID.
 *
 *  \par Description:
 *       This API can be used to get the pointer to the attribute value
 *       of the next record. Use this API to extract the portion of
 *       Attribute list for the next record and it is relevent only to the
 *       attribute list return by the SDP_servicesearchattributerequest API.

 *  \param [in] attribute_data
 *         Application allocated UCHAR buffer containing the Service Attribute
 *         data, as received as a result of the SDP Service Search Attribute
 *         Request APIs.
 *
 *  \param [in] record_no
 *         SDP record number for which the Atribute List to be returned.
 *
 *  \return
 *        UCHAR*: Pointer to the next attribute list
 */
UCHAR * BT_sdp_get_Pointer_to_next_AttributeList
        (
            /* IN */  UCHAR * attribute_data,
            /* IN */  UCHAR   record_no
        );

#define BT_sdp_get_attr_value BT_sdp_get_DataSequence_attr_val

/**
 *  \brief To convert 16/32-bit UUID to a 128-bit UUID.
 *
 *  \par Description:
 *       This API can be used to convert a 16/32-bit UUID to a 128-bit UUID.
 *       A 16/32-bit UUID is converted to 128-bit UUID format using following
 *       operation.
 *       <*> 128_bit_value = 16_bit_value * 2^96 + Bluetooth_Base_UUID
 *       <*> 128_bit_value = 32_bit_value * 2^96 + Bluetooth_Base_UUID
 *       Note: Bluetooth_Base_UUID = 00000000-0000-1000-8000-00805F9B34FB
 *
 *  \param [in] source_uuid
 *         16/32-bit UUID to be converted
 *
 *  \param [out] dest_uuid
 *         Pointer to a caller allocated UINT128_ID variable, which will contain the
 *         converted 128-bit UUID value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
void BT_uuid_32_to_128
     (
         /* IN */  UINT32       source_uuid,
         /* OUT */ UINT128_ID * dest_uuid
     );

/**
 *  \brief To convert 16-bit UUID to a 32-bit UUID.
 *
 *  \par Description:
 *       This API can be used to convert a 16-bit UUID to a 32-bit UUID.
 *       A 16-bit UUID is converted to 32-bit UUID format by zero-extending
 *       the 16-bit value to 32-bits.
 *
 *  \param [in] source_uuid
 *         16-bit UUID to be converted
 *
 *  \param [out] dest_uuid
 *         Pointer to a caller allocated UINT32 variable, which will contain the
 *         converted 32-bit UUID value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
void BT_uuid_16_to_32
     (
         /* IN */  UINT16   source_uuid,
         /* OUT */ UINT32 * dest_uuid
     );


/* ---------------- SDP Server Callback Registration API */
#ifdef SDP_SERVER_HAVE_CB_IND_SUPPORT
/**
 *  \brief To register callback with SDP server to get notification about
 *         connection and disconnection from SDP Client running on a peer device.
 *
 *  \par Description:
 *       This API registers callback with SDP server to get notification about
 *       connection and disconnection from SDP Client running on a peer device.
 *       Whenever a peer bluetooth device creates SDP connection or disconnects
 *       an existing SDP connection, using the application registered callback
 *       the information will be given to the application by the SDP server.
 *       Application using this callback mechanism
 *       i)Can detect if peer device has kept the SDP connection open for more than some configured amount of time
 *      ii)Can decide if the existing ACL link need to be disconnected
 *
 *  \param [in] callback_ptr
 *         The callback function pointer registered by the application with SDP Server.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The SDP Server Callback is called on SDP connection establishment or
 *        SDP disconnection.
 *        The compilation flag "SDP_SERVER_HAVE_CB_IND_SUPPORT" shall be defined
 *        to use SDP Server Callback feature.
 */
API_RESULT BT_sdp_server_register_ind_cb
           (
               API_RESULT (* callback_ptr)
                          (
                              UCHAR    event_type,
                              UCHAR *  event_data,
                              UCHAR    event_datalen
                          )
           );
#endif /* SDP_SERVER_HAVE_CB_IND_SUPPORT */

#ifdef __cplusplus
};
#endif  /* __cplusplus */
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_SDP_API_ */

