
/**
 *  \file BT_avctp_api.h
 *
 *  \brief EtherMind AVCTP API Header File.
 *
 *  This Header File contains declaration of AVCTP APIs, Data Types, various
 *  Macros and Constant Definitions for use in appliaction/profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_AVCTP_API_
#define _H_BT_AVCTP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_bottom_half.h"


/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  avctp_module  AVCTP (Audio/Video Control Transport Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Advanced Audio Distribution Profile module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup  avctp_defines Defines
 * \{
 * Describes defines for AVCTP module.
 */
/**
 * \defgroup avctp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 *  @name AVCTP Event Notification Callback Event Types
 *  Constant Definitioins for AVCTP Event Notification Callback Event Types.
 */

/*@{*/

#define AVCTP_CONNECT_IND                               0x01U
#define AVCTP_CONNECT_CNF                               0x02U
#define AVCTP_DISCONNECT_IND                            0x03U
#define AVCTP_DISCONNECT_CNF                            0x04U
#define AVCTP_MESSAGE_IND                               0x05U
#define AVCTP_MESSAGE_SEND_CNF                          0x06U

#define AVCTP_BOW_MESSAGE_IND                           0x07U
#define AVCTP_BOW_CONNECT_IND                           0x08U
#define AVCTP_BOW_DISCONNECT_IND                        0x09U
#define AVCTP_BOW_CONNECT_CNF                           0x0AU
#define AVCTP_BOW_DISCONNECT_CNF                        0x0BU
/*@}*/


/**
 *  @name AVCTP Control Message Types
 *  Constant Definitioins for AVCTP Control Message Types.
 */

/*@{*/
#define AVCTP_CTR_MSG_COMMAND                           0x00U
#define AVCTP_CTR_MSG_RESPONSE                          0x02U

/*@}*/


/** AVCTP Channel type */
#define AVCTP_CHANNEL_TYPE_COMMAND                    0x01U
#define AVCTP_CHANNEL_TYPE_BROWSING                   0x02U
#define AVCTP_CHANNEL_TYPE_INVALID                    0xFFU

/**
 *  @name AVCTP Control Message Header Length
 *  Constant Definitioins for AVCTP Control Message Header Length.
 */

/*@{*/
#define AVCTP_HDR_LEN                                   3

/*@}*/

/** \} */
/* ----------------------------------------- Internal Macros */

/* ----------------------------------------- Structures/Data Types */
/**
 * \defgroup avctp_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 *  \struct AVCTP_HANDLE
 *  \brief  AVCTP Handle.
 *
 *  This data type represents the abstract handle to refer while
 *  initiating a procedure with AVDTP. All AVCTP APIs require an
 *  AVCTP Handle to be provided.
 *
 *  \note
 *  \li    Not all elements of AVCTP handle is required to be populated
 *         correctly everytime an AVCTP API. Requirement of AVCTP Handle
 *         for each API is described in the respective API documentation.
 *  \li    The elements of AVCTP Handle should not be referenced directly,
 *         instead appropriate macros should be used.
 *
 *  \sa
 *  AVCTP_EVENT_NTF_CB
 */
typedef struct _AVCTP_HANDLE
{
    /** The Profile Entity Index */
    UCHAR profile_index;

    /**
     * This is an index to AVCTP signaling channel used for transporting
     * A/V commands and responses.
     */
    UCHAR ctrl_ch_index;

#ifdef AVCTP_1_3

    /**
     * This is an index to AVCTP signaling channel used for transporting
     * browsing commands and responses.
     */
    UCHAR brow_ch_index;

#endif /* AVCTP_1_3 */

    /** The BD_ADDR of the Remote Device */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} AVCTP_HANDLE;

/** \} */
/** \} */

