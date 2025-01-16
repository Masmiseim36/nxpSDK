/*
 * Copyright 2023 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>

static sys_slist_t threads = SYS_SLIST_STATIC_INIT(&threads);

const char k_thread_alloc_mem_tag[4] = {0xA5,0xAA,0x55,0x5A};

struct k_thread_alloc_mem
{
    char header[4];
    char buffer[0];
};

/**
 * @brief Dynamically allocate a thread stack.
 *
 * Relevant stack creation flags include:
 * - @ref K_USER allocate a userspace thread (requires `CONFIG_USERSPACE=y`)
 *
 * @param size Stack size in bytes.
 * @param flags Stack creation flags, or 0.
 *
 * @retval the allocated thread stack on success.
 * @retval NULL on failure.
 *
 * @see CONFIG_DYNAMIC_THREAD
 */
k_thread_stack_t *k_thread_stack_alloc(size_t size, int flags)
{
    struct k_thread_alloc_mem *mem = NULL;

    if (size > 0)
    {
        mem = (struct k_thread_alloc_mem *)pvPortMalloc(size + sizeof(*mem));
        if (NULL != mem)
        {
            memcpy(&mem->header[0], &k_thread_alloc_mem_tag[0], sizeof(mem->header));
        }
    }

    return (k_thread_stack_t *)&mem->buffer[0];
}

/**
 * @brief Free a dynamically allocated thread stack.
 *
 * @param stack Pointer to the thread stack.
 *
 * @retval 0 on success.
 * @retval -EBUSY if the thread stack is in use.
 * @retval -EINVAL if @p stack is invalid.
 * @retval -ENOSYS if dynamic thread stack allocation is disabled
 *
 * @see CONFIG_DYNAMIC_THREAD
 */
int k_thread_stack_free(k_thread_stack_t *stack)
{
    struct k_thread_alloc_mem *mem = CONTAINER_OF(stack, struct k_thread_alloc_mem, buffer);

    if (0 == memcmp(&mem->header[0], &k_thread_alloc_mem_tag[0], sizeof(mem->header)))
    {
        vPortFree (mem);
        return 0;
    }
    else
    {
        return -EINVAL;
    }
}

static void _k_thread_function_wrap( void * pvParameters )
{
    struct k_thread *thread = (struct k_thread *)pvParameters;

    if ((NULL != thread) && (NULL != thread->entry))
    {
        thread->entry(thread->p1, thread->p2, thread->p3);
    }
}
/**
 * @brief Create a thread.
 *
 * This routine initializes a thread, then schedules it for execution.
 *
 * The new thread may be scheduled for immediate execution or a delayed start.
 * If the newly spawned thread does not have a delayed start the kernel
 * scheduler may preempt the current thread to allow the new thread to
 * execute.
 *
 * Thread options are architecture-specific, and can include K_ESSENTIAL,
 * K_FP_REGS, and K_SSE_REGS. Multiple options may be specified by separating
 * them using "|" (the logical OR operator).
 *
 * Stack objects passed to this function must be originally defined with
 * either of these macros in order to be portable:
 *
 * - K_THREAD_STACK_DEFINE() - For stacks that may support either user or
 *   supervisor threads.
 * - K_KERNEL_STACK_DEFINE() - For stacks that may support supervisor
 *   threads only. These stacks use less memory if CONFIG_USERSPACE is
 *   enabled.
 *
 * The stack_size parameter has constraints. It must either be:
 *
 * - The original size value passed to K_THREAD_STACK_DEFINE() or
 *   K_KERNEL_STACK_DEFINE()
 * - The return value of K_THREAD_STACK_SIZEOF(stack) if the stack was
 *   defined with K_THREAD_STACK_DEFINE()
 * - The return value of K_KERNEL_STACK_SIZEOF(stack) if the stack was
 *   defined with K_KERNEL_STACK_DEFINE().
 *
 * Using other values, or sizeof(stack) may produce undefined behavior.
 *
 * @param new_thread Pointer to uninitialized struct k_thread
 * @param stack Pointer to the stack space.
 * @param stack_size Stack size in bytes.
 * @param entry Thread entry function.
 * @param p1 1st entry point parameter.
 * @param p2 2nd entry point parameter.
 * @param p3 3rd entry point parameter.
 * @param prio Thread priority.
 * @param options Thread options.
 * @param delay Scheduling delay, or K_NO_WAIT (for no delay).
 *
 * @return ID of new thread.
 *
 */
