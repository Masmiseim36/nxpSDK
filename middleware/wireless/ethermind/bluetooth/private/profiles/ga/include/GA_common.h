
/**
 *  \file GA_common.h
 *
 *  \brief This file defines the Generic Audio Core Application Interface - includes
 *  feature defines
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_COMMON_
#define _H_GA_COMMON_


/* --------------------------------------------- Header File Inclusion */
/* GA OSAL headers */
#include "GA_os.h"
#include "GA_timer.h"
#include "GA_debug.h"

/* GA Assigned numbers header */
#include "GA_assigned_numbers.h"

#ifdef BT_GAM
/* GA features header */
#include "GA_features.h"
#endif /* BT_GAM */

/* GA limits header */
#include "GA_limits.h"

/* GA error header */
#include "GA_error.h"


/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \addtogroup ga_bap_defines
 * \{
 */

/**
 * \addtogroup ga_bap_constants
 * \{
 */

/**
 * \name Boolean Values
 * \{
 * \brief Describes Values of Boolean Values defined for GA module.
 */

/** GA Boolean True */
#define GA_TRUE                                     0x01U

/** GA Boolean False */
#define GA_FALSE                                    0x00U

/** \} */


/**
 * \name GA General
 * \{
 * \brief This section describes the General Macros defined for GA.
 */

/** Device Address size */
#define GA_BD_ADDR_SIZE            6

/** GA Response Type - Read */
#define GA_RSP_TYPE_RD             0x00

/** GA Response Type - Write */
#define GA_RSP_TYPE_WT             0x01

/** GA Response Type - Invalid */
#define GA_RSP_TYPE_INVALID        0xFF

/** Unused return value to avoid compilation warning - MISRA C-2012 Rule 17.7 */
#define GA_IGNORE_RETURN_VALUE    void

/** Loop for ever */
#define GA_LOOP_FOREVER()        for(;;)

/**
 * Module Identifier definitions.
 * Currently used for runtime debug enable/disable scenario.
 * In future, this can be used for other purposes as well,
 * hence these defines are placed under common header file.
 */
/* Page 0  */
#define GA_MODULE_PAGE_0                      0x00000000U

/* Module - Bit Mask */
#define GA_MODULE_BIT_MASK_BRR                0x00000001U
#define GA_MODULE_BIT_MASK_BAP                0x00000002U
#define GA_MODULE_BIT_MASK_BASS               0x00000004U
#define GA_MODULE_BIT_MASK_CAP                0x00000008U
#define GA_MODULE_BIT_MASK_CSIP               0x00000010U
#define GA_MODULE_BIT_MASK_MICP               0x00000020U
#define GA_MODULE_BIT_MASK_VCP                0x00000040U
#define GA_MODULE_BIT_MASK_MCP                0x00000080U
#define GA_MODULE_BIT_MASK_CCP                0x00000100U
#define GA_MODULE_BIT_MASK_HAP                0x00000200U
#define GA_MODULE_BIT_MASK_TMAP               0x00000400U

/* Module ID */
#define GA_MODULE_ID                          (GA_MODULE_PAGE_0)
#define GA_MODULE_ID_BRR                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_BRR)
#define GA_MODULE_ID_BAP                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_BAP)
#define GA_MODULE_ID_BASS                     (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_BASS)
#define GA_MODULE_ID_CAP                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_CAP)
#define GA_MODULE_ID_CSIP                     (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_CSIP)
#define GA_MODULE_ID_MICP                     (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_MICP)
#define GA_MODULE_ID_VCP                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_VCP)
#define GA_MODULE_ID_MCP                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_MCP)
#define GA_MODULE_ID_CCP                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_CCP)
#define GA_MODULE_ID_HAP                      (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_HAP)
#define GA_MODULE_ID_TMAP                     (GA_MODULE_PAGE_0 | GA_MODULE_BIT_MASK_TMAP)

/* Module global init states */
#define GA_MODULE_STATE_INVALID               0x00
#define GA_MODULE_STATE_INITIALIZED           0x01

#ifndef GA_CLEAN_SHUTDOWN
#define GA_MODULE_STATE_SHUTDOWN              0x02
#else /* GA_CLEAN_SHUTDOWN */
#define GA_MODULE_STATE_SHUTDOWN              GA_MODULE_STATE_INVALID
#endif /* GA_CLEAN_SHUTDOWN */

/** \} */
/** \} */

/* --------------------------------------------- Typedefs/Datastrucutres */
/**
 * \addtogroup ga_bap_structures
 * \{
 */
/** GA Endpoint */
typedef struct _GA_ENDPOINT
{
    /** Bluetooth Device Address */
    UCHAR bd_addr[GA_BD_ADDR_SIZE];

    /** Bluetooth Device Type */
    UINT8 bd_type;

} GA_ENDPOINT;

/** Characteristic UUID type */
typedef UINT16 GA_CHAR_UUID;

/** Characteristic Information */
typedef struct _GA_CHAR_INFO
{
    /** Characteristic Handle */
    UINT16 handle;

    /** Client Characteristic Descriptor Handle */
    UINT16 ccd_handle;

    /** Characteristic range */
    UINT16 range;

    /** Characteristic Property */
    UINT8 prop;

} GA_CHAR_INFO;

/** Codec Information */
typedef struct _GA_CODEC_INFO
{
    /** Coding Format */
    UINT8 coding_format;

    /** Company ID */
    UINT16 company_id;

    /** Vendor Specific Codec ID */
    UINT16 vendor_codec_id;

    /** Codec Specific Capabilities/Configuration Information Element */
    UCHAR ie[GA_CODEC_IE_LEN];

    /** Codec Information element length */
    UCHAR ie_len;

} GA_CODEC_INFO;

/** Codec Specific Capabilities/Configuration Information */
typedef struct _GA_CODEC_IE
{
    /** Codec Specific Capabilities/Configuration Information Element */
    UCHAR value[GA_CODEC_IE_LEN];

    /** Codec Information element length */
    UCHAR length;

} GA_CODEC_IE;

/** GA Metadata */
typedef struct _GA_METADATA
{
    /** Metadata byte stream */
    UCHAR data[GA_METADATA_LEN];

    /** Metadata length */
    UINT8 length;

} GA_METADATA;

#ifdef GA_HAVE_DYNAMIC_CONFIG
/**
 *  Dynamic configuration of GA Datastructure.
 *  Used only if 'GA_HAVE_DYNAMIC_CONFIG' is defined.
 */
typedef struct _GA_CONFIG
{
    /** Configure MAX Source PAC Records. \ref GA_MAX_SOURCE_PAC_RECORDS */
    UINT8 config_GA_MAX_SOURCE_PAC_RECORDS;
    /** Configure MAX Sink PAC Records. \ref GA_MAX_SINK_PAC_RECORDS */
    UINT8 config_GA_MAX_SINK_PAC_RECORDS;
    /** Configure MAX GA SEP. \ref GA_MAX_SEP */
    UINT8 config_GA_MAX_SEP;
    /** Configure MAX GA Contexts. \ref GA_MAX_CONTEXTS */
    UINT8 config_GA_MAX_CONTEXTS;
    /**
     * Configure MAX Source Stream Contexts.
     * \ref GA_MAX_SOURCE_STREAM_CONTEXTS
     */
    UINT8 config_GA_MAX_SOURCE_STREAM_CONTEXTS;
    /** Configure MAX Sink Stream Contexts. \ref GA_MAX_SINK_STREAM_CONTEXTS */
    UINT8 config_GA_MAX_SINK_STREAM_CONTEXTS;
    /** Configure MAX CCS. \ref GA_MAX_CCS */
    UINT8 config_GA_MAX_CCS;
    /** Configure MAX Broadcast Sessions. \ref GA_MAX_BROADCAST_SESSIONS */
    UINT8 config_GA_MAX_BROADCAST_SESSIONS;
    /** Configure MAX Broadcast SEP. \ref GA_MAX_BROADCAST_SEP */
    UINT8 config_GA_MAX_BROADCAST_SEP;
    /**
     * Configure MAX Broadcast Sink Sessions.
     * \ref GA_MAX_BROADCAST_SINK_SESSIONS
     */
    UINT8 config_GA_MAX_BROADCAST_SINK_SESSIONS;
    /** Configure CAP CSM lock timeout. \ref GA_CAP_CSM_LOCK_TIMEOUT */
    UINT8 config_GA_CAP_CSM_LOCK_TIMEOUT;
    /** Configure MAX VCP-VCS Contexts. \ref VCP_MAX_VCS_CONTEXTS */
    UINT8 config_VCP_MAX_VCS_CONTEXTS;
    /** Configure MAX VCP-VOCS Contexts. \ref VCP_MAX_VOCS_CONTEXTS */
    UINT8 config_VCP_MAX_VOCS_CONTEXTS;
    /** Configure MAX VCP-AICS Contexts. \ref VCP_MAX_AICS_CONTEXTS */
    UINT8 config_VCP_MAX_AICS_CONTEXTS;
    /** Configure MAX VCP-VOCS Entities. \ref VCP_MAX_VOCS_ENTITIES */
    UINT8 config_VCP_MAX_VOCS_ENTITIES;
    /** Configure MAX VCP-AICS Entities. \ref VCP_MAX_AICS_ENTITIES */
    UINT8 config_VCP_MAX_AICS_ENTITIES;
    /** Configure MAX MICP-MICS Contexts. \ref MICP_MAX_MICS_CONTEXTS */
    UINT8 config_MICP_MAX_MICS_CONTEXTS;
    /** Configure MAX MICP-AICS Contexts. \ref MICP_MAX_AICS_CONTEXTS */
    UINT8 config_MICP_MAX_AICS_CONTEXTS;
    /** Configure MAX MICP-AICS Entities. \ref MICP_MAX_AICS_ENTITIES */
    UINT8 config_MICP_MAX_AICS_ENTITIES;
    /** Configure MAX CSIS Contexts. \ref CSIP_MAX_CONTEXTS */
    UINT8 config_CSIP_MAX_CONTEXTS;
    /** Configure MAX CSIS Entities. \ref CSIP_MAX_CSIS_ENTITIES */
    UINT8 config_CSIP_MAX_CSIS_ENTITIES;
    /** Configure MAX CCP Contexts. \ref CCP_MAX_CONTEXTS */
    UINT8 config_CCP_MAX_CONTEXTS;
    /** Configure MAX TBS Contexts. \ref CCP_MAX_TBS_CONTEXTS */
    UINT8 config_CCP_MAX_TBS_CONTEXTS;
    /** Configure MAX TBS Entities. \ref CCP_MAX_TBS_ENTITIES */
    UINT8 config_CCP_MAX_TBS_ENTITIES;
    /** Configure MAX MCP Contexts. \ref MCP_MAX_CONTEXTS */
    UINT8 config_MCP_MAX_CONTEXTS;
    /** Configure MAX MCS Contexts. \ref MCP_MAX_MCS_CONTEXTS */
    UINT8 config_MCP_MAX_MCS_CONTEXTS;
    /** Configure MAX MCS Entities. \ref MCP_MAX_MCS_ENTITIES */
    UINT8 config_MCP_MAX_MCS_ENTITIES;
    /** Configure MAX HAS Contexts. \ref HAP_MAX_HAS_CONTEXTS */
    UINT8 config_HAP_MAX_HAS_CONTEXTS;
    /** Configure MAX TMAS Contexts. \ref TMAP_MAX_TMAS_CONTEXTS */
    UINT8 config_TMAP_MAX_TMAS_CONTEXTS;
    /** Configure MAX BASS Contexts. \ref BASS_CE_MAX_INSTANCES */
    UINT8 config_BASS_CE_MAX_INSTANCES;
    /** Configure MAX BASS Entities. \ref BASS_CE_MAX_BC_RX_STATE_INSTANCES */
    UINT8 config_BASS_CE_MAX_BC_RX_STATE_INSTANCES;
    /** Configure MAX BC RX State Entries. \ref BASS_SE_MAX_BC_RX_STATE_ENTITIES */
    UINT8 config_BASS_SE_MAX_BC_RX_STATE_ENTITIES;
} GA_CONFIG;

/** Global Configuration for GA */
extern GA_CONFIG ga_global_config;
#endif /* GA_HAVE_DYNAMIC_CONFIG */

/** \} */

/* --------------------------------------------- Macros */

/**
 * \defgroup ga_cap_utility_macros Macros
 * \{
 * \brief This section describes Initialization and other Utility Macros
 * offered by GA.
 */

/**
 * \name GA Packing Unpacking Related
 * \{
 * \brief This section describes the Packing Unpacking macros defined for GA.
 */

/** Pack LE 1 Byte */
#define GA_PACK_LE_1_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (src);

/** Pack LE 2 Byte */
#define GA_PACK_LE_2_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src); \
    *((UCHAR *)(dst) + 1) = (UCHAR)((src) >> 8);

/** Pack LE 3 Byte */
#define GA_PACK_LE_3_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src);\
    *((UCHAR *)(dst) + 1) = (UCHAR)((src) >> 8);\
    *((UCHAR *)(dst) + 2) = (UCHAR)((src) >> 16);

/** Pack LE 4 Byte */
#define GA_PACK_LE_4_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src);\
    *((UCHAR *)(dst) + 1) = (UCHAR)((src) >> 8);\
    *((UCHAR *)(dst) + 2) = (UCHAR)((src) >> 16);\
    *((UCHAR *)(dst) + 3) = (UCHAR)((src) >> 24);

/** Unpack LE 1 Byte */
#define GA_UNPACK_LE_1_BYTE(dst,src)\
    *((UCHAR *)(dst)) = (UCHAR)(*((UCHAR *)(src)));

/** Unpack LE 2 Byte */
#define GA_UNPACK_LE_2_BYTE(dst,src)\
        *((UINT16 *)(dst))  = *((src) + 1); \
        *((UINT16 *)(dst))  = *((UINT16 *)(dst)) << 8; \
        *((UINT16 *)(dst)) |= *((src) + 0);

/** Unpack LE 3 Byte */
#define GA_UNPACK_LE_3_BYTE(dst,src)\
        *((UINT32 *)(dst))  = *((src) + 2);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 1);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 0);

