
/**
 *  \file appl_hdp.c
 *
 *  Source file for HDP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_hdp.h"
#include "appl_hci.h"
#include "appl_utils.h"

#ifdef HDP
/* --------------------------------------------- Macros */
#define APPL_HDP_DATA_SIZE     1000U
#define APPL_MAX_DATA_CHNLS    10U

#ifdef HDP_CLOCK_SYNC
/**
 * Resolution (in half baseband slots) of the bluetooth clock that can be
 * accessed by the implemntation. 0x00 implies, implementaion has no access
 * to bluetooth clock.
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 */
#define APPL_BT_CLK_ACCESS_RESOLUTION  (0x12U)

/**
 * Minimum time (in ms) offset required by the implentation to process a sync
 * request from the sync master.
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 */
#define APPL_SYNC_LEAD_TIME                     (0x0145U)

/**
 * Native Resolution (in us) of the timestamp clock
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 */
#define APPL_TIMESTAMP_NATIVE_RESOLUTION        (0x0123U)

/**
 * Worst case accuracy (in ppm) of timestamp clock
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 */
#define APPL_TIMESTAMP_NATIVE_ACCURACY          (0x0019U)

#define APPL_BLUETOOTH_CLOCK_SYNC_TIME          (0x00003214U)

/* HDP Handle */
DECL_STATIC HDP_HANDLE  appl_hdp_handle;
DECL_STATIC UCHAR       rsp_params[14U], cmd_rsp[16U];

/* HDP peer BD_ADDR */
DECL_STATIC UCHAR appl_hdp_peer_bd_addr[BT_BD_ADDR_SIZE];

DECL_STATIC UINT8   send_sync_set_rsp, send_sync_info_ind;
DECL_STATIC UINT16  req_accr, sync_lead_time, ts_samp_accuracy, appl_clk_hdp_handle;
DECL_STATIC UINT16  sync_info_ind_interval, ts_native_accuracy;
DECL_STATIC UINT32  req_bt_inst, g_bt_clk_value;
DECL_STATIC UINT64  ts_clk;

DECL_STATIC BT_timer_handle appl_hdp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
DECL_STATIC BT_timer_handle appl_hdp_timer_handle1 = BT_TIMER_HANDLE_INIT_VAL;

#endif  /* HDP_CLOCK_SYNC */

/* #define SDP_INPUT */

/* ----------------------------------------- Exported Global Variables */
/* HDP operations */
#ifdef HDP_CLOCK_SYNC

static const char hdp_options[] = "\n\
-------------- HDP Menu ---------------\n\
     0.  Exit \n\
     1.  Refresh \n\
\n\
     2.  Write Class of Device \n\
     3.  Register BD_ADDR of peer HDP \n\
     4.  Create ACL Connection\n\
     5.  ACL Disconnection\n\
     6.  Perform SDP Query\n\
\n\
    10.  Register with HDP\n\n\
    11.  Establish Control Channel\n\
    12.  Disconnect Control Channel\n\
\n\
    13.  Establish Data Channel\n\
    14.  Write on Data Channel\n\
    15.  Disconnect Data Channel\n\
    16.  Delete Data Channel\n\
    17.  Delete All Data Channels\n\
    18.  Reconnect Data Channel\n\
    19.  Abort Data Channel\n\
\n\
    21.  Request Synchronization Capabilty\n\
    22.  Request Synchronization\n\
    23.  Stop Sending sync info indications\n\
    30.  Set Streaming Channel Configuration\n\
    31.  Set Reliable Channel Configuration\n\
\n\
    50.  Send Echo Request \n\
    51.  Send incorrect Echo Request \n\
Your Choice  \n";

#else /* HDP_CLOCK_SYNC */
static const char hdp_options[] = "\n\
-------------- HDP Menu ---------------\n\
     0.  Exit \n\
     1.  Refresh \n\
                 \n\
     2.  Write Class of Device \n\
     3.  Perform SDP Query\n\n\
     4.  Perform HCI Enquiry\n\
    10.  Register with HDP\n\n\
    11.  Establish Control Channel\n\
    12.  Disconnect Control Channel\n\
    \n\
    13.  Establish Data Channel\n\
    14.  Write on Data Channel\n\
    15.  Disconnect Data Channel\n\
    16.  Delete Data Channel\n\
    17.  Delete All Data Channels\n\
    18.  Reconnect Data Channel\n\
    19.  Abort Data Channel\n\
\n\
    30.  Set Streaming Channel Configuration\n\
    31.  Set Reliable Channel Configuration\n\
\n\
    50.  Send Echo Request \n\
    51.  Send incorrect Echo Request \n\
Your Choice  \n";
#endif /* HDP_CLOCK_SYNC */

/* ----------------------------------------- Static Global Variables */

DECL_STATIC UINT32      initial_flag = 0U;

/* HDP Handle */
DECL_STATIC UCHAR       appl_mep_id;
DECL_STATIC UCHAR       hdp_echo_data[64U];

/* Database record handle for HDP */
DECL_STATIC UINT32 hdp_dbase_rec_handle;
DECL_STATIC L2CAP_FEC_OPTION  control_channel_fec_params;
DECL_STATIC L2CAP_FEC_OPTION  str_channel_fec_params;

/* Parameters for the streaming channel */
DECL_STATIC HDP_MD_PARAMS appl_strm_ch_params;

/* Parameters for the reliable channel */
DECL_STATIC HDP_MD_PARAMS appl_rlbl_ch_params;

/* This variable is used by the source send its config pref to sink */
DECL_STATIC HDP_MD_PARAMS *appl_ch_params;

/* Buffer containing the data which will be sent in send data operation */
DECL_STATIC UCHAR appl_hdp_data[APPL_HDP_DATA_SIZE];

/* L2CAP Configurations for reliable data channel */
DECL_STATIC L2CAP_CONFIG_OPTION appl_rlbl_data_l2cap_config;

/* L2CAP Configurations for streaming data channel */
DECL_STATIC L2CAP_CONFIG_OPTION appl_strm_data_l2cap_config;

/* L2CAP Configurations for control channel */
DECL_STATIC L2CAP_CONFIG_OPTION appl_cntrl_l2cap_config;

/* Structure containing the role to MDEP ID mapping */
DECL_STATIC HDP_MDEP appl_mdep[HDP_APPL_MAX_MDEP_IDS];

/* Array of Control Channels */
DECL_STATIC HDP_APPL_CNTRL_CHNL cntrl_chnls[HDP_APPL_MAX_CNTRL_CHNLS];

DECL_STATIC L2CAP_CONFIG_OPTION appl_dflt_ch_l2cap_params;

/*
 * Protocol Descriptor List
 *
 * Control Channel PSM value is marked as 0XAABB
 * Assign actual Control Channel PSM value
 */
DECL_STATIC UCHAR hdp_protocol_descriptor_list[] =
                  {
                      0x35U, 0x10U, 0x35U, 0x06U,
                      0x19U, 0x01U, 0x00U, 0x09U,
                      0xAAU, 0xBBU, 0x35U, 0x06U,
                      0x19U, 0x00U, 0x1EU, 0x09U,
                      0x01U, 0x00U
                  };

/*
 * Additional Protocol Descriptor List
 *
 * Data Channel PSM value is marked as 0XAABB
 * Assign actual Data Channel PSM value
 */
DECL_STATIC UCHAR hdp_additional_protocol_descriptor_list[] =
                  {
                      0x35U, 0x0FU, 0x35U, 0x0DU,
                      0x35U, 0x06U, 0x19U, 0x01U,
                      0x00U, 0x09U, 0xAAU, 0xBBU,
                      0x35U, 0x03U, 0x19U, 0x00U,
                      0x1FU
                  };

/* HDP Supported Features created on the fly */
DECL_STATIC UCHAR  *hdp_supported_features;
DECL_STATIC UINT16  hdp_supported_features_length;

/* HDP Service Class ID List on the fly */
DECL_STATIC UCHAR  *hdp_service_class_list;
DECL_STATIC UINT8   hdp_service_class_list_length;

/* SDP Handle */
DECL_STATIC SDP_HANDLE hdp_sdp_handle ;

/* Array to store HDP Attribute value during SDP query */
DECL_STATIC UCHAR hdp_attribute_value [1024U];

DECL_STATIC HDP_MDEP_DATA_TYPE hdp_mdep_data_type[] =
{
    {
        0x1004U,
        BT_HMC_PULSE_OXIMETER,
        (UCHAR *)"Pulse Oximeter"
    },
    {
        0x1007U,
        BT_HMC_BLOOD_PRESSURE_MONITOR,
        (UCHAR *)"Blood Pressure Monitor"
    },
    {
        0x1008U,
        BT_HMC_THERMOMETER,
        (UCHAR *)"Thermometer"
    },
    {
        0x100FU,
        BT_HMC_WEIGHING_SCALE,
        (UCHAR *)"Weighing Scale"
    },
    {
        0x1011U,
        BT_HMC_GLUCOSE_METER,
        (UCHAR *)"Glucose Meter"
    }
};

/* HDP Echo Request */
DECL_STATIC UCHAR appl_hdp_echo_req_payload[] = {'H', 'D', 'P', ' ', 'E', 'C', 'H', 'O'};
DECL_STATIC UCHAR appl_hdp_echo_req_incorrect_payload[] = {'H', 'D', 'P', ' ', 'E', 'C', 'H', 'O', ' ', 'I', 'N', 'C', 'O', 'R', 'R', 'E', 'C', 'T', ' ', 'D', 'A', 'T', 'A'};
DECL_STATIC UCHAR appl_hdp_waiting_for_echo_rsp;

/* ----------------------------------------- Static Function Declarations */


/* ------------------------------------------ Functions */

/* To initialize hdp application */
void hdp_appl_init(void)
{
    UINT16 chnl_index;

    HDP_INIT_MD_PARAMS (appl_strm_ch_params);
    HDP_INIT_MD_PARAMS (appl_rlbl_ch_params);

    /* Parameters for Reliable data channel  */
    l2cap_init_config_option (&appl_rlbl_data_l2cap_config);
    l2cap_init_config_option (&appl_dflt_ch_l2cap_params);
    appl_dflt_ch_l2cap_params.mtu = 672U;
    appl_rlbl_data_l2cap_config.mtu = 672U;
    control_channel_fec_params.mode = L2CAP_MODE_ERTM;
    control_channel_fec_params.tx_window = 1U;
    control_channel_fec_params.max_transmit = 2U;
    control_channel_fec_params.retx_timeout = 1000U;
    control_channel_fec_params.monitor_timeout = 300U;
    control_channel_fec_params.mps = 48U;
    l2cap_init_config_option (&appl_cntrl_l2cap_config);
    appl_cntrl_l2cap_config.mtu = 672U;
#ifndef UPF_PTS_30
    appl_cntrl_l2cap_config.fec = &control_channel_fec_params;
#endif /* UPF_PTS_30 */

    appl_rlbl_data_l2cap_config.fec = &control_channel_fec_params;
    appl_rlbl_ch_params.channel_preference = HDP_RELIABLE_CHNL;
    appl_rlbl_data_l2cap_config.fcs = L2CAP_FCS_OPTION_DEFAULT;

    /* Parameters for the streaming data channel*/
    l2cap_init_config_option (&appl_strm_data_l2cap_config);
    appl_strm_data_l2cap_config.mtu = 672U;
    str_channel_fec_params.mode = L2CAP_MODE_SM;
    str_channel_fec_params.tx_window = 4U;
    str_channel_fec_params.mps = 48U;
    appl_strm_data_l2cap_config.fec = &str_channel_fec_params;
    appl_strm_data_l2cap_config.fcs = L2CAP_FCS_OPTION_DEFAULT;
    appl_strm_ch_params.channel_preference = HDP_STREAMING_CHNL;

    /* Make Reliable chnl Params for Response sent by Source as default */
    appl_ch_params = &appl_rlbl_ch_params;

    /* Fill the buffer with 'a' which is used in send data operation */
    BT_mem_set(appl_hdp_data, 'a', APPL_HDP_DATA_SIZE);

    /* Initialize the control/data channel stuructures */
    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_CNTRL_CHNLS; chnl_index ++)
    {
        UINT16 mdl_index;
        cntrl_chnls[chnl_index].mcap_mcl_id = HDP_APPL_INVALID_ID;

        for (mdl_index = 0U; mdl_index < HDP_APPL_MAX_MDL_IDS; mdl_index++)
        {
            cntrl_chnls[chnl_index].mcap_mdl_id[mdl_index] = HDP_APPL_INVALID_ID;
        }
        cntrl_chnls[chnl_index].rel_data_chnl_flag = 0x00U;
    }

    /* Reset Waiting for Echo Response Flag */
    appl_hdp_waiting_for_echo_rsp = BT_FALSE;

#ifdef HDP_APPL_STORAGE
    hdp_appl_storage_read ();
#endif  /* HDP_APPL_STORAGE */
}


