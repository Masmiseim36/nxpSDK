/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

//This provides a stub uname* implementation that does nothing

#include <gre/sdk/greal.h>
#include <stdio.h>
#include <string.h>

int
greal_uname(greal_utsname_t *un) {
	memset(un, 0, sizeof(*un));

	snprintf(un->sysname, sizeof(un->sysname), "freertos");

	return 0;
}
