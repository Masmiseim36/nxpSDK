/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief sln face database reference implementation.
 * Simple RAM-based database for reference and algorithm bring up purposes.
 */

#include <FreeRTOS.h>
#include "semphr.h"

#include "fwk_log.h"
#include "fwk_flash.h"
#include "hal_sln_facedb.h"
#include "hal_flash_dev.h"
#include "stdio.h"

#define FEATURE_VERSION 0x0001
/* TODO this needs to be defined at runtime */
#define MODEL_VERSION 0x0001

#define FACEDB_SLOT_EMPTY 0x0

#define RESERVED_DATA 0x8

#define OASIS_DIR "oasis"

#define METADATA_FILE_NAME \
    OASIS_DIR              \
    "/"                    \
    "Metadata"

#define OASIS_FACEDATABASE \
    OASIS_DIR              \
    "/"

typedef enum _face_mapping_bitwise
{
    kFaceMappingBitWise_Saved,
    kFaceMappingBitWise_Updated,
    kFaceMappingBitWise_Used,
} face_mapping_bitwise_t;

#define FACE_NOT_USED    0X0
#define FACE_IN_USE      (1 << kFaceMappingBitWise_Used)
#define FACE_NOT_SAVED   0x0
#define FACE_SAVED       (1 << kFaceMappingBitWise_Saved)
#define FACE_NOT_UPDATED 0x0
#define FACE_UPDATED     (1 << kFaceMappingBitWise_Updated)

typedef struct _facedb_metadata
{
    uint32_t featureVersion;
    uint32_t modelVersion;
    uint8_t numberFaces;
    /* RESERVED DATA for future updates */
    uint8_t reservedData[RESERVED_DATA];
    uint8_t faceMapping[MAX_FACE_DB_SIZE];
} facedb_metadata_t;

typedef struct _facedb_entry
{
    char name[FACE_NAME_MAX_LEN + 1];
    unsigned char face[MAX_OASIS_FACE_SIZE];
} facedb_entry_t;

// memory face database
static facedb_entry_t s_FaceDB[MAX_FACE_DB_SIZE];

static facedb_metadata_t s_OasisMetadata;

// face database lock
static SemaphoreHandle_t s_FaceDBLock = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int _Facedb_Lock();
static void _Facedb_Unlock();
static void _Facedb_SetMetaDataDefault();
static void _facedb_face_setdefault();
static facedb_status_t _Facedb_Init();
static sln_flash_status_t _Facedb_Load();
static sln_flash_status_t _Facedb_UpdateMetadata();
static sln_flash_status_t _Facedb_SaveFace(uint16_t id);
static sln_flash_status_t _Facedb_DeleteFace(uint16_t id);
static sln_flash_status_t _Facedb_DeleteAllFaces();

/*******************************************************************************
 * Code
 ******************************************************************************/
// internal function to take the database lock
static int _Facedb_Lock()
{
    if (pdTRUE != xSemaphoreTake(s_FaceDBLock, portMAX_DELAY))
    {
        return kFaceDBStatus_Failed;
    }

    return kFaceDBStatus_Success;
}

// internal function to relase the database lock
static void _Facedb_Unlock()
{
    if (s_FaceDBLock != NULL)
    {
        xSemaphoreGive(s_FaceDBLock);
    }
}

static void _Facedb_SetMetaDataDefault()
{
    s_OasisMetadata.featureVersion = FEATURE_VERSION;
    s_OasisMetadata.modelVersion   = MODEL_VERSION;
    s_OasisMetadata.numberFaces    = 0;
    memset(s_OasisMetadata.faceMapping, FACEDB_SLOT_EMPTY, MAX_FACE_DB_SIZE * sizeof(uint8_t));
}

static void _facedb_face_setdefault()
{
    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        memset(&s_FaceDB[id], 0, sizeof(facedb_entry_t));
    }
}

static void _Facedb_GeneratePathFromIndex(uint16_t id, char *path)
{
    if (path != NULL)
    {
        char index[4];
        itoa(id, index, 10);
        strcpy(path, OASIS_FACEDATABASE);
        strcat(path, index);
    }
}

