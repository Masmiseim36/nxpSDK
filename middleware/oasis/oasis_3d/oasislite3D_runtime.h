/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OASIS_LT_3D_RT_H
#define OASIS_LT_3D_RT_H
#include "stdbool.h"
#include "stdint.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 27
/*this version number only used for hot fix on frozen release or branch*/
#define VERSION_HOTFIX 1

#define INVALID_FACE_ID (0xFFFFUL)

/*these macros are used in OASISLT_init, they can be combined to indicate
 * what functions should be enabled in OASIS LIB.*/
enum
{
    /* To control liveness is enable or not */
    OASIS_ENABLE_LIVENESS = 1U << 0,
    /* This flag is used to enable/disable multi-view check in face registration */
    OASIS_ENABLE_MULTI_VIEW = 1U << 1,
    /* This flag used for brightness check on face recognitions */
    OASIS_ENABLE_FACE_REC_BRIGHTNESS_CHECK = 1U << 2,
    /* This flag is only valid for library with mask face support */
    OASIS_ENABLE_MASK_FACE_REC = 1U << 3,
    /* This flag decides whether face feature smart learning is enabled or not */
    OASIS_ENABLE_FACE_FEA_SMART_LEARNING = 1U << 4,
    OASIS_ENABLE_INVALID                 = 0xFF
};

typedef enum
{
    OASISLT_OK = 0,
    OASIS_INIT_INVALID_PARAMETERS,
    OASIS_INIT_INVALID_MODEL_CLASS,
    OASIS_INIT_ALREADY_INIT,
    OASIS_INIT_INVALID_FAR,
    OASIS_INIT_INVALID_CB, // 5
    OASIS_INIT_MEMORYPOOL_SMALL,
    OASIS_INIT_INVALID_MEMORYPOOL,
    OASIS_INIT_INVALID_IMAGE_MIN_DIM,
    OASIS_INIT_INVALID_MASK_BUF,
    OASIS_INIT_INVALID_IMG_TYPE_FOR_MASK_FACE, // 10
    OASIS_INIT_MASK_REC_NOTSUPORTED,
    OASIS_INIT_INVALID_IMAGE_TYPE,
    OASIS_INIT_INSTANCE_NUM_OVERFLOW,
    OASIS_INIT_AUTHENTICATION_FAIL,
    OASIS_INIT_MEM_ALLOCATE_FAIL, // 15
    OASISLT_SNAPSHOT_INVALID_INPUT_PARAMETERS,
    OASISLT_SNAPSHOT_LIB_UNINIT,
    OASISLT_SNAPSHOT_INVALID_FRAME_NUM,
    OASISLT_SNAPSHOT_IMG_TYPE_NOT_SUPPORT,
    OASISLT_SNAPSHOT_RESIZE_FAILED, // 20
    OASIS_INIT_INVALID_FASTMEMORYBUFFER,

} OASISLTResult_t;

typedef enum
{
    OASIS_DET_ONLY = 0,
    OASIS_DET_WITH_QUALITY_CHECK,
    OASIS_DET_REC,
    OASIS_DET_REC_REG,        // face registration
    OASIS_DET_REC_DEREG,      // face deregistration
    OASIS_DET_REC_REG_REMOTE, // registration with pictures
    OASIS_RUN_FLAG_NUM
} OASISRunFlag_t;

typedef enum
{
    OASIS_REC_RESULT_KNOWN_FACE,
    OASIS_REC_RESULT_UNKNOWN_FACE,
    OASIS_REC_RESULT_INVALID = 0xFF
} OASISLTRecognizeRes_t;

typedef enum
{
    /*these results are used by event OASISLT_EVT_REG_COMPLETE*/
    OASIS_REG_RESULT_OK,
    OASIS_REG_RESULT_DUP,
    OASIS_REG_RESULT_CANCELED,
    OASIS_REG_RESULT_DB_OP_FAILED,
    OASIS_REG_RESULT_INVALID = 0xFF
} OASISLTRegisterRes_t;

