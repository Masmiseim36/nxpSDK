
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

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* XML header for folder & message listing object */
static UCHAR xml_hdr[] = "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n";

static UCHAR msg_readline[512];

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

    *num_entity = 0;

    if (NULL == dir_entry)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Invalid Parameter\n");

        return MAP_INVALID_PARAMETERS;
    }

    BT_fops_file_open (listingfile, (UCHAR *)"wb", &xml_fd);
    if (NULL == xml_fd)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Failed to open file\n");

        return API_FAILURE;
    }

    BT_str_copy(dir, dir_entry);

    BT_debug_trace(BT_MODULE_ID_MAP,
    "[MAP_PL] Path = %s\n", dir_entry);

    first = BT_FALSE;

#if 0
    if(API_SUCCESS != BT_fops_set_path_forward(dir))
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Failure : could not change directory\n");
    }
    else
    {
        BT_debug_info(BT_MODULE_ID_MAP,
        "[MAP_PL] Success : could change directory\n");

        BT_str_cat(dir, "*.*");
    }
#endif /* 0 */

    BT_fops_file_print(xml_fd ,"%s<%s %s>\n", xml_hdr, parent, version);
    while (1)
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

        if(info.fattrib & BT_FOPS_MASK_FOLDER)
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

    BT_fops_access_close(&h);
    BT_fops_file_close(xml_fd);

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

    *num_entity = 0;

    if (NULL == dir_entry)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Invalid Parameter\n");

        return MAP_INVALID_PARAMETERS;
    }

    BT_fops_file_open (listingfile, (UCHAR *)"wb", &xml_fd);
    if (NULL == xml_fd)
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Failed to open file\n");

        return API_FAILURE;
    }

    BT_str_copy(dir, dir_entry);

    BT_debug_trace(BT_MODULE_ID_MAP,
    "[MAP_PL] Path = %s\n", dir_entry);

    first = BT_FALSE;

#if 0
    if(API_SUCCESS != BT_fops_set_path_forward(dir))
    {
        BT_debug_error(BT_MODULE_ID_MAP,
        "[MAP_PL] Failure : could not change directory\n");
    }
    else
    {
        BT_debug_info(BT_MODULE_ID_MAP,
        "[MAP_PL] Success : could change directory\n");

        BT_str_cat(dir, "*.*");
    }
