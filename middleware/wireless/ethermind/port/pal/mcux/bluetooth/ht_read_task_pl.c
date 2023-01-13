
/**
 *  \file ht_read_task_pl.c
 *
 *  This file contains source codes that platform specific for the
 *  HCI Transport Read Task/Thread.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* Module Header File Inclusion */
#include "hci_transport.h"


/* -------------------------------------- External Global Variables */


/* -------------------------------------- Exported Global Variables */


#ifndef BT_TRANSPORT_SINGLE_TASK
/* -------------------------------------- Static Global Variables */
/* Thread ID of the Read Task */
DECL_STATIC BT_thread_type ht_tid;


/* ------------------------------------- Functions */

/*
 *  Function Name:
 *    ht_read_task_create_pl
 *
 *  Description:
 *    This function is used to create the Read Task/Thread.
 *    If the Read Task/Thread is created by other means, return from here.
 *    This routine is called from the hci_transport_task_init() function.
 *
 *  Input Parameters:
 *      None.
 *
 *  Output Parameters:
 *      None.
 */
void ht_read_task_create_pl (BT_THREAD_START_ROUTINE routine)
{
    BT_thread_attr_type ht_task_attr;

    /* Initialize the Write Task Attributes */
    ht_task_attr.thread_name       = (DECL_CONST CHAR  *)"EtherMind RD Task";
    ht_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    ht_task_attr.thread_priority   = BT_TASK_PRIORITY;

    if ( BT_thread_create ( &ht_tid,
                            &ht_task_attr,
                            routine,
                            NULL ) < 0 )
    {
        PL_ERR(
        "[HT]: FAILED to Create Read Task.\n");
    }
}

#ifdef BT_HAVE_SHUTDOWN
/*
 *  Function Name:
 *    ht_read_task_shutdown_pl
 *
 *  Description:
 *    This function is used to shutdown the task created previously through
 *    ht_read_task_create_pl().
 *
 *    This routine is called from ht_read_task_start_routine() when the state
 *    of Read Task is HT_SHUTDOWN.
 *
 *  Input Parameters:
 *      None.
 *
 *  Output Parameters:
 *      API_SUCCESS or any of the error code.
 */
API_RESULT ht_read_task_shutdown_pl ( void )
{
    /* ht_tid = NULL; */
    BT_thread_delete(ht_tid);

    return API_SUCCESS;
}
#endif /* BT_HAVE_SHUTDOWN */
#endif /* BT_TRANSPORT_SINGLE_TASK */

