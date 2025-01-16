
/*
 * Copyright 2023 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>

static void poll_event_update(struct k_poll_event *event);
static void poller_init(void);

static struct z_poller poller = K_POLL_INITIALIZER(NULL);

static struct k_spinlock lock;

static void sem_post(QueueHandle_t sem)
{
    if (k_is_in_isr())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreGiveFromISR(sem, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
        }
        else
        {
        }
    }
    else
    {
        if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreGive(sem))
        {
        }
        else
        {
        }
    }
}

static void poll_event_update(struct k_poll_event *event)
{
    if (event->poller != &poller)
    {
        return;
    }

    if (NULL != poller.sem)
    {
        sem_post(poller.sem);
    }
}

static void poller_init(void)
{
    if (NULL == poller.sem)
    {
        poller.sem = xSemaphoreCreateCountingStatic(0xFFFFU, 0, &poller.sem_buffer);
    }
    if (NULL == poller.event_update)
    {
        poller.event_update = poll_event_update;
    }
}

/**
 * @brief Initialize one struct k_poll_event instance
 *
 * After this routine is called on a poll event, the event it ready to be
 * placed in an event array to be passed to k_poll().
 *
 * @param event The event to initialize.
 * @param type A bitfield of the types of event, from the K_POLL_TYPE_xxx
 *             values. Only values that apply to the same object being polled
 *             can be used together. Choosing K_POLL_TYPE_IGNORE disables the
 *             event.
 * @param mode Future. Use K_POLL_MODE_NOTIFY_ONLY.
 * @param obj Kernel object or poll signal.
 */

void k_poll_event_init(struct k_poll_event *event, uint32_t type,
			      int mode, void *obj)
{
	__ASSERT(mode == K_POLL_MODE_NOTIFY_ONLY,
		 "only NOTIFY_ONLY mode is supported\n");
	__ASSERT(type < (BIT(_POLL_NUM_TYPES)), "invalid type\n");
	__ASSERT(obj != NULL, "must provide an object\n");

    event->type = type;
    event->mode = mode;
    event->state = K_POLL_STATE_NOT_READY;
    event->obj = obj;
}

/* must be called with interrupts locked */
static inline bool is_condition_met(struct k_poll_event *event, uint32_t *state)
{
	switch (event->type) {
	case K_POLL_TYPE_SEM_AVAILABLE:
		if (k_sem_count_get(event->sem) > 0U) {
			*state = K_POLL_STATE_SEM_AVAILABLE;
			return true;
		}
		break;
	case K_POLL_TYPE_DATA_AVAILABLE:
		if (!k_queue_is_empty(event->queue)) {
			*state = K_POLL_STATE_FIFO_DATA_AVAILABLE;
			return true;
		}
		break;
	case K_POLL_TYPE_SIGNAL:
		if (event->signal->signaled != 0U) {
			*state = K_POLL_STATE_SIGNALED;
			return true;
		}
		break;
#if 0
	case K_POLL_TYPE_MSGQ_DATA_AVAILABLE:
		if (event->msgq->used_msgs > 0) {
			*state = K_POLL_STATE_MSGQ_DATA_AVAILABLE;
			return true;
		}
		break;
#ifdef CONFIG_PIPES
	case K_POLL_TYPE_PIPE_DATA_AVAILABLE:
		if (k_pipe_read_avail(event->pipe)) {
			*state = K_POLL_STATE_PIPE_DATA_AVAILABLE;
			return true;
		}
#endif
#endif
	case K_POLL_TYPE_IGNORE:
		break;
	default:
		__ASSERT(false, "invalid event type (0x%x)\n", event->type);
		break;
	}

	return false;
}

static inline void add_event(sys_slist_t *events, struct k_poll_event *event,
			     struct z_poller *poller)
{
    #if 0
	struct k_poll_event *pending;

	pending = (struct k_poll_event *)sys_slist_peek_tail(events);
	if ((pending == NULL) ||
		(z_sched_prio_cmp(poller_thread(pending->poller),
							   poller_thread(poller)) > 0)) {
		sys_dlist_append(events, &event->_node);
		return;
	}

