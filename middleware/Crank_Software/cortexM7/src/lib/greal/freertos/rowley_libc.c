/*
 * Copyright 2013, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>

#if GR_TOOLCHAIN(rowley)

#include <__libc.h>

#include <FreeRTOS.h>

#include "task.h"

void 
__heap_lock(void)
{
	taskENTER_CRITICAL();
}

void 
__heap_unlock(void)
{
	taskEXIT_CRITICAL();
}

#endif
