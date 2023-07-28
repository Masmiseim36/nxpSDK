/**
 *  \file appl_tmap.h
 *
 *  \brief This file defines the Telephony and Media Audio Profile:
 *  Application Data Structures and Methods.
 *  This application is used to enable a Generic Attribute Profile (GATT)
 *  client to setup TMAP profile roles.
 *
 *  The Call Gateway (CG): This role is defined for telephony or VoIP
 *  applications. The CG device has the connection to the call network
 *  infrastructure. Typical devices implementing the CG role include
 *  smartphones, laptops, tablets, and PCs.
 *  The Call Terminal (CT ): This role is defined for headset type devices in
 *  telephony or VoIP applications. Typical devices implementing the CT role
 *  include wireless headsets, speakers, and microphones that participate in
 *  conversational audio.
 *  The Unicast Media Sender (UMS): This role is defined for devices that send
 *  media audio content in one or more Unicast Audio Streams. Typical devices
 *  implementing the UMS role include smartphones, media players, TVs, laptops,
 *  tablets, and PCs.
 *  The Unicast Media Receiver (UMR): This role is defined for devices that
 *  receive media audio content from a source device in a Unicast Audio Stream.
 *  Typical devices implementing the UMR role include headphones, earbuds, and
 *  wireless speakers.
 *  The Broadcast Media Sender (BMS): This role is defined for devices that
 *  send media audio content to an unlimited number of receiving devices.
 *  Typical devices implementing the BMS role include smartphones, media
 *  players, TVs, laptops, tablets, and PCs.
 *  The Broadcast Media Receiver (BMR): This role is defined for devices that
 *  receive media audio content from a source device in a Broadcast Audio
 *  Stream. Typical devices implementing the BMR role include headphones,
 *  earbuds, and speakers. A smartphone may also support this role to receive
 *  Broadcast Audio Streams from a BMS.
 */
#ifndef _H_APPL_TMAP_
#define _H_APPL_TMAP_

/* --------------------------------------------- Header File Inclusion */
#include "GA_tmap_api.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

void main_tmap_menu_operations(void);

GA_RESULT appl_tmap_cb
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ TMAP_EVT_ID     evt_id,
              /* IN */ UINT16          evt_status,
              /* IN */ TMAP_EVT      * evt_data
          );

#ifdef TMAP_CLIENT
void appl_tmap_parse_role(TMAP_ROLE role);

GA_RESULT appl_tmap_ce_cb
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ TMAP_EVT_ID     evt_id,
              /* IN */ UINT16          evt_status,
              /* IN */ TMAP_EVT      * evt_data
          );
#endif /* TMAP_CLIENT */

void appl_tmap_parse_ga_result(GA_RESULT status);

#endif /* _H_APPL_TMAP_ */