	SYS_DLIST_FOR_EACH_CONTAINER(events, pending, _node) {
		if (z_sched_prio_cmp(poller_thread(poller),
					poller_thread(pending->poller)) > 0) {
			sys_dlist_insert(&pending->_node, &event->_node);
			return;
		}
	}
    #endif
	sys_slist_append(events, &event->_node);
}

static inline void remove_event(sys_slist_t *events, struct k_poll_event *event,
			     struct z_poller *poller)
{
    #if 0
	struct k_poll_event *pending;

	pending = (struct k_poll_event *)sys_slist_peek_tail(events);
	if ((pending == NULL) ||
		(z_sched_prio_cmp(poller_thread(pending->poller),
							   poller_thread(poller)) > 0)) {
		sys_dlist_append(events, &event->_node);
		return;
	}

	SYS_DLIST_FOR_EACH_CONTAINER(events, pending, _node) {
		if (z_sched_prio_cmp(poller_thread(poller),
					poller_thread(pending->poller)) > 0) {
			sys_dlist_insert(&pending->_node, &event->_node);
			return;
		}
	}
    #endif
	sys_slist_find_and_remove(events, &event->_node);
}

/* must be called with interrupts locked */
static inline void register_event(struct k_poll_event *event,
				 struct z_poller *poller)
{
	switch (event->type) {
	case K_POLL_TYPE_SEM_AVAILABLE:
		__ASSERT(event->sem != NULL, "invalid semaphore\n");
		add_event(&event->sem->poll_events, event, poller);
		break;
	case K_POLL_TYPE_DATA_AVAILABLE:
		__ASSERT(event->queue != NULL, "invalid queue\n");
		add_event(&event->queue->poll_events, event, poller);
		break;
	case K_POLL_TYPE_SIGNAL:
		__ASSERT(event->signal != NULL, "invalid poll signal\n");
		add_event(&event->signal->poll_events, event, poller);
		break;
#if 0
	case K_POLL_TYPE_MSGQ_DATA_AVAILABLE:
		__ASSERT(event->msgq != NULL, "invalid message queue\n");
		add_event(&event->msgq->poll_events, event, poller);
		break;
#ifdef CONFIG_PIPES
	case K_POLL_TYPE_PIPE_DATA_AVAILABLE:
		__ASSERT(event->pipe != NULL, "invalid pipe\n");
		add_event(&event->pipe->poll_events, event, poller);
		break;
#endif
#endif
	case K_POLL_TYPE_IGNORE:
		/* nothing to do */
		break;
	default:
		__ASSERT(false, "invalid event type\n");
		break;
	}

	event->poller = poller;
}

static inline void set_event_ready(struct k_poll_event *event, uint32_t state)
{
	event->poller = NULL;
	event->state |= state;
}

static inline int register_events(struct k_poll_event *events,
				  int num_events,
				  struct z_poller *poller,
				  bool just_check)
{
	int events_registered = 0;

	for (int ii = 0; ii < num_events; ii++) {
		k_spinlock_key_t key;
		uint32_t state;

		key = k_spin_lock(&lock);
		if (is_condition_met(&events[ii], &state)) {
			set_event_ready(&events[ii], state);
			poller->is_polling = false;
		} else if (!just_check && poller->is_polling) {
			register_event(&events[ii], poller);
			events_registered += 1;
		} else {
			/* Event is not one of those identified in is_condition_met()
			 * catching non-polling events, or is marked for just check,
			 * or not marked for polling. No action needed.
			 */
			;
		}
		k_spin_unlock(&lock, key);
	}

	return events_registered;
}

