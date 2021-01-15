
/**
 *  \file write_task_extern_pl.h
 *
 *  Platform specific Header File for Write Task module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_WRITE_TASK_EXTERN_PL_
#define _H_WRITE_TASK_EXTERN_PL_

/* -------------------------------------- External Global Variables */
/** Write Task Mutex */
BT_DEFINE_MUTEX_TYPE (extern, wt_mutex)

/** Write Task Conditional Variable for Waiting/Signalling */
BT_DEFINE_COND_TYPE (extern, wt_cv)

#endif /* _H_WRITE_TASK_EXTERN_PL_ */

