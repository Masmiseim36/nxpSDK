
/**
 *  \file appl_ctn.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CTN_
#define _H_APPL_CTN_

/* --------------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_ctn_api.h"
#include "BT_fops.h"
#include "ctn_pl.h"

/* --------------------------------------------- Global Definitions */

#define CTN_CLIENT_NUM_INSTANCES         2U /* 1 */
#define CTN_SERVER_NUM_INSTANCES         2U /* 1 */

#define APPL_CTN_MAX_TAG_LEN             32U

#define CTN_SDP_RECORD_DATA_SIZE         500U

/* Max. file object name length */
#define CTN_MAX_FILE_OBJ_NAME_LEN        190U
/* --------------------------------------------- Structures/Data Types */

typedef struct _CTN_SDP_RECORDS
{
    UCHAR ctn_attrib_data[CTN_SDP_RECORD_DATA_SIZE];

    UINT16 ctn_len_attrib_data;

    SDP_HANDLE    ctn_sdp_handle;

}CTN_SDP_RECORD;

typedef struct _CTN_INSTANCE
{
    /* CTN Handle */
    CTN_HANDLE    handle;

    /* CTN SDP record */
    CTN_SDP_RECORD ctn_sdp_record;

    /* Peer BD Address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

} CTN_INSTANCE;

typedef struct _CTN_CAS_INSTANCE
{
    /* CTN Instance */
    CTN_INSTANCE instance;

    /* CNS Instance ID reference */
    UCHAR cns_id;

} CTN_CAS_INSTANCE;

/* To Reset a CTN handle */
#define CTN_RESET_INSTANCE(hdl)    \
        (hdl)->handle = CTN_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);


/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
#ifdef CTN_CCE

API_RESULT appl_ctn_cce_callback
           (
               CTN_HANDLE_INFO * ctn_handle,
               UINT8             event_type,
               UINT16            event_result,
               CTN_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

void appl_ctn_cce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void ctn_cce_print_appl_instances (void);


void main_ctn_cce_operations (void);

#endif /* CTN_CCE */

#ifdef CTN_CSE

API_RESULT appl_ctn_cse_callback
           (
               CTN_HANDLE_INFO  * ctn_handle,
               UINT8              event_type,
               UINT16             event_result,
               CTN_HEADERS      * event_data,
               UINT16             event_datalen
           );

void appl_ctn_cse_cns_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void ctn_cse_print_appl_instances (void);

void main_ctn_cse_operations (void);

/**
 * This function will update the CTN object list information
 * in 'event', 'notes' & 'tasks' folders
 */
API_RESULT appl_ctn_cse_update_folders(void);

/**
 * This function is to get CTN object name and object folder type for
 * a given CTN handle.
 */
API_RESULT appl_ctn_cse_get_object_name_for_handle
           (
               /* IN */  UCHAR  *obj_handle,
               /* IN */  UCHAR   obj_handle_len,
               /* OUT */ UCHAR  *obj_name,
               /* OUT */ UCHAR  *obj_folder_type
           );

#endif /* CTN_CSE */

#endif /* _H_APPL_CTN_ */

