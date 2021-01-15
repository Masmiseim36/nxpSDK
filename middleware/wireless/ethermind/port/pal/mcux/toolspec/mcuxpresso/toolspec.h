
/**
 *  \file toolsoec.h
 *  Toolchain specific header
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_TOOLSPEC_
#define _H_TOOLSPEC_

/* --------------------------------------------- Header File Inclusion */
#include <stdint.h>

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
int _write (int fd, char *ptr, int len);
int _read (int fd, char *ptr, int len);
int _close (int fd);
int _fstat (int fd);
int _isatty (int fd);
int _lseek (int fd);

void HardFault_Handler(void);
void debugHardfault(uint32_t *sp);

#endif /* _H_TOOLSPEC_ */