k_tid_t k_thread_create(struct k_thread *new_thread,
				  k_thread_stack_t *stack,
				  size_t stack_size,
				  k_thread_entry_t entry,
				  void *p1, void *p2, void *p3,
				  int prio, uint32_t options, k_timeout_t delay)
{
    uint32_t add = (uint32_t)entry;
    char buffer[configMAX_TASK_NAME_LEN + 1] = "INVALID";
    size_t ret;

    ret = bin2hex((const uint8_t *)&add, sizeof(add), buffer, sizeof(buffer) - 1);
    assert(ret > 0);
    (void)ret;
    if (0 == ret)
    {
        /* TODO: */
    }
    new_thread->p1 = p1;
    new_thread->p2 = p2;
    new_thread->p3 = p3;
    new_thread->entry = entry;
    new_thread->options = options;

    vTaskSuspendAll();
    new_thread->handle = xTaskCreateStatic(_k_thread_function_wrap, buffer, stack_size/sizeof(StackType_t), new_thread, prio, (StackType_t *)stack, &new_thread->task_buffer);

    if (NULL != new_thread->handle)
    {
        sys_slist_append(&threads, &new_thread->node);

        if (K_TIMEOUT_EQ(delay, K_FOREVER))
        {
            vTaskSuspend(new_thread->handle);
        }
        else
        {
            vTaskDelay(delay);
        }
    }

    if (!xTaskResumeAll())
    {
        taskYIELD();
    }

    return new_thread;
}

#if 0
/**
 * @brief Drop a thread's privileges permanently to user mode
 *
 * This allows a supervisor thread to be re-used as a user thread.
 * This function does not return, but control will transfer to the provided
 * entry point as if this was a new user thread.
 *
 * The implementation ensures that the stack buffer contents are erased.
 * Any thread-local storage will be reverted to a pristine state.
 *
 * Memory domain membership, resource pool assignment, kernel object
 * permissions, priority, and thread options are preserved.
 *
 * A common use of this function is to re-use the main thread as a user thread
 * once all supervisor mode-only tasks have been completed.
 *
 * @param entry Function to start executing from
 * @param p1 1st entry point parameter
 * @param p2 2nd entry point parameter
 * @param p3 3rd entry point parameter
 */
FUNC_NORETURN void k_thread_user_mode_enter(k_thread_entry_t entry,
						   void *p1, void *p2,
						   void *p3)
{
    __ASSERT_NO_MSG(0);
}
#endif

#if defined(CONFIG_INIT_STACKS) && defined(CONFIG_THREAD_STACK_INFO)
/**
 * @brief Obtain stack usage information for the specified thread
 *
 * User threads will need to have permission on the target thread object.
 *
 * Some hardware may prevent inspection of a stack buffer currently in use.
 * If this API is called from supervisor mode, on the currently running thread,
 * on a platform which selects @kconfig{CONFIG_NO_UNUSED_STACK_INSPECTION}, an
 * error will be generated.
 *
 * @param thread Thread to inspect stack information
 * @param unused_ptr Output parameter, filled in with the unused stack space
 *	of the target thread in bytes.
 * @return 0 on success
 * @return -EBADF Bad thread object (user mode only)
 * @return -EPERM No permissions on thread object (user mode only)
 * #return -ENOTSUP Forbidden by hardware policy
 * @return -EINVAL Thread is uninitialized or exited (user mode only)
 * @return -EFAULT Bad memory address for unused_ptr (user mode only)
 */
int k_thread_stack_space_get(const struct k_thread *thread,
				       size_t *unused_ptr)
{
    return (int)uxTaskGetStackHighWaterMark((NULL == thread) ? NULL : thread->handle);
}
#endif

#if (CONFIG_HEAP_MEM_POOL_SIZE > 0)
/**
 * @brief Assign the system heap as a thread's resource pool
 *
 * Similar to k_thread_heap_assign(), but the thread will use
 * the kernel heap to draw memory.
 *
 * Use with caution, as a malicious thread could perform DoS attacks on the
 * kernel heap.
 *
 * @param thread Target thread to assign the system heap for resource requests
 *
 */
void k_thread_system_pool_assign(struct k_thread *thread)
{
    __ASSERT_NO_MSG(0);
}
#endif /* (CONFIG_HEAP_MEM_POOL_SIZE > 0) */

static k_ticks_t k_thread_get_tick(void)
{
    return (k_ticks_t)k_uptime_ticks();
}

