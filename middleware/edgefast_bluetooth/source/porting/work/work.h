/*
 * Copyright 2024-2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __EDGEFAST_BT_BLE_WORK_H__
#define __EDGEFAST_BT_BLE_WORK_H__

#include <system_c.h>

/**
 * @cond INTERNAL_HIDDEN
 */

struct k_work_delayable;
struct k_work_sync;
struct k_work;

struct _timeout;
struct k_work_q;
struct k_work_queue_config;
extern struct k_work_q k_sys_work_q;

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @defgroup workqueue_apis Work Queue APIs
 * @ingroup kernel_apis
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief The signature for a work item handler function.
 *
 * The function will be invoked by the thread animating a work queue.
 *
 * @param work the work item that provided the handler.
 */
typedef void (*k_work_handler_t)(struct k_work *work);

/** @brief Initialize a (non-delayable) work structure.
 *
 * This must be invoked before submitting a work structure for the first time.
 * It need not be invoked again on the same work structure.  It can be
 * re-invoked to change the associated handler, but this must be done when the
 * work item is idle.
 *
 * @funcprops \isr_ok
 *
 * @param work the work structure to be initialized.
 *
 * @param handler the handler to be invoked by the work item.
 */
void k_work_init(struct k_work *work,
		  k_work_handler_t handler);

/** @brief Busy state flags from the work item.
 *
 * A zero return value indicates the work item appears to be idle.
 *
 * @note This is a live snapshot of state, which may change before the result
 * is checked.  Use locks where appropriate.
 *
 * @funcprops \isr_ok
 *
 * @param work pointer to the work item.
 *
 * @return a mask of flags K_WORK_DELAYED, K_WORK_QUEUED,
 * K_WORK_RUNNING, and K_WORK_CANCELING.
 */
int k_work_busy_get(const struct k_work *work);

/** @brief Test whether a work item is currently pending.
 *
 * Wrapper to determine whether a work item is in a non-idle dstate.
 *
 * @note This is a live snapshot of state, which may change before the result
 * is checked.  Use locks where appropriate.
 *
 * @funcprops \isr_ok
 *
 * @param work pointer to the work item.
 *
 * @return true if and only if k_work_busy_get() returns a non-zero value.
 */
static inline bool k_work_is_pending(const struct k_work *work);

/** @brief Submit a work item to a queue.
 *
 * @param queue pointer to the work queue on which the item should run.  If
 * NULL the queue from the most recent submission will be used.
 *
 * @funcprops \isr_ok
 *
 * @param work pointer to the work item.
 *
 * @retval 0 if work was already submitted to a queue
 * @retval 1 if work was not submitted and has been queued to @p queue
 * @retval 2 if work was running and has been queued to the queue that was
 * running it
 * @retval -EBUSY
 * * if work submission was rejected because the work item is cancelling; or
 * * @p queue is draining; or
 * * @p queue is plugged.
 * @retval -EINVAL if @p queue is null and the work item has never been run.
 * @retval -ENODEV if @p queue has not been started.
 */
int k_work_submit_to_queue(struct k_work_q *queue,
			   struct k_work *work);

/** @brief Submit a work item to the system queue.
 *
 * @funcprops \isr_ok
 *
 * @param work pointer to the work item.
 *
 * @return as with k_work_submit_to_queue().
 */
extern int k_work_submit(struct k_work *work);

/** @brief Wait for last-submitted instance to complete.
 *
 * Resubmissions may occur while waiting, including chained submissions (from
 * within the handler).
 *
 * @note Be careful of caller and work queue thread relative priority.  If
 * this function sleeps it will not return until the work queue thread
 * completes the tasks that allow this thread to resume.
 *
 * @note Behavior is undefined if this function is invoked on @p work from a
 * work queue running @p work.
 *
 * @param work pointer to the work item.
 *
 * @param sync pointer to an opaque item containing state related to the
 * pending cancellation.  The object must persist until the call returns, and
 * be accessible from both the caller thread and the work queue thread.  The
 * object must not be used for any other flush or cancel operation until this
 * one completes.  On architectures with CONFIG_KERNEL_COHERENCE the object
 * must be allocated in coherent memory.
 *
 * @retval true if call had to wait for completion
 * @retval false if work was already idle
 */
