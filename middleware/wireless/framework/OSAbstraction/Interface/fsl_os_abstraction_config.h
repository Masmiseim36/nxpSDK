/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


#ifndef FSL_OS_ABSTRACTION_CONFIG_H
#define FSL_OS_ABSTRACTION_CONFIG_H

#ifndef osNumberOfSemaphores
#define osNumberOfSemaphores 5
#endif
#ifndef osNumberOfMutexes
#define osNumberOfMutexes    5
#endif
#ifndef osNumberOfMessageQs
#define osNumberOfMessageQs  0
#endif
#ifndef osNumberOfMessages
#define osNumberOfMessages   10
#endif
#ifndef osNumberOfEvents
#define osNumberOfEvents     5
#endif

#ifndef gMainThreadStackSize_c
#define gMainThreadStackSize_c 1024
#endif
#ifndef gMainThreadPriority_c
#define gMainThreadPriority_c 7
#endif

#ifndef gTaskMultipleInstancesManagement_c
#define gTaskMultipleInstancesManagement_c  0
#endif

#ifndef osCustomStartup
#define osCustomStartup 0
#endif

#endif /* FSL_OS_ABSTRACTION_CONFIG_H */
