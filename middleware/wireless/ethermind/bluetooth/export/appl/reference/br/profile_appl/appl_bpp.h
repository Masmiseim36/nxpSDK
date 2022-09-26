
/**
 *  \file appl_bpp.h
 *
 *  Header file for BPP Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_BPP_
#define _H_APPL_BPP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_bpp_api.h"
#include "BT_sdp_api.h"
#include "BT_hci_api.h"
#include "BT_fops.h"

/* ----------------------------------------- External Global Variables */
#define BPP_NUM_PRINTER_INSTANCE         1U
#define BPP_NUM_SENDER_INSTANCE          1U

#define BPP_ROOT_FOLDER_BASE             BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "bpp")
#define BPP_ROOT_RX_FOLDER_BASE          BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "bpp" BT_FOPS_PATH_SEP "rx")
#define BPP_FOLDER_NAME_LEN              64U

#define BPP_SDP_RECORD_DATA_SIZE         512U

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

typedef struct _BPP_SDP_RECORDS
{
    UCHAR bpp_attrib_data[BPP_SDP_RECORD_DATA_SIZE];

    UINT16 bpp_len_attrib_data;

    SDP_HANDLE    bpp_sdp_handle;

}BPP_SDP_RECORD;

typedef struct _BPP_INSTANCE
{
    /* BPP Handle */
    BPP_HANDLE    handle;

    /* BPP SDP record */
    BPP_SDP_RECORD bpp_sdp_record;

    /* Instance current folder path */
    UCHAR path[BPP_FOLDER_NAME_LEN];

    /* Root depth of the instance */
    INT8 root_depth;

    /* Peer BD Address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Max. Tx. data size */
    UINT32 max_xchg_size;

}BPP_INSTANCE;

/* User Id and associated password */
typedef struct _APPL_BPP_SER_USER_INFO
{
    UCHAR *user_id;

    UCHAR *passwd;
}APPL_BPP_SER_USER_INFO;

/* To Reset a BPP handle */
#define BPP_RESET_INSTANCE(hdl)    \
        (hdl)->handle = BPP_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);

/* ----------------------------------------- Functions */
void main_bpp_sender_operations(void);
void main_bpp_printer_operations(void);

API_RESULT appl_bpp_sender_callback
           (
               BPP_HANDLE      * bpp_handle,
               UINT8             event_type,
               UINT16            event_result,
               BPP_HEADERS     * event_header,
               UINT16            event_hdrlen,
               UCHAR             bpp_channel_type
           );

API_RESULT appl_bpp_printer_callback
           (
               BPP_HANDLE      * bpp_handle,
               UINT8             event_type,
               UINT16            event_result,
               BPP_HEADERS     * event_header,
               UINT16            event_hdrlen,
               UCHAR             bpp_channel_type
           );

void appl_bpp_sender_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void appl_bpp_printer_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void bpp_sender_print_appl_instances (UCHAR bpp_channel_type);

void bpp_printer_print_appl_instances (UCHAR bpp_channel_type);

#endif /* _H_APPL_BPP_ */