bool k_work_flush(struct k_work *work,
		  struct k_work_sync *sync);

/** @brief Cancel a work item.
 *
 * This attempts to prevent a pending (non-delayable) work item from being
 * processed by removing it from the work queue.  If the item is being
 * processed, the work item will continue to be processed, but resubmissions
 * are rejected until cancellation completes.
 *
 * If this returns zero cancellation is complete, otherwise something
 * (probably a work queue thread) is still referencing the item.
 *
 * See also k_work_cancel_sync().
 *
 * @funcprops \isr_ok
 *
 * @param work pointer to the work item.
 *
 * @return the k_work_busy_get() status indicating the state of the item after all
 * cancellation steps performed by this call are completed.
 */
int k_work_cancel(struct k_work *work);

/** @brief Cancel a work item and wait for it to complete.
 *
 * Same as k_work_cancel() but does not return until cancellation is complete.
 * This can be invoked by a thread after k_work_cancel() to synchronize with a
 * previous cancellation.
 *
 * On return the work structure will be idle unless something submits it after
 * the cancellation was complete.
 *
 * @note Be careful of caller and work queue thread relative priority.  If
 * this function sleeps it will not return until the work queue thread
 * completes the tasks that allow this thread to resume.
 *
 * @note Behavior is undefined if this function is invoked on @p work from a
 * work queue running @p work.
 *
 * @param work pointer to the work item.
 *
 * @param sync pointer to an opaque item containing state related to the
 * pending cancellation.  The object must persist until the call returns, and
 * be accessible from both the caller thread and the work queue thread.  The
 * object must not be used for any other flush or cancel operation until this
 * one completes.  On architectures with CONFIG_KERNEL_COHERENCE the object
 * must be allocated in coherent memory.
 *
 * @retval true if work was pending (call had to wait for cancellation of a
 * running handler to complete, or scheduled or submitted operations were
 * cancelled);
 * @retval false otherwise
 */
bool k_work_cancel_sync(struct k_work *work, struct k_work_sync *sync);

/** @brief Initialize a work queue structure.
 *
 * This must be invoked before starting a work queue structure for the first time.
 * It need not be invoked again on the same work queue structure.
 *
 * @funcprops \isr_ok
 *
 * @param queue the queue structure to be initialized.
 */
void k_work_queue_init(struct k_work_q *queue);

/** @brief Initialize a work queue.
 *
 * This configures the work queue thread and starts it running.  The function
 * should not be re-invoked on a queue.
 *
 * @param queue pointer to the queue structure. It must be initialized
 *        in zeroed/bss memory or with @ref k_work_queue_init before
 *        use.
 *
 * @param stack pointer to the work thread stack area.
 *
 * @param stack_size size of the the work thread stack area, in bytes.
 *
 * @param prio initial thread priority
 *
 * @param cfg optional additional configuration parameters.  Pass @c
 * NULL if not required, to use the defaults documented in
 * k_work_queue_config.
 */
void k_work_queue_start(struct k_work_q *queue,
			k_thread_stack_t *stack, size_t stack_size,
			int prio, const struct k_work_queue_config *cfg);

/** @brief Access the thread that animates a work queue.
 *
 * This is necessary to grant a work queue thread access to things the work
 * items it will process are expected to use.
 *
 * @param queue pointer to the queue structure.
 *
 * @return the thread associated with the work queue.
 */
static inline k_tid_t k_work_queue_thread_get(struct k_work_q *queue);

