
/**
 *  \file appl_bip_initiator.c
 *
 *  Source file for BIP initiator Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_bip.h"
#include "appl_utils.h"

#ifdef BIP
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
#ifdef APPL_ENABLE_AUTOPROCEDURE
static                       UCHAR bip_autoconn;
static                       UCHAR bip_peer_addr[BT_BD_ADDR_SIZE];
static                       UCHAR bip_transport;
static                       INT32 bipc_handle;
#endif /* APPL_ENABLE_AUTOPROCEDURE */

static UCHAR                appl_reject_opn;
static UCHAR                bd_addr[BT_BD_ADDR_SIZE];
static UINT16               connection_handle;
static UCHAR                appl_bip_initiator_read_only;
static BIP_INSTANCE         bip_initiator_pri_instances[BIP_NUM_INITIATOR_INSTANCE];
static BIP_INSTANCE         bip_initiator_sec_instances[BIP_NUM_INITIATOR_INSTANCE];
static UCHAR                *initiator_image_handle;
static UCHAR                bip_attrib_data[BIP_SDP_RECORD_DATA_SIZE];
static UINT16               bip_attrib_data_len = BIP_SDP_RECORD_DATA_SIZE;
static SDP_HANDLE           bip_sdp_handle;
static UCHAR                bip_server_channel;
static UINT16               bip_l2cap_psm;
static BIP_APPL_PARAMS      appl_param;
static BIP_CONNECT_STRUCT   connect_info;

static UCHAR                bip_attachment_descriptor_data[256U];

static UCHAR                image_descriptor_data[256U];
static UCHAR                image_handles_descriptor_data[256U];
static UINT16               img_width, img_height;
static UCHAR                *img_frmt;

BIP_IMAGE_HANDLER           bip_img_handle[BIP_IMAGE_HANDLE_SIZE];

static BT_fops_file_handle  bip_tx_fp;
static UINT32               fsize;
static UINT32               remaining, sent;
static UINT32               bip_xchg_size;
static UINT32               appl_bip_xchg_size;

static BT_fops_file_handle  bip_rx_fp;
static UCHAR                start_date[8U], end_date[8U];
static UCHAR                file_name[40U];
static UCHAR                image_thumbnail_name[32U], image_file_name[32U];
static UCHAR                thumbnail_file_name[64U];
static UCHAR                file_object[BIP_FOLDER_NAME_LEN * 2U];
static UCHAR                appl_bip_push_image_thumbnail;
static UCHAR                appl_bip_push_image_thumbnail_flag;

/* SRMP wait count */
static UCHAR srmp_wait_count;

static UCHAR bip_img_referenced_val[16U] =
             {
                 0x8EU, 0x61U, 0xF9U, 0x5DU,
                 0x1AU, 0x79U, 0x11U, 0xD4U,
                 0x8EU, 0xA4U, 0x00U, 0x80U,
                 0x5FU, 0x9BU, 0x98U, 0x34U
             };

static UCHAR bip_img_archived_val[16U] =
             {
                 0x8EU, 0x61U, 0xF9U, 0x5EU,
                 0x1AU, 0x79U, 0x11U, 0xD4U,
                 0x8EU, 0xA4U, 0x00U, 0x80U,
                 0x5FU, 0x9BU, 0x98U, 0x34U
             };

static const UCHAR bip_initiator_menu[] =
" \n \
-------------- BIP initiator Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. BIP initiator Initialize \n \
\t 3. BIP initiator Shutdown \n \
\t 4. BIP initiator Start \n \
\t 5. BIP initiator Stop \n \
\n\
\t 7. Register Peer BD Addrs. \n\
\t 8. Get Registered Peer BD Addrs. \n\
\n \
\t10. Create ACL Connection\n\
\t11. ACL Disconnection\n\
\t12. Do SDP Query \n\
\n \
\t15. Connect to BIP Responder \n \
\t16. Disconnect from BIP Responder \n \
\t17. Close Primary Session Transport Connection with BIP Responder \n \
\t18. Close Secondary Session Transport Connection with BIP Responder \n \
\n\
======= PushImage Operations ========= \n \
\t20. Get Capabilities \n \
\t21. Push Image \n \
\t22. Push Image Linked Attachment \n \
======= PullImage Operations ========= \n \
\t20. Get Capabilities \n \
\t24. Get Image List \n\
\t25. Get Image Properties \n\
\t26. Get Image Linked Thumbnail \n\
\t27. Get Image  \n\
\t28. Get Image Linked Attachment \n\
\t34. Delete Image\n\
======= Advanced Image Printing Operations ========= \n \
\t20. Get Capabilities \n \
\t31. Start Print\n\
\t33. Get Status\n\
======= Automatic Archive Operations ========= \n \
\t32. Start Archive\n\
\t33. Get Status\n\
======= Remote Camera Operations ========= \n \
\t29. Get Monitoring Image\n\
\t25. Get Image Properties \n\
\t27. Get Image  \n\
\t26. Get Image Linked Thumbnail \n\
======= Remote Display Operations ========= \n \
\t20. Get Capabilities \n \
\t21. Push Image \n \
\t23. Push Image Linked Thumbnail \n \
\t24. Get Image List \n\
\t30. Remote Display\n\
\n \
\t50. BIP Request set number of SRMP wait stages \n\
Your Choice: ";

/* ----------------------------------------- Functions */
#ifdef APPL_ENABLE_AUTOPROCEDURE
/* Callback registered with HCI */
API_RESULT bipc_appl_hci_event_ind_cb
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           )
{
    API_RESULT retval;
    UINT16     connection_handle, value_2;
    UCHAR      status, value_1, link_type;

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

            if (BT_TRUE == bip_autoconn)
            {
                /* Set the SDP Handle */
                SDP_SET_HANDLE
                (
                   bip_sdp_handle,
                   bip_peer_addr,
                   appl_bip_initiator_sdp_callback
                );

                retval = BT_sdp_open(&bip_sdp_handle);
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP Open Failed. Reason: 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG("Started SDP Query\n");
                }
            }

            break;

        default:
            LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
            break;
    }

    return API_SUCCESS;
}

void appl_init_bipc(void)
{
    API_RESULT retval;
    UCHAR i;

    LOG_DEBUG ("Initializing BIP initiator...\n");
    retval = BT_bip_initiator_init ();
    LOG_DEBUG ("Retval - 0x%04X\n", retval);

    if (API_SUCCESS == retval)
    {
        /* Reset BD_ADDR & BIP handle */
        for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
        {
            BIP_RESET_INSTANCE(&bip_initiator_pri_instances[i]);
            BIP_RESET_INSTANCE(&bip_initiator_sec_instances[i]);
        }
    }

    bip_autoconn = BT_FALSE;

    /* Register callback with HCI */
    retval = appl_hci_register_callback (bipc_appl_hci_event_ind_cb);
    if (retval != API_SUCCESS)
    {
        LOG_DEBUG ("[ERR] Failed to register callback with HCI\n");
    }
}

void appl_start_bipc(void)
{
    API_RESULT retval;
    UCHAR i;

    for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
    {
        LOG_DEBUG ("Starting BIP initiator...\n");
        retval = BT_bip_initiator_start
                 (
                     &bip_initiator_pri_instances[i].handle,
                     appl_bip_initiator_callback,
                     0U
                 );/*default set for image push feature*/
        LOG_DEBUG ("Retval - 0x%04X\n", retval);
    }
}
#endif /* APPL_ENABLE_AUTOPROCEDURE */