/** Unpack LE 4 Byte */
#define GA_UNPACK_LE_4_BYTE(dst,src)\
        *((UINT32 *)(dst))  = *((src) + 3);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 2);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 1);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 0);

/** \} */

/** Define/Declare global module state */
#define GA_DEFINE_MODULE_STATE(module_name) UINT8 g_ga_##module_name = GA_MODULE_STATE_INVALID;
#define GA_DECLARE_MODULE_STATE(module_name) extern UINT8 g_ga_##module_name;

/** Set/Check global module state */
#define GA_SET_MODULE_STATE(module_name, s) g_ga_##module_name = GA_MODULE_STATE_##s;
#define IF_GA_MODULE_STATE(module_name, s)  if(g_ga_##module_name == GA_MODULE_STATE_##s)
#define IFN_GA_MODULE_STATE(module_name, s) if(g_ga_##module_name != GA_MODULE_STATE_##s)

#ifndef GA_DISABLE_MUTEX

/**
 * \name GA Synchronization Related
 * \{
 * \brief This section describes the Mutex and Conditional Variables
 *        related macros defined for GA.
 */

/* Macro to define a Mutex Variable */
#define GA_DEFINE_MUTEX(mutex) EM_thread_mutex_type mutex;

/* Macro to define a Mutex Variable with a type qualifier */
#define GA_DEFINE_MUTEX_TYPE(type, mutex) type EM_thread_mutex_type mutex;