/** @brief Wait until the work queue has drained, optionally plugging it.
 *
 * This blocks submission to the work queue except when coming from queue
 * thread, and blocks the caller until no more work items are available in the
 * queue.
 *
 * If @p plug is true then submission will continue to be blocked after the
 * drain operation completes until k_work_queue_unplug() is invoked.
 *
 * Note that work items that are delayed are not yet associated with their
 * work queue.  They must be cancelled externally if a goal is to ensure the
 * work queue remains empty.  The @p plug feature can be used to prevent
 * delayed items from being submitted after the drain completes.
 *
 * @param queue pointer to the queue structure.
 *
 * @param plug if true the work queue will continue to block new submissions
 * after all items have drained.
 *
 * @retval 1 if call had to wait for the drain to complete
 * @retval 0 if call did not have to wait
 * @retval negative if wait was interrupted or failed
 */
int k_work_queue_drain(struct k_work_q *queue, bool plug);

/** @brief Release a work queue to accept new submissions.
 *
 * This releases the block on new submissions placed when k_work_queue_drain()
 * is invoked with the @p plug option enabled.  If this is invoked before the
 * drain completes new items may be submitted as soon as the drain completes.
 *
 * @funcprops \isr_ok
 *
 * @param queue pointer to the queue structure.
 *
 * @retval 0 if successfully unplugged
 * @retval -EALREADY if the work queue was not plugged.
 */
int k_work_queue_unplug(struct k_work_q *queue);

/** @brief Initialize a delayable work structure.
 *
 * This must be invoked before scheduling a delayable work structure for the
 * first time.  It need not be invoked again on the same work structure.  It
 * can be re-invoked to change the associated handler, but this must be done
 * when the work item is idle.
 *
 * @funcprops \isr_ok
 *
 * @param dwork the delayable work structure to be initialized.
 *
 * @param handler the handler to be invoked by the work item.
 */
void k_work_init_delayable(struct k_work_delayable *dwork,
			   k_work_handler_t handler);

/**
 * @brief Get the parent delayable work structure from a work pointer.
 *
 * This function is necessary when a @c k_work_handler_t function is passed to
 * k_work_schedule_for_queue() and the handler needs to access data from the
 * container of the containing `k_work_delayable`.
 *
 * @param work Address passed to the work handler
 *
 * @return Address of the containing @c k_work_delayable structure.
 */
static inline struct k_work_delayable *
k_work_delayable_from_work(struct k_work *work);

/** @brief Busy state flags from the delayable work item.
 *
 * @funcprops \isr_ok
 *
 * @note This is a live snapshot of state, which may change before the result
 * can be inspected.  Use locks where appropriate.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @return a mask of flags K_WORK_DELAYED, K_WORK_QUEUED, K_WORK_RUNNING, and
 * K_WORK_CANCELING.  A zero return value indicates the work item appears to
 * be idle.
 */
int k_work_delayable_busy_get(const struct k_work_delayable *dwork);

/** @brief Test whether a delayed work item is currently pending.
 *
 * Wrapper to determine whether a delayed work item is in a non-idle state.
 *
 * @note This is a live snapshot of state, which may change before the result
 * can be inspected.  Use locks where appropriate.
 *
 * @funcprops \isr_ok
 *
 * @param dwork pointer to the delayable work item.
 *
 * @return true if and only if k_work_delayable_busy_get() returns a non-zero
 * value.
 */
static inline bool k_work_delayable_is_pending(
	const struct k_work_delayable *dwork);

/** @brief Get the absolute tick count at which a scheduled delayable work
 * will be submitted.
 *
 * @note This is a live snapshot of state, which may change before the result
 * can be inspected.  Use locks where appropriate.
 *
 * @funcprops \isr_ok
 *
 * @param dwork pointer to the delayable work item.
 *
 * @return the tick count when the timer that will schedule the work item will
 * expire, or the current tick count if the work is not scheduled.
 */
static inline k_ticks_t k_work_delayable_expires_get(
	const struct k_work_delayable *dwork);

