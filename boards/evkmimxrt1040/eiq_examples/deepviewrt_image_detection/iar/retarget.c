/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <LowLevelIOInterface.h>

#pragma diag_suppress = Pe826

#pragma weak __close
int __close(int handle)
{
  return 0;
}

#pragma weak __lseek
long __lseek(int handle, long offset, int whence)
{
  return -1;
}

#pragma weak remove
int remove(const char * filename)
{
  return 0;
}
