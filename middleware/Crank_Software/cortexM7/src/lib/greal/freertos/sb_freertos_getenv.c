/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
 * This provides a stub getenv function that does nothing.
 */

#include <stdlib.h>


/**
 * https://pubs.opengroup.org/onlinepubs/009695399/functions/getenv.html
 */
char *
getenv(const char *s1) {
	return NULL;
}