/* must be called with interrupts locked */
static inline void clear_event_registration(struct k_poll_event *event)
{
    struct z_poller *poller = event->poller;

	event->poller = NULL;

	switch (event->type) {
	case K_POLL_TYPE_SEM_AVAILABLE:
		__ASSERT(event->sem != NULL, "invalid semaphore\n");
		remove_event(&event->sem->poll_events, event, poller);
		break;
	case K_POLL_TYPE_DATA_AVAILABLE:
		__ASSERT(event->queue != NULL, "invalid queue\n");
		remove_event(&event->queue->poll_events, event, poller);
		break;
	case K_POLL_TYPE_SIGNAL:
		__ASSERT(event->signal != NULL, "invalid poll signal\n");
		remove_event(&event->signal->poll_events, event, poller);
		break;
#if 0
	case K_POLL_TYPE_MSGQ_DATA_AVAILABLE:
		__ASSERT(event->msgq != NULL, "invalid message queue\n");
		break;
#ifdef CONFIG_PIPES
	case K_POLL_TYPE_PIPE_DATA_AVAILABLE:
		__ASSERT(event->pipe != NULL, "invalid pipe\n");
		break;
#endif
#endif
	case K_POLL_TYPE_IGNORE:
		/* nothing to do */
		break;
	default:
		__ASSERT(false, "invalid event type\n");
		break;
	}
}

/* must be called with interrupts locked */
static inline void clear_event_registrations(struct k_poll_event *events,
					      int num_events,
					      k_spinlock_key_t key)
{
	while (num_events--) {
		clear_event_registration(&events[num_events]);
		k_spin_unlock(&lock, key);
		key = k_spin_lock(&lock);
	}
}

/**
 * @brief Wait for one or many of multiple poll events to occur
 *
 * This routine allows a thread to wait concurrently for one or many of
 * multiple poll events to have occurred. Such events can be a kernel object
 * being available, like a semaphore, or a poll signal event.
 *
 * When an event notifies that a kernel object is available, the kernel object
 * is not "given" to the thread calling k_poll(): it merely signals the fact
 * that the object was available when the k_poll() call was in effect. Also,
 * all threads trying to acquire an object the regular way, i.e. by pending on
 * the object, have precedence over the thread polling on the object. This
 * means that the polling thread will never get the poll event on an object
 * until the object becomes available and its pend queue is empty. For this
 * reason, the k_poll() call is more effective when the objects being polled
 * only have one thread, the polling thread, trying to acquire them.
 *
 * When k_poll() returns 0, the caller should loop on all the events that were
 * passed to k_poll() and check the state field for the values that were
 * expected and take the associated actions.
 *
 * Before being reused for another call to k_poll(), the user has to reset the
 * state field to K_POLL_STATE_NOT_READY.
 *
 * When called from user mode, a temporary memory allocation is required from
 * the caller's resource pool.
 *
 * @param events An array of events to be polled for.
 * @param num_events The number of events in the array.
 * @param timeout Waiting period for an event to be ready,
 *                or one of the special values K_NO_WAIT and K_FOREVER.
 *
 * @retval 0 One or more events are ready.
 * @retval -EAGAIN Waiting period timed out.
 * @retval -EINTR Polling has been interrupted, e.g. with
 *         k_queue_cancel_wait(). All output events are still set and valid,
 *         cancelled event(s) will be set to K_POLL_STATE_CANCELLED. In other
 *         words, -EINTR status means that at least one of output events is
 *         K_POLL_STATE_CANCELLED.
 * @retval -ENOMEM Thread resource pool insufficient memory (user mode only)
 * @retval -EINVAL Bad parameters (user mode only)
 */