void main_bip_initiator_operations (void)
{
    API_RESULT          retval;
    UCHAR               i;
    UINT8               *phandle;
    UINT16              size;
    BIP_REQUEST_STRUCT  req_info;
    BIP_HEADER_STRUCT   name_info, body_info, image_handle_info;
    BIP_IMAGE_DESC      image_descriptor_info;

    UINT16              file_name_len;
    UINT16              actual;
    UCHAR               more;
    UCHAR               img_type;

    int choice, menu_choice, handle, cn_handle, server_ch, val, service_type;

    retval  = API_SUCCESS;
    more    = 0U;

    BT_LOOP_FOREVER()
    {
        printf ("%s", bip_initiator_menu);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            LOG_DEBUG ("Initializing BIP Initiator...\n");
            retval = BT_bip_initiator_init();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & BIP handle */
                for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
                {
                    BIP_RESET_INSTANCE(&bip_initiator_pri_instances[i]);
                    BIP_RESET_INSTANCE(&bip_initiator_sec_instances[i]);
                }
            }

            bip_initiator_pri_print_appl_instances ();
            bip_initiator_sec_print_appl_instances ();

            break;

        case 3:
            LOG_DEBUG ("Shutting Down BIP Initiator...\n");
            retval = BT_bip_initiator_shutdown();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            bip_initiator_pri_print_appl_instances ();
            bip_initiator_sec_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            LOG_DEBUG(" Imaging Features : \n");
            LOG_DEBUG("     0 => IMAGE PUSH\n");
            LOG_DEBUG("     1 => IMAGE PULL\n");
            LOG_DEBUG("     2 => BIP ADVANCED IMAGE PRINTING\n");
            LOG_DEBUG("     3 => BIP AUTOMATIC ARCHIVE\n");
            LOG_DEBUG("     4 => BIP REMOTE CAMERA\n");
            LOG_DEBUG("     5 => BIP REMOTE DISPLAY\n");
            LOG_DEBUG (" Please Select The Feature: ");
            scanf ("%d", &val);

            if (5U < val)
            {
                LOG_DEBUG("Invalid feature\n");
                break;
            }

            if((2U == val) || (3U == val))
            {
                LOG_DEBUG(" Start Primary/Secondary session (0/1)\n");
                scanf("%d",&service_type);
            }
            else
            {
                /* Only Primary */
                service_type = 0;
            }

            /* BIP application instance handle */
            if(service_type == 1U)
            {
                phandle = &bip_initiator_sec_instances[handle].handle;
            }
            else
            {
                phandle = &bip_initiator_pri_instances[handle].handle;
            }

            if (BIP_ENTITY_INVALID != *phandle)
            {
                printf ("Application Instance not free!\n");
                break;
            }

            printf ("Starting BIP initiator...\n");
            retval = BT_bip_initiator_start
                     (
                         phandle,
                         appl_bip_initiator_callback,
                         (UCHAR )val,
                         (UCHAR)service_type
                     );
            printf ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = BIP_ENTITY_INVALID;
            }

            if(service_type == 1U)
            {
                bip_initiator_sec_print_appl_instances ();
            }
            else
            {
                bip_initiator_pri_print_appl_instances ();
            }

            break;

        case 5:
            bip_initiator_pri_print_appl_instances ();

            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            printf(" Imaging Features : \n");
            printf("     0 => IMAGE PUSH\n");
            printf("     1 => IMAGE PULL\n");
            printf("     2 => BIP ADVANCED IMAGE PRINTING\n");
            printf("     3 => BIP AUTOMATIC ARCHIVE\n");
            printf("     4 => BIP REMOTE CAMERA\n");
            printf("     5 => BIP REMOTE DISPLAY\n");
            printf (" Please Select The Feature: ");
            scanf ("%d", &val);

            if((2U == val) || (3U == val))
            {
                printf(" Stop Primary/Secondary session (0/1)\n");
                scanf("%d",&service_type);
            }
            else
            {
                /* Only Primary */
                service_type = 0;
            }

            /* BIP application instance handle */
            if(service_type == 1U)
            {
                phandle = &bip_initiator_sec_instances[handle].handle;
            }
            else
            {
                phandle = &bip_initiator_pri_instances[handle].handle;
            }

            printf ("Stopping  BIP Initiator...\n");
            retval = BT_bip_initiator_stop
                     (
                        phandle,
                        (UCHAR)service_type
                     );
            printf ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = BIP_ENTITY_INVALID;

                BT_mem_set
                (
                    bip_initiator_pri_instances[handle].bd_addr,
                    0x00,
                    BT_BD_ADDR_SIZE
                );

                /* Close the files if opened */
                if (NULL != bip_tx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bip_tx_fp);
                    bip_tx_fp = NULL;
                }

                if (NULL != bip_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bip_rx_fp);
                    bip_rx_fp = NULL;
                }
            }

            bip_initiator_pri_print_appl_instances ();
            break;

        case 7:
            /* Register Peer BD address */
            printf("Enter Peer BD Address:");
            appl_get_bd_addr(bd_addr);
            break;

        case 8:
            /* Get Registered BD_ADDR of Peer Device */
            printf("\nRegistered Peer BD "\
                BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR(bd_addr));
            break;

        case 10:

#if 0
            /* Use option 7 to register the peer BD address */
            /**
             * First Application should establish ACL Connection with
             * the peer Device
             */
            printf("Enter BIP Responder's BD Address: ");

            /* Read the BD_ADDR of Remote Device */
            appl_get_bd_addr(bd_addr);