#endif /* 0 */

    BT_fops_file_print(xml_fd ,"%s<%s %s>\n", xml_hdr, parent, version);
    while (1)
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

        if(info.fattrib & BT_FOPS_MASK_FOLDER)
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

            /* Get the Date Time */
            sprintf (msg_attr.datetime, "%d%02d%02dT%02d%02d%02d",
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

    BT_fops_access_close (&h);
    BT_fops_file_close (xml_fd);

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
    UCHAR file_object[128];
    UCHAR fn[24];
    UCHAR readstr[256];
    UINT16 count, i;

    /* Form the file handle */
    sprintf ((CHAR *)fn, "sample.msg");
    BT_vfops_create_object_name
    (
        idir,
        fn,
        file_object
    );

    BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
    if (NULL == fd)
    {
        return API_FAILURE;
    }

    fp = NULL;

    for (i = 1; i < item_id; i ++)
    {
        sprintf ((CHAR *)fn, "l123456789%07X.vmg", i);
        BT_vfops_create_object_name
        (
            idir,
            fn,
            file_object
        );

        BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
        if (NULL == fp)
        {
            sprintf ((CHAR *)fn, "h123456789%07X.vmg", i);
            BT_vfops_create_object_name
            (
                idir,
                fn,
                file_object
            );

            BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if (NULL == fp)
            {
                /* Free item id found */
                break;
            }
        }

        /* Will hit here only on Non-NULL value */
        BT_fops_file_close (fp);
    }

    sprintf ((CHAR *)fn, "l123456789%07X.vmg", i);
    BT_vfops_create_object_name
    (
        idir,
        fn,
        file_object
    );

    BT_fops_file_open (file_object, (UCHAR *)"wb", &fp);
    if (NULL == fp)
    {
        BT_fops_file_close (fd);
        return API_FAILURE;
    }

    count = sizeof(readstr);
    do
    {
        BT_mem_set(readstr, 0, count);
        retval = BT_fops_file_get(fd, readstr, &count);

        if (API_SUCCESS != retval)
        {
            break;
        }

        BT_fops_file_print(fp, "%s", readstr);

    } while (1);

    BT_fops_file_close (fp);
    BT_fops_file_close(fd);

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
    UCHAR file_object[128];
    UCHAR fn[24];
    BT_fops_file_handle fp;
    int i;

    fp = NULL;

    for (i = 1; i < item_id; i ++)
    {
        sprintf ((CHAR *)fn, "l123456789%07X.vmg", i);
        BT_vfops_create_object_name
        (
            idir,
            fn,
            file_object
        );

        BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
        if (NULL == fp)
        {
            sprintf ((CHAR *)fn, "h123456789%07X.vmg", i);
            BT_vfops_create_object_name
            (
                idir,
                fn,
                file_object
            );

            BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if (NULL == fp)
            {
                /* Free item id found */
                break;
            }
        }

        /* Will hit here only on Non-NULL value */
        BT_fops_file_close (fp);
    }

    sprintf ((CHAR *)message_handle, "l123456789%07X.vmg", i);

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
    UCHAR file_object[128];
    UCHAR vcard_count, sender, recipient, message;
    UINT16 count;
    INT16 i;

    BT_vfops_create_object_name
    (
        idir,
        (UCHAR *)filename,
        file_object
    );

    BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
    if (NULL == fd)
    {
        return API_FAILURE;
    }

    vcard_count = 0;
    sender = 0x00;
    recipient = 0x00;
    message = 0x00;

    readstr = msg_readline;
    count = sizeof(msg_readline);

    /* Get the Message Handle. This will be from the filename */
    BT_mem_copy (attr->handle, &filename[1], 16);
    attr->handle[16] = '\0';

    /* Get Priority */
    sprintf (attr->priority, "%s", ('h' == filename[0])? "yes": "no");

    /* Get Reception Status */
    sprintf (attr->rstatus, "%s", "complete");

    /* Get Attachment Size */
    sprintf (attr->asize, "%s", "0");

    do
    {
        BT_mem_set(readstr, 0, count);
        retval = BT_fops_file_get(fd, readstr, &count);

        if (API_SUCCESS != retval)
        {
            break;
        }

        i = (UINT16)(BT_str_len(readstr) - 1);

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

        if (0 == BT_str_len(readstr))
        {
            continue;
        }

        /* Get Type */
        if ((0 == BT_mem_cmp("TYPE:", readstr, 5)) ||
            (0 == BT_mem_cmp("type:", readstr, 5)))
        {
            sprintf (attr->type, "%s", &readstr[5]);
            continue;
        }

        /* Get Size */
        if ((0 == BT_mem_cmp("LENGTH:", readstr, 7)) ||
            (0 == BT_mem_cmp("length:", readstr, 7)))
        {
            sprintf (attr->size, "%s", &readstr[7]);
            continue;
        }

        /* Get Read Status */
        if ((0 == BT_mem_cmp("STATUS:", readstr, 7)) ||
            (0 == BT_mem_cmp("status:", readstr, 7)))
        {
            sprintf (attr->read, "%s",
            ((!BT_str_cmp(&readstr[7], "UNREAD")) || (!BT_str_cmp(&readstr[7], "unread")))? "no": "yes");

            continue;
        }

        if ((0 == BT_mem_cmp("BEGIN:VCARD", readstr, 11)) ||
            (0 == BT_mem_cmp("begin:vcard", readstr, 11)))
        {
            vcard_count ++;

            if (1 == vcard_count)
            {
                sender = 0x01;
                continue;
            }
            else if (1 < vcard_count)
            {
                recipient = 0x01;
                continue;
            }
        }

        if ((0 == BT_mem_cmp("N:", readstr, 2)) ||
            (0 == BT_mem_cmp("n:", readstr, 2)))
        {
            if (0x01 == sender)
            {
                /* Get Sender Name */
                sprintf (attr->sname, "%s", &readstr[2]);
            }
            else if (0x01 == recipient)
            {
                /* Get Sender Name */
                sprintf (attr->rname, "%s", &readstr[2]);
            }
        }

        if ((0 == BT_mem_cmp("FN:", readstr, 3)) ||
            (0 == BT_mem_cmp("fn:", readstr, 3)))
        {
            if (0x01 == sender)
            {
                /* Get Sender Name */
                sprintf (attr->sname, "%s", &readstr[3]);
            }
            else if (0x01 == recipient)
            {
                /* Get Sender Name */
                sprintf (attr->rname, "%s", &readstr[3]);
            }
        }

        if ((0 == BT_mem_cmp("TEL:", readstr, 4)) ||
            (0 == BT_mem_cmp("tel:", readstr, 4)))
        {
            if (0x01 == sender)
            {
                /* Get Sender Addressing */
                sprintf (attr->saddr, "%s", &readstr[4]);
            }
            else if (0x01 == recipient)
            {
                /* Get Sender Addressing */
                sprintf (attr->raddr, "%s", &readstr[4]);
            }

            sender = 0x00;
            recipient = 0x00;
        }

        if ((0 == BT_mem_cmp("EMAIL:", readstr, 6)) ||
            (0 == BT_mem_cmp("email:", readstr, 6)))
        {
            if (0x01 == sender)
            {
                /* Get Sender Addressing */
                sprintf (attr->saddr, "%s", &readstr[6]);
            }
            else if (0x01 == recipient)
            {
                /* Get Sender Addressing */
                sprintf (attr->raddr, "%s", &readstr[6]);
            }

            sender = 0x00;
            recipient = 0x00;
        }

        /* Get Subject */
        if ((0 == BT_mem_cmp("BEGIN:MSG", readstr, 9)) ||
            (0 == BT_mem_cmp("begin:msg", readstr, 9)))
        {
            message = 0x01;
            continue;
        }

        if (0x01 == message)
        {
            message = 0x00;
            BT_str_n_copy (attr->subject, readstr, sizeof (attr->subject));
            continue;
        }
    } while (1);

    BT_fops_file_close (fd);

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
    UCHAR readstr[256], readstr_t[256];
    UCHAR file_object[128], new_object[128];
    UCHAR fn[24], curr_folder[128];
    UINT16 count;
    INT16 i;

    BT_str_copy (fn, "l");
    BT_str_cat (fn, handle);
    BT_str_cat (fn, ".vmg");
    BT_vfops_create_object_name
    (
        idir,
        fn,
        file_object
    );

    BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
    if (NULL == fd)
    {
        fn[0] = 'h';
        BT_vfops_create_object_name
        (
            idir,
            fn,
            file_object
        );

        BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
        if (NULL == fd)
        {
            return API_FAILURE;
        }
    }

    /* Is status indicator Deleted? */
    if (0x01 == ((UCHAR)(status >> 8)))
    {
        BT_fops_file_close (fd);

        if (0x01 == ((UCHAR)status))
        {
            /* Backup current path */
            BT_str_copy (curr_folder, idir);

            BT_vfops_set_path_backward(idir);
            BT_vfops_set_path_forward(idir, (UCHAR *)"deleted");

            BT_vfops_create_object_name
            (
                idir,
                fn,
                new_object
            );

            /* Move the file to deleted folder */
            BT_fops_file_move (file_object, new_object);

            /* Restore current path */
            BT_str_copy (idir, curr_folder);
        }
        else if (0x00 == ((UCHAR)status))
        {
            /* If current folder is deleted, move to Inbox. Else return Error */
            if (NULL == BT_str_str(idir, "deleted"))
            {
                BT_fops_file_close(fd);
                return API_FAILURE;
            }

            /* Backup current path */
            BT_str_copy (curr_folder, idir);

            BT_vfops_set_path_backward(idir);
            BT_vfops_set_path_forward(idir, (UCHAR *)"inbox");

            BT_vfops_create_object_name
            (
                idir,
                fn,
                new_object
            );

            /* Move the file to deleted folder */
            BT_fops_file_move (file_object, new_object);

            /* Restore current path */
            BT_str_copy (idir, curr_folder);
        }
    }

    /* Is status indicator Read? */
    if (0x00 == ((UCHAR)(status >> 8)))
    {
        BT_vfops_create_object_name
        (
            idir,
            (UCHAR *)"tmp.vmg",
            new_object
        );

        BT_fops_file_open (new_object, (UCHAR *)"wb", &fp);

        count = sizeof(readstr);
        if (NULL != fp)
        {
            do
            {
                BT_mem_set(readstr, 0, count);
                BT_mem_set(readstr_t, 0, count);
                retval = BT_fops_file_get(fd, readstr, &count);

                if (API_SUCCESS != retval)
                {
                    break;
                }

                BT_str_copy (readstr_t, readstr);

                if (0 == BT_str_len(readstr))
                {
                    BT_fops_file_print(fp, "%s", readstr_t);
                    continue;
                }

                i = (UINT16)(BT_str_len(readstr) - 1);

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
                if ((0 == BT_mem_cmp("STATUS:", readstr, 7)) ||
                    (0 == BT_mem_cmp("status:", readstr, 7)))
                {
                    /* Update the read status */
                    BT_fops_file_print(fp, "STATUS:%s\n",
                             (0x00 == ((UCHAR)status))? "UNREAD": "READ");
                    continue;
                }

                BT_fops_file_print(fp, "%s", readstr_t);

            } while (1);

            BT_fops_file_close (fp);
        }

        BT_fops_file_close(fd);
        fd = NULL;

        /* Delete the existing file */
        BT_fops_object_delete(file_object);

        /* Rename the updated temp file */
        BT_fops_file_move (new_object, file_object);
    }

    if (NULL != fd)
    {
        BT_fops_file_close(fd);
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
    UCHAR fn[24];
    UCHAR file_object[128];
    UCHAR dir[128], rdir[64];

    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;

    API_RESULT retval;
    UCHAR first;

    BT_IGNORE_UNUSED_PARAM(idir);

    BT_str_copy(rdir, MAP_ROOT_FOLDER_BASE);
    BT_str_cat(rdir, BT_FOPS_PATH_SEP"root_0"BT_FOPS_PATH_SEP"telecom"BT_FOPS_PATH_SEP"msg");
    BT_str_copy(dir, rdir);

    first = BT_FALSE;

    while (1)
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

        if (info.fattrib & BT_FOPS_MASK_FOLDER)
        {
            UCHAR cdir[128];

            BT_debug_info(BT_MODULE_ID_MAP,
                "[MAP_PL] It is a directory\n");

            BT_str_copy(cdir, rdir);
            BT_str_cat(cdir, BT_FOPS_PATH_SEP);
            BT_str_cat(cdir, info.fname);

            BT_str_copy(fn, "l");
            BT_str_cat(fn, msg_handle);
            BT_str_cat(fn, ".vmg");

            printf ("%s, %s\n", cdir, fn);

            BT_vfops_create_object_name
            (
                cdir,
                fn,
                file_object
            );

            BT_fops_file_open(file_object, (UCHAR *)"rb", &fd);
            if (NULL == fd)
            {
                fn[0] = 'h';
                BT_vfops_create_object_name
                    (
                    cdir,
                    (UCHAR *)fn,
                    (UCHAR *)file_object
                    );

                BT_fops_file_open(file_object, (UCHAR *)"rb", &fd);
                if (NULL == fd)
                {
                    continue;
                }
            }

            /* Got a valid file handle */
            BT_fops_file_close(fd);
            BT_str_copy(message_file, file_object);
            retval = API_SUCCESS;
            break;
        }
    }

    return retval;
}

