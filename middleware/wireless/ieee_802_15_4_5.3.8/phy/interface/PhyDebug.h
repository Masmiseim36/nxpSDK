/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _PHY_DEBUG_H_
#define _PHY_DEBUG_H_


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
//#include "MacPhyDebug.h"


/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

// a value of zero disables the logging of that event
#define  PDBG_SET_EVENT_TRIGGER       0x01 // 2 parameters
#define  PDBG_SET_EVENT_TIMEOUT       0x02 // 3 parameters
#define  PDBG_DISABLE_EVENT_TIMEOUT   0x03
#define  PDBG_READ_CLOCK              0x04
#define  PDBG_INIT_EVENT_TIMER        0

#define  PDBG_WTRMRK_VECT_INTERRUPT   0x06
#define  PDBG_TIMERS_VECT_INTERRUPT   0x07
#define  PDBG_TX_VECT_INTERRUPT       0x08
#define  PDBG_RX_VECT_INTERRUPT       0x09

#define  PDBG_CCA_CONFIRM_IDLE        0x0A
#define  PDBG_CCA_CONFIRM_BUSY        0x0B
#define  PDBG_DATA_CONFIRM            0x0C
#define  PDBG_DATA_INDICATION         0x0D
#define  PDBG_ED_CONFIRM              0x0E
#define  PDBG_SYNC_LOSS_INDICATION    0x0F

#define  PDBG_UNLOCK_IRQ              0x10
#define  PDBG_WTRMARK_IRQ             0x11
#define  PDBG_START_INDICATION        0x12
#define  PDBG_TIMEOUT_INDICATION      0x13
#define  PDBG_TMR1_IRQ                0x14
#define  PDBG_TMR4_IRQ                0x15
#define  PDBG_FILTER_FAIL_IRQ         0x16
#define  PDBG_TX_IRQ                  0x17
#define  PDBG_RX_IRQ                  0x18 /* 1 parameter */
#define  PDBG_CCA_IRQ                 0x19
#define  PDBG_SEQ_IRQ                 0x1A /* 1 parameter */

#define  PDBG_PHY_INIT                0x20
#define  PDBG_PHY_ABORT               0x21
#define  PDBG_PP_SET_PROMISC          0
#define  PDBG_PP_SET_PANID_PAN0       0x23
#define  PDBG_PP_SET_SHORTADDR_PAN0   0x24
#define  PDBG_PP_SET_LONGADDR_PAN0    0x25
#define  PDBG_PP_SET_MACROLE_PAN0     0x26
#define  PDBG_PP_ADD_INDIRECT         0x27 /* 1 parameter */
#define  PDBG_PP_REMOVE_INDIRECT      0x28 /* 1 parameter */
#define  PDBG_PP_READ_LATEST_INDEX    0x29 /* 1 parameter */

#define  PDBG_DATA_REQUEST            0x30
#define  PDBG_PLME_RX_REQUEST         0x31
#define  PDBG_PLME_CCA_ED_REQUEST     0x32
#define  PDBG_PLME_SET_CHAN_REQUEST   0x33
#define  PDBG_PLME_SET_POWER_REQUEST  0x34
#define  PDBG_PLME_WAIT_REQUEST       0x35

#define  PDBG_PP_SET_PANID_PAN1       0x36
#define  PDBG_PP_SET_SHORTADDR_PAN1   0x37
#define  PDBG_PP_SET_LONGADDR_PAN1    0x38
#define  PDBG_PP_SET_MACROLE_PAN1     0x39

/* simple events - without timestamp */
/******  PDBG_MAC_FUNCTION_CALL       PDBG_SIMPLE_EVENT           3 parameters */
#define  PDBG_READ_FSM                PDBG_SIMPLE_EVENT + 0x01 /* 1 parameter  */
#define  PDBG_FILTER_FAIL_CODE        PDBG_SIMPLE_EVENT + 0x02 /* 2 parameters */


#ifdef MAC_PHY_DEBUG

#define  DEBUG_LOG_ENTRIES  768

#define  PHY_DEBUG_LOG(item)                       if(PDBG_RESERVED_EVENT!=item){PhyDebugLogTime(item);}
#define  PHY_DEBUG_LOG1(item,param)                if(PDBG_RESERVED_EVENT!=item){PhyDebugLogTime(PDBG_EXTENDED_EVENT | item);PhyDebugLogParam1(item,param);}
#define  PHY_DEBUG_LOG2(item,param1,param2)        if(PDBG_RESERVED_EVENT!=item){PhyDebugLogTime(PDBG_EXTENDED_EVENT | item);PhyDebugLogParam2(item,param1,param2);}
#define  PHY_DEBUG_LOG3(item,param1,param2,param3) if(PDBG_RESERVED_EVENT!=item){PhyDebugLogTime(PDBG_EXTENDED_EVENT | item);PhyDebugLogParam3(item,param1,param2,param3);}

#define  PHY_DEBUG_LOG_SIMPLE1(item,param)                if(PDBG_RESERVED_EVENT!=item){PhyDebugLogParam1(item,param);}
#define  PHY_DEBUG_LOG_SIMPLE2(item,param1,param2)        if(PDBG_RESERVED_EVENT!=item){PhyDebugLogParam2(item,param1,param2);}
#define  PHY_DEBUG_LOG_SIMPLE3(item,param1,param2,param3) if(PDBG_RESERVED_EVENT!=item){PhyDebugLogParam3(item,param1,param2,param3);}

void PhyUnexpectedTransceiverReset(void);

void PhyDebugLogTime(uint8_t item);
void PhyDebugLogParam1(uint8_t item, uint8_t param1);
void PhyDebugLogParam2(uint8_t item, uint8_t param1, uint8_t param2);
void PhyDebugLogParam3(uint8_t item, uint8_t param1, uint8_t param2, uint8_t param3);


#else /* not def MAC_PHY_DEBUG */

#define PhyUnexpectedTransceiverReset()

#define  PHY_DEBUG_LOG(item)
#define  PHY_DEBUG_LOG1(item,param)
#define  PHY_DEBUG_LOG2(item,param1,param2)
#define  PHY_DEBUG_LOG3(item,param1,param2,param3)

#define  PHY_DEBUG_LOG_SIMPLE1(item,param)
#define  PHY_DEBUG_LOG_SIMPLE2(item,param1,param2)
#define  PHY_DEBUG_LOG_SIMPLE3(item,param1,param2,param3)

#endif /* MAC_PHY_DEBUG */


#endif /* _PHY_DEBUG_H_ */


