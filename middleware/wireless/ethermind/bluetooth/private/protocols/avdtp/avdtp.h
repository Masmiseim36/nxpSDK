
/**
 *  \file avdtp.h
 *
 *  This is the Module Header File for AVDTP Layer.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_AVDTP_
#define _H_AVDTP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Data Type/Structures/Typedefs */


/* ----------------------------------------- Unions */


/* ----------------------------------------- API Declarations */

/* AVDTP Initialization APIs */
void em_avdtp_init ( void );
void avdtp_bt_init ( void );

/* AVDTP Shutdown API */
#ifdef BT_HAVE_SHUTDOWN
void em_avdtp_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */
void avdtp_bt_shutdown ( void );

#endif /* _H_AVDTP_ */