/* Macro to define a Conditional Variable */
#define GA_DEFINE_COND(cond) EM_thread_cond_type cond;

/* Macro to define a Conditional Variable with a type qualifier */
#define GA_DEFINE_COND_TYPE(type, cond) type EM_thread_cond_type cond;

/*
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define GA_MUTEX_INIT_VOID(mutex, MODULE)                                \
    if (EM_thread_mutex_init(&(mutex), NULL) < 0)                        \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Mutex in " #MODULE ".\n");                 \
        return;                                                          \
    }

/*
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define GA_MUTEX_INIT(mutex, MODULE)                                     \
    if (EM_thread_mutex_init(&(mutex), NULL) < 0)                        \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Mutex in " #MODULE ".\n");                 \
        return MODULE##_MUTEX_INIT_FAILED;                               \
    }

#ifdef GA_CLEAN_SHUTDOWN
#error "Complete Implementation of Mutex De-Initialization"

/*
 *  Macro to De-Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define GA_MUTEX_DEINIT_VOID(mutex, MODULE)                              \
    if (0)                                                               \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to De-Initialize Mutex in " #MODULE ".\n");              \
        return;                                                          \
    }

/*
 *  Macro to De-Initialize Mutex.
 *  It returns an error if mutex de-initialization fails.
 */
