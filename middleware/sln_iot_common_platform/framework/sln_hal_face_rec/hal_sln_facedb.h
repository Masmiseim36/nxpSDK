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
 * @brief sln face database reference declaration.
 * Simple database implementation for reference use and algorithm bring-up purposes.
 */

#ifndef _HAL_SLN_FACE_DB_H_
#define _HAL_SLN_FACE_DB_H_

#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INVALID_ID          0xFFFF
#define MAX_FACE_DB_SIZE    100
#define FACE_NAME_MAX_LEN   31
#define MAX_OASIS_FACE_SIZE 900

#ifndef AUTOSAVE
#define AUTOSAVE 1
#endif

#if defined(AUTOSAVE) & (AUTOSAVE == 1)
#warning "A screen flicker might be observed when registering faces if autosave is enabled."
#endif

typedef enum _facedb_status
{
    kFaceDBStatus_Success,
    kFaceDBStatus_AlreadyInit,
    kFaceDBStatus_VersionMismatch,
    kFaceDBStatus_Full,
    kFaceDBStatus_WrongID,
    kFaceDBStatus_WrongParam,
    kFaceDBStatus_Failed,
} facedb_status_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Face database init create and loads faces.
 * @returns a status
 */
facedb_status_t HAL_Facedb_Init(void);

/*!
 * @brief Save all users to flash
 * @return kFaceDBStatus_Success if the operation was successfull.
 */
facedb_status_t HAL_Facedb_SaveFace(void);

/*!
 * @brief Add a face into RAM database. If autosave is enable also save to flash.
 * @returns a status
 */
facedb_status_t HAL_Facedb_AddFace(uint16_t id, char *name, void *face, int size);

/*!
 * @brief Delete a face from the database
 * @param id - the identification number of the face to be removed. INVALID_ID
 * should be passed to delete all
 * @returns a status
 */
facedb_status_t HAL_Facedb_DelFaceWithID(uint16_t id);

/*!
 * @brief Delete a face from the database
 * @param name the name of the face to be removed. First occurrence of the name
 * will be removed.
 * @returns a status
 */
facedb_status_t HAL_Facedb_DelFaceWithName(char *name);

/*!
 * @brief get the list with all the ids and faces from the database
 * @para face_ids, pointer to an array;
 * @param pFace, pointer to an array;
 * @returns a status
 */

facedb_status_t HAL_Facedb_GetAllFaceIds(uint16_t *face_ids, void **pFace);

/*!
 * @brief get the face attribute from an id
 * @param id the id of the face;
 * @returns a status
 */

facedb_status_t HAL_Facedb_GetFace(uint16_t id, void **pFace);

/*!
 * @brief Generates an id to be used when adding in the database
 * status
 */
facedb_status_t HAL_Facedb_GenId(uint16_t *new_id);

/*!
 * @brief get the list with all the ids from the database
 * @param id face_ids, pointer to an array;
 * @returns a status
 */
facedb_status_t HAL_Facedb_GetIds(uint16_t *face_ids);

/*!
 * @brief Determines if the user identified by the given id
 * has been saved to flash.
 * If the id given is INVALID_ID, determines whether ALL registered users
 * have been saved to flash.
 * @param id - identification number of the user
 * @return false if not saved, true if saved.
 */
bool HAL_Facedb_GetSaveStatus(uint16_t id);

/*!
 * @brief Get the face item count in the database
 * @returns the number of faces inside the database
 */
int facedb_get_count();

/*!
 * @brief Get the name of a face identified by the id
 * @returns a pointer to the name
 */
char *HAL_Facedb_GetName(uint16_t id);

/*!
 * @brief update the name of a face identified by the id
 * @returns a status
 */
facedb_status_t HAL_Facedb_UpdateName(uint16_t id, char *name);

/*!
 * @brief update the face data by the id and name
 * @returns a status
 */
facedb_status_t HAL_Facedb_UpdateFace(uint16_t id, char *name, void *face, int size);

#if defined(__cplusplus)
}
#endif

#endif /*_HAL_SLN_FACE_DB_H_*/
