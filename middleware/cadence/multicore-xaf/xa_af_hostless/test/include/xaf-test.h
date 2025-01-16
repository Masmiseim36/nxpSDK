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
#ifndef __XAF_TEST_H__
#define __XAF_TEST_H__

#define XAF_SHMEM_STRUCT_SIZE                12288
#define XAF_SIZEOFPROXY                     2272
#define XAF_SIZEOFHANDLE                    44

#include "xtensa/xos_errors.h"

#define XA_NUM_API_ERRS    7

typedef struct _xos_error_map_t{
#if 0
    xos_err_t    err
#else
    int         err;  /* Fix for RG 2017.8 tools' build problem due to renaming of <typedef enum xos_err_t{}xos_err_t> to <enum{}> in xos_errors.h */
#endif
    char        *perr;
}_XOS_ERR_MAP;

typedef struct _api_err_t{
    int     err;
    char    *perr;
}_XA_API_ERR_MAP;


/* ...check the API call succeeds */
/* ...xaf_adev_close should not be called if xaf_adev_open fails. */
#if ((XF_NUM_CFG_CORES == 1) || (XF_CORE_ID == XF_CORE_ID_MASTER))
#define TST_CHK_API(cond, func_name)                                              \
({                                                                                \
    int __ret;                                                                    \
                                                                                  \
    if ((__ret = (int)(cond)) < 0)                                                \
    {                                                                             \
        extern _XA_API_ERR_MAP error_map_table_api[];                             \
        extern int (abort_blocked_threads)();                                     \
        int i;                                                                    \
        for(i=0; i<XA_NUM_API_ERRS; i++){                                         \
            if(error_map_table_api[i].err == __ret){                              \
                FIO_PRINTF(stderr,"%s failed, Error code : %d (%s at %s:%d)\n\n", func_name, __ret, error_map_table_api[i].perr, __FILE__, __LINE__);\
                if(strcmp(func_name, "xaf_adev_open") && (xaf_adev_close(p_adev, XAF_ADEV_FORCE_CLOSE) == XAF_NO_ERR)) \
                    abort_blocked_threads();                                      \
                return __ret;                                                     \
            }\
        }\
        FIO_PRINTF(stderr,"%s failed, Error code : %d\n\n", func_name, __ret);    \
        if(strcmp(func_name, "xaf_adev_open") && (xaf_adev_close(p_adev, XAF_ADEV_FORCE_CLOSE) == XAF_NO_ERR)) \
            abort_blocked_threads();                                              \
        return __ret;                                                             \
    }                                                                             \
    __ret;                                                                        \
})
#else //((XF_NUM_CFG_CORES == 1) || (XF_CORE_ID == XF_CORE_ID_MASTER))
#define TST_CHK_API(cond, func_name)                                              \
({                                                                                \
    int __ret;                                                                    \
                                                                                  \
    if ((__ret = (int)(cond)) < 0)                                                \
    {                                                                             \
        extern _XA_API_ERR_MAP error_map_table_api[];                             \
        int i;                                                                    \
        for(i=0; i<XA_NUM_API_ERRS; i++){                                         \
            if(error_map_table_api[i].err == __ret){                              \
                FIO_PRINTF(stderr,"c[%d] %s failed, Error code : %d (%s at %s:%d)\n\n", XF_CORE_ID, func_name, __ret, error_map_table_api[i].perr, __FILE__, __LINE__);\
                return __ret;                                                     \
            }\
        }\
        FIO_PRINTF(stderr,"c[%d] %s failed, Error code : %d\n\n", XF_CORE_ID, func_name, __ret);    \
        return __ret;                                                             \
    }                                                                             \
    __ret;                                                                        \
})
#endif //((XF_NUM_CFG_CORES == 1) || (XF_CORE_ID == XF_CORE_ID_MASTER))

/* ...check null pointer */
#define TST_CHK_PTR(ptr, func_name)                                                    \
({                                                                                     \
    int __ret;                                                                         \
                                                                                       \
    if ((__ret = (int)(ptr)) == 0)                                                     \
    {                                                                                  \
        FIO_PRINTF(stderr,"%s failed, Null pointer error : %d\n\n", func_name, __ret);    \
        return XAF_INVALIDPTR_ERR;                                                          \
    }                                                                                  \
    __ret;                                                                             \
})

#endif /* __XAF_TEST_H__ */
