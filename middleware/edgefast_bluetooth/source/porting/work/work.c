/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <porting.h>

/*! @brief Work queue task priority.
 */
#ifndef CONFIG_WORK_QUEUE_TASK_PRIORITY
    #define CONFIG_WORK_QUEUE_TASK_PRIORITY (configMAX_PRIORITIES - 2)
#endif

/*! @brief Work queue task stack size.
 */
#ifndef CONFIG_WORK_QUEUE_TASK_STACK_SIZE
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
    #define CONFIG_WORK_QUEUE_TASK_STACK_SIZE 4096
#else
    #define CONFIG_WORK_QUEUE_TASK_STACK_SIZE 2048
#endif /* CONFIG_BT_SMP */
#endif

enum {
	K_WORK_DELAY_Q_TIMER = 0,
};

/** @brief A structure used to hold work until it can be processed. */
struct k_work_delay_q {
    /* The thread that animates the work. */
	TimerHandle_t timer;

	StaticTimer_t timer_buffer;

	/* All the following fields must be accessed only while the
	 * work module spinlock is held.
	 */

	/* List of k_work items to be worked. */
	sys_slist_t pending;

	uint32_t tick;

	/* Flags describing queue state. */
	atomic_t flags;
};

/* Lock to protect the internal state of all work items, work queues,
 * and pending_cancels.
 */
static struct k_spinlock lock;

static bool k_sys_work_q_init = false;
struct k_work_q k_sys_work_q;
static K_KERNEL_STACK_DEFINE(k_sys_work_q_thread_stack, CONFIG_WORK_QUEUE_TASK_STACK_SIZE);

struct k_work_delay_q k_sys_work_delay_q;

static void work_queue_main(void * p1, void *p2, void *p3);

static void _sys_work_queue_init(void)
{
	bool init = false;

	k_spinlock_key_t key;

	key = k_spin_lock(&lock);
	if (false == k_sys_work_q_init)
	{
		k_sys_work_q_init = true;
		init = true;
	}
	k_spin_unlock(&lock, key);

	if (true == init)
	{
		/* SYS work queue init */
		k_work_queue_init(&k_sys_work_q);
		k_work_queue_start(&k_sys_work_q, k_sys_work_q_thread_stack,
			CONFIG_WORK_QUEUE_TASK_STACK_SIZE,
			K_PRIO_COOP(CONFIG_WORK_QUEUE_TASK_PRIORITY), NULL);
		k_thread_name_set(&k_sys_work_q.thread, "SYS WORK QUEUE");
	}
}

static inline void flag_clear(uint32_t *flagp,
			      uint32_t bit)
{
	*flagp &= ~BIT(bit);
}

static inline void flag_set(uint32_t *flagp,
			    uint32_t bit)
{
	*flagp |= BIT(bit);
}

static inline bool flag_test(const uint32_t *flagp,
			     uint32_t bit)
{
	return (*flagp & BIT(bit)) != 0U;
}

static inline bool flag_test_and_clear(uint32_t *flagp,
				       int bit)
{
	bool ret = flag_test(flagp, bit);

	flag_clear(flagp, bit);

	return ret;
}

static inline void flags_set(uint32_t *flagp,
			     uint32_t flags)
{
	*flagp = flags;
}

static inline uint32_t flags_get(const uint32_t *flagp)
{
	return *flagp;
}

void k_work_init(struct k_work *work, k_work_handler_t handler)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(handler != NULL);

	*work = (struct k_work)Z_WORK_INITIALIZER(handler);
}

int k_work_busy_get(const struct k_work *work)
{
	__ASSERT_NO_MSG(work != NULL);

	k_spinlock_key_t key;
	int ret;

	key = k_spin_lock(&lock);

	ret = flags_get(&work->flags) & K_WORK_MASK;

	k_spin_unlock(&lock, key);

	return ret;
}

