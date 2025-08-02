/*
 * Copyright 2023,2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __EDGEFAST_BT_BLE_THREAD_H__
#define __EDGEFAST_BT_BLE_THREAD_H__

#include <system_c.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*k_thread_entry_t)(void *p1, void *p2, void *p3);

struct k_thread {
	sys_snode_t node;
	TaskHandle_t handle;
	StaticTask_t task_buffer;
	k_thread_entry_t entry;
	void *p1;
	void *p2;
	void *p3;
	uint32_t options;
	struct k_heap *resource_pool;
	void *custom_data;
	char task_name[ configMAX_TASK_NAME_LEN ];
};

typedef struct k_thread _thread_t;
typedef struct k_thread *k_tid_t;

#define MSEC_PER_SEC   1000

/**
 * @brief Define a toplevel kernel stack memory region
 *
 * This defines a region of memory for use as a thread stack, for threads
 * that exclusively run in supervisor mode. This is also suitable for
 * declaring special stacks for interrupt or exception handling.
 *
 * Stacks defined with this macro may not host user mode threads.
 *
 * It is legal to precede this definition with the 'static' keyword.
 *
 * It is NOT legal to take the sizeof(sym) and pass that to the stackSize
 * parameter of k_thread_create(), it may not be the same as the
 * 'size' parameter. Use K_KERNEL_STACK_SIZEOF() instead.
 *
 * The total amount of memory allocated may be increased to accommodate
 * fixed-size stack overflow guards.
 *
 * @param sym Thread stack symbol name
 * @param size Size of the stack memory region
 */
#define K_KERNEL_STACK_DEFINE(sym, size) uint8_t sym[ROUND_UP((size), sizeof(size_t))]

#define K_KERNEL_STACK_SIZEOF(sym) (sizeof(sym))

#define K_KERNEL_STACK_MEMBER(sym, size) K_KERNEL_STACK_DEFINE(sym, size)

#define K_THREAD_STACK_DEFINE K_KERNEL_STACK_DEFINE

#define K_THREAD_STACK_SIZEOF K_KERNEL_STACK_SIZEOF

#ifndef MSEC_TO_TICK
#define MSEC_TO_TICK(msec) \
    (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)
#endif /* MSEC_TO_TICK */

#ifndef TICKS_TO_MSEC
#define TICKS_TO_MSEC(tick) ((uint32_t)((uint64_t)(tick)*1000uL / (uint64_t)configTICK_RATE_HZ))
#endif /* TICKS_TO_MSEC */

#ifndef TICKS_TO_MSEC_64
#define TICKS_TO_MSEC_64(tick) ((uint64_t)((uint64_t)(tick)*1000uL / (uint64_t)configTICK_RATE_HZ))
#endif /* TICKS_TO_MSEC_64 */

#ifndef TICKS_TO_USEC_64
#define TICKS_TO_USEC_64(tick) ((uint64_t)((uint64_t)(tick)*1000000uL / (uint64_t)configTICK_RATE_HZ))
#endif /* TICKS_TO_USEC_64 */

#ifndef TICKS_TO_NSEC_64
#define TICKS_TO_NSEC_64(tick) ((uint64_t)((uint64_t)(tick)*1000000000uL / (uint64_t)configTICK_RATE_HZ))
#endif /* TICKS_TO_NSEC_64 */

/**
 * @defgroup thread_apis Thread APIs
 * @ingroup kernel_apis
 * @{
 */


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
__syscall k_thread_stack_t *k_thread_stack_alloc(size_t size, int flags);

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
__syscall int k_thread_stack_free(k_thread_stack_t *stack);

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
__syscall k_tid_t k_thread_create(struct k_thread *new_thread,
				  k_thread_stack_t *stack,
				  size_t stack_size,
				  k_thread_entry_t entry,
				  void *p1, void *p2, void *p3,
				  int prio, uint32_t options, k_timeout_t delay);
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
extern FUNC_NORETURN void k_thread_user_mode_enter(k_thread_entry_t entry,
						   void *p1, void *p2,
						   void *p3);