/**
 * @brief Sleep until a thread exits
 *
 * The caller will be put to sleep until the target thread exits, either due
 * to being aborted, self-exiting, or taking a fatal error. This API returns
 * immediately if the thread isn't running.
 *
 * This API may only be called from ISRs with a K_NO_WAIT timeout,
 * where it can be useful as a predicate to detect when a thread has
 * aborted.
 *
 * @param thread Thread to wait to exit
 * @param timeout upper bound time to wait for the thread to exit.
 * @retval 0 success, target thread has exited or wasn't running
 * @retval -EBUSY returned without waiting
 * @retval -EAGAIN waiting period timed out
 * @retval -EDEADLK target thread is joining on the caller, or target thread
 *                  is the caller
 */
int k_thread_join(struct k_thread *thread, k_timeout_t timeout)
{
    eTaskState state;
    int ret;
    uint32_t current;
    uint32_t start = k_uptime_get_32();

    do
    {
        state = eTaskGetState((NULL == thread) ? NULL : thread->handle);
        if (eRunning == state)
        {
            ret = -EDEADLK;
            break;
        }
        else if (eBlocked == state)
        {
            vTaskDelete((NULL == thread) ? NULL : thread->handle);
        }
        else if (eReady == state)
        {
            vTaskDelete((NULL == thread) ? NULL : thread->handle);
        }
        else if (eSuspended == state)
        {
            vTaskDelete((NULL == thread) ? NULL : thread->handle);
        }
        else if (eDeleted == state)
        {
            ret = 0;
            break;
        }
        else
        {
            ret = 0;
            break;
        }
        if (k_is_in_isr())
        {
            ret = -EBUSY;
            break;
        }
        vTaskDelay(timeout);
        current = k_uptime_get_32();
        if ((current - start) > timeout)
        {
            ret = -EAGAIN;
            break;
        }
    } while (true);

    return ret;
}

/**
 * @brief Put the current thread to sleep.
 *
 * This routine puts the current thread to sleep for @a duration,
 * specified as a k_timeout_t object.
 *
 * @note if @a timeout is set to K_FOREVER then the thread is suspended.
 *
 * @param timeout Desired duration of sleep.
 *
 * @return Zero if the requested time has elapsed or the number of milliseconds
 * left to sleep, if thread was woken up by \ref k_wakeup call.
 */
int32_t k_sleep(k_timeout_t timeout)
{
    uint32_t current;
    uint32_t start = k_uptime_get_32();

    vTaskDelay(timeout);
    current = k_uptime_get_32();
    if ((current - start) > timeout)
    {
        return 0;
    }
    else
    {
        return (int32_t)(timeout - (current - start));
    }
}

/**
 * @brief Put the current thread to sleep with microsecond resolution.
 *
 * This function is unlikely to work as expected without kernel tuning.
 * In particular, because the lower bound on the duration of a sleep is
 * the duration of a tick, @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC} must be
 * adjusted to achieve the resolution desired. The implications of doing
 * this must be understood before attempting to use k_usleep(). Use with
 * caution.
 *
 * @param us Number of microseconds to sleep.
 *
 * @return Zero if the requested time has elapsed or the number of microseconds
 * left to sleep, if thread was woken up by \ref k_wakeup call.
 */
int32_t k_usleep(int32_t us)
{
    return k_sleep((us+1000)/1000);
}

/**
 * @brief Cause the current thread to busy wait.
 *
 * This routine causes the current thread to execute a "do nothing" loop for
 * @a usec_to_wait microseconds.
 *
 * @note The clock used for the microsecond-resolution delay here may
 * be skewed relative to the clock used for system timeouts like
 * k_sleep().  For example k_busy_wait(1000) may take slightly more or
 * less time than k_sleep(K_MSEC(1)), with the offset dependent on
 * clock tolerances.
 *
 * @note In case when @kconfig{CONFIG_SYSTEM_CLOCK_SLOPPY_IDLE} and
 * @kconfig{CONFIG_PM} options are enabled, this function may not work.
 * The timer/clock used for delay processing may be disabled/inactive.
 */
void k_busy_wait(uint32_t usec_to_wait)
{
    (void)k_usleep(usec_to_wait);
}

/**
 * @brief Check whether it is possible to yield in the current context.
 *
 * This routine checks whether the kernel is in a state where it is possible to
 * yield or call blocking API's. It should be used by code that needs to yield
 * to perform correctly, but can feasibly be called from contexts where that
 * is not possible. For example in the PRE_KERNEL initialization step, or when
 * being run from the idle thread.
 *
 * @return True if it is possible to yield in the current context, false otherwise.
 */