#endif /* 0 */

            retval = BT_hci_create_connection
                     (
                         bd_addr,
                         LMP_ACL_DH5,
                         0x0U, 0x0U, 0x0U, 0x1U
                     );
            if (API_SUCCESS != retval)
            {
                printf("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                        retval);
            }
            break;

        case 11:
            /* To disconnect read the ACL Connection Handle */
            printf("Enter Connection Handle (in Hex): "); fflush(stdout);
            scanf("%x", &cn_handle);
            connection_handle = (UINT16)cn_handle;

            printf("\n");
            printf("Starting HCI Disconnection ... "); fflush(stdout);

            /* Mention Reason for Disconnect as User Initiated Disconnection */
            retval = BT_hci_disconnect (connection_handle, 0x13U);
            if (retval != API_SUCCESS)
            {
                printf("HCI Disconnection FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 12:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

#if 0
            printf ("Enter peer device address: ");
            appl_get_bd_addr (bd_addr);
#endif /* 0 */

            SDP_SET_HANDLE
            (
                bip_sdp_handle,
                bd_addr,
                appl_bip_initiator_sdp_callback
            );

            retval = BT_sdp_open(&bip_sdp_handle);

            if (retval != API_SUCCESS)
            {
                printf("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            printf("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            bip_initiator_pri_print_appl_instances();

            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

#if 0
            printf ("Enter Peer Device Address: ");
            appl_get_bd_addr (bd_addr);
#endif /* 0 */

            connect_info.bd_addr = bd_addr;

#ifdef OBEX_OVER_L2CAP
            printf ("Enter Peer L2CAP PSM (in Hex): ");
            scanf ("%x", &val);
            connect_info.psm = (UINT16)val;
#endif /* OBEX_OVER_L2CAP */

            printf ("Enter Peer Server Channel (in Hex): ");
            scanf ("%x", &server_ch);
            connect_info.server_channel = (UCHAR )server_ch;

            printf ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            size = (UINT16) val;
            connect_info.max_recv_size = size;

            printf ("Connecting...\n");
            retval = BT_bip_initiator_connect
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &connect_info
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 16:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            printf ("Disconnecting BIP initiator Instance %d\n", handle);
            retval = BT_bip_initiator_disconnect (&bip_initiator_pri_instances[handle].handle);
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 17:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            printf ("Disconnecting BIP Primary Session Transport with BIP Responder\n");
            retval = BT_bip_initiator_pri_transport_close (&bip_initiator_pri_instances[handle].handle);
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 18:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            printf ("Disconnecting BIP Secondary Session Transport with BIP Responder\n");
            retval = BT_bip_initiator_sec_transport_close (&bip_initiator_sec_instances[handle].handle);
            printf ("Retval - 0x%04X\n", retval);
            break;

         case 20:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            bip_initiator_pri_print_appl_instances ();
            printf ("Requesting to Get Capabilities.....\n");
            retval = BT_bip_initiator_get_capabilities
                     (
                         &bip_initiator_pri_instances[handle].handle
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 21:
            bip_initiator_pri_print_appl_instances ();

            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));
            bip_xchg_size = bip_initiator_pri_instances[handle].max_xchg_size;
            appl_bip_xchg_size = bip_xchg_size;

            BIP_INIT_HEADER_STRUCT (name_info);
            BIP_INIT_HEADER_STRUCT (body_info);
            BIP_INIT_HEADER_STRUCT (image_descriptor_info);

            printf ("Enter the Image Name to be sent: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len (file_name, sizeof(file_name));

            /* Form Thumbnail file name from image file name */
            BT_str_n_copy (thumbnail_file_name, file_name, sizeof(file_name));
            BT_str_print((CHAR *)&thumbnail_file_name[file_name_len - 4U], "_thumbnail.jpg");

            printf ("Enter Image Descriptor \n");

            printf ("Enter Image Format:\n");
            printf ("\t1 -> JPEG\n");
            printf ("\t2 -> GIF\n:");
            scanf ("%d", &val);
            if (1U == val)
            {
                img_frmt = (UCHAR *)"JPEG";
            }
            else if (2U == val)
            {
                img_frmt = (UCHAR *)"GIF";
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Get Image Width */
            printf ("Enter Image Width:");
            scanf ("%d", &val);
            img_width = (UINT16)val;

            /* Get Image Length */
            printf ("Enter Image Height:");
            scanf ("%d", &val);
            img_height = (UINT16)val;

            /* Should follow with thumbnail? */
            printf ("Follow with Thumbnail (1/0): ");
            scanf ("%d", &val);
            appl_bip_push_image_thumbnail = (UCHAR)val;

            /* Should follow with thumbnail? */
            printf("Follow with Thumbnail irrespective of  BIP_PARTIAL_CONTENT_RSP (1/0): ");
            scanf("%d", &val);
            appl_bip_push_image_thumbnail_flag = (UCHAR)val;

            /* Form image descriptor object with the given parameters */
            BT_str_print((CHAR *)image_descriptor_data,
                    "<image-descriptor version=\"1.0\" > \n"
                    "<image encoding = \"%s\" "
                    "pixel = \"%d*%d\" /> \n"
                    "</image-descriptor>",
                    img_frmt, img_width, img_height);

            /* Image Descriptor */
            image_descriptor_info.value = image_descriptor_data;
            image_descriptor_info.length = (UINT16)BT_str_n_len(image_descriptor_data, sizeof(image_descriptor_data));

            appl_bip_xchg_size -= (3U + image_descriptor_info.length);

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16)(BT_str_n_len (file_name, sizeof(file_name)) + 1U);

            appl_bip_xchg_size -= (3U + name_info.length);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                file_name,
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                printf ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > appl_bip_xchg_size)
            {
                body_info.length = (UINT16)appl_bip_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    bip_tx_fp,
                    &actual
                );
            }

            /* Update the hdrs  for put request */
            req_info.name = &name_info;
            req_info.body = &body_info;
            req_info.image_descriptor = &image_descriptor_info;

            actual = 0U;
            printf ("Requesting to Push Image...\n");
            retval = BT_bip_initiator_push_image
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            printf ("Retval - 0x%04X\n", retval);

            /* Update object size sent, remaining to send and file read pointer */
            sent += actual;
            remaining = fsize - sent;

            printf("read length = %d, actual sent = %d\n", body_info.length, actual);
            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bip_tx_fp, sent, SEEK_SET);
            }

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }
            break;

        case 22:
            bip_initiator_pri_print_appl_instances ();

            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            BIP_INIT_HEADER_STRUCT (name_info);
            BIP_INIT_HEADER_STRUCT (body_info);
            BIP_INIT_HEADER_STRUCT (image_handle_info);

            bip_xchg_size = bip_initiator_pri_instances[handle].max_xchg_size;
            appl_bip_xchg_size = bip_xchg_size;

            printf ("Enter Attachement File Name:");
            scanf ("%s", file_name);

            printf ("Enter the Image Handle: ");
            scanf ("%s", bip_img_handle);

            image_handle_info.value = bip_img_handle;
            image_handle_info.length = (UINT16)BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U;
            appl_bip_xchg_size -= (3U + image_handle_info.length);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                file_name,
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                printf ("Failed to open file\n");
                break;
            }

            /* Form image attachment descriptor object with the given parameters */
            BT_str_print((CHAR *)bip_attachment_descriptor_data,
                    "<attachment-descriptor version=\"1.0\">\n"
                    "<attachment name = \"%s\" "
                    "content-type = \"text/plain\" size = \"%d\" /> \n"
                    "</attachment-descriptor>",
                    file_name, (int)fsize);

            /* Image Descriptor */
            image_descriptor_info.value = bip_attachment_descriptor_data;
            image_descriptor_info.length = (UINT16)BT_str_n_len(bip_attachment_descriptor_data, sizeof(bip_attachment_descriptor_data));
            appl_bip_xchg_size -= (3U + image_descriptor_info.length);

            /* Fill body hdr values */
            if (remaining > appl_bip_xchg_size)
            {
                body_info.length = (UINT16)appl_bip_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    bip_tx_fp,
                    &actual
                );
            }

            /* Update the hdrs  for put request */
            req_info.image_handle = &image_handle_info;
            req_info.body = &body_info;
            req_info.image_descriptor = &image_descriptor_info;

            printf ("Requesting to Push Image Attachment...\n");
            retval = BT_bip_initiator_put_linked_attachment
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            printf ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            printf("read length = %d, actual sent = %d\n", body_info.length, actual);
            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bip_tx_fp, sent, SEEK_SET);
            }

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }
            break;

        case 23:
        {
            BIP_IMAGE_HANDLE    image_handle;

            bip_initiator_pri_print_appl_instances();

            printf("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set(&req_info, 0x00, sizeof(BIP_REQUEST_STRUCT));

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bip_xchg_size = bip_initiator_pri_instances[handle].max_xchg_size;
            appl_bip_xchg_size = bip_xchg_size;

            /* Update the hdrs  for put request */
            image_handle.value = bip_img_handle;
            image_handle.length = (UINT16)BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U;
            appl_bip_xchg_size -= (3U + image_handle.length);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                thumbnail_file_name,
                file_object
            );

            /* Open the Thumbnail file to be sent */
            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                printf("Failed to open file\n");
                break;
            }

            body_info.value = NULL;
            body_info.length = 0U;

            /* Fill body hdr values */
            if (remaining > appl_bip_xchg_size)
            {
                body_info.length = (UINT16)appl_bip_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem(body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    bip_tx_fp,
                    &actual
                );
            }

            req_info.image_handle = &image_handle;
            req_info.body = &body_info;

            printf("BT_bip_initiator_put_linked_thumbnail\n");
            retval = BT_bip_initiator_put_linked_thumbnail
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            if (API_SUCCESS != retval)
            {
                printf("BT_bip_initiator_get_linked_thumbnail Request Failed\n");
            }

            /* Update object size sent & remaining */
            sent += actual;
            remaining = fsize - sent;

            printf("read length = %d, actual sent = %d\n", body_info.length, actual);
            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bip_tx_fp, sent, SEEK_SET);
            }

            if (NULL != body_info.value)
            {
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }
        }
            break;

        case 24:
            bip_initiator_pri_print_appl_instances ();

            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            BIP_INIT_HEADER_STRUCT (image_descriptor_info);
            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            /* Update request info for application parameters */
            req_info.appl_params = &appl_param;

            printf("Number of Image Handles to be returned: ");
            scanf("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_NB_RETURNED_HANDLES
            );
            appl_param.nb_returned_handles = (UINT16)val;

            printf("List Start Offset : ");
            scanf("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_LIST_START_OFFSET
            );
            appl_param.list_start_offset = (UINT16)val;

            printf("Latest Captured Images Flag (1/0) : ");
            scanf("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_LATEST_CAPTURED_IMAGES
            );
            appl_param.latest_captured_images = (UINT8)val;

            printf("Enter Start Date for Image Listing: ");
            scanf("%s", start_date);
            printf("Enter End Date for Image Listing: ");
            scanf("%s",end_date);

            /* Form image handles descriptor object with the given parameters */
            BT_str_print((CHAR *)image_handles_descriptor_data,
                    "<image-handles-descriptor version=\"1.0\" >\n"
                    "<filtering-parameters created = \"%s\"-\"%s\"/>\n"
                    "</image-handles-descriptor>",
                    start_date, end_date);

            /* Image Descriptor */
            image_descriptor_info.value = image_handles_descriptor_data;
            image_descriptor_info.length = (UINT16)BT_str_n_len(image_handles_descriptor_data, sizeof(image_handles_descriptor_data));

            /* Update the hdrs  for get request */
            req_info.image_descriptor = &image_descriptor_info;

            printf ("Requesting to Get Image List.....\n");
            retval = BT_bip_initiator_get_image_listing
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 25:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            printf ("Enter the Image Handle: ");
            scanf ("%s", bip_img_handle);
            image_handle_info.value = bip_img_handle;
            image_handle_info.length =  (UINT16)(BT_str_n_len(bip_img_handle, sizeof(bip_img_handle))+1U);

            req_info.image_handle = &image_handle_info;

            printf ("Requesting to Get Image Properties.....\n");
            retval = BT_bip_initiator_get_image_properties
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 26:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            printf ("Enter The Image Handle For The Thumbnail: ");
            scanf ("%s", bip_img_handle);
            image_handle_info.value = bip_img_handle;
            image_handle_info.length =  (UINT16)(BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U);

            req_info.image_handle = &image_handle_info;

            printf ("Requesting to Get Image Thumbnail.....\n");
            retval = BT_bip_initiator_get_linked_thumbnail
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
            BT_str_n_copy((CHAR *)image_thumbnail_name, (CHAR *)bip_img_handle, sizeof(image_thumbnail_name));
            BT_str_n_cat((CHAR *)image_thumbnail_name, "_thumbnail.jpg", (sizeof(image_thumbnail_name) - BT_str_len(image_thumbnail_name)));

           /**
            * Open file for dumping Get Image Thumbnail
            * request with the respective file name
            */
            if (API_SUCCESS == retval)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    (UCHAR *)image_thumbnail_name,
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    printf ("Failed to File Open\n");
                }

            }
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 27:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP Initiator Instance: ");

