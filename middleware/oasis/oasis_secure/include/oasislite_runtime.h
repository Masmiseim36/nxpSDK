/*
* Copyright 2019 NXP.
* This software is owned or controlled by NXP and may only be used strictly in accordance with the
* license terms that accompany it. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you
* agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
* applicable license terms, then you may not retain, install, activate or otherwise use the software.
*
*/

#ifndef OASIS_LT_RT_H
#define OASIS_LT_RT_H
#include "stdbool.h"
#include "stdint.h"


#define VERSION_MAJOR 4
#define VERSION_MINOR 11


#define INVALID_FACE_ID (0xFFFFUL)


/*these macros are used in OASISLT_init, they can be combined to indicate
 * what functions should be enabled in OASIS LIB.*/
enum {
    OASIS_ENABLE_DET = 1U << 0,
    OASIS_ENABLE_REC = 1U << 1,
    OASIS_ENABLE_EMO = 1U << 2,
    OASIS_ENABLE_LIVENESS = 1U << 3,
    OASIS_ENABLE_MULTI_VIEW = 1U << 4,
    OASIS_ENABLE_BRIGHTNESS_FAIL_CHECK = 1U << 5,
    OASIS_ENABLE_INVALID = 0xFF
};


#define OASIS_REG_MODE_SHIFT (5)
typedef enum {
    OASIS_DET_ONLY = 0,
    OASIS_DET_REC,
    OASIS_DET_EMO,
    OASIS_DET_REC_EMO,
    OASIS_RUN_FLAG_NUM,

    //reg mode can be ored with OASIS_DET_REC,OASIS_DET_REC_EMO flags if needed
    OASIS_REG_MODE = 1 << OASIS_REG_MODE_SHIFT,
    OASIS_INVALID_RUN_FLAG = 0xFF
} OASISRunFlag_t;


/*this enum is used to indicate emotion recognition model mode.
 * 2 means two emotions recognition model:happy and neutral
 * 4 means four emotions recognition model:happy,anger,surprise and neutral
 * 7 means all 7 emotions recognition model: see OASISLTEmoID_t definition.*/
typedef enum {
    OASIS_EMOTION_MODE_2 = 2,
    OASIS_EMOTION_MODE_4 = 4,
    OASIS_EMOTION_MODE_7 = 7,
    OASIS_EMOTION_MODE_INVALID = 0xFF

} OASISLTEmoMode_t;

typedef enum {
    OASIS_EMO_ID_ANGER,
    OASIS_EMO_ID_DISGUST,
    OASIS_EMO_ID_FEAR,
    OASIS_EMO_ID_HAPPY,
    OASIS_EMO_ID_SAD,
    OASIS_EMO_ID_SURPRISED,
    OASIS_EMO_ID_NEUTRAL,
    OASIS_EMO_ID_NUM,
    OASIS_EMO_ID_INVALID = 0xFF

} OASISLTEmoID_t;


typedef enum {
    OASIS_REC_RESULT_KNOWN_FACE,
    OASIS_REC_RESULT_UNKNOWN_FACE,
    OASIS_REC_RESULT_INVALID = 0xFF
} OASISLTRecognizeRes_t;


typedef enum {
    /*these results are used by event OASISLT_EVT_REG_COMPLETE*/
    OASIS_REG_RESULT_OK,
    OASIS_REG_RESULT_DUP,
    OASIS_REG_RESULT_CANCELED,
    OASIS_REG_RESULT_DB_OP_FAILED,
    OASIS_REG_RESULT_INVALID = 0xFF
} OASISLTRegisterRes_t;


typedef enum {
    /*these results are used by event OASISLT_EVT_QUALITY_CHK_COMPLETE*/
    OASIS_QUALITY_RESULT_FACE_OK, /*for internal use only*/
    OASIS_QUALITY_RESULT_FACE_OK_WITHOUT_GLASSES,
    OASIS_QUALITY_RESULT_FACE_OK_WITH_GLASSES,
    OASIS_QUALITY_RESULT_FACE_TOO_SMALL,
    OASIS_QUALITY_RESULT_FACE_SIDE_FACE,
    OASIS_QUALITY_RESULT_FACE_BLUR,
    OASIS_QUALITY_RESULT_FAIL_LIVENESS_IR,
    OASIS_QUALITY_RESULT_FAIL_LIVENESS_RGB,
    OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK,
    OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE,
    OASIS_QUALITY_RESULT_INVALID = 0xFF
} OASISLTFaceQualityRes_t;


typedef enum {
    OASIS_FAR_1_10000,
    OASIS_FAR_1_100000,
    OASIS_FAR_1_1000000,
    OASIS_FAR_NUM,
    OASIS_FAR_INVALID = 0xFF
} OASISLTFar_t;

/*Face recognition Model class:
 * light model means a light calculation,fast speed and a lower accuracy.
 * heavy model means a heavy calculation,lower speed and a higher accuracy.
 * */
