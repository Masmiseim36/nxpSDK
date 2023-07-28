
/**
 *  \file GA_api.h
 *
 *  \brief This file defines the Generic Audio Core Application Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_API_
#define _H_GA_API_

/**
 * \addtogroup bt_ga GA
 * \{
 */

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"

#include "bap.h"
#include "vcp.h"
#include "micp.h"
#include "csip.h"

/* --------------------------------------------- Global Definitions */
/**
 * \defgroup bt_ga_profiles GA Profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Profile module to the Application.
 */

/** \} */

/**
 * \defgroup bt_ga_core GA Core
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Core module to the Application and other upper
 * layers of the stack.
 */

/**
 * \defgroup ga_cap_module GA CAP
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA CAP module to the Application.
 */

/**
 * \defgroup ga_csip_module Coordinated Set Identification Profile (CSIP)
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Coordinated Set Identification Profile(CSIP) module to the CAP
 * layers of the stack.
 * Coordinated Set Identification Service shall be instantiated as a Primary
 * Service.
 */

/** \} */

/**
 * \defgroup ga_micp_module Microphone Control Profile (MICP)
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Microphone Control Profile (MICP) module to the CAP layer of the stack.
 * Microphone Control Profile Service shall be instantiated as Primary
 * Service.
 * The Audio Input Control Service shall only be instantiated as an
 * included service.
 * Microphone Control Profile includes Audio Input Control Service.
 */

/** \} */

/**
 * \defgroup ga_vcp_module Volume Control Profile (VCP)
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Volume Control Profile (VCP) module to the CAP layer of the stack.
 * Volume Control Service is a Primary Service.
 * Volume Offset Control Service shall only be instantiated as an included
 * service.
 * Audio Input Control Service shall only be instantiated as an included
 * service.
 * Volume Control Service includes Volume Offset Control Service and
 * Audio Input Control Service.
 */

/** \} */

/**
 * \defgroup ga_bap_module Basic Audio Profile (BAP)
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Basic Audio Profile (BAP) module to the CAP layer of the stack.
 * Published Audio Capabilities Service (PACS) is a Primary Service.
 * Audio Stream Control Service (ASCS) is a Primary Service.
 * Broadcast Audio Scan Service (BASS) is a Primary Service.
 */

/** \} */

/**
 * \defgroup ga_cap_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * GA CAP Layer.
 */

/**
 * \defgroup ga_cap_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * GA CAP Layer.
 */

/**
 * \name GA Core Modules
 * \{
 * \brief Identify BAP, VCP, MICP, CSIP
 */

/** GA Module - BAP */
#define GA_MODULE_BAP       0x00U
/** GA Module - VCP */
#define GA_MODULE_VCP       0x01U
/** GA Module - MICP */
#define GA_MODULE_MICP      0x02U
/** GA Module - CSIP */
#define GA_MODULE_CSIP      0x03U

/** \} */

/**
 * \name GA CAP Roles
 * \{
 * \brief Bitmasks Type : Initiator, Acceptor, Commander
 */

/** CAP Role - Initiator */
#define CAP_ROLE_INITIATOR   0x01U
/** CAP Role - Acceptor */
#define CAP_ROLE_ACCEPTOR    0x02U
/** CAP Role - Commander */
#define CAP_ROLE_COMMANDER   0x04U

/** \} */

/**
 * \name GA CAP Announcement Type
 * \{
 * \brief Type : Size(Octets): 1
 */

/** CAP Announcement Type - General */
#define CAS_GENERAL_ANNOUNCEMENT   0x00U
/** CAP Announcement Type - Targeted */
#define CAS_TARGETED_ANNOUNCEMENT  0x01U

/** \} */

/** \} */

/** \} */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \defgroup ga_bap_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * GA BAP Layer.
 */

/**
 * \defgroup ga_bap_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * GA BAP Layer.
 */

/**
 * \name GA Audio Role
 * \{
 * \brief Type : Size(Octets): 1
 */

/** GA Audio Role - Sink */
#define GA_ROLE_SINK                BAP_ROLE_SINK
/** GA Audio Role - Source */
#define GA_ROLE_SOURCE              BAP_ROLE_SOURCE

/** \} */

/**
 * \name GA BAP Announcement Type
 * \{
 * \brief Type : Size(Octets): 1
 */

/** BAP Announcement Type - General */
#define BAP_ASCS_GENERAL_ANNOUNCEMENT   0x00U
/** BAP Announcement Type - Targeted */
#define BAP_ASCS_TARGETED_ANNOUNCEMENT  0x01U

/** \} */

/**
 * \name GA Capabilities Type
 * \{
 * \brief Type : Size(Octets): 1
 */

/** GA Capabilities Type - Codecs */
#define GA_CAP_TYPE_CODECS          0x01U
/** GA Capabilities Type - Locations */
#define GA_CAP_TYPE_LOCATIONS       0x02U
/** GA Capabilities Type - Supported Contexts */
#define GA_CAP_TYPE_SUPP_CONTEXTS   0x03U
/** GA Capabilities Type - Available Contexts */
#define GA_CAP_TYPE_AVAIL_CONTEXTS  0x04U

/** \} */

/** Broadcast_ID Length */
#define GA_BC_ID_LEN                BAP_BC_ID_LEN

/** \} */


/**
 * \defgroup ga_bap_api_evt_defs Events
 * \{
 * \brief This section describes the Generic Audio Events.
 */

/**
 * \defgroup ga_common_evt_defs GA Common Event
 * \{
 * \brief This section describes the Generic Audio Common
 * Events.
 */

/**
 * This event is notified once the discovery and configuration for
 * notification is done on the peer for PACS Service. The roles discovered on
 * the remote is notified in the callback \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref ga_bap_constants and the length is 1B.
 * NOTE: If both Source and Sink PAC records are discovered on remote, the
 * data will include both roles ORred in a single event.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SETUP_CNF                0x01U

/**
 * This event is notified once the configuration for
 * notification is disabled on the peer for PACS and ASCS Service.
 * The context of the remote device is cleared.
 * Once complete, callback \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is NULL and the length is 0B.
 * NOTE: If both PACS records and ASCS is discovered on the remote device,
 * the configuration disable and the context clear happens for both.
 * If ASCS is not done, then only PACS is cleared.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_RELEASE_CNF              0x02U

/**
 * This event is notified once the Capabilities of the peer for the requested
 * role is determined. The parameters shall be as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * PACS - Read request on the PACS char as per role requested is done in a
 * chained manner to determine number of PACS records, record details,
 * Audio Locations, Available and Supported Contexts. The event is notified
 * multiple times indicating the capability type with status as
 * \ref GA_CONTINUE and finally with \ref GA_SUCCESS.
 *
 * The data is of type \ref GA_CAP_ENDPOINT.
 * Based on the role and the type defined in \ref GA_CAP_ENDPOINT, the data
 * parameters shall vary.
 * Capability Type: \ref ga_bap_constants
 * \ref GA_CAP_TYPE_CODECS :  data: Pointer to data with First byte indicating
 *                                 Number of PACS Records, followed by
 *                                 \ref GA_CODEC_INFO followed by \ref GA_METADATA.
 *                           datalen: Length of above.
 * \ref GA_CAP_TYPE_LOCATIONS : data: Pointer to \ref UINT32,
 *                             Represents Supported Context values as bitmask.
 *                             Refer \ref ga_bap_constants
 *                             datalen: sizeof( \ref UINT32 )
 * \ref GA_CAP_TYPE_SUPP_CONTEXTS : data: Pointer to \ref UINT16,
 *                                 Represents Supported Context values as bitmask.
 *                                 Refer \ref ga_bap_constants
 *                                 datalen: sizeof( \ref UINT16 )
 * \ref GA_CAP_TYPE_AVAIL_CONTEXTS : data: Pointer to \ref UINT16,
 *                                  Represents Available Context values as bitmask.
 *                                  Refer \ref ga_bap_constants
 *                                  datalen: sizeof( \ref UINT16 )
 * \return \ref GA_SUCCESS (always)
 */
#define GA_GET_CAPABILITIES_CNF     0x03U

/** \} */

/**
 * \defgroup ga_unicast_client_evt_defs GA Unicast Client Event
 * \{
 * \brief This section describes the Generic Audio
 * Events for Unicast Client.
 */

/**
 * This event is notified once the discovery of ASCS Service, Char, Desc and
 * configuration of notification is complete along with reading of the
 * ASE Sink and ASE Source Char on the peer.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The event is notified multiple times indicating the ASEs present on the
 * peer device with role and its ASE along with the status as
 * \ref GA_CONTINUE and finally with \ref GA_SUCCESS.
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be \ref GA_ASE_INFO_IN_IDLE.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_DISCOVER_CNF         0x04U

/**
 * This event is notified once the response for Config Codec operation is
 * received form the peer. In this event, the server exposed preferred QoS
 * parameters are notified in this format \ref GA_ASE_INFO_IN_CODEC_CONF.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be \ref GA_ASE_INFO_IN_CODEC_CONF.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_CONFIGURE_CNF        0x05U

/**
 * This event is notified once the response for QoS Config operation is
 * received form the peer. In this event, the server provides the accepted QoS
 * parameters which is notified in this format \ref GA_ASE_INFO_IN_QOS_CONF.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be \ref GA_ASE_INFO_IN_QOS_CONF.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_SETUP_CNF            0x06U

/**
 * This event is notified once the response for Enable operation is
 * received form the peer. In this event, the server provides the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_START_CNF            0x07U

/**
 * This event is notified once the response for Disable operation is
 * received form the peer. In this event, the server provides the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_SUSPEND_CNF          0x08U

/**
 * This event is notified once the response for Update Metadata operation is
 * received form the peer. In this event, the server provides the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_UPDATE_CNF           0x09U

/**
 * This event is notified once the response for Release operation is
 * received form the peer.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be NULL.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_RELEASE_CNF          0x0AU

/**
 * This event is notified once the response for Receiver Start Ready operation
 * is received form the peer. In this event, the server provides the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 *
 * NOTE: This event is generated post calling \ref GA_sep_receiver_start_ready,
 * when Local Client ASE is a Sink.
 */
#define GA_SEP_RX_START_READY_CNF   0x0BU

/**
 * This event is notified once the response for Receiver Start Ready operation
 * is received form the peer. In this event, the server provides the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 *
 * NOTE: This event is generated post calling \ref GA_sep_receiver_stop_ready,
 * when Local Client ASE is a Sink.
 */
#define GA_SEP_RX_STOP_READY_CNF    0x0CU

/** \} */

/**
 * \defgroup ga_unicast_server_evt_defs GA Unicast Server Event
 * \{
 * \brief This section describes the Generic Audio
 * Events for Unicast Server.
 */

/**
 * This event is notified once the Codec Config opcode
 * is received form the peer. In this event, the client provides the Codec
 * conf parameters which is notified in this format
 * \ref GA_SEP_CONF.
 * Internally this event is generated once the ASE CP for Config Codec is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be \ref GA_SEP_CONF.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_CONFIGURE_IND        0x11U


/**
 * This event is notified once the Codec QoS opcode
 * is received form the peer. In this event, the client provides the Codec
 * conf parameters which is notified in this format
 * \ref GA_QOS_CONF.
 * Internally this event is generated once the ASE CP for Config QoS is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be \ref GA_QOS_CONF.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_SETUP_IND            0x12U

/**
 * This event is notified once the Enable opcode
 * is received form the peer. In this event, the client provides the
 * Metadata parameters which is notified in this format
 * \ref GA_METADATA.
 * Internally this event is generated once the ASE CP for Enable is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be \ref GA_METADATA.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_START_IND            0x13U

/**
 * This event is notified once the Disable opcode
 * is received form the peer.
 * Internally this event is generated once the ASE CP for Disable is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be NULL.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_SUSPEND_IND          0x14U

/**
 * This event is notified once the Update opcode
 * is received form the peer. In this event, the client provides the
 * Metadata parameters which is notified in this format
 * \ref GA_METADATA.
 * Internally this event is generated once the ASE CP for Update is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be \ref GA_METADATA.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_UPDATE_IND           0x15U

/**
 * This event is notified once the Release opcode
 * is received form the peer.
 * Internally this event is generated once the ASE CP for Release is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be NULL.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_RELEASE_IND          0x16U

/**
 * This event is notified once the Receiver Start Ready opcode
 * is received form the peer Sink ASE.
 * Internally this event is generated once the ASE CP for Release is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be NULL.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_RX_START_READY_IND   0x17U

/**
 * This event is notified once the Receiver Stop Ready opcode
 * is received form the peer Sink ASE.
 * Internally this event is generated once the ASE CP for Release is
 * received, The Application shall set the response along with the
 * reason for the respective ASE CP. Post this the ASE Sink or ASE Source
 * Notification is done by the Stack.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref GA_SEP_INFO.
 * The info inside \ref GA_SEP_INFO shall be NULL.
 *
 * \return \ref UINT16
 * \ref ga_bap_constants
 */
#define GA_SEP_RX_STOP_READY_IND    0x18U

/**
 * This event is notified when the Client has triggered a write on the Sink Audio
 * Locations.
 * Internally this event is generated once the basic validation of the param is
 * complete and is accepted.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref UINT32.
 *
 * \return \ref UINT16
 */
#define GA_SINK_SET_AUDIO_LOCATIONS_IND        0x27U

/**
 * This event is notified when the Client has triggered a write on the Source Audio
 * Locations.
 * Internally this event is generated once the basic validation of the param is
 * complete and is accepted.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gas_cb ).
 *
 * The data is of type \ref UINT32.
 *
 * \return \ref UINT16
 */
#define GA_SOURCE_SET_AUDIO_LOCATIONS_IND      0x28U

/** \} */

/**
 * \addtogroup ga_unicast_client_evt_defs
 * \{
 */

/**
 * This event is notified when there is a Autonomous Config Codec Operation
 * initiated by the Server. In this event, the server exposed preferred QoS
 * parameters are notified in this format \ref GA_ASE_INFO_IN_CODEC_CONF.
 * Also, this event shall get generated when Released Operation is performed
 * by the Server, also when the Server has Caching Support.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The info inside \ref GA_ASE_INFO shall be \ref GA_ASE_INFO_IN_CODEC_CONF.
 *
 * \return \ref GA_SUCCESS (always)
 * \ref ga_bap_constants
 */
#define GA_SEP_CONFIGURE_NTF        0x19U

/**
 * This event is notified when there is a Autonomous Disable Operation
 * initiated by the Server on a Sink ASE(Server Local Role).
 * In this event, the server provides the accepted QoS parameters which
 * is notified in this format \ref GA_ASE_INFO_IN_QOS_CONF for this active
 * ASE establishment.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The info inside \ref GA_ASE_INFO shall be \ref GA_ASE_INFO_IN_QOS_CONF.
 *
 * \return \ref GA_SUCCESS (always)
 * \ref ga_bap_constants
 */
#define GA_SEP_SETUP_NTF            0x20U

/**
 * This event is notified when there is a Autonomous Disable Operation
 * initiated by the Server on a Source ASE(Server Local Role).
 * In this event, the server provides the the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The info inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_SUSPEND_NTF          0x21U

/**
 * This event is notified when there is a Autonomous Update Metadata Operation
 * initiated by the Server.
 * In this event, the server provides the the Metadata
 * parameters which is notified in this format
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The info inside \ref GA_ASE_INFO shall be
 * \ref GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_UPDATE_NTF           0x22U

/**
 * This event is notified when there is a Autonomous Release Operation
 * initiated by the Server.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The info inside \ref GA_ASE_INFO shall be NULL.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_RELEASE_NTF          0x23U

/**
 * This event is notified when there is a Autonomous Receiver Start Ready
 * Operation initiated by the Server for a Sink ASE(Server Local Role).
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The data inside \ref GA_ASE_INFO shall be NULL.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_RX_START_READY_NTF   0x24U

/**
 * This event is notified when there is a Released Operation initiated by the
 * Server. This is generated when the Server moves from Releasing, also when
 * the Server has No Caching Support.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref gac_cb ).
 *
 * The data is of type \ref GA_ASE_INFO.
 * The info inside \ref GA_ASE_INFO shall be NULL.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_SEP_IDLE_NTF             0x25U

/* TODO: Comment */
#define GA_CAPABILITIES_CHANGE_NTF  0x26U

/** \} */

/**
 * \defgroup ga_bc_src_evt_defs GA Broadcast Source Events
 * \{
 * \brief This section describes the Generic Audio
 * Events for Broadcast Source.
 */

/**
 * This event is notified when the EA for Broadcast Audio Announcement
 * and PA for Basic Audio Announcement is enabled successfully/failed.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsrc_cb ).
 *
 * The data is NULL and length is 0U.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SETUP_ANNOUNCEMENT_CNF        BAP_BC_SETUP_ANNOUNCEMENT_CNF

/**
 * This event is notified when the EA for Broadcast Audio Announcement
 * and PA for Basic Audio Announcement is disabled successfully/failed.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsrc_cb ).
 *
 * The data is NULL and length is 0U.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_END_ANNOUNCEMENT_CNF          BAP_BC_END_ANNOUNCEMENT_CNF

/**
 * This event is notified when the Streaming has started successfully/failed.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsrc_cb ).
 *
 * The data is of type \ref BAP_BC_START_PARAMS.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_START_CNF                     BAP_BC_START_CNF

/**
 * This event is notified when the Streaming has stopped successfully/failed.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsrc_cb ).
 *
 * The data is of type \ref BAP_BC_SUSPEND_PARAMS.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SUSPEND_CNF                   BAP_BC_SUSPEND_CNF

/**
 * This event is notified when the Streaming has stopped without the
 * user triggering a Suspend. This can be caused if the controller
 * terminates the BIG.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsrc_cb ).
 *
 * The data is of type \ref BAP_BC_SUSPEND_PARAMS.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SUSPEND_IND                   BAP_BC_SUSPEND_IND
/** \} */

/**
 * \defgroup ga_bc_snk_evt_defs GA Broadcast Sink Events
 * \{
 * \brief This section describes the Generic Audio
 * Events for Broadcast Sink.
 */

/**
 * This event is notified when the device started succesfully/failed the scan
 * for Broadcast Audio Annoucements on EA from other devices.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is NULL and length is 0U.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SCAN_ANNOUNCEMENT_CNF         BAP_BC_SCAN_ANNOUNCEMENT_CNF

/**
 * This event is notified when the device stopped succesfully/failed the scan
 * for Broadcast Audio Annoucements on EA from other devices.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is NULL and length is 0U.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SCAN_END_CNF                  BAP_BC_SCAN_END_CNF

/**
 * This event is notified when the device creates/fail to create Sync to the
 * Broadcast Source identified during Broadcast Audio Annoucement Scan EA.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_ASSOCIATE_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_ASSOCIATE_CNF                 BAP_BC_ASSOCIATE_CNF

/**
 * This event is notified when the device stops/fails to stop Sync to the
 * Broadcast Source identified during Broadcast Audio Annoucement Scan EA.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_DISSOCIATE_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_DISSOCIATE_CNF                BAP_BC_DISSOCIATE_CNF

/**
 * This event is notified when the device lost Periodic Advertising Sync to the
 * Broadcast Source identified during Broadcast Audio Annoucement Scan EA.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_DISSOCIATE_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_DISSOCIATE_IND                BAP_BC_DISSOCIATE_IND

/**
 * This event is notified when the device creates/fails to create the Broadcast
 * Sync to the Broadcast Source identified during Broadcast Audio Annoucement
 * Scan EA.
 * Once the Sync to Basic Audio Annoucement PA is complete, and when the
 * the BIG Create Sync is created/failed to create this event is generated.
 *
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_ENABLE_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_ENABLE_CNF                    BAP_BC_ENABLE_CNF

/**
 * This event is notified when the device terminate/fails to terminate the
 * Broadcast Sync to the Broadcast Source identified during Broadcast Audio
 * Annoucement Scan EA.
 * Once the Sync to Basic Audio Annoucement PA is complete, and when the
 * BIG Create Sync is created and when a successful/failed terminate of the
 * Broadcast Sync happens, this event is generated.
 *
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_DISABLE_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_DISABLE_CNF                   BAP_BC_DISABLE_CNF

/**
 * This event is notified when the device lost the Broadcast Sync to the Broadcast
 * Source identified during Broadcast Audio Annoucement Scan EA.
 * Once the Sync to Basic Audio Annoucement PA is complete, and when the
 * BIG Create Sync is created and when a Loss to Broadcast Sync happens,
 * this event is generated.
 *
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_DISABLE_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_DISABLE_IND                   BAP_BC_DISABLE_IND

/**
 * This event is notified when the device is receiving Broadcast Audio
 * Announcement EA from remote devices.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SOURCE_ANNOUNCEMENT_IND       BAP_BC_SOURCE_ANNOUNCEMENT_IND

/**
 * This event is notified when the device is receiving Basic Audio
 * Announcement PA from remote device.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_SOURCE_CONFIG_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SOURCE_CONFIG_IND             BAP_BC_SOURCE_CONFIG_IND

/**
 * This event is notified when the device is receiving BigInfo Event
 * that gets generated post creating successful Broadcast Sync.
 * A Callback is made with the parameters as below and is notified via
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref bcsnk_cb ).
 *
 * The data is of type \ref BAP_BC_SOURCE_STREAM_PARAMS.
 * \sa GA_broadcast_decode_event_params() to fetch the event decoded params.
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BC_SOURCE_STREAM_IND             BAP_BC_SOURCE_STREAM_IND

/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_vcp_module
 * \{
 */

/**
 * \defgroup ga_vcp_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * GA VCP Layer.
 */

/**
 * \defgroup ga_vcp_api_evt_defs Events
 * \{
 * \brief This section describes the Volume Control Events.
 */

/**
 * \defgroup ga_vcp_volcntrl_evt_defs VCP Volume Controller Events
 * \{
 * \brief This section describes the Volume Control Profile
 * Events for Volume Controller.
 */

/**
 * \name VCP Volume Controller APIs-Setup
 * \{
 * \brief This section describes the Volume Control Profile Setup
 * Events for Volume Controller.
 */

/**
 * This event indicates the setup of a VCS Session.
 * Callback is triggered once discovery of service, char and descriptors,
 * configuration for Notifications is complete.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref vcc_cb )
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_SETUP_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_SETUP_CNF                                 0xA9U

/**
 * This event is notified on discovery of secondary/included services VOCS and
 * AICS with peer device with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_GET_CAPABILITIES_CNF
 * \param [in] ga_status  \ref GA_CONTINUE Service Handle range of discovered
 *                        VOCS(if any) and again for AICS(if any) \n
 *                        \ref GA_SUCCESS Service Discovery Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_CONTINUE \n
 *                         - ga_data - Pointer to object of type
 *                           \ref GA_BRR_SVC_INFO \n
 *                         .
 * \param [in] ga_datalen  If status is \ref GA_CONTINUE \n
 *                            - ga_datalen - Size of \ref GA_BRR_SVC_INFO \n
 *                            .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_GET_CAPABILITIES_CNF                      0xAAU

/**
 * This event indicates the setup of an VOCS/AICS Session as requested
 * during call to \ref GA_vc_set_capability().
 * The Char and Desc Discovery is
 * performed post which the Configuration of Notification of Char is done.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref vcc_cb )
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_SET_CAPABILITY_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_SET_CAPABILITY_CNF                        0xABU

/** \} */

/**
 * \name VCP Volume Controller APIs-Release
 * \{
 * \brief This section describes the Volume Control Profile Release
 * Events for Volume Controller.
 */

/**
 * This event is notified when a VCS Service context is released.
 * Configuration of Notifications for char are disabled and followed by release
 * of VCS context.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_RELEASE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Release Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_RELEASE_CNF                               0xAC

/**
 * This event is notified after VOCS/AICS Service context is released as
 * requested by \ref GA_vc_release_capability().
 * Configuration of Notifications for char are disabled followed by release
 * of VOCS/AICS context.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_RELEASE_CAPABILITY_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Release Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_RELEASE_CAPABILITY_CNF                    0xAD

/** \} */

/**
 * \name VCP Volume Controller APIs-Read
 * \{
 * \brief This section describes the Volume Control Profile Read
 * Events for Volume Controller.
 */