/**
 *  \defgroup avctp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 *  \typedef AVCTP_EVENT_NTF_CB
 *  \brief   AVCTP Event Notification Callback
 *
 *  This data type represents the AVCTP Event Notification Callback, which
 *  AVCTP uses for the following purposes:
 *  \li    To confirm result of an operation initiated by the upper layer
 *         application/profile.
 *  \li    To inform upper layer application/profile of the events - for
 *         example, disconnection of the AVCTP connection, or, reception of
 *         AVCTP control messages.
 *
 *  \param [in] event_type
 *         This parameter defines the AVCTP event being reported. The valid
 *         values for this parameter are defined in "AVCTP Event Notification
 *         Callback Event Types" section.
 *  \param [in] event_result
 *         If the event being reported is a result of an earlier trigger (an
 *         AVCTP API) by the application, this parameter holds the result of
 *         the procedure. On successful completion of the procedure this
 *         parameter will have a value of 0x0000 (API_SUCCESS), else it will
 *         contain an Error Code describing the reason for failure.
 *         For asynchronous event indications (e.g. AVCTP_CONNECT_IND), this
 *         parameter holds the possible reason due to which the event is being
 *         reported.
 *  \param [in] avctp_handle
 *         This parameter is a pointer to AVCTP_HANDLE describing the
 *         application/profile responsible for this event.
 *  \param [in] event_data
 *         Depending upon the event_type, this parameters holds any data
 *         being given to the AVCTP application. Description of any such
 *         data will be provided whenever appropriate in the description
 *         of AVCTP APIs that follows.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data above.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \par AVCTP Event Notification Callback
 *
 *  \sa
 *  AVCTP_HANDLE, AVCTP Event Notification Callback Event Types
 */
typedef API_RESULT (* AVCTP_EVENT_NTF_CB)
                   (
                       UCHAR  event_type,
                       UINT16 event_result,
                       AVCTP_HANDLE *avctp_handle,
                       void  *event_data,
                       UINT16 event_datalen
                   );

/** \} */


