
/**
 *  \file pbap_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "pbap_pl.h"
#include "object_parser_api.h"

#if ((defined PBAP_PCE) || (defined PBAP_PSE))

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const CHAR pbap_vcard_listing_xml_hdr[] = "<?xml version=\"1.0\"?>\n\
<!DOCTYPE vcard-listing SYSTEM \"vcard-listing.dtd\">\n\
<vCard-listing version=\"1.0\">\n";

static const CHAR pbap_vcard_listing_xml_trailer[] = "</vCard-listing>\n";

/* --------------------------------------------- Functions */

/**
 *  \fn BT_pbap_build_xml_vcard_listing_pl
 *
 *  \brief To build PBAP vcard xml file
 *
 *  \Description
 *      This Function constructs PBAP vCard listing xml file in the given directory.
 *      PBAP application paramater filtering is applied while constructing listing xml file.
 *
 *  \param dir_entry
 *  \param listingfile
 *  \param vcards
 *  \param vcard_count
 *  \param appl_params
 *
 *  \return None
 */

API_RESULT BT_pbap_build_xml_vcard_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* IN */  PBAP_VCARD_LIST   * vcards,
               /* IN */  UINT16  vcard_count,
               /* IN */  PBAP_APPL_PARAMS *appl_params,
               /* OUT */ UINT16   *filtered_vcard_cnt
           )
{
    API_RESULT  retval;
    BT_fops_file_handle xml_fd;
    UCHAR       obj_name[128U];
    UINT16      i;
    UINT16      vcard_start_index;
    UINT32      vcard_prop_selector_mask;
    UINT32      vcard_prop_bit_mask;
    UINT16      _filtered_vcard_cnt;

    /* Init */
    retval              = API_SUCCESS;
    vcard_start_index   = 0U;
    vcard_prop_selector_mask = 0U;
    vcard_prop_bit_mask = 0U;
    *filtered_vcard_cnt = 0U;
    _filtered_vcard_cnt = 0U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    xml_fd              = NULL;

    /* Valide data input params */
    if ((NULL == dir_entry) || (NULL == listingfile))
    {
        BT_debug_error(BT_MODULE_ID_PBAP,
        "[PBAP_PL] Invalid Parameter\n");

        return PBAP_INVALID_PARAMETERS;
    }

    if (NULL != appl_params)
    {
        if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_LIST_START_OFFSET)))
        {
            vcard_start_index = appl_params->list_start_offset;
        }

#ifdef PBAP_1_2
       /**
        * if vCardSelect operator is not received in the request,
        * then set it to OR as default operator.
        */
        if (0x00U == PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR_OPERATOR))
        {
            appl_params->vcard_selector_operator = 0x00U;
        }
        else
        {
            /* Set the operation as AND */
            appl_params->vcard_selector_operator = 0x01U;
        }

        /* Apply VCARD selector filtering */
        if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR)))
        {
            for (i = 0U; i < vcard_count; i++)
            {
                vcards[i].listing_status = BT_TRUE;

                /* Extract initial 32 bits of vcard selector field */
                vcard_prop_bit_mask = (UINT32 )appl_params->vcard_selector[1U];
                vcard_prop_bit_mask =  (vcard_prop_bit_mask << 16U) |(UINT32 )appl_params->vcard_selector[0U];

                vcard_prop_selector_mask =   vcard_prop_bit_mask &  vcards[i].prop_bit_mask;

                if (((0x00U == appl_params->vcard_selector_operator) &&
                     (0x00U == vcard_prop_selector_mask)) ||
                    ((0x01U == appl_params->vcard_selector_operator) &&
                     (vcard_prop_selector_mask != vcard_prop_bit_mask)))
                {
                    vcards[i].listing_status = BT_FALSE;
                }
            }
        }
