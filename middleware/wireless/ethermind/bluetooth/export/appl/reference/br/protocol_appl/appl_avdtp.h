
/**
 *  \file appl_avdtp.h
 *
 *  Header File for AVDTP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_AVDTP_
#define _H_APPL_AVDTP_

#include "BT_common.h"
#include "BT_avdtp_api.h"

#include "l2cap.h"
#include "avdtp.h"

#define APPL_MAX_AVDTP_HANDLES          3U

void main_avdtp_operations(void);

void appl_avdtp_init_handles ( void );
void appl_avdtp_show_handles ( void );
void appl_avdtp_choose_handle (UCHAR *retval);
void appl_avdtp_set_handle_params ( void );

void appl_avdtp_sep_register ( void );
void appl_avdtp_connect_req ( void );
void appl_avdtp_disconnect_req ( void );
void appl_avdtp_set_configuration ( void );
void appl_avdtp_get_configuration ( void );

void appl_avdtp_discover ( void );
void appl_avdtp_get_cap ( void );
void appl_avdtp_open ( void );
void appl_avdtp_suspend ( void );
void appl_avdtp_start ( void );
void appl_avdtp_close ( void );
void appl_avdtp_abort ( void );
void appl_avdtp_write ( void );
void appl_avdtp_reconfigure ( void );

API_RESULT av_notify_cb
           ( UCHAR  event_type,
             UINT16 event_result,
             AVDTP_HANDLE *avdtp_handle,
             void  *event_data,
             UINT16 event_datalen );


#endif /* _H_APPL_AVDTP_ */

