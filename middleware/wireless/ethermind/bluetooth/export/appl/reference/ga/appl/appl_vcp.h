/**
 *  \file appl_vcp.h
 *
 *  \brief This file defines the Volume Control Profile:
 *  Application Data Structures and Methods.
 *  This application is used to enable a device to control the audio volume
 *  of a device that exposes the Volume Control Service and to obtain the
 *  volume status of that device.
 *  This application also allows to perform the procedures used to control
 *  and obtain the status of the Volume Offset Control Service (VOCS)
 *  and Audio Input Control Service (AICS) if supported.
 *  Roles: VCP Controller: Device that controls the volume and related state
 *                         of this audio.
 *         VCP Renderer: Device that receives one or more audio inputs and
 *         exhibits control over an audio output device.
 *  GATT Requirement: VCP Controller: GATT Client
 *                    VCP Renderer: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_VCP_
#define _H_APPL_VCP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_aics.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Macros */

/* Sample Audio Input/Output Descriptions */
#define APPL_VCS_VOCS_OUTPUT_DESC                         "VCS_VOCS_SPKR_0"
#define APPL_VCS_AICS_INPUT_DESC                          "VCS_AICS_MIC_0"

#define APPL_VCP_MAX_VOCS_CHAR_AUDIO_OUTPUT_DESC_LEN      64U

#ifdef VCP_RENDERER
#define APPL_VCP_RD_VOL_STEP_SIZE                         2U
#endif /* VCP_RENDERER */

/* --------------------------------------------- Data types /Structures */

/**
 * Volume Control Service Volume State Characteristic Fields.
 */
typedef struct _APPL_VCP_VCS_CHAR_VOL_STATE_PARAMS
{
    /**
     * Volume Setting
     * Range: 0 - 255
     */
    UCHAR volume_setting;

    /**
     * Mute
     *  0 -> UnMute
     *  1 -> Mute
     */
    UCHAR mute;

    /**
     * Change Counter
     * Range: 0 - 255
     */
    UCHAR change_counter;
}APPL_VCP_VCS_CHAR_VOL_STATE_PARAMS;

/**
 * Volume Control Service Characteristics Info.
 */
typedef struct _APPL_VCP_VCS_CHAR_INFO
{
    /**
     * Volume State
     */
    APPL_VCP_VCS_CHAR_VOL_STATE_PARAMS vol_state;

    /**
     * Volume Flags
     */
    UCHAR volume_flags;
}APPL_VCP_VCS_CHAR_INFO;

/**
 * VOCS: Offset State Characteristic Fields.
 */
typedef struct _APPL_VCP_VOCS_CHAR_OFFSET_STATE_PARAMS
{
    /**
     * Volume Offset
     */
     INT16 vol_offset;

    /**
     * Change Counter
     * Range: 0 - 255
     */
    UCHAR change_counter;
}APPL_VCP_VOCS_CHAR_OFFSET_STATE_PARAMS;

/**
 * Volume Offset Control Service Characteristics Info.
 */
typedef struct _APPL_VCP_VOCS_CHAR_INFO
{
    /**
     * Offset State
     */
    APPL_VCP_VOCS_CHAR_OFFSET_STATE_PARAMS * offset_state;

    /**
     * Audio Location
     * TODO: this is Assign number relook the data type.
     */
    UINT32 audio_loc;

    /**
     * Audio Output Description
     * TODO: Make as Pointer?
     */
    UCHAR audio_out_desc[APPL_VCP_MAX_VOCS_CHAR_AUDIO_OUTPUT_DESC_LEN];

    /**
     * Audio Output Description Length
     */
    UCHAR audio_out_desc_len;

}APPL_VCP_VOCS_CHAR_INFO;

/** Audio Input Control Notification Info */
typedef struct _APPL_VCP_NTF_INFO
{
    /** Service instance */
    UCHAR srvc_inst;

    /** Peer device */
    GA_ENDPOINT device;

}APPL_VCP_NTF_INFO;

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- APIs */
/*** APIs - VCP Controller ***/

void appl_vcp_ct_set_default_values(void);