/**
 *  \name avctp_event_notification
 *  \{
 *  This section describes the use of AVCTP_EVENT_NTF_CB for AVCTP event
 *  notifications, that is, when called with the following event
 *  types:
 *      \li \ref Section_1 (AVCTP_CONNECT_IND)
 *      \li \ref Section_2 (AVCTP_CONNECT_CNF)
 *      \li \ref Section_3 (AVCTP_DISCONNECT_IND)
 *      \li \ref Section_4 (AVCTP_DISCONNECT_CNF)
 *      \li \ref Section_5 (AVCTP_MESSAGE_IND)
 *
 *  The sections below describe how AVCTP Event Notfication is called for
 *  events and also gives a guidelines on how they should be handled.
 *
 *
 *  \section Section_1 AVCTP Connect Indication
 *  The AVCTP will call the Event Notification Callback for this event
 *  whenever it receives the AVCTP Connect Request from a remote device.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVCTP_CONNECT_IND.
 *  \param [in] event_result
 *         This parameter will always be set to 0x0000, i.e. API_SUCCESS.
 *         AVCTP will only give this event to application, once AVCTP Signaling
 *         Channel is established successfully.
 *  \param [in] avctp_handle
 *         This parameter is a pointer to AVCTP_HANDLE describing the profile
 *         responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \remark
 *       AVCTP will notify all the profiles registered with AVCTP with this
 *       event
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *
 *  \section Section_2 AVCTP Connect Confirmation
 *  The AVCTP will call the Event Notification Callback for this event when
 *  AVCTP Signaling Channel is established succssefullty on request of
 *  application/profile.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVCTP_CONNECT_CNF.
 *  \param [in] event_result
 *         On successful completion of the procedure this parameter will have
 *         a value of 0x0000, else it will contain an AVCTP Error Code
 *         describing the reason for failure.
 *  \param [in] avctp_handle
 *         This parameter is a pointer to AVCTP_HANDLE describing the profile
 *         responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \remark
 *       AVCTP will notify all the  other profiles registered with AVCTP with
 *       AVCTP_CONNECT_IND event
 *
 *
 *  \section Section_3 AVCTP Disconnect Indication
 *  The AVCTP will call the Event Notification Callback for this event when
 *  AVCTP Signaling Channel connection is disconnected by a remote AVCTP
 *  Entity.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVCTP_DISCONNECT_IND.
 *  \param [in] event_result
 *         This parameter will always be set to 0x0000, i.e. API_SUCCESS.
 *         AVCTP will only give this event to application, once AVCTP Signaling
 *         Channel is released successfully.
 *  \param [in] avctp_handle
 *         This parameter is a pointer to AVCTP_HANDLE describing the profile
 *         responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \remark
 *       AVCTP will notify all the profiles registered with AVCTP with
 *       this event
 *
 *
 *  \section Section_4 AVCTP Disconnect Confirmation
 *  The AVCTP will call the Event Notification Callback for this event when
 *  AVCTP Signaling Channel is disconnected succssefullty on request of
 *  application/profile.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVCTP_DISCONNECT_CNF.
 *  \param [in] event_result
 *         On successful completion of the procedure this parameter will have
 *         a value of 0x0000, else it will contain an AVCTP Error Code
 *         describing the reason for failure.
 *  \param [in] avctp_handle
 *         This parameter is a pointer to AVCTP_HANDLE describing the profile
 *         responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \remark
 *       AVCTP will notify all the  other profiles registered with AVCTP with
 *       AVCTP_DISCONNECT_IND event
 *
 *
 *  \section Section_5 AVCTP Message Recieved Indication
 *  The AVCTP will call the Event Notification Callback for this event when
 *  AVCTP recieves AVCTP control message packet.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVCTP_MESSAGE_IND.
 *  \param [in] event_result
 *         This parameter will always be set to 0x0000, i.e. API_SUCCESS.
 *         When AVCTP has recieved the AVCTP control message packet
 *         successfully.
 *         For AVCTP control response packet with invalid profile ID bit is
 *         set, this will have the AVCTP error code.
 *  \param [in] avctp_handle
 *         This parameter is a pointer to AVCTP_HANDLE describing the profile
 *         responsible for this event.
 *  \param [in] event_data
 *         This parameter is the AVCTP control message packet. The fisrt byte
 *         of this will contain the AVCTP control packet header that will have
 *         Transcation Label, Message Type. AVCTP provides utility macros
 *         to extract these parameters from the AVCTP control packet header.
 *  \param [in] event_datalen
 *         This parameter is ths length of AVCTP control message packet.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 * \}
 */
/** \} */

/* ----------------------------------------- Macros */
/**
 *  \defgroup AVCTP_Utility_Macros Utility Macros
 *
 *  \{
 *
 *  This section describes the EtherMind Utility Macros of AVCTP module.
 */
/**
 *  @name AVCTP Utility Macros
 *  Defining AVCTP Utility Macros.
 */

/*@{*/

/**
 *  This utility macro initializes an AVCTP Handle. Applications may use
 *  this macro prior to set important elements of an AVCTP Handle.
 *
 *  \param [in] hdl
 *         The AVCTP Handle to be initialized.
 *
 *  \hideinitializer
 */
#define AVCTP_INIT_HANDLE(hdl)                           \
        ((hdl).profile_index) = 0xFFU;                   \
        ((hdl).ctrl_ch_index) = AVCTP_MAX_SIG_CHANNELS;  \
        ((hdl).brow_ch_index) = AVCTP_MAX_SIG_CHANNELS;  \
        BT_mem_set ((hdl).bd_addr, 0x00, BT_BD_ADDR_SIZE)

/**
 *  This utility macro sets BD_ADDR in an AVCTP Handle.
 *
 *  \param [in] hdl
 *         The AVCTP Handle
 *  \param [in] bd
 *         The BD_ADDR
 *
 *  \hideinitializer
 */
#define AVCTP_SET_HANDLE_BD_ADDR(hdl, bd) \
        BT_COPY_BD_ADDR((hdl).bd_addr, (bd))


