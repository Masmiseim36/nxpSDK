
/**
 *  \file appl_bip_responder.c
 *
 *  Source file for BIP Responder Command Line Test Application.
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
static BIP_INSTANCE         bip_responder_pri_instances[BIP_NUM_RESPONDER_INSTANCE];
static BIP_INSTANCE         bip_responder_sec_instances[BIP_NUM_RESPONDER_INSTANCE];
static BIP_CONNECT_STRUCT   connect_info;
static UCHAR                bip_push_started;
static UCHAR                bd_addr[BT_BD_ADDR_SIZE];
static BT_fops_file_handle  bip_rx_fp;
static BT_fops_file_handle  bip_tx_fp;
static SDP_HANDLE           bip_sdp_handle;
static UCHAR                bip_server_channel;
static UINT16               bip_l2cap_psm;
static UINT32               fsize;
static UINT32               remaining, sent;
static UINT32               bip_xchg_size;
static UCHAR                appl_bip_responder_read_only;
static UCHAR                bip_attrib_data[BIP_SDP_RECORD_DATA_SIZE];
static UINT16               bip_attrib_data_len = BIP_SDP_RECORD_DATA_SIZE;
static UCHAR                appl_reject_opn;
static BIP_IMAGE_HANDLER    appl_image_handle[BIP_IMAGE_HANDLE_SIZE];
static UCHAR                appl_responder_img_handle_cnt;
static UCHAR                thumbnail_file_name[64U];
static UCHAR                attachment_file_name[64U];
static UCHAR                *responder_image_handle;
static BIP_APPL_PARAMS      appl_param;
static UCHAR                file_object[BIP_FOLDER_NAME_LEN * 2U];
static UCHAR                feature_flag;
static BIP_IMAGE_HANDLER    bip_res_img_handle[BIP_IMAGE_HANDLE_SIZE];
static UCHAR                start_date[8U], end_date[8U];
static UCHAR                file_name[40U];
static UCHAR                image_thumbnail_name[32U], image_file_name[32U];
static UCHAR                image_descriptor_data[128U];
static UCHAR                image_handles_descriptor_data[256U];
static UINT16               img_width, img_height;
static UCHAR                *img_frmt;
static UCHAR                sec_obex_connection;

/* SRMP wait count */
static UCHAR srmp_wait_count;

/**
 *  Flag to send 'BIP_PARTIAL_CONTENT_RSP' to get the ThumbNail of PushImage
 *  after the PushImage operation.
 */
static UCHAR                appl_get_linked_thumbnail_flag = 0U;

static const UCHAR bip_responder_menu[] =
" \n \
-------------- BIP Responder Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. BIP Responder Initialize \n \
\t 3. BIP Responder Shutdown \n \
\t 4. BIP Responder Start \n \
\t 5. BIP Responder Stop \n \
\n \
\t 6. BIP Responder Reject Operation\n\
\n\
\t 7. Register Peer BD Addrs. \n\
\t 8. Get Registered Peer BD Addrs. \n\
\n \
\t 12. Do SDP Query(for secondary obex connection)\n\
\n \
\t 15. Connect to BIP Initiator(for secondary obex connection) \n \
\t 16. Disconnect from BIP Initiator(for secondary obex connection) \n \
\t 17. Close Primary session Transport Connection with BIP Initiator \n \
\t 18. Close Secondary session Transport Connection with BIP Initiator \n \
\n \
======= Advanced Image Printing Operations ========= \n \
\t 21. Get Partial Image \n \
======= Automatic Archive Operations ========= \n \
\t 20. Get Capabilities \n \
\t 24. Get Image List \n\
\t 25. Get Image Properties \n\
\t 26. Get Image Linked Thumbnail \n\
\t 27. Get Image  \n\
\t 28. Get Image Linked Attachment \n\
\t 34. Delete Image\n\
\n \
\t 40. Want to get the Linked ThumbNail after PutImage?\n\
\n \
\t 50. BIP Respond set number of SRMP wait stages \n\
Your Choice: ";

/* ----------------------------------------- Functions */

