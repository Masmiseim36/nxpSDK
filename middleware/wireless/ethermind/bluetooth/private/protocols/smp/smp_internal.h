
/**
 *  \file smp_internal.h
 *
 *  Security Manager Protocol Internal header file. This file contains
 *  data structures, type definitions and function declarations that security
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SMP_INTERNAL_
#define _H_SMP_INTERNAL_

/* ----------------------------------------- Header File Inclusion */
#include "BT_smp_api.h"
#include "BT_bottom_half.h"
#include "BT_hci_api.h"
#include "BT_status.h"

#include "smp.h"
#include "smp_pl.h"
#include "l2cap.h"
#include "fsm_engine.h"
#include "write_task.h"

#include "smp_fsm.h"
#include "smp_fsm_engine.h"

#if ((defined SMP_DATA_SIGNING) || (defined SMP_LESC))
#include "aes_cmac.h"
#endif /* SMP_DATA_SIGNING || SMP_LESC */

#ifdef BT_SECURITY_VU_VALIDATION
#include "BT_security.h"
#endif /* BT_SECURITY_VU_VALIDATION */

/* ----------------------------------------- SMP Debugging */
#ifndef SMP_NO_DEBUG
#define SMP_ERR(...)          BT_debug_error(BT_MODULE_ID_SMP, __VA_ARGS__)
#define SMP_LP_ERR(...)       BT_debug_error(BT_MODULE_ID_SMP, __VA_ARGS__)
#else   /* SMP_NO_DEBUG */
#define SMP_ERR               BT_debug_null
#define SMP_LP_ERR            BT_debug_null
#endif  /* SMP_NO_DEBUG */

#ifdef SMP_DEBUG

#define SMP_TRC(...)          BT_debug_trace(BT_MODULE_ID_SMP, __VA_ARGS__)
#define SMP_INF(...)          BT_debug_info(BT_MODULE_ID_SMP, __VA_ARGS__)

#define SMP_debug_dump_bytes(data, datalen) BT_debug_dump_bytes(BT_MODULE_ID_SMP, (data), (datalen))

#else /* SMP_DEBUG */

#define SMP_TRC               BT_debug_null
#define SMP_INF               BT_debug_null

#define SMP_debug_dump_bytes(data, datalen)

#endif /* SMP_DEBUG */


#ifdef SMP_LP_DEBUG

#define SMP_LP_TRC(...)       BT_debug_trace(BT_MODULE_ID_SMP, __VA_ARGS__)
#define SMP_LP_INF(...)       BT_debug_info(BT_MODULE_ID_SMP, __VA_ARGS__)

#else /* SMP_DEBUG */

#define SMP_LP_TRC            BT_debug_null
#define SMP_LP_INF            BT_debug_null

#endif /* SMP_DEBUG */


/* ----------------------------------------- Global Definitions */
/** SMP Commands size */
#define SMP_PAIRING_REQUEST_CMD_SIZE                7U
#define SMP_PAIRING_RESPONSE_CMD_SIZE               7U
#define SMP_PAIRING_CONFIRM_CMD_SIZE                17U
#define SMP_PAIRING_FAILED_CMD_SIZE                 2U
#define SMP_ENCRYPTION_INFORMATION_CMD_SIZE         17U
#define SMP_MASTER_IDENTIFICATION_CMD_SIZE          11U
#define SMP_IDENTITY_INFORMATION_CMD_SIZE           17U
#define SMP_IDENTITY_ADDRESS_INFORMATION_CMD_SIZE   8U
#define SMP_SIGNATURE_INFORMATION_CMD_SIZE          17U
#define SMP_SECURITY_REQUEST_CMD_SIZE               2U
#ifdef SMP_LESC
#define SMP_PAIRING_PUBLIC_KEY_CMD_SIZE             65U
#define SMP_PAIRING_DHKEY_CHECK_CMD_SIZE            17U
#define SMP_KEYPRESS_NOTIFICATION_CMD_SIZE          2U
#endif /* SMP_LESC */

/** Minimum and Maximum SMP PDU size */
#define SMP_MINIMUM_PDU_SIZE   SMP_PAIRING_FAILED_CMD_SIZE
#ifndef SMP_LESC
#define SMP_MAXIMUM_PDU_SIZE   SMP_ENCRYPTION_INFORMATION_CMD_SIZE
#else
#define SMP_MAXIMUM_PDU_SIZE   SMP_PAIRING_PUBLIC_KEY_CMD_SIZE
#endif /* SMP_LESC */
/** SMP Invalid State */
#define SMP_STATE_INVALID                       0xFFU

/* SMP Validity values */
#define SMP_ENTITY_INVALID                      0x00U
#define SMP_ENTITY_VALID_TRUSTED                0x01U
#define SMP_ENTITY_VALID_UNTRUSTED              0x02U
#define SMP_ENTITY_VALIDITY_MASK                0xFCU

/* SMP Entity States */
#define SMP_ENTITY_IDLE                         0x00U
#define SMP_ENTITY_CONNECTED                    0x04U
#define SMP_ENTITY_CONNECTION_MASK              0xF3U

/* SMP Device Role */
#define SMP_ENTITY_MASTER                       0x00U
#define SMP_ENTITY_SLAVE                        0x10U
#define SMP_ENTITY_ROLE_MASK                    0xEFU

/* SMP Connected over BR/EDR */
#define SMP_LINK_BREDR_ACTIVE                   0x80U
#define SMP_LINK_BLE_ACTIVE                     0x40U
#define SMP_LINK_MASK                           0x3FU

/* SMP Entity Encryption state masks for security attribute */
#define SMP_ENTITY_ENC_OFF                      0x00U
#define SMP_ENTITY_ENC_IN_PROGRESS              0x04U
#define SMP_ENTITY_ENC_ON                       0x08U
#define SMP_ENTITY_ENC_MASK                     0xF3U

