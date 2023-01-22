/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
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
	RDSP_VOICESEEKER_LICENSE_EXPIRED = -12
} RdspStatus;


#ifdef __cplusplus
}
#endif

#endif // RDSP_VOICESEEKER_STATUS_CODES_CHELSEA_H_