#endif /* PBAP_1_2 */

        /* Apply search filtering */
        if (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_SEARCH_ATTRIBUTE) &&
            PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_SEARCH_VALUE))
        {
            /* Search attr_val in the list */
            for (i = 0U; i < vcard_count; i++)
            {
                CHAR *ret = NULL;

                if (0x00U == appl_params->search_attr)
                {
                    ret = BT_str_str ((CHAR *)vcards[i].n, (CHAR *)appl_params->search_value.value);
                }
                else if (0x01U == appl_params->search_attr)
                {
                    ret = BT_str_str ((CHAR *)vcards[i].tel, (CHAR *)appl_params->search_value.value);
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                if (NULL == ret)
                {
                    vcards[i].listing_status = BT_FALSE;
                }
            }
        }

        /* Apply Order filtering */
        if (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_ORDER) &&
            (0x01U == appl_params->order))
        {
            pbap_sort_pb_list(vcards, vcard_count);
        }
    }

    /* Append the path of the new directory */
    BT_str_n_copy (obj_name, dir_entry, (sizeof(obj_name) - 1));
    BT_str_n_cat (obj_name, BT_FOPS_PATH_SEP, (sizeof(obj_name) - BT_str_len(obj_name) - 1));
    BT_str_n_cat (obj_name, listingfile, (sizeof(obj_name) - BT_str_len(obj_name) - 1));

    /* Open the file */
    (BT_IGNORE_RETURN_VALUE) BT_fops_file_open (obj_name, (UCHAR *)"wb", &xml_fd);
    if (NULL == xml_fd)
    {
        BT_debug_error(BT_MODULE_ID_PBAP,
        "[PBAP_PL] Invalid Parameter\n");

        return PBAP_INVALID_PARAMETERS;
    }

    /* Construct vCard Listing */
    BT_fops_file_print (xml_fd, "%s", pbap_vcard_listing_xml_hdr);
    for (i = vcard_start_index; i < vcard_count; i++)
    {
        if (BT_FALSE == vcards[i].listing_status)
        {
            continue;
        }

        BT_fops_file_print(xml_fd, "  <card handle = \"%s\"", vcards[i].vcard_file_name);
        BT_fops_file_print(xml_fd, "    name = \"%s\"", vcards[i].n);

        _filtered_vcard_cnt += 1U;

#if 0
        /* For Debug */
        if (0 != BT_str_len(vcards[i].tel))
        {
            BT_fops_file_print(xml_fd, "    tel = \"%s\"", vcards[i].tel);
        }

        if (0 != BT_str_len(vcards[i].vcard_bt_uid))
        {
            BT_fops_file_print(xml_fd, "    bt_uid = \"%s\"", vcards[i].vcard_bt_uid);
        }
#endif /* 0 */

        BT_fops_file_print(xml_fd, "/>\n");
    }

    *filtered_vcard_cnt = _filtered_vcard_cnt;

    BT_fops_file_print(xml_fd, "%s", pbap_vcard_listing_xml_trailer);

    /* close the file */
    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void)BT_fops_file_close(xml_fd);

    return retval;
}

/**
 *  \fn BT_pbap_get_vcard_list_pl
 *
 *  \brief To get list of vcards in given directory entry.
 *
 *  \Description
 *      This Functions gets list of vCards(with handles in accending order) in given directory entry.
 *
 *  \param dir_entry
 *  \param vcards
 *  \param num_vcards
 *
 *  \return None
 */