#define GA_MUTEX_DEINIT(mutex, MODULE)                                   \
    if (0)                                                               \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to De-Initialize Mutex in " #MODULE ".\n");              \
        return MODULE##_MUTEX_INIT_FAILED;                               \
    }

#else /* GA_CLEAN_SHUTDOWN */
/*
 *  Macro to De-Initialize Mutex.
 */
#define GA_MUTEX_DEINIT_VOID(mutex, MODULE)
#define GA_MUTEX_DEINIT(mutex, MODULE)

#endif /* GA_CLEAN_SHUTDOWN */

/*
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define GA_COND_INIT_VOID(cond, MODULE)                                  \
    if (EM_thread_cond_init(&(cond), NULL) < 0)                          \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Conditional Variable in " #MODULE ".\n");  \
        return;                                                          \
    }

/*
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define GA_COND_INIT(cond, MODULE)                                       \
    if (EM_thread_cond_init(&(cond), NULL) < 0)                          \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Conditional Variable in " #MODULE ".\n");  \
        return MODULE##_COND_INIT_FAILED;                                \
    }

#ifdef GA_CLEAN_SHUTDOWN
#error "Complete Implementation of CondVar De-Initialization"

/*
 *  Macro to De-Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define GA_COND_DEINIT_VOID(cond, MODULE)                                \
    if (0)                                                               \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to De-Initialize Conitional Variable in " #MODULE ".\n");\
        return;                                                          \
    }

/*
 *  Macro to De-Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define GA_COND_DEINIT(cond, MODULE)                                     \
    if (0)                                                               \
    {                                                                    \
        GA_debug_error(                                                  \
        GA_MODULE_ID_##MODULE,                                           \
        "FAILED to De-Initialize Conitional Variable in " #MODULE ".\n");\
        return MODULE##_COND_INIT_FAILED;                                \
    }

#else /* GA_CLEAN_SHUTDOWN */
/*
 *  Macro to De-Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define GA_COND_DEINIT_VOID(cond, MODULE)
#define GA_COND_DEINIT(cond, MODULE)

#endif /* GA_CLEAN_SHUTDOWN */

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define GA_MUTEX_LOCK(mutex, MODULE)                                 \
    if (EM_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        GA_debug_error(                                              \
        GA_MODULE_ID_##MODULE,                                       \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return MODULE##_MUTEX_LOCK_FAILED;                           \
    }

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define GA_MUTEX_LOCK_VOID(mutex, MODULE)                            \
    if (EM_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        GA_debug_error(                                              \
        GA_MODULE_ID_##MODULE,                                       \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return;                                                      \
    }

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define GA_MUTEX_UNLOCK(mutex, MODULE)                               \
    if (EM_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        GA_debug_error(                                              \
        GA_MODULE_ID_##MODULE,                                       \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return MODULE##_MUTEX_UNLOCK_FAILED;                         \
    }

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define GA_MUTEX_UNLOCK_VOID(mutex, MODULE)                          \
    if (EM_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        GA_debug_error(                                              \
        GA_MODULE_ID_##MODULE,                                       \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return;                                                      \
    }

/** \} */

#else /* GA_DISABLE_MUTEX */

/**
 * \name GA Mutex Related
 * \{
 * \brief This section describes the Mutex related macros defined for GA.
 */

/* Macro to define a Mutex Variable */
#define GA_DEFINE_MUTEX(mutex)

/* Macro to define a Mutex Variable with a type qualifier */
#define GA_DEFINE_MUTEX_TYPE(type, mutex)

/* Macro to define a Conditional Variable */
#define GA_DEFINE_COND(cond)

/* Macro to define a Conditional Variable with a type qualifier */
#define GA_DEFINE_COND_TYPE(type, cond)

/*
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define GA_MUTEX_INIT_VOID(mutex, MODULE)

/*
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define GA_MUTEX_INIT(mutex, MODULE)

/*
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define GA_COND_INIT_VOID(cond, MODULE)

/*
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define GA_COND_INIT(cond, MODULE)

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define GA_MUTEX_LOCK(mutex, MODULE)

/*
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define GA_MUTEX_LOCK_VOID(mutex, MODULE)

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define GA_MUTEX_UNLOCK(mutex, MODULE)

/*
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define GA_MUTEX_UNLOCK_VOID(mutex, MODULE)

/** \} */

#endif /* GA_DISABLE_MUTEX */

/**
 * \name GA BD Address Related
 * \{
 * \brief This section describes the BD address related macros defined for GA.
 */

/** Initialize the endpoint */
#define GA_INIT_ENDPOINT(dev) \
        EM_mem_set ((dev)->bd_addr, 0, GA_BD_ADDR_SIZE); \
        (dev)->bd_type = 0

/** Check Endpoint address for Non-Zero */
#define GA_IS_NON_ZERO_ENDPOINT_ADDR(addr)\
        ((0x00 == (*((addr) + 0) | *((addr) + 1) | *((addr) + 2) | *((addr) + 3) | *((addr) + 4) | *((addr) + 5)))?\
        GA_FALSE:GA_TRUE)

/** Copy Endpoint address */
#define GA_COPY_ENDPOINT_ADDR(dst, src) \
        EM_mem_copy((dst)->bd_addr, (src)->bd_addr, GA_BD_ADDR_SIZE);

/** Copy Endpoint type */
#define GA_COPY_ENDPOINT_TYPE(dst, src) \
        (dst)->bd_type = (src)->bd_type

/**
 * Copy Endpoint.
 * It copies Endpoint address and type
 */
#define GA_COPY_ENDPOINT(dst, src) \
        GA_COPY_ENDPOINT_ADDR(dst, src) \
        GA_COPY_ENDPOINT_TYPE(dst, src)

/** Compare Endpoint type */
#define GA_COMPARE_ENDPOINT_TYPE(type_a,type_b)\
        (((type_a) == (type_b))?GA_TRUE:GA_FALSE)

/** Compare Endpoint address */
#define GA_COMPARE_ENDPOINT_ADDR(addr_a,addr_b)\
        ((0 == GA_mem_cmp((addr_a), (addr_b), GA_BD_ADDR_SIZE))?GA_TRUE:GA_FALSE)

/**
 * Compare Endpoint.
 * It compares Endpoint address and type
 */
#define GA_COMPARE_ENDPOINT(addr_a,addr_b)\
        (((GA_COMPARE_ENDPOINT_TYPE((addr_a)->bd_type,(addr_b)->bd_type)) &&\
          (GA_COMPARE_ENDPOINT_ADDR((addr_a)->bd_addr,(addr_b)->bd_addr)))? GA_TRUE: GA_FALSE)

/** Dynamic memory var definition, initialization and deinitialization */
#define GA_DEFINE_PTR(type, var, s) \
        type * var

/* TODO: Check if the allocation is success */
#define GA_INIT_PTR(type, var, s, i) \
        var = EM_alloc_mem((s) * sizeof(type)); \
        GA_debug_trace(GA_MODULE_ID, \
        "Allocated %lu bytes for %s. Ptr: %p\n", ((s) * sizeof(type)), #var, var); \
        EM_mem_set(var, (i), ((s) * sizeof(type)))

#define GA_DEINIT_PTR(type, var, s) \
        EM_free_mem(var)

#ifdef GA_HAVE_DYNAMIC_CONFIG
#define GA_INIT_CONFIG(config) \
    (config).config_GA_MAX_SOURCE_PAC_RECORDS = GA_MAX_SOURCE_PAC_RECORDS; \
    (config).config_GA_MAX_SINK_PAC_RECORDS = GA_MAX_SINK_PAC_RECORDS; \
    (config).config_GA_MAX_SEP = GA_MAX_SEP; \
    (config).config_GA_MAX_CONTEXTS = GA_MAX_CONTEXTS; \
    (config).config_GA_MAX_SOURCE_STREAM_CONTEXTS = GA_MAX_SOURCE_STREAM_CONTEXTS; \
    (config).config_GA_MAX_SINK_STREAM_CONTEXTS = GA_MAX_SINK_STREAM_CONTEXTS; \
    (config).config_GA_MAX_CCS = GA_MAX_CCS; \
    (config).config_GA_MAX_BROADCAST_SESSIONS = GA_MAX_BROADCAST_SESSIONS; \
    (config).config_GA_MAX_BROADCAST_SEP = GA_MAX_BROADCAST_SEP; \
    (config).config_GA_MAX_BROADCAST_SINK_SESSIONS = GA_MAX_BROADCAST_SINK_SESSIONS; \
    (config).config_GA_CAP_CSM_LOCK_TIMEOUT = GA_CAP_CSM_LOCK_TIMEOUT; \
    (config).config_VCP_MAX_VCS_CONTEXTS = VCP_MAX_VCS_CONTEXTS; \
    (config).config_VCP_MAX_VOCS_CONTEXTS = VCP_MAX_VOCS_CONTEXTS; \
    (config).config_VCP_MAX_AICS_CONTEXTS = VCP_MAX_AICS_CONTEXTS; \
    (config).config_VCP_MAX_VOCS_ENTITIES = VCP_MAX_VOCS_ENTITIES; \
    (config).config_VCP_MAX_AICS_ENTITIES = VCP_MAX_AICS_ENTITIES; \
    (config).config_MICP_MAX_MICS_CONTEXTS = MICP_MAX_MICS_CONTEXTS; \
    (config).config_MICP_MAX_AICS_CONTEXTS = MICP_MAX_AICS_CONTEXTS; \
    (config).config_MICP_MAX_AICS_ENTITIES = MICP_MAX_AICS_ENTITIES; \
    (config).config_CSIP_MAX_CONTEXTS = CSIP_MAX_CONTEXTS; \
    (config).config_CSIP_MAX_CSIS_ENTITIES = CSIP_MAX_CSIS_ENTITIES; \
    (config).config_CCP_MAX_CONTEXTS = CCP_MAX_CONTEXTS; \
    (config).config_CCP_MAX_TBS_CONTEXTS = CCP_MAX_TBS_CONTEXTS; \
    (config).config_CCP_MAX_TBS_ENTITIES = CCP_MAX_TBS_ENTITIES; \
    (config).config_MCP_MAX_CONTEXTS = MCP_MAX_CONTEXTS; \
    (config).config_MCP_MAX_MCS_CONTEXTS = MCP_MAX_MCS_CONTEXTS; \
    (config).config_MCP_MAX_MCS_ENTITIES = MCP_MAX_MCS_ENTITIES; \
    (config).config_HAP_MAX_HAS_CONTEXTS = HAP_MAX_HAS_CONTEXTS; \
    (config).config_TMAP_MAX_TMAS_CONTEXTS = TMAP_MAX_TMAS_CONTEXTS; \
    (config).config_BASS_CE_MAX_INSTANCES = BASS_CE_MAX_INSTANCES; \
    (config).config_BASS_CE_MAX_BC_RX_STATE_INSTANCES = BASS_CE_MAX_BC_RX_STATE_INSTANCES; \
    (config).config_BASS_SE_MAX_BC_RX_STATE_ENTITIES = BASS_SE_MAX_BC_RX_STATE_ENTITIES; \

#define GA_CONFIG_LIMITS(x) \
        ga_global_config.config_##x
#define GA_CONFIG_ISEQUAL(x, y)  0
#define GA_CONFIG_ISNEQUAL(x, y) 1

#define GA_DEFINE_GLOBAL_ARRAY(type, var, s) \
        type * var

#define GA_DECLARE_GLOBAL_ARRAY(type, var, s) \
        extern type * var

/* TODO: Check if the allocation is success */
#define GA_INIT_GLOBAL_ARRAY(type, var, s, i) \
        var = EM_alloc_mem((s) * sizeof(type)); \
        GA_debug_trace(GA_MODULE_ID, \
        "Allocated %lu bytes for %s. Ptr: %p\n", ((s) * sizeof(type)), #var, var); \
        EM_mem_set(var, (i), ((s) * sizeof(type)))

#define GA_DEINIT_GLOBAL_ARRAY(type, var, s) \
        EM_free_mem(var)

#define GA_DEFINE_LOCAL_ARRAY(type, var, s) \
        type * var

/* TODO: Check if the allocation is success */
#define GA_INIT_LOCAL_ARRAY(type, var, s, i) \
        var = EM_alloc_mem((s) * sizeof(type)); \
        GA_debug_trace(GA_MODULE_ID, \
        "Allocated %lu bytes for %s. Ptr: %p\n", ((s) * sizeof(type)), #var, var); \
        EM_mem_set(var, (i), ((s) * sizeof(type)))

#define GA_DEINIT_LOCAL_ARRAY(type, var, s) \
        EM_free_mem(var)

#else /* GA_HAVE_DYNAMIC_CONFIG */

#define GA_INIT_CONFIG(config)
#define GA_CONFIG_LIMITS(x) (x)
#define GA_CONFIG_ISEQUAL(x, y)    ((x) == (y))
#define GA_CONFIG_ISNEQUAL(x, y)   ((x) != (y))

/** Global variables definition, declaration, initialization and deinitialization */
#define GA_DEFINE_GLOBAL_ARRAY(type, var, s) \
        type var[(s)]

#define GA_DECLARE_GLOBAL_ARRAY(type, var, s) \
        extern type var[(s)]

#define GA_INIT_GLOBAL_ARRAY(type, var, s, i) \
        EM_mem_set(var, (i), ((s) * sizeof(type)))

#define GA_DEINIT_GLOBAL_ARRAY(type, var, s)

/** Local variables definition, initialization and deinitialization */
#define GA_DEFINE_LOCAL_ARRAY(type, var, s) \
        type var[(s)]

#define GA_INIT_LOCAL_ARRAY(type, var, s, i) \
        EM_mem_set(var, (i), ((s) * sizeof(type)))

#define GA_DEINIT_LOCAL_ARRAY(type, var, s)

#endif /* GA_HAVE_DYNAMIC_CONFIG */

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */
/** \} */

#endif /* _H_GA_COMMON_ */