/**
 *  This utility macro resets BD_ADDR in an AVCTP Handle.
 *
 *  \param [in] hdl
 *         The AVCTP Handle
 *
 *  \hideinitializer
 */
#define AVCTP_RESET_HANDLE_BD_ADDR(hdl) \
        BT_mem_set ((hdl).bd_addr, 0x00, BT_BD_ADDR_SIZE)

/**
 *  This utility macro gets BD_ADDR from an AVCTP Handle.
 *
 *  \param [in] hdl
 *         The AVCTP Handle
 *  \param [in] bd
 *         The BD_ADDR
 *
 *  \hideinitializer
 */
#define AVCTP_GET_HANDLE_BD_ADDR(hdl, bd) \
        BT_COPY_BD_ADDR((bd), (hdl).bd_addr)

/**
 *  This utility macro updates the AVCTP Control Message Packet Header.
 *
 *  \param [in] buffer
 *         The AVCTP Control Message Packet.
 *  \param [in] tl
 *         The AVCTP Control Message Packet "Transaction Lable".
 *  \param [in] mt
 *         The AVCTP Control Message Packet "Message Type".
 *
 *  \hideinitializer
 */
#define AVCTP_UPDATE_MSG_HDR(buffer, tl, mt)  \
        (*(buffer))  = (UCHAR) ((tl) << 4U);   \
        (*(buffer)) |= (UCHAR) (mt)

/**
 *  This utility macro retrieves the AVCTP Control Message Packet Header.
 *
 *  \param [in] buffer
 *         The AVCTP Control Message Packet.
 *  \param [in] tl
 *         The AVCTP Control Message Packet "Transaction Lable".
 *  \param [in] mt
 *         The AVCTP Control Message Packet "Message Type".
 *
 *  \hideinitializer
 */
#define AVCTP_GET_MSG_HDR(buffer, tl, mt) \
        (tl) = (*(buffer) >> 4U);        \
        (mt) = (*(buffer) & 0x02U)

/**
 *  This utility macro is used to compare two AVCTP handles
 *  based on Profile Index.
 *
 *  \param [in] h1
 *         The AVCTP Handle 1.
 *  \param [in] h2
 *         The AVCTP Handle 2.
 *
 *  \hideinitializer
 */
#define AVCTP_COMPARE_HANDLE(h1, h2) \
        (((h1)->profile_index == (h2).profile_index) && \
        (0U == BT_mem_cmp((h1)->bd_addr, (h2).bd_addr, BT_BD_ADDR_SIZE)))

/**
 *  This utility macro is used to copy the AVCTP handle
 *
 *  \param [in] dhdl
 *         The destination AVCTP Handle.
 *  \param [in] shdl
 *         The source AVCTP Handle.
 *
 *  \hideinitializer
 */
#ifdef AVCTP_1_3
#define AVCTP_COPY_HANDLE(dhdl, shdl) \
        (dhdl).profile_index = (shdl).profile_index;  \
        (dhdl).ctrl_ch_index = (shdl).ctrl_ch_index;  \
        (dhdl).brow_ch_index = (shdl).brow_ch_index;  \
        BT_COPY_BD_ADDR((dhdl).bd_addr, (shdl).bd_addr)
#else
#define AVCTP_COPY_HANDLE(dhdl, shdl) \
        (dhdl).profile_index = (shdl).profile_index;  \
        (dhdl).ctrl_ch_index = (shdl).ctrl_ch_index;  \
        BT_COPY_BD_ADDR((dhdl).bd_addr, (shdl).bd_addr)
#endif /* AVCTP_1_3 */

/*@}*/
/** \} */

/* ----------------------------------------- API Declarations */
/**
 * \defgroup AVCTP_API API Definitions
 * \{
 * This section describes the EtherMind AVCTP APIs.
 */
/**
 *  @name AVCTP APIs - Profile Registration
 */