void main_bip_responder_operations (void)
{
    API_RESULT  retval;
    UINT16      i;
    UINT8       *phandle;
    UINT16      size;
    int         choice, menu_choice, handle, server_ch, val, option, service_type;
    UCHAR       img_type;
    UINT16      image_thumbnail_name_len=0;

    /* For secondary obex connection */
    BIP_REQUEST_STRUCT  req_info;
    BIP_HEADER_STRUCT   name_info, image_handle_info;
    BIP_IMAGE_DESC      image_descriptor_info;

    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG ("%s", bip_responder_menu);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:

            LOG_DEBUG ("Initializing BIP Responder...\n");
            retval = BT_bip_responder_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & BIP handle */
                for (i = 0U; i < BIP_NUM_RESPONDER_INSTANCE; i++)
                {
                    BIP_RESET_INSTANCE(&bip_responder_pri_instances[i]);
                    BIP_RESET_INSTANCE(&bip_responder_sec_instances[i]);
                }
            }

            appl_reject_opn = 0U;

            bip_responder_pri_print_appl_instances ();

            bip_responder_sec_print_appl_instances ();
            break;

        case 3:
            LOG_DEBUG ("Shutting Down BIP Responder...\n");
            retval = BT_bip_responder_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            bip_responder_pri_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Responder Instance:");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            LOG_DEBUG(" Imaging Features : \n");
            LOG_DEBUG("     0 => IMAGE PUSH\n");
            LOG_DEBUG("     1 => IMAGE PULL\n");
            LOG_DEBUG("     2 => BIP ADVANCED IMAGE PRINTING\n");
            LOG_DEBUG("     3 => BIP AUTOMATIC ARCHIVE\n");
            LOG_DEBUG("     4 => BIP REMOTE CAMERA\n");
            LOG_DEBUG("     5 => BIP REMOTE DISPLAY\n");
            LOG_DEBUG ("Please Select the feature : ");
            scanf ("%d", &option);

            if (5U < option)
            {
                LOG_DEBUG("Invalid feature\n");
                break;
            }

            if((2U == option) || (3U == option))
            {
                feature_flag = (UCHAR)option;

                LOG_DEBUG("Start Primary/Secondary session (0/1)\n");

                scanf("%d",&service_type);
            }
            else
            {
                /* Only Primary session */
                service_type = 0;
            }

            /* BIP application instance handle */
            if(service_type == 1U)
            {
                phandle = &bip_responder_sec_instances[handle].handle;
            }
            else
            {
                phandle = &bip_responder_pri_instances[handle].handle;
            }
            if (BIP_ENTITY_INVALID != *phandle)
            {
                LOG_DEBUG ("Application Instance not free!\n");
                break;
            }

            LOG_DEBUG ("Starting BIP Responder...\n");
            retval = BT_bip_responder_start
                     (
                         phandle,
                         appl_bip_responder_callback,
                         (UCHAR )option,
                         (UCHAR )service_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = BIP_ENTITY_INVALID;
            }
            else
            {
                /* Set path to root folder */
                BT_str_copy
                (
                    bip_responder_pri_instances[handle].path,
                    BIP_ROOT_FOLDER_BASE
                );

                bip_responder_pri_instances[handle].root_depth = 0U;

                appl_responder_img_handle_cnt = 0x01U;
            }

            if(service_type == 1U)
            {
                bip_responder_sec_print_appl_instances ();
            }
            else
            {
                bip_responder_pri_print_appl_instances ();
            }

            break;

        case 5:
            bip_responder_pri_print_appl_instances();

            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            LOG_DEBUG(" Imaging Features : \n");
            LOG_DEBUG("     0 => IMAGE PUSH\n");
            LOG_DEBUG("     1 => IMAGE PULL\n");
            LOG_DEBUG("     2 => BIP ADVANCED IMAGE PRINTING\n");
            LOG_DEBUG("     3 => BIP AUTOMATIC ARCHIVE\n");
            LOG_DEBUG("     4 => BIP REMOTE CAMERA\n");
            LOG_DEBUG("     5 => BIP REMOTE DISPLAY\n");
            LOG_DEBUG ("Please Select the feature : ");
            scanf ("%d", &option);

            if((2U == option) || (3U == option))
            {
                LOG_DEBUG("Stop Primary/Secondary session (0/1)\n");
                scanf("%d",&service_type);
            }
            else
            {
                /* Only Primary session */
                service_type = 0;
            }

            /* BIP application instance handle */
            if(service_type == 1U)
            {
                phandle = &bip_responder_sec_instances[handle].handle;
            }
            else
            {
                phandle = &bip_responder_pri_instances[handle].handle;
            }

            LOG_DEBUG ("Stopping  BIP Responder...\n");
            retval = BT_bip_responder_stop
                     (
                        phandle,
                        (UCHAR )service_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = BIP_ENTITY_INVALID;

                BT_mem_set
                (
                    bip_responder_pri_instances[handle].bd_addr,
                    0x00,
                    BT_BD_ADDR_SIZE
                );
            }

            bip_responder_pri_print_appl_instances ();

            break;

        case 6:
            LOG_DEBUG ("Select Below options:\n");
            LOG_DEBUG ("    0     -> BIP_SUCCESS_RSP\n");
            LOG_DEBUG ("  0xA6    -> BIP_PARTIAL_CONTENT_RSP \n");
            LOG_DEBUG ("  0x<Err> -> Error code\n");
            scanf("%x",&option);
            appl_reject_opn = (UCHAR )option;
            break;

        case 7:
            /* Register Peer BD address */
            LOG_DEBUG("Enter Peer BD Address:");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr);
            break;

        case 8:
            /* Get Registered BD_ADDR of Peer Device */
            LOG_DEBUG("\nRegistered Peer BD "\
                BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR(bd_addr));
            break;

        case 12:
            LOG_DEBUG ("Enter BIP Responder instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

#if 0
            LOG_DEBUG ("Enter peer device address: ");
            appl_get_bd_addr (bd_addr);
#endif /* 0 */

            SDP_SET_HANDLE
            (
                bip_sdp_handle,
                bd_addr,
                appl_bip_responder_sdp_callback
            );

            retval = BT_sdp_open(&bip_sdp_handle);

            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            bip_responder_pri_print_appl_instances();

            LOG_DEBUG ("enter BIP responder instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

#if 0
            LOG_DEBUG ("Enter peer device address: ");
            appl_get_bd_addr (bd_addr);
#endif /* 0 */

            connect_info.bd_addr = bd_addr;

#ifdef OBEX_OVER_L2CAP
            LOG_DEBUG ("Enter peer L2CAP PSM (in Hex): ");
            scanf ("%x", &val);
            connect_info.psm = (UINT16)val;
#endif /* OBEX_OVER_L2CAP */

            LOG_DEBUG ("Enter peer server channel (in Hex): ");
            scanf ("%x", &server_ch);
            connect_info.server_channel = (UINT8 )server_ch;

            LOG_DEBUG ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            size = (UINT16) val;
            connect_info.max_recv_size = size;

            LOG_DEBUG ("Connecting...\n");
            LOG_DEBUG("\nconnect handle -->%x\n",bip_responder_sec_instances[handle].handle);
            retval = BT_bip_responder_connect
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &connect_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            sec_obex_connection = 1U;
            break;

        case 16:
            LOG_DEBUG ("enter BIP responder instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            LOG_DEBUG ("Disconnecting BIP responder secondary Instance %d\n", handle);
            retval = BT_bip_responder_disconnect (&bip_responder_sec_instances[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            sec_obex_connection = 0U;
            break;

        case 17:
            LOG_DEBUG ("Enter BIP responder instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            LOG_DEBUG ("Disconnecting BIP Primary Session Transport with BIP Initiator\n");
            retval = BT_bip_responder_pri_transport_close
                     (
                         &bip_responder_pri_instances[handle].handle
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 18:
            LOG_DEBUG ("Enter BIP responder instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            LOG_DEBUG ("Disconnecting BIP Secondary session Transport with BIP Initiator\n");
            retval = BT_bip_responder_sec_transport_close
                     (
                         &bip_responder_sec_instances[handle].handle
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

         case 20:
            LOG_DEBUG ("Enter BIP responder instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            bip_responder_sec_print_appl_instances ();
            LOG_DEBUG ("Requesting to Get capabilities.....\n");
            retval = BT_bip_responder_get_capabilities
                     (
                         &bip_responder_sec_instances[handle].handle
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

          case 21:
            bip_responder_sec_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));
            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            LOG_DEBUG ("Enter The Image File  Name: ");
            scanf ("%s", file_name);
            name_info.value = file_name;
            name_info.length = (UINT16 )(BT_str_n_len (file_name, sizeof(file_name)) + 1U);

            LOG_DEBUG("Enter the Partial File Length(in Hex): ");
            scanf("%x", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_PARTIAL_FILE_LENGTH
            );
            appl_param.partial_file_length = (UINT32)val;

            LOG_DEBUG("Enter the Partial File start offset(in Hex): ");
            scanf("%x", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_PARTIAL_FILE_START_OFFSET
            );
            appl_param.partial_file_start_offset = (UINT32)val;

            req_info.appl_params = &appl_param;
            req_info.name = &name_info;

            LOG_DEBUG ("Requesting to Get Partial Image .....\n");
            retval = BT_bip_responder_get_partial_image
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );

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
                    (UCHAR *)"partial_img.jpg",
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
                    LOG_DEBUG ("Failed to File Open\n");
                }
            }
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 24:
            bip_responder_sec_print_appl_instances ();

            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            BIP_INIT_HEADER_STRUCT (image_descriptor_info);
            BIP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            /* Update request info for application parameters */
            req_info.appl_params = &appl_param;

            LOG_DEBUG("Number of Image Handles to be returned: ");
            scanf("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_NB_RETURNED_HANDLES
            );
            appl_param.nb_returned_handles = (UINT16)val;

            LOG_DEBUG("List Start Offset : ");
            scanf("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_LIST_START_OFFSET
            );
            appl_param.list_start_offset = (UINT16)val;

            LOG_DEBUG("Latest Captured Images Flag (1/0) : ");
            scanf("%d", &val);
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_LATEST_CAPTURED_IMAGES
            );
            appl_param.latest_captured_images = (UINT8)val;

            LOG_DEBUG("Enter Start Date for Image Listing: ");
            scanf("%s", start_date);
            LOG_DEBUG("Enter End Date for Image Listing: ");
            scanf("%s",end_date);

            /* Form image handles descriptor object with the given parameters */
            BT_str_print((CHAR *)image_handles_descriptor_data,
                    "<image-handles-descriptor version=\"1.0\" >\n"
                    "<filtering-parameters created = \"%s\"-\"%s\"/>\n"
                    "</image-handles-descriptor>",
                    start_date, end_date);

            /* Image Descriptor */
            image_descriptor_info.value = image_handles_descriptor_data;
            image_descriptor_info.length = (UINT16 )BT_str_n_len(image_handles_descriptor_data, sizeof(image_handles_descriptor_data));

            /* Update the hdrs  for get request */
            req_info.image_descriptor = &image_descriptor_info;

            LOG_DEBUG ("Requesting to Get Image List.....\n");
            retval = BT_bip_responder_get_image_listing
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 25:
            bip_responder_pri_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter the Image Handle: ");
            scanf ("%s", bip_res_img_handle);
            image_handle_info.value = bip_res_img_handle;
            image_handle_info.length = (UINT16 )(BT_str_n_len(bip_res_img_handle, sizeof(bip_res_img_handle))+1U);

            req_info.image_handle = &image_handle_info;

            LOG_DEBUG ("Requesting to Get Image Properties.....\n");
            retval = BT_bip_responder_get_image_properties
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 26:
            bip_responder_pri_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));
            BT_mem_set(image_thumbnail_name, 0x00, sizeof(image_thumbnail_name));
            BT_mem_set(bip_res_img_handle, 0, sizeof(bip_res_img_handle));

            LOG_DEBUG ("Enter The Image Handle For The Thumbnail: ");
            scanf ("%s", bip_res_img_handle);
            image_handle_info.value = bip_res_img_handle;
            image_handle_info.length = (UINT16 )(BT_str_n_len(bip_res_img_handle, sizeof(bip_res_img_handle)-1) + 1U);
            bip_res_img_handle[sizeof(bip_res_img_handle)-1] = '\0';

            req_info.image_handle = &image_handle_info;

            LOG_DEBUG ("Requesting to Get Image Thumbnail.....\n");
            retval = BT_bip_responder_get_linked_thumbnail
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );
            image_thumbnail_name_len = BT_str_n_len(image_thumbnail_name,sizeof(image_thumbnail_name)-1);
            BT_str_n_copy((CHAR *)image_thumbnail_name, (CHAR *)bip_res_img_handle, (sizeof(bip_res_img_handle)-1));
            BT_str_n_cat((CHAR *)image_thumbnail_name, "_thumbnail.jpg", ((sizeof(image_thumbnail_name)-1) - image_thumbnail_name_len));

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
                    LOG_DEBUG ("Failed to File Open\n");
                }
            }
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 27:
            bip_responder_sec_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Responder Instance: ");