/* SMP Entity Bonding state masks for security attribute */
#define SMP_ENTITY_BOND_OFF                     0x00U
#define SMP_ENTITY_BOND_ON                      0x10U
#define SMP_ENTITY_BOND_MASK                    0xEFU

#ifdef SMP_LESC
/** SMP LE Secure Connections state masks for security attribute */
#define SMP_ENTITY_LESC_PAIRING_OFF             0x00U
#define SMP_ENTITY_LESC_PAIRING_ON              0x20U
#define SMP_ENTITY_LESC_PAIRING_MASK            0xDFU
#endif /* SMP_LESC */

/* Local Key Exchange Bit Masks */
#define SMP_LOCAL_ENCKEY                        0x01U
#define SMP_LOCAL_IDKEY                         0x02U
#define SMP_LOCAL_SIGNKEY                       0x04U
#define SMP_LOCAL_KEY_MASK                      0xF8U

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_LOCAL_LINK_KEY                      0x08U
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_SUPPORTED_KEY_MASK                  0x0FU
#else
#define SMP_SUPPORTED_KEY_MASK                  0x07U
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/* Remote Key Exchange Bit Masks */
#define SMP_REMOTE_ENCKEY                       0x10U
#define SMP_REMOTE_IDKEY                        0x20U
#define SMP_REMOTE_SIGNKEY                      0x40U
#define SMP_REMOTE_KEY_MASK                     0x8FU

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_REMOTE_LINK_KEY                     0x80U
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/* SMP Private Address states */
#define SMP_W4_RAND_COMPLETE                    0x01U
#define SMP_PVT_ADDR_CREATE_W4_AH_EVAL_COMPLETE 0x02U
#define SMP_PVT_ADDR_RESOLV_W4_AH_EVAL_COMPLETE 0x03U

/** Key value size defines */
#define SMP_CONFIRM_VALUE_SIZE                  16U
#define SMP_TEMP_KEY_SIZE                       16U
#define SMP_PVT_ADDR_HASH_SIZE                  3U
#define SMP_PVT_ADDR_RAND_SIZE                  3U
#ifdef SMP_LESC
#define SMP_LESC_KEY_SIZE                       16U
#define SMP_LESC_DHKEY_CHECK_VALUE_SIZE         16U
#define SMP_LESC_IOCAPB_SIZE                    3U
#define SMP_LESC_KEYID_SALT_SIZE                16U
#define SMP_LESC_KEYID_SIZE                     4U

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_LESC_KEYID_TMP_SIZE                 16U
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#ifdef SMP_LESC
/** SMP LESC Tool Box Input Length Defines */
#define SMP_TBX_F4_PLAIN_TEXT_LEN            SMP_LESC_PUBLIC_KEY_X_SIZE +\
                                             SMP_LESC_PUBLIC_KEY_X_SIZE +\
                                             sizeof(UCHAR)

#define SMP_TBX_F5_KEYGEN_PLAIN_TEXT_LEN     sizeof(UCHAR)          +\
                                             SMP_LESC_KEYID_SIZE    +\
                                             SMP_RAND_128B_SIZE     +\
                                             SMP_RAND_128B_SIZE     +\
                                             BT_BD_DEV_ADDR_SIZE    +\
                                             BT_BD_DEV_ADDR_SIZE    +\
                                             sizeof(UINT16)

#define SMP_TBX_F6_PLAIN_TEXT_LEN            SMP_RAND_128B_SIZE     +\
                                             SMP_RAND_128B_SIZE     +\
                                             SMP_TEMP_KEY_SIZE      +\
                                             SMP_LESC_IOCAPB_SIZE   +\
                                             BT_BD_DEV_ADDR_SIZE    +\
                                             BT_BD_DEV_ADDR_SIZE

#define SMP_TBX_G2_PLAIN_TEXT_LEN            SMP_LESC_PUBLIC_KEY_X_SIZE +\
                                             SMP_LESC_PUBLIC_KEY_X_SIZE +\
                                             SMP_RAND_128B_SIZE

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_TBX_H6_PLAIN_TEXT_LEN            SMP_LESC_KEYID_SIZE
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

#endif /* SMP_LESC */

/** Invalid Index Values */
#define SMP_INVALID_DEVICE_INDEX                0xFFU
#define SMP_L2CAP_INVALID_SIG_ID                0xFFFFFFFFU

/* SMP Association Models */
#define SMP_JW                                   0x00U
#define SMP_PK_0                                 0x01U
#define SMP_PK_1                                 0x02U
#define SMP_PK_2                                 0x03U
#define SMP_OOB                                  0x04U
#ifdef SMP_LESC
#define SMP_NC                                   0x05U
#endif /* SMP_LESC */

/* SMP Security property bitmasks */
#define SMP_AUTHREQ_BOND_BITMASK                 0x03U
#define SMP_AUTHREQ_MITM_BITMASK                 0x04U
#define SMP_AUTHREQ_LESC_BITMASK                 0x08U
#define SMP_AUTHREQ_KEY_PRESS_BITMASK            0x10U
#define SMP_AUTHREQ_CT2_BITMASK                  0x20U

