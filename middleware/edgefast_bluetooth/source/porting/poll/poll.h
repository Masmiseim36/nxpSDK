/*
 * Copyright 2024-2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BT_BLE_POLL_H__
#define __BT_BLE_POLL_H__

#include <system_c.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _POLL_EVENT_OBJ_INIT(obj) \
	.poll_events = {NULL},
#define _POLL_EVENT sys_slist_t poll_events

struct z_poller;

/* polling API - PRIVATE */

#ifdef CONFIG_POLL
#define _INIT_OBJ_POLL_EVENT(obj) do { (obj)->poll_event = NULL; } while (false)
#else
#define _INIT_OBJ_POLL_EVENT(obj) do { } while (false)
#endif

/* private - types bit positions */
enum _poll_types_bits {
	/* can be used to ignore an event */
	_POLL_TYPE_IGNORE,

	/* to be signaled by k_poll_signal_raise() */
	_POLL_TYPE_SIGNAL,

	/* semaphore availability */
	_POLL_TYPE_SEM_AVAILABLE,

	/* queue/FIFO/LIFO data availability */
	_POLL_TYPE_DATA_AVAILABLE,

	/* msgq data availability */
	_POLL_TYPE_MSGQ_DATA_AVAILABLE,

	/* pipe data availability */
	_POLL_TYPE_PIPE_DATA_AVAILABLE,

	_POLL_NUM_TYPES
};

#define Z_POLL_TYPE_BIT(type) (1U << ((type) - 1U))

/* private - states bit positions */
enum _poll_states_bits {
	/* default state when creating event */
	_POLL_STATE_NOT_READY,

	/* signaled by k_poll_signal_raise() */
	_POLL_STATE_SIGNALED,

	/* semaphore is available */
	_POLL_STATE_SEM_AVAILABLE,

	/* data is available to read on queue/FIFO/LIFO */
	_POLL_STATE_DATA_AVAILABLE,

	/* queue/FIFO/LIFO wait was cancelled */
	_POLL_STATE_CANCELLED,

	/* data is available to read on a message queue */
	_POLL_STATE_MSGQ_DATA_AVAILABLE,

	/* data is available to read from a pipe */
	_POLL_STATE_PIPE_DATA_AVAILABLE,

	_POLL_NUM_STATES
};

#define Z_POLL_STATE_BIT(state) (1U << ((state) - 1U))

#define _POLL_EVENT_NUM_UNUSED_BITS \
	(32 - (0 \
	       + 8 /* tag */ \
	       + _POLL_NUM_TYPES \
	       + _POLL_NUM_STATES \
	       + 1 /* modes */ \
	      ))

/* end of polling API - PRIVATE */

/**
 * @defgroup poll_apis Async polling APIs
 * @ingroup kernel_apis
 * @{
 */

/* Public polling API */


/* public - values for k_poll_event.type bitfield */
#define K_POLL_TYPE_IGNORE 0
#define K_POLL_TYPE_SIGNAL Z_POLL_TYPE_BIT(_POLL_TYPE_SIGNAL)
#define K_POLL_TYPE_SEM_AVAILABLE Z_POLL_TYPE_BIT(_POLL_TYPE_SEM_AVAILABLE)
#define K_POLL_TYPE_DATA_AVAILABLE Z_POLL_TYPE_BIT(_POLL_TYPE_DATA_AVAILABLE)
#define K_POLL_TYPE_FIFO_DATA_AVAILABLE K_POLL_TYPE_DATA_AVAILABLE
#define K_POLL_TYPE_MSGQ_DATA_AVAILABLE Z_POLL_TYPE_BIT(_POLL_TYPE_MSGQ_DATA_AVAILABLE)
#define K_POLL_TYPE_PIPE_DATA_AVAILABLE Z_POLL_TYPE_BIT(_POLL_TYPE_PIPE_DATA_AVAILABLE)

/* public - polling modes */
enum k_poll_modes {
	/* polling thread does not take ownership of objects when available */
	K_POLL_MODE_NOTIFY_ONLY = 0,

	K_POLL_NUM_MODES
};

/* public - values for k_poll_event.state bitfield */
#define K_POLL_STATE_NOT_READY 0
#define K_POLL_STATE_SIGNALED Z_POLL_STATE_BIT(_POLL_STATE_SIGNALED)
#define K_POLL_STATE_SEM_AVAILABLE Z_POLL_STATE_BIT(_POLL_STATE_SEM_AVAILABLE)
#define K_POLL_STATE_DATA_AVAILABLE Z_POLL_STATE_BIT(_POLL_STATE_DATA_AVAILABLE)
#define K_POLL_STATE_FIFO_DATA_AVAILABLE K_POLL_STATE_DATA_AVAILABLE
#define K_POLL_STATE_MSGQ_DATA_AVAILABLE Z_POLL_STATE_BIT(_POLL_STATE_MSGQ_DATA_AVAILABLE)
#define K_POLL_STATE_PIPE_DATA_AVAILABLE Z_POLL_STATE_BIT(_POLL_STATE_PIPE_DATA_AVAILABLE)
#define K_POLL_STATE_CANCELLED Z_POLL_STATE_BIT(_POLL_STATE_CANCELLED)

