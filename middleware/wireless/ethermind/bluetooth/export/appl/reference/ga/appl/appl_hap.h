/**
 *  \file appl_hap.h
 *
 *  \brief This file defines the Hearing Access/Aid Profile:
 *  Application Data Structures and Methods.
 *  This application is used to identify a hearing aid and optionally to
 *  control hearing aid presets.
 *  A hearing aid preset represents a configuration of the hearing aid signal
 *  processing parameters tailored to a specific listening situation.
 *  This application implements both HAP Client and Server Role.
 *  It shows how to discover a Hearing Aid, its features and also perform
 *  control point procedures on them.
 *  It also indicates how the remote Hearing Aid handles the requests from the
 *  Client device and responds to the same.
 *
 *  Roles: HAP Client: Device that control the Hearing Aid Presets.
 *         HAP Server: Device that receives CP procedures from HAP Client.
 *  GATT Requirement: HAP Client: GATT Client
 *                    HAP Server: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HAP_
#define _H_APPL_HAP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_hap_api.h"

/* --------------------------------------------- Macros */
#define APPL_HAP_MAX_SUPP_PRESET_RECORDS             5U
#define APPL_HAP_MAX_PRESET_RECORD_NAME              40U
#define APPL_HAP_PRESET_RECORD_INDEX_INVALID         0x00
#define APPL_HAP_PRESET_RECORD_IS_AVAILABLE_BIT_LOC  0x02

/* --------------------------------------------- Data types /Structures */

typedef struct _APPL_HAS_PRESET_RECORD_
{
    /* Preset Record - Index */
    UINT8 index;

    /* Preset Record - Properties */
    UINT8 properties;

    /* Preset Record - Name Length */
    UINT8 name_len;

    /* Preset Record - Name */
    UCHAR name[40U];

}APPL_HAS_PRESET_RECORD;

typedef struct _APPL_HAS_CHAR_INFO_
{
    /**
     * Hearing Aid Features
     */
    HAP_FEATURES features;

    APPL_HAS_PRESET_RECORD preset_records[APPL_HAP_MAX_SUPP_PRESET_RECORDS];

    UINT8 num_of_preset_records;

    /**
     * Active Preset Index
     */
    UINT8 active_preset_index;
}APPL_HAS_CHAR_INFO;
/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- APIs */

void main_hap_menu_operations(void);

GA_RESULT appl_hap_cb
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ void        * ctx,
              /* IN */ HAP_EVT_ID    evt_id,
              /* IN */ UINT16        evt_status,
              /* IN */ HAP_EVT     * evt_data
          );

void appl_hap_parse_features(HAP_FEATURES features);

void appl_hap_fetch_preset_record_properties(UINT8 * prop);

void appl_hap_parse_ga_result(GA_RESULT status);

void appl_hap_parse_and_display_is_last(UCHAR is_last);

void appl_hap_parse_and_display_preset_record_properties
     (
         UCHAR preset_record_properties
     );

#ifdef HAP_CLIENT
GA_RESULT appl_hap_ce_cb
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ void        * ctx,
              /* IN */ HAP_EVT_ID    evt_id,
              /* IN */ UINT16        evt_status,
              /* IN */ HAP_EVT     * evt_data
          );

GA_RESULT appl_hap_parse_hap_cp(UCHAR * data, UINT16 len);

void appl_hap_parse_change_id(UINT8 changeId);

void appl_hap_parse_and_display_preset_record(UCHAR * data, UINT16 len);
#endif /* HAP_CLIENT */

#ifdef HAP_SERVER

void appl_hap_ha_initialize_records(void);

void appl_hap_ha_register_preset_records(void);

void appl_hap_ha_create_preset_records(void);

GA_RESULT appl_hap_se_cb
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ void        * ctx,
              /* IN */ HAP_EVT_ID    evt_id,
              /* IN */ UINT16        evt_status,
              /* IN */ HAP_EVT     * evt_data
          );

GA_RESULT appl_hap_ha_preset_cp_handler
          (
              GA_ENDPOINT * device,
              HAP_EVT_ID    evt_id,
              HAP_EVT     * evt_data
          );

void appl_hap_ha_send_ntf_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     );

GA_RESULT appl_hap_send_preset_changed_ntf(void);

void appl_has_create_preset_records_array(UCHAR i, UCHAR* data, UCHAR* len);

#endif /* HAP_SERVER */

#endif /* _H_APPL_HAP_ */