typedef enum {
    OASISLT_MODEL_CLASS_LIGHT,
    OASISLT_MODEL_CLASS_HEAVY,
    OASISLT_MODEL_CLASS_NUM
} OASISLTModelClass_t;

typedef enum {
    OASIS_IMG_FORMAT_RGB888, // 3 channels
    OASIS_IMG_FORMAT_BGR888, // 3 channels

    //theses formats are used internal only
    OASIS_IMG_FORMAT_GREY888,//3 channels
    OASIS_IMG_FORMAT_GREY,   // 1 channel
    OASIS_IMG_FORMAT_NUM,
    OASIS_IMG_FORMAT_INVALID = 0xFF
} OASISLTImageFormat_t;

typedef enum {
    OASIS_IMG_TYPE_RGB_SINGLE,  //input only RGB frame
    OASIS_IMG_TYPE_IR_SINGLE,   //input only IR frame
    OASIS_IMG_TYPE_IR_RGB_DUAL, //IR and RGB frames, do face recognition on IR frame
    OASIS_IMG_TYPE_RGB_IR_DUAL, //IR and RGB frames, do face recognition on RGB frame
    OASIS_IMG_TYPE_IR_RGB_3D_TRIPLE,//IR, RGB and 3D frames, do face recognition on IR frame
    OASIS_IMG_TYPE_RGB_IR_3D_TRIPLE,//IR, RGB and 3D frames, do face recognition on RGB frame
    OASIS_IMG_TYPE_NUM,
    OASIS_IMG_TYPE_INVALID = 0xFF
} OASISLTImageType_t;

/*idx of input frames*/
enum {
    OASISLT_INT_FRAME_IDX_RGB,
    OASISLT_INT_FRAME_IDX_IR,
    OASISLT_INT_FRAME_IDX_3D,
    OASISLT_INT_FRAME_IDX_LAST
};


typedef struct FBox_ {
    int rect[4];    // left, top, right, bottom.
    float fld[10];  // 5 laddmark point.
} FBox;

typedef struct {
    short height;
    short width;
    //this int is used for internal purpose, must be set as 0 on init
    int reserved0;
    unsigned char* data;
} ImageFrame_t;


typedef struct {
    FBox* faceBoxIR; //face rect and landmark on IR image
    FBox* faceBoxRGB; //face rect and landmark on RGB image
    uint16_t faceID;//only valid when a face recognized or registered
    OASISLTEmoID_t emoID; //valid only for emo recognition callback
    OASISLTRegisterRes_t regResult; // only valid for registration
    OASISLTRecognizeRes_t recResult;//only valid for face recognition
    OASISLTFaceQualityRes_t qualityResult;//only valid for face quality check event.

#define OASISLT_CB_PARA_RESERVED_INT 16
    int reserved[OASISLT_CB_PARA_RESERVED_INT];//this field is reserved for debugging purpose
} OASISLTCbPara_t;

typedef enum {
    /*indicate the start of face detection, user can update frame data if it is needed.
     * all parameter in callback parameter is invalid.*/
    OASISLT_EVT_DET_START,

    /*The end of face detection.
     *if a face is found, pfaceBox(OASISLTCbPara_t) indicated the rect(left,top,right,bottom point value)
     *info and landmark value of the face.
     *if no face is found,pfaceBox is NULL, following event will not be triggered for current frame.
     *other parameter in callback parameter is invalid */
    OASISLT_EVT_DET_COMPLETE,

    /*Face quality check is done before face recognition*/
    OASISLT_EVT_QUALITY_CHK_START,
    OASISLT_EVT_QUALITY_CHK_COMPLETE,

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
    /*when registration start, for each valid frame is handled,this event will be triggered and indicate
     * registration process is going forward a little.
     * */
    OASISLT_EVT_REG_IN_PROGRESS,
    OASISLT_EVT_REG_COMPLETE,
    OASISLT_EVT_NUM

} OASISLTEvt_t;


/*this callback definition is used for calling of OASISLT_run
 *for single frame type, only frames[0] is valid and it points to the single input frame
 *for dual frame type, frames[0] point to rgb frame input while frames[1] point to IR frame input
 *for triple frame type,frames[0] point to rgb frame input while frames[1] point to IR frame input, frame[2] point to 3D frame input
 **/
typedef void (*OASISLTEvtCb)(ImageFrame_t* frames[OASISLT_INT_FRAME_IDX_LAST], OASISLTEvt_t evt, OASISLTCbPara_t* para, void* user_data);