/* public - poll signal object */
struct k_poll_signal {
	/** PRIVATE - DO NOT TOUCH */
	_POLL_EVENT;

	/**
	 * 1 if the event has been signaled, 0 otherwise. Stays set to 1 until
	 * user resets it to 0.
	 */
	unsigned int signaled;

	/** custom result value passed to k_poll_signal_raise() if needed */
	int result;
};

#define K_POLL_SIGNAL_INITIALIZER(obj) \
	{ \
	_POLL_EVENT_OBJ_INIT(obj) \
	.signaled = 0, \
	.result = 0, \
	}

struct k_poll_event {
	/** PRIVATE - DO NOT TOUCH */
	sys_snode_t _node;

	/** PRIVATE - DO NOT TOUCH */
	struct z_poller *poller;

	/** optional user-specified tag, opaque, untouched by the API */
	uint32_t tag:8;

	/** bitfield of event types (bitwise-ORed K_POLL_TYPE_xxx values) */
	uint32_t type:_POLL_NUM_TYPES;

	/** bitfield of event states (bitwise-ORed K_POLL_STATE_xxx values) */
	uint32_t state:_POLL_NUM_STATES;

	/** mode of operation, from enum k_poll_modes */
	uint32_t mode:1;

	/** unused bits in 32-bit word */
	uint32_t unused:_POLL_EVENT_NUM_UNUSED_BITS;

	/** per-type data */
	union {
		void *obj;
		struct k_poll_signal *signal;
		struct k_sem *sem;
		struct k_fifo *fifo;
		struct k_queue *queue;
	};
};

typedef void (*poll_event_update_t)(struct k_poll_event *event);

enum POLL_MODE { MODE_NONE, MODE_POLL, MODE_TRIGGERED };

struct z_poller
{
	/** PRIVATE - DO NOT TOUCH */
    QueueHandle_t sem;

	/** PRIVATE - DO NOT TOUCH */
    StaticSemaphore_t  sem_buffer;

    /** PRIVATE - DO NOT TOUCH */
    bool is_polling;

    /** PRIVATE - DO NOT TOUCH */
    enum POLL_MODE mode;

    /* event update callback */
    poll_event_update_t event_update;
};

#define K_POLL_INITIALIZER(cb) \
	{                          \
	.sem = NULL,               \
	.sem_buffer = {{0}},         \
    .is_polling = false,       \
    .mode = MODE_NONE,         \
	.event_update = cb,        \
	}


#define K_POLL_EVENT_INITIALIZER(_event_type, _event_mode, _event_obj) \
	{ \
	.poller = NULL, \
	.type = _event_type, \
	.state = K_POLL_STATE_NOT_READY, \
	.mode = _event_mode, \
	.unused = 0, \
	{ \
		.obj = _event_obj, \
	}, \
	}

#define K_POLL_EVENT_STATIC_INITIALIZER(_event_type, _event_mode, _event_obj, \
					event_tag) \
	{ \
	.tag = event_tag, \
	.type = _event_type, \
	.state = K_POLL_STATE_NOT_READY, \
	.mode = _event_mode, \
	.unused = 0, \
	{ \
		.obj = _event_obj, \
	}, \
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

extern void k_poll_event_init(struct k_poll_event *event, uint32_t type,
			      int mode, void *obj);

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

__syscall int k_poll(struct k_poll_event *events, int num_events,
		     k_timeout_t timeout);

/**
 * @brief Initialize a poll signal object.
 *
 * Ready a poll signal object to be signaled via k_poll_signal_raise().
 *
 * @param sig A poll signal.
 */

__syscall void k_poll_signal_init(struct k_poll_signal *sig);

/*
 * @brief Reset a poll signal object's state to unsignaled.
 *
 * @param sig A poll signal object
 */
__syscall void k_poll_signal_reset(struct k_poll_signal *sig);

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
__syscall void k_poll_signal_check(struct k_poll_signal *sig,
				   unsigned int *signaled, int *result);

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

__syscall int k_poll_signal_raise(struct k_poll_signal *sig, int result);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __BT_BLE_POLL_H__ */