typedef enum
{
    /*these results are used by event OASISLT_EVT_DEREG_COMPLETE*/
    OASIS_DEREG_RESULT_OK,
    OASIS_DEREG_RESULT_CANCELED,
    OASIS_DEREG_RESULT_DB_OP_FAILED,
    OASIS_DEREG_RESULT_INVALID = 0xFF
} OASISLTDeregisterRes_t;

typedef enum
{
    /*these results are used by event OASISLT_EVT_QUALITY_CHK_COMPLETE*/
    OASIS_QUALITY_RESULT_FACE_OK,
    OASIS_QUALITY_RESULT_FACE_TOO_SMALL,
    OASIS_QUALITY_RESULT_FACE_SIDE_FACE,
    OASIS_QUALITY_RESULT_FACE_BLUR,
    /*Following 2 values are only valid for RGB+IR dual camera module case*/
    OASIS_QUALITY_RESULT_IR_FAKE,
    OASIS_QUALITY_RESULT_RGB_FAKE, // 5
    /*Following 3 values are only valid for 3D camera module case*/
    OASIS_QUALITY_RESULT_2D_FAKE,
    OASIS_QUALITY_RESULT_3D_FAKE,
    OASIS_QUALITY_RESULT_DEPTH_INVALID,
    OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK,
    OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE,
    OASIS_QUALITY_RESULT_INVALID = 0xFF
} OASISLTFaceQualityRes_t;

typedef enum
{
    OASIS_GLASSES_CHECK_RESULT_FACE_WITHOUT_GLASSES,
    OASIS_GLASSES_CHECK_RESULT_FACE_WITH_GLASSES,
    OASIS_GLASSES_CHECK_RESULT_INVALID = 0xFF
} OASISLTFaceGlassesCheckRes_t;

typedef enum
{
    OASIS_MASK_CHECK_RESULT_FACE_WITHOUT_MASK,
    OASIS_MASK_CHECK_RESULT_FACE_WITH_MASK,
    OASIS_MASK_CHECK_RESULT_INVALID = 0xFF
} OASISLTFaceMaskCheckRes_t;