int k_work_submit_to_queue(struct k_work_q *queue, struct k_work *work)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, submit_to_queue, queue, work);

	__ASSERT_NO_MSG(work != NULL);
	k_spinlock_key_t key;
	int ret = 0;

	key = k_spin_lock(&lock);
	if (flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
		/* Disallowed */
		ret = -EBUSY;
	} else if (!flag_test(&work->flags, K_WORK_QUEUED_BIT)) {
		/* Not currently queued */

		/* If no queue specified resubmit to last queue.
		 */
		if (queue == NULL) {
		    queue = work->queue;
		}

		/* If the work is currently running we have to use the
		 * queue it's running on to prevent handler
		 * re-entrancy.
		 */
		if (flag_test(&work->flags, K_WORK_RUNNING_BIT)) {
			__ASSERT_NO_MSG(work->queue != NULL);
			queue = work->queue;
		}

		bool chained = (xTaskGetCurrentTaskHandle() == queue->thread.handle) && !k_is_in_isr();
		bool draining = flag_test(&queue->flags, K_WORK_QUEUE_DRAIN_BIT);
		bool plugged = flag_test(&queue->flags, K_WORK_QUEUE_PLUGGED_BIT);

		/* Test for acceptability, in priority order:
		*
		* * -ENODEV if the queue isn't running.
		* * -EBUSY if draining and not chained
		* * -EBUSY if plugged and not draining
		* * otherwise OK
		*/
		if (!flag_test(&queue->flags, K_WORK_QUEUE_STARTED_BIT)) {
			ret = -ENODEV;
		} else if (draining && !chained) {
			ret = -EBUSY;
		} else if (plugged && !draining) {
			ret = -EBUSY;
		} else {
			k_queue_append(&queue->pending, &work->node);
			ret = 1;
		}

		if (ret >= 0)
		{
			flag_set(&work->flags, K_WORK_QUEUED_BIT);
			work->queue = queue;
		}
	} else {
		/* Already queued, do nothing. */
	}
	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, submit_to_queue, queue, work, ret);

	return ret;
}

int k_work_submit(struct k_work *work)
{
	int ret;

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, submit, work);

	_sys_work_queue_init();

	ret = k_work_submit_to_queue(&k_sys_work_q, work);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, submit, work, ret);

	return ret;
}

bool k_work_flush(struct k_work *work,
		  struct k_work_sync *sync)
{
	/* Unsupported */
	return false;
}

int k_work_cancel(struct k_work *work)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(!flag_test(&work->flags, K_WORK_DELAYABLE_BIT));

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel, work);

	k_spinlock_key_t key = k_spin_lock(&lock);

	int ret = 0;

	/* If we haven't already started canceling, do it now. */
	if (!flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
		/* Remove it from the queue, if it's queued. */
		if (flag_test_and_clear(&work->flags, K_WORK_QUEUED_BIT)) {
			(void)k_queue_remove(&work->queue->pending, &work->node);
		}
	}

	if (flags_get(&work->flags) & K_WORK_MASK)
	{
		flag_set(&work->flags, K_WORK_CANCELING_BIT);
	}

	ret = (flags_get(&work->flags) & K_WORK_MASK);

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel, work, ret);

	return ret;
}

bool k_work_cancel_sync(struct k_work *work, struct k_work_sync *sync)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(!flag_test(&work->flags, K_WORK_DELAYABLE_BIT));

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel, work);

	k_spinlock_key_t key = k_spin_lock(&lock);

	int ret = 0;

	/* If we haven't already started canceling, do it now. */
	if (!flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
		/* Remove it from the queue, if it's queued. */
		if (flag_test_and_clear(&work->flags, K_WORK_QUEUED_BIT)) {
			(void)k_queue_remove(&work->queue->pending, &work->node);
		}
	}

	if (flags_get(&work->flags) & K_WORK_MASK)
	{
	flag_set(&work->flags, K_WORK_CANCELING_BIT);
	}

	ret = (flags_get(&work->flags) & K_WORK_MASK);

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel, work, ret);

	return ret;
}

