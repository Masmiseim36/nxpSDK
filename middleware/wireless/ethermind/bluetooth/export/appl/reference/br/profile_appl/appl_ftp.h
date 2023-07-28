
/**
 *  \file appl_ftp.h
 *
 *  Header file for FTP Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_FTP_
#define _H_APPL_FTP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_ftp_api.h"

#include "BT_fops.h"

/* ----------------------------------------- External Global Variables */
#define FTP_NUM_CLIENT_INSTANCE          1U
#define FTP_NUM_SERVER_INSTANCE          1U

#define FOLDERLISTING_FILE               BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "folder_listing.xml")
#define FTP_ROOT_FOLDER_BASE             BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "ftp")
#define FTP_FOLDER_NAME_LEN              64U

#define FTP_SDP_RECORD_DATA_SIZE         512U

/**
 * Max. File Name Length
 */
#define FTP_FILE_NAME_LEN                64U

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

typedef struct _FTP_SDP_RECORDS
{
    UCHAR ftp_attrib_data[FTP_SDP_RECORD_DATA_SIZE];

    UINT16 ftp_len_attrib_data;

    SDP_HANDLE    ftp_sdp_handle;

}FTP_SDP_RECORD;

typedef struct _FTP_INSTANCE
{
    /* FTP Handle */
    FTP_HANDLE    handle;

    /* FTP SDP record */
    FTP_SDP_RECORD ftp_sdp_record;

    /* Instance current folder path */
    UCHAR path[FTP_FOLDER_NAME_LEN];

    /* Root depth of the instance */
    INT8 root_depth;

    /* Peer BD Address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Max. Tx. data size */
    UINT16 max_xchg_size;

}FTP_INSTANCE;

/* To Reset a FTP handle */
#define FTP_RESET_INSTANCE(hdl)    \
        (hdl)->handle = FTP_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);

/* ----------------------------------------- Functions */
void main_ftp_client_operations(void);
void main_ftp_server_operations(void);

API_RESULT appl_ftp_client_callback
           (
               FTP_HANDLE      * ftp_handle,
               UINT8             event_type,
               UINT16            event_result,
               FTP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

API_RESULT appl_ftp_server_callback
           (
               FTP_HANDLE      * ftp_handle,
               UINT8             event_type,
               UINT16            event_result,
               FTP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

void appl_ftp_ce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void ftp_client_print_appl_instances (void);
void ftp_server_print_appl_instances (void);

#endif /* _H_APPL_FTP_ */