/*OASIS LITE would use this fun to  get all registered face array.
 * start_idx[input] indicate the start index of the face record OASIS LIB is trying to get.
 * for example, if there are N face records in database, their indexes are 0 to N-1
 * face_id[output]: buffer used to save face ID array returned. it should align with records in "faces".
 * faces[output]: buffer used to save face records data,this buffer size is *face_num*OASISLT_getFaceItemSize()
 * face_num[input/output]: as input, it indicates the number of unit in "face_id" and "faces"
 * as output, it indicates how many faces returned actually.
 * specially, as input when *frame_num is 0, actual face record number should be set in *face_num before return.
 * in this case, "face_id" and 'faces' can be NULL.
 * return 0 if call succeed, otherwise failed.
 * in order to enumerate all registered faces in the system, this function may be called multi times
 *  */
typedef int (*GetRegisteredFaces)(int start_idx, uint16_t* face_id, void* faces, unsigned int* face_num);

/*in register mode, OASIS LITE would call this function to add new/delete/update face
 *face_data: pointer to the faces' data, the data length of each face can get by OASISLT_getFaceItemSize()
 *face_id: output parameter,this id identified this face data uniquely, it should be generated by outside DB.
 *         it can be used in following delete/update face operation
 *return 0 if succeed; otherwise failed*/
typedef int (*FaceOperationAdd)(uint16_t* face_id, void* face_data);


typedef int (*FaceOperationUpdate)(uint16_t face_id, void* face_data);

/*Using for print out ANSI string in self test API*/
typedef void (*StringPrint)(const char* str);

/*Used to get current system millisecond number*/
typedef uint32_t (*GetSystemCurrentMS)(void);

/*Used to dynamically adjust face brightness
  * frame_idx: which frame is need to be adjusted on, OASISLT_INT_FRAME_IDX_RGB or OASISLT_INT_FRAME_IDX_IR ?
  * direction: 1: up 0: down*/
typedef void (*FaceBrightnessAdjust)(uint8_t frame_idx, uint8_t direction);

typedef struct {
    OASISLTEvtCb EvtCb;
    GetRegisteredFaces GetFaces;
    FaceOperationAdd AddFace;
    FaceOperationUpdate UpdateFace;
    FaceBrightnessAdjust AdjustBrightness;

    //internel debugging use only
    void* reserved;

} InfCallbacks_t;

typedef struct {
    //max input image height, width and channel, min_face: minimum face can be detected
    int height;
    int width;

    //only valid for RGB images; for IR image, always GREY888 format
    OASISLTImageFormat_t img_format;
    OASISLTImageType_t img_type;

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

    /*false accept rate*/
    OASISLTFar_t far;

    /*model class */
    OASISLTModelClass_t mod_class;

} OASISLTInitPara_t;



#ifdef __cplusplus
extern "C" {
#endif
/* Initialize OASIS LITE lib, it should be called before any other APIs.
 * para: initialzing parameter. refer to OASISLTInitPara_t for detail information.
 * */
int OASISLT_init(OASISLTInitPara_t* para);
int OASISLT_uninit(void);

/*return version information of OASIS LITE library*/
void OASISLT_getVersion(int* major, int* minor);

/* Run face detection/recognition/emotion recognition/registration according given run flags.
 * event callback function will be called when each of job complete.
 * min_face: minimum face size for current frame face detection.
 * userData: user data for callback functions.
 * */
int OASISLT_run(ImageFrame_t* frame, uint8_t flag, int minFace, void* userData);
int OASISLT_run2D(ImageFrame_t* irFrame, ImageFrame_t* rgbFrame, uint8_t flag, int minFace, void* userData);

/*this API can be used to replace OASISLT_run and OASISLT_run2D API with a more flexiable input parameters.
 * user can input RGB/IR/3D frame with different combinations according image types in intializing.
 * */
int OASISLT_run_extend(ImageFrame_t* frames[OASISLT_INT_FRAME_IDX_LAST], uint8_t flag, int minFace, void* userData);


enum {
    OASISLT_RUN_IDENTIFY_RESULT_OK,
    OASISLT_RUN_IDENTIFY_RESULT_NO_FACE_ON_INPUT,
    OASISLT_RUN_IDENTIFY_RESULT_NO_FACE_ON_TARGET,
    OASISLT_RUN_IDENTIFY_RESULT_PARAM_ERROR,
    OASISLT_RUN_IDENTIFY_RESULT_LIB_UNINIT,
    OASISLT_RUN_IDENTIFY_RESULT_FAIL_UNKNOW,

};
/*Used to compare and get similarity of faces in input image and target image.
 * OASIS_IMG_FORMAT_RGB888 and OASIS_IMG_FORMAT_BGR888 are supported*/
int OASISLT_run_identification(ImageFrame_t* input, ImageFrame_t* target, float* sim);

/*return minimum free memory size since OASISLT initialization.
 **/
unsigned int OASISHeapGetMinimumEverFreeHeapSize(void);

/*return the size of each face feature. unit: byte*/
uint32_t OASISLT_getFaceItemSize(void);


/*OASIS LITE runtime library self test. Internal use only*/
int OASISLT_selftest(void* mempool,int size,StringPrint print,GetSystemCurrentMS getMS);

#ifdef __cplusplus
}
#endif

#endif
