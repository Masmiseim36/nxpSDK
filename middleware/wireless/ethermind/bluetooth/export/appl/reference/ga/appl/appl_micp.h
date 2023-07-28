/**
 *  \file appl_micp.h
 *
 *  \brief This file defines the Microphone Control Profile:
 *  Application Data Structures and Methods.
 *  This application is used to enable a Generic Attribute Profile (GATT)
 *  client to control and obtain the status of a microphone through the
 *  Microphone Control Service (MICS).
 *  This application also allows to perform the procedures used to control and
 *  obtain the status of instances of the Audio Input Control Service (AICS)
 *  if supported.
 *  Roles: Microphone Controller: Device that controls the microphone such as
 *                                a mobile phone.
 *         Microphone Device: Device that exposes controls for a microphone
 *                            such as a headset.
 *  GATT Requirement: Microphone Controller: GATT Client
 *                    Microphone Devic: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_MICP_
#define _H_APPL_MICP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_aics.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Macros */
#define APPL_MICS_AICS_INPUT_DESC                   "MICS_AICS_MIC"

/* --------------------------------------------- Data types /Structures */

/**
 * Microphone Control Service Characteristics Info.
 */
typedef struct _MICS_CHAR_INFO
{
    /**
     * Mute
     */
    UCHAR mute;
}MICS_CHAR_INFO;

/** AICS/MICS Notification Info */
typedef struct _APPL_MICP_NTF_INFO
{
    /** AICS/MICS service instance */
    UCHAR srvc_inst;

    /** Peer device */
    GA_ENDPOINT device;

}APPL_MICP_NTF_INFO;

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- APIs */

/*** APIs - Microphone Controller ***/

void appl_micp_ct_set_default_values(void);

void appl_micp_ct_operations(void);

/* MICP Controller Callback */
GA_RESULT appl_micp_ct_callback
          (
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          );

/*** APIs - Microphone Device ***/

GA_RESULT appl_micp_dev_reg_opt_service(void);

void appl_micp_dev_set_default_values(void);

void appl_micp_dev_operations(void);

void appl_micp_dev_set_aics_sdp_record_status(UCHAR flag);

/* MICP Device Callback */
GA_RESULT appl_micp_dev_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

GA_RESULT appl_micp_dev_aics_cp_write_handler
          (
              MICP_EVENT_INFO * mcd_event_info
          );

GA_RESULT appl_micp_dev_pack_aics_chr_value
          (
              /* IN  */  UINT16                chr_uuid,
              /* IN  */  APPL_AICS_CHAR_INFO * aics_params,
              /* OUT */  UCHAR               * chr_val,
              /* OUT */  UCHAR               * chr_val_len
          );

void appl_micp_mics_dev_cp_write_timer_handler(void * data, UINT16 datalen);
void appl_micp_aics_dev_cp_write_timer_handler(void * data, UINT16 datalen);
void appl_micp_aics_rd_set_audio_inp_desc_timer_handler
     (
         void  * data,
         UINT16  datalen
     );

/*** Common APIs - Microphone Controller and Device ***/

/* MICP Server Callback */
GA_RESULT appl_micp_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

/* MICP Client Callback */
GA_RESULT appl_micp_client_callback
          (
              GA_ENDPOINT * ga_device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

#endif /* _H_APPL_MICP_ */
