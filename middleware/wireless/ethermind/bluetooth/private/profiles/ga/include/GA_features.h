
/**
 *  \file GA_features.h
 *
 *  \brief This file defines all the features of the GA module.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_FEATURES_
#define _H_GA_FEATURES_

/* ----------------------------------------------------------------------- */
/* ============== GA Profiles Flags ====================================== */
/* ----------------------------------------------------------------------- */
/*
 *  GA_BAP
 *
 *  This flag enables support for the Basic Audio Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_BAP

/*
 *  GA_VCP
 *
 *  This flag enables support for the Volume Control Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_VCP

/*
 *  GA_MICP
 *
 *  This flag enables support for the Microphone Control Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_MICP

/*
 *  GA_CSIP
 *
 *  This flag enables support for the Coordinated Set Identification Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_CSIP

/*
 *  GA_CCP
 *
 *  This flag enables support for the Call Control Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_CCP

/*
 *  GA_MCP
 *
 *  This flag enables support for the Media Control Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_MCP

/*
 *  GA_HAP
 *
 *  This flag enables support for the Hearing Access Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_HAP

/*
 *  GA_TMAP
 *
 *  This flag enables support for the Telephony and Media Audio Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_TMAP

/*
 *  GA_PBP
 *
 *  This flag enables support for the Public Broadcast Profile
 *
 *  Dependency: BT_GAM
 */
#define GA_PBP

/* ----------------------------------------------------------------------- */
/* ============== BAP Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_BAP
/*
 *  BAP_UCC
 *
 *  This flag enables the Unicast Client role of BAP
 *
 *  Dependency: GA_BAP
 */
#define BAP_UCC

/*
 *  BAP_UCS
 *
 *  This flag enables the Unicast Server role of BAP
 *
 *  Dependency: GA_BAP
 */
#define BAP_UCS

/*
 *  BAP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in BAP services
 *
 *  Dependency: GA_BAP
 */
/* #define BAP_SUPPORT_CONFIG_SELECTION */

/*
 *  BAP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of BAP Services.
 *
 *  Dependency: GA_BAP
 */
/* #define BAP_SUPPORT_CONTEXT_MANAGE */

/*
 *  BAP_PACS_DEREGISTER
 *
 *  This flag enables the interface to deregister the audio capabilities
 *
 *  Dependency: GA_PACS_DEREGISTER
 */
/* #define BAP_PACS_DEREGISTER */

/*
 *  BAP_BROADCAST
 *
 *  This flag enables the broadcast feature support in GA
 *
 *  Dependency: GA_BAP
 */
#define BAP_BROADCAST

#ifdef BAP_BROADCAST
/*
 *  BAP_BROADCAST_SOURCE
 *
 *  This flag enables the broadcast feature support in GA
 *
 *  Dependency: BAP_BROADCAST
 */
#define BAP_BROADCAST_SOURCE

/*
 *  BAP_BROADCAST_SINK
 *
 *  This flag enables the broadcast feature support in GA
 *
 *  Dependency: BAP_BROADCAST
 */
#define BAP_BROADCAST_SINK

/*
 *  BAP_BROADCAST_ASSISTANT
 *
 *  This flag enables the broadcast assistant feature support in GA
 *
 *  Dependency: BAP_BROADCAST
 */
#define BAP_BROADCAST_ASSISTANT

/*
 *  BAP_BROADCAST_SCAN_DELEGATOR
 *
 *  This flag enables the broadcast assistant feature support in GA.
 *  Mandatory if Broadcast Sink support is enabled.
 *
 *  Dependency: BAP_BROADCAST
 */
#define BAP_BROADCAST_SCAN_DELEGATOR

#if ((defined BAP_BROADCAST_SINK) && (!defined BAP_BROADCAST_SCAN_DELEGATOR))
#error "Mandatory to support BAP_BROADCAST_SCAN_DELEGATOR with BAP_BROADCAST_SINK"
#endif /* ((defined BAP_BROADCAST_SINK) && (!defined BAP_BROADCAST_SCAN_DELEGATOR)) */
#endif /* BAP_BROADCAST */

#ifdef BAP_BROADCAST_SOURCE
/*
 *  GA_SUPPORT_BC_ANNOUNCEMENT_END_IN_STREAMING
 *
 *  This flag enables the GA to support the possiblity to end
 *  announcements during streaming. This would be non-compliant to the
 *  specification statemachine. Hence disabled by default.
 *
 *  Dependency: BAP_BROADCAST_SOURCE
 */
/* #define GA_SUPPORT_BC_ANNOUNCEMENT_END_IN_STREAMING */
#endif /* BAP_BROADCAST_SOURCE */