static sln_flash_status_t _Facedb_UpdateMetadata()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_metadata_t oasisMetadata;
    oasisMetadata.featureVersion = s_OasisMetadata.featureVersion;
    oasisMetadata.modelVersion   = s_OasisMetadata.modelVersion;
    oasisMetadata.numberFaces    = s_OasisMetadata.numberFaces;
    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        /* Clear the used flag */
        if (s_OasisMetadata.faceMapping[id] == (1 << kFaceMappingBitWise_Used))
        {
            oasisMetadata.numberFaces--;
        }

        oasisMetadata.faceMapping[id] = s_OasisMetadata.faceMapping[id] & (~(1 << kFaceMappingBitWise_Used));
    }

    do
    {
        LOGI("Facedb: Trying to update metadata in flash...");
        status = FWK_Flash_Save(METADATA_FILE_NAME, &oasisMetadata, sizeof(facedb_metadata_t));
    } while (status != kStatus_HAL_FlashSuccess);

    LOGI("Facedb: Metadata updated.");

    return status;
}

static sln_flash_status_t _Facedb_Load()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    bool updateMetadata       = false;

    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED)
        {
            char path[20];
            _Facedb_GeneratePathFromIndex(id, path);
            status = FWK_Flash_Read(path, &s_FaceDB[id], sizeof(facedb_entry_t));
            if (status != kStatus_HAL_FlashSuccess)
            {
                LOGE("Facedb: Failed to load face database at path \"%s\".", path);
                if (status == kStatus_HAL_FlashFileNotExist)
                {
                    updateMetadata                  = true;
                    s_OasisMetadata.faceMapping[id] = FACEDB_SLOT_EMPTY;
                    s_OasisMetadata.numberFaces--;
                }
                else
                {
                    LOGE("Facedb: Status \"%d\".", status);
                    break;
                }
            }
            else
            {
                s_OasisMetadata.faceMapping[id] = FACE_SAVED | FACE_IN_USE;
            }
        }
        else if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Updated)) == FACE_UPDATED)
        {
            /* loads older version */
            char path[20];
            LOGD("Facedb: Update operation not saved on last run for id %d, load older version", id);
            _Facedb_GeneratePathFromIndex(id, path);
            status         = FWK_Flash_Read(path, &s_FaceDB[id], sizeof(facedb_entry_t));
            updateMetadata = true;
            if (status != kStatus_HAL_FlashSuccess)
            {
                LOGE("Facedb: Failed to load face database at path \"%s\".", path);
                if (status == kStatus_HAL_FlashFileNotExist)
                {
                    s_OasisMetadata.faceMapping[id] = FACEDB_SLOT_EMPTY;
                    s_OasisMetadata.numberFaces--;
                }
                else
                {
                    LOGE("Facedb: Status \"%d\".", status);
                    break;
                }
            }
            else
            {
                s_OasisMetadata.faceMapping[id] = FACE_SAVED | FACE_IN_USE;
            }
        }
        else if (s_OasisMetadata.faceMapping[id] != FACEDB_SLOT_EMPTY)
        {
            LOGE("Facedb: Wrong metadata for id \"%d\".", id);
            status = kStatus_HAL_FlashFail;
            break;
        }
    }

    if ((status == kStatus_HAL_FlashSuccess) && (updateMetadata == true))
    {
        _Facedb_UpdateMetadata();
    }

    return status;
}

