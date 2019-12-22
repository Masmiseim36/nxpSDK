/*
 * Copyright 2016, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */

/**
 * This is a series of cover functions to allow the traceability and replacement 
 * of the standard C library memory allocation routines.
 */
#include <gre/sdk/greal.h>

#include <stdlib.h>
#include <string.h>

//RJD:  Any of the implementations below can be overridden by placing that define in a header file that will get included in greal.h.  This
// allows for a particular greal implementation to cover the functions and then place the defines in their own implementation header files and 
// overide the functionality.  

#ifndef _GREAL_MALLOC_
void *
greal_malloc(size_t size) {
	return malloc(size);
}
#endif

#ifndef _GREAL_FREE_
void 
greal_free(void *ptr) {
	free(ptr);
}
#endif

#ifndef _GREAL_REALLOC_
void *
greal_realloc(void *ptr, size_t size) {
	return realloc(ptr, size);
}
#endif

#ifndef _GREAL_CALLOC_
void *
greal_calloc(size_t nelem, size_t elsize) {
	return calloc(nelem, elsize);
}
#endif

#ifndef _GREAL_STRDUP_
char *
greal_strdup(const char *s1) {
	return strdup(s1);
}
#endif