/*@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief To register a Profile with AVCTP.
 *
 *  \par Description:
 *  This API enables application/profile to register a Profile to AVCTP.
 *  On successful registration, AVCTP allocates a Profile Entity and sets the
 *  index of Profile Entity in the AVCTP Handle provided by the application.
 *  The AVCTP Handle shall be used hereafter by the application/profile
 *  to initiate any procedure (API).
 *
 *  \param [in] ntf_cb
 *         The AVCTP Event Notfication Callback which AVCTP should use to report
 *         AVCTP events.
 *  \param [in] profile_id
 *         Profile Identifier.
 *  \param [out] handle
 *         The AVCTP Handle in which Profile Entity index will be
 *         appropriately set, on successful registration.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \sa
 *  BT_avctp_profile_deregister
 */
API_RESULT BT_avctp_profile_register
           (
               /* IN */   AVCTP_EVENT_NTF_CB ntf_cb,
               /* IN */   UINT16             profile_id,
               /* OUT */  AVCTP_HANDLE *     handle
           );

/**
 *  \brief To deregister a Profile form AVCTP.
 *
 *  \par Description:
 *  This API enables application/profile to deregister a Profile from AVCTP.
 *  The AVCTP handle becomes invalid after deregistering the profile.
 *
 *  \param [in] handle
 *         The AVCTP Handle, from which Profile Index will be used to identify
 *         the registered Profile to deregister.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \sa
 *  BT_avctp_profile_register
 */
API_RESULT BT_avctp_profile_deregister
           (
               /* IN */ AVCTP_HANDLE * handle
           );

/*@}*/

/**
 *  @name AVCTP APIs - Signaling Connection Management
 */

/*@{*/
/**
 *  \brief To establish an AVCTP Signaling Connection with a remote AVCTP
 *         Entity.
 *
 *  \par Description:
 *  This API enables applications/profiles to establish an AVCTP Signaling
 *  Connection with a remote AVCTP Entity.
 *
 *  \param [in] handle
 *         The AVCTP Handle, from which Profile Index will be used to identify
 *         the Profile Entity.
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of remote device.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - AVCTP Error Code, describing the reason for failure.
 *
 *  \remark
 *
 *
 *  \par
 *  AVCTP will confirm the result of the procedure by invoking the
 *  AVCTP_EVENT_NTF_CB, with AVCTP_CONNECT_CNF.
 *
 *  \note
 *  Below is the example of how to use this API:
 *
 *  \sa
 *  BT_avctp_channel_disconnect_req, AVCTP_HANDLE, AVCTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
API_RESULT BT_avctp_channel_connect_req
           (
               /* IN */ AVCTP_HANDLE * handle,
               /* IN */ UCHAR *        bd_addr,
               /* IN */ UCHAR          channel_type
           );

/**
 *  \brief To disconnect an AVCTP Signaling Connection with a remote AVCTP
 *         Entity.
 *
 *  \par Description:
 *  This API enables applications/profiles to disconnect an AVCTP Signaling
 *  Connection with a remote AVCTP Etity.
 *
 *  \param [in] handle
 *         The AVCTP Handle, from which Profile Index will be used to identify
 *         the Profile Entity and Signaling Index will be used to identify
 *         AVCTP SIgnaling Channel.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - AVCTP Error Code, describing the reason for failure.
 *
 *
 *  \par
 *  AVCTP will confirm the result of the procedure by invoking the
 *  AVCTP_EVENT_NTF_CB, with AVCTP_DISCONNECT_CNF.
 *
 *  \note
 *  Below is the example of how to use this API:
 *
 *  \sa
 *  BT_avctp_channel_connect_req, AVCTP_HANDLE, AVCTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
API_RESULT BT_avctp_channel_disconnect_req
           (
               /* IN */ AVCTP_HANDLE * handle,
               /* IN */ UCHAR          channel_type
           );

