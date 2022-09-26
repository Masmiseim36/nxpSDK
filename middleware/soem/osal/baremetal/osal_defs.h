/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/*
 * Copyright 2022 NXP
*/

#ifndef _osal_defs_
#define _osal_defs_

#include <stdint.h>

#include "board.h"

#ifndef PACKED
#define PACKED_BEGIN
#define PACKED          __PACKED    /* from CMSIS */
#define PACKED_END
#endif

typedef char mtx_t;
#define osal_mutex_create()    0
#define osal_mutex_lock(mtx)   (mtx) = 1
#define osal_mutex_unlock(mtx) (mtx) = 0

#define OSAL_THREAD_HANDLE  int *
#define OSAL_THREAD_FUNC    void
#define OSAL_THREAD_FUNC_RT void


#ifdef __NEWLIB__

#include "sys/time.h"

#else /* __NEWLIB__ */

struct timeval
{
    int32_t tv_sec;
    int32_t tv_usec;
};

#ifndef timercmp
#define timercmp(a, b, CMP) \
    (((a)->tv_sec == (b)->tv_sec) ? ((a)->tv_usec CMP(b)->tv_usec) : ((a)->tv_sec CMP(b)->tv_sec))
#endif

#ifndef timeradd
#define timeradd(a, b, result)                           \
    do                                                   \
    {                                                    \
        (result)->tv_sec  = (a)->tv_sec + (b)->tv_sec;   \
        (result)->tv_usec = (a)->tv_usec + (b)->tv_usec; \
        if ((result)->tv_usec >= 1000000)                \
        {                                                \
            ++(result)->tv_sec;                          \
            (result)->tv_usec -= 1000000;                \
        }                                                \
    } while (0)
#endif

#ifndef timersub
#define timersub(a, b, result)                           \
    do                                                   \
    {                                                    \
        (result)->tv_sec  = (a)->tv_sec - (b)->tv_sec;   \
        (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
        if ((result)->tv_usec < 0)                       \
        {                                                \
            --(result)->tv_sec;                          \
            (result)->tv_usec += 1000000;                \
        }                                                \
    } while (0)
#endif

#endif /* __NEWLIB__ */

#endif