/*
 *  GA_BASS_CLIENT, GA_BASS_SERVER
 *
 *  This flag enables the Broadcast Assistant and Scan delegator
 *  interfaces.
 *
 *  Dependency: BAP_BROADCAST
 */
#define GA_BASS_CLIENT
#define GA_BASS_SERVER

/*
 *  BASS_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in BASS service
 *
 *  Dependency: BAP_BROADCAST
 */
/* #define BASS_SUPPORT_CONFIG_SELECTION */

/*
 *  BASS_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of BASS Service.
 *
 *  Dependency: BAP_BROADCAST
 */
/* #define BASS_SUPPORT_CONTEXT_MANAGE */

/*
 *  BASS_DB
 *
 *  This flag enables the DB for BASS Server.
 *
 *  Dependency: GA_BASS_SERVER
 */
/* #define BASS_DB */

#endif /* GA_BAP */

/* ----------------------------------------------------------------------- */
/* ============== VCP Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_VCP
/*
 *  VCP_CONTROLLER
 *
 *  This flag enables the Controller role of VCP
 *
 *  Dependency: GA_VCP
 */
#define VCP_CONTROLLER

/*
 *  VCP_RENDERER
 *
 *  This flag enables the Renderer role of VCP
 *
 *  Dependency: GA_VCP
 */
#define VCP_RENDERER

/*
 *  VCP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in VCP services
 *
 *  Dependency: GA_VCP
 */
/* #define VCP_SUPPORT_CONFIG_SELECTION */

/*
 *  VCP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of VCP Services.
 *
 *  Dependency: VCP_CONTROLLER
 */
/* #define VCP_SUPPORT_CONTEXT_MANAGE */
#endif /* GA_VCP */

/* ----------------------------------------------------------------------- */
/* ============== MICP Specific Flags ==================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_MICP
/*
 *  MICP_CONTROLLER
 *
 *  This flag enables the Controller role of MICP
 *
 *  Dependency: GA_MICP
 */
#define MICP_CONTROLLER

/*
 *  MICP_DEVICE
 *
 *  This flag enables the Device role of MICP
 *
 *  Dependency: GA_MICP
 */
#define MICP_DEVICE

/*
 *  MICP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in MICP services
 *
 *  Dependency: GA_MICP
 */
/* #define MICP_SUPPORT_CONFIG_SELECTION */

/*
 *  MICP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of MICP Services.
 *
 *  Dependency: MICP_CONTROLLER
 */
/* #define MICP_SUPPORT_CONTEXT_MANAGE */
#endif /* GA_MICP */

/* ----------------------------------------------------------------------- */
/* ============== CSIP Specific Flags ==================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_CSIP
/*
 *  CSIP_COORDINATOR
 *
 *  This flag enables the Set Coordinator role of CSIP
 *
 *  Dependency: GA_CSIP
 */
#define CSIP_COORDINATOR

/*
 *  CSIP_MEMBER
 *
 *  This flag enables the Set Member role of CSIP
 *
 *  Dependency: GA_CSIP
 */
#define CSIP_MEMBER

 /*
 *  CSIP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in CSIP services
 *
 *  Dependency: GA_CSIP
 */
/* #define CSIP_SUPPORT_CONFIG_SELECTION */

/*
 *  CSIP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of CSIP Services.
 *
 *  Dependency: GA_CSIP
 */
/* #define CSIP_SUPPORT_CONTEXT_MANAGE */
#endif /* GA_CSIP */

/* ----------------------------------------------------------------------- */
/* ============== CCP Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_CCP
/*
 *  CCP_CLIENT
 *
 *  This flag enables the Client role of CCP
 *
 *  Dependency: GA_CCP
 */
#define CCP_CLIENT

/*
 *  CCP_SERVER
 *
 *  This flag enables the Server role of CCP
 *
 *  Dependency: GA_CCP
 */
#define CCP_SERVER

/*
 *  CCP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in CCP services
 *
 *  Dependency: GA_CCP
 */
/* #define CCP_SUPPORT_CONFIG_SELECTION */

/*
 *  CCP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of MCP Services.
 *
 *  Dependency: CCP_CLIENT
 */
/* #define CCP_SUPPORT_CONTEXT_MANAGE */
#endif /* GA_CCP */

/* ----------------------------------------------------------------------- */
/* ============== MCP Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_MCP
/*
 *  MCP_CLIENT
 *
 *  This flag enables the Client role of MCP
 *
 *  Dependency: GA_MCP
 */
#define MCP_CLIENT

/*
 *  MCP_SERVER
 *
 *  This flag enables the Server role of MCP
 *
 *  Dependency: GA_MCP
 */
