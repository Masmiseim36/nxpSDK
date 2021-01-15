
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
#define printf PRINTF

#ifdef scanf
#undef scanf
#endif /* scanf */
#define scanf SCANF

#ifdef getchar
#undef getchar
#endif /* getchar */
#define getchar GETCHAR

#ifdef fflush
#undef fflush
#endif /* fflush */
#define fflush(x)

/* TODO: Map to SDK/platform interfaces */
#define	fgets(...)
#define rewind(...)
#define ftell(...)      0
#define fgetc(x)        0
#define	fscanf(fp,f,x)  (*x) = 0
#define fprintf(...)
#define fread(...)
#define fwrite(...)
#define fopen(...) 0
#define fclose(...)
#define fseek(...) 

#if defined(__IAR_SYSTEMS_ICC__)
#define stdin
#endif /* defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM) || defined(__ARMCC_VERSION) */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void EM_enter_sleep_pl(void);
void EM_exit_sleep_pl(void);

int _write (int fd, char *ptr, int len);
int _read (int fd, char *ptr, int len);
int _close (int fd);
int _fstat (int fd);
int _isatty (int fd);
int _lseek (int fd);

void HardFault_Handler(void);
void debugHardfault(uint32_t *sp);

#endif /* _H_EM_PLATFORM_ */