bool k_can_yield(void)
{
    return true;
}

/**
 * @brief Yield the current thread.
 *
 * This routine causes the current thread to yield execution to another
 * thread of the same or higher priority. If there are no other ready threads
 * of the same or higher priority, the routine returns immediately.
 */
void k_yield(void)
{
    taskYIELD();
}

/**
 * @brief Wake up a sleeping thread.
 *
 * This routine prematurely wakes up @a thread from sleeping.
 *
 * If @a thread is not currently sleeping, the routine has no effect.
 *
 * @param thread ID of thread to wake.
 */
void k_wakeup(k_tid_t thread)
{
    if (NULL == thread)
    {
        return;
    }
    (void)xTaskAbortDelay(thread->handle);
}

/**
 * @brief Query thread ID of the current thread.
 *
 * This unconditionally queries the kernel via a system call.
 *
 * @note Use k_current_get() unless absolutely sure this is necessary.
 *       This should only be used directly where the thread local
 *       variable cannot be used or may contain invalid values
 *       if thread local storage (TLS) is enabled. If TLS is not
 *       enabled, this is the same as k_current_get().
 *
 * @return ID of current thread.
 */
k_tid_t k_sched_current_thread_query(void)
{
    TaskHandle_t id;
    struct k_thread * t;
    struct k_thread * temp;

    id = xTaskGetCurrentTaskHandle();

    SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&threads, t, temp, node)
    {
        if (id == t->handle)
        {
            return t;
        }
    }
    return NULL;
}

/**
 * @brief Abort a thread.
 *
 * This routine permanently stops execution of @a thread. The thread is taken
 * off all kernel queues it is part of (i.e. the ready queue, the timeout
 * queue, or a kernel object wait queue). However, any kernel resources the
 * thread might currently own (such as mutexes or memory blocks) are not
 * released. It is the responsibility of the caller of this routine to ensure
 * all necessary cleanup is performed.
 *
 * After k_thread_abort() returns, the thread is guaranteed not to be
 * running or to become runnable anywhere on the system.  Normally
 * this is done via blocking the caller (in the same manner as
 * k_thread_join()), but in interrupt context on SMP systems the
 * implementation is required to spin for threads that are running on
 * other CPUs.  Note that as specified, this means that on SMP
 * platforms it is possible for application code to create a deadlock
 * condition by simultaneously aborting a cycle of threads using at
 * least one termination from interrupt context.  Zephyr cannot detect
 * all such conditions.
 *
 * @param thread ID of thread to abort.
 */
void k_thread_abort(k_tid_t thread)
{
	if (thread != NULL) {
		if (thread->handle == NULL) {
			return;
		}
	}
	vTaskDelete((NULL == thread) ? NULL : thread->handle);
	if (thread != NULL) {
    		thread->handle = NULL;
	}
}

/**
 * @brief Start an inactive thread
 *
 * If a thread was created with K_FOREVER in the delay parameter, it will
 * not be added to the scheduling queue until this function is called
 * on it.
 *
 * @param thread thread to start
 */
void k_thread_start(k_tid_t thread)
{
    BaseType_t xYieldRequired;
    eTaskState state;

    if ((NULL == thread) || (NULL == thread->handle))
    {
        return;
    }

    state = eTaskGetState(thread->handle);
    if (eBlocked == state)
    {
        (void)xTaskAbortDelay(thread->handle);
    }

    if (k_is_in_isr())
    {
        xYieldRequired = xTaskResumeFromISR(thread->handle);
        portYIELD_FROM_ISR(xYieldRequired);
    }
    else
    {
        vTaskResume(thread->handle);
    }
}

#ifdef CONFIG_SYS_CLOCK_EXISTS

/**
 * @brief Get time when a thread wakes up, in system ticks
 *
 * This routine computes the system uptime when a waiting thread next
 * executes, in units of system ticks.  If the thread is not waiting,
 * it returns current system time.
 */
k_ticks_t k_thread_timeout_expires_ticks(const struct k_thread *t)
{
    __ASSERT_NO_MSG(0);
    return 0;
}

/**
 * @brief Get time remaining before a thread wakes up, in system ticks
 *
 * This routine computes the time remaining before a waiting thread
 * next executes, in units of system ticks.  If the thread is not
 * waiting, it returns zero.
 */
k_ticks_t k_thread_timeout_remaining_ticks(const struct k_thread *t)
{
    __ASSERT_NO_MSG(0);
    return 0;
}

#endif /* CONFIG_SYS_CLOCK_EXISTS */


