/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>


#if (GR_OS(freertos) && ( !GR_TOOLCHAIN(rowley)) || !GR_TOOLCHAIN(iar))

#include <stdlib.h>
#include <string.h>
char *
strdup(const char *s1) {
	char *cpy;
	int   len;

	if(s1 == NULL) {
		return NULL;
	}

	len = strlen(s1);
	cpy = greal_malloc(len + 1);
	if(cpy == NULL) {
		return NULL;
	}

	memcpy(cpy, s1, len);
	cpy[len] = '\0';

	return cpy;
} 
#endif

