
/**
 *  \file write_task_pl.c
 *
 *  This file contains source code for the platform specific portions of
 *  Write Task.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* -------------------------------------- Header File Inclusion */
#include "write_task_pl.h"


/* -------------------------------------- External Global Variables */


/* -------------------------------------- Exported Global Variables */
/** Write Task Mutex */
BT_DEFINE_MUTEX (wt_mutex)

/** Write Task Conditional Variable for Waiting/Signalling */
BT_DEFINE_COND (wt_cv)


/* -------------------------------------- Staic Global Variables */
/* Write Task ID */
DECL_STATIC BT_thread_type wt_tid;


/* -------------------------------------- Functions */
/**
 *  \fn wt_init_pl
 *
 *  \par Description
 *  This function is called by Write Task during its initialization to
 *  handle platform specific operations such as initialization of the
 *  mutex & conditional variables, and, creation of the write task itself.
 *
 *  \param None
 *
 *  \return API_RESULT
 */
API_RESULT wt_init_pl (BT_THREAD_START_ROUTINE routine)
{
    BT_thread_attr_type wt_task_attr;

    /* Initialize Write Task Mutex */
    BT_MUTEX_INIT(wt_mutex, WT);

    /* Initialize Write Task CV & Mutex */
    BT_COND_INIT(wt_cv, WT);

    /* Initialize the Write Task Attributes */
    wt_task_attr.thread_name       = (DECL_CONST CHAR  *)"EtherMind WT Task";
    wt_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    wt_task_attr.thread_priority   = BT_TASK_PRIORITY;

    PL_TRC(
    "[WT] Creating Write Task ...\n");

    /* Create Write Task */
    if (BT_thread_create (&wt_tid,
                          &wt_task_attr,
                          routine,
                          NULL) < 0)
    {
        PL_ERR(
        "[WT] FATAL ERROR: FAILED to Create Write Task\n");

        return API_FAILURE;
    }

    return API_SUCCESS;
}

#ifdef BT_HAVE_SHUTDOWN
API_RESULT wt_shutdown_pl(void)
{
    /* De-Initialize Write Task Mutex */
    BT_MUTEX_DEINIT(wt_mutex, WT);

    /* De-Initialize Write Task CV & Mutex */
    BT_COND_DEINIT(wt_cv, WT);

    /* wt_tid = NULL; */
    BT_thread_delete(wt_tid);

    return API_SUCCESS;
}
#endif /* BT_HAVE_SHUTDOWN */

