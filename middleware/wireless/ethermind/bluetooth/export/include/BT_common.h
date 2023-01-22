
/**
 *  \file BT_common.h
 *
 *  This Header file describes common declarations for the
 *  EtherMind Bluetooth Stack & Profile modules.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_COMMON_
#define _H_BT_COMMON_

/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup bt_common_module Bluetooth Common Module
 * \{
 */

/* -------------------------------------------- Header File Inclusion */
/* The EtherMind OS Abstraction */
#include "BT_os.h"

/* The EtherMind Tunable Constant */
#include "BT_limits.h"

/* The Bluetooth Assigned Numbers */
#include "BT_assigned_numbers.h"

/* The EtherMind Error Codes */
#include "BT_error.h"

/* The EtherMind Debug Library */
#include "BT_debug.h"

/* For BT_assert() macro */
#include "BT_assert.h"

/* For btsnoop APIs */
#include "BT_snoop.h"

/* The EtherMind Timer Library */
#include "BT_timer.h"

/* The EtherMind SDP Database APIs */
#include "BT_dbase_api.h"

/* EtherMind Platform Specific Initialization & Shutdown Handlers */
#include "BT_common_pl.h"

#ifdef BT_STORAGE
#include "BT_storage.h"
#endif /* BT_STORAGE */

/* -------------------------------------------- Global Definitions */

/**
 * \cond ignore_this
 * \{
 */

/* COMMON Debug Macros */
#ifndef COMMON_NO_DEBUG
#define COMMON_ERR(module_id, ...)          BT_debug_error((module_id), __VA_ARGS__)
#else   /* COMMON_NO_DEBUG */
#define COMMON_ERR               BT_debug_null
#endif  /* COMMON_NO_DEBUG */

#ifdef COMMON_DEBUG

#define COMMON_TRC(module_id, ...)          BT_debug_trace((module_id), __VA_ARGS__)
#define COMMON_INF(module_id, ...)          BT_debug_info((module_id), __VA_ARGS__)

#else /* COMMON_DEBUG */

#define COMMON_TRC               BT_debug_null
#define COMMON_INF               BT_debug_null

#endif /* COMMON_DEBUG */

/** \endcond */

/**
 * \defgroup bt_common_defines Defines
 * \{
 */

/**
 * \defgroup bt_common_constants Constants
 * \{
 */

/**
 * \defgroup bt_common_stack_init_states Stack Init States
 * \{
 */

#define STACK_INIT_UNDEFINED                0x00U
#define STACK_INIT_ETHERMIND_INIT           0x01U
#define STACK_INIT_BT_ON_STARTED            0x02U
#define STACK_INIT_BT_ON_COMPLETE           0x03U

/** \} */

/* Definition for True/False */
#ifndef BT_FALSE
#define BT_FALSE                            (UINT8)0U
#endif /* BT_FALSE */

#ifndef BT_TRUE
#define BT_TRUE                             (UINT8)1U
#endif /* BT_TRUE */

/* Size of Bluetooth Device Address (BD_ADDR) in number of Octets */
#define BT_BD_ADDR_SIZE                         6U
#define BT_BD_ADDR_TYPE_SIZE                    1U
#define BT_BD_DEV_ADDR_SIZE                     (BT_BD_ADDR_SIZE + BT_BD_ADDR_TYPE_SIZE)

/* Type Definitions for 16, 32 & 128-bit UUIDs */
#define UUID_16                                 1U
#define UUID_32                                 2U
#define UUID_128                                4U

/* Bluetooth Device Address type masks */
#define BT_RANDOM_ADDR_TYPE_MASK                0xC0U
#define BT_STATIC_ADDR_BIT_MASK                 0xC0U
#define BT_NON_RESOLV_PVT_ADDR_BIT_MASK         0x00U
#define BT_RESOLV_PVT_ADDR_BIT_MASK             0x40U

/** \} */

/** \} */

/* -------------------------------------------- Macros */

/**
 * \defgroup bt_common_utility_macros Utility Macros
 * \{
 */

/**
 *  Packing Macros.
 *
 *  Syntax: BT_PACK_<Endian-ness LE/BE>_<no_of_bytes>_BYTE
 *
 *  Usage: Based on the endian-ness defined for each protocol/profile layer,
 *  appropriate packing macros to be used by each layer.
 *
 *  Example: HCI is defined as little endian protocol,
 *  so if HCI defines HCI_PACK_2_BYTE for packing a parameter of size 2 byte,
 *  that shall be mapped to BT_PACK_LE_2_BYTE
 *
 *  By default both the packing and unpaking macros uses pointer to
 *  a single or multi-octet variable which to be packed to or unpacked from
 *  a buffer (unsinged character array).
 *
 *  For the packing macro, another variation is available,
 *  where the single or multi-octet variable itself is used (not its pointer).
 *
 *  Syntax: BT_PACK_<Endian-ness LE/BE>_<no_of_bytes>_BYTE_VAL
 */

/* Little Endian Packing Macros */
#define BT_PACK_LE_1_BYTE(dst, src) \
    { \
        UCHAR val; \
        val = (UCHAR)(*(src)); \
        BT_PACK_LE_1_BYTE_VAL((dst), val); \
    }

