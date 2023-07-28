
/**
 *  \file appl_sap_client.h
 *
 *  Header file for SAP Client Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SAP_CLIENT_
#define _H_APPL_SAP_CLIENT_

/* ----------------------------------------- Header File Inclusion */
#include "BT_sap_api.h"

/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Macros */

/* ----------------------------------------- Structures/ Data Types */

/* ----------------------------------------- Function Declarations */
void main_sap_client_operations(void);

API_RESULT appl_sap_client_appl_cb
           (
               UCHAR    event_type,
               UINT16   event_result,
               void   * event_data,
               UINT16   event_datalen
           );

void appl_sap_client_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

API_RESULT sap_client_appl_get_acl_connection (UCHAR * indx);

#endif /* _H_APPL_SAP_CLIENT_ */

