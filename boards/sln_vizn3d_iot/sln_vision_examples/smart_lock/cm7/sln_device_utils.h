/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _SLN_DEVICE_UTILS_H_
#define _SLN_DEVICE_UTILS_H_

#include "stdbool.h"
#include "ctype.h"

/**
 * @brief checks a string for any characters that do not match any of the [a-Z], [0-9], '-', or '_' characters as these
 * are the only characters that are valid usernames for adding and deleting faces
 *
 *
 * @param str the string to check for special characters
 * @return true contains a special character
 * @return false does not contain a special character
 */
bool hasSpecialCharacters(const char *str);

#endif /* __SLN_DEVICE_UTILS */
