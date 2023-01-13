
/**
 *  \file obex.h
 *
 *  This is the Module Header File of OBEX layer
 *  that is common to both the profiles and the stack
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_OBEX_
#define _H_OBEX_

/* -------------------------------------------- Header File Inclusion */


/* -------------------------------------------- Type Definitions */


/* -------------------------------------------- Structure/Data Types */


/* -------------------------------------------- Macros */


/* -------------------------------------------- Function Declarations */
void em_obex_init( void );
#ifdef BT_HAVE_SHUTDOWN
void em_obex_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */
void obex_bt_init( void );
void obex_bt_shutdown( void );


#endif /* _H_OBEX_ */