#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            printf ("Enter the image handle for which image has to be requested: ");
            scanf ("%s", bip_img_handle);
            image_handle_info.value = bip_img_handle;
            image_handle_info.length = (UINT16)(BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U);

            printf ("Enter Image Descriptor \n");

            printf ("Enter Image Format\n");
            printf ("    1 -> JPEG\n");
            printf ("    2 -> GIF\n:");
            scanf ("%d", &val);
            if (1U == val)
            {
                img_frmt = (UCHAR *)"JPEG";
            }
            else if (2U == val)
            {
                img_frmt = (UCHAR *)"GIF";
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            img_type = (UCHAR )val;

            /* Get Image Width */
            printf ("Enter Image Width:");
            scanf ("%d", &val);
            img_width = (UINT16)val;

            /* Get Image Length */
            printf ("Enter Image Height:");
            scanf ("%d", &val);
            img_height = (UINT16)val;

            /* Form image descriptor object with the given parameters */
            BT_str_print((CHAR *)image_descriptor_data,
                    "<image-descriptor version=\"1.0\" > \n"
                    "<image encoding = \"%s\" "
                    "pixel = \"%d*%d\" /> \n"
                    "</image-descriptor>",
                    img_frmt, img_width, img_height);

            /* Image Descriptor */
            image_descriptor_info.value = image_descriptor_data;
            image_descriptor_info.length = (UINT16)BT_str_n_len(image_descriptor_data, sizeof(image_descriptor_data));

            req_info.image_handle = &image_handle_info;
            req_info.image_descriptor = &image_descriptor_info;

            printf ("Requesting to Get Image .....\n");
            retval = BT_bip_initiator_get_image
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );

            BT_str_n_copy((CHAR *)image_file_name, (CHAR *)bip_img_handle, sizeof(image_file_name));
            if (0x01U == img_type)
            {
                BT_str_n_cat((CHAR *)image_file_name,".jpg", (sizeof(image_file_name) - BT_str_len(image_file_name)));
            }
            else
            {
                BT_str_n_cat((CHAR *)image_file_name,".gif", (sizeof(image_file_name) - BT_str_len(image_file_name)));
            }

            /**
             * Open file for dumping Get Image request
             * with the respective file name
             */
            if (API_SUCCESS == retval)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    image_file_name,
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    printf ("Failed to File Open\n");
                }

            }
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 28:

            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            printf ("Enter The Image Handle: ");
            scanf ("%s", bip_img_handle);
            image_handle_info.value = bip_img_handle;
            image_handle_info.length = (UINT16)(BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U);

            printf ("Enter The Attachment Name: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len (file_name, sizeof(file_name));
            name_info.value = file_name;
            name_info.length = (UINT16)(file_name_len + 1U);

            req_info.image_handle = &image_handle_info;
            req_info.name = &name_info;

            printf("Enter number of wait stages: ");
            scanf("%d", &choice);
            srmp_wait_count = (UCHAR)choice;

            if (0U < srmp_wait_count)
            {
                req_info.wait = BT_TRUE;
                srmp_wait_count --;
            }
            else
            {
                req_info.wait = BT_FALSE;
            }

            printf ("Requesting to Get Image Attachment.....\n");
            retval = BT_bip_initiator_get_linked_attachment
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 29:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            printf ("Want to Store The Image(0/1): ");
            scanf ("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_STORE_FLAG
            );
            appl_param.store_flag = (UINT8)val;

            req_info.appl_params = &appl_param;

            printf ("Requesting to Get Monitoring Image .....\n");
            retval = BT_bip_initiator_get_monitoring_image
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
            printf ("Retval - 0x%04X\n", retval);

            /* Open file for dumping Get file request with the respective file name */
            if (API_SUCCESS == retval)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    (UCHAR *)"monitoring_image.jpg",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    printf ("Failed to File Open\n");
                }
            }
            break;

        case 30:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            printf ("Enter the Image Handle: ");
            scanf ("%s", bip_img_handle);
            image_handle_info.value = bip_img_handle;
            image_handle_info.length = (UINT16)(BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U);

            printf("Remote display, Select option to request respective image\n");
            printf("     1 => Next Image\n");
            printf("     2 => Previous Image\n");
            printf("     3 => Select Image\n");
            printf("     4 => Current Image\n");
            scanf ("%x", &val);
            if(0U >= val || 5U <= val)
            {
                printf("Invalid choice\n");
                break;
            }

            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_REMOTE_DISPLAY
            );
            appl_param.remote_display = (UINT8)val;

            req_info.image_handle = &image_handle_info;
            req_info.appl_params = &appl_param;

            printf ("Requesting for Remote Display .....\n");
            retval =BT_bip_initiator_remote_display
                    (
                        &bip_initiator_pri_instances[handle].handle,
                        &req_info
                    );
           printf ("Retval - 0x%04X\n", retval);
           break;

        case 31:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP initiator instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
            BIP_INIT_HEADER_STRUCT (body_info);

            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_SERVICE_ID
            );

            appl_param.service_id.value = bip_img_referenced_val;
            appl_param.service_id.length = (UINT16)(BT_str_n_len(bip_img_referenced_val, sizeof(bip_img_referenced_val))+1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bip_xchg_size = bip_initiator_pri_instances[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                (UCHAR *)"print_job.txt",
                file_object
            );

            /* Open the Print job file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                printf ("Failed to open file\n");
                break;
            }

             /* Fill body hdr values */
            if (remaining > bip_xchg_size)
            {
                body_info.length = (UINT16 )bip_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    bip_tx_fp,
                    &actual
                );
            }

            /* Update the hdrs  for put request */
            req_info.body = &body_info;
            req_info.appl_params = & appl_param;

            printf ("Requesting to start print job .....\n");
            retval = BT_bip_initiator_start_print
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            printf ("Retval - 0x%04X\n", retval);

            sent += actual;
            remaining = fsize - sent;

            printf("read length = %d, actual sent = %d\n", body_info.length, actual);
            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bip_tx_fp, sent, SEEK_SET);
            }

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }

            break;

        case 32:
            bip_initiator_pri_print_appl_instances ();
            printf ("Enter BIP initiator instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_SERVICE_ID
            );

            appl_param.service_id.value = bip_img_archived_val;
            appl_param.service_id.length = (UINT16 )(BT_str_n_len(bip_img_archived_val, sizeof(bip_img_archived_val))+1U);

            req_info.appl_params = & appl_param;

            printf ("Requesting to start archive .....\n");
            retval = BT_bip_initiator_start_archive
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
           printf ("Retval - 0x%04X\n", retval);
           break;

         case 33:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            bip_initiator_pri_print_appl_instances ();
            printf ("Requesting to Get Status.....\n");
            retval = BT_bip_initiator_get_status
                     (
                         &bip_initiator_pri_instances[handle].handle
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

         case 34:
            printf ("Enter BIP Initiator Instance: ");
#if (BIP_NUM_INITIATOR_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_INITIATOR_INSTANCE > 1) */
            handle = 0;
            printf ("%d\n", handle);
#endif /* (BIP_NUM_INITIATOR_INSTANCE > 1) */

            bip_initiator_pri_print_appl_instances ();
            BIP_INIT_HEADER_STRUCT (image_handle_info);

            printf ("Enter the Image Handle: ");
            scanf ("%s", bip_img_handle);

            image_handle_info.value = bip_img_handle;
            image_handle_info.length = (UINT16)BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U;

            /* Update the hdrs  for put request */
            req_info.image_handle = &image_handle_info;

            printf ("Requesting to Delete Image ...\n");
            retval = BT_bip_initiator_delete_image
                     (
                         &bip_initiator_pri_instances[handle].handle,
                         &req_info
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

         case 50:
             printf("Enter number of SRMP wait stages: ");
             scanf("%d", &choice);
             srmp_wait_count = (UCHAR)choice;
             break;

        default:
            printf ("Invalid Choice.\n");
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

API_RESULT appl_bip_initiator_callback
           (
               BIP_HANDLE      * bip_handle,
               UINT8             event_type,
               UINT16            event_result,
               BIP_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{
    API_RESULT          retval;
    BIP_HANDLE          handle;
    BIP_HEADERS         *bip_rx_hdrs;
    UINT16              i;
    UCHAR               send_request;
    BIP_IMAGE_HANDLE    image_handle;
    BIP_HEADERS         bip_tx_hdrs;
    BIP_HEADER_STRUCT   body_info;
    BIP_REQUEST_STRUCT  req_info;
    BIP_IMAGE_DESC      bip_image_desc;
    BIP_RESPONSE_STRUCT bip_rsp_info;
    UCHAR               more;
    UINT16              actual;
    UCHAR               tx_response;
    UINT16              length;
    UCHAR               *data;
    UCHAR               send_response;
    UCHAR               object_name[64U];
    UINT32              file_attribute;
    UINT16              img_desc_length;
    UCHAR               *img_desc_data;

    /* Initialize to default */
    handle = *bip_handle;
    retval = API_SUCCESS;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    BIP_INIT_HEADER_STRUCT(image_handle);
    BIP_INIT_HEADER_STRUCT (body_info);
    BIP_INIT_HEADER_STRUCT(bip_image_desc);

    bip_rsp_info.name = NULL;
    bip_rsp_info.body = NULL;
    bip_rsp_info.image_descriptor = NULL;
    bip_rsp_info.img_len_info = 0U;
    bip_rsp_info.image_handle = NULL;

    bip_rx_hdrs = event_header;
    tx_response = (UCHAR )event_result;

    send_response = 0U;
    data    = NULL;
    length  = 0x00U;
    actual  = 0x00U;
    more    = 0x00U;
    send_request = 0x00U;

    BT_mem_set(&req_info, 0, sizeof(req_info));
    BT_mem_set(&bip_tx_hdrs, 0, sizeof(bip_tx_hdrs));

    switch(event_type)
    {
    case BIP_INITIATOR_PRI_TRANSPORT_CLOSE_CNF: /* Fall Through */
    case BIP_INITIATOR_PRI_TRANSPORT_CLOSE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received %s\n", (BIP_INITIATOR_PRI_TRANSPORT_CLOSE_IND == event_type) ?
        "BIP_INITIATOR_PRI_TRANSPORT_CLOSE_IND" : "BIP_INITIATOR_PRI_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("BIP initiators Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
        {
            if (bip_initiator_pri_instances[i].handle == handle)
            {
                BT_mem_set (bip_initiator_pri_instances[i].bd_addr, 0x00,
                BT_BD_ADDR_SIZE);
            }
        }

        bip_initiator_pri_print_appl_instances ();
        break;

    case BIP_INITIATOR_SEC_TRANSPORT_CLOSE_IND: /* Fall Through */
    case BIP_INITIATOR_SEC_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("ReceivedS %s\n", (BIP_INITIATOR_SEC_TRANSPORT_CLOSE_IND == event_type) ?
        "BIP_INITIATOR_SEC_TRANSPORT_CLOSE_IND" : "BIP_INITIATOR_SEC_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("BIP initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
        {
            if (bip_initiator_sec_instances[i].handle == handle)
            {
                BT_mem_set (bip_initiator_sec_instances[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            }
        }

        bip_initiator_sec_print_appl_instances ();
        break;

    case BIP_INITIATOR_PRI_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_CONNECT_CNF\n");
        LOG_DEBUG("BIP initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bip_rx_hdrs->bip_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        bip_rx_hdrs->bip_connect_info->max_recv_size);

        /* Get the Handle parameters */
        for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
        {
            if (bip_initiator_pri_instances[i].handle == handle)
            {
                BT_mem_copy
                (
                    bip_initiator_pri_instances[i].bd_addr,
                    bip_rx_hdrs->bip_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                bip_initiator_pri_instances[i].max_xchg_size =
                    bip_rx_hdrs->bip_connect_info->max_recv_size;
                break;
            }
        }

        bip_initiator_pri_print_appl_instances ();
        break;

    case BIP_INITIATOR_SEC_CONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_CONNECT_IND\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_SUCCESS_RSP != event_result)
        {
            tx_response = BIP_NOT_ACCEPTABLE_RSP;

            send_response = 1U;

            break;
        }

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bip_rx_hdrs->bip_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        bip_rx_hdrs->bip_connect_info->max_recv_size);
        tx_response = BIP_SUCCESS_RSP;

        /* Set response to be sent */
        send_response = 1U;

        /* Get the Handle parameters */
        BT_mem_copy
        (
            bip_initiator_sec_instances[handle].bd_addr,
            bip_rx_hdrs->bip_connect_info->bd_addr,
            BT_BD_ADDR_SIZE
        );

        bip_initiator_sec_instances[handle].max_xchg_size =
            bip_rx_hdrs->bip_connect_info->max_recv_size;

        bip_initiator_sec_print_appl_instances ();

        /* Set path to root folder */
        if (0U != bip_initiator_sec_instances[handle].root_depth)
        {
            while (0U != bip_initiator_sec_instances[handle].root_depth)
            {
                retval = BT_vfops_set_path_backward(bip_initiator_sec_instances[handle].path);
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to set path backward\n");
                }
                bip_initiator_sec_instances[handle].root_depth --;
            }

            LOG_DEBUG ("Folder set to ROOT\n");
        }

        break;

    case BIP_INITIATOR_PRI_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_DISCONNECT_CNF\n");
        LOG_DEBUG("BIP initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bip_initiator_pri_print_appl_instances ();
        break;

    case BIP_INITIATOR_SEC_DISCONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_DISCONNECT_IND\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bip_initiator_sec_print_appl_instances ();
        break;

    case BIP_INITIATOR_PRI_GET_CAPABILITIES_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_CAPABILITES_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BIP Get Capabilities Object data ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

#if 0
            LOG_DEBUG("\n==========Readable Format==========\n");
            retval = parse_xml_stream (data);
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed to covert to Readable format\n");
            }
            LOG_DEBUG("\n====================================\n");
#endif /* 0 */

            /* Open file for dumping received GetCapabilities data */
            if (NULL == bip_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    (UCHAR *)"bip_rx_getcapabilities_obj.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    LOG_DEBUG ("Failed to open BIP Capabilities File\n");
                }
            }

            /* Write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

    case BIP_INITIATOR_PRI_GET_STATUS_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_STATUS_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            LOG_DEBUG("\nSecondary connection is still active\n");
        }
        else
        {
            LOG_DEBUG("\nSecondary connection is successfully terminated\n");
        }
        break;

    case BIP_INITIATOR_PRI_PUT_IMAGE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_PUT_IMAGE_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BIP_SUCCESS_RSP != event_result) &&
            (BIP_PARTIAL_CONTENT_RSP != event_result) &&
            (BIP_CONTINUE_RSP != event_result))
        {
            break;
        }

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if (NULL != bip_rx_hdrs->bip_rsp_info->image_handle->value)
        {
            LOG_DEBUG ("Received Image Handle: ");
            for (i = 0U; i < bip_rx_hdrs->bip_rsp_info->image_handle->length; i++)
            {
                LOG_DEBUG ("%c", bip_rx_hdrs->bip_rsp_info->image_handle->value[i]);
            }
            LOG_DEBUG ("\n");

            BT_mem_copy
            (
                bip_img_handle,
                bip_rx_hdrs->bip_rsp_info->image_handle->value,
                bip_rx_hdrs->bip_rsp_info->image_handle->length
            );
        }

        if (((BIP_PARTIAL_CONTENT_RSP == event_result) &&
             (BT_TRUE == appl_bip_push_image_thumbnail)) ||
            ((0x00U != appl_bip_push_image_thumbnail_flag) &&
             (0x00U != appl_bip_push_image_thumbnail) &&
             (BIP_SUCCESS_RSP == event_result)))
        {
            appl_bip_push_image_thumbnail = BT_FALSE;
            appl_bip_push_image_thumbnail_flag = 0U;

            /**
             * Hack for the 'BIP/IPSI/MFS/BV-03-C', PTS is sending the Image handle
             * for every put request.
             */
            BT_str_copy((CHAR *)bip_img_handle, (CHAR *)"1234567");

            /**
             * Received Partial Content Respsone, Send Thumbnail of the
             * Image send Earlier
             */

            if (NULL != bip_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }

            BT_sleep(1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bip_xchg_size = bip_initiator_pri_instances[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                thumbnail_file_name,
                file_object
            );

            /* Open the Thumbnail file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bip_xchg_size)
            {
                body_info.length = (UINT16 )bip_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    bip_tx_fp,
                    &actual
                );
            }

            /* Update the hdrs  for put request */
            image_handle.value = bip_img_handle;
            image_handle.length = (UINT16)BT_str_n_len(bip_img_handle, sizeof(bip_img_handle)) + 1U;;

            req_info.image_handle = &image_handle;
            req_info.body = &body_info;

            LOG_DEBUG("BT_bip_initiator_put_linked_thumbnail\n");
            retval = BT_bip_initiator_put_linked_thumbnail
                     (
                         &handle,
                         &req_info,
                         more,
                         &actual
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("BT_bip_initiator_get_linked_thumbnail Request Failed\n");
            }

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }

            /* Free memory */
            if (NULL != body_info.value)
            {
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }

            break;
        }

        /* Fill body hdr values of the Image */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        req_info.body = &body_info;

        bip_tx_hdrs.bip_req_info = &req_info;

        break;

    case BIP_INITIATOR_PRI_PUT_LINKED_THUMBNAIL_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_PUT_LINKED_THUMBNAIL_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BIP_SUCCESS_RSP == event_result) ||
            (BIP_CONTINUE_RSP != event_result))
        {
            break;
        }

        send_request = 0x01U;

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        req_info.body = &body_info;

        bip_tx_hdrs.bip_req_info = &req_info;

        break;

    case BIP_INITIATOR_PRI_PUT_LINKED_ATTACHMENT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_PUT_LINKED_ATTACHMENT_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BIP_SUCCESS_RSP != event_result) &&
            (BIP_CONTINUE_RSP != event_result))
        {
            break;
        }

        /* Send Request for continue resonse */
        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        req_info.body = &body_info;

        bip_tx_hdrs.bip_req_info = &req_info;

        break;

    case BIP_INITIATOR_PRI_GET_IMAGE_LIST_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_IMAGE_LIST_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BIP Get Image Listing Object data ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            LOG_DEBUG("Number of Image Handles returned: %d\n", bip_rx_hdrs->bip_rsp_info->appl_params->nb_returned_handles);

            if ((NULL != bip_rx_hdrs->bip_rsp_info->image_descriptor->value) &&
                (0U != bip_rx_hdrs->bip_rsp_info->image_descriptor->length))
            {
                img_desc_length = bip_rx_hdrs->bip_rsp_info->image_descriptor->length;
                img_desc_data = bip_rx_hdrs->bip_rsp_info->image_descriptor->value;

                /* Print received data on console */
                LOG_DEBUG ("\n====== BIP Pull Image Descriptor  ===== \n");
                for (i = 0U; i < img_desc_length; i++)
                {
                    LOG_DEBUG ("%c", img_desc_data[i]);
                }
                LOG_DEBUG ("\n================================= \n");
            }

            /* Open file for dumping received GetImageListing request data */
            if (NULL == bip_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    (UCHAR *)"bip_image_list_obj.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Folder Listing file\n");
                }
            }

            /* Write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

    case BIP_INITIATOR_PRI_GET_IMAGE_PROPERTIES_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_PROPERTIES_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== Image Properties Object Data ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            /* Open file for dumping received Get Image Properties data */
            if (NULL == bip_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    (UCHAR *)"bip_get_image_properties_obj.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Image Properties file\n");
                }
            }

            /* Write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

    case BIP_INITIATOR_PRI_GET_LINKED_THUMBNAIL_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_LINKED_THUMBNAIL_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

#if 0
            /* Print received data on console */
            LOG_DEBUG ("\n ====== BIP Pull Image Thumbnail Object data ===== \n");
            for (i = 0; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n================================= \n");
#endif
            /* Write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

    case BIP_INITIATOR_PRI_GET_LINKED_ATTACHMENT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_LINKED_ATTACHMENT_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BIP Pull Linked Attachemnt Object data ===== \n");
            for (i = 0U; i<length; i++)
            {

                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            /* Open file for dumping received Get attachment request data */
            if (NULL == bip_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                    (UCHAR *)"bip_get_image_attachment_obj.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bip_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Attachment file\n");
                }
            }

            /* Write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

     case BIP_INITIATOR_PRI_GET_IMAGE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_IMAGE_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (BIP_CONTINUE_RSP == event_result)
        {
            if(0U < bip_rx_hdrs->bip_rsp_info->img_len_info)
            {
                LOG_DEBUG("\nimage Size  ---> %d\n",bip_rx_hdrs->bip_rsp_info->img_len_info);
            }

            LOG_DEBUG("\nin countinue response\n");
            send_request = 0x01U;
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

#if 0
            /* Print received data on console */
            LOG_DEBUG ("\n\n ====== BIP Pull Object data ===== \n\n");
            LOG_DEBUG ("received data length: %d", length);
            for (i = 0; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");
#endif
            /* write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

    case BIP_INITIATOR_PRI_GET_MONITORING_IMAGE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_GET_MONITORING_CNF\n");
        LOG_DEBUG("BIP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

         if ((BIP_SUCCESS_RSP != event_result) &&
            (BIP_PARTIAL_CONTENT_RSP != event_result) &&
            (BIP_CONTINUE_RSP != event_result))
         {
             break;
         }

        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL != bip_rx_hdrs->bip_rsp_info->image_handle->value)
            && (0U < bip_rx_hdrs->bip_rsp_info->image_handle->length))
        {
            LOG_DEBUG ("Received Image Handle: ");
            for (i = 0U; i < bip_rx_hdrs->bip_rsp_info->image_handle->length; i++)
            {
                LOG_DEBUG ("%c", bip_rx_hdrs->bip_rsp_info->image_handle->value[i]);
            }
            LOG_DEBUG ("\n");

            BT_mem_copy
            (
                bip_img_handle,
                bip_rx_hdrs->bip_rsp_info->image_handle->value,
                bip_rx_hdrs->bip_rsp_info->image_handle->length
            );
        }

        if ((NULL == bip_rx_hdrs->bip_rsp_info->body) ||
            (NULL == bip_rx_hdrs->bip_rsp_info->body->value) ||
            (0U == bip_rx_hdrs->bip_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = bip_rx_hdrs->bip_rsp_info->body->value;
            length = bip_rx_hdrs->bip_rsp_info->body->length;

            /* Write to file */
            if (NULL != bip_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);

            }

            /* Close the file if final chunk is received */
            if (BIP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (bip_rx_fp);
                bip_rx_fp = NULL;
            }
        }
        break;

    case BIP_INITIATOR_PRI_REMOTE_DISPLAY_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_REMOTE_DISPLAY_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BIP_SUCCESS_RSP == event_result) ||
            (BIP_CONTINUE_RSP != event_result))
        {
            if ((NULL == bip_rx_hdrs->bip_rsp_info->image_handle) )
            {
                LOG_DEBUG ("No Image Found ");
            }
            else
            {
                LOG_DEBUG("Image handle is :%s\n", bip_rx_hdrs->bip_rsp_info->image_handle->value);
            }
            break;
        }
        break;

    case BIP_INITIATOR_SEC_GET_CAPABILITIES_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_CAPABILITIES_IND \n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bip_rsp_info.body = &body_info;
        bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

        /* send response */
        send_response = 1U;

        /* Reject if application set to reject */
        if (0U != appl_reject_opn)
        {
            tx_response = appl_reject_opn;
            LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
            break;
        }

        if (0U == sent)
        {
            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;
            tx_response = BIP_SUCCESS_RSP;

            /* Create the listing file with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                (UCHAR *)CAPABILITIES_FILE,
                file_object
            );

            /* Get the sample Get Capbilities from file */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }
        break;

    case BIP_INITIATOR_PRI_START_ARCHIVE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_START_ARCHIVE_CNF\n");
        LOG_DEBUG("BIP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case BIP_INITIATOR_PRI_START_PRINT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_START_PRINT_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BIP_SUCCESS_RSP != event_result) &&
            (BIP_CONTINUE_RSP != event_result))
        {
            break;
        }

        /* Send Request for continue resonse */
        if (BIP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        req_info.body = &body_info;

        bip_tx_hdrs.bip_req_info = &req_info;

        break;

    case BIP_INITIATOR_PRI_DELETE_IMAGE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_PRI_DELETE_IMAGE_CNF\n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case BIP_INITIATOR_SEC_DELETE_IMAGE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_RESPONDER_SEC_DELETE_IMAGE_IND\n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* send response */
        send_response = 1U;

        tx_response = BIP_SUCCESS_RSP;

        /* Delete image_handle  */
        if ((NULL != bip_rx_hdrs->bip_req_info) &&
            (NULL != bip_rx_hdrs->bip_req_info->image_handle) &&
            (NULL != bip_rx_hdrs->bip_req_info->image_handle->value) &&
            (0U != bip_rx_hdrs->bip_req_info->image_handle->length))
        {
            LOG_DEBUG("Delete IMAGE HANDLE:");
            for (i = 0U; i <= bip_rx_hdrs->bip_req_info->image_handle->length; i++)
            {
                LOG_DEBUG("%c", bip_rx_hdrs->bip_req_info->image_handle->value[i]);
            }
            LOG_DEBUG("\n");

            /* Reject if operation reject is set or if Read ONly is set */
            if (0U != appl_bip_initiator_read_only)
            {
                tx_response = appl_bip_initiator_read_only;
                LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                break;
            }
            else if (0U != appl_reject_opn)
            {
                tx_response = appl_reject_opn;
                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                break;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                (UCHAR *)SAMPLE_IMAGE,
                file_object
            );

            retval = BT_fops_get_file_attributes
                     (
                         file_object,
                         &file_attribute
                     );

            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }

            if ((API_SUCCESS == retval) &&
                (BT_FOPS_MASK_READONLY == (file_attribute & BT_FOPS_MASK_READONLY)))
            {
                LOG_DEBUG ("object cannot be deleted : %s",object_name);
                tx_response = OBEX_UNAUTHORIZED_RSP;
            }
            else
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
               (void)BT_fops_file_close(bip_tx_fp);
               bip_tx_fp = NULL;

               (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_object);
            }
        }
        break;

    case BIP_INITIATOR_SEC_GET_IMAGE_LIST_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_IMAGE_LIST_IND \n");
        LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
        bip_rsp_info.body             = &body_info;
        bip_rsp_info.image_descriptor = &bip_image_desc;
        bip_rsp_info.appl_params      = &appl_param;
        bip_tx_hdrs.bip_rsp_info      = &bip_rsp_info;

        /* Check for appl hdr in the request */
        if(NULL == bip_rx_hdrs->bip_req_info->appl_params)
        {
            tx_response = BIP_BAD_REQ_RSP;
            break;
        }

        LOG_DEBUG("Number of Image Handles to be returned: %d\n",
           bip_rx_hdrs->bip_req_info->appl_params->nb_returned_handles);

        LOG_DEBUG("List Start Offset: %d\n",
           bip_rx_hdrs->bip_req_info->appl_params->list_start_offset);

        LOG_DEBUG("Latest Captured Images Flag :%d\n",
           bip_rx_hdrs->bip_req_info->appl_params->latest_captured_images);

        /*set application parameter */
        BIP_SET_APPL_PARAM_FLAG
        (
            appl_param.appl_param_flag,
            BIP_FLAG_NB_RETURNED_HANDLES
        );
        appl_param.nb_returned_handles = bip_rx_hdrs->bip_req_info->appl_params->nb_returned_handles;

        LOG_DEBUG("Number of Image Handles returned: %d\n", bip_rsp_info.appl_params->nb_returned_handles);

        /* Check for Image Descriptor Header in the request */
        if(NULL == bip_rx_hdrs->bip_req_info->image_descriptor->value)
        {
            tx_response = BIP_BAD_REQ_RSP;
            break;
        }

        if ((NULL != bip_rx_hdrs->bip_req_info->image_descriptor->value) &&
            (0U != bip_rx_hdrs->bip_req_info->image_descriptor->length))
        {
            length = bip_rx_hdrs->bip_req_info->image_descriptor->length;
            data = bip_rx_hdrs->bip_req_info->image_descriptor->value;

            /* Print received data on console */
            LOG_DEBUG ("\n ====== BIP Get Image List Descriptor  ===== \n\n");
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");
        }

        /*  Set image descriptor */
        bip_image_desc.value = bip_rx_hdrs->bip_req_info->image_descriptor->value;
        bip_image_desc.length = bip_rx_hdrs->bip_req_info->image_descriptor->length;

        /* send response */
        send_response = 1U;

        /* Reject if operation reject */
        if (0U != appl_reject_opn)
        {
            tx_response = appl_reject_opn;
            LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
            break;
        }

        if (0U == sent)
        {
            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;
            tx_response = BIP_SUCCESS_RSP;

            /* Create the listing file with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                (UCHAR *)IMAGE_LIST_FILE,
                file_object
            );

            /* Get the Sample listing from file */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }
        break;

    case BIP_INITIATOR_SEC_GET_IMAGE_PROPERTIES_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_IMAGE_PROPERTIES_IND \n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bip_rsp_info.body = &body_info;
        bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

        /* send response */
        send_response = 1U;

        /* Reject if operation reject */
        if (0U != appl_reject_opn)
        {
            tx_response = appl_reject_opn;
            LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

            break;
        }

        /* Check for Name hdr in the request */
        if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
           (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
           (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
        {
            tx_response = BIP_BAD_REQ_RSP;
            break;
        }
        else
        {
            LOG_DEBUG ("Received Image Handle: %s",
                bip_rx_hdrs->bip_req_info->image_handle->value);
        }

        if (0U == sent)
        {
            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;
            tx_response = BIP_SUCCESS_RSP;

            /* Create the listing file with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                (UCHAR *)IMAGE_PROPERTIES_FILE,
                file_object
            );

            /* Get the sample Image Properties from file */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }
        break;

    case BIP_INITIATOR_SEC_GET_LINKED_THUMBNAIL_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_LINKED_THUMBNAIL_IND\n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* send response */
        send_response = 1U;

        bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

        bip_rsp_info.body = &body_info;

        if (0U == sent)
        {
            /* Check for image handle hdr in the request */
            if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
               (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
               (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
            {
                tx_response = BIP_BAD_REQ_RSP;

                break;
            }
            else
            {
                LOG_DEBUG ("Received Image Handle: %s",
                    bip_rx_hdrs->bip_req_info->image_handle->value);
            }

            /* Check if operation is rejected */
            if (0U != appl_reject_opn)
            {
                tx_response = appl_reject_opn;
                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
                break;
            }
            else
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_ROOT_FOLDER_BASE,
                    (UCHAR *)SAMPLE_IMAGE_THUMBNAIL,
                    file_object
                );
            }

            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        /* Fill the Image Size */
        bip_rsp_info.img_len_info = remaining;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            actual = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }
        break;

    case BIP_INITIATOR_SEC_GET_IMAGE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_IMAGE_IND\n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        bip_rsp_info.img_len_info = 0U;

        /* send response */
        send_response = 1U;

        bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

        bip_rsp_info.body = &body_info;
        if (0U == sent)
        {
            /* Check for image handle hdr in the request */
            if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
               (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
               (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
            {
                tx_response = BIP_BAD_REQ_RSP;
                break;
            }
            else
            {
                initiator_image_handle = bip_rx_hdrs->bip_req_info->image_handle->value;
                LOG_DEBUG ("Received Image Handle: %s\n",
                    bip_rx_hdrs->bip_req_info->image_handle->value);

                if ((NULL != bip_rx_hdrs->bip_req_info->image_descriptor->value) &&
                    (0U != bip_rx_hdrs->bip_req_info->image_descriptor->length))
                {
                    length = bip_rx_hdrs->bip_req_info->image_descriptor->length;
                    data = bip_rx_hdrs->bip_req_info->image_descriptor->value;

                    /* Print received data on console */
                    LOG_DEBUG ("\n====== BIP Get Image Descriptor  ===== \n");
                    for (i = 0U; i < length; i++)
                    {
                        LOG_DEBUG ("%c", data[i]);
                    }
                    LOG_DEBUG ("\n ================================= \n");
                }
            }

            /* Check if operation is rejected */
            if (0U != appl_reject_opn)
            {
                tx_response = appl_reject_opn;
                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
                break;
            }
            else
            {
                if(0 == BT_str_cmp(initiator_image_handle,"1000001"))
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_ROOT_FOLDER_BASE,
                        (UCHAR *)SAMPLE_IMAGE1,
                        file_object
                    );
                    LOG_DEBUG("\n%s\n",file_object);
                }
                else if(0 == BT_str_cmp(initiator_image_handle,"1000002"))
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_ROOT_FOLDER_BASE,
                        (UCHAR *)SAMPLE_IMAGE2,
                        file_object
                    );
                    LOG_DEBUG("\n%s\n",file_object);
                }
                else if(0 == BT_str_cmp(initiator_image_handle,"1000003"))
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_ROOT_FOLDER_BASE,
                        (UCHAR *)SAMPLE_IMAGE3,
                        file_object
                    );
                    LOG_DEBUG("\n%s\n",file_object);
                }
                else
                {
                    LOG_DEBUG("\n No Image for the handle:%s\n",initiator_image_handle);
                    tx_response = BIP_NOT_FOUND_RSP;
                    break;
                }
            }

            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;

                /* Fill the Image size */
                bip_rsp_info.img_len_info = fsize; /*remaining*/
                LOG_DEBUG("Image Size: %d\n" , bip_rsp_info.img_len_info);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }
        }
        else
        {       /* Fill the Image size */
                bip_rsp_info.img_len_info = 0U;
                LOG_DEBUG("Image Size: %d\n" , bip_rsp_info.img_len_info);
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            actual = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }
        break;

    case BIP_INITIATOR_SEC_GET_LINKED_ATTACHMENT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_LINKED_ATTACHMENT_IND \n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bip_rsp_info.body = &body_info;
        bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

        /* send response */
        send_response = 1U;

        /* Reject if operation reject */
        if (0U != appl_reject_opn)
        {
            tx_response = appl_reject_opn;
            LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
            break;
        }

        if (0U == sent)
        {
            /* Check for Name hdr in the request */
            if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
                (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
                (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
            {
                tx_response = BIP_BAD_REQ_RSP;
                break;
            }
            else
            {
                LOG_DEBUG("Received Image Handle:");
                for (i = 0U; i <= bip_rx_hdrs->bip_req_info->image_handle->length; i++)
                {
                    LOG_DEBUG("%c", bip_rx_hdrs->bip_req_info->image_handle->value[i]);
                }
                LOG_DEBUG("\n");
            }

            /* Check for Name hdr in the request */
            if((NULL == bip_rx_hdrs->bip_req_info->name) ||
                (NULL == bip_rx_hdrs->bip_req_info->name->value) ||
                (0U == bip_rx_hdrs->bip_req_info->name->length))
            {
                tx_response = BIP_BAD_REQ_RSP;
                break;
            }
            else
            {
                LOG_DEBUG("Image Name: ");
                for(i = 0U; i <= bip_rx_hdrs->bip_req_info->name->length; i++)
                {
                    LOG_DEBUG("%c", bip_rx_hdrs->bip_req_info->name->value[i]);
                }
                LOG_DEBUG("\n");
            }

            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;
            tx_response = BIP_SUCCESS_RSP;

            /* Create the Linked Attachment file with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BIP_ROOT_FOLDER_BASE,
                (UCHAR *)IMAGE_ATTACHMENT_FILE,
                file_object
            );

            /* Open the file */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )bip_xchg_size;
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            actual = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }
        break;

     case BIP_INITIATOR_SEC_GET_PARTIAL_IMAGE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_INITIATOR_SEC_GET_PARTIAL_IMAGE_IND\n");
        LOG_DEBUG("BIP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* send response */
        send_response = 1U;

        BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
        bip_rsp_info.body             = &body_info;
        bip_rsp_info.appl_params      = &appl_param;
        bip_tx_hdrs.bip_rsp_info      = &bip_rsp_info;

        if (0U == sent)
        {
            if((NULL == bip_rx_hdrs->bip_req_info->name) ||
                (NULL == bip_rx_hdrs->bip_req_info->name->value) ||
                (0U == bip_rx_hdrs->bip_req_info->name->length))
            {
                tx_response = BIP_BAD_REQ_RSP;
                break;
            }
            else
            {
                LOG_DEBUG("Image Name: ");
                for(i = 0U; i <= bip_rx_hdrs->bip_req_info->name->length; i++)
                {
                    LOG_DEBUG("%c", bip_rx_hdrs->bip_req_info->name->value[i]);
                }
            }

            LOG_DEBUG("\nPartial file  Start Offset: 0x%08x\n",
                bip_rx_hdrs->bip_req_info->appl_params->partial_file_start_offset);

            LOG_DEBUG("Partial file image size :0x%8x\n",
                bip_rx_hdrs->bip_req_info->appl_params->partial_file_length);

            /* Check if operation is rejected */
            if (0U != appl_reject_opn)
            {
                tx_response = appl_reject_opn;
                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
                break;
            }
            else
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BIP_ROOT_FOLDER_BASE,
                    (UCHAR *)SAMPLE_IMAGE3,
                    file_object
                );
            }

            bip_xchg_size = bip_initiator_sec_instances[handle].max_xchg_size;
#if 1
            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &bip_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bip_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bip_tx_fp, &fsize);
                remaining = fsize;

                /* set application parameter */
                BIP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    BIP_FLAG_TOTAL_FILE_SIZE
                );
                appl_param.total_file_size = fsize;

                /* Fill the Image size */
                bip_rsp_info.img_len_info = fsize; /* remaining */
            }
