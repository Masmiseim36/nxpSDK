/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(__ICCARM__)
void __aeabi_assert(const char *failedExpr, const char *file, int line);
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    __aeabi_assert(failedExpr, file, line);
}
#endif