#define MCP_SERVER

/*
 *  MCP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in MCP services
 *
 *  Dependency: GA_MCP
 */
/* #define MCP_SUPPORT_CONFIG_SELECTION */

/*
 *  MCP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of MCP Services.
 *
 *  Dependency: MCP_CLIENT
 */
/* #define MCP_SUPPORT_CONTEXT_MANAGE */

/*
 *  MCP_SUPPORT_OBJECT_TRANSFER
 *
 *  This flag enables the Object Transsfer handling support in MCP
 *
 *  Dependency: GA_MCP
 */
/* #define MCP_SUPPORT_OBJECT_TRANSFER */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/*
 *  OTP_CLIENT
 *
 *  This flag enables the Object Transfer Client support in GA
 *
 *  Dependency: MCP_SUPPORT_OBJECT_TRANSFER
 */
#define OTP_CLIENT

/*
 *  OTP_SERVER
 *
 *  This flag enables the Object Transsfer Server support in GA
 *
 *  Dependency: MCP_SUPPORT_OBJECT_TRANSFER
 */
#define OTP_SERVER
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
#endif /* GA_MCP */

/* ----------------------------------------------------------------------- */
/* ============== HAP Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_HAP
/*
 *  HAP_CLIENT
 *
 *  This flag enables the Client role of HAP
 *
 *  Dependency: GA_HAP
 */
#define HAP_CLIENT

/*
 *  HAP_SUPPORT_CONFIG_SELECTION
 *
 *  This flag enables application interface to configure the
 *  notification selection of the characteristics in HAP services
 *
 *  Dependency: GA_HAP
 */
/* #define HAP_SUPPORT_CONFIG_SELECTION */

/*
 *  HAP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of HAP Services.
 *
 *  Dependency: GA_HAP
 */
/* #define HAP_SUPPORT_CONTEXT_MANAGE */

/*
 *  HAP_SERVER
 *
 *  This flag enables the Server role of HAP
 *
 *  Dependency: GA_HAP
 */
#define HAP_SERVER
#endif /* GA_HAP */

/* ----------------------------------------------------------------------- */
/* ============== TMAP Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef GA_TMAP
/*
 *  TMAP_CLIENT
 *
 *  This flag enables the Client role of TMAP
 *
 *  Dependency: GA_TMAP
 */
#define TMAP_CLIENT

/*
 *  TMAP_SUPPORT_CONTEXT_MANAGE
 *
 *  This flag enables application interface to Retrieve and Save the
 *  context of TMAP Services.
 *
 *  Dependency: GA_TMAP
 */
/* #define TMAP_SUPPORT_CONTEXT_MANAGE */

/*
 *  TMAP_SERVER
 *
 *  This flag enables the Server role of TMAP
 *
 *  Dependency: GA_TMAP
 */
#define TMAP_SERVER
#endif /* GA_TMAP */

/* ----------------------------------------------------------------------- */
/* ============== Common GA SDK Flags ==================================== */
/* ----------------------------------------------------------------------- */
/*
 *  GA_HAVE_DYNAMIC_CONFIG
 *
 *  This flag enables the dynamic configuration of GA at runtime
 *
 *  Dependency: None
 */
#define GA_HAVE_DYNAMIC_CONFIG

/* ----------------------------------------------------------------------- */
/* ==== Debug Specification Flags ======================================== */
/* ----------------------------------------------------------------------- */

/**
 *  Flag to control debug logging at run time.
 *  Debug level can be set using GA_set_debug_level() API.
 *  In addition there are interfaces to enable/disable module specific flag at run time.
 */
#define GA_ENABLE_DISABLE_RUNTIME_DEBUG

/* Profile Modules */

/* #define  BRR_NO_DEBUG */
#define  BRR_DEBUG

/* #define  BAP_NO_DEBUG */
#define  BAP_DEBUG

/* #define  BASS_NO_DEBUG */
#define  BASS_DEBUG

/* #define  CAP_NO_DEBUG */
#define  CAP_DEBUG

/* #define  CSIP_NO_DEBUG */
#define  CSIP_DEBUG

/* #define  MICP_NO_DEBUG */
#define  MICP_DEBUG

/* #define  VCP_NO_DEBUG */
#define  VCP_DEBUG

/* #define  MCP_NO_DEBUG */
#define  MCP_DEBUG

/* #define  CCP_NO_DEBUG */
#define  CCP_DEBUG

/* #define  HAP_NO_DEBUG */
#define  HAP_DEBUG

/* #define  TMAP_NO_DEBUG */
#define  TMAP_DEBUG

#endif /* _H_GA_FEATURES_ */
