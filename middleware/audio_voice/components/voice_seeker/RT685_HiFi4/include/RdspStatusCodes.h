/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RDSP_VOICESEEKERLIGHT_STATUS_CODES_H_
#define RDSP_VOICESEEKERLIGHT_STATUS_CODES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RdspStatus {
    OK = 0,
    GENERAL_ERROR = 1,
    MALLOC_FAIL = 2,
    INVALID_PARAMETERS = 3,
    NOT_ENABLED = 4,
    TRIGGER_START_OVERFLOW = 5,
	RDSP_VOICESEEKER_LICENSE_EXPIRED = -12
} RdspStatus;


#ifdef __cplusplus
}
#endif

#endif // RDSP_VOICESEEKER_STATUS_CODES_CHELSEA_H_


