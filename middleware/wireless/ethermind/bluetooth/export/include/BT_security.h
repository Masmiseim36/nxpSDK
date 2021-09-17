
/**
 *  \file BT_security.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_SECURITY_
#define _H_BT_SECURITY_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"
#include "BT_sm_api.h"

#ifdef BT_LE
#include "BT_smp_api.h"
#endif /* BT_LE */
/**
 * \defgroup security_module Security
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  security module to the Application and other upper layers of the stack.
 *
 */
/**
 * \defgroup security_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup security_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* --------------------------------------------- Global Definitions */
/* Device link types mask */
#define BT_LINK_CLASSIC                     0x00U
#define BT_LINK_LE                          0x01U

/* Status Bitmasks for security informations */
#define BT_SEC_BONDED                       0x02U
#define BT_SEC_AUTHENTICATED                0x04U
#define BT_SEC_ENCRYPTED                    0x08U

/* Security Mode BitMasks */
#define BT_SEC_SM_MASK                      0xF0U
/* Security Mode 1 - No Security Requirement for device link */
#define BT_SEC_SM_1                         0x10U
/* Security Mode 2 - Security Required for device link */
#define BT_SEC_SM_2                         0x20U

/* Security Level BitMasks */
#define BT_SEC_SL_MASK                      0x0FU
/* Security Level 1 - No Security Requirement */
#define BT_SEC_SL_0                         0x00U
/* Security Level 2 - Unauthenticated Security Requirement */
#define BT_SEC_SL_1                         0x01U
/* Security Level 3 - Authenticated Security Requirement */
#define BT_SEC_SL_2                         0x02U
/* Security Level 4 - Secure Connection Requirement */
#define BT_SEC_SL_3                         0x04U
/** \} */

#ifdef BT_SECURITY_VU_VALIDATION
/* Security Vulnerability IDs */
#define BT_SECURITY_VU_KNOB_ATTACK              1U
#define BT_SECURITY_VU_ASBU_ATTACK              2U
#define BT_SECURITY_VU_INVALID_PUBKEY           3U
#define BT_SECURITY_VU_BLURTOOTH                4U
#define BT_SECURITY_VU_ANSSI_REFLECT_CONFVAL    5U
#define BT_SECURITY_VU_ANSSI_REFLECT_PUBKEY     6U
#define BT_SECURITY_VU_UNEXPECTED_ENC_START     7U
#define BT_SECURITY_VU_UNEXPECTED_PUBKEY        8U
#define BT_SECURITY_VU_KEYSIZE_OVERFLOW         9U
#define BT_SECURITY_VU_ZERO_LTK_INSTALL        10U
#define BT_SECURITY_VU_DHCHECK_SKIP            11U
#define BT_SECURITY_VU_NONZERO_EDIV_RAND       12U
#define BT_SECURITY_VU_TRUNCATED_L2CAP         13U
#define BT_SECURITY_VU_SILENT_LEN_OVERFLOW     14U
#define BT_SECURITY_VU_INVALID_L2CAP_FRAGMENT  15U
#define BT_SECURITY_VU_SEQUENTIAL_ATT_DEADLOCK 16U

#endif /* BT_SECURITY_VU_VALIDATION */

/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup security_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/** Security Information Structure */
typedef struct _BT_SEC_INFO
{
    /**
     * Security status bitmask
     *
     * BIT 0 - Link Type (0 - BR/EDR, 1 - LE)
     * BIT 1 - Bonded
     * BIT 2 - Authenticated
     * BIT 3 - Encrypted
     * BIT 4 - Reserved
     * BIT 5 - Reserved
     * BIT 6 - Reserved
     * BIT 7 - Reserved
     */
    UCHAR status;

    /** Security Mode/Level */
    UCHAR security;

    /** Encryption Key size */
    UCHAR eksize;

} BT_SEC_INFO;
/** \} */
/** \} */
/* --------------------------------------------- Macros */
/**
 * \defgroup security_utility_macros Utitlity Macros
 * \{
 * Describes utility macros for the module.
 */
/* Macros to set link and security properties */
#define BT_SEC_SET_LINKTYPE(x, lt)      (x) |= (lt)
#define BT_SEC_SET_BONDED(x)            (x) |= BT_SEC_BONDED
#define BT_SEC_SET_AUTHENTICATED(x)     (x) |= BT_SEC_AUTHENTICATED
#define BT_SEC_SET_ENCRYPTED(x)         (x) |= BT_SEC_ENCRYPTED

/* Macros to check link and security properties */
#define BT_SEC_IS_LINK_LE(x)            (((x) & BT_LINK_LE)? BT_TRUE: BT_FALSE)
#define BT_SEC_IS_BONDED(x)             (((x) & BT_SEC_BONDED)? BT_TRUE: BT_FALSE)
#define BT_SEC_IS_AUTHENTICATED(x)      (((x) & BT_SEC_AUTHENTICATED)? BT_TRUE: BT_FALSE)
#define BT_SEC_IS_ENCRYPTED(x)          (((x) & BT_SEC_ENCRYPTED)? BT_TRUE: BT_FALSE)

/* Macros to set security modes and levels */
#define BT_SEC_SET_SM(x, m)             (x) |= (m)
#define BT_SEC_SET_SL(x, l)             (x) |= (l)

/* Macros to get security modes and levels */
#define BT_SEC_GET_SM(x)                ((x) & BT_SEC_SM_MASK)
#define BT_SEC_GET_SL(x)                ((x) & BT_SEC_SL_MASK)
/** \} */
/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup security_api API definitions
 * \{
 * Describes API definitions for the module.
 */
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_security_get_device_status
           (
               /* IN */  DEVICE_HANDLE       * bd_handle,
               /* IN */  DEVICE_LINK_TYPE    link_type,
               /* OUT */ BT_SEC_INFO         * info
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
void BT_security_get_trusted_device_count
     (
         /* OUT */ UCHAR * ntrusted
     );

#ifdef BT_SECURITY_VU_VALIDATION
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param [in]
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
void BT_security_vu_set(UCHAR vu);

/**
 *  \brief
 *
 *  \par Description:
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
UCHAR BT_security_vu_get(void);
#endif /* BT_SECURITY_VU_VALIDATION */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
#endif /* _H_BT_SECURITY_ */

