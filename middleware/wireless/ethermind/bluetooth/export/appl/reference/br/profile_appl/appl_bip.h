
/**
 *  \file appl_bip.h
 *
 *  Header file for BIP Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_BIP_
#define _H_APPL_BIP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_bip_api.h"
#include "BT_fops.h"

/* ----------------------------------------- External Global Variables */
#define BIP_NUM_INITIATOR_INSTANCE          1U
#define BIP_NUM_RESPONDER_INSTANCE          1U

#define CAPABILITIES_FILE               "images_capabilities.xml"
#define IMAGE_LIST_FILE                 "image_list.xml"
#define IMAGE_PROPERTIES_FILE           "image_properties.xml"
#define IMAGE_ATTACHMENT_FILE           "bip_attachment.txt"
#define SAMPLE_IMAGE                    "sample.jpg"
#define SAMPLE_IMAGE1                   "sample1.png"
#define SAMPLE_IMAGE2                   "sample2.jpg"
#define SAMPLE_IMAGE3                   "sample3.jpg"
#define SAMPLE_MONITORING_IMAGE         "sample.jpg"
#define SAMPLE_IMAGE_THUMBNAIL          "sample_thumbnail.jpg"

#define BIP_ROOT_FOLDER_BASE             BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "bip")
#define BIP_RX_ROOT_FOLDER_BASE          BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "bip" BT_FOPS_PATH_SEP "rx")
#define BIP_FOLDER_NAME_LEN              64U


#define BIP_RESPONDER_BASE_HANDLE       "1000000"

#define BIP_SDP_RECORD_DATA_SIZE         512U

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

typedef struct _BIP_SDP_RECORDS
{
    UCHAR bip_attrib_data[BIP_SDP_RECORD_DATA_SIZE];

    UINT16 bip_len_attrib_data;

    SDP_HANDLE    bip_sdp_handle;

}BIP_SDP_RECORD;

typedef struct _BIP_INSTANCE
{
    /* BIP Handle */
    BIP_HANDLE    handle;

    /* BIP SDP record */
    BIP_SDP_RECORD bip_sdp_record;

    /* Instance current folder path */
    UCHAR path[BIP_FOLDER_NAME_LEN];

    /* Root depth of the instance */
    INT8 root_depth;

    /* Peer BD Address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Max. Tx. data size */
    UINT32 max_xchg_size;

}BIP_INSTANCE;

/*for application purpose*/
typedef struct _BIP_IMAGE_INFO
{
    /*BIP IMAGE NAME*/
    UCHAR file_name[32];

    /* BIP IMAGE HANDLE */
    UCHAR image_handle[8];
}BIP_IMAGE_INFO;


/* To Reset a BIP handle */
#define BIP_RESET_INSTANCE(hdl)    \
        (hdl)->handle = BIP_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);

/* ----------------------------------------- Functions */
void main_bip_initiator_operations(void);
void main_bip_responder_operations(void);

API_RESULT appl_bip_initiator_callback
           (
               BIP_HANDLE      * bip_handle,
               UINT8             event_type,
               UINT16            event_result,
               BIP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

API_RESULT appl_bip_responder_callback
           (
               BIP_HANDLE      * bip_handle,
               UINT8             event_type,
               UINT16            event_result,
               BIP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

void appl_bip_initiator_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void appl_bip_responder_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void bip_initiator_pri_print_appl_instances (void);

void bip_initiator_sec_print_appl_instances (void);

void bip_responder_pri_print_appl_instances (void);

void bip_responder_sec_print_appl_instances (void);

#endif /* _H_APPL_BIP_ */