static void work_queue_main(void * p1, void *p2, void *p3)
{
	struct k_work_q *queue = (struct k_work_q *)p1;

	while (true) {
		sys_snode_t *node;
		struct k_work *work = NULL;
		k_work_handler_t handler = NULL;
		k_spinlock_key_t key;
		bool yield;

		/* Check for and prepare any new work. */
		node = k_queue_get(&queue->pending, K_FOREVER);

        	key = k_spin_lock(&lock);
		if (node != NULL) {
			/* Mark that there's some work active that's
			 * not on the pending list.
			 */
			flag_set(&queue->flags, K_WORK_QUEUE_BUSY_BIT);
			work = CONTAINER_OF(node, struct k_work, node);
			flag_set(&work->flags, K_WORK_RUNNING_BIT);
			flag_clear(&work->flags, K_WORK_QUEUED_BIT);

			/* Static code analysis tool can raise a false-positive violation
			 * in the line below that 'work' is checked for null after being
			 * dereferenced.
			 *
			 * The work is figured out by CONTAINER_OF, as a container
			 * of type struct k_work that contains the node.
			 * The only way for it to be NULL is if node would be a member
			 * of struct k_work object that has been placed at address NULL,
			 * which should never happen, even line 'if (work != NULL)'
			 * ensures that.
			 * This means that if node is not NULL, then work will not be NULL.
			 */
			handler = work->handler;
		} else if (flag_test_and_clear(&queue->flags,
					       K_WORK_QUEUE_DRAIN_BIT)) {
			/* Not busy and draining: move threads waiting for
			 * drain to ready state.  The held spinlock inhibits
			 * immediate reschedule; released threads get their
			 * chance when this invokes z_sched_wait() below.
			 *
			 * We don't touch K_WORK_QUEUE_PLUGGABLE, so getting
			 * here doesn't mean that the queue will allow new
			 * submissions.
			 */
			/* (void)z_sched_wake_all(&queue->drainq, 1, NULL); */
		} else {
			/* No work is available and no queue state requires
			 * special handling.
			 */
			;
		}
		k_spin_unlock(&lock, key);

		__ASSERT_NO_MSG(handler != NULL);
		handler(work);

		/* Mark the work item as no longer running and deal
		 * with any cancellation issued while it was running.
		 * Clear the BUSY flag and optionally yield to prevent
		 * starving other threads.
		 */
		key = k_spin_lock(&lock);

		flag_clear(&work->flags, K_WORK_RUNNING_BIT);
		if (flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
			flag_clear(&work->flags, K_WORK_CANCELING_BIT);
		}

		flag_clear(&queue->flags, K_WORK_QUEUE_BUSY_BIT);
		yield = !flag_test(&queue->flags, K_WORK_QUEUE_NO_YIELD_BIT);
		k_spin_unlock(&lock, key);

		/* Optionally yield to prevent the work queue from
		 * starving other threads.
		 */
		if (yield) {
			/* k_yield(); */
		}
	}
}

void k_work_queue_init(struct k_work_q *queue)
{
	__ASSERT_NO_MSG(queue != NULL);

	if (flag_test(&queue->flags, K_WORK_QUEUE_INITED_BIT))
	{
		return;
	}

	*queue = (struct k_work_q) {
		.flags = 0,
	};

	flag_set(&queue->flags, K_WORK_QUEUE_INITED_BIT);

	SYS_PORT_TRACING_OBJ_INIT(k_work_queue, queue);
}

void k_work_queue_start(struct k_work_q *queue,
			k_thread_stack_t *stack, size_t stack_size,
			int prio, const struct k_work_queue_config *cfg)
{
	__ASSERT_NO_MSG(queue);
	__ASSERT_NO_MSG(stack);
	__ASSERT_NO_MSG(!flag_test(&queue->flags, K_WORK_QUEUE_STARTED_BIT));

	uint32_t flags = K_WORK_QUEUE_STARTED;
	const char *name = "work_queue_main";

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work_queue, start, queue);

	if (flag_test(&queue->flags, K_WORK_QUEUE_STARTED_BIT))
	{
		return;
	}

	flag_set(&queue->flags, K_WORK_QUEUE_STARTED_BIT);

	k_queue_init(&queue->pending);

	if ((cfg != NULL) && cfg->no_yield) {
		flags |= K_WORK_QUEUE_NO_YIELD;
	}

	/* It hasn't actually been started yet, but all the state is in place
	 * so we can submit things and once the thread gets control it's ready
	 * to roll.
	 */
	flags_set(&queue->flags, flags);

	/*
	* queue->thread.handle = xTaskCreateStatic(work_queue_main,
	*                                          name, stack_size/sizeof(StackType_t),
	*                                          queue, prio,
	*                                          (StackType_t * const)stack,
	*                                          &queue->thread.task_buffer);
	*/

	k_thread_create(&queue->thread, stack,
					stack_size,
					work_queue_main,
					queue, NULL, NULL,
					prio, 0, K_NO_WAIT);

	if ((NULL != cfg) && (NULL != cfg->name))
	{
		name = cfg->name;
		k_thread_name_set(&queue->thread, name);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work_queue, start, queue);
}

int k_work_queue_drain(struct k_work_q *queue, bool plug)
{
	while (!k_queue_is_empty(&queue->pending)) {
		k_sleep(1);
	}
	if (plug == true) {
		k_thread_suspend(&queue->thread);
	}
	return 0;
}

int k_work_queue_unplug(struct k_work_q *queue)
{
	k_thread_resume(&queue->thread);
	return 0;
}

void k_work_init_delayable(struct k_work_delayable *dwork,
			   k_work_handler_t handler)
{
	__ASSERT_NO_MSG(dwork != NULL);
	__ASSERT_NO_MSG(handler != NULL);

	*dwork = (struct k_work_delayable){
		.work = {
			.handler = handler,
			.flags = K_WORK_DELAYABLE,
		},
	};

	SYS_PORT_TRACING_OBJ_INIT(k_work_delayable, dwork);
}