/**
 * This event is notified when a read response is received for Volume State
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_GET_VOLUME_STATE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Volume_Setting ( \ref UINT8 ) \n
 *                                       \ref ga_vcp_vcs_vs_vol_setting
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_vcp_vcs_vs_mute \n
 *                                     - Change_Counter ( \ref UINT8 )
 *                                       \ref ga_vcp_vcs_vs_chng_cntr \n
 * \param [in] ga_datalen  3
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_vcs_vs_char_val
 */
#define GA_VC_GET_VOLUME_STATE_CNF                      VCS_EVENT_GET_VOLUME_STATE_CNF

/**
 * This event is notified when a read response is received for Volume Flags
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_GET_VOLUME_FLAGS_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Volume_Setting_Persisted
 *                      ( \ref UINT8 ) \n
 *                      \ref ga_vcp_vcs_vol_setting_persisted
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_vcs_vol_setting_persisted
 */
#define GA_VC_GET_VOLUME_FLAGS_CNF                      VCS_EVENT_GET_VOLUME_FLAGS_CNF

/**
 * This event is notified when a read response is received for Volume Offset
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_GET_OFFSET_STATE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Volume_Offset ( \ref INT16 ) \n
 *                                     - Change_Counter ( \ref UINT8 ) \n
 * \param [in] ga_datalen  3
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_vocs_cp_vo
 * \sa ga_vcp_vocs_cp_cc
 */
#define GA_VC_VOCS_GET_OFFSET_STATE_CNF                 VOCS_EVENT_GET_OFFSET_STATE_CNF

/**
 * This event is notified when a read response is received for Audio Location
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_GET_AUDIO_LOCATION_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Audio Location ( \ref UINT32 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_bap_constants
 */
#define GA_VC_VOCS_GET_AUDIO_LOCATION_CNF               VOCS_EVENT_GET_AUDIO_LOCATION_CNF

/**
 * This event is notified when a read response is received for Audio Output
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_bap_constants
 */