/* To Update the values in the HDP Service record */
void hdp_appl_dbase_register (void)
{
    UINT16  attr_id;
    UCHAR   attr_val[18U], attr_buf_len;
    UINT32  act_len;
    /* Get record handle */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle
    (
        DB_RECORD_HDP,
        0U,
        &hdp_dbase_rec_handle
    );

    /* Service Class List */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value
    (
        hdp_dbase_rec_handle,
        0x0001U,
        hdp_service_class_list,
        hdp_service_class_list_length
    );

    /* Protocol Description List */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value
    (
        hdp_dbase_rec_handle,
        0x0004U,
        hdp_protocol_descriptor_list,
        18U
    );

    /* Additional Protocol Description List */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value
    (
        hdp_dbase_rec_handle,
        0x000DU,
        hdp_additional_protocol_descriptor_list,
        17U
    );

    /* Supported Features */
    if (NULL != hdp_supported_features)
    {
        (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value
        (
            hdp_dbase_rec_handle,
            0x0200U,
            hdp_supported_features,
            hdp_supported_features_length
        );
    }

    (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record ( hdp_dbase_rec_handle );

    attr_id = 0x0001U;
    attr_buf_len = 18U;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_attr_value
    (
        hdp_dbase_rec_handle,
        &attr_id ,
        attr_val,
        attr_buf_len,
        &act_len
    );

    /* return */
    return;
}


/* To get a free instance of cntrl_chnls structure array */
API_RESULT hdp_appl_get_free_cntrl_chnl_id
           (
               UINT16 *appl_cntrl_chnl_id
           )
{
    UINT16 chnl_index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_CNTRL_CHNLS; chnl_index ++)
    {
        if ((HDP_APPL_INVALID_ID == cntrl_chnls[chnl_index].mcap_mcl_id)
            && (HDP_APPL_INVALID_ID == cntrl_chnls[chnl_index].mcap_mdl_id[0U]))
        {
            *appl_cntrl_chnl_id = chnl_index;
            break; /* return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_CNTRL_CHNLS == chnl_index)
    {
        LOG_DEBUG("[***ERR***]:No free cntrl chnl instance exists\n");
        retval = API_FAILURE;
    }

    return retval;
}


/* To get the cntrl_chnls array instance corresponding to mcap provided mcl_id */
API_RESULT hdp_appl_get_cntrl_chnl_id
           (
               UINT16 mcap_mcl_id,
               UINT16 *appl_cntrl_chnl_id
           )
{
    UINT16 chnl_index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_CNTRL_CHNLS; chnl_index ++)
    {
        if (cntrl_chnls[chnl_index].mcap_mcl_id == mcap_mcl_id)
        {
            *appl_cntrl_chnl_id = chnl_index;
            break; /*  return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_CNTRL_CHNLS == chnl_index)
    {
        LOG_DEBUG
        (
            "[***ERR***]:[MCL %04X]: Control channel instance doesn't exist\n",
            mcap_mcl_id
        );
        retval = API_FAILURE;
    }

    return retval;
}


/**
 * To get the data channel array instance in the cntrl_chnls array
 * corresponding to mcap provided mdl_id
 */
API_RESULT hdp_appl_get_data_chnl_id
           (
               UINT16 hdp_appl_cntrl_chnl_id,
               UINT16 mcap_mdl_id,
               UINT16 *hdp_appl_data_chnl_id
           )
{
    UINT16 index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (index = 0U; index < HDP_APPL_MAX_MDL_IDS; index ++)
    {
        if (cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mdl_id[index] ==
            mcap_mdl_id)
        {
            *hdp_appl_data_chnl_id = index;
            break; /* return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_MDL_IDS == index)
    {
        LOG_DEBUG("[***ERR***]:No Such data channel %04X in the control channel %04X"
            "\n", mcap_mdl_id, hdp_appl_cntrl_chnl_id);
        retval = API_FAILURE;
    }

    return retval;
}


/* To get the free data channel instance in the cntrl chnl instance */
API_RESULT hdp_appl_get_free_data_chnl_id
           (
               UINT16 hdp_appl_cntrl_chnl_id,
               UINT16 *hdp_appl_data_chnl_id
           )
{
    UINT16 index;

    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (index = 0U; index < HDP_APPL_MAX_MDL_IDS; index ++)
    {
        if (HDP_APPL_INVALID_ID == cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mdl_id[index])
        {
            *hdp_appl_data_chnl_id = index;
            break; /*  return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_MDL_IDS == index)
    {
        LOG_DEBUG
        (
            "[***ERR***]:[MCL %04X] hdp_appl_get_free_data_chnl_id: No free data channel instance available,"
            " delete any existing data channel to create new one\n",
            hdp_appl_cntrl_chnl_id
        );

        retval = API_FAILURE;
    }

    return retval;
}


/**
 * This routine takes mdep_id as a parameter and return the role
 * played by the mdep_id
 */
API_RESULT hdp_appl_get_mdep_role
           (
               /* IN */ UINT8   mdep_id,
               /* OUT */ UINT8  *mdep_role
           )
{
    UINT8 index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (index = 0U; index < HDP_APPL_MAX_MDEP_IDS; index ++)
    {
        if (appl_mdep[index].mdep_id == mdep_id)
        {
            *mdep_role = appl_mdep[index].mdep_role;
            break; /*  return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_MDEP_IDS == index)
    {
        LOG_DEBUG("[***ERR***]:No Such MDEP ID %02X exists\n", mdep_id);
        retval = HDP_INVALID_MDEP;
    }

    return retval;
}


/* Function for displaying the current active channels */
void hdp_appl_display_chnls (void)
{
    /* Control and data Channel index */
    UINT16 chnl_index, index;

    LOG_DEBUG("Control and Data information\n");

    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_CNTRL_CHNLS; chnl_index ++)
    {
        if (cntrl_chnls[chnl_index].mcap_mcl_id != HDP_APPL_INVALID_ID)
        {
            LOG_DEBUG("Control Channel Id ---> %04X\n",
            cntrl_chnls[chnl_index].mcap_mcl_id);

            for (index = 0U; index < HDP_APPL_MAX_MDL_IDS; index ++)
            {
                if (cntrl_chnls[chnl_index].mcap_mdl_id[index] !=
                    HDP_APPL_INVALID_ID)
                {
                    LOG_DEBUG("Data channel Id ---> ");
                    LOG_DEBUG ("%04X: ",
                    cntrl_chnls[chnl_index].mcap_mdl_id[index]);
                }
            }
            LOG_DEBUG("\n\n");
        }
    }
}


/**
 * Function to find the index of the cntrl_chnls array based on remote BD_ADDR
 */
API_RESULT hdp_appl_get_cntrl_chnl_bdaddr
           (
               UCHAR   *bd_addr,
               UINT16  *hdp_appl_cntrl_chnl_id
           )
{
    UINT16 chnl_index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_CNTRL_CHNLS; chnl_index ++)
    {
        if (0 == (BT_mem_cmp(cntrl_chnls[chnl_index].bd_addr, bd_addr, BT_BD_ADDR_SIZE)))
        {
            *hdp_appl_cntrl_chnl_id = chnl_index;
            break; /* return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_CNTRL_CHNLS == chnl_index)
    {
        retval = API_FAILURE;
    }

    return retval;
}


/**
 * Function return API_SUCCESS if atleast one reliable data channel is present
 * in the MCL, otherwise API_FAILURE
 */
API_RESULT hdp_appl_check_rel_data_chnls
           (
               UINT16  hdp_appl_cntrl_chnl_id
           )
{
    /* Data channel index */
    UINT16 chnl_index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    BT_IGNORE_UNUSED_PARAM(hdp_appl_cntrl_chnl_id);

    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_MDL_IDS; chnl_index ++)
    {
        if ((cntrl_chnls[chnl_index].mcap_mdl_id[chnl_index]
             != HDP_APPL_INVALID_ID) &&
             (HDP_RELIABLE_CHNL == cntrl_chnls[chnl_index].mcap_mdl_config[chnl_index]))
        {
            /* Atleat one reliable channel present in the MCL */
            break; /* return API_SUCCESS; */
        }
    }

    if (HDP_APPL_MAX_MDL_IDS == chnl_index)
    {
        retval = API_FAILURE;
    }

    /* No reliable data channel exists in the MCL */
    return retval;
}

/**
 * Function return API_SUCCESS if all data channels are deleted
 */
API_RESULT hdp_appl_delete_all_data_chnls
           (
               UCHAR * bd_addr
           )
{
    UINT16 chnl_index;
    UINT16 mdl_index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (chnl_index = 0U; chnl_index < HDP_APPL_MAX_CNTRL_CHNLS; chnl_index++)
    {
        if (NULL == bd_addr)
        {
            LOG_DEBUG("Deleting data channels for all HDP entities\n");
            cntrl_chnls[chnl_index].rel_data_chnl_flag = 0x00U;

            for (mdl_index = 0U; mdl_index < HDP_APPL_MAX_MDL_IDS; mdl_index++)
            {
                cntrl_chnls[chnl_index].mcap_mdl_id[mdl_index] = HDP_APPL_INVALID_ID;
            }
        }
        else if (0 == (BT_mem_cmp(cntrl_chnls[chnl_index].bd_addr, bd_addr, BT_BD_ADDR_SIZE)))
        {
            LOG_DEBUG("Deleting All data channels for HDP entity %d\n", chnl_index);
            cntrl_chnls[chnl_index].rel_data_chnl_flag = 0x00U;

            for (mdl_index = 0U; mdl_index < HDP_APPL_MAX_MDL_IDS; mdl_index++)
            {
                cntrl_chnls[chnl_index].mcap_mdl_id[mdl_index] = HDP_APPL_INVALID_ID;
            }
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

#ifdef HDP_APPL_STORAGE
    hdp_appl_storage_write();
#endif  /* HDP_APPL_STORAGE */

    return retval;
}

#ifdef HDP_APPL_STORAGE

/* Writes the active channels info to the file */
void hdp_appl_storage_write (void)
{
    FILE *fp;
    UINT16 iindex;

    fp = fopen ("hdp_appl.db","wb");
    if (NULL == fp)
    {
        LOG_DEBUG("[***ERR***]:Failed to open the file for writing\n");
        /* return; */
    }
    else
    {
        /* LOG_DEBUG("Writing data to persistant storage\n"); */

        for (iindex = 0U; iindex < HDP_APPL_MAX_CNTRL_CHNLS; iindex ++)
        {
            if (cntrl_chnls[iindex].mcap_mcl_id != HDP_APPL_INVALID_ID)
            {
                hdp_appl_ps_write (cntrl_chnls[iindex].bd_addr, BT_BD_ADDR_SIZE);

                hdp_appl_ps_write (cntrl_chnls[iindex].mcap_mdl_id,
                    sizeof(cntrl_chnls[iindex].mcap_mdl_id));

                hdp_appl_ps_write (cntrl_chnls[iindex].mcap_mdl_config,
                    sizeof(cntrl_chnls[iindex].mcap_mdl_config));
            }
        }
        fclose (fp);
    }

    return;
}


/**
 *  \fn hdp_storage_read
 *
 *  \brief Reads HDP data channel info from persistent storage media.
 *
 *  \Description
 *  This routine reads HDP data channel information which include
 *  mcap provided mdl_id and the hdp data channel id from persistent
 *  storage media into hdp data channels structure
 *
 *  \param None
 *
 *  \return None
 */
void hdp_appl_storage_read (void)
{
    FILE *fp;
    UINT16 index;

    index = 0U;

    fp = fopen("hdp_appl.db","rb");
    if (NULL == fp)
    {
        LOG_DEBUG("[***ERR***]:Failed to open the file for reading\n");
        /* return; */
    }
    else
    {
        BT_LOOP_FOREVER()
        {
            hdp_appl_ps_read(&cntrl_chnls[index].bd_addr, BT_BD_ADDR_SIZE);
            /* If end of file then break */
            if(feof(fp))
                break;
            hdp_appl_ps_read
            (
                &cntrl_chnls[index].mcap_mdl_id,
                sizeof(cntrl_chnls[index].mcap_mdl_id)
            );

            hdp_appl_ps_read
            (
                &cntrl_chnls[index].mcap_mdl_config,
                sizeof(cntrl_chnls[index].mcap_mdl_config)
            );

            index ++;
        }
        fclose (fp);
    }

    return;
}
#endif  /* HDP_APPL_STORAGE */


/* Application registered this callback with HDP */
API_RESULT hdp_appl_cb
           (
               HDP_HANDLE    * handle,
               UINT8          event_type,
               UINT16         response,
               void          * event_params,
               UINT16         event_len
           )
{
    API_RESULT retval;
    UINT8 * config_pref, mdep_role, free_data_chnls_count, index;
    UINT16 hdp_appl_cntrl_chnl_id, hdp_appl_data_chnl_id;
#ifdef  HDP_CLOCK_SYNC
    UCHAR * event_par;
    UINT16  time_stamp_accuracy;
    UINT32  bt_inst;
    UINT64  time_stamp;

    event_par = (UCHAR *)event_params;
#endif  /* HDP_CLOCK_SYNC */

    retval = API_SUCCESS;
    hdp_appl_cntrl_chnl_id = 0U;
    hdp_appl_data_chnl_id = 0U;

    if ((event_type != HDP_MC_CREATE_IND) &&
       (event_type != HDP_MC_CREATE_CNF))
    {
        retval = hdp_appl_get_cntrl_chnl_id
                 (
                     handle->mcl_id,
                     &hdp_appl_cntrl_chnl_id
                 );
#if 0
        if (retval != API_SUCCESS)
        {
            return retval;
        }
#endif /* 0 */
    }

    if (API_SUCCESS == retval)
    {
        switch (event_type)
        {
        case HDP_MC_CREATE_IND:
            LOG_DEBUG ("\n[MCL %04X]:Recieved HDP_MC_CREATE_IND event\n",
            handle->mcl_id);

            LOG_DEBUG ("From remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (handle->bd_addr));

            /**
             * Check if cntrl_chnls array instance is already allocated to this
             * remote device
             */
            retval = hdp_appl_get_cntrl_chnl_bdaddr
                     (
                         handle->bd_addr,
                         &hdp_appl_cntrl_chnl_id
                     );

            /* If not allocated then get a free cntrl_chnls array instance */
            if (retval != API_SUCCESS)
            {
                retval = hdp_appl_get_free_cntrl_chnl_id
                         (
                             &hdp_appl_cntrl_chnl_id
                         );
            }

            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("[MCL %04X]: Failed to find a free instance\n",
                handle->mcl_id);
                break;
            }
            cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mcl_id = handle->mcl_id;
            BT_COPY_BD_ADDR
            (
                cntrl_chnls[hdp_appl_cntrl_chnl_id].bd_addr,
                handle->bd_addr
            );

            retval = BT_hdp_mc_connect_rsp (handle,API_SUCCESS);
            LOG_DEBUG ("[MCL %04X]: Sent Control Channel Response with result %04X\n",
            handle->mcl_id,retval);

            break;

        case HDP_MC_CREATE_CNF:
            LOG_DEBUG ("\n[MCL %04X]:Recieved HDP_MC_CREATE_CNF with result %04X\n",
            handle->mcl_id,response);

            if (API_SUCCESS == response)
            {
                /**
                 * Check if cntrl_chnls array instance is already allocated to this
                 * remote device
                 */
                retval = hdp_appl_get_cntrl_chnl_bdaddr
                         (
                             handle->bd_addr,
                             &hdp_appl_cntrl_chnl_id
                         );

                /* If not allocated then get a free cntrl_chnls array instance */
                if (retval != API_SUCCESS)
                {
                    retval = hdp_appl_get_free_cntrl_chnl_id
                             (
                                 &hdp_appl_cntrl_chnl_id
                             );
                }

                if (retval != API_SUCCESS)
                {
                    LOG_DEBUG ("[MCL %04X]: Failed to find a free instance\n",
                    handle->mcl_id);
                    break;
                }
                cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mcl_id = handle->mcl_id;
                BT_COPY_BD_ADDR
                (
                    cntrl_chnls[hdp_appl_cntrl_chnl_id].bd_addr,
                    handle->bd_addr
                );
                cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag = 0x00U;
                hdp_appl_display_chnls ();

    #ifdef HDP_CLOCK_SYNC
                BT_mem_copy
                (
                    &appl_hdp_handle.bd_addr,
                    handle->bd_addr,
                    BT_BD_ADDR_SIZE
                );
                appl_hdp_handle.mcl_id = handle->mcl_id;
    #endif  /* HDP_CLOCK_SYNC */
            }
            break;

        case HDP_MC_DISCONNECT_IND:
            LOG_DEBUG ("\n[MCL %04X]:Received HDP_MC_DISCONNECT_IND with result "
            "%04X\n",handle->mcl_id,response);
            cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mcl_id = HDP_APPL_INVALID_ID;
            break;

        case HDP_MC_DISCONNECT_CNF:
            LOG_DEBUG ("\n[MCL %04X]:Received HDP_MC_DISCONNECT_CNF with result "
            "%04X\n",handle->mcl_id,response);

            if (API_SUCCESS == response)
            {
                cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mcl_id
                    = HDP_APPL_INVALID_ID;
            }
            break;

        case HDP_MD_CREATE_IND:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:Received HDP_MD_CREATE_IND with "
            "result %04X\n",handle->mcl_id,handle->mdl_id,response);

            config_pref = (UCHAR *) event_params;
            if (handle->mdep_id != 0x00U)
            {
                /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
                mdep_role = HDP_INVALID_ROLE;

                retval = hdp_appl_get_mdep_role (handle->mdep_id, &mdep_role);

                if ((HDP_SOURCE == mdep_role) &&
                    ((*config_pref) != HDP_NO_PREFERENCE))
                {
                    LOG_DEBUG("[MCL %04X]:[MDL %04X]:[MDEP %02X]: Invalid Channel "
                    "Configuration %02X received by Source\n",handle->mcl_id,
                    handle->mdl_id,handle->mdep_id,*config_pref);
                    retval = HDP_CONFIG_REJECTED;
                }

                if ((HDP_SINK == mdep_role) &&
                    (HDP_NO_PREFERENCE == (*config_pref)))
                {
                    LOG_DEBUG("[MCL %04X]:[MDL %04X]:[MDEP %02X]: Invalid Channel "
                    "Configuration %02X received by Sink\n",handle->mcl_id,
                    handle->mdl_id,handle->mdep_id,*config_pref);

                    retval = HDP_CONFIG_REJECTED;
                }

                if ((HDP_STREAMING_CHNL == (*config_pref)) &&
                    (0x00U == cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag))
                {
                    LOG_DEBUG("\n\n[**ERR**][MCL %04X]: Reliable data channel not "
                    "established\n\n",handle->mcl_id);
                    retval = HDP_CONFIG_REJECTED;
                }

                if (retval != API_SUCCESS)
                {
                    retval = BT_hdp_md_create_rsp
                             (
                                 handle,
                                 retval,
                                 event_params,
                                 event_len
                             );
                    LOG_DEBUG ("[MCL %04X]: Configuration Rejected Response Sent with"
                    "Result %04X\n",handle->mcl_id,retval);
                    break;
                }
            }
            /* Received No Configuration preference */
            if (HDP_NO_PREFERENCE == (*config_pref))
            {
                HDP_MD_PARAMS *ch_params;

                ch_params = appl_ch_params;

                /**
                 * Propose either Streaming mode or Reliable mode based on the
                 * appl_ch_params variable value
                 */
                if (0x00U == cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag)
                {
                    /* First channel has to be reliable */
                    ch_params = &appl_rlbl_ch_params;
                }

                retval = BT_hdp_md_create_rsp
                         (
                             handle,
                             (UCHAR)API_SUCCESS,
                             (UCHAR *)(&ch_params->channel_preference),
                             sizeof(UCHAR)
                         );
                LOG_DEBUG("[MCL %04X]:[MDL %04X]:[MDEP %02X]: Channel Config Pref = "
                "%02X\n",handle->mcl_id,handle->mdl_id,handle->mdep_id,
                ch_params->channel_preference);
            }
            /* Received Configuration preference as Streaming mode */
            else if (HDP_STREAMING_CHNL == (*config_pref))
            {
                retval = BT_hdp_md_create_rsp
                         (
                             handle,
                             API_SUCCESS,
                             &appl_strm_ch_params.channel_preference,
                             sizeof(UCHAR)
                         );
            }
            /* Received Configuration preference as Reliable mode */
            else
            {
                retval = BT_hdp_md_create_rsp
                         (
                             handle,
                             API_SUCCESS,
                             &appl_rlbl_ch_params.channel_preference,
                             sizeof(UCHAR)
                         );
            }

            LOG_DEBUG ("[MCL %04X]:[MDL %04X]:[MDEP %02X]: Sent Data Channel CREATE"
            " Response with result = %04X\n\n",handle->mcl_id,handle->mdl_id,
            handle->mdep_id,retval);
            break;

        case HDP_MD_CREATE_CNF:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]:Received HDP_MD_CREATE_CNF "
            "with result %04X\n",handle->mcl_id,handle->mdl_id,handle->mdep_id,
            response);
            if (API_SUCCESS == response)
            {
                config_pref = (UCHAR *) event_params;
                retval = hdp_appl_get_free_data_chnl_id
                         (
                             hdp_appl_cntrl_chnl_id,
                             &hdp_appl_data_chnl_id
                         );
                if (retval != API_SUCCESS)
                {
                    break;
                }

                cntrl_chnls[hdp_appl_cntrl_chnl_id].
                    mcap_mdl_id[hdp_appl_data_chnl_id] = handle->mdl_id;

                cntrl_chnls[hdp_appl_cntrl_chnl_id].
                    mcap_mdl_config[hdp_appl_data_chnl_id] = *config_pref;

                if (handle->mdep_id != 0x00U)
                {
                    if ((HDP_STREAMING_CHNL == (*config_pref)) &&
                    (0x00U == cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag))
                    {
                        LOG_DEBUG("\n\n[***ERR***]:[MCL %04X]: Reliable Data Channel "
                        "Not Established\n\n",handle->mcl_id);

                        (BT_IGNORE_RETURN_VALUE) BT_hdp_md_delete_req (handle);
                    }
                    cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag = 0x01U;
                }

                free_data_chnls_count = 0U;
                for (index = 0U; index < HDP_APPL_MAX_MDL_IDS; index ++)
                {
                    if (HDP_APPL_INVALID_ID == cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mdl_id[index])
                    {
                        free_data_chnls_count ++;
                    }
                }
                LOG_DEBUG("\n[MCL %04X]: %02X additional data channels can "
                    "be created\n",handle->mcl_id,free_data_chnls_count);

    #ifdef HDP_APPL_STORAGE
            hdp_appl_storage_write ();
    #endif  /* HDP_APPL_STORAGE */
            }

            if (MCAP_NO_CHANNEL_INSTANCE_AVAILABLE == response)
            {
                LOG_DEBUG
                (
                    "[***ERR***]:[MCL %04X]: No free data channel instance avail"
                    "able, delete any existing data channel to create new one\n",
                    handle->mcl_id
                );
            }
            break;

        case HDP_MD_DISCONNECT_IND:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_MD_DISCONNECT_IND with result %04X\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,response);
            break;

        case HDP_MD_DISCONNECT_CNF:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_MD_DISCONNECT_CNF with result %04X\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,response);
            break;

        case HDP_MD_DELETE_IND: /* Fall Through */
        case HDP_MD_DELETE_CNF:

            if (HDP_MD_DELETE_IND == event_type)
            {
                LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
                "HDP_MD_DELETE_IND with result %04X\n",handle->mcl_id,
                handle->mdl_id,handle->mdep_id,response);
            }
            else
            {
                LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
                "HDP_MD_DELETE_CNF with result %04X\n",handle->mcl_id,
                handle->mdl_id,handle->mdep_id,response);
            }

            if (API_SUCCESS == response)
            {
                retval = hdp_appl_get_data_chnl_id
                         (
                             hdp_appl_cntrl_chnl_id,
                             handle->mdl_id,
                             &hdp_appl_data_chnl_id
                         );
                if (retval != API_SUCCESS)
                {
                    break;
                }

                /* Delete the data channel instance */
                cntrl_chnls[hdp_appl_cntrl_chnl_id].
                    mcap_mdl_id[hdp_appl_data_chnl_id] = HDP_APPL_INVALID_ID;

                /* If deleting reliable data channel */
                if (HDP_RELIABLE_CHNL == cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mdl_config[hdp_appl_data_chnl_id])
                {
                    /* Check if atleast one reliabl data chnl exists in the MCL */
                    retval = hdp_appl_check_rel_data_chnls(hdp_appl_cntrl_chnl_id);

                    if (retval != API_SUCCESS)
                    {
                        /* If no reliable data channel exists */
                        cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag = 0U;
                    }
                }
    #ifdef HDP_APPL_STORAGE
            hdp_appl_storage_write ();
    #endif  /* HDP_APPL_STORAGE */
            }
            break;

        case HDP_MD_DELETE_ALL_CNF: /* Fall Through */
        case HDP_MD_DELETE_ALL_IND:
            {
                UINT16 mdl_index;

                if (HDP_MD_DELETE_ALL_CNF == event_type)
                {
                    LOG_DEBUG ("\n[MCL %04X]: Received HDP_MD_DELETE_ALL_CNF with result "
                     "%04X\n",handle->mcl_id,response);
                }
                else
                {
                    LOG_DEBUG ("\n[MCL %04X]: Received HDP_MD_DELETE_ALL_IND with result "
                    "%04X\n",handle->mcl_id,response);
                }

                cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag = 0x00U;

                for (mdl_index = 0U; mdl_index < HDP_APPL_MAX_MDL_IDS; mdl_index++)
                {
                    cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mdl_id[mdl_index] = HDP_APPL_INVALID_ID;
                }
            }
            break;

        case HDP_MD_RECONNECT_IND:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_MD_RECONNECT_IND with result %04X\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,response);

            retval = hdp_appl_get_data_chnl_id
                     (
                         hdp_appl_cntrl_chnl_id,
                         handle->mdl_id,
                         &hdp_appl_data_chnl_id
                     );
            if (API_SUCCESS == retval)
            {
                if (0x00U == cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag)
                {
                    if (HDP_STREAMING_CHNL == cntrl_chnls[hdp_appl_cntrl_chnl_id].mcap_mdl_config[hdp_appl_data_chnl_id])
                    {
                        LOG_DEBUG ("\n\n[***ERR***]:[MCL %04X]: Reliable Data Channel Not "
                        "Established\n\n",handle->mcl_id);
                        retval = HDP_CONFIG_REJECTED;
                    }
                }
            }

            retval = BT_hdp_md_reconnect_rsp
                     (
                         handle,
                         retval
                     );

            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Sent Data Channel "
            "Reconnect Response with Result = %04X\n\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,retval);
            break;

        case HDP_MD_RECONNECT_CNF:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_MD_RECONNECT_CNF with result %04X\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,response);
            cntrl_chnls[hdp_appl_cntrl_chnl_id].rel_data_chnl_flag = 0x01U;
            break;

        case HDP_MD_ABORT_IND:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_MD_ABORT_IND with result %04X\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,response);
            break;

        case HDP_MD_ABORT_CNF:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_MD_ABORT_CNF with result %04X\n",handle->mcl_id,
            handle->mdl_id,handle->mdep_id,response);
            break;

        case HDP_DATA_IND:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received HDP_DATA_IND "
            "with result %04X, Data Length %04X\n",handle->mcl_id,handle->mdl_id,
            handle->mdep_id,response,event_len);

            /* Send Echo Response */
            if (0x00U == handle->mdep_id)
            {
               /* If received Echo Request */
               if (BT_FALSE == appl_hdp_waiting_for_echo_rsp)
               {
                    BT_mem_copy(hdp_echo_data,event_params,event_len);
                    retval = BT_hdp_md_write
                             (
                                 handle,
                                 hdp_echo_data,
                                 event_len
                             );
               }
               else
               {
                   /**
                    * Reseting waiting for Echo Response flag,
                    * irrespective of it is correct or not
                    */
                   appl_hdp_waiting_for_echo_rsp = BT_FALSE;

                   /* Compare received data and see if matched */
                   if ((event_len != sizeof(appl_hdp_echo_req_payload)) ||
                       (0 != BT_mem_cmp (appl_hdp_echo_req_payload, event_params, event_len)))
                   {
                       LOG_DEBUG ("\nEcho Response does not match\n");
                       LOG_DEBUG ("\nUse HDP menu options to delete Data channel and disconnect Control Channel\n");
                   }
                   else
                   {
                       LOG_DEBUG ("Received Echo Response\n");
                   }
               }
            }

            break;

        case HDP_DATA_WRITE_CNF:
            LOG_DEBUG ("\n[MCL %04X]:[MDL %04X]:[MDEP %02X]: Received "
            "HDP_DATA_WRITE_CNF with result %04X, Data Length %04X\n",
            handle->mcl_id,handle->mdl_id,handle->mdep_id,response,event_len);
            break;

    #ifdef HDP_CLOCK_SYNC
        case HDP_SYNC_CAP_REQ:
            LOG_DEBUG ("\n[MCL %04X]:Received HDP_SYNC_CAP_REQ with result %04X\n",
            handle->mcl_id,response);

            HDP_UNPACK_2_BYTES (&req_accr, (UCHAR*) event_par);
            LOG_DEBUG ("[MCL %04X]:Required accuracy is = %04X ppm\n", handle->mcl_id,
            req_accr);

            /* 1 octect - bluetooth access resolution */
            HDP_WRITE_BLUETOOTH_CLOCK_ACCESS_RESOLUTION (cmd_rsp,\
                APPL_BT_CLK_ACCESS_RESOLUTION);

            /* 2 octect - sync clead time */
            HDP_WRITE_SYNC_LEAD_TIME ((&cmd_rsp[1U]),APPL_SYNC_LEAD_TIME);
            sync_lead_time = APPL_SYNC_LEAD_TIME;

            /* 2 octets - native timestamp resolution */
            HDP_WRITE_TIMESTAMP_NATIVE_RESOLUTION ((&cmd_rsp[3U]),\
                APPL_TIMESTAMP_NATIVE_RESOLUTION);

            /* 2 octets - native timestamp accuracy */
            HDP_WRITE_TIMESTAMP_NATIVE_ACCURACY ((&cmd_rsp[5U]),\
                APPL_TIMESTAMP_NATIVE_ACCURACY);

            ts_native_accuracy = APPL_TIMESTAMP_NATIVE_ACCURACY;
            retval = BT_hdp_sync_cap_rsp (handle , API_SUCCESS, cmd_rsp);
            LOG_DEBUG ("[MCL %04X]: Sent Sync Cap Response with result = %04X\n",
            handle->mcl_id,retval);
            break;

        case HDP_SYNC_CAP_RSP:
            LOG_DEBUG ("\n[MCL %04X]: Received HDP_SYNC_CAP_RSP with result %04X\n",
            handle->mcl_id,response);
            if (API_SUCCESS == response)
            {
                HDP_UNPACK_2_BYTES (&sync_lead_time, (UCHAR *)(&event_par[1U]));
                HDP_UNPACK_2_BYTES (&ts_native_accuracy, (UCHAR *)(&event_par[5U]));
                LOG_DEBUG ("[MCL %04X]:Slave sync lead time is %04X milli secs\n",
                handle->mcl_id, sync_lead_time);
                LOG_DEBUG ("[MCL %04X]:Slave timestamp native accuracy is %04X ppm\n",
                handle->mcl_id,ts_native_accuracy);
            }
            break;

        case HDP_SYNC_SET_REQ:
            LOG_DEBUG ("\n[MCL %04X]: Received HDP_SYNC_SET_REQ with result %04X\n",
            handle->mcl_id,response);
            HDP_UNPACK_1_BYTE  (&send_sync_info_ind, (UCHAR *)event_par);
            HDP_UNPACK_4_BYTES (&req_bt_inst, (UCHAR *)(&event_par[1U]));
            HDP_UNPACK_8_BYTES (&ts_clk , (UCHAR *)(&event_par[5U]));

            LOG_DEBUG ("[MCL %04X]: Requested bt_clk time %08X and timestamp %010llX\n",
            handle->mcl_id, req_bt_inst, ts_clk);

            if (0xFFFFFFFFU == req_bt_inst)
            {
                UINT32 val;
                retval = BT_stop_timer (appl_hdp_timer_handle1);
                val = 0x00000000U;
                HDP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME (cmd_rsp,(UINT32)val);
                HDP_WRITE_TIMESTAMP_SYNC_TIME ((&cmd_rsp[4U]),(UINT64)ts_clk);
                val = 0x0000U;
                HDP_WRITE_TIMESTAMP_SAMPLE_ACCURACY (&cmd_rsp[12U],(UINT16)val);

                /**
                 * Since req_bt_inst is ffffffff, the Sync-Slave shall
                 * immediately set the Time-Stamp clock to the
                 * "TimeStamp_SyncTime" indicated value, and then shall
                 * respond with a MD_SYNC_SET_RSP.
                 */
                retval = BT_hdp_sync_set_rsp
                         (
                             handle,
                             API_SUCCESS,
                             cmd_rsp
                         );

                if (1U == send_sync_info_ind)
                {
                    if (ts_native_accuracy == 0U)
                    {
                        LOG_DEBUG("Native Accuracy is not set.\n");
                        LOG_DEBUG("Check if Sync Capability procedure is performed.\n");
                    }
                    else
                    {
                        /**
                         *  MD_SYNC_INFO_IND indicator to be called every Time-Stamp Update Interval
                         *  and is calculated as the TimeStamp_RequiredAccuracy (provided by the Sync-Master
                         *  as part of the MD_SYNC_CAP_REQ command) divided by the TimeStamp_NativeAccuracy
                         *  (provided by the Sync-Slave as part of the MD_SYNC_CAP_RSP response).
                         *
                         *  For example, if the Sync-Master indicates a TimeStamp_RequiredAccuracy of 1000ppm,
                         *  and the Sync-Slave indicates a worst case TimeStamp_NativeAccuracy of 20ppm,
                         *  the Sync-Slave is required to send a MD_SYNC_INFO_IND to the Sync-Master
                         *  at least every 1000/20 or 50 seconds.
                         */
                        sync_info_ind_interval = ((req_accr + (ts_native_accuracy - 1)) / ts_native_accuracy);

                        /**
                         * Read the BT clock and start the timer to send the
                         * sync_ind if "TimeStamp_UpdateInformation" is set to 1
                         */
                        retval = BT_hci_read_clock(1U, appl_clk_hdp_handle);
                        if (retval != API_SUCCESS)
                        {
                            LOG_DEBUG("[MCL %04X]: Failed to Read BT Piconet Clock\n",
                            handle->mcl_id);
                        }
                        else
                        {
                            retval = BT_stop_timer(appl_hdp_timer_handle1);
                            /* Setting send_sync_set_rsp to 0 as sync_set_rsp is already sent */
                            send_sync_set_rsp = 0U;
                        }
                    }
                }

                LOG_DEBUG ("[MCL %04X]: Synchronization Response Sent with Result "
                "%04X\n",handle->mcl_id,retval);

                /* return retval; */
            }
            else
            {
                if (0U != (req_bt_inst & 0xF0000000U))
                {
                    LOG_DEBUG ("[MCL %04X]: Invalid  BT Clock Instant %08X requested\n",
                    handle->mcl_id, req_bt_inst);
                    retval = BT_hdp_sync_set_rsp
                             (
                                 handle,
                                 MCAP_INVALID_PARAMETER_VALUE,
                                 NULL
                             );

                    LOG_DEBUG ("[MCL %04X]: Synchronization Response Sent with Response "
                    "Code MCAP_INVALID_PARAMETER_VALUE with Result %04X\n",
                    handle->mcl_id,retval);
                    retval = API_FAILURE; /* return API_FAILURE; */
                }
                else
                {
                    /**
                     * If the BluetoothClock_SyncTime is set to another valid value
                     * (see MD_SYNC_SET_RSP description for details), the Sync - Slave
                     * shall set the Time - Stamp Clock to the TimeStamp_SyncTime indicated
                     * value when the Bluetooth Clock time reaches the Baseband Half - Slot Instant
                     * defined in the BluetoothClock_SyncTime and then shall respond
                     * with a MD_SYNC_SET_RSP.
                     */
                    retval = BT_hci_read_clock(1U, appl_clk_hdp_handle);
                    if (retval != API_SUCCESS)
                    {
                        LOG_DEBUG("[MCL %04X]: Failed to Read BT Piconet Clock\n",
                        handle->mcl_id);
                    }
                    else
                    {
                        retval = BT_stop_timer(appl_hdp_timer_handle1);
                        send_sync_set_rsp = 1U;
                    }
                }
            }
            break;

        case HDP_SYNC_SET_RSP:
            LOG_DEBUG ("\n[MCL %04X]: Received HDP_SYNC_SET_RSP, with result %04X\n",
            handle->mcl_id,response);

            if (API_SUCCESS == response)
            {
                HDP_UNPACK_4_BYTES (&bt_inst, (UCHAR*)event_par);
                HDP_UNPACK_8_BYTES (&time_stamp, (UCHAR*)(&event_par[4U]));
                HDP_UNPACK_2_BYTES \
                (&time_stamp_accuracy, (UCHAR*)(&event_par[12U]));


                LOG_DEBUG ("[MCL %04X]: BT Clock Instant = %08X, Timestamp = %016llX,"
                "Timestamp Accuracy = %04X\n", handle->mcl_id, bt_inst,
                time_stamp, time_stamp_accuracy);
            }
            break;

        case HDP_SYNC_INFO_IND:
            LOG_DEBUG ("\n[MCL %04X]:Received HDP_SYNC_INFO_IND with result %04X\n",
            handle->mcl_id,response);

            if (API_SUCCESS == retval)
            {
                HDP_UNPACK_4_BYTES (&bt_inst, (UCHAR*)event_par);
                HDP_UNPACK_8_BYTES (&time_stamp, (UCHAR*)(&event_par[4U]));
                HDP_UNPACK_2_BYTES (&time_stamp_accuracy, (UCHAR*)(&event_par[12U]));

                LOG_DEBUG ("[MCL %04X]: BT Clock Instant = %08X, Timestamp = %016llX,"
                "Timestamp Accuracy = %04X\n",handle->mcl_id, bt_inst, time_stamp,
                time_stamp_accuracy);
            }
            break;

    #endif  /* HDP_CLOCK_SYNC */
        default:
            LOG_DEBUG ("[MCL %04X]:[MDL %04X]:[MDEP %04X]:Unknown HDP event %02X with"
            "rsp %04X\n",handle->mcl_id,handle->mdl_id,handle->mdep_id,
            event_type, response);
            break;
        }
    }

    return retval;
}

#ifdef HDP_CLOCK_SYNC
/**
 * This function is used to send sync set response to the peer(master)
 * who has sent the sync set request
 */
API_RESULT hdp_appl_send_sync_set_rsp (UINT32  *bt_clk_value, UINT16 *value_2)
{
    UINT32  bt_clk_diff;
    API_RESULT  retval;

    /* Init */
    retval = API_SUCCESS;
    send_sync_set_rsp = 0U;
    bt_clk_diff = req_bt_inst - (*bt_clk_value);
    bt_clk_diff /= 0xc80U;  /* bt_clk_diff in secs */
    sync_lead_time /= 0x3e8U; /* sync_lead_time in secs */

    if ((bt_clk_diff < (UINT32)sync_lead_time) ||
        (bt_clk_diff > (UINT32)(sync_lead_time + 60U)))
    {
        LOG_DEBUG ("[***ERR***]:Invaid request\n");
        LOG_DEBUG ("Requested bluetooth clock time = %08X, cur_bt_int = %08X\n",
        req_bt_inst, (*bt_clk_value));
        LOG_DEBUG ("Difference in number of secs = %08X\n", bt_clk_diff);

        retval = BT_hdp_sync_set_rsp
                 (
                     &appl_hdp_handle,
                     MCAP_INVALID_PARAMETER_VALUE,
                     NULL
                 );
        LOG_DEBUG ("Sent Synchronization Response with Response invalid param\n");

        retval = MCAP_INVALID_PARAMETER_VALUE; /* return MCAP_INVALID_PARAMETER_VALUE; */
    }
    else
    {
        ts_samp_accuracy = *value_2;

        appl_hdp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        retval = BT_start_timer
                 (
                     &appl_hdp_timer_handle,
                     (UINT16) (bt_clk_diff),
                     appl_hdp_timer_cb,
                     NULL,
                     0U
                 );

        if (retval != API_SUCCESS)
        {
            LOG_DEBUG ("[***ERR***]:Failed to start timer for sending response\n");
        }
        else
        {
            LOG_DEBUG ("Timer started for %08X secs\n", bt_clk_diff);
        }
    }

    return retval;
}


void appl_hdp_timer_cb (void *args, UINT16 len)
{
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(len);

    HDP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME (rsp_params, req_bt_inst);
    HDP_WRITE_TIMESTAMP_SYNC_TIME ((&rsp_params[4U]),(UINT64)ts_clk);
    HDP_WRITE_TIMESTAMP_SAMPLE_ACCURACY (&rsp_params[12U], ts_samp_accuracy);

    retval = BT_hdp_sync_set_rsp (&appl_hdp_handle, API_SUCCESS, rsp_params);
    LOG_DEBUG ("The sync set rsp returned %04X\n" , retval);

    if (1U == send_sync_info_ind)
    {
        appl_hdp_timer_handle1 = BT_TIMER_HANDLE_INIT_VAL;

        if (ts_native_accuracy == 0U)
        {
            LOG_DEBUG("Native Accuracy is not set.\n");
            LOG_DEBUG("Check if Sync Capability procedure is performed.\n");
        }
        else
        {
            /**
             *  MD_SYNC_INFO_IND indicator to be called every Time-Stamp Update Interval
             *  and is calculated as the TimeStamp_RequiredAccuracy (provided by the Sync-Master
             *  as part of the MD_SYNC_CAP_REQ command) divided by the TimeStamp_NativeAccuracy
             *  (provided by the Sync-Slave as part of the MD_SYNC_CAP_RSP response).
             *
             *  For example, if the Sync-Master indicates a TimeStamp_RequiredAccuracy of 1000ppm,
             *  and the Sync-Slave indicates a worst case TimeStamp_NativeAccuracy of 20ppm,
             *  the Sync-Slave is required to send a MD_SYNC_INFO_IND to the Sync-Master
             *  at least every 1000/20 or 50 seconds.
             */
            sync_info_ind_interval = ((req_accr + (ts_native_accuracy - 1)) / ts_native_accuracy);
            retval = BT_start_timer
                     (
                         &appl_hdp_timer_handle1,
                         sync_info_ind_interval,
                         appl_hdp_sync_info_cb,
                         NULL,
                         0U
                     );

            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("Start timer failed for sending sync info indication\n");
            }
            else
            {
                LOG_DEBUG ("Timer started for %04X secs\n", sync_info_ind_interval);
            }
        }
    }
}

/**
 * Callback registered with timer set for sending sync info indications
 * if peer has requested for it in the last sync set request
 */
void appl_hdp_sync_info_cb (void *args, UINT16 len)
{
    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(len);

    API_RESULT  retval;

    retval = BT_hci_read_clock (1U, appl_clk_hdp_handle);

    if (retval != API_SUCCESS)
    {
        LOG_DEBUG ("HCI Read clock failed %04X\n", retval);
    }
}


/**
 * This function is used to send sync info indications to the peer
 */
API_RESULT hdp_appl_send_sync_info_ind (UINT32  *bt_clk_value, UINT16 *value_2)
{
    API_RESULT  retval;

    HDP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME (rsp_params, *bt_clk_value);
    ts_clk += ((*bt_clk_value - req_bt_inst) * (313U));
    HDP_WRITE_TIMESTAMP_SYNC_TIME ((&rsp_params[4U]),(UINT64)ts_clk);
    HDP_WRITE_TIMESTAMP_SAMPLE_ACCURACY (&rsp_params[12U], *value_2);

    retval = BT_hdp_sync_info_ind (&appl_hdp_handle, rsp_params);

    LOG_DEBUG ("BT_hdp_sync_info_ind API returned %04X\n", retval);

    if ((API_SUCCESS == retval) && (1U == send_sync_info_ind))
    {
        appl_hdp_timer_handle1 = BT_TIMER_HANDLE_INIT_VAL;

        retval = BT_start_timer
                 (
                     &appl_hdp_timer_handle1,
                     sync_info_ind_interval,
                     appl_hdp_sync_info_cb,
                     NULL,
                     0U
                 );

        if (retval != API_SUCCESS)
        {
            LOG_DEBUG ("Start timer failed for sending sync info indication\n");
        }
        else
        {
            LOG_DEBUG ("Timer started for %04X secs\n", sync_info_ind_interval);
        }
    }
    return retval;
}


#endif  /* HDP_CLOCK_SYNC */

UCHAR * appl_hdp_get_device_data_type (UINT16  device_data_type)
{
    UINT32 index;
    UCHAR  *dev_type;

    /* Init */
    dev_type = NULL;

    for (index = 0U;
         index < (sizeof(hdp_mdep_data_type) / sizeof(HDP_MDEP_DATA_TYPE));
         index ++)
    {
        if (device_data_type == hdp_mdep_data_type[index].mdep_data_type)
        {
            dev_type = hdp_mdep_data_type[index].data_type_name; /* return hdp_mdep_data_type[index].data_type_name; */
            break;
        }
    }

    return dev_type;
}

void appl_print_mdep_element
     (
         /* IN */ UCHAR *mdep_element,
         /* IN */ UINT32 mdep_element_length
     )
{
    UCHAR  mdep_id;
    UINT16 mdep_type;
    UCHAR  mdep_role;

    /* Minimum length is (1+1) + (1+2) + (1+1) = 7 */
    if (mdep_element_length < 7U)
    {
        LOG_DEBUG("Minimum length of MDEP element is 7 octets.\n");
        LOG_DEBUG("MDEP Element Length = %d\n", mdep_element_length);

        /* return; */
    }
    else
    {
        /* Not checking the MDEP element order and their associated SDP types */
        mdep_id = mdep_element[1U];

        mdep_type = mdep_element[3U];
        mdep_type <<= 8U;
        mdep_type |= mdep_element[4U];

        mdep_role = mdep_element[6U];

        LOG_DEBUG("\n\t[ID]: %02X\n\t[Type]: (%04X) %s\n\t[Role]: %s\n",
               mdep_id, mdep_type, appl_hdp_get_device_data_type(mdep_type),
              (0U == mdep_role) ? "Source" : "Sink");

        /* Not printing MDEP description */
    }

    return;
}


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
           )
{
    UCHAR * data;
    UINT16 length;
    API_RESULT retval;
    UCHAR  mdep_element_offset, index;
    UINT32 mdep_element_length;

     /* Initialize */
    data = attribute_data;
    length = 0xFFFFU;
    retval = API_SUCCESS;

    if ((NULL == attribute_data) || (NULL == mdep) || (NULL == num_mdeps))
    {
        LOG_DEBUG ("[***ERR***] NULL value not accpeted\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     SUPPORTED_FEATURES,
                     data,
                     mdep,
                     &length
                 );

        if (API_SUCCESS == retval)
        {
            index = 0U;

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            mdep_element_offset = 0U;
            mdep_element_length = 0U;

            while (length > 0U)
            {
                /* Get MDEP Element */
                retval = BT_sdp_get_data_element
                         (
                             mdep,
                             length,
                             &mdep_element_offset,
                             &mdep_element_length
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("Parsing MDEP ID Failed. Reason 0x%04X\n", retval);
                    break;
                }

                LOG_DEBUG("\nMDEP Element [%d]:", index);

                appl_print_mdep_element
                (
                    &mdep[mdep_element_offset],
                    mdep_element_length
                );

                /* Mode to next mdep element */
                mdep += (mdep_element_offset + mdep_element_length);
                length -= (UINT16)(mdep_element_offset + mdep_element_length);
                index ++;
            }

            LOG_DEBUG("\n");
        }
    }

    return retval;
}


/* Function is registered with SDP handles SDP events */
void hdp_sdp_cb
     (
          /* IN */ UCHAR command,
          /* IN */ UCHAR * data,
          /* IN */ UINT16 length,
          /* IN */ UINT16 status
     )
{
    API_RESULT retval;

    S_UUID     uuid[3U];
    UCHAR      num_uuids                = 1U;

    UINT16     attrib_ids[4U];
    UINT16     num_attribs              = 0U;

    UINT16     len_attrib_data          = sizeof(hdp_attribute_value);
    UINT16     control_channel_psm, data_channel_psm;
    UINT8      index, num_mdeps;
    UCHAR      * mdep_list;

    if(API_SUCCESS != status)
    {
        LOG_DEBUG ("SDP Operation Failed.\n");
        /* return; */
    }
    else
    {
        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        BT_mem_set(uuid, 0, sizeof(S_UUID) * 3U);
        BT_mem_set(attrib_ids, 0, sizeof(UINT16) * 4U);

        switch(command)
        {
            case SDP_Open : /* SDP open callback */
                uuid[0U].uuid_type = UUID_16;
                uuid[1U].uuid_type = UUID_16;
                uuid[2U].uuid_type = UUID_16;

                uuid[0U].uuid_union.uuid_16 = 0x1400U;
                uuid[1U].uuid_union.uuid_16 = 0x001EU;
                uuid[2U].uuid_union.uuid_16 = 0x001FU;


                /* Service Class Id List */
                attrib_ids[0U] = SERVICE_CLASS_ID_LIST;

                /* Attribute ID for Protocol Descriptor List */
                attrib_ids[1U] = PROTOCOL_DESC_LIST;

                /* Attribute ID for Additional Protocol Descriptor List */
                attrib_ids[2U] = ADDITIONAL_PROT_DESC_LIST_ID;

                /* Attribute ID for HDP Supported Features */
                attrib_ids[3U] = 0x0200U;

                num_attribs = 4U;

                /* Do Service Search Request */
                retval = BT_sdp_servicesearchattributerequest
                         (
                             &hdp_sdp_handle,
                             uuid,
                             num_uuids,
                             attrib_ids,
                             num_attribs,
                             NULL,
                             0x00U,
                             hdp_attribute_value,
                             &len_attrib_data
                         );

                if(API_SUCCESS != retval)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close (&hdp_sdp_handle);
                }
                break;

            case SDP_Close: /* SDP Close callback */
                LOG_DEBUG ("SDP Closed.\n");
                break;

            case SDP_ServiceSearchAttributeResponse:
                /* Extract SDP role */
                num_attribs = 3U;
                LOG_DEBUG ("\nPeer's HDP Information:\n");
                if (API_SUCCESS != BT_sdp_get_ServiceClassIDList
                                   (
                                       data,
                                       uuid,
                                       &num_attribs
                                   ))
                {
                    LOG_DEBUG ("[***ERR***]Failed to find HDP role\n");
                    appl_dump_bytes (data,length);
                }
                else
                {
                    LOG_DEBUG ("HDP Roles:");
                    for (index = 0U ; index < num_attribs ; index++)
                    {
                        if (UUID_16 == uuid[index].uuid_type)
                        {
                            if (0x1401U == uuid[index].uuid_union.uuid_16)
                            {
                                LOG_DEBUG ("HDP Source ");
                            }
                            if (0x1402U == uuid[index].uuid_union.uuid_16)
                            {
                                LOG_DEBUG ("HDP Sink");
                            }
                        }
                        else
                        {
                            LOG_DEBUG ("UUID = %04X\n",uuid[index].uuid_union.uuid_16);
                        }
                    }
                    LOG_DEBUG ("\n");
                }

                /* Extract Control Channel PSM */
                if ( API_SUCCESS != BT_sdp_get_psm (data, &control_channel_psm))
                {
                    LOG_DEBUG ("Failed to find control channel PSM\n");
                    break;
                }

                LOG_DEBUG ("Control Channel PSM = 0x%04X\n", control_channel_psm);

                /* Extract Data Channel PSM */
                if ( API_SUCCESS != BT_sdp_get_additional_psm
                                    (
                                        data,
                                        &data_channel_psm
                                    ))
                {
                    LOG_DEBUG ("Failed to find data channel PSM\n");
                    break;
                }

                LOG_DEBUG ("Data Channel PSM = 0x%04X\n", data_channel_psm);
                mdep_list = BT_alloc_mem (length);
                /* Extract MDEP ID's */
                if (API_SUCCESS != appl_sdp_get_hdp_supported_features
                                   (
                                        data,
                                        mdep_list,
                                        &num_mdeps
                                   ))
                {
                    LOG_DEBUG ("Failed to find MDEP ID\n");
                }

                BT_free_mem (mdep_list);
                (BT_IGNORE_RETURN_VALUE) BT_sdp_close ( &hdp_sdp_handle );

                break;

            case SDP_ErrorResponse:
                break;

            default: /* Invalid: Nothing to do */
                break;
        }
    }

    return;
}


/* Callback registered with HCI */
API_RESULT hdp_appl_hci_event_ind_cb
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           )
{
    UINT16 connection_handle, value_2;
    UCHAR  status, value_1, link_type;

    switch (event_type)
    {
        case HCI_CONNECTION_COMPLETE_EVENT:
            /* Status */
            hci_unpack_1_byte_param(&status, event_data);
            event_data += 1U;

            /* Connection Handle */
            hci_unpack_2_byte_param(&connection_handle, event_data);
            event_data += 8U;

            /* Link Type */
            hci_unpack_1_byte_param(&link_type, event_data);

#ifdef  HDP_CLOCK_SYNC
            /* Save ACL Connection Handle */
            if ((0x00U == status) &&
                (HCI_ACL_LINK == link_type))
            {

                appl_clk_hdp_handle = connection_handle;
            }
#endif  /* HDP_CLOCK_SYNC */
            break;

        case HCI_COMMAND_COMPLETE_EVENT:
            /* Number of Command Packets */
            hci_unpack_1_byte_param(&value_1, event_data);
            event_data += 1U;

            /* Command Opcode */
            hci_unpack_2_byte_param(&value_2, event_data);
            event_data += 2U;

            /* Command Status */
            hci_unpack_1_byte_param(&status, event_data);
            event_data += 1U;

#ifdef HDP_CLOCK_SYNC
            /* Command Return Parameters */
            if (4U < event_datalen)
            {
                if (HCI_READ_CLOCK_OPCODE == value_2)
                {
                     /* Extract Connection handle */
                    hci_unpack_2_byte_param (&connection_handle,event_data);
                    LOG_DEBUG ("Connection handle = %04X\n", connection_handle);

                    if ((0x00U == status) &&
                        (appl_clk_hdp_handle == connection_handle))
                    {
                        /* Extract clock value */
                        hci_unpack_4_byte_param(&g_bt_clk_value,&event_data[2U]);
                        LOG_DEBUG ("Bluetooth clock value = %08X\n", g_bt_clk_value);

                        /* Extract accuracy of the clock */
                        hci_unpack_2_byte_param(&value_2,&event_data[6U]);
                        LOG_DEBUG ("Accuracy = %04X\n", value_2);

                        if (1U == send_sync_set_rsp)
                        {
                            (BT_IGNORE_RETURN_VALUE) hdp_appl_send_sync_set_rsp (&g_bt_clk_value, &value_2);
                        }
                        else if (1U == send_sync_info_ind)
                        {
                            (BT_IGNORE_RETURN_VALUE) hdp_appl_send_sync_info_ind (&g_bt_clk_value, &value_2);
                        }
                        else
                        {
                            /* MISRA C-2012 Rule 15.7 */
                        }
                    }
                }
            }
#endif /* HDP_CLOCK_SYNC */
            break;

        case HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT:
            break;

        default:
            /* LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type); */
            break;
    }

    BT_IGNORE_UNUSED_PARAM(value_1);

    return API_SUCCESS;
}


/* The main HDP operations function */
void main_hdp_operations (void)
{
    int choice, menu_choice, val, ctrl_chnl_id, data_chnl_id;
    int num_mdeps, invalid_role_flag, j;
    API_RESULT retval;
#ifdef HDP_CLOCK_SYNC
    UCHAR    cmd[13U];
    UCHAR    cmd_req[2U];
    int      ts_req_accur;
    long long int   lval;
#endif  /* HDP_CLOCK_SYNC */

    UCHAR mdep_roles, mdep_role;
    UINT16 data_chnl_psm, index, mdep_data_type, appl_data_chnl_id;
    UINT16 num_bytes, supported_features_index, appl_cntrl_chnl_id;

    UINT16 connection_handle;

    HDP_MD_PARAMS md_params;
    HDP_MEP mep_info;
    /* For Updating Class of Device */
    UINT32 hdp_CoD;

    HDP_HANDLE hdp_handle;
    HDP_MC_CONNECT_PARAMS mc_params;


#ifdef SDP_INPUT

    UINT16               attr_id;
    UINT32               attr_val_len;

#endif /* SDP_INPUT */

    BT_mem_set(&hdp_handle, 0U, sizeof(HDP_HANDLE));
    appl_cntrl_chnl_id = 0U;
    if (0U == initial_flag)
    {
        initial_flag = 1U;

        /* Initialize hdp application once */
        hdp_appl_init ();

        /* Register callback with HCI */
        retval = appl_hci_register_callback (hdp_appl_hci_event_ind_cb);
        if (retval != API_SUCCESS)
        {
            LOG_DEBUG ("[ERR] Failed to register callback with HCI\n");
        }

#ifdef BT_2_1_EDR
        /* Write Enhanced Inquiry mode */
        retval = BT_hci_write_inquiry_mode (0x02U);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("[***ERR***]: Failed to write Inquiry mode, reason %04X\n",
            retval);
        }

        appl_hci_write_extended_inquiry_response ();
#endif /* BT_2_1_EDR */
    }

    BT_LOOP_FOREVER()
    {
        fflush(stdout);
        LOG_DEBUG ("%s",hdp_options);
        scanf ("%d",&choice);
        invalid_role_flag = 0;
        menu_choice = choice;

        if (((choice >= 12U) && (choice < 23U)) || (50U == choice) || (51U == choice))
        {
            hdp_appl_display_chnls ();
            LOG_DEBUG ("Enter the Control Channel Id\n");
            scanf ("%x",&ctrl_chnl_id);

            retval = hdp_appl_get_cntrl_chnl_id
                     (
                         (UINT16) ctrl_chnl_id,
                         &appl_cntrl_chnl_id
                     );
            if (retval != API_SUCCESS)
            {
                continue;
            }

            HDP_INIT_HANDLE
            (
                hdp_handle,
                cntrl_chnls[appl_cntrl_chnl_id].bd_addr
            );

            hdp_handle.mcl_id = (UINT16)ctrl_chnl_id;

            if ((choice >= 14U && choice <= 19U && choice != 17U) || (50U == choice) || (51U == choice))
            {
                LOG_DEBUG("Enter the Data Channel Id\n");
                scanf("%x",&data_chnl_id);
                hdp_handle.mdl_id = (UINT16)data_chnl_id;
            }
        }

        switch(choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            break;

        case 2: /* Write Class of Device */
             LOG_DEBUG("Enter CoD in Hex\n");
             scanf("%x", &val);
             hdp_CoD = (UINT32)val;

             LOG_DEBUG("Setting HDP Class of Device as 0x%06X\n", hdp_CoD);

             retval = BT_hci_write_class_of_device (hdp_CoD);

             LOG_DEBUG("Class of device request sent with result 0x%04X\n",
             retval);

             break;

        case 3: /* Register BD_ADDR of peer device */
            LOG_DEBUG("Please enter BD ADDR of peer HDP\n");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_hdp_peer_bd_addr);
            break;

        case 4:
            /*
             * First Application should establish ACL Connection with
             * the peer Device
             */
            retval = BT_hci_create_connection
                     (
                         appl_hdp_peer_bd_addr,
                         LMP_ACL_DH5,
                         0x0U, 0x0U, 0x0U, 0x1U
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 5:
            /* To disconnect read the ACL Connection Handle */
            LOG_DEBUG("Enter Connection Handle (in Hex) = "); fflush(stdout);
            scanf("%x", &val);

            connection_handle = (UINT16)val;

            LOG_DEBUG("\n");
            LOG_DEBUG("Starting HCI Disconnection ... "); fflush(stdout);

            /* Mention Reason for Disconnect as User Initiated Disconnection */
            retval = BT_hci_disconnect ((UINT16)connection_handle, 0x13U );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("HCI Disconnection FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 6: /* Perform SDP Query */
             /* Set the SDP Handle */
             SDP_SET_HANDLE
             (
                hdp_sdp_handle,
                appl_hdp_peer_bd_addr,
                hdp_sdp_cb
             );

             retval = BT_sdp_open(&hdp_sdp_handle);
             if(API_SUCCESS != retval)
             {
                 LOG_DEBUG("SDP Open Failed. Reason: 0x%04X\n",
                 retval);
             }
             else
             {
                 LOG_DEBUG("Started SDP Query\n");
             }
             break;

        case 10:
#ifndef SDP_INPUT
            /* Register Application end point with HDP */
            LOG_DEBUG ("Enter Local Control Channel PSM\n");
            scanf ("%x", &val);
            mep_info.control_channel_psm = (UINT16) val;

            /* Assign the value to Protocol Descriptor List */
            hdp_protocol_descriptor_list[9U] =
                (UCHAR)mep_info.control_channel_psm;
            hdp_protocol_descriptor_list[8U] =
                (UCHAR)(mep_info.control_channel_psm >> 8U);

            mep_info.ntf_cb = hdp_appl_cb;
            mep_info.config_cb = hdp_appl_config_cb;

#ifdef MCAP_STANDARD_COMMANDS
            LOG_DEBUG ("Enter Local Data Channel PSM\n");
            scanf ("%x", &val);
            mep_info.data_channel_psm = (UINT16) val;

            /* Assign the value to Additional Protocol Descriptor List */
            hdp_additional_protocol_descriptor_list[11U] =
                (UCHAR)mep_info.data_channel_psm;
            hdp_additional_protocol_descriptor_list[10U] =
                (UCHAR)(mep_info.data_channel_psm >> 8U);
#endif /* MCAP_STANDARD_COMMANDS */

            LOG_DEBUG("Enter the number of Unique MDEP Id's\n");
            scanf("%d",&num_mdeps);

            /*
             * Allocate memory for HDP Supported Features SDP attribute
             *
             * Each Supported Feature - MDEP List will have following mandatory parameters
             * - MDEP ID (UINT 8)
             * - MDEP Data Type (UINT 16)
             * - MDEP Role (UINT 8)
             * And MDEP Description String as optional parameter, which we are right now
             * not keeping in the database
             *
             * The total length of each Supported Feature = (4 + 3) = 7 Octet
             * (Each of the above items contains UINT8 and UINT16 datatype related headers additionally)
             *
             * Additionally each supported feature will have 2 Octet of Data Element Sequence Header
             * And at the top level supported feature itself is also a Data Element Sequence
             *
             * So, the total size of HDP Supported Features attribute will be
             * 2 + num_mdeps * (7 + 2) octets
             *
             * Note: Assumed the length will not cross 256 Octet.
             */
            hdp_supported_features_length = (UINT16)(2U + num_mdeps * (7U + 2U));

            /* Allocate memory for HDP Supported Features */
            hdp_supported_features =
                (UCHAR *)BT_alloc_mem (hdp_supported_features_length);

            if (NULL == hdp_supported_features)
            {
                LOG_DEBUG("Failed to allocate memory for HDP Supported Features\n");
                break;
            }

            /*
             * Fill the HDP Supported Feature Data element Sequence Header.
             */
            supported_features_index = 0U;

            hdp_supported_features[supported_features_index ++] = 0x35U;
            hdp_supported_features[supported_features_index ++] =
                (UCHAR)(num_mdeps * (7U + 2U));

            hdp_CoD = BT_MDC_HEALTH;
            mdep_roles = 0U;
            for (j = 0U;j < num_mdeps;j ++)
            {
                /*
                 * Fill the Data element Sequence Header.
                 */
                hdp_supported_features[supported_features_index ++] = 0x35U;
                hdp_supported_features[supported_features_index ++] = 0x07U;
                LOG_DEBUG("Enter the MDEP Id\n");
                scanf("%02x",&val);

                if (0U != ((val > 0x7fU)))
                {
                    LOG_DEBUG("Invalid MDEP ID provided\n");
                    invalid_role_flag = 1;
                    break;
                }
                appl_mdep[j].mdep_id = (UCHAR) val;
                /* Fill UINT8 value */
                hdp_supported_features[supported_features_index ++] = 0x08U;
                hdp_supported_features[supported_features_index ++] =
                    appl_mdep[j].mdep_id;

                /* MISRA C - 2012 Rule 15.2 */
                /* get_hdp_mdep_data_type: */
                BT_LOOP_FOREVER()
                {
                    /* Enter MDEP Data Type */
                    LOG_DEBUG("Select MDEP Data Type\n");

                    for (index = 0U;
                        index < (sizeof(hdp_mdep_data_type) / sizeof(HDP_MDEP_DATA_TYPE));
                        index++)
                    {
                        LOG_DEBUG("%d: %s\n", index,
                        hdp_mdep_data_type[index].data_type_name);
                    }

                    scanf("%d", &val);

                    /* Validate Data Type */
                    if (val >= index)
                    {
                        LOG_DEBUG("Invalid MDEP Data Type\n");

                        /* MISRA C - 2012 Rule 15.2 */
                        /* goto get_hdp_mdep_data_type; */
                    }
                    /* MISRA C - 2012 Rule 15.2 */
                    else
                    {
                        break;
                    }
                }

                mdep_data_type = hdp_mdep_data_type[val].mdep_data_type;
                hdp_CoD |=  hdp_mdep_data_type[val].health_minor_device_class;

                /* Fill UINT16 value */
                hdp_supported_features[supported_features_index ++] = 0x09U;
                hdp_supported_features[supported_features_index ++] =
                (UCHAR)(mdep_data_type >> 8U);

                hdp_supported_features[supported_features_index ++] =
                (UCHAR)(mdep_data_type);
                LOG_DEBUG("Enter MDEP Role( 0-Source, 1-Sink)\n");
                scanf("%02x",&val);
                appl_mdep[j].mdep_role = (UINT8)val;
                if (appl_mdep[j].mdep_role > 0x1U)
                {
                    LOG_DEBUG("Invalid Role provided\n");
                    invalid_role_flag = 1;
                    break;
                }
                else
                {
                    /* Fill UINT8 value */
                    hdp_supported_features[supported_features_index ++] = 0x08U;
                    hdp_supported_features[supported_features_index ++] =
                        appl_mdep[j].mdep_role;
                }
                /* Set the Major Service Class based on the role */
                if (0x00U == appl_mdep[j].mdep_role)
                {
                    /* Source is a capturing device - loosely */
                    hdp_CoD |= BT_MSC_CAPTURING;

                    mdep_roles |= 0x01U;
                }
                else
                {
                    hdp_CoD |= BT_MSC_RENDERING;

                    mdep_roles |= 0x02U;
                }
            }
            if (0 != invalid_role_flag)
            {
                break;
            }

#else
            /* Get the HDP record handle value from the database*/
            retval = BT_dbase_get_record_handle
                     (
                         DB_RECORD_HDP,
                         0U,
                         &hdp_dbase_rec_handle
                     );
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("Error in getting hdp record handle\n");
                break;
            }

            retval = BT_dbase_activate_record (hdp_dbase_rec_handle);
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("Error in activating hdp record handle\n");
                break;
            }

            /**
             * Get the value for the attribute Protocol Descriptor List
             * attrid = 0x0004
             */
            attr_id = 0x0004U;
            retval = BT_dbase_get_attr_value
                     (
                         hdp_dbase_rec_handle,
                         &attr_id,
                         hdp_protocol_descriptor_list,
                         18U,
                         &attr_val_len
                      );
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("Error in getting hdp_protocol_descriptor_list val\n");
                break;
            }
            /**
             * Extract the Control channel psm from the Protocol Descriptor
             * List attribute
             */
            mep_info.control_channel_psm = hdp_protocol_descriptor_list[8U];
            mep_info.control_channel_psm <<=8U;
            mep_info.control_channel_psm |= hdp_protocol_descriptor_list[9U];

            /**
             * Get the value for the attribute Additional Protocol
             * Descriptor List attrid = 0x000D
             */
            attr_id = 0x000DU;
            retval = BT_dbase_get_attr_value
                     (
                         hdp_dbase_rec_handle,
                         &attr_id,
                         hdp_additional_protocol_descriptor_list,
                         17U,
                         &attr_val_len
                      );
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("Error in getting hdp_additional_protocol_descriptor_list val\n");
                break;
            }
            /**
             * Extract the Data channel psm from the Additional Protocol
             * Descriptor List attribute (attrid = 0x000D)
             */
            mep_info.data_channel_psm =
                hdp_additional_protocol_descriptor_list[10U];
            mep_info.data_channel_psm <<=8U;
            mep_info.data_channel_psm |=
                hdp_additional_protocol_descriptor_list[11U];

            hdp_supported_features =
                (UCHAR *)BT_alloc_mem(256U);
            hdp_supported_features_length = 256U;

            attr_id = 0x0200U;

            retval = BT_dbase_get_attr_value
                     (
                         hdp_dbase_rec_handle,
                         &attr_id,
                         hdp_supported_features,
                         hdp_supported_features_length,
                         &attr_val_len
                     );
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("Error in getting hdp_supported_features val\n");
                break;
            }
            num_mdeps = (hdp_supported_features[1U]/9U);
            supported_features_index = 5U;

            for (j = 0U; j < num_mdeps; j ++)
            {
                mdep[j].mdep_id =
                    hdp_supported_features[supported_features_index];
                supported_features_index += 5U;
                mdep[j].mdep_role =
                    hdp_supported_features[supported_features_index];
                supported_features_index += 4U;
            }
            mdep_roles = 0x03U;
