
/**
 *  \file sdp.h
 *
 *  Module header file for SDP.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SDP_
#define _H_SDP_

/* -------------------------------- Header File Inclusion */


/* -------------------------------- Global Definitions */


/* -------------------------------- Data Type/Structures/Typedefs */


/* -------------------------------- Unions */


/* -------------------------------- Function Declarations */
/*
 * Stack Initialization modules call this function to initialize
 * the SDP structures before calling any other SDP APIs.
 */
void em_sdp_init (void);
#ifdef BT_HAVE_SHUTDOWN
void em_sdp_shutdown (void);
#endif /* BT_HAVE_SHUTDOWN */
void sdp_bt_init (void);

/*
 * Shutdown function disconnects all established connections with the
 * peer SDP applications  followed by  the deletion of memory consumed
 * by these connections.
 */
void sdp_bt_shutdown(void);

#ifdef BT_DRIVER_MODE
/*
 * This function returns the Global SDP User Context Identifier,
 * that stores the SDP context index for the recent-most
 * SDP Completed Operation and for which the Registered
 * Callback has been called.
 */
API_RESULT bt_sdp_get_user_context_id ( /* OUT */ UCHAR *context_id );
#endif /* BT_DRIVER_MODE */


#endif /* _H_SDP_ */