#define BT_PACK_LE_1_BYTE_VAL(dst, src) \
    (dst)[0U] = (src);

#define BT_PACK_LE_2_BYTE(dst, src) \
    { \
        UINT16 val; \
        val = (UINT16)(*(src)); \
        BT_PACK_LE_2_BYTE_VAL((dst), val); \
    }

#define BT_PACK_LE_2_BYTE_VAL(dst, src) \
    (dst)[0U] = (UCHAR)(src); \
    (dst)[1U] = (UCHAR)((src) >> 8U);

#define BT_PACK_LE_3_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*(src)); \
        BT_PACK_LE_3_BYTE_VAL((dst), val); \
    }

#define BT_PACK_LE_3_BYTE_VAL(dst, src) \
    (dst)[0U] = (UCHAR)(src);\
    (dst)[1U] = (UCHAR)((src) >> 8U);\
    (dst)[2U] = (UCHAR)((src) >> 16U);

#define BT_PACK_LE_4_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*(src)); \
        BT_PACK_LE_4_BYTE_VAL((dst), val); \
    }

#define BT_PACK_LE_4_BYTE_VAL(dst, src) \
    (dst)[0U] = (UCHAR)(src);\
    (dst)[1U] = (UCHAR)((src) >> 8U);\
    (dst)[2U] = (UCHAR)((src) >> 16U);\
    (dst)[3U] = (UCHAR)((src) >> 24U);

/* TBD: Update based on 64 Bit, 128 Bit Data Types */
#define BT_PACK_LE_8_BYTE(dst,val)\
        BT_mem_copy ((dst), (val), 8U)

#define BT_PACK_LE_16_BYTE(dst,val)\
        BT_mem_copy ((dst), (val), 16U)

#define BT_PACK_LE_N_BYTE(dst,val,n)\
        BT_mem_copy ((dst), (val), (n))

/* Big Endian Packing Macros */
#define BT_PACK_BE_1_BYTE(dst, src) \
    { \
        UCHAR val; \
        val = (UCHAR)(*((UCHAR *)(src))); \
        BT_PACK_BE_1_BYTE_VAL((dst), val); \
    }

#define BT_PACK_BE_1_BYTE_VAL(dst, src) \
    (dst)[0U] = (src);

#define BT_PACK_BE_2_BYTE(dst, src) \
    { \
        UINT16 val; \
        val = (UINT16)(*((UINT16 *)(src))); \
        BT_PACK_BE_2_BYTE_VAL((dst), val); \
    }

#define BT_PACK_BE_2_BYTE_VAL(dst, src) \
    (dst)[1U] = (UCHAR)(src); \
    (dst)[0U] = (UCHAR)((src) >> 8U);

#define BT_PACK_BE_3_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*((UINT32 *)(src))); \
        BT_PACK_BE_3_BYTE_VAL((dst), val); \
    }

#define BT_PACK_BE_3_BYTE_VAL(dst, src) \
    (dst)[2U] = (UCHAR)(src);\
    (dst)[1U] = (UCHAR)((src) >> 8U);\
    (dst)[0U] = (UCHAR)((src) >> 16U);

#define BT_PACK_BE_4_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*((UINT32 *)(src))); \
        BT_PACK_BE_4_BYTE_VAL((dst), val); \
    }

#define BT_PACK_BE_4_BYTE_VAL(dst, src) \
    (dst)[3U] = (UCHAR)(src);\
    (dst)[2U] = (UCHAR)((src) >> 8U);\
    (dst)[1U] = (UCHAR)((src) >> 16U);\
    (dst)[0U] = (UCHAR)((src) >> 24U);

/* TBD: Update based on 64 Bit, 128 Bit Data Types */
#define BT_PACK_BE_8_BYTE(dst,val)\
        BT_mem_copy ((dst), (val), 8U)

#define BT_PACK_BE_16_BYTE(dst,val)\
        BT_mem_copy ((dst), (val), 16U)

#define BT_PACK_BE_N_BYTE(dst,val,n)\
        BT_mem_copy ((dst), (val), (n))


/**
 *  Unpacking Macros.
 *
 *  Syntax: BT_UNPACK_<Endian-ness LE/BE>_<no_of_bytes>_BYTE
 *
 *  Usage: Based on the endian-ness defined for each protocol/profile layer,
 *  appropriate unpacking macros to be used by each layer.
 *
 *  Example: HCI is defined as little endian protocol,
 *  so if HCI defines HCI_UNPACK_4_BYTE for unpacking a parameter of size 4 byte,
 *  that shall be mapped to BT_UNPACK_LE_4_BYTE
 */

/* Little Endian Unpacking Macros */
#define BT_UNPACK_LE_1_BYTE(dst,src)\
    *((UCHAR *)(dst)) = (UCHAR)(*((UCHAR *)(src)));

#define BT_UNPACK_LE_2_BYTE(dst,src)\
        *((UINT16 *)(dst))  = (src)[1U]; \
        *((UINT16 *)(dst))  = *((UINT16 *)(dst)) << 8U; \
        *((UINT16 *)(dst)) |= (src)[0U];

