
/**
 *  \file GA_limits.h
 *
 *  \brief This file defines all the tunable constants of the GA module
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_LIMITS_
#define _H_GA_LIMITS_

/**
 * \addtogroup bt_ga
 * \{
 */

/* --------------------------------------------- Header File Inclusion */
/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

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
 * \name GA Limits
 * \{
 * \brief This section describes the Limits defined for GA.
 */

/**
 *  This parameter defines the maximum number of capability records
 *  that can be registered by the application as an Audio Source.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_SOURCE_PAC_RECORDS                       1U

/**
 *  This parameter defines the maximum number of capability records
 *  that can be registered by the application as an Audio Sink.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_SINK_PAC_RECORDS                         1U

/**
 *  This parameter defines the maximum number of Audio Stream Endpoints
 *  that can be registered by the application.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_SEP                                      2U

/**
 *  This parameter defines the maximum number of GA contexts that
 *  can be setup by the application. There can be one context per
 *  connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_CONTEXTS                                 1U

/**
 *  This parameter defines the maximum number of source stream contexts that
 *  can be setup by the application. There can be multiple stream contexts
 *  setup per connected peer device. The number of stream contexts used
 *  depends on the number of ASEs exposed by the peer and is allocated
 *  during the ASE discovery procedure.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_SOURCE_STREAM_CONTEXTS                   2U

/**
 *  This parameter defines the maximum number of sink stream contexts that
 *  can be setup by the application. There can be multiple stream contexts
 *  setup per connected peer device. The number of stream contexts used
 *  depends on the number of ASEs exposed by the peer and is allocated
 *  during the ASE discovery procedure.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_SINK_STREAM_CONTEXTS                     2U

/**
 *  This parameter defines the maximum number of Content Control Services
 *  can be registered by the application.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define GA_MAX_CCS                                      2U

/**
 *  This parameter defines the maximum Codec Information Element
 *  length supported. This value will hold the maximum of all
 *  the IE lengths of supported codecs.
 *
 *  Minimum Value: 4 \n
 *  Maximum Value: As required \n
 *  Dynamic: No \n
 */
#define GA_CODEC_IE_LEN                                 24U

/**
 *  This parameter defines the maximum Metadata length
 *  configuration. This value depends on the maximum LTV
 *  sets that would get used in any procedure.
 *
 *  Minimum Value: 4 \n
 *  Maximum Value: As required \n
 *  Dynamic: No \n
 */
#define GA_METADATA_LEN                                 24U

/**
 *  This parameter defines the number of broadcast sessions supported
 *  by the GA stack. This would depend on the number of Extended
 *  Advertising sets supported by the Bluetooth controller.
 *
 *  Minimum Value: 1
 *  Maximum Value: Number of Advertising sets supported by controller
 *  Dynamic: Yes
 */
#define GA_MAX_BROADCAST_SESSIONS                       1U

/**
 *  This parameter defines the number of broadcast source end-points
 *  supported by the GA stack.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required
 *  Dynamic: Yes
 */
#define GA_MAX_BROADCAST_SEP                            1U

/**
 *  This parameter defines the number of broadcast sink sessions supported
 *  by the GA stack. This would depend on the number of Syncs supported
 *  by the controller.
 *
 *  Minimum Value: 1
 *  Maximum Value: Number of Syncs supported by controller
 *  Dynamic: Yes
 */
#define GA_MAX_BROADCAST_SINK_SESSIONS                  1U

/**
 *  This parameter defines the Lock timeout for the CAP acceptor
 *  as a coordinated set member
 *
 *  Default Value: As required.
 *  Dynamic: Yes
 */
#define GA_CAP_CSM_LOCK_TIMEOUT                         3U

/**
 *  This parameter defines the maximum number of Volume Control Contexts
 *  as Volume Controller that can be setup by the application. There can
 *  be only one context setup per connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define VCP_MAX_VCS_CONTEXTS                            1U

/**
 *  This parameter defines the maximum number of Volume Offset Control Contexts
 *  as Volume Controller that can be setup by the application. There can be
 *  multiple contexts setup per connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define VCP_MAX_VOCS_CONTEXTS                           1U

/**
 *  This parameter defines the maximum number of Audio Input Control Contexts
 *  as Volume Controller that can be setup by the application. There can be
 *  multiple contexts setup per connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define VCP_MAX_AICS_CONTEXTS                           1U

/**
 *  This parameter defines the maximum number of Volume Offset Control Service
 *  as Volume Renderer that can be registered by the application.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define VCP_MAX_VOCS_ENTITIES                           1U

/**
 *  This parameter defines the maximum number of Audio Input Control Service
 *  as Volume Renderer that can be registered by the application.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define VCP_MAX_AICS_ENTITIES                           1U

/**
 *  This parameter defines the maximum number of Microphone Control Contexts
 *  as Microphone Controller that can be setup by the application. There can be
 *  only one context setup per connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define MICP_MAX_MICS_CONTEXTS                          1U

/**
 *  This parameter defines the maximum number of Audio Input Control Contexts
 *  as Microphone Controller that can be setup by the application. There can be
 *  multiple contexts setup per connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define MICP_MAX_AICS_CONTEXTS                          1U

/**
 *  This parameter defines the maximum number of Audio Input Control Service
 *  as Microphone Device that can be registered by the application.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define MICP_MAX_AICS_ENTITIES                          2U

/**
 *  This parameter defines the maximum number Coordinated Set Contexts
 *  that can be setup by the application. There can be multiple contexts
 *  that can be setup with a connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define CSIP_MAX_CONTEXTS                               1U

/**
 *  This parameter defines the maximum number of Coordinated Set Member
 *  entities that can be registered by the application.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define CSIP_MAX_CSIS_ENTITIES                          1U

/**
 *  This parameter defines the maximum number Generic Telephony Bearer contexts
 *  that can be setup by the Call Control Client application. There can be only
 *  one context that can be setup with a connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define CCP_MAX_CONTEXTS                                2U

/**
 *  This parameter defines the maximum number of Telephony Bearer contexts
 *  that can be setup by the Call Control Client application. There can be
 *  multiple contexts that can be setup with a connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define CCP_MAX_TBS_CONTEXTS                            2U

/**
 *  This parameter defines the maximum number of Telephony Bearer Service
 *  endpoints that can be registered by the Call Control Server application.
 *  This does not include the Generic Telephony Bearer Service endpoint which is
 *  a default component on initialization.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define CCP_MAX_TBS_ENTITIES                            2U

/**
 *  This parameter defines the maximum number of Generic Media Control contexts
 *  that can be setup by the Media Control Client application. There can be
 *  only one context that can be setup with a connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define MCP_MAX_CONTEXTS                                2U

/**
 *  This parameter defines the maximum number of Media Control contexts
 *  that can be setup by the Media Control Client application. There can
 *  be multiple contexts that can be setup with a connected peer device.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define MCP_MAX_MCS_CONTEXTS                            2U

/**
 *  This parameter defines the maximum number of Media Control Service
 *  endpoints that can be registered by the Media Control Server application.
 *  This does not include the Generic Media Control Service endpoint which is
 *  a default component on initialization.
 *
 *  Minimum Value: 1 \n
 *  Maximum Value: As required \n
 *  Dynamic: Yes \n
 */
#define MCP_MAX_MCS_ENTITIES                            2U

/**
 *  This parameter defines the maximum number of Hearing Aid contexts
 *  that can be setup by the Hearing Aid Client application. There can be
 *  only one context that can be setup with a connected peer device.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required
 *  Dynamic: Yes
 */
#define HAP_MAX_HAS_CONTEXTS                            1U

/**
 *  This parameter defines the maximum number of Telephony and Media Audio
 *  contexts that can be setup by the Telephony and Media Audio Client
 *  application. There can be only one context that can be setup with a
 *  connected peer device.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required
 *  Dynamic: Yes
 */
#define TMAP_MAX_TMAS_CONTEXTS                          1U


/**
 * Maximum number of simultaneous BLE connections that BASS CE supports.
 */
#define BASS_CE_MAX_INSTANCES                           5U

/**
 * Maximum number of BASS Broadcast Receive State Instances [inclusive of all device].
 */
#define BASS_CE_MAX_BC_RX_STATE_INSTANCES               5U

/**
 * Maximum number of BASS Broadcast Receive State Entries supported by Local Server.
 */
#define BASS_SE_MAX_BC_RX_STATE_ENTITIES                3U

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

#endif /* _H_GA_LIMITS_ */