#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter the image handle for which image has to be requested: ");
            scanf ("%s", bip_res_img_handle);
            image_handle_info.value = bip_res_img_handle;
            image_handle_info.length = (UINT16 )(BT_str_n_len(bip_res_img_handle, sizeof(bip_res_img_handle)) + 1U);

            LOG_DEBUG ("Enter Image Descriptor \n");

            LOG_DEBUG ("Enter Image Format\n");
            LOG_DEBUG ("    1 -> JPEG\n");
            LOG_DEBUG ("    2 -> GIF\n:");
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
            LOG_DEBUG ("Enter Image Width:");
            scanf ("%d", &val);
            img_width = (UINT16)val;

            /* Get Image Length */
            LOG_DEBUG ("Enter Image Height:");
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
            image_descriptor_info.length = (UINT16 )BT_str_n_len(image_descriptor_data, sizeof(image_descriptor_data));

            req_info.image_handle = &image_handle_info;
            req_info.image_descriptor = &image_descriptor_info;

            LOG_DEBUG (" Requesting to Get Image .....\n");
            retval = BT_bip_responder_get_image
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );

            BT_str_n_copy((CHAR *)image_file_name, (CHAR *)bip_res_img_handle, sizeof(bip_res_img_handle));
            if (0x01U == img_type)
            {
                BT_str_n_cat((CHAR *)image_file_name,".jpg", 4U);
            }
            else
            {
                BT_str_n_cat((CHAR *)image_file_name,".gif", 4U);
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
                    LOG_DEBUG ("Failed to File Open\n");
                }

            }
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 28:
            bip_responder_pri_print_appl_instances ();
            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            /* Reset */
            BT_mem_set (&req_info, 0x00, sizeof (BIP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter The Image Handle: ");
            scanf ("%s", bip_res_img_handle);
            image_handle_info.value = bip_res_img_handle;
            image_handle_info.length = (UINT16 )(BT_str_n_len(bip_res_img_handle, sizeof(bip_res_img_handle)) + 1U);

            LOG_DEBUG ("Enter The Attachment Name: ");
            scanf ("%s", file_name);
            name_info.value = file_name;
            name_info.length = (UINT16 )(BT_str_n_len (file_name, sizeof(file_name)) + 1U);

            req_info.image_handle = &image_handle_info;
            req_info.name = &name_info;

            LOG_DEBUG ("Requesting to Get Image Attachment.....\n");
            retval = BT_bip_responder_get_linked_attachment
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 34:
            LOG_DEBUG ("Enter BIP Responder Instance: ");
#if (BIP_NUM_RESPONDER_INSTANCE > 1)
            scanf ("%d", &handle);
#else /* (BIP_NUM_RESPONDER_INSTANCE > 1) */
            handle = 0;
            LOG_DEBUG ("%d\n", handle);
#endif /* (BIP_NUM_RESPONDER_INSTANCE > 1) */

            bip_responder_sec_print_appl_instances ();
            BIP_INIT_HEADER_STRUCT (image_handle_info);

            LOG_DEBUG ("Enter the Image Handle: ");
            scanf ("%s", bip_res_img_handle);

            image_handle_info.value = bip_res_img_handle;
            image_handle_info.length = (UINT16)BT_str_n_len(bip_res_img_handle, sizeof(bip_res_img_handle))+1U;

            /* Update the hdrs  for put request */
            req_info.image_handle = &image_handle_info;

            LOG_DEBUG ("Requesting to Delete Image ...\n");
            retval = BT_bip_responder_delete_image
                     (
                         &bip_responder_sec_instances[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 40:
             LOG_DEBUG("Want to Get the Linked ThumbNail after PushImage?\n");
             LOG_DEBUG("1 -> Yes\n");
             LOG_DEBUG("0 -> No\n");
             scanf("%d", &option);

             appl_get_linked_thumbnail_flag = (UCHAR )option;

             break;

        case 50:
             LOG_DEBUG("Enter number of SRMP wait stages: ");
             scanf("%d", &choice);
             srmp_wait_count = (UCHAR)choice;
             break;

        default:
            LOG_DEBUG ("Invalid Choice.\n");
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

API_RESULT appl_bip_responder_callback
           (
               BIP_HANDLE   * bip_handle,
               UINT8          event_type,
               UINT16         event_result,
               BIP_HEADERS  * event_header,
               UINT16         event_hdrlen
           )
{
    API_RESULT          retval;
    BIP_HANDLE          handle;
    UCHAR               send_response;
    UCHAR               send_request;
    BIP_RESPONSE_STRUCT bip_rsp_info;
    BIP_HEADER_STRUCT   body_info,image_handle_info;
    BIP_HEADERS         *bip_rx_hdrs;
    BIP_HEADERS         bip_tx_hdrs;
    BIP_IMAGE_DESC      bip_image_desc;
    UCHAR               tx_response;
    UINT16              i;
    UCHAR               more;
    UINT32              file_attribute;
    UINT16              length;
    UINT16              actual;
    UCHAR               *data;
    UCHAR               object_name[64U];
    UINT16              img_desc_length;
    UCHAR               *img_desc_data;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    /* Initialize to default values */
    handle = *bip_handle;
    retval = API_SUCCESS;

    BT_mem_set(&bip_tx_hdrs, 0, sizeof(bip_tx_hdrs));
    BIP_INIT_HEADER_STRUCT(body_info);
    BIP_INIT_HEADER_STRUCT(image_handle_info);
    BIP_INIT_HEADER_STRUCT(bip_image_desc);

    bip_rsp_info.name = NULL;
    bip_rsp_info.body = NULL;
    bip_rsp_info.image_descriptor = NULL;
    bip_rsp_info.img_len_info = 0U;
    bip_rsp_info.image_handle = NULL;

    send_response   = 0U;
    send_request = 0x00U;
    bip_rx_hdrs     = event_header;
    tx_response     = (UCHAR )event_result;

    data            = NULL;
    length          = 0x00U;
    actual          = 0x00U;
    more            = 0x00U;

    for (i = 0U; i < BIP_NUM_RESPONDER_INSTANCE; i++)
    {
        if (bip_responder_pri_instances[i].handle == handle)
        {
            break;
        }
    }

    if (BIP_NUM_RESPONDER_INSTANCE == i)
    {
        LOG_DEBUG ("Failed to find application BIP responder instance!\n");
        retval = API_SUCCESS; /* return API_SUCCESS; */
    }
    else
    {
        switch(event_type)
        {
        case BIP_RESPONDER_PRI_TRANSPORT_CLOSE_CNF: /* Fall Through */
        case BIP_RESPONDER_PRI_TRANSPORT_CLOSE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (BIP_RESPONDER_PRI_TRANSPORT_CLOSE_IND == event_type) ?
            "BIP_RESPONDER_PRI_TRANSPORT_CLOSE_IND" : "BIP_RESPONDER_PRI_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BT_mem_set (bip_responder_pri_instances[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            bip_responder_pri_print_appl_instances ();
            break;

        case BIP_RESPONDER_SEC_TRANSPORT_CLOSE_IND: /* Fall Through */
        case BIP_RESPONDER_SEC_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (BIP_RESPONDER_SEC_TRANSPORT_CLOSE_IND == event_type) ?
            "BIP_RESPONDER_SEC_TRANSPORT_CLOSE_IND" : "BIP_RESPONDER_SEC_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BT_mem_set (bip_responder_sec_instances[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            bip_responder_sec_print_appl_instances ();
            break;

        case BIP_RESPONDER_PRI_CONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_CONNECT_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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
                bip_responder_pri_instances[i].bd_addr,
                bip_rx_hdrs->bip_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            bip_responder_pri_instances[i].max_xchg_size =
                bip_rx_hdrs->bip_connect_info->max_recv_size;

            bip_responder_pri_print_appl_instances ();

            /* Set path to root folder */
            if (0U != bip_responder_pri_instances[i].root_depth)
            {
                while (0U != bip_responder_pri_instances[i].root_depth)
                {
                    retval = BT_vfops_set_path_backward(bip_responder_pri_instances[i].path);
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path backward\n");
                    }
                    bip_responder_pri_instances[i].root_depth --;
                }

                LOG_DEBUG ("Folder set to ROOT\n");
            }
            break;

         case BIP_RESPONDER_SEC_CONNECT_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("ReceivedS BIP_RESPONDER_SEC_CONNECT_CNF\n");
            LOG_DEBUG("BIP responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bip_rx_hdrs->bip_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            bip_rx_hdrs->bip_connect_info->max_recv_size);

            /* Get the Handle parameters */
            for (i = 0U; i < BIP_NUM_RESPONDER_INSTANCE; i++)
            {
                if (bip_responder_sec_instances[i].handle == handle)
                {
                    BT_mem_copy
                    (
                        bip_responder_sec_instances[i].bd_addr,
                        bip_rx_hdrs->bip_connect_info->bd_addr,
                        BT_BD_ADDR_SIZE
                    );

                    bip_responder_sec_instances[i].max_xchg_size =
                        bip_rx_hdrs->bip_connect_info->max_recv_size;
                    break;
                }
            }

            bip_responder_sec_print_appl_instances ();

            break;

        case BIP_RESPONDER_PRI_DISCONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_DISCONNECT_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bip_responder_pri_print_appl_instances ();
            break;

        case BIP_RESPONDER_SEC_DISCONNECT_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_DISCONNECT_CNF\n");
            LOG_DEBUG("BIP responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bip_responder_sec_print_appl_instances ();
            break;

         case BIP_RESPONDER_PRI_GET_CAPABILITIES_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_CAPABILITIES_IND \n");
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
                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;
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
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bip_tx_fp);
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

        case BIP_RESPONDER_PRI_GET_STATUS_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_STATUS_IND \n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* send response */
            send_response = 1U;

            if(1U == sec_obex_connection)
            {
                tx_response = BIP_CONTINUE_RSP;
            }
            else
            {
                tx_response = BIP_SUCCESS_RSP;
            }

            break;
       case BIP_RESPONDER_SEC_GET_CAPABILITIES_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_GET_CAPABILITES_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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
                LOG_DEBUG ("\n\n ====== BIP Get Capabilities Object data ===== \n\n");
                for (i = 0U; i<length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");

                /* Open file for dumping received GetFolderListing request data */
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

        case BIP_RESPONDER_PRI_PUT_IMAGE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_PUT_IMAGE_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            if ((BIP_SUCCESS_RSP != event_result) &&
                (BIP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bip_push_started)
            {
                /* Check for Name hdr in the request */
                if((NULL == bip_rx_hdrs->bip_req_info->name) ||
                    (NULL == bip_rx_hdrs->bip_req_info->name->value) ||
                    (0U == bip_rx_hdrs->bip_req_info->name->length))
                {
                    tx_response = BIP_BAD_REQ_RSP;
                    break;
                }
                else if ((0x00U != appl_reject_opn) &&
                         (BIP_PARTIAL_CONTENT_RSP != appl_reject_opn))
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
                        (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                        bip_rx_hdrs->bip_req_info->name->value,
                        file_object
                    );
                    LOG_DEBUG ("Received Image Name: %s\n", file_object);

                    bip_push_started = 0x01U;
                }

                bip_rsp_info.image_handle = &image_handle_info;

                BT_str_copy (appl_image_handle, BIP_RESPONDER_BASE_HANDLE);

                /* TODO: Handling when appl_responder_img_handle_cnt > 9 */
                if (appl_responder_img_handle_cnt < 9U)
                {
                    appl_image_handle[6U] = appl_responder_img_handle_cnt + '0';
                }

                /* Increment the count */
                appl_responder_img_handle_cnt++;

                image_handle_info.value = appl_image_handle;
                image_handle_info.length = (UINT16)BT_str_n_len(appl_image_handle, sizeof(appl_image_handle)) + 1U;

                if (BIP_PARTIAL_CONTENT_RSP == appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                    /* reset */
                    appl_reject_opn = 0x00U;
                    bip_push_started = 0x00U;

                    break;
                }
            }

            if ((NULL != bip_rx_hdrs->bip_req_info->image_descriptor->value) &&
                (0U != bip_rx_hdrs->bip_req_info->image_descriptor->length))
            {
                length = bip_rx_hdrs->bip_req_info->image_descriptor->length;
                data = bip_rx_hdrs->bip_req_info->image_descriptor->value;

                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BIP Push Image Descriptor  ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
            }

            if ((NULL == bip_rx_hdrs->bip_req_info->body->value) &&
                (0U == bip_rx_hdrs->bip_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bip_rx_hdrs->bip_req_info->body->length;
                data = bip_rx_hdrs->bip_req_info->body->value;

#if 0
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BIP Push Image data ===== \n\n");
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == bip_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bip_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open PUSH Image file\n");
                    }
                }

                /* Write to file */
                if (NULL != bip_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BIP_SUCCESS_RSP == event_result)
            {
                if (NULL != bip_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bip_rx_fp);
                    bip_rx_fp = NULL;
                }

                bip_push_started = 0x00U;

                if (0x00U != appl_get_linked_thumbnail_flag)
                {
                    LOG_DEBUG("Sending Partial Content Response\n");
                    tx_response = BIP_PARTIAL_CONTENT_RSP;

                    /* Reset */
                    appl_get_linked_thumbnail_flag = 0x00U;
                }
            }
            break;

        case BIP_RESPONDER_PRI_PUT_LINKED_THUMBNAIL_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_PUT_LINKED_THUMBNAIL_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            if ((BIP_SUCCESS_RSP != event_result) &&
                (BIP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bip_push_started)
            {
                /* Check for Name hdr in the request */
                if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
                   (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
                   (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
                {
                    tx_response = BIP_BAD_REQ_RSP;

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
                    BT_mem_copy
                    (
                        thumbnail_file_name,
                        bip_rx_hdrs->bip_req_info->image_handle->value,
                        bip_rx_hdrs->bip_req_info->image_handle->length
                    );

                    thumbnail_file_name[bip_rx_hdrs->bip_req_info->image_handle->length] = '\0';

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                        thumbnail_file_name,
                        file_object
                    );
                    BT_str_n_cat(file_object, "_thumbnail.jpg", 14U);

                    bip_push_started = 0x01U;
                }
            }

            if ((NULL == bip_rx_hdrs->bip_req_info->body->value) &&
                (0U == bip_rx_hdrs->bip_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bip_rx_hdrs->bip_req_info->body->length;
                data = bip_rx_hdrs->bip_req_info->body->value;

#if 0
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BIP Thumbnail Object data ===== \n\n");
                LOG_DEBUG ("Thumbnail data length: %d\n", length);
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif
                /* Open file PUT request */
                if (NULL == bip_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bip_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open PUSH Object file\n");
                    }

                    LOG_DEBUG ("Received Thumbmail Image Name: %s\n", file_object);
                }

                /* write to file */
                if (NULL != bip_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BIP_SUCCESS_RSP == event_result)
            {
                if (NULL != bip_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bip_rx_fp);
                    bip_rx_fp = NULL;
                }

                bip_push_started = 0x00U;
            }
            break;

        case BIP_RESPONDER_PRI_PUT_LINKED_ATTACHMENT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_PUT_LINKED_ATTACHMENT_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            if ((BIP_SUCCESS_RSP != event_result) &&
                (BIP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bip_push_started)
            {
                /* Check for image hdr in the request */
                if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
                   (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
                   (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
                {
                    tx_response = BIP_BAD_REQ_RSP;

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
                    BT_mem_copy
                    (
                        attachment_file_name,
                        bip_rx_hdrs->bip_req_info->image_handle->value,
                        bip_rx_hdrs->bip_req_info->image_handle->length
                    );

                    attachment_file_name[bip_rx_hdrs->bip_req_info->image_handle->length] = (UCHAR)'\0';

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                        attachment_file_name,
                        file_object
                    );
                    BT_str_n_cat(file_object, "_attachemnt.txt", 15U);

                    bip_push_started = 0x01U;
                }
            }

            if ((NULL == bip_rx_hdrs->bip_req_info->body->value) &&
                (0U == bip_rx_hdrs->bip_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bip_rx_hdrs->bip_req_info->body->length;
                data = bip_rx_hdrs->bip_req_info->body->value;

#if 0
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BIP attachment data application ===== \n\n");
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == bip_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bip_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open Attachemnt file\n");
                    }

                    LOG_DEBUG ("Received Received Attachement: %s\n", file_object);
                }

                /* write to file */
                if (NULL != bip_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BIP_SUCCESS_RSP == event_result)
            {
                if (NULL != bip_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bip_rx_fp);
                    bip_rx_fp = NULL;
                }

                bip_push_started = 0x00U;
            }

            break;

        case BIP_RESPONDER_PRI_GET_IMAGE_LIST_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_IMAGE_LIST_IND \n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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

            /* set application parameter */
            BIP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BIP_FLAG_NB_RETURNED_HANDLES
            );

#if 0
            appl_param.nb_returned_handles = bip_rx_hdrs->bip_req_info->appl_params->nb_returned_handles;
#else
            /* For now always 3, from the resource file */
            appl_param.nb_returned_handles = 0x03U;
#endif /* 0 */

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
                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;
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
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bip_tx_fp);
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

        case BIP_RESPONDER_PRI_GET_IMAGE_PROPERTIES_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_IMAGE_PROPERTIES_IND \n");
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
                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;
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
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bip_tx_fp);
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

        case BIP_RESPONDER_PRI_GET_LINKED_THUMBNAIL_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_LINKED_THUMBNAIL_IND\n");
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

                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;

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

        case BIP_RESPONDER_PRI_GET_IMAGE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_IMAGE_IND\n");
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
                    responder_image_handle = bip_rx_hdrs->bip_req_info->image_handle->value;
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
                    if(0 == BT_str_cmp(responder_image_handle,"1000001"))
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
                    else if(0 == BT_str_cmp(responder_image_handle,"1000002"))
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
                    else if(0 == BT_str_cmp(responder_image_handle,"1000003"))
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
                        LOG_DEBUG("\n No Image for the handle:%s\n",responder_image_handle);
                        tx_response = BIP_NOT_FOUND_RSP;
                        break;
                    }
                }

                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;

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
            {
                /* Fill the Image size */
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

        case BIP_RESPONDER_PRI_GET_LINKED_ATTACHMENT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_LINKED_ATTACHMENT_IND \n");
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
                    LOG_DEBUG("1- tx_response = BIP_BAD_REQ_RSP\n");
                    tx_response = BIP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    LOG_DEBUG ("Received Image Handle: %s",
                        bip_rx_hdrs->bip_req_info->image_handle->value);
                }

                /* Check for Name hdr in the request */
                if((NULL == bip_rx_hdrs->bip_req_info->name) ||
                   (NULL == bip_rx_hdrs->bip_req_info->name->value) ||
                   (0U == bip_rx_hdrs->bip_req_info->name->length))
                {
                    LOG_DEBUG("2- tx_response = BIP_BAD_REQ_RSP\n");
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

                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;

                LOG_DEBUG("tx_response = BIP_SUCCESS_RSP\n");
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
                    LOG_DEBUG("tx_response = BIP_NOT_FOUND_RSP\n");
                    tx_response = BIP_NOT_FOUND_RSP;
                    break;
                }
            }

            /* Fill body hdr values */
            if (remaining > bip_xchg_size)
            {
                body_info.length = (UINT16 )bip_xchg_size;
                LOG_DEBUG("tx_response = BIP_CONTINUE_RSP\n");
                tx_response = BIP_CONTINUE_RSP;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;

                LOG_DEBUG("tx_response = BIP_SUCCESS_RSP\n");
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

          case BIP_RESPONDER_PRI_GET_MONITORING_IMAGE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_GET_MONITORING_IMAGE_IND \n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Send response */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            bip_rsp_info.body = &body_info;
            bip_rsp_info.image_handle = &image_handle_info;

            if (0U == sent)
            {
                /* Check for appl hdr in the request */
                if(NULL == bip_rx_hdrs->bip_req_info->appl_params)
                {
                    tx_response = BIP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    LOG_DEBUG("Store Image Flag: %d\n",
                        bip_rx_hdrs->bip_req_info->appl_params->store_flag);

                    /* TODO: Handling when appl_responder_img_handle_cnt > 9 */
                    if (appl_responder_img_handle_cnt < 9U)
                    {
                        appl_image_handle[6U] = appl_responder_img_handle_cnt + '0';
                    }

                    /* Increment the count */
                    appl_responder_img_handle_cnt++;

                    image_handle_info.length = BIP_IMAGE_HANDLE_SIZE;

                    if(bip_rx_hdrs->bip_req_info->appl_params->store_flag == 1U)
                    {
                        image_handle_info.value = (UCHAR *)"1000001";
                        LOG_DEBUG("Image Handle: %s\n",image_handle_info.value);
                    }
                    else
                    {
                        image_handle_info.value = (UCHAR *)"0000000";
                    }
                }

                /* Check if operation is rejected   */
                if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reasons - 0x%02X\n", tx_response);

                    break;
                }
                else
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_ROOT_FOLDER_BASE,
                        (UCHAR *)SAMPLE_MONITORING_IMAGE,
                        file_object
                    );
                }

                bip_xchg_size = bip_responder_pri_instances[handle].max_xchg_size;

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
            else
            {
                BIP_INIT_HEADER_STRUCT (image_handle_info);
            }

            /* Fill body hdr values */
            if (remaining > bip_xchg_size)
            {
                if(NULL != image_handle_info.value)
                {
                    body_info.length = (UINT16 )(bip_xchg_size - 8U);
                    tx_response = BIP_CONTINUE_RSP;
                }
                else
                {
                    body_info.length = (UINT16 )bip_xchg_size;
                    tx_response = BIP_CONTINUE_RSP;
                }
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

        case BIP_RESPONDER_PRI_REMOTE_DISPLAY_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_REMOTE_DISPLAY_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            if ((BIP_SUCCESS_RSP != event_result) &&
                (BIP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check for image hdr in the request */
            if((NULL == bip_rx_hdrs->bip_req_info->image_handle) ||
               (NULL == bip_rx_hdrs->bip_req_info->image_handle->value) ||
               (0U == bip_rx_hdrs->bip_req_info->image_handle->length))
            {
                tx_response = BIP_BAD_REQ_RSP;
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
                LOG_DEBUG ("Image Handle: %s\n",
                    bip_rx_hdrs->bip_req_info->image_handle->value);
            }

            /* Check for appl hdr in the request */
            if(NULL == bip_rx_hdrs->bip_req_info->appl_params)
            {
                tx_response = BIP_BAD_REQ_RSP;
                break;
            }
            else
            {
                LOG_DEBUG("Remote Display:%d\n",
                    bip_rx_hdrs->bip_req_info->appl_params->remote_display);
            }

            if (BIP_SUCCESS_RSP == event_result)
            {
                /* Send image handle header */
                BIP_INIT_HEADER_STRUCT (image_handle_info);

                UCHAR handle_value[10U];
                UINT16 handle_value_len,handle_last_digit;
                UINT32 handle_number;
                handle_value_len = BT_str_len(bip_rx_hdrs->bip_req_info->image_handle->value);
                BT_str_n_copy(handle_value,bip_rx_hdrs->bip_req_info->image_handle->value,handle_value_len);
                handle_number = appl_str_to_num(handle_value,handle_value_len);
                handle_last_digit = appl_str_to_num((handle_value + (handle_value_len-1U)),1U);

                /* TODO: Handling when appl_responder_img_handle_cnt > 9 */
                /*Checking image handles range between 1000001 to max images pushed from initiator (handle count)*/
                if((handle_number > 1000000U) &&
                   (handle_number < 1000010U) &&
                   (appl_responder_img_handle_cnt > handle_last_digit))
                {
                    if(1U == bip_rx_hdrs->bip_req_info->appl_params->remote_display)
                    {
                        LOG_DEBUG("Next Image is Selected\n");

                        /*checking whether next image is there to display or not based on the handle count*/
                        if((appl_responder_img_handle_cnt - 1) > handle_last_digit)
                        {
                            handle_number++;
                            BT_str_print((CHAR *)handle_value, "%ld", handle_number);
                            image_handle_info.value = (UCHAR *)handle_value;
                        }
                        else
                        {
                            LOG_DEBUG("No next image to display\n");
                            image_handle_info.value = (UCHAR *)"000000";
                        }
                    }
                    else if(2U == bip_rx_hdrs->bip_req_info->appl_params->remote_display)
                    {
                        LOG_DEBUG("Previous Image is Selected\n");
                        /*checking whether Previous image is there to display or not based on the handle count*/
                        if(1U < handle_last_digit)
                        {
                            handle_number--;
                            BT_str_print((CHAR *)handle_value, "%ld", handle_number);
                            image_handle_info.value = (UCHAR *)handle_value;
                        }
                        else
                        {
                            LOG_DEBUG("No previous image to display\n");
                            image_handle_info.value = (UCHAR *)"000000";
                        }
                    }
                    else if(3U == bip_rx_hdrs->bip_req_info->appl_params->remote_display)
                    {
                        LOG_DEBUG("Select Image\n");
                        image_handle_info.value = bip_rx_hdrs->bip_req_info->image_handle->value;
                    }
                    else if(4U == bip_rx_hdrs->bip_req_info->appl_params->remote_display)
                    {
                        LOG_DEBUG("Current Image is Selected\n");
                        image_handle_info.value = bip_rx_hdrs->bip_req_info->image_handle->value;
                    }
                    else
                    {
                        LOG_DEBUG("Wrong Remote Display Flag Received\n");

                        /* Should not reach here */
                        tx_response = BIP_BAD_REQ_RSP;
                        break;
                    }
                }
                else
                {
                   LOG_DEBUG("Different/Wrong Handle Selected\n");
                   image_handle_info.value = (UCHAR *)"000000";
                }

                image_handle_info.length = BIP_IMAGE_HANDLE_SIZE;

                bip_tx_hdrs.bip_rsp_info->image_handle=&image_handle_info;
            }
            break;

        case BIP_RESPONDER_PRI_DELETE_IMAGE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_PRI_DELETE_IMAGE_IND\n");
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
                LOG_DEBUG ("Delete IMAGE HANDLE: %s\n", bip_rx_hdrs->bip_req_info->image_handle->value);

                /* Reject if operation reject is set or if Read ONly is set */
                if (0U != appl_bip_responder_read_only)
                {
                    tx_response = appl_bip_responder_read_only;
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

                    LOG_DEBUG ("object cannot be  deleted : %s",object_name);
                    tx_response = OBEX_UNAUTHORIZED_RSP;
                }

                else
                {
                   (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_object);
                }
            }

            break;

        case BIP_RESPONDER_PRI_START_PRINT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("ReceivedS BIP_RESPONDER_PRI_START_PRINT_IND\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            if ((BIP_SUCCESS_RSP != event_result) &&
                (BIP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bip_push_started)
            {
                /* Check for appl hdr in the request */
                if(NULL == bip_rx_hdrs->bip_req_info->appl_params)
                {
                    tx_response = BIP_BAD_REQ_RSP;
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
                    LOG_DEBUG("\nservice id\n");
                    for(i = 0U; i <bip_rx_hdrs->bip_req_info->appl_params->service_id.length; i++)
                    {
                        LOG_DEBUG("0x%02x",bip_rx_hdrs->bip_req_info->appl_params->service_id.value[i]);
                    }
                    LOG_DEBUG("\n");

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BIP_RX_ROOT_FOLDER_BASE,
                        attachment_file_name,
                        file_object
                    );
                    BT_str_n_cat(file_object, "rcvd_print_job.txt", 18U);

                    bip_push_started = 0x01U;
               }
            }

            if ((NULL == bip_rx_hdrs->bip_req_info->body->value) &&
                (0U == bip_rx_hdrs->bip_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bip_rx_hdrs->bip_req_info->body->length;
                data = bip_rx_hdrs->bip_req_info->body->value;

#if 1
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BIP attachment data application ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == bip_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bip_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bip_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open print job file\n");
                    }
                }

                /* write to file */
                if (NULL != bip_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bip_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BIP_SUCCESS_RSP == event_result)
            {
                if (NULL != bip_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bip_rx_fp);
                    bip_rx_fp = NULL;
                }

                bip_push_started = 0x00U;
            }

            break;

           case BIP_RESPONDER_PRI_START_ARCHIVE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("ReceiveSd BIP_RESPONDER_PRI_START_ARCHIVE_IND\n");
            LOG_DEBUG("BIP RESPONDER Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;

            if ((BIP_SUCCESS_RSP != event_result) &&
                (BIP_CONTINUE_RSP != event_result))
            {
                break;
            }

            if (BIP_SUCCESS_RSP == event_result)
            {
                    UINT32 d;
                    LOG_DEBUG("\nservice id\n");
                    for(d = 0U; d <bip_rx_hdrs->bip_req_info->appl_params->service_id.length; d++)
                    {
                        LOG_DEBUG("0x%02x,",bip_rx_hdrs->bip_req_info->appl_params->service_id.value[d]);
                    }
                    LOG_DEBUG("\n");
            }

            break;

        case BIP_RESPONDER_SEC_GET_IMAGE_LIST_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_GET_IMAGE_LIST_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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

        case BIP_RESPONDER_SEC_GET_IMAGE_PROPERTIES_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_PROPERTIES_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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

        case BIP_RESPONDER_SEC_GET_LINKED_THUMBNAIL_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_GET_LINKED_THUMBNAIL_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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

        case BIP_RESPONDER_SEC_GET_LINKED_ATTACHMENT_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_GET_LINKED_ATTACHMENT_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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

         case BIP_RESPONDER_SEC_GET_IMAGE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_GET_IMAGE_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
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

         case BIP_RESPONDER_SEC_DELETE_IMAGE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_INITIATOR_SEC_DELETE_IMAGE_CNF\n");
            LOG_DEBUG("BIP Initiator Instance: %d\n", handle);
            LOG_DEBUG(" Result = 0x%04X\n", event_result);
            break;

         case BIP_RESPONDER_SEC_GET_PARTIAL_IMAGE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BIP_RESPONDER_SEC_GET_PARTIAL_IMAGE_CNF\n");
            LOG_DEBUG("BIP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (BIP_CONTINUE_RSP == event_result)
            {
                if((0U < bip_rx_hdrs->bip_rsp_info->img_len_info) &&
                   (0x01U != send_request))
                {
                    LOG_DEBUG("Image Size  ---> %d\n",bip_rx_hdrs->bip_rsp_info->img_len_info);
                }

                /* LOG_DEBUG("\nin countinue response\n"); */
                send_request = 0x01U;
            }

            if(NULL != bip_rx_hdrs->bip_rsp_info->appl_params)
            {
                if (0U != (BIP_GET_APPL_PARAM_FLAG
                   (bip_rx_hdrs->bip_rsp_info->appl_params->appl_param_flag,
                    BIP_TAG_ID_TOTAL_FILE_SIZE)))
                {
                    LOG_DEBUG("TOTAL SIZE: %d\n", bip_rx_hdrs->bip_rsp_info->appl_params->total_file_size);
                }

                if (0U != (BIP_GET_APPL_PARAM_FLAG
                    (bip_rx_hdrs->bip_rsp_info->appl_params->appl_param_flag,
                    BIP_TAG_ID_END_FLAG)))
                {
                    LOG_DEBUG("END FLAG: %d\n", bip_rx_hdrs->bip_rsp_info->appl_params->end_flag);
                }
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

        default:
            /* Should not reach here */
            tx_response = BIP_BAD_REQ_RSP;
            break;
        }

        /* Send response if required */
        if (0U != send_response)
        {
            if (0U < srmp_wait_count)
            {
                bip_rsp_info.wait = BT_TRUE;
                bip_tx_hdrs.bip_rsp_info = &bip_rsp_info;
                srmp_wait_count--;
            }

            retval = BT_bip_responder_send_response
                     (
                         &handle,
                         event_type,
                         tx_response,
                         &bip_tx_hdrs
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send BIP responder Response - 0x%04x\n", retval);
            }

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;

                /* MISRA C - 2012 Rule 2.2 */
                /* body_info.length = 0U; */
            }
        }

        if (0U != send_request)
        {
            retval = BT_bip_responder_send_request
                     (
                         &bip_responder_sec_instances[handle].handle,
                         event_type,
                         event_result,
                         &bip_tx_hdrs,
                         more,
                         &actual
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("BIP responder Send Request Failed - 0x%04X\n", retval);
            }
        }
    }

    return retval;
}

void appl_bip_responder_sdp_callback
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

    LOG_DEBUG("feature flag == %d\n", feature_flag);
    if (feature_flag == 2U)
    {
        uuid[0U].uuid_union.uuid_16 = IMAGING_REFERENCED_OBJECTS_UUID;
    }
    else
    {
        uuid[0U].uuid_union.uuid_16 = IMAGING_AUTOMATIC_ARCHIVE_UUID;
    }
    uuid[1U].uuid_union.uuid_16 = L2CAP_UUID;
    uuid[2U].uuid_union.uuid_16 = RFCOMM_UUID;

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

            /* LOG_DEBUG ("==== AttrRsp, rsp_len:0x%04x, status: 0x%04x \n", length, status); */

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
            retval = BT_bip_responder_connect
                     (
                         &bip_responder_pri_instances[bipc_handle].handle,
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

void bip_responder_pri_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n================PRIMARY BIP CONNECTION====================\n");
    LOG_DEBUG ("Instance  BIP Handles\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < BIP_NUM_RESPONDER_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", bip_responder_pri_instances[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        bip_responder_pri_instances[i].bd_addr[0U], bip_responder_pri_instances[i].bd_addr[1U],
        bip_responder_pri_instances[i].bd_addr[2U], bip_responder_pri_instances[i].bd_addr[3U],
        bip_responder_pri_instances[i].bd_addr[4U], bip_responder_pri_instances[i].bd_addr[5U]);

        LOG_DEBUG ("\n");

    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

void bip_responder_sec_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n================SECONDARY BIP CONNECTION====================\n");
    LOG_DEBUG ("Instance  BIP Handles\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < BIP_NUM_RESPONDER_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", bip_responder_sec_instances[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        bip_responder_sec_instances[i].bd_addr[0U], bip_responder_sec_instances[i].bd_addr[1U],
        bip_responder_sec_instances[i].bd_addr[2U], bip_responder_sec_instances[i].bd_addr[3U],
        bip_responder_sec_instances[i].bd_addr[4U], bip_responder_sec_instances[i].bd_addr[5U]);

        LOG_DEBUG ("\n");

    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}
#endif /* BIP */