/** @brief Get the number of ticks until a scheduled delayable work will be
 * submitted.
 *
 * @note This is a live snapshot of state, which may change before the result
 * can be inspected.  Use locks where appropriate.
 *
 * @funcprops \isr_ok
 *
 * @param dwork pointer to the delayable work item.
 *
 * @return the number of ticks until the timer that will schedule the work
 * item will expire, or zero if the item is not scheduled.
 */
static inline k_ticks_t k_work_delayable_remaining_get(
	const struct k_work_delayable *dwork);

/** @brief Submit an idle work item to a queue after a delay.
 *
 * Unlike k_work_reschedule_for_queue() this is a no-op if the work item is
 * already scheduled or submitted, even if @p delay is @c K_NO_WAIT.
 *
 * @funcprops \isr_ok
 *
 * @param queue the queue on which the work item should be submitted after the
 * delay.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @param delay the time to wait before submitting the work item.  If @c
 * K_NO_WAIT and the work is not pending this is equivalent to
 * k_work_submit_to_queue().
 *
 * @retval 0 if work was already scheduled or submitted.
 * @retval 1 if work has been scheduled.
 * @retval -EBUSY if @p delay is @c K_NO_WAIT and
 *         k_work_submit_to_queue() fails with this code.
 * @retval -EINVAL if @p delay is @c K_NO_WAIT and
 *         k_work_submit_to_queue() fails with this code.
 * @retval -ENODEV if @p delay is @c K_NO_WAIT and
 *         k_work_submit_to_queue() fails with this code.
 */
int k_work_schedule_for_queue(struct k_work_q *queue,
			       struct k_work_delayable *dwork,
			       k_timeout_t delay);

/** @brief Submit an idle work item to the system work queue after a
 * delay.
 *
 * This is a thin wrapper around k_work_schedule_for_queue(), with all the API
 * characteristics of that function.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @param delay the time to wait before submitting the work item.  If @c
 * K_NO_WAIT this is equivalent to k_work_submit_to_queue().
 *
 * @return as with k_work_schedule_for_queue().
 */
extern int k_work_schedule(struct k_work_delayable *dwork,
				   k_timeout_t delay);

/** @brief Reschedule a work item to a queue after a delay.
 *
 * Unlike k_work_schedule_for_queue() this function can change the deadline of
 * a scheduled work item, and will schedule a work item that is in any state
 * (e.g. is idle, submitted, or running).  This function does not affect
 * ("unsubmit") a work item that has been submitted to a queue.
 *
 * @funcprops \isr_ok
 *
 * @param queue the queue on which the work item should be submitted after the
 * delay.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @param delay the time to wait before submitting the work item.  If @c
 * K_NO_WAIT this is equivalent to k_work_submit_to_queue() after canceling
 * any previous scheduled submission.
 *
 * @note If delay is @c K_NO_WAIT ("no delay") the return values are as with
 * k_work_submit_to_queue().
 *
 * @retval 0 if delay is @c K_NO_WAIT and work was already on a queue
 * @retval 1 if
 * * delay is @c K_NO_WAIT and work was not submitted but has now been queued
 *   to @p queue; or
 * * delay not @c K_NO_WAIT and work has been scheduled
 * @retval 2 if delay is @c K_NO_WAIT and work was running and has been queued
 * to the queue that was running it
 * @retval -EBUSY if @p delay is @c K_NO_WAIT and
 *         k_work_submit_to_queue() fails with this code.
 * @retval -EINVAL if @p delay is @c K_NO_WAIT and
 *         k_work_submit_to_queue() fails with this code.
 * @retval -ENODEV if @p delay is @c K_NO_WAIT and
 *         k_work_submit_to_queue() fails with this code.
 */
int k_work_reschedule_for_queue(struct k_work_q *queue,
				 struct k_work_delayable *dwork,
				 k_timeout_t delay);

/** @brief Reschedule a work item to the system work queue after a
 * delay.
 *
 * This is a thin wrapper around k_work_reschedule_for_queue(), with all the
 * API characteristics of that function.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @param delay the time to wait before submitting the work item.
 *
 * @return as with k_work_reschedule_for_queue().
 */
