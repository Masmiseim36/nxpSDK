/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OASIS_LT_RT_H
#define OASIS_LT_RT_H
#include "stdbool.h"
#include "stdint.h"

#define VERSION_MAJOR 3
#define VERSION_MINOR 5

#define INVALID_FACE_ID (0xFFFFUL)

/*these macros are used in OASISLT_init, they can be combined to indicate
 * what functions should be enabled in OASIS LIB.*/
enum{
    OASIS_ENABLE_DET = 1U<<0,
    OASIS_ENABLE_REC = 1U<<1,
    OASIS_ENABLE_EMO = 1U<<2,
    OASIS_ENABLE_LIVENESS = 1U<<3,
    OASIS_ENABLE_INVALID = 0xFF
};

#define OASIS_REG_MODE_SHIFT (5)
typedef enum
{
    OASIS_DET_ONLY = 0,
    OASIS_DET_REC,
    OASIS_DET_EMO,
    OASIS_DET_REC_EMO,
    OASIS_RUN_FLAG_NUM,

    //reg mode can be ored with OASIS_DET_REC,OASIS_DET_REC_EMO flags if needed
    OASIS_REG_MODE = 1<<OASIS_REG_MODE_SHIFT,
    OASIS_INVALID_RUN_FLAG = 0xFF
}OASISRunFlag_t;

/*this enum is used to indicate emotion recognition model mode.
 * 2 means two emotions recognition model:happy and neutral
 * 4 means four emotions recognition model:happy,anger,surprise and neutral
 * 7 means all 7 emotions recognition model: see OASISLTEmoID_t definition.*/
typedef enum
{
    OASIS_EMOTION_MODE_2 = 2,
    OASIS_EMOTION_MODE_4 = 4,
    OASIS_EMOTION_MODE_7 = 7,
    OASIS_EMOTION_MODE_INVALID = 0xFF

}OASISLTEmoMode_t;

typedef enum{
    OASIS_EMO_ID_ANGER,
    OASIS_EMO_ID_DISGUST,
    OASIS_EMO_ID_FEAR,
    OASIS_EMO_ID_HAPPY,
    OASIS_EMO_ID_SAD,
    OASIS_EMO_ID_SURPRISED,
    OASIS_EMO_ID_NEUTRAL,
    OASIS_EMO_ID_NUM,
    OASIS_EMO_ID_INVALID = 0xFF

}OASISLTEmoID_t;


typedef enum{
    OASIS_FAR_1_10000,
    OASIS_FAR_1_100000,
    OASIS_FAR_1_1000000,
    OASIS_FAR_NUM,
    OASIS_FAR_INVALID = 0xFF
}OASISLTFar_t;

typedef enum{
    OASIS_IMG_FORMAT_RGB888, //3 channels
    OASIS_IMG_FORMAT_BGR888, //3 channels
    OASIS_IMG_FORMAT_GREY,   //1 channel
    OASIS_IMG_FORMAT_NUM,
    OASIS_IMG_FORMAT_INVALID=0xFF
}OASISLTImageFormat_t;


typedef struct FBox_{
    int rect[4];    // left, top, right, bottom.
    float fld[10];  // 5 laddmark point.
} FBox;

typedef struct{
    short height;
    short width;
    //uint8_t channel;

    //this int is used for internal purpose, must be set as 0 on init
    int reserved0;
    unsigned char* data;
}ImageFrame_t;

typedef struct{
    FBox* pfaceBox; //
    OASISLTEmoID_t emoID; //valid only for emo recognition callback
    //uint8_t emo_conf;//valid only for emo recognition 0 -100
    uint16_t faceID;//valid only for rec callback
    //OASISLTUnqulifiedReason_t reason;//if recognition/registration failed, it indicate the reason
    //uint8_t sim; //valid for rec callback 0 -100
    //uint8_t liveness;//valid for live check 0:fake,1:real

}OASISLTCbPara_t;