#define GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF               VOCS_EVENT_GET_AUDIO_OUT_DESC_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_GET_INPUT_STATE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Gain_Setting ( \ref INT8 ) \n
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_vcp_aics_mute_field_values \n
 *                                     - Gain_Mode ( \ref UINT8 )
 *                                       \ref ga_vcp_aics_gain_mode_field_values \n
 *                                     - Change_Counter ( \ref UINT8 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_aics_mute_field_values
 * \sa ga_vcp_aics_gain_mode_field_values
 */
#define GA_VC_AICS_GET_INPUT_STATE_CNF                  AICS_EVENT_GET_INPUT_STATE_CNF

/**
 * This event is notified when a read response is received for Gain Setting
 * Properties characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Gain_Setting_Units ( \ref UINT8 ) \n
 *                                     - Gain_Setting_Minimum ( \ref INT8 ) \n
 *                                     - Gain_Setting_Maximum ( \ref INT8 ) \n
 * \param [in] ga_datalen  3
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_aics_gain_setting
 */
#define GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF         AICS_EVENT_GET_GAIN_SETTING_PROPRTY_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * Type characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_GET_INPUT_TYPE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR.
 *                      Refer \ref ga_bap_constants
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_bap_constants
 */
#define GA_VC_AICS_GET_INPUT_TYPE_CNF                   AICS_EVENT_GET_INPUT_TYPE_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * Status characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_GET_INPUT_STATUS_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer of type \ref UCHAR.
 *                      Refer \ref ga_vcp_aics_input_status
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_aics_input_status
 */
#define GA_VC_AICS_GET_INPUT_STATUS_CNF                 AICS_EVENT_GET_INPUT_STATUS_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF             AICS_EVENT_GET_AUDIO_INPUT_DESC_CNF

/** \} */

/**
 * \name VCP Volume Controller APIs-Write
 * \{
 * \brief This section describes the Volume Control Profile Write
 * Events for Volume Controller.
 */

/**
 * This event is notified when a write response is received for Volume
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_CP_WT_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Write Response received \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0
 *
 * \return\ref GA_SUCCESS (always)
 */
#define GA_VC_CP_WT_CNF                                 VCS_EVENT_CP_WT_CNF

/**
 * This event is notified when a write response is received for Volume Offset
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_CP_WT_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Write Response received \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0
 *
 * \return\ref GA_SUCCESS (always)
 * \sa ga_vcp_vocs_cp_opc
 */
#define GA_VC_VOCS_CP_WT_CNF                            VOCS_EVENT_CP_WT_CNF

/**
 * This event is notified when a write response is received for Audio Input
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_CP_WT_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Write Response received \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0
 *
 * \return\ref GA_SUCCESS (always)
 * \sa ga_vcp_aics_cp_req_opc
 */
#define GA_VC_AICS_CP_WT_CNF                            AICS_EVENT_CP_WT_CNF

/** \} */

/**
 * \name VCP Volume Controller APIs-Notification
 * \{
 * \brief This section describes the Volume Control Profile
 * Notification Events for Volume Controller.
 */

/**
 * This event is notified when a notification is received for Volume State
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOLUME_STATE_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Volume_Setting ( \ref UINT8 ) \n
 *                                       \ref ga_vcp_vcs_vs_vol_setting
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_vcp_vcs_vs_mute \n
 *                                     - Change_Counter ( \ref UINT8 )
 *                                       \ref ga_vcp_vcs_vs_chng_cntr \n
 * \param [in] ga_datalen  3
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_vcs_vs_char_val
 */
#define GA_VC_VOLUME_STATE_NTF                          VCS_EVENT_VOLUME_STATE_NTF

/**
 * This event is notified when a notification is received for Volume Flags
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOLUME_FLAGS_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Volume_Setting_Persisted
 *                      ( \ref UINT8 ) \n
 *                      \ref ga_vcp_vcs_vol_setting_persisted
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_vcp_vcs_vol_setting_persisted
 */
#define GA_VC_VOLUME_FLAGS_NTF                          VCS_EVENT_VOLUME_FLAGS_NTF

/**
 * This event is notified when a notification is received for Offset State
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_OFFSET_STATE_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 *                      Includes Param - Volume_Setting ( \ref UINT8 ) \n
 *                                       \ref ga_vcp_vcs_vs_vol_setting
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_vcp_vcs_vs_mute \n
 *                                     - Change_Counter ( \ref UINT8 )
 *                                       \ref ga_vcp_vcs_vs_chng_cntr \n
 * \param [in] ga_datalen  3
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_bap_constants
 */
#define GA_VC_VOCS_OFFSET_STATE_NTF                     VOCS_EVENT_OFFSET_STATE_NTF

/**
 * This event is notified when a notification is received for Audio Location
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_AUDIO_LOCATION_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Audio Location ( \ref UINT32 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_bap_constants
 */
#define GA_VC_VOCS_AUDIO_LOCATION_NTF                   VOCS_EVENT_AUDIO_LOCATION_NTF

/**
 * This event is notified when a notification is received for Audio Output
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_AUDIO_OUT_DESC_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_VOCS_AUDIO_OUT_DESC_NTF                   VOCS_EVENT_AUDIO_OUT_DESC_NTF

/**
 * This event is notified when a notification is received for Audio Input
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_INPUT_STATE_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Gain_Setting ( \ref INT8 ) \n
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_vcp_aics_mute_field_values \n
 *                                     - Gain_Mode ( \ref UINT8 )
 *                                       \ref ga_vcp_aics_gain_mode_field_values \n
 *                                     - Change_Counter ( \ref UINT8 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_AICS_INPUT_STATE_NTF                      AICS_EVENT_CP_INPUT_STATE_NTF

/**
 * This event is notified when a notification is received for Audio Input
 * Status characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_INPUT_STATUS_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer of type \ref UCHAR.
 *                      Refer \ref ga_vcp_aics_input_status
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_AICS_INPUT_STATUS_NTF                     AICS_EVENT_CP_INPUT_STATUS_NTF

/**
 * This event is notified when a notification is received for Audio Input
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_AUDIO_INPUT_DESC_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_VC_AICS_AUDIO_INPUT_DESC_NTF                 AICS_EVENT_AUDIO_INPUT_DESC_NTF

/** \} */

/** \} */

/**
 * \defgroup ga_vcp_vol_renderer_evt_defs VCP Volume Renderer Events
 * \{
 * \brief This section describes the Volume Control Profile Events
 * for Volume Renderer.
 */

/**
 * \name VCP Volume Renderer APIs-Read
 * \{
 * \brief This section describes the Volume Control Profile Read
 * Events for Volume Renderer.
 */

/**
 * This event is notified when a read request is received for
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_RD_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref VCP_EVENT_INFO
 *                      context - Use this while calling GA_vc_send_rsp() to
 *                                send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof ( \ref VCP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_vc_send_rsp() has to be called to send the read response.
 *       Refer to \ref GA_BRR_UUID inside \ref GA_BRR_CHR_CONTEXT in
 *       \ref VCP_EVENT_INFO.
 * \sa ga_bap_constants
 */
#define GA_VC_RD_IND                                    VCS_EVENT_RD_IND

/**
 * This event is notified when a read request is received for
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_RD_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref VCP_EVENT_INFO
 *                      context - Use this while calling GA_vc_send_rsp() to
 *                                send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof ( \ref VCP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_vc_send_rsp() has to be called to send the read response.
 *       Refer to \ref GA_BRR_UUID inside \ref GA_BRR_CHR_CONTEXT in
 *       \ref VCP_EVENT_INFO.
 * \sa ga_bap_constants
 */
#define GA_VC_VOCS_RD_IND                               VOCS_EVENT_RD_IND

/**
 * This event is notified when a read request is received for
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_RD_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref VCP_EVENT_INFO
 *                      context - Use this while calling GA_vc_send_rsp() to
 *                                send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof ( \ref VCP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_vc_send_rsp() has to be called to send the read response.
 */
#define GA_VC_AICS_RD_IND                               AICS_EVENT_RD_IND

/** \} */

/**
 * \name VCP Volume Renderer APIs-Write
 * \{
 * \brief This section describes the Volume Control Profile Write
 * Events for Volume Renderer.
 */

/**
 * This event is notified when a write request is received for Volume
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_CP_WT_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref VCP_EVENT_INFO
 *                      context - Use this while calling GA_vc_send_rsp() to
 *                                send response.
 *                      data - Pointer to UCHAR, that includes Opcode followed
 *                             by Parameters.
 *                      datalen - Data length of the Parameter.
 * \param [in] ga_datalen  \ref sizeof ( \ref VCP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_vc_send_rsp() has to be called to send the write response.
 */
#define GA_VC_CP_WT_IND                                 VCS_EVENT_CP_WT_IND

/**
 * This event is notified when a write request is received for Audio Location
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_SET_AUDIO_LOCATION_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Audio Location ( \ref UINT32 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_bap_constants
 * \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 *       The Audio Location characteristic value is a bitmask that shall be
 *       set to a value that reflects the location of an audio output that
 *       is associated with the instance of VOCS
 */
#define GA_VC_VOCS_SET_AUDIO_LOCATION_IND               VOCS_EVENT_SET_AUDIO_LOCATION_IND

/**
 * This event is notified when a write request is received for Audio Output
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_SET_AUDIO_OUT_DESC_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 * \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 */
#define GA_VC_VOCS_SET_AUDIO_OUT_DESC_IND               VOCS_EVENT_SET_AUDIO_OUT_DESC_IND

/**
 * This event is notified when a write request is received for Audio Input
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_VOCS_CP_WT_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref VCP_EVENT_INFO
 *                      context - Use this while calling GA_vc_send_rsp() to
 *                                send response.
 *                      data - Pointer to UCHAR, that includes Opcode followed
 *                             by Parameters (Change_Counter (1B),
 *                             Volume_Offset (2B)).
 *                      datalen - Data length of the Parameter.
 * \param [in] ga_datalen  \ref sizeof ( \ref VCP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_vc_send_rsp() has to be called to send the write response.
 */
#define GA_VC_VOCS_CP_WT_IND                            VOCS_EVENT_CP_WT_IND

/**
 * This event is notified when a write without response request is received
 * for Audio Input Description characteristic from peer device with the
 * following values as parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_SET_AUDIO_INPUT_DESC_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Without Response Request
 *                        received \n
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 * \note There is no response expected for this event.
 */
#define GA_VC_AICS_SET_AUDIO_INPUT_DESC_IND             AICS_EVENT_SET_AUDIO_INPUT_DESC_IND

/**
 * This event is notified when a write request is received for Audio Input
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref vcs_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_VC_AICS_CP_WT_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref VCP_EVENT_INFO
 *                      context - Use this while calling GA_vc_send_rsp() to
 *                                send response.
 *                      data - Pointer to UCHAR, that includes Opcode followed
 *                             by Parameters.
 *                      datalen - Data length of the Parameter.
 * \param [in] ga_datalen  \ref sizeof ( \ref VCP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_vc_send_rsp() has to be called to send the write response.
 */
#define GA_VC_AICS_CP_WT_IND                            AICS_EVENT_CP_WT_IND

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_micp_module
 * \{
 */

/**
 * \defgroup ga_micp_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * GA MICP Layer.
 */

/**
 * \defgroup ga_micp_api_evt_defs Events
 * \{
 * \brief This section describes the Microphone Control Events.
 */

/**
 * \defgroup ga_micp_cont_evt_defs MICP Microphone Controller Events
 * \{
 * \brief This section describes the Microphone Control Profile Events for
 * Microphone Controller.
 */

/**
 * \name MICP Microphone Controller-Setup
 * \{
 * \brief This section describes the Microphone Control Profile Setup Events
 * for Microphone Controller.
 */

/**
 * This event indicates the setup of a MICS Session.
 * Callback is triggered once discovery of service, char and descriptors,
 * configuration for Notifications is complete.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref micc_cb )
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_SETUP_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  MICP Handle for the Device - \ref MICP_HANDLE \n
 *                      If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_SETUP_CNF                                 MC_SETUP_MICS_CNF

/**
 * This event is notified on discovery of secondary service AICS with peer
 * device with the following values as parameters in the \ref GA_CALLBACK
 * callback ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_GET_CAPABILITIES_CNF
 * \param [in] ga_status  \ref GA_CONTINUE Service Handle range of discovered
 *                        AICS \n
 *                        \ref GA_SUCCESS Service Discovery Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  MICP Handle for the Device - \ref MICP_HANDLE \n
 *                      If status is \ref GA_CONTINUE \n
 *                         - ga_data - Pointer to object of type
 *                           \ref GA_BRR_SVC_INFO \n
 *                         .
 *                      Else \n
 *                         - ga_data - NULL \n
 *                         .
 * \param [in] ga_datalen  If status is \ref GA_CONTINUE \n
 *                            - ga_datalen - Size of \ref GA_BRR_SVC_INFO \n
 *                            .
 *                         Else
 *                            - ga_datalen - NULL \n
 *                            .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_GET_CAPABILITIES_CNF                      MC_DISCOVER_MICS_OPTIONAL_SRVC_CNF

/**
 * This event indicates the setup of an AICS Session.
 * Callback is triggered once discovery of service, char and descriptors,
 * configuration for Notifications is complete.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref micc_cb )
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_SET_CAPABILITIES_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  MICP Handle for the Device - \ref MICP_HANDLE \n
 *                      If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_SET_CAPABILITIES_CNF                      MC_SETUP_AICS_CNF

/** \} */

/**
 * \name MICP Microphone Controller-Read
 * \{
 * \brief This section describes the Microphone Control Profile Read Events
 * for Microphone Controller.
 */

/**
 * This event is notified when a read response is received for Mute
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_GET_MUTE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR - Mute
 *                      \ref ga_micp_mics_mute_char_values
 * \param [in] ga_datalen  sizeof ( \ref UINT8 )
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_micp_mics_mute_char_values
 */
#define GA_MC_GET_MUTE_CNF                              MC_EVENT_GET_MUTE_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_GET_INPUT_STATE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Gain_Setting ( \ref INT8 ) \n
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_micp_aics_mute_field_values \n
 *                                     - Gain_Mode ( \ref UINT8 )
 *                                       \ref ga_micp_aics_gain_mode_field_values \n
 *                                     - Change_Counter ( \ref UINT8 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_micp_aics_mute_field_values
 * \sa ga_micp_aics_gain_mode_field_values
 */
#define GA_MC_AICS_GET_INPUT_STATE_CNF                  MC_AICS_EVENT_GET_INPUT_STATE_CNF

/**
 * This event is notified when a read response is received for Gain Setting
 * Properties characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Gain_Setting_Units ( \ref UINT8 ) \n
 *                                     - Gain_Setting_Minimum ( \ref INT8 ) \n
 *                                     - Gain_Setting_Maximum ( \ref INT8 ) \n
 * \param [in] ga_datalen  3
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_micp_aics_gain_setting
 */
#define GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF         MC_AICS_EVENT_GET_GAIN_SETTING_PROPRTY_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * Type characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_GET_INPUT_TYPE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to \ref UCHAR.
 *                      Refer \ref ga_bap_constants
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa ga_bap_constants
 */
#define GA_MC_AICS_GET_INPUT_TYPE_CNF                   MC_AICS_EVENT_GET_INPUT_TYPE_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * Status characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_GET_INPUT_STATUS_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer of type \ref UCHAR.
 *                      Refer \ref ga_micp_aics_input_status
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_micp_aics_input_status
 */
#define GA_MC_AICS_GET_INPUT_STATUS_CNF                 MC_AICS_EVENT_GET_INPUT_STATUS_CNF

/**
 * This event is notified when a read response is received for Audio Input
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF             MC_AICS_EVENT_GET_AUDIO_INPUT_DESC_CNF

/** \} */

/**
 * \name MICP Microphone Controller-Write
 * \{
 * \brief This section describes the Microphone Control Profile Write Events
 * for Microphone Controller.
 */

/**
 * This event is notified when a write response is received for Mute
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_SET_MUTE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Write Response received \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_micp_mics_mute_char_values
 */
#define GA_MC_SET_MUTE_CNF                              MC_EVENT_SET_MUTE_CNF

/**
 * This event is notified when a write response is received for Audio Input
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_CP_WT_CNF
 * \param [in] ga_data  \ref GA_SUCCESS : Write Response received \n
 * \param [in] ga_datalen  NULL \n
 * \param [in] ga_datalen  0
 *
 * \return\ref GA_SUCCESS (always)
 * \sa ga_micp_aics_cp_req_opc
 */
#define GA_MC_AICS_CP_WT_CNF                            MC_AICS_EVENT_CP_WT_CNF

/** \} */

/**
 * \name MICP Microphone Controller-Notification
 * \{
 * \brief This section describes the Microphone Control Profile Notification
 * Events for Microphone Controller.
 */

/**
 * This event is notified when a notification is received for Mute
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_MUTE_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to \ref UCHAR - Mute
 *                      \ref ga_micp_mics_mute_char_values
 * \param [in] ga_datalen  sizeof ( \ref UINT8 )
 *
 * \return \ref GA_SUCCESS (always)
 * \sa ga_micp_mics_mute_char_values
 */
#define GA_MC_MUTE_NTF                                  MC_EVENT_MUTE_NTF

/**
 * This event is notified when a notification is received for Audio Input
 * State characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_INPUT_STATE_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to \ref UCHAR with params as below:
 *                      Includes Param - Gain_Setting ( \ref INT8 ) \n
 *                                     - Mute ( \ref UINT8 )
 *                                       \ref ga_micp_aics_mute_field_values \n
 *                                     - Gain_Mode ( \ref UINT8 )
 *                                       \ref ga_micp_aics_gain_mode_field_values \n
 *                                     - Change_Counter ( \ref UINT8 ) \n
 * \param [in] ga_datalen  4
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_AICS_INPUT_STATE_NTF                      MC_AICS_EVENT_CP_INPUT_STATE_NTF

/**
 * This event is notified when a notification is received for Audio Input
 * Status characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_INPUT_STATUS_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer of type \ref UCHAR.
 *                      Refer \ref ga_micp_aics_input_status
 * \param [in] ga_datalen  1
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_AICS_INPUT_STATUS_NTF                     MC_AICS_EVENT_CP_INPUT_STATUS_NTF

/**
 * This event is notified when a notification is received for Audio Input
 * Description characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_AUDIO_INPUT_DESC_NTF
 * \param [in] ga_status  \ref GA_SUCCESS Notification received
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_AICS_AUDIO_INPUT_DESC_NTF                 MC_AICS_EVENT_AUDIO_INPUT_DESC_NTF

/** \} */

/**
 * \name MICP Microphone Controller-Release
 * \{
 * \brief This section describes the Microphone Control Profile Release Events
 * for Microphone Controller.
 */

/**
 * This event is notified when a MICS Service context is released.
 * Configuration of Notifications for char are disabled followed by release
 * of MICS context.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_RELEASE_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_RELEASE_CNF                               MC_RELEASE_MICS_CNF

/**
 * This event is notified after AICS Service context is released.
 * Configuration of Notifications for char are disabled followed by release
 * of AICS context.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref micc_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_RELEASE_CAPABILITY_CNF
 * \param [in] ga_status  \ref GA_SUCCESS Release Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ga_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_data - NULL \n
 *                           .
 * \param [in] ga_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ga_datalen - 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_MC_RELEASE_CAPABILITY_CNF                    MC_RELEASE_AICS_CNF

/** \} */

/** \} */

/**
 * \defgroup ga_micp_dev_evt_defs MICP Microphone Device Events
 * \{
 * \brief This section describes the Microphone Control Profile Events
 * for Microphone Device.
 */

/**
 * \name MICP Microphone Device-Read
 * \{
 * \brief This section describes the Microphone Control Profile Read Events
 * for Microphone Device.
 */

/**
 * This event is notified when a read request is received for Mute
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref mics_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_RD_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref MICP_EVENT_INFO
 *                      context - Use this while calling GA_mc_send_rsp() to
 *                                send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof( \ref MICP_EVENT_INFO)
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_mc_send_rsp() has to be called to send the read response.
 * Allowed values for Mute characteristics are defined in \ref
 * ga_micp_mics_mute_char_values
 */
#define GA_MC_RD_IND                                    MC_EVENT_GET_MUTE_IND

/**
 * This event is notified when a read request is received for
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref mics_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_RD_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref MICP_EVENT_INFO
 *                      context - Use this while calling GA_mc_send_rsp() to
 *                                send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof ( \ref MICP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_mc_send_rsp() has to be called to send the read response.
 */
#define GA_MC_AICS_RD_IND                               MC_AICS_EVENT_RD_IND

/** \} */

/**
 * \name MICP Microphone Device-Write
 * \{
 * \brief This section describes the Microphone Control Profile Write Events
 * for Microphone Device.
 */

/**
 * This event is notified when a write request is received for Mute
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref mics_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_SET_MUTE_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref MICP_EVENT_INFO
 *                      context - Use this while calling GA_mc_send_rsp() to
 *                                send response.
 *                      data - UINT8 depicting value of Mute
 *                             \ref ga_micp_mics_mute_char_values
 *                      datalen - sizeof ( \ref UINT8 )
 * \param [in] ga_datalen  \ref sizeof( \ref MICP_EVENT_INFO)
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_mc_send_rsp() has to be called to send the write response.
 */
#define GA_MC_SET_MUTE_IND                              MC_EVENT_SET_MUTE_IND

/**
 * This event is notified when a write without response request is received
 * for Audio Input Description characteristic from peer device with the
 * following values as parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref mics_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_SET_AUDIO_INPUT_DESC_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Without Response Request
 *                        received \n
 * \param [in] ga_data  Pointer to UCHAR, Can be a UTF-8 String of 0 or more
 *                      characters \n
 * \param [in] ga_datalen  0 or Length of UTF-8 String.
 *
 * \return \ref GA_SUCCESS (always)
 * \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 */
#define GA_MC_AICS_SET_AUDIO_INPUT_DESC_IND             MC_AICS_EVENT_SET_AUDIO_INPUT_DESC_IND

/**
 * This event is notified when a write request is received for Audio Input
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref mics_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_MC_AICS_CP_WT_IND
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to Object of type \ref MICP_EVENT_INFO
 *                      context - Use this while calling GA_mc_send_rsp() to
 *                                send response.
 *                      data - Pointer to UCHAR, that includes Opcode followed
 *                             by Parameters.
 *                      datalen - Data length of the Parameter.
 * \param [in] ga_datalen  \ref sizeof ( \ref MICP_EVENT_INFO).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_mc_send_rsp() has to be called to send the write response.
 */
#define GA_MC_AICS_CP_WT_IND                            MC_AICS_EVENT_CP_WT_IND

/** \} */

/** \} */

/**
 * This is an event to indicate random event. This event shall never be
 * generated.
 *
 * \param [in] ga_ep  NULL
 * \param [in] ga_event  \ref GA_EVENT_INVALID
 * \param [in] ga_status  \ref NULL \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_EVENT_INVALID                                MC_EVENT_INVALID

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_csip_module
 * \{
 */

/**
 * \defgroup ga_csip_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * GA CSIP Layer.
 */

/**
 * \defgroup ga_csip_api_evt_defs Events
 * \{
 * \brief This section describes the Coordinated Set Identification Events.
 */

/**
 * \defgroup ga_csip_setco_evt_defs CSIP SetCoordinator Event
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Events for SetCoordinator.
 */

/**
 * \name CSIP SetCoordinator-Setup
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Setup Events for SetCoordinator.
 */

/**
 * This event is notified on discovery of CSIS service with the
 * following values as parameters in the
 * \ref GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_DISCOVER_CNF \n
 * \param [in] ga_status  \ref GA_SUCCESS : Service found \n
 *                        \ref GA_FAILURE : Service not found \n
 * \param [in] ga_data  Pointer to object of type \ref GA_BRR_SVC_INFO \n
 *                      Paramter depicts the Service Handle - Start and End.
 * \param [in] ga_datalen  \ref sizeof( \ref GA_BRR_SVC_INFO) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_DISCOVER_CNF                              CSIP_DISCOVER_CNF

/**
 * This event is notified on successful setup of CSIP context with the peer
 * device with following values as parameters for \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 * Char and Descriptor discovery is followed by Configuration of Notification
 * of any char if applicable.
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETUP_CNF \n
 * \param [in] ga_status  \ref GA_CONTINUE : Setup Ongoing \n
 *                        \ref GA_SUCCESS : Setup Done \n
 *                        \ref GA_FAILURE : Setup Failed \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETUP_CNF                                 CSIP_SETUP_CNF

/** \} */

/**
 * \name CSIP SetCoordinator-Read
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Read Events for SetCoordinator.
 */

/**
 * This event is notified when a read response is received for Set Identity
 * Resolving Key characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SIRK_GET_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_SIRK \n
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_SIRK) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SIRK_GET_CNF                              CSIP_SIRK_GET_CNF

/**
 * This event is notified when a read response is received for Coordinated
 * Set Size characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETSIZE_GET_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to object of type \ref UINT8 \n
 * \param [in] ga_datalen  \ref sizeof( \ref UINT8) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETSIZE_GET_CNF                           CSIP_SETSIZE_GET_CNF


/**
 * This event is notified when a read response is received for Set Member
 * Lock characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_LOCK_GET_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to object of type \ref UINT8 \n
 * \param [in] ga_datalen  \ref sizeof( \ref UINT8) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETMEMBER_LOCK_GET_CNF                    CSIP_SETMEMBER_LOCK_GET_CNF

/**
 * This event is notified when a read response is received for Set Member Rank
 * characteristic from peer device with the following values as parameters
 * in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_RANK_GET_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] ga_data  Pointer to object of type \ref UINT8 \n
 * \param [in] ga_datalen  \ref sizeof( \ref UINT8 ) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETMEMBER_RANK_GET_CNF                    CSIP_SETMEMBER_RANK_GET_CNF

/** \} */

/**
 * \name CSIP SetCoordinator-Write
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Write Events for SetCoordinator.
 */

/**
 * This event is notified when a write response is received for Set Member
 * Lock characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_LOCK_SET_CNF
 * \param [in] ga_status  \ref GA_SUCCESS : Write Response received \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETMEMBER_LOCK_SET_CNF                    CSIP_SETMEMBER_LOCK_SET_CNF

/** \} */

/**
 * \name CSIP SetCoordinator-Notification
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Notification Events for SetCoordinator.
 */

/**
 * This event is notified when a notification is received for Set Identity
 * Resolving Key characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SIRK_NTF \n
 * \param [in] ga_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_SIRK \n
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_SIRK) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SIRK_NTF                                CSIP_SIRK_NTF

/**
 * This event is notified when a notification is received for Coordinated Set
 * Size characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETSIZE_NTF \n
 * \param [in] ga_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] ga_data  Pointer to object of type \ref UINT8 \n
 * \param [in] ga_datalen  \ref sizeof( \ref UINT8) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETSIZE_NTF                             CSIP_SET_SIZE_NTF

/**
 * This event is notified when a notification is received for Coordinated Set
 * Member Lock characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csic_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_LOCK_STATUS_NTF \n
 * \param [in] ga_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] ga_data  Pointer to object of type \ref UINT8 \n
 * \param [in] ga_datalen  \ref sizeof( \ref UINT8) \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_SETMEMBER_LOCK_STATUS_NTF               CSIP_SETMEMBER_LOCK_STATUS_NTF

/** \} */

/**
 * \name CSIP SetCoordinator-Release
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Release Events for SetCoordinator.
 */

/**
 * The event is notified when CSIS context is released with the \ref
 * GA_CALLBACK callback ( \ref GA_PROFILE_CB \ref csic_cb ).
 * When the event is notified, notification is disabled and context will be
 * cleared.
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_RELEASE_CNF \n
 * \param [in] ga_status  \ref GA_SUCCESS : Release completed \n
 *                        \ref GA_FAILURE : Release Failed \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_CS_RELEASE_CNF                               CSIP_RELEASE_CNF

/** \} */

/** \} */

/**
 * \defgroup ga_csip_setmem_evt_defs CSIP SetMember Event
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Events for SetMember.
 */

/**
 * \name CSIP SetMember-Read
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Read Events for SetMember.
 */

/**
 * This event is notified when a read request is received for Set Identity
 * Resolving Key characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csis_cb )
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SIRK_GET_IND \n
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_EVENT_INFO \n
 *                      context - Use this while calling GA_cs_send_response()
 *                                to send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_EVENT_INFO) \n
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_cs_send_response() has to be called to send the read response.
 */
#define GA_CS_SIRK_GET_IND                              CSIP_SIRK_GET_IND

/**
 * This event is notified when a read request is received for Coordinated Set
 * Size characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csis_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETSIZE_GET_IND \n
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_EVENT_INFO \n
 *                      context - Use this while calling GA_cs_send_response()
 *                                to send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_EVENT_INFO) \n
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_cs_send_response() has to be called to send the read response.
 * Allowed values for the Coordinated Set Size characteristic are
 * integers in the range 0x02 to 0xFF. Values 0x00 and 0x01 are Prohibited.
 */
#define GA_CS_SETSIZE_GET_IND                           CSIP_SETSIZE_GET_IND

/**
 * This event is notified when a read request is received for Coordinated Set
 * Member Lock characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csis_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_LOCK_GET_IND \n
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_EVENT_INFO \n
 *                      context - Use this while calling GA_cs_send_response()
 *                                to send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_EVENT_INFO) \n
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_cs_send_response() has to be called to send the read response.
 * Allowed values for the Coordinated Set Member Lock characteristic are
 * defined as \ref ga_csip_set_member_lock.
 */
#define GA_CS_SETMEMBER_LOCK_GET_IND                    CSIP_SETMEMBER_LOCK_GET_IND

/**
 * This event is notified when a read request is received for Coordinated Set
 * Member Rank characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csis_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_RANK_GET_IND \n
 * \param [in] ga_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_EVENT_INFO \n
 *                      context - Use this while calling GA_cs_send_response()
 *                                to send response.
 *                      data - NULL
 *                      datalen - 0
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_EVENT_INFO) \n
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_cs_send_response() has to be called to send the read response.
 * Allowed values for the Set Member Rank characteristics are positive integer
 * numbers starting from 0x01.
 */
#define GA_CS_SETMEMBER_RANK_GET_IND                    CSIP_SETMEMBER_RANK_GET_IND

/** \} */

/**
 * \name CSIP SetMember-Write
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Write Events for SetMember.
 */

/**
 * This event is notified when a write request is received for Set Member Lock
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_CALLBACK callback
 * ( \ref GA_PROFILE_CB \ref csis_cb ).
 *
 * \param [in] ga_ep  Pointer to peer device handle \ref
 * \param [in] ga_event  \ref GA_CS_SETMEMBER_LOCK_SET_IND \n
 * \param [in] ga_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] ga_data  Pointer to object of type \ref CSIP_EVENT_INFO \n
 *                      context - Use this while calling GA_cs_send_response()
 *                                to send response.
 *                      data - UINT8 depicting value of Lock
 *                             \ref ga_csip_set_member_lock
 *                      datalen - sizeof ( \ref UINT8 )
 * \param [in] ga_datalen  \ref sizeof( \ref CSIP_EVENT_INFO) \n
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_cs_send_response() has to be called to send the write response.
 */
#define GA_CS_SETMEMBER_LOCK_SET_IND                    CSIP_SETMEMBER_LOCK_SET_IND

/** \} */

/** \} */

/**
 * This is an event to indicate unknown event. This event shall never be
 * generated.
 *
 * \param [in] ga_ep  NULL
 * \param [in] ga_event  \ref CSIP_EVENT_INVALID
 * \param [in] ga_status  \ref NULL \n
 * \param [in] ga_data  NULL \n
 * \param [in] ga_datalen  0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CSIP_EVENT_INVALID                              0xFFU

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \addtogroup ga_bap_defines
 * \{
 */

/**
 * \addtogroup ga_bap_constants
 * \{
 */

/**
 * \name Broadcast Encryption Code Size
 * \{
 * \brief This section defines the Broadcast Code Encryption Size.
 * As defined in BAP Specification.
 */

/** Size of Broadcast code for encryption */
#define GA_BROADCAST_CODE_SIZE                      BAP_BC_CODE_SIZE

/** \} */

/**
 * \name Broadcast Profile Types
 * \{
 * \brief This section defines the different Broadcast profile types
 * based on the adopted Broadcast profile specifiations
 */

/** No Broadcast Profile */
#define GA_BCAST_SIG_PROFILE_NONE                   0U

/** Public Broadcast Profile */
#define GA_BCAST_SIG_PROFILE_PUBLIC_BROADCAST       1U

/** \} */

/**
 * \name ASE CP Response Codes
 * \{
 * \brief This section lists the response codes for use
 * by the module. As defined in ASCS Specification.
 */

/**
 * ASE CP Response codes: The server has successfully completed the
 * client-initiated ASE Control operation.
 */
#define GA_SEP_RSP_SUCCESS                      0x00
/**
 * ASE CP Response codes: The server does not support the client-initiated
 * ASE Control operation defined by the opcode written by the client.
 */
#define GA_SEP_RSP_UNSUPP_OPCODE                0x01U
/**
 * ASE CP Response codes: The server has detected an invalid length operation
 * written by the client.
 */
#define GA_SEP_RSP_INVALID_LENGTH               0x02U
/**
 * ASE CP Response codes: The server has detected that an ASE_ID written by
 * the client does not match an ASE_ID in an exposed ASE characteristic value
 * for that client.
 */
#define GA_SEP_RSP_INVALID_ASEID                0x03U
/**
 * ASE CP Response codes: The server has detected that the client-initiated
 * ASE Control operation would cause an invalid ASE state machine transition.
 */
#define GA_SEP_RSP_INVALID_ASE_SM_TRANSITION    0x04U
/**
 * ASE CP Response codes: The server has detected that the client-initiated
 * ASE Control operation is not valid for the ASE direction.
 */
#define GA_SEP_RSP_INVALID_ASE_DIRECTION        0x05U
/**
 * ASE CP Response codes: The server has detected that the audio capabilities
 * requested during a Config Codec operation are not supported
 * (i.e., the server has not exposed the requested configuration in any PAC
 * record).
 */

#define GA_SEP_RSP_UNSUPP_AUDIO_CAPABILITIES    0x06U
/**
 * ASE CP Response codes: The server has detected that it does not support one
 * or more configuration parameter values written by the client. Shall not be
 * used when the Reason value is 0x04(Framing).
 * Refer \ref ga_bap_constants for reason codes.
 */
#define GA_SEP_RSP_UNSUPP_CONFIG_PARAM_VAL      0x07U
/**
 * ASE CP Response codes: The server has rejected one or more configuration
 * parameter values written by the client.
 * Refer \ref ga_bap_constants for reason codes.
 */
#define GA_SEP_RSP_REJECTED_CONFIG_PARAM_VAL    0x08U
/**
 * ASE CP Response codes: The server has detected one or more invalid
 * configuration parameter values written by the client.
 * Refer \ref ga_bap_constants for reason codes.
 */
#define GA_SEP_RSP_INVALID_CONFIG_PARAM_VAL     0x09U
/**
 * ASE CP Response codes: The server has detected an unsupported Metadata
 * Type written by the client.
 */
#define GA_SEP_RSP_UNSUPP_METADATA              0x0AU
/**
 * ASE CP Response codes: The server has rejected a Metadata Type written
 * by the client.
 */
#define GA_SEP_RSP_REJECTED_METADATA            0x0BU
/**
 * ASE CP Response codes: This Response_Code is used to inform the client
 * that the Metadata Value is incorrectly formatted.
 */
#define GA_SEP_RSP_INVALID_METADATA             0x0CU
/**
 * ASE CP Response codes: The server is unable to successfully complete the
 * client-initiated ASE Control operation because of insufficient resources.
 */
#define GA_SEP_RSP_INSUFFICIENT_RESOURCES       0x0DU
/**
 * ASE CP Response codes: The server has encountered an unspecified error.
 */
#define GA_SEP_RSP_UNSPECIFIED_ERROR            0x0EU

/** \} */

/**
 * \name ASE CP Response Reason Codes
 * \{
 * \brief This section lists the reason codes for use
 * by the module. As defined in ASCS Specification.
 * ASE CP Reason codes: Used for only below ASE CP Response Codes.
 * \ref GA_SEP_RSP_UNSUPP_CONFIG_PARAM_VAL
 * \ref GA_SEP_RSP_REJECTED_CONFIG_PARAM_VAL
 * \ref GA_SEP_RSP_INVALID_CONFIG_PARAM_VAL
 */

/** ASE CP Response Reason codes: None */
#define GA_SEP_RSP_RSN_NONE                         0x00U
/** ASE CP Response Reason codes: Codec_ID */
#define GA_SEP_RSP_RSN_CODEC_ID                     0x01U
/** ASE CP Response Reason codes: Codec_Specific_Configuration */
#define GA_SEP_RSP_RSN_CODEC_SPECIFIC_CONFIG        0x02U
/** ASE CP Response Reason codes: SDU_Interval */
#define GA_SEP_RSP_RSN_SDU_INTERVAL                 0x03U
/** ASE CP Response Reason codes: Framing */
#define GA_SEP_RSP_RSN_FRAMING                      0x04U
/** ASE CP Response Reason codes: PHY */
#define GA_SEP_RSP_RSN_PHY                          0x05U
/** ASE CP Response Reason codes: Maximum_SDU_Size */
#define GA_SEP_RSP_RSN_MAX_SDU_SIZE                 0x06U
/** ASE CP Response Reason codes: Retransmission_Number */
#define GA_SEP_RSP_RSN_RETRANSMISSION_NUMBER        0x07U
/** ASE CP Response Reason codes: Max_Transport_Latency */
#define GA_SEP_RSP_RSN_MAX_TRANSPORT_LATENCY        0x08U
/** ASE CP Response Reason codes: Presentation_Delay */
#define GA_SEP_RSP_RSN_PRESENTATION_DELAY           0x09U
/** ASE CP Response Reason codes: Invalid_ASE_CIS_Mapping */
#define GA_SEP_RSP_RSN_INVALID_ASE_CIS_MAPPING      0x0AU

/** \} */

/**
 * \name ASE State
 * \{
 * \brief This section lists the ASE STates for use
 * by the module. As defined in ASCS Specification.
 * Represented as \ref GA_ASE_STATE
 */

/**
 * ASE State: Idle \n
 * The ASE has no codec configuration or QoS configuration applied
 */
#define GA_ASE_STATE_IDLE                           SEP_STATE_IDLE

/**
 * ASE State: Codec Configured \n
 * The ASE has a codec configuration applied. The codec configuration may
 * have been autonomously applied by the server or it may have been requested
 * by the client. The server is exposing its preferred QoS parameters;
 * however, the ASE has no QoS configuration applied yet.
 */
#define GA_ASE_STATE_CODEC_CONF                     SEP_STATE_CODEC_CONFIGURED

/**
 * ASE State: QoS Configured  \n
 * The ASE has a codec configuration and a QoS configuration applied.
 * The applied QoS configuration at the host level may be different from the
 * actual configuration applied to a CIS by the client controller.
 * A CIS can exist, but the ASE has not been coupled to the CIS.
 */
#define GA_ASE_STATE_QOS_CONF                       SEP_STATE_QOS_CONFIGURED

/**
 * ASE State: Enabling \n
 * The ASE has a codec configuration and a QoS configuration applied and
 * any Metadata applied by the client or server is associated with the ASE.
 * A CIS can exist, but the ASE has not been coupled to the CIS.
 * There is a risk of some lost audio data packets in this state if either
 * server or client begin transmitting audio data before the ASE is
 * in the Streaming state.
 */
#define GA_ASE_STATE_ENABLING                       SEP_STATE_ENABLING

/**
 * ASE State: Streaming \n
 * The ASE has a codec configuration and a QoS configuration applied,
 * and any Metadata applied by the client or server is associated
 * with the ASE. The ASE is coupled to a CIS. The device acting as Audio Sink
 * has initiated a Receiver Start Ready operation that has successfully
 * completed and the ASE is ready to receive or transmit audio data.
 */
#define GA_ASE_STATE_STREAMING                      SEP_STATE_STREAMING

/**
 * ASE State: Disabling \n
 * Applies only to Source ASE. The ASE has a codec configuration and a QoS
 * configuration applied. Any CIS established to transport audio data for
 * the ASE might remain established or might be disconnected however the ASE
 * is being decoupled from the CIS. Any Metadata previously applied remains
 * associated with the ASE in this state. The ASE remains ready to transmit
 * audio data until the device acting as Audio Sink has initiated a
 * Receiver Stop Ready operation that has successfully completed.
 */
#define GA_ASE_STATE_DISABLING                      SEP_STATE_DISABLING

/**
 * ASE State: Releasing \n
 * Any CIS established to transport audio data for the ASE is being
 * disconnected or has been disconnected. Any previously applied codec
 * configuration may be cached by the server, or the server may cache a codec
 * configuration of the server’s choosing, or the codec configuration
 * may be removed. Any previously applied QoS configuration is no longer
 * valid. Any Metadata previously applied is no longer associated with the ASE.
 */
#define GA_ASE_STATE_RELEASING                      SEP_STATE_RELEASING

/** \} */

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_csip_module
 * \{
 */

/**
 * \addtogroup ga_csip_defines
 * \{
 */

/**
 * \defgroup ga_csip_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind GA CSIP Layer.
 */

/**
 * \name Application error code
 * \{
 * \brief This section lists Application error codes,
 * As defined in CSIS Specification.
 */

/** The lock cannot be granted because the server is already locked. */
#define GA_CSIS_ERR_LOCK_DENIED                     CSIS_ERR_LOCK_DENIED

/**
 * The lock cannot be released because another
 * client has locked the Coordinated Set.
 */
#define GA_CSIS_ERR_LOCK_RELEASE_NOT_ALLOWED        CSIS_ERR_LOCK_RELEASE_NOT_ALLOWED

/**
 * The client attempts to write an RFU value to the
 * Set Member Lock characteristic value.
 */
#define GA_CSIS_ERR_INVALID_LOCK_VALUE              CSIS_ERR_INVALID_LOCK_VALUE

/**
 * The server only supports exposing the Set Identity Resolving
 * Key (SIRK) via an out-of-band (OOB) procedure.
 */
#define GA_CSIS_ERR_OOB_SIRK_ONLY                   CSIS_ERR_OOB_SIRK_ONLY

/** The client that made the request is the current owner of the lock. */
#define GA_CSIS_ERR_LOCK_ALREADY_GRANTED            CSIS_ERR_LOCK_ALREADY_GRANTED

/** \} */
/** \} */
/** \} */
/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \addtogroup ga_vcp_module
 * \{
 */

/**
 * \addtogroup ga_vcp_defines
 * \{
 */

/**
 * \defgroup ga_vcp_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/** GA Volume Control Profile Context Reference for an Endpoint */
typedef UINT8 GA_VC_CONTEXT;

/** GA Volume Control Profile Renderer Reference for an Endpoint */
typedef UINT8 GA_VR_HANDLE;

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_micp_module
 * \{
 */

/**
 * \addtogroup ga_micp_defines
 * \{
 */

/**
 * \defgroup ga_micp_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/** GA Microphone Control Profile Context Reference for an Endpoint */
typedef UINT8 GA_MC_CONTEXT;

/** GA Microphone Control Device Reference for an Endpoint */
typedef UINT8 GA_MD_HANDLE;

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_csip_module
 * \{
 */

/**
 * \addtogroup ga_csip_defines
 * \{
 */

/**
 * \defgroup ga_csip_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/**
 * GA Coordinated Set Identification Profile Context Reference for an
 * Endpoint
 */
typedef UINT8 GA_CS_CONTEXT;

/** GA Coordinated Set Member Reference for an Endpoint */
typedef UINT8 GA_CM_HANDLE;

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \addtogroup ga_bap_defines
 * \{
 */

/**
 * \defgroup ga_bap_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/** GA ASE States */
typedef UINT8 GA_ASE_STATE;

/** GA Context Reference for an Endpoint */
typedef UINT8 GA_CONTEXT;

/** GA LC3 Codec Specific Capability */
typedef struct _GA_LC3_CODEC_SPECIFIC_CAP
{
    /**
     * Supported Sampling Frequencies,
     * Refer \ref ga_ltv_val
     */
    UINT16 supported_sf;

    /**
     * Supported Frame Durations,
     * Refer \ref ga_ltv_val
     */
    UINT8 supported_fd;

    /**
     * Supported Audio Channel Counts,
     * Refer \ref ga_ltv_val
     */
    UINT8 supported_acc;

    /**
     * Supported Octets per Codec Frame,
     * Refer \ref ga_ltv_val
     */
    UINT32 supported_opcf;

    /**
     * Supported Max Codec Frames Per SDU,
     * Refer \ref ga_ltv_val
     */
    UINT8 supported_mcfpSDU;

} GA_LC3_CODEC_SPECIFIC_CAP;

/** GA LC3 Codec Configuration */
typedef struct _GA_LC3_CODEC_SPECIFIC_CONF
{
    /** Sampling Frequency, Refer \ref ga_ltv_val */
    UINT8 sf;

    /** Frame Duration, Refer \ref ga_ltv_val */
    UINT8 fd;

    /** Audio Channel Allocation, Refer \ref ga_ltv_val */
    UINT32 aca;

    /** Octets per Codec Frame, Refer \ref  ga_ltv_val */
    UINT16 opcf;

    /**
     * Codec Frame Blocks Per SDU,
     * Refer \ref  ga_ltv_val
     */
    UINT8 mcfpSDU;

} GA_LC3_CODEC_SPECIFIC_CONF;

/** GA Supported Audio Capability */
typedef struct _GA_AUDIO_CAP
{
    /** Codec Information */
    GA_CODEC_INFO codec;

    /** Preferred Context Types for the capability */
    UINT16 pref_audio_contexts;

} GA_AUDIO_CAP;

/** GA PACS Audio Record */
typedef struct _GA_PACS_AUDIO_RECORD
{
    /**
     * Sink Audio Locations, Refer \ref ga_bap_constants
     */
    UINT32 sink_audio_locations;

    /**
     * Source Audio Locations, Refer \ref ga_bap_constants
     */
    UINT32 src_audio_locations;

    /**
     * Available Audio Contexts for the capability, Refer \ref ga_bap_constants
     */
    UINT16 avail_sink_contexts;

    /**
     * Available Audio Contexts for the capability, Refer \ref ga_bap_constants
     */
    UINT16 avail_src_contexts;

    /**
     * Supported Audio Contexts for the capability, Refer \ref ga_bap_constants
     */
    UINT16 supp_sink_contexts;

    /**
     * Supported Audio Contexts for the capability, Refer \ref ga_bap_constants
     */
    UINT16 supp_src_contexts;

} GA_PACS_AUDIO_RECORD;

/** GA Codec Configuration */
typedef struct _GA_CODEC_CONFIG
{
    /** Codec ID */
    UINT8 id;

    /** Codec Configuration */
    void * conf;

} GA_CODEC_CONFIG;

/** GA QOS Capability */
typedef struct _GA_QOS_CAP
{
    /** Framing Preference */
    UINT8 pref_framing;

    /** PHY Preference */
    UINT8 pref_phy;

    /** Retransmission Number Preference */
    UINT8 pref_retxn;

    /** Max Transport Latency Preference */
    UINT16 pref_max_transport_latency;

    /** Presentation Delay supported - Minimum */
    UINT32 supp_presentation_delay_min;
    /** Presentation Delay supported - Maximum */
    UINT32 supp_presentation_delay_max;

    /** Presentation Delay preferred - Minimum */
    UINT32 pref_presentation_delay_min;
    /** Presentation Delay preferred - Maximum */
    UINT32 pref_presentation_delay_max;

} GA_QOS_CAP;

/** GA QOS Configuration */
typedef BAP_ASE_QOS_CONF GA_QOS_CONF;

/** GA Channel Reference */
typedef BAP_CHANNEL GA_CHANNEL;

/** GA Content Information */
typedef struct _GA_CONTENT_INFO
{
    /** Contexts for the content */
    UINT16 contexts;

    /** List of CCIDs */
    UINT8 * ccid;

    /** Number of CCIDs */
    UINT8 num_ccid;

} GA_CONTENT_INFO;

/** Stream End point Configuration */
typedef struct _GA_SEP_CONF
{
    /** Role */
    UINT8 role;

    /** Target Latency */
    UINT8 target_latency;

    /** Target Phy */
    UINT8 target_phy;

    /** Codec Capability */
    GA_CODEC_INFO codec;

} GA_SEP_CONF;

/** GA Endpoint Capability */
typedef struct _GA_CAP_ENDPOINT
{
    /**
     * Capability data:
     * If \ref GA_CAP_TYPE_CODECS
     *     data: Pointer to data with First byte indicating Number of PACS Records,
     *           followed by \ref GA_CODEC_INFO followed by \ref GA_METADATA.
     *     data[0] -> No of PAC Records followed by params as above.
     *
     * If \ref GA_CAP_TYPE_LOCATIONS,
     *     data: Pointer to \ref UINT32, Represents Audio Location values as
     *           bitmask.
     *           Refer \ref ga_bap_constants
     *
     * If \ref GA_CAP_TYPE_SUPP_CONTEXTS
     *     data: Pointer to \ref UINT16, Represents Supported Context values
     *           as bitmask.
     *           Refer \ref ga_bap_constants
     *
     * If \ref GA_CAP_TYPE_AVAIL_CONTEXTS,
     *     data: Pointer to \ref UINT16, Represents Supported Context values
     *           as bitmask.
     *           Refer \ref ga_bap_constants
     */
    UCHAR * data;

    /**
     * Capability data length:
     * If \ref GA_CAP_TYPE_CODECS
     *     datalen: Length of the data above.
     *
     * If \ref GA_CAP_TYPE_LOCATIONS,
     *     datalen: sizeof( \ref UINT32 )
     *
     * If \ref GA_CAP_TYPE_SUPP_CONTEXTS,
     *     datalen: sizeof( \ref UINT16 )
     *
     * If \ref GA_CAP_TYPE_AVAIL_CONTEXTS,
     *     datalen: sizeof( \ref UINT16 )
     */
    UINT16 datalen;

    /** Peer role under consideration, Refer \ref ga_bap_constants */
    UINT8 role;

    /** Capability type, Refer \ref ga_bap_constants */
    UINT8 type;

} GA_CAP_ENDPOINT;

/** ASE Information */
typedef struct _GA_ASE_INFO
{
    /** ASE Info data */
    UCHAR * data;

    /** ASE Infor datalen */
    UINT16 datalen;

    /** ASE Index */
    UINT8 sep_id;

    /** ASE Role of the Remote */
    UINT8 ase_role;

} GA_ASE_INFO;

/** SEP Callback Information */
typedef struct _GA_SEP_INFO
{
    /** Event specific information */
    void * info;

    /** SEP Id */
    UINT8 sep_id;

} GA_SEP_INFO;

/** ASE Format referenced in \ref GA_SEP_DISCOVER_CNF */
typedef struct _GA_ASE_INFO_IN_IDLE
{
    /** ASE ID */
    UINT8 ase_id;

    /** ASE State, Ref \ref GA_ASE_STATE */
    UINT8 ase_state;
} GA_ASE_INFO_IN_IDLE;

/** ASE Format referenced in \ref GA_SEP_CONFIGURE_CNF */
typedef struct _GA_ASE_INFO_IN_CODEC_CONF
{
    /** ASE ID */
    UINT8 ase_id;

    /** ASE State, Ref \ref GA_ASE_STATE */
    UINT8 ase_state;

    /** QoS Codec Capabilities of the peer ASE */
    GA_QOS_CAP qos_cap;

    /** Codec Info of the peer ASE */
    GA_CODEC_INFO codec_info;
} GA_ASE_INFO_IN_CODEC_CONF;

/** ASE Format referenced in \ref GA_SEP_SETUP_CNF */
typedef struct _GA_ASE_INFO_IN_QOS_CONF
{
    /** ASE ID */
    UINT8 ase_id;

    /** ASE State, Ref \ref GA_ASE_STATE */
    UINT8 ase_state;

    /** QoS Configuration post config with the peer ASE */
    GA_QOS_CONF qos_cfg;
} GA_ASE_INFO_IN_QOS_CONF;

/**
 * ASE Format referenced in \ref GA_SEP_START_CNF, \ref GA_SEP_SUSPEND_CNF,
 * \ref GA_SEP_UPDATE_CNF
 */
typedef struct _GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING
{
    /** ASE ID */
    UINT8 ase_id;

    /** ASE State, Ref \ref GA_ASE_STATE */
    UINT8 ase_state;

    /** Channel Information */
    GA_CHANNEL channel;

    /** Metadata Total Length */
    UINT8 metadata_length;

    /** LTV Formatted Metadata */
    UINT8* metadata;

} GA_ASE_INFO_IN_ENABLING_STREAMING_DISABLING;

/** \} */
/** \} */

/**
 *  \defgroup ga_cb Application Callback
 *  \{
 *  \brief This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 * GA Profile Callback
 *
 * \param [in] ga_ep      Peer Device Info
 * \param [in] ga_event   Event type
 * \param [in] ga_status  Event status
 * \param [in] ga_data    Event Data
 * \param [in] ga_datalen Event Data Length
 */
typedef GA_RESULT (* GA_PROFILE_CB)
                  (
                      GA_ENDPOINT * ga_ep,
                      UINT8 ga_event,
                      UINT16 ga_status,
                      void * ga_data,
                      UINT16 ga_datalen
                  );

/** GA Set Member parameters */
typedef struct _GA_SET_MEMBER
{
    /* Set SIRK, 1 octet type + 16 octet value */
    UCHAR sirk[1U + CSIP_SIRK_LEN];

    /* Set Size */
    UINT8 size;

    /* Set Member Rank */
    UINT8 rank;

} GA_SET_MEMBER;

/** Collective callback structure */
typedef struct _GA_CALLBACK
{
    /** GA Profile Callback - Unicast Client */
    GA_PROFILE_CB gac_cb;
    /** GA Profile Callback - Unicast Server */
    GA_PROFILE_CB gas_cb;

    /** GA Profile Callback - Broadcast Source */
    GA_PROFILE_CB bcsrc_cb;
    /** GA Profile Callback - Broadcast Sink */
    GA_PROFILE_CB bcsnk_cb;

    /** GA Profile Callback - VCP Volume Controller */
    GA_PROFILE_CB vcc_cb;
    /** GA Profile Callback - VCP Volume Renderer */
    GA_PROFILE_CB vcs_cb;

    /** GA Profile Callback - MICP Microphone Controller */
    GA_PROFILE_CB micc_cb;
    /** GA Profile Callback - MICP Microphone Device */
    GA_PROFILE_CB mics_cb;

    /** GA Profile Callback - CSIP Set Coordinator */
    GA_PROFILE_CB csic_cb;
    /** GA Profile Callback - CSIP Set Member */
    GA_PROFILE_CB csis_cb;

} GA_CALLBACK;

/** \} */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

/**
 * \defgroup ga_bap_api_sequences API Sequences
 * \{
 * \brief This section describes the EtherMind GA CAP Layer API Sequences.
 */

/**
 * \defgroup ga_bap_api_seq GA BAP API Sequences
 * \{
 * \brief This section describes the BAP API Sequences.
 * \image html ASE_Source_State_Machine.png
 * <b>ASE Source State Machine</b> \n
 * \image html ASE_Sink_State_Machine.png
 * <b>ASE Sink State Machine</b> \n
 * As defined in ASCS Specification. \n
 * MSC depicting the flow of APIs and Events.
 * \mscfile BAP.signalling
 */

/** \} */
/** \} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup ga_bap_api_defs API Definitions
 * \{
 * \brief This section describes the Generic Audio APIs.
 * This section describes the BAP Procedure APIs.
 */

/**
 * \defgroup ga_common_defs GA Common APIs
 * \{
 * \brief This section describes the Generic Audio Common APIs.
 */

/**
 *  \brief Initialize the Generic Audio (GA) layer.
 *
 *  \par Description
 *  This routine initializes the GA layer and registers and application callback.
 *
 *  \param None
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_init(void);

#ifdef GA_HAVE_DYNAMIC_CONFIG
/**
 *  \brief Setup the configuration of the Generic Audio (GA) layer.
 *
 *  \par Description
 *  This routine initializes the default configuration for the GA components.
 *
 *  \param None
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_configure
          (
              /* IN */ GA_CONFIG * config
          );
#endif /* GA_HAVE_DYNAMIC_CONFIG */

/**
 *  \brief Prepare the roles for Generic Audio (GA) CAP layer.
 *
 *  \par Description
 *  This routine sets the role requirements of the application at the
 *  GA CAP layer. In case of support for Acceptor role, the application
 *  can also specify if it is a member of a Coordinated set with the
 *  parameters in \ref GA_SET_MEMBER
 *
 *  \param [in] role Bitmask of CAP roles to be supported
 *  \param [in] member Member information of type \ref GA_SET_MEMBER
 *  if Acceptor is part of a Coordinated Set, otherwise NULL.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_prepare(UINT8 role, GA_SET_MEMBER * member);

/**
 *  \brief Shutdown the Generic Audio (GA) layer.
 *
 *  \par Description
 *  This routine shuts down the GA layer and cleans up all data.
 *
 *  \param None
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_shutdown(void);

/**
 *  \brief Initialize the GA core module specified for the role.
 *
 *  \par Description
 *  This routine initializes the given GA core module for the specified
 *  role, and saves the application callback provided.
 *
 *  \param [in] id GA Module ID
 *  \param [in] role GA Specific role for the module
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_module_init
          (
              /* IN */ UINT8 id,
              /* IN */ UINT8 role,
              /* IN */ GA_PROFILE_CB cb
          );

/**
 *  \brief Shutdown the GA core module specified for the role.
 *
 *  \par Description
 *  This routine shuts down the given GA core module for the specified
 *  role.
 *
 *  \param [in] id GA Module ID
 *  \param [in] role GA Specific role for the module
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_module_shutdown
          (
              /* IN */ UINT8 id,
              /* IN */ UINT8 role
          );

/** \} */

/**
 * \defgroup ga_server_defs GA Server APIs
 * \{
 * \brief This section describes the Generic Audio APIs
 * for Server.
 */

/**
 * \name GA Server-Setup
 * \{
 * \brief This section describes the Generic Audio Setup
 * APIs for Server.
 */

/**
 *  \brief Initialize the GA Unicast Server.
 *
 *  \par Description
 *  This routine initializes GA Unicast Server
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_uc_server_init(cb)       GA_module_init(GA_MODULE_BAP, BAP_ROLE_SERVER, (cb))

/**
 *  \brief Shutdown the GA Unicast Server.
 *
 *  \par Description
 *  This routine shuts down GA Unicast Server
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_uc_server_shutdown()   GA_module_shutdown(GA_MODULE_BAP, BAP_ROLE_SERVER)

/**
 *  \brief Register the GA capabilities.
 *
 *  \par Description
 *  This routine registers the audio capabilities of the application profile
 *  residing over the GA layer.
 *
 *  \param [in] role \ref GA_ROLE_SOURCE or \ref GA_ROLE_SINK role for this capability
 *  \param [in] cap_count Count of capabilitites supported for this role
 *  \param [in] audio_cap Reference to \ref GA_AUDIO_CAP data giving the audio capability for this role
 *  \param[out] record_id Record ID of the registered capabilities.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_register_audio_capabilities
          (
              /* IN */ UINT8 role,
              /* IN */ UINT8 cap_count,
              /* IN */ GA_AUDIO_CAP * audio_cap,
              /* OUT */ UINT8* record_id
          );

/**
 *  \brief Register the GA Audio Contexts and Locations.
 *
 *  \par Description
 *  This routine registers the audio capabilities of the application profile
 *  residing over the GA layer.
 *
 *  \param [in] role \ref GA_ROLE_SOURCE or \ref GA_ROLE_SINK role for this
 *              capability
 *
 *  \param [in] avail_audio_contexts Available Audio contexts for this role
 *              Represented as Bitmask field.
 *              \ref ga_bap_constants
 *
 *  \param [in] supp_audio_contexts Supported Audio contexts for this role
 *              Represented as Bitmask field.
 *              \ref ga_bap_constants
 *
 *  \param [in] locations Locations for this role
 *              Represented as Bitmask field.
 *              \ref ga_bap_constants
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_register_audio_contexts_and_locations(role, aac, sac, loc) \
        BAP_register_audio_contexts_and_locations((role), (aac), (sac), (loc))

/** \} */

/**
 * \name GA Unicast Server-BAP Procedures
 * \{
 * \brief This section describes the Generic Audio BAP Procedures
 * APIs for Unicast Server.
 */

/**
 *  \brief Register GA Streaming Endpoint.
 *
 *  \par Description
 *  This routine registers a GA streaming endpoint for a given SEP configuration
 *  and QOS capability.
 *
 *  \param [in] sep_conf Role and Codec configuration for the SEP
 *  \param [in] qos_cap Preferred QOS capability for the SEP
 *  \param [out] ase_id Endpoint identifier for future reference on successful registration
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_register_audio_sep
          (
              /* IN */ UINT8 role,
              /* IN */ GA_CODEC_INFO * codec,
              /* IN */ GA_QOS_CAP * qos_cap,
              /* OUT */ UINT8 * ase_id
          );

/**
 *  \brief Update QOS Capabilities of GA Streaming Endpoint.
 *
 *  \par Description
 *  This routine updates the QOS capabilities registered with
 *  a GA streaming endpoint.
 *
 *  \param [in] ase_id Endpoint identifier
 *  \param [in] qos_cap Preferred QOS capability for the SEP
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_update_audio_sep_qos_capabilities
          (
              /* IN */ UINT8 ase_id,
              /* IN */ GA_QOS_CAP * qos_cap
          );

/**
 *  \brief Send Auto configure for a given set of ASEs of a device.
 *
 *  \par Description
 *  This routine transitins the ASE state to Codec Configured state.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [in] ase_count Number of Endpoints included
 *  \param [in] ase_index List of ASE IDs
 *  \param [in] codec_info Codec Info of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_notify_ase_configure
          (
              /* IN */ GA_ENDPOINT* device,
              /* IN */ UINT8 ase_count,
              /* IN */ UINT8* ase_index,
              /* IN */ GA_CODEC_INFO * codec_info
          );

/**
 *  \brief Send Auto ASE Receiver Ready when local Server is a Sink ASE.
 *
 *  \par Description
 *  This routine is to be called once the CIS is established with the remote
 *  Src ASE and when the Local Sink is ready for receiving Audio data.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [in] ase_count Number of Endpoints included
 *  \param [in] ase_index List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_notify_ase_receiver_start_ready
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8 ase_count,
              /* IN */ UINT8* ase_index
          );

