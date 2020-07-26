/*
 * Copyright 2016, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */

/**
*  This file contains the description for an abstraction layer
*  to standard POSIX malloc/free
*/

#ifndef GR_GREAL_ALLOC_H
#define GR_GREAL_ALLOC_H

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Cover function for standard malloc() API
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/malloc.html
 */
void * greal_malloc(size_t size);

/**
 * Cover function for standard free() API
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/free.html
 */
void greal_free(void *ptr);

/**
 * Cover function for standard realloc() API
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/realloc.html
 */
void * greal_realloc(void *ptr, size_t size);

/**
 * Cover function for standard calloc() API
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/calloc.html
 */
void * greal_calloc(size_t nelem, size_t elsize);

/**
 * Cover function for standard strdup() API
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/strdup.html
 */
char * greal_strdup(const char *s1);

#if defined(__cplusplus)
}
#endif

#endif /* GR_GREAL_ALLOC_H */
