
/**
 *  \file BT_snoop.h
 *
 *  This file lists all the macros and APIs that are exported
 *  for logging HCI rx/tx data in btsnoop format.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_SNOOP_
#define _H_BT_SNOOP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup snoop_module SNOOP
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Snoop functionality module to the Application and other upper layers of the
 *  stack.
 */

/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Function Declarations */
/**
 * \defgroup snoop_api_defs API Definitions
 * \{
 * This section describes the EtherMind Snoop APIs.
 */
#ifdef __cplusplus
extern "C"{
#endif
#ifdef BT_SNOOP
/** Initialization/Shutdown of EtherMind btsnoop module */
/**
 *  \brief To do power on initialization of EtherMind btsnoop module
 *
 *  \par Description:
 *       This function is the EtherMind-Init handler for the btsnoop module
 *       and performs power-on initialization.
 *
 *  \note This function must be called only once.
 */
void em_snoop_init (void);

#ifdef BT_HAVE_SHUTDOWN
void em_snoop_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \brief To perform Bluetooth specific initializations for EtherMind btsnoop module
 *
 *  \par Description:
 *       This function is the Bluetooth-ON handler for btsnoop module, and it
 *       performs bluetooth specific initializations for the btsnoop module.
 */
void snoop_bt_init (void);

/**
 *  \brief To perform Bluetooth specific shutdown for EtherMind btsnoop module
 *
 *  \par Description:
 *       This function is the Bluetooth-OFF handler for btsnoop module, and it
 *       performs bluetooth specific shutdown for the btsnoop module.
 */
void snoop_bt_shutdown (void);

/**
 *  \brief To write btsnoop packet
 *
 *  \par Description
 *  This function writes btsnoop packet record.
 *
 *  \param [in] pkt_type
 *         HCI Packet Type
 *
 *  \param [in] is_rxed
 *         If the packet is received.
 *
 *  \param [in] pkt
 *         HCI packet
 *
 *  \param [in] pkt_len
 *         HCI packet length
 *
 *  \return None
 */
void BT_snoop_write_packet
     (
         /* IN */ UCHAR   pkt_type,
         /* IN */ UCHAR   is_rxed,
         /* IN */ UCHAR  *pkt,
         /* IN */ UINT16  pkt_len
     );

/**
 *  \brief To enable/disable btsnoop logging
 *
 *  \par Description
 *  This function allows run-time enable/disable of btsnoop logging.
 *
 *  \param [in] enable
 *         BT_TRUE to enable, BT_FALSE to disable
 *
 *  \return None
 */
void BT_snoop_logging
     (
         /* IN */ UCHAR   enable
     );
#else  /* BT_SNOOP */

#define em_snoop_init()
#define snoop_bt_init()

#define BT_snoop_write_packet(t, r, p, l)

#define BT_snoop_logging(e)

/** Shutdown of EtherMind Snoop */
#define snoop_bt_shutdown()

#endif /* BT_SNOOP */

/** Macro to enable/disable btsnoop logging */
#define BT_snoop_logging_enable()   BT_snoop_logging(BT_TRUE)
#define BT_snoop_logging_disable()  BT_snoop_logging(BT_FALSE)

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_SNOOP_ */

