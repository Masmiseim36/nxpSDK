/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
 * This provides a stub getcwd function that does nothing
 */

#include <stdlib.h>
#include <gre/sdk/greal.h>


/**
 * https://pubs.opengroup.org/onlinepubs/009695399/functions/getcwd.html
 */
char *
getcwd(char *buf, int size) {
	*buf = '\0';
	return buf;
}
