/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_UPDATE_H_
#define _SLN_UPDATE_H_

#include "stdbool.h"

/**
 * @brief Check if there is an update request pending. This is project/platform related.
 *
 * @return Return true if there is a request waiting, false otherwise.
 */
bool SLN_CheckForUpdate(void);

#endif /* _SLN_UPDATE_H_ */
