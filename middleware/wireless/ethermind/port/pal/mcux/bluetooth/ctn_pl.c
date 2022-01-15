
/**
 *  \file ctn_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "ctn_pl.h"

#if ((defined CTN_CCE) || (defined CTN_CSE))

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static CHAR                    ctn_msg_readline[MAX_CTN_ATTR_SIZE];

/* --------------------------------------------- Functions */
/**
 *  \fn BT_ctn_build_object_listing_pl
 *
 *  \brief To build CTN object listing object with filters
 *
 *  \Description
 *
 *
 *  \param [in] dir_entry
 *  \param [in] listingfile
 *  \param [in] appl_param
 *  \param [out] listing_obj_info
 *
 *  \return None
 */
API_RESULT BT_ctn_build_object_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* IN */  CTN_APPL_PARAMS      *appl_param,
               /* OUT */ CTN_OBJ_LISTING_INFO *listing_obj_info
           )
{
    BT_fops_file_handle xml_fd;
    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;
    UCHAR           parent[] = "CTN-listing";
    UCHAR           version[] = "version = \"1.0\"";
    UCHAR           dir[MAX_CTN_FOLDER_NAME_LEN];
    CTN_OBJ_ATTR_PL ctn_obj_attr;
    API_RESULT      retval;
    BT_LOCAL_TIME   local_time;
    UINT16          object_count;
    UINT32          ctn_obj_param_mask;
    INT16           time_cmp;
    UINT16          start_offset;
    UCHAR first;

    UINT16          max_list_size;

    /* param check */
    if ((NULL == dir_entry) ||
        (NULL == listingfile) ||
        (NULL == appl_param) ||
        (NULL == listing_obj_info))
    {
        CTN_PL_ERR(
        "[CTN_PL] Invalid Parameter\n");

        return CTN_INVALID_PARAMETERS;
    }

    /* init */
    object_count                    = 0x00U;
    ctn_obj_param_mask              = 0xFFFFFFU;
    listing_obj_info->listing_count = 0x00U;
    max_list_size = 0xFFFFU;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    xml_fd                          = NULL;
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));
    BT_mem_set(&info, 0, sizeof(BT_FOPS_FILINFO));

    /* open the listing file */
    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open(listingfile, (UCHAR *)"wb", &xml_fd);
    if (NULL == xml_fd)
    {
        CTN_PL_ERR(
        "[CTN_PL] Invalid Parameter\n");

        return CTN_INVALID_PARAMETERS;
    }

    BT_str_n_copy(dir, dir_entry, (sizeof(dir) - 1));

    CTN_PL_TRC(
    "[CTN_PL] Path = %s\n", dir_entry);

    first = BT_FALSE;

    /* check for application param mask*/
    if (0U != (CTN_GET_APPL_PARAM_FLAG
        (
        appl_param->appl_param_flag,
        CTN_FLAG_PARMETERMASK
        )))
    {
        ctn_obj_param_mask = appl_param->parameter_mask;
    }

    /* Listing object Header start */
    BT_fops_file_print(xml_fd ,"<%s %s>\n", parent, version);

    start_offset = 0x00U;
    if (0U != (CTN_GET_APPL_PARAM_FLAG
        (
        appl_param->appl_param_flag,
        CTN_FLAG_LISTSTARTOFFSET
        )))
    {
        start_offset = appl_param->list_start_offset;
    }

    if (0U != (CTN_GET_APPL_PARAM_FLAG
              (
                  appl_param->appl_param_flag,
                   CTN_FLAG_MAXLISTCOUNT
              )))
    {
        max_list_size = appl_param->max_list_count;
    }

    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        if ((0U != max_list_size) &&
            (object_count >= max_list_size))
        {
            break;
        }

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
            break;
        }

        if((0 == BT_str_cmp(".", info.fname)) ||
           (0 == BT_str_cmp("..", info.fname)))
        {
            continue;
        }
        if(0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            CTN_PL_INF(
            "[CTN_PL] It is a directory\n");
        }
        else
        {
            CTN_PL_INF(
            "[CTN_PL] It is a file\n");

            /* Check if a valid ctn object file */
            if (NULL == BT_str_str(info.fname, ".ics"))
            {
                /* skip reading the attributes */
                continue;
            }

            if (0U < start_offset)
            {
                start_offset -= 1U;
                continue;
            }

            BT_mem_set (&ctn_obj_attr, 0x00, sizeof(CTN_OBJ_ATTR_PL));

            /* Get the object attributes data */
            retval = ctn_get_object_attributes_pl (dir_entry, info.fname, &ctn_obj_attr);

            /* Check for time periods */
            if ((CTN_GET_APPL_PARAM_FLAG
                (
                appl_param->appl_param_flag,
                CTN_FLAG_FILTERPERIODBEGIN
                )) &&
              (CTN_OBJ_PROP_MASK_UPDATE == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_UPDATE)))
            {

                time_cmp = ctn_compare_timestamp_pl
                           (
                               appl_param->filter_period_end.value,
                               (UCHAR )appl_param->filter_period_end.length,
                               appl_param->filter_period_begin.value,
                               (UCHAR )appl_param->filter_period_begin.length
                           );

                if (time_cmp == -1)
                {
                    /**
                     * period_end < period_begin, return error
                     */
                    retval = API_FAILURE;
                    break;
                }

                time_cmp = ctn_compare_timestamp_pl
                           (
                               appl_param->filter_period_begin.value,
                               (UCHAR )appl_param->filter_period_begin.length,
                               (UCHAR *)ctn_obj_attr.update,
                               (UCHAR )BT_str_len(ctn_obj_attr.update)
                            );
                if (time_cmp == 1U)
                {
                    /**
                     * period_begin > update_time, ignore
                     */
                     continue;
                }

                time_cmp = ctn_compare_timestamp_pl
                           (
                               (UCHAR *)ctn_obj_attr.update,
                               (UCHAR)BT_str_len(ctn_obj_attr.update),
                               appl_param->filter_period_end.value,
                               (UCHAR)appl_param->filter_period_end.length
                            );
                if (time_cmp == 1U)
                {
                    /**
                     *  update time > period_end, ignore
                     */
                     continue;
                 }
            }

            if (API_SUCCESS == retval)
            {
                BT_fops_file_print(xml_fd, "\t<ctn-entry handle = \"%s\"", ctn_obj_attr.handle);

                if (CTN_OBJ_PROP_MASK_UPDATE == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_UPDATE))
                {
                    BT_fops_file_print(xml_fd, " update = \"%s\"", ctn_obj_attr.update);
                }

                if (CTN_OBJ_PROP_MASK_CALTYPE == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_CALTYPE))
                {
                    BT_fops_file_print(xml_fd, " cal_type = \"%s\"", ctn_obj_attr.cal_type);
                }

                /* === For now NOT considering/supporting attachement === */

                if ((CTN_OBJ_PROP_MASK_SUMMARY == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_SUMMARY)) &&
                    (CTN_OBJ_PROP_MASK_SUMMARY == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_SUMMARY)))
                {
                    BT_fops_file_print(xml_fd, " summary = \"%s\"", ctn_obj_attr.summary);
                }

                if (CTN_OBJ_PROP_MASK_STARTTIME == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_STARTTIME))
                {
                    BT_fops_file_print(xml_fd, " starttime = \"%s\"", ctn_obj_attr.start_time);
                }

                if ((CTN_OBJ_PROP_MASK_ENDTIME == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_ENDTIME)) &&
                   (CTN_OBJ_PROP_MASK_ENDTIME == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_ENDTIME)))
                {
                    BT_fops_file_print(xml_fd, " endtime = \"%s\"", ctn_obj_attr.end_time);
                }

                if ((CTN_OBJ_PROP_MASK_ORIGINATOR_NAME == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_ORIGINATOR_NAME)) &&
                    (CTN_OBJ_PROP_MASK_ORIGINATOR_NAME == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_ORIGINATOR_NAME)))
                {
                    BT_fops_file_print(xml_fd, " originator_name = \"%s\"", ctn_obj_attr.org_name);
                }

                if ((CTN_OBJ_PROP_MASK_ORIGINATOR_ADDRS == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_ORIGINATOR_ADDRS)) &&
                    (CTN_OBJ_PROP_MASK_ORIGINATOR_ADDRS == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_ORIGINATOR_ADDRS)))
                {
                    BT_fops_file_print(xml_fd, " originator_address = \"%s\"", ctn_obj_attr.org_addr);
                }

                if (CTN_OBJ_PROP_MASK_SIZE == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_SIZE))
                {
                    BT_fops_file_print(xml_fd, " size = \"%d\"", ctn_obj_attr.size);
                }

                if ((CTN_OBJ_PROP_MASK_PRIORITY == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_PRIORITY)) &&
                    (CTN_OBJ_PROP_MASK_PRIORITY == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_PRIORITY)))
                {
                    BT_fops_file_print(xml_fd, " priority = \"%s\"", ctn_obj_attr.priority);
                }

                if ((CTN_OBJ_PROP_MASK_PSTATUS == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_PSTATUS)) &&
                    (CTN_OBJ_PROP_MASK_PSTATUS == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_PSTATUS)))
                {
                    BT_fops_file_print(xml_fd, " pstatus = \"%s\"", ctn_obj_attr.pstaus);
                }

                if ((CTN_OBJ_PROP_MASK_ALARMSTATUS == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_ALARMSTATUS)) &&
                    (CTN_OBJ_PROP_MASK_ALARMSTATUS == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_ALARMSTATUS)))
                {
                    BT_fops_file_print(xml_fd, " alaramstatus = \"%s\"", ctn_obj_attr.alarm_status);
                }

                if ((CTN_OBJ_PROP_MASK_SENDSTATUS == (ctn_obj_param_mask& CTN_OBJ_PROP_MASK_SENDSTATUS)) &&
                    (CTN_OBJ_PROP_MASK_SENDSTATUS == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_SENDSTATUS)))
                {
                    BT_fops_file_print(xml_fd, " sentstatus = \"%s\"", ctn_obj_attr.sentstatus);
                }

                if ((CTN_OBJ_PROP_MASK_RECURRENT == (ctn_obj_param_mask & CTN_OBJ_PROP_MASK_RECURRENT)) &&
                    (CTN_OBJ_PROP_MASK_RECURRENT == (ctn_obj_attr.obj_param_mask & CTN_OBJ_PROP_MASK_RECURRENT)))
                {
                    BT_fops_file_print(xml_fd, " recurrent = \"%s\"", ctn_obj_attr.recurrent);
                }

                BT_fops_file_print(xml_fd, "/>\n");

                object_count += 1U;
            }
        }
    }

    /* if (API_SUCCESS == retval) */
    {
        BT_get_local_time(&local_time);

        /* CSE offet */
        BT_str_print((CHAR *)listing_obj_info->cse_time_offset, "%d%02d%02dT%02d%02d%02d",
                local_time.dyear, local_time.dmonth, local_time.dday,
                local_time.thour, local_time.tmin, local_time.tsec);

        /* Update number of objects in listing file */
        listing_obj_info->listing_count = object_count;

        /* Listing object Header end */
        BT_fops_file_print(xml_fd ,"</%s>\n",parent);
    }

    (BT_IGNORE_RETURN_VALUE) BT_fops_access_close(&h);

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close (xml_fd);

    return retval;
}