typedef enum
{
    /*indicate the start of face detection, user can update frame data if it is needed.
     * all parameter in callback parameter is invalid.*/
    OASISLT_EVT_DET_START,

    /*The end of face detection.
     *if a face is found, pfaceBox(OASISLTCbPara_t) indicated the rect(left,top,right,bottom point value)
     *info and landmark value of the face.
     *if no face is found,pfaceBox is NULL, following event will not be triggered for current frame.
     *other parameter in callback parameter is invalid */
    OASISLT_EVT_DET_COMPLETE,

    /*Start of face recognition*/
    OASISLT_EVT_REC_START,

    /*The end of face recogniton.
     * when face feature in current frame is gotten, GetRegisteredFaces callback will be called to get all
     * faces feature registered and OASIS lib will try to search this face in registered faces, if this face
     * is matched, a valid face ID will be set in callback parameter faceID and corresponding simularity(indicate
     * how confidence for the match) also will be set.
     * if no face match, a invalid(INVALID_FACE_ID) will be set.*/
    OASISLT_EVT_REC_COMPLETE,

    /*start of emotion recognition*/
    OASISLT_EVT_EMO_REC_START,

    /*End of emotion recognition, emoID indicate which emotion current face is.*/
    OASISLT_EVT_EMO_REC_COMPLETE,

    /*if user set a registration flag in a call of OASISLT_run and a face is detected, this two events will be notified
     * for auto registration mode, only new face(not recognized) is added(call AddNewFace callback function)
     * for manu registration mode, face will be added forcely.
     * for both cases, face ID of new added face will be set in callback function */
    OASISLT_EVT_REG_START,
    OASISLT_EVT_REG_COMPLETE,
    OASISLT_EVT_NUM

}OASISLTEvt_t;

typedef void (*OASISLTEvtCb)(ImageFrame_t* frame,OASISLTEvt_t evt,OASISLTCbPara_t* para,void* user_data);

/*OASIS LITE would use this fun to  get all registered face array.
 * start_idx indicate the start position of the face OASIS LIB is trying to get
 * faces: buffer used to save face data,this buffer size is *face_num*OASISLT_getFaceItemSize()
 * face_num indicate faces item size, as output it also indicate how many faces returned actually
 * return 0 if call succeed, otherwise failed.
 * in order to enumerate all registered faces in the system, this function may be called multi times
 *  */
typedef int (*GetRegisteredFaces)(int start_idx,void* faces,unsigned int* face_num);

/*in register mode, OASIS LITE would call this function to add new/delete/update face
 *face_data: pointer to the faces' data, the data length of each face can get by OASISLT_getFaceItemSize()
 *face_id: this id identified this face data uniquely, it can be used in following delete/update face operation
 *return 0 if succeed; otherwise failed*/
typedef int (*FaceOperation)(uint16_t face_id,void* face_data);

/*Critical enter/exit function*/
typedef void (*CriticalSecEnterExitFun)(void);
typedef int (*LogOutputFun)(const char* format);

typedef struct
{
    OASISLTEvtCb EvtCb;
    GetRegisteredFaces GetFaces;
    FaceOperation AddFace;
    FaceOperation UpdateFace;
    LogOutputFun Print;

}InfCallbacks_t;

typedef struct
{
    //max input image height, width and channel, min_face: minimum face can be detected
    int height;
    int width;
    OASISLTImageFormat_t img_format;

    //min_face should not smaller than 40
    int min_face;

    /*memory pool pointer, this memory pool should only be used by OASIS LIB*/
    char* mem_pool;

    /*memory pool size*/
    int size;

    /*output parameter,indicate authenticated or not*/
    int auth;

    /*callback functions provided by caller*/
    InfCallbacks_t cbs;

    /*what functions should be enabled in OASIS LIB*/
    uint8_t enable_flags;

    /*only valid when OASIS_ENABLE_EMO is activated*/
    OASISLTEmoMode_t emo_mode;

    /*minimum False accept rate */
    OASISLTFar_t far;

}OASISLTInitPara_t;

#ifdef __cplusplus
extern "C" {
#endif
int OASISLT_init(OASISLTInitPara_t* para);
int OASISLT_uninit();

/*Run face detection/recognition/emotion recognition/registration according given run flags.
 * event callback function will be called when each of job complete.
 * min_face: minimum face size for current frame face detection.
 * userData: user data for callback functions.
 * */
int OASISLT_run(ImageFrame_t *frame, uint8_t flag, int minFace, void* userData);
unsigned int OASISHeapGetMinimumEverFreeHeapSize( void );
uint32_t OASISLT_getFaceItemSize();

#ifdef __cplusplus
}
#endif

#endif /* OASIS_LT_RT_H */
