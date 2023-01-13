
/**
 *  \file BT_status.h
 *
 *  This file lists all the macros and APIs that are exported
 *  to get/set the EtherMind Status Flag.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_STATUS_
#define _H_BT_STATUS_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup status_module STATUS (Status Module)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Status (STATIS) module to the Application and other upper
 *  layers of the stack.
 */

/**
 * \defgroup status_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup status_constants Constants
 * \{
 * Describes Constants defined by the module.
 */

/*
 *  EtherMind Status Flag
 *  ---------------------
 *      31 30 29 28           12 11 10 09 08 07 06 05 04 03 02 01 00
 *     _____________________________________________________________
 *     |  |  |  |  | ....... |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *     |__|__|__|__|_________|__|__|__|__|__|__|__|__|__|__|__|__|__|
 *
 *     <-----------------------><-----------><-------><------------->
 *        |                           |                     |
 *        |                           |                     |
 *        v                           v                     v
 *     Reserved for future use   Defined Stack         Defined Status Bits
 *                               Activity Bits
 *
 *  Status Bit Definitions
 *  ----------------------
 *       04 03 02 01 00
 *      _______________
 *      |  |  |  |  |  |
 *      |__|__|__|__|__|
 *        |  |  |  |  |
 *        |  |  |  |  |____ Memory Allocation Failed
 *        |  |  |  |
 *        |  |  |  |_______ HCI/L2CAP Data Buffer Full
 *        |  |  |
 *        |  |  |__________ HCI Command Queue Full
 *        |  |
 *        |  |_____________ Timer Entity Unavailable
 *        |
 *        |________________ Write/EtherMind Task Entity Unavailable
 *
 *  Stack Activity Definitions
 *  --------------------------
 *       11 10  9  8
 *      _____________
 *      |  |  |  |  |
 *      |__|__|__|__|
 *        |  |  |  |
 *        |  |  |  |_______ ATT Busy
 *        |  |  |
 *        |  |  |__________ SMP Busy
 *        |  |
 *        |  |_____________ L2CAP Busy
 *        |
 *        |________________ HCI Busy
 *
 */

/** Status Bit Definitions */
/**
 * @name  Status Bit Definitions
 *
 * Constant Definitions for Status Bit Definitions
 */
/*@{*/
#define STATUS_BIT_MEM_ALLOC_FAILED             0U
#define STATUS_BIT_DATA_BUFFER_FULL             1U
#define STATUS_BIT_HCI_COMMAND_Q_FULL           2U
#define STATUS_BIT_TIMER_ENTITY_FULL            3U
#define STATUS_BIT_WT_ENTITY_FULL               4U
#define STATUS_ATT_INVALID_PEER_RSP             5U
#define STATUS_ATT_INVALID_PEER_PDU             6U
#define STATUS_ATT_NO_FREE_INSTANCE             7U
#define STATUS_BIT_HCI_ISO_DATA_Q_FULL          8U
/*@}*/
/* Stack Activity Bit Definitions */
/**
 * @name  Stack Activity Bit Definitions
 *
 * Constant Definitions for Stack Activity Bit Definitions
 */
/*@{*/
#define STATUS_BIT_ATT_BUSY                     8U
#define STATUS_BIT_SMP_BUSY                     9U
#define STATUS_BIT_L2CAP_BUSY                   10U
#define STATUS_BIT_HCI_BUSY                     11U
/*@}*/
/** Count of maximum defined bit in Status Flag */
#define STATUS_NUM_DEFINED_BITS                 12U

/** Set/Reset Status Flag Bits */
/**
 * @name  Stack Activity Bit Definitions
 *
 * Constant Definitions for Stack Activity Bit Definitions
 */
/*@{*/
#define STATUS_BIT_SET                          0x01U
#define STATUS_BIT_RESET                        0x00U
/*@}*/

/** \} */
/** \} */

/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Function Declarations */
/**
 * \defgroup status_api_defs API Definitions
 * \{
 * This section describes the EtherMind Status APIs.
 */
#ifdef __cplusplus
extern "C"{
#endif

#ifdef BT_STATUS

/** Initialization of EtherMind Status Flag */
void em_status_init (void);
void status_bt_init (void);

#ifdef BT_HAVE_SHUTDOWN
void em_status_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \par Description
 *  This function returns the value of the EtherMind Status Flag.
 *  The EtherMind Status Flag is reset to zero after returning the
 *  current value.
 *
 *  \param [out] flag_val
 *         Caller allocated UINT32 variable, where the current value
 *         of the EtherMind Status Flag needs to be copied.
 *
 *  \return None
 */
void BT_status_get_flag
     (
         /* OUT */ UINT32   * flag_val
     );

/**
 *  \par Description
 *  This API returns the value of a bit of EtherMind Status Flag.
 *  The value of bit in EtherMind Status Flag is reset to zero
 *  after returning the current value.
 *
 *  \param [in] bit_no
 *         The position of the bit in the EtherMind Status Flag
 *  \param [out] bit_val
 *         Caller allocated UCHAR variable, where the current value
 *         of the specified bit of EtherMind Status Flag needs to be
 *         copied.
 *
 *  \return None
 */
void BT_status_get_bit
     (
         /* IN */  UCHAR     bit_no,
         /* OUT */ UCHAR   * bit_val
     );

/**
 *  \par Description
 *  This API sets/resets the value of a bit of EtherMind Status Flag.
 *  Upon a change in EtherMind Status Flag, this function calls the
 *  platform specific function to handle change in flag value.
 *
 *  \param [in] bit_no
 *         The position of the bit in the EtherMind Status Flag
 *  \param [in] op
 *         STATUS_BIT_SET or STATUS_BIT_RESET
 *
 *  \return None
 */
void BT_status_set_bit
     (
         /* IN */ UCHAR     bit_no,
         /* IN */ UCHAR     op
     );

/** Platform specific handling of change in Status Flag */
void BT_status_set_bit_pl
     (
         /* IN */ UINT32    flag
     );

#else  /* BT_STATUS */

#define em_status_init()
#define status_bt_init()

#define BT_status_get_flag(x)
#define BT_status_get_bit(x, y)
#define BT_status_set_bit(x, y)

#endif /* BT_STATUS */

/** Shutdown of EtherMind Status Flag */
#define status_bt_shutdown()

#ifdef __cplusplus
};
#endif

/** \} */

/** \} */
/** \} */

#endif /* _H_BT_STATUS_ */