static facedb_status_t _Facedb_Init()
{
    sln_flash_status_t status = FWK_Flash_Mkdir(OASIS_DIR);
    facedb_status_t ret       = kFaceDBStatus_Success;
    if (status == kStatus_HAL_FlashDirExist)
    {
        /* Already exists assume everything is ok don't over engineer for now */

        status = FWK_Flash_Read(METADATA_FILE_NAME, &s_OasisMetadata, sizeof(facedb_metadata_t));
        if (status == kStatus_HAL_FlashSuccess)
        {
            if ((s_OasisMetadata.featureVersion != FEATURE_VERSION) || (s_OasisMetadata.modelVersion != MODEL_VERSION))
            {
                LOGE(
                    "Facedb Oasis_Version found in flash different from current version. Features might be "
                    "different.");
                /* Different version erase */
                s_OasisMetadata.featureVersion = FEATURE_VERSION;
                s_OasisMetadata.modelVersion   = MODEL_VERSION;
                FWK_Flash_Save(METADATA_FILE_NAME, &s_OasisMetadata, sizeof(facedb_metadata_t));
                /* TODO */
                ret = kFaceDBStatus_VersionMismatch;
            }
            else
            {
                /* Same version */
                LOGI("Facedb: Same version. Number of faces %d.", s_OasisMetadata.numberFaces);
                status = _Facedb_Load();
                if (status == kStatus_HAL_FlashSuccess)
                {
                    ret = kFaceDBStatus_AlreadyInit;
                }
                else
                {
                    ret = kFaceDBStatus_Failed;
                }
            }
        }
        else
        {
            LOGE("Facedb: Failed to open the config file.");
            ret = kFaceDBStatus_Failed;
        }
    }
    else if (status == kStatus_HAL_FlashSuccess)
    {
        /* First time write default */
        _Facedb_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_OasisMetadata, sizeof(facedb_metadata_t));
        LOGI("Facedb: Metadata saved. Database is empty.");
        ret = kFaceDBStatus_Success;
    }
    else
    {
        LOGE("Facedb: Failed to init the config file.");
        ret = kFaceDBStatus_Failed;
    }

    return ret;
}

static sln_flash_status_t _Facedb_SaveFace(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;

    /* Save in a file */
    char path[20];
    _Facedb_GeneratePathFromIndex(id, path);

    status = FWK_Flash_Save(path, &s_FaceDB[id], sizeof(facedb_entry_t));
    if (status == kStatus_HAL_FlashSuccess)
    {
        /* Update metadata */
        s_OasisMetadata.faceMapping[id] = FACE_IN_USE | FACE_SAVED;
    }
    else
    {
        LOGE("Facedb: Failed to save face.");
    }

    return status;
}

static sln_flash_status_t _Facedb_DeleteFaceFromFlash(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    char path[20];

    LOGD("Facedb: delete file from flash id %d", id);
    _Facedb_GeneratePathFromIndex(id, path);
    status = FWK_Flash_Rm(path);
    if (status == kStatus_HAL_FlashSuccess || status == kStatus_HAL_FlashFileNotExist)
    {
        s_OasisMetadata.faceMapping[id] &= ~FACE_SAVED;
        s_OasisMetadata.faceMapping[id] &= ~FACE_UPDATED;
    }

    return status;
}

static sln_flash_status_t _Facedb_DeleteAllFaces()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    uint8_t updateMetadata    = false;

    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        /* Try to delete gracefully */
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            /* Del from RAM */
            memset(&s_FaceDB[id], 0, sizeof(facedb_entry_t));
            s_OasisMetadata.faceMapping[id] &= ~(1 << kFaceMappingBitWise_Used);
            s_OasisMetadata.numberFaces--;

            if (((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED) ||
                (s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Updated)) == FACE_UPDATED)
            {
                status = _Facedb_DeleteFaceFromFlash(id);
                if (status != kStatus_HAL_FlashSuccess && status != kStatus_HAL_FlashFileNotExist)
                {
                    LOGE("Facedb: Failed to remove face from database.");
                    continue;
                }
                updateMetadata = true;
            }
        }
    }

    if ((s_OasisMetadata.numberFaces != 0) || (status != kStatus_HAL_FlashSuccess))
    {
        /* Something went wrong delete all from RAM and clean metadata */
        _facedb_face_setdefault();
        _Facedb_SetMetaDataDefault();
        updateMetadata = true;
    }
    else
    {
        LOGI("Facedb: Metadata is empty. Updating metadata in flash...");
    }

    if ((status == kStatus_HAL_FlashSuccess) && (updateMetadata == true))
    {
        status = _Facedb_UpdateMetadata();
    }

    return status;
}