/* SMP Toolbox state defines */
#define SMP_TBX_AH_W4_ENCRYPTION_COMPLETE        0x00U
#define SMP_TBX_C11_W4_ENCRYPTION_COMPLETE       0x01U
#define SMP_TBX_C12_W4_ENCRYPTION_COMPLETE       0x02U
#define SMP_TBX_S1_W4_ENCRYPTION_COMPLETE        0x03U
#define SMP_TBX_W4_RAND                          0x04U
#ifdef SMP_LESC
#define SMP_TBX_W4_READ_PUB_KEY_COMPLETE         0x05U
#define SMP_TBX_W4_DHKEY_GEN_COMPLETE            0x06U
/* TODO: Define TBX states for f4, f5, f6, g2 & h6 */
#define SMP_TBX_F4_W4_ENCRYPTION_COMPLETE        0x07U
#define SMP_TBX_F51_W4_ENCRYPTION_COMPLETE       0x08U
#define SMP_TBX_F52_W4_ENCRYPTION_COMPLETE       0x09U
#define SMP_TBX_F53_W4_ENCRYPTION_COMPLETE       0x0AU
#define SMP_TBX_F6_W4_ENCRYPTION_COMPLETE        0x0BU
#define SMP_TBX_G2_W4_ENCRYPTION_COMPLETE        0x0CU
#define SMP_TBX_H6_W4_ENCRYPTION_COMPLETE        0x0DU
#endif /* SMP_LESC */

#ifdef SMP_LESC
#define SMP_TBX_F5_T_KEY_GEN_MODE                0x01U
#define SMP_TBX_F5_LTK_GEN_MODE                  0x02U
#define SMP_TBX_F5_MACKEY_GEN_MODE               0x03U
#endif /* SMP_LESC */

/* SMP Toolbox Command defines */
#define SMP_TBX_ENCRYPT_CMD                      0x81U
#define SMP_TBX_RAND_CMD                         0x82U
#define SMP_TBX_READ_L_P256_PUB_KEY_CMD          0x83U
#define SMP_TBX_GEN_DHKEY_CMD                    0x84U

/* SMP Toolbox Command states */
#define SMP_TBX_COMMAND_IN_USE                   0x01U
#define SMP_TBX_COMMAND_IS_FREE                  0x00U

/* SMP Passkey Types */
#define SMP_PASSKEY_USER                         0x00U
#define SMP_PASSKEY_GEN                          0x01U

#ifdef SMP_LESC
/* SMP Numeric Key Comparison Types */
#define SMP_NUM_COMP_CNF                         0x00U
#define SMP_NUM_COMP_GEN                         0x01U
#endif /* SMP_LESC */

/* Number of key exchange commands */
#define SMP_NUM_KEY_INFO_EXCHANGE                5U

#if (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT)
/** SMP Unavailability of OOB Data */
#define SMP_LESC_OOB_DATA_UNAVAILABLE               0x00U

/** SMP LESC Availability of OOB Data */
#define SMP_LESC_OOB_DATA_AVAILABLE                 0x01U

#define SMP_LESC_L_OOB_DATA_SET                     0x01U

#define SMP_LESC_R_OOB_DATA_SET                     0x01U
#endif /* (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT) */

#ifdef SMP_LESC
/* SMP Tool box Related LESC Constant Defines */
/* #define SMP_CODE_LESC_PAIRING_STK_GEN_KEYID         "btle" */
#define SMP_CODE_LESC_PAIRING_STK_GEN_KEYID         {0x65, 0x6C, 0x74, 0x62}
#define SMP_CODE_LESC_TBX_KEY_GEN_SALT              {0xBE, 0x83, 0x60, 0x5A, \
                                                     0xDB, 0x0B, 0x37, 0x60, \
                                                     0x38, 0xA5, 0xF5, 0xAA, \
                                                     0x91, 0x83, 0x88, 0x6C}
#define SMP_CODE_LESC_TBX_F5_KEY_LENGTH_PARAM        0x0100U
#endif /* SMP_LESC */

/* ----------------------------------------- Macros */
/** To Lock SMP global mutex */
#define smp_lock()          BT_MUTEX_LOCK(smp_mutex, SMP)

/** To Unlock SMP global mutex */
#define smp_unlock()        BT_MUTEX_UNLOCK(smp_mutex, SMP)

/** To Lock SMP global mutex */
#define smp_lock_void()     BT_MUTEX_LOCK_VOID(smp_mutex, SMP)

/** To Unlock SMP global mutex */
#define smp_unlock_void()   BT_MUTEX_UNLOCK_VOID(smp_mutex, SMP)

#define SMP_ACCESS_CONTEXT_STATE(i)    (smp_context[(i)].state)

#define SMP_SET_STATE(i,s)\
        (SMP_ACCESS_CONTEXT_STATE((i)) =  (s))

#define SMP_AUTHREQ_IS_BONDING(auth_req) \
    ((SMP_BONDING == ((auth_req) & SMP_AUTHREQ_BOND_BITMASK))? \
    (SMP_TRUE): (SMP_FALSE))

#define SMP_AUTHREQ_IS_MITM(auth_req) \
    (((auth_req) & SMP_AUTHREQ_MITM_BITMASK)? (SMP_TRUE): (SMP_FALSE))

#ifdef SMP_LESC
#define SMP_AUTHREQ_IS_LESC(auth_req) \
    (((auth_req) & SMP_AUTHREQ_LESC_BITMASK)? (SMP_TRUE): (SMP_FALSE))

#define SMP_AUTHREQ_IS_KEYPRESS(auth_req) \
    (((auth_req) & SMP_AUTHREQ_KEY_PRESS_BITMASK)? (SMP_TRUE): (SMP_FALSE))

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_AUTHREQ_IS_CT2(auth_req) \
    (((auth_req) & SMP_AUTHREQ_CT2_BITMASK)? (SMP_TRUE): (SMP_FALSE))
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#define SMP_AUTHREQ_SET_BONDING(auth_req, bond) \
    ((auth_req) = (SMP_TRUE == (bond))? \
    ((auth_req) | (SMP_AUTHREQ_BOND_BITMASK & SMP_BONDING)): \
    ((auth_req) & (UCHAR)(~(SMP_AUTHREQ_BOND_BITMASK & SMP_BONDING))))

#define SMP_AUTHREQ_SET_MITM(auth_req, sec) \
    ((auth_req) = (SMP_TRUE == (sec))? \
    ((auth_req) | SMP_AUTHREQ_MITM_BITMASK): ((auth_req) & (UCHAR)(~SMP_AUTHREQ_MITM_BITMASK)))

