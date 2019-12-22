/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

//This provides a stub dl* implementation that does nothing

#include <gre/sdk/greal.h>
#include <stdlib.h>

/**
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlopen.html
 */
void *
dlopen(const char *_file, int _mode) {
	return NULL;
}

/**
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlsym.html
 */
void *
dlsym(void *_handle, const char *_name) {
	return NULL;
}

/**
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlclose.html
 */
int
dlclose(void *_handle) {
	return 0;
}

/**
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlerror.html
 */
char *
dlerror(void) {
	return NULL;
}
