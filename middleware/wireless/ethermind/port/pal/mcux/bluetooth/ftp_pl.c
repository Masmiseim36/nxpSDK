
/**
 *  \file ftp_pl.c
 *
 *  This file contains the implementation for all platform specific
 *  extensions for FTP module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "ftp_pl.h"

#ifdef FTP
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */


/* ----------------------------------------- API Functions */

/**
 *  \fn BT_ftp_server_create_xml_dir_listing
 *
 *  \brief Forms the folder listing of the given folder
 *
 *  \par Description:
 *   This takes as input the path of the folder and writes the list of files
 *   and folders into the file pointed by the given descriptor.
 *
 *  \param [in] dir_entry
 *         Path of the folder, of which the listing has to be formed.
 *  \param [in] xml_fd
 *         The file descriptor of the file where the listing has to be written
 *
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *        Form the XML string of the folder listing
 *      return
 *  \endcode
 *  \endif
 */

API_RESULT BT_ftp_server_create_xml_dir_listing
           (
               /* IN */  UCHAR               *dir_entry,
               /* IN */  BT_fops_file_handle  xml_fd
           )
{
    BT_fops_object_handle h;
    API_RESULT retval;
    BT_FOPS_FILINFO info;
    UCHAR parent[]="folder-listing";
    UCHAR dir[BT_FOPS_MAX_DIRECTORY_SIZE];

    if ((NULL == xml_fd) || (NULL == dir_entry))
    {
        FTP_PL_ERR(
        "[FTP_PL] Invalid Parameter\n");

        return FTP_INVALID_PARAMETERS;
    }

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&h, 0, sizeof(BT_fops_object_handle));
    BT_mem_set(&info, 0, sizeof(BT_FOPS_FILINFO));

    BT_str_n_copy(dir, dir_entry, (sizeof(dir) - 1));

    FTP_PL_TRC(
    "[FTP_PL] Path = %s\n", dir_entry);

    retval = BT_fops_access_first(dir, NULL, &h, &info);
    if (API_SUCCESS != retval)
    {
        FTP_PL_ERR(
        "[FTP_PL] Invalid File Handle. Get Last Error reports  %d\n");

        return API_FAILURE;
    }
    else
    {
        FTP_PL_INF(
        "[FTP_PL] The first file/directory found is %s\n",
        FindFileData.cFileName);

        if(0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
        {
            FTP_PL_INF(
            "[FTP_PL] It is a directory\n");
        }
        else
        {
            FTP_PL_INF(
            "[FTP_PL] It is a file\n");
        }
    }

    BT_fops_file_print(xml_fd ,"<%s>\n",parent);

    BT_LOOP_FOREVER()
    {
        if((0 != BT_str_cmp(".", info.fname)) &&
           (0 != BT_str_cmp("..", info.fname)))
        {
			if(0U != (info.fattrib & BT_FOPS_MASK_FOLDER))
			{
				FTP_PL_INF(
				"[FTP_PL] It is a directory\n");

				BT_fops_file_print(xml_fd,"\t<folder name=\"%s\"/>\n", info.fname);
			}
			else
			{
				FTP_PL_INF(
				"[FTP_PL] It is a file\n");

				BT_fops_file_print
				(
					xml_fd,
					"\t<file name=\"%s\" size=\"%d\"/>\n",
					info.fname,
					info.flsize
				);
			}
        }

        /* Get the next item */
        retval = BT_fops_access_next(&h, &info);
        if (API_SUCCESS != retval)
        {
            break;
        }
    }

    BT_fops_file_print(xml_fd ,"</%s>\n",parent);
    (BT_IGNORE_RETURN_VALUE) BT_fops_access_close(&h);

    return API_SUCCESS;
}
#endif /* FTP */
