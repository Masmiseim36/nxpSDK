/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_THREAD_H
#define OSA_THREAD_H

/*!
 * @file    osa_thread.h
 * @brief   Contains thread function prototypes for libosa.
 */

/*!
 * Scheduler Priority
 * Enumeration of possible priorities for thread scheduling.
 */
typedef enum {
    OSA_SCHED_PRIORITY_LOW = 0,     /**< @brief Low priority */
    OSA_SCHED_PRIORITY_NORMAL,      /**< @brief Normal priority */
    OSA_SCHED_PRIORITY_HIGH,        /**< @brief High priority */
    OSA_SCHED_PRIORITY_URGENT       /**< @brief Urgent priority */
} OsaSchedPriority;

/*!
 * Scheduler Policy
 * Enumeration of scheduling policies for threads.
 * @warning OSA_SCHED_POLICY_OTHER is only valid for Linux.
 */
typedef enum {
    OSA_SCHED_POLICY_FIFO = 0,  /**< @brief FIFO policy */
    OSA_SCHED_POLICY_RR,        /**< @brief Round-robin policy */
    OSA_SCHED_POLICY_OTHER      /**< @brief Other policy (non-real-time) */
} OsaSchedPolicy;

#define OSA_MAX_THREAD_NAME_LENGTH 40

/*!
 * Thread Attributes.
 * Stores attributes for a thread.  The following should be included:
 */
typedef struct _OsaThreadAttr {
    size_t stack_size;              /**< @brief Stack size */
    OsaSchedPriority priority;      /**< @brief Scheduling priority */
    OsaSchedPolicy policy;          /**< @brief Scheduling policy */
    uint32_t rr_interval;           /**< @brief Scheduling interval */
    uint32_t core;                  /**< @brief Target core */
    char name[OSA_MAX_THREAD_NAME_LENGTH]; /**< @brief Thread name */
} OsaThreadAttr;

/*!
 * @ingroup libosa
 * @brief   Initialize a thread attribute structure.
 * @details Allocate memory for a thread attribute structure and initialize
 *          the parameters to default values.
 *
 * @param   attr Attribute structure.  This will be initialized and returned
 *          as output.
 *
 * @retval  ERRCODE_NO_ERROR     Attribute structure successfully initialized.
 * @retval  ERRCODE_OUT_OF_MEMORY      No memory available.
 */
int osa_thread_attr_init(OsaThreadAttr *attr);

/*!
 * @ingroup libosa
 * @brief   Destroy a thread attribute structure.
 * @details Free memory for a thread attribute structure.
 *
 * @param   attr Attribute structure.  The memory initialized in
 *          osa_thread_attr_init will be freed.
 */
void osa_thread_attr_destroy(OsaThreadAttr *attr);

/*!
 * @ingroup libosa
 * @brief   Set thread attribute maximum stack size
 * @details Set the stack size for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   size The maximum stack size
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or stack size
 *                       outside of limits
 */
int osa_thread_attr_set_stack_size(OsaThreadAttr *attr, size_t size);

/*!
 * @ingroup libosa
 * @brief   Get thread attribute maximum stack size
 * @details Retrieve the stack size for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   size An output parameter that will store the stack size
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or NULL size
 *                          pointer
 */
int osa_thread_attr_get_stack_size(const OsaThreadAttr *attr, size_t *size);

/*!
 * @ingroup libosa
 * @brief   Set thread attribute scheduling policy
 * @details Set the scheduling policy for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   policy The scheduling policy
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or policy
 */
int osa_thread_attr_set_sched_policy(OsaThreadAttr *attr,
                                     OsaSchedPolicy policy);

/*!
 * @ingroup libosa
 * @brief   Get thread attribute scheduling policy
 * @details Retrieve the scheduling policy for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   policy An output parameter that will store the policy
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or NULL policy
 *                      pointer
 */
int osa_thread_attr_get_sched_policy(const OsaThreadAttr *attr,
                                     OsaSchedPolicy *policy);

