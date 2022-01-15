
/**
 *  \file map_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "map_pl.h"

#if ((defined MAP_MCE) || (defined MAP_MSE))

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* XML header for folder & message listing object */
static UCHAR xml_hdr[] = "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n";

/*
static UCHAR conv_sample_participant_id[] =
"<participant uci = \"sample@email.de\" display_name = \"Sample\" chat_state = \"2\" last_activity = \"20140801T012900+01:00\" x_bt_uid = \"K1A2A3A4B1B2C1C2D1D2E1E2E3E4F1F2\" />\n";
*/

static UCHAR msg_readline[512U];

/* --------------------------------------------- Functions */

/**
 *  \fn BT_map_create_xml_folder_listing_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param dir_entry
 *  \param listingfile
 *  \param num_entity
 *
 *  \return None
 */
API_RESULT BT_map_create_xml_folder_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* OUT */ UINT16  *num_entity
           )
{
    BT_fops_file_handle xml_fd;
    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;
    API_RESULT retval;
    UCHAR parent[]="folder-listing";
    UCHAR version[] = "version = \"1.0\"";
    UCHAR dir[MAP_FOLDER_NAME_LEN];
    UCHAR first;

    *num_entity = 0U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    xml_fd = NULL;
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));
    BT_mem_set(&info, 0, sizeof(BT_FOPS_FILINFO));

    if (NULL == dir_entry)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Invalid Parameter\n");

        return MAP_INVALID_PARAMETERS;
    }

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (listingfile, (UCHAR *)"wb", &xml_fd);
    if (NULL == xml_fd)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Failed to open file\n");

        return API_FAILURE;
    }

    BT_str_n_copy(dir, dir_entry, (sizeof(dir) - 1));

    BT_debug_trace(BT_MODULE_ID_MAP,
    "[MAP_PL] Path = %s\n", dir_entry);

    first = BT_FALSE;

    BT_fops_file_print(xml_fd ,"%s<%s %s>\n", xml_hdr, parent, version);

    BT_LOOP_FOREVER()
    {
        if (BT_FALSE == first)
        {
            first = BT_TRUE;
            retval = BT_fops_access_first(dir, NULL, &h, &info);
        }
        else
        {
            retval = BT_fops_access_next(&h, &info);
        }

        if (API_SUCCESS != retval)
        {
            BT_debug_error(BT_MODULE_ID_MAP,
            "[MAP_PL] Invalid File Handle. Get Last Error reports\n");
            break;
        }

        if((0 == BT_str_cmp(".", info.fname)) ||
           (0 == BT_str_cmp("..", info.fname)))
        {
            continue;
        }

        if(0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            BT_debug_info(BT_MODULE_ID_MAP,
            "[MAP_PL] It is a directory\n");

            BT_fops_file_print(xml_fd,"\t<folder name = \"%s\"/>\n", info.fname);

            (*num_entity)++;
        }
        else
        {
            BT_debug_info(BT_MODULE_ID_MAP,
            "[MAP_PL] It is a file\n");

#if 0
            BT_fops_file_print
            (
                xml_fd,
                "\t<file name=\"%s\" size=\"%d\"/>\n",
                info.fname,
                info.flsize
            );
#endif /* 0 */
        }
    }

    BT_fops_file_print(xml_fd ,"</%s>\n",parent);

    (BT_IGNORE_RETURN_VALUE) BT_fops_access_close(&h);

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close(xml_fd);

    return API_SUCCESS;
}


/**
 *  \fn BT_map_create_xml_messages_listing_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param dir_entry
 *  \param listingfile
 *  \param num_entity
 *
 *  \return None
 */
