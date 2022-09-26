/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    file_utils.c
 * @brief   This file provides file manipulation functions.
 */

#include "file_utils.h"

#ifdef FILE_ENABLE_FATFS
#include "ff.h"
#endif // FILE_ENABLE_FATFS
#include <stdbool.h>

#include "fsl_debug_console.h"

#ifdef FILE_ENABLE_SEMI
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fsl_os_abstraction.h"
#endif // FILE_ENABLE_SEMI

/**
 * @brief File descriptor structure
 *
 */
typedef struct
{
    int32_t fs_type;  /*!< @brief File system type */
    size_t file_size; /*!< @brief File size */
#ifdef FILE_UTILS_SEMI_CACHE
    void *file_cache_start;
    int32_t file_cache_pos;
    int8_t dirty;
#endif
    union
    {
#ifdef FILE_ENABLE_FATFS
        FIL fatfs_file;
#endif
#ifdef FILE_ENABLE_SEMI
        FILE semi_file;
#endif
    } file_struct; /*!< @brief File system structure */
} File_Fs_Rec;

/**
 * @brief File descriptor table
 *
 */
static File_Fs_Rec file_fh_table[FILE_FS_TABLE_SIZE];

#ifdef FILE_UTILS_SEMI_CACHE
extern char __base_BOARD_SDRAM;
#define fs_buff           (&__base_BOARD_SDRAM)
#define FS_BUFF_SIZE      (32 * 1024 * 1024)
#define FS_WR_BUFF_SIZE   (4 * 1024 * 1024)
#define MAX_BUFF_READ_LEN 10 * 1024
static size_t fs_buff_pos = 0;
#endif

/**
 * @brief Allocate file descriptor
 *
 * @param fs_type File system type
 * @return int32_t
 *
 * @retval fd File descriptor on success
 * @retval -1 Failure
 */
static int32_t alloc_fd(int32_t fs_type)
{
    size_t i = 0;
    for (; i < FILE_FS_TABLE_SIZE; i++)
    {
        if (file_fh_table[i].fs_type == 0)
        {
            break;
        }
    }
    if (i < FILE_FS_TABLE_SIZE)
    {
        size_t fd                 = i;
        file_fh_table[fd].fs_type = fs_type;
        return (int32_t)fd;
    }
    else
    {
        return -1; // fd table is full
    }
}

/**
 * @brief Free file descriptor
 *
 * @param idx File descriptor ID
 * @return int32_t
 *
 * @retval 0 Success
 * @retval -1 Failure
 */
static int32_t free_fd(int32_t idx)
{
    if (idx < 0 || idx >= (int32_t)FILE_FS_TABLE_SIZE)
    {
        return -1;
    }
    if (file_fh_table[idx].fs_type == FILE_UTILS_NONE)
    {
        return -1; // fd is already empty
    }
    else
    {
        file_fh_table[idx].fs_type = FILE_UTILS_NONE;
        return 0;
    }
}

/**
 * @brief Get the file descriptor object
 *
 * @param idx File descriptor ID
 * @return File_Fs_Rec*
 */
static File_Fs_Rec *get_fd(int32_t idx)
{
    if (idx < 0 || idx >= (int32_t)FILE_FS_TABLE_SIZE)
    {
        return NULL;
    }
    else if (file_fh_table[idx].fs_type == FILE_UTILS_NONE)
    {
        return NULL;
    }
    else
    {
        return &file_fh_table[idx];
    }
}

#ifdef FILE_UTILS_SEMI_CACHE
static int fill_cache(int32_t fd)
{
    PRINTF("Fill cache\r\n");
    get_fd(fd)->file_cache_pos   = -1; // Mark that buffer is empty
    get_fd(fd)->file_cache_start = fs_buff + fs_buff_pos;
    if (get_fd(fd)->file_cache_start + file_getsize(fd) >= (void *)(fs_buff + FS_BUFF_SIZE))
    {
        return -1;
    }
    for (int32_t read = 0; read < get_fd(fd)->file_size;)
    {
        uint32_t rs = MIN(get_fd(fd)->file_size - read, MAX_BUFF_READ_LEN);
        int32_t r   = file_read(fd, (get_fd(fd)->file_cache_start + read), rs);
        read += r;
    }
    fs_buff_pos += file_getsize(fd);
    get_fd(fd)->file_cache_pos = 0;
    PRINTF("Cache filled\r\n");
    return 0;
}
#endif

