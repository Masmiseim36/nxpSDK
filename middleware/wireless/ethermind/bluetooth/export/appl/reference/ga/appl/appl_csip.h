/**
 *  \file appl_csip.h
 *
 *  \brief This file defines the Coordinated Set Identification Profile:
 *  Application Data Structures and Methods.
 *  This application can be used by devices to be discovered as part of one
 *  or more Coordinated Sets.
 *  A Coordinated Set is defined as a group of devices that are configured to
 *  support a specific scenario. Examples of Coordinated Sets include
 *  a pair of hearing aids, a pair of earbuds, or a speaker set that receives
 *  multi-channel audio and that reacts to control commands in a
 *  coordinated way (e.g., volume up and volume down).
 *  The purpose of this application is to specify a mechanism to discover a
 *  Coordinated Set and its members, and to specify how a device can be
 *  discovered as part of one or more Coordinated Sets. It also specifies a
 *  way to grant exclusive access to the Coordinated Set to a client such that
 *  race conditions can be avoided when multiple clients want to access the
 *  Coordinated Set at the same time.
 *  Roles: CSIP Set Coordinator: Device that discovers a Coordinated Set and
 *                               its Set Members.
 *         CSIP Set Member: Device that is part of the Coordinated Set.
 *  GATT Requirement: CSIP Set Coordinator: GATT Client
 *                    CSIP Set Member: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CSIP_
#define _H_APPL_CSIP_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */
#define APPL_CS_SIRK_TYPE   CSIP_SIRK_TYPE_PLAIN_TEXT
#define APPL_CS_SIRK        {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

/*** APIs - CSIP Set Coordinator ***/

/* CSIP Set Coordinator Callback */
GA_RESULT appl_csip_sc_callback
          (
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          );

int appl_csip_sc_operations(void);

/*** APIs - CSIP Set Member ***/

void appl_csip_sm_set_csis_sdp_record_status(UCHAR flag);

/* CSIP Set Member Callback */
GA_RESULT appl_csip_sm_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

void appl_csip_sm_sirk_callback(UCHAR * data, UINT16 datalen);

int appl_csip_sm_operations(void);

/*** Common APIs - CSIP Set Coordinator and Set Member ***/

/* CSIP Server Callback */
GA_RESULT appl_csip_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

/* CSIP Client Callback */
GA_RESULT appl_csip_client_callback
          (
              GA_ENDPOINT * ga_device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

/* Parser for SIRK Type */
void appl_csip_parse_and_display_sirk_type_value(UINT8 sirk);

/* Parser for Set Member Lock */
void appl_csip_parse_and_display_set_mem_lock_value(UINT8 lock);

#endif /* _H_APPL_CSIP_ */