API_RESULT BT_map_create_xml_messages_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* IN */  MAP_APPL_PARAMS * appl_params,
               /* OUT */ UINT16  *num_entity
           )
{
    BT_fops_file_handle xml_fd;
    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;
    UCHAR parent[] = "MAP-msg-listing";

#ifdef MAP_1_3
    UCHAR version[] = "version = \"1.1\"";
#else
    UCHAR version[] = "version = \"1.0\"";
#endif /* MAP_1_3 */

    UCHAR dir[MAP_FOLDER_NAME_LEN];
    MAP_MSG_ATTR_PL msg_attr;
    API_RESULT retval;
    UCHAR first;

    *num_entity = 0U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    xml_fd = NULL;
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));
    BT_mem_set(&info, 0, sizeof(BT_FOPS_FILINFO));
    BT_mem_set(&msg_attr, 0, sizeof(MAP_MSG_ATTR_PL));

    if (NULL == dir_entry)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Invalid Parameter\n");

        return MAP_INVALID_PARAMETERS;
    }

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (listingfile, (UCHAR *)"wb", &xml_fd);
    if (NULL == xml_fd)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Failed to open file\n");

        return API_FAILURE;
    }

    BT_str_n_copy(dir, dir_entry, (sizeof(dir) - 1));

    BT_debug_trace(BT_MODULE_ID_MAP,
    "[MAP_PL] Path = %s\n", dir_entry);

    first = BT_FALSE;

    BT_fops_file_print(xml_fd ,"%s<%s %s>\n", xml_hdr, parent, version);

    BT_LOOP_FOREVER()
    {
        if (BT_FALSE == first)
        {
            first = BT_TRUE;
            retval = BT_fops_access_first(dir, NULL, &h, &info);
        }
        else
        {
            retval = BT_fops_access_next(&h, &info);
        }

        if (API_SUCCESS != retval)
        {
            BT_debug_error(BT_MODULE_ID_MAP,
            "[MAP_PL] Invalid File Handle. Get Last Error reports\n");
            break;
        }

        if((0 == BT_str_cmp(".", info.fname)) ||
           (0 == BT_str_cmp("..", info.fname)))
        {
            continue;
        }

        if(0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            BT_debug_info(BT_MODULE_ID_MAP,
            "[MAP_PL] It is a directory\n");
        }
        else
        {
            BT_debug_info(BT_MODULE_ID_MAP,
            "[MAP_PL] It is a file\n");

            /* Check if a valid message file */
            if (NULL == BT_str_str(info.fname, ".vmg"))
            {
                /* It is not */
                continue;
            }

            /* Get the file listing data */
            retval = map_get_message_attributes_pl (dir_entry, info.fname, &msg_attr);

            /* Apply filtering if requested for */
            if (NULL != appl_params)
            {
                if (0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                    (
                        appl_params->appl_param_flag,
                        MAP_FLAG_FILTERMESSAGETYPE,
                        0U
                    )))
                {
                    if (0U == (msg_attr.m_type & appl_params->filter_message_type))
                    {
                        continue;
                    }
                }

                if (0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                    (
                        appl_params->appl_param_flag,
                        MAP_FLAG_FILTERREADSTATUS,
                        0U
                    )))
                {
                    if ((0x00U != appl_params->filter_read_status) &&
                        (msg_attr.rd_status != appl_params->filter_read_status))
                    {
                        continue;
                    }
                }

                if (0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                    (
                        appl_params->appl_param_flag,
                        MAP_FLAG_FILTER_MSG_HANDLE,
                        1U
                    )))
                {
                    INT32 ret;

                    ret = BT_mem_cmp
                          (
                              (UCHAR *)appl_params->filter_msg_handle.value,
                              (UCHAR *)msg_attr.handle,
                              (appl_params->filter_msg_handle.length)
                          );
                    if (0 != ret)
                    {
                        continue;
                    }
                }
            }

            /* Get the Date Time */
            BT_str_print(msg_attr.datetime, "%d%02d%02dT%02d%02d%02d",
            (int)info.fdyear, (int)info.fdmonth, (int)info.fdday, (int)info.fthour, (int)info.ftmin, (int)info.ftsec);

            if (API_SUCCESS == retval)
            {
                BT_fops_file_print(xml_fd, "<msg handle = \"%s\" ", msg_attr.handle);
                BT_fops_file_print(xml_fd, "subject = \"%s\" ", msg_attr.subject);
                BT_fops_file_print(xml_fd, "datetime = \"%s\" ", msg_attr.datetime);
                BT_fops_file_print(xml_fd, "sender_name = \"%s\" ", msg_attr.sname);
                BT_fops_file_print(xml_fd, "sender_addressing = \"%s\" ", msg_attr.saddr);
                BT_fops_file_print(xml_fd, "recipient_name = \"%s\" ",  msg_attr.rname);
                BT_fops_file_print(xml_fd, "recipient_addressing = \"%s\" ", msg_attr.raddr);
                BT_fops_file_print(xml_fd, "type = \"%s\" ", msg_attr.type);
                BT_fops_file_print(xml_fd, "size = \"%s\" ", msg_attr.size);
                BT_fops_file_print(xml_fd, "reception_status = \"%s\" ", msg_attr.rstatus);
#ifdef MAP_1_3
                /* For Now adding conversation_id for all massages */
                BT_fops_file_print(xml_fd, "conversation_id = \"3909231965\" ");
#endif /* MAP_1_3 */

                BT_fops_file_print(xml_fd, "attachment_size = \"%s\" ", msg_attr.asize);
                BT_fops_file_print(xml_fd, "priority = \"%s\" ", msg_attr.priority);
                BT_fops_file_print(xml_fd, "read = \"%s\"", msg_attr.read);
                BT_fops_file_print(xml_fd, "/>\n");

                (*num_entity)++;
            }
        }
    }

    BT_fops_file_print(xml_fd ,"</%s>\n",parent);

    (BT_IGNORE_RETURN_VALUE) BT_fops_access_close (&h);

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close (xml_fd);

    return API_SUCCESS;
}