extern int k_work_reschedule(struct k_work_delayable *dwork,
				     k_timeout_t delay);

/** @brief Flush delayable work.
 *
 * If the work is scheduled, it is immediately submitted.  Then the caller
 * blocks until the work completes, as with k_work_flush().
 *
 * @note Be careful of caller and work queue thread relative priority.  If
 * this function sleeps it will not return until the work queue thread
 * completes the tasks that allow this thread to resume.
 *
 * @note Behavior is undefined if this function is invoked on @p dwork from a
 * work queue running @p dwork.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @param sync pointer to an opaque item containing state related to the
 * pending cancellation.  The object must persist until the call returns, and
 * be accessible from both the caller thread and the work queue thread.  The
 * object must not be used for any other flush or cancel operation until this
 * one completes.  On architectures with CONFIG_KERNEL_COHERENCE the object
 * must be allocated in coherent memory.
 *
 * @retval true if call had to wait for completion
 * @retval false if work was already idle
 */
bool k_work_flush_delayable(struct k_work_delayable *dwork,
			    struct k_work_sync *sync);

/** @brief Cancel delayable work.
 *
 * Similar to k_work_cancel() but for delayable work.  If the work is
 * scheduled or submitted it is canceled.  This function does not wait for the
 * cancellation to complete.
 *
 * @note The work may still be running when this returns.  Use
 * k_work_flush_delayable() or k_work_cancel_delayable_sync() to ensure it is
 * not running.
 *
 * @note Canceling delayable work does not prevent rescheduling it.  It does
 * prevent submitting it until the cancellation completes.
 *
 * @funcprops \isr_ok
 *
 * @param dwork pointer to the delayable work item.
 *
 * @return the k_work_delayable_busy_get() status indicating the state of the
 * item after all cancellation steps performed by this call are completed.
 */
int k_work_cancel_delayable(struct k_work_delayable *dwork);

/** @brief Cancel delayable work and wait.
 *
 * Like k_work_cancel_delayable() but waits until the work becomes idle.
 *
 * @note Canceling delayable work does not prevent rescheduling it.  It does
 * prevent submitting it until the cancellation completes.
 *
 * @note Be careful of caller and work queue thread relative priority.  If
 * this function sleeps it will not return until the work queue thread
 * completes the tasks that allow this thread to resume.
 *
 * @note Behavior is undefined if this function is invoked on @p dwork from a
 * work queue running @p dwork.
 *
 * @param dwork pointer to the delayable work item.
 *
 * @param sync pointer to an opaque item containing state related to the
 * pending cancellation.  The object must persist until the call returns, and
 * be accessible from both the caller thread and the work queue thread.  The
 * object must not be used for any other flush or cancel operation until this
 * one completes.  On architectures with CONFIG_KERNEL_COHERENCE the object
 * must be allocated in coherent memory.
 *
 * @retval true if work was not idle (call had to wait for cancellation of a
 * running handler to complete, or scheduled or submitted operations were
 * cancelled);
 * @retval false otherwise
 */
bool k_work_cancel_delayable_sync(struct k_work_delayable *dwork,
				  struct k_work_sync *sync);

k_ticks_t k_work_delayable_timeout_expires(const struct k_work_delayable *dwork);

k_ticks_t k_work_delayable_timeout_remaining(const struct k_work_delayable *dwork);

enum {
/**
 * @cond INTERNAL_HIDDEN
 */

	/* The atomic API is used for all work and queue flags fields to
	 * enforce sequential consistency in SMP environments.
	 */

	/* Bits that represent the work item states.  At least nine of the
	 * combinations are distinct valid stable states.
	 */
	K_WORK_RUNNING_BIT = 0,
	K_WORK_CANCELING_BIT = 1,
	K_WORK_QUEUED_BIT = 2,
	K_WORK_DELAYED_BIT = 3,

	K_WORK_MASK = BIT(K_WORK_DELAYED_BIT) | BIT(K_WORK_QUEUED_BIT)
		| BIT(K_WORK_RUNNING_BIT) | BIT(K_WORK_CANCELING_BIT),

