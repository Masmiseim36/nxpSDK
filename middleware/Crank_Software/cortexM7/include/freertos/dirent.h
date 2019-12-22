/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file dirent.h
*
*    Declaration of POSIX directory browsing functions and types for Win32.
*
*    Author:  Kevlin Henney (kevlin@acm.org, kevlin@curbralan.com)
*    History: Created March 1997. Updated June 2003.
*    Rights:  See end of file.
*/

#ifndef GR_DIRENT_H
#define GR_DIRENT_H

#ifdef __cplusplus
extern "C"
{
#endif
 
#include <ff.h>

struct dirent {
    char        d_name[256];
};

DIR            *opendir(const char *);
int             closedir(DIR *);
struct dirent  *readdir(DIR *);
void            rewinddir(DIR *);

#ifdef __cplusplus
}
#endif

#endif /* GR_DIRENT_H */