/*!
 * @ingroup libosa
 * @brief   Set thread attribute scheduling priority
 * @details Set the scheduling policy for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   priority The scheduling policy
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or priority
 */
int osa_thread_attr_set_sched_priority(OsaThreadAttr *attr,
                                       OsaSchedPriority priority);

/*!
 * @ingroup libosa
 * @brief   Get thread attribute scheduling priority
 * @details Retrieve the scheduling priority for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   priority An output parameter that will store the priority
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or NULL
 *                      priority pointer
 */
int osa_thread_attr_get_sched_priority(const OsaThreadAttr *attr,
                                       OsaSchedPriority *priority);

/*!
 * @ingroup libosa
 * @brief   Set thread attribute scheduling interval
 * @details Set the scheduling interval for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   interval The scheduling interval, in milliseconds
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or interval
 */
int osa_thread_attr_set_sched_interval(OsaThreadAttr *attr,
                                       uint32_t interval);

/*!
 * @ingroup libosa
 * @brief   Get thread attribute scheduling interval
 * @details Retrieve the scheduling interval for a thread attribute object.
 * @param   attr A valid thread attribute object
 * @param   interval An output parameter that will store the interval
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or NULL
 *                      interval pointer
 */
int osa_thread_attr_get_sched_interval(const OsaThreadAttr *attr,
                                       uint32_t *interval);

/*!
 * @ingroup libosa
 * @brief   Set thread name attribute
 * @details Set the name to represent a thread in the attribute structure
 * @param   attr A valid thread attribute object
 * @param   name Name for the thread (max of OSA_MAX_THREAD_NAME_LENGTH)
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid attribute object or NULL
 *                      interval pointer
 */
int osa_thread_attr_set_name(OsaThreadAttr *attr, const char *name);

/*!
 * @ingroup libosa
 * @brief   Create a new thread
 * @details Create a new thread of execution starting at the given function as
 *          an entry point.  The new thread properties will be defined in
 *          'attr'.  Passing in NULL will use default properties.  'arg' is
 *          passed as the function argument to the new thread.  The new thread
 *          may or may not be scheduled immediately.
 * @param   thread Output argument that is returned to the caller that
 *                 represents the ID of the newly-created thread.
 * @param   function The entry function for the new thread
 * @param   attr A valid thread attribute object or NULL
 * @param   arg The argument to be passed to the new thread entry point
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid input parameters.  Thread
 *                       attributes could be invalid.
 * @retval  ERRCODE_TRY_AGAIN   Exceeded system limits for threads.
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error.
*/
int osa_thread_create(OsaThread *thread, const OsaThreadAttr *attr,
                      void (*function)(void*), void *arg);

/*!
 *  @ingroup libosa
 *  @brief   Destroy a thread
 *  @param   thread The thread represented with this object will be destroyed.
 *
 *  @retval  ERRCODE_NO_ERROR          Function succeeded
 *  @retval  ERRCODE_OUT_OF_MEMORY     Out of memory
 *  @retval  ERRCODE_INVALID_ARGUMENT  Invalid input parameters.  Thread
 *                                     attributes could be invalid.
 *  @retval  ERRCODE_TRY_AGAIN         Exceed system limits for threads
 *  @retval  ERRCODE_GENERAL_ERROR     Unexpected system error
 */
int osa_thread_destroy(OsaThread thread);

/*!
 *  @ingroup libosa
 *  @brief   Obtain ID of the calling thread
 *
 *  @returns The ID of the calling thread.  This function always succeeds.
 */
OsaThread osa_thread_self(void);

/*!
 * @ingroup libosa
 * @brief   Join with a terminated thread
 * @details Wait for a given thread to terminate before continuing execution.
 * @param   thread The thread to wait on for termination.
 * @param   value  Not currently used, will be set to zero.
 * @todo    do something with value or remove? Not used in Linux or MQX
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid input parameters
 * @retval  ERRCODE_DEADLOCK  Two threads attempted to join with each other
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_thread_join(OsaThread thread, void **value);

/*!
 * @ingroup libosa
 * @brief   Yield a thread
 * @details Force the currently running thread to yield the processor.  This
 *          will cause another ready thread to be scheduled.
*/
void osa_thread_yield(void);

