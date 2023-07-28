
/**
 *  \file appl_opp.h
 *
 *  Header file for OPP Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_OPP_
#define _H_APPL_OPP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_opp_api.h"
#include "BT_fops.h"

#ifdef OPP

/* ----------------------------------------- External Global Variables */
#define OPP_NUM_CLIENT_INSTANCE          1U
#define OPP_NUM_SERVER_INSTANCE          1U

/* Default OPP object Names */
#define OPP_OBJECT_VCARD                    "mycard.vcf"
#define OPP_OBJECT_VCAL                     "mycal.vcs"
#define OPP_OBJECT_VMSG                     "mymsg.vmg"

/* OPP Object Type */
#define OPP_OBJECT_TYPE_VCARD               "VCARD"
#define OPP_OBJECT_TYPE_VCAL                "VCALENDAR"
#define OPP_OBJECT_TYPE_VMSG                "VMSG"
#define OPP_OBJECT_TYPE_VNOTE               "VNOTE"

#define OPP_ROOT_FOLDER_BASE             BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "opp")
#define OPP_ROOT_RX_FOLDER_BASE          BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "opp" BT_FOPS_PATH_SEP "rx")
#define OPP_FOLDER_NAME_LEN              64U
#define OPP_OBJECT_NAME_LEN              32U

#define OPP_SDP_RECORD_DATA_SIZE         512U

#define OPP_MAX_NUM_OBJECTS              5U
#define OPP_MAX_OBJECT_TYPE_LEN          32U
#define OPP_MAX_OBJECT_NAME_LEN          32U


/**
 * OPP Object Type Header.Values
 */
#define OPP_TYPE_HDR_VCALENDER           "text/x-vcalender"
#define OPP_TYPE_HDR_VMSG                "text/x-vmsg"
#define OPP_TYPE_HDR_VNOTE               "text/x-vnote"

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

typedef struct _OPP_SDP_RECORDS
{
    UCHAR opp_attrib_data[OPP_SDP_RECORD_DATA_SIZE];

    UINT16 opp_len_attrib_data;

    SDP_HANDLE    opp_sdp_handle;

}OPP_SDP_RECORD;

typedef struct _OPP_INSTANCE
{
    /* OPP Handle */
    OPP_HANDLE    handle;

    /* OPP SDP record */
    OPP_SDP_RECORD opp_sdp_record;

    /* Instance current folder path */
    UCHAR path[OPP_FOLDER_NAME_LEN];

    /* Peer BD Address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Max. Tx. data size */
    UINT32 max_xchg_size;

}OPP_INSTANCE;

/**
 *  \brief Data type to specify the information used while pushing multiple objects.
 *
 *  \par Description:
 *       This structure contains object type, file name and length of the file name.
 */
typedef struct _OPP_APP_MULTIPLE_OBJECTS
{
    /**
     * object type
     */
    UCHAR   object_type[OPP_MAX_OBJECT_TYPE_LEN];

    /**
     * file name.
     */
    UCHAR   file_name[OPP_MAX_OBJECT_NAME_LEN];

    /**
     * object type length
     */
    UINT16  object_type_len;

    /**
     * file name length.
     */
    UINT16  file_name_len;

} OPP_APP_MULTIPLE_OBJECTS;



/* To Reset a OPP handle */
#define OPP_RESET_INSTANCE(hdl)    \
        (hdl)->handle = OPP_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);

/* ----------------------------------------- Functions */
void main_opp_client_operations(void);
void main_opp_server_operations(void);

API_RESULT appl_opp_client_callback
           (
               OPP_HANDLE      * opp_handle,
               UINT8             event_type,
               UINT16            event_result,
               OPP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

API_RESULT appl_opp_server_callback
           (
               OPP_HANDLE      * opp_handle,
               UINT8             event_type,
               UINT16            event_result,
               OPP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

void appl_opp_ce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void opp_client_print_appl_instances (void);
void opp_server_print_appl_instances (void);

#endif  /* OPP */

#endif /* _H_APPL_OPP_ */