static sln_flash_status_t _Facedb_DeleteFace(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;

    if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
    {
        /* Del from RAM */
        LOGD("Facedb: delete file from ram id %d", id);
        memset(&s_FaceDB[id], 0, sizeof(facedb_entry_t));
        s_OasisMetadata.faceMapping[id] &= ~(1 << kFaceMappingBitWise_Used);
        s_OasisMetadata.numberFaces--;

        /* Delete from flash */
        if (((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED) ||
            (s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Updated)) == FACE_UPDATED)
        {
            status = _Facedb_DeleteFaceFromFlash(id);

            if ((status != kStatus_HAL_FlashSuccess) && (status != kStatus_HAL_FlashFileNotExist))
            {
                LOGE("Facedb: Failed to delete face from id: %d.", id);
                status = kStatus_HAL_FlashFail;
            }
            else
            {
                status = _Facedb_UpdateMetadata();
            }
        }
    }

    return status;
}

static facedb_status_t _Facedb_GetIdFromName(char *name, uint16_t *pId)
{
    *pId = INVALID_ID;
    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        if (strcmp(name, s_FaceDB[id].name) == 0)
        {
            *pId = id;
            return kFaceDBStatus_Success;
        }
    }

    return kFaceDBStatus_Failed;
}

/* face database init */
facedb_status_t HAL_Facedb_Init(void)
{
    facedb_status_t status = kFaceDBStatus_Success;

    if (NULL == s_FaceDBLock)
    {
        s_FaceDBLock = xSemaphoreCreateMutex();

        if (NULL == s_FaceDBLock)
        {
            LOGE("Facedb: Failed to create DB lock semaphore");
            status = kFaceDBStatus_Failed;
        }
    }

    if (status == kFaceDBStatus_Success)
    {
        status = _Facedb_Init();
    }

    return status;
}

/* get the face item count in the database */
int facedb_get_count()
{
    return s_OasisMetadata.numberFaces;
}

facedb_status_t HAL_Facedb_SaveFace()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_status_t ret       = kFaceDBStatus_Success;
    uint8_t updateMetadata    = false;
    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        /* Try to save */
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_NOT_SAVED)
            {
                status = _Facedb_SaveFace(id);
                if (status == kStatus_HAL_FlashSuccess)
                {
                    LOGD("Facedb: Face with id \"%d\" was successfully saved to flash.", id);
                    updateMetadata = true;
                }
                else
                {
                    LOGE("Facedb: Saving face with id \"%d\" failed with error code \"%d\".", id, status);
                }
            }
        }
    }

    LOGI("Facedb: Finished saving faces to flash.");

    /* Update Flash metadata */
    if ((status == kStatus_HAL_FlashSuccess) && (updateMetadata == true))
    {
        status = _Facedb_UpdateMetadata();
    }

    if (status != kStatus_HAL_FlashSuccess)
    {
        ret = kFaceDBStatus_Failed;
    }

    return ret;
}

facedb_status_t HAL_Facedb_AddFace(uint16_t id, char *name, void *face, int size)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if (size >= MAX_OASIS_FACE_SIZE || id >= MAX_FACE_DB_SIZE || face == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_NOT_USED)
        {
            /* Update RAM face */
            if (name != NULL)
            {
                strcpy(s_FaceDB[id].name, name);
            }
            else
            {
                sprintf(s_FaceDB[id].name, "user_%03d", id);
                s_FaceDB[id].name[FACE_NAME_MAX_LEN - 1] = 0;
            }

            memcpy(s_FaceDB[id].face, face, size);

            s_OasisMetadata.faceMapping[id] = FACE_IN_USE;
            s_OasisMetadata.numberFaces++;

            LOGD("Facedb: Added face to RAM successfully :%d %s.", id, name);

#if AUTOSAVE
            /* Save to Flash */
            sln_flash_status_t status = kStatus_HAL_FlashSuccess;
            status                    = _Facedb_SaveFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("Facedb: Added face to flash successfully :%d %s.", id, name);

                /* Update Flash metadata */
                _Facedb_UpdateMetadata();

                ret = kFaceDBStatus_Success;
            }
            else
            {
                LOGE("Facedb: Failed to save face into flash.");
                ret = kFaceDBStatus_Failed;
            }