/*!
 * @ingroup libosa
 * @brief   Terminate a thread
 * @details Force the currently running thread to exit.  This differs from
 *          osa_exit in that the OS/process does not exit on MQX/Linux, only
 *          the calling thread.
 *
 * @param   value Data to pass back upon exit
*/
void osa_thread_exit(void *value);

/*!
 * @ingroup libosa
 * @brief   Return an ID for a thread.
 * @details Return a unique identifier for a specific thread.
 *
 * @param   thread A valid thread object
 * @param   tid    Output parameter holding the thread id
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread parameter
 */
int osa_thread_get_id(OsaThread thread, uint32_t *tid);

/*!
 * @ingroup libosa
 * @brief   Set thread scheduling policy
 * @details Set the scheduling policy for a thread.
 * @param   thread A valid thread object
 * @param   policy The scheduling policy
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or policy
 */
int osa_sched_set_policy(OsaThread thread, OsaSchedPolicy policy);

/*!
 * @ingroup libosa
 * @brief   Get thread scheduling policy
 * @details Get the scheduling policy for a thread.
 * @param   thread A valid thread object
 * @param   policy Output parameter to store the scheduling policy
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or policy
 */
int osa_sched_get_policy(OsaThread thread, OsaSchedPolicy *policy);

/*!
 * @ingroup libosa
 * @brief   Set thread scheduling priority
 * @details Set the scheduling priority for a thread.
 * @param   thread A valid thread object
 * @param   priority The scheduling priority
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or priority
 */
int osa_sched_set_priority(OsaThread thread, OsaSchedPriority priority);

/*!
 * @ingroup libosa
 * @brief   Get thread scheduling priority
 * @details Get the scheduling priority for a thread.
 * @param   thread A valid thread object
 * @param   priority Output parameter to store the scheduling priority
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or priority
 */
int osa_sched_get_priority(OsaThread thread, OsaSchedPriority *priority);

/*!
 * @ingroup libosa
 * @brief   Set thread scheduling interval
 * @details Set the scheduling interval for a thread.  This is only valid if
 *          the scheduling policy of the thread is set to round robin and the
 *          OS is MQX.  It's not policy to adjust RR interval on Linux.
 * @param   thread A valid thread object
 * @param   interval The scheduling interval in milliseconds.  The task will
 *          execute for this much time before yielding the processor.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or interval out of
 *                          range
 * @retval  ERRCODE_NOT_SUPPORTED  Not supported (Linux only)
 */
int osa_sched_set_rr_interval(OsaThread thread, uint32_t interval);

/*!
 * @ingroup libosa
 * @brief   Get thread scheduling interval
 * @details Get the scheduling interval for a thread.
 * @param   thread A valid thread object
 * @param   interval Output parameter used to store the scheduling interval
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or interval out of
 *                          range
 * @retval  ERRCODE_NOT_SUPPORTED  Not supported (Linux only)
 */
int osa_sched_get_rr_interval(OsaThread thread, uint32_t *interval);

/*!
 * @ingroup libosa
 * @brief   Get thread name
 * @details Get the thread name string for a thread.
 * @param   thread A valid thread object
 * @param   name name string
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or invalid name
 * @retval  ERRCODE_NOT_FOUND  No name string (MQX only)
 */
int osa_thread_get_name(OsaThread thread, char **name);

/*!
 * @ingroup libosa
 * @brief   Set thread name
 * @details Set the thread name string for a thread.
 * @param   thread A valid thread object
 * @param   name name string
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid thread object or invalid name
 * @retval  ERRCODE_NOT_FOUND  No name string (MQX only)
 */
int osa_thread_set_name(OsaThread thread, const char *name);

#endif