/**
 *  \fn BT_map_update_inbox_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param idir
 *  \param item_id
 *
 *  \return None
 */
API_RESULT BT_map_update_inbox_pl
           (
               /* IN */  UCHAR  *idir,
               /* IN */  UINT16  item_id
           )
{
    API_RESULT retval;
    BT_fops_file_handle fp, fd;
    UCHAR file_object[128U];
    UCHAR fn[24U];
    UCHAR readstr[256U];
    UINT16 count, i;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    fd = NULL;
    BT_mem_set(file_object, 0, sizeof(file_object));

    /* Form the file handle */
    BT_str_print((CHAR *)fn, "sample.msg");

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        idir,
        fn,
        file_object
    );

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
    if (NULL == fd)
    {
        return API_FAILURE;
    }

    fp = NULL;

    for (i = 1U; i < item_id; i ++)
    {
        BT_str_print((CHAR *)fn, "l123456789%07X.vmg", i);

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            idir,
            fn,
            file_object
        );

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
        if (NULL == fp)
        {
            BT_str_print((CHAR *)fn, "h123456789%07X.vmg", i);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                idir,
                fn,
                file_object
            );

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if (NULL == fp)
            {
                /* Free item id found */
                break;
            }
        }

        /* Will hit here only on Non-NULL value */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close (fp);
    }

    BT_str_print((CHAR *)fn, "l123456789%07X.vmg", i);

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        idir,
        fn,
        file_object
    );

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"wb", &fp);
    if (NULL == fp)
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close (fd);
        return API_FAILURE;
    }

    count = sizeof(readstr);

    BT_LOOP_FOREVER()
    {
        BT_mem_set(readstr, 0, count);
        retval = BT_fops_file_get(fd, readstr, &count);

        if (API_SUCCESS != retval)
        {
            break;
        }

        BT_fops_file_print(fp, "%s\n", readstr);

    }

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close (fp);
    (void)BT_fops_file_close(fd);

    return API_SUCCESS;
}


/**
 *  \fn BT_map_push_message_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param idir
 *  \param item_id
 *  \param message_handle
 *
 *  \return None
 */
