
/**
 *  \file appl_spp.h
 *
 *  Header file for SPP Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SPP_
#define _H_APPL_SPP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_spp_api.h"
#include "db_gen.h"

#include "BT_fops.h"

/* ----------------------------------------- Global Definitions */
#define SPP_HANDLE_INVALID                   0xFFU
#define APPL_SPP_FILE_NAME_MAX_LEN           100U

/* ----------------------------------------- Macros */


/* ----------------------------------------- Structures/ Data Types */

typedef enum
{
    SPP_STARTED,
    SPP_CONNECTED,
    SPP_DATA_TRANSFER
}APPL_SPP_HANDLE_STATUS;

typedef struct _APPL_SPP_CONN_STATUS
{
    /**
     * Stores the BDADDR of device connected.
     */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /**
     * Connection Status:
     *  - 0->started
     *  - 1->connected,
     *  - 2->data transfer
     */
    APPL_SPP_HANDLE_STATUS status;

    /* spp_handle */
    SPP_HANDLE spp_handle;

    /* Number of bytes in each SPP write */
    UINT16 appl_nbytes;
    /* Number of times the SPP Write to be performed */
    UINT16 appl_loop;
    /* Counter to maintain the number of SPP Write/Read has completed */
    UINT16 appl_count;

}APPL_SPP_CONN_STATUS;

/* ----------------------------------------- Function Declarations */

/* Main */
void main_spp_operations (void);
void spp_write_data(SPP_HANDLE spp_handle);
/* SPP callback */
API_RESULT spp_application_callback
           (
               /* IN */  SPP_HANDLE       spp_handle,
               /* IN */  SPP_EVENTS       spp_event,
               /* IN */  API_RESULT       status,
               /* IN */  void           * data,
               /* IN */  UINT16           data_length
           );

/* Application SDP Callback */
void appl_spp_sdp_cb
     (
         UCHAR    command,
         UCHAR  * data,
         UINT16   length,
         UINT16   status
     );

/* Display the status of spp_handle instances */
UCHAR appl_spp_display_conn_status(void);
#endif /* _H_APPL_SPP_ */


