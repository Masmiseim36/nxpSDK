
/**
 *  \file appl_avctp.h
 *
 *  Header File for AVCTP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_AVCTP_
#define _H_APPL_AVCTP_

#include "BT_common.h"
#include "BT_avctp_api.h"

#include "l2cap.h"
#include "avctp.h"

#define APPL_MAX_AVCTP_HANDLES          3U
#define AVRCP_PROFILE_ID                0x110DU

void main_avctp_operations(void);

void appl_avctp_init_handles (void);
void appl_avctp_show_handles (void);
void appl_avctp_choose_handle (UCHAR * retval);
/* void appl_avctp_set_handle_params (void); */

void appl_avctp_connect_req (void);
void appl_avctp_disconnect_req (void);
void appl_avctp_profile_register (void);
void appl_avctp_profile_deregister (void);
void appl_avctp_send_message (void);
void appl_avctp_test_menu (int choice);

API_RESULT avctp_ntf_cb
           ( UCHAR  event_type,
             UINT16 event_result,
             AVCTP_HANDLE *handle,
             void  *event_data,
             UINT16 event_datalen );


#endif /* _H_APPL_AVCTP_ */