#ifdef SMP_LESC
#define SMP_AUTHREQ_SET_LESC(auth_req, lesc) \
    ((auth_req) = (SMP_TRUE == (lesc))? \
    ((auth_req) | SMP_AUTHREQ_LESC_BITMASK): ((auth_req) & (UCHAR)(~SMP_AUTHREQ_LESC_BITMASK)))

#define SMP_AUTHREQ_SET_CT2(auth_req, ct2) \
    ((auth_req) = (SMP_TRUE == (ct2))? \
    ((auth_req) | SMP_AUTHREQ_CT2_BITMASK): ((auth_req) & (UCHAR)(~SMP_AUTHREQ_CT2_BITMASK)))
#endif /* SMP_LESC */

#ifdef SMP_AUTH_REQ_RFU_TESTING
#define SMP_SET_AUTHREQ_RFU_FILED(auth_req) \
    (auth_req)  = (auth_req) | ( ~(SMP_AUTHREQ_KEY_PRESS_BITMASK | SMP_AUTHREQ_LESC_BITMASK | SMP_AUTHREQ_MITM_BITMASK | SMP_AUTHREQ_BOND_BITMASK))
#endif /* SMP_AUTH_REQ_RFU_TESTING */

#define SMP_SET_ENTITY_CONNECTION(var, s) \
    (var) = (((var) & SMP_ENTITY_CONNECTION_MASK) | (s))

#define SMP_GET_ENTITY_CONNECTION(var) \
    ((var) & (UCHAR)(~SMP_ENTITY_CONNECTION_MASK))

#define SMP_SET_ENTITY_VALIDITY(var, s) \
    (var) = (((var) & SMP_ENTITY_VALIDITY_MASK) | (s))

#define SMP_GET_ENTITY_VALIDITY(var) \
    ((var) & (UCHAR)(~SMP_ENTITY_VALIDITY_MASK))

#define SMP_SET_ENTITY_ROLE(var, s) \
    (var) = (((var) & SMP_ENTITY_ROLE_MASK) | (s))

#define SMP_IS_MASTER(i) \
    (((smp_devices[(i)].state & ((UCHAR)(~SMP_ENTITY_ROLE_MASK))) == \
    (SMP_ENTITY_MASTER))? SMP_TRUE: SMP_FALSE)

#define SMP_IS_SLAVE(i) \
    (((smp_devices[(i)].state & ((UCHAR)(~SMP_ENTITY_ROLE_MASK))) == \
    (SMP_ENTITY_SLAVE))? SMP_TRUE: SMP_FALSE)

#define SMP_SET_LINK_ACTIVE(var, link) \
    (var) = ((var) | (link))

#define SMP_RESET_LINK_ACTIVE(var, link) \
    (var) = ((var) & ((UCHAR)~(link)))

#define SMP_GET_ENTITY_CONNECTION(var) \
    ((var) & (UCHAR)(~SMP_ENTITY_CONNECTION_MASK))

#define SMP_SET_CONTEXT_STATE(i, s) \
    smp_context[(i)].state = (s)

#define SMP_SET_ENCRYPTION_ON(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_ENC_MASK) | SMP_ENTITY_ENC_ON)

#define SMP_SET_ENCRYPTION_IN_PROGRESS(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_ENC_MASK) | SMP_ENTITY_ENC_IN_PROGRESS)

#define SMP_SET_ENCRYPTION_OFF(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_ENC_MASK) | SMP_ENTITY_ENC_OFF)

#define SMP_GET_ENCRYPTION_STATE(dev_attr) \
    ((dev_attr) & (UCHAR)(~SMP_ENTITY_ENC_MASK))

#define SMP_SET_AUTHENTICATION_ON(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_AUTH_MASK) | SMP_ENTITY_AUTH_ON)

#define SMP_SET_AUTHENTICATION_IN_PROGRESS(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_AUTH_MASK) | SMP_ENTITY_AUTH_IN_PROGRESS)

#define SMP_SET_AUTHENTICATION_OFF(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_AUTH_MASK) | SMP_ENTITY_AUTH_OFF)

#define SMP_GET_AUTHENTICATION_STATE(dev_attr) \
    ((dev_attr) & (UCHAR)(~SMP_ENTITY_AUTH_MASK))

#define SMP_SET_BONDING_ON(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_BOND_MASK) | SMP_ENTITY_BOND_ON)

#define SMP_SET_BONDING_OFF(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_BOND_MASK) | SMP_ENTITY_BOND_OFF)

#define SMP_GET_BONDING_STATE(dev_attr) \
    ((dev_attr) & (UCHAR)(~SMP_ENTITY_BOND_MASK))

#ifdef SMP_LESC
#define SMP_SET_LESC_PAIRING_ON(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_LESC_PAIRING_MASK) | SMP_ENTITY_LESC_PAIRING_ON)

#define SMP_SET_LESC_PAIRING_OFF(dev_attr) \
    (dev_attr) = (((dev_attr) & SMP_ENTITY_LESC_PAIRING_MASK) | SMP_ENTITY_LESC_PAIRING_OFF)

#define SMP_GET_LESC_PAIRING_STATE(dev_attr) \
    ((dev_attr) & (UCHAR)(~SMP_ENTITY_LESC_PAIRING_MASK))
#endif /* SMP_LESC */

#define SMP_SET_KEY_ON(keyvar, key) \
    ((keyvar) |= (key))

#define SMP_SET_KEY_OFF(keyvar, key) \
    ((keyvar) &= (UCHAR)(~(key)))

#define SMP_IS_KEY_ON(keyvar, key) \
    (((keyvar) & (key))? (SMP_TRUE): (SMP_FALSE))

