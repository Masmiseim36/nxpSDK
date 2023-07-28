
/*
 *  \file lc3_os.h
 *
 *  This header file contains the various OS dependent data types.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_LC3_OS_
#define _H_LC3_OS_

#ifndef LC3_TEST_MODE

#include "EM_os.h"

#else

/* ---------------------------------- System Header File Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------- Type Definitions */
#if 0
typedef unsigned int            UINT32;
typedef int                     INT32;
#else
typedef unsigned long int       UINT32;
typedef long int                INT32;
typedef unsigned long long      UINT64;
typedef long long               INT64;
#endif /* 0 */
typedef unsigned short int      UINT16;
typedef short int               INT16;
typedef unsigned char           UINT8;
typedef unsigned char           UCHAR;
typedef unsigned char           BOOLEAN;
typedef char                    INT8;
typedef char                    CHAR;

/* Definition of API_RESULT */
typedef UINT16                  API_RESULT;

/* Definitions of API_SUCCESS & API_FAILURE */
#define API_SUCCESS             0x0000
#define API_FAILURE             0xFFFF

#endif /* LC3_TEST_MODE */

#endif  /* _H_LC3_OS_ */

