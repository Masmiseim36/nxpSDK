
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
#define APPL_ERR(...)
#define APPL_TRC(...)
#define CONSOLE_OUT(...)
#define CONSOLE_IN(...)
#else /* DONT_USE_STANDARD_IO */
#define APPL_ERR     printf
#define APPL_TRC     printf
#define CONSOLE_OUT  printf
#define CONSOLE_IN   scanf
#define ENABLE       1
#endif /* DONT_USE_STANDARD_IO */

extern int appl_debug_enabled;
#define LOG_DEBUG(...) if(ENABLE == appl_debug_enabled) printf(__VA_ARGS__)
/* ----------------------------------------- Structures/Data Types */

/* ----------------------------------------- Macros */

/* ----------------------------------------- Function Declarations */
API_RESULT appl_get_bd_addr(UCHAR *bd_addr);
void appl_dump_bytes (UCHAR *buffer, UINT16 length);
void appl_dump_bytes_no_limit_logs(UCHAR *buffer, UINT16 length);

UINT32 appl_str_to_num
       (
           /* IN */  UCHAR  * str,
           /* IN */  UINT16 len
       );
UINT32 appl_str_to_hexnum
       (
           /* IN */ UCHAR* str,
           /* IN */ UINT16 len
       );
void appl_num_to_str
     (
         /* IN  */ UINT32 num,
         /* OUT */ UCHAR* str,
         /* OUT */ UCHAR *len
     );

#endif /* _H_APPL_PROFILE_ */