#endif /* SDP_INPUT */

            /* Create HDP Service Class ID List */

            /* If both roles present */
            if (0x03U == mdep_roles)
            {
                hdp_service_class_list_length = 2U + 3U * 2U;
            }
            else
            {
                hdp_service_class_list_length = 2U + 3U;
            }

            hdp_service_class_list =
                (UCHAR *)BT_alloc_mem(hdp_service_class_list_length);

            if (NULL == hdp_service_class_list)
            {
                LOG_DEBUG ("Failed to allocate memory for hdp_service_class_list\n");
                break;
            }

            index = 0U;

            hdp_service_class_list[index ++] = 0x35U;
            hdp_service_class_list[index ++] =
                (UCHAR)(hdp_service_class_list_length - 2U);

            /* If SRC role is supported */
            if (0U != (mdep_roles & 0x01U))
            {
                hdp_service_class_list[index ++] = 0x19U;

                hdp_service_class_list[index ++] = 0x14U;
                hdp_service_class_list[index ++] = 0x01U;
            }

            /* If SINK role is supported */
            if (0U != (mdep_roles & 0x02U))
            {
                hdp_service_class_list[index ++] = 0x19U;

                hdp_service_class_list[index ++] = 0x14U;
                hdp_service_class_list[index] = 0x02U;
            }

            LOG_DEBUG("Setting HDP Class of Device as 0x%06X\n", hdp_CoD);

            retval = BT_hci_write_class_of_device (hdp_CoD);

            LOG_DEBUG("HCI write class of device API returned 0x%04X\n",
            retval);

            retval = BT_hdp_register (&mep_info,&appl_mep_id);

            LOG_DEBUG("HDP register function return value = %04X\n\n",
            retval);

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("\nRegistered Successfully with HDP\n");
                hdp_appl_dbase_register ();
            }
            else
            {
                LOG_DEBUG ("[***ERR***]: Failed to register with HDP, reason %04X",
                    retval);
            }
            break;

        case 11: /* Establish Control Channel */
            LOG_DEBUG("Enter Remote Control channel PSM\n");
            scanf ("%x", &val);
            mc_params.cntrl_ch_psm = (UINT16) val;
            mc_params.mep_id = appl_mep_id;

            /* Populate the HDP handle with the bd_addr */
            HDP_INIT_HANDLE (hdp_handle, appl_hdp_peer_bd_addr);

            retval = BT_hdp_mc_connect_req
                     (
                         &hdp_handle,
                         &mc_params
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Successfully initiated Control Channel Connection "
                "procedure\n");
                LOG_DEBUG ("Wait for MCAP_MC_CREATE_CNF\n");
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:Failed to initiate Control Channel "
                "Connection procedure, reason %04X\n",retval);
            }
            break;

        case 12:
            /* Disconnect Control Channel */
            /* Populate the HDP handle */
            retval = BT_hdp_mc_disconnect_req (&hdp_handle);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]: Successfully initiated Control Channel "
                "Disconnection procedure\n",hdp_handle.mcl_id);
                LOG_DEBUG ("[MCL %04X]:Wait for MCAP_MC_DISCONNECT_CNF\n",
                hdp_handle.mcl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to initiate Control "
                "Channel Disonnection procedure, reason %04X\n",
                hdp_handle.mcl_id,retval);
            }
            break;

        case 13:
            /* Establish Data Channel */
            /* Populate the HDP handle */
            LOG_DEBUG("Enter Local MDEP Id\n");
            scanf("%02x",&val);
            hdp_handle.mdep_id = (UCHAR)val;

            LOG_DEBUG("Enter Remote Data Channel PSM\n");
            scanf ("%x", &val);
            md_params.rem_data_ch_psm = (UINT16) val;

            LOG_DEBUG("Enter Remote MDEP Id\n");
            scanf("%02x",&val);
            md_params.rem_mdep_id = (UCHAR)val;

            LOG_DEBUG("Enter the channel configuration preference\n");
            LOG_DEBUG(" 0 - No Preference\n 1 - Reliable\n 2 - Streaming\n");
            scanf("%02x",&val);
            md_params.channel_preference = (UCHAR)val;

            if (0x00U == hdp_handle.mdep_id && md_params.rem_mdep_id != 0x00U)
            {
                LOG_DEBUG("Remote & Local MDEP Ids must be 0 for echo test\n");
                break;
            }
            if (0x00U == md_params.rem_mdep_id && hdp_handle.mdep_id != 0x00U)
            {
                LOG_DEBUG("Remote& Local MDEP Ids must be 0 for echo test\n");
                break;
            }

            if (!((0x00U == hdp_handle.mdep_id) && (0x00U == md_params.rem_mdep_id)))
            {
                    retval = hdp_appl_get_mdep_role
                             (
                                 hdp_handle.mdep_id,
                                 &mdep_role
                             );

                if (retval != API_SUCCESS)
                {
                    break;
                }

                if ((HDP_SOURCE == mdep_role) && (HDP_NO_PREFERENCE == val))
                {
                    LOG_DEBUG("[***ERR***]:[MCL %04X]:[MDEP %04X]: Invalid Data "
                    "Channel Configuration Preference provided for Source MDEP"
                    "\n",hdp_handle.mcl_id,hdp_handle.mdep_id);
                    break;
                }

                if ((HDP_SINK == mdep_role) && (val != HDP_NO_PREFERENCE))
                {
                    LOG_DEBUG("[***ERR***]:[MCL %04X]:[MDEP %04X]: Invalid Data "
                    "Channel Configuration Preference provided for Sink MDEP"
                    "\n",hdp_handle.mcl_id,hdp_handle.mdep_id);
                    break;
                }

                if ((HDP_STREAMING_CHNL == md_params.channel_preference) &&
                    (0x00U == cntrl_chnls[appl_cntrl_chnl_id].rel_data_chnl_flag))
                {
                    LOG_DEBUG("\n\n[**ERR**][MCL %04X]: Reliable data channel not "
                    "established\n\n",hdp_handle.mcl_id);
                    break;
                }
            }

            retval = BT_hdp_md_create_req (&hdp_handle,&md_params);

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]: Successfully initiated Data Channel "
                "Connection procedure\n",hdp_handle.mcl_id);
                LOG_DEBUG ("[MCL %04X]:Wait for HDP_MD_CREATE_CNF\n",
                hdp_handle.mcl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to initiate Data "
                "Channel Connection procedure, reason %04X\n",
                hdp_handle.mcl_id,retval);
            }
            break;

        case 14:
            /* Write Data on the Data Channel */
            LOG_DEBUG ("[MCL %04X]:[MDL %04X]: Enter Number of bytes to be "
            "Written on Data Channel\n",hdp_handle.mcl_id,hdp_handle.mdl_id);

            scanf ("%d",&val);
            num_bytes = (UINT16)val;

            /* Call the BT_hdp_md_write function */
            retval = BT_hdp_md_write
                     (
                         &hdp_handle,
                         appl_hdp_data,
                         num_bytes
                     );
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]:[MDL %04X]: Successfully initiated Write on"
                "Data Channel\n",hdp_handle.mcl_id,hdp_handle.mdl_id);
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]:Wait for HDP_DATA_WRITE_CNF\n",
                hdp_handle.mcl_id,hdp_handle.mdl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to initiate Data "
                "Channel Connection procedure, reason %04X\n",
                hdp_handle.mcl_id,retval);
            }
            break;

        case 15:
            /* Disconnect Data Channel */
            retval = BT_hdp_md_disconnect_req (&hdp_handle);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]:[MDL %04X]: Successfully initiated Data "
                "Channel Disconnection procedure\n",hdp_handle.mcl_id,
                hdp_handle.mdl_id);
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]:Wait for HDP_MD_DISCONNECT_CNF\n",
                hdp_handle.mcl_id,hdp_handle.mdl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]:[MDL %04X]: Failed to initiate"
                " Data Channel Disonnection procedure, reason %04X\n",
                hdp_handle.mcl_id,hdp_handle.mdl_id,retval);
            }
            break;


        case 16:
            /* Delete Data Channel */
            retval = BT_hdp_md_delete_req (&hdp_handle);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]:[MDL %04X]: Successfully initiated Data "
                "Channel Delete procedure\n",hdp_handle.mcl_id,
                hdp_handle.mdl_id);
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]: Wait for HDP_MD_DELETE_CNF\n",
                hdp_handle.mcl_id,hdp_handle.mdl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]:[MDL %04X]: Failed to initiate"
                " Data Channel Delete procedure, reason %04X\n",
                hdp_handle.mcl_id,hdp_handle.mdl_id,retval);
            }
            break;

        case 17:
            /* Delete all Data Channels */

            retval = BT_hdp_md_delete_all_req (&hdp_handle);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]: Successfully initiated Delete ALL "
                "Data Channel procedure\n",hdp_handle.mcl_id);
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]:Wait for "
                "HDP_MD_DELETE_ALL_CNF\n",hdp_handle.mcl_id,
                hdp_handle.mdl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to initiate"
                "Delete ALL Data Channel procedure, reason %04X\n",
                hdp_handle.mcl_id,retval);
            }
            break;

        case 18:
            /* Reconnect data channel */
            LOG_DEBUG ("Enter Remote Data Channel PSM\n");
            scanf ("%x",&val);

            data_chnl_psm = (UINT16)(val);

            retval = hdp_appl_get_data_chnl_id
                     (
                         appl_cntrl_chnl_id,
                         hdp_handle.mdl_id,
                         &appl_data_chnl_id
                     );

            if (API_SUCCESS == retval)
            {
                if (0x00U == cntrl_chnls[appl_cntrl_chnl_id].rel_data_chnl_flag)
                {
                    if (HDP_STREAMING_CHNL == cntrl_chnls[appl_cntrl_chnl_id].mcap_mdl_config[appl_data_chnl_id])
                    {
                         LOG_DEBUG("\n\n[**ERR**][MCL %04X]: Reliable data "
                         "channel not established\n\n",hdp_handle.mcl_id);
                        break;
                    }
                }

                retval = BT_hdp_md_reconnect_req
                         (
                             &hdp_handle,
                             &data_chnl_psm
                          );
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("[MCL %04X]:[MDL %04X]: Successfully initiated "
                    "Reconnect procedure\n",hdp_handle.mcl_id,
                    hdp_handle.mdl_id);
                    LOG_DEBUG ("[MCL %04X]:[MDL %04X]:Wait for "
                    "HDP_MD_RECONNECT_CNF\n",hdp_handle.mcl_id,
                    hdp_handle.mdl_id);
                }
                else
                {
                    LOG_DEBUG ("[***ERR***]:[MCL %04X]:[MDL %04X]: Failed to "
                    "initiate Reconnect procedure, reason %04X\n",
                    hdp_handle.mcl_id,hdp_handle.mdl_id,retval);
                }
            }
            break;

        case 19:
            /* Abort Data Channel */
            retval = BT_hdp_md_abort_req (&hdp_handle);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]:[MDL %04X]: Successfully initiated "
                "Abort procedure\n",hdp_handle.mcl_id,hdp_handle.mdl_id);
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]:Wait for "
                "HDP_MD_ABORT_CNF\n",hdp_handle.mcl_id, hdp_handle.mdl_id);
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]:[MDL %04X]: Failed to "
                "initiate Abort procedure, reason %04X\n",
                hdp_handle.mcl_id,hdp_handle.mdl_id,retval);
            }
            break;

