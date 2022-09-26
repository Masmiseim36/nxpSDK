
/**
 *  \file appl_sm.h
 *
 *  Header file for SM Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SM_
#define _H_APPL_SM_

/* ----------------------------------------- Header File Inclusion */
#include "BT_sm_api.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Macros */


/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Function Declarations */
void main_sm_operations(void);

API_RESULT appl_sm_service_cb
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UCHAR *    event_data
           );

API_RESULT appl_sm_ui_notify_cb
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UCHAR *    event_data
           );


#endif /* _H_APPL_SM_ */