/**
 *  \brief Suspend a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine disables the given set of one or more ASE IDs of
 *  the GA Endpoint.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [in] ase_count Number of Endpoints included
 *  \param [in] ase_index List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_notify_ase_suspend
          (
              /* IN */ GA_ENDPOINT* device,
              /* IN */ UINT8 ase_count,
              /* IN */ UINT8* ase_index
          );

/**
 *  \brief Update a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine updates the given set of one or more ASE IDs of
 *  the GA Endpoint with new set of Content Control Information.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [in] ase_count Number of Endpoints included
 *  \param [in] ase_index List of ASE IDs
 *  \param [in] ase_content List of Content Control information for
 *  every endpoint included
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_notify_ase_update
          (
              /* IN */ GA_ENDPOINT* device,
              /* IN */ UINT8 ase_count,
              /* IN */ UINT8* ase_index,
              /* IN */ GA_CONTENT_INFO* ase_content
          );

/**
 *  \brief Initiate release of a given set of ASEs to a GA endpoint.
 *
 *  \par Description
 *  This routine indicates the release on the given set of one or
 *  more ASE IDs to the GA Endpoint.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [in] ase_count Number of Endpoints included
 *  \param [in] ase_index List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_notify_ase_release
          (
              /* IN */ GA_ENDPOINT* device,
              /* IN */ UINT8 ase_count,
              /* IN */ UINT8* ase_index
          );

/**
 *  \brief Indicate release completion of a given set of ASEs
 *  to a GA endpoint.
 *
 *  \par Description
 *  This routine indicates the release completion on the given set
 *  of one or more ASE IDs to the GA Endpoint.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [in] ase_count Number of Endpoints included
 *  \param [in] ase_index List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_notify_release_complete
          (
              /* IN */ GA_ENDPOINT* device,
              /* IN */ UINT8 ase_count,
              /* IN */ UINT8* ase_index
          );

/**
 *  \brief Update and/or Notify Audio Capabilities for Source when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Source Audio Capabilities.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] role \ref GA_CHAR_PACS_SOURCE_PAC or \ref GA_CHAR_PACS_SINK_PAC
 *  \param [in] record_id ID of the registered Source PAC record
 *  \param [in] cap Address of \ref GA_AUDIO_CAP holding the audio capabilities.
 *                   NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
GA_RESULT GA_update_audio_capability
          (
              GA_ENDPOINT  * device,
              UINT16         role,
              UINT8          record_id,
              GA_AUDIO_CAP * pcap
          );

/**
 *  \brief Update and/or Notify Supported Audio Contexts for Source when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Source Supported Audio Contexts.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] audio_contexts Address of UINT16 type holding Supported Audio Contexts.
 *                             NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_source_supported_audio_contexts(device, audio_contexts) \
        BAP_update_and_notify_source_capabilities \
        ((device), GA_CHAR_PACS_SUPPORTED_AUDIO_CONTEXTS, 0U, (audio_contexts), sizeof(UINT16))

/**
 *  \brief Update and/or Notify Available Audio Contexts for Source when changed.
 *
 *  \par Description
 *  This routine updates and/or Notifies the Source Available Audio Contexts.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] audio_contexts Address of UINT16 type holding Available Audio Contexts.
 *                             NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_source_available_audio_contexts(device, audio_contexts) \
        BAP_update_and_notify_source_capabilities \
        ((device), GA_CHAR_PACS_AVAILABLE_AUDIO_CONTEXTS, 0U, (audio_contexts), sizeof(UINT16))

/**
 *  \brief Update and/or Notify Audio locations for Source when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Source Audio locations.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] audio_locations Address of UINT32 type holding Audio Locations.
 *                             NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_source_audio_locations(device, audio_locations) \
        BAP_update_and_notify_source_capabilities \
        ((device), GA_CHAR_PACS_SOURCE_AUDIO_LOCATIONS, 0U, (audio_locations), sizeof(UINT32))

/**
 *  \brief Update and/or Notify Audio Capabilities for Source when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Source Audio Capabilities.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] record_id ID of the registered Source PAC record
 *  \param [in] cap Address of \ref GA_AUDIO_CAP holding the audio capabilities.
 *                   NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_source_audio_capability(device, record_id, cap) \
        GA_update_audio_capability(device, GA_CHAR_PACS_SOURCE_PAC, record_id, cap)

/**
 *  \brief Update and/or Notify Supported Audio Contexts for Sink when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Sink Supported Audio Contexts.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] audio_contexts Address of UINT16 type holding Supported Audio Contexts.
 *                             NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_sink_supported_audio_contexts(device, audio_contexts) \
        BAP_update_and_notify_sink_capabilities \
        ((device), GA_CHAR_PACS_SUPPORTED_AUDIO_CONTEXTS, 0U, (audio_contexts), sizeof(UINT16))

/**
 *  \brief Update and/or Notify Available Audio Contexts for Sink when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Sink Available Audio Contexts.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] audio_contexts Address of UINT16 type holding Available Audio Contexts.
 *                             NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_sink_available_audio_contexts(device, audio_contexts) \
        BAP_update_and_notify_sink_capabilities \
        ((device), GA_CHAR_PACS_AVAILABLE_AUDIO_CONTEXTS, 0U, (audio_contexts), sizeof(UINT16))

/**
 *  \brief Update and/or Notify Audio locations for Sink when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Sink Audio locations.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] audio_locations Address of UINT32 type holding Audio Locations.
 *                             NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_sink_audio_locations(device, audio_locations) \
        BAP_update_and_notify_sink_capabilities \
        ((device), GA_CHAR_PACS_SINK_AUDIO_LOCATIONS, 0U, (audio_locations), sizeof(UINT32))

 /**
 *  \brief Update and/or Notify Audio Capabilities for Sink when changed.
 *
 *  \par Description
 *  This routine updates and/or notifies the Sink Audio Capabilities.
 *
 *  \param [in] device Address of \ref GA_ENDPOINT type holding peer device.
 *                     NULL here updates the value locally
 *  \param [in] record_id ID of the registered Sink PAC record
 *  \param [in] cap Address of \ref GA_AUDIO_CAP holding the audio capabilities.
 *                   NULL here notifies the existing value to peer
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Both parameters to the API cannot be of NULL value
 */
#define GA_update_sink_audio_capability(device, record_id, cap) \
        GA_update_audio_capability(device, GA_CHAR_PACS_SINK_PAC, record_id, cap)

/**
 *  \brief Register a GA content control service.
 *
 *  \par Description
 *  This routine registers a supported content control service to the GA layer and
 *  obtains a unique identifier CCID.
 *
 *  \param [in] ccs_uuid UUID of the Content Control Service registered
 *  \param [out] ccid Content Control Identifier allocated for the service
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_register_content_service
          (
              /* IN */ UINT16 ccs_uuid,
              /* OUT */ UINT8 * ccid
          );

/**
 *  \brief Notify SEP state updates to an Endpoint.
 *
 *  \par Description
 *  This routine notifies updates in all SEP states to the endpoint specified.
 *  To notify to all connected endpoints, the parameter is given NULL.
 *
 *  \param [in] device Endpoint to be notified. NULL if all connected endpoints
 *  to be notified.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_sep_notify_updates
          (
              /* IN */ GA_ENDPOINT * device
          );

/** \} */
/** \} */

