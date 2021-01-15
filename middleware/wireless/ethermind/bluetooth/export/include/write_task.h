
/**
 *  \file write_task.h
 *
 *  Module Header File for Write Task module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_WRITE_TASK_
#define _H_WRITE_TASK_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------------- Type Definition */
/* The Write Task Queues - in the order of decreasing priority */
#define WRITE_TASK_INTERNAL_QUEUE       0x00
#define WRITE_TASK_USER_API_QUEUE       0x01
#define WRITE_TASK_USER_DATA_QUEUE      0x02


/* ----------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** Init routine */
API_RESULT em_write_task_init ( void );

/** Activating routine */
API_RESULT write_task_bt_init ( void );

/** Shutdown routine */
API_RESULT write_task_bt_shutdown ( void );

/** Enqueue routine */
API_RESULT write_task_enqueue
           (
               /* Queue Type */
               UCHAR   q_type,
               /* Bottom Half Function Pointer */
               void    (* bh_func) ( void *, UINT16 ),
               /* Data to be passed to Bottom Half */
               void    * data_param,
               /* Data length */
               UINT16  datalen
           );

#ifdef __cplusplus
};
#endif

#endif /* _H_WRITE_TASK_ */

