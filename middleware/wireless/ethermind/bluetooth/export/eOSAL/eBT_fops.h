
/**
 *  \file eBT_fops.h
 *
 *  This Header File contains the APIs and the ADTs exported by the
 *  EtherMind Timer Library for QNX.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EBT_FOPS_
#define _H_EBT_FOPS_

/* --------------------------------------------- Header File Inclusion */
#include "EM_fops.h"

/* --------------------------------------------- Global Definitions */
#define BT_TIMER_HANDLE_INIT_VAL            EM_TIMER_HANDLE_INIT_VAL

#define BT_FOPS_MASK_READONLY               EM_FOPS_MASK_READONLY
#define BT_FOPS_MASK_HIDDEN                 EM_FOPS_MASK_HIDDEN
#define BT_FOPS_MASK_SYSTEM                 EM_FOPS_MASK_SYSTEM
#define BT_FOPS_MASK_FOLDER                 EM_FOPS_MASK_FOLDER
#define BT_FOPS_MASK_ARCHIVE                EM_FOPS_MASK_ARCHIVE

#define BT_FOPS_SEEK_SET                    EM_FOPS_SEEK_SET
#define BT_FOPS_SEEK_CUR                    EM_FOPS_SEEK_CUR
#define BT_FOPS_SEEK_END                    EM_FOPS_SEEK_END

#define BT_FOPS_BASE                        EM_FOPS_BASE
#define BT_FOPS_PATH_SEP                    EM_FOPS_PATH_SEP
#define BT_FOPS_MAX_DIRECTORY_SIZE          EM_FOPS_MAX_DIRECTORY_SIZE
#define BT_FOPS_MAX_FN_SIZE                 EM_FOPS_MAX_FN_SIZE


/* --------------------------------------------- Structures/Data Types */
#define BT_fops_file_handle                 EM_fops_file_handle
#define BT_fops_object_handle               EM_fops_object_handle

/* --------------------------------------------- Macros */
#define BT_FOPS_PATH_JOIN                   EM_FOPS_PATH_JOIN

#define BT_FOPS_SET_BIT                     EM_FOPS_SET_BIT
#define BT_FOPS_GET_BIT                     EM_FOPS_GET_BIT
#define BT_FOPS_RESET_BIT                   EM_FOPS_RESET_BIT

/* --------------------------------------------- Macros */

/* --------------------------------------------- Data Structures */
#define BT_FOPS_FILINFO                     EM_FOPS_FILINFO

/* --------------------------------------------- API Declarations */
#define BT_fops_get_current_directory       EM_fops_get_current_directory
#define BT_fops_get_file_attributes         EM_fops_get_file_attributes
#define BT_fops_set_file_attributes         EM_fops_set_file_attributes
#define BT_fops_access_first                EM_fops_access_first
#define BT_fops_access_next                 EM_fops_access_next
#define BT_fops_access_close                EM_fops_access_close
#define BT_fops_set_path_forward            EM_fops_set_path_forward
#define BT_fops_set_path_backward           EM_fops_set_path_backward
#define BT_fops_create_folder               EM_fops_create_folder
#define BT_fops_file_open                   EM_fops_file_open
#define BT_fops_file_write                  EM_fops_file_write
#define BT_fops_file_read                   EM_fops_file_read
#define BT_fops_file_print                  EM_fops_file_print
#define BT_fops_file_put                    EM_fops_file_put
#define BT_fops_file_get                    EM_fops_file_get
#define BT_fops_file_get_formatted          EM_fops_file_get_formatted
#define BT_fops_file_close                  EM_fops_file_close
#define BT_fops_object_delete               EM_fops_object_delete
#define BT_fops_file_size                   EM_fops_file_size
#define BT_fops_file_seek                   EM_fops_file_seek
#define BT_fops_file_copy                   EM_fops_file_copy
#define BT_fops_file_move                   EM_fops_file_move
#define BT_vfops_create_object_name         EM_vfops_create_object_name
#define BT_vfops_set_path_backward          EM_vfops_set_path_backward
#define BT_vfops_set_path_forward           EM_vfops_set_path_forward
#define BT_fops_list_directory              EM_fops_list_directory

#endif /* _H_EBT_FOPS_ */