int appl_vcp_ct_operations(void);

GA_RESULT appl_vcp_ct_callback
          (
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          );

/*** APIs - VCP Renderer ***/

GA_RESULT appl_vcp_rd_reg_opt_service
          (
              UCHAR srvc_type
          );
void appl_vcp_rd_set_default_values(void);
void appl_vcp_rd_cp_write_timer_handler
     (
         void   * data,
         UINT16   datalen
     );

void appl_vcp_vocs_rd_cp_write_timer_handler
     (
         void   * data,
         UINT16   datalen
     );

void appl_vcp_vocs_rd_set_audio_loc_timer_handler
     (
         void   * data,
         UINT16  datalen
     );

void appl_vcp_vocs_rd_set_audio_out_desc_timer_handler
     (
         void   * data,
         UINT16  datalen
     );

void appl_vcp_aics_rd_cp_write_timer_handler
     (
         void   * data,
         UINT16   datalen
     );

void appl_vcp_aics_rd_set_audio_inp_desc_timer_handler
     (
         void   * data,
         UINT16   datalen
     );

int appl_vcp_rd_operations(void);
void appl_vcp_rd_set_vcs_sdp_record_status
     (
         UCHAR flag
     );
void appl_vcp_rd_set_vocs_sdp_record_status
     (
         UCHAR flag
     );
void appl_vcp_rd_set_aics_sdp_record_status
     (
         UCHAR flag
     );
GA_RESULT appl_vcp_rd_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );
GA_RESULT appl_vcp_rd_vcs_cp_write_handler
          (
              VCP_EVENT_INFO * vcp_event_info
          );
GA_RESULT appl_vcp_rd_vocs_cp_write_handler
          (
              VCP_EVENT_INFO * vcp_event_info
          );
GA_RESULT appl_vcp_rd_aics_cp_write_handler
          (
              VCP_EVENT_INFO * vcp_event_info
          );
GA_RESULT appl_vcp_rd_pack_vcs_chr_value
          (
              /* IN  */  UINT16                   chr_uuid,
              /* IN  */  APPL_VCP_VCS_CHAR_INFO * vcs_params,
              /* OUT */  UCHAR                  * chr_val,
              /* OUT */  UCHAR                  * chr_val_len
          );
GA_RESULT appl_vcp_rd_pack_vocs_chr_value
          (
              /* IN  */  UINT16                    chr_uuid,
              /* IN  */  APPL_VCP_VOCS_CHAR_INFO * vocs_params,
              /* OUT */  UCHAR                   * chr_val,
              /* OUT */  UCHAR                   * chr_val_len
          );
GA_RESULT appl_vcp_rd_pack_aics_chr_value
          (
              /* IN  */  UINT16                chr_uuid,
              /* IN  */  APPL_AICS_CHAR_INFO * aics_params,
              /* OUT */  UCHAR               * chr_val,
              /* OUT */  UCHAR               * chr_val_len
          );

/*** Common APIs - VCP Controller and Renderer ***/

/* VCP Server Callback */
GA_RESULT appl_vcp_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

/* VCP Client Callback */
GA_RESULT appl_vcp_client_callback
          (
              GA_ENDPOINT * ga_device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );
/* Parser to print the characteristic data of VCS */
void appl_vcp_parse_and_display_vcs_char_value
     (
         UINT16   char_uuid,
         UCHAR  * vcs_data
     );
/* Parser to print the characteristic data of VOCS */
void appl_vcp_parse_and_display_vocs_char_value
     (
         UINT16   char_uuid,
         UCHAR  * vocs_data,
         UINT16   datalen
     );

/* Parser for printing error codes */
void appl_vcp_parse_and_display_ga_result(GA_RESULT status, UINT8 service);

/* Parser for printing VCS application error codes */
void appl_vcp_parse_and_display_vcs_appl_err_codes(GA_RESULT status);

/* Parser for printing VOCS application error codes */
void appl_vcp_parse_and_display_vocs_appl_err_codes(GA_RESULT status);

#endif /* _H_APPL_VCP_ */