int32_t file_open(const char *pathname, uint32_t mode)
{
    int32_t fd;
    fd = alloc_fd(FILE_UTILS_FS_SEL);
    if (fd < 0)
    {
        return -1;
    }
#ifdef FILE_ENABLE_FATFS
#if FILE_UTILS_FS_SEL == FILE_UTILS_FATFS
    BYTE fatfs_mode        = 0;
    uint_fast8_t uniq_flag = 0;
#ifdef FILE_UTILS_SEMI_CACHE
    get_fd(fd)->dirty = 0;
#endif // FILE_UTILS_SEMI_CACHE
    if (mode & FILE_RDONLY)
    {
        fatfs_mode |= FA_READ;
        uniq_flag = 1;
    }
    if (mode & FILE_WRONLY)
    {
        if (uniq_flag)
            return -1;
        fatfs_mode |= FA_WRITE;
        uniq_flag = 1;
    }
    if (mode & FILE_RDWR)
    {
        if (uniq_flag)
            return -1;
        fatfs_mode |= FA_WRITE | FA_READ;
        uniq_flag = 1;
    }
    if (mode & FILE_APPEND)
    {
        fatfs_mode |= FA_WRITE | FA_READ;
    }
    if (mode & FILE_CREAT)
    {
        fatfs_mode |= FA_OPEN_ALWAYS;
#ifdef FILE_UTILS_SEMI_CACHE
        get_fd(fd)->dirty = 1;
#endif
    }
    if (mode & FILE_TRUNC)
    {
        fatfs_mode |= FA_CREATE_ALWAYS;
#ifdef FILE_UTILS_SEMI_CACHE
        get_fd(fd)->dirty = 1;
#endif
    }
    FRESULT ret = f_open(&get_fd(fd)->file_struct.fatfs_file, pathname, fatfs_mode);
    if (ret != FR_OK)
    {
        free_fd(fd);
        return -1;
    }
    else
    {
        return fd;
    }
#endif // FILE_UTILS_FS_SEL==FILE_UTILS_FATFS
#endif // FILE_ENABLE_FATFS
#ifdef FILE_ENABLE_SEMI
#if FILE_UTILS_FS_SEL == FILE_UTILS_SEMI
    char file_mode[5]  = "";
    uint32_t uniq_flag = 0;
    size_t idx         = 0;
    get_fd(fd)->dirty  = 0;
    if (mode & FILE_RDONLY)
    {
        file_mode[idx++] = 'r';
        uniq_flag += 1u;
    }
    if (mode & FILE_WRONLY)
    {
        file_mode[idx++] = 'w';
        uniq_flag += 1u;
    }
    if (mode & FILE_RDWR)
    {
        file_mode[idx++] = 'r';
        file_mode[idx++] = '+';
        uniq_flag += 1u;
    }
    if (mode & FILE_APPEND)
    {
        file_mode[idx++] = 'a';
    }
    file_mode[idx] = '\0';
    if (uniq_flag > 1)
    {
        return -1; // conflicting flags detected
    }
    FILE *fs = fopen(pathname, file_mode);
    if (fs == NULL)
    {
        return -1;
    }
    memcpy(&(get_fd(fd)->file_struct.semi_file), fs, sizeof(FILE));
    // get file size for future calls of file_getsize
    fseek(fs, 0, SEEK_END);
    get_fd(fd)->file_size = (uint32_t)ftell(fs);
    fseek(fs, 0, SEEK_SET);
    rewind(fs);
    free(fs);
#ifdef FILE_UTILS_SEMI_CACHE
    if (mode & (FILE_RDONLY | FILE_RDWR))
    {
        fill_cache(fd);
    }
    else
    { // prepare write buffer
        get_fd(fd)->file_cache_start = fs_buff + fs_buff_pos;
        fs_buff_pos += (uint32_t)FS_WR_BUFF_SIZE;
        get_fd(fd)->file_cache_pos = 0;
    }
#endif // FILE_UTILS_SEMI_CACHE
    return fd;
#endif // FILE_UTILS_FS_SEL==FILE_UTILS_SEMI
#endif // FILE_ENABLE_SEMI
}

int32_t file_close(int32_t fd)
{
    File_Fs_Rec *fs_rec = get_fd(fd);
    if (fs_rec == NULL)
    {
        return -1; // fd is not used
    }
#ifdef FILE_ENABLE_FATFS
    if (fs_rec->fs_type == FILE_UTILS_FATFS)
    {
        FRESULT res = f_close(&fs_rec->file_struct.fatfs_file);
        if (res == FR_OK)
        {
            return free_fd(fd);
        }
        return (int32_t)res;
    }
#endif // FILE_ENABLE_FATFS
#ifdef FILE_ENABLE_SEMI
    if (fs_rec->fs_type == FILE_UTILS_SEMI)
    {
#ifdef FILE_UTILS_SEMI_CACHE
        if (fs_rec->dirty != 0)
        {
            fwrite(fs_rec->file_cache_start, fs_rec->file_size, 1, &fs_rec->file_struct.semi_file);
        }
#endif
        int ret = fclose(&fs_rec->file_struct.semi_file);
        if (ret != 0)
        {
            return ret;
        }
        return free_fd(fd);
    };
#endif // FILE_ENABLE_SEMI
    return -1;
};

