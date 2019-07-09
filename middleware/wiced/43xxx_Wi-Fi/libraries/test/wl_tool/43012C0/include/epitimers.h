/*
 * A few os-independent timer macros considered useful by some.
 *
 * Copyright 1998-1999 Epigram, Inc.
 *
 * $Copyright Cypress Semiconductor$
 *
 * $Id: epitimers.h 241182 2011-02-17 21:50:03Z gmo $
 */

#ifndef _EPITIMERS_H_
#define _EPITIMERS_H_

/*
 * the basic type used for arq timers, support defs below
 */
#if !SHORT_TIMERS

typedef uint tstamp_t;
typedef int _ststamp_t;

#define NO_TIMER (0)
#define TIMER_WRAP_MASK (0x80000000)
#define IL_INFINITE (TIMER_WRAP_MASK)
#define TSTAMP2UINT(stamp) ((uint)(stamp))

#else /* SHORT_TIMERS */

typedef uint16 tstamp_t;
typedef int16  _ststamp_t;

#define NO_TIMER (0)
#define TIMER_WRAP_MASK (0x8000)
#define IL_INFINITE (0xffff)
#define _STAMP_MASK 0xffff
#define TSTAMP2UINT(stamp) ((uint)((stamp) & _STAMP_MASK))

#endif /* !SHORT_TIMERS */
/*
 * Stuff for timer support
 *
 * A timer is off if its value is 0.
 *   A zero result for (now + interval) is always replaced with a 1
 *   Note that the routine that returns the current time in milliseconds
 *      must never return 0 as a value.
 */

#define NEW_TSTAMP(now, interval) \
(tstamp_t)(((tstamp_t)(now + interval) != NO_TIMER) ? now + interval : NO_TIMER + 1)

/*
 * a simple test to check expired timers, using wrapping tick counter
 *
 * Evaluates to TRUE if first arg is "later than" second.
 * Evaluates to FALSE otherwise, including the cases that the second
 *   argument is NO_TIMER or the timers are equal.
 * First arg should not be NO_TIMER.
 */
#define LATER_THAN(now, timer) \
	((timer != NO_TIMER) && ((((_ststamp_t)timer - (_ststamp_t)now) & TIMER_WRAP_MASK) != 0))

/*
 * a simple test to check for timers, using wrapping tick counter
 *
 * Evaluates to TRUE if first arg is "earlier than" second, including
 *   the case that the second argument is NO_TIMER.
 * Evaluates to FALSE otherwise, including when the timers are equal.
 * First arg should not be NO_TIMER.
 */
#define EARLIER_THAN(new, timer) \
	((timer == NO_TIMER) || ((((_ststamp_t)new - (_ststamp_t)timer) & TIMER_WRAP_MASK) != 0))

#endif /* _EPITIMERS_H_ */