#ifdef HDP_CLOCK_SYNC
        case 21:
            /* Send sync capabiltiy request (As Sync Master) */
            LOG_DEBUG ("[MCL %04X]: Enter the Required Accuracy of Timestamp in "
            "second (ppm) (hex)\n", hdp_handle.mcl_id);
            scanf ("%x", &ts_req_accur);

            HDP_WRITE_TIMESTAMP_REQUIRED_ACCURACY(cmd_req,\
                (UINT16)ts_req_accur);

            retval = BT_hdp_sync_cap_req (&hdp_handle, cmd_req);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]: Successfully initiated Synchronization "
                "Capability Request\n",hdp_handle.mcl_id);
                LOG_DEBUG ("[MCL %04X]:Wait for HDP_SYNC_CAP_RSP\n",
                hdp_handle.mcl_id);
                break;
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to "
                "initiate Synchronization Capability Request, reason %04X\n",
                hdp_handle.mcl_id,retval);
            }
            break;

        case 22:
            /* Send set time stamp  request (As Sync Master) */
            LOG_DEBUG ("[MCL %04X]:Request Timestamp Update Information?\n",
            hdp_handle.mcl_id);

            LOG_DEBUG ("0. No\n1. Yes\n");
            scanf ("%d",&val);

            if (1U < val)
            {
                LOG_DEBUG ("Invalid Option, please try again\n");
                break;
            }
            HDP_WRITE_TIMESTAMP_UPDATE_INFORMATION (cmd, (UCHAR)val);

            retval = BT_hci_read_clock (1U, appl_clk_hdp_handle);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("Wait for HCI_READ_CLOCK for the current BT Clock\n");
                /* Sleep (100); */
                BT_sleep(1U);
            }
            LOG_DEBUG ("Enter the synchronization BT Clock (in hex)- Most signf "
            "4-bits shall be 0\n");
            scanf ("%x",&val);

            if ((val != (int)0xFFFFFFFFU) && (val & (int)0xF0000000U))
            {
                LOG_DEBUG ("[***ERR***]:Invalid BT Clock, Most signf 4-bits shall be 0\n");
                break;
            }

            HDP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME ((&cmd[1U]), (UINT32)val);

            LOG_DEBUG ("Enter Timestamp Clock Instant(in hex)\n");
            scanf ("%llx", &lval);

            HDP_WRITE_TIMESTAMP_SYNC_TIME ((&cmd[5U]),((UINT64)lval));

            retval = BT_hdp_sync_set_req
                     (
                         &hdp_handle,
                         cmd
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("[MCL %04X]: Successfully initiated Synchronization "
                "Request\n",hdp_handle.mcl_id);

                LOG_DEBUG ("[MCL %04X]:Wait for HDP_SYNC_SET_RSP\n",
                hdp_handle.mcl_id);
                break;
            }
            else
            {
                LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to "
                "initiate Synchronization Set Request, reason %04X\n",
                hdp_handle.mcl_id,retval);
            }
            break;

        case 23:
            /* Stop sending sync_info indications */
            send_sync_info_ind = 0x00U;
            if (BT_TIMER_HANDLE_INIT_VAL != appl_hdp_timer_handle1)
            {
                retval = BT_stop_timer (appl_hdp_timer_handle1);

                if (retval != API_SUCCESS)
                {
                    LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to stop sneding "
                    "Synchronization Update Indications\n",hdp_handle.mcl_id);
                }
                else
                {
                    LOG_DEBUG ("[MCL %04X]: Successfully stopped sneding "
                    "Snchronization Udpates\n",hdp_handle.mcl_id);
                }
            }
            else
            {
                LOG_DEBUG ("[MC %04X]: No Information Updates being sent\n",
                hdp_handle.mcl_id);
            }
            break;

