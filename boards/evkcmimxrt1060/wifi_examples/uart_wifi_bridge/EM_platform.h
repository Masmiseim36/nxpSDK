
/**
 *  \file EM_platform.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_EM_PLATFORM_
#define _H_EM_PLATFORM_

/* --------------------------------------------- Header File Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "ff.h"

/* --------------------------------------------- Global Definitions */
#define EM_ENABLE_PAL_OS
#define EM_PLATFORM_MAIN

#define EM_HAVE_STATIC_DECL
#define EM_HAVE_CONST_DECL

#ifdef printf
#undef printf
#endif /* printf */
#define printf(...) (void)PRINTF(__VA_ARGS__)

#ifdef scanf
#undef scanf
#endif /* scanf */
#define scanf(...) (void)SCANF((char *)__VA_ARGS__)

#ifdef getchar
#undef getchar
#endif /* getchar */
#define getchar GETCHAR

#ifdef fflush
#undef fflush
#endif /* fflush */
#define fflush(x)

/* TODO: Map to SDK/platform interfaces */
#define fgets(...)
#define rewind(...)
#define ftell(...)      0
#define fgetc(x)        0
#define fscanf(fp,f,x)  (*x) = 0
#define fprintf(...)
#define fread(...)
#define fwrite(...)
#define fopen(...) 0
#define fclose(...)
#define fseek(...)

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void EM_enter_sleep_pl(void);
void EM_exit_sleep_pl(void);

int EM_str_to_num_pl
       (
           /* IN */  char * str,
           /* IN */  int    len
       );
int EM_str_to_hexnum_pl
       (
           /* IN */ char * str,
           /* IN */ int    len
       );
void EM_num_to_str_pl
     (
         /* IN  */ int num,
         /* OUT */ char * str,
         /* OUT */ int  * len
     );
#endif /* _H_EM_PLATFORM_ */

