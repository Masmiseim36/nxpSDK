
/**
 *  \file btsnoop_pl.h
 *
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BTSNOOP_PL_
#define _H_BTSNOOP_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_fops.h"

/* --------------------------------------------- Global Definitions */
/* Snoop file name */
#define BT_SNOOP_FILE_NAME \
		EM_FOPS_PATH_JOIN(EM_FOPS_BASE, "btsnoop")

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void btsnoop_init_pl (void);

API_RESULT btsnoop_open_pl (void);
API_RESULT btsnoop_close_pl (void);

UINT16 btsnoop_write_pl (void * buffer, UINT16 size);
UINT64 btsnoop_get_us_timestamp_pl(void);

#endif /* _H_BTSNOOP_PL_ */