#endif
/**
 * @brief Grant a thread access to a set of kernel objects
 *
 * This is a convenience function. For the provided thread, grant access to
 * the remaining arguments, which must be pointers to kernel objects.
 *
 * The thread object must be initialized (i.e. running). The objects don't
 * need to be.
 * Note that NULL shouldn't be passed as an argument.
 *
 * @param thread Thread to grant access to objects
 * @param ... list of kernel object pointers
 */
#define k_thread_access_grant(thread, ...) \
	FOR_EACH_FIXED_ARG(k_object_access_grant, (;), thread, __VA_ARGS__)

/**
 * @brief Assign a resource memory pool to a thread
 *
 * By default, threads have no resource pool assigned unless their parent
 * thread has a resource pool, in which case it is inherited. Multiple
 * threads may be assigned to the same memory pool.
 *
 * Changing a thread's resource pool will not migrate allocations from the
 * previous pool.
 *
 * @param thread Target thread to assign a memory pool for resource requests.
 * @param heap Heap object to use for resources,
 *             or NULL if the thread should no longer have a memory pool.
 */
static inline void k_thread_heap_assign(struct k_thread *thread,
					struct k_heap *heap)
{
	thread->resource_pool = heap;
}

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
__syscall int k_thread_stack_space_get(const struct k_thread *thread,
				       size_t *unused_ptr);
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
void k_thread_system_pool_assign(struct k_thread *thread);
#endif /* (CONFIG_HEAP_MEM_POOL_SIZE > 0) */

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
__syscall int k_thread_join(struct k_thread *thread, k_timeout_t timeout);

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
__syscall int32_t k_sleep(k_timeout_t timeout);

/**
 * @brief Put the current thread to sleep.
 *
 * This routine puts the current thread to sleep for @a duration milliseconds.
 *
 * @param ms Number of milliseconds to sleep.
 *
 * @return Zero if the requested time has elapsed or the number of milliseconds
 * left to sleep, if thread was woken up by \ref k_wakeup call.
 */
