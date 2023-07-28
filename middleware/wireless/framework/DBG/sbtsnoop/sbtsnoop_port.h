/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SBTSNOOP_PORT_H_
#define _SBTSNOOP_PORT_H_

#define SBTSNOOP_STRINGIZE(x) SBTSNOOP_STRINGIZE2(x)
#define SBTSNOOP_STRINGIZE2(x) #x

#include SBTSNOOP_STRINGIZE(SBTSNOOP_PORT_FILE)

#endif /* _SBTSNOOP_PORT_H_ */