#define SMP_IS_LOCAL_KEY_XCHG_ON(keyvar) \
    (((keyvar) & (UCHAR)(~SMP_LOCAL_KEY_MASK))? (SMP_TRUE): (SMP_FALSE))

#define SMP_IS_REMOTE_KEY_XCHG_ON(keyvar) \
    (((keyvar) & (UCHAR)(~SMP_REMOTE_KEY_MASK))? (SMP_TRUE): (SMP_FALSE))

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define SMP_IS_LOCAL_LINK_KEY_ON(keyvar) \
    SMP_IS_KEY_ON((keyvar), SMP_LOCAL_LINK_KEY)

#define SMP_IS_REMOTE_LINK_KEY_ON(keyvar) \
    SMP_IS_KEY_ON((keyvar), SMP_REMOTE_LINK_KEY)
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

#define SMP_MIN(x, y) (((x) < (y))? ((x)): ((y)))

#define SMP_COMPARE_ID_ADDR(id_addr_a,id_addr_b)\
        ((0 == BT_mem_cmp((id_addr_a), (id_addr_b), SMP_IDADDR_SIZE)) ? SMP_TRUE : SMP_FALSE)

#define SMP_PVTADDR_STATE_SET(s) \
        smp_pvt_addr.state = (s)

#define SMP_PVTADDR_STATE_GET() \
        smp_pvt_addr.state

/* ----------------------------------------- Structures/Data Types */

/** SMP Event Code MAP structure */
typedef struct _EVENT_MAP
{
    /** Request/Reply opcode */
    UCHAR code_param;

    /** Event type */
    UCHAR event_type;

} EVENT_MAP;

typedef struct _SMP_PVT_ADDR
{
    /** Procedure state */
    UCHAR state;

    /** IRK reference */
    UCHAR * irk;

    /** Verify Address reference */
    UCHAR * verify_addr;

    /** Create Address memory */
    UCHAR create_addr[BT_BD_ADDR_SIZE];

} SMP_PVT_ADDR;

/** Security Manager Protocol device persistent information */
typedef struct _SMP_DEVICE_DB
{
#ifdef SMP_SAVE_REMOTE_IOCAP
    /** Remote IO Capabilites */
    SMP_IOCAPS iocaps;
#endif /* SMP_SAVE_REMOTE_IOCAP */

    /** Device platform attribute */
    SMP_DEVICE_ATTR_PL     device_attr_pl;

    /**
     * Local and Remote Keys exchanged during pairing
     *      Bit 0: Local EncKey
     *      Bit 1: Local IdKey
     *      Bit 2: Local Sign
     *      Bit 3: Reserved
     *      Bit 4: Remote EncKey
     *      Bit 5: Remote IdKey
     *      Bit 6: Remote Sign
     *      Bit 7: Reserved
     */
    UCHAR keys;

    /** Authentication Negotiated */
    UCHAR    auth_req;

    /** Encryption key size */
    UCHAR    ekey_size;

    /** Device handle for the remote device connection */
    SMP_BD_HANDLE   bd_handle;

    /** Remote Device shared key disribution info */
    SMP_KEY_DIST r_key;

} SMP_DEVICE_DB;

/** Security Manager Protocol device database */
typedef struct
{
    /**
     * State of the device entity
     *
     * Bit 1,0: Validity
     *          00 - Invalid
     *          01 - Valid and Trusted
     *          10 - Valid and Untrusted
     *          11 - Reserved
     *
     * Bit 3,2: Device Entity state
     *          00 - Idle
     *          01 - Connected
     *          10 - Reserved
     *          11 - Reserved
     *
     * Bit 4  : Role (SMP_MASTER/SMP_SLAVE)
     *
     * Bit 5  : Reserved
     * Bit 6  : Reserved
     * Bit 7  : Reserved
     */
    UCHAR state;

    /**
     * Device attributes of this device entry
     *      Bit 1,0: Authentication Status
     *               00 - Unauthenticated
     *               01 - Authentication in Progress
     *               10 - Authenticated
     *               11 - Reserved
     *
     *      Bit 3,2: Encryption Status
     *               00 - Unencrypted
     *               01 - Encryption in Progress
     *               10 - Encrypted
     *               11 - Reserved
     *
     *      Bit 4  : Bonding Status
     *      Bit 5  : Reserved
     *      Bit 6  : Reserved
     *      Bit 7  : Reserved
     */
    UCHAR     dev_attr;

    /** HCI Connection Handle */
    UINT16    acl_handle;

    /** L2CAP Local Signalling Identifier */
    UINT32    l2cap_sid;

    /** SMP Device Database */
    SMP_DEVICE_DB    db;

    /** Remote Key Info pointer */
    UCHAR * key_info[SMP_NUM_KEY_INFO_EXCHANGE];

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
    /** Unmasked LESC LTK for CTKD access */
    UCHAR lesc_ltk[SMP_LTK_SIZE];
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

} SMP_DEVICE_ENTITY;