#endif

#if 0
            /* Get the sample listing from file */
            bip_tx_fp = fopen (SAMPLE_IMAGE3, "rb");
            if (NULL != bip_tx_fp)
            {
                /* Get the file size */
                fseek (bip_tx_fp, 0L, SEEK_END);
                fsize = (UINT16)ftell(bip_tx_fp);
                rewind(bip_tx_fp);
                /*set application parameter */
                 BIP_SET_APPL_PARAM_FLAG
                 (
                    appl_param.appl_param_flag,
                    BIP_FLAG_TOTAL_FILE_SIZE
                 );
                appl_param.total_file_size = fsize;
                /* Fill the Image size */
                bip_rsp_info.img_len_info = fsize; /*remaining*/
                remaining = fsize;
            }
#endif
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                tx_response = BIP_NOT_FOUND_RSP;
                break;
            }

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_seek(bip_tx_fp, bip_rx_hdrs->bip_req_info->appl_params->partial_file_start_offset,SEEK_SET);
            if (0xFFFFFFFFU == bip_rx_hdrs->bip_req_info->appl_params->partial_file_length)
            {
                /* Send Complete Image */
                remaining = fsize;
            }
            else
            {
                remaining = bip_rx_hdrs->bip_req_info->appl_params->partial_file_length; /*fsize - bip_rx_hdrs->bip_req_info->appl_params->partial_file_start_offset;*/
            }

            fsize = remaining;
        }
        else
        {
            /* Fill the Image size */
            bip_rsp_info.img_len_info = 0U;
            LOG_DEBUG("Image Size: %d\n" , bip_rsp_info.img_len_info);
        }

        /* Fill body hdr values */
        if (remaining > bip_xchg_size)
        {
            body_info.length = (UINT16 )(bip_xchg_size - 20U);
            tx_response = BIP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BIP_SUCCESS_RSP;

            /* Final Chunk */
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_END_FLAG
            );
            appl_param.end_flag = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        if (NULL != body_info.value)
        {
#if 1
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                bip_tx_fp,
                &actual
            );
