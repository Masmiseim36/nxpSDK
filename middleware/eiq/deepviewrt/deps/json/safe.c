/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization of this file is not implied by any DeepViewRT license
 * agreements unless explicitly stated.
 */

#include "json/safe.h"
#include "json/writer.h"

#include <stdlib.h>
#include <string.h>

char*
json_objOpen_safe(char* buf, size_t siz, char* dst, const char* name)
{
    size_t pos = dst - buf;
    size_t    len = name ? strlen(name) + 6 : 2;
    if (pos + len > siz) { return NULL; }
    return json_objOpen(dst, name);
}

char*
json_objClose_safe(char* buf, size_t siz, char* dst)
{
    size_t pos = dst - buf;
    size_t    len = 2;
    if (pos + len > siz) { return NULL; }
    return json_objClose(dst);
}

char*
json_arrOpen_safe(char* buf, size_t siz, char* dst, const char* name)
{
    size_t pos = dst - buf;
    size_t    len = name ? strlen(name) + 6 : 2;
    if (pos + len > siz) { return NULL; }
    return json_arrOpen(dst, name);
}

char*
json_arrClose_safe(char* buf, size_t siz, char* dst)
{
    size_t pos = dst - buf;
    size_t    len = 2;
    if (pos + len > siz) { return NULL; }
    return json_arrClose(dst);
}

char*
json_str_safe(char*       buf,
              size_t      siz,
              char*       dst,
              const char* key,
              const char* value)
{
    size_t pos = dst - buf;
    size_t    len = (key ? strlen(key) + 5 : 0) + strlen(value) + 4;
    if (pos + len > siz) { return NULL; }
    return json_str(dst, key, value);
}

char*
json_int_safe(char* buf, size_t siz, char* dst, const char* key, int value)
{
    size_t pos = dst - buf;
    size_t    len = (key ? strlen(key) + 5 : 0) + 20 + 4;
    if (pos + len > siz) { return NULL; }
    return json_int(dst, key, value);
}