int k_work_delayable_busy_get(const struct k_work_delayable *dwork)
{
	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret = flags_get(&dwork->work.flags) & K_WORK_MASK;

	k_spin_unlock(&lock, key);
	return ret;
}

static void k_work_delay_queue_main( TimerHandle_t xTimer )
{
	struct k_work_delayable *dwork = NULL;
	struct k_work *work = NULL;
	struct k_work *temp = NULL;
	struct k_work_q *queue = NULL;
	BaseType_t ret;
	uint32_t tick;
	uint32_t tick_delta;

	k_spinlock_key_t key = k_spin_lock(&lock);

	tick = k_uptime_get_32();

	tick_delta = tick - k_sys_work_delay_q.tick;

	k_sys_work_delay_q.tick = tick;

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&k_sys_work_delay_q.pending, work, temp, node)
	{
		dwork = CONTAINER_OF(work, struct k_work_delayable, work);

		if (dwork->timeout.count > tick_delta)
		{
			dwork->timeout.count = dwork->timeout.count - tick_delta;
		}
		else
		{
			/* Timeout */
			dwork->timeout.count = 0;

			/* If the work is still marked delayed (should be) then clear that
			* state and submit it to the queue.  If successful the queue will be
			* notified of new work at the next reschedule point.
			*
			* If not successful there is no notification that the work has been
			* abandoned.  Sorry.
			*/
			if (flag_test_and_clear(&work->flags, K_WORK_DELAYED_BIT))
			{
				queue = dwork->queue;
				sys_slist_find_and_remove(&k_sys_work_delay_q.pending, &dwork->work.node);
				k_work_submit_to_queue(queue, work);
			}
		}
	}

	if (bt_list_is_empty(&k_sys_work_delay_q.pending))
	{
		if (atomic_test_and_clear_bit(&k_sys_work_delay_q.flags, K_WORK_DELAY_Q_TIMER)) {
			/* k_spin_unlock(&lock, key); */
			ret = xTimerStop(xTimer, (TickType_t)1000);
			assert(pdPASS == ret);
			(void)ret;
			/* key = k_spin_lock(&lock); */
		}
	}

	k_spin_unlock(&lock, key);
}

static void k_work_delay_queue_init()
{
	k_spinlock_key_t key = k_spin_lock(&lock);

	if (NULL == k_sys_work_delay_q.timer)
	{
		k_sys_work_delay_q.timer = xTimerCreateStatic("Delay_queue", 10, pdTRUE, &k_sys_work_delay_q, k_work_delay_queue_main, &k_sys_work_delay_q.timer_buffer);

		sys_slist_init(&k_sys_work_delay_q.pending);

		k_sys_work_delay_q.tick = 0;
	}
	k_spin_unlock(&lock, key);
}

static void k_work_delay_queue_append(struct k_work_delayable *dwork, k_timeout_t delay)
{
	bool is_empty;
	BaseType_t ret;

	k_work_delay_queue_init();

	k_spinlock_key_t key = k_spin_lock(&lock);

	dwork->timeout.count = delay;
	is_empty = bt_list_is_empty(&k_sys_work_delay_q.pending);
	sys_slist_append(&k_sys_work_delay_q.pending, &dwork->work.node);

	k_spin_unlock(&lock, key);

	if (!atomic_test_and_set_bit(&k_sys_work_delay_q.flags, K_WORK_DELAY_Q_TIMER))
	{
		ret = xTimerStart(k_sys_work_delay_q.timer, (TickType_t)10);
		assert(pdPASS == ret);
		if (pdPASS == ret)
		{
			k_sys_work_delay_q.tick = k_uptime_get_32();
		}
	}
	(void)is_empty;
}

int k_work_schedule_for_queue(struct k_work_q *queue,
			       struct k_work_delayable *dwork,
			       k_timeout_t delay)
{
	__ASSERT_NO_MSG(dwork != NULL);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, schedule_for_queue, queue, dwork, delay);

	struct k_work *work = &dwork->work;
	int ret = -EBUSY;
	k_spinlock_key_t key = k_spin_lock(&lock);

	/* Schedule the work item if it's idle or running. */
	if (((flags_get(&work->flags) & K_WORK_MASK) & ~K_WORK_RUNNING) == 0U) {
		if (K_TIMEOUT_EQ(delay, K_NO_WAIT))
		{
			ret = k_work_submit_to_queue(queue, work);
		}
		else
		{
			flag_set(&work->flags, K_WORK_DELAYED_BIT);
			dwork->queue = queue;

			k_work_delay_queue_append(dwork, delay);
			ret = 0;
		}
	}

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, schedule_for_queue, queue, dwork, delay, ret);

	return ret;
}

