/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the license terms that accompany it. By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*
 * @brief facerec-specific event descriptor declarations.
 */

#ifndef _HAL_EVENT_DESCRIPTOR_FACE_REC_H_
#define _HAL_EVENT_DESCRIPTOR_FACE_REC_H_

#include "hal_event_descriptor_common.h"
#include "hal_sln_facedb.h"

typedef enum _event_face_rec_id
{
    kEventFaceRecID_AddUser = kEventType_FaceRec,
    kEventFaceRecID_AddUserRemote,
    kEventFaceRecID_DelUser,
    kEventFaceRecID_DelUserAll,
    kEventFaceRecID_GetUserList,
    kEventFaceRecID_GetUserCount,

    kEventFaceRecID_RenameUser,
    kEventFaceRecID_UpdateUserInfo,

    kEventFaceRecID_SaveUserList,
    kEventFaceRecID_COUNT
} event_face_rec_id_t;

typedef struct _face_user_info
{
    uint32_t id;
    char name[FACE_NAME_MAX_LEN + 1];
    uint8_t isSaved;
} face_user_info_t;

typedef struct _user_info_event
{
    face_user_info_t *userInfo;
    uint16_t count;
} user_info_event_t;

typedef struct _wuart_event
{
    int length;
    union
    {
        int status;
        void *data;
    };
} wuart_event_t;

typedef struct _add_face_event
{
    char name[FACE_NAME_MAX_LEN];
    bool hasName;
} add_face_event_t;

typedef struct _del_face_event
{
    uint16_t id;
    char name[FACE_NAME_MAX_LEN];
    bool hasName;
    bool hasID;
} del_face_event_t;

typedef struct _update_user_event_t
{
    uint32_t id;
    char name[FACE_NAME_MAX_LEN + 1];
} update_user_event_t;

typedef struct _remote_reg_data_t
{
    char name[FACE_NAME_MAX_LEN + 1];
    uint8_t facedata[0];
} remote_reg_data_t;

typedef struct _remote_reg_result_t
{
    uint8_t result;
    char *name;
} remote_reg_result_t;

typedef struct _remote_reg_event_t
{
    uint32_t isReRegister;
    uint32_t dataLen;
    remote_reg_data_t *regData;
} remote_reg_event_t;

typedef struct _event_face_rec
{
    event_base_t eventBase;

    union
    {
        void *data;
        add_face_event_t addFace;
        del_face_event_t delFace;
        update_user_event_t updateFace;
        remote_reg_event_t remoteReg;
        wuart_event_t wuart;
    };
} event_face_rec_t;

#endif /* _HAL_EVENT_DESCRIPTOR_FACE_REC_H_ */
