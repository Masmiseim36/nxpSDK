/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>

#include <stdlib.h>

#if GR_CPU(SH2A)

/**
 * Required functionality for the SH2A platform in order to support malloc() routines
 * with a non-rentrant platform configuration.
 * For more details see:
 *  http://documentation.renesas.com/eng/products/tool/doc/rej10j2202_r0c40700xsw09rum.pdf
 *  http://www.renesas.com/support/faqs/faq_results/Q100901-Q101000/tool_linker_2004060410.jsp
 */
#define GR_HEAP_SIZE		100 * 1024

unsigned char *		g_gr_heap_top = NULL;
unsigned char 		g_gr_heap[GR_HEAP_SIZE];


/**
 * From:  Renesas SuperH RISC engineC/C++ Compiler, Assembler,
 *
 * The size of the memory area to be allocated is passed as a parameter.
 * When calling the sbrk routine several times, memory areas should be
 * allocated in succession starting from lower addresses. If the memory area for
 * allocation is insufficient, an error should occur. When allocation is
 * successful, the address of the beginning of the allocated memory area should
 * be returned; if unsuccessful, (char *) -1 should be returned.
 *
 * @return (char *)-1 on error, otherwise return the the address of the block.
 */
char *sbrk (int size) {
	unsigned char *ret;

	//This assumes that pointers grow upwards
	if(g_gr_heap_top == NULL) {
		g_gr_heap_top = g_gr_heap;
	}

	if((GR_HEAP_SIZE - (g_gr_heap_top - g_gr_heap)) < size) {
		return (char *)-1;
	}

	ret = g_gr_heap_top;

	g_gr_heap_top += size;

	return (char *)ret;
}

#endif


