/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization of this file is not implied by any DeepViewRT license
 * agreements unless explicitly stated.
 */

#ifndef JSON_FLEX_H
#define JSON_FLEX_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char*
json_end(char* dst);

extern char*
json_objOpen_flex(char** buf, size_t* siz, char* dst, const char* name);

extern char*
json_objClose_flex(char** buf, size_t* siz, char* dst);

extern char*
json_arrOpen_flex(char** buf, size_t* siz, char* dst, const char* name);

extern char*
json_arrClose_flex(char** buf, size_t* siz, char* dst);

extern char*
json_str_flex(char**      buf,
              size_t*     siz,
              char*       dst,
              const char* key,
              const char* value);

extern char*
json_int_flex(char** buf, size_t* siz, char* dst, const char* key, int value);

extern char*
json_int64_flex(char**      buf,
                size_t*     siz,
                char*       dst,
                const char* key,
                int64_t     value);

extern char*
json_uint_flex(char**       buf,
               size_t*      siz,
               char*        dst,
               char const*  name,
               unsigned int value);

extern char*
json_uint64_flex(char**      buf,
                 size_t*     siz,
                 char*       dst,
                 char const* name,
                 uint64_t    value);

extern char*
json_double_flex(char**      buf,
                 size_t*     siz,
                 char*       dst,
                 const char* key,
                 double      value);

extern char*
json_null_flex(char** buf, size_t* siz, char* dst, const char* key);

extern char*
json_base64_flex(char**      buf,
                 size_t*     siz,
                 char*       dst,
                 const char* key,
                 const void* data,
                 size_t      len);

#ifdef __cplusplus
}
#endif

#endif /* JSON_FLEX_H */