#endif
           /* fread(body_info.value, body_info.length, 1, bip_tx_fp);*/
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            actual = 0U;
            LOG_DEBUG("\n === in last chunck === \n");

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bip_tx_fp);
            bip_tx_fp = NULL;
        }

        break;

    default:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BIP_EVENT_INVALID\n");
        break;
    }

    if (0U != send_request)
    {
        if (0U < srmp_wait_count)
        {
            req_info.wait = BT_TRUE;
            bip_tx_hdrs.bip_req_info = &req_info;
            srmp_wait_count--;
        }

        retval = BT_bip_initiator_send_request
                 (
                     &bip_initiator_pri_instances[handle].handle,
                     event_type,
                     event_result,
                     &bip_tx_hdrs,
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("BIP Initiator Send Request Failed - 0x%04X\n", retval);
        }

        if ((BIP_INITIATOR_PRI_PUT_IMAGE_CNF == event_type) ||
            (BIP_INITIATOR_PRI_PUT_LINKED_THUMBNAIL_CNF == event_type) ||
            (BIP_INITIATOR_PRI_PUT_LINKED_ATTACHMENT_CNF == event_type) ||
            (BIP_INITIATOR_PRI_START_PRINT_CNF == event_type))
        {
            /* Free memory */
            if (NULL != body_info.value)
            {
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            LOG_DEBUG("read length = %d, actual sent = %d\n", body_info.length, actual);

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                LOG_DEBUG("appl_bip_initiator_callback: Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bip_tx_fp, sent, SEEK_SET);
            }

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }
        }
    }

    if (0U != send_response)
    {
        retval = BT_bip_initiator_send_response
                 (
                     &handle,
                     event_type,
                     tx_response,
                     &bip_tx_hdrs
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("Failed to send BIP initiator Response - 0x%04x\n", retval);
            tx_response = 0xFFU;
        }

        if ((BIP_CONTINUE_RSP != tx_response) &&
            (BIP_SUCCESS_RSP != tx_response))
        {
            if (NULL != bip_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bip_tx_fp);
                bip_tx_fp = NULL;
            }
        }

        if (NULL != body_info.value)
        {
            BT_free_mem(body_info.value);
            body_info.value = NULL;
        }
    }

    return retval;
}