#endif /* HDP_CLOCK_SYNC */

        case 30:
            /* Set Streaming chnl Params for Response sent by Source */
            appl_ch_params = &appl_strm_ch_params;
            LOG_DEBUG ("\nData Streaming channel preferred\n");
            break;

        case 31:
            /* Set Reliable chnl Params for Response sent by Source */
            appl_ch_params = &appl_rlbl_ch_params;
            LOG_DEBUG ("\nData Reliable channel preferred\n");
            break;

        case 50: /* Send Echo Request */
            LOG_DEBUG ("[MCL %04X]:[MDL %04X]: Sending Echo Req of %d bytes\n",
            hdp_handle.mcl_id,hdp_handle.mdl_id, sizeof(appl_hdp_echo_req_payload));

            (BT_IGNORE_RETURN_VALUE) appl_hdp_send_echo_request
            (
                &hdp_handle,
                appl_hdp_echo_req_payload,
                sizeof(appl_hdp_echo_req_payload)
            );
            break;

        case 51: /* Send Echo Request */
            LOG_DEBUG ("[MCL %04X]:[MDL %04X]: Sending Echo Req of %d bytes\n",
            hdp_handle.mcl_id,hdp_handle.mdl_id, sizeof(appl_hdp_echo_req_incorrect_payload));
            (BT_IGNORE_RETURN_VALUE) appl_hdp_send_echo_request
            (
                &hdp_handle,
                appl_hdp_echo_req_incorrect_payload,
                sizeof(appl_hdp_echo_req_incorrect_payload)
            );
            break;

        default:
            LOG_DEBUG("Unkown Choice - Try again\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}


void hdp_appl_config_cb
     (
         /* IN */ HDP_HANDLE              * handle,
         /* IN */  UCHAR                  channel_prf,
         /* OUT */ L2CAP_CONFIG_OPTION    **l2cap_config
     )
{
#ifndef UPF_PTS_30

    if (MCAP_INVALID_MDL_ID == handle->mdl_id)
    {
        LOG_DEBUG ("[MCL %04X]: Providing L2CAP Config for Control Channel\n",
        handle->mcl_id);
        *l2cap_config = &appl_cntrl_l2cap_config;
    }
    else
    {
        switch (channel_prf)
        {
            case L2CAP_CONFIG_PREF_ERTM_ONLY:
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]:[MDEP %02X]: Providing ERTM "
                "L2CAP Config for Data Channel\n",handle->mcl_id,
                handle->mdl_id,handle->mdep_id);
                *l2cap_config = &appl_rlbl_data_l2cap_config;
                break;

            case L2CAP_CONFIG_PREF_SM_ONLY:
                LOG_DEBUG ("[MCL %04X]:[MDL %04X]:[MDEP %02X]: Providing "
                "STREAMING L2CAP Config for Data Channel\n",handle->mcl_id,
                handle->mdl_id,handle->mdep_id);
                *l2cap_config = &appl_strm_data_l2cap_config;
                break;

            default:
                LOG_DEBUG ("[***ERR***]:[MCL %04X]:[MDL %04X]:[MDEP %02X]: "
                "Received Unkown Prefernce\n",handle->mcl_id,
                handle->mdl_id,handle->mdep_id);
                *l2cap_config =  &appl_dflt_ch_l2cap_params;
                break;
        }
    }
