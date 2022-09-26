
/**
 *  \file appl_rfcomm.h
 *
 *  Header File for RFCOMM Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_RFCOMM_
#define _H_APPL_RFCOMM_

/* ----------------------------------------- Header File Inclusion */
#include "BT_rfcomm_api.h"
#include "l2cap.h"

#include "BT_fops.h"

#ifdef RFCOMM_ENABLE_SUPPORT_APIS
#include "rfcomm_support.h"
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */


/* ----------------------------------------- Global Definitions */
#define APPL_RFCOMM_FILE_NOOP                   0U
#define APPL_RFCOMM_FILE_RECV                   1U
#define APPL_RFCOMM_FILE_SEND                   2U
#define APPL_RFCOMM_WRITE_BUF_SIZE              1500U
#define APPL_RFCOMM_FILE_NAME_MAX_LEN           100U
#define APPL_RFCOMM_DEV_NAME_MAX_LEN            100U
#define APPL_RFCOMM_DUMP_CHARS_MAX_LENGTH       8U


/* ----------------------------------------- Structures/Data Types */
typedef struct
{
    char rfcomm_send_file_name [APPL_RFCOMM_FILE_NAME_MAX_LEN];
    BT_time_type start_time;
    BT_time_type end_time;
    unsigned int bytes_send;

} APPL_RFCOMM_SEND_FILE_STAT;


/* ----------------------------------------- Macros */


/* ----------------------------------------- Function Declarations */
void main_rfcomm_operations(void);

/* RFCOMM API Handlers */
/* void appl_rfcomm_register (void); */

void appl_rfcomm_open (void);
void appl_rfcomm_close (void);
void appl_rfcomm_accept (void);
void appl_rfcomm_accept_cancel (void);

void appl_rfcomm_write (void);

void appl_rfcomm_recv_file (void);
void appl_rfcomm_send_file (void);
void appl_rfcomm_stop_recv_file (void);
void appl_print_file_transfer_stat (void);

void appl_rfcomm_session_open (void);
void appl_rfcomm_session_close (void);
void appl_rfcomm_dlc_open (void);
void appl_rfcomm_dlc_close (void);

void appl_rfcomm_session_fc_off (void);
void appl_rfcomm_session_fc_on (void);
void appl_rfcomm_session_test (void);

void appl_rfcomm_send_dlc_pn (void);
void appl_rfcomm_send_dlc_rpn (void);
void appl_rfcomm_send_dlc_msc (void);
void appl_rfcomm_send_dlc_rls (void);
void appl_rfcomm_enable_credit_tx(void);
void appl_rfcomm_disable_credit_tx(void);

/* The RFCOMM Callback Handler */
API_RESULT appl_rfcomm_callback
           (
               UCHAR   event_type,
               RFCOMM_HANDLE *  handle,
               UINT16  result,
               UCHAR * data,
               UINT16  datalen
           );

/* Utility Functions */
void appl_rfcomm_print_rpn (RFCOMM_RPN *rpn);
void appl_rfcomm_print_handle (RFCOMM_HANDLE *handle);

#endif /* _H_APPL_RFCOMM_ */