/**
 * \defgroup ga_client_defs GA Client APIs
 * \{
 * \brief This section describes the Generic Audio APIs
 * for Client.
 */

/**
 * \name GA Client-Setup
 * \{
 * \brief This section describes the Generic Audio Setup
 * APIs for Client.
 */

/**
 *  \brief Initialize the GA Unicast Client.
 *
 *  \par Description
 *  This routine initializes GA Unicast Client
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_uc_client_init(cb)       GA_module_init(GA_MODULE_BAP, BAP_ROLE_CLIENT, (cb))

/**
 *  \brief Shutdown the GA Unicast Client.
 *
 *  \par Description
 *  This routine shuts down GA Unicast Client
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_uc_client_shutdown()   GA_module_shutdown(GA_MODULE_BAP, BAP_ROLE_CLIENT)

/**
 *  \brief Setup a GA context for a peer endpoint.
 *
 *  \par Description
 *  This routine allocates a GA context for a peer endpoint, discovers the
 *  audio roles supported on the peer endpoint.
 *  The PACS Service, Char, Desc discovery is done and configuration of
 *  notification on the remote is performed.
 *
 *  \param [in] device Peer Endpoint address
 *  \param [out] context Allocated GA context for the peer endpoint
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SETUP_CNF event.
 */
GA_RESULT GA_setup
          (
              /* IN */ GA_ENDPOINT * device,
              /* OUT */ GA_CONTEXT * context
          );

/**
 * \cond EXCLUDE_PART
 */
#ifdef BAP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To enable configuration for Characteristic Notifications.
 *
 *  \par Description
 *       The routine enables to configure notifications for individual characteristics
 *       of BAP related service.
 *
 *  \param [in] config
 *              Bitmask for characteristics IDs.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
#define GA_update_ntf_configuration(config) \
        BAP_update_ntf_configuration((config))
#endif /* BAP_SUPPORT_CONFIG_SELECTION */

#ifdef BAP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the GA BAP characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to get/set the service and
 *       characteristic handle ranges from/to a device context.
 *
 *  \param [in] set
 *              GA_TRUE for Context SET and GA_FALSE for Context GET
 *  \param [in] device
 *              Remote device endpoint
 *  \param [in] handle
 *              Context handle for the endpoint
 *  \param [in] range
 *              Service range array for PACS and ASCS
 *  \param [in] info
 *              Array of characteristic handles for PACS and ASCS
 *  \param [in] info_count
 *              Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
GA_RESULT GA_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ BAP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To set the GA BAP characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to set the service and
 *       characteristic handle ranges to a device context.
 *
 *  \param [in] d
 *              Remote device endpoint
 *  \param [in] h
 *              Context handle for the endpoint
 *  \param [in] r
 *              Service range array for PACS and ASCS
 *  \param [in] i
 *              Array of characteristic handles for PACS and ASCS
 *  \param [in] c
 *              Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
#define GA_set_context_info(d, h, r, i, c) \
        GA_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))

/**
 *  \brief To get the GA BAP characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to get the service and
 *       characteristic handle ranges from a device context.
 *
 *  \param [in] d
 *              Remote device endpoint
 *  \param [in] h
 *              Context handle for the endpoint
 *  \param [in] r
 *              Service range array for PACS and ASCS
 *  \param [in] i
 *              Array of characteristic handles for PACS and ASCS
 *  \param [in] c
 *              Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
#define GA_get_context_info(d, h, r, i, c) \
        GA_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

#endif /* BAP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/**
 *  \brief Release or free the given GA context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling GA_setup() if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] context Context to be released/freed
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_terminate
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UCHAR free
          );

/**
 *  \brief Release the given GA context.
 *
 *  \par Description
 *  This routine initiates the release procedure for the context. Once release
 *  is done, the context is freed up and the setup must be freshly done by
 *  calling GA_setup() if required for the same device again.
 *
 *  \param [in] context Context to be released
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 */
#define GA_release(ctx) \
        GA_terminate((ctx), GA_FALSE)

/**
 *  \brief Free the given GA context.
 *
 *  \par Description
 *  This routine frees up the given context of the GA layer.
 *
 *  \param [in] context Context to be freed
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_close(ctx) \
        GA_terminate((ctx), GA_TRUE)

/**
 *  \brief Get the capabilities of a GA endpoint.
 *
 *  \par Description
 *  This routine gets the capabilities for a GA endpoint. Internally based on
 *  the role specified, the values of the PACS Chars is done by read operation.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] role Role on the endpoint for which the capabilities is required
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_GET_CAPABILITIES_CNF event
 *        multiple times with \ref GA_CONTINUE and finally with
 *        \ref GA_SUCCESS.
 *        The role provided as part of \ref GA_SETUP_CNF shall be used here.
 */
GA_RESULT GA_get_capabilities
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 role
          );

/** \} */

/**
 * \name GA Unicast Client-BAP Procedures
 * \{
 * \brief This section describes the Generic Audio BAP Procedures
 * APIs for Unicast Client.
 */

/**
 *  \brief Discover the Audio Stream Endpoints (ASE) of a GA endpoint.
 *
 *  \par Description
 *  This routine discovers the available audio stream endpoints on a GA endpoint
 *  that is setup previously. Internally ASCS Service, Char, Desc is done
 *  post which the notification is enabled for the applicable ASCS
 *  Char. Read on the ASE Source and ASE Sink is done to fetch the current
 *  state of the ASE. The event \ref GA_SEP_DISCOVER_CNF is notified based on
 *  the number of ASCS Source and/or Sink Char are discovered.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_DISCOVER_CNF event
 *        multiple times with \ref GA_CONTINUE and finally with
 *        \ref GA_SUCCESS.
 */
GA_RESULT GA_sep_discover
          (
              /* IN */ GA_CONTEXT * context
          );

/**
 *  \brief Configure a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine configures the given set of one or more ASE IDs of the
 *  GA Endpoint with the requested Codec Configuration.
 *  This call shall only submit codec configuration parameters that are
 *  supported by the peer received via \ref GA_GET_CAPABILITIES_CNF.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_CONFIGURE_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *  \param [in] sep_role List of roles for every endpoint included
 *  \param [in] sep_codec List of Codec Configurations for every endpoint included
 *  \param [in] sep_phy List of Phy for the ASE IDs.
 *  \param [in] sep_latency List of Latency for the ASE IDs.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_CONFIGURE_CNF event
 */
GA_RESULT GA_sep_configure
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 sep_count,
              /* IN */ UINT8 * sep_id,
              /* IN */ UINT8 * sep_role,
              /* IN */ GA_CODEC_INFO * sep_codec,
              /* IN */ UINT8 * sep_phy,
              /* IN */ UINT8 * sep_latency
          );

/**
 *  \brief Setup a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine sets up the QOS configuration on the given set of one or
 *  more ASE IDs of the GA Endpoint.
 *  This call shall submit the parameters provided by the peer in the event
 *  \ref GA_SEP_CONFIGURE_CNF.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_SETUP_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *  \param [in] sep_qos List of QOS Configurations for every endpoint included
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_SETUP_CNF event
 */
GA_RESULT GA_sep_setup
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 sep_count,
              /* IN */ UINT8 * sep_id,
              /* IN */ GA_QOS_CONF * sep_qos
          );

/**
 *  \brief Start a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine enables the given set of one or more ASE IDs of
 *  the GA Endpoint.
 *  This call shall request the Metadata from peer for this ASE.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_START_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *  \param [in] sep_content List of Content Control information for
 *  every endpoint included
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_START_CNF event
 */
GA_RESULT GA_sep_start
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 sep_count,
              /* IN */ UINT8 * sep_id,
              /* IN */ GA_CONTENT_INFO * sep_content
          );

/**
 *  \brief Receiver Start Ready to receive Audio data for a given set of ASEs
 *         of a GA endpoint.
 *
 *  \par Description
 *  This routine indicates the given set of one or more ASE IDs of
 *  the GA Endpoint is ready to receive audio data and completes coupling an
 *  ASE to a CIS.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_RX_START_READY_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_RX_START_READY_CNF event
 */
GA_RESULT GA_sep_receiver_start_ready
          (
              GA_CONTEXT * context,
              UINT8 sep_count,
              UINT8 * sep_id
          );

/**
 *  \brief Receiver Stop Ready to receive Audio data for a given set of ASEs
 *         of a GA endpoint.
 *
 *  \par Description
 *  This routine indicates the given set of one or more ASE IDs of
 *  the GA Endpoint is ready to stop receiving audio data and completes
 *  decoupling an ASE to a CIS.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_RX_STOP_READY_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_RX_STOP_READY_CNF event
 */
GA_RESULT GA_sep_receiver_stop_ready
          (
              GA_CONTEXT * context,
              UINT8 sep_count,
              UINT8 * sep_id
          );

/**
 *  \brief Update a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine updates the given set of one or more ASE IDs of
 *  the GA Endpoint with new set of Content Control Information.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *  \param [in] sep_content List of Content Control information for
 *  every endpoint included
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_UPDATE_CNF event
 */
GA_RESULT GA_sep_update
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 sep_count,
              /* IN */ UINT8 * sep_id,
              /* IN */ GA_CONTENT_INFO * sep_content
          );

/**
 *  \brief Suspend a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine disables the given set of one or more ASE IDs of
 *  the GA Endpoint. This call starts decoupling a Source/Sink ASE from a CIS
 *  for one or more Source ASE/Sink ASEs.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_SUSPEND_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_SUSPEND_CNF event
 */
GA_RESULT GA_sep_suspend
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 sep_count,
              /* IN */ UINT8 * sep_id
          );

/**
 *  \brief Release a given set of ASEs of a GA endpoint.
 *
 *  \par Description
 *  This routine releases the given set of one or more ASE IDs of
 *  the GA Endpoint. This call releases resources associated with an ASE,
 *  immediately decouples the ASE from any previously coupled CIS, and tears
 *  down any CIS previously established for the ASE for one or more ASEs.
 *  Internally, an ASE CP is performed and post receiving of the response
 *  and the notification of the ASE Sink/ASE Source, an event
 *  \ref GA_SEP_RELEASE_CNF is generated.
 *
 *  \param [in] context Context of the peer endpoint that is setup
 *  \param [in] sep_count Number of Endpoints included
 *  \param [in] sep_id List of ASE IDs
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_SEP_RELEASE_CNF event
 */
GA_RESULT GA_sep_release
          (
              /* IN */ GA_CONTEXT * context,
              /* IN */ UINT8 sep_count,
              /* IN */ UINT8 * sep_id
          );

/** \} */
/** \} */

/**
 * \defgroup ga_bc_src_api_defs GA Broadcast Source APIs
 * \{
 * \brief This section describes the Generic Audio APIs
 * for Broadcast Source.
 */

/**
 *  \brief Initialize the GA Broadcast Source.
 *
 *  \par Description
 *  This routine initializes GA Broadcast Source
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_bc_source_init(cb)       GA_module_init(GA_MODULE_BAP, BAP_ROLE_BCSRC, (cb))

/**
 *  \brief Shutdown the GA Broadcast Source
 *
 *  \par Description
 *  This routine shuts down GA Broadcast Source
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_bc_source_shutdown()   GA_module_shutdown(GA_MODULE_BAP, BAP_ROLE_BCSRC)

/**
 *  \brief Allocate a Broadcast Source session.
 *
 *  \par Description
 *  This routine allocates a Broadcast Source session at the GA and provides
 *  a session identifier on successful allocation.
 *
 *  \param [out] ssn Session Identifier for Broadcast Source
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_alloc_session(ssn) \
        BAP_broadcast_alloc_session((ssn))

/**
 *  \brief Free the Broadcast Source session.
 *
 *  \par Description
 *  This routine frees the Broadcast Source session at the GA identified
 *  by the Session Identifier.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_free_session(ssn) \
        BAP_broadcast_free_session((ssn))

/**
 *  \brief Configure a Broadcast Source session.
 *
 *  \par Description
 *  This routine configures a Broadcast Source session identified by
 *  the session identifier with some of the broadcast parameters.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *  \param [in] phy The Physical channel to be used for the Broadcast
 *  of streams on this session. This can take any of the valid values
 *  from \ref ga_pref_phy
 *  \param [in] packing Recommendation for a preferred arrangement of
 *  multiple BIS subevents. 0 - Sequential, 1 - Interleaved
 *  \param [in] broadcast_code Address to 16 octet broadcast code if
 *  encryption is to be enabled, else NULL
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_configure_session(ssn, phy, packing, broadcast_code) \
        BAP_broadcast_configure_session((ssn), (phy), (packing), (broadcast_code))

/**
 *  \brief Register a Source Endpoint.
 *
 *  \par Description
 *  This routine registers a source endpoint with a given configuration
 *  on to a Broadcast session.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *  \param [in] codec Codec Information for the endpoint
 *  \param [in] meta Metadata Information for the endpoint
 *  \param [in] nstream Number of media streams at this endpoint
 *  \param [in] stream Array of stream specific codec information if any
 *  \param [out] sep Endpoint ID allocated
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_register_sep(ssn, codec, meta, nstream, stream, sep) \
        BAP_broadcast_register_sep((ssn), (codec), (meta), (nstream), (stream), (sep))

/**
 *  \brief Register an Advertising Set Handle to a Broadcast Source session.
 *
 *  \par Description
 *  This routine registers an advertising set handle to be used by
 *  the broadcast session
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *  \param [in] advset The advertising handle to be used during EA
 *  and PA broadcast
 *
 *  \note Application should ensure that a valid handle is registered
 *  before trying to setup announcements
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_register_advset(ssn, advset) \
        BAP_broadcast_register_advset_info((ssn), (advset), NULL, 0U, NULL, 0U)

/**
 *  \brief Register an Advertising Set Handle to a Broadcast Source session.
 *
 *  \par Description
 *  This routine registers an advertising set handle to be used by
 *  the broadcast session
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *  \param [in] advset The advertising handle to be used during EA
 *  and PA broadcast
 *  \param [in] sd Service Data of the application profile
 *  \param [in] sdlen Length of Service Data
 *  \param [in] sd Additional Adv Data of the application profile
 *  \param [in] sdlen Length of Additional Adv Data
 *
 *  \note Application should ensure that a valid handle is registered
 *  before trying to setup announcements
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_register_advset_info(ssn, advset, sd, sdlen, ad, adlen) \
        BAP_broadcast_register_advset_info((ssn), (advset), (sd), (sdlen), (ad), (adlen))

/**
 *  \brief Unregister an Advertising Set Handle from a Broadcast Source session.
 *
 *  \par Description
 *  This routine unregisters the Advertising Set handle from a session
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_unregister_advset(ssn) \
        BAP_broadcast_register_advset_info((ssn), BAP_BC_INVALID_ADVSET_HANDLE, NULL, 0U, NULL, 0U)

/**
 *  \brief Setup Audio Announcement for a Broadcast Source session
 *
 *  \par Description
 *  This routine initiates the audio announcement on a given session with
 *  the given configurations. This procedure moves the broadcast session
 *  into CONFIGURED state.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *  \param [in] broadcast_id 3 octet broadcast id to identify the announcement
 *  \param [in] type SQ or HQ specifier. Currently unused.
 *  \param [in] presentation_delay The presentation delay information for the
 *  session to the Broadcast Sinks
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_SETUP_ANNOUNCEMENT_CNF event.
 */
#define GA_broadcast_setup_announcement(ssn, broadcast_id, type, presentation_delay) \
        BAP_broadcast_setup_announcement((ssn), (broadcast_id), (type), (presentation_delay))

/**
 *  \brief End Audio Announcement for a Broadcast Source session
 *
 *  \par Description
 *  This routine terminates an ongoing announcement in a Broadcast Source
 *  session. This procedure moves the broadcast session into IDLE state.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_END_ANNOUNCEMENT_CNF event.
 */
#define GA_broadcast_end_announcement(ssn) \
        BAP_broadcast_end_announcement((ssn))

/**
 *  \brief Start the Isochronous Stream on Broadcast Source session.
 *
 *  \par Description
 *  This routine sets up the Broadcast Isochronous stream with the given
 *  configuration on the session where the announcements are started.
 *  This procedure moves the broadcast session into STREAMING state.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *  \param [in] sdu_interval SDU frame interval in uSec
 *  \param [in] max_sdu Maximum SDU size requirement for application
 *  \param [in] max_latency Maximum latency requirement for application
 *  \param [in] rtn SDU retransmission recommendation
 *  \param [in] framing SDU framing requirement for application
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_START_CNF event.
 */
#define GA_broadcast_start(ssn, sdu_interval, max_sdu, max_latency, rtn, framing) \
        BAP_broadcast_start((ssn), (sdu_interval), (max_sdu), (max_latency), (rtn), (framing))

/**
 *  \brief Suspend the Isochronous stream on a Broadcast Source session.
 *
 *  \par Description
 *  This routine suspends an active Isochronous stream on a Broadcast
 *  Source session identified by  the session identifier. This procedure
 *  moves the broadcast session into CONFIGURED state.
 *
 *  \param [in] ssn Session Identifier for Broadcast Source
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_SUSPEND_CNF event.
 */
#define GA_broadcast_suspend(ssn) \
        BAP_broadcast_suspend((ssn))

/** \} */

/**
 * \defgroup ga_bc_snk_api_defs GA Broadcast Sink APIs
 * \{
 * \brief This section describes the Generic Audio APIs
 * for Broadcast Sink.
 */

/**
 *  \brief Initialize the GA Broadcast Sink.
 *
 *  \par Description
 *  This routine initializes GA Broadcast Sink
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_bc_sink_init(cb)       GA_module_init(GA_MODULE_BAP, BAP_ROLE_BCSNK, (cb))

/**
 *  \brief Shutdown the GA Broadcast Sink.
 *
 *  \par Description
 *  This routine shuts down GA Broadcast Sink
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_bc_sink_shutdown()   GA_module_shutdown(GA_MODULE_BAP, BAP_ROLE_BCSNK)

/**
 *  \brief Scan for Broadcast Audio announcements.
 *
 *  \par Description
 *  This routine is used by a Broadcast Sink to start scanning for
 *  Broadcast Audio announcements
 *
 *  \param None
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_SCAN_ANNOUNCEMENT_CNF event.
 *  \sa GA_BC_SOURCE_ANNOUNCEMENT_IND
 */
#define GA_broadcast_scan_announcement() \
        BAP_broadcast_scan_announcement()

/**
 *  \brief Stop scanning for Announcements.
 *
 *  \par Description
 *  This routine is used by Broadcast Sink to stop scanning for
 *  Broadcast Audio Announcements
 *
 *  \param None
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_SCAN_END_CNF event.
 */
#define GA_broadcast_scan_end() \
        BAP_broadcast_scan_end()

/**
 *  \brief Associate to a Broadcast Audio announcement.
 *
 *  \par Description
 *  This routine sets up an periodic sync of the Broadcast sink device
 *  with the selected source of Broadcast Audio announcement.
 *
 *  \param [in] sid The advertising Source ID as received in the announcement
 *  \param [in] device The source endpoint device address
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_ASSOCIATE_CNF event.
 *  \sa GA_BC_SOURCE_CONFIG_IND
 */
#define GA_broadcast_associate(sid, device) \
        BAP_broadcast_associate((sid), (device))

/**
 *  \brief Update Associate Info of a Broadcast Audio announcement.
 *
 *  \par Description
 *  This routine updates the Associate Info.
 *
 *  \param [in] sid The advertising Source ID as received in the announcement
 *  \param [in] device The source endpoint device address
 *  \param [in] sync_handle SyncHandle received as part of PAST or by any other
 *              accepted method.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_update_associate_info(sid, device, sync_handle) \
        BAP_update_associate_info((sid), (device), (sync_handle))

/**
 *  \brief Dissociate from a Broadcast Audio Announcement.
 *
 *  \par Description
 *  This routine terminates the periodic sync on the Broadcast sink
 *  with the source of the announcement.
 *
 *  \param [in] sync_handle The SyncHandle as received in a successful
 *  GA_BC_ASSOCIATE_CNF event.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_DISSOCIATE_CNF event.
 */
#define GA_broadcast_dissociate(sync_handle) \
        BAP_broadcast_dissociate((sync_handle))

/**
 *  \brief Enable streaming mode at Broadcast Sink.
 *
 *  \par Description
 *  This routine establishes a sync with the Broadcast stream of the
 *  audio announcement source with the given configuration. This
 *  procedure puts the Broadcast Sink in STREAMING state.
 *
 *  \param [in] sync_handle The SyncHandle as received in a successful
 *  GA_BC_ASSOCIATE_CNF event
 *  \param [in] broadcast_code Address to 16 octet broadcast code if
 *  encryption is enabled at source, else NULL
 *  \param [in] bis_count Number of streams to be synchronized
 *  \param [in] bis Array of bis_count number of stream ID to be synchronized
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_ENABLE_CNF event.
 *  \sa GA_BC_SOURCE_STREAM_IND
 */
#define GA_broadcast_enable(sync_handle, broadcast_code, bis_count, bis) \
        BAP_broadcast_enable((sync_handle), (broadcast_code), (bis_count), (bis))

/**
 *  \brief Disable streaming mode at Broadcast Sink.
 *
 *  \par Description
 *  This routine terminates sync with the Broadcast stream of the
 *  audio announcement source. This procedure puts the Broadcast Sink.
 *
 *  \param [in] sync_handle The SyncHandle as received in a successful
 *  GA_BC_ASSOCIATE_CNF event
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure triggers the \ref GA_BC_DISABLE_CNF event.
 */
#define GA_broadcast_disable(sync_handle) \
        BAP_broadcast_disable((sync_handle))

/**
 *  \brief Utility to decode broadcast event parameters.
 *
 *  \par Description
 *  This routine helps with decoding of the Broadcast event parameters
 *  for each of the event type received at the application
 *
 *  \param [in] ev_type Broadcast Event Type
 *  \param [in] ev_data Broadcast Event Data
 *  \param [out] param Address to one of Broadcast event param structures
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *  \sa GA_BC_SOURCE_ANNOUNCEMENT_IND
 *  \sa GA_BC_ASSOCIATE_CNF
 *  \sa GA_BC_SOURCE_CONFIG_IND
 *  \sa GA_BC_DISSOCIATE_IND
 *  \sa GA_BC_DISSOCIATE_CNF
 *  \sa GA_BC_ENABLE_CNF
 *  \sa GA_BC_SOURCE_STREAM_IND
 *  \sa GA_BC_DISABLE_IND
 *  \sa GA_BC_DISABLE_CNF
 */
#define GA_broadcast_decode_event_params(ev_type, ev_data, param) \
        BAP_broadcast_decode_event_params((ev_type), (ev_data), (param))

/**
 *  \brief Utility to decode Broadcast Audio Announcement
 *
 *  \par Description
 *  This routine helps with decoding of the Broadcast Audio
 *  Announcements received at the application.
 *
 *  \param [in] data
 *  \param [out] param
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_decode_broadcast_announcement(data, data_len, param) \
        BAP_broadcast_decode_broadcast_announcement((data), (data_len), (param))

/**
 *  \brief Utility to decode Basic Audio Announcement
 *
 *  \par Description
 *  This routine helps with decoding of the Basic Audio
 *  Announcements received at the application.
 *
 *  \param [in] data
 *  \param [out] param
 *  \param [out] nsg
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_broadcast_decode_basic_announcement_info(data, delay, nsg, len) \
        BAP_broadcast_decode_basic_announcement_info((data), (delay), (nsg), (len))

#define GA_broadcast_decode_basic_announcement_level_1(data, param, nbis, len) \
        BAP_broadcast_decode_basic_announcement_level_1((data), (param), (nbis), (len))

#define GA_broadcast_decode_basic_announcement_level_2(data, param, len) \
        BAP_broadcast_decode_basic_announcement_level_2((data), (param), (len))

/** \} */

/** \} */
/** \} */
/* ================ VCP Related ================ */

/**
 * \addtogroup ga_vcp_module
 * \{
 */

/**
 * \defgroup ga_vcp_api_sequences API Sequences
 * \{
 * \brief This section describes the EtherMind GA VCP Layer API Sequences.
 */

