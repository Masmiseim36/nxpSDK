/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xf-flx-lock-if.h
 *
 * Flexible lock interface layer
 ******************************************************************************/

#ifndef _XF_FLX_LOCK_IF_H_
#define _XF_FLX_LOCK_IF_H_

typedef enum {
    XF_MUTEX_BASED_LOCK  = 0,
    XF_INTRPT_BASED_LOCK = 1,
    XF_DUMMY_LOCK        = 2,
} xf_lock_type;

/* ...flexible lock struct definition */
typedef struct xf_flx_lock
{
    xf_lock_t    lock;
    xf_lock_type lock_type;
    UWORD32      intr_mask;

} xf_flx_lock_t;

/* ...flexible lock initialization */
static inline void xf_flx_lock_init(xf_flx_lock_t *flx_lock, xf_lock_type lock_type)
{
    flx_lock->lock_type = lock_type;

    switch (lock_type) {
    case XF_DUMMY_LOCK:
    case XF_INTRPT_BASED_LOCK:
        break;

    default:
    case XF_MUTEX_BASED_LOCK:
        flx_lock->lock_type = XF_MUTEX_BASED_LOCK;
        __xf_lock_init(&flx_lock->lock);
        break;
    }
}

/* ...flexible lock re-initialization */
static inline void xf_flx_lock_reinit(xf_flx_lock_t *flx_lock, xf_lock_type lock_type)
{
    BUG(flx_lock->lock_type != XF_DUMMY_LOCK, _x("invalid current lock type %d"), flx_lock->lock_type);

    flx_lock->lock_type = lock_type;

    switch (lock_type) {
    case XF_DUMMY_LOCK:
    case XF_INTRPT_BASED_LOCK:
        break;

    default:
    case XF_MUTEX_BASED_LOCK:
        flx_lock->lock_type = XF_MUTEX_BASED_LOCK;
        __xf_lock_init(&flx_lock->lock);
        break;
    }
}

/* ...flexible lock deletion */
static inline void xf_flx_lock_destroy(xf_flx_lock_t *flx_lock)
{
    switch (flx_lock->lock_type) {
    case XF_DUMMY_LOCK:
    case XF_INTRPT_BASED_LOCK:
        break;

    default:
    case XF_MUTEX_BASED_LOCK:
        __xf_lock_destroy(&flx_lock->lock);
        break;
    }

}

/* ...flexible lock acquisition */
static inline void xf_flx_lock(xf_flx_lock_t *flx_lock)
{
    switch (flx_lock->lock_type) {
    case XF_DUMMY_LOCK:
        break;

    case XF_INTRPT_BASED_LOCK:
        flx_lock->intr_mask = __xf_disable_interrupts();
        break;

    default:
    case XF_MUTEX_BASED_LOCK:
        __xf_lock(&flx_lock->lock);
        break;
    }
}

/* ...flexible lock release */
static inline void xf_flx_unlock(xf_flx_lock_t *flx_lock)
{
    switch (flx_lock->lock_type) {
    case XF_DUMMY_LOCK:
        break;

    case XF_INTRPT_BASED_LOCK:
        __xf_restore_interrupts(flx_lock->intr_mask);
        break;

    default:
    case XF_MUTEX_BASED_LOCK:
        __xf_unlock(&flx_lock->lock);
        break;
    }
}

#endif /* _XF_FLX_LOCK_IF_H_ */

