/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <rt_sys.h>
#include <rt_misc.h>

__asm(".global __use_no_semihosting");

extern void $Super$$_sys_open(void);
FILEHANDLE $Sub$$_sys_open(const char *name, int openmode)
{
    return 1;
}

extern void $Super$$_sys_close(void);
int $Sub$$_sys_close(FILEHANDLE fh)
{
    return 0;
}

extern void $Super$$_sys_write(void);
int $Sub$$_sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
    return 0;
}

extern void $Super$$_sys_read(void);
int $Sub$$_sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode)
{
    return -1;
}

extern void $Super$$_ttywrch(void);
void $Sub$$_ttywrch(int ch)
{
    fputc(ch, stdout);
}

extern void $Super$$_sys_istty(void);
int $Sub$$_sys_istty(FILEHANDLE fh)
{
    return 0;
}

extern void $Super$$_sys_seek(void);
int $Sub$$_sys_seek(FILEHANDLE fh, long pos)
{
    return -1;
}

extern void $Super$$_sys_flen(void);
long $Sub$$_sys_flen(FILEHANDLE fh)
{
    return -1;
}

extern void $Super$$_sys_exit(void);
long $Sub$$_sys_exit(FILEHANDLE fh)
{
    return -1;
}

extern void $Super$$_sys_command_string(void);
char *$Sub$$_sys_command_string(char *cmd, int len)
{
    return NULL;
}
