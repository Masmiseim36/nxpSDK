
/**
 *  \file appl_ga_features.h
 *
 *  \brief This file defines all the application features of the GA module.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_FEATURES_
#define _H_APPL_GA_FEATURES_

/* --------------------------------------------- Header File Inclusion */

/* ----------------------------------------------------------------------- */
/* =============== APPL GA Feature Flags ================================= */
/* ----------------------------------------------------------------------- */
#define APPL_GA_LEGACY_SUPPORT

#define APPL_GA_USE_CONFIG_FILE_FOR_INPUT

#define AUDIO_SRC_ISO_SUPPORT
#define AUDIO_SNK_ISO_SUPPORT

#define AUDIO_SRC_PL_SUPPORT
#define AUDIO_SNK_PL_SUPPORT

#define AUDIO_SRC_LC3_SUPPORT
#define AUDIO_SNK_LC3_SUPPORT

#define AUDIO_SRC_LC3_CHANNELS_MAX 1U
#define AUDIO_SNK_LC3_CHANNELS_MAX 1U

#define SRC_DUMP_AUDIO_PREENCODED
#define SRC_DUMP_AUDIO_ENCODED
#define SNK_DUMP_AUDIO_PREDECODED
#define SNK_DUMP_AUDIO_DECODED

#ifdef BT_GAM
#define APPL_BAP_BROADCAST_SOURCE
#if ((defined APPL_BAP_BROADCAST_SOURCE) && (!defined BAP_BROADCAST_SOURCE))
#error "To support APPL_BAP_BROADCAST_SOURCE, it's mandatory to support BAP_BROADCAST_SOURCE"
#endif /* ((defined APPL_BAP_BROADCAST_SOURCE) && (!defined BAP_BROADCAST_SOURCE)) */

#define APPL_BAP_BROADCAST_SINK
#if ((defined APPL_BAP_BROADCAST_SINK) && (!defined BAP_BROADCAST_SINK))
#error "To support APPL_BAP_BROADCAST_SINK, it's mandatory to support BAP_BROADCAST_SINK"
#endif /* ((defined APPL_BAP_BROADCAST_SINK) && (!defined BAP_BROADCAST_SINK)) */

#define APPL_BAP_BROADCAST_ASSISTANT
#if ((defined APPL_BAP_BROADCAST_ASSISTANT) && (!defined BAP_BROADCAST_ASSISTANT))
#error "To support APPL_BAP_BROADCAST_ASSISTANT, it's mandatory to support BAP_BROADCAST_ASSISTANT"
#endif /* ((defined APPL_BAP_BROADCAST_ASSISTANT) && (!defined BAP_BROADCAST_ASSISTANT)) */

#define APPL_BAP_BROADCAST_SCAN_DELEGATOR
#if ((defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) && (!defined BAP_BROADCAST_SCAN_DELEGATOR))
#error "To support APPL_BAP_BROADCAST_SCAN_DELEGATOR, it's mandatory to support BAP_BROADCAST_SCAN_DELEGATOR"
#endif /* ((defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) && (!defined BAP_BROADCAST_SCAN_DELEGATOR)) */

#define APPL_VCP_CONTROLLER
#if ((defined APPL_VCP_CONTROLLER) && (!defined VCP_CONTROLLER))
#error "To support APPL_VCP_CONTROLLER, it's mandatory to support VCP_CONTROLLER"
#endif /* ((defined APPL_VCP_CONTROLLER) && (!defined VCP_CONTROLLER)) */

#define APPL_VCP_RENDERER
#if ((defined APPL_VCP_RENDERER) && (!defined VCP_RENDERER))
#error "To support APPL_VCP_RENDERER, it's mandatory to support VCP_RENDERER"
#endif /* ((defined APPL_VCP_RENDERER) && (!defined VCP_RENDERER)) */

#define APPL_MICP_CONTROLLER
#if ((defined APPL_MICP_CONTROLLER) && (!defined MICP_CONTROLLER))
#error "To support APPL_MICP_CONTROLLER, it's mandatory to support MICP_CONTROLLER"
#endif /* ((defined APPL_MICP_CONTROLLER) && (!defined MICP_CONTROLLER)) */

#define APPL_MICP_DEVICE
#if ((defined APPL_MICP_DEVICE) && (!defined MICP_DEVICE))
#error "To support APPL_MICP_DEVICE, it's mandatory to support MICP_DEVICE"
#endif /* ((defined APPL_MICP_DEVICE) && (!defined MICP_DEVICE)) */

#define APPL_GA_CSIP
#if ((defined APPL_GA_CSIP) && (!defined GA_CSIP))
#error "To support APPL_GA_CSIP, it's mandatory to support GA_CSIP"
#endif /* ((defined APPL_GA_CSIP) && (!defined GA_CSIP)) */

#define APPL_CS_MEMBER

#define APPL_CCP_CLIENT
#if ((defined APPL_CCP_CLIENT) && (!defined CCP_CLIENT))
#error "To support APPL_CCP_CLIENT, it's mandatory to support CCP_CLIENT"
#endif /* ((defined APPL_CCP_CLIENT) && (!defined CCP_CLIENT)) */

#define APPL_CCP_SERVER
#if ((defined APPL_CCP_SERVER) && (!defined CCP_SERVER))
#error "To support APPL_CCP_SERVER, it's mandatory to support CCP_SERVER"
#endif /* ((defined APPL_CCP_SERVER) && (!defined CCP_SERVER)) */

#define APPL_MCP_CLIENT
#if ((defined APPL_MCP_CLIENT) && (!defined MCP_CLIENT))
#error "To support APPL_MCP_CLIENT, it's mandatory to support MCP_CLIENT"
#endif /* ((defined APPL_MCP_CLIENT) && (!defined MCP_CLIENT)) */

#define APPL_MCP_SERVER
#if ((defined APPL_MCP_SERVER) && (!defined MCP_SERVER))
#error "To support APPL_MCP_SERVER, it's mandatory to support MCP_SERVER"
#endif /* ((defined APPL_MCP_SERVER) && (!defined MCP_SERVER)) */
#endif /* BT_GAM */

#if 0
#if ((defined AUDIO_SRC_LC3_SUPPORT) && (AUDIO_SRC_LC3_CHANNELS_MAX > LC3_CHANNELS_MAX))
#error "EtherMind LC3 does not support AUDIO_SRC_LC3_CHANNELS_MAX"
#endif /* ((defined INCLUDE_ETHERMIND_LC3) && (AUDIO_SRC_LC3_CHANNELS_MAX > LC3_CHANNELS_MAX) */

#if ((defined AUDIO_SNK_LC3_SUPPORT) && (AUDIO_SNK_LC3_CHANNELS_MAX > LC3_CHANNELS_MAX))
#error "EtherMind LC3 does not support AUDIO_SNK_LC3_CHANNELS_MAX"
#endif /* ((defined INCLUDE_ETHERMIND_LC3) && (AUDIO_SRC_LC3_CHANNELS_MAX > LC3_CHANNELS_MAX) */
#endif /* 0 */
#endif /* _H_APPL_GA_FEATURES_ */