#endif /* AUTOSAVE */
        }
        else
        {
            LOGE("Facedb: Face slot for ID %d already in use.", id);
        }
        _Facedb_Unlock();
    }
    return ret;
}

facedb_status_t HAL_Facedb_DelFaceWithName(char *name)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_status_t ret       = kFaceDBStatus_Success;
    if (name == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        uint16_t id;
        _Facedb_GetIdFromName(name, &id);
        if (id != INVALID_ID)
        {
            /* Delete from Flash */
            status = _Facedb_DeleteFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGI("Facedb: Successfully deleted face:%d \"%s\".", id, name);
                ret = kFaceDBStatus_Success;
            }
            else
            {
                ret = kFaceDBStatus_Failed;
            }
        }

        _Facedb_Unlock();
    }
    return ret;
}

/*  del a face item in the database */
facedb_status_t HAL_Facedb_DelFaceWithID(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_status_t ret       = _Facedb_Lock();

    if (ret == kFaceDBStatus_Success)
    {
        if (id == INVALID_ID)
        {
            /* Delete all maybe ? */
            status = _Facedb_DeleteAllFaces();
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("Facedb: Successfully deleted all registered faces.");
                ret = kFaceDBStatus_Success;
            }
            else
            {
                ret = kFaceDBStatus_Failed;
            }
        }
        else if (id >= MAX_FACE_DB_SIZE)
        {
            ret = kFaceDBStatus_WrongID;
        }
        else if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            /* Delete from Flash */
            status = _Facedb_DeleteFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                // TODO: Add name of deleted face
                LOGI("Facedb: Successfully deleted face: %d.", id);
                ret = kFaceDBStatus_Success;
            }
            else
            {
                ret = kFaceDBStatus_Failed;
            }
        }
        else
        {
            LOGE("Facedb: No face associated with the specified ID \"%d\".", id);
        }

        _Facedb_Unlock();
    }

    return ret;
}

facedb_status_t HAL_Facedb_GetAllFaceIds(uint16_t *face_ids, void **pFace)
{
    facedb_status_t ret = kFaceDBStatus_Success;
    uint16_t index      = 0;

    if (face_ids == NULL || pFace == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
        {
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
            {
                face_ids[index]  = id;
                *(pFace + index) = s_FaceDB[id].face;
                index++;
            }
        }
        _Facedb_Unlock();
    }

    return ret;
}

/* get the face item pointer with the specified face id from the database */
facedb_status_t HAL_Facedb_GetFace(uint16_t id, void **pFace)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if (pFace == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if (id >= MAX_FACE_DB_SIZE)
        {
            ret    = kFaceDBStatus_WrongID;
            *pFace = NULL;
        }
        else if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            *pFace = s_FaceDB[id].face;
        }
        else
        {
            ret    = kFaceDBStatus_Failed;
            *pFace = NULL;
        }
        _Facedb_Unlock();
    }

    return ret;
}

