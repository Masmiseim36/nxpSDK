
/**
 *  \file rfcomm.h
 *
 *  Module Header File for RFCOMM.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_RFCOMM_
#define _H_RFCOMM_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"

/* --------------------------------------------- API Declarations */

/* RFCOMM Initialization and Shutdown related APIs */
void em_rfcomm_init ( void );

#ifdef BT_HAVE_SHUTDOWN
void em_rfcomm_shutdown (void);
#endif /* BT_HAVE_SHUTDOWN */
void rfcomm_bt_init ( void );
void rfcomm_bt_shutdown ( void );

/* RFCOMM SM Access Response */
API_RESULT rfcomm_sm_access_response
           (
               /* IN */  DEVICE_HANDLE    * handle,
               /* IN */  UCHAR              server_ch,
               /* IN */  UINT32             rfcomm_data,
               /* IN */  UINT16             result
           );

#endif /* _H_RFCOMM_ */