/** Security Manager Protocol Context */
typedef struct
{
    /** Device index for Security Manager Database */
    UCHAR     device_id;

    /** Negotiated Authentication Requirements */
    UCHAR     auth_req;

    /** State of the security context */
    UINT32    state;

    /** Max Encryption Key Size */
    UCHAR     ekey_size;

    /** Key distribution flag */
    UCHAR     keys;

    /** Pairing request packet buffer */
    UCHAR     preq[SMP_PAIRING_REQUEST_CMD_SIZE];

    /** Pairing request packet buffer */
    UCHAR     pres[SMP_PAIRING_RESPONSE_CMD_SIZE];

    /** Remote confirm value */
    UCHAR     confirm[SMP_CONFIRM_VALUE_SIZE];

    /** Local Random value used to generate confirm */
    UCHAR     l_rand[SMP_RAND_128B_SIZE];

    /** Remote Random value used to generate confirm */
    UCHAR     r_rand[SMP_RAND_128B_SIZE];

    /** Temp key */
    UCHAR     temp_key[SMP_TEMP_KEY_SIZE];

    /** BT Timer handle */
    BT_timer_handle  timer_handle;

    /** Toolbox context */
    struct
    {
        /* State */
        UCHAR state;

#if (SMP_MAX_CONTEXTS > 1)
        /* Encrypt Index */
        UCHAR enc_index;
#endif /* (SMP_MAX_CONTEXTS > 1) */

        /* C1 function temp data */
        UCHAR c1_p2[SMP_TEMP_KEY_SIZE];

#ifdef SMP_LESC
        /* F5 Function Temp Key 'T' */
        UCHAR f5_T[SMP_TEMP_KEY_SIZE];
#endif /* SMP_LESC */

    } tbx;

#ifdef SMP_LESC
    /** Remote Public Key */
    UCHAR r_pubkey[SMP_LESC_PUBLIC_KEY_SIZE];

    /** local Public Key */
    UCHAR l_pubkey[SMP_LESC_PUBLIC_KEY_SIZE];

    /** Local DHKey */
    UCHAR dhkey[SMP_LESC_DHKEY_SIZE];

    /** Remote DHKey Check */
    UCHAR r_dhkey_check[SMP_LESC_DHKEY_CHECK_VALUE_SIZE];

    /** Local LESC LTK */
    UCHAR l_lesc_ltk[SMP_LTK_SIZE];

    /** Local LESC MACKEY */
    UCHAR l_lesc_mackey[SMP_LESC_MACKEY_SIZE];

    /** Passkey context */
    struct
    {
        /* passkey value */
        UINT32 pkey_val;

        /* passkey iteration */
        UCHAR pkey_iter;
    } pkey_lesc;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
    /* Current link type for the context */
    UINT8 transport;

    /* Identify cross transport key generation */
    UCHAR xtx_keygen;
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

} SMP_CONTEXT;

/** Global toolbox context for non-device context operations */
typedef struct _SMP_TBX_CONTEXT
{
    /* State */
    UCHAR state;

#if (SMP_MAX_CONTEXTS > 1)
    /* Encrypt Index */
    UCHAR enc_index;
#endif /* (SMP_MAX_CONTEXTS > 1) */

} SMP_TBX_CONTEXT;

/** Global toolbox context for non-device context operations */
typedef struct _SMP_TBX_CMD_OUTPUT
{
    /* SMP toolbox CMD Code */
    UCHAR cmd_code;

    /* Status */
    UCHAR status;

    /* Pointer to hold the Operation/CMD Output */
    UCHAR * cmd_out;

    /* Output CMD length */
    UINT16 cmd_out_len;

    /* State holder */
    UCHAR state;

    /* Command Counter */
    UCHAR cmd_counter;

} SMP_TBX_CMD_OUTPUT;

#if ((defined SMP_DATA_SIGNING) || (defined SMP_LESC))
/** Global Conext for AES CMAC invoked operation */
typedef UCHAR SMP_AES_CMAC_OP;

/** Global context for AES CMAC related operations */
typedef struct _SMP_AES_CONTEXT
{
    AES_CMAC_CONTEXT cmac_context;

    SMP_AES_CMAC_OP  cmac_op;

    UCHAR            cmac_data_to_free;

    UCHAR            cmac_mac_to_free;
} SMP_AES_CONTEXT;

#endif /* ((defined SMP_DATA_SIGNING) || (defined SMP_LESC)) */

/* SMP AES CMAC utility related callback function pointer type */
typedef void (* SMP_AES_CMAC_UTILITY_CB)(void) DECL_REENTRANT;

/* ----------------------------------------- Internal Functions */

/* ----------------------------------------- API Declarations */
void smp_get_auth_info_from_req (UCHAR auth_req, SMP_AUTH_INFO * auth_info);
void smp_get_auth_req_from_info (SMP_AUTH_INFO * auth_info, UCHAR * auth_req);
API_RESULT smp_validate_security (SMP_AUTH_INFO * curr, SMP_AUTH_INFO * reqd);
API_RESULT smp_handle_security_request (UCHAR di, SMP_AUTH_INFO * auth);
API_RESULT smp_validate_pairing_params (UCHAR * param, UCHAR direction);
API_RESULT smp_validate_keyinfo_len (UCHAR opcode, UINT16 len);
void smp_pvt_addr_rand_complete (UCHAR * data_param, UINT16 datalen);

UCHAR smp_get_fsm_event (UCHAR code_param);

UCHAR smp_search_device
      (
          SMP_BD_HANDLE * bd_handle,
          UINT32 sid
      );

UCHAR smp_create_device_entity
      (
          /* IN */ SMP_BD_HANDLE * bd_handle
      );

void smp_reset_device_entity
     (
         /* IN */ UCHAR    di,
         /* IN */ UCHAR    del
     );

UCHAR smp_search_and_add_device
      (
          SMP_BD_HANDLE * bd_handle
      );

UCHAR smp_get_context(UCHAR di, UCHAR alloc);
void smp_free_context(UCHAR ci);

#define smp_search_context(di)      \
    smp_get_context((di), SMP_FALSE)

#define smp_alloc_context(di)      \
    smp_get_context((di), SMP_TRUE)

UCHAR smp_set_device_validity
      (
          /* IN */ UCHAR      di
      );

UCHAR smp_get_iocap_assoc_model(UCHAR auth_req, UCHAR i_iocap, UCHAR r_iocap);

void smp_timer_start (UCHAR ci);
void smp_timer_stop (UCHAR ci);
void smp_procedure_timeout_cb (void *data_param, UINT16 data_len);

#define smp_hci_start_encryption BT_hci_le_start_encryption

#define smp_hci_ltk_requested_reply BT_hci_le_long_term_key_requested_reply

#define smp_hci_ltk_requested_negative_reply \
            BT_hci_le_long_term_key_requested_negative_reply