	/* Static work flags */
	K_WORK_DELAYABLE_BIT = 8,
	K_WORK_DELAYABLE = BIT(K_WORK_DELAYABLE_BIT),

	/* Dynamic work queue flags */
	K_WORK_QUEUE_STARTED_BIT = 0,
	K_WORK_QUEUE_STARTED = BIT(K_WORK_QUEUE_STARTED_BIT),
	K_WORK_QUEUE_BUSY_BIT = 1,
	K_WORK_QUEUE_BUSY = BIT(K_WORK_QUEUE_BUSY_BIT),
	K_WORK_QUEUE_DRAIN_BIT = 2,
	K_WORK_QUEUE_DRAIN = BIT(K_WORK_QUEUE_DRAIN_BIT),
	K_WORK_QUEUE_PLUGGED_BIT = 3,
	K_WORK_QUEUE_PLUGGED = BIT(K_WORK_QUEUE_PLUGGED_BIT),
	K_WORK_QUEUE_INITED_BIT = 4,
	K_WORK_QUEUE_INITED = BIT(K_WORK_QUEUE_INITED_BIT),

	/* Static work queue flags */
	K_WORK_QUEUE_NO_YIELD_BIT = 8,
	K_WORK_QUEUE_NO_YIELD = BIT(K_WORK_QUEUE_NO_YIELD_BIT),

/**
 * INTERNAL_HIDDEN @endcond
 */
	/* Transient work flags */

	/** @brief Flag indicating a work item that is running under a work
	 * queue thread.
	 *
	 * Accessed via k_work_busy_get().  May co-occur with other flags.
	 */
	K_WORK_RUNNING = BIT(K_WORK_RUNNING_BIT),

	/** @brief Flag indicating a work item that is being canceled.
	 *
	 * Accessed via k_work_busy_get().  May co-occur with other flags.
	 */
	K_WORK_CANCELING = BIT(K_WORK_CANCELING_BIT),

	/** @brief Flag indicating a work item that has been submitted to a
	 * queue but has not started running.
	 *
	 * Accessed via k_work_busy_get().  May co-occur with other flags.
	 */
	K_WORK_QUEUED = BIT(K_WORK_QUEUED_BIT),

	/** @brief Flag indicating a delayed work item that is scheduled for
	 * submission to a queue.
	 *
	 * Accessed via k_work_busy_get().  May co-occur with other flags.
	 */
	K_WORK_DELAYED = BIT(K_WORK_DELAYED_BIT),
};

/** @brief A structure used to submit work. */
struct k_work {
	/* All fields are protected by the work module spinlock.  No fields
	 * are to be accessed except through kernel API.
	 */

	/* Node to link into k_work_q pending list. */
	sys_snode_t node;

	/* The function to be invoked by the work queue thread. */
	k_work_handler_t handler;

	/* The queue on which the work item was last submitted. */
	struct k_work_q *queue;

	/* State of the work item.
	 *
	 * The item can be DELAYED, QUEUED, and RUNNING simultaneously.
	 *
	 * It can be RUNNING and CANCELING simultaneously.
	 */
	uint32_t flags;
};

#define Z_WORK_INITIALIZER(work_handler) { \
	.handler = work_handler, \
}

/** @brief A structure used to submit work after a delay. */
struct k_work_delayable {
	/* The work item. */
	struct k_work work;

	/* Timeout used to submit work after a delay. */
	struct _timeout timeout;

	/* The queue to which the work should be submitted. */
	struct k_work_q *queue;
};

#define Z_WORK_DELAYABLE_INITIALIZER(work_handler) { \
	.work = { \
		.handler = work_handler, \
		.flags = K_WORK_DELAYABLE, \
	}, \
}

/**
 * @brief Initialize a statically-defined work item.
 *
 * This macro can be used to initialize a statically-defined workqueue work
 * item, prior to its first use. For example,
 *
 * @code static K_WORK_DEFINE(<work>, <work_handler>); @endcode
 *
 * @param work Symbol name for work item object
 * @param work_handler Function to invoke each time work item is processed.
 */