typedef enum
{
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
typedef enum
{
    OASISLT_MODEL_CLASS_LIGHT,
    OASISLT_MODEL_CLASS_HEAVY,
    OASISLT_MODEL_CLASS_NUM
} OASISLTModelClass_t;

typedef enum
{
    OASIS_IMG_FORMAT_RGB888, // 3 channels
    OASIS_IMG_FORMAT_BGR888, // 3 channels

    // theses formats are used internal only
    OASIS_IMG_FORMAT_GREY888, // 3 channels
    OASIS_IMG_FORMAT_GREY8,   // 1 channel
    OASIS_IMG_FORMAT_NUM,
    OASIS_IMG_FORMAT_INVALID = 0xFF
} OASISLTImageFormat_t;

typedef enum
{
    OASIS_IMG_TYPE_RGB_SINGLE,       // input only RGB frame
    OASIS_IMG_TYPE_IR_SINGLE,        // input only IR frame
    OASIS_IMG_TYPE_IR_RGB_DUAL,      // IR and RGB frames, do face recognition on IR frame
    OASIS_IMG_TYPE_RGB_IR_DUAL,      // IR and RGB frames, do face recognition on RGB frame
    OASIS_IMG_TYPE_IR_RGB_3D_TRIPLE, // IR, RGB and 3D frames, do face recognition on IR frame
    OASIS_IMG_TYPE_RGB_IR_3D_TRIPLE, // IR, RGB and 3D frames, do face recognition on RGB frame
    OASIS_IMG_TYPE_NUM,
    OASIS_IMG_TYPE_INVALID = 0xFF
} OASISLTImageType_t;

/*idx of input frames*/
enum
{
    OASISLT_INT_FRAME_IDX_RGB,
    OASISLT_INT_FRAME_IDX_IR,
    OASISLT_INT_FRAME_IDX_3D,
    OASISLT_INT_FRAME_IDX_LAST
};

/*Landmark index*/
enum
{
    OASISLT_LM_LEFT_EYE_X,
    OASISLT_LM_RIGHT_EYE_X,
    OASISLT_LM_NOSE_X,
    OASISLT_LM_MOUTH_LEFT_CORNER_X,
    OASISLT_LM_MOUTH_RIGHT_CORNER_X,
    OASISLT_LM_LEFT_EYE_Y, /*5*/
    OASISLT_LM_RIGHT_EYE_Y,
    OASISLT_LM_NOSE_Y,
    OASISLT_LM_MOUTH_LEFT_CORNER_Y,
    OASISLT_LM_MOUTH_RIGHT_CORNER_Y,
    OASISLT_LM_IDX_NUM
};

typedef struct _FaceBox
{
    int rect[4];                   // left, top, right, bottom.
    float fld[OASISLT_LM_IDX_NUM]; // 5 landmark point.
} FaceBox_t;

typedef struct
{
    short height;
    short width;
    OASISLTImageFormat_t fmt; // indicate the format of this frame
    unsigned char *data;
} ImageFrame_t;

typedef struct
{
    FaceBox_t *faceBoxIR;                            // face rect and landmark on IR image
    FaceBox_t *faceBoxRGB;                           // face rect and landmark on RGB image
    uint16_t faceID;                            // only valid when a face recognized or registered
    OASISLTRegisterRes_t regResult;             // only valid for registration
    OASISLTDeregisterRes_t deregResult;         // only valid for deregistration
    OASISLTRecognizeRes_t recResult;            // only valid for face recognition
    OASISLTFaceQualityRes_t qualityResult;      // only valid for face quality check event.
    OASISLTFaceMaskCheckRes_t maskResult;       // only valid for face mask check event.
    OASISLTFaceGlassesCheckRes_t glassesResult; // only valid for face glasses check event.
#define OASISLT_CB_PARA_RESERVED_INT 16
    int reserved[OASISLT_CB_PARA_RESERVED_INT]; // this field is reserved for debugging purpose
} OASISLTCbPara_t;

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

    /*Face quality check is done before face recognition*/
    OASISLT_EVT_QUALITY_CHK_START,
    OASISLT_EVT_QUALITY_CHK_COMPLETE,

    /*Start of face recognition*/
    OASISLT_EVT_REC_START,

    /*The end of face recognition.
     * when face feature in current frame is gotten, GetRegisteredFaces callback will be called to get all
     * faces feature registered and OASIS lib will try to search this face in registered faces, if this face
     * is matched, a valid face ID will be set in callback parameter faceID and corresponding simularity(indicate
     * how confidence for the match) also will be set.
     * if no face match, a invalid(INVALID_FACE_ID) will be set.*/
    OASISLT_EVT_REC_COMPLETE,

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

    // for face deregistration
    OASISLT_EVT_DEREG_START,
    OASISLT_EVT_DEREG_COMPLETE,

    OASISLT_EVT_NUM

} OASISLTEvt_t;

typedef void *OASISLTHandler_t;

/*this callback definition is used for calling of OASISLT_run
 *for single frame type, only frames[0] is valid and it points to the single input frame
 *for dual frame type, frames[0] point to RGB frame input while frames[1] point to IR frame input
 *for triple frame type,frames[0] point to RGB frame input while frames[1] point to IR frame input, frame[2] point to 3D
 *frame input frames [input]: frames input. evt [input]: event type para [input]: parameters related to this event.
 *userData [input]: user data transferring from OASISLT_run_extend
 **/
typedef void (*OASISLTEvtCb)(ImageFrame_t *frames[OASISLT_INT_FRAME_IDX_LAST],
                             OASISLTEvt_t evt,
                             OASISLTCbPara_t *para,
                             void *userData);