static inline int32_t k_msleep(int32_t ms)
{
	return k_sleep(ms);
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
__syscall int32_t k_usleep(int32_t us);

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
__syscall void k_busy_wait(uint32_t usec_to_wait);

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
bool k_can_yield(void);

/**
 * @brief Yield the current thread.
 *
 * This routine causes the current thread to yield execution to another
 * thread of the same or higher priority. If there are no other ready threads
 * of the same or higher priority, the routine returns immediately.
 */
__syscall void k_yield(void);

/**
 * @brief Wake up a sleeping thread.
 *
 * This routine prematurely wakes up @a thread from sleeping.
 *
 * If @a thread is not currently sleeping, the routine has no effect.
 *
 * @param thread ID of thread to wake.
 */
__syscall void k_wakeup(k_tid_t thread);

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
__syscall k_tid_t k_sched_current_thread_query(void);

/**
 * @brief Get thread ID of the current thread.
 *
 * @return ID of current thread.
 *
 */
static inline k_tid_t k_current_get(void)
{
#ifdef CONFIG_THREAD_LOCAL_STORAGE
	/* Thread-local cache of current thread ID, set in z_thread_entry() */
	extern __thread k_tid_t z_tls_current;

	return z_tls_current;
#else
	return k_sched_current_thread_query();
#endif
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
__syscall void k_thread_abort(k_tid_t thread);


/**
 * @brief Start an inactive thread
 *
 * If a thread was created with K_FOREVER in the delay parameter, it will
 * not be added to the scheduling queue until this function is called
 * on it.
 *
 * @param thread thread to start
 */
__syscall void k_thread_start(k_tid_t thread);

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
 * When the target thread is active on another CPU, the caller will block until
 * the target thread is halted (suspended or aborted).  But if the caller is in
 * an interrupt context, it will spin waiting for that target thread active on
 * another CPU to halt.
 *
 * If @a thread is already suspended, the routine has no effect.
 *
 * @param thread ID of thread to suspend.
 */
__syscall void k_thread_suspend(k_tid_t thread);

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
__syscall void k_thread_resume(k_tid_t thread);

/**
 * @addtogroup isr_apis
 * @{
 */

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
extern bool k_is_in_isr(void);

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
__syscall int k_is_preempt_thread(void);

/**
 * @brief Test whether startup is in the before-main-task phase.
 *
 * This routine allows the caller to customize its actions, depending on
 * whether it being invoked before the kernel is fully active.
 *
 * @funcprops \isr_ok
 *
 * @return true if invoked before post-kernel initialization
 * @return false if invoked during/after post-kernel initialization
 */
static inline bool k_is_pre_kernel(void)
{
	return false;
}

/**
 * @addtogroup thread_apis
 * @{
 */

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
extern void k_sched_lock(void);

/**
 * @brief Unlock the scheduler.
 *
 * This routine reverses the effect of a previous call to k_sched_lock().
 * A thread must call the routine once for each time it called k_sched_lock()
 * before the thread becomes preemptible.
 */
extern void k_sched_unlock(void);

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
__syscall void k_thread_custom_data_set(void *value);

/**
 * @brief Get current thread's custom data.
 *
 * This routine returns the custom data for the current thread.
 *
 * @return Current custom data value.
 */
__syscall void *k_thread_custom_data_get(void);

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
__syscall int k_thread_name_set(k_tid_t thread, const char *str);

/**
 * @brief Get thread name
 *
 * Get the name of a thread
 *
 * @param thread Thread ID
 * @retval Thread name, or NULL if configuration not enabled
 */
const char *k_thread_name_get(k_tid_t thread);

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
__syscall int k_thread_name_copy(k_tid_t thread, char *buf,
				 size_t size);

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
const char *k_thread_state_str(k_tid_t thread_id, char *buf, size_t buf_size);

/**
 * @}
 */


/**
 * @addtogroup clock_apis
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Get system uptime, in system ticks.
 *
 * This routine returns the elapsed time since the system booted, in
 * ticks (c.f. @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC}), which is the
 * fundamental unit of resolution of kernel timekeeping.
 *
 * @return Current uptime in ticks.
 */
__syscall int64_t k_uptime_ticks(void);

/**
 * @brief Get system uptime.
 *
 * This routine returns the elapsed time since the system booted,
 * in milliseconds.
 *
 * @note
 *    While this function returns time in milliseconds, it does
 *    not mean it has millisecond resolution. The actual resolution depends on
 *    @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC} config option.
 *
 * @return Current uptime in milliseconds.
 */
static inline int64_t k_uptime_get(void)
{
	return (int64_t)TICKS_TO_MSEC_64(k_uptime_ticks());
}

/**
 * @brief Get system uptime (32-bit version).
 *
 * This routine returns the lower 32 bits of the system uptime in
 * milliseconds.
 *
 * Because correct conversion requires full precision of the system
 * clock there is no benefit to using this over k_uptime_get() unless
 * you know the application will never run long enough for the system
 * clock to approach 2^32 ticks.  Calls to this function may involve
 * interrupt blocking and 64-bit math.
 *
 * @note
 *    While this function returns time in milliseconds, it does
 *    not mean it has millisecond resolution. The actual resolution depends on
 *    @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC} config option
 *
 * @return The low 32 bits of the current uptime, in milliseconds.
 */
static inline uint32_t k_uptime_get_32(void)
{
	return (uint32_t)k_uptime_get();
}

/**
 * @brief Get elapsed time.
 *
 * This routine computes the elapsed time between the current system uptime
 * and an earlier reference time, in milliseconds.
 *
 * @param reftime Pointer to a reference time, which is updated to the current
 *                uptime upon return.
 *
 * @return Elapsed time.
 */
static inline int64_t k_uptime_delta(int64_t *reftime)
{
	int64_t uptime, delta;

	uptime = k_uptime_get();
	delta = uptime - *reftime;
	*reftime = uptime;

	return delta;
}

/**
 * @}
 */

 #ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_BT_BLE_THREAD_H__ */