/**
 * @brief Get a thread's priority.
 *
 * This routine gets the priority of @a thread.
 *
 * @param thread ID of thread whose priority is needed.
 *
 * @return Priority of @a thread.
 */
int k_thread_priority_get(k_tid_t thread)
{
    BaseType_t priority;

    if (k_is_in_isr())
    {
        priority = uxTaskPriorityGetFromISR((NULL == thread) ? NULL : thread->handle);
    }
    else
    {
        priority = uxTaskPriorityGet((NULL == thread) ? NULL : thread->handle);
    }
    return (int)priority;
}

/**
 * @brief Set a thread's priority.
 *
 * This routine immediately changes the priority of @a thread.
 *
 * Rescheduling can occur immediately depending on the priority @a thread is
 * set to:
 *
 * - If its priority is raised above the priority of the caller of this
 * function, and the caller is preemptible, @a thread will be scheduled in.
 *
 * - If the caller operates on itself, it lowers its priority below that of
 * other threads in the system, and the caller is preemptible, the thread of
 * highest priority will be scheduled in.
 *
 * Priority can be assigned in the range of -CONFIG_NUM_COOP_PRIORITIES to
 * CONFIG_NUM_PREEMPT_PRIORITIES-1, where -CONFIG_NUM_COOP_PRIORITIES is the
 * highest priority.
 *
 * @param thread ID of thread whose priority is to be set.
 * @param prio New priority.
 *
 * @warning Changing the priority of a thread currently involved in mutex
 * priority inheritance may result in undefined behavior.
 */
void k_thread_priority_set(k_tid_t thread, int prio)
{
    vTaskPrioritySet((NULL == thread) ? NULL : thread->handle, prio);
}

#ifdef CONFIG_SCHED_DEADLINE
/**
 * @brief Set deadline expiration time for scheduler
 *
 * This sets the "deadline" expiration as a time delta from the
 * current time, in the same units used by k_cycle_get_32().  The
 * scheduler (when deadline scheduling is enabled) will choose the
 * next expiring thread when selecting between threads at the same
 * static priority.  Threads at different priorities will be scheduled
 * according to their static priority.
 *
 * @note Deadlines are stored internally using 32 bit unsigned
 * integers.  The number of cycles between the "first" deadline in the
 * scheduler queue and the "last" deadline must be less than 2^31 (i.e
 * a signed non-negative quantity).  Failure to adhere to this rule
 * may result in scheduled threads running in an incorrect deadline
 * order.
 *
 * @note Despite the API naming, the scheduler makes no guarantees the
 * the thread WILL be scheduled within that deadline, nor does it take
 * extra metadata (like e.g. the "runtime" and "period" parameters in
 * Linux sched_setattr()) that allows the kernel to validate the
 * scheduling for achievability.  Such features could be implemented
 * above this call, which is simply input to the priority selection
 * logic.
 *
 * @note You should enable @kconfig{CONFIG_SCHED_DEADLINE} in your project
 * configuration.
 *
 * @param thread A thread on which to set the deadline
 * @param deadline A time delta, in cycle units
 *
 */
void k_thread_deadline_set(k_tid_t thread, int deadline)
{
    assert(0);
}
#endif

#ifdef CONFIG_SCHED_CPU_MASK
/**
 * @brief Sets all CPU enable masks to zero
 *
 * After this returns, the thread will no longer be schedulable on any
 * CPUs.  The thread must not be currently runnable.
 *
 * @note You should enable @kconfig{CONFIG_SCHED_CPU_MASK} in your project
 * configuration.
 *
 * @param thread Thread to operate upon
 * @return Zero on success, otherwise error code
 */
int k_thread_cpu_mask_clear(k_tid_t thread)
{
    __ASSERT_NO_MSG(0);
    return -EFAULT;
}

/**
 * @brief Sets all CPU enable masks to one
 *
 * After this returns, the thread will be schedulable on any CPU.  The
 * thread must not be currently runnable.
 *
 * @note You should enable @kconfig{CONFIG_SCHED_CPU_MASK} in your project
 * configuration.
 *
 * @param thread Thread to operate upon
 * @return Zero on success, otherwise error code
 */
int k_thread_cpu_mask_enable_all(k_tid_t thread)
{
    __ASSERT_NO_MSG(0);
    return -EFAULT;
}