#define K_WORK_DEFINE(work, work_handler) \
	struct k_work work = Z_WORK_INITIALIZER(work_handler)

/**
 * @brief Initialize a statically-defined delayable work item.
 *
 * This macro can be used to initialize a statically-defined delayable
 * work item, prior to its first use. For example,
 *
 * @code static K_WORK_DELAYABLE_DEFINE(<dwork>, <work_handler>); @endcode
 *
 * Note that if the runtime dependencies support initialization with
 * k_work_init_delayable() using that will eliminate the initialized
 * object in ROM that is produced by this macro and copied in at
 * system startup.
 *
 * @param work Symbol name for delayable work item object
 * @param work_handler Function to invoke each time work item is processed.
 */
#define K_WORK_DELAYABLE_DEFINE(work, work_handler) \
	struct k_work_delayable work \
	  = Z_WORK_DELAYABLE_INITIALIZER(work_handler)

/**
 * INTERNAL_HIDDEN @endcond
 */

/** @brief A structure holding internal state for a pending synchronous
 * operation on a work item or queue.
 *
 * Instances of this type are provided by the caller for invocation of
 * k_work_flush(), k_work_cancel_sync() and sibling flush and cancel APIs.  A
 * referenced object must persist until the call returns, and be accessible
 * from both the caller thread and the work queue thread.
 *
 * @note If CONFIG_KERNEL_COHERENCE is enabled the object must be allocated in
 * coherent memory; see arch_mem_coherent().  The stack on these architectures
 * is generally not coherent.  be stack-allocated.  Violations are detected by
 * runtime assertion.
 */
struct k_work_sync {
	uint8_t dummy;
};

/** @brief A structure holding optional configuration items for a work
 * queue.
 *
 * This structure, and values it references, are not retained by
 * k_work_queue_start().
 */
struct k_work_queue_config
{
	/** The name to be given to the work queue thread.
	 *
	 * If left null the thread will not have a name.
	 */
	const char *name;

	/** Control whether the work queue thread should yield between
	 * items.
	 *
	 * Yielding between items helps guarantee the work queue
	 * thread does not starve other threads, including cooperative
	 * ones released by a work item.  This is the default behavior.
	 *
	 * Set this to @c true to prevent the work queue thread from
	 * yielding between items.  This may be appropriate when a
	 * sequence of items should complete without yielding
	 * control.
	 */
	bool no_yield;
};

/** @brief A structure used to hold work until it can be processed. */
struct k_work_q {
	/* The thread that animates the work. */
	struct k_thread thread;

	/* All the following fields must be accessed only while the
	 * work module spinlock is held.
	 */

	/* List of k_work items to be worked. */
	struct k_queue pending;

	uint32_t tick;

	/* Flags describing queue state. */
	uint32_t flags;
};

/* Provide the implementation for inline functions declared above */

static inline bool k_work_is_pending(const struct k_work *work)
{
	return k_work_busy_get(work) != 0;
}

static inline struct k_work_delayable *
k_work_delayable_from_work(struct k_work *work)
{
	return CONTAINER_OF(work, struct k_work_delayable, work);
}

static inline bool k_work_delayable_is_pending(
	const struct k_work_delayable *dwork)
{
	return k_work_delayable_busy_get(dwork) != 0;
}

static inline k_ticks_t k_work_delayable_expires_get(
	const struct k_work_delayable *dwork)
{
	return k_work_delayable_timeout_expires(dwork);
}

static inline k_ticks_t k_work_delayable_remaining_get(
	const struct k_work_delayable *dwork)
{
	return k_work_delayable_timeout_remaining(dwork);
}

static inline k_tid_t k_work_queue_thread_get(struct k_work_q *queue)
{
	return &queue->thread;
}

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __EDGEFAST_BT_BLE_WORK_H__ */