bool HAL_Facedb_GetSaveStatus(uint16_t id)
{
    bool isSaved = true;

    if ((id < MAX_FACE_DB_SIZE) || (id == INVALID_ID))
    {
        if (id == INVALID_ID)
        {
            for (uint16_t i = 0; i < MAX_FACE_DB_SIZE; i++)
            {
                if (((s_OasisMetadata.faceMapping[i] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE) &&
                    ((s_OasisMetadata.faceMapping[i] & (1 << kFaceMappingBitWise_Saved)) == FACE_NOT_SAVED))
                {
                    isSaved = false;
                    break;
                }
            }
        }
        else
        {
            if (((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE) &&
                ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_NOT_SAVED))
            {
                isSaved = false;
            }
        }
    }
    else
    {
        isSaved = false;
    }

    return isSaved;
}

/* get the name of the face item with the specified face id from the database */
char *HAL_Facedb_GetName(uint16_t id)
{
    facedb_status_t ret = _Facedb_Lock();
    char *face_name     = NULL;

    if (ret == kFaceDBStatus_Success)
    {
        // lock success
        if ((id < MAX_FACE_DB_SIZE) &&
            ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE))
        {
            face_name = s_FaceDB[id].name;
        }
        else
        {
            LOGE("Facedb: No face associated with the specified ID \"%d\"", id);
        }

        _Facedb_Unlock();
    }

    return face_name;
}

/* update the name of the face item with the specified face id from the database */
facedb_status_t HAL_Facedb_UpdateName(uint16_t id, char *name)
{
    facedb_status_t ret = _Facedb_Lock();

    if (ret == kFaceDBStatus_Success)
    {
        // lock success
        if ((id < MAX_FACE_DB_SIZE) &&
            ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE))
        {
            memcpy(s_FaceDB[id].name, name, sizeof(s_FaceDB[id].name));
#if AUTOSAVE
            sln_flash_status_t status = kStatus_HAL_FlashSuccess;

            /* Save to Flash */
            status = _Facedb_SaveFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("[DEBUG] Facedb: Added Flash success :%d %s \r\n", id, name);
            }
#else
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED)
            {
                s_OasisMetadata.faceMapping[id] &= ~FACE_SAVED;
                s_OasisMetadata.faceMapping[id] |= FACE_UPDATED;
            }
#endif /* AUTOSAVE */
        }
        else
        {
            LOGE("Facedb: No face associated with the specified ID \"%d\"", id);
        }

        _Facedb_Unlock();
    }

    return ret;
}

/* update old face item into the database */
facedb_status_t HAL_Facedb_UpdateFace(uint16_t id, char *name, void *face, int size)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if (size >= MAX_OASIS_FACE_SIZE || id >= MAX_FACE_DB_SIZE || face == NULL || name == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            /* Update RAM face */
            strcpy(s_FaceDB[id].name, name);
            memcpy(s_FaceDB[id].face, face, size);

            LOGD("Facedb: Successfully saved face to RAM:%d %s \r\n", id, name);
#if AUTOSAVE
            sln_flash_status_t status = kStatus_HAL_FlashSuccess;

            /* Save to Flash */
            status = _Facedb_SaveFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("Facedb: Successfully saved face to Flash:%d %s \r\n", id, name);
            }
#else
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED)
            {
                s_OasisMetadata.faceMapping[id] &= ~FACE_SAVED;
                s_OasisMetadata.faceMapping[id] |= FACE_UPDATED;
            }
#endif /* AUTOSAVE */
        }
        else
        {
            LOGE("Facedb: slot is not used for this id:%d.", id);
        }
        _Facedb_Unlock();
    }
    return ret;
}

facedb_status_t HAL_Facedb_GetIds(uint16_t *face_ids)
{
    facedb_status_t ret = kFaceDBStatus_Success;
    uint8_t index       = 0;

    if (face_ids == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
        {
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
            {
                face_ids[index++] = id;
            }
        }
        _Facedb_Unlock();
    }

    return ret;
}

/* gen the unique face id for the new face */
facedb_status_t HAL_Facedb_GenId(uint16_t *new_id)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if (new_id == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }

    if (ret == kFaceDBStatus_Success)
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        *new_id = INVALID_ID;

        if (s_OasisMetadata.numberFaces == MAX_FACE_DB_SIZE)
        {
            ret = kFaceDBStatus_Full;
        }

        if (ret == kFaceDBStatus_Success)
        {
            for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
            {
                if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_NOT_USED)
                {
                    *new_id = id;
                    break;
                }
            }
        }
        _Facedb_Unlock();
    }

    return ret;
}