API_RESULT BT_map_push_message_pl
           (
               /* IN */  UCHAR  *idir,
               /* IN */  UINT16  item_id,
               /* OUT */ UCHAR  *message_handle
           )
{
    UCHAR file_object[128U];
    UCHAR fn[24U];
    BT_fops_file_handle fp;
    int i;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(file_object, 0, sizeof(file_object));
    fp = NULL;

    for (i = 1U; i < item_id; i ++)
    {
        BT_str_print((CHAR *)fn, "l123456789%07X.vmg", i);

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            idir,
            fn,
            file_object
        );

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
        if (NULL == fp)
        {
            BT_str_print((CHAR *)fn, "h123456789%07X.vmg", i);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                idir,
                fn,
                file_object
            );

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if (NULL == fp)
            {
                /* Free item id found */
                break;
            }
        }

        /* Will hit here only on Non-NULL value */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close (fp);
    }

    BT_str_print((CHAR *)message_handle, "l123456789%07X.vmg", i);

    return API_SUCCESS;
}

/**
 *  \fn map_get_message_attributes_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param idir
 *  \param filename
 *  \param attr
 *
 *  \return None
 */
API_RESULT map_get_message_attributes_pl
           (
               /* IN */  UCHAR * idir,
               /* IN */  CHAR * filename,
               /* OUT */ MAP_MSG_ATTR_PL * attr
           )
{
    API_RESULT retval;
    BT_fops_file_handle fd;
    UCHAR *readstr;
    UCHAR file_object[128U];
    UCHAR vcard_count, sender, recipient, message;
    UINT16 count;
    INT16 i;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    fd = NULL;
    BT_mem_set(file_object, 0, sizeof(file_object));

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        idir,
        (UCHAR *)filename,
        file_object
    );

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
    if (NULL == fd)
    {
        return API_FAILURE;
    }

    vcard_count = 0U;
    sender = 0x00U;
    recipient = 0x00U;
    message = 0x00U;

    readstr = msg_readline;
    count = sizeof(msg_readline) - 1;

    /* Get the Message Handle. This will be from the filename */
    BT_mem_copy (attr->handle, &filename[1U], 16U);
    attr->handle[16U] = '\0';

    /* Get Priority */
    BT_str_print(attr->priority, "%s", ('h' == filename[0U]) ? "yes" : "no");

    /* Get Reception Status */
    BT_str_print(attr->rstatus, "%s", "complete");

    /**
     * Get Attachment Size
     *
     * Note:
     * For the following Message Forward PTS test cases, the attachement size should be NON-zero
     *  - MAP/MSE/MFMH/BV-02-I
     *  - MAP/MSE/MFMH/BV-03-I
     *  - MAP/MSE/MFMH/BV-04-I
     *  - MAP/MSE/MFMH/BV-05-I
     */
    BT_str_print(attr->asize, "%s", "100");

    BT_LOOP_FOREVER()
    {
        BT_mem_set(readstr, 0, sizeof(msg_readline));
        retval = BT_fops_file_get(fd, readstr, &count);

        if (API_SUCCESS != retval)
        {
            break;
        }

        i = 0;

        do
        {
            if (('\r' == readstr[i]) || ('\n' == readstr[i]))
            {
                readstr[i] = '\0';
                break;
            }

            i++;
        } while (i < count);

        if (0U == BT_str_len(readstr))
        {
            continue;
        }

        /* Get Type */
        if ((NULL != BT_str_str(readstr, "TYPE:")) ||
            (NULL != BT_str_str(readstr, "type:")))
        {
            BT_str_print(attr->type, "%s", &readstr[5U]);

            if (0 == BT_mem_cmp("SMS_GSM", attr->type, 7U))
            {
                attr->m_type = 0x01U;
            }
            else if (0 == BT_mem_cmp("SMS_CDMA", attr->type, 8U))
            {
                attr->m_type = 0x02U;
            }
            else if (0 == BT_mem_cmp("EMAIL", attr->type, 5U))
            {
                attr->m_type = 0x04U;
            }
            else if (0 == BT_mem_cmp("MMS", attr->type, 3U))
            {
                attr->m_type = 0x08U;
            }
            else if (0 == BT_mem_cmp("IM", attr->type, 2U))
            {
                attr->m_type = 0x10U;
            }
            else
            {
                /* Should not reach here */
                attr->m_type = 0x00U;
            }

            continue;
        }

        /* Get Size */
        if ((NULL != BT_str_str(readstr, "LENGTH:")) ||
            (NULL != BT_str_str(readstr, "length:")))
        {
            BT_str_print(attr->size, "%s", &readstr[7U]);
            continue;
        }

        /* Get Read Status */
        if ((NULL != BT_str_str(readstr, "STATUS:")) ||
            (NULL != BT_str_str(readstr, "status:")))
        {
            if ((0 == BT_str_cmp(&readstr[7U], "UNREAD")) ||
                (0 == BT_str_cmp(&readstr[7U], "unread")))
            {
                /* UnRead */
                BT_str_print(attr->read, "%s", "no");
                attr->rd_status = 0x01U;
            }
            else
            {
                /* Read */
                BT_str_print(attr->read, "%s", "yes");
                attr->rd_status = 0x02U;
            }

            continue;
        }

        if ((NULL != BT_str_str(readstr, "BEGIN:VCARD")) ||
            (NULL != BT_str_str(readstr, "begin:vcard")))
        {
            vcard_count ++;

            if (1U == vcard_count)
            {
                sender = 0x01U;
                continue;
            }
            else if (1U < vcard_count)
            {
                recipient = 0x01U;
                continue;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }

        if ((NULL != BT_str_str(readstr, "N:")) ||
            (NULL != BT_str_str(readstr, "n:")))
        {
            if (0x01U == sender)
            {
                /* Get Sender Name */
                BT_str_print(attr->sname, "%s", &readstr[2U]);
            }
            else if (0x01U == recipient)
            {
                /* Get Sender Name */
                BT_str_print(attr->rname, "%s", &readstr[2U]);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }

        if ((NULL != BT_str_str(readstr, "FN:")) ||
            (NULL != BT_str_str(readstr, "fn:")))
        {
            if (0x01U == sender)
            {
                /* Get Sender Name */
                BT_str_print(attr->sname, "%s", &readstr[3U]);
            }
            else if (0x01U == recipient)
            {
                /* Get Sender Name */
                BT_str_print(attr->rname, "%s", &readstr[3U]);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }

        if ((NULL != BT_str_str(readstr, "TEL:")) ||
            (NULL != BT_str_str(readstr, "tel:")))
        {
            if (0x01U == sender)
            {
                /* Get Sender Addressing */
                BT_str_print(attr->saddr, "%s", &readstr[4U]);
            }
            else if (0x01U == recipient)
            {
                /* Get Sender Addressing */
                BT_str_print(attr->raddr, "%s", &readstr[4U]);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            sender = 0x00U;
            recipient = 0x00U;
        }

        if ((NULL != BT_str_str(readstr, "EMAIL:")) ||
            (NULL != BT_str_str(readstr, "email:")))
        {
            if (0x01U == sender)
            {
                /* Get Sender Addressing */
                BT_str_print(attr->saddr, "%s", &readstr[6U]);
            }
            else if (0x01U == recipient)
            {
                /* Get Sender Addressing */
                BT_str_print(attr->raddr, "%s", &readstr[6U]);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            sender = 0x00U;
            recipient = 0x00U;
        }

        /* Get Subject */
        if ((NULL != BT_str_str(readstr, "BEGIN:MSG")) ||
            (NULL != BT_str_str(readstr, "begin:msg")))
        {
            message = 0x01U;
            continue;
        }

        if (0x01U == message)
        {
            message = 0x00U;
            BT_str_n_copy (attr->subject, readstr, sizeof (attr->subject));
            continue;
        }
    }

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close (fd);

    return API_SUCCESS;
}


/**
 *  \fn BT_map_mse_message_status_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param idir
 *  \param handle
 *  \param status
 *
 *  \return None
 */
API_RESULT BT_map_set_message_status_pl
           (
               /* IN */ UCHAR * idir,
               /* IN */ CHAR * handle,
               /* IN */ UINT16 status
           )
{
    API_RESULT retval;
    BT_fops_file_handle fd, fp;
    UCHAR readstr[256U], readstr_t[256U];
    UCHAR file_object[128U], new_object[128U];
    UCHAR fn[24U], curr_folder[128U];
    UINT16 count;
    INT16 i;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    fd = NULL;
    fp = NULL;
    BT_mem_set(file_object, 0, sizeof(file_object));
    BT_mem_set(new_object, 0, sizeof(new_object));

    BT_str_n_copy (fn, "l", sizeof("1"));
    BT_str_n_cat (fn, handle, (sizeof(fn) - BT_str_len(fn) - 1));
    BT_str_n_cat (fn, ".vmg", (sizeof(fn) - BT_str_len(fn) - 1));

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        idir,
        fn,
        file_object
    );

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
    if (NULL == fd)
    {
        fn[0U] = 'h';

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            idir,
            fn,
            file_object
        );

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
        if (NULL == fd)
        {
            return API_FAILURE;
        }
    }

    /* Is status indicator Deleted? */
    if (0x01U == ((UCHAR)(status >> 8U)))
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close (fd);

        if (0x01U == ((UCHAR)status))
        {
            /* Backup current path */
            BT_str_n_copy (curr_folder, idir, (sizeof(curr_folder) - 1));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_set_path_backward(idir);
            (void)BT_vfops_set_path_forward(idir, (UCHAR *)"deleted");

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                idir,
                fn,
                new_object
            );

            /* Move the file to deleted folder */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_move (file_object, new_object);

            /* Restore current path */
            BT_str_copy (idir, curr_folder);
        }
        else if (0x00U == ((UCHAR)status))
        {
            /* If current folder is deleted, move to Inbox. Else return Error */
            if (NULL == BT_str_str(idir, "deleted"))
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fd);
                return API_FAILURE;
            }

            /* Backup current path */
            BT_str_n_copy (curr_folder, idir, (sizeof(curr_folder) - 1));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_set_path_backward(idir);
            (void)BT_vfops_set_path_forward(idir, (UCHAR *)"inbox");

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                idir,
                fn,
                new_object
            );

            /* Move the file to deleted folder */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_move (file_object, new_object);

            /* Restore current path */
            BT_str_copy (idir, curr_folder);
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    /* Is status indicator Read? */
    if (0x00U == ((UCHAR)(status >> 8U)))
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            idir,
            (UCHAR *)"tmp.vmg",
            new_object
        );

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (new_object, (UCHAR *)"wb", &fp);

        count = sizeof(readstr);
        if (NULL != fp)
        {
            BT_LOOP_FOREVER()
            {
                BT_mem_set(readstr, 0, count);
                BT_mem_set(readstr_t, 0, count);
                retval = BT_fops_file_get(fd, readstr, &count);

                if (API_SUCCESS != retval)
                {
                    break;
                }

                BT_str_copy (readstr_t, readstr);

                if (0U == BT_str_len(readstr))
                {
                    BT_fops_file_print(fp, "%s", readstr_t);
                    continue;
                }

                i = (UINT16)(BT_str_len(readstr) - 1U);

                do
                {
                    if (('\r' == readstr[i]) || ('\n' == readstr[i]) ||
                        (' ' == readstr[i]) || ('\t' == readstr[i]))
                    {
                        readstr[i] = '\0';
                    }
                    else
                    {
                        break;
                    }

                    i --;

                } while (i >= 0);

                /* Get Type */
                if ((0 == BT_mem_cmp("STATUS:", readstr, 7U)) ||
                    (0 == BT_mem_cmp("status:", readstr, 7U)))
                {
                    /* Update the read status */
                    BT_fops_file_print(fp, "STATUS:%s\n",
                             (0x00U == ((UCHAR)status))? "UNREAD": "READ");
                    continue;
                }

                BT_fops_file_print(fp, "%s", readstr_t);

            }

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (fp);
        }

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close(fd);
        fd = NULL;

        /* Delete the existing file */
        (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_object);

        /* Rename the updated temp file */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_move (new_object, file_object);
    }

    if (NULL != fd)
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close(fd);
    }

    return API_SUCCESS;
}


