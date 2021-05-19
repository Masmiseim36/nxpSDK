/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization of this file is not implied by any DeepViewRT license
 * agreements unless explicitly stated.
 */

#include "json/flex.h"
#include "json/writer.h"

#include "flatcc/portable/pbase64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
json_objOpen_flex(char** buf, size_t* siz, char* dst, const char* name)
{
    size_t pos = dst - *buf;
    size_t len = name ? strlen(name) + 6 : 2;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_objOpen(dst, name);
}

char*
json_objClose_flex(char** buf, size_t* siz, char* dst)
{
    size_t pos = dst - *buf;
    size_t len = 2;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_objClose(dst);
}

char*
json_arrOpen_flex(char** buf, size_t* siz, char* dst, const char* name)
{
    size_t pos = dst - *buf;
    size_t len = name ? strlen(name) + 6 : 2;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_arrOpen(dst, name);
}

char*
json_arrClose_flex(char** buf, size_t* siz, char* dst)
{
    size_t pos = dst - *buf;
    size_t len = 2;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_arrClose(dst);
}

char*
json_str_flex(char**      buf,
              size_t*     siz,
              char*       dst,
              const char* key,
              const char* value)
{
    size_t pos = dst - *buf;
    size_t len = (key ? strlen(key) + 5 : 0) + strlen(value) + 4;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_str(dst, key, value);
}

char*
json_int_flex(char** buf, size_t* siz, char* dst, const char* key, int value)
{
    size_t pos = dst - *buf;
    size_t len = (key ? strlen(key) + 5 : 0) + 20 + 4;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_int(dst, key, value);
}

char*
json_int64_flex(char**      buf,
                size_t*     siz,
                char*       dst,
                const char* key,
                int64_t     value)
{
    size_t pos = dst - *buf;
    size_t len = (key ? strlen(key) + 5 : 0) + 20 + 6;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_int64(dst, key, value);
}

extern char*
json_uint_flex(char**       buf,
               size_t*      siz,
               char*        dst,
               char const*  key,
               unsigned int value)
{
    size_t pos = dst - *buf;
    size_t len = (key ? strlen(key) + 5 : 0) + 20 + 4;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_uint(dst, key, value);
}

extern char*
json_uint64_flex(char**      buf,
                 size_t*     siz,
                 char*       dst,
                 char const* key,
                 uint64_t    value)
{
    size_t pos = dst - *buf;
    size_t len = (key ? strlen(key) + 5 : 0) + 20 + 6;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_uint64(dst, key, value);
}

char*
json_double_flex(char**      buf,
                 size_t*     siz,
                 char*       dst,
                 const char* key,
                 double      value)
{
    size_t pos = dst - *buf;
    /*
     * FIXME: Should inspect the value to figure out the string lenght but
     * currently we assume the worst case 1.8x10^308.
     */
    size_t len = (key ? strlen(key) + 5 : 0) + 310 + 6;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_double(dst, key, value);
}

char*
json_null_flex(char** buf, size_t* siz, char* dst, const char* key)
{
    size_t pos = dst - *buf;
    size_t len = (key ? strlen(key) + 5 : 0) + 5;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    return json_null(dst, key);
}

char*
json_base64_flex(char**      buf,
                 size_t*     siz,
                 char*       dst,
                 const char* key,
                 const void* data,
                 size_t      n_data)
{
    size_t pos     = dst - *buf;
    size_t b64_len = base64_encoded_size(n_data, 0);
    size_t len     = (key ? strlen(key) + 5 : 0) + b64_len + 5;

    if (pos + len > *siz) {
        size_t nsiz = (*siz + len) * 2;
        char*  ptr  = realloc(*buf, nsiz);
        if (!ptr) { return NULL; }
        *buf = ptr;
        *siz = nsiz;
        dst  = *buf + pos;
    }

    dst = json_strname(dst, key);

    if (base64_encode((uint8_t*) dst, data, &b64_len, &n_data, 0)) {
        return NULL;
    }

    dst += b64_len;
    *(dst++) = '=';
    *(dst++) = '\"';
    *(dst++) = ',';

    return dst;
}
