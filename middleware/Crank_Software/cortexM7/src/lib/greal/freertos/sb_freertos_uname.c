/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>
#include <stdio.h>
#include <string.h>

/**
 * The uname function is used by the engine to report what system the engine is running on
 */
int
greal_uname(greal_utsname_t *un) {
	memset(un, 0, sizeof(*un));
	snprintf(un->sysname, sizeof(un->sysname), "freertos");
	return 0;
}
