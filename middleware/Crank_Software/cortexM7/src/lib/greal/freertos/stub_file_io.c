/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>

#include <stdlib.h>

#if GR_FEATURE(FILEIO_STUB)

/**
 * Required functionality for the SH2A platform in order to support fopen/fread/fwrite/fclose()
 * routines.
 * For more details see:
 *  http://documentation.renesas.com/eng/products/tool/doc/rej10j2202_r0c40700xsw09rum.pdf
 */

/*
 * From:  Renesas SuperH RISC engineC/C++ Compiler, Assembler,
 *
 * Description Prepares for operations on the file corresponding to the filename of the first
 * parameter. In the open routine, the file type (console, printer, disk file, etc.)
 * must be determined in order to enable writing or reading at a later time. The
 * file type must be referenced using the file number returned by the open
 * routine each time reading or writing is to be performed.
 *
 *  The second parameter, mode, specifies processing to be performed when the
 *  file is opened. The meanings of each of the bits of this parameter are as
 *  follows.
 *
 * Bit Description
 * O_RDONLY (bit 0) When this bit is 1, the file is opened in read-only mode
 * O_WRONLY (bit 1) When this bit is 1, the file is opened in write-only mode
 * O_RDWR (bit 2)   When this bit is 1, the file is opened for both reading and writing
 * O_CREAT (bit 3)  When this bit is 1, if a file with the filename given does not
 *                  exist, it is created
 * O_TRUNC (bit 4)  When this bit is 1, if a file with the filename given exists, the
 *                  file contents are deleted and the file size is set to 0
 * O_APPEND (bit 5) Sets the position within the file for the next read/write operation
 *                  When 0:  Set to read/write from the beginning of file
 *                  When 1:  Set to read/write from file end
 * flg is always assumed to be 0777
 */
int
open(char *name, int mode, int flg) {
	return 1;
}

/**
 * From:  Renesas SuperH RISC engineC/C++ Compiler, Assembler,
 *
 * The file number obtained using the open routine is passed as an
 * parameter. The file management information area set using the open routine
 * should be released to enable reuse. Also, when output file buffering is
 * performed in low-level interface routines, the buffer contents should be
 * written to the actual file.
 *
 * When the file is closed successfully, 0 is returned; if the close
 * operation fails, -1 is returned.
 */
int
close(int fileno) {
	return 0;
}

/*
 * From:  Renesas SuperH RISC engineC/C++ Compiler, Assembler,
 *
 * Data is read from the file specified by the first parameter (fileno) to the
 * area in memory specified by the second parameter (buf). The number of bytes of
 * data to be read is specified by the third parameter (count).
 *
 * When the end of the file is reached, only a number of bytes fewer than or
 * equal to count bytes can be read.
 *
 * The position for file reading/writing advances by the number of bytes read.
 *
 * When reading is performed successfully, the actual number of bytes read is
 * returned; if the read operation fails, -1 is returned.
 */
int
read(int fileno, char *buf, unsigned int count) {
	return 0;
}
#endif