/**
 * \defgroup ga_vcp_api_seq VCP API Sequences
 * \{
 * \brief This section describes the Volume Control Profile API Sequences.
 * MSC depicting the flow of APIs and Events.
 * \mscfile VCP.signalling
 */

/** \} */
/** \} */

/**
 * \defgroup ga_vcp_api_defs API Definitions
 * \{
 * \brief This section describes the Volume Control Profile APIs.
 */

/**
 * \defgroup ga_vcp_volcntrl_defs VCP Volume Controller APIs
 * \{
 * \brief This section describes the Volume Control Profile APIs
 * for Volume Controller.
 */

/**
 * \name VCP Volume Controller-Setup
 * \{
 * \brief This section describes the Volume Control Profile Setup
 * APIs for Volume Controller.
 */

/**
 *  \brief Initialize the GA Volume Control Client.
 *
 *  \par Description
 *  This routine initializes GA Volume Control Client
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_vc_client_init(cb)       GA_module_init(GA_MODULE_VCP, VCP_ROLE_CLIENT, (cb))

/**
 *  \brief Shutdown the GA Volume Control Client
 *
 *  \par Description
 *  This routine shuts down GA Volume Control Client
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_vc_client_shutdown()   GA_module_shutdown(GA_MODULE_VCP, VCP_ROLE_CLIENT)

/**
 *  \brief To Setup Volume Control Service(VCS) Context
 *
 *  \par Description
 *       This function enables to setup VCS context with given device.
 *       Internally, Service, Characterstics and Descriptors Discovery is
 *       done and Notifications are enabled. Application/Upper Layer is
 *       expected to use the allocated 'vctx' for all further
 *       transactions on VCP with peer device.
 *
 *  \param [in] device
 *              Peer Device with which the context is setup.
 *
 *  \param [out] vctx
 *               Context Allocated for GA VCP with the peer device.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_VC_SETUP_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa GA_VC_SETUP_CNF
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_setup
          (
              /* IN */  GA_ENDPOINT    * device,
              /* OUT */ GA_VC_CONTEXT  * vctx
          );

/**
 * \cond EXCLUDE_PART
 */
#ifdef VCP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To enable configuration for Characteristic Notifications.
 *
 *  \par Description
 *       The routine enables to configure notifications for individual characteristics
 *       of a given service.
 *
 *  \param [in] s_type
 *               - \ref VCP_SERVICE_TYPE_VCS
 *               - \ref VCP_SERVICE_TYPE_VOCS
 *               - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [in] config
 *              Bitmask for characteristics IDs.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
#define GA_vc_update_ntf_configuration(s_type, config) \
        VCP_update_ntf_configuration((s_type), (config))
#endif /* VCP_SUPPORT_CONFIG_SELECTION */

#ifdef VCP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the VCP VCS Context Information.
 *
 *  \par Description:
 *       This function enables to manage the context information of VCS.
 *
 *  \param [in] set
 *              GA_TRUE for Context SET and GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] handle
 *         VCP Handle which is allocated during VCS context setup.
 *
 *  \param [inout] range
 *         VCS characteristics handle range.
 *
 *  \param [inout] info
 *          VCS characteristics information.
 *
 *  \param [inout] info_count
 *          VCS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_vc_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ GA_VC_CONTEXT    * vctx,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To retrieve the VCS Context Information.
 *
 *  \par Description:
 *       This function enables to retrieve the context information of VCS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         VCP Handle which is allocated during VCS context setup.
 *
 *  \param [inout] r
 *         VCS characteristics handle range.
 *
 *  \param [inout] i
 *          VCS characteristics information.
 *
 *  \param [inout] c
 *          VCS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_vc_get_context_info(d, h, r, i, c) \
        GA_vc_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To save the VCS Context Information.
 *
 *  \par Description:
 *       This function enables to save the context information of VCS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         VCP Handle which is allocated during VCS context setup.
 *
 *  \param [inout] r
 *         VCS characteristics handle range.
 *
 *  \param [inout] i
 *          VCS characteristics information.
 *
 *  \param [inout] c
 *          VCS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_vc_set_context_info(d, h, r, i, c) \
        GA_vc_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))

/**
 *  \brief To manage the VOCS Context Information.
 *
 *  \par Description:
 *       This function enables to manage the context information of VOCS.
 *
 *  \param [in] set
 *         GA_TRUE for Context SET and GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] vocs_ctx
 *         VOCS Handle which is allocated during VOCS context setup.
 *
 *  \param [inout] range
 *         VOCS characteristics handle range.
 *
 *  \param [inout] info
 *         VOCS characteristics information.
 *
 *  \param [inout] info_coun
 *         VOCS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_vc_manage_vocs_context_info
          (
              /* IN     */ UINT8                 set,
              /* IN     */ GA_ENDPOINT        *  device,
              /* INOUT  */ GA_VC_CONTEXT      *  vocs_ctx,
              /* INOUT  */ GA_BRR_SVC_RANGE   ** range,
              /* INOUT  */ GA_CHAR_INFO       ** info,
              /* INOUT  */ UINT16             * info_count
          );

/**
 *  \brief To retrieve the VOCS Context Information.
 *
 *  \par Description:
 *       This function enables to retrieve the context information of VOCS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         VOCS Handle which is allocated during VOCS context setup.
 *
 *  \param [inout] r
 *         VOCS characteristics handle range.
 *
 *  \param [inout] i
 *         VOCS characteristics information.
 *
 *  \param [inout] c
 *         VOCS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_vc_get_vocs_context_info(d, h, r, i, c) \
        GA_vc_manage_vocs_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To retrieve the VOCS Context Information.
 *
 *  \par Description:
 *       This function enables to retrieve the context information of VOCS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         VOCS Handle which is allocated during VOCS context setup.
 *
 *  \param [inout] r
 *         VOCS characteristics handle range.
 *
 *  \param [inout] i
 *         VOCS characteristics information.
 *
 *  \param [inout] c
 *         VOCS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_vc_set_vocs_context_info(d, h, r, i, c) \
        GA_vc_manage_vocs_context_info(GA_TRUE, (d), (h), (r), (i), (c))

/**
 *  \brief To manage the AICS Context Information.
 *
 *  \par Description:
 *       This function enables to manage the context information of AICS.
 *
 *  \param [in] set
 *         GA_TRUE for Context SET and GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] vocs_ctx
 *         VOCS Handle which is allocated during AICS context setup.
 *
 *  \param [inout] range
 *         AICS characteristics handle range.
 *
 *  \param [inout] info
 *         AICS characteristics information.
 *
 *  \param [inout] info_coun
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_vc_manage_aics_context_info
          (
              /* IN     */ UINT8                 set,
              /* IN     */ GA_ENDPOINT        *  device,
              /* INOUT  */ GA_VC_CONTEXT      *  aics_ctx,
              /* INOUT  */ GA_BRR_SVC_RANGE   ** range,
              /* INOUT  */ GA_CHAR_INFO       ** info,
              /* INOUT  */ UINT16             * info_count
          );

/**
 *  \brief To retrieve the AICS Context Information.
 *
 *  \par Description:
 *       This function enables to retrieve the context information of AICS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         AICS Handle which is allocated during AICS context setup.
 *
 *  \param [inout] r
 *         AICS characteristics handle range.
 *
 *  \param [inout] i
 *         AICS characteristics information.
 *
 *  \param [inout] c
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_vc_get_aics_context_info(d, h, r, i, c) \
        GA_vc_manage_aics_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To save the AICS Context Information.
 *
 *  \par Description:
 *       This function enables to save the context information of AICS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         AICS Handle which is allocated during AICS context setup.
 *
 *  \param [inout] r
 *         AICS characteristics handle range.
 *
 *  \param [inout] i
 *         AICS characteristics information.
 *
 *  \param [inout] c
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_vc_set_aics_context_info(d, h, r, i, c) \
        GA_vc_manage_aics_context_info(GA_TRUE, (d), (h), (r), (i), (c))
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/**
 *  \brief To Discover the Included Service - VOCS and AICS from the
 *         remote device.
 *
 *  \par Description
 *       This function enables to discover the optional included services -
 *       VOCS and AICS from the remote device.
 *
 *  \param [in] vctx
 *              Context associated with VCP.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note This procedure returns the \ref GA_VC_GET_CAPABILITIES_CNF event
 *        for each optional service with event status \ref GA_CONTINUE.
 *        On completion of this procedure, \ref GA_VC_GET_CAPABILITIES_CNF
 *        event is notified with event status \ref GA_SUCCESS.
 *        Application/Upper Layer is expected to use the information returned
 *        along with \ref GA_VC_GET_CAPABILITIES_CNF event for setting up the
 *        optional capabilities.
 *
 *  \sa GA_VC_GET_CAPABILITIES_CNF
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_get_capabilities
          (
              /* IN */ GA_VC_CONTEXT * vctx
          );

/**
 *  \brief Setup GA VCP optional capabiliy with the peer device.
 *
 *  \par Description
 *       This routine allocates a VCP optional service context with a peer
 *       endpoint, discovers the optional(VOCS, AICS) service charactersicstic
 *       and configure for notification.
 *
 *  \param [in] vcp_cntx
 *              VCP Context
 *
 *  \param [in] svc
 *              Optional Service(VOCS/AICS) Service details to Setup.
 *
 *  \param [out] opt_context
 *              Context Allocated for optional service.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_VC_SET_CAPABILITY_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa GA_VC_SET_CAPABILITY_CNF
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_set_capability
          (
              /* IN */  GA_VC_CONTEXT   * vctx,
              /* IN */  GA_BRR_SVC_INFO * svc,
              /* OUT */ GA_VC_CONTEXT   * cctx
          );

/**
 *  \brief To Release or Free a Volume Control Profile (VCP) context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling GA_vc_setup() if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] context Context Allocated for GA VCP with the peer device.
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_VC_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \note Before releasing the context for VCP, the application is expected to
 *        release context of all the included optional service(VOCS/AICS)
 *        instances.
 *
 *  \sa GA_VC_RELEASE_CNF
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_terminate
          (
              /* IN */ GA_VC_CONTEXT * vctx,
              /* IN */ UCHAR free
          );

/**
 *  \brief To Release a Volume Control Profile (VCP) context.
 *
 *  \par Description
 *       This function enables to release context of given VCP
 *       service handle. It disable the configuration for notifications and
 *       releases the context for VCS.
 *
 *  \param [in] context
 *               Context Allocated for GA VCP with the peer device.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_VC_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \note Before releasing the context for VCP, the application is expected to
 *        release context of all the included optional service(VOCS/AICS)
 *        instances.
 *
 *  \sa GA_VC_RELEASE_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_release(ctx) \
        GA_vc_terminate((ctx), GA_FALSE)

/**
 *  \brief To Free a Volume Control Profile (VCP) context.
 *
 *  \par Description
 *       This function frees the context of given VCP service handle.
 *
 *  \param [in] context
 *               Context Allocated for GA VCP with the peer device.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 *
 *  \note Before freeing the context for VCP, the application is expected to
 *        free context of all the included optional service(VOCS/AICS)
 *        instances.
 */
#define GA_vc_close(ctx) \
        GA_vc_terminate((ctx), GA_TRUE)

/**
 *  \brief To Release/Free a VCP-VOCS or VCP-AICS context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling GA_vc_set_capability() if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] vcp_srvc_type
 *              VCP optional service type
 *               - \ref VCP_SERVICE_TYPE_VOCS
 *               - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [in] context
 *              Context Allocated for GA VCP Capability with the peer device.
 *
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_VC_RELEASE_CAPABILITY_CNF is notified
 *          on completion with status as success or failure.
 *
 *  \sa GA_VC_RELEASE_CAPABILITY_CNF
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_terminate_capability
          (
              /* IN */ UCHAR           vcp_srvc_type,
              /* IN */ GA_VC_CONTEXT * vctx,
              /* IN */ UCHAR free
          );

/**
 *  \brief To Release a VCP-VOCS or VCP-AICS context.
 *
 *  \par Description
 *       This function enables to release context of given VCP VOCS
 *       or VCP AICS service context. It disable the configuration for
 *       notifications and releases the context for VOCS/AICS.
 *       Once release is done, the context is freed up and the setup
 *       must be freshly done by calling GA_vc_set_capability() if
 *       required for the same device again.
 *
 *  \param [in] vcp_srvc_type
 *              VCP optional service type
 *               - \ref VCP_SERVICE_TYPE_VOCS
 *               - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [in] context
 *              Context Allocated for GA VCP Capability with the peer device.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_VC_RELEASE_CAPABILITY_CNF is notified
 *          on completion with status as success or failure.
 *
 *  \sa GA_VC_RELEASE_CAPABILITY_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_release_capability(vcp_srvc_type, vctx) \
        GA_vc_terminate_capability((vcp_srvc_type), (vctx), GA_FALSE)

/**
 *  \brief To Free a VCP-VOCS or VCP-AICS context.
 *
 *  \par Description
 *       This function enables to free context of given VCP VOCS
 *       or VCP AICS service context. It disable the configuration for
 *       notifications and releases the context for VOCS/AICS.
 *
 *  \param [in] vcp_srvc_type
 *              VCP optional service type
 *               - \ref VCP_SERVICE_TYPE_VOCS
 *               - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [in] context
 *              Context Allocated for GA VCP Capability with the peer device.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_close_capability(vcp_srvc_type, vctx) \
        GA_vc_terminate_capability((vcp_srvc_type), (vctx), GA_TRUE)

/** \} */

/**
 * \name VCP Volume Controller-Read
 * \{
 * \brief This section describes the Volume Control Profile Read APIs
 * for Volume Controller.
 */

/**
 *  \brief To get Volume State Information.
 *
 *  \par Description
 *       The routine enables to get Volume State Information.
 *
 *  \param [in] s_ctx
 *              Context Allocated for GA VCP with the peer device.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_GET_VOLUME_STATE_CNF event with associated data
 *          through the registered callback.
 *
 *  \sa GA_VC_GET_VOLUME_STATE_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_get_volume_state(s_ctx)                              \
        VCP_read_request                                           \
        (                                                          \
             (s_ctx),                                              \
             (GA_VC_GET_VOLUME_STATE_CNF)                          \
        )

/**
 *  \brief To get Volume Flags Information.
 *
 *  \par Description
 *       The routine enables to get Volume Flags Information.
 *
 *  \param [in] s_ctx
 *              Context Allocated for GA VCP with the peer device.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_GET_VOLUME_FLAGS_CNF event with associated data
 *          through the registered callback.
 *
 *  \sa GA_VC_GET_VOLUME_FLAGS_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_get_volume_flags(s_ctx)                              \
        VCP_read_request                                           \
        (                                                          \
             (s_ctx),                                              \
             (GA_VC_GET_VOLUME_FLAGS_CNF)                          \
        )

/**
 *  \brief To get Volume Offset State Information.
 *
 *  \par Description
 *       The routine enables to get Volume Offset State Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with VOCS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_VOCS_GET_OFFSET_STATE_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_VOCS_GET_OFFSET_STATE_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_vocs_get_volume_offset_state(s_ctx)                      \
        VCP_vocs_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_VOCS_GET_OFFSET_STATE_CNF)                         \
        )

/**
 *  \brief To get Audio Location Information.
 *
 *  \par Description
 *       The routine enables to get Audio Location Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with VOCS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_VOCS_GET_AUDIO_LOCATION_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_VOCS_GET_AUDIO_LOCATION_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_vocs_get_audio_location(s_ctx)                           \
        VCP_vocs_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_VOCS_GET_AUDIO_LOCATION_CNF)                       \
        )

/**
 *  \brief To get Audio Output Information.
 *
 *  \par Description
 *       The routine enables to get Audio Output Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with VOCS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_vocs_get_audio_out_description(s_ctx)                    \
        VCP_vocs_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF)                       \
        )

/**
 *  \brief To get Audio Input State Information.
 *
 *  \par Description
 *       The routine enables to get Audio Input State Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_AICS_GET_INPUT_STATE_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_AICS_GET_INPUT_STATE_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_get_input_state(s_ctx)                              \
        VCP_aics_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_AICS_GET_INPUT_STATE_CNF)                          \
        )

/**
 *  \brief To get Gain Setting Properties Information.
 *
 *  \par Description
 *       The routine enables to get Gain Setting Properties Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_get_gain_setting_proprty(s_ctx)                     \
        VCP_aics_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF)                 \
        )

/**
 *  \brief To get Audio Input Type Information.
 *
 *  \par Description
 *       The routine enables to get Audio Input Type Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_AICS_GET_INPUT_TYPE_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_AICS_GET_INPUT_TYPE_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_get_input_type(s_ctx)                               \
        VCP_aics_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_AICS_GET_INPUT_TYPE_CNF)                           \
        )

/**
 *  \brief To get Audio Input Status Information.
 *
 *  \par Description
 *       The routine enables to get Audio Input State Status Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_AICS_GET_INPUT_STATUS_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_AICS_GET_INPUT_STATUS_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_get_input_status(s_ctx)                             \
        VCP_aics_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_AICS_GET_INPUT_STATUS_CNF)                         \
        )

/**
 *  \brief To get Audio Input Description Information.
 *
 *  \par Description
 *       The routine enables to get Audio Input Description Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *          describing the cause of failure.
 *          On completion of this procedure, the module returns the
 *          \ref GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF event with associated
 *          data through the registered callback.
 *
 *  \sa GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_get_audio_input_description(s_ctx)                  \
        VCP_aics_read_request                                          \
        (                                                              \
             (s_ctx),                                                  \
             (GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF)                     \
        )

/** \} */

/**
 * \name VCP Volume Controller-Write
 * \{
 * \brief This section describes the Volume Control Profile Write
 * APIs for Volume Controller.
 */

/**
 *  \brief To set Relative Volume Down.
 *
 *  \par Description:
 *       This routine enables to set Relative Volume Down.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data for Relative Volume Down.
 *              - Change_Counter
 *
 *  \param [in] len
 *              Data length for Relative Volume Down.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_rel_vol_down(sh, val, len)                           \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_RELATIVE_VOL_DOWN),                            \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief
 *  To set Relative Volume Up.
 *
 *  \par Description:
 *       This routine enables to set Relative Volume Up.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data for Relative Volume Up.
 *              - Change_Counter
 *
 *  \param [in] len
 *              Data length for Relative Volume Up.
 *
 *  \return  \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_rel_vol_up(sh, val, len)                             \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_RELATIVE_VOL_UP),                              \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief
 *  To set UnMute and Relative Volume Down.
 *
 *  \par Description:
 *       This routine enables to set Unmute and Relative Volume Down.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data for Unmute & Relative Volume Down.
 *              - Change_Counter
 *
 *  \param [in] len
 *              Data length for Unmute & Relative Volume Down.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_unmute_rel_vol_down(sh, val, len)                    \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_UNMUTE_RELATIVE_VOL_DOWN),                     \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief
 *  To set UnMute and Relative Volume Up.
 *
 *  \par Description:
 *       This routine enables to set Unmute and Relative Volume Up.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data for Unmute & Relative Volume Up.
 *              - Change_Counter
 *
 *  \param [in] len
 *              Data length for Unmute & Relative Volume Up.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_unmute_rel_vol_up(sh, val, len)                      \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_UNMUTE_RELATIVE_VOL_UP),                       \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief
 *  To set Absolute Volume.
 *
 *  \par Description:
 *       This routine enables to set Absolution Volume.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data to set absolute volume.
 *              - Change_Counter
 *              - Volume_Setting
 *
 *  \param [in] len
 *              Data length of set absolute volume information.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_abs_volume(sh, val, len)                             \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_SET_ABSOLUE_VOL),                              \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief
 *  To set Unmute.
 *
 *  \par Description:
 *       This routine enables to Unmute.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data to Unmute.
 *               - Change_Counter
 *
 *  \param [in] len
 *              Data length of Unmute information.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_unmute(sh, val, len)                                 \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_UNMUTE),                                       \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief
 *  To set Mute.
 *
 *  \par Description:
 *       This routine enables to Mute.
 *
 *  \param [in] sh
 *         VCP Context
 *
 *  \param [in] val
 *              Data to Mute.
 *              - Change_Counter
 *
 *  \param [in] len
 *              Data length of Mute information.
 *
 *  \return \ref GA_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_set_mute(sh, val, len)                                   \
        VCP_cp_request                                                 \
        (                                                              \
             (sh),                                                     \
             (VCS_CP_OP_MUTE),                                         \
             (val),                                                    \
             (len)                                                     \
        )


/**
 *  \brief To set Audio Location Information.
 *
 *  \par Description
 *       The routine enables to set Audio Location Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with VOCS.
 *
 *  \param [in] al
 *         Audio Location Value.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_vocs_set_audio_location(s_ctx, al)                       \
        VCP_vocs_set_audio_location                                    \
        (                                                              \
             (s_ctx),                                                  \
             (al)                                                      \
        )

/**
 *  \brief To set Audio Output Description Information.
 *
 *  \par Description
 *       The routine enables to set Audio Output Description Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with VOCS.
 *
 *  \param [in] aod
 *         Pointer to Audio Output Description Information.
 *
 *  \param [in] aodl
 *         Audio Output Description Length.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_vocs_set_audio_output_description(s_ctx, aod, aodl)      \
        VCP_vocs_set_audio_output_description                          \
        (                                                              \
             (s_ctx),                                                  \
             (aod),                                                    \
             (aodl)                                                    \
        )

/**
 *  \brief To Set Volume Offset Information.
 *
 *  \par Description
 *       The routine enables to set Volume Offset Information.
 *
 *  \param [in] sh
 *         Context Handle associated with VOCS.
 *
 *  \param [in] val
 *          Volume Ofset Information.
 *              - Change_Counter
 *              - Volume_Offset
 *
 *  \param [in] len
 *         Length of Volume Flag Information.
 *
 *  \return \ref GA_SUCCESS on success or an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_VOCS_CP_WT_CNF is notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_vocs_set_volume_offset(sh, val, len)                     \
        VCP_vocs_cp_request                                            \
        (                                                              \
             (sh),                                                     \
             (VOCS_CP_OP_SET_VOLUME_OFFSET),                           \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To set Audio Input Description Information.
 *
 *  \par Description
 *       The routine enables to set Audio Input Description Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \param [in] aid
 *         Pointer to Audio Input Description Information.
 *
 *  \param [in] aidl
 *         Audio Input Description Length.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_set_audio_input_description(s_ctx, aid, aidl)       \
        VCP_aics_set_audio_input_description                           \
        (                                                              \
             (s_ctx),                                                  \
             (aid),                                                    \
             (aidl)                                                    \
        )

/**
 *  \brief To Set Gain Setting Information.
 *
 *  \par Description
 *       The routine enables to set Gain Setting Information.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Gain Setting Information.
 *              - Change_Counter
 *              - Gain_setting
 *
 *  \param [in] len
 *         Length of Gain Setting Information.
 *
 *  \return \ref GA_SUCCESS on success or an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_AICS_CP_WT_CNF
 *           will be notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_set_gain_setting(sh, val, len)                      \
        VCP_aics_cp_request                                            \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_SET_GAIN_SETTING),                            \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To Unmute.
 *
 *  \par Description
 *       The routine enables to Unmute.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Unmute Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Unmute Information.
 *
 *  \return \ref GA_SUCCESS on success or an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_AICS_CP_WT_CNF
 *           will be notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_unmute(sh, val, len)                                \
        VCP_aics_cp_request                                            \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_UNMUTE),                                      \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To Mute.
 *
 *  \par Description
 *       The routine enables to Mute.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Mute Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Mute Information.
 *
 *  \return \ref GA_SUCCESS on success or an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_AICS_CP_WT_CNF
 *           will be notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_mute(sh, val, len)                                  \
        VCP_aics_cp_request                                            \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_MUTE),                                        \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To set Manual Gain Mode.
 *
 *  \par Description
 *       The routine enables to Set Manual Gain Mode.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Set Manual Gain Mode Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Set Manual Gain Mode Information.
 *
 *  \return \ref GA_SUCCESS on success or an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_AICS_CP_WT_CNF
 *           will be notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_set_manual_gain_mode(sh, val, len)                  \
        VCP_aics_cp_request                                            \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_SET_MANUAL_GAIN_MODE),                        \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To set Automatic Gain Mode.
 *
 *  \par Description
 *       The routine enables to Set Automatic Gain Mode.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Set Automatic Gain Mode Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Set Automatic Gain Mode Information.
 *
 *  \return \ref GA_SUCCESS on success or an Error Code
 *           describing the cause of failure.
 *           If \ref GA_SUCCESS, \ref GA_VC_AICS_CP_WT_CNF
 *           will be notified.
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_aics_set_auto_gain_mode(sh, val, len)                    \
        VCP_aics_cp_request                                            \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_SET_AUTO_GAIN_MODE),                          \
             (val),                                                    \
             (len)                                                     \
        )

/** \} */
/** \} */

