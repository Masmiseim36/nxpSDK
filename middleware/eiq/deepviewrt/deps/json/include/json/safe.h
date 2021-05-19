/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization of this file is not implied by any DeepViewRT license
 * agreements unless explicitly stated.
 */

#ifndef JSON_SAFE_H
#define JSON_SAFE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char*
json_end(char* dst);

extern char*
json_objOpen_safe(char* buf, size_t siz, char* dst, const char* name);

extern char*
json_objClose_safe(char* buf, size_t siz, char* dst);

extern char*
json_arrOpen_safe(char* buf, size_t siz, char* dst, const char* name);

extern char*
json_arrClose_safe(char* buf, size_t siz, char* dst);

extern char*
json_str_safe(char*       buf,
              size_t      siz,
              char*       dst,
              const char* key,
              const char* value);

extern char*
json_int_safe(char* buf, size_t siz, char* dst, const char* key, int value);

#ifdef __cplusplus
}
#endif

#endif /* JSON_SAFE_H */