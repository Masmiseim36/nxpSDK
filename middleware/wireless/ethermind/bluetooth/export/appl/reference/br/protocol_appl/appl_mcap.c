
/**
 *  \file appl_mcap.c
 *
 *  Source File for MCAP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_mcap.h"
#include "appl_utils.h"

#ifdef MCAP

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */

#define APPL_DATA_CHANNEL_MTU   672U

/**
 * Resolution (in half baseband slots) of the bluetooth clock that can be
 * accessed by the implemntation. 0x00 implies, implementaion has no access
 * to bluetooth clock.
 * This information is sent by the sync slave to sync master, in response to
 * a MD_SYNC_CAP_REQ from the sync master.
 */
#define APPL_BLUETOOTH_CLOCK_ACCESS_RESOLUTION  (0x12U)

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

#ifdef MCAP_CLOCK_SYNC
DECL_STATIC UCHAR appl_mcap_read_clock_in_progress;
#endif /* MCAP_CLOCK_SYNC */


static const char main_mcap_options[] = "\n\
========= MCAP Menu =============== \n\
    0.  Exit. \n\
    1.  Refresh \n\
 \n\n\
    2.  MCAP Registration \n\
 \n\n\
    10. Connect MCAP Control Channel \n\
    11. Disconnect MCAP Control Channel \n\
 \n\n\
    20. Create Data Channel\n\
    21. Disconnect Data Channel\n\
    22. Delete Data Channel\n\
    13. Delete all data channels\n\
    23. Reconnect Data Channel\n\
    24. Abort Data Channel\n\
    25. Write Data on Data Channel\n\
\n\
    26. Set Streaming Channel Param for accept\n\
    27. Set Reliable Channel Params for accept\n\
    28. Set No Channel Preference Params for accept\n\
    30. Set Reject MDL code\n\n\
    31. Set send response flag\n\
    32. Reset send response flag\n\
\n\n\
    40. Send Invalid Cmd Req\n\
\
    41. Send Sync Cap Req\n\
    42. Send Sync Cap Rsp\n\
    43. Send Sync Set Req\n\
    44. Send Sync Set Rsp\n\
    45. Send Sync Info Ind\n\
    46. Read Piconet clock\n\
\
\n\
Your Option ?";

DECL_STATIC L2CAP_FEC_OPTION data_strming_ch_fec_params;
DECL_STATIC L2CAP_FEC_OPTION data_rlbl_ch_fec_params;
DECL_STATIC L2CAP_FEC_OPTION data_bsc_ch_fec_params;
DECL_STATIC MCAP_MD_PARAMS appl_strm_ch_params;
DECL_STATIC MCAP_MD_PARAMS appl_rlbl_ch_params;
DECL_STATIC MCAP_MD_PARAMS appl_dflt_ch_params;
DECL_STATIC MCAP_MD_PARAMS *appl_ch_params;

DECL_STATIC L2CAP_CONFIG_OPTION appl_cntrl_ch_l2cap_params;
DECL_STATIC L2CAP_CONFIG_OPTION appl_strm_ch_l2cap_params;
DECL_STATIC L2CAP_CONFIG_OPTION appl_rlbl_ch_l2cap_params;
DECL_STATIC L2CAP_CONFIG_OPTION appl_dflt_ch_l2cap_params;


#define APPL_MCAP_DATA_SIZE    1000U

DECL_STATIC UCHAR appl_mcap_data[APPL_MCAP_DATA_SIZE];
DECL_STATIC UINT16 accept_mdl;

DECL_STATIC UCHAR appl_clk_cmd[5U];
DECL_STATIC const UINT16 appl_clk_cmd_len[5] = {0x03, 0x09, 0x0E, 0x10, 0x0F};
DECL_STATIC UCHAR count = 0;
DECL_STATIC UCHAR send_md_create_rsp = BT_TRUE;


DECL_STATIC L2CAP_FEC_OPTION control_channel_fec_params;
DECL_STATIC L2CAP_FLOW_SPEC control_channel_flow_spec;

DECL_STATIC UCHAR mep_id = 0xFF;

static const char config_menu[] = "\n\
========= Config Menu ===============\n\
     0. No Preference\n\
     1. Streaming Channel\n\
     2. Reliable Channel\n\
\n\
Your Option ?\0";

DECL_STATIC MCAP_HANDLE appl_mcap_handle;


/* ----------------------------------------- Functions */
void main_mcap_operations (void)
{
    int choice, menu_choice;

    BT_LOOP_FOREVER()
    {
        printf ("%s", main_mcap_options); fflush(stdout);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            appl_mcap_register();
#ifdef MCAP_STANDARD_COMMANDS
            appl_initialize_data_channel_options ();
#endif /* MCAP_STANDARD_COMMANDS */
            break;

        case 10:
            appl_mcap_mc_connect();
            break;

        case 11:
            appl_mcap_mc_disconnect ();
            break;

#ifdef MCAP_STANDARD_COMMANDS
        case 20:
            appl_mcap_md_create ();
            break;

        case 21:
            appl_mcap_md_disconnect ();
            break;

        case 22:
            appl_mcap_md_delete ();
            break;

        case 13:
            appl_mcap_md_delete_all ();
            break;

        case 23:
            appl_mcap_md_reconnect ();
            break;

        case 24:
            appl_mcap_md_abort ();
            break;

        case 25:
            appl_mcap_md_write ();
            break;

        case 26:
            appl_ch_params = &appl_strm_ch_params;
            break;

        case 27:
            appl_ch_params = &appl_rlbl_ch_params;
            break;

        case 28:
            appl_ch_params = &appl_dflt_ch_params;
            break;

        case 30:
            {
                int val;

                printf ("Enter Reject code\n");
                scanf ("%x",&val);

                accept_mdl = (UINT16)val;
            }
            break;

        case 31:
            {
                send_md_create_rsp = BT_TRUE;
            }
            break;

        case 32:
            {
                send_md_create_rsp = BT_FALSE;
            }
            break;

#endif /* MCAP_STANDARD_COMMANDS */
        case 40:
            appl_mcap_send_invalid_cmd ();
            break;

#ifdef MCAP_CLOCK_SYNC

        case 41:
            appl_mcap_sync_cap_req ();
            break;
        case 42:
            appl_mcap_sync_cap_rsp ();
            break;
        case 43:
            appl_mcap_sync_set_req ();
            break;
        case 44:
            appl_mcap_sync_set_rsp ();
            break;
        case 45:
            appl_mcap_sync_info_ind ();
            break;
        case 46:
            appl_hci_read_clk ();
            break;

#endif /* MCAP_CLOCK_SYNC */

        default:
            printf ("Enter a valid entry\n");
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


API_RESULT mcap_cb
           (
               MCAP_HANDLE    * handle,
               UCHAR          mcap_event,
               UINT16         response,
               void           * event_param,
               UINT16         event_len
           )
{
    API_RESULT   retval;
    UINT32       index;

    switch (mcap_event)
    {
    case MCAP_MC_CREATE_IND:
    {
        UINT16    psm;

        psm = *(UINT16 *)event_param;

        printf ("MCAP_MC_CREATE_IND on PSM 0x%04X\n",psm);
        retval = BT_mcap_mc_connect_rsp (handle,API_SUCCESS);
        printf ("Send connect rsp status = %04X\n",retval);
        printf ("Wait for MCAP_MC_CONNECT_CNF in case of success\n");
        break;
    }

    case MCAP_MC_CREATE_CNF:
        printf ("MCAP_MC_CREATE_CNF with result %04X\n",response);
        if (API_SUCCESS == response)
        {
            UCHAR   * bd_addr;
            bd_addr = appl_mcap_handle.bd_addr;
            BT_mem_copy (&appl_mcap_handle,handle,sizeof(MCAP_HANDLE));
            printf ("Remote " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
            printf ("MCL ID %04X\n",appl_mcap_handle.mcl_id);
        }
        break;

    case MCAP_MC_DISCONNECT_IND:
        printf ("MCAP_MC_DISCONNECT_IND\n");
        /* appl_mcap_handle.mcl_id = 0xFFFF; */
        break;

    case MCAP_MC_DISCONNECT_CNF:
        printf ("MCAP_MC_DISCONNECT_CNF with result %04X\n",response);
        /* appl_mcap_handle.mcl_id = 0xFFFF; */
        break;
#ifdef MCAP_STANDARD_COMMANDS
    case MCAP_MD_CREATE_IND:
    {
        UCHAR    * rsp_param;
        UINT16   rsp_paramlen;

        printf ("MCAP_MD_CREATE_IND with config %02X, mdl id %04X\n",
            ((UCHAR *)event_param)[0U],handle->mdl_id); fflush(stdout);

        appl_mcap_handle = *handle;

        if (BT_FALSE != send_md_create_rsp)
        {
            rsp_param = (UCHAR *)&appl_ch_params->channel_preference;
            rsp_paramlen = sizeof (UCHAR);

            if (API_SUCCESS == accept_mdl)
            {
                printf ("Accepting MDL %04X with ch prf %04X rqstd %04X\n",
                handle->mdl_id,appl_ch_params->channel_preference,
                ((UCHAR *)event_param)[0U]);
            }
            else
            {
                printf ("Rejecting MDL %04X with Response code %04X, ch prf "
                "%04X rqstd %04X\n",handle->mdl_id, accept_mdl,
                appl_ch_params->channel_preference,((UCHAR *)event_param)[0U]);
            }

            retval = BT_mcap_md_create_rsp
                     (
                         &appl_mcap_handle,
                         accept_mdl,
                         rsp_param,
                         rsp_paramlen
                     );

            printf ("Response sent with retval %04X\n",retval); fflush(stdout);
            break;
        }
        else
        {
            printf ("Not sending any response for MDL %04X\n",handle->mdl_id);
        }
    }
    break;

    case MCAP_MD_CREATE_CNF:
    {
        printf("MCAP_MD_CREATE_CNF with result %04X received\n", response);
        if (API_SUCCESS == response)
        {
            printf("Configuration = %02X, MDL ID = %04x\n", ((UCHAR *)event_param)[0U],
                handle->mdl_id);
            appl_mcap_handle = *handle;
        }
    }
    break;

    case MCAP_MD_RECONNECT_IND:
    {
        printf ("MCAP_MD_RECONNECT_IND for MDL id %04X\n",handle->mdl_id);
        printf ("Sending response %04X to peer\n",accept_mdl);

        retval = BT_mcap_md_reconnect_rsp
                 (
                     handle,
                     accept_mdl
                 );
        printf ("Sent response with retval %04X\n",retval);

    }
    break;

    case MCAP_MD_RECONNECT_CNF:
        printf ("MCAP_MD_RECONNECT_CNF with result %04X received for MDL:"
                "%04X\n",response,handle->mdl_id);
        break;
    case MCAP_MD_ABORT_IND:
        printf ("MCAP_MD_ABORT_IND for MDL id %04X\n",handle->mdl_id);
        break;
    case MCAP_MD_ABORT_CNF:
        printf ("MCAP_MD_ABORT_CNF with result %04X received for MDL: %04X\n",
               response,handle->mdl_id);
        break;
    case MCAP_MD_DELETE_IND:
        printf ("MCAP_MD_DELETE_IND for MDL id %04X\n",handle->mdl_id);
        break;
    case MCAP_MD_DELETE_CNF:
        printf ("MCAP_MD_DELETE_CNF with result %04X received\n",response);
        break;
    case MCAP_MD_DISCONNECT_IND:
        printf ("MCAP_MD_DISCONNECT_IND for MDL id %04X\n",handle->mdl_id);
        break;
    case MCAP_MD_DISCONNECT_CNF:
        printf ("MCAP_MD_DISCONNECT_CNF with result %04X received for MDL"
                " %04X\n",response,handle->mdl_id);
        break;
    case MCAP_DATA_IND:
        printf ("MCAP_MD_DATA_IND on MDL %04X MCL %04X MDEP %04X data "
        "length %04X\n",handle->mdl_id,handle->mcl_id,handle->mdep_id,
        event_len);
        break;
    case MCAP_DATA_WRITE_CNF:
        printf ("MCAP_DATA_WRITE_CNF on MDL %04X data %p of len %04X with "
            "result %04X\n",handle->mdl_id,event_param,event_len,response);
        break;
#endif  /* MCAP_STANDARD_COMMANDS */
    case MCAP_SYNC_CAP_REQ:
        printf ("MCAP_SYNC_CAP_REQ  paramlen = %04X\n",event_len);
        index = event_len;

        while (0U != index)
        {
            printf ("%02X ",((UCHAR *)event_param)[--index]);
        }
        printf ("\n");
        printf("MCL ID %04X\n", handle->mcl_id);
        break;

    case MCAP_SYNC_CAP_RSP:
        printf ("MCAP_SYNC_CAP_RSP with result %04X, paramlen = %04X\n",
                response,event_len);
        index = event_len;

        while (0U != index)
        {
            printf ("%02X ",((UCHAR *)event_param)[--index]);
        }
        printf ("\n");
        break;
    case MCAP_SYNC_SET_REQ:
        printf ("MCAP_SYNC_SET_REQ, paramlen = %04X\n",event_len);
        index = event_len;

        while (0U != index)
        {
            printf ("%02X ",((UCHAR *)event_param)[--index]);
        }
        printf ("\n");
        printf("MCL ID %04X\n", handle->mcl_id);
        break;
    case MCAP_SYNC_SET_RSP:
        printf ("MCAP_SYNC_SET_RSP, with result %04X paramlen = %04X\n",
                response,event_len);

        index = event_len;
        while (0U != index)
        {
            printf ("%02X ",((UCHAR *)event_param)[--index]);
        }
        printf ("\n");
        break;
    case MCAP_SYNC_INFO_IND:
        printf ("MCAP_SYNC_INFO_IND, with result %04X paramlen = %04X\n",
                response,event_len);

        index = event_len;
        while (0U != index)
        {
            printf ("%02X ",((UCHAR *)event_param)[--index]);
        }
        printf ("\n");
        break;
    default:
        printf ("MCAP mcap_event %02X with response %04X\n",mcap_event,response);
        break;
    }

    return API_SUCCESS;
}


void appl_mcap_register (void)
{
    int val;
    API_RESULT retval;
    UINT16 control_psm, data_psm;
    MCAP_MEP mep_info;

    MCAP_INIT_MEP (mep_info);

    l2cap_init_config_option (&appl_cntrl_ch_l2cap_params);

    appl_cntrl_ch_l2cap_params.mtu = 672U;
    appl_cntrl_ch_l2cap_params.flush_to = 0xFFFFU;
    control_channel_fec_params.mode = L2CAP_MODE_ERTM;
    control_channel_fec_params.tx_window = 1U;
    control_channel_fec_params.max_transmit = 2U;
    control_channel_fec_params.retx_timeout = 1000U;
    control_channel_fec_params.monitor_timeout = 300U;
    control_channel_fec_params.mps = 48U;
    appl_cntrl_ch_l2cap_params.fec = &control_channel_fec_params;
    l2cap_init_flow_spec_default (&control_channel_flow_spec);
    appl_cntrl_ch_l2cap_params.flow_spec =
        &control_channel_flow_spec;
    /* appl_cntrl_ch_l2cap_params.fec = &control_channel_fec_params; */

    printf ("Enter Control Channel PSM: "); fflush(stdout);
    scanf ("%x", &val);
    control_psm = (UINT16) val;

    printf ("Enter Data Channel PSM: "); fflush(stdout);
    scanf ("%x", &val);
    data_psm = (UINT16) val;

    mep_info.control_channel_psm = control_psm;
#ifdef MCAP_STANDARD_COMMANDS
    mep_info.data_channel_psm = data_psm;
#endif  /* MCAP_STANDARD_COMMANDS */
    mep_info.ntf_cb = mcap_cb;
    mep_info.config_cb = appl_config_cb;

    retval = BT_mcap_register_mep (&mep_info, &mep_id);

    if (API_SUCCESS != retval)
    {
        printf("*** FAILED to Register MEP with MCAP: 0x%04X\n", retval);
    }
}


void appl_mcap_mc_connect (void)
{
    int val;
    API_RESULT retval;
    MCAP_MC_CONNECT_PARAMS connect_params;

    printf ("Enter BD_ADDR: "); fflush(stdout);
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (appl_mcap_handle.bd_addr);

    printf ("Enter Control Channel PSM: "); fflush(stdout);
    scanf ("%04x", &val);
    connect_params.cntrl_ch_psm = (UINT16) val;
    connect_params.mep_id = mep_id;

    /* Create Control Channel */
    retval = BT_mcap_mc_connect_req
             (
                 &appl_mcap_handle,
                 &connect_params
             );

    printf ("MC connect retval = %04X\n",retval);
    printf ("Wait for MCAP_MC_CONNECT_CNF in case of success\n");
}

void appl_mcap_mc_disconnect (void)
{
    API_RESULT    retval;

    printf ("Initiating MCAP MC disconnect request with %02X %02X %02X %02X"
           "%02X %02X, MCL: %04X\n",appl_mcap_handle.bd_addr[0U],
            appl_mcap_handle.bd_addr[1U],appl_mcap_handle.bd_addr[2U],
            appl_mcap_handle.bd_addr[3U],appl_mcap_handle.bd_addr[4U],
            appl_mcap_handle.bd_addr[5U], appl_mcap_handle.mcl_id);

    retval = BT_mcap_mc_disconnect_req (&appl_mcap_handle);

    printf ("MC disconnect returned %04X\n",retval);
    printf ("Wait for MCAP_MC_DISCONNECT_CNF in case of success\n");
}

#ifdef MCAP_STANDARD_COMMANDS
void appl_mcap_md_create (void)
{
    int               val;
    API_RESULT        retval;
    UINT16            psm;
    UCHAR             remote_mdep_id;
    UCHAR             config;

    config = 0U;

    printf ("\nInitiating MCAP MD Create req on Control Channel %04X\n Rem "
            BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",appl_mcap_handle.mcl_id,
            BT_DEVICE_ADDR_ONLY_PRINT_STR (appl_mcap_handle.bd_addr));

    printf ("Enter remote side's data channel PSM\n"); fflush(stdout);
    scanf ("%4x",&val);
    psm = (UINT16)val;

    printf ("Enter remote side's MDEP Id\n"); fflush(stdout);
    scanf ("%2x",&val);
    remote_mdep_id = (UCHAR)val;

    printf ("Enter local MDEP Id\n"); fflush(stdout);
    scanf ("%2x",&val);
    appl_mcap_handle.mdep_id = (UCHAR)val;

    printf ("%s",config_menu); fflush(stdout);
    scanf ("%d",&val);
    config = (UCHAR) val;

    switch (config)
    {
    case 1U:
        /* streaming channel */
        appl_strm_ch_params.rem_data_ch_psm = psm;
        appl_strm_ch_params.rem_mdep_id = remote_mdep_id;
        appl_ch_params = &appl_strm_ch_params;
        break;

    case 2U:
        /* reliable channel */
        appl_rlbl_ch_params.rem_data_ch_psm = psm;
        appl_rlbl_ch_params.rem_mdep_id = remote_mdep_id;
        appl_ch_params = &appl_rlbl_ch_params;
        break;

    default:
        /* no preference - basic? */
        appl_dflt_ch_params.rem_data_ch_psm = psm;
        appl_dflt_ch_params.rem_mdep_id = remote_mdep_id;
        appl_ch_params  = &appl_dflt_ch_params;
        break;
    }

    retval = BT_mcap_md_create_req (&appl_mcap_handle,appl_ch_params);

    printf ("Data channel initiation procedure retunred %04X\n",retval);
    printf ("Wait for MCAP_MD_CREATE_CNF in case of success\n");

}

void appl_mcap_md_disconnect (void)
{
    int            val;
    API_RESULT     retval;
    MCAP_HANDLE    handle;

    handle = appl_mcap_handle;
    printf ("\nEnter MDL ID\n");
    scanf ("%x",&val);

    handle.mdl_id = (UINT16) val;

    retval = BT_mcap_md_disconnect_req (&handle);

    printf ("\nMDL Disconnect procedure returned %04X\n"
            "Wait for MCAP_MD_DISCONNECT_CNF cnf in case of success",retval);
}


void appl_mcap_md_delete (void)
{
    int            val;
    API_RESULT     retval;
    MCAP_HANDLE    handle;

    handle = appl_mcap_handle;
    printf ("Enter MDL ID to delete\n");
    scanf ("%x",&val);

    handle.mdl_id = (UINT16)val;
    printf ("Initiating DELTE on MCL %04X MDL %04X",
        handle.mcl_id,handle.mdl_id);
    retval = BT_mcap_md_delete_req (&handle);

    printf ("\nMDL Delete procedure returned %04X\n"
            "Wait for MCAP_MD_DELETE_CNF in case of success\n",retval);
}


void appl_mcap_md_delete_all (void)
{
    API_RESULT     retval;
    MCAP_HANDLE    handle;

    handle = appl_mcap_handle;
    handle.mdl_id = 0xFFFFU;
    printf ("Initiating DELTE on MCL %04X MDL %04X",
        handle.mcl_id,handle.mdl_id);
    retval = BT_mcap_md_delete_all_req (&handle);

    printf ("\nMDL Delete procedure returned %04X\n"
            "Wait for MCAP_MD_DELETE_CNF in case of success\n",retval);
}

void appl_mcap_md_reconnect (void)
{
    int           val;
    UINT16        remote_data_ch_psm;
    API_RESULT    retval;

    printf ("\nEnter MDL ID\n");
    scanf ("%x",&val);
    appl_mcap_handle.mdl_id = (UINT16) val;
    printf ("\nEnter remote data channel PSM\n");
    scanf ("%x",&val);

    remote_data_ch_psm = (UINT16)(val);

    printf ("Initating Reconnect Req on MCL %04X for MDL %04X\n"
            "Wait for MCAP_MD_RECONNECT_CNF in case of success\n",
            appl_mcap_handle.mcl_id,appl_mcap_handle.mdl_id);


    retval = BT_mcap_md_reconnect_req
             (
                 &appl_mcap_handle,
                 &remote_data_ch_psm
             );

    printf ("Reconnect Initiation procedure return %04X\n",retval);
}


void appl_mcap_md_abort (void)
{
    API_RESULT    retval;

    printf ("Initiating ABORT request for MDL %04X on MCL %04X\n",
           appl_mcap_handle.mdl_id,appl_mcap_handle.mcl_id);

    retval = BT_mcap_md_abort_req (&appl_mcap_handle);

    printf ("Abort initiation returned %04X\nWait for MCAP_MD_ABORT_CNF in"
            " case of success\n",retval);
}


void appl_mcap_md_write (void)
{
    MCAP_HANDLE     handle;
    int             val;
    UINT16          nbytes;
    API_RESULT      retval;
    static UCHAR    ch;

    ch = 'A';
    BT_IGNORE_UNUSED_PARAM(ch);

    handle = appl_mcap_handle;

    printf ("\nEnter MDL on which data is to be sent\n");
    scanf ("%x",&val);
    handle.mdl_id = (UINT16)val;

    printf ("Enter number of bytes to be written\n");
    scanf ("%x",&val);
    nbytes = (UINT16)val;

    if (1000U < nbytes)
    {
        nbytes = 1000U;
    }

    printf ("Sending data on MDL %04X MCL %04X\nWait for MCAP_DATA_WRITE_CNF "
            "in case of success\n",handle.mdl_id,handle.mcl_id);

    retval = BT_mcap_md_write
             (
                 &handle,
                 appl_mcap_data,
                 nbytes
             );
    printf ("BT_mcap_md_write returned %04X\n", retval);

}
#endif  /* MCAP_STANDARD_COMMANDS */
void appl_initialize_data_channel_options (void)
{
    /* initialize l2cap parameters */
    data_strming_ch_fec_params.mode = L2CAP_MODE_SM;
    data_strming_ch_fec_params.tx_window = 1U;
    data_strming_ch_fec_params.max_transmit = 2U;
    data_strming_ch_fec_params.retx_timeout = 1000U;
    data_strming_ch_fec_params.monitor_timeout = 300U;
    data_strming_ch_fec_params.mps = 48U;

    data_rlbl_ch_fec_params.mode = L2CAP_MODE_ERTM;
    data_rlbl_ch_fec_params.tx_window = 1U;
    data_rlbl_ch_fec_params.max_transmit = 2U;
    data_rlbl_ch_fec_params.retx_timeout = 1000U;
    data_rlbl_ch_fec_params.monitor_timeout = 300U;
    data_rlbl_ch_fec_params.mps = 48U;

    l2cap_init_fec_option_default (&data_bsc_ch_fec_params);

    MCAP_INIT_MD_PARAMS (appl_strm_ch_params);
    MCAP_INIT_MD_PARAMS (appl_rlbl_ch_params);
    MCAP_INIT_MD_PARAMS (appl_dflt_ch_params);

    appl_strm_ch_l2cap_params.fec = &data_strming_ch_fec_params;
    appl_rlbl_ch_l2cap_params.fec = &data_rlbl_ch_fec_params;
    appl_dflt_ch_l2cap_params.fec = &data_bsc_ch_fec_params;

    appl_strm_ch_l2cap_params.mtu = APPL_DATA_CHANNEL_MTU;
    appl_rlbl_ch_l2cap_params.mtu = APPL_DATA_CHANNEL_MTU;
    appl_dflt_ch_l2cap_params.mtu = APPL_DATA_CHANNEL_MTU;

    appl_strm_ch_params.channel_preference = MCAP_HDP_STREAMING;
    appl_rlbl_ch_params.channel_preference = MCAP_HDP_RELIABLE;
    appl_dflt_ch_params.channel_preference = 0x00U;

    BT_mem_set(appl_mcap_data, 'a', APPL_MCAP_DATA_SIZE);

    appl_clk_cmd[0U] = 0x11U;
    appl_clk_cmd[1U] = 0x12U;
    appl_clk_cmd[2U] = 0x13U;
    appl_clk_cmd[3U] = 0x14U;
    appl_clk_cmd[4U] = 0x15U;

    accept_mdl = API_SUCCESS;

    appl_ch_params = &appl_rlbl_ch_params;

}

void appl_mcap_send_invalid_cmd (void)
{
#ifdef MCAP_TEST
    static UCHAR    cmd[3U];
    API_RESULT      retval;

    cmd[0U] = 0x17U;
    cmd[1U] = 0xFFU;
    cmd[2U] = 0xFFU;

    printf ("Sending Invalid cmd %02X\n",cmd[0U]);

    retval = mcap_send_invalid_command
             (
                 &appl_mcap_handle,
                 cmd[0U],
                 &cmd[1U],
                 2U
             );
    printf ("Sent Invalid cmd with retval %04X\n",retval);

    cmd[0U]++;
#else
    printf ("MCAP_TEST flag is not defined\n");
#endif /* 0 */

    return;
}

void appl_send_clk_sync_cmd (void)
{
#ifdef MCAP_TEST
    UCHAR         cmd[20U];
    API_RESULT    retval;

    cmd[0U] = appl_clk_cmd[count%5U];

    retval = mcap_send_invalid_command
             (
                 &appl_mcap_handle,
                 cmd[0U],
                 &cmd[1U],
                 appl_clk_cmd_len[count%5U]
             );

    printf ("Sending Clck command returned %04X\n",retval);
    count++;
#else
    printf ("MCAP_TEST flag is not defined\n");
#endif /* MCAP_TEST */

    return;
}

void appl_config_cb
     (
         /* IN */  MCAP_HANDLE            * handle,
         /* IN */  UCHAR                  channel_prf,
         /* OUT */ L2CAP_CONFIG_OPTION    ** config_option
     )
{
    BT_assert (NULL != handle);
    BT_assert (NULL != config_option);

    if (MCAP_INVALID_MDL_ID == handle->mdl_id)
    {
        /* Control channel */
        printf ("ERTM params for MCL %04X\n",handle->mcl_id);
        *config_option = &appl_cntrl_ch_l2cap_params;
    }
    else
    {
        /* Data channel */
        switch (channel_prf)
        {
            case L2CAP_CONFIG_PREF_BASIC:
            {
                printf ("Basic param for MDL %04X\n",handle->mdl_id);
                *config_option =  &appl_dflt_ch_l2cap_params;
            }
            break;
            case L2CAP_CONFIG_PREF_ERTM_ONLY:
            {
                printf ("ERTM param for MDL %04X\n",handle->mdl_id);
                *config_option = &appl_rlbl_ch_l2cap_params;
            }
            break;
            case L2CAP_CONFIG_PREF_SM_ONLY:
            {
                printf ("STREAMING param for MDL %04X\n",handle->mdl_id);
                *config_option = &appl_strm_ch_l2cap_params;
            }
            break;
            default:
                printf ("Unkown Channel preference %02X\n",channel_prf);
                break;
        }
    }

    printf ("\nConfig params:\n");
    printf ("MTU %04X\n",(*config_option)->mtu);
    printf ("Flush timeout %04X\n\n",(*config_option)->flush_to);
    printf ("L2CAP FEC params\n");
    printf ("Mode = %04X\n",(*config_option)->fec->mode);
    printf ("Max transmit = %04X\n",(*config_option)->fec->max_transmit);
    printf ("Monitor timeout = %04X\n",(*config_option)->fec->monitor_timeout);
    printf ("RTX timeout = %04X\n",(*config_option)->fec->retx_timeout);
    printf ("MPS = %04X\n",(*config_option)->fec->mps);
    printf ("TX Window = %04X\n\n",(*config_option)->fec->tx_window);
}

#ifdef MCAP_CLOCK_SYNC

void appl_mcap_sync_cap_req (void)
{
    int           val;
    API_RESULT    retval;
    UCHAR         cmd_req[2U];

    printf ("Enter the required accuracy\n");
    scanf ("%d",&val);

    MCAP_WRITE_TIMESTAMP_REQUIRED_ACCURACY (cmd_req,((UINT16)val));

    printf ("Cmd params:\n %02X %02X\n\n",cmd_req[0U],cmd_req[1U]);

    retval = BT_mcap_sync_cap_req
             (
                 &appl_mcap_handle,
                 cmd_req
             );

    printf ("BT mcap sync cap req returned %04X\n",retval);
}


void appl_mcap_sync_cap_rsp (void)
{
    API_RESULT    retval;
    UINT16        rsp_code;
    UCHAR         cmd_rsp[7U];

#if 0
    int           read;
#endif /* 0 */

    rsp_code = API_SUCCESS;

#if 0
    /*
     * Enable below code segment to test MCAP/ERR/BI-16-C test case with PTS,
     * which requires IUT to send Response Code "Resource Unavailable" (0x08).
     */
    printf("Enter Accept (1) or Reject (0) Sync Cap Request\n");
    scanf("%d", &read);

    /* Treat any value other than Accept (1), as reject */
    if (1 != read)
    {
        printf("Enter Response Code for the reason of rejection\n");
        scanf("%d", &read);

        rsp_code = (UINT16)read;
    }
#endif /* 0 */

    /**
     * Cap rsp structure
     *
     * 0          8            16             24                 40
     * |<-opcode->|<-rsp_code->|<-BT CLK RES->|<-SYNC LEAD TIME->|..
     *
     * 40              56              72
     * |<- TS CLK RES->|<- TS CLK ACC->|
     */

    /* 1 octect - bluetooth access resolution */
    MCAP_WRITE_BLUETOOTH_CLOCK_ACCESS_RESOLUTION (cmd_rsp,\
                    APPL_BLUETOOTH_CLOCK_ACCESS_RESOLUTION);

    /* 2 octect - sync clead time */
    MCAP_WRITE_SYNC_LEAD_TIME ((&cmd_rsp[1U]),APPL_SYNC_LEAD_TIME);

    /* 2 octets - native timestamp resolution */
    MCAP_WRITE_TIMESTAMP_NATIVE_RESOLUTION ((&cmd_rsp[3U]),\
        APPL_TIMESTAMP_NATIVE_RESOLUTION);

    /* 2 octets - native timestamp accuracy */
    MCAP_WRITE_TIMESTAMP_NATIVE_ACCURACY ((&cmd_rsp[5U]),\
        APPL_TIMESTAMP_NATIVE_ACCURACY);

    printf ("SYNC CAP rsp params:\n " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER
            " %02X\n\n", BT_DEVICE_ADDR_ONLY_PRINT_STR(cmd_rsp),cmd_rsp[6U]);

#if 0
    /*
     * Enable below code segment to test MCAP/CS-M/BV-01-C test case with PTS .
     */
    printf("Enter MCL ID\n");
    scanf("%d", &read);

    appl_mcap_handle.mcl_id = (UINT16)read;
#endif /* 0 */

    retval = BT_mcap_sync_cap_rsp
             (
                 &appl_mcap_handle,
                 rsp_code,
                 cmd_rsp
             );

    printf ("BT_mcap_sync_cap_rsp returned %04X\n",retval);
}


void appl_mcap_sync_set_req (void)
{
    API_RESULT       retval;
    long long int    val;
    UCHAR            cmd[13U];

    printf ("Request tinmestamp update information?\n");
    printf ("0. No\n");
    printf ("1. Yes\n");
    scanf ("%lld",&val);

    val = ((1U == val)? 1U: 0U);

    MCAP_WRITE_TIMESTAMP_UPDATE_INFORMATION (cmd,(UCHAR)val);

    printf ("Enter the piconet clock at which sync is desired\n");
    scanf ("%lld",&val);

    MCAP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME ((&cmd[1U]), (UINT32)val);

    printf ("Enter the timestamp sync time\n");
    scanf ("%lld",&val);

    MCAP_WRITE_TIMESTAMP_SYNC_TIME ((&cmd[5U]),((UINT64)val));

    printf ("Sync set req rsp params:\n %02X %02X %02X %02X %02X %02X %02X "
            "%02X %02X %02X %02X %02X %02X\n\n",cmd[0U],cmd[1U],cmd[2U],cmd[3U],
            cmd[4U],cmd[5U],cmd[6U],cmd[7U],cmd[8U],cmd[9U],cmd[10U],cmd[11U],cmd[12U]);

    retval = BT_mcap_sync_set_req
             (
                 &appl_mcap_handle,
                 cmd
             );

    printf ("BT_mcap_sync_set_req returned %04X\n",retval);
}


void appl_mcap_sync_set_rsp (void)
{
    API_RESULT    retval;
    UINT16        rsp_code;
    long long int val;
    UCHAR         cmd_rsp[14U];

    /* *(cmd_rsp+0x00) = 0x00; */
    /* *cmd_rsp = 0x00; */
    /* cmd_rsp[0] = 0x00;*/

    rsp_code = API_SUCCESS;

    printf ("Enter the Bluetooth clock sync time\n");
    scanf ("%lld", &val);

    MCAP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME (cmd_rsp,(UINT32)val);

    printf ("Enter the timestamp sync time\n");
    scanf ("%lld",&val);

    MCAP_WRITE_TIMESTAMP_SYNC_TIME ((&cmd_rsp[4U]),(UINT64)val);

    printf ("Enter timestamp sample accuracy\n");
    scanf ("%lld", &val);

    MCAP_WRITE_TIMESTAMP_SAMPLE_ACCURACY (&cmd_rsp[12U],(UINT16)val);

    printf ("Set sync rsp params:\n %02X %02X %02X %02X %02X %02X %02X %02X"
            " %02X %02X %02X %02X %02X %02X\n\n",cmd_rsp[0U],cmd_rsp[1U],
            cmd_rsp[2U],cmd_rsp[3U],cmd_rsp[4U],cmd_rsp[5U],cmd_rsp[6U],cmd_rsp[7U],
            cmd_rsp[8U],cmd_rsp[9U],cmd_rsp[10U],cmd_rsp[11U],cmd_rsp[12U],
            cmd_rsp[13U]);

#if 0
    {
        int read;
        /*
         * Enable below code segment to test MCAP/CS-M/BV-01-C test case with PTS .
         */
        printf("Enter MCL ID\n");
        scanf("%d", &read);

        appl_mcap_handle.mcl_id = (UINT16)read;
    }
#endif /* 0 */

    retval = BT_mcap_sync_set_rsp
             (
                 &appl_mcap_handle,
                 rsp_code,
                 cmd_rsp
             );

    printf ("BT_mcap_sync_set_rsp returned %04X\n",retval);
}


void appl_mcap_sync_info_ind (void)
{
    API_RESULT    retval;
    long long int val;
    UCHAR         cmd[14U];

    printf ("Enter the Bluetooth clock sync time\n");
    scanf ("%lld",&val);

    MCAP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME (cmd,(UINT32)val);

    printf ("Enter the timestamp sync time\n");
    scanf ("%lld",&val);

    MCAP_WRITE_TIMESTAMP_SYNC_TIME ((&cmd[4U]),(UINT64)val);

    printf ("Enter timestamp sample accuracy\n");
    scanf ("%lld",&val);

    MCAP_WRITE_TIMESTAMP_SAMPLE_ACCURACY (&cmd[12U],(UINT16)val);

    printf ("Sync Info Ind params:\n %02X %02X %02X %02X %02X %02X %02X %02X"
            " %02X %02X %02X %02X %02X %02X",cmd[0U],cmd[1U],cmd[2U],
            cmd[3U],cmd[4U],cmd[5U],cmd[6U],cmd[7U],cmd[8U],cmd[9U],cmd[10U],cmd[11U],
            cmd[12U],cmd[13U]);

    retval = BT_mcap_sync_info_ind
             (
                 &appl_mcap_handle,
                 cmd
             );

    printf ("BT_mcap_sync_info_ind returned %04X\n",retval);
}

void appl_hci_read_clk (void)
{
    API_RESULT retval;

    /* Initiate read clock if command not already in progress */
    if (BT_FALSE == appl_mcap_read_clock_in_progress)
    {
       /* printf ("Initiating read clock on handle %04X\n",l2cap_dt_acl_handle);
       */

        retval = BT_hci_read_clock
                 (
                     0x00U, /* Piconet clock */
                     0x00U  /* l2cap_dt_acl_handle */
                 );

        if (API_SUCCESS == retval)
        {
            printf ("Successfully initiated read clock command, wait for "
                    "HCI_COMMAND_COMPLETE_EVENT\n");
            appl_mcap_read_clock_in_progress = BT_TRUE;
        }
        else
        {
            printf ("Failed to initiate read clock, result = %04X\n",retval);
        }
    }
    /* Already issues read clock, awaiting HCI_COMMAND_COMPLETE_EVENT */
    else
    {
        printf ("Cannot initiate read clock, already in progress!\n");
    }
}
#endif /* MCAP_CLOCK_SYNC */

#endif /* MCAP */
