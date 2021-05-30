/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __TIMER_H__
#define __TIMER_H__


uint32_t GetElapsedTime(void);
void SysTickHandler(void);
void StartTimer(void);
void StopTimer(void);

#endif /* __TIMER_H__ */