/**
 * @brief Enable thread to run on specified CPU
 *
 * The thread must not be currently runnable.
 *
 * @note You should enable @kconfig{CONFIG_SCHED_CPU_MASK} in your project
 * configuration.
 *
 * @param thread Thread to operate upon
 * @param cpu CPU index
 * @return Zero on success, otherwise error code
 */
int k_thread_cpu_mask_enable(k_tid_t thread, int cpu)
{
    __ASSERT_NO_MSG(0);
    return -EFAULT;
}

/**
 * @brief Prevent thread to run on specified CPU
 *
 * The thread must not be currently runnable.
 *
 * @note You should enable @kconfig{CONFIG_SCHED_CPU_MASK} in your project
 * configuration.
 *
 * @param thread Thread to operate upon
 * @param cpu CPU index
 * @return Zero on success, otherwise error code
 */
int k_thread_cpu_mask_disable(k_tid_t thread, int cpu)
{
    __ASSERT_NO_MSG(0);
    return -EFAULT;
}

/**
 * @brief Pin a thread to a CPU
 *
 * Pin a thread to a CPU by first clearing the cpu mask and then enabling the
 * thread on the selected CPU.
 *
 * @param thread Thread to operate upon
 * @param cpu CPU index
 * @return Zero on success, otherwise error code
 */
int k_thread_cpu_pin(k_tid_t thread, int cpu)
{
    __ASSERT_NO_MSG(0);
    return -EFAULT;
}
#endif


/**
 * @brief Suspend a thread.
 *
 * This routine prevents the kernel scheduler from making @a thread
 * the current thread. All other internal operations on @a thread are
 * still performed; for example, kernel objects it is waiting on are
 * still handed to it.  Note that any existing timeouts
 * (e.g. k_sleep(), or a timeout argument to k_sem_take() et. al.)
 * will be canceled.  On resume, the thread will begin running
 * immediately and return from the blocked call.
 *
 * If @a thread is already suspended, the routine has no effect.
 *
 * @param thread ID of thread to suspend.
 */
void k_thread_suspend(k_tid_t thread)
{
    vTaskSuspend((NULL == thread) ? NULL : thread->handle);
}

/**
 * @brief Resume a suspended thread.
 *
 * This routine allows the kernel scheduler to make @a thread the current
 * thread, when it is next eligible for that role.
 *
 * If @a thread is not currently suspended, the routine has no effect.
 *
 * @param thread ID of thread to resume.
 */
void k_thread_resume(k_tid_t thread)
{
    BaseType_t xYieldRequired;

    if (NULL == thread)
    {
        return;
    }

    if (k_is_in_isr())
    {
        xYieldRequired = xTaskResumeFromISR(thread->handle);
        portYIELD_FROM_ISR(xYieldRequired);
    }
    else
    {
        vTaskResume(thread->handle);
    }
}

#if 0 /* Unsupported */
typedef void (*k_thread_timeslice_fn_t)(struct k_thread *thread, void *data);

/**
 * @brief Set time-slicing period and scope.
 *
 * This routine specifies how the scheduler will perform time slicing of
 * preemptible threads.
 *
 * To enable time slicing, @a slice must be non-zero. The scheduler
 * ensures that no thread runs for more than the specified time limit
 * before other threads of that priority are given a chance to execute.
 * Any thread whose priority is higher than @a prio is exempted, and may
 * execute as long as desired without being preempted due to time slicing.
 *
 * Time slicing only limits the maximum amount of time a thread may continuously
 * execute. Once the scheduler selects a thread for execution, there is no
 * minimum guaranteed time the thread will execute before threads of greater or
 * equal priority are scheduled.
 *
 * When the current thread is the only one of that priority eligible
 * for execution, this routine has no effect; the thread is immediately
 * rescheduled after the slice period expires.
 *
 * To disable timeslicing, set both @a slice and @a prio to zero.
 *
 * @param slice Maximum time slice length (in milliseconds).
 * @param prio Highest thread priority level eligible for time slicing.
 */
void k_sched_time_slice_set(int32_t slice, int prio)
{
    __ASSERT_NO_MSG(0);
}