/*OASIS LITE would use this fun to  get all registered face array.
 * faceId[output]: buffer used to save face ID array returned. This buffer size is sizeof(*faceId)*faceNum
 * pfaces[output]: buffer used to save pointer array, each pointer point to a face records data
 * this buffer size is *faceNum*4
 * faceNum[input/output]: as input, it indicates the number "faceId" and "pFaces"
 * as output, it indicates how many items in faceId array and pfaces returned actually.
 * specially, if *faceNum is 0, actual face record number should be set in *faceNum before return.
 * in this case, no "faceId" and 'pFaces' be returned.
 * userData [input]: user data transferring from OASISLT_run_extend
 * return 0 if call succeed, otherwise failed.
 *  */
typedef int (*GetRegisteredFaces)(uint16_t *faceId, void **pFaces, uint32_t *faceNum, void *userData);

/*in register mode, OASIS LITE would call this function to add new face data to database.
 *faceData:[input]pointer to the faces data, the data length can be get by OASISLT_getFaceItemSize()
 *faceId: [output]this id identifies this face uniquely, it should be generated by the caller and
           return back to library.it is used in following face update/recognition procedure.
 *snapshot:[input]snapshot data corresponding to this face. it can be saved for face feature generating
           purpose. if caller don't need it,ignore it.
 *snapshotLength:[input]snapshot data length.
 *userData [input]: user data transferring from OASISLT_run_extend/OASISLT_registration_by_feature
 *return 0 if succeed; otherwise failed*/
typedef int (*FaceOperationAdd)(
    uint16_t *faceId, void *faceData, void *snapshot, int snapshotLength, void *userData);

/*When library beleives a face data/snapshot need to be update during face recognition procedure,
this function will be called to update corresponding data in database.
*faceId:[input] which face data should be update.
*faceData:[input] new face data which should replace original face data in whole.
*snapshotData:[input] new data which should be replace original snapshot.
*dataLength:[input] snapshot data length.
*offset:[input] in most cases, not whole snapshot need update, offset indicate start position of
        snapshot where snapshotData should be written.
*userData [input]: user data transferring from OASISLT_run_extend
*return 0 if successful, other value means failed.
*/
typedef int (*FaceOperationUpdate)(
    uint16_t faceId, void *faceData, void *snapshotData, int dataLength, int offset, void *userData);

/*when library is going to delete a face feature, this function will be called
 * return 0 if successful, other value means failed.
 * */
typedef int (*FaceOperationDelete)(uint16_t faceId, void *userData);

/*Using for print out ANSI string in self test API*/
typedef void (*StringPrint)(const char *str);

typedef void (*EnterCriticalArea)(void);
typedef void (*ExitCriticalArea)(void);

/*Used to dynamically adjust face brightness
 * frameIdx: which frame is need to be adjusted on, OASISLT_INT_FRAME_IDX_RGB or OASISLT_INT_FRAME_IDX_IR ?
 * direction: 1: up 0: down*/
typedef void (*FaceBrightnessAdjust)(uint8_t frameIdx, uint8_t direction, void *userData);


typedef struct {
    const uint8_t* model;
    const uint8_t* modelData;
    uint32_t outputID;

    //input HWC
    int inputHeight, inputWidth, inputChn;

    //face recognition threshold
    float th;
    //how many features is output?
    int outputChn;
} OASISLTCustFaceRec_t;


typedef struct {
    const uint8_t* model;
    const uint8_t* modelData;
    uint32_t outputID;

    //input HWC
    int inputHeight, inputWidth, inputChn;
    //liveness threshold
    float th;
} OASISLTCustLiveness_t;


typedef struct {
    /*This callback function is called when any event start/complete inside library.
     * It can be NULL if caller does not care about any event.*/
    OASISLTEvtCb EvtCb;
    /*By this function, library can get how many face are registered or get a part of registered face items.
     * It can not be NULL.*/
    GetRegisteredFaces GetFaces;
    /*By this function, library can save a face item record to somewhere( it depend on caller, can be ram/disk)
     * If is NULL, no face item should be saved.*/
    FaceOperationAdd AddFace;

    /*By this function,library can update a face item. if is NULL, no update will be done.*/
    FaceOperationUpdate UpdateFace;

    FaceOperationDelete DeleteFace;

    /*By this function, caller can know RGB and IR image's brightness according input parameters */
    FaceBrightnessAdjust AdjustBrightness;

    /*These 2 callback functions are used for multi-thread support. can be set to NULL for non-multi-thread
     * environment*/
    EnterCriticalArea lock;
    ExitCriticalArea unlock;

    // internal debugging use only
    void *reserved;

} InfCallbacks_t;

