/*
 * Copyright 2016, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#ifdef GRE_TARGET_TOOLCHAIN_iar

#include <FreeRTOS.h>
#include <yfuns.h>
#include "task.h"
#include <gre/sdk/greal.h>

/**
 * This function can be used to initialize a global, sytem wide lock. 
 */
void
__iar_system_Mtxinit(__iar_Rmtx* lock) {
	/**
	 * Not implemented.
	 */
}

/**
 * This function can be used to destroy a global, sytem wide lock. 
 */
void
__iar_system_Mtxdst(__iar_Rmtx* lock) {
	/**
	 * Not implemented.
	 */
}

/**
 * This function locks a systen lock.  This is the entry point for thread
 * safety from the IAR C library.   
 */
void
__iar_system_Mtxlock(__iar_Rmtx* lock) {
    vTaskSuspendAll();
}

/**
 * This function unlocks a systen lock.  This is the exit point for thread
 * safety from the IAR C library.   
 */
void
__iar_system_Mtxunlock(__iar_Rmtx* lock) {
    xTaskResumeAll();
}

/**
 * This function can be used to initialize a global, sytem wide lock used specifically for files. 
 */
void
__iar_file_Mtxinit(__iar_Rmtx* lock) {
	/**
	 * Not implemented.
	 */
}

/**
 * This function can be used to destroy a global, sytem wide lock used specifically for files. 
 */
void
__iar_file_Mtxdst(__iar_Rmtx* lock) {
	/**
	 * Not implemented.
	 */
}

/**
 * This function locks a file systen lock.  This is the entry point for thread
 * safety from the IAR C library for file operations.   
 */
void
__iar_file_Mtxlock(__iar_Rmtx* lock) {
    vTaskSuspendAll();
}

/**
 * This function unlocks a file systen lock.  This is the entry point for thread
 * safety from the IAR C library for file operations.   
 */
void
__iar_file_Mtxunlock(__iar_Rmtx* lock) {
   xTaskResumeAll();
}

long __lseek(int handle, long offset, int whence) {
    /**
	 * Not implemented.
	 */
    return -1;
}

int __close(int handle) {
    /**
	 * Not implemented.
	 */
    return 0;
}

int remove(const char* filename) {
    /**
	 * Not implemented.
	 */
    return 0;
}

size_t __read(int handle, unsigned char * buffer, size_t size) {
    /**
	 * Not implemented.
	 */
    return _LLIO_ERROR;
}

int __open(const char* filename, int mode) {
	/**
	 * Not implemented.
	 */
    return -1;
}

size_t __write(int handle, const unsigned char * buffer, size_t size) {
	/**
	 * Not implemented.
	 */
    return _LLIO_ERROR;
}

#endif
