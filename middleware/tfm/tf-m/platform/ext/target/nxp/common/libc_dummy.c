/*
 * Copyright 2023 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#if defined( __MCUXPRESSO ) || defined ( __GNUC__ )
/* Dummy function to avoid libc_nano.a error during linking */
void _close(void)
{
}
void _fstat(void)
{
}
void _isatty(void)
{
}
void _lseek(void)
{
}
void _read(void)
{
}
#endif