/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */

/**
* This file contains macros for OS abstraction layer selection,
* as well as branching to the required OS level <OS>_greal.h specific header
* required for the currently selected OS/HW
*/

#ifndef GR_GREAL_H
#define GR_GREAL_H

/**
 * Common configuration tests used to evaluate the target OS/CPU/TOOLCHAIN
 */
#define GR_OS(os) ((defined GRE_TARGET_OS_##os) && GRE_TARGET_OS_##os)
#define GR_CPU(cpu) ((defined GRE_TARGET_CPU_##cpu) && GRE_TARGET_CPU_##cpu)
#define GR_TOOLCHAIN(tc) ((defined GRE_TARGET_TOOLCHAIN_##tc) && GRE_TARGET_TOOLCHAIN_##tc)
#define GR_FEATURE(tc) ((defined GRE_FEATURE_##tc) && GRE_FEATURE_##tc)

/**
 * Prototypes
 */

/** 
 * System level metric gathering related functions
 */ 

/**
 * Setups up any system resources that are needed to acquire metrics 
 * 
 * @return A void pointer to data that will be passed in to subsequent calls to get system metrics.  This can be null. 
 * 
 */
void *greal_setup_metrics(void);

/**
 * Frees up any ssytem resources that were needed to gather system metrics
 * 
 * @param data A pointer to the system  metrics that were created through the call to greal_setup_metrics
 */ 
void greal_teardown_metrics(void *data); 

/**
 * Reports the currently used amount of memory by the Storyboard engine. 
 * 
 * @param data A pointer to data that may be needed to obtain the information that is being requested 
 * from the system
 * @return The number of bytes of memory used by the Storyboard engine, or 0 if that information
 * is unavailable. 
 */
long greal_get_process_memory_usage(void *data);

/**
 * Reports the amount of heap allocated to the Storyboard engine by the OS.
 * This number can be diffence than process memory usage based on decisions 
 * made by the system allocator, such as when to reclaim unused pages.  
 *
 * @param data A pointer to data that may be needed to obtain the information that is being requested 
 * from the system. 
 * @return The number of bytes of heap allocated to the Storyboard engine or 0 if the 
 * information is unavailable. 
 */
long greal_get_heap_memory_usage(void *data);

/**
 * Reported the amount of time in MS used by the Storyboard engine.  This time is the total amount of 
 * time spent by the Storyboard engine since it began running. 
 * 
 * @param data A pointer to data that may be needed to obtain the information that is being requested 
 * from the system. 
 * @return The total amount of time in MS used by the Storyboard application since its launch or 0 if the information is
 * unavailable.  
 */ 
long greal_get_sbengine_cpu_time(void *data);


/**
 * Platform specific values
 */
#if GR_OS(freertos)
#include <freertos_greal.h>
#elif GR_OS(wec2013) || GR_OS(win32) || GR_OS(wince) || GR_OS(wincompact7) 
#include <win32_greal.h>
#elif GR_OS(macos)
#include <macos_greal.h>
#elif GR_OS(ios)
#include <ios_greal.h>
#elif GR_OS(android)
#include <android_greal.h>
#elif GR_OS(segger)
#include <segger_greal.h>
#elif GR_OS(mqx)
#include <mqx_greal.h>
#elif GR_OS(threadx)
#include <threadx_greal.h>
#elif GR_OS(ucosii)
#include <ucosii_greal.h>
#elif GR_OS(ucosiii)
#include <ucosiii_greal.h>
#elif GR_OS(vxworks)
#include <vxworks_greal.h>
#elif GR_OS(integrity)
#include <integrity_greal.h>
#elif GR_OS(itron)
#include <itron_greal.h>
#else
#include <gre/sdk/generic_greal.h>
#endif

#include <gre/sdk/greal_alloc.h>
#include <gre/sdk/greal_timer.h>

#endif /* GR_GREAL_H */