typedef struct
{
    // max input image height, width and channel, minFace: minimum face can be detected
    int height;
    int width;

    OASISLTImageType_t imgType;

    //min_face should not smaller than 40
    int minFace;

    /*memory pool pointer, this memory pool should only be used by OASIS LIB*/
    char* memPool;

    /*memory pool size*/
    int size;

    /*Fast memory buffer pointer, this buffer should only be used by inference engine inside OASIS LIB
     * set NULL as disable */
    char* fastMemBuf;

    /*Fast memory buffer size, set 0 as disable, suggest (32*1024) or (64*1024) or (128*1024) */
    int fastMemSize;

    /*callback functions provided by caller*/
    InfCallbacks_t cbs;

    /*what functions should be enabled in OASIS LIB*/
    uint8_t enableFlags;

    /*false accept rate*/
    OASISLTFar_t falseAcceptRate;

    /*model class */
    OASISLTModelClass_t modClass;

} OASISLTInitPara_t;

enum
{
    OASISLT_RUN_IDENTIFY_RESULT_OK,
    OASISLT_RUN_IDENTIFY_RESULT_NO_FACE_ON_BOTH,
    OASISLT_RUN_IDENTIFY_RESULT_NO_FACE_ON_INPUT,
    OASISLT_RUN_IDENTIFY_RESULT_NO_FACE_ON_TARGET,
    OASISLT_RUN_IDENTIFY_RESULT_PARAM_ERROR,
    OASISLT_RUN_IDENTIFY_RESULT_LIB_UNINIT,
    OASISLT_RUN_IDENTIFY_RESULT_FAIL_UNKNOW,

};