/* Toolbox Functions */
API_RESULT smp_tbx_ah_eval (UCHAR * k, UCHAR * r);
API_RESULT smp_tbx_c1_eval
           (
               SMP_CONTEXT * context,
               SMP_DEVICE_ENTITY * device,
               UCHAR confirm
           );
API_RESULT smp_tbx_s1_eval (SMP_CONTEXT * context, UCHAR * r1, UCHAR * r2);

void smp_ah_eval_complete (UCHAR * data_param, UINT16 datalen);

#ifdef SMP_LESC
/* SMP LESC Tool Box Functions */
API_RESULT smp_tbx_f4_eval
           (
               SMP_CONTEXT       * context,
               SMP_DEVICE_ENTITY * device,
               UCHAR             conf_check_flag
           );
API_RESULT smp_tbx_f5_eval
           (
               SMP_CONTEXT       * context,
               SMP_DEVICE_ENTITY * device,
               UCHAR             gen_mode
           );
API_RESULT smp_tbx_f6_eval
           (
               SMP_CONTEXT       * context,
               SMP_DEVICE_ENTITY * device,
               UCHAR             dhkey_check_flag
           );
API_RESULT smp_tbx_g2_eval (SMP_CONTEXT * context);
API_RESULT smp_tbx_read_p256_pub_key (SMP_CONTEXT * context);
API_RESULT smp_tbx_dhkey_generate (SMP_CONTEXT * context);
API_RESULT smp_is_valid_dh_key(UCHAR *key);
API_RESULT smp_validate_pubkeys(UCHAR * local, UCHAR * remote);
#endif /* SMP_LESC */

#ifdef SMP_HAVE_TBX_PL_ENC
API_RESULT smp_tbx_128B_encrypt(UCHAR * key, UCHAR * data, UCHAR * encout);
#else /* SMP_HAVE_TBX_PL_ENC */
#define smp_tbx_128B_encrypt(key, data, count)       BT_hci_le_encrypt((key), (data), (count))
#endif /* SMP_HAVE_TBX_PL_ENC */

#ifdef SMP_HAVE_TBX_PL_RAND
API_RESULT smp_tbx_generate_rand (UCHAR * n_rand, UINT16 size);
#else /* SMP_HAVE_TBX_PL_RAND */
#define smp_tbx_generate_rand()     BT_hci_le_rand()
#endif /* SMP_HAVE_TBX_PL_RAND */

#ifdef SMP_HAVE_TBX_PL_ECDH
API_RESULT smp_tbx_gen_dhkey (UCHAR * r_pub_key);
API_RESULT smp_tbx_read_l_p256_pub_key (void);
#else /* SMP_HAVE_TBX_PL_ECDH */
#define smp_tbx_read_l_p256_pub_key() BT_hci_le_read_local_p256_public_key()
#define smp_tbx_gen_dhkey(r_pub_key)           BT_hci_le_generate_dhkey(r_pub_key)
#endif /* SMP_HAVE_TBX_PL_ECDH */

#ifdef SMP_LESC
#ifdef SMP_HAVE_TBX_PL_LESC_FUNC

API_RESULT smp_tbx_128B_aes_cmac
           (
               UCHAR  * key,
               UCHAR  * plain_text,
               UINT16 length,
               UCHAR  * enc_msg
           );

API_RESULT smp_tbx_f4_eval
           (
               /* IN */  SMP_CONTEXT * context,
               /* IN */  SMP_DEVICE_ENTITY * device,
               /* IN */  UCHAR confirm,
               /* OUT */ UCHAR * output
           );

API_RESULT smp_tbx_f5_eval
           (
               /* IN */  SMP_CONTEXT * context,
               /* IN */  SMP_DEVICE_ENTITY * device,
               /* IN */  UCHAR confirm,
               /* OUT */ UCHAR * output
           );

API_RESULT smp_tbx_f6_eval
           (
               /* IN */  SMP_CONTEXT * context,
               /* IN */  SMP_DEVICE_ENTITY * device,
               /* IN */  UCHAR confirm,
               /* OUT */ UCHAR * output
           );

API_RESULT smp_tbx_g2_eval
           (
               /* IN */  SMP_CONTEXT * context,
               /* IN */  SMP_DEVICE_ENTITY * device,
               /* IN */  UCHAR confirm,
               /* OUT */ UCHAR * output
           );

API_RESULT smp_tbx_h6_eval
           (
               /* IN */  SMP_CONTEXT * context,
               /* IN */  SMP_DEVICE_ENTITY * device,
               /* IN */  UCHAR confirm,
               /* OUT */ UCHAR * output
           );

#endif /* SMP_HAVE_TBX_PL_LESC_FUNC */
#endif /* SMP_LESC */

void smp_tbx_encryption_complete
     (
         UCHAR index,
         UCHAR status,
         UCHAR * data_param,
         UINT16 datalen
     );
void smp_tbx_rand_complete (UCHAR status, UCHAR * data_param, UINT16 datalen);

#ifdef SMP_LESC
void smp_tbx_p256_pub_key_read_complete
     (
         UCHAR status,
         UCHAR * data_param,
         UINT16 datalen
     );

void smp_tbx_generate_dhkey_complete
     (
         UCHAR status,
         UCHAR * data_param,
         UINT16 datalen
     );
API_RESULT  smp_tbx_128_aes_cmac
            (
                /* IN */  UCHAR                      action,
                /* IN */  UCHAR                      op,
                /* IN */  UCHAR                    * buffer,
                /* IN */  UINT16                     size,
                /* IN */  UCHAR                    * key,
                /* IN */  UINT16                     mac_len,
                /* OUT */ UCHAR                    * mac,
                /* IN */  SMP_AES_CMAC_UTILITY_CB    smp_aes_cmac_cb,
                /* IN */  UCHAR                      text_free,
                /* IN */  UCHAR                      mac_free
            );

