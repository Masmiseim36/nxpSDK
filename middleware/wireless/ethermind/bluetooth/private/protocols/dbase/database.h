
/**
 *  \file database.h
 *
 *  This module contains the constants that will be used by user
 *  applications and SDP and function declerations.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_DATABASE_
#define _H_DATABASE_

/* ------------------------ Header File Inclusion */
#include "BT_dbase_api.h"
#include "BT_debug.h"
#include "BT_sdp_api.h"


/* ------------------------ Function Declarations */
void em_db_sdp_init(void);

#ifdef BT_HAVE_SHUTDOWN
void em_db_sdp_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

#endif /* _H_DATABASE_ */