/**
 * \defgroup ga_vcp_vol_renderer_defs VCP Volume Renderer APIs
 * \{
 * \brief This section describes the Volume Control Profile APIs
 * for Volume Renderer.
 */

/**
 * \name VCP Volume Renderer-Init/Deinit
 * \{
 * \brief This section describes the Volume Control Profile Init/Deinit APIs
 * for Volume Renderer.
 */

/**
 *  \brief Initialize the GA Volume Control Server.
 *
 *  \par Description
 *  This routine initializes GA Volume Control Server
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_vc_server_init(cb)       GA_module_init(GA_MODULE_VCP, VCP_ROLE_SERVER, (cb))

/**
 *  \brief Shutdown the GA Volume Control Server.
 *
 *  \par Description
 *  This routine shuts down GA Volume Control Server
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_vc_server_shutdown()   GA_module_shutdown(GA_MODULE_VCP, VCP_ROLE_SERVER)

/**
 *  \brief To register optional services with VCP Renderer.
 *
 *  \par Description:
 *       This function enables to register optional (VOCS/AICS) service
 *       with VCP Render module.
 *
 *  \param [in] srvc_type
 *         Flag indicating Service type.
 *         - \ref VCP_SERVICE_TYPE_VOCS
 *         - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [out] handle
 *         Allocated Service Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_register
          (
              /* IN  */ UCHAR          srvc_type,
              /* OUT */ GA_VR_HANDLE * handle
          );

/**
 *  \brief To unregister optional services with VCP Renderer.
 *
 *  \par Description:
 *       This function enables to unregister optional (VOCS/AICS) service
 *       with VCP Renderer module.
 *
 *  \param [in] srvc_type
 *         Flag indicating Service type.
 *         - \ref VCP_SERVICE_TYPE_VOCS
 *         - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [out] handle
 *         Allocated Service Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
GA_RESULT GA_vc_unregister
          (
              /* IN  */ UCHAR          vc_type,
              /* IN */  GA_VR_HANDLE   handle
          );

/** \} */

/**
 * \name VCP Volume Renderer-Read/Write
 * \{
 * \brief This section describes the Volume Control Profile Read/Write
 * APIs for Volume Renderer.
 */

/**
 *  \brief To send response to incoming read/write request.
 *
 *  \par Description:
 *       This function enables to send response to incoming read/write
 *       request from the peer.
 *
 *  \param [in] d
 *         Peer Device Address Information.
 *
 *  \param [in] et
 *         Event type indicating response for read or write.
 *           - \ref GA_RSP_TYPE_RD
 *           - \ref GA_RSP_TYPE_WT
 *
 *  \param [in] es
 *         Response status for the read/write operation.
 *
 *  \param [in] rd
 *         Response information for read/write operatios \ref VCP_RSP_INF.
 *
 *  \param [in] rdl
 *         Response information size.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_send_rsp(d, et, es, rd, rdl)                             \
        VCP_send_rsp                                                   \
        (                                                              \
            (d),                                                       \
            (et),                                                      \
            (es),                                                      \
            (rd),                                                      \
            (rdl)                                                      \
        )

/** \} */

/**
 * \name VCP Volume Renderer-Notify
 * \{
 * \brief This section describes the Volume Control Profile Notify
 * APIs for Volume Renderer.
 */

/**
 *  \brief To Notify Charateristic Values from Volume renderer to connected
 *         Volume Controller.
 *
 *  \par Description
 *       This routine enables to notify updated char value to specified
 *       remote device.
 *
 *  \param [in] d
 *          Endpoint to be notified.
 *
 *   \param [in] si
 *           Associated Service Handle.
 *
 *   \param [in] st
 *           Flag to indicate Service Type.
 *           - \ref VCP_SERVICE_TYPE_VCS
 *           - \ref VCP_SERVICE_TYPE_VOCS
 *           - \ref VCP_SERVICE_TYPE_AICS
 *
 *  \param [in] cuid
 *              characteristic to be notified.
 *
 *  \param [in] cd
 *              characteristic data to be notificed.
 *
 *  \param [in] cdl
 *              characteristic data length.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify(d, si, st, cuid, cd, cdl)        \
        VCP_notify_char                               \
        (                                             \
            (d),                                      \
            (si),                                     \
            (st),                                     \
            (cuid),                                   \
            (cd),                                     \
            (cdl)                                     \
        )

/**
 *  \brief Notify Volume state Information.
 *
 *  \par Description
 *       This routine enables to notify Volume state information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] c_info
 *              Volume data Information.
 *
 *  \param [in] c_info_len
 *              Length of Volume State Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_vcs_volume_state(ep, c_info, c_info_len)        \
        GA_vc_notify                                                 \
        (                                                            \
             (ep),                                                   \
             (0x00),                                                 \
             (VCP_SERVICE_TYPE_VCS),                                 \
             (GA_CHAR_VCS_VOLUME_STATE),                            \
             (c_info),                                               \
             (c_info_len)                                            \
        )

/**
 *  \brief Notify Volume Flag Information.
 *
 *  \par Description
 *       This routine enables to notify Volume Flag information to the specified
 *       peer device.
 *
*  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] c_info
 *              Volume Flag Information.
 *
 *  \param [in] c_info_len
 *              Length of Volume Flag Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_vcs_volume_flag(ep, c_info, c_info_len)         \
        GA_vc_notify                                                 \
        (                                                            \
             (ep),                                                   \
             0x00,                                                   \
             VCP_SERVICE_TYPE_VCS,                                   \
             (GA_CHAR_VCS_VOLUME_FLAGS),                             \
             (c_info),                                               \
             (c_info_len)                                            \
        )

/**
 *  \brief Notify Volume Offset State Information.
 *
 *  \par Description
 *       This routine enables to notify Volume Offset Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with VOCS.
 *
 *  \param [in] c_info
 *         Volume Offset State Information.
 *           - Volume_Offset
 *           - Change_Counter
 *
 *  \param [in] c_info_len
 *         Length of Volume Offset State Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_vocs_volume_offset(ep, si, c_info, c_info_len)  \
        GA_vc_notify                                                 \
        (                                                            \
             (ep),                                                   \
             (si),                                                   \
             VCP_SERVICE_TYPE_VOCS,                                  \
             (GA_CHAR_VOCS_OFFSET_STATE),                           \
             (c_info),                                               \
             (c_info_len)                                            \
        )

/**
 *  \brief Notify Audio Location Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Location Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with VOCS.
 *
 *  \param [in] c_info
 *         Audio Location Information.
 *
 *  \param [in] c_info_len
 *         Length of Audio Location Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_vocs_audio_location(ep, si, c_info, c_info_len) \
        GA_vc_notify                                                 \
        (                                                            \
             (ep),                                                   \
             (si),                                                   \
             VCP_SERVICE_TYPE_VOCS,                                  \
             (GA_CHAR_VOCS_AUDIO_LOCATION),                         \
             (c_info),                                               \
             (c_info_len)                                            \
        )

/**
 *  \brief Notify Audio Output Description Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Output Description Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with VOCS.
 *
 *  \param [in] c_info
 *         Audio Location Information.
 *
 *  \param [in] c_info_len
 *         Length of Audio Location Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_vocs_audio_out_desc(ep, si, c_info, c_info_len) \
        GA_vc_notify                                                 \
        (                                                            \
             (ep),                                                   \
             (si),                                                   \
             VCP_SERVICE_TYPE_VOCS,                                  \
             (GA_CHAR_VOCS_AUDIO_OUTPUT_DESC),               \
             (c_info),                                               \
             (c_info_len)                                            \
        )

/**
 *  \brief Notify Audio Input state Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Input State Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with AICS.
 *
 *  \param [in] c_info
 *         Audio Input State Information.
 *          - Gain_Setting
 *          - Mute
 *          - Gain_Mode
 *          - Change_Counter
 *
 *  \param [in] c_info_len
 *         Length of Audio Input State Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_aics_audio_input_state(ep, si, c_info, c_info_len) \
        GA_vc_notify                                                    \
        (                                                               \
             (ep),                                                      \
             (si),                                                      \
             VCP_SERVICE_TYPE_AICS,                                     \
             (GA_CHAR_AICS_INPUT_STATE),                               \
             (c_info),                                                  \
             (c_info_len)                                               \
        )

/**
 *  \brief Notify Audio Input Status Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Input Status Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with AICS.
 *
 *  \param [in] c_info
 *         Audio Input Status Information.
 *
 *  \param [in] c_info_len
 *         Length of Audio Input Status Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_aics_audio_input_status(ep, si, c_info, c_info_len)\
        GA_vc_notify                                                    \
        (                                                               \
             (ep),                                                      \
             (si),                                                      \
             VCP_SERVICE_TYPE_AICS,                                     \
             (GA_CHAR_AICS_INPUT_STATUS),                              \
             (c_info),                                                  \
             (c_info_len)                                               \
        )

/**
 *  \brief Notify Audio Input Description Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Input Description Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with AICS.
 *
 *  \param [in] c_info
 *         Audio Input Description Information.
 *
 *  \param [in] c_info_len
 *         Length of Audio Input Description Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_vcp_error_code
 */
#define GA_vc_notify_aics_audio_input_desc(ep, si, c_info, c_info_len)  \
        GA_vc_notify                                                    \
        (                                                               \
             (ep),                                                      \
             (si),                                                      \
             VCP_SERVICE_TYPE_AICS,                                     \
             (GA_CHAR_AICS_AUDIO_INPUT_DESC),                           \
             (c_info),                                                  \
             (c_info_len)                                               \
        )

/** \} */
/** \} */
/** \} */
/** \} */

/* =====  MICP Related Defines ============= */

/**
 * \addtogroup ga_micp_module
 * \{
 */

/**
 * \defgroup ga_micp_api_sequences API Sequences
 * \{
 * \brief This section describes the EtherMind GA MICP Layer API Sequences.
 */

/**
 * \defgroup ga_micp_api_seq MICP API Sequences
 * \{
 * \brief This section describes the Microphone Control Profile API Sequences.
 * MSC depicting the flow of APIs and Events.
 * \mscfile MICP.signalling
 */

/** \} */
/** \} */

/**
 * \defgroup ga_micp_api_defs API Definitions
 * \{
 * \brief This section describes the Microphone Control Profile APIs.
 */

/**
 * \defgroup ga_micp_cont_defs MICP Microphone Controller API
 * \{
 * \brief This section describes the Microphone Control Profile APIs
 * for Microphone Controller.
 */

/**
 * \name MICP Microphone Controller APIs-Setup
 * \{
 * \brief This section describes the Microphone Control Profile Setup APIs
 * for Microphone Controller.
 */

/**
 *  \brief Initialize the GA Microphone Control Client
 *
 *  \par Description
 *  This routine initializes GA Microphone Control Client
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_mc_client_init(cb)       GA_module_init(GA_MODULE_MICP, MICP_ROLE_CLIENT, (cb))

/**
 *  \brief Shutdown the GA Microphone Control Client.
 *
 *  \par Description
 *  This routine shuts down GA Microphone Control Client
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_mc_client_shutdown()   GA_module_shutdown(GA_MODULE_MICP, MICP_ROLE_CLIENT)

/**
 *  \brief To Setup Microphone Control Service(MICS) Context
 *
 *  \par Description:
 *       This function enables to setup MICS context with given device.
 *       Internally, Service, Characterstics and Descriptors Discovery is
 *       done and Notifications are enabled.
 *       mics_ctx provided in this function to be used by application
 *       for further calls to have transaction on this MICS Handle.
 *
 *  \param [in] device
 *        Peer Device with which the context needs to be set-up.
 *
 *  \param [out] mics_ctx
 *        Allocated MICS Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_MC_SETUP_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa GA_MC_SETUP_CNF
 *  \sa ga_micp_error_code
 */
GA_RESULT GA_mc_setup
          (
              /* IN */  GA_ENDPOINT * device,
              /* OUT */ GA_MC_CONTEXT  * mc_ctx
          );

/**
 * \cond EXCLUDE_PART
 */
#ifdef MICP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To enable configuration for Characteristic Notifications.
 *
 *  \par Description
 *       The routine enables to configure notifications for individual characteristics
 *       of a given service.
 *
 *  \param [in] s_type
 *               - \ref MICP_SERVICE_TYPE_MICS
 *               - \ref MICP_SERVICE_TYPE_AICS
 *
 *  \param [in] config
 *              Bitmask for characteristics IDs.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
#define GA_mc_update_ntf_configuration(s_type, config)           \
        MICP_update_ntf_configuration((s_type), (config))
#endif /* MICP_SUPPORT_CONFIG_SELECTION */

#ifdef MICP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the MICS Context Information.
 *
 *  \par Description:
 *       This function enables to manage the context information of MICS.
 *
 *  \param [in] set
 *              GA_TRUE for Context SET and GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] mctx
 *         MICP Handle which is allocated during MICS context setup.
 *
 *  \param [inout] range
 *         AICS characteristics handle range.
 *
 *  \param [inout] info
 *         AICS characteristics information.
 *
 *  \param [inout] info_count
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_mc_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT       * device,
              /* INOUT  */ GA_MC_CONTEXT     * mctx,
              /* INOUT  */ GA_BRR_SVC_RANGE  ** range,
              /* INOUT  */ GA_CHAR_INFO      ** info,
              /* INOUT  */ UINT16            * info_count
          );

/**
 *  \brief To retrieve the MICS Context Information.
 *
 *  \par Description:
 *       This function enables to retreive the context information of MICS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         MICP Handle which is allocated during MICS context setup.
 *
 *  \param [inout] r
 *         MICS characteristics handle range.
 *
 *  \param [inout] i
 *         MICS characteristics information.
 *
 *  \param [inout] c
 *         MICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_mc_get_context_info(d, h, r, i, c) \
        GA_mc_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To save the MICS Context Information.
 *
 *  \par Description:
 *       This function enables to save the context information of MICS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         MICP Handle which is allocated during MICS context setup.
 *
 *  \param [inout] r
 *         MICS characteristics handle range.
 *
 *  \param [inout] i
 *         MICS characteristics information.
 *
 *  \param [inout] c
 *         MICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_mc_set_context_info(d, h, r, i, c) \
        GA_mc_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))

/**
 *  \brief To manage the AICS Context Information.
 *
 *  \par Description:
 *       This function enables to manage the context information of AICS.
 *
 *  \param [in] set
 *         GA_TRUE for Context SET and GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] aics_ctx
 *         AICS Handle which is allocated during AICS context setup.
 *
 *  \param [inout] range
 *         AICS characteristics handle range.
 *
 *  \param [inout] info
 *         AICS characteristics information.
 *
 *  \param [inout] info_count
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_mc_manage_aics_context_info
          (
              /* IN     */ UINT8                 set,
              /* IN     */ GA_ENDPOINT        *  device,
              /* INOUT  */ GA_MC_CONTEXT      *  aics_ctx,
              /* INOUT  */ GA_BRR_SVC_RANGE   ** range,
              /* INOUT  */ GA_CHAR_INFO       ** info,
              /* INOUT  */ UINT16             * info_count
          );

/**
 *  \brief To retrieve the AICS Context Information.
 *
 *  \par Description:
 *       This function enables to retrieve the context information of AICS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         AICS Handle which is allocated during AICS context setup.
 *
 *  \param [inout] r
 *         AICS characteristics handle range.
 *
 *  \param [inout] i
 *         AICS characteristics information.
 *
 *  \param [inout] c
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_mc_get_aics_context_info(d, h, r, i, c) \
        GA_mc_manage_aics_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To save the AICS Context Information.
 *
 *  \par Description:
 *       This function enables to save the context information of AICS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] aics_h
 *         AICS Handle which is allocated during AICS context setup.
 *
 *  \param [inout] r
 *         AICS characteristics handle range.
 *
 *  \param [inout] i
 *         AICS characteristics information.
 *
 *  \param [inout] c
 *         AICS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_mc_set_aics_context_info(d, h, r, i, c) \
        GA_mc_manage_aics_context_info(GA_TRUE, (d), (h), (r), (i), (c))
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/**
 *  \brief Get the Optional MICP capabilities from the peer device.
 *
 *  \par Description:
 *       This function enables to discover the optional included services (i.e. AICS)
 *       from the remote device.
 *
 *  \param [in] micp_cntx
 *         Context associated with MICP.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_MC_GET_CAPABILITIES_CNF is
 *          notified.
 *
 *  \note This procedure returns the \ref GA_MC_GET_CAPABILITIES_CNF event for
 *        each optional service instance discovered with event status
 *        \ref GA_CONTINUE.
 *        On completion of this procedure, \ref GA_MC_GET_CAPABILITIES_CNF
 *        event is notified with event status \ref GA_SUCCESS.
 *        Application/Upper Layer is expected use the information returned along with
 *        \ref GA_MC_GET_CAPABILITIES_CNF event for setting up the optional
 *        capabilities.
 *
 *        The device must have performed MICP setup before calling
 *        this function.
 *        If AICS context has to be setup, \ref GA_mc_set_capability() has to
 *        be called with the service handle range received as part of this
 *        event.
 *
 *  \sa GA_MC_GET_CAPABILITIES_CNF
 *  \sa ga_micp_error_code
 */
GA_RESULT GA_mc_get_capabilities
          (
              /* IN */ GA_MC_CONTEXT * mc_cntx
          );

/**
 *  \brief Setup MICP optional capabiliy with the peer device.
 *
 *  \par Description
 *       This routine allocates a MICP optional service context with a peer endpoint,
 *       discovers the optional(AICS) service charactersicstics and configures
 *       for notification.
 *
 *  \param [in] mctx
 *              MICP Context
 *
 *  \param [in] svc
 *              Optional Service(AICS) Service details to Setup.
 *
 *  \param [out] cctx
 *              Context Allocated for optional service.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref GA_MC_SET_CAPABILITIES_CNF is
 *          notified.
 *
 *  \sa GA_MC_SET_CAPABILITIES_CNF
 *  \sa ga_micp_error_code
 */
GA_RESULT GA_mc_set_capability
          (
              /* IN */  GA_MC_CONTEXT   * mctx,
              /* IN */  GA_BRR_SVC_INFO * svc,
              /* OUT */ GA_MC_CONTEXT   * cctx
          );

/**
 *  \brief To release/free a MICS Context.
 *
 *  \par Description:
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling GA_mc_setup() if required for
 *  the same device again.
 *
 *  \param [in] mctx Allocated MICS Context Handle.
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note
 *   1. Before releasing the context for MICS, the application is expected to release
 *       context of all the included optional AICS service instances.
 *   2. On completion of this procedure, the module returns the \ref GA_MC_RELEASE_CNF event
 *      through the registered callback.
 */
GA_RESULT GA_mc_terminate
          (
              /* IN */ GA_MC_CONTEXT * mctx,
              /* IN */ UCHAR free
          );

/**
 *  \brief To release a MICS Context.
 *
 *  \par Description:
 *       This function enables to release an allocated context.
 *       Internally, it clears the context and disable the config for notifications.
 *
 *  \param [in] mctx
 *        Allocated MICS Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note
 *   1. Before releasing the context for MICS, the application is expected to release
 *       context of all the included optional AICS service instances.
 *   2. On completion of this procedure, the module returns the \ref GA_MC_RELEASE_CNF event
 *      through the registered callback.
 */
#define GA_mc_release(mctx) \
        GA_mc_terminate((mctx), GA_FALSE)

/**
 *  \brief To free a MICS Context.
 *
 *  \par Description:
 *       This function enables to free an allocated context.
 *       Internally, it clears the context and disable the config for notifications.
 *
 *  \param [in] mctx
 *        Allocated MICS Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note
 *   1. Before freeing the context for MICS, the application is expected to release
 *       context of all the included optional AICS service instances.
 */
#define GA_mc_close(mctx) \
        GA_mc_terminate((mctx), GA_TRUE)

/**
 *  \brief To release/free a MICS Optional Service Context.
 *
 *  \par Description:
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling GA_mc_set_capability() if
 *  required for the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] micp_aics_cntx Allocated AICS Context Handle.
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note \ref GA_MC_RELEASE_CAPABILITY_CNF is notified once release is
 *        complete.
 *
 *  \sa GA_MC_RELEASE_CAPABILITY_CNF
 *  \sa ga_micp_error_code
 */
GA_RESULT GA_mc_terminate_capability
          (
              /* IN */ GA_CONTEXT * micp_aics_cntx,
              /* IN */ UCHAR free
          );

/**
 *  \brief To release a MICS Optional Service Context.
 *
 *  \par Description:
 *       This function enables to release an allocated context of MICS optional
 *       Service - AICS. Internally, it clears the context and disables the
 *       config for notifications. Once release is done, the context is freed up
 *       and the setup must be freshly done by calling GA_mc_set_capability() if
 *       required for the same device again.
 *
 *  \param [in] micp_aics_cntx Allocated AICS Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note \ref GA_MC_RELEASE_CAPABILITY_CNF is notified once release is
 *        complete.
 *
 *  \sa GA_MC_RELEASE_CAPABILITY_CNF
 *  \sa ga_micp_error_code
 */
#define GA_mc_release_capability(micp_aics_cntx) \
        GA_mc_terminate_capability((micp_aics_cntx), GA_FALSE)

/**
 *  \brief To free a MICS Optional Service Context.
 *
 *  \par Description:
 *       This function enables to free an allocated context of MICS optional
 *       Service - AICS.
 *
 *  \param [in] micp_aics_cntx Allocated AICS Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_close_capability(micp_aics_cntx) \
        GA_mc_terminate_capability((micp_aics_cntx), GA_TRUE)

/** \} */

/**
 * \name MICP Microphone Controller APIs-Read
 * \{
 * \brief This section describes the Microphone Control Profile Read APIs
 * for Microphone Controller.
 */

/**
 *  \brief To get Mute Status Information.
 *
 *  \par Description
 *       The routine enables to get Mute Status Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with MICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, Char is Mute \ref GA_MC_GET_MUTE_CNF is
 *          notified with associated data through the registered callback.
 *
 *  \sa GA_MC_GET_MUTE_CNF
 *  \sa ga_micp_error_code
 */
#define GA_mc_mics_get_mute(s_ctx)                                      \
        MICP_read_request                                               \
        (                                                               \
             (s_ctx),                                                   \
             (GA_MC_GET_MUTE_CNF)                                       \
        )

/**
 *  \brief To get Input State Information.
 *
 *  \par Description
 *       The routine enables to get Input State Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_GET_INPUT_STATE_CNF is notified
 *          through the registered callback.
 *
 *  \sa ga_micp_cont_defs
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_get_input_state(s_ctx)                              \
        MICP_aics_read_request                                         \
        (                                                              \
             (s_ctx),                                                  \
             (GA_MC_AICS_GET_INPUT_STATE_CNF)                          \
        )

/**
 *  \brief To get Gain Setting Property Information.
 *
 *  \par Description
 *       The routine enables to get Gain Setting Property Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_cont_defs
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_get_gain_setting_proprty(s_ctx)                     \
        MICP_aics_read_request                                         \
        (                                                              \
             (s_ctx),                                                  \
             (GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF)                 \
        )

/**
 *  \brief To get Input Type Information.
 *
 *  \par Description
 *       The routine enables to get Input Type Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_GET_INPUT_TYPE_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_cont_defs
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_get_input_type(s_ctx)                               \
        MICP_aics_read_request                                         \
        (                                                              \
             (s_ctx),                                                  \
             (GA_MC_AICS_GET_INPUT_TYPE_CNF)                           \
        )

/**
 *  \brief To get Input Status Information.
 *
 *  \par Description
 *       The routine enables to get Input State Status Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_GET_INPUT_STATUS_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_cont_defs
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_get_input_status(s_ctx)                             \
        MICP_aics_read_request                                         \
        (                                                              \
             (s_ctx),                                                  \
             (GA_MC_AICS_GET_INPUT_STATUS_CNF)                         \
        )

/**
 *  \brief To get Audio Input Description Information.
 *
 *  \par Description
 *       The routine enables to get Audio Input Description Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_cont_defs
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_get_audio_input_description(s_ctx)                  \
        MICP_aics_read_request                                         \
        (                                                              \
             (s_ctx),                                                  \
             (GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF)                     \
        )

/** \} */

