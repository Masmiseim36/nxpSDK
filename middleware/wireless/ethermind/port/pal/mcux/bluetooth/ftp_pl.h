
/**
 *  \file ftp_pl.h
 *
 *  Header file for all platform specific implementation & extensions for
 *  FTP module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_FTP_PL_
#define _H_FTP_PL_

/* ----------------------------------------- Header File Inclusion */
/* EtherMind Header Files */
#include "BT_common.h"
#include "BT_fops.h"

/* ----------------------------------------- FTP PL Debug Macros */
#ifndef FTP_PL_NO_DEBUG
#define FTP_PL_ERR(...)             BT_debug_error(BT_MODULE_ID_FTP, __VA_ARGS__)
#else  /* FTP_PL_NO_DEBUG */
#define FTP_PL_ERR                  BT_debug_null
#endif /* FTP_PL_NO_DEBUG */

#ifdef FTP_PL_DEBUG
    #define FTP_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_FTP, __VA_ARGS__)
    #define FTP_PL_INF(...)         BT_debug_info(BT_MODULE_ID_FTP, __VA_ARGS__)
#else /* FTP_PL_DEBUG */
    #define FTP_PL_TRC              BT_debug_null
    #define FTP_PL_INF              BT_debug_null
#endif   /* FTP_PL_DEBUG */

/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */


/* ----------------------------------------- API Declarations */
API_RESULT BT_ftp_server_create_xml_dir_listing
           (
               /* IN */  UCHAR               *dir_entry,
               /* IN */  BT_fops_file_handle  xml_fd
           );


/* ----------------------------------------- Function Declarations */

#endif /* _H_FTP_PL_ */