/**
 *  \fn BT_ctn_get_object_list
 *
 *  \brief To get ctn objects details in given directory
 *
 *  \Description
 *
 *  \param [in] dir_entry
 *  \param [in] obj_list_info
 *
 *  \return None
 */
API_RESULT BT_ctn_get_object_list
           (
               /* IN */  UCHAR   *dir_entry,
               /* OUT */ CTN_OBJ_LIST_INFO *obj_list_info
           )
{
    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;
    UCHAR           dir[MAX_CTN_FOLDER_NAME_LEN];
    CTN_OBJ_ATTR_PL ctn_obj_attr;
    API_RESULT      retval;
    UINT16          object_count;

    UCHAR first;

    /* param check */
    if ((NULL == dir_entry) || (NULL == obj_list_info))
    {
        CTN_PL_ERR(
        "[CTN_PL] Invalid Parameter\n");

        return CTN_INVALID_PARAMETERS;
    }

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));
    BT_mem_set(&info, 0, sizeof(BT_FOPS_FILINFO));

    /* init */
    object_count = 0x00U;

    BT_str_n_copy(dir, dir_entry, (sizeof(dir) - 1));

    CTN_PL_TRC(
    "[CTN_PL] Path = %s\n", dir_entry);

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
            break;
        }

        if((0 == BT_str_cmp(".", info.fname)) ||
           (0 == BT_str_cmp("..", info.fname)))
        {
            continue;
        }
        if(0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            CTN_PL_INF(
            "[CTN_PL] It is a directory\n");
        }
        else
        {
            CTN_PL_INF(
            "[CTN_PL] It is a file\n");

            /* Check if a valid ctn object file */
            if (NULL == BT_str_str(info.fname, ".ics"))
            {
                /* skip reading the attributes */
                continue;
            }

            /* copy object name */
            BT_str_n_copy (obj_list_info->obj_details[object_count].name, info.fname, (sizeof(obj_list_info->obj_details[object_count].name) - 1));
            obj_list_info->obj_details[object_count].name_len = (UCHAR)BT_str_len (info.fname);

            BT_mem_set (&ctn_obj_attr, 0x00, sizeof(CTN_OBJ_ATTR_PL));

            /* Get the object attributes data */
            retval = ctn_get_object_attributes_pl (dir_entry, info.fname, &ctn_obj_attr);

            if (API_SUCCESS == retval)
            {
                /* copy object handle */
                BT_str_copy (obj_list_info->obj_details[object_count].handle, ctn_obj_attr.handle);
                obj_list_info->obj_details[object_count].handle_len = (UCHAR)BT_str_len(ctn_obj_attr.handle);

                object_count += 1U;
            }
        }
    }

    /* Update number of objects in the folder */
    obj_list_info->num_objects = (UCHAR )object_count;

    (BT_IGNORE_RETURN_VALUE) BT_fops_access_close(&h);
    return API_SUCCESS;
}