/**
 * \name MICP Microphone Controller APIs-Write
 * \{
 * \brief This section describes the Microphone Control Profile Write APIs
 * for Microphone Controller.
 */

/**
 *  \brief To Set Mute Status as Unmuted
 *
 *  \par Description
 *       The routine enables to set Mute Status as Unmuted.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with MICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_SET_MUTE_CNF is notified
 *          through the registered callback.
 *
 *  \sa MICS_CHR_VAL_NOT_MUTED
 *  \sa ga_micp_error_code
 */
#define GA_mc_mics_set_unmute(s_ctx)                                    \
        MICP_set_mute                                                   \
        (                                                               \
             (s_ctx),                                                   \
             (MICS_CHR_VAL_NOT_MUTED)                                   \
        )

/**
 *  \brief To Set Mute Status as Muted
 *
 *  \par Description
 *       The routine enables to set Mute Status as Muted
 *
 *  \param [in] s_ctx
 *         Context Handle associated with MICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_SET_MUTE_CNF is notified
 *          through the registered callback.
 *
 *  \sa MICS_CHR_VAL_MUTED
 *  \sa ga_micp_error_code
 */
#define GA_mc_mics_set_mute(s_ctx)                                      \
        MICP_set_mute                                                   \
        (                                                               \
             (s_ctx),                                                   \
             (MICS_CHR_VAL_MUTED)                                       \
        )

/**
 *  \brief To Set Mute Status as Disabled
 *
 *  \par Description
 *       The routine enables to set Mute Status as Disabled
 *
 *  \param [in] s_ctx
 *         Context Handle associated with MICS.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_SET_MUTE_CNF is notified
 *          through the registered callback.
 *
 *  \sa MICS_CHR_VAL_DISABLED
 *  \sa ga_micp_error_code
 */
#define GA_mc_mics_set_mute_disable(s_ctx)                              \
        MICP_set_mute                                                   \
        (                                                               \
             (s_ctx),                                                   \
             (MICS_CHR_VAL_DISABLED)                                    \
        )

/**
 *  \brief To set Audio Input Description Information.
 *
 *  \par Description
 *       The routine enables to set Audio Input Description Information.
 *
 *  \param [in] s_ctx
 *         Context Handle associated with AICS.
 *
 *  \param [in] aid
 *         Pointer to Audio Input Description Information.
 *
 *  \param [in] aidl
 *         Audio Input Description Length.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Since it is WriteWithoutResponse request,
 *       There is no response expected for this event.
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_set_audio_input_description(s_ctx, aid, aidl)       \
        MICP_aics_set_audio_input_description                          \
        (                                                              \
             (s_ctx),                                                  \
             (aid),                                                    \
             (aidl)                                                    \
        )

/**
 *  \brief To Set Gain Setting Information.
 *
 *  \par Description
 *       The routine enables to set Gain Setting Information.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Gain Setting Information.
 *              - Change_Counter
 *              - Gain_setting
 *
 *  \param [in] len
 *         Length of Gain Setting Information.
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_CP_WT_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_set_gain_setting(sh, val, len)                      \
        MICP_aics_cp_request                                           \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_SET_GAIN_SETTING),                            \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To Unmute.
 *
 *  \par Description
 *       The routine enables to Unmute.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Unmute Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Unmute Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_CP_WT_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_unmute(sh, val, len)                                \
        MICP_aics_cp_request                                           \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_UNMUTE),                                      \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To Mute.
 *
 *  \par Description
 *       The routine enables to Mute.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Mute Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Mute Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_CP_WT_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_mute(sh, val, len)                                  \
        MICP_aics_cp_request                                           \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_MUTE),                                        \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To set Manual Gain Mode.
 *
 *  \par Description
 *       The routine enables to Set Manual Gain Mode.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Set Manual Gain Mode Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Set Manual Gain Mode Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_CP_WT_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_error_code
 */

#define GA_mc_aics_set_manual_gain_mode(sh, val, len)                  \
        MICP_aics_cp_request                                           \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_SET_MANUAL_GAIN_MODE),                        \
             (val),                                                    \
             (len)                                                     \
        )

/**
 *  \brief To set Automatic Gain Mode.
 *
 *  \par Description
 *       The routine enables to Set Automatic Gain Mode.
 *
 *  \param [in] sh
 *         Context Handle associated with AICS.
 *
 *  \param [in] val
 *          Set Automatic Gain Mode Information.
 *              - Change_Counter
 *
 *  \param [in] len
 *         Length of Set Automatic Gain Mode Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS \ref GA_MC_AICS_CP_WT_CNF is
 *          notified through the registered callback.
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_aics_set_auto_gain_mode(sh, val, len)                    \
        MICP_aics_cp_request                                           \
        (                                                              \
             (sh),                                                     \
             (AICS_CP_OP_SET_AUTO_GAIN_MODE),                          \
             (val),                                                    \
             (len)                                                     \
        )

/** \} */
/** \} */

/**
 * \defgroup ga_micp_dev_defs MICP Microphone Device APIs
 * \{
 * \brief This section describes the Microphone Control Profile APIs
 * for Microphone Device.
 */

/**
 * \name MICP Microphone Device APIs-Init/Deinit
 * \{
 * \brief This section describes the Microphone Control Profile Init/Deinit APIs
 * for Microphone Device.
 */

/**
 *  \brief Initialize the GA Microphone Control Server.
 *
 *  \par Description
 *  This routine initializes GA Microphone Control Server
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_mc_server_init(cb)       GA_module_init(GA_MODULE_MICP, MICP_ROLE_SERVER, (cb))

/**
 *  \brief Shutdown the GA Microphone Control Server.
 *
 *  \par Description
 *  This routine shuts down GA Microphone Control Server
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_mc_server_shutdown()   GA_module_shutdown(GA_MODULE_MICP, MICP_ROLE_SERVER)

/**
 *  \brief To register optional AICS service with MICS.
 *
 *  \par Description:
 *       This function enables to register optional AICS service.
 *       It will include the AICS service and add its characteristics.
 *
 *  \param [out] aics_hndle
 *       Allocated service Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_register_aics(aics_hndle)                      \
        MICP_register_aics((aics_hndle))


/**
 *  \brief To unregister optional AICS service with MICS.
 *
 *  \par Description:
 *       This function enables to unregister optional AICS service.
 *
 *  \param [in] aics_hndle
 *       Allocated service Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_unregister_aics(aics_hndle)                   \
        MICP_unregister_aics((aics_hndle))

/** \} */

/**
 * \name MICP Microphone Device APIs-Read/Write
 * \{
 * \brief This section describes the Microphone Control Profile Read/Write APIs
 * for Microphone Device.
 */

/**
 *  \brief To send response to incoming request.
 *
 *  \par Description:
 *       This function enables to send response to incoming request from the peer.
 *
 *  \param [in] d
 *         Peer Device Address Information.
 *
 *  \param [in] et
 *         Event type indicating response for read or write.
 *           - \ref GA_RSP_TYPE_RD
 *           - \ref GA_RSP_TYPE_WT
 *
 *  \param [in] es
 *         Response status for the read/write operation.
 *
 *  \param [in] rd
 *         Response information for read/write operatios \ref MICP_RSP_INF.
 *
 *  \param [in] rdl
 *         Response information size.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_error_code
 */
#define GA_mc_send_rsp(d, et, es, rd, rdl)                             \
        MICP_send_rsp                                                  \
        (                                                              \
            (d),                                                       \
            (et),                                                      \
            (es),                                                      \
            (rd),                                                      \
            (rdl)                                                      \
        )

/** \} */

/**
 * \name MICP Microphone Device APIs-Notify
 * \{
 * \brief This section describes the Microphone Control Profile Profile Notify
 * APIs for Microphone Device.
 */

/**
 *  \brief Notify Mute Status Information.
 *
 *  \par Description
 *       This routine enables to notify MICS Mute state Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] c_info
 *         Mute Status Information.
 *
 *  \param [in] c_info_len
 *         Length of Mute Status Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_dev_defs
 *  \sa ga_micp_error_code
 */
#define GA_mcd_notify_mics_mute_status(ep, c_info, c_info_len)           \
        MICP_notify_char                                                 \
        (                                                                \
             (ep),                                                       \
             (0x00),                                                     \
             (MICP_SERVICE_TYPE_MICS),                                   \
             (GA_CHAR_MICS_MUTE),                                        \
             (c_info),                                                   \
             (c_info_len)                                                \
        )

/**
 *  \brief Notify Audio Input state Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Input State Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with AICS.
 *
 *  \param [in] c_info
 *         Audio Input State Information.
 *          - Gain_Setting
 *          - Mute
 *          - Gain_Mode
 *          - Change_Counter
 *
 *  \param [in] c_info_len
 *         Length of Audio Input State Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_dev_defs
 *  \sa ga_micp_error_code
 */
#define GA_mcd_notify_aics_audio_input_state(ep, si, c_info, c_info_len)  \
        MICP_notify_char                                                  \
        (                                                                 \
             (ep),                                                        \
             (si),                                                        \
             (MICP_SERVICE_TYPE_AICS),                                    \
             (GA_CHAR_AICS_INPUT_STATE),                                  \
             (c_info),                                                    \
             (c_info_len)                                                 \
        )

/**
 *  \brief Notify Audio Input Status Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Input Status Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with AICS.
 *
 *  \param [in] c_info
 *         Audio Input Status Information.
 *
 *  \param [in] c_info_len
 *         Length of Audio Input Status Information.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_micp_dev_defs
 *  \sa ga_micp_error_code
 */
#define GA_mcd_notify_aics_audio_input_status(ep, si, c_info, c_info_len) \
        MICP_notify_char                                                  \
        (                                                                 \
             (ep),                                                        \
             (si),                                                        \
             (MICP_SERVICE_TYPE_AICS),                                    \
             (GA_CHAR_AICS_INPUT_STATUS),                                 \
             (c_info),                                                    \
             (c_info_len)                                                 \
        )

/**
 *  \brief Notify Audio Input Description Information.
 *
 *  \par Description
 *       This routine enables to notify Audio Input Description Information to the specified
 *       peer device.
 *
 *  \param [in] ep
 *         Device Endpoint Address to be notified.
 *
 *  \param [in] si
 *         Service Instance associated with AICS.
 *
 *  \param [in] c_info
 *         Audio Input Description Information.
 *
 *  \param [in] c_info_len
 *         Length of Audio Input Description Information.
 *
 *  \return ref GA_SUCCESS or one of the error codes as defined in \ref
 *          ga_micp_error_code
 *
 *  \sa ga_micp_dev_defs
 *  \sa ga_micp_error_code
 */
#define GA_mcd_notify_aics_audio_input_desc(ep, si, c_info, c_info_len)   \
        MICP_notify_char                                                  \
        (                                                                 \
             (ep),                                                        \
             (si),                                                        \
             (MICP_SERVICE_TYPE_AICS),                                    \
             (GA_CHAR_AICS_AUDIO_INPUT_DESC),                             \
             (c_info),                                                    \
             (c_info_len)                                                 \
        )

/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_csip_module
 * \{
 */

/**
 * \defgroup ga_csip_api_sequences API Sequences
 * \{
 * \brief This section describes the EtherMind GA CSIP Layer API Sequences.
 */

/**
 * \defgroup ga_csip_api_seq CSIP API Sequences
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * API Sequences.
 * MSC depicting the flow of APIs and Events.
 * \mscfile CSIP.signalling
 */

/** \} */
/** \} */

/**
 * \defgroup ga_csip_api_defs API Definitions
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * APIs.
 */

/**
 * \defgroup ga_csip_setco_defs CSIP SetCoordinator APIs
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * APIs for SetCoordinator.
 */

/**
 * \name CSIP SetCoordinator APIs-Setup/Release
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Setup/Release APIs for SetCoordinator.
 */

/**
 *  \brief Initialize the GA Coordinated Set Client.
 *
 *  \par Description
 *  This routine initializes GA Coordinated Set Client
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_cs_client_init(cb)       GA_module_init(GA_MODULE_CSIP, CSIP_ROLE_CLIENT, (cb))

/**
 *  \brief Shutdown the GA Coordinated Set Client.
 *
 *  \par Description
 *  This routine shuts down GA Coordinated Set Client
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_cs_client_shutdown()   GA_module_shutdown(GA_MODULE_CSIP, CSIP_ROLE_CLIENT)

/**
 *  \brief To Setup Coordinated Set Idenitfication Profile (CSIP) Context
 *
 *  \par Description:
 *       This function enables to setup CSIP context with given device.
 *       Internally, CSIS Char/Desc is discovered, post which configuration
 *       of notification for applicable char is done.
 *
 *  \param [in] device
 *        Peer Device with which the context is setup.
 *
 *  \param [in] svc_info
 *        Service UUID and Range information.
 *
 *  \param [out] cs_context
 *        Allocated CSIP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_SETUP_CNF will be notified.
 *
 *  \sa ga_csip_error_code
 */
GA_RESULT GA_cs_setup
          (
              /* IN */  GA_ENDPOINT * device,
              /* IN */  GA_BRR_SVC_INFO * svc_info,
              /* OUT */ GA_CS_CONTEXT  * cs_context
          );

/**
 * \cond EXCLUDE_PART
 */
#ifdef CSIP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To enable configuration for Characteristic Notifications.
 *
 *  \par Description
 *       The routine enables to configure notifications for individual characteristics
 *       of CSIS service.
 *
 *  \param [in] config
 *              Bitmask for characteristics IDs.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h
 */
#define GA_cs_update_ntf_configuration(config) \
        CSIP_update_ntf_configuration((config))
#endif /* CSIP_SUPPORT_CONFIG_SELECTION */

#ifdef CSIP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the CSIS Context Information.
 *
 *  \par Description:
 *       This function enables to manage the context information of CSIS.
 *
 *  \param [in] set
 *              GA_TRUE for Context SET and GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] cctx
 *         CSIP Handle which is allocated during CSIS context setup.
 *
 *  \param [inout] range
 *         CSIS characteristics handle range.
 *
 *  \param [inout] info
 *          CSIS characteristics information.
 *
 *  \param [inout] info_count
 *          CSIS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_cs_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ GA_CS_CONTEXT    * cctx,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To retrieve the CSIS Context Information.
 *
 *  \par Description:
 *       This function enables to retrieve the context information of CSIS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         CSIP Handle which is allocated during CSIS context setup.
 *
 *  \param [inout] r
 *         CSIS characteristics handle range.
 *
 *  \param [inout] i
 *          CSIS characteristics information.
 *
 *  \param [inout] c
 *          CSIS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_cs_get_context_info(d, h, r, i, c) \
        GA_cs_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To save the CSIS Context Information.
 *
 *  \par Description:
 *       This function enables to save the context information of CSIS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         CSIP Handle which is allocated during CSIS context setup.
 *
 *  \param [inout] r
 *         CSIS characteristics handle range.
 *
 *  \param [inout] i
 *          CSIS characteristics information.
 *
 *  \param [inout] c
 *          CSIS characteristics count.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_cs_set_context_info(d, h, r, i, c) \
        GA_cs_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))
#endif /* CSIP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/**
 *  \brief To Release/Free Coordinated Set Idenitfication Profile (CSIP) Context
 *
 *  \par Description:
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling GA_cs_setup() if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] cs_context
 *       CSIP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_RELEASE_CNF will be notified
 *
 *  \sa ga_csip_error_code
 */
GA_RESULT GA_cs_terminate
          (
              /* IN */ GA_CS_CONTEXT * cs_context,
              /* IN */ UCHAR free
          );

/**
 *  \brief To Release Coordinated Set Idenitfication Profile (CSIP) Context
 *
 *  \par Description:
 *       This function enables to release the given CSIP context.
 *       Internally, it clears the context and disables the config
 *       for notifications.
 *
 *  \param [in] cs_context
 *       CSIP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_RELEASE_CNF will be notified
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_release(cs_context) \
        GA_cs_terminate((cs_context), GA_FALSE)

/**
 *  \brief To Free Coordinated Set Idenitfication Profile (CSIP) Context
 *
 *  \par Description:
 *       This function enables to free the given CSIP context.
 *
 *  \param [in] cs_context
 *       CSIP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_close(cs_context) \
        GA_cs_terminate((cs_context), GA_TRUE)

/**
 *  \brief To discover Coordinated Set Idenitfication Service (CSIS)
 *
 *  \par Description:
 *       This function discovers the CSIS in a given device. \n
 *       It triggers a Service discovery for CSIS on the remote device.
 *
 *  \param [in] device
 *        Peer Device on which the discovery is performed.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_DISCOVER_CNF will be notified. \n
 *        To setup a CSIS context, Call \ref GA_cs_setup().
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_discover(device) \
        CSIP_discover_csis((device))

/** \} */

/**
 * \name CSIP SetCoordinator APIs-Read
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Read APIs for SetCoordinator.
 */

/**
 *  \brief To get the SIRK from the context set member.
 *
 *  \par Description:
 *       This function reads the SIRK attribute from the set member pointed
 *       by the context.
 *
 *  \param [in] ch
 *        Allocated CISP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_SIRK_GET_CNF will be notified.
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_get_sirk(ch) \
        CSIP_get_value((ch), CSIP_SIRK_GET_CNF)

/**
 *  \brief To get the Set Size from the context set member.
 *
 *  \par Description:
 *       This function reads the Set Size attribute from the set member pointed
 *       by the context
 *
 *  \param [in] ch
 *        Allocated CISP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_SETSIZE_GET_CNF will be notified.
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_get_setsize(ch) \
        CSIP_get_value((ch), CSIP_SETSIZE_GET_CNF)

/**
 *  \brief To get the SetMember rank from the context set member.
 *
 *  \par Description:
 *       This function reads the SetMember rank attribute from the set member pointed
 *       by the context
 *
 *  \param [in] ch
 *        Allocated CISP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_SETMEMBER_RANK_GET_CNF will be notified.
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_get_setmember_rank(ch) \
        CSIP_get_value((ch), CSIP_SETMEMBER_RANK_GET_CNF)

/**
 *  \brief To get the SetMember Lock from the context set member.
 *
 *  \par Description:
 *       This function reads the SetMember Lock attribute from the set member pointed
 *       by the context
 *
 *  \param [in] ch
 *        Allocated CISP Context Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_SETMEMBER_LOCK_GET_CNF will be notified.
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_get_setmember_lock(ch) \
        CSIP_get_value((ch), CSIP_SETMEMBER_LOCK_GET_CNF)

/** \} */

/**
 * \name CSIP SetCoordinator APIs-Write
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Write APIs for SetCoordinator.
 */

/**
 *  \brief To set the SetMember lock from the context set member.
 *
 *  \par Description:
 *       This function writes the SetMember lock attribute from the set member
 *       pointed by the context.
 *
 *  \param [in] ch
 *        Allocated CISP Context Handle.
 *
 *  \param [in] s
 *        Data for the attribute (Lock Value)
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note For \ref GA_SUCCESS, \ref GA_CS_SETMEMBER_LOCK_SET_CNF will be notified.
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_set_setmember_lock(ch, s) \
        CSIP_set_value((ch), CSIP_SETMEMBER_LOCK_SET_CNF, &(s), sizeof(UINT8))

/** \} */
/** \} */

/**
 * \defgroup ga_csip_setmem_defs CSIP SetMember APIs
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * APIs for SetMember.
 */

/**
 * \name CSIP SetMember APIs-Init
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Init APIs for SetMember.
 */

/**
 *  \brief Initialize the GA Coordinated Set Server.
 *
 *  \par Description
 *  This routine initializes GA Coordinated Set Server
 *
 *  \param [in] cb Application profile callback of type \ref GA_PROFILE_CB
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_cs_server_init(cb)       GA_module_init(GA_MODULE_CSIP, CSIP_ROLE_SERVER, (cb))

/**
 *  \brief Shutdown the GA Coordinated Set Server.
 *
 *  \par Description
 *  This routine shuts down GA Coordinated Set Server
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_cs_server_shutdown()   GA_module_shutdown(GA_MODULE_CSIP, CSIP_ROLE_SERVER)

/**
 *  \brief To register a local CSIP set member instance.
 *
 *  \par Description:
 *       This function registers a local CSIP Set Member instance
 *
 *  \param [out] handle
 *        Address to hold assigned member handle as in \ref CSIP_MEMBER_HANDLE
 *
 * \param [out] svc_id
 *        Service ID
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_register(handle, svc_id) \
        CSIP_register_member((handle), svc_id)

/** \} */

/**
 * \name CSIP SetMember APIs-Read/Write
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Read/Write APIs for SetMember.
 */

/**
 *  \brief To send response to incoming request.
 *
 *  \par Description:
 *       This function enables to send response to incoming request from the peer.
 *
 *  \param [in] d
 *         Peer Device Address Information.
 *
 *  \param [in] et
 *         Event type for which response is sent.
 *
 *  \param [in] es
 *         Response status for the read/write operation.
 *
 *  \param [in] rs
 *         Response information for read/write operatios \ref CISP_RSP_INFO.
 *
 *  \param [in] sz
 *         Response information size.
 *
 *  \return \ref GA_SUCCESS or one of the error codes.
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_send_response(d, et, es, rs, sz) \
        CSIP_send_rsp((d), (et), (es), (rs), (sz))

/** \} */

/**
 * \name CSIP SetMember APIs-Notify
 * \{
 * \brief This section describes the Coordinated Set Identification Profile
 * Notify APIs for SetMember.
 */

/**
 *  \brief To send Notifications - Set Member Lock.
 *
 *  \par Description:
 *       This function sends notifications if configured for Set Member Lock.
 *
 *  \param [in] d
 *         If NULL, Ntf to all connected CSIP sessions will be sent.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] ch
 *         CSIS Handle Identifier.
 *
 *  \param [in] s
 *         Data to be sent as part of the notification - UINT8
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_send_setmember_lock_ntf(d, ch, s) \
        CSIP_send_ntf((d), (ch), GA_CHAR_CSIS_LOCK, &(s), sizeof(UINT8))

/**
 *  \brief To send Notifications - Set Identity Resolving Key.
 *
 *  \par Description:
 *       This function sends notifications if configured for SIRK.
 *
 *  \param [in] d
 *         If NULL, Ntf to all connected CSIP sessions will be sent.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] ch
 *         CSIS Handle Identifier.
 *
 *  \param [in] ntf_data
 *         Data to be sent as part of the notification
 *
 *  \param [in] ntf_datalen
 *         Length of the data to be sent as part of the notification.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_send_sirk_ntf(d, ch, ntf_data, ntf_datalen)               \
        CSIP_send_ntf((d), (ch), GA_CHAR_CSIS_SIRK, (ntf_data), (ntf_datalen))

/**
 *  \brief To send Notifications - Coordinated Set Size.
 *
 *  \par Description:
 *       This function sends notifications if configured for Coordinated Set Size.
 *
 *  \param [in] d
 *         If NULL, Ntf to all connected CSIP sessions will be sent.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] ch
 *         CSIS Handle Identifier.
 *
 *  \param [in] s
 *         Data to be sent as part of the notification - UINT8
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_csip_error_code
 */
#define GA_cs_send_coord_set_size_ntf(d, ch, s) \
        CSIP_send_ntf((d), (ch), GA_CHAR_CSIS_SIZE, &(s), sizeof(UINT8))

/** \} */
/** \} */
/** \} */
/** \} */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */

#endif /* _H_GA_API_ */

