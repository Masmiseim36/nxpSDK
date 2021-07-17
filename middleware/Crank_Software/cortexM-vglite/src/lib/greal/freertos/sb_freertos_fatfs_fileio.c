/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/*
 * Use the uEZ calls to map fileio for sdcard access
 */

#include <gre/sdk/greal.h>


#if GR_FEATURE(FILEIO_FATFS)


#include <string.h>
#include <dirent.h>

#include <FreeRTOS.h>
#include "task.h"

#if !defined(GRE_TARGET_TOOLCHAIN_mcuxpresso)
typedef FIL FILE;
#endif

#if defined(GRE_TARGET_CPU_cortexm7)
// RJD:  On the RT1050, the fgets call does not get mapped to the right place, so we need to override it.
char * 
fgets(char *ptr, int size, FILE * fp)
{
  return f_gets(ptr, size, (FIL *) fp);
}
#endif

#if !defined(GRE_TARGET_TOOLCHAIN_mcuxpresso)
size_t
fread(void * ptr, size_t size, size_t nitems, FILE * fp)
#else
size_t
__wrap_fread(void * ptr, size_t size, size_t nitems, FILE * fp)
#endif
{
	FRESULT fr;
	unsigned nread;

	fr = f_read((FIL *)fp, ptr, size*nitems, &nread);
	if (fr == FR_OK) {
		return nread;
	}

	return 0;
}

size_t
fwrite(const void * ptr, size_t size, size_t nitems, FILE * fp)
{
  uint16_t byteswritten;
  FRESULT res;
  static int count = 0;
  res = f_write((FIL*)fp, ptr, size*nitems, (void *)&byteswritten);
  
  if((byteswritten == 0) || (res != FR_OK)) {
    return -1;
  }
  count++;
  if (count > 25) {
    f_sync((FIL*)fp);
    count = 0;
  }
 return byteswritten;
}

FILE *
fopen(const char * filename, const char * mode) {

    FIL *fp = (FIL*) greal_calloc(sizeof(*fp), 1);
    FRESULT fr;
    char path[256];

#ifdef USE_UEZ
    sprintf(path, "1:%s", filename);
#else
    sprintf(path, "%s", filename);
#endif
    
if (strcmp(mode, "w") == 0) {
	fr = f_open(fp, path, FA_CREATE_ALWAYS | FA_WRITE);
}else {
	fr = f_open(fp, path, FA_READ);
}
       if (fr == FR_OK) {
		return (FILE*)fp;
	}
printf("Fail %s\n", filename);
	return NULL;
}

#if !defined(GRE_TARGET_TOOLCHAIN_mcuxpresso)
int
fclose(FILE *fp)
#else
int
__wrap_fclose(FILE *fp)
#endif
{
	f_close((FIL*)fp);
	greal_free(fp);
	return 0;
}

#ifndef SEEK_SET
#  define SEEK_SET        0       /* Seek from beginning of file.  */
#  define SEEK_CUR        1       /* Seek from current position.  */
#  define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
#endif

int
fseek(FILE *fp, long offset, int whence)
{
	int off = 0;

	if (whence == SEEK_END) {
		off = f_size((FIL*)fp) - offset;
	} else {
		off = offset;
	}
	if (f_lseek((FIL*)fp, off) == FR_OK) {
		return 0;
	}
	return -1;
}

long
ftell(FILE *fp)
{
	return f_tell((FIL*)fp);
}

int
feof(FILE *stream)
{
	return f_eof((FIL*)stream);
}

int
fgetc(FILE *fp)
{
  return 0;
}

#if !defined(GRE_TARGET_TOOLCHAIN_mcuxpresso)
int
ungetc(int c, FILE *fp)
#else
int
__wrap_ungetc(int c, FILE *fp)
#endif
{
return 0;
}

int ferror(FILE *stream)
{
  return f_error((FIL*)stream);
}

/*
 * Dirent
 */

#ifdef __cplusplus
extern "C"
{
#endif

struct myDIR {
  DIR dir;
  struct dirent direntp;
};

DIR *opendir(const char *name)
{
    struct myDIR *dir = 0;
    int res;
#ifdef USE_UEZ
    char path[256];
    
    sprintf(path, "1:%s", name);
    dir = (struct myDIR *) greal_malloc(sizeof(*dir));
    res = f_opendir(&dir->dir, path);
#else
    dir = (struct myDIR *) greal_calloc(1, sizeof(*dir));
    res = f_opendir(&dir->dir, name);
#endif
    if (res == FR_OK) {
       return (DIR *)dir;
    } else {
      printf("f_opendir failed (%s): %d\n", name, res);
    }
    greal_free(dir);

    return NULL;
}

int closedir(DIR *dir)
{
  struct myDIR *d = (struct myDIR *) dir;

  f_closedir(&d->dir);
  greal_free(dir);
  return 0;
}

struct dirent *readdir(DIR *dir)
{
  struct myDIR *d = (struct myDIR *) dir;
  FILINFO fno;
  int res;
  
  memset(&fno, 0, (sizeof(FILINFO))); 
  res = f_readdir(&d->dir, &fno);
  if (res != FR_OK || !fno.fname[0]) {
    return NULL;
  }

  strcpy(d->direntp.d_name, fno.fname);
  d->direntp.d_name[strlen(fno.fname)] = 0;

  return &d->direntp;
}

#ifdef __cplusplus
}
#endif

#endif