int k_work_schedule(struct k_work_delayable *dwork,
				   k_timeout_t delay)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, schedule, dwork, delay);

	_sys_work_queue_init();

	int ret = k_work_schedule_for_queue(&k_sys_work_q, dwork, delay);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, schedule, dwork, delay, ret);

	return ret;
}

int k_work_reschedule_for_queue(struct k_work_q *queue,
				 struct k_work_delayable *dwork,
				 k_timeout_t delay)
{
	__ASSERT_NO_MSG(dwork != NULL);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, reschedule_for_queue, queue, dwork, delay);

	k_spinlock_key_t key = k_spin_lock(&lock);
	struct k_work *work = &dwork->work;
	int ret;

	/* If scheduled, try to cancel.  If it fails, that means the
	 * callback has been dequeued and will inevitably run (or has
	 * already run), so treat that as "undelayed" and return
	 * false.
	 */
	if (flag_test_and_clear(&work->flags, K_WORK_DELAYED_BIT)) {
		sys_slist_find_and_remove(&k_sys_work_delay_q.pending, &dwork->work.node);
	}

	ret = k_work_schedule_for_queue(queue, dwork, delay);

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, reschedule_for_queue, queue, dwork, delay, ret);

	return ret;
}

int k_work_reschedule(struct k_work_delayable *dwork,
				     k_timeout_t delay)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, reschedule, dwork, delay);

	_sys_work_queue_init();

	int ret = k_work_reschedule_for_queue(&k_sys_work_q, dwork, delay);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, reschedule, dwork, delay, ret);

	return ret;
}

bool k_work_flush_delayable(struct k_work_delayable *dwork,
			    struct k_work_sync *sync)
{
	return false;
}

int k_work_cancel_delayable(struct k_work_delayable *dwork)
{
	__ASSERT_NO_MSG(dwork != NULL);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel_delayable, dwork);

	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret;
	struct k_work *work = &dwork->work;

	/* If scheduled, try to cancel.  If it fails, that means the
	 * callback has been dequeued and will inevitably run (or has
	 * already run), so treat that as "undelayed" and return
	 * false.
	 */
	if (flag_test_and_clear(&work->flags, K_WORK_DELAYED_BIT)) {
		sys_slist_find_and_remove(&k_sys_work_delay_q.pending, &dwork->work.node);
	}

	/* If we haven't already started canceling, do it now. */
	if (!flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
		/* Remove it from the queue, if it's queued. */
        	if (flag_test_and_clear(&work->flags, K_WORK_QUEUED_BIT)) {
			(void)k_queue_remove(&work->queue->pending, &work->node);
		}
	}

	if (flags_get(&work->flags) & K_WORK_MASK)
	{
		flag_set(&work->flags, K_WORK_CANCELING_BIT);
	}

	ret = (flags_get(&work->flags) & K_WORK_MASK);

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel_delayable, dwork, ret);

	return ret;
}

bool k_work_cancel_delayable_sync(struct k_work_delayable *dwork,
				  struct k_work_sync *sync)
{
	__ASSERT_NO_MSG(dwork != NULL);
	__ASSERT_NO_MSG(sync != NULL);
	__ASSERT_NO_MSG(!k_is_in_isr());

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel_delayable_sync, dwork, sync);

	k_work_cancel_delayable(dwork);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel_delayable_sync, dwork, sync, pending);

	return true;
}

k_ticks_t k_work_delayable_timeout_expires(const struct k_work_delayable *dwork)
{
	k_ticks_t tick = 0U;
	k_spinlock_key_t key = k_spin_lock(&lock);

	if (flag_test(&dwork->work.flags, K_WORK_DELAYED_BIT)) {
		tick = k_sys_work_delay_q.tick + dwork->timeout.count;
	}

	k_spin_unlock(&lock, key);

	return tick;
}

k_ticks_t k_work_delayable_timeout_remaining(const struct k_work_delayable *dwork)
{
	k_ticks_t tick = 0U;
	k_spinlock_key_t key = k_spin_lock(&lock);

	if (flag_test(&dwork->work.flags, K_WORK_DELAYED_BIT)) {
		tick = dwork->timeout.count;
	}

	k_spin_unlock(&lock, key);

	return tick;
}
