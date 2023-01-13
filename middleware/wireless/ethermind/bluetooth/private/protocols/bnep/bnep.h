
/**
 *  \file bnep.h
 *
 *  Module Header File for the implementation of Bluetooth Network
 *  Encapsulation Protocol (BNEP) over L2CAP.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BNEP_
#define _H_BNEP_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* --------------------------------------------- Global Definitions */


/* --------------------------------------------- Structures/Data Types */


/* --------------------------------------------- Unions */


/* --------------------------------------------- Macros */


/* --------------------------------------------- API Declarations */
/** Initialization/Shutdown of BNEP */
void em_bnep_init ( void );

#ifdef BT_HAVE_SHUTDOWN
void em_bnep_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

void bnep_bt_init ( void );
void bnep_bt_shutdown ( void );


#ifdef BNEP_TEST_MODE_RAW
/* To Enable Raw Data Transmission & Reception via BNEP */
API_RESULT BT_bnep_enable_raw
           (
               UCHAR    /* Flag */
           );

/* To Write Raw Data over a BNEP Connection */
API_RESULT BT_bnep_write_raw
           (
               UCHAR *    /* Remote BD_ADDR */,
               UCHAR *    /* Data */,
               UINT16     /* Data Length */
           );
#endif /* BNEP_TEST_MODE_RAW */

#endif /* _H_BNEP_ */


