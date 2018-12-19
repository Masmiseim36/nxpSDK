/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
*
* ile
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _FSL_OS_ABSTRACTION_CONFIG_H_
#define _FSL_OS_ABSTRACTION_CONFIG_H_

#ifndef gMainThreadStackSize_c
#define gMainThreadStackSize_c 1024
#endif

#ifndef gMainThreadPriority_c
#define gMainThreadPriority_c 1
#endif

#ifndef gTaskMultipleInstancesManagement_c
#define gTaskMultipleInstancesManagement_c  0
#endif

#ifndef osCustomStartup
#define osCustomStartup 0
#endif

#endif /* _FSL_OS_ABSTRACTION_CONFIG_H_ */
