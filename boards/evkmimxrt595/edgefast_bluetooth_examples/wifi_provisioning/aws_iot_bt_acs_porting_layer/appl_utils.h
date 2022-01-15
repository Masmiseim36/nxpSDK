
/**
 *  \file appl_utils.h
 *
 *  Main Application Utility Header File
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_UTILS_
#define _H_APPL_UTILS_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* ----------------------------------------- Global Definitions */
#ifdef DONT_USE_STANDARD_IO
#define CONSOLE_IN(...)
#else /* DONT_USE_STANDARD_IO */
#define CONSOLE_IN   scanf
#endif /* DONT_USE_STANDARD_IO */

/* ----------------------------------------- Structures/Data Types */

/* ----------------------------------------- Macros */

/* ----------------------------------------- Function Declarations */
void appl_get_bd_addr(UCHAR *bd_addr);
void appl_dump_bytes (UCHAR *buffer, UINT16 length);

#endif /* _H_APPL_PROFILE_ */
