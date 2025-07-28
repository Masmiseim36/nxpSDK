/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#if defined( __MCUXPRESSO ) || defined ( __GNUC__ )
/* Dummy function to avoid libc_nano.a error during linking */
__attribute__((weak)) void _close(void)
{
}
__attribute__((weak)) void _fstat(void)
{
}
__attribute__((weak)) void _isatty(void)
{
}
__attribute__((weak)) void _lseek(void)
{
}
__attribute__((weak)) void _read(void)
{
}
__attribute__((weak)) void _getpid_r(void)
{
}
__attribute__((weak)) void _kill_r(void)
{
}
#endif
