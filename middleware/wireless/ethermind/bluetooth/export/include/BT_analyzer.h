
/**
 *  \file BT_analyzer.h
 *
 *  This file lists all the macros and APIs that are exported
 *  for logging HCI rx/tx data in various format/live viewer
 *  - like snoop, Ellisys, FTS etc.
 */

/*
 *  Copyright (C) 2023. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_ANALYZER_
#define _H_BT_ANALYZER_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup analyzer_module ANALYZER
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Analyzer functionality module to the Application and other upper layers of the
 *  stack.
 */

/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Function Declarations */
/**
 * \defgroup analyzer_api_defs API Definitions
 * \{
 * This section describes the EtherMind Analyzer APIs.
 */
#ifdef __cplusplus
extern "C"{
#endif
#ifdef BT_ANALYZER
/** Initialization/Shutdown of EtherMind btanalyzer module */
/**
 *  \brief To do power on initialization of EtherMind btanalyzer module
 *
 *  \par Description:
 *       This function is the EtherMind-Init handler for the btanalyzer module
 *       and performs power-on initialization.
 *
 *  \note This function must be called only once.
 */
void em_analyzer_init (void);

#ifdef BT_HAVE_SHUTDOWN
void em_analyzer_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \brief To perform Bluetooth specific initializations for EtherMind btanalyzer module
 *
 *  \par Description:
 *       This function is the Bluetooth-ON handler for btanalyzer module, and it
 *       performs bluetooth specific initializations for the btanalyzer module.
 */
void analyzer_bt_init (void);

/**
 *  \brief To perform Bluetooth specific shutdown for EtherMind btanalyzer module
 *
 *  \par Description:
 *       This function is the Bluetooth-OFF handler for btanalyzer module, and it
 *       performs bluetooth specific shutdown for the btanalyzer module.
 */
void analyzer_bt_shutdown (void);

/**
 *  \brief To write btanalyzer packet
 *
 *  \par Description
 *  This function writes btanalyzer packet record.
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
void BT_analyzer_write_packet
     (
         /* IN */ UCHAR   pkt_type,
         /* IN */ UCHAR   is_rxed,
         /* IN */ UCHAR  *pkt,
         /* IN */ UINT16  pkt_len
     );

/**
 *  \brief To enable/disable btanalyzer logging
 *
 *  \par Description
 *  This function allows run-time enable/disable of btanalyzer logging.
 *
 *  \param [in] enable
 *         BT_TRUE to enable, BT_FALSE to disable
 *
 *  \return None
 */
void BT_analyzer_logging
     (
         /* IN */ UCHAR   enable
     );
#else  /* BT_ANALYZER */

#define em_analyzer_init()
#define analyzer_bt_init()

#define BT_analyzer_write_packet(t, r, p, l)

#define BT_analyzer_logging(e)

/** Shutdown of EtherMind Analyzer */
#define analyzer_bt_shutdown()

#endif /* BT_ANALYZER */

/** Macro to enable/disable btanalyzer logging */
#define BT_analyzer_logging_enable()   BT_analyzer_logging(BT_TRUE)
#define BT_analyzer_logging_disable()  BT_analyzer_logging(BT_FALSE)

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_ANALYZER_ */

