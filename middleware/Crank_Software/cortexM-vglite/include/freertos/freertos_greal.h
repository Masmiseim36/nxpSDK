/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#ifndef GR_FREERTOS_GREAL_H_
#define GR_FREERTOS_GREAL_H_

#include <stdio.h>
#include <stdint.h>
#include <pthread.h> 

#if defined(GRE_TARGET_TOOLCHAIN_mplabx)
#define _GREAL_MALLOC_
#define _GREAL_CALLOC_
#define _GREAL_FREE_
#define _GREAL_REALLOC_
#define _GREAL_STRDUP_
#endif

#define _MAX_UNAME 				30
typedef struct _greal_utsname {
	char  			sysname[_MAX_UNAME];	///<name of this implementation of the operating system
	char  			nodename[_MAX_UNAME];	///<name of this node within an implementation-dependent communications network
	char  			release[_MAX_UNAME];	///<current release level of this implementation
	char  			version[_MAX_UNAME];	///<current version level of this release
	char  			machine[_MAX_UNAME];	///<name of the hardware type on which the system is running
} greal_utsname_t;

typedef struct _greal_timespec_t {
	unsigned int 	tv_sec;
	unsigned int 	tv_nsec;
} greal_timespec_t;

#if defined(GRE_FEATURE_FILEIO_FATFS)
#if defined(GRE_TARGET_TOOLCHAIN_mcuxpresso)
#undef getc
#undef feof
#undef ferror
#undef fflush
#endif
#endif

#define GREAL_CLOCK_REALTIME	0
#define GREAL_CLOCK_MONOTONIC	1

#ifndef ETIMEDOUT
#define ETIMEDOUT				12
#endif

#ifndef ENOMEM
#define ENOMEM 					12
#endif 

#ifndef EINVAL
#define EINVAL  				22
#endif

#ifndef EFBIG
#define EFBIG 					27
#endif

int greal_uname(greal_utsname_t *_uname);

void greal_nanosleep(greal_timespec_t *_to, greal_timespec_t *_remaining);

int greal_clock_gettime(int _clock, greal_timespec_t *_ts);

#define PTHREAD_NAME_MAX_LENGTH 9
int greal_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg, const char *name);

/**
 * General Non-ANSI Functions
 */
char *strdup(const char *_str);
char *getenv(const char *_var);

char *strtok_r(char * _str, const char * _sep, char ** _laststr);

int getsubopt(char **_optionp, char * const *_keylistp, char **_valuep);

char *getcwd(char *buf, int size);


#if(GR_TOOLCHAIN(dassgcc))

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "custom_config_oqspi.h"
#include "bsp_defaults.h"
#include "bsp_debug.h"

#include "cmsis_gcc.h"
#include "hw_gpio.h"
#include "hw_cpm.h"
#include "hw_watchdog.h"
#include "hw_clk.h"
#include "ad_flash.h"
#include <FreeRTOS.h>

#endif //dassgcc

/**
 * Platform specific values
 */

/**
 * Prototypes needed for IAR in order to compile libgreal.a 
 */
#if(GR_TOOLCHAIN(iar))

char * getcwd(char *buf, int size); 

/**
 * Thread safety calls.  These are enabled through the general library option "Enable threading support"
 */
void __iar_system_Mtxinit(__iar_Rmtx* lock);
void __iar_system_Mtxdst(__iar_Rmtx* lock);
void __iar_system_Mtxlock(__iar_Rmtx* lock);
void __iar_system_Mtxunlock(__iar_Rmtx* lock);
void __iar_file_Mtxinit(__iar_Rmtx* lock);
void __iar_file_Mtxdst(__iar_Rmtx* lock);
void __iar_file_Mtxlock(__iar_Rmtx* lock);
void __iar_file_Mtxunlock(__iar_Rmtx* lock);
int remove(const char* filename);

/**
 * The dl* function calls
 */
void * dlopen(const char *_file, int _mode);
void * dlsym(void *_handle, const char *_name);
int    dlclose(void *_handle);
char * dlerror(void);

/**
 * The calls to monitor CPU usage
 */ 
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void StartIdleMonitor(void);

void EndIdleMonitor(void);

/**
 * Threading prototypes
 */
pthread_t pthread_self(void);
int pthread_kill(pthread_t tid, int sig);
int pthread_create_named(pthread_t *thread, const pthread_attr_t *_attr, void *(*start_routine)(void*), void *arg, const char *name);
void taskYield(void);

/**
 * Timer prorotypes
 */
#include <FreeRTOS.h>
#include <timers.h>
void vCallbackFunction(TimerHandle_t xTimer);
#endif //iar

/**
 * Metrics prototypes
 */
void greal_task_switched_in(void);
void greal_task_switched_out(void);

#endif /* GR_FREERTOS_GREAL_H_ */
