
/**
 *  \file mcap.h
 *
 *  This file contains declaration of APIs that are exported for
 *  the other EtherMind modules interfacing with MCAP.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MCAP_
#define _H_MCAP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- API Declarations */
/** Initialization & Shutdown of MCAP */
void em_mcap_init (void);

#ifdef BT_HAVE_SHUTDOWN
void em_mcap_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

void mcap_bt_init (void);
void mcap_bt_shutdown (void);

#endif /* _H_MCAP_ */