/**
 * @brief Set thread time slice
 *
 * As for k_sched_time_slice_set, but (when
 * CONFIG_TIMESLICE_PER_THREAD=y) sets the timeslice for a specific
 * thread.  When non-zero, this timeslice will take precedence over
 * the global value.
 *
 * When such a thread's timeslice expires, the configured callback
 * will be called before the thread is removed/re-added to the run
 * queue.  This callback will occur in interrupt context, and the
 * specified thread is guaranteed to have been preempted by the
 * currently-executing ISR.  Such a callback is free to, for example,
 * modify the thread priority or slice time for future execution,
 * suspend the thread, etc...
 *
 * @note Unlike the older API, the time slice parameter here is
 * specified in ticks, not milliseconds.  Ticks have always been the
 * internal unit, and not all platforms have integer conversions
 * between the two.
 *
 * @note Threads with a non-zero slice time set will be timesliced
 * always, even if they are higher priority than the maximum timeslice
 * priority set via k_sched_time_slice_set().
 *
 * @note The callback notification for slice expiration happens, as it
 * must, while the thread is still "current", and thus it happens
 * before any registered timeouts at this tick.  This has the somewhat
 * confusing side effect that the tick time (c.f. k_uptime_get()) does
 * not yet reflect the expired ticks.  Applications wishing to make
 * fine-grained timing decisions within this callback should use the
 * cycle API, or derived facilities like k_thread_runtime_stats_get().
 *
 * @param th A valid, initialized thread
 * @param slice_ticks Maximum timeslice, in ticks
 * @param expired Callback function called on slice expiration
 * @param data Parameter for the expiration handler
 */
void k_thread_time_slice_set(struct k_thread *th, int32_t slice_ticks,
			     k_thread_timeslice_fn_t expired, void *data)
{
    __ASSERT_NO_MSG(0);
}
#endif

/**
 * @brief Determine if code is running at interrupt level.
 *
 * This routine allows the caller to customize its actions, depending on
 * whether it is a thread or an ISR.
 *
 * @funcprops \isr_ok
 *
 * @return false if invoked by a thread.
 * @return true if invoked by an ISR.
 */
bool k_is_in_isr(void)
{
    return (0U != __get_IPSR());
}

/**
 * @brief Determine if code is running in a preemptible thread.
 *
 * This routine allows the caller to customize its actions, depending on
 * whether it can be preempted by another thread. The routine returns a 'true'
 * value if all of the following conditions are met:
 *
 * - The code is running in a thread, not at ISR.
 * - The thread's priority is in the preemptible range.
 * - The thread has not locked the scheduler.
 *
 * @funcprops \isr_ok
 *
 * @return 0 if invoked by an ISR or by a cooperative thread.
 * @return Non-zero if invoked by a preemptible thread.
 */
int k_is_preempt_thread(void)
{
    int ret = 0;

    if (k_is_in_isr())
    {
        ret = 0;
    }
    else
    {
        BaseType_t state = xTaskGetSchedulerState();
        if (taskSCHEDULER_RUNNING == state)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }
    return ret;
}

/**
 * @brief Lock the scheduler.
 *
 * This routine prevents the current thread from being preempted by another
 * thread by instructing the scheduler to treat it as a cooperative thread.
 * If the thread subsequently performs an operation that makes it unready,
 * it will be context switched out in the normal manner. When the thread
 * again becomes the current thread, its non-preemptible status is maintained.
 *
 * This routine can be called recursively.
 *
 * Owing to clever implementation details, scheduler locks are
 * extremely fast for non-userspace threads (just one byte
 * inc/decrement in the thread struct).
 *
 * @note This works by elevating the thread priority temporarily to a
 * cooperative priority, allowing cheap synchronization vs. other
 * preemptible or cooperative threads running on the current CPU.  It
 * does not prevent preemption or asynchrony of other types.  It does
 * not prevent threads from running on other CPUs when CONFIG_SMP=y.
 * It does not prevent interrupts from happening, nor does it prevent
 * threads with MetaIRQ priorities from preempting the current thread.
 * In general this is a historical API not well-suited to modern
 * applications, use with care.
 */
void k_sched_lock(void)
{
    vTaskSuspendAll();
}

/**
 * @brief Unlock the scheduler.
 *
 * This routine reverses the effect of a previous call to k_sched_lock().
 * A thread must call the routine once for each time it called k_sched_lock()
 * before the thread becomes preemptible.
 */
void k_sched_unlock(void)
{
    if (!xTaskResumeAll())
    {
        taskYIELD();
    }
}

/**
 * @brief Set current thread's custom data.
 *
 * This routine sets the custom data for the current thread to @ value.
 *
 * Custom data is not used by the kernel itself, and is freely available
 * for a thread to use as it sees fit. It can be used as a framework
 * upon which to build thread-local storage.
 *
 * @param value New custom data value.
 *
 */
void k_thread_custom_data_set(void *value)
{
    k_tid_t id;

    id = k_sched_current_thread_query();

    if (NULL == id)
    {
        return;
    }
    id->custom_data = value;
}