API_RESULT BT_pbap_get_vcard_list_pl
           (
               /* IN  */  UCHAR             * dir_entry,
               /* OUT */  PBAP_VCARD_LIST   * vcards,
               /* OUT */  UINT16            * num_vcards
           )
{

    API_RESULT  retval;
    UCHAR       dir[PBAP_FOLDER_NAME_LEN];
    UINT16      vcard_count;
    UCHAR       temp[MAX_VCARD_NAME_LEN];
    UINT16      i, j, handle1, handle2;

    UCHAR       first;

    BT_fops_object_handle h;
    BT_FOPS_FILINFO info;

    retval      = API_SUCCESS;
    vcard_count = 0U;

    /* Valide data input params */
    if ((NULL == dir_entry) || (NULL == vcards))
    {
        BT_debug_error(BT_MODULE_ID_PBAP,
        "[PBAP_PL] Invalid Parameter\n");

        return PBAP_INVALID_PARAMETERS;
    }

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));
    BT_mem_set(&info, 0, sizeof(BT_FOPS_FILINFO));

    BT_str_n_copy(dir, dir_entry, (sizeof(dir) - 1));

    BT_debug_trace(BT_MODULE_ID_PBAP,
    "[PBAP_PL] Path = %s\n", dir_entry);

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

        if(0 != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            BT_debug_info(BT_MODULE_ID_PBAP,
                "[PBAP_PL] It is a directory\n");
        }
        else
        {
            BT_debug_info(BT_MODULE_ID_PBAP,
            "[PBAP_PL] It is a file\n");

            /* Check if a valid vcard file */
            if (NULL == BT_str_str(info.fname, ".vcf"))
            {
                /* It is not */
                continue;
            }

            BT_str_n_copy(vcards[vcard_count].vcard_file_name, info.fname, (sizeof(vcards[vcard_count].vcard_file_name) - 1));

            vcards[vcard_count].listing_status = BT_TRUE;

            vcard_count++;

        }
    }

    /* Sorting vCard list in increasing handle order */
    for (i = 0U; i < vcard_count; i++)
    {
        for (j = i+1U; j < vcard_count; j++)
        {
            get_vcard_handle(vcards[i].vcard_file_name, &handle1);
            get_vcard_handle(vcards[j].vcard_file_name, &handle2);

            if (handle1  > handle2)
            {
                BT_str_n_copy(temp, vcards[i].vcard_file_name, sizeof(temp));

                BT_str_n_copy(vcards[i].vcard_file_name, vcards[j].vcard_file_name, sizeof(vcards[i].vcard_file_name));

                BT_str_n_copy(vcards[j].vcard_file_name, temp, sizeof(vcards[j].vcard_file_name));
            }
        }
    }

    (BT_IGNORE_RETURN_VALUE) BT_fops_access_close(&h);

    *num_vcards = vcard_count;

    return retval;
}

/**
 *  \fn BT_pbap_build_vcard_pl
 *
 *  \brief To construct PBAP vCard file
 *
 *  \Description
 *      This Function builds PBAP vCard after applying PBAP vcard property filtering.
 *
 *  \param vcard
 *  \param vcard_prop_filter
 *  \param in_vcard_data
 *  \param in_vcard_data_len
 *  \param out_vcard_data
 *  \param out_vcard_data_len
 *
 *  \return None
 */