#else /* UPF_PTS_30 */
    *l2cap_config =  &appl_dflt_ch_l2cap_params;
#endif /* UPF_PTS_30 */
}

API_RESULT appl_hdp_send_echo_request
           (
               /* IN */ HDP_HANDLE * hdp_handle,
               /* IN */ UCHAR      * echo_payload,
               /* IN */ UINT16       echo_payload_len
           )
{
    API_RESULT retval;

    /* Write Data on the Data Channel */
    retval = BT_hdp_md_write
             (
                 hdp_handle,
                 echo_payload,
                 echo_payload_len
             );
    if (API_SUCCESS == retval)
    {
        /* Set Waiting for Echo Response Flag */
        appl_hdp_waiting_for_echo_rsp = BT_TRUE;

        LOG_DEBUG("[MCL %04X]:[MDL %04X]: Successfully initiated Write on"
        "Data Channel\n",hdp_handle->mcl_id,hdp_handle->mdl_id);
        LOG_DEBUG ("[MCL %04X]:[MDL %04X]:Wait for HDP_DATA_WRITE_CNF\n",
        hdp_handle->mcl_id,hdp_handle->mdl_id);
    }
    else
    {
        LOG_DEBUG ("[***ERR***]:[MCL %04X]: Failed to initiate Data "
        "Channel Connection procedure, reason %04X\n",
        hdp_handle->mcl_id,retval);
    }
    return retval;
}

#endif /* HDP */