int32_t file_read(int32_t fd, void *buff, size_t btr)
{
    File_Fs_Rec *fs_rec = get_fd(fd);
    if (fs_rec == NULL)
    {
        return -1; // fd is not used
    };
#ifdef FILE_ENABLE_FATFS
    if (fs_rec->fs_type == FILE_UTILS_FATFS)
    {
        UINT br;
        FRESULT res = f_read(&fs_rec->file_struct.fatfs_file, buff, btr, &br);
        if (res != FR_OK)
        {
            return -(int32_t)res;
        }
        return (int32_t)br;
    }
#endif // FILE_ENABLE_FATFS
#ifdef FILE_ENABLE_SEMI
    if (fs_rec->fs_type == FILE_UTILS_SEMI)
    {
        int32_t br;
#ifdef FILE_UTILS_SEMI_CACHE
        btr = MIN((int32_t)btr, fs_rec->file_size - (int32_t)fs_rec->file_cache_pos);
        if (fs_rec->file_cache_pos >= 0)
        {
            memcpy(buff, get_fd(fd)->file_cache_start + fs_rec->file_cache_pos, btr);
            fs_rec->file_cache_pos += (int32_t)btr;
            br = (int32_t)btr;
        }
        else
        {
            br = (int32_t)fread(buff, 1, btr, &fs_rec->file_struct.semi_file);
        }
#else // FILE_UTILS_SEMI_CACHE
        br         = fread(buff, 1, btr, &fs_rec->file_struct.semi_file);
#endif
        return br;
    }
#endif // FILE_ENABLE_SEMI
    return -1;
}

int32_t file_write(int32_t fd, void *buff, size_t cnt)
{
    File_Fs_Rec *fs_rec = get_fd(fd);
    if (fs_rec == NULL)
    {
        return -1; // fd is not used
    };
#ifdef FILE_ENABLE_FATFS
    if (fs_rec->fs_type == FILE_UTILS_FATFS)
    {
        UINT written = 0;
        FRESULT res  = f_write(&fs_rec->file_struct.fatfs_file, buff, cnt, &written);
        if (res != FR_OK)
        {
            return -((int32_t)res);
        }
        return (int32_t)written;
    }
#endif // FILE_ENABLE_FATFS
#ifdef FILE_ENABLE_SEMI
    if (fs_rec->fs_type == FILE_UTILS_SEMI)
    {
#ifdef FILE_UTILS_SEMI_CACHE
        size_t to_copy = MIN(cnt, (uint32_t)(FS_WR_BUFF_SIZE - fs_rec->file_cache_pos));
        memcpy((void *)fs_rec->file_cache_start + fs_rec->file_cache_pos, buff, to_copy);
        fs_rec->file_cache_pos += (int32_t)to_copy;
        get_fd(fd)->file_size = fs_rec->file_cache_pos;
        get_fd(fd)->dirty     = 1;
        return to_copy;
#else
        size_t ret = fwrite(buff, 1, cnt, &fs_rec->file_struct.semi_file);
        if (ret < 0)
        {
            return -1;
        }
        return ret;
#endif
    }
#endif // FILE_ENABLE_SEMI
    return -1;
}

int32_t file_seek(int32_t fd, size_t offset)
{
    File_Fs_Rec *fs_rec = get_fd(fd);
    if (fs_rec == NULL)
    {
        return -1; // fd is not used
    };
#ifdef FILE_ENABLE_FATFS
    if (fs_rec->fs_type == FILE_UTILS_FATFS)
    {
        return (int32_t)f_lseek(&fs_rec->file_struct.fatfs_file, offset);
    }
#endif // FILE_ENABLE_FATFS
#ifdef FILE_ENABLE_SEMI
    if (fs_rec->fs_type == FILE_UTILS_SEMI)
    {
#ifdef FILE_UTILS_SEMI_CACHE
        fs_rec->file_cache_pos = (int32_t)offset;
        return 0;
#else
        return fseek(&fs_rec->file_struct.semi_file, offset, SEEK_SET);
#endif // FILE_UTILS_SEMI_CACHE
    }
#endif // FILE_ENABLE_SEMI
    return -1;
}

int32_t file_getsize(int32_t fd)
{
    File_Fs_Rec *fs_rec = get_fd(fd);
    if (fs_rec == NULL)
    {
        return -1; // fd is not used
    };
#ifdef FILE_ENABLE_FATFS
    if (fs_rec->fs_type == FILE_UTILS_FATFS)
    {
        return (int32_t)f_size(&fs_rec->file_struct.fatfs_file);
    }
#endif // FILE_ENABLE_FATFS
#ifdef FILE_ENABLE_SEMI
    if (fs_rec->fs_type == FILE_UTILS_SEMI)
    {
        return fs_rec->file_size;
    };
#endif // FILE_ENABLE_SEMI
    return -1;
};

#ifdef FILE_ENABLE_FATFS
bool file_exists(const char *filename)
{
    FIL fp;
    FRESULT ret;

    ret = f_open(&fp, _T(filename), FA_READ);
    if (ret == FR_OK)
    {
        if (f_close(&fp) != FR_OK)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}
#endif // FILE_ENABLE_FATFS

#if 0
bool dir_exists(const char *dirname)
{
    OsaDir *dp = osa_opendir(dirname);
    if (dp) {
        osa_closedir(dp);
        return true;
    }

    return false;
}
#endif
