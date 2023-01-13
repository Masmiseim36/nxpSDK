
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
/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup  write_task_module  Write Task
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Write Task module to the Application and other upper layers of the stack.
 *
 */
/**
 * \defgroup  write_task_defines Defines
 * \{
 * Describes defines for Write Task module.
 */
/**
 * \defgroup storage_constants Constants
 * \{
 * Describes Constants defined by the module.
 */

/**
 *  \addtogroup write_task_defines
 *  Write Task Queue Types - in the order of decreasing priority
 *  \{
 */
#ifndef WT_HAVE_USER_MEDIA_QUEUE
#define WRITE_TASK_INTERNAL_QUEUE       0x00U /**< Queue Type - Internal */
#define WRITE_TASK_USER_API_QUEUE       0x01U /**< Queue Type - User API */
#define WRITE_TASK_USER_DATA_QUEUE      0x02U /**< Queue Type - User Data */
#else /* WT_HAVE_USER_MEDIA_QUEUE */
#define WRITE_TASK_USER_MEDIA_QUEUE     0x00U /**< Queue Type - User Media */
#define WRITE_TASK_INTERNAL_QUEUE       0x01U /**< Queue Type - Internal */
#define WRITE_TASK_USER_API_QUEUE       0x02U /**< Queue Type - User API */
#define WRITE_TASK_USER_DATA_QUEUE      0x03U /**< Queue Type - User Data */
#endif /* WT_HAVE_USER_MEDIA_QUEUE */
/** \} */
/** \} */
/** \} */
/* ----------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup write_task_api API Definitions
 * \{
 * Describes Write Task API definitions defined by the module.
 */
/**
 *  \brief To initialize globals used in Write Task.
 *
 *  \par Description
 *       This function initializes the globals used in Write Task.
 *       and calls a function which creates the WT thread.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT em_write_task_init ( void );

#ifdef BT_HAVE_SHUTDOWN
/**
 *  \brief To de-initialize globals used in Write Task.
 *
 *  \par Description
 *       This function de-initializes the globals used in Write Task.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT em_write_task_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \brief To activate Write Task.
 *
 *  \par Description
 *       This function wakes up the WT inorder to enable it to
 *       find out if it can service any of the populated queues.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT write_task_bt_init ( void );

/**
 *  \brief To shutdown Write Task.
 *
 *  \par Description
 *       This function sets the state of Write Task to shutdown state.
 *
 *       This halts Write Task from servicing the queued elements.
 *       However, the task itself is not terminated but brought to a
 *       state where it can start functioning once it is signalled.
 *       As part of this, the queues which holds the bottom halves are
 *       reset.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT write_task_bt_shutdown ( void );

/**
 *  \brief To enqueue to Write Task.
 *
 *  \par Description
 *  This function enqueues/populates the bottom halves and the data to
 *  bottom halves into specified Write Task queue.
 *
 *  \param [in] q_type
 *         The type of the WT Queue:
 *         - WRITE_TASK_INTERNAL_QUEUE
 *         - WRITE_TASK_USER_API_QUEUE
 *         - WRITE_TASK_USER_DATA_QUEUE
 *  \param [in] bh_func
 *         Pointer to the Write Task Bottom Half function
 *
 *  \param [in] data
 *         Pointer to the data to be sent to the bottom half
 *
 *  \param [in] datalen
 *         Length of the data to be sent to bottom half.
 *
 *  \return API_RESULT
 */
API_RESULT write_task_enqueue
           (
               /* Queue Type */
               UCHAR   q_type,
               /* Bottom Half Function Pointer */
               void    (* bh_func) ( void *args, UINT16 args_len),
               /* Data to be passed to Bottom Half */
               void    * data_param,
               /* Data length */
               UINT16  datalen
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_WRITE_TASK_ */

