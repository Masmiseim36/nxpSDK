/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/*
 * Use the uEZ calls to map fileio for sdcard access
 */

#include <gre/sdk/greal.h>

#if GR_FEATURE(FILEIO_UEZ)

#include <gre/sdk/greal.h>


#include <stdlib.h>


/**
 * Required functionality for the SH2A platform in order to support fopen/fread/fwrite/fclose()
 * routines.
 * For more details see:
 *  http://documentation.renesas.com/eng/products/tool/doc/rej10j2202_r0c40700xsw09rum.pdf
 */

#include <uEZFile.h>

typedef struct {
	T_uezFile iFile;
} FILE;

size_t
fread(void * ptr, size_t size, size_t nitems, FILE * fp)
{
	T_uezError error;
	int nbytes = nitems * size;
	int nread = 0;

	error = UEZFileRead(fp->iFile, ptr, nbytes, &nread);
	if (error != UEZ_ERROR_NONE) {
		return -1;
	}

	return nread;
}

size_t
fwrite(void * ptr, size_t size, size_t nitems, FILE * fp)
{
	T_uezError error;
	int nbytes = nitems * size;
	int nread = 0;

	error = UEZFileWrite(fp->iFile, ptr, nbytes, &nread);
	if (error != UEZ_ERROR_NONE) {
		return -1;
	}

	return nread;
}

FILE *
fopen(const char * filename, const char * mode) {
	T_uezError error;
	T_uezFile iFile;
    FILE *fp;
	char *fname = greal_malloc(strlen(filename) + strlen("1:") + 1);

    fp = greal_malloc(sizeof(FILE));
    if (fp == NULL) {
    	return NULL;
    }
	sprintf(fname, "1:%s", filename);
	if (strchr(mode, 'w')) {
		error = UEZFileOpen(fname, FILE_FLAG_WRITE, &fp->iFile);
	} else {
		error = UEZFileOpen(fname, FILE_FLAG_READ_ONLY, &fp->iFile);
	}
	if (error == UEZ_ERROR_NONE) {
//		printf("  Looking ... Found!\n");
	} else if (error == UEZ_ERROR_NOT_AVAILABLE)  {
//		printf("Not plugged in\n");
	} else if (error == UEZ_ERROR_OUT_OF_MEMORY)  {
//		printf("Internal Error\n");
	} else if (error == UEZ_ERROR_NOT_READY) {
//		printf("Not Ready\n");
	} else if (error == UEZ_ERROR_NOT_FOUND) {
//		printf("Not Found\n");
	} else if (error == UEZ_ERROR_NOT_AVAILABLE) {
//		printf("Not available\n");
	} else if (error == UEZ_ERROR_READ_WRITE_ERROR) {
//		printf("Read/Write Error\n");
	} else {
//		printf("No SLIDES.TXT\n");
	}

	greal_free(fname);
	if (error != UEZ_ERROR_NONE) {
		greal_free(fp);
		fp = NULL;
	}

	return fp;
}

int
fclose(FILE *fp)
{
	if (fp == NULL) {
		return -1;
	}
	UEZFileClose(fp->iFile);
    greal_free(fp);
	return 0;
}


int
fseek(FILE *fp, long offset, int whence)
{
//	UEZFileSeekPosition(fp->iFile, offset);
	return -1;
}

long
ftell(FILE *fp)
{
	TUInt32 aPosition;
	UEZFileTellPosition(fp->iFile, &aPosition);
	return aPosition;
}

int
feof(FILE *stream)
{
	return 0;
}

int
fgetc(FILE *fp)
{
  return 0;
}

#endif

