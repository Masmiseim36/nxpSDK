
#ifndef FSL_SSS_TYPES_H
#define FSL_SSS_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __STDC__
#include <unistd.h>
#endif

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / (sizeof(array[0])))
#endif

#define assert_static(e)                    \
    {                                       \
        char assert_static__[(e) ? 1 : -1]; \
        assert_static__;                    \
    }

#endif /* FSL_SSS_TYPES_H */
