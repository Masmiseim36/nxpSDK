/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_DIR_H
#define OSA_DIR_H

/*!
 * @file    osa_dir.h
 * @brief   Contains file and directory function prototypes for libosa.
 */

/*! Bit masks that define properties of a file or directory. */
#define OSA_OWNER_READ           0x0400
#define OSA_OWNER_WRITE          0x0200
#define OSA_OWNER_EXECUTE        0x0100
#define OSA_GROUP_READ           0x0040
#define OSA_GROUP_WRITE          0x0020
#define OSA_GROUP_EXECUTE        0x0010
#define OSA_OTHER_READ           0x0004
#define OSA_OTHER_WRITE          0x0002
#define OSA_OTHER_EXECUTE        0x0001

#define OSA_PERM_ALL             0x0777

/*! Type field for OsaDirEnt structure. */
enum OsaDirEntType {
    OSA_DT_DIR,             /*! Entry is a directory */
    OSA_DT_FILE,            /*! Entry is a regular file */
    OSA_DT_UNKNOWN          /*! Entry is unknown type (Linux only) */
};

/*! Directory mount structure. */
typedef struct _OsaDirMount {
    char mount_point[PATHNAME_SIZE];  /*! Mount point for device */
    char type;                        /*! Type of device (TODO: define types) */
} OsaDirMount;

/*! Directory entry structure. */
typedef struct _OsaDirEnt {
    char name[PATHNAME_SIZE];         /*! Name of the file/directory */
    char type;                        /*! Type of entry (OsaDirEntType) */
} OsaDirEnt;

/*! Abstract type to represent a directory handle. */
typedef void OsaDir;

/*!
 * @ingroup libosa
 * @{
 */

/*!
 * @brief   Create a new directory
 * @details Create a directory at the given 'path' with the given 'mode'.  The
 *          path can be a full path including device/mount point, or it can be
 *          a relative path.  On Linux, the current path will be based on
 *          where the application is executed from.  On MQX the current path
 *          will be the root of the device unless a call to osa_chdir() is
 *          made to change it.  Current paths can be different for each
 *          process on Linux and each thread on MQX.
 * @param   path The path and name of the directory to make
 * @param   mode The directory mode.  This should be a combination of bit
 *          modes defined above.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory (MQX only)
 * @retval  ERRCODE_PERMISSION_DENIED    Permission denied
 * @retval  ERRCODE_PARAMETER_EXISTS   Directory exists
 * @retval  ERRCODE_OUT_OF_SPACE   No space left on device
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid input parameters
 * @retval  ERRCODE_GENERAL_ERROR   An error occurred creating the directory
*/
int osa_mkdir(const char *path, int mode);

/*!
 * @brief   Change current working directory
 * @details Change the current working directory for the executing process or
 *          thread to 'path'.  On Linux, this directory can be unique for the
 *          entire process (all threads) and on MQX it can be unique for each
 *          thread.
 * @param   path The directory path to change to
 * @warning This function has non-expected behavior on MQX and should be
 *          avoided until corrected.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory (MQX only)
 * @retval  ERRCODE_PERMISSION_DENIED    Permission denied
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid input parameters
 * @retval  ERRCODE_GENERAL_ERROR   An error occurred creating the directory
*/
int osa_chdir(const char *path);

/*!
 * @brief   Open a directory for reading
 * @details Open a directory at path 'dirname' and return a pointer to an
 *          object for reading.
 * @param   dirname The path and name of the directory to open.  This can be
 *          absolute or relative to current working directory.
 *
 * @returns Valid pointer on success or NULL on failure.
*/
OsaDir *osa_opendir(const char *dirname);

/*!
 * @brief   Close a directory
 * @details Close the pointer object to an open directory and release any
 *          memory associated with it.
 * @param   dirp Directory pointer
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid input parameter
*/
int osa_closedir(OsaDir *dirp);

/*!
 * @brief   Read contents of a directory
 * @details Read a single directory entry for the directory object pointed to
 *          by 'dirp' and return the data in 'entry'.
 * @param   dirp Directory pointer
 * @param   entry Output parameter to store directory entry contents.  This
 *          must be a valid pointer to an object.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_END_OF_DIRECTORY      End of directory reached
 * @retval  ERRCODE_GENERAL_ERROR   An error occurred reading the directory
*/
int osa_readdir(OsaDir *dirp, OsaDirEnt *entry);

/*!
 * @brief   Remove a directory entry
 * @details Remove a file or directory from the filesystem.
 * @param   path Path to the file or directory
 *
 * @retval  ERRCODE_NO_ERROR           Function succeeded
 * @retval  ERRCODE_PERMISSION_DENIED  Permission Denied
 * @retval  ERRCODE_BUSY               Entry is being accessed
 * @retval  ERRCODE_NOT_FOUND          Entry name not found
 * @retval  ERRCODE_INVALID_ARGUMENT   The path did not contain a valid device
 * @retval  ERRCODE_GENERAL            An error occurred deleting the entry
*/
int osa_unlink(const char *path);

#endif

/*! @} */