/**
 *  \brief To request transfer of AVCTP Control Message Packets.
 *
 *  \par Description:
 *  This API enables application/profile to send AVCTP Control Message Packets
 *  over an AVCTP Signaling Connection.
 *
 *  \param [in] handle
 *         The AVCTP Handle, from which Profile Index will be used to identify
 *         the Profile Entity and Signaling Index will be used to identify
 *         AVCTP SIgnaling Channel.
 *  \param [in] packet
 *         The AVCTP Control Message Packet to transfer over the AVCTP
 *         Signaling Connection.
 *  \param [in] packet_len
 *         Length of the AVCTP Control Message Packet.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - AVCTP Error Code, describing the reason for failure.
 *
 *  \remark
 *  The application should provide a complete AVCTP Control Message Packet for
 *  transmission, which includes the following:
 *  - Space for L2CAP Header - 4 Bytes. This will be filled by L2CAP module.
 *  - Space for AVCTP Control Message Packet Header - 3 Bytes. This will be
 *    filled by AVCTP module.
 *  - AVCTP Control Message Packet Payload.
 *
 *  \par
 *  The memory for AVCTP Control Message Packet should always be dynamically
 *  allocated, and it will be freed by the EtherMind Stack, upon successful
 *  transmission.
 *
 *  \par
 *  The Trascation Lable, Message Type, Packet Type field of the AVCTP control
 *  packet header should be updated by the application/profile. AVCTP has
 *  provided macros to update these parameters.
 *
 *  \sa
 *  AVCTP_HANDLE
 */
API_RESULT BT_avctp_channel_send_message
           (
               /* IN */ AVCTP_HANDLE * handle,
               /* IN */ UCHAR *        packet,
               /* IN */ UINT16         packet_len,
               /* IN */ UCHAR          channel_type
           );
/*@}*/

/**
 *  @name AVCTP APIs - Utility Functions
 */

/*@{*/

/**
 *  \brief To set In MTU for AVCTP Signaling Channel.
 *
 *  \par Description:
 *  This API enables application/profile to set the In MTU value for AVCTP
 *  Signaling Channel. This value will be used by the AVCTP during  AVCTP
 *  Signaling Channel establishment procedure.
 *
 *  \param [in] handle
 *         The AVCTP Handle, from which Profile Index will be used to identify
 *         the Profile Entity.
 *  \param [in] in_mtu
 *         Value of In MTU in octects.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - AVCTP Error Code, describing the reason for failure.
 *
 */
API_RESULT BT_avctp_set_mtu
           (
               /* IN */  AVCTP_HANDLE * handle,
               /* IN */  UINT16         in_mtu
           );

/**
 *  \brief To set Flush Time Out for AVCTP Signaling Channel.
 *
 *  \par Description:
 *  This API enables application/profile to set the Flush Time Out value for
 *  AVCTP Signaling Channel. This value will be used by the AVCTP during AVCTP
 *  Signaling Channel establishment procedure.
 *
 *  \param [in] handle
 *         The AVCTP Handle, from which Profile Index will be used to identify
 *         the Profile Entity.
 *  \param [in] flush_to
 *         Value of Flsuh TimeOut.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - AVCTP Error Code, describing the reason for failure.
 *
 */
API_RESULT BT_avctp_set_flush_to
           (
               /* IN */  AVCTP_HANDLE * handle,
               /* IN */  UINT16         flush_to
           );
/*@}*/

/* Abstraction for AVCTP 1.0 APIs */
#define BT_avctp_connect_req(handle, bd_addr) \
        BT_avctp_channel_connect_req          \
        (                                     \
            (handle),                         \
            (bd_addr),                        \
            AVCTP_CHANNEL_TYPE_COMMAND        \
        )

#define BT_avctp_send_message(handle, packet, packet_len) \
        BT_avctp_channel_send_message                     \
        (                                                 \
            (handle),                                     \
            (packet),                                     \
            (packet_len),                                 \
            AVCTP_CHANNEL_TYPE_COMMAND                    \
        )

#define BT_avctp_disconnect_req(handle) \
        BT_avctp_channel_disconnect_req \
        (                               \
            (handle),                   \
            AVCTP_CHANNEL_TYPE_COMMAND  \
        )

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_AVCTP_API_ */

