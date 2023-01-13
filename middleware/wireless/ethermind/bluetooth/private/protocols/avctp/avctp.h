
/**
 *  \file avctp.h
 *
 *  This is the Module Header File for AVCTP Layer.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_AVCTP_
#define _H_AVCTP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- API Declarations */

/* AVCTP Initialization APIs */
void em_avctp_init ( void );
void avctp_bt_init ( void );

/* AVCTP Shutdown API */
#ifdef BT_HAVE_SHUTDOWN
void em_avctp_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */
void avctp_bt_shutdown ( void );

#endif /* _H_AVCTP_ */