#ifdef __cplusplus
extern "C" {
#endif
//=====================================================================
/*These APIs are used for all libraries.*/
//=====================================================================

/* Initialize OASIS LITE lib, it should be called before any other APIs.
 * para: initializing parameter. refer to OASISLTInitPara_t for detail information.
 * */
OASISLTResult_t OASISLT_init(OASISLTInitPara_t *para);

OASISLTResult_t OASISLT_uninit();

/*return version information string of OASIS LITE library, please note that:
 * string buffer size input should not less than 64 bytes
 * verStrBuf: buffer used to save version string.
 * length: verStrBuf lenght, unit: byte, it should not less than 64*/
/*return minimum free memory size since OASISLT initialization.
 **/
unsigned int OASISHeapGetMinimumEverFreeHeapSize();

/*return the size of each face feature. unit: byte*/
uint32_t OASISLT_getFaceItemSize(void);

void OASISLT_getVersion(char *verStrBuf, int length);

//=====================================================================
/*These APIs are used only for RT106F and RT117F libraries.*/
//=====================================================================
/*this API can be used to replace OASISLT_run and OASISLT_run2D API with a more flexible input parameters.
 * user can input RGB/IR/3D frame with different combinations according image types in intializing.
 * this API also can be used to extract feature from a given image, user can get face feature by AddFace
 * callback.
 * */
// int OASISLT_run_extend(ImageFrame_t* frames[OASISLT_INT_FRAME_IDX_LAST], uint8_t flag, int minFace, void* userData);
int OASISLT_run_extend(ImageFrame_t *frames[OASISLT_INT_FRAME_IDX_LAST], uint8_t flag, int minFace, void *userData);

/*Used to compare and get similarity of faces in input image and target image.
 * OASIS_IMG_FORMAT_RGB888 and OASIS_IMG_FORMAT_BGR888 are supported*/
int OASISLT_run_identification(ImageFrame_t *input, ImageFrame_t *target, float *sim);

/*This function is used for registration by a face feature generated by OASISLT_run_extend and AddFace
 * callback.
 * faceData: [input] face feature is trying to be added to face database.
 * userData:[input] this user data is going to be transferred to getFace/addFace/updateFace directly.
 * id:[input/output] if *id equal to INVALID_FACE_ID, it means registration by given feature, when registration
 * successfully, corresponding id will be returned; if *id is not INVALID_FACE_ID, it means a feature update,
 * face data of corresponding "id" will be updated with given face data
 * if registration successful, it is a unique ID allocated for the input feature
 * snapshot/snapshotLength:[input] if there is snapshot, it can be also saved.
 * */
OASISLTRegisterRes_t OASISLT_registration_by_feature(
    void *faceData, void *snapshot, int snapshotLength, uint16_t *id, void *userData);

//=====================================================================
/*These APIs are used only for Linux64 and Android libraries.*/
//=====================================================================
OASISLTRegisterRes_t OASISLT_MT_registration_by_feature(
    OASISLTHandler_t handler, void *faceData, void *snapshot, int snapshotLength, uint16_t *id, void *userData);
int OASISLT_MT_run_identification(OASISLTHandler_t handler, ImageFrame_t *input, ImageFrame_t *target, float *sim);
int OASISLT_MT_run_extend(OASISLTHandler_t handler,
                          ImageFrame_t *frames[OASISLT_INT_FRAME_IDX_LAST],
                          uint8_t flag,
                          int minFace,
                          void *userData);
/*These 2 callback functions are used for multi-thread support. can be set to NULL for non-multi-thread environment*/
OASISLTResult_t OASISLT_CreateInstance(OASISLTHandler_t* pHandler);
OASISLTResult_t OASISLT_DeleteInstance(OASISLTHandler_t handler);


//======================================================================
/*These APIs are utility set, can be called without library initialization*/
//======================================================================
/*Crop image with HWC format, format can be RGB888/BGR888 or GRAY8.
 * src [input] pointer to source image data.
 * srcw [input] source image width in pixels(X axis)
 * srch [input] source image height in pixels (Y axis)
 * dst [output] pointer to destination image data
 * dstw [input] destination image width in pixels(X axis)
 * dsth [input] destination image height in pixels (Y axis)
 * top [input] x of crop start point
 * left [input] y of crop start point
 * fmt [input] source and destination image format
 * */
void OASISLT_util_crop(unsigned char* src, int srcw, int srch,
                       unsigned char* dst, int dstw, int dsth,
                       int top, int left, OASISLTImageFormat_t fmt);

/*Convert RGB888 format data to GRAY8 format, input data should be HWC format.
 * rgb [input] pointer to RGB888 data
 * w [input] RGB888 image width in pixels
 * h [input] RGB888 image height in pixels
 * gray [output] pointer to GRAY data, the buffer size should >= w*h
 * */
void OASISLT_util_rgb2gray(const unsigned char* rgb, int w, int h, unsigned char* gray);
void OASISLT_util_bgr2gray(const unsigned char* bgr, int w, int h, unsigned char* gray);


/*Resize RGB888/BGR888/GRAY888/GRAY8 to specified size, input data should be HWC format.
 * it is caller's responsibility to allocate required temporary buffer size for each resizing.
 * src [input] pointer to source image data.
 * srcw [input] source image width in pixels(X axis)
 * srch [input] source image height in pixels (Y axis)
 * dst [output] pointer to destination image data
 * dstw [input] destination image width in pixels(X axis)
 * dsth [input] destination image height in pixels (Y axis)
 * fmt [input] source and destination image format
 * tmp_buf [input] pointer to a temporary buffer which will be used during resizing procedure,
 * it must be 4 bytes aligned.
 * return:  > 0: the size of temporary buffer required if tmp_buf is NULL.
 *         -1: if there are some errors.
 *         0: successful.
 * */
int OASISLT_util_resize(const unsigned char* src, int srcw, int srch,
                        unsigned char* dst, int dstw, int dsth,
                        OASISLTImageFormat_t fmt, void* tmpBuf);


#ifdef __cplusplus
}
#endif

#endif