/**
 * @brief Get current thread's custom data.
 *
 * This routine returns the custom data for the current thread.
 *
 * @return Current custom data value.
 */
void *k_thread_custom_data_get(void)
{
    k_tid_t id;

    id = k_sched_current_thread_query();

    if (NULL == id)
    {
        return NULL;
    }
    return id->custom_data;
}

/**
 * @brief Set current thread name
 *
 * Set the name of the thread to be used when @kconfig{CONFIG_THREAD_MONITOR}
 * is enabled for tracing and debugging.
 *
 * @param thread Thread to set name, or NULL to set the current thread
 * @param str Name string
 * @retval 0 on success
 * @retval -EFAULT Memory access error with supplied string
 * @retval -ENOSYS Thread name configuration option not enabled
 * @retval -EINVAL Thread name too long
 */
int k_thread_name_set(k_tid_t thread, const char *str)
{
    int len;

    if (NULL == thread)
    {
        thread = k_sched_current_thread_query();
    }

    if (NULL == thread)
    {
        return -EFAULT;
    }

    len = MIN(strlen(str), sizeof(thread->task_name) - 1);
    memcpy(&thread->task_name[0], str, len);
    thread->task_name[len] = '\0';

    len = MIN(strlen(str), configMAX_TASK_NAME_LEN - 1);
    memcpy(thread->task_buffer.ucDummy7, str, len);
    thread->task_buffer.ucDummy7[len] = '\0';

    return 0;
}

/**
 * @brief Get thread name
 *
 * Get the name of a thread
 *
 * @param thread Thread ID
 * @retval Thread name, or NULL if configuration not enabled
 */
const char *k_thread_name_get(k_tid_t thread)
{
    if (NULL == thread)
    {
        thread = k_sched_current_thread_query();
    }

    if (NULL == thread)
    {
        return NULL;
    }

    return &thread->task_name[0];
}

/**
 * @brief Copy the thread name into a supplied buffer
 *
 * @param thread Thread to obtain name information
 * @param buf Destination buffer
 * @param size Destination buffer size
 * @retval -ENOSPC Destination buffer too small
 * @retval -EFAULT Memory access error
 * @retval -ENOSYS Thread name feature not enabled
 * @retval 0 Success
 */
int k_thread_name_copy(k_tid_t thread, char *buf,
				 size_t size)
{
    int len;

    if (NULL == buf)
    {
        return -ENOSPC;
    }

    if (NULL == thread)
    {
        thread = k_sched_current_thread_query();
    }

    if (NULL == thread)
    {
        return -EFAULT;
    }

    len = MIN(size - 1, sizeof(thread->task_name) - 1);
    memcpy(buf, &thread->task_name[0], len);
    buf[len] = '\0';

    return 0;
}

/**
 * @brief Get thread state string
 *
 * This routine generates a human friendly string containing the thread's
 * state, and copies as much of it as possible into @a buf.
 *
 * @param thread_id Thread ID
 * @param buf Buffer into which to copy state strings
 * @param buf_size Size of the buffer
 *
 * @retval Pointer to @a buf if data was copied, else a pointer to "".
 */
const char *k_thread_state_str(k_tid_t thread_id, char *buf, size_t buf_size)
{
    const char * const state_string[] =
    {
        "running",
        "ready",
        "blocked",
        "suspended",
        "deleted",
        "invalid",
    };

    int len;
    eTaskState state;

    if (NULL == buf)
    {
        return "";
    }

    if (NULL == thread_id)
    {
        thread_id = k_sched_current_thread_query();
    }

    if (NULL == thread_id)
    {
        return "";
    }

    state = eTaskGetState(thread_id->handle);

    if (state < ARRAY_SIZE(state_string))
    {
        len = MIN(buf_size - 1, strlen(state_string[state]));
        memcpy(buf, state_string[state], len);
        buf[len] = '\0';
        return buf;
    }
    else
    {
        return "";
    }
}

/**
 * @brief Get system uptime, in system ticks.
 *
 * This routine returns the elapsed time since the system booted, in
 * ticks (c.f. @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC}), which is the
 * fundamental unit of resolution of kernel timekeeping.
 *
 * @return Current uptime in ticks.
 */
int64_t k_uptime_ticks(void)
{
    TickType_t ticks;

    if (k_is_in_isr())
    {
        ticks = xTaskGetTickCountFromISR();
    }
    else
    {
        ticks = xTaskGetTickCount();
    }

    return (int64_t)ticks;
}