int k_poll(struct k_poll_event *events, int num_events,
		     k_timeout_t timeout)
{
	int events_registered;
	k_spinlock_key_t key;

    __ASSERT(!k_is_in_isr(), "");
	__ASSERT(events != NULL, "NULL events\n");
	__ASSERT(num_events >= 0, "<0 events\n");

    if ((k_is_in_isr()) || (events == NULL) || (0 == num_events))
    {
        return -EINVAL;
    }

    poller_init();

    poller.mode = MODE_POLL;
    poller.is_polling = true;

	SYS_PORT_TRACING_FUNC_ENTER(k_poll_api, poll, events);

	events_registered = register_events(events, num_events, &poller,
					    K_TIMEOUT_EQ(timeout, K_NO_WAIT));

	key = k_spin_lock(&lock);

	/*
	 * If we're not polling anymore, it means that at least one event
	 * condition is met, either when looping through the events here or
	 * because one of the events registered has had its state changed.
	 */
	if (!poller.is_polling) {
		clear_event_registrations(events, events_registered, key);
		k_spin_unlock(&lock, key);

		SYS_PORT_TRACING_FUNC_EXIT(k_poll_api, poll, events, 0);

		return 0;
	}

	poller.is_polling = false;

	if (K_TIMEOUT_EQ(timeout, K_NO_WAIT)) {
		k_spin_unlock(&lock, key);

		SYS_PORT_TRACING_FUNC_EXIT(k_poll_api, poll, events, -EAGAIN);

		return -EAGAIN;
	}

    k_spin_unlock(&lock, key);

    if (NULL != poller.sem)
    {
        (void)xSemaphoreTake(poller.sem, timeout);
    }

    SYS_PORT_TRACING_FUNC_EXIT(k_poll_api, poll, events, swap_rc);

    return 0;
}

/**
 * @brief Initialize a poll signal object.
 *
 * Ready a poll signal object to be signaled via k_poll_signal_raise().
 *
 * @param sig A poll signal.
 */

void k_poll_signal_init(struct k_poll_signal *sig)
{
    sys_slist_init(&sig->poll_events);

    sig->result = 0;
    sig->signaled = 0U;

	SYS_PORT_TRACING_FUNC(k_poll_api, signal_init, sig);
}

/*
 * @brief Reset a poll signal object's state to unsignaled.
 *
 * @param sig A poll signal object
 */
void k_poll_signal_reset(struct k_poll_signal *sig)
{
    sig->signaled = 0U;

	SYS_PORT_TRACING_FUNC(k_poll_api, signal_reset, sig);
}

/**
 * @brief Fetch the signaled state and result value of a poll signal
 *
 * @param sig A poll signal object
 * @param signaled An integer buffer which will be written nonzero if the
 *		   object was signaled
 * @param result An integer destination buffer which will be written with the
 *		   result value if the object was signaled, or an undefined
 *		   value if it was not.
 */
void k_poll_signal_check(struct k_poll_signal *sig,
				   unsigned int *signaled, int *result)
{
	*signaled = sig->signaled;
	*result = sig->result;

	SYS_PORT_TRACING_FUNC(k_poll_api, signal_check, sig);
}

static bool slist_scan_callback(void* node, void *param);
static bool slist_scan_callback(void* node, void *param)
{
    struct k_poll_event *poll_event = CONTAINER_OF(node, struct k_poll_event, _node);

    if (NULL == poll_event)
    {
        return false;
    }

    if (NULL == poll_event->poller)
    {
        return false;
    }

    if (NULL == poll_event->poller->event_update)
    {
        return false;
    }

    poll_event->poller->event_update(poll_event);

    return false;
}

/**
 * @brief Signal a poll signal object.
 *
 * This routine makes ready a poll signal, which is basically a poll event of
 * type K_POLL_TYPE_SIGNAL. If a thread was polling on that event, it will be
 * made ready to run. A @a result value can be specified.
 *
 * The poll signal contains a 'signaled' field that, when set by
 * k_poll_signal_raise(), stays set until the user sets it back to 0 with
 * k_poll_signal_reset(). It thus has to be reset by the user before being
 * passed again to k_poll() or k_poll() will consider it being signaled, and
 * will return immediately.
 *
 * @note The result is stored and the 'signaled' field is set even if
 * this function returns an error indicating that an expiring poll was
 * not notified.  The next k_poll() will detect the missed raise.
 *
 * @param sig A poll signal.
 * @param result The value to store in the result field of the signal.
 *
 * @retval 0 The signal was delivered successfully.
 * @retval -EAGAIN The polling thread's timeout is in the process of expiring.
 */

int k_poll_signal_raise(struct k_poll_signal *sig, int result)
{
    sig->result = result;
	sig->signaled = 1U;

    bt_list_scan(&sig->poll_events, slist_scan_callback, sig);

    return 0;
}