#define BT_UNPACK_LE_3_BYTE(dst,src)\
        *((UINT32 *)(dst))  = (src)[2U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[1U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[0U];

#define BT_UNPACK_LE_4_BYTE(dst,src)\
        *((UINT32 *)(dst))  = (src)[3U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[2U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[1U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[0U];

/* TBD: Update based on 64 Bit, 128 Bit Data Types */
#define BT_UNPACK_LE_8_BYTE(dst,src)\
        BT_mem_copy ((dst), (src), 8U)

#define BT_UNPACK_LE_16_BYTE(dst,src)\
        BT_mem_copy ((dst), (src), 16U)

#define BT_UNPACK_LE_N_BYTE(dst,src,n)\
        BT_mem_copy ((dst), (src), (n))

/* Big Endian Unpacking Macros */
#define BT_UNPACK_BE_1_BYTE(dst,src)\
    *((UCHAR *)(dst)) = (UCHAR)(*((UCHAR *)(src)));

#define BT_UNPACK_BE_2_BYTE(dst,src)\
        *((UINT16 *)(dst))  = (src)[0U]; \
        *((UINT16 *)(dst))  = *((UINT16 *)(dst)) << 8U; \
        *((UINT16 *)(dst)) |= (src)[1U];

#define BT_UNPACK_BE_3_BYTE(dst,src)\
        *((UINT32 *)(dst))  = (src)[0U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[1U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[2U];

#define BT_UNPACK_BE_4_BYTE(dst,src)\
        *((UINT32 *)(dst))  = (src)[0U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[1U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[2U];\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8U;\
        *((UINT32 *)(dst)) |= (src)[3U];

/* TBD: Update based on 64 Bit, 128 Bit Data Types */
#define BT_UNPACK_BE_8_BYTE(dst,src)\
        BT_mem_copy ((dst), (src), 8U)

#define BT_UNPACK_BE_16_BYTE(dst,src)\
        BT_mem_copy ((dst), (src), 16U)

#define BT_UNPACK_BE_N_BYTE(dst,src,n)\
        BT_mem_copy ((dst), (src), (n))

#define BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER\
       "ADDR: %02X:%02X:%02X:%02X:%02X:%02X"

#define BT_DEVICE_ADDR_ONLY_SPACED_FRMT_SPECIFIER\
       "ADDR: %02X %02X %02X %02X %02X %02X"

#define BT_DEVICE_ADDR_ONLY_PRINT_STR(ref)\
        (ref)[0U],(ref)[1U],(ref)[2U],(ref)[3U],(ref)[4U],(ref)[5U]

#define BT_DEVICE_ADDR_FRMT_SPECIFIER\
        "ADDR: %02X:%02X:%02X:%02X:%02X:%02X, TYPE: %02X"

#define BT_DEVICE_ADDR_SPACED_FRMT_SPECIFIER\
        "ADDR: %02X %02X %02X %02X %02X %02X, TYPE: %02X"

#define BT_DEVICE_ADDR_PRINT_STR(ref)\
        (ref)->addr[0U],(ref)->addr[1U],(ref)->addr[2U],\
        (ref)->addr[3U],(ref)->addr[4U],(ref)->addr[5U],\
        (ref)->type

/* Macro to Copy a BD_ADDR from a Source to Destination */
#define BT_COPY_BD_ADDR_AND_TYPE(dest,src)\
        BT_COPY_TYPE((dest)->type,(src)->type)\
        BT_COPY_BD_ADDR((dest)->addr,(src)->addr)

#define BT_COPY_TYPE(dest,src)\
        (dest) = (src);

#define BT_COMPARE_BD_ADDR_AND_TYPE(addr_a,addr_b)\
        ((BT_COMPARE_TYPE((addr_a)->type,(addr_b)->type)) &&\
         (BT_COMPARE_ADDR((addr_a)->addr,(addr_b)->addr)))

#define BT_COMPARE_TYPE(type_a,type_b)\
        (((type_a) == (type_b))?BT_TRUE:BT_FALSE)

#define BT_COMPARE_ADDR(addr_a,addr_b)\
        ((0 == BT_mem_cmp((addr_a), (addr_b), BT_BD_ADDR_SIZE))?BT_TRUE:BT_FALSE)

#define BT_INIT_BD_ADDR(bd_addr) \
        BT_mem_set ((bd_addr)->addr, 0, BT_BD_ADDR_SIZE); \
        (bd_addr)->type = BT_BD_PUBLIC_ADDRESS_TYPE

#define BT_BD_ADDR(bd_addr) (bd_addr)->addr
#define BT_BD_ADDR_TYPE(bd_addr) (bd_addr)->type

#define BT_COPY_BD_ADDR(dest, src) \
        BT_mem_copy ((dest), (src), BT_BD_ADDR_SIZE);

#define BT_BD_ADDR_IS_NON_ZERO(addr)\
        ((0x00U == ((addr)[0U] | (addr)[1U] | (addr)[2U] | (addr)[3U] | (addr)[4U] | (addr)[5U]))?\
        BT_FALSE:BT_TRUE)

/* Macros to check Bluetooth Device Address type */
#define BT_IS_BD_ADDR_PUBLIC(bd_addr) \
    (BT_BD_PUBLIC_ADDRESS_TYPE == (bd_addr)->type)

#define BT_IS_BD_ADDR_RANDOM(bd_addr) \
    (BT_BD_RANDOM_ADDRESS_TYPE == (bd_addr)->type)

#define BT_IS_BD_ADDR_STATIC(bd_addr) \
    ((BT_IS_BD_ADDR_RANDOM(bd_addr)) && \
     (BT_STATIC_ADDR_BIT_MASK == (BT_RANDOM_ADDR_TYPE_MASK & (bd_addr)->addr[5])))

#define BT_IS_BD_ADDR_NON_RESOLV_PRIVATE(bd_addr) \
    ((BT_IS_BD_ADDR_RANDOM(bd_addr)) && \
     (BT_NON_RESOLV_PVT_ADDR_BIT_MASK == (BT_RANDOM_ADDR_TYPE_MASK & (bd_addr)->addr[5U])))

#define BT_IS_BD_ADDR_RESOLV_PRIVATE(bd_addr) \
    ((BT_IS_BD_ADDR_RANDOM(bd_addr)) && \
     (BT_RESOLV_PVT_ADDR_BIT_MASK == (BT_RANDOM_ADDR_TYPE_MASK & (bd_addr)->addr[5U])))

#define BT_IS_ARRAY_EMPTY(buffer, buffer_size) \
        (((0U == (buffer)[0U]) && (0 == BT_mem_cmp((buffer), &(buffer)[1U], ((buffer_size) - 1U)))) ? BT_TRUE : BT_FALSE)

#ifndef BT_DISABLE_MUTEX

/* Macro to define a Mutex Variable */
#define BT_DEFINE_MUTEX(mutex) BT_thread_mutex_type mutex;

/* Macro to define a Mutex Variable with a type qualifier */
#define BT_DEFINE_MUTEX_TYPE(type, mutex) type BT_thread_mutex_type mutex;

/* Macro to define a Conditional Variable */
#define BT_DEFINE_COND(cond) BT_thread_cond_type cond;

/* Macro to define a Conditional Variable with a type qualifier */
#define BT_DEFINE_COND_TYPE(type, cond) type BT_thread_cond_type cond;

/*
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define BT_MUTEX_INIT_VOID(mutex, MODULE)                                \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_init(&(mutex), NULL);                      \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Initialize Mutex in " #MODULE ".\n");             \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define BT_MUTEX_INIT(mutex, MODULE)                                     \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_init(&(mutex), NULL);                      \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Initialize Mutex in " #MODULE ".\n");             \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define BT_COND_INIT_VOID(cond, MODULE)                                  \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_cond_init(&(cond), NULL);                        \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Initialize CondVar in " #MODULE ".\n");           \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define BT_COND_INIT(cond, MODULE)                                       \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_cond_init(&(cond), NULL);                        \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Initialize CondVar in " #MODULE ".\n");           \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define BT_MUTEX_LOCK_VOID(mutex, MODULE)                                \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_lock(&(mutex));                            \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define BT_MUTEX_LOCK(mutex, MODULE)                                 \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_lock(&(mutex));                            \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define BT_MUTEX_UNLOCK_VOID(mutex, MODULE)                          \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_unlock(&(mutex));                          \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define BT_MUTEX_UNLOCK(mutex, MODULE)                               \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_unlock(&(mutex));                          \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(                                                  \
            BT_MODULE_ID_##MODULE,                                       \
            "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

#define BT_MUTEX_DEINIT(mutex, MODULE)                                   \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_deinit(&(mutex));                          \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(\
                BT_MODULE_ID_##MODULE, \
            "FAILED to De-Initialize Mutex in " #MODULE ".\n");          \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

#define BT_MUTEX_DEINIT_VOID(mutex, MODULE)                              \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_mutex_deinit(&(mutex));                          \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(\
                BT_MODULE_ID_##MODULE, \
            "FAILED to De-Initialize Mutex in " #MODULE ".\n");          \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

#define BT_COND_DEINIT(mutex, MODULE)                                    \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_cond_deinit(&(mutex));                           \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(\
                BT_MODULE_ID_##MODULE, \
            "FAILED to De-Initialize CondV in " #MODULE ".\n");          \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

#define BT_COND_DEINIT_VOID(mutex, MODULE)                               \
    {                                                                    \
        INT32 ret;                                                       \
        ret = BT_thread_cond_deinit(&(mutex));                           \
        if (0 > ret)                                                     \
        {                                                                \
            COMMON_ERR(\
                BT_MODULE_ID_##MODULE, \
            "FAILED to De-Initialize CondV in " #MODULE ".\n");          \
        }                                                                \
        BT_assert(0 == ret);                                             \
    }

#else  /* BT_DISABLE_MUTEX */

/* Macro to define a Mutex Variable */
#define BT_DEFINE_MUTEX(mutex)

/* Macro to define a Mutex Variable with a type qualifier */
#define BT_DEFINE_MUTEX_TYPE(type, mutex)

/* Macro to define a Conditional Variable */
#define BT_DEFINE_COND(cond)

/* Macro to define a Conditional Variable with a type qualifier */
#define BT_DEFINE_COND_TYPE(type, cond)

/*
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define BT_MUTEX_INIT_VOID(mutex, MODULE)

/*
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define BT_MUTEX_INIT(mutex, MODULE)

/*
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define BT_COND_INIT_VOID(cond, MODULE)

/*
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define BT_COND_INIT(cond, MODULE)

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define BT_MUTEX_LOCK(mutex, MODULE)

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define BT_MUTEX_LOCK_VOID(mutex, MODULE)

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define BT_MUTEX_UNLOCK(mutex, MODULE)

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define BT_MUTEX_UNLOCK_VOID(mutex, MODULE)

#endif /* BT_DISABLE_MUTEX */


#ifndef BT_NO_STATIC_DATA_SIZE

/* Define UCHAR array of size 'BT_STATIC_DATA_SIZE' */
#define BT_DEFINE_STATIC_DATA(data) UCHAR data [BT_STATIC_DATA_SIZE];

#else

#define BT_DEFINE_STATIC_DATA(data)

#endif /* BT_NO_STATIC_DATA_SIZE */

/* Abstractions for bit-wise operation */
#define BT_EXTRACT_BITNUM(val, bitnum)        (((val) >> (bitnum)) & 1U)
#define BT_SET_BITNUM(val, bitnum)            ((val) |= (1U << (bitnum)))
#define BT_CLR_BITNUM(val, bitnum)            ((val) &= (~(1U << (bitnum))))

/* Macro to find Minimum and Maximum value */
#define BT_GET_MIN(a, b) \
        (((a) > (b)) ? (b) : (a))

#define BT_GET_MAX(a, b) \
        (((a) > (b)) ? (a) : (b))

/* Unreferenced variable macro to avoid compilation warnings */
#define BT_IGNORE_UNUSED_PARAM(v) (void)(v)

/* Loop for ever */
#define BT_LOOP_FOREVER() for(;;)

/* Unused return value to avoid compilation warning - MISRA C-2012 Rule 17.7 */
#define BT_IGNORE_RETURN_VALUE    void

/** \} */

/*
 * Module Identifier definitions.
 * Currently used for runtime debug enable/disable scenario.
 * In future, this can be used for other purposes as well,
 * hence these defines are placed under common header file.
 */
/* Page 0 - Bluetooth Protocol Modules */
#define BT_MODULE_PAGE_0                      0x00000000U

/* Module - Bit Mask */
#define BT_MODULE_BIT_MASK_HCI                0x00000001U
#define BT_MODULE_BIT_MASK_L2CAP              0x00000002U
#define BT_MODULE_BIT_MASK_SDP                0x00000004U
#define BT_MODULE_BIT_MASK_RFCOMM             0x00000008U
#define BT_MODULE_BIT_MASK_AVCTP              0x00000010U
#define BT_MODULE_BIT_MASK_AVDTP              0x00000020U
#define BT_MODULE_BIT_MASK_BNEP               0x00000040U
#define BT_MODULE_BIT_MASK_COMMON             0x00000080U
/* DB SDP */
#define BT_MODULE_BIT_MASK_DB                 0x00000100U
#define BT_MODULE_BIT_MASK_DQ                 0x00000200U
#define BT_MODULE_BIT_MASK_MCAP               0x00000400U
#define BT_MODULE_BIT_MASK_OBEX               0x00000800U
#define BT_MODULE_BIT_MASK_SM                 0x00001000U
#define BT_MODULE_BIT_MASK_ATT                0x00002000U
#define BT_MODULE_BIT_MASK_SMP                0x00004000U
#define BT_MODULE_BIT_MASK_WT                 0x00008000U
#define BT_MODULE_BIT_MASK_GATT               0x00010000U
#define BT_MODULE_BIT_MASK_GATT_DB            0x00020000U
#define BT_MODULE_BIT_MASK_FSM                0x00040000U
#define BT_MODULE_BIT_MASK_BCSP               0x00080000U
#define BT_MODULE_BIT_MASK_RACP               0x00100000U

/* Module ID */
#define BT_MODULE_ID_HCI                      (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_HCI)
#define BT_MODULE_ID_L2CAP                    (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_L2CAP)
#define BT_MODULE_ID_SDP                      (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_SDP)
#define BT_MODULE_ID_RFCOMM                   (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_RFCOMM)
#define BT_MODULE_ID_AVDTP                    (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_AVDTP)
#define BT_MODULE_ID_AVCTP                    (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_AVCTP)
#define BT_MODULE_ID_BNEP                     (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_BNEP)
#define BT_MODULE_ID_COMMON                   (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_COMMON)
#define BT_MODULE_ID_DB                       (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_DB)
#define BT_MODULE_ID_DQ                       (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_DQ)
#define BT_MODULE_ID_MCAP                     (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_MCAP)
#define BT_MODULE_ID_OBEX                     (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_OBEX)
#define BT_MODULE_ID_SM                       (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_SM)
#define BT_MODULE_ID_ATT                      (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_ATT)
#define BT_MODULE_ID_SMP                      (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_SMP)
#define BT_MODULE_ID_WT                       (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_WT)
#define BT_MODULE_ID_GATT_DB                  (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_GATT_DB)
#define BT_MODULE_ID_GATT                     (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_GATT)
#define BT_MODULE_ID_FSM                      (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_FSM)
#define BT_MODULE_ID_BCSP                     (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_BCSP)
#define BT_MODULE_ID_RACP                     (BT_MODULE_PAGE_0 | BT_MODULE_BIT_MASK_RACP)

/* Page 1 - Classic Profile Modules */
#define BT_MODULE_PAGE_1                      0x10000000U

/* Module - Bit Mask */
#define BT_MODULE_BIT_MASK_A2DP               0x00000001U
#define BT_MODULE_BIT_MASK_AVRCP              0x00000002U
#define BT_MODULE_BIT_MASK_BIP                0x00000004U
#define BT_MODULE_BIT_MASK_BPP                0x00000008U
#define BT_MODULE_BIT_MASK_CTN                0x00000010U
#define BT_MODULE_BIT_MASK_DUNP_DT            0x00000020U
#define BT_MODULE_BIT_MASK_DUNP_GW            0x00000040U
#define BT_MODULE_BIT_MASK_FTP                0x00000080U
#define BT_MODULE_BIT_MASK_GNSS               0x00000100U
#define BT_MODULE_BIT_MASK_HFP_AG             0x00000200U
#define BT_MODULE_BIT_MASK_HFP_UNIT           0x00000400U
#define BT_MODULE_BIT_MASK_HID                0x00000800U
#define BT_MODULE_BIT_MASK_HID_HOST           0x00001000U
#define BT_MODULE_BIT_MASK_HSP_AG             0x00002000U
#define BT_MODULE_BIT_MASK_HSP_UNIT           0x00004000U
#define BT_MODULE_BIT_MASK_MAP                0x00008000U
#define BT_MODULE_BIT_MASK_OPP                0x00010000U
#define BT_MODULE_BIT_MASK_PAN                0x00020000U
#define BT_MODULE_BIT_MASK_PBAP               0x00040000U
#define BT_MODULE_BIT_MASK_SAP                0x00080000U
#define BT_MODULE_BIT_MASK_SPP                0x00100000U
#define BT_MODULE_BIT_MASK_SYNCP              0x00200000U
#define BT_MODULE_BIT_MASK_DID                0x00400000U

/* Module ID */
#define BT_MODULE_ID_A2DP                     (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_A2DP)
#define BT_MODULE_ID_AVRCP                    (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_AVRCP)
#define BT_MODULE_ID_BIP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_BIP)
#define BT_MODULE_ID_BPP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_BPP)
#define BT_MODULE_ID_CTN                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_CTN)
#define BT_MODULE_ID_DUNP_DT                  (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_DUNP_DT)
#define BT_MODULE_ID_DUNP_GW                  (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_DUNP_GW)
#define BT_MODULE_ID_FTP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_FTP)
#define BT_MODULE_ID_GNSS                     (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_GNSS)
#define BT_MODULE_ID_HFP_AG                   (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_HFP_AG)
#define BT_MODULE_ID_HFP_UNIT                 (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_HFP_UNIT)
#define BT_MODULE_ID_HID                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_HID)
#define BT_MODULE_ID_HID_HOST                 (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_HID_HOST)
#define BT_MODULE_ID_HSP_AG                   (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_HSP_AG)
#define BT_MODULE_ID_HSP_UNIT                 (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_HSP_UNIT)
#define BT_MODULE_ID_MAP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_MAP)
#define BT_MODULE_ID_OPP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_OPP)
#define BT_MODULE_ID_PAN                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_PAN)
#define BT_MODULE_ID_PBAP                     (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_PBAP)
#define BT_MODULE_ID_SAP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_SAP)
#define BT_MODULE_ID_SPP                      (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_SPP)
#define BT_MODULE_ID_SYNCP                    (BT_MODULE_PAGE_1 | BT_MODULE_BIT_MASK_SYNCP)

/* Page 2 - GATT based Profile Modules */
#define BT_MODULE_PAGE_2                      0x20000000U

/* Module - Bit Mask */
#define BT_MODULE_BIT_MASK_GA                 0x000000001
#define BT_MODULE_BIT_MASK_OTP                0x000000002

/* Module ID */
#define BT_MODULE_ID_GA                      (BT_MODULE_PAGE_2 | BT_MODULE_BIT_MASK_GA)
#define BT_MODULE_ID_OTP                     (BT_MODULE_PAGE_2 | BT_MODULE_BIT_MASK_OTP)

/* Page 3 - Utilities Modules */
#define BT_MODULE_PAGE_3                      0x30000000U

/* Module - Bit Mask */
#define BT_MODULE_BIT_MASK_STORAGE            0x00000001U
#define BT_MODULE_BIT_MASK_STATUS             0x00000002U
#define BT_MODULE_BIT_MASK_SNOOP              0x00000004U
#define BT_MODULE_BIT_MASK_BPPA               0x00000008U
#define BT_MODULE_BIT_MASK_OBEX_AL            0x00000010U
#define BT_MODULE_BIT_MASK_COVER_ART          0x00000020U
#define BT_MODULE_BIT_MASK_HT                 0x00000040U
#define BT_MODULE_BIT_MASK_OBJECT_PARSER      0x00000080U
#define BT_MODULE_BIT_MASK_AT_PARSER          0x00000100U
#define BT_MODULE_BIT_MASK_AES_CMAC           0x00000200U
#define BT_MODULE_BIT_MASK_TRANSPORT          0x00000400U
#define BT_MODULE_BIT_MASK_JPL                0x00000800U
#define BT_MODULE_BIT_MASK_SBC                0x00001000U
#define BT_MODULE_BIT_MASK_XML                0x00002000U
#define BT_MODULE_BIT_MASK_VCOM               0x00004000U
#define BT_MODULE_BIT_MASK_NIFACE             0x00008000U
#define BT_MODULE_BIT_MASK_TIMER              0x00010000U
#define BT_MODULE_BIT_MASK_FOPS               0x00020000U
#define BT_MODULE_BIT_MASK_ASSERT             0x00040000U
#define BT_MODULE_BIT_MASK_A2DP_PL            0x00080000U
#define BT_MODULE_BIT_MASK_CTN_PL             0x00100000U
#define BT_MODULE_BIT_MASK_MAP_PL             0x00200000U
#define BT_MODULE_BIT_MASK_HFP_PL             0x00400000U
#define BT_MODULE_BIT_MASK_GEN_PL             0x00800000U
#define BT_MODULE_BIT_MASK_HPS_PL             0x01000000U
#define BT_MODULE_BIT_MASK_HTTPS_PL           0x02000000U
/* Module ID */
#define BT_MODULE_ID_STORAGE                  (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_STORAGE)
#define BT_MODULE_ID_STATUS                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_STATUS)
#define BT_MODULE_ID_SNOOP                    (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_SNOOP)
#define BT_MODULE_ID_BPPA                     (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_BPPA)
#define BT_MODULE_ID_OBEX_AL                  (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_OBEX_AL)
#define BT_MODULE_ID_COVER_ART                (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_COVER_ART)
#define BT_MODULE_ID_HT                       (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_HT)
#define BT_MODULE_ID_OBJECT_PARSER            (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_OBJECT_PARSER)
#define BT_MODULE_ID_AT_PARSER                (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_AT_PARSER)
#define BT_MODULE_ID_AES_CMAC                 (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_AES_CMAC)
#define BT_MODULE_ID_TRANSPORT                (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_TRANSPORT)
#define BT_MODULE_ID_JPL                      (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_JPL)
#define BT_MODULE_ID_SBC                      (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_SBC)
#define BT_MODULE_ID_XML                      (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_XML)
#define BT_MODULE_ID_VCOM                     (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_VCOM)
#define BT_MODULE_ID_NIFACE                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_NIFACE)
#define BT_MODULE_ID_TIMER                    (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_TIMER)
#define BT_MODULE_ID_FOPS                     (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_FOPS)
#define BT_MODULE_ID_ASSERT                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_ASSERT)
#define BT_MODULE_ID_A2DP_PL                  (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_A2DP_PL)
#define BT_MODULE_ID_CTN_PL                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_CTN_PL)
#define BT_MODULE_ID_MAP_PL                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_MAP_PL)
#define BT_MODULE_ID_HFP_PL                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_HFP_PL)
#define BT_MODULE_ID_GEN_PL                   (BT_MODULE_PAGE_3 | BT_MODULE_BIT_MASK_GEN_PL)

#define BT_MODULE_STATE_INVALID               0x00U
#define BT_MODULE_STATE_INITIALIZED           0x01U

#define BT_DEFINE_MODULE_STATE(module_name) UINT8 g_bt_##module_name = BT_MODULE_STATE_INVALID;
#define BT_DECLARE_MODULE_STATE(module_name) extern UINT8 g_bt_##module_name;

/* Set init or deinit */
#define BT_SET_MODULE_STATE(module_name, s) g_bt_##module_name = BT_MODULE_STATE_##s;
#define IF_BT_MODULE_STATE(module_name, s) if(g_bt_##module_name == BT_MODULE_STATE_##s)

/* -------------------------------------------- Structures/Data Types */

/**
 * \addtogroup bt_common_defines Defines
 * \{
 */

/**
 * \defgroup bt_common_structures Structures
 * \{
 */

/* 128-Bit UUID */
typedef struct
{
    UCHAR byte[16U];

} UINT128_ID;


/* The UUID Data Type */
typedef struct
{
    UCHAR uuid_type;

    union
    {
        UINT16 uuid_16;
        UINT32 uuid_32;
        UINT128_ID uuid_128;
    } uuid_union;

} S_UUID;

/* Abstracted Bluetooth Device Address */
typedef struct _BT_DEVICE_ADDR
{
    UCHAR    addr[BT_BD_ADDR_SIZE];

    UCHAR    type;

} BT_DEVICE_ADDR;

#ifdef BT_SUPPORT_ERR_IND_CALLBACK
typedef struct _BT_ERROR_MSG
{
    /* Fixed Part */

    /**
     *  Identifies the remote device/connection, for which the error
     *  is being reported.
     *
     *  Note: If there is no remote device/connection involved,
     * the connection handle will be invalid (0xFFFF)
     */
    UINT16   connection_handle;

    /* Variable Part */

    /**
     * Error specific information, typically as a module specific
     * data structure.
     */
    void* error_info;

    /**
     * Length of error specific information.
     *
     * Note: For some of the module specific data structure,
     * the length field might not be used.
     */
    UINT16  length;

} BT_ERROR_MSG;
#endif /* BT_SUPPORT_ERR_IND_CALLBACK */
/** \} */

/** \} */

/* -------------------------------------------- Function/API Declarations */
#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup bt_common_callback Callback
 * \{
 */

/* Function to trigger indication of Bluetooth ON Complete */
API_RESULT BT_common_bluetooth_on_complete
           (
               void
           );


/** \} */

/**
 * \defgroup bt_common_api API Definitions
 * \{
 */

/**
 * API to initialize BlueLitE Stack. This is the first API that the
 * application should call before any other API. This function
 * initializes all the internal stack modules and creates necessary tasks.
 *
 * \note
 * - After this function, the application should call \ref BT_bluetooth_on
 * - BT_ethermind_init should be called only once during the lifetime of
 *   application. However, application may call \ref BT_bluetooth_on and
 *   \ref BT_bluetooth_off multiple times.
 */
void BT_ethermind_init
     (
         void
     );

#ifdef BT_HAVE_SHUTDOWN
/**
 * API to de-initialize/shutdown EtherMind Stack. This is the final API that the
 * application should call for the bluetooth stack. This function
 * de-initializes all the internal stack modules.
 *
 * \note
 * - After this function, the application should call \ref BT_ethermind_init()
 * before calling any other stack APIs.
 */
void BT_ethermind_shutdown
     (
         void
     );
#endif /* BT_HAVE_SHUTDOWN */

/**
 * API to initialize the Bluetooth Hardware. This API should be called after
 * \ref BT_ethermind_init. Application should call other API only after
 * successful completion of Bluetooth ON.
 *
 * \param [in] hci_event_ind_cb Application callback to receive HCI events
 * \param [in] bt_on_complete_cb Application callback to confirm
 *                                      completion of Bluetooth ON procedure
 * \param [in] local_name Name to be for the local device. When other devices
 *                   search, they see this name.
 *
 * \return
 *      \ref API_RESULT on successful start of Bluetooth ON procedure
 *
 * \note This is a non-blocking API that returns immediately after starting
 *       the Bluetooth ON procedure. The completion of the procedure is
 *       notified to the application through
 *       appl_bluetooth_on_complete_cb() callback. The application should
 *       wait for this callback before proceding further.
 */
API_RESULT BT_bluetooth_on
           (
               /* IN */ API_RESULT (* hci_event_ind_cb) (UCHAR event_type, UCHAR *event_data, UCHAR event_datalen),
               /* IN */ API_RESULT (* bt_on_complete_cb) (void),
               /* IN */ CHAR * local_name
           );

/**
 * API to turn off Bluetooth Hardware. This API should be called after
 * \ref BT_bluetooth_on.
 *
 * \return
 *      \ref API_RESULT on successful Bluetooth OFF
 */
API_RESULT BT_bluetooth_off
           (
               void
           );

#ifdef BT_SUPPORT_GET_STACK_INIT_STATE
/**
 * API to get current state of BlueLitE stack.
 *
 * \param [out] state  State of the stack - one of the states defined in
 *                                         \ref bt_common_stack_init_states
 *
 * \return
 *      \ref API_RESULT on successful retrieval of state.
 */
API_RESULT BT_bluetooth_get_stack_init_state (/* OUT */ UCHAR *state);
#endif /* BT_SUPPORT_GET_STACK_INIT_STATE */

#ifdef BT_SUPPORT_ERR_IND_CALLBACK
/**
 *  API for registering common the error Indication Callback.
 *
 *  \param: [in] Function Pointer: callback_ptr
 *
 *  \return
 *       \ref API_RESULT On successful registration of the callback pointer.
 */
API_RESULT BT_ethermind_register_error_indication_callback
           (
               API_RESULT (* callback_ptr)
                          (
                              UINT32    module_id,
                              UINT16   error_code,
                              void   * error_msg
                          )
           );

/**
 *  API for Core Modules to report their Errors to Upper layers through the
 *  Error Indication callback.
 *
 *  \param: [in] module_id   Bluetooth Module ID
 *  \param: [in] error_code  Identifier of the reported error
 *  \param: [in] error_msg   Information associated with the reported error
 *
 *  \return
 *       \ref API_RESULT Return value
 */
API_RESULT BT_error_indication_callback
           (
               UINT32   module_id,
               UINT16   error_code,
               void   * error_msg
           );
#endif /* BT_SUPPORT_ERR_IND_CALLBACK */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */

#endif /* _H_BT_COMMON_ */

