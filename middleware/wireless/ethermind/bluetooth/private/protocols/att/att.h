
/**
 *  \file att.h
 *
 *  This file contains the fuction definitions which are exported to other
 *  EtherMind modules for interfacing with Attribute Protocol.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_ATT_
#define _H_ATT_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Functions */
/**
 *  \fn em_att_init
 *
 *  \brief Initializes Module.
 *
 *  \Description Initializes Module tables and registers interface with L2CAP.
 *
 *  \return None
 */
void em_att_init(void);

#ifdef BT_HAVE_SHUTDOWN
/**
 *  \brief De-Initializes Module.
 *
 *  \Description De-Initializes ATT Module.
 *
 *  \return None
 */
void em_att_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \fn att_bt_init
 *
 *  \brief Module Initialization on BT ON.
 *
 *  \Description Registers Interface with L2CAP.
 *
 *  \return None
 */
void att_bt_init (void);

/**
 *  \fn att_bt_shutdown
 *
 *  \par Description:
 *  This function is the Bluetooth-OFF handler for ATT module, and it
 *  performs bluetooth specific shutdown for the module - currently,
 *  nothing is done here.
 *
 *  \return None
 */
void att_bt_shutdown (void);

#endif /* _H_ATT_ */