API_RESULT BT_pbap_build_vcard_pl
           (
                /* IN */    PBAP_VCARD_OBJECT   * vcard,
                /* IN */    UINT32 vcard_prop_filter,
                /* IN */    UCHAR  * in_vcard_data,
                /* IN */    UINT16   in_vcard_data_len,
                /* OUT */   UCHAR  * out_vcard_data,
                /* OUT */   UINT16 * out_vcard_data_len
           )
{
    API_RESULT  retval = API_SUCCESS;
    UCHAR       num_properties;
    UINT16      j, k;
    UINT16      out_offset = 0U;

    if ((NULL == vcard) ||
        (0U == in_vcard_data_len) ||
        (NULL == in_vcard_data) ||
        (NULL == out_vcard_data))
    {
        BT_debug_error(BT_MODULE_ID_PBAP,
        "[PBAP_PL] Invalid Parameter\n");

        return PBAP_INVALID_PARAMETERS;
    }


    num_properties = vcard->num_properties;

    /**
     * Fill out_vcard_data with vcard info.
     */
    for (j = 0U; j < num_properties; j++)
    {
        /* vcard property filtering */
        if (!(vcard->info[j].prop_bit_mask & vcard_prop_filter) &&
             ( (BEGIN != vcard->info[j].prop_name_id) &&
               (END != vcard->info[j].prop_name_id) &&
               (VERSION != vcard->info[j].prop_name_id) &&
               (N != vcard->info[j].prop_name_id) &&
               (TEL != vcard->info[j].prop_name_id)))
        {
            /* Skip adding to vcard */
            continue;
        }

        /* Add Property Name */
        BT_mem_copy
        (
            &out_vcard_data[out_offset],
            vcard->info[j].prop_name,
            vcard->info[j].prop_name_len
        );

        out_offset += vcard->info[j].prop_name_len;

        if (0U < vcard->info[j].num_params)
        {
            for (k = 0U; k < vcard->info[j].num_params; k++)
            {
                if (0x00U == vcard->info[j].parm_info[k].param_name_len)
                {
                    out_vcard_data[out_offset] = ';';
                    out_offset ++;

                    BT_mem_copy
                    (
                        &out_vcard_data[out_offset],
                        vcard->info[j].parm_info[k].param_value,
                        vcard->info[j].parm_info[k].param_value_len
                    );

                    out_offset += vcard->info[j].parm_info[k].param_value_len;
                }
                else
                {
                    out_vcard_data[out_offset] = ';';
                    out_offset ++;

                    /* Add Property param name */
                    BT_mem_copy
                    (
                        &out_vcard_data[out_offset],
                        vcard->info[j].parm_info[k].param_name,
                        vcard->info[j].parm_info[k].param_name_len
                    );

                    out_offset += vcard->info[j].parm_info[k].param_name_len;

                    out_vcard_data[out_offset] = '=';
                    out_offset ++;

                    /* Add Property param value */
                    BT_mem_copy
                    (
                        &out_vcard_data[out_offset],
                        vcard->info[j].parm_info[k].param_value,
                        vcard->info[j].parm_info[k].param_value_len
                    );

                    out_offset += vcard->info[j].parm_info[k].param_value_len;
                }
            }
        }

        /* Add Property delimiter */
        out_vcard_data[out_offset] = ':';
        out_offset ++;

        /* Add Property value */
        BT_mem_copy
        (
            &out_vcard_data[out_offset],
            vcard->info[j].prop_val,
            vcard->info[j].prop_val_len
        );

        out_offset += vcard->info[j].prop_val_len;

        /**
         * For Photo property one extra new line char required, but our object parser
         * sending OBJECT_TOKEN_ERROR and not adding as PHOTO property value.
         * so appending new line here.
         */
        if(PHOTO == vcard->info[j].prop_name_id)
        {
            out_vcard_data[out_offset] = '\n';
            out_offset ++;
        }

        out_vcard_data[out_offset] = '\n';
        out_offset ++;
    }

    *out_vcard_data_len = out_offset;

    return retval;
}

/* To Sort the list based on Name property */
void pbap_sort_pb_list
     (
         PBAP_VCARD_LIST   * vcards,
         UINT16              vcard_count
     )
{
    UINT16  i, j;
    PBAP_VCARD_LIST temp;

    /* Sorting vcard list in asscending order of handle */
    for (i = 0U; i < vcard_count; i++)
    {
        for (j = i+1U; j < vcard_count; j++)
        {
            if (0 < BT_str_cmp(vcards[i].n, vcards[j].n))
            {
                BT_mem_copy (&temp, &vcards[i], sizeof(PBAP_VCARD_LIST));
                BT_mem_copy (&vcards[i], &vcards[j], sizeof(PBAP_VCARD_LIST));
                BT_mem_copy (&vcards[j], &temp, sizeof(PBAP_VCARD_LIST));
            }
        }
    }

    return;
}

/* To Get the vcard handle in numeric form */
void get_vcard_handle(UCHAR *vcard, UINT16 *handle)
{
    UINT16  i, j, len;
    UCHAR   input;
    UINT16  num;

    /* Init */
    num = 0U;
    j   = 0U;
    i   = 0U;
    len = (UINT16)BT_str_len(vcard);

    while (len > 0U)
    {
        input  = vcard[i];

        if ((' ' == input) || ('\t' == input))
        {
            len--;
            i++;

            continue;
        }

        if (input >= '0' && input <= '9')
        {
            num = num * 10U + input - '0';

            j++;
        }
        else if ('.' == input)
        {
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        len--;
        i++;
    }

    *handle = num;

    return;
}
#endif /* ((defined PBAP_PCE) || (defined PBAP_PSE)) */