/**
 *  \fn ctn_get_object_attributes_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param idir
 *  \param object_name
 *  \param attr
 *
 *  \return None
 */
API_RESULT ctn_get_object_attributes_pl
           (
               /* IN */  UCHAR  *idir,
               /* IN */  CHAR   *object_name,
               /* OUT */ CTN_OBJ_ATTR_PL *attr
           )
{
    API_RESULT              retval;
    BT_fops_file_handle     fd;
    CHAR                    *readstr;
    CHAR                    *str_ptr, *str_ptr1;
    UCHAR                   object_full_name[MAX_CTN_FOLDER_NAME_LEN];
    int                     i, len;
    UINT32                  fsize, offset;
    UINT16                  count;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    fd = NULL;
    fsize = 0U;
    BT_mem_set(object_full_name, 0, sizeof(object_full_name));

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        idir,
        (UCHAR *)object_name,
        object_full_name
    );

    retval = BT_fops_file_open(object_full_name, (UCHAR *)"rb", &fd);
    BT_IGNORE_UNUSED_PARAM(retval);
    if (NULL == fd)
    {
        return API_FAILURE;
    }

    /* Get the file size */
    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fd, &fsize);

    readstr = ctn_msg_readline;
    count = sizeof(ctn_msg_readline);

    /* Get Priority */
    BT_str_print(attr->priority, "%s", "low");
    attr->obj_param_mask |= CTN_OBJ_PROP_MASK_PRIORITY;

    /* set alarm status */
    BT_str_print(attr->alarm_status, "%s", "no");
    attr->obj_param_mask |= CTN_OBJ_PROP_MASK_ALARMSTATUS;

    /* set recurrent status */
    BT_str_print(attr->recurrent, "%s", "no");
    attr->obj_param_mask |= CTN_OBJ_PROP_MASK_RECURRENT;

    /* set sent status */
    BT_str_print(attr->sentstatus, "%s", "no");
    attr->obj_param_mask |= CTN_OBJ_PROP_MASK_SENDSTATUS;

    /* Get Attachment Size */
    attr->size = (UINT16)fsize;
    attr->obj_param_mask |= CTN_OBJ_PROP_MASK_SIZE;

    BT_LOOP_FOREVER()
    {
        /* using fgets instead fscanf to read white spaces also */
        BT_mem_set(readstr, 0, count);
        if (API_SUCCESS != (BT_fops_file_get(fd, (UCHAR *)readstr, &count)))
        {
            break;
        }

        len = BT_str_len(readstr);

        for (i = 0U; i < len; i++)
        {
            if (('\r' == readstr[i]) || ('\n' == readstr[i]))
            {
                readstr[i] = '\0';
                break;
            }
        }

        /* Handle */
        if (NULL != BT_str_str(readstr, "HANDLE:"))
        {
            BT_str_print(attr->handle, "%s", &readstr[7U]);
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_HANDLE;
            continue;
        }

        /* update field */
        if (NULL != BT_str_str(readstr, "UPDATE"))
        {
            offset = 6U;

            if (':' == readstr[offset])
            {
                offset += 1U;
            }
            else
            {
                offset += 1U;

                /**
                 * Note: Timezone field also present
                 */
                for (; offset < BT_str_len(readstr); offset++)
                {
                    if(':' == readstr[offset])
                    {
                        offset += 1U;
                        break;
                    }
                }
            }

            BT_str_print(attr->update, "%s", &readstr[offset]);
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_UPDATE;
            continue;
        }

        /* cal-type */
        if (NULL != BT_str_str(readstr, "BEGIN:VEVENT"))
        {
            /* sprintf(attr->cal_type, "%s", "VEVENT"); */
            BT_str_print(attr->cal_type, "%s", "event");
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_CALTYPE;
            attr->obj_type = APPL_CTN_OBJ_TYPE_VEVENT;
            continue;
        }

        if (NULL != BT_str_str(readstr, "BEGIN:VTODO"))
        {
            /* sprintf(attr->cal_type, "%s", "VTODO"); */
            BT_str_print(attr->cal_type, "%s", "task");
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_CALTYPE;
            attr->obj_type = APPL_CTN_OBJ_TYPE_VTODO;
            continue;
        }

        if (NULL != BT_str_str(readstr, "BEGIN:VJOURNAL"))
        {
            /* sprintf(attr->cal_type, "%s", "VJOURNAL"); */
            BT_str_print(attr->cal_type, "%s", "note");
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_CALTYPE;
            attr->obj_type = APPL_CTN_OBJ_TYPE_VNOTE;
            continue;
        }

        /* summary */
        if (NULL != BT_str_str(readstr, "SUMMARY:"))
        {
            BT_str_print(attr->summary, "%s", &readstr[8U]);
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_SUMMARY;
            continue;
        }

        /* alarm */
        if (NULL != BT_str_str(readstr, "BEGIN:VALARM:"))
        {
            BT_str_print(attr->alarm_status, "%s", "yes");
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_ALARMSTATUS;
            continue;
        }

        if (NULL != BT_str_str(readstr, "ORGANIZER;"))
        {
            str_ptr = BT_str_str(readstr, "CN=\"");
            str_ptr += 4U;

            str_ptr1 = BT_str_str(str_ptr, "\"");

            len = str_ptr1 - str_ptr;

            BT_mem_copy(attr->org_name, str_ptr, len);
            attr->org_name[len] = '\0';
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_ORIGINATOR_NAME;

            str_ptr = BT_str_str(readstr, "MAILTO");
            str_ptr += 7U; /* 6 + 1(for :)*/

            BT_str_print(attr->org_addr, "%s", str_ptr);
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_ORIGINATOR_ADDRS;
            continue;
        }

        /* start time */
        if (NULL != BT_str_str(readstr, "DTSTART"))
        {
            offset = 7U;
            if (':' == readstr[offset])
            {
                offset += 1U;
            }
            else
            {
                /**
                 * Note: Timezone field also present in the string
                 */
                offset += 1U;

                for (; offset < BT_str_len(readstr); offset++)
                {
                    if(':' == readstr[offset])
                    {
                        offset += 1U;
                        break;
                    }
                }
            }

            BT_str_print(attr->start_time, "%s", &readstr[offset]);
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_STARTTIME;
            continue;
        }

        /* end time */
        if (NULL != BT_str_str(readstr, "DTEND"))
        {
            offset = 5U;
            if (':' == readstr[offset])
            {
                offset += 1U;
            }
            else
            {
                /**
                 * Note: Timezone field also present in the string
                 */
                offset += 1U;
                for (; offset < BT_str_len(readstr); offset++)
                {
                    if(':' == readstr[offset])
                    {
                        offset += 1U;
                        break;
                    }
                }
            }

            BT_str_print(attr->end_time, "%s", &readstr[offset]);
            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_ENDTIME;
            continue;
        }

        /* pstatus */
        if (NULL != BT_str_str(readstr, "ATTENDEE"))
        {
            str_ptr = BT_str_str(readstr, "PARTSTAT=");
            str_ptr += 9U;

            str_ptr1 = BT_str_str(str_ptr, ";");

            len = str_ptr1 - str_ptr;

            BT_mem_copy(attr->pstaus, str_ptr, len);
            attr->pstaus[len] = '\0';

            attr->obj_param_mask |= CTN_OBJ_PROP_MASK_PSTATUS;
            continue;
        }

    }

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close(fd);
    fd = NULL;

    return API_SUCCESS;
}