void smp_tbx_aes_cmac_proc_complete ( UCHAR state, UCHAR event_type);

/* SMP TBX AES CMAC operation specific Handlings */
#define SMP_TBX_HNDL_SIGN_AES_CMAC()
#define SMP_LESC_TBX_HNDL_CONF_VAL_GEN_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_F4_W4_ENCRYPTION_COMPLETE,ev_smp_pairing_confirm);
#define SMP_LESC_TBX_HNDL_T_KEY_GEN_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_F51_W4_ENCRYPTION_COMPLETE,ev_smp_lesc_key_gen);
#define SMP_LESC_TBX_HNDL_LTK_GEN_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_F52_W4_ENCRYPTION_COMPLETE,ev_smp_lesc_key_gen);
#define SMP_LESC_TBX_HNDL_MACKEY_GEN_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_F53_W4_ENCRYPTION_COMPLETE,ev_smp_lesc_key_gen);
#define SMP_LESC_TBX_HNDL_CHK_VAL_GEN_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_F6_W4_ENCRYPTION_COMPLETE,ev_smp_dhkey_check);
#define SMP_LESC_TBX_HNDL_NUM_COMP_VAL_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_G2_W4_ENCRYPTION_COMPLETE,ev_smp_nkey_gen);
#define SMP_LESC_TBX_HNDL_LINK_KEY_AES_CMAC() \
        smp_tbx_aes_cmac_proc_complete(SMP_TBX_H6_W4_ENCRYPTION_COMPLETE,ev_smp_unknown);

#endif /* SMP_LESC */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
/* TODO: Change it accrding to the changes in L2CAP */
#define smp_l2cap_write(di, link, data, len) \
            l2cap_fixed_channel_data_write \
            (&smp_devices[(di)].db.bd_handle, \
            (SMP_LINK_BREDR != (link))?L2CAP_SMP_CID:L2CAP_SMP_BREDR_CID, \
            (data), (len));

#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
/* TODO: Change it accrding to the changes in L2CAP */
#define smp_l2cap_write(di, data, len) \
            l2cap_fixed_channel_data_write \
            (&smp_devices[(di)].db.bd_handle, \
            L2CAP_SMP_CID, (data), (len));

#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/* XOR thwo 16-byte streams */
void smp_16byte_xor(UCHAR * input, UCHAR * inout);

void smp_l2cap_event_cb
     (
         SMP_BD_HANDLE     * bd_handle,
         UCHAR               event_type,
         void                * data_param,
         UINT16              datalen
     );

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
void smp_bredr_l2cap_event_cb
     (
         SMP_BD_HANDLE     * bd_handle,
         UCHAR               event_type,
         void                * data_param,
         UINT16              datalen
     );
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

API_RESULT smp_send_pairing_reqres
           (
               UCHAR code_param,
               SMP_CONTEXT * context,
               SMP_AUTH_INFO * auth
           );
API_RESULT smp_send_pairing_failed (UCHAR di, UINT16 reason);
API_RESULT smp_send_command
           (
               UCHAR di,
               UCHAR code_param,
               UCHAR * param,
               UINT16 param_len
           );
void smp_authentication_complete
     (
         UCHAR di,
         UINT16 status
     );

#ifndef SMP_LESC_CROSS_TXP_KEY_GEN
void smp_clear_connection_entity (UCHAR di);
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
void smp_clear_connection_entity(UCHAR di, UCHAR transport);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

API_RESULT smp_send_event
           (
               UCHAR di,
               UCHAR event_type,
               API_RESULT event_result,
               void * data_param,
               UINT16 datalen
           );

#ifdef SMP_STORAGE
void smp_storage_write(void);
void smp_storage_read(void);
void smp_pstorage_cb (UCHAR id, UCHAR type, UCHAR action);
#endif /* SMP_STORAGE */

/* API_RESULT smp_handle_key_exchange (UCHAR ci, UCHAR di); */
API_RESULT smp_passkey_handler (UCHAR type, SMP_EVENT_INFO * param);
API_RESULT smp_remote_pairing_confirm_handler (SMP_EVENT_INFO    * param);

API_RESULT smp_invoke_rand_gen (UCHAR ci, UCHAR di, SMP_CONTEXT * context);
#ifdef SMP_LESC
API_RESULT smp_invoke_public_key_read (UCHAR ci, UCHAR di, SMP_CONTEXT * context);
API_RESULT smp_invoke_dhkey_gen (UCHAR ci, UCHAR di, SMP_CONTEXT * context);
API_RESULT smp_remote_dhkey_check_handler (SMP_EVENT_INFO    * param);
API_RESULT smp_num_comp_handler (UCHAR type, SMP_EVENT_INFO * param);
API_RESULT smp_invoke_lesc_enc_handler
           (
                UCHAR ci,
                UCHAR di,
                SMP_CONTEXT * context,
                SMP_DEVICE_ENTITY * device
            );
#endif /* SMP_LESC */

#ifdef SMP_HAVE_TBX_CMD_WT_BH
API_RESULT smp_enqueue_cmd_op_to_wt
           (
               UCHAR   * enc_out,
               UCHAR   status,
               UINT16  len,
               UCHAR   opcode,
               UCHAR   cmd_counter
           );
#endif /* SMP_HAVE_TBX_CMD_WT_BH */

#if ((defined SMP_DATA_SIGNING) || (defined SMP_LESC))
void smp_clear_aes_cmac_context(UCHAR reset);
void smp_reset_aes_cmac_context(void);
void smp_tbx_aes_cmac_128_encryption_complete (UCHAR index, UCHAR status, UCHAR * data, UINT16 datalen);
#endif /* ((defined SMP_DATA_SIGNING) || (defined SMP_LESC)) */

#endif /* _H_SMP_INTERNAL_ */
