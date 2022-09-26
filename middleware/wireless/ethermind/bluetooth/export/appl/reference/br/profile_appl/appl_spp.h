
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


/* ----------------------------------------- Function Declarations */

/* Main */
void main_spp_operations (void);
void spp_write_data(void);
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
#endif /* _H_APPL_SPP_ */