void appl_bip_initiator_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    API_RESULT retval;
    UINT16 num_attrib_ids;

    S_UUID uuid[3U];

#ifndef OBEX_OVER_L2CAP
    UINT16 attrib_id[1U] = {PROTOCOL_DESC_LIST};

    num_attrib_ids = 1U;
#else /* OBEX_OVER_L2CAP */
    UINT16 attrib_id[2U] = {PROTOCOL_DESC_LIST,
                           GOEP_L2CAP_PSM_ATTR_ID};

    num_attrib_ids = 2U;
#endif /* OBEX_OVER_L2CAP */

    BT_IGNORE_UNUSED_PARAM(length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - BIP_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer BIP Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16= IMAGING_RESPONDER_UUID;
    uuid[1U].uuid_union.uuid_16 =L2CAP_UUID;
    uuid[2U].uuid_union.uuid_16 =RFCOMM_UUID;

    switch(command)
    {
    case SDP_Open:
        if(status != API_SUCCESS)
        {
            LOG_DEBUG("SDP OPEN FAILED!\n");
            break;
        }
        LOG_DEBUG("SDP OPEN\n\n");

        LOG_DEBUG("Initiating SDP Service Attribute Request ....\n");
        retval = BT_sdp_servicesearchattributerequest
                 (
                     &bip_sdp_handle,
                     uuid,
                     3U,
                     attrib_id,
                     num_attrib_ids,
                     NULL,
                     0U,
                     bip_attrib_data,
                     &bip_attrib_data_len
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for BIP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&bip_sdp_handle);
        }
        break;

    case SDP_Close:
        LOG_DEBUG("SDP CLOSE\n\n");
        break;

    case SDP_ErrorResponse:
        LOG_DEBUG("SDP ERROR RESPONSE\n\n");
        break;

    case SDP_ServiceSearchAttributeResponse:
        LOG_DEBUG("\n");

        {
            UCHAR no_server_channels;
            UCHAR * sdp_query_result;

#ifdef OBEX_OVER_L2CAP
            UCHAR attr_val_2B[2U];
            UINT16 attr_size;
            UINT16 psm;
#endif /* OBEX_OVER_L2CAP */

            sdp_query_result = data;
            no_server_channels = 1U;

            while (sdp_query_result != NULL)
            {
                sdp_query_result =
                BT_sdp_get_Pointer_to_next_AttributeList
                (
                    data,
                    no_server_channels
                );

                if (NULL == sdp_query_result)
                {
                    continue;
                }

#ifdef OBEX_OVER_L2CAP
                psm = 0x0000U;

                /* Check for OBEX L2CAP PSM for BIP in the Protocol Descriptor List */
                retval = BT_sdp_get_psm (sdp_query_result, &psm);
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find PSM in Protocol Descriptor List\n");
                }
                else
                {
                    LOG_DEBUG("PSM: 0x%04X\n", psm);
                }

                /*
                 * PSM not found in Protocol Descriptor List.
                 * Check for GOEP L2CAP PSM attribute
                 */
                if ((API_SUCCESS != retval) || (RFCOMM_PSM == psm))
                {
                    attr_size = 2U;
                    /* Get the GOEP L2CAP PSM */
                    retval = BT_sdp_get_attr_value
                             (
                                 GOEP_L2CAP_PSM_ATTR_ID,
                                 sdp_query_result,
                                 attr_val_2B,
                                 &attr_size
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find GOEP L2CAP PSM\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_2_BYTE(&bip_l2cap_psm, attr_val_2B);
                        LOG_DEBUG("GOEP L2CAP PSM: 0x%04X\n", bip_l2cap_psm);
                    }
                }

                /* if ((API_SUCCESS != retval) && (RFCOMM_PSM == psm)) */
#endif /* OBEX_OVER_L2CAP */
                {
                    /* No PSMs found, check the BIP server channel number */
                    retval = BT_sdp_get_channel_number
                             (
                                 sdp_query_result,
                                 &bip_server_channel
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find BIP Responder Server Channel\n");
                    }
                    else
                    {
                        LOG_DEBUG("Server Channel: 0x%02X\n", bip_server_channel);

                        /*
                         * Store the Server Channel globally.
                         * Later it will be used while Creating BIP Connection
                         */
                    }
                }

                /* BIP PSM or Server channel found */
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("BIP Service Found\n");
                }
                else
                {
                    LOG_DEBUG ("Failed to find BIP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&bip_sdp_handle);

#ifdef APPL_ENABLE_AUTOPROCEDURE
        if (BT_TRUE == bip_autoconn)
        {
            BT_sleep(1U);

            bip_autoconn = BT_FALSE;

            connect_info.bd_addr = bip_peer_addr;

#ifdef OBEX_OVER_L2CAP
            if (0x01U == bip_transport)
            {
                connect_info.psm = bip_l2cap_psm;
                connect_info.server_channel = 0U;
            }
            else
#endif /* OBEX_OVER_L2CAP */
            {
                connect_info.server_channel = bip_server_channel;
                connect_info.psm = 0U;
            }

            connect_info.max_recv_size = 1024U;

            LOG_DEBUG ("Connecting...\n");
            retval = BT_bip_initiator_connect
                     (
                         &bip_initiator_pri_instances[bipc_handle].handle,
                         &connect_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
        }
#endif /* APPL_ENABLE_AUTOPROCEDURE */
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
}


void bip_initiator_pri_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n================PRIMARY BIP CONNECTION===================\n");
    LOG_DEBUG ("Instance  BIP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", bip_initiator_pri_instances[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        bip_initiator_pri_instances[i].bd_addr[0U], bip_initiator_pri_instances[i].bd_addr[1U],
        bip_initiator_pri_instances[i].bd_addr[2U], bip_initiator_pri_instances[i].bd_addr[3U],
        bip_initiator_pri_instances[i].bd_addr[4U], bip_initiator_pri_instances[i].bd_addr[5U]);

        LOG_DEBUG ("\n");
    }
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

void bip_initiator_sec_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n================SECONDARY BIP CONNECTION===================\n");
    LOG_DEBUG ("Instance  BIP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < BIP_NUM_INITIATOR_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", bip_initiator_sec_instances[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        bip_initiator_sec_instances[i].bd_addr[0U], bip_initiator_sec_instances[i].bd_addr[1U],
        bip_initiator_sec_instances[i].bd_addr[2U], bip_initiator_sec_instances[i].bd_addr[3U],
        bip_initiator_sec_instances[i].bd_addr[4U], bip_initiator_sec_instances[i].bd_addr[5U]);

        LOG_DEBUG ("\n");
    }
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}
#endif /* BIP */