/* compares timestamp values */
INT16 ctn_compare_timestamp_pl
      (
          UCHAR *t1,
          UCHAR  t1_len,
          UCHAR *t2,
          UCHAR  t2_len
    )
{
    CHAR    src_year[6U], src_month[4U], src_day[4U];
    CHAR    dst_year[6U], dst_month[4U], dst_day[4U];
    UCHAR   offset;
    UINT32   src_val, dst_val;

    /* param check */
    if ((NULL == t1) || (NULL == t2) ||
        (0x00U == t1_len) || (0x00U == t2_len))
    {
        return API_FAILURE;
    }

    offset = 0x00U;
    BT_mem_copy(src_year, &t1[offset], 4U);
    BT_mem_copy(dst_year, &t2[offset], 4U);
    src_year[4U] = '\0';
    dst_year[4U] = '\0';
    offset += 4U;

    src_val = EM_str_to_num_pl(src_year, (UINT16)BT_str_len(src_year));
    dst_val = EM_str_to_num_pl(dst_year, (UINT16)BT_str_len(dst_year));

    /* comaparing years */
    if (src_val > dst_val)
    {
        return 1U;
    }
    else if (src_val < dst_val)
    {
        return -1;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    BT_mem_copy(src_month, &t1[offset], 2U);
    BT_mem_copy(dst_month, &t2[offset], 2U);
    src_month[2U] = '\0';
    dst_month[2U] = '\0';

    offset += 2U;

    src_val = EM_str_to_num_pl(src_month, (UINT16)BT_str_len(src_month));
    dst_val = EM_str_to_num_pl(dst_month, (UINT16)BT_str_len(dst_month));

    /* comparing months */
    if (src_val > dst_val)
    {
        return 1U;
    }
    else if (src_val < dst_val)
    {
        return -1;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    BT_mem_copy(src_day, &t1[offset], 2U);
    BT_mem_copy(dst_day, &t2[offset], 2U);
    src_day[2U] = '\0';
    dst_day[2U] = '\0';

    src_val = EM_str_to_num_pl(src_day, (UINT16)BT_str_len(src_day));
    dst_val = EM_str_to_num_pl(dst_day, (UINT16)BT_str_len(dst_day));

    /* comparing days */
    if (src_val > dst_val)
    {
        return 1U;
    }
    else if (src_val < dst_val)
    {
        return -1;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    offset += 3U;

    /* ==== */
    BT_mem_copy(src_day, &t1[offset], 2U);
    BT_mem_copy(dst_day, &t2[offset], 2U);
    src_day[2U] = '\0';
    dst_day[2U] = '\0';

    src_val = EM_str_to_num_pl(src_day, (UINT16)BT_str_len(src_day));
    dst_val = EM_str_to_num_pl(dst_day, (UINT16)BT_str_len(dst_day));

    /* compare hours */
    if (src_val > dst_val)
    {
        return 1U;
    }
    else if (src_val < dst_val)
    {
        return -1;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    BT_mem_copy(src_day, &t1[offset], 2U);
    BT_mem_copy(dst_day, &t2[offset], 2U);
    src_day[2U] = '\0';
    dst_day[2U] = '\0';

    /* compare minutes */
    src_val = EM_str_to_num_pl(src_day, (UINT16)BT_str_len(src_day));
    dst_val = EM_str_to_num_pl(dst_day, (UINT16)BT_str_len(dst_day));

    if (src_val > dst_val)
    {
        return 1U;
    }
    else if (src_val < dst_val)
    {
        return -1;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    BT_mem_copy(src_day, &t1[offset], 2U);
    BT_mem_copy(dst_day, &t2[offset], 2U);
    src_day[2U] = '\0';
    dst_day[2U] = '\0';

    src_val = EM_str_to_num_pl(src_day, (UINT16)BT_str_len(src_day));
    dst_val = EM_str_to_num_pl(dst_day, (UINT16)BT_str_len(dst_day));

    if (src_val > dst_val)
    {
        return 1U;
    }
    else if (src_val < dst_val)
    {
        return -1;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return 0U;
}

/**
 *  \fn BT_ctn_convert_bytes_to_chars
 *
 *  \brief To convert byte stream to character format
 *
 *  \Description
 *
 *
 *  \param [in] byte_stream
 *  \param [in] byte_stream_len
 *  \param [out] chars
 *  \param [out] chars_len
 *
 *  \return None
 */
API_RESULT BT_ctn_convert_bytes_to_chars
           (
               UCHAR    *byte_stream,
               UCHAR     byte_stream_len,
               UCHAR    *chrs,
               UCHAR    *chrs_len
           )
{
    API_RESULT  retval;
    UCHAR       i, j;

    retval = API_SUCCESS;

    if ((NULL == byte_stream) ||
        (NULL == chrs) ||
        (0x00U == byte_stream_len))
    {
        retval = API_FAILURE;
    }
    else
    {
        j = 0U;
        for (i = 0U; i < byte_stream_len; i++)
        {
            chrs[j] = ctn_nibble_to_char(byte_stream[i] >> 4U);
            chrs[j+1U] = ctn_nibble_to_char(byte_stream[i] & 0x0FU);

            j += 2U;
        }

        *chrs_len = j;
    }
    return retval;
}

CHAR  ctn_nibble_to_char(UCHAR nibble)
{
    CHAR ch;

    ch = '0';
    nibble &= 0x0Fu;

    if (nibble <= 9u )
    {
        ch = nibble + '0';
    }
    else if ((0x0au <= nibble) && (0x0fu >= nibble))
    {
         ch = (CHAR)'a' + (CHAR)(nibble - (UCHAR)0x0au);
    }
    else
    {
         /* TODO: */
    }

    return ch;
}

/**
 *  \fn BT_ctn_update_object_handle_pl
 *
 *  \brief To add CTN handle property to the object
 *
 *  \Description
 *
 *
 *  \param dir_entry
 *  \param src_file_name
 *  \param ctn_handle
 *  \param ctn_handle_len
 *
 *  \return None
 */
 API_RESULT BT_ctn_update_object_handle_pl
            (
                /* IN */  UCHAR   *dir_entry,
                /* IN */  UCHAR   *src_file_name,
                /* IN */  UCHAR   *ctn_handle,
                /* IN */  UCHAR    ctn_handle_len
            )
{
    API_RESULT retval;
    BT_fops_file_handle  src_fd;
    BT_fops_file_handle  dst_fd;
    UCHAR   src_obj_full_name[MAX_CTN_FOLDER_NAME_LEN];
    UCHAR   dst_obj_full_name[MAX_CTN_FOLDER_NAME_LEN];
    CHAR    *readstr;
    CHAR    handle_field[64U];
    UCHAR   dst_file_name[64U];
    CHAR   *tmp_ptr;
    UCHAR   flag;
    UINT16  count;

    retval = API_SUCCESS;
    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    src_fd = NULL;
    dst_fd = NULL;

    /* param check */
    if ((NULL == dir_entry) ||
        (NULL == src_file_name) ||
        (NULL == ctn_handle))
    {
        return API_FAILURE;
    }

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        dir_entry,
        src_file_name,
        src_obj_full_name
    );

    /* open src_file, dst file */
    retval = BT_fops_file_open(src_obj_full_name, (UCHAR *)"rb", &src_fd);
    if (NULL == src_fd)
    {
        return API_FAILURE;
    }

    BT_str_n_copy
    (
        dst_file_name,
        ctn_handle,
        (sizeof(dst_file_name) - 1)
    );

    BT_str_n_cat
    (
        dst_file_name,
        ".ics",
        (sizeof(dst_file_name) - BT_str_len(dst_file_name) - 1)
    );

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_vfops_create_object_name
    (
        dir_entry,
        dst_file_name,
        dst_obj_full_name
    );

    retval = BT_fops_file_open(dst_obj_full_name, (UCHAR *)"wb", &dst_fd);
    if (NULL == dst_fd)
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close(src_fd);

        return API_FAILURE;
    }

    readstr = ctn_msg_readline;
    count = sizeof(ctn_msg_readline);

    flag = 0x00U;

    BT_LOOP_FOREVER()
    {
        /* Read one line */
        count = sizeof(ctn_msg_readline);
        BT_mem_set(readstr, 0, count);
        if (API_SUCCESS != (BT_fops_file_get(src_fd, (UCHAR *)readstr, &count)))
        {
            break;
        }

        if (0x00U == flag)
        {
            /* Search for VERSION property */
            tmp_ptr = BT_str_str(readstr, "VERSION");
            if (NULL != tmp_ptr)
            {
                /* Yes, Version field found */

                /* Construct HANDLE field */
                BT_str_n_copy
                (
                    handle_field,
                    "HANDLE:",
                    (sizeof("HANDLE:"))
                );

                /* Append handle value */
                BT_str_n_cat
                (
                    handle_field,
                    ctn_handle,
                    (sizeof(handle_field) - BT_str_len(handle_field) - 1)
                );

               handle_field[7U+ctn_handle_len] = '\r';
               handle_field[7U+ctn_handle_len+1U] = '\n';
               handle_field[7U+ctn_handle_len+2U] = '\0';

                flag = 0x01U;
            }
        }

        tmp_ptr = BT_str_str(readstr, "HANDLE");
        if (NULL != tmp_ptr)
        {
            /**
             * skip writing this field to the file
             */
            continue;
        }

        count = (UINT16)BT_str_len(readstr);
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_put (dst_fd, (UCHAR *)readstr, &count);

        if (0x01U == flag)
        {
            /* reset */
            flag = 0x02U;

            /* write the constructed 'HANDLE' */
            count = (UINT16)BT_str_len(handle_field);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_put(dst_fd, (UCHAR *)handle_field, &count);
        }

    }

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close(src_fd);
    src_fd = NULL;

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close(dst_fd);
    dst_fd = NULL;

    return retval;
}

/**
 *  \fn BT_ctn_build_event_file_pl
 *
 *  \brief
 *      This is function will build CTN event object
 *
 *  \Description
 *
 *
 *  \param [in] dir_entry
 *  \param [in] event_file
 *  \param [in] event_type
 *  \param [in] handle
 *  \param [in] cal_type
 *
 *  \return None
 */
API_RESULT BT_ctn_build_event_file_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *event_file,
               /* IN */  UCHAR   *event_type,
               /* IN */  UCHAR   *handle,
               /* IN */  UCHAR   *cal_type
           )
{
    API_RESULT retval;
    CHAR    event_file_full_name[MAX_CTN_FOLDER_NAME_LEN];
    UCHAR parent[] = "CTN-event-report";
    CHAR version[] = "version = \"1.0\"";
    BT_fops_file_handle  event_fd;

    retval = API_SUCCESS;
    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    event_fd = NULL;

    /* param check */
    if ((NULL == dir_entry) ||
        (NULL == event_file) ||
        (NULL == event_type) ||
        (NULL == handle) ||
        (NULL == cal_type))
    {
        retval = API_FAILURE;
    }
    else
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            dir_entry,
            (UCHAR *)event_file,
            (UCHAR *)event_file_full_name
        );

        /* open event file */
        retval = BT_fops_file_open((UCHAR *)event_file_full_name, (UCHAR *)"wb", &event_fd);
        if (NULL == event_fd)
        {
            retval = API_FAILURE;
        }
        else
        {
            BT_fops_file_print(event_fd,"<%s %s>\n", parent, version);
            BT_fops_file_print(event_fd, "<event type =\"%s\" ", event_type);
            BT_fops_file_print(event_fd, "handle =\"%s\" ", handle);
            BT_fops_file_print(event_fd, "cal_type =\"%s\" ", cal_type);
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "summary=\"we have to talk\" summary=\"we have to talk\" summary=\"we have to talk\" ");
            BT_fops_file_print(event_fd, "datetime=\"20130501T200000\"" );
            BT_fops_file_print(event_fd, "originator_name=\"Ruediger\" originator_address=\"ruediger@bms.com\" />\n");
            BT_fops_file_print(event_fd,"</%s>\n",parent);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(event_fd);
            event_fd = NULL;
        }
    }
    return retval;
}
#endif /* ((defined CTN_CCE) || (defined CTN_CSE)) */
