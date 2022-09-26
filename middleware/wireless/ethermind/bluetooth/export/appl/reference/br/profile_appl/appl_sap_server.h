
/**
 *  \file appl_sap_server.h
 *
 *  Header file for SAP Server Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SAP_SERVER_
#define _H_APPL_SAP_SERVER_

/* ----------------------------------------- Header File Inclusion */
#include "BT_sap_api.h"
#include "appl_utils.h"
/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Macros */

/* ----------------------------------------- Structures/ Data Types */

/* ----------------------------------------- Function Declarations */
void main_sap_server_operations(void);

API_RESULT sap_server_appl_cb
           (
               UCHAR    event_type,
               UINT16   event_result,
               void   * event_data,
               UINT16   event_datalen
           );

void sap_server_appl_passwd_generator(UCHAR *passwd, UCHAR len);

#endif /* _H_APPL_SAP_SERVER_ */