/**
 *  \fn BT_map_get_message_file_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param msg_handle
 *  \param idir
 *  \param message_file
 *
 *  \return None
 */
API_RESULT BT_map_get_message_file_pl
           (
               /* IN */   UCHAR    *msg_handle,
               /* IN */   UCHAR    *idir,
               /* OUT */  UCHAR    *message_file
           )
{
    BT_fops_file_handle fd;
    UCHAR fn[24U];
    UCHAR file_object[128U];
    UCHAR dir[128U], rdir[64U];

    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;

    API_RESULT retval;
    UCHAR first;

    BT_IGNORE_UNUSED_PARAM(idir);

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));

    BT_str_n_copy(rdir, MAP_ROOT_FOLDER_BASE, (sizeof(rdir) - 1));
    BT_str_n_cat
    (
        rdir,
        BT_FOPS_PATH_SEP"root_0"BT_FOPS_PATH_SEP"telecom"BT_FOPS_PATH_SEP"msg",
        (sizeof(rdir) - BT_str_len(rdir) - 1)
    );

    BT_str_n_copy(dir, rdir, (sizeof(dir) - 1));

    first = BT_FALSE;

    BT_LOOP_FOREVER()
    {
        if (BT_FALSE == first)
        {
            first = BT_TRUE;
            retval = BT_fops_access_first(dir, NULL, &h, &info);
        }
        else
        {
            retval = BT_fops_access_next(&h, &info);
        }

        if (API_SUCCESS != retval)
        {
            BT_debug_error(BT_MODULE_ID_MAP,
            "[MAP_PL] Invalid File Handle. Get Last Error reports\n");
            break;
        }

        if ((0 == BT_str_cmp(".", info.fname)) ||
            (0 == BT_str_cmp("..", info.fname)))
        {
            continue;
        }

        if (0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            UCHAR cdir[128U];

            BT_debug_info(BT_MODULE_ID_MAP,
                "[MAP_PL] It is a directory\n");

            BT_mem_set (file_object, 0x00, sizeof(file_object));

            BT_str_n_copy(cdir, rdir, sizeof(rdir));
            BT_str_n_cat(cdir, BT_FOPS_PATH_SEP, (sizeof(cdir) - BT_str_len(cdir) - 1));
            BT_str_n_cat(cdir, info.fname, (sizeof(cdir) - BT_str_len(cdir) - 1));

            BT_str_n_copy(fn, "l", sizeof("l"));
            BT_str_n_cat(fn, msg_handle, (sizeof(fn) - BT_str_len(fn) - 1));
            BT_str_n_cat(fn, ".vmg", (sizeof(fn) - BT_str_len(fn) - 1));

            printf ("%s, %s\n", cdir, fn);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                cdir,
                fn,
                file_object
            );

            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fd);
            if (API_SUCCESS != retval)
            {
                fn[0U] = 'h';

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    cdir,
                    (UCHAR *)fn,
                    (UCHAR *)file_object
                );

                retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fd);
                if (API_SUCCESS != retval)
                {
                    continue;
                }
            }

            /* Got a valid file handle */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fd);
            BT_str_copy(message_file, file_object);
            retval = API_SUCCESS;
            break;
        }
    }

    return retval;
}
#endif /* ((defined MAP_MCE) || (defined MAP_MSE)) */
