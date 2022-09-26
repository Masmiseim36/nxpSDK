
/**
 *  \file appl_hdp.h
 *
 *  Header File for HDP Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HDP_
#define _H_APPL_HDP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"

#include "BT_hdp_api.h"
#include "db_gen.h"

/* --------------------------------------------- Global Definitions */

#define HDP_APPL_MAX_MDEP_IDS                         MCAP_MAX_MEP_INSTANCES
#define HDP_APPL_MAX_CNTRL_CHNLS                      MCAP_MAX_CONTROL_CHANNELS
#define HDP_APPL_MAX_MDL_IDS                          MCAP_MAX_DATA_CHANNELS
#define HDP_APPL_INVALID_MDEP                         0xFFU
#define HDP_APPL_INVALID_ID                           0xFFFFU

#define hdp_appl_ps_write(p,nb)                               \
        fwrite (p,sizeof (UCHAR),nb,fp)

#define hdp_appl_ps_read(p,nb)                                \
        fread (p,sizeof (UCHAR),nb,fp)

/* #define HDP_APPL_STORAGE */

/* --------------------------------------------- Structures/Data Types */

/**
 * This data type holds the role to MDEP ID's mapping
 */
typedef struct
{
    /* This variable is the id of the MCAP Data end point */
    UINT8 mdep_id;

    /* This variable holda the role(Source or Sink) played by MDEP ID */
    UINT8 mdep_role;

}HDP_MDEP;


/* MDEP Data Type Structure */
typedef struct
{
    /* MDEP Data Type - UINT16 */
    UINT16  mdep_data_type;

    /* Corresponding Health Minor Device Class - Unsigned 3 Octet value */
    UINT32  health_minor_device_class;

    /* Data Type Description - Text String */
    UCHAR  *data_type_name;

}HDP_MDEP_DATA_TYPE;


/**
 * This structure holds the HDP control channel attributes.
 */
typedef struct
{
    /**
     * This variable holds mcl_id given by the MCAP for the control channel
     */
    UINT16 mcap_mcl_id;

    /**
     * Flag to check if atleast one reliable channel is established
     * in an MCL.
     */
    UCHAR rel_data_chnl_flag;

    /* Remote device bdaddr */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Array of Data channel Id's */
    UINT16 mcap_mdl_id[HDP_APPL_MAX_MDL_IDS];

    /* Data channel Configuration */
    UINT8 mcap_mdl_config[HDP_APPL_MAX_MDL_IDS];

}HDP_APPL_CNTRL_CHNL;


/* --------------------------------------------- Macros */

#define appl_hci_print_bd_addr(bd)      appl_dump_bytes((bd), 6);
/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/* To initialize hdp application */
void hdp_appl_init (void);

/* To Update the values in the HDP Service record */
void hdp_appl_dbase_register (void);

/**
 * Application registers this callback with HDP for asyncronous event
 * notification.
 */
API_RESULT hdp_appl_cb
           (
               HDP_HANDLE *  handle,
               UINT8         event_type,
               UINT16        response,
               void *        event_params,
               UINT16        event_len
           );

/* To get a free instance of cntrl_chnls structure array */
API_RESULT hdp_appl_get_free_cntrl_chnl_id
           (
               UINT16 *appl_cntrl_chnl_id
           );

/* To get the cntrl_chnls array instance corresponding to mcap provided mcl_id */
API_RESULT hdp_appl_get_cntrl_chnl_id
           (
               UINT16    mcap_mcl_id,
               UINT16 *  appl_cntrl_chnl_id
           );

#if 0
/* To get a free instance of cntrl_chnls structure array */
API_RESULT hdp_appl_get_free_cntrl_chnl
           (
               UINT16 *  appl_cntrl_chnl_id
           );
#endif /* 0 */

/* To get the free data channel instance in the cntrl chnl instance */
API_RESULT hdp_appl_get_free_data_chnl_id
           (
               UINT16    hdp_appl_cntrl_chnl_id,
               UINT16 *  hdp_appl_data_chnl_id
           );

/**
 * This routine takes mdep_id as a parameter and return the role
 * played by the mdep_id
 */
API_RESULT hdp_appl_get_mdep_role
           (
               /* IN */ UINT8   mdep_id,
               /* OUT */ UINT8  *mdep_role
           );

/**
 * To get the data chnl array instance in the cntrl_chnl array instance
 * corresponding to mcap provided mdl_id
 */
API_RESULT hdp_appl_get_data_chnl_id
           (
               UINT16    hdp_appl_cntrl_chnl_id,
               UINT16    mcap_mdl_id,
               UINT16 *  hdp_appl_data_chnl_id
           );


/* Function to find the index of the cntrl_chnls array based on remote BD_ADDR */
API_RESULT hdp_appl_get_cntrl_chnl_bdaddr
           (
               UCHAR *   bd_addr,
               UINT16 *  hdp_appl_cntrl_chnl_id
           );


/**
 * Function return API_SUCCESS if atleast one reliable data channel is present
 * in the MCL, otherwise API_FAILURE */
API_RESULT hdp_appl_check_rel_data_chnls
           (
               UINT16  hdp_appl_cntrl_chnl_id
           );


#ifdef HDP_APPL_STORAGE
/*  Add Comment  */
void hdp_appl_storage_write (void);

/*  Add Comment */
void hdp_appl_storage_read (void);

#endif  /* HDP_APPL_STORAGE */


/* Function for displaying the current active channels */
void hdp_appl_display_chnls (void);

UCHAR * appl_hdp_get_device_data_type (UINT16  device_data_type);

/**
 *  Extract the mdep_id value from the
 *  Supported features list in the
 *  attribute data of the response.
 *
 *  @param UCHAR  *attribute_data : Attribute data
 *  @param UINT8  *mdep_id : Pointer to array of mdep id's
 *
 *  @result
 *     API_RESULT : API_SUCCESS or Error code
 */
API_RESULT appl_sdp_get_hdp_supported_features
           (
               /* IN */ UCHAR       * attribute_data,
               /* OUT */ UCHAR      * mdep,
               /* OUT */ UINT8      * num_mdeps
           );


/* The main HDP operations function */
void main_hdp_operations (void);


/**
 * Application registered configuration callback with HDP for
 * config request notification.
 */
void hdp_appl_config_cb
     (
         /* IN */ HDP_HANDLE            * handle,
         /* IN */ UCHAR                   config_pref,
         /* OUT */ L2CAP_CONFIG_OPTION  **l2cap_config
     );

/* Function is registered with SDP handles SDP events */
void hdp_sdp_cb
     (
          /* IN */ UCHAR command,
          /* IN */ UCHAR * data,
          /* IN */ UINT16 length,
          /* IN */ UINT16 status
     );

/* Callback registered with HCI */
API_RESULT hdp_appl_hci_event_ind_cb
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           );

/* HDP Echo Request */
API_RESULT appl_hdp_send_echo_request
           (
               /* IN */ HDP_HANDLE * hdp_handle,
               /* IN */ UCHAR      * echo_payload,
               /* IN */ UINT16       echo_payload_len
           );

#ifdef HDP_CLOCK_SYNC

API_RESULT hdp_appl_send_sync_set_rsp (UINT32 *  bt_clk_value, UINT16 *  value_2);


API_RESULT hdp_appl_send_sync_info_ind (UINT32 *  bt_clk_value, UINT16 *  value_2);


void appl_hdp_timer_cb (void *  args, UINT16  len);


void appl_hdp_sync_info_cb (void *  args, UINT16  len);

#endif   /* MCAP_CLOCK_SYNC */
#endif /* _H_APPL_